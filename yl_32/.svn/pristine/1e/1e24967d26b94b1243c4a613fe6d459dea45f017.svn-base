

#include "stdafx.h"
#include "GlobalDataModule.h"
#include "config/kglobaldatadef.h"
#include "GlobalDataProtocolProcess.h"
#include "gcmodule/globaldata/kglobaldataptc.h"
#include "config/kglobaldatadef.h"
#include "onlinegamebase/mysqldatabase.h"
#include "GlobalDataDbProcess.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "ScheduleTaskCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



KGC_GlobalDataModule g_cGlobalDataModule;
IKGcServer *g_pGcServer = NULL;
IKGcQuery* g_pGcQuery = NULL;
IKGcDatabase* g_pDatabase = NULL;
//IKGcScriptEnv* g_pScriptEnv = NULL;


//GCMODULE_API IKGC_Module* CreateModuleInstance(IKGcQuery* piQuery)
//{
//	QCONFIRM_RET_NULL(piQuery);
//	g_pGcQuery = piQuery;
//	return &g_cGlobalDataModule;
//}

// -------------------------------------------------------------------------
KGC_GlobalDataModule::KGC_GlobalDataModule()
{

}

extern BOOL g_RegisterKGlobalDataScriptFun(KScriptManager &rcScriptMgr);

extern BOOL s_init_save_map();
BOOL KGC_GlobalDataModule::OnInit()
{
	s_init_save_map();
	QCONFIRM_RET_FALSE(g_pGcQuery);
	HRESULT hr = g_pGcQuery->QueryInterface(IID_IKG_GCServer, (void**)&g_pGcServer);
	QCONFIRM_RET_NULL(SUCCEEDED(hr));
	g_pGcServer->RegisterGsProcessor(&g_cGlobalDataAcceptor);

	hr = g_pGcQuery->QueryInterface(IID_IKG_GCDatabase, (void**)&g_pDatabase);
	QCONFIRM_RET_NULL(SUCCEEDED(hr));

	// schedule task
	BOOL bRet = g_ScheduleTask.Init();
	QCONFIRM_RET_FALSE(bRet);

	return TRUE;
}


BOOL KGC_GlobalDataModule::OnStartUp()
{
	KMySqlDatabase& rcDatabase = g_pDatabase->GetDatabase();
	CHAR szSQLBuf[2048] = {0};
	KGC_MySqlQueryStream rcSQLStream = rcDatabase.CreateQueryStream(szSQLBuf);

	// -------------------------------------------------------------------------
	// 创建Int数据库表
	rcSQLStream.AddCreateTable(emKTABLE_CREATE_IF_NOT_EXIST, s_GlobalIntDataTableDesc);
	rcSQLStream.AddCreateTableSingleFieldIndex(
		sizeof(s_TableSingleColIndex) / sizeof(s_TableSingleColIndex[0]), 
		s_TableSingleColIndex);
	BOOL nRet = rcDatabase.DoQuery(rcSQLStream);
	QCONFIRM_RET_FALSE(nRet);

	// -------------------------------------------------------------------------
	// 创建Bin数据库表
	memset(szSQLBuf, 0, sizeof(szSQLBuf));
	rcSQLStream.Reset();
	rcSQLStream.AddCreateTable(emKTABLE_CREATE_IF_NOT_EXIST, s_GlobalBinDataTableDesc);
	rcSQLStream.AddCreateTableSingleFieldIndex(
		sizeof(s_TableSingleColIndex) / sizeof(s_TableSingleColIndex[0]), 
		s_TableSingleColIndex);
	nRet = rcDatabase.DoQuery(rcSQLStream);
	QCONFIRM_RET_FALSE(nRet);


	// -------------------------------------------------------------------------
	static KDBFieldDesc s_SelectFiled[] = {
		s_TableFieldDesc[emKGLOBALDATA_GROUPID],
		s_TableFieldDesc[emKGLOBALDATA_GROUPDATA]
	};
	// 载入INT数据
	memset(szSQLBuf, 0, sizeof(szSQLBuf));
	rcSQLStream.Reset();
	rcSQLStream.AddSelectQuery(2, s_SelectFiled, szGlobalIntDataName);
	nRet = rcDatabase.DoQuery(rcSQLStream);
	QCONFIRM_RET_FALSE(nRet);
	KMyqlResultProtected cQueryIntResult = rcDatabase.GetQueryResult();
	QCONFIRM_RET_FALSE(cQueryIntResult.IsValid());
	INT nRowCount = cQueryIntResult.GetRowCount();
	for (INT i = 0; i < nRowCount; ++i)
	{
		KMySqlRow rcGroupRow = cQueryIntResult.GetNextRow();
		QCONFIRM_RET_FALSE(rcGroupRow.IsValid());
		QCONFIRM_RET_FALSE(rcGroupRow.aFieldData[0]);
		WORD wGroupId = atoi(rcGroupRow.aFieldData[0]);
		QCONFIRM_RET_FALSE(rcGroupRow.aFieldData[1]);
		INT nDataLen = rcGroupRow.aFieldLenth[1];
		QCONFIRM_RET_FALSE(nDataLen % (sizeof(WORD) + sizeof(INT)) == 0);
		CHAR *pszItemData = rcGroupRow.aFieldData[1];
		while (nDataLen > 0)
		{
			WORD wKey = *(WORD*)pszItemData;
			pszItemData += sizeof(WORD);
			nDataLen -= sizeof(WORD);

			INT nValue = *(INT*)pszItemData;
			pszItemData += sizeof(INT);
			nDataLen -= sizeof(INT);
			SetIntValue(wGroupId, wKey, nValue, 0);

		}
	}

	for (INT_DATA_GROUP_MAP::iterator it = m_mapIntDataGroup.begin(); it != m_mapIntDataGroup.end(); ++it)
		it->second.byState &= ~(1 << emKDATA_STATE_MODIFYED);

	// -------------------------------------------------------------------------
	// 载入Bin数据
	memset(szSQLBuf, 0, sizeof(szSQLBuf));
	rcSQLStream.Reset();
	rcSQLStream.AddSelectQuery(2, s_SelectFiled, szGlobalBinDataName);
	nRet = rcDatabase.DoQuery(rcSQLStream);
	QCONFIRM_RET_FALSE(nRet);
	KMyqlResultProtected cQueryBinResult = rcDatabase.GetQueryResult();
	QCONFIRM_RET_FALSE(cQueryBinResult.IsValid());
	nRowCount = cQueryBinResult.GetRowCount();
	for (INT i = 0; i < nRowCount; ++i)
	{
		KMySqlRow rcGroupRow = cQueryBinResult.GetNextRow();
		QCONFIRM_RET_FALSE(rcGroupRow.IsValid());
		QCONFIRM_RET_FALSE(rcGroupRow.aFieldData[0]);
		WORD wGroupId = atoi(rcGroupRow.aFieldData[0]);
		QCONFIRM_RET_FALSE(rcGroupRow.aFieldData[1]);
		INT nDataLen = rcGroupRow.aFieldLenth[1];
		CHAR *pszItemData = rcGroupRow.aFieldData[1];
		while (nDataLen > 0)
		{
			WORD wKey = *(WORD*)pszItemData;
			pszItemData += sizeof(WORD);
			nDataLen -= sizeof(WORD);
			QCONFIRM_RET_FALSE(nDataLen > 0);

			WORD wLen = *(WORD*)pszItemData;
			pszItemData += sizeof(WORD);
			nDataLen -= sizeof(WORD);
			QCONFIRM_RET_FALSE(nDataLen > 0);

			BYTE *pData = (BYTE *)pszItemData;
			pszItemData += wLen;
			nDataLen -= wLen;
			QCONFIRM_RET_FALSE(nDataLen >= 0);
			SetBinaryValue(wGroupId, wKey, pData, wLen, 0);
		}
	}

	for (BINARY_DATA_GROUP_MAP::iterator it = m_mapBinaryDataGroup.begin(); it != m_mapBinaryDataGroup.end(); ++it)
		it->second.byState &= ~(1 << emKDATA_STATE_MODIFYED);

	return TRUE;
}

BOOL KGC_GlobalDataModule::Breath()
{
	static SIZE_T s_nCount = 0;
	static SIZE_T s_nIntIt = 0;
	static SIZE_T s_nBinIt = 0;

	if (++s_nCount % GAME_FPS == 0)
	{
		{
			s_nIntIt = (s_nIntIt + 1) % (m_mapIntDataGroup.size() + 1);
			INT_DATA_GROUP_MAP::iterator it = m_mapIntDataGroup.begin();
			SIZE_T i = 0;
			do 
			{
				if (it == m_mapIntDataGroup.end())
					break;
				if (i++ >= s_nIntIt)
					break;

			} while (TRUE);

			while(it != m_mapIntDataGroup.end())
			{
				if (SaveIntGroup(it->first))
					break;
				++it;
			}
		}

		{
			s_nBinIt = (s_nBinIt + 1) % (m_mapBinaryDataGroup.size() + 1);
			BINARY_DATA_GROUP_MAP::iterator it = m_mapBinaryDataGroup.begin();
			SIZE_T i = 0;
			do 
			{
				if (it == m_mapBinaryDataGroup.end())
					break;
				if (i++ >= s_nBinIt)
					break;

			} while (TRUE);

			while(it != m_mapBinaryDataGroup.end())
			{
				if (SaveBinGroup(it->first))
					break;
				++it;
			}
		}

		if (m_mapBinaryDataGroup.size() > 0)
			SaveBinGroup((INT)(s_nCount % m_mapBinaryDataGroup.size()));

	}

	// schedule task
	g_ScheduleTask.Breath();

	return TRUE;
}

BOOL KGC_GlobalDataModule::GetIntValue(WORD wGroupId, WORD wKey, INT &rnValue)
{
	INT_DATA_GROUP_MAP::iterator itGroup = m_mapIntDataGroup.find(wGroupId);
	if (itGroup == m_mapIntDataGroup.end())
		return 0;
//	QCONFIRM_RET_FALSE(itGroup != m_mapIntDataGroup.end());

	INT_GROUP_BASE_DATA::iterator itData = itGroup->second.cIntDataMap.find(wKey);
	if (itData == itGroup->second.cIntDataMap.end())
		return 0;
//	QCONFIRM_RET_FALSE(itData != itGroup->second.cIntDataMap.end());

	rnValue = itData->second;

	return TRUE;
}

BOOL KGC_GlobalDataModule::SetIntValue(WORD wGroupId, WORD wKey, INT nValue, INT nConnectId)
{
	INT_DATA_GROUP_MAP::iterator itGroup = m_mapIntDataGroup.find(wGroupId);
	if (itGroup == m_mapIntDataGroup.end())
	{
		std::pair<INT_DATA_GROUP_MAP::iterator, bool> cInsRet = 
			m_mapIntDataGroup.insert(INT_DATA_GROUP_MAP::value_type(wGroupId, KGB_INT_DATA_GROUP()));
		QCONFIRM_RET_FALSE(cInsRet.second);
		itGroup = cInsRet.first;
	}
	
	itGroup->second.byState |= 1 << emKDATA_STATE_MODIFYED;
	itGroup->second.cIntDataMap[wKey] = nValue;

	// 同步给GS
	KGLOBALDATA_G2S_INT_VALUE cSendData;
	cSendData.wGroupId = wGroupId;
	cSendData.wKey = wKey;
	cSendData.nValue = nValue;
	g_pGcServer->Send(-1, &cSendData, sizeof(KGLOBALDATA_G2S_INT_VALUE));

	return TRUE;
}

BOOL KGC_GlobalDataModule::AddIntValue(WORD wGroupId, WORD wKey, INT nValue, INT nConnectId)
{
	INT_DATA_GROUP_MAP::iterator itGroup = m_mapIntDataGroup.find(wGroupId);
	if (itGroup == m_mapIntDataGroup.end())
	{
		std::pair<INT_DATA_GROUP_MAP::iterator, bool> cInsRet = 
			m_mapIntDataGroup.insert(INT_DATA_GROUP_MAP::value_type(wGroupId, KGB_INT_DATA_GROUP()));
		QCONFIRM_RET_FALSE(cInsRet.second);
		itGroup = cInsRet.first;
	}
	itGroup->second.byState |= 1 << emKDATA_STATE_MODIFYED;
	itGroup->second.cIntDataMap[wKey] += nValue;

	// 同步给GS
	KGLOBALDATA_G2S_INT_VALUE cSendData;
	cSendData.wGroupId = wGroupId;
	cSendData.wKey = wKey;
	cSendData.nValue = itGroup->second.cIntDataMap[wKey];
	g_pGcServer->Send(-1, &cSendData, sizeof(KGLOBALDATA_G2S_INT_VALUE));

	return TRUE;
}


BOOL KGC_GlobalDataModule::GetBinaryValue(WORD wGroupId, WORD wKey, BYTE abValue[], WORD &rwLength)
{
	QCONFIRM_RET_FALSE(abValue);
	

	BINARY_DATA_GROUP_MAP::iterator itGroup = m_mapBinaryDataGroup.find(wGroupId);
	QCONFIRM_RET_FALSE(itGroup != m_mapBinaryDataGroup.end());

	BINARY_GROUP_BASE_DATA::iterator itData = itGroup->second.cBinDataMap.find(wKey);
	QCONFIRM_RET_FALSE(itData != itGroup->second.cBinDataMap.end());
	
	QCONFIRM_RET_FALSE(rwLength >= itData->second.size());
	rwLength = (WORD)itData->second.size();
	QCONFIRM_RET_FALSE(rwLength > 0);
	memcpy(abValue, &itData->second[0], rwLength);

	return TRUE;
}

BOOL KGC_GlobalDataModule::SetBinaryValue(WORD wGroupId, WORD wKey, BYTE abValue[], WORD wLength, INT nConnectId)
{
	QCONFIRM_RET_FALSE(abValue);
	QCONFIRM_RET_FALSE(wLength > 0 && wLength < KD_BIN_DATA_MAX_LEN);
	BINARY_DATA_GROUP_MAP::iterator itGroup = m_mapBinaryDataGroup.find(wGroupId);
	if (itGroup == m_mapBinaryDataGroup.end())
	{
		std::pair<BINARY_DATA_GROUP_MAP::iterator, bool> cInsRet = 
			m_mapBinaryDataGroup.insert(BINARY_DATA_GROUP_MAP::value_type(wGroupId, KGB_BINARY_DATA_GROUP()));
		QCONFIRM_RET_FALSE(cInsRet.second);
		itGroup = cInsRet.first;
	}

	itGroup->second.byState |= 1 << emKDATA_STATE_MODIFYED;
	itGroup->second.cBinDataMap[wKey] = std::vector<BYTE>(abValue, abValue+wLength);

	static BYTE bySyncPackBuffer[10240];
	memset(bySyncPackBuffer, 0, sizeof(bySyncPackBuffer));
	KGLOBALDATA_G2S_BIN_VALUE& cSendData = *(KGLOBALDATA_G2S_BIN_VALUE* )bySyncPackBuffer;
	cSendData = KGLOBALDATA_G2S_BIN_VALUE();
	cSendData.wGroupId = wGroupId;
	cSendData.wKey = wKey;
	cSendData.wLength = wLength;
	memcpy(cSendData.abValue, abValue, wLength);
	g_pGcServer->Send(-1, &cSendData, sizeof(KGLOBALDATA_G2S_BIN_VALUE) + wLength);

	return TRUE;
}

BOOL KGC_GlobalDataModule::DeleteBinaryValue(WORD wGroupId, WORD wKey, INT nConnectId)
{
	BINARY_DATA_GROUP_MAP::iterator itGroup = m_mapBinaryDataGroup.find(wGroupId);
	QCONFIRM_RET_FALSE(itGroup != m_mapBinaryDataGroup.end());
	QCONFIRM_RET_FALSE(itGroup->second.cBinDataMap.find(wKey) != itGroup->second.cBinDataMap.end());
	itGroup->second.byState |= 1 << emKDATA_STATE_MODIFYED;
	itGroup->second.cBinDataMap.erase(wKey);

	KGLOBALDATA_G2S_DEL_BIN_VALUE cSendData;
	cSendData.wGroupId = wGroupId;
	cSendData.wKey = wKey;
	g_pGcServer->Send(-1, &cSendData, sizeof(KGLOBALDATA_G2S_DEL_BIN_VALUE));
	return TRUE;
}

BOOL KGC_GlobalDataModule::BatchSyncIntData(INT nConnectId)
{
	static BYTE bySyncPackBuffer[10240];
	memset(bySyncPackBuffer, 0, sizeof(bySyncPackBuffer));
	KGLOBALDATA_G2S_BATCH_INT_VALUE& cSendData = *(KGLOBALDATA_G2S_BATCH_INT_VALUE* )bySyncPackBuffer;
	cSendData = KGLOBALDATA_G2S_BATCH_INT_VALUE();
	cSendData.wItemCount = 0;
	for (INT_DATA_GROUP_MAP::iterator itGroup = m_mapIntDataGroup.begin(); itGroup != m_mapIntDataGroup.end(); ++itGroup)
	{
		KGB_INT_DATA_GROUP &rcGroup = itGroup->second;
		WORD wGroupId = itGroup->first;
		for (INT_GROUP_BASE_DATA::iterator itItem = rcGroup.cIntDataMap.begin(); itItem != rcGroup.cIntDataMap.end(); ++itItem)
		{
			cSendData.acIntItem[cSendData.wItemCount].dwKey = MAKELONG(itItem->first, wGroupId);
			cSendData.acIntItem[cSendData.wItemCount].nValue = itItem->second;
			++cSendData.wItemCount;
		}

	}
	g_pGcServer->Send(nConnectId, &cSendData, sizeof(KGLOBALDATA_G2S_BATCH_INT_VALUE) + sizeof(INT_ITEM) * cSendData.wItemCount);

	return TRUE;
}

BOOL KGC_GlobalDataModule::BatchSyncBinData(INT nConnectId)
{
	static BYTE bySyncPackBuffer[10240];
	memset(bySyncPackBuffer, 0, sizeof(bySyncPackBuffer));
	KGLOBALDATA_G2S_BATCH_BIN_VALUE& cSendData = *(KGLOBALDATA_G2S_BATCH_BIN_VALUE* )bySyncPackBuffer;
	cSendData = KGLOBALDATA_G2S_BATCH_BIN_VALUE();
	cSendData.wItemCount = 0;
	cSendData.wDataLen = sizeof(KGLOBALDATA_G2S_BATCH_BIN_VALUE);
	BYTE *pCursor = (BYTE *)cSendData.acBinItem;
	for (BINARY_DATA_GROUP_MAP::iterator itGroup = m_mapBinaryDataGroup.begin(); itGroup != m_mapBinaryDataGroup.end(); ++itGroup)
	{
		WORD wGroupId = itGroup->first;
		for (BINARY_GROUP_BASE_DATA::iterator itItem = itGroup->second.cBinDataMap.begin(); itItem != itGroup->second.cBinDataMap.end(); ++itItem)
		{
			BIN_ITEM *pBinItem = (BIN_ITEM *)pCursor;

			WORD wLength = (WORD)itItem->second.size();
			QCONFIRM_RET_FALSE(wLength);
			QCONFIRM_RET_FALSE(wLength < KD_BIN_DATA_MAX_LEN);

			cSendData.wDataLen += sizeof(BIN_ITEM) - 1 + wLength;
			QCONFIRM_RET_FALSE(cSendData.wDataLen <= sizeof(bySyncPackBuffer) && "buff overflow");

			pBinItem->dwKey = MAKELONG(itItem->first, wGroupId);
			pBinItem->wLength = wLength;
			memcpy(pBinItem->abValue, &itItem->second[0], pBinItem->wLength);

			pCursor += sizeof(BIN_ITEM) - 1 + pBinItem->wLength;
			cSendData.wItemCount++;
		}
	}
	g_pGcServer->Send(nConnectId, &cSendData, sizeof(KGLOBALDATA_G2S_BATCH_BIN_VALUE) + cSendData.wDataLen);

	return TRUE;
}


BOOL KGC_GlobalDataModule::SaveIntGroup(INT nGroupId)
{
	KGlobalDataQuery_Save_INT *pQuery = NULL;
	WORD wGroupId = 0;
	KDB_INT_DATA_ITEM *pData = NULL;
	KGB_INT_DATA_GROUP *pGroup = NULL;
	KDB_INT_DATA_ITEM *pCursor = NULL;

	INT_DATA_GROUP_MAP::iterator itGroup = m_mapIntDataGroup.find(nGroupId);
	PROCESS_ERROR(itGroup != m_mapIntDataGroup.end());

	wGroupId = itGroup->first;
	pGroup = &itGroup->second;
	PROCESS_ERROR(!pGroup->cIntDataMap.empty());
	PROCESS_ERROR((pGroup->byState & (1 << emKDATA_STATE_MODIFYED)) != 0);

	pQuery = KDBCallbackClassGentor<KGlobalDataQuery_Save_INT>::CreateNew(g_pDatabase);
	LOG_PROCESS_ERROR(pQuery);
	pQuery->m_pGroupData = (KDB_INT_DATA_ITEM *)malloc(pGroup->cIntDataMap.size() * sizeof(KDB_INT_DATA_ITEM));
	LOG_PROCESS_ERROR(pQuery->m_pGroupData);

	pCursor = pQuery->m_pGroupData;
	for (INT_GROUP_BASE_DATA::iterator itItem = pGroup->cIntDataMap.begin(); itItem != pGroup->cIntDataMap.end(); ++itItem)
	{
		WORD wKey = itItem->first;
		DWORD dwCompleteKey = MAKELONG(wKey, wGroupId);
		if (s_need_save(dwCompleteKey))
		{
			pCursor->wKey = wKey;
			pCursor->nValue = itItem->second;
			++pCursor;
		}
	}
	LOG_PROCESS_ERROR(pCursor != pQuery->m_pGroupData);
	
	pQuery->m_nGroupId = nGroupId;
	pQuery->m_nDataSize = INT((pCursor - pQuery->m_pGroupData) * sizeof(KDB_INT_DATA_ITEM));

	return g_pDatabase->PushDBAffair(pQuery);
	
EXIT0:
	if (pQuery)
		pQuery->Release();

	return FALSE;
}


BOOL KGC_GlobalDataModule::OnSaveIntGroup(INT nGroupId)
{
	INT_DATA_GROUP_MAP::iterator itGroup = m_mapIntDataGroup.find(nGroupId);
	QCONFIRM_RET_FALSE(itGroup != m_mapIntDataGroup.end());
	itGroup->second.byState &= ~(1 << emKDATA_STATE_MODIFYED);

	return TRUE;
}


BOOL KGC_GlobalDataModule::SaveBinGroup(INT nGroupId)
{
	KGlobalDataQuery_SaveBinGroup *pQuery = NULL;
	WORD wGroupId = 0;
	KGB_BINARY_DATA_GROUP *pGroup = NULL;

	BINARY_DATA_GROUP_MAP::iterator itGroup = m_mapBinaryDataGroup.find(nGroupId);
	PROCESS_ERROR(itGroup != m_mapBinaryDataGroup.end());

	wGroupId = itGroup->first;
	pGroup = &itGroup->second;
	PROCESS_ERROR(!pGroup->cBinDataMap.empty());
	PROCESS_ERROR((pGroup->byState & (1 << emKDATA_STATE_MODIFYED)));

	pQuery = KDBCallbackClassGentor<KGlobalDataQuery_SaveBinGroup>::CreateNew(g_pDatabase);
	LOG_PROCESS_ERROR(pQuery);

	pQuery->m_pGroupData = (BYTE *)malloc(pGroup->cBinDataMap.size() * sizeof(KDB_BIN_DATA_ITEM));
	LOG_PROCESS_ERROR(pQuery->m_pGroupData);
	{
		BYTE *pCursor = pQuery->m_pGroupData;
		for (BINARY_GROUP_BASE_DATA::iterator itItem = pGroup->cBinDataMap.begin(); itItem != pGroup->cBinDataMap.end(); ++itItem)
		{
			WORD wKey = itItem->first;
			DWORD dwCompleteKey = MAKELONG(wKey, wGroupId);
			if (s_need_save(dwCompleteKey))
			{
				KDB_BIN_DATA_ITEM *pItem = (KDB_BIN_DATA_ITEM *)pCursor;
				pItem->wKey = wKey;
				pCursor += sizeof(pItem->wKey);

				pItem->wLen = (WORD)itItem->second.size();
				pCursor += sizeof(pItem->wLen);
				LOG_PROCESS_ERROR(pItem->wLen > 0 && pItem->wLen < KD_BIN_DATA_MAX_LEN);

				memcpy(pItem->abValue, &itItem->second[0], pItem->wLen);
				pCursor += pItem->wLen;
			}
		}

		pQuery->m_nGroupId = nGroupId;
		pQuery->m_nDataSize = INT(pCursor - pQuery->m_pGroupData);
	}

	return g_pDatabase->PushDBAffair(pQuery);
EXIT0:
	if (pQuery)
		pQuery->Release();

	return FALSE;
}


BOOL KGC_GlobalDataModule::OnSaveBinGroup(INT nGroupId)
{
	BINARY_DATA_GROUP_MAP::iterator itGroup = m_mapBinaryDataGroup.find(nGroupId);
	QCONFIRM_RET_FALSE(itGroup != m_mapBinaryDataGroup.end());
	itGroup->second.byState &= ~(1 << emKDATA_STATE_MODIFYED);

	return TRUE;
}

// -------------------------------------------------------------------------
