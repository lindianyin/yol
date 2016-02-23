
#include "stdafx.h"
#include "GlobalDataDbProcess.h"
#include "GlobalDataModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
VOID KGlobalDataQuery_Save_INT::Release()
{
	SAFE_DELETE_ARRAY(m_pGroupData);
	KDBCallbackClassGentor<KGlobalDataQuery_Save_INT>::Free(g_pDatabase, this);
}

BOOL KGlobalDataQuery_Save_INT::DBThreadCallback(INT nParam, LPVOID pParam)
{
	KMySqlDatabase& rcDatabase = g_pDatabase->GetDatabase();
	CHAR szSQLBuf[2048] = {0};
	KGC_MySqlQueryStream rcSQLStream = rcDatabase.CreateQueryStream(szSQLBuf);
	s_TableFieldDesc[emKGLOBALDATA_GROUPDATA].pvData = m_pGroupData;
	s_TableFieldDesc[emKGLOBALDATA_GROUPDATA].uDataSize = m_nDataSize;
	s_TableFieldDesc[emKGLOBALDATA_GROUPID].pvData = &m_nGroupId;
	rcSQLStream.AddInsertRecord(s_nColumnNum, s_TableFieldDesc, szGlobalIntDataName);
	rcSQLStream.AddDupliUpdate(1, &s_TableFieldDesc[emKGLOBALDATA_GROUPDATA]);
//	rcSQLStream.AddFieldCondition(emK_DBSQL_WHERE, emKOPERATOR_EQUAL, s_TableFieldDesc[emKGLOBALDATA_GROUPID]);
	BOOL bRet = rcDatabase.DoQuery(rcSQLStream);
	QCONFIRM_RET_FALSE(bRet);

	KGlobalDataQueryRet_SaveIntGroup *pQueryRet = KDBCallbackClassGentor<KGlobalDataQueryRet_SaveIntGroup>::CreateNew(g_pDatabase);
	QCONFIRM_RET_FALSE(pQueryRet);
	pQueryRet->m_nGroupId = m_nGroupId;
	g_pDatabase->PushMainThreadCallBack(pQueryRet);

	return TRUE;
}


VOID KGlobalDataQueryRet_SaveIntGroup::Release()
{
	KDBCallbackClassGentor<KGlobalDataQueryRet_SaveIntGroup>::Free(g_pDatabase, this);
}

BOOL KGlobalDataQueryRet_SaveIntGroup::MainThreadCallback(INT nParam, LPVOID pParam)
{
	return g_cGlobalDataModule.OnSaveIntGroup(m_nGroupId);
}


VOID KGlobalDataQuery_SaveBinGroup::Release()
{
	SAFE_FREE(m_pGroupData);
	KDBCallbackClassGentor<KGlobalDataQuery_SaveBinGroup>::Free(g_pDatabase, this);
}

BOOL KGlobalDataQuery_SaveBinGroup::DBThreadCallback(INT nParam, LPVOID pParam)
{
	KMySqlDatabase& rcDatabase = g_pDatabase->GetDatabase();
	CHAR szSQLBuf[2048] = {0};
	KGC_MySqlQueryStream rcSQLStream = rcDatabase.CreateQueryStream(szSQLBuf);
	s_TableFieldDesc[emKGLOBALDATA_GROUPDATA].pvData = m_pGroupData;
	s_TableFieldDesc[emKGLOBALDATA_GROUPDATA].uDataSize = m_nDataSize;
	s_TableFieldDesc[emKGLOBALDATA_GROUPID].pvData = &m_nGroupId;
	rcSQLStream.AddInsertRecord(s_nColumnNum, s_TableFieldDesc, szGlobalBinDataName);
	rcSQLStream.AddDupliUpdate(1, &s_TableFieldDesc[emKGLOBALDATA_GROUPDATA]);
//	rcSQLStream.AddFieldCondition(emK_DBSQL_WHERE, emKOPERATOR_EQUAL, s_TableFieldDesc[emKGLOBALDATA_GROUPID]);
	BOOL bRet = rcDatabase.DoQuery(rcSQLStream);
	QCONFIRM_RET_FALSE(bRet);

	KGlobalDataQueryRet_SaveBinGroup *pQueryRet = KDBCallbackClassGentor<KGlobalDataQueryRet_SaveBinGroup>::CreateNew(g_pDatabase);
	QCONFIRM_RET_FALSE(pQueryRet);
	pQueryRet->m_nGroupId = m_nGroupId;
	g_pDatabase->PushMainThreadCallBack(pQueryRet);

	return TRUE;
}

VOID KGlobalDataQueryRet_SaveBinGroup::Release()
{
	KDBCallbackClassGentor<KGlobalDataQueryRet_SaveBinGroup>::Free(g_pDatabase, this);
}

BOOL KGlobalDataQueryRet_SaveBinGroup::MainThreadCallback(INT nParam, LPVOID pParam)
{
	return g_cGlobalDataModule.OnSaveBinGroup(m_nGroupId);
}
// -------------------------------------------------------------------------
