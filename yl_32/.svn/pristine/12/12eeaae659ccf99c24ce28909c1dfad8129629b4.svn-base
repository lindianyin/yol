
#include "stdafx.h"
#include "Misc/objevent.h"
#include "kpursedata.h"
#include "kluaplayerpurse.h"
#include "onlinegamemodule/trade/kplayerpurse_i.h"
#include "onlinegamemodule/trade/kpurseprotocol.h"
#include "onlinegamebase/ksysservice.h"
#include "kpursemodule.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "vip/playervip.h"
#include "award/kawardsetting.h"
#include "qpursesetting.h"
#include "awardlobby/playerawardlobby.h"
#include "../onlinegameworld/KRelayClient.h"



#define ZHENQI_CHANGE_MODULE_NAME "ExchangeZhengQi" 
#define MONEY_CHANGE_MODULE_NAME "ExchangeMoney"
#define COIN_COST_MODULE_NAME "ExchangeMoney"
#define ENERGY_DECREASE 2
#define MONEY_DECREASE 1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KPlayerPurse::KPlayerPurse(KCharacter& rcPlayer)
			: KCharacterCtrlBase(rcPlayer), 
			  m_pPurseData(NULL),
			  m_pLuna(NULL),
			  m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	ASSERT(NULL == m_pLuna);
	m_pLuna = new KLuaPlayerPurse(this);
	ASSERT(NULL != m_pLuna);

	ASSERT(NULL == m_pPurseData);
	m_pPurseData = new KPurseData();
	ASSERT(NULL != m_pPurseData);
	nMoneyTimes = 5;
	nEnergyTimes = 5;
}

KPlayerPurse::~KPlayerPurse()
{
	SAFE_DELETE(m_pPurseData);
	SAFE_DELETE(m_pLuna);
}

BOOL KPlayerPurse::Init()
{
	QCONFIRM_RET_FALSE(m_pPurseData);
	m_pPurseData->Clear();

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	BOOL bRetCode = cSafeScript->CallTableFunction(
		"Purse",
		"Init",
		1,
		"o", m_rcPlayer.GetScriptInterface());
	QCONFIRM_RET_FALSE(bRetCode);

	return TRUE;
}

BOOL KPlayerPurse::UnInit()
{
	QCONFIRM_RET_FALSE(m_pPurseData);
	m_pPurseData->Clear();
	return TRUE;
}

BOOL KPlayerPurse::RegistCurrencyType(BYTE byCurrencyID)
{
	ASSERT(m_pPurseData);
	BOOL bResult = m_pPurseData->RegistCurrency(byCurrencyID);
	QCONFIRM_RET_FALSE(bResult);

#ifdef GAME_SERVER
	SyncRegistCurrencyType(byCurrencyID);
#endif
	return TRUE;
}

BOOL KPlayerPurse::SetCurrency(BYTE byCurrencyID, INT nCurrency, KE_PURSE_RESULT* pResult, BOOL bSync /* = 1 */)
{
	ASSERT(m_pPurseData);
	BOOL bResult = m_pPurseData->SetCurrency(byCurrencyID, nCurrency, pResult);
	if (!bResult)
		return FALSE;

#ifdef GAME_SERVER
	if (bSync)
		SyncCurrency(byCurrencyID, nCurrency);
#else
	m_rcPlayer.FireEvent(emKOBJEVENTTYPE_PURSE_CHANGECURRENCY, (INT)byCurrencyID, nCurrency, 0);
#endif
	return TRUE;
}

INT  KPlayerPurse::GetCurrency(BYTE byCurrencyID)
{
	ASSERT(m_pPurseData);
	return m_pPurseData->GetCurrency(byCurrencyID);
}

INT  KPlayerPurse::GetCoin()
{
	ASSERT(m_pPurseData);
	return m_pPurseData->GetCoin();
}

BOOL KPlayerPurse::SetCoin(INT nCoin, KE_PURSE_RESULT* pResult, BOOL bSync /*= 1*/)
{
	ASSERT(m_pPurseData);
	ASSERT(pResult);

	BOOL bResult = m_pPurseData->SetCoin(nCoin, pResult);
	if (!bResult)
		return FALSE;

#ifdef GAME_SERVER
	m_rcPlayer.m_cPlayerServer.SetAccountCoin(nCoin); // 临时设置金币

	if (bSync)
		SyncCoin();
#else
	m_rcPlayer.FireEvent(emKOBJEVENTTYPE_PURSE_CHANGECOIN, 1, (DWORD)nCoin, 0);
#endif
	return TRUE;
}

//钱
BOOL KPlayerPurse::RegistMoneyType(BYTE byMoneyType)
{
	ASSERT(m_pPurseData);
	BOOL bResult = m_pPurseData->RegistMoney(byMoneyType);
	QCONFIRM_RET_FALSE(bResult);

#ifdef GAME_SERVER
	SyncRegistMoneyType(byMoneyType);
#endif
	return TRUE;
}

BOOL KPlayerPurse::SetMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult, BOOL bSync /* = 1 */)
{
	ASSERT(m_pPurseData);
	BOOL bResult = m_pPurseData->SetMoney(byMoneyType, nMoney, pResult);
	if (!bResult)
		return FALSE;

#ifdef GAME_SERVER
	if (bSync)
		SyncMoney(byMoneyType, nMoney);
#else
	m_rcPlayer.FireEvent(emKOBJEVENTTYPE_PURSE_CHANGEMONEY, (DWORD)byMoneyType, (DWORD)nMoney, 0);
#endif
	return TRUE;
}

INT  KPlayerPurse::GetMoney(BYTE byMoneyType)
{
	ASSERT(m_pPurseData);
	return m_pPurseData->GetMoney(byMoneyType);
}

BOOL KPlayerPurse::OnLogin()
{
#ifdef GAME_SERVER
	DWORD dwCoin = m_rcPlayer.m_cPlayerServer.GetAccountCoin();
	KE_PURSE_RESULT nResult;
	SetCoin((INT)dwCoin, &nResult, TRUE);
#endif
	g_RelayClient.DoPayOrderRequest(m_rcPlayer.GetId());
	return TRUE;
}

BOOL KPlayerPurse::OnLogout()
{
#ifdef GAME_SERVER
	OnSaveBowlFreeCumulativeTimeLogout();
#endif
	return TRUE;
}

BOOL KPlayerPurse::OnSaveBowlFreeCumulativeTimeLogout()
{
	//保存聚宝盆免费获取累计时间
	DWORD dwLastUpdateTime = m_rcPlayer.GetLastUpdateBowlFreeTime();
	DWORD dwFreeCumlativeTime = m_rcPlayer.GetBowlFreeCumulativeTime();
	DWORD dwlastLoginTime = m_rcPlayer.GetLastLoginTime();
	time_t tNow = KSysService::GameTime(NULL);
	dwFreeCumlativeTime += MIN (tNow - dwLastUpdateTime, tNow - dwlastLoginTime);
	m_rcPlayer.SetBowlFreeCumulativeTime(dwFreeCumlativeTime);
	return TRUE;
}

#ifdef GAME_SERVER

BOOL KPlayerPurse::AddCurrency(BYTE byCurrencyID, INT nCurrency, KE_PURSE_RESULT* pResult, BOOL bSync /*= 1*/)
{
	ASSERT(m_pPurseData);
	ASSERT(pResult);
	INT nOldCurrency = m_pPurseData->GetCurrency(byCurrencyID);
	return SetCurrency(byCurrencyID, nOldCurrency + nCurrency, pResult, bSync);
}

BOOL KPlayerPurse::AddCoin(INT nCoin, KE_PURSE_RESULT* pResult, BOOL bSync /*= 1*/)
{
	ASSERT(m_pPurseData);
	ASSERT(pResult);

	INT nOldCoin = m_pPurseData->GetCoin();
	return SetCoin(nOldCoin + nCoin, pResult, bSync);
}

BOOL KPlayerPurse::AddMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult, BOOL bSync /*= 1*/)
{
	
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	ASSERT(m_pPurseData);
	ASSERT(pResult);
	INT nOldMoney = m_pPurseData->GetMoney(byMoneyType);
	bRetCode = SetMoney(byMoneyType, nOldMoney + nMoney, pResult, bSync);
	LOG_PROCESS_ERROR(bRetCode);
	bResult = TRUE;
EXIT0:
	return bResult;
}


BOOL KPlayerPurse::LogMoney(BYTE byMoneyType, INT nMoney,INT nActionType, KE_PURSE_RESULT* pResult, BOOL bSync /*= 1*/,char *str )
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	ASSERT(m_pPurseData);
	ASSERT(pResult);
	INT nOldMoney = m_pPurseData->GetMoney(byMoneyType);
	DWORD dwRoleID =  m_rcPlayer.GetPlayerId();
	PROCESS_ERROR(str);
	//g_pGameCenter->m_LogServerAgency.LogConsumeAction(nActionType,dwRoleID,nMoney,"LogCoinConsume");
	if (strlen(str) < 1 )
	{
		SyncInfoToC(nActionType,dwRoleID,nMoney,"LogCoinConsume"); 
	}
	else
	{
		SyncInfoToC(nActionType,dwRoleID,nMoney,str); 
	}
	bResult = TRUE;
EXIT0:
	return bResult;

}





BOOL KPlayerPurse::SyncCoin()
{
	S2C_PURSE_SYNC_COIN sync;
	sync.nJbCoin = m_pPurseData->GetCoin();
	return GetPurseModule().GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sync);
}


BOOL KPlayerPurse::SyncMoney( BYTE byMoneyType, INT nMoney )
{
	S2C_PURSE_SYNC_MONEY sync;
	sync.byMoneyType = byMoneyType;
	sync.nMoney      = nMoney;
	return GetPurseModule().GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sync);
}

BOOL KPlayerPurse::SyncRegistMoneyType(BYTE byMoneyType)
{
	S2C_PURSE_SYNC_REGIST_MONEYTYPE sync;
	sync.byMoneyType = byMoneyType;
	return GetPurseModule().GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sync);
}

BOOL KPlayerPurse::SyncCurrency( BYTE byCurrencyID, INT nCurrency )
{
	S2C_PURSE_SYNC_CURRENCY sync;
	sync.byCurrencyType = byCurrencyID;
	sync.nCurrency = nCurrency;

	KPlayerProtocolProcessor* pProcessor = GetPurseModule().GetProtocolProcessor();
	QCONFIRM_RET_FALSE(NULL != pProcessor);	
	return pProcessor->SendData(m_rcPlayer.GetPlayerIndex(), sync);
}

BOOL KPlayerPurse::SyncRegistCurrencyType(BYTE byCurrencyID)
{
	S2C_PURSE_SYNC_REGIST_CURRENCYTYPE sync;
	sync.byCurrencyType = byCurrencyID;
	return GetPurseModule().GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sync);
}

BOOL KPlayerPurse::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	return OnLoadPurseData(pData, uSize);
}

BOOL KPlayerPurse::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	return OnSavePurseData(pBuffer, uBuffSize, uUsedSize);
}

BOOL KPlayerPurse::OnClientReady()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	BOOL bRetCode = cSafeScript->CallTableFunction(
		"Purse", 
		"Init", 
		1, 
		"o",
		m_rcPlayer.GetScriptInterface());
	if (!bRetCode)
	{
		QLogPrintf(LOG_ERR, "[Purse] Register CurrencyType Failure!\n");
		return FALSE;
	}
	QLogPrintf(LOG_INFO, "[Purse] Script Purse::Init Success.\n");
	return TRUE;
}

BOOL KPlayerPurse::SyncBowlData(INT nRefresh)
{
	//调用脚本系统初始化
	ScriptSafe cSafeScript1 = g_cScriptManager.GetSafeScript();
	ScriptSafe cSafeScript2 = g_cScriptManager.GetSafeScript();
	ScriptSafe cSafeScript3 = g_cScriptManager.GetSafeScript();
	ScriptSafe cSafeScript4 = g_cScriptManager.GetSafeScript();
	CONST QAssetAward* pAssetAward = NULL;
	S2C_PURSE_SYNC_AL_CHEMY sendinfo;
	sendinfo.nNeedCoin = 0;
	DWORD nFreeNow = 0;
	DWORD nCoinNow = 0;
	DWORD nMaxFree = 0;
	DWORD nMaxCoin = 0;
	INT nCurrenStage = m_rcPlayer.GetVipLevel();
	CONST QNumericalAward* pNumericalAward = NULL;

	pAssetAward = g_cAwardSetting.RequestAssetAward(ZHENQI_CHANGE_MODULE_NAME, m_rcPlayer.GetLevel());
	QCONFIRM_RET_FALSE(pAssetAward);
	sendinfo.nChangeEnergy = pAssetAward->nEnergy;
	
	pAssetAward = g_cAwardSetting.RequestAssetAward(MONEY_CHANGE_MODULE_NAME, m_rcPlayer.GetLevel());
	QCONFIRM_RET_FALSE(pAssetAward);
	sendinfo.nChangeMoney = pAssetAward->nSilver;

	if (nCurrenStage > 0)
	{
		nMoneyTimes = g_cAwardSetting.RequestMaxLineValue(COIN_COST_MODULE_NAME, nCurrenStage);
		nEnergyTimes = g_cAwardSetting.RequestMaxLineValue(COIN_COST_MODULE_NAME, nCurrenStage);
	}

	cSafeScript1->CallTableFunction("Player" , "GetDegreeValue", 1, "os", m_rcPlayer.GetScriptInterface(), "AlFreeTimes");
	nFreeNow = cSafeScript1->GetInt(-1);
	cSafeScript2->CallTableFunction("Player" , "GetDegreeValue", 1, "os", m_rcPlayer.GetScriptInterface(), "AlCoinTimes");
	nCoinNow = cSafeScript2->GetInt(-1);
	cSafeScript3->CallTableFunction("Player" , "GetMaxDegreeValue", 1, "os", m_rcPlayer.GetScriptInterface(), "AlFreeTimes");
	nMaxFree = cSafeScript3->GetInt(-1);
	cSafeScript4->CallTableFunction("Player" , "GetMaxDegreeValue", 1, "os", m_rcPlayer.GetScriptInterface(), "AlCoinTimes");
	nMaxCoin = cSafeScript4->GetInt(-1);
	
	nFreeNow = nMaxFree - nFreeNow;
	nCoinNow = nMaxCoin - nCoinNow;

	if (nCurrenStage > 0)
	{
		pNumericalAward = g_cAwardSetting.RequestNumericalAward(COIN_COST_MODULE_NAME, nCurrenStage, nCoinNow + 1);
		if (pNumericalAward == NULL)
		{
			CONST QNumericalAward *pNumericalCoinAward = g_cAwardSetting.RequestNumericalAward(COIN_COST_MODULE_NAME, nCurrenStage, nCoinNow );
			if (pNumericalCoinAward != NULL)
			{
				sendinfo.nNeedCoin = pNumericalCoinAward->nCost;
			}
			else
			{
				sendinfo.nNeedCoin = 0;
			}
		}
		else
		{
			sendinfo.nNeedCoin = pNumericalAward->nCost;
		}
		sendinfo.nNeedCoinTen = g_cAwardSetting.RequestAwardSum(COIN_COST_MODULE_NAME, nCurrenStage, nCoinNow + 1, 10);
	}

	sendinfo.nLeaveCoinTimes = nMaxCoin - nCoinNow;//nEnergyTimes - nCoinNow - 1;

	DWORD dwLastUpdateTime = m_rcPlayer.GetLastUpdateBowlFreeTime();
	DWORD dwFreeCumlativeTime = m_rcPlayer.GetBowlFreeCumulativeTime();
	DWORD dwlastLoginTime = m_rcPlayer.GetLastLoginTime();
	TIME_T tNow = KSysService::GameTime(NULL);
	DWORD dwTimeSpan = g_cPursesetting.RequestTimeSpan(nFreeNow);
	//当前登陆没领取过
	if (dwFreeCumlativeTime > 0)
	{
		DWORD dwOnlineCumlativeTime = dwFreeCumlativeTime + tNow - dwlastLoginTime;//在线累计时间
		sendinfo.nfreeTimeSpan = dwTimeSpan > dwOnlineCumlativeTime ? dwTimeSpan - dwOnlineCumlativeTime : 0;
	}
	//当前登陆已经领取过
	else
	{
		DWORD dwOnlineCumlativeTime = tNow - dwLastUpdateTime;//在线累计时间
		sendinfo.nfreeTimeSpan = dwTimeSpan > dwOnlineCumlativeTime ? dwTimeSpan - dwOnlineCumlativeTime : 0;
	}
	
	if(nRefresh == emFree)
	{
		m_rcPlayer.SetLastUpdateBowlFreeTime(tNow);
		if (dwFreeCumlativeTime > 0)
			m_rcPlayer.SetBowlFreeCumulativeTime(0);
		sendinfo.nfreeTimeSpan = g_cPursesetting.RequestTimeSpan(nFreeNow);
	}

	KPlayerProtocolProcessor* pProcessor = GetPurseModule().GetProtocolProcessor();
	QCONFIRM_RET_FALSE(NULL != pProcessor);

	pProcessor->SendData(m_rcPlayer.GetPlayerIndex(), sendinfo);

	return TRUE;
}

BOOL KPlayerPurse::OnSavePurseData(LPBYTE& pBuffer, 
								   SIZE_T uBuffSize, 
								   SIZE_T& uUsedSize)
{
	//存放数据: 
	//1. {珍币个数, 钱个数}
	//2.存珍币
	//{BYTE, nCurrency}, {BYTE, byCurrency},....
	//3.存钱
	//{BYTE, nMoney}, {BYTE, nMoney}, .....
	
	//存珍币
	KPurseData::MPCURRENCYS* pCurrencyList = m_pPurseData->GetCurrencyList();
	QCONFIRM_RET_FALSE(NULL != pCurrencyList);
	DWORD dwCurrencySize = pCurrencyList->size();

	//存钱
	KPurseData::MPMONEYS* pMoneyList = m_pPurseData->GetMoneyList();
	QCONFIRM_RET_FALSE(NULL != pMoneyList);
	DWORD dwMoneySize = pMoneyList->size();
	
	//先判断一下uBuffSize空间是否够存
	SIZE_T uWillSaveTotal = 2 * sizeof(DWORD) + 
							(sizeof(BYTE) + sizeof(INT)) * dwCurrencySize + 
							(sizeof(BYTE) + sizeof(INT)) * dwMoneySize;
						 
	if (uBuffSize < uWillSaveTotal)
	{
		QLogPrintf(LOG_ERR, "[Purse]Player Purse Save Failed, because Not Enough Buffer size.\n");
		return FALSE;
	}
	
	*((DWORD*)pBuffer) = dwCurrencySize;
	pBuffer += sizeof(DWORD);
	uUsedSize += sizeof(DWORD);
	
	*((DWORD*)pBuffer) = dwMoneySize;
	pBuffer += sizeof(DWORD);
	uUsedSize += sizeof(DWORD);
	
	KPurseData::MPCURRENCYS::iterator it;
	for (it = pCurrencyList->begin(); it != pCurrencyList->end(); it++)
	{
		*((BYTE*)pBuffer) = it->first;
		pBuffer   += sizeof(BYTE);
		uUsedSize += sizeof(BYTE);

		*((INT*)pBuffer) = it->second;
		pBuffer   += sizeof(INT);
		uUsedSize += sizeof(INT);
	}

	KPurseData::MPMONEYS::iterator iter;
	for (iter = pMoneyList->begin(); iter != pMoneyList->end(); iter++)
	{
		*((BYTE*)pBuffer) = iter->first;
		pBuffer   += sizeof(BYTE);
		uUsedSize += sizeof(BYTE);

		*((INT*)pBuffer) = iter->second;
		pBuffer   += sizeof(INT);
		uUsedSize += sizeof(INT);
	}

	return TRUE;
}




BOOL KPlayerPurse::OnLoadPurseData(LPCBYTE pData, SIZE_T uSize)
{
	//没有数据，不加载
	if (uSize == 0)
		return TRUE;

	QCONFIRM_RET_FALSE(NULL != pData);

	SIZE_T uLoadBuffSize  = 0;
	DWORD dwCurrencySize = 0;
	DWORD dwMoneySize    = 0;

	QCONFIRM_RET_FALSE(uSize >= 2 * sizeof(DWORD))
	
	dwCurrencySize = *((DWORD*)pData);
	pData += sizeof(DWORD);
	uLoadBuffSize += sizeof(DWORD);

	dwMoneySize = *((DWORD*)pData);
	pData += sizeof(DWORD);
	uLoadBuffSize += sizeof(DWORD);

	while (uLoadBuffSize < uSize)
	{
		for (DWORD i = 0; i < dwCurrencySize; i++)
		{
			BYTE byCurrencyID = *((BYTE*)pData);
			pData += sizeof(BYTE);
			uLoadBuffSize += sizeof(BYTE);

			INT nCurrency = *((INT*)pData);
			pData += sizeof(INT);
			uLoadBuffSize += sizeof(INT);

			KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
			//先注册，再加入
			BOOL bResult = RegistCurrencyType(byCurrencyID);
			QCONFIRM_RET_FALSE(bResult);
			bResult = AddCurrency(byCurrencyID, nCurrency, &nResult, 1);
			QCONFIRM_RET_FALSE(bResult);
		}

		for (DWORD j = 0; j < dwMoneySize; j++)
		{
			BYTE byMoneyID = *((BYTE*)pData);
			pData += sizeof(BYTE);
			uLoadBuffSize += sizeof(BYTE);

			INT nMoney = *((INT*)pData);
			pData += sizeof(INT);
			uLoadBuffSize += sizeof(INT);

			KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
			BOOL bResult = RegistMoneyType(byMoneyID);
			QCONFIRM_RET_FALSE(bResult);
			bResult = AddMoney(byMoneyID, nMoney, &nResult, 1);
			QCONFIRM_RET_FALSE(bResult);
		}
	}
	
	return TRUE;
}

BOOL KPlayerPurse::SyncInfoToC( INT nActionType,DWORD dwRoleID,INT number,const char* pszComment )
{ 
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_LOG_INFO_SAVE_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;
	INT                         nRetLen = 0;

	piPackage = QCreateMemBlock(sizeof(S2R_LOG_INFO_SAVE_REQUEST));
	pRequest = (S2R_LOG_INFO_SAVE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);


	pRequest->wProtocolID = s2r_log_info_save_request;
	pRequest->nActionType = nActionType;
	pRequest->dwRoleID = dwRoleID;
	pRequest->number = number;
	//memcpy(pRequest->pszComment,pszComment,strlen(pszComment)+1);
	nRetLen = snprintf(pRequest->pszComment,sizeof(pRequest->pszComment),"%s",pszComment);
	LOG_PROCESS_ERROR(nRetLen > 0 &&  nRetLen < sizeof(pRequest->pszComment));
	
	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);
	
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}



#endif


IKPlayerPurse* GetPlayerPurse(KPlayer* pPlayer)
{
	QCONFIRM_RET_NULL(NULL != pPlayer);
	KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	return static_cast<IKPlayerPurse*>(pPlayerPurse);
}




// -------------------------------------------------------------------------
