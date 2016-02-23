#include "CorePrivate.h"

#ifdef SCRIPT_BINDK

#include "QLuaScriptGroup.h"
#include "QLuaScript.h"

#include "QStrBase.h"
#include "File.h"

#include<sys/stat.h>
#ifndef WIN32
	#include <unistd.h>
	#include <sys/types.h>
	#include <dirent.h>
#else 
	#include <io.h>
	#include <direct.h>
#endif

#define LUNA_OBJ_SCRIPTGROUP	"_QLuaScriptGroup"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


QScriptGroup* QScriptGroup::GetThis(QLuaScript& sc)
{
	sc.GetGlobal(LUNA_OBJ_SCRIPTGROUP);

	QScriptGroup* psgThis	= (QScriptGroup*)sc.GetUser(-1);

	sc.PopStack(1);

	_ASSERT(psgThis && psgThis->GetScript() == &sc);

	return psgThis;
}

int QScriptGroup::_LuaInclude(QLuaScript& sc)
{
	BOOL bOK				= FALSE;
	QScriptGroup* pGroup	= GetThis(sc);//todo:可能对协程会造成影响，待观察 by dzh
	LPCSTR pszFileName		= sc.GetStr(1);
	CHECK_REPORT(pGroup);
	CHECK_REPORT(pszFileName);

	bOK	= pGroup->_LoadScript(pszFileName);

	sc.PushNumber(bOK);

EXIT0:
	return bOK;
}

int QScriptGroup::_LuaDoFile(QLuaScript& sc)
{
	QScriptGroup* pGroup	= GetThis(sc);//todo:可能对协程会造成影响，待观察 by dzh
	LPCSTR pszFileName		= sc.GetStr(1);
	CHECK_REPORT(pGroup);
	CHECK_REPORT(pszFileName);

	sc.PushNumber(pGroup->_DoFile(pszFileName));
	return 1;

EXIT0:
	return 0;
}

QScriptGroup::QScriptGroup(LPCSTR pszGroupPath /* = NULL */)
{
	m_bCanUseUnpackFile	= TRUE;

	if (pszGroupPath)
	{
		QStrCpyLen(m_szGroupPath, pszGroupPath, sizeof(m_szGroupPath));
		m_szGroupPath[sizeof(m_szGroupPath)-1]	= '\0';
	}
	else
	{
		m_szGroupPath[0]	= 0;
	}

	LPSTR ptr;
	for (ptr = m_szGroupPath; *ptr; ptr++)
		if(*ptr == '/')
			*ptr = '\\';
	if (ptr > m_szGroupPath && *(ptr - 1) != '\\')
	{
		ptr[0]	= '\\';
		ptr[1]	= '\0';
	}

	// Clear(TRUE);
}

void QScriptGroup::Init(BOOL bClearAll, BOOL bDebug /* =FALSE */)
{
	if (bClearAll)
	{
		m_scScript.Init(bDebug);

		m_scScript.PushPointer(this);
		m_scScript.SetGlobalName(LUNA_OBJ_SCRIPTGROUP);

		m_scScript.RegisterGlobalFunction("Require",	QLuaScript::Conv2LuaFunc<_LuaInclude>);
		m_scScript.RegisterGlobalFunction("DoScript",	QLuaScript::Conv2LuaFunc<_LuaDoFile>);
		m_scScript.RegisterGlobalFunction("Include",	QLuaScript::Conv2LuaFunc<_LuaInclude>);
		m_scScript.RegisterGlobalFunction("DoFile",	QLuaScript::Conv2LuaFunc<_LuaDoFile>);
	}

	m_mapFileStates.clear();
}

BOOL QScriptGroup::LoadScriptInDirectory(LPCSTR pszScriptPath)
{
	if (!pszScriptPath || !pszScriptPath[0])
		return FALSE;

	char szFullPath[MAX_PATH];
	g_GetFullPath(szFullPath, pszScriptPath);

	for (LPSTR ptr = szFullPath; *ptr; ptr++)
	{
		if(*ptr == '/' || *ptr == '\\')
		{
			if (*(ptr+1))
				*ptr = CH_SEPARATOR;
			else
				*ptr = '\0';
		}
	}

	if (m_bCanUseUnpackFile)
	{
		// 递归加载包外脚本
		if (!_LoadScriptInDirectory(szFullPath))
			return FALSE;
	}

	// TODO: 加载包内脚本

	QLuaScript::ms_pOutFun("%d loaded!%s", m_mapFileStates.size(), QLuaScript::ms_szOutEnd);

	return TRUE;
}

BOOL QScriptGroup::LoadScript(LPCSTR pszFileName, BOOL bFource /* = FALSE */)
{
	if (!pszFileName)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	if (bFource)
		return _DoFile(pszFileName);
	else
		return _LoadScript(pszFileName);
}

BOOL QScriptGroup::_LoadScript(LPCSTR pszFileName)
{
	if (*pszFileName == '\\' || *pszFileName == '/')
		pszFileName++;

	DWORD dwFileID	= g_FileNameHash(pszFileName);
	_QE_FILESTATE eFileState	= emFILESTATE_NONE;

	_MapFileState::iterator it = m_mapFileStates.find(dwFileID);
	if (it != m_mapFileStates.end())
		eFileState	= it->second;

	switch(eFileState)
	{
	case emFILESTATE_NONE:
		m_mapFileStates.insert(_MapFileState::value_type(dwFileID, emFILESTATE_NONE));
		break;

	case emFILESTATE_LOADING:
		_ASSERT(FALSE);	// 发生循环Include
		return FALSE;

	case emFILESTATE_LOADED:
		return TRUE;	// 已加载过

	case emFILESTATE_ERROR:
		break;

	default:
		_ASSERT(FALSE);
		return FALSE;
	}

	return _DoFile(pszFileName);
}

BOOL QScriptGroup::_DoFile(LPCSTR pszFileName)
{
	QLogPrintf(LOG_INFO,"..QScriptGroup::_DoFile(LPCSTR pszFileName=%s)",pszFileName);
	DWORD dwFileID	= g_FileNameHash(pszFileName);
	m_mapFileStates[dwFileID]		= emFILESTATE_LOADING;
	if (!m_scScript.DoFile(pszFileName, !m_bCanUseUnpackFile))
	{
		m_mapFileStates[dwFileID]	= emFILESTATE_ERROR;
		//QLuaScript::ms_pErrOutFun("%sCan't load file '%s', please check it!%s",
		//	QLuaScript::ms_szOutEnd, pszFileName, QLuaScript::ms_szOutEnd);
		QLogPrintf(LOG_ERR,"%sCan't load file '%s', please check it!%s",
			QLuaScript::ms_szOutEnd, pszFileName, QLuaScript::ms_szOutEnd);
		return FALSE;
	}

	m_mapFileStates[dwFileID]	= emFILESTATE_LOADED;
	return TRUE;
}

//加载某目录下含子目录的所有脚本（传入目录参数为绝对路径）
BOOL QScriptGroup::_LoadScriptInDirectory(LPCSTR pszDirFullPath)
{
	int nFlag;
	char szOldPath[MAX_PATH];
	char szPathBuf[MAX_PATH];
	char szRootPath[MAX_PATH];

	QStrCpy(szPathBuf, pszDirFullPath);
	g_GetRootPath(szRootPath);

	int nLenRoot = strlen(szRootPath);

	LPSTR pszFileName	= szPathBuf + strlen(szPathBuf);
	*(pszFileName++)	= CH_SEPARATOR;
	*pszFileName		= '\0';

	getcwd(szOldPath, sizeof(szOldPath));

	if(chdir(szPathBuf))
		return TRUE;

#ifdef unix
	DIR *dp;
	struct dirent *ep;
	dp = opendir(".");
	CHECK_REPORT(dp);
	while(ep = readdir(dp)) 
	{
		QStrCpy(pszFileName, ep->d_name);
#else
	_finddata_t FindData;
	int dir = _findfirst("*.*", &FindData);
	CHECK_REPORT(dir != -1);
	do
	{
		QStrCpy(pszFileName, FindData.name);
#endif
		if(QStrCmp(pszFileName, ".") || QStrCmp(pszFileName, "..") || QStrCmp(pszFileName, "CVS"))
			continue;

		nFlag = 0;
#ifdef unix
		if(ep->d_type ==4) 
#else
		if(FindData.attrib & _A_SUBDIR)
#endif
		{
			nFlag = 1;
		}
		else
		{
			for (LPCSTR p = pszFileName; *p && *p != '.'; p++)
			{
				if (*p == CH_SEPARATOR)
				{
					nFlag = 1;
					break;
				}
			}
		}
		if (nFlag == 1)
		{
			if (!_LoadScriptInDirectory(szPathBuf))
			{
				chdir(szOldPath);
				return FALSE;
			}
		}
		else
		{
			int nLenPath		= strlen(szPathBuf);
			if (nLenPath > nLenRoot && (memcmp(szRootPath, szPathBuf, nLenRoot) == 0))
			{
				LPCSTR pszRelativeName	= szPathBuf + nLenRoot;
				int nRelativeLen		= nLenPath - nLenRoot;
				if (nRelativeLen >= 4 && QStrCmp(pszRelativeName + nRelativeLen - 4, ".lua"))
					_LoadScript(pszRelativeName);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
#ifdef unix
	}
	closedir(dp);
#else
	} while (!_findnext(dir, &FindData));
	_findclose(dir);
#endif

EXIT0:
	chdir(szOldPath);
	return TRUE;
}

#endif	// SCRIPT_BINDK
