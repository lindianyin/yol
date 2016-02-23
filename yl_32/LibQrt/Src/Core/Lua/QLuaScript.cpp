
#include "CorePrivate.h"

#ifdef SCRIPT_BINDK

#include "QLuaScript.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
};

#include "QFile.h"
//#include "File/PackFile.h"
#include "QLog.h"

// --------------------------------------------------------------

class _KBlockAlloc
{
private:
	struct _KBlockHead
	{
		_KBlockHead*	psNext;
	};

public:
	_KBlockAlloc(UINT uBlockSize)
	{
		m_psFreeBlocks	= NULL;
		m_uBlockSize	= uBlockSize;
		m_nAllocCount	= 0;
	}

	~_KBlockAlloc()
	{
		ReleaseFreeBlocks();
		
		_ASSERT(m_nAllocCount == 0);
	}

	LPVOID Alloc()
	{
		_KBlockHead* psBlock	= m_psFreeBlocks;
		if (psBlock)
		{
			m_psFreeBlocks	= psBlock->psNext;
			psBlock->psNext	= NULL;
		}
		else
		{
			psBlock	= (_KBlockHead*)new BYTE[m_uBlockSize + sizeof(_KBlockHead)];
			psBlock->psNext		= NULL;
			m_nAllocCount ++;
		}

		return psBlock + 1;
	}

	VOID Free(LPVOID pMem)
	{
		*(CHAR*)pMem; // debug
		_KBlockHead* psBlock	= (_KBlockHead*)pMem - 1;
		_ASSERT(psBlock->psNext == NULL);

		psBlock->psNext	= m_psFreeBlocks;
		m_psFreeBlocks	= psBlock;
	}

	UINT GetBlockSize() CONST { return m_uBlockSize; }

	INT GetAllocCount() CONST { return m_nAllocCount; }

	BOOL ReleaseFreeBlocks()
	{
		for (_KBlockHead* psBlock = m_psFreeBlocks; psBlock; )
		{
			_KBlockHead* psNext	= psBlock->psNext;

			delete psBlock;
			m_nAllocCount --;

			psBlock	= psNext;
		}

		m_psFreeBlocks	= FALSE;

		return TRUE;
	}

	INT GetFreeBlocksCount() CONST
	{
		INT nCount	= 0;

		for (_KBlockHead* psBlock = m_psFreeBlocks; psBlock; psBlock = psBlock->psNext)
		{
			nCount	++;
		}

		return nCount;
	}

private:
	_KBlockHead*	m_psFreeBlocks;
	INT m_uBlockSize;
	INT m_nAllocCount;
};

class _KMemAlloc
{
private:
	struct _KMemHead
	{
		_KBlockAlloc* pcBlockAlloc;
	};

public:
	_KMemAlloc()
	{
		ZeroStruct(m_apBlockAlloc);

		for(INT n = 0; n < countof(m_apBlockAlloc); ++ n)
		{
			// 26个分配器，分别对应16字节、32字节、64字节、……、512M
			m_apBlockAlloc[n]	= new _KBlockAlloc((1 << (n + 4)) + sizeof(_KMemHead));
		}
	}
	~_KMemAlloc()
	{
		for(INT n = 0; n < countof(m_apBlockAlloc); ++ n)
		{
			SAFE_DELETE(m_apBlockAlloc[n]);
		}
	}

	LPVOID Alloc(UINT uNewSize)
	{
		LPVOID pRet	= NULL;

		UINT uNeedSize		= uNewSize + sizeof(_KMemHead);

		// 找到合适的内存块分配器
		for(INT n = 0; n < countof(m_apBlockAlloc); ++ n)
		{
			_KBlockAlloc* pcBlockAlloc	= m_apBlockAlloc[n];

			if (pcBlockAlloc->GetBlockSize() >= uNeedSize)
			{
				_KMemHead* psMem	= (_KMemHead*)pcBlockAlloc->Alloc();
				psMem->pcBlockAlloc	= pcBlockAlloc;
				pRet	= psMem + 1;
				break;
			}
		}

		if (!pRet)
		{
			// 内存太大了，肯定有问题
			QLogPrintf(LOG_ERR, "Lua need too large mem! (%d)", uNewSize);
		}

		return pRet;
	}

	VOID Free(LPVOID pMem)
	{
		_KMemHead* psMem	= (_KMemHead*)pMem - 1;
		assert(psMem->pcBlockAlloc);
		psMem->pcBlockAlloc->Free(psMem);
	}

	LPVOID Realloc(LPVOID pMem, UINT nOldSize, UINT uNewSize)
	{
		LPVOID pRet			= NULL;
		_KMemHead* psMem	= (_KMemHead*)pMem - 1;
		assert(pMem);
		_KBlockAlloc* pcBlockAlloc	= psMem->pcBlockAlloc;
		assert(pcBlockAlloc);

		if (pcBlockAlloc->GetBlockSize() < uNewSize + sizeof(_KMemHead))	// 原有内存块空间已经不足
		{
			pRet	= Alloc(uNewSize);
			assert(pRet);
			memcpy(pRet, pMem, nOldSize);	// uNewSize必然比nOldSize大
			pcBlockAlloc->Free(psMem);
			psMem	= NULL;
		}
		else
		{
			pRet	= psMem + 1;
		}

		return pRet;
	}

	BOOL ReleaseFreeMemory()
	{
		for(INT n = 0; n < countof(m_apBlockAlloc); ++ n)
		{
			if (m_apBlockAlloc[n])
				m_apBlockAlloc[n]->ReleaseFreeBlocks();
		}

		return TRUE;
	}

	UINT GetFreeMemorySize()
	{
		UINT nSize	= 0;

		for(INT n = 0; n < countof(m_apBlockAlloc); ++ n)
		{
			nSize	+= m_apBlockAlloc[n]->GetBlockSize() * m_apBlockAlloc[n]->GetFreeBlocksCount();
		}

		return nSize;
	}

	UINT GetAllocMemorySize()
	{
		UINT nSize	= 0;

		for(INT n = 0; n < countof(m_apBlockAlloc); ++ n)
		{
			nSize	+= m_apBlockAlloc[n]->GetBlockSize() * m_apBlockAlloc[n]->GetAllocCount();
		}

		return nSize;
	}

private:
	_KBlockAlloc* m_apBlockAlloc[26];
};

//static _KMemAlloc	s_cMemAlloc;


void lua_pushcfunction_f(lua_State *L, lua_CFunction fn)
{
	lua_pushcclosure(L, fn, 0);
}

LPCSTR QLuaScript::ms_szOutSplit		= "\t";
LPCSTR QLuaScript::ms_szOutEnd		= "\n";
QLuaScript::ScriptOutFun QLuaScript::ms_pOutFun	= printf;
QLuaScript::ScriptOutFun QLuaScript::ms_pErrOutFun	= printf;

QLuaScript* QLuaScript::GetThis(lua_State* L)
{
	LPVOID pThis	= NULL;
	lua_getallocf(L, &pThis);
	return (QLuaScript*)pThis;
}

BOOL QLuaScript::OutPutErrMsgV(lua_State* L, LPCSTR pszFormat, va_list vlist)
{
	LPCSTR pszMsg	= NULL;
	int nTopIndex	= 0;

	CHECK_REPORT(L);
	nTopIndex	= lua_gettop(L);

	pszMsg	= lua_pushvfstring(L, pszFormat, vlist);
	CHECK_REPORT(pszMsg);

	lua_getglobal(L, "debug");
	if (lua_istable(L, -1)) {
		lua_getfield(L, -1, "traceback");
		if (lua_isfunction(L, -1)) {
			lua_pushvalue(L, nTopIndex + 1);  // 第一个参数是错误信息
			lua_pushinteger(L, 2);  // 第二个参数：2	-- skip this function and traceback
			lua_call(L, 2, 1);		// 调用 debug.traceback
			pszMsg	= lua_tostring(L, -1);
		}
	}

	//ms_pErrOutFun("%s%s", pszMsg, ms_szOutEnd);
	QLogPrintf(LOG_ERR,"%s%s",pszMsg,ms_szOutEnd);
	lua_settop(L, nTopIndex);
	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QLuaScript::ReleaseAllFreeMemory()
{
	return TRUE;
	//return s_cMemAlloc.ReleaseFreeMemory();
}

class KLuaStateSaver
{
private:
	lua_State *m_L;
	QLuaScript* m_pKJxScript;
public:
	KLuaStateSaver():m_pKJxScript(NULL){}
	void Init(QLuaScript* pJxScript, lua_State *pNewL)
	{
		m_L = pJxScript->GetLuaState();
		m_pKJxScript = pJxScript;
		((KJxScriptEx*)m_pKJxScript)->m_pLuaState = pNewL;
	}
	~KLuaStateSaver()
	{
		if (m_pKJxScript)
			((KJxScriptEx*)m_pKJxScript)->m_pLuaState = m_L;
	}
	class KJxScriptEx : public QLuaScript
	{
		friend class KLuaStateSaver;
	};
};

int QLuaScript::_LuaFunction(lua_State *L)
{
	int nRet = false;
	QLuaScript* pscThis = NULL;
	SCRIPT_FUNCTION* pScriptFunc = NULL;
	KLuaStateSaver _saver;
	
	CHECK_REPORT(L);

	CHECK_REPORT(lua_checkstack(L, KD_N_BASE_CALL_STACK));

	pscThis	= GetThis(L);
	CHECK_REPORT(pscThis);

	_saver.Init(pscThis, L); //by dzh:保存现场，析构会恢复现场
	//pscThis->m_pLuaState = L;

	pScriptFunc	= (SCRIPT_FUNCTION*)lua_touserdata(L, lua_upvalueindex(1));
	CHECK_REPORT(pScriptFunc);
	
	if (!pscThis->CheckList(1, pScriptFunc->pszParam))
	{
		OutPutErrMsgF(L, "[script] error param when call function %s(%s)",
						pScriptFunc->pszName, pScriptFunc->pszParam);
		
		CHECK_REPORT(false);
	}

	nRet	= pScriptFunc->pfnFunc(*pscThis);

EXIT0:
	return nRet;
}

LPVOID QLuaScript::_Alloc_Fun(LPVOID pScript, LPVOID pMem, size_t nOldSize, size_t nNewSize)
{
	LPVOID pRet	= NULL;
	QLuaScript* pcScript	=	(QLuaScript*)pScript;

	if (nNewSize == 0)
	{
		free(pMem);
	}
	else
	{
		pRet = realloc(pMem, nNewSize);
	}
	
//	if (nNewSize == 0)
//	{
//		if (pMem)
//		{
//			s_cMemAlloc.Free(pMem);
//		}	
//	}
//	else
//	{
//		if (pMem)
//		{
//			pRet	= s_cMemAlloc.Realloc(pMem, nOldSize, nNewSize);
//		}
//		else
//		{
//			pRet	= s_cMemAlloc.Alloc(nNewSize);
//		}
//		_ASSERT(pRet);
//	}
	
	pcScript->m_uMemSize	+= (INT)nNewSize - (INT)nOldSize;
	pcScript->m_uAllocTimes ++;
	if (nNewSize > pcScript->m_uBlockMaxSize)
	{
		pcScript->m_uBlockMaxSize	=	nNewSize;
	}
	return pRet;
}

QLuaScript::QLuaScript(BOOL bInit)
{
	m_pLuaState		= NULL;
	m_bCanDebug		= FALSE;

	m_uMemSize		= 0;
	m_uAllocTimes	= 0;
	m_uBlockMaxSize	= 0;

	// 要保持定义一致
	_ASSERT(LUA_TNONE == KE_DATA_TYPE_NONE);
	_ASSERT(LUA_TNIL == KE_DATA_TYPE_NULL);
	_ASSERT(LUA_TBOOLEAN == KE_DATA_TYPE_BOOL);
	_ASSERT(LUA_TLIGHTUSERDATA == KE_DATA_TYPE_LUSER);
	_ASSERT(LUA_TNUMBER == KE_DATA_TYPE_NUMBER);
	_ASSERT(LUA_TSTRING == KE_DATA_TYPE_STRING);
	_ASSERT(LUA_TTABLE == KE_DATA_TYPE_TABLE);
	_ASSERT(LUA_TFUNCTION == KE_DATA_TYPE_FUNCTION);
	_ASSERT(LUA_TUSERDATA == KE_DATA_TYPE_USER);
	_ASSERT(LUA_TTHREAD == KE_DATA_TYPE_THREAD);

	if (bInit)
	{
		Init();
	}
		
}

QLuaScript::~QLuaScript(void)
{
//#ifdef WIN32
//	if (m_bCanDebug)
//	{
//		KGC_UnInit_VSLuaDebugger();
//	}
//#endif
	Exit();
}

BOOL QLuaScript::Init(BOOL bDebug/* = FALSE*/)
{
	Exit();
	m_pLuaState	= lua_newstate(_Alloc_Fun, this);
	//m_pLuaState	= luaL_newstate();//改成直接分配内存，弄一个内存池没有必要
	CHECK_REPORT(m_pLuaState);

	CHECK_REPORT(lua_checkstack(m_pLuaState, KD_N_BASE_CALL_STACK));

	luaopen_base(m_pLuaState);
	luaL_openlibs(m_pLuaState);

//	if (bDebug)
//	{
//#ifdef WIN32
//		m_bCanDebug = KGC_Init_VSLuaDebugger(m_pLuaState);
//		if (m_bCanDebug)
//			printf("KGC_Init_VSLuaDebugger..ok!\n");
//		else
//			printf("KGC_Init_VSLuaDebugger..fail.........!\n");
//#endif
//	}

	lua_register(m_pLuaState, "print", _LuaPrint);	// 重定义输出
	
	return TRUE;
EXIT0:
	return FALSE;
}

void QLuaScript::Exit()
{
	if (m_pLuaState)
	{
		lua_close(m_pLuaState);
		m_pLuaState	= NULL;
	}
}

BOOL QLuaScript::RegisterGlobalFunction(LPCSTR pszFunName, lua_CFunction pFunc)
{
	CHECK_REPORT(m_pLuaState);

	lua_register(m_pLuaState, pszFunName, pFunc);

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QLuaScript::RegisterTableFunctions(LPCSTR pszTableName, const TScriptFunc arFunc[], DWORD dwCount)
{
	CHECK_REPORT(m_pLuaState);

	lua_getglobal(m_pLuaState, pszTableName);
	if (!lua_istable(m_pLuaState, -1))
	{
		lua_pop(m_pLuaState, 1);
		lua_createtable(m_pLuaState, 0, dwCount);
		lua_pushstring(m_pLuaState, pszTableName);
		lua_pushvalue(m_pLuaState, -2);
		lua_rawset(m_pLuaState, LUA_GLOBALSINDEX);
	}

	for (DWORD i = 0; i < dwCount; i++)
	{
		lua_pushstring(m_pLuaState, arFunc[i].pszName);
		lua_pushcfunction(m_pLuaState, arFunc[i].pfnFunc);
		lua_rawset(m_pLuaState, -3);
	}

	lua_pop(m_pLuaState, 1);

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QLuaScript::RegisterTableFunctions(LPCSTR pszTableName, const SCRIPT_FUNCTION* pFunc, DWORD dwCount)
{
	CHECK_REPORT(m_pLuaState);

	lua_getglobal(m_pLuaState, pszTableName);
	if (!lua_istable(m_pLuaState, -1))
	{
		lua_pop(m_pLuaState, 1);
		lua_createtable(m_pLuaState, 0, dwCount);
		lua_pushstring(m_pLuaState, pszTableName);
		lua_pushvalue(m_pLuaState, -2);
		lua_rawset(m_pLuaState, LUA_GLOBALSINDEX);
	}

	for (DWORD i = 0; i < dwCount; i++)
	{
		lua_pushstring(m_pLuaState, pFunc[i].pszName);
		lua_pushlightuserdata(m_pLuaState, (void *)&pFunc[i]);
		lua_pushcclosure(m_pLuaState, _LuaFunction, 1);
		lua_rawset(m_pLuaState, -3);
	}

	lua_pop(m_pLuaState, 1);

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QLuaScript::LoadBuffer(LPBYTE pbyBuffer, SIZE_T uLen, LPCSTR pszFileName /* = NULL */)
{
	int nRet;

	CHECK_REPORT(m_pLuaState);

	nRet	= luaL_loadbuffer(m_pLuaState, (LPCSTR)pbyBuffer, uLen, pszFileName);

	return _CheckRet(nRet);

EXIT0:
	return FALSE;
}

BOOL QLuaScript::_LoadFile(LPCSTR pszFileName, BOOL bPackOnly)
{
	CHAR szFullPathName[MAX_PATH];
	INT nRet		= 0;
	IFile* pcFile	= NULL;

	CHECK_REPORT(m_pLuaState);

	CHECK_REPORT(pszFileName);

	g_GetFullPath(szFullPathName + 1, pszFileName);
	szFullPathName[0] = '@';

	//if (bPackOnly)
	//{
	//	KPackFile cFile;
	//	CHECK_REPORT(cFile.Open(pszFileName));

	//	nRet	= luaL_loadbuffer(m_pLuaState, (LPCSTR)cFile.GetBuffer(), cFile.Size(), szFullPathName);
	//	cFile.Close();
	//}
	//else
	{
		QFile cFile;
		CHECK_REPORT(cFile.Open(pszFileName));

		nRet	= luaL_loadbuffer(m_pLuaState, (LPCSTR)cFile.GetBuffer(), cFile.Size(), szFullPathName);
		cFile.Close();
	}

	return _CheckRet(nRet);

EXIT0:
	OutPutErrMsgF("[script] file '%s' read failed!", pszFileName);
	return FALSE;
}

BOOL QLuaScript::SafeCallBegin(int &nBeginIndex)
{
	CHECK_REPORT(m_pLuaState);

	nBeginIndex	= lua_gettop(m_pLuaState);

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QLuaScript::SafeCallEnd(int nBeginIndex)
{
	CHECK_REPORT(m_pLuaState);

	lua_settop(m_pLuaState, nBeginIndex);

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QLuaScript::CallGlobalFunctionV(LPCSTR pszFunName, int nResults, LPCSTR pszFormat, va_list vlist)
{
	CHECK_REPORT(m_pLuaState);

	lua_getglobal(m_pLuaState, pszFunName);
	if (!lua_isfunction(m_pLuaState, -1))
	{
		OutPutErrMsgF("[script]GlobalFunction \"%s\" not found!", pszFunName);
		lua_pop(m_pLuaState, 1);
		return FALSE;
	}

	return _Execute(nResults, FALSE, pszFormat, vlist);

EXIT0:
	return FALSE;
}

BOOL QLuaScript::CallTableFunctionV(LPCSTR pszTableName, LPCSTR pszFunName, int nResults, LPCSTR pszFormat, va_list vlist)
{
	CHECK_REPORT(m_pLuaState);

	lua_getglobal(m_pLuaState, pszTableName);
	if (!lua_istable(m_pLuaState, -1))
	{
		OutPutErrMsgF("[script]Table \"%s\" not found!", pszTableName);
		lua_pop(m_pLuaState, 1);
		return FALSE;
	}

	lua_getfield(m_pLuaState, -1, pszFunName);
	if (!lua_isfunction(m_pLuaState, -1))
	{
		OutPutErrMsgF("[script]TableFunction \"%s.%s\" not found!", pszTableName, pszFunName);
		lua_pop(m_pLuaState, 2);
		return FALSE;
	}

	lua_insert(m_pLuaState, -2);	// 函数与Table位置互换

	return _Execute(nResults, TRUE, pszFormat, vlist);

EXIT0:
	return FALSE;
}

BOOL QLuaScript::DoCall(int nResults, int nParams)
{
	int nFunIdx;

	CHECK_REPORT(m_pLuaState);

	nFunIdx	= -(nParams + 1);

	if (!lua_isfunction(m_pLuaState, nFunIdx))
	{
		OutPutErrMsgF("[script]function (%d) not found!", nFunIdx);
		lua_pop(m_pLuaState, 1);
		return FALSE;
	}

	return _DoCall(nResults, nParams);

EXIT0:
	return FALSE;
}

BOOL QLuaScript::Execute(int nResults, LPCSTR pszFormat, ...)
{
	CHECK_REPORT(m_pLuaState);

	CHECK_REPORT(lua_isfunction(m_pLuaState, -1));

	{
		va_list vlist;
		va_start(vlist, pszFormat);
		BOOL bOK	= _Execute(nResults, FALSE, pszFormat, vlist);
		va_end(vlist);
		return bOK;
	}

EXIT0:
	return FALSE;
}

BOOL QLuaScript::DoBuffer(LPCSTR pszBuffer, LPCSTR pszFileName)
{
	CHECK_REPORT(m_pLuaState);

	{
		int nRet		= luaL_loadbuffer(m_pLuaState, pszBuffer, strlen(pszBuffer), pszFileName);

		int nTopIndex	= lua_gettop(m_pLuaState);
		BOOL bOK		= _CheckRet(nRet) && _DoCall(0, 0);
		lua_settop(m_pLuaState, nTopIndex);

		return bOK;
	}

EXIT0:
	return FALSE;
}

BOOL QLuaScript::DoFile(LPCSTR pszFileName, BOOL bPackOnly /* = FALSE */)
{
	CHECK_REPORT(m_pLuaState);

	{
		int nTopIndex	= lua_gettop(m_pLuaState);
		BOOL bOK	= _LoadFile(pszFileName, bPackOnly) && _DoCall(0, 0);
		lua_settop(m_pLuaState, nTopIndex);

		return bOK;
	}

EXIT0:
	return FALSE;
};

int QLuaScript::GetTopIndex()
{
	if (!m_pLuaState)
	{
		_ASSERT(FALSE);
		return -1;
	}

	return lua_gettop(m_pLuaState);
}

BOOL QLuaScript::SetTopIndex(int nIndex)
{
	if (!m_pLuaState)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	lua_settop(m_pLuaState, nIndex);

	return TRUE;
}

QLuaScript::KE_DATA_TYPE QLuaScript::GetType(int nIndex)
{
	return (QLuaScript::KE_DATA_TYPE)lua_type(m_pLuaState, nIndex);
}

int QLuaScript::GetInt(int nIndex)
{
	return (int)(unsigned int)lua_tonumber(m_pLuaState, nIndex);
}

BOOL QLuaScript::GetBool(int nIndex)
{
	return lua_toboolean(m_pLuaState, nIndex);
}

double QLuaScript::GetNum(int nIndex)
{
	return lua_tonumber(m_pLuaState, nIndex);
}

LPCSTR QLuaScript::GetStr(int nIndex)
{
	return lua_tostring(m_pLuaState, nIndex);
}

LPCSTR QLuaScript::GetLStr(int nIndex, int& nLen)
{
	size_t outLen;
	const char* ret = lua_tolstring(m_pLuaState, nIndex, &outLen);
	nLen = outLen;
	return ret;
}

void* QLuaScript::GetUser(int nIndex)
{
	return lua_touserdata(m_pLuaState, nIndex);
}

BOOL QLuaScript::GetObjLen(int nIndex)
{
	return lua_objlen(m_pLuaState, nIndex);
}

BOOL QLuaScript::GetTableIndex(int nIndex, int i)
{
	lua_rawgeti(m_pLuaState, nIndex, i);
	return TRUE;
}

BOOL QLuaScript::GetTableField(int nIndex, LPCSTR pszName)
{
	lua_getfield(m_pLuaState, nIndex, pszName);
	return TRUE;
}

BOOL QLuaScript::GetGlobal(LPCSTR pszName)
{
	CHECK_REPORT(m_pLuaState);

	lua_getglobal(m_pLuaState, pszName);
	return TRUE;

EXIT0:
	return FALSE;
}

int  QLuaScript::GetGlobalF(LPCSTR pszString)
{
	if (!pszString)
		return 0;

	char szNameBuf[100];		// 保存识别到的名字
	int nNameLen	= 0;		// 识别到的名字长度
	BOOL bMethod	= FALSE;	// 是否是方法（含有“:”）
	BOOL bFisrtName	= TRUE;		// 正要取得第一个名字
	int nTopIndex	= lua_gettop(m_pLuaState);
	for (LPCSTR psz = pszString; ; ++psz)
	{
		switch(*psz)
		{
		case '.':
			CHECK_REPORT(!bMethod);	// “:”后面不应该再有“.”
			break;
		case ':':
			CHECK_REPORT(!bMethod);	// 不应该出现多个“:”
			bMethod	= TRUE;
			break;
		case '\0':
			break;
		case ' ':
			continue;	// 忽略空格
		default:
			szNameBuf[nNameLen++]	= *psz;
			CHECK_REPORT(nNameLen < sizeof(szNameBuf));	// 名字太长了
			continue;
		}
		szNameBuf[nNameLen]	= '\0';

		if (bFisrtName)
		{
			lua_getglobal(m_pLuaState, szNameBuf);
			bFisrtName	= FALSE;
		}
		else
			lua_getfield(m_pLuaState, -1, szNameBuf);

		if (lua_isnil(m_pLuaState, -1))
		{
			OutPutErrMsgF("[script]key \"%s\" not found when get \"%s\"!", szNameBuf, pszString);
			goto EXIT0;
		}

		if (!*psz)
			break;

		nNameLen	= 0;
	}

	if (bMethod)
	{
		lua_insert(m_pLuaState, nTopIndex + 1);
		lua_insert(m_pLuaState, nTopIndex + 2);
		lua_settop(m_pLuaState, nTopIndex + 2);
		return 2;
	}
	else
	{
		lua_insert(m_pLuaState, nTopIndex + 1);
		lua_settop(m_pLuaState, nTopIndex + 1);
		return 1;
	}

EXIT0:
	lua_settop(m_pLuaState, nTopIndex);
	return 0;
}

BOOL QLuaScript::PushNumber(double fValue)
{
	lua_pushnumber(m_pLuaState, fValue);
	return TRUE;
}

BOOL QLuaScript::PushBool(BOOL bValue)
{
	lua_pushboolean(m_pLuaState, bValue);
	return TRUE;
}

BOOL QLuaScript::PushString(LPCSTR pszValue)
{
	lua_pushstring(m_pLuaState, pszValue);
	return TRUE;
}

BOOL QLuaScript::PushLString(LPCSTR pszValue, int nLen)
{
	lua_pushlstring(m_pLuaState, pszValue, nLen);
	return TRUE;
}

BOOL QLuaScript::PushStringF(LPCSTR pszFormat, ...)
{
	va_list vlist;
	va_start(vlist, pszFormat);
	lua_pushvfstring(m_pLuaState, pszFormat, vlist);
	va_end(vlist);

	return TRUE;
}

BOOL QLuaScript::PushObj(const QLunaBase* pLuna)
{
	CHECK_REPORT(pLuna);

	pLuna->PushCObj(m_pLuaState);
	return TRUE;

EXIT0:
	return FALSE;
}

QLunaBase* QLuaScript::GetObj(int nIndex)
{
	return QLunaBase::GetCObj(m_pLuaState, nIndex);
}

BOOL QLuaScript::PushPointer(const void* p)
{
	lua_pushlightuserdata(m_pLuaState, (void*)p);
	return TRUE;
}

BOOL QLuaScript::PushNull()
{
	lua_pushnil(m_pLuaState);
	return TRUE;
}

BOOL QLuaScript::PushTable()
{
	lua_newtable(m_pLuaState);
	return TRUE;
}

BOOL QLuaScript::SetTableIndex(int nIndex)
{
	lua_rawseti(m_pLuaState, -2, nIndex);
	return TRUE;
}

BOOL QLuaScript::SetTableField(LPCSTR pszName)
{
	lua_setfield(m_pLuaState, -2, pszName);
	return TRUE;
}

BOOL QLuaScript::SetGlobalName(LPCSTR pszName)
{
	lua_pushstring(m_pLuaState, pszName);
	lua_pushvalue(m_pLuaState, -2);
	lua_rawset(m_pLuaState, LUA_GLOBALSINDEX);
	lua_pop(m_pLuaState, 1);
	return TRUE;
}

BOOL QLuaScript::SetMetaTable(int nIndex)
{
	lua_setmetatable(m_pLuaState, nIndex);
	return TRUE;
}

BOOL QLuaScript::PushFromStack(int nIndex)
{
	lua_pushvalue(m_pLuaState, nIndex);
	return TRUE;
}

BOOL QLuaScript::RemoveFromStack(int nIndex)
{
	lua_remove(m_pLuaState, nIndex);
	return TRUE;
}

BOOL QLuaScript::MoveStack(int nIndex)
{
	lua_insert(m_pLuaState, nIndex);
	return TRUE;
}

BOOL QLuaScript::CopyStack(QLuaScript& sc, int nFromIdx, int nCount)
{
	CHECK_REPORT(sc.m_pLuaState && m_pLuaState);
	CHECK_REPORT(sc.m_pLuaState != m_pLuaState);
	CHECK_REPORT(nCount >= 0)

	{
		int t	= lua_gettop(sc.m_pLuaState);
		int n	= nFromIdx;
		if (n < 0)
			n	= t + n + 1;
		CHECK_REPORT(n <= t);
		CHECK_REPORT(n + nCount - 1 <= t);
		t	= n + nCount - 1;
		for (; n <= t; n++)
		{
			_CopyStack(sc.m_pLuaState, n);
		}
	}

	return TRUE;

EXIT0:
	return FALSE;
}

int QLuaScript::SaveValue2Buffer(LPBYTE pbyBuf, int nBufSize, int nIndex)
{
	CHECK_REPORT(m_pLuaState);
	CHECK_REPORT(pbyBuf);
	CHECK_REPORT(nBufSize > 0);

	lua_pushvalue(m_pLuaState, nIndex);

	return _SaveValue2Buffer(pbyBuf, nBufSize);

EXIT0:
	return 0;
}

BOOL QLuaScript::LoadBuffer2Value(const BYTE* pbyBuf, int nBufSize)
{
	CHECK_REPORT(m_pLuaState);
	CHECK_REPORT(pbyBuf);
	CHECK_REPORT(nBufSize > 0);

	CHECK_REPORT(_LoadBuffer2Value(pbyBuf, nBufSize));

	return TRUE;

EXIT0:
	return FALSE;
}

UINT QLuaScript::GetAllocMemSize() CONST
{
	return 0;
	//return s_cMemAlloc.GetAllocMemorySize();
}

UINT QLuaScript::GetFreeMemSize() CONST
{
	return 0;
	//return s_cMemAlloc.GetFreeMemorySize();
}

BOOL QLuaScript::_DoCall(int nResults, int nParams)
{
	int nFunIdx	= lua_gettop(m_pLuaState) - nParams;
	lua_pushcfunction(m_pLuaState, _LuaError);
	lua_insert(m_pLuaState, nFunIdx);	// 插入_LuaError
	int nRet = lua_pcall(m_pLuaState, nParams, nResults, nFunIdx);
	lua_remove(m_pLuaState, nFunIdx);	// 移出_LuaError

	return _CheckRet(nRet);
}

BOOL QLuaScript::_Execute(int nResults, BOOL bCallMethod, LPCSTR pszFormat, va_list vlist)
{
	int nParams	= _PushList(pszFormat, vlist);
	if (nParams < 0)
		return FALSE;

	return _DoCall(nResults, bCallMethod ? nParams + 1 : nParams);
}

int QLuaScript::_PushList(LPCSTR pszFormat, va_list vlist)
{
	int nCount	= 0;
	while(*pszFormat)
	{
		if(!_PushOne(pszFormat, vlist))
		{
			_ASSERT(FALSE);
			return -1;
		}
		nCount ++;
	}

	return nCount;
}

// 压入一个Vector
template <class TPushType, class TDataType>
BOOL QLuaScript::_PushVector(void (*pPushFun)(lua_State *, TPushType), const std::vector<TDataType> *pvArry)
{
	CHECK_REPORT(pvArry);
	lua_createtable(m_pLuaState, 0, pvArry->size());
	for(int i = 0; (DWORD)i < pvArry->size(); i++)
	{
		pPushFun(m_pLuaState, (*pvArry)[i]);
		lua_rawseti(m_pLuaState, -2, i + 1);
	}
	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QLuaScript::_PushOne(LPCSTR& pszFormat, _VA_LIST_REF vlist)
{
	switch(*(pszFormat++))
	{
	case 'n':	// double
		lua_pushnumber(m_pLuaState, va_arg(vlist, double));
		break;
	case 'd':	// int
		lua_pushnumber(m_pLuaState, (double)va_arg(vlist, int));
		break;
	case 'u':	// DWORD
		lua_pushnumber(m_pLuaState, (double)va_arg(vlist, DWORD));
		break;
	case 's':	// string
		lua_pushstring(m_pLuaState, va_arg(vlist, LPCSTR));
		break;
	case 'N':	// (nil)
		lua_pushnil(m_pLuaState);
		break;
	case 'f':	// function
		lua_pushcfunction(m_pLuaState, va_arg(vlist, lua_CFunction));
		break;
	case 'o':	// obj
		{
			QLunaBase* pLuna	= va_arg(vlist, QLunaBase*);
			if (pLuna)
				pLuna->PushCObj(m_pLuaState);
			else
				lua_pushnil(m_pLuaState);
		}
		break;
	case 'v':	// stack value
		lua_pushvalue(m_pLuaState, va_arg(vlist, int));
		break;
	case '{':	// static table
		{
			lua_newtable(m_pLuaState);
			for(int nTbIdx = 1; *pszFormat != '}'; nTbIdx ++)
			{
				if (!_PushOne(pszFormat, vlist))
					return FALSE;
				lua_rawseti(m_pLuaState, -2, nTbIdx);
			}
			pszFormat ++;
		}
		break;
	case '*':	// dynamic table
		{
//			lua_newtable(m_pLuaState);				//liuchang del 10.1.8 在 _PushVector里面已经加了一个表，删除掉
			switch(*(pszFormat++))
			{
			case 'n':
				_PushVector(lua_pushnumber, va_arg(vlist, const std::vector<double>*));
				break;
			case 'd':
				_PushVector(lua_pushnumber, va_arg(vlist, const std::vector<int>*));
				break;
			case 'u':
				_PushVector(lua_pushnumber, va_arg(vlist, const std::vector<DWORD>*));
				break;
			case 's':
				_PushVector(lua_pushstring, va_arg(vlist, const std::vector<LPCSTR>*));
				break;
			case 'f':
				_PushVector(lua_pushcfunction_f, va_arg(vlist, const std::vector<lua_CFunction>*));
				break;
			case 'v':
				_PushVector(lua_pushvalue, va_arg(vlist, const std::vector<int>*));
				break;
			default:
				_ASSERT(FALSE);
				return FALSE;
			}
		}
		break;
	default:
		_ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

int QLuaScript::_ReadList(int nFromIdx, LPCSTR pszFormat, va_list vlist /* = NULL */)
{
	int nTopIdx	= lua_gettop(m_pLuaState);
	BOOL bOptional	= FALSE;
	if (nFromIdx < 0)
		nFromIdx	+= nTopIdx + 1;
	if (nFromIdx <= 0)
		return -1;	// 起始位置不对

	for (int nIndex = nFromIdx; ; )
	{
		switch(*pszFormat)
		{
		case '\0':
			_ASSERT(!bOptional);	// 参数需求字符串错误，括号“[]”不匹配
			return nIndex - nFromIdx;
		case ']':
			_ASSERT(bOptional);		// 参数需求字符串错误，括号“[]”不匹配
			_ASSERT(!pszFormat[1]);	// 参数需求字符串错误，括号“]”没有在末尾
			return nIndex - nFromIdx;
		case '[':
			_ASSERT(!bOptional);	// 参数需求字符串错误，不应该有两个'['
			bOptional	= TRUE;
			pszFormat++;
			continue;
		}

		if (nIndex > nTopIdx)
		{
			if (!bOptional)	// 如果此部分不是可选，则参数不够
				return -1;
			return nTopIdx - nFromIdx + 1;	// 已读取的参数个数是全部
		}

		if(!_ReadOne(nIndex, pszFormat, vlist))
			return -1;

		nIndex++;
	}
}

BOOL QLuaScript::_ReadOne(int nIndex, LPCSTR& pszFormat, _VA_LIST_REF vlist)
{
	switch(*(pszFormat++))
	{
	case 'n':
		if (lua_type(m_pLuaState, nIndex) != LUA_TNUMBER)
			return FALSE;
		if (vlist)
			*va_arg(vlist, double*)	= lua_tonumber(m_pLuaState, nIndex);
		break;
	case 'd':
		if (lua_type(m_pLuaState, nIndex) != LUA_TNUMBER)
			return FALSE;
		if (vlist)
			*va_arg(vlist, int*)	= (int)lua_tonumber(m_pLuaState, nIndex);
		break;
	case 'u':
		if (lua_type(m_pLuaState, nIndex) != LUA_TNUMBER)
			return FALSE;
		if (vlist)
			*va_arg(vlist, DWORD*)	= (DWORD)lua_tonumber(m_pLuaState, nIndex);
		break;
	case 's':
		if (lua_type(m_pLuaState, nIndex) != LUA_TSTRING)
			return FALSE;
		if (vlist)
			*va_arg(vlist, LPCSTR*)	= lua_tostring(m_pLuaState, nIndex);
		break;
	case 'N':
		if (lua_type(m_pLuaState, nIndex) != LUA_TNIL)
			return FALSE;
		break;
	case 'o':
		if (lua_type(m_pLuaState, nIndex) != LUA_TUSERDATA)
			return FALSE;
		if (vlist)
			*va_arg(vlist, QLunaBase**)	= QLunaBase::GetCObj(m_pLuaState, nIndex);
		break;
	case 'f':
		if (lua_type(m_pLuaState, nIndex) != LUA_TFUNCTION)
			return FALSE;
		if (vlist)
			*va_arg(vlist, lua_CFunction*)	= lua_tocfunction(m_pLuaState, nIndex);
		break;
	case '?':	// any
		break;
	case '{':	// table
		{
			if (lua_type(m_pLuaState, nIndex) != LUA_TTABLE)
				return FALSE;
			for (int n = 1; *pszFormat != '}'; n++)
			{
				if (!*pszFormat)
				{
					_ASSERT(FALSE);	// 参数需求字符串错误，括号“{}”不匹配
					return FALSE;
				}

				// 检查元素table[n]
				lua_rawgeti(m_pLuaState, nIndex, n);
				BOOL bOK	= _ReadOne(-1, pszFormat, vlist);
				lua_pop(m_pLuaState, 1);
				if (!bOK)
					return FALSE;
			}
			pszFormat++;
		}
		break;
	default:
		_ASSERT(FALSE);	// 参数需求字符串错误，不可识别的标识
		return FALSE;
	}
	return TRUE;
}

BOOL QLuaScript:: _CopyStack(lua_State *L, int n)
{
	switch(lua_type(m_pLuaState, n))
	{
	//case LUA_TNONE:
	case LUA_TNIL:
		lua_pushnil(m_pLuaState);
		break;
	case LUA_TBOOLEAN:
		lua_pushboolean(m_pLuaState, lua_toboolean(L, n));
		break;
	case LUA_TLIGHTUSERDATA:
		lua_pushlightuserdata(m_pLuaState, lua_touserdata(L, n));
		break;
	case LUA_TNUMBER:
		lua_pushnumber(m_pLuaState, lua_tonumber(L, n));
		break;
	case LUA_TSTRING:
		lua_pushstring(m_pLuaState, lua_tostring(L, n));
		break;
	case LUA_TTABLE:
		lua_pushstring(m_pLuaState, lua_tostring(L, n));
		break;
	case LUA_TFUNCTION:
		if (lua_iscfunction(L, n))
		{
			lua_pushcfunction(m_pLuaState, lua_tocfunction(L, n));
		}
		else
		{
			_ASSERT(FALSE);	// 不能传递Lua自定义函数
			lua_pushnil(m_pLuaState);
		}
		break;
	case LUA_TUSERDATA:
		_ASSERT(FALSE);	// 暂时不能传递类对象
		lua_pushnil(m_pLuaState);
		break;
	//case LUA_TTHREAD:
	default:
		_ASSERT(FALSE);	// ？？？
		lua_pushnil(m_pLuaState);
		break;
	}

	return TRUE;
}

BOOL QLuaScript::_CheckRet(int nRet)
{
	if (nRet == 0)
		return TRUE;

	// 显示出错信息
	LPCSTR pszMsg	= lua_tostring(m_pLuaState, -1);
	if (!pszMsg)
		pszMsg	= "(error object is not a string)";
	//ms_pErrOutFun("%s%s", pszMsg, ms_szOutEnd);
	QLogPrintf(LOG_ERR,"%s%s", pszMsg, ms_szOutEnd);

	lua_pop(m_pLuaState, 1);

	// 运营环境下比较费时，脚本报错时会卡住几秒，先屏蔽 [12/23/2011 simon]
	lua_gc(m_pLuaState, LUA_GCCOLLECT, 0);	// 回收一次垃圾

	return FALSE;
}

int QLuaScript::_LuaError(lua_State *L)
{	// 入口参数（szMsg:出错信息）
	lua_getglobal(L, "debug");
	if (!lua_istable(L, -1)) {
		lua_settop(L, 1);
		return 1;	// 直接返回入口参数
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) {
		lua_settop(L, 1);
		return 1;	// 直接返回入口参数
	}
	lua_pushvalue(L, 1);	// 调用 debug.traceback 的第一个参数就是入口参数
	lua_pushinteger(L, 2);  // 调用 debug.traceback 的第二个参数	-- skip this function and traceback
	lua_call(L, 2, 1);		// 调用 debug.traceback
	return 1;	// 返回 debug.traceback 的返回值
}

int QLuaScript::_LuaPrint(lua_State *L)
{
	int n		= lua_gettop(L);
	LPCSTR s	= NULL;
	std::string strMsg;
	for (int i = 1; i <= n; i++)
	{
		if (luaL_callmeta(L, i, "__tostring"))	// 检查是否有metafield
		{
			s	= lua_tostring(L, -1);
		}
		else
		{
			switch (lua_type(L, i))
			{
			case LUA_TNUMBER:
			case LUA_TSTRING:
				s	= lua_tostring(L, i);
				break;
			case LUA_TBOOLEAN:
				s	= lua_toboolean(L, i) ? "true" : "false";
				break;
			case LUA_TNIL:
				s	= "nil";
				break;
			default:
				s	= NULL;
				break;
			}
		}

		if (i > 1)
			strMsg	+= ms_szOutSplit;
		
		if (s)
		{
			strMsg	+= s;
		}
		else
		{
			char szVarDesc[30];
			sprintf(szVarDesc, "%s: %p", luaL_typename(L, i), lua_topointer(L, i));
			strMsg	+= szVarDesc;
		}
	}

	strMsg	+= ms_szOutEnd;

	//ms_pOutFun("%s", strMsg.c_str());
	QLogPrintf(LOG_INFO,"%s",strMsg.c_str());
	return 0;
}

int QLuaScript::_SaveValue2Buffer(LPBYTE pbyBuf, int nBufSize)
{
	int nRet	= 0;
	switch(lua_type(m_pLuaState, -1))
	{
	case LUA_TNIL:
		if (nBufSize < 1)
		{
			nRet		= -1;
		}
		else
		{
			pbyBuf[0]	= 'N';							// 'N' -> nil
			nRet		= 1;
		}
		break;;
	case LUA_TBOOLEAN:
		if (nBufSize < 1)
		{
			nRet		= -1;
		}
		else
		{
			INT nBool	= lua_toboolean(m_pLuaState, -1);
			if (nBool)
				pbyBuf[0]	= 'T';						// 'T' -> true
			else
				pbyBuf[0]	= 'F';						// 'F' -> false
			nRet		= 1;
		}
		break;
	case LUA_TNUMBER:
		if (nBufSize < 9)
		{
			nRet		= -1;
		}
		else
		{
			DOUBLE dNum	= lua_tonumber(m_pLuaState, -1);
			INT nNum	= (INT)dNum;
			BOOL bIsInt	= ((DOUBLE)nNum == dNum);
			if (bIsInt)
			{
				if (nNum >= 0 && nNum <= 9)
				{
					pbyBuf[0]	= (BYTE)('0' + nNum);	// '0'-'9' -> 0-9
					nRet		= 1;
				}
				else if (nNum >= 0 && nNum <= 0xff)
				{
					pbyBuf[0]	= 'b';					// 'b' -> BYTE
					pbyBuf[1]	= (BYTE)nNum;
					nRet		= 2;
				}
				else if (nNum >= 0 && nNum <= 0xffff)
				{
					pbyBuf[0]			= 'w';			// 'w' -> WORD
					*(LPWORD)(pbyBuf+1)	= (WORD)nNum;
					nRet				= 3;
				}
				else
				{
					pbyBuf[0]			= 'd';			// 'd' -> INT
					*(LPINT)(pbyBuf+1)	= (INT)nNum;
					nRet				= 5;
				}
			}
			else
			{
				pbyBuf[0]				= 'n';			// 'n' -> DOUBLE
				*(DOUBLE*)(pbyBuf+1)	= dNum;
				nRet					= 9;
			}
		}
		break;
	case LUA_TSTRING:
		{
			size_t nStrLen	= 0;
			LPCSTR pszStr	= lua_tolstring(m_pLuaState, -1, &nStrLen);
			if (nStrLen > 0xffff)
			{
				_ASSERT(FALSE);	// 字符串太长了
				nRet	= -1;
			}
			if (nBufSize < 3 + (int)nStrLen)
			{
				nRet	= -1;
			}
			else
			{
				pbyBuf[0]			= 's';				// 's' -> STRING (strlen <= 0xffff)
				*(LPWORD)(pbyBuf+1)	= (WORD)nStrLen;
				memcpy(pbyBuf + 3, pszStr, nStrLen);
				nRet				= 3 + nStrLen;
			}
		}
		break;
	case LUA_TTABLE:
		nRet	= _SaveTable2Buffer(pbyBuf, nBufSize);
		break;
	default:
		nRet	= 0;
		break;
	}

	lua_pop(m_pLuaState, 1);
	return nRet;
}

int QLuaScript::_SaveTable2Buffer(LPBYTE pbyBuf, int nBufSize)
{
	int nResult	= 0;

	int nLen	= 0;
	int nOrdinalCount	= 0;

	BOOL bHaveSplit	= FALSE;

	INT nTopIndex	= lua_gettop(m_pLuaState);

	LOG_PROCESS_ERROR_RET(nBufSize >= 2, -1);

	pbyBuf[nLen++]	= '{';								// '{' -> LuaTable Start

	// -=先试图按顺序方式遍历table=-
	for (;; ++nOrdinalCount)
	{
		lua_rawgeti(m_pLuaState, -1, nOrdinalCount + 1);
		if (lua_isnil(m_pLuaState, -1))	// 找不到顺序成员则终止
		{
			//lua_pop(m_pLuaState, 1);	// value (nil)		(=>*刚好后面要用)
			break;
		}

		INT nRet	= _SaveValue2Buffer(pbyBuf + nLen, nBufSize - nLen);	// 存入value并出栈
		LOG_PROCESS_ERROR_RET(nRet > 0, nRet);

		nLen	+= nRet;
	}

	// -=然后穷举找到其它成员=-
	//lua_pushnil(m_pLuaState);	// 寻找第一对，用nil作key	(<=*刚好前面有了)
	while (lua_next(m_pLuaState, -2))	// -1变成key了，这里用-2
	{
		// 检查此成员是否已顺序遍历
		if (lua_type(m_pLuaState, -2) == LUA_TNUMBER)	// key是数值型
		{
			DOUBLE dKey	= lua_tonumber(m_pLuaState, -2);
			INT nKey	= (INT)dKey;
			if (dKey == (DOUBLE)nKey && nKey > 0 && nKey <= nOrdinalCount)	// 此成员已顺序遍历
			{
				lua_pop(m_pLuaState, 1);	// value
				continue;
			}
		}

		if (!bHaveSplit)
		{
			LOG_PROCESS_ERROR_RET(nBufSize - nLen >= 2, -1);
			pbyBuf[nLen++]	= ';';						// '}' -> LuaTable Split
			bHaveSplit		= TRUE;
		}

		// 存入key
		lua_pushvalue(m_pLuaState, -2);	// 将key放至栈顶
		INT nRet	= _SaveValue2Buffer(pbyBuf + nLen, nBufSize - nLen);	// 存入key并出栈
		LOG_PROCESS_ERROR_RET(nRet > 0, nRet);
		nLen	+= nRet;

		// 存入value
		nRet	= _SaveValue2Buffer(pbyBuf + nLen, nBufSize - nLen);	// 存入value并出栈
		LOG_PROCESS_ERROR_RET(nRet > 0, nRet);
		nLen	+= nRet;
	}

	LOG_PROCESS_ERROR_RET(nBufSize - nLen >= 1, -1);

	pbyBuf[nLen++]	= '}';								// '}' -> LuaTable End

	nResult	= nLen;

EXIT0:
	lua_settop(m_pLuaState, nTopIndex);

	return nResult;
}

int QLuaScript::_LoadBuffer2Value(const BYTE* pbyBuf, int nBufSize)
{
	INT nResult	= 0;

	switch(pbyBuf[0])	// 外面保证了nBufSize > 0
	{
	case 'N':
		lua_pushnil(m_pLuaState);
		nResult	= 1;
		break;
	case 'T':
		lua_pushboolean(m_pLuaState, 1);
		nResult	= 1;
		break;
	case 'F':
		lua_pushboolean(m_pLuaState, 0);
		nResult	= 1;
		break;
	case 'b':
		LOG_PROCESS_ERROR(nBufSize >= 2);
		lua_pushnumber(m_pLuaState, pbyBuf[1]);
		nResult	= 2;
		break;
	case 'w':
		LOG_PROCESS_ERROR(nBufSize >= 3);
		lua_pushnumber(m_pLuaState, *(LPWORD)(pbyBuf + 1));
		nResult	= 3;
		break;
	case 'd':
		LOG_PROCESS_ERROR(nBufSize >= 5);
		lua_pushnumber(m_pLuaState, *(LPINT)(pbyBuf + 1));
		nResult	= 5;
		break;
	case 'n':
		LOG_PROCESS_ERROR(nBufSize >= 9);
		lua_pushnumber(m_pLuaState, *(double*)(pbyBuf + 1));
		nResult	= 9;
		break;
	case 's':
		{
			LOG_PROCESS_ERROR(nBufSize >= 3);
			INT nStrLen	= *(LPWORD)(pbyBuf + 1);
			LOG_PROCESS_ERROR(nBufSize >= 3 + nStrLen);
			lua_pushlstring(m_pLuaState, (LPCSTR)pbyBuf + 3, (UINT)nStrLen);
			nResult	= 3 + nStrLen;
		}
		break;
	case '{':
		{
			INT nRet = _LoadBuffer2Table(pbyBuf, nBufSize);
			LOG_PROCESS_ERROR(nRet);

			nResult	= nRet;
		}
		break;
	default:
		LOG_PROCESS_ERROR(pbyBuf[0] >= '0' && pbyBuf[0] <= '9');	// 1位数字
		lua_pushnumber(m_pLuaState, pbyBuf[0] - '0');
		nResult = 1;
	}

EXIT0:
	return nResult;
}

int QLuaScript::_LoadBuffer2Table(const BYTE* pbyBuf, int nBufSize)
{
	INT nResult	= 0;

	INT nLen	= 1;	// 外面保证Buffer以'{'开头

	INT nTopIndex	= lua_gettop(m_pLuaState);

	// 证留有足够的剩余堆栈（正常情况不应当使用这么多的堆栈，这里主要是防止意外攻击）
	// （不采用自动分配更多堆栈的方式，以免攻击代码有意消耗内存）
	LOG_PROCESS_ERROR(nTopIndex < KD_N_BASE_CALL_STACK - 100);

	lua_newtable(m_pLuaState);

	// -=先读入顺序部分=-
	for (INT nOrdinalCount = 1; ; ++nOrdinalCount)
	{
		LOG_PROCESS_ERROR(nBufSize - nLen >= 1);

		// 分隔符
		if (pbyBuf[nLen] == ';')
		{
			nLen ++;
			LOG_PROCESS_ERROR(nBufSize - nLen >= 1);
			break;
		}

		// Table结束
		if (pbyBuf[nLen] == '}')
			break;

		// 读取value
		INT nRet	= _LoadBuffer2Value(pbyBuf + nLen, nBufSize - nLen);
		LOG_PROCESS_ERROR(nRet > 0);
		nLen	+= nRet;

		// 设置table[nOrdinalCount]	= value;
		lua_rawseti(m_pLuaState, -2, nOrdinalCount);
	}

	// -=然后处理非顺序部分=-
	while (pbyBuf[nLen] != '}')
	{
		// 读取key
		INT nRet	= _LoadBuffer2Value(pbyBuf + nLen, nBufSize - nLen);
		LOG_PROCESS_ERROR(nRet > 0);
		nLen	+= nRet;

		// 读取value
		nRet	= _LoadBuffer2Value(pbyBuf + nLen, nBufSize - nLen);
		LOG_PROCESS_ERROR(nRet > 0);
		nLen	+= nRet;

		// 设置table[key]	= value;
		lua_settable(m_pLuaState, -3);
	}

	nResult	= nLen + 1;	// 加1是'}'

EXIT0:
	if (!nResult)	// 失败恢复堆栈
		lua_settop(m_pLuaState, nTopIndex);

	return nResult;
}

/*
int QLuaScript::_LuaGetStack(lua_State *L)
{
	int nParams	= lua_gettop(L);
	int nStart	= 0;
	int nEnd	= 100;
	if (nParams >= 1)
	{
		nStart	= (int)lua_tonumber(L, 1);
		if (nParams >= 2)
			nEnd	= nStart + (int)lua_tonumber(L, 2);
	}
	lua_Debug ar;
	lua_newtable(L);
	for (int nLevel = nStart; nLevel < nEnd; ++nLevel)
	{
		if (!lua_getstack(L, nLevel, &ar))
			break;
		lua_getinfo(L, "Snl", &ar);
		lua_newtable(L);
		lua_pushnumber(L, nLevel);
		lua_setfield(L, -2, "nLevel");
		lua_pushstring(L, ar.source);
		lua_setfield(L, -2, "szSource");
		lua_pushnumber(L, ar.currentline);
		lua_setfield(L, -2, "nCurrentLine");
		lua_pushstring(L, ar.namewhat);
		lua_setfield(L, -2, "szNameWhat");
		lua_pushstring(L, ar.name);
		lua_setfield(L, -2, "szName");
		lua_pushstring(L, ar.what);
		lua_setfield(L, -2, "szWaht");
		lua_pushnumber(L, ar.linedefined);
		lua_setfield(L, -2, "nLineDefined");
		lua_rawseti(L, -2, nLevel - nStart + 1);
	}
	return 1;
}
*/
#endif	// SCRIPT_BINDK
