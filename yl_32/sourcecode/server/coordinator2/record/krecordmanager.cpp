#include "stdafx.h"
#include "krecordmanager.h"
#include "KRole.h"
#include "GameCenter.h"
#include "onlinegamebase/ksysservice.h"
#include "recorddef.h"
#include "krecordserverhandler.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"

KRecordManager::KRecordManager(void)
{

}

KRecordManager::~KRecordManager(void)
{

}

BOOL KRecordManager::Init()
{
	BOOL bResult    = false;

	LOG_PROCESS_ERROR(LoadRecords());

	SetServerStartTime();
	memset(m_openServerData,0,sizeof(m_openServerData));
	bResult = true;
	//m_loop = 0;
	//m_time = KSysService::Time(NULL);
EXIT0:
	return bResult;
}

BOOL KRecordManager::LoadRecords()
{
	BOOL bResult    = false;
	BOOL bRetCode   = false;

	bRetCode = g_pGameCenter->m_MiscDB.DoLoadServerRecord(emRecordKinBattle);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_pGameCenter->m_MiscDB.DoLoadServerRecord(emRecordKinBattleWinnerId);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_pGameCenter->m_MiscDB.DoLoadServerRecord(emRecordBankInvest);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_pGameCenter->m_MiscDB.DoLoadServerRecord(emRecordBuyAllPurchase);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_pGameCenter->m_MiscDB.DoLoadServerRecord(emRecordServerStartTime);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_pGameCenter->m_MiscDB.DoLoadServerRecord(emRecordOpenServerData);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

VOID KRecordManager::UnInit()
{
	SaveAll();
}

VOID KRecordManager::SaveAll()
{
	//if(m_loop % 30 ==  0)
	//{
		//重置所有限购次数
		//ResetBuyPurchases();
	//}
	//DWORD tNow =(KSysService::Time(NULL) / 3600) * 3600;
	//mp_timeNow = localtime((const time_t*)&tNow);
	//QLogPrintf(LOG_INFO,"mp_timeNow->tm_hour == %d && tNow=%d",mp_timeNow->tm_hour,tNow);
	
	//if(mp_timeNow->tm_hour == 15 && m_time != tNow && g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_MASTER)
	//{
		ResetBuyPurchases();
	//	m_time = tNow;
	//}
	
	// 保存家族战记录
	SaveKinBattleRecord();

	// 保存三界神皇
	SaveKinBattleWinnerId();

	//保存钱庄投资记录
	SaveBankInvestRecord();

	//保存商店全服限购记录
	SaveBuyAllPurcharseRecord();

	// 开服时间
	SaveServerStartTime();
	
	//保存开服活动时间
	SaveOpenServerData();
	//m_loop ++;
}

BOOL KRecordManager::OnLoadRecordFromDB(INT nRecordId, BYTE* pData, size_t uDataLen)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	switch(nRecordId)
	{
	case emRecordKinBattle:
		bRetCode = LoadKinBattleRecord(pData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);
		break;
	case emRecordKinBattleWinnerId:
		bRetCode = LoadKinBattleWinnerId(pData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);
		break;
	case emRecordLastWinKinInfo:
		bRetCode = LoadLastWinKinInfo(pData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);
		break;
	case emRecordBankInvest:
		bRetCode = LoadBankInvestRecord(pData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);
		break;
	case emRecordBuyAllPurchase:
		bRetCode = LoadBuyAllPurchaseRecord(pData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);
		break;
	case emRecordServerStartTime:
		bRetCode = LoadServerStartTime(pData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);
		break;
	case emRecordOpenServerData:
		bRetCode = LoadOpenServerData(pData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);
		break;
	default:
		break;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KRecordManager::SaveKinBattleRecord()
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	PROCESS_ERROR(m_vecKinBattleRecords.size());

	uPakSize = m_vecKinBattleRecords.size() * sizeof(KKinBattleRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();

	for(KIN_BATTLE_RECORD_LIST::iterator it = m_vecKinBattleRecords.begin(); it != m_vecKinBattleRecords.end(); ++it)
	{
		KKinBattleRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KKinBattleRecord));
		pBuffer += sizeof(KKinBattleRecord);
	}

	bRetCode = g_pGameCenter->m_MiscDB.DoSaveServerRecord(emRecordKinBattle, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}

VOID KRecordManager::SaveKinBattleWinnerId()
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	uPakSize = sizeof(DWORD);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();

	memcpy(pBuffer, &m_dwKinBattleWinnerId, sizeof(DWORD));

	bRetCode = g_pGameCenter->m_MiscDB.DoSaveServerRecord(emRecordKinBattleWinnerId, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}

VOID KRecordManager::SaveServerStartTime()
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	uPakSize = sizeof(DWORD);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();

	memcpy(pBuffer, &m_dwServerStartTime, sizeof(DWORD));

	LOG_PROCESS_ERROR(m_dwServerStartTime > 0);
	bRetCode = g_pGameCenter->m_MiscDB.DoSaveServerRecord(emRecordServerStartTime, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}

VOID  KRecordManager::SaveOpenServerData()
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	uPakSize = sizeof(m_openServerData);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();

	memcpy(pBuffer, m_openServerData, sizeof(m_openServerData));

	bRetCode = g_pGameCenter->m_MiscDB.DoSaveServerRecord(emRecordOpenServerData, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}


VOID KRecordManager::SaveLastWinKinInfo()
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	uPakSize = sizeof(DWORD);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();

	memcpy(pBuffer, &m_sLastWinKinInfo, sizeof(KKinBattleRecord));

	bRetCode = g_pGameCenter->m_MiscDB.DoSaveServerRecord(emRecordLastWinKinInfo, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}

VOID KRecordManager::SaveBankInvestRecord()
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	PROCESS_ERROR(m_vecBankInvestRecords.size());

	uPakSize = m_vecBankInvestRecords.size() * sizeof(KBankInvestRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();

	for(std::vector<KBankInvestRecord>::iterator it = m_vecBankInvestRecords.begin(); it != m_vecBankInvestRecords.end(); ++it)
	{
		KBankInvestRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KBankInvestRecord));
		pBuffer += sizeof(KBankInvestRecord);
	}

	bRetCode = g_pGameCenter->m_MiscDB.DoSaveServerRecord(emRecordBankInvest, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}

VOID KRecordManager::SaveBuyAllPurcharseRecord()
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	PROCESS_ERROR(m_vecBuyAllPurchaseRecords.size());

	uPakSize = m_vecBuyAllPurchaseRecords.size() * sizeof(KBuyAllPurchaseRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();

	for(std::vector<KBuyAllPurchaseRecord>::iterator it = m_vecBuyAllPurchaseRecords.begin(); it != m_vecBuyAllPurchaseRecords.end(); ++it)
	{
		KBuyAllPurchaseRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KBuyAllPurchaseRecord));
		pBuffer += sizeof(KBuyAllPurchaseRecord);
	}

	bRetCode = g_pGameCenter->m_MiscDB.DoSaveServerRecord(emRecordBuyAllPurchase, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}

BOOL KRecordManager::LoadKinBattleRecord(BYTE* pData, size_t uDataLen)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	BYTE*		pBuffer = pData;
	size_t		uPakSize = uDataLen;

	m_vecKinBattleRecords.clear();

	while(uPakSize >= sizeof(KKinBattleRecord))
	{
		KKinBattleRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KKinBattleRecord));
		m_vecKinBattleRecords.push_back(sRecord);
		pBuffer += sizeof(KKinBattleRecord);
		uPakSize -= sizeof(KKinBattleRecord);
	}

	LOG_PROCESS_ERROR(uPakSize == 0);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::LoadKinBattleWinnerId(BYTE* pData, size_t uDataLen)
{
	BOOL bResult = FALSE;

	m_dwKinBattleWinnerId = 0;

	if(uDataLen <= 0)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	LOG_PROCESS_ERROR(uDataLen == sizeof(DWORD));

	memcpy(&m_dwKinBattleWinnerId, pData, sizeof(DWORD));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::LoadServerStartTime(BYTE* pData, size_t uDataLen)
{
	BOOL bResult = FALSE;

	//m_dwServerStartTime = 0;

	if(uDataLen <= 0)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	LOG_PROCESS_ERROR(uDataLen == sizeof(DWORD));

	memcpy(&m_dwServerStartTime, pData, sizeof(DWORD));
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::LoadOpenServerData(BYTE* pData, size_t uDataLen)
{
	BOOL bResult = FALSE;
	if(uDataLen <= 0)
	{
		bResult = TRUE;
		goto EXIT0;
	}
	LOG_PROCESS_ERROR(uDataLen == sizeof(m_openServerData));
	memcpy(&m_openServerData, pData, sizeof(m_openServerData));
	bResult = TRUE;
EXIT0:
	return bResult;
}






BOOL KRecordManager::LoadLastWinKinInfo(BYTE* pData, size_t uDataLen)
{
	BOOL bResult = FALSE;

	memset(&m_sLastWinKinInfo, 0, sizeof(KKinBattleRecord));

	if(uDataLen <= 0)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	LOG_PROCESS_ERROR(uDataLen == sizeof(KKinBattleRecord));

	memcpy(&m_sLastWinKinInfo, pData, sizeof(KKinBattleRecord));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::LoadBankInvestRecord(BYTE* pData, size_t uDataLen)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	BYTE*		pBuffer = pData;
	size_t		uPakSize = uDataLen;

	m_vecBankInvestRecords.clear();

	while(uPakSize >= sizeof(KBankInvestRecord))
	{
		KBankInvestRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KBankInvestRecord));
		m_vecBankInvestRecords.push_back(sRecord);
		pBuffer += sizeof(KBankInvestRecord);
		uPakSize -= sizeof(KBankInvestRecord);
	}

	LOG_PROCESS_ERROR(uPakSize == 0);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::LoadBuyAllPurchaseRecord(BYTE* pData, size_t uDataLen)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	BYTE*		pBuffer = pData;
	size_t		uPakSize = uDataLen;

	m_vecBuyAllPurchaseRecords.clear();

	while(uPakSize >= sizeof(KBuyAllPurchaseRecord))
	{
		KBuyAllPurchaseRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KBuyAllPurchaseRecord));
		m_vecBuyAllPurchaseRecords.push_back(sRecord);
		pBuffer += sizeof(KBuyAllPurchaseRecord);
		uPakSize -= sizeof(KBuyAllPurchaseRecord);
	}

	LOG_PROCESS_ERROR(uPakSize == 0);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::AddKinBattleRecord(DWORD dwRoleId)
{
	KKinBattleRecord	sRecord;
	KRole*				pRole		= NULL;
	KTong*				pTong		= NULL;
	BOOL				bResult		= FALSE;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleId);
	LOG_PROCESS_ERROR(pRole && pRole->m_dwTongID);

	pTong = g_pGameCenter->m_TongManager.GetTongById(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	sRecord.dwRoleId = dwRoleId;
	sRecord.dwTimestamp = KSysService::Time(NULL);
	sRecord.nBattleNum = m_vecKinBattleRecords.size() + 1;
	::strncpy(sRecord.szKinMasterName, pRole->m_szName, _NAME_LEN);
	::strncpy(sRecord.szKinName, pTong->m_szName, _NAME_LEN);

	m_vecKinBattleRecords.push_back(sRecord);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::SetLastWinKinInfo(DWORD dwRoleId)
{
	KRole*				pRole		= NULL;
	KTong*				pTong		= NULL;
	BOOL				bResult		= FALSE;

	memset(&m_sLastWinKinInfo, 0, sizeof(KKinBattleRecord));

	PROCESS_ERROR(dwRoleId);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleId);
	LOG_PROCESS_ERROR(pRole && pRole->m_dwTongID);

	pTong = g_pGameCenter->m_TongManager.GetTongById(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	m_sLastWinKinInfo.dwRoleId = dwRoleId;
	m_sLastWinKinInfo.dwTimestamp = KSysService::Time(NULL);
	m_sLastWinKinInfo.nBattleNum = m_vecKinBattleRecords.size() + 1;
	::strncpy(m_sLastWinKinInfo.szKinMasterName, pRole->m_szName, _NAME_LEN);
	::strncpy(m_sLastWinKinInfo.szKinName, pTong->m_szName, _NAME_LEN);

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KRecordManager::SetServerStartTime()
{
	if(!m_dwServerStartTime)
	{
		m_dwServerStartTime = time(NULL);
	}
}

BOOL KRecordManager::SetBankInvestRecord(INT nCoins, DWORD dwRoleId)
{
	KRole*				pRole		= NULL;
	BOOL				bResult		= FALSE;
	KBankInvestRecord	sBankInvestRecord;

	PROCESS_ERROR(dwRoleId);
	pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleId);
	LOG_PROCESS_ERROR(pRole);

	sBankInvestRecord.dwTimestamp = KSysService::Time(NULL);
	sBankInvestRecord.nCoins = nCoins;
	::strncpy(sBankInvestRecord.szRoldName, pRole->m_szName, _NAME_LEN);

	m_vecBankInvestRecords.push_back(sBankInvestRecord);
	if (m_vecBankInvestRecords.size() > MAX_BANK_RECORD_NUM)
		m_vecBankInvestRecords.erase(m_vecBankInvestRecords.begin());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::SetBuyAllPurchaseRequest(S2R_BUY_ALL_PURCHASE_REQUEST &sRequest)
{
	KRole*				pRole		= NULL;
	BOOL				bResult		= FALSE;
	KBuyAllPurchaseRecord*	pRecord = NULL;
	KBuyAllPurchaseRecord	sRecord;
	DWORD dwNow = KSysService::GameTime(NULL);
	INT 	nLast = 0;
	LOG_PROCESS_ERROR(sRequest.dwRoleId);
	pRole = g_pGameCenter->m_RoleManager.GetRole(sRequest.dwRoleId);
	LOG_PROCESS_ERROR(pRole);

	for (std::vector<KBuyAllPurchaseRecord>::iterator it = m_vecBuyAllPurchaseRecords.begin(); it != m_vecBuyAllPurchaseRecords.end(); ++it)
	{
		if (it->nShopId == sRequest.nShopId && it->nCategoryId == sRequest.nCategoryId && it->nGoodsId == sRequest.nGoodsId)
		{
			pRecord = &(*it);
			break;
		}
	}

	if (pRecord != NULL)
	{
		nLast = pRecord->nMaxCount - pRecord->nTotal;
		LOG_PROCESS_ERROR(nLast >= sRequest.nCount);
		if (sRequest.dwLimitTime <= 0 || 
			(pRecord->dwLastTime - sRequest.dwStartTime) / sRequest.dwLimitTime < (dwNow - sRequest.dwStartTime) / sRequest.dwLimitTime)
		{
			pRecord->nCount += sRequest.nCount;
			pRecord->nTotal += sRequest.nCount;
		}
		else
		{
			pRecord->nCount = sRequest.nCount;
			pRecord->nTotal = sRequest.nCount;
		}
		pRecord->dwRoleId = sRequest.dwRoleId;
		pRecord->dwLastTime = dwNow;
	}
	else
	{
		nLast = sRequest.nMaxCount - 0;
		LOG_PROCESS_ERROR(nLast > sRequest.nCount);
		
		sRecord.nShopId = sRequest.nShopId;
		sRecord.nCategoryId = sRequest.nCategoryId;
		sRecord.nGoodsId = sRequest.nGoodsId;
		sRecord.nCount = sRequest.nCount;
		sRecord.dwRoleId = sRequest.dwRoleId;
		sRecord.dwLastTime = dwNow;
		sRecord.nMaxCount = sRequest.nMaxCount;
		m_vecBuyAllPurchaseRecords.push_back(sRecord);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRecordManager::OnGetServerRecordRequest(INT nConnIdx, INT nRecordId)
{
	BOOL bResult = false;
	BOOL bRetCode = false;

	switch(nRecordId)
	{
	case emRecordKinBattle:
		OnGetKinBattleRecordRespond(nConnIdx);
		break;
	case emRecordKinBattleWinnerId:
		OnGetKinBattleWinnerIdRespond(nConnIdx);
		break;
	case emRecordLastWinKinInfo:
		OnGetLastWinKinInfoRespond(nConnIdx);
		break;
	case emRecordBankInvest:
		OnGetBankInvestRecordBroadcast();
		break;
	case emRecordBuyAllPurchase:
		OnBuyAllPurchaseRecordBroadcast();
		break;
	case emRecordServerStartTime:
		QLogPrintf(LOG_INFO,"record OnGetServerRecordRequest %d",nRecordId);
		OnGetServerStartTimeRespond(nConnIdx);
	default:
		break;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KRecordManager::OnGetKinBattleRecordRespond(INT nConnIndex)
{
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;
	LPBYTE				pData		= NULL;

	PROCESS_ERROR(m_vecKinBattleRecords.size());

	uPakSize = m_vecKinBattleRecords.size() * sizeof(KKinBattleRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();
	pBuffer = pData;

	for(KIN_BATTLE_RECORD_LIST::iterator it = m_vecKinBattleRecords.begin(); it != m_vecKinBattleRecords.end(); ++it)
	{
		KKinBattleRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KKinBattleRecord));
		pBuffer += sizeof(KKinBattleRecord);
	}

	bRetCode = KRecordHandler::DoGetRecordRespond(nConnIndex, emRecordKinBattle, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KRecordManager::OnGetKinBattleWinnerIdRespond(INT nConnIndex)
{
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pData		= NULL;

	uPakSize = sizeof(DWORD);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();

	memcpy(pData, &m_dwKinBattleWinnerId, sizeof(DWORD));

	bRetCode = KRecordHandler::DoGetRecordRespond(nConnIndex, emRecordKinBattleWinnerId, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KRecordManager::OnGetServerStartTimeRespond(INT nConnIndex)
{
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pData		= NULL;

	uPakSize = sizeof(DWORD);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();

	memcpy(pData, &m_dwServerStartTime, sizeof(DWORD));

	bRetCode = KRecordHandler::DoGetRecordRespond(nConnIndex, emRecordServerStartTime, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KRecordManager::OnGetLastWinKinInfoRespond(INT nConnIndex)
{
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pData		= NULL;

	uPakSize = sizeof(KKinBattleRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();

	memcpy(pData, &m_sLastWinKinInfo, sizeof(KKinBattleRecord));

	bRetCode = KRecordHandler::DoGetRecordRespond(nConnIndex, emRecordLastWinKinInfo, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KRecordManager::OnGetBankInvestRecordBroadcast()
{
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;
	LPBYTE				pData		= NULL;

	PROCESS_ERROR(m_vecBankInvestRecords.size());

	uPakSize = m_vecBankInvestRecords.size() * sizeof(KBankInvestRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();
	pBuffer = pData;

	for(std::vector<KBankInvestRecord>::iterator it = m_vecBankInvestRecords.begin(); it != m_vecBankInvestRecords.end(); ++it)
	{
		KBankInvestRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KBankInvestRecord));
		pBuffer += sizeof(KBankInvestRecord);
	}

	bRetCode = KRecordHandler::DoGetRecordBroadcast(emRecordBankInvest, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}






BOOL KRecordManager::OnBuyAllPurchaseRecordBroadcast()
{
	QLogPrintf(LOG_INFO,"..KRecordManager::OnBuyAllPurchaseRecordBroadcast()..");
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;
	LPBYTE				pData		= NULL;
	std::vector<KBuyAllPurchaseRecord> _temp;
	BOOL                isContain   = FALSE;
	PROCESS_ERROR(m_vecBuyAllPurchaseRecords.size());

	uPakSize = m_vecBuyAllPurchaseRecords.size() * sizeof(KBuyAllPurchaseRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();
	pBuffer = pData;

	
	for(std::vector<KBuyAllPurchaseRecord>::iterator it = m_vecBuyAllPurchaseRecords.begin(); it != m_vecBuyAllPurchaseRecords.end(); ++it)
	{
		KBuyAllPurchaseRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KBuyAllPurchaseRecord));
		pBuffer += sizeof(KBuyAllPurchaseRecord);
	}


	bRetCode = KRecordHandler::DoGetRecordBroadcast(emRecordBuyAllPurchase, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KRecordManager::OnBuyAllPurchaseRecordRespond(INT nConnIndex)
{
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;
	LPBYTE				pData		= NULL;

	uPakSize = sizeof(KBuyAllPurchaseRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();
	pBuffer = pData;

	/*for(std::vector<KBuyAllPurchaseRecord>::iterator it = m_vecBuyAllPurchaseRecords.begin(); it != m_vecBuyAllPurchaseRecords.end(); ++it)
	{
		if (it->)
		KBuyAllPurchaseRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KBuyAllPurchaseRecord));
		pBuffer += sizeof(KBuyAllPurchaseRecord);
	}*/

	//bRetCode = KRecordHandler::Send(nConnIndex, emRecordBuyAllPurchase, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KRecordManager::OnAllPurchaseRespond(INT nConnIndex, INT nShopId, INT nCategoryId, INT nGoodsId, INT nBuycount, DWORD dwRoleId)
{
	BOOL				bResult		= FALSE;
	INT nCount = 0;
	for(std::vector<KBuyAllPurchaseRecord>::iterator it = m_vecBuyAllPurchaseRecords.begin(); it != m_vecBuyAllPurchaseRecords.end(); ++it)
	{
		if (it->nShopId == nShopId && it->nCategoryId == nCategoryId && it->nGoodsId == nGoodsId)
		{
			 nCount = it->nCount;
			 break;
		}
	}

	KRecordHandler::DoGetAllPurchaseRespond(nConnIndex, nShopId, nCategoryId, nGoodsId, nCount, nBuycount,dwRoleId);
	bResult = TRUE;
EXIT0:
	return bResult;
}

DWORD KRecordManager::GetOpenServerTime()
{
	return m_dwServerStartTime;
}


DWORD * KRecordManager::GetOpenServerData()
{
	return m_openServerData;
}

BOOL KRecordManager::ResetBuyPurchases()
{
	//QLogPrintf(LOG_INFO,"..ResetBuyPurchases..");
	//DWORD tNow = (KSysService::Time(NULL) / 3600) * 3600;
	time_t tNow = KSysService::Time(NULL);
	struct tm *  mp_timeNow = localtime(&tNow);
	//QLogPrintf(LOG_INFO,"mp_timeNow->tm_hour == %d && tNow=%d",mp_timeNow->tm_hour,tNow);
	if(mp_timeNow->tm_hour == 12 && mp_timeNow->tm_min == 1)
	{
		for(std::vector<KBuyAllPurchaseRecord>::iterator it = m_vecBuyAllPurchaseRecords.begin(); it != m_vecBuyAllPurchaseRecords.end(); ++it)
		{		
				KBuyAllPurchaseRecord* pRecord = &(*it);
				pRecord->nCount = 0;
				pRecord->nTotal = 0;
				pRecord->dwLastTime = tNow;
		}
		OnBuyAllPurchaseRecordBroadcast();
	}
	return TRUE;
}