
#pragma once

#include "Core/QLuaScriptGroup.h"
#include "Core/QLuaScript.h"

class ScriptSafe
{
public:
	ScriptSafe(QLuaScript& rJxScript) : m_rJxScript(rJxScript), m_bCalled(false)
	{
		m_nTopIndex	= m_rJxScript.GetTopIndex();
	}
	virtual ~ScriptSafe()
	{
		if (m_bCalled)
		{
			m_rJxScript.SetTopIndex(m_nTopIndex);
			m_bCalled = false;
		}
	}

	QLuaScript* operator->()
	{
		m_bCalled = true;
		return (&m_rJxScript);
	}

	INT GetBeginTopIndex() CONST
	{ return m_nTopIndex; }

protected:
	QLuaScript& m_rJxScript;
	INT m_nTopIndex;
	bool m_bCalled;
};

class KScriptManager
{
public:
	struct KLibFuncs
	{
		CONST TScriptFunc* m_aryLuaFuncs;
		UINT m_uCount;
	};
	struct KTableFuncs
	{
		std::string strTableName;
		CONST SCRIPT_FUNCTION* pScriptFunc;
		DWORD dwCount;
		
		KTableFuncs(LPCSTR szTableName, CONST SCRIPT_FUNCTION* pScriptFunc, DWORD dwCount)
			:strTableName(szTableName), pScriptFunc(pScriptFunc), dwCount(dwCount)
		{}
	};

	typedef std::map<std::string, KLibFuncs> KMapLibFuncs;
	typedef std::vector<KTableFuncs> KVecTableFuncs;

public:
	BOOL	Init();
	BOOL	Uninit();

	BOOL	LoadScript(LPCSTR pszFileName, BOOL bFource = FALSE);

	BOOL	LoadDir(LPCSTR pszDirName, LPCSTR pszPartnerFile = NULL);

	BOOL	ReloadDir(LPCSTR pszDirName);

	BOOL	RegisterTableFuncs(LPCSTR pszTableName, CONST SCRIPT_FUNCTION *arFuncs, INT nFuncCount);

	QLuaScript&		GetMainScript(); // «Î∏ƒ”√GetSafeScript“‘±‹√‚Ω≈±æ’ª–π¬∂
	ScriptSafe		GetSafeScript();
	QScriptGroup*	GetScriptGroup();

	QLunaBase*		SetMe(QLunaBase* pMe);
	QLunaBase*		SetIt(QLunaBase* pIt);
	QLunaBase*		SetHim(QLunaBase* pHim);

private:
	BOOL			_SetHelperObject(QLunaBase* pLunaObj, LPCSTR pszName);

private:
	QScriptGroup		m_cScriptGroup;
	KVecTableFuncs		m_vecTableFunc;
};

extern KScriptManager g_cScriptManager;


