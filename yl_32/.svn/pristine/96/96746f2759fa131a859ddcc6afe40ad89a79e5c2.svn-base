
#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KScriptManager g_cScriptManager;

BOOL KScriptManager::Init()
{
	m_cScriptGroup.Init(TRUE);

	return TRUE;
}
BOOL KScriptManager::Uninit()
{
	m_cScriptGroup.Uninit();
	return TRUE;
}

BOOL KScriptManager::LoadScript(LPCSTR pszFileName, BOOL bFource /* = FALSE */)
{
	return m_cScriptGroup.LoadScript(pszFileName, bFource);
}

BOOL KScriptManager::LoadDir(LPCSTR pszDirName, LPCSTR pszPartnerFile /* = NULL */)
{
	QLogPrintf(LOG_INFO,"..KScriptManager::LoadDir pszDirName=%s",pszDirName);
	//if (pszPartnerFile)
	//	m_cScriptGroup.SetPackPartnerFile(pszPartnerFile);
	return m_cScriptGroup.LoadScriptInDirectory(pszDirName);
}

BOOL KScriptManager::ReloadDir(LPCSTR pszDirName)
{
	m_cScriptGroup.Init(FALSE, TRUE);
	return m_cScriptGroup.LoadScriptInDirectory(pszDirName);
}

BOOL KScriptManager::RegisterTableFuncs(LPCSTR pszTableName, CONST SCRIPT_FUNCTION *arFuncs, INT nFuncCount)
{
	QCONFIRM_RET_FALSE(arFuncs && nFuncCount > 0);
	ScriptSafe cSafeScript = GetSafeScript();
	return cSafeScript->RegisterTableFunctions(pszTableName, arFuncs, nFuncCount);
}

QLuaScript&	KScriptManager::GetMainScript()
{
	// 不可能为null，之后应该给group加个返回引用的接口
	return *m_cScriptGroup.GetScript();
}

QLunaBase* KScriptManager::SetMe( QLunaBase* pMe )
{
	static QLunaBase* pOldObj = NULL;
	QLunaBase* pReturn = pOldObj;
	pOldObj = pMe;

	_SetHelperObject(pMe, "me");
	return pReturn;
}
QLunaBase* KScriptManager::SetIt( QLunaBase* pIt )
{
	static QLunaBase* pOldObj = NULL;
	QLunaBase* pReturn = pOldObj;
	pOldObj = pIt;

	_SetHelperObject(pIt, "it");
	return pReturn;
}
QLunaBase* KScriptManager::SetHim( QLunaBase* pHim )
{
	static QLunaBase* pOldObj = NULL;
	QLunaBase* pReturn = pOldObj;
	pOldObj = pHim;

	_SetHelperObject(pHim, "him");
	return pReturn;
}

BOOL KScriptManager::_SetHelperObject( QLunaBase* pLunaObj, LPCSTR pszName )
{
	QLuaScript& rcScript = *m_cScriptGroup.GetScript();

	if (pLunaObj)
		rcScript.PushObj(pLunaObj);
	else
		rcScript.PushNull();

	return rcScript.SetGlobalName(pszName);
}

ScriptSafe KScriptManager::GetSafeScript()
{
	return ScriptSafe(*m_cScriptGroup.GetScript());
}

QScriptGroup* KScriptManager::GetScriptGroup()
{
	return &m_cScriptGroup;
}
