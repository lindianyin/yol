#include "stdafx.h"
#include "playerbank.h"
#include "onlinegamebase/ksysservice.h"
#include "bankmodule.h"
#include "bankprotocol.h"
#include "item2/kitemmgr.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "bankmanager.h"
#include "record/RecordRelayClient.h"
#include "banksetting.h"
#include "Protocol/KG_CSLogServerProtocal.h"
//#include "../gameworld/nsworld/kcharacterhelper.h"


PlayerBank::PlayerBank(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerBank::~PlayerBank()
{

}

BOOL PlayerBank::Init()
{
	//m_pLunaObj = new LuaPlayerGem(m_rcCharacter);
	m_vecSaveData.clear();
	return TRUE;
}

BOOL PlayerBank::UnInit()
{
	//SAFE_DELETE(m_pLunaObj);
	m_vecSaveData.clear();
	return TRUE;
}

BOOL PlayerBank::Active()
{
	// 每秒检测一次
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;

	return TRUE;
}


BOOL PlayerBank::OnLogin()
{
	SyncAllInvestInfo();
	return TRUE;
}

BOOL PlayerBank::OnLogout()
{
	return TRUE;
}


BOOL PlayerBank::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	LPCBYTE pBuffer = pData;
	SIZE_T uUsedSize = uSize;

	if (uSize == 0)
		return TRUE;

	if (pData == NULL)
	{
		QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(BufferSaveData)");
		return FALSE;
	}

	while(uUsedSize > 0)
	{
		/*KITEM_INDEX sItem;
		memcpy(&sItem, pBuffer, sizeof(KITEM_INDEX));
		pBuffer += sizeof(KITEM_INDEX);
		uUsedSize -= sizeof(KITEM_INDEX);*/

		BankSaveData sSavedata;
		memcpy(&sSavedata, pBuffer, sizeof(BankSaveData));
		pBuffer += sizeof(BankSaveData);
		uUsedSize -= sizeof(BankSaveData);

		m_vecSaveData.push_back(sSavedata);
	}

	SyncMyInvestInfo();

EXIT0:
	return TRUE;
}

BOOL PlayerBank::OnDbSave(LPBYTE pData, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	LPBYTE pBuffer = pData;
	LOG_PROCESS_ERROR(pBuffer);

	for (std::vector<BankSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		/*memcpy(pBuffer, &(it->first), sizeof(KITEM_INDEX));
		pBuffer += sizeof(KITEM_INDEX);
		uUsedSize += sizeof(KITEM_INDEX);*/

		memcpy(pBuffer, &(*it), sizeof(BankSaveData));
		pBuffer += sizeof(BankSaveData);
		uUsedSize += sizeof(BankSaveData);
	}


EXIT0:
	return TRUE;
}

BOOL PlayerBank::DoGetIncomeRequest(INT nId)
{
	QLogPrintf(LOG_INFO,"..PlayerBank::DoGetIncomeRequest(INT nId=%d)",nId);
	BankSaveData* pSaveData = GetInvestData(nId);
	BankSaveData* pLastGetInvest = GetLastSaveData(nId);
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	DWORD tNow = KSysService::GameTime(NULL) + 8 * 3600;
	if (pSaveData == NULL || pLastGetInvest == NULL)
		return FALSE;

	if (pLastGetInvest->nCoins == 0 && tNow - pLastGetInvest->dwTime + pLastGetInvest->dwTime % 86400 <= 86400)
		return FALSE;

	INT nTotalDay = 0;
	INT nRate = 0;
	INT nDay = (tNow - pSaveData->dwTime + pSaveData->dwTime % 86400) / 86400;

	if (nId == BNAK_HONOR_ID)
	{
		nTotalDay = BNAK_HONOR_DAYS;
		nRate = g_cBankSetting.GetHonorRate(nDay+1);
	}
	else
	{
		nTotalDay = BNAK_NORMAL;
		nRate = g_cBankSetting.GetNoramlRate(nDay+1);
	}

	if (tNow > pSaveData->dwTime + nTotalDay * 86400)
		return FALSE;

	KPlayerPurse* pPlayerPurse = m_rcPlayer.GetCtrl<KPlayerPurse>();
	QCONFIRM_RET_FALSE(pPlayerPurse);
	pPlayerPurse->AddMoney(EM_MONEY_COIN, pSaveData->nCoins * nRate / 100, &nResult);

	BankSaveData sSaveData;
	sSaveData.nId = nId;
	sSaveData.nCoins = 0;
	sSaveData.nGetCoins = pSaveData->nCoins * nRate / 100;
	sSaveData.dwTime = tNow;

	m_vecSaveData.push_back(sSaveData);
	SyncMyInvestInfo();
EXIT0:
	return TRUE;
}

BOOL PlayerBank::DoInvestRequest(INT nConis, INT nId)
{
	QLogPrintf(LOG_INFO,"PlayerBank::DoInvestRequest(INT nConis=%d, INT nId=%d)",nConis,nId);
	KPlayerPurse* pPlayerPurse = NULL;
	BankSaveData* pSaveData = NULL;
	TIME_T tNow = 0;
	TIME_T tLastTime = 0;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	CONST KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	QCONFIRM_RET_FALSE(pConfig);

	tNow = KSysService::GameTime(NULL) + 8 * 3600;
	tLastTime = pConfig->dwStartSeverTime + 8 * 3600 + 7 * 86400; //结束时间
	QCONFIRM_RET_FALSE(tNow <= tLastTime);

	pSaveData = GetInvestData(nId);
	QCONFIRM_RET_FALSE(pSaveData == NULL);
	pPlayerPurse = m_rcPlayer.GetCtrl<KPlayerPurse>();
	QCONFIRM_RET_FALSE(pPlayerPurse || pPlayerPurse->GetMoney(EM_MONEY_COIN) >= nConis);
	pPlayerPurse->AddMoney(EM_MONEY_COIN, nConis * (-1), &nResult);
	pPlayerPurse->LogMoney(EM_MONEY_COIN,nConis * (-1),121,&nResult);
	BankSaveData sSaveData;
	sSaveData.nId = nId;
	sSaveData.nCoins = nConis;
	sSaveData.nGetCoins = 0;
	sSaveData.dwTime = tNow;

	m_vecSaveData.push_back(sSaveData);

	SyncMyInvestInfo();

	RecordRelayClient::DoSetBankInvestRequest(nConis, m_rcPlayer.GetId());
EXIT0:
	return TRUE;
}

BOOL PlayerBank::SyncAllInvestInfo()
{
	IMemBlock* piPackage					= NULL;
	KS2C_ALL_INVEST_INFO* ptcInfo			= NULL;
	BOOL bOk								= FALSE;
	CONST std::vector<KBankInvestRecord> vecBankInvestRecord = g_cBankManager.GetBankInvestRecord();
	INT nInx								= vecBankInvestRecord.size();
	piPackage = QCreateMemBlock(sizeof(KS2C_ALL_INVEST_INFO)+sizeof(KAllInvestInfo) * nInx);
	ptcInfo = (KS2C_ALL_INVEST_INFO*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcInfo);

	ptcInfo->byProtocol					= (BYTE)s2c_sync_all_invest_info;
	ptcInfo->nCount						= nInx;

	nInx = 0;
	for (std::vector<KBankInvestRecord>::const_iterator it = vecBankInvestRecord.begin(); it != vecBankInvestRecord.end(); ++it)
	{
		ptcInfo->arrInfo[nInx].nCoins = it->nCoins;
		ptcInfo->arrInfo[nInx].dwTime = it->dwTimestamp;
		QStrCpyLen(ptcInfo->arrInfo[nInx].szName, it->szRoldName, sizeof(ptcInfo->arrInfo[nInx].szName));
		nInx++;

	}

	bOk =  g_cBankModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcInfo, sizeof(KS2C_ALL_INVEST_INFO)+sizeof(KAllInvestInfo) * nInx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

BOOL PlayerBank::SyncMyInvestInfo()
{
	IMemBlock* piPackage					= NULL;
	KS2C_MY_INVEST_INFO* ptcInfo			= NULL;
	INT nInx								= m_vecSaveData.size();
	BOOL bOk								= FALSE;
	piPackage = QCreateMemBlock(sizeof(KS2C_MY_INVEST_INFO)+sizeof(KMyInvestInfo) * nInx);
	ptcInfo = (KS2C_MY_INVEST_INFO*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcInfo);

	ptcInfo->byProtocol					= (BYTE)s2c_sync_my_invest_info;
	ptcInfo->nCount						= nInx;

	nInx = 0;
	for (std::vector<BankSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		ptcInfo->arrInfo[nInx].nId = it->nId;
		ptcInfo->arrInfo[nInx].nCoins = it->nCoins;
		ptcInfo->arrInfo[nInx].nGetCoins = it->nGetCoins;
		ptcInfo->arrInfo[nInx].dwTime = it->dwTime;
		nInx++;

	}

	bOk =  g_cBankModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcInfo, sizeof(KS2C_MY_INVEST_INFO)+sizeof(KMyInvestInfo) * nInx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

//LuaPlayerGem* PlayerGem::GetScriptInterface() CONST
//{
//	return m_pLunaObj;
//}

BankSaveData* PlayerBank::GetLastSaveData(INT nId)
{
	BOOL bGet = FALSE;
	BankSaveData* pLastInvest = NULL;
	for (std::vector<BankSaveData>::reverse_iterator it = m_vecSaveData.rbegin(); it != m_vecSaveData.rend(); ++it)
	{
		if (nId == it->nId)
		{
			//投资
			if (it->nCoins > 0)
			{
				if (bGet == FALSE)
				{
					pLastInvest = &(*it);
					break;
				}
			}
			//获取
			else
			{
				if (bGet == FALSE)
				{
					TIME_T tNow = KSysService::GameTime(NULL) + 8 * 3600;
					tm *pNowTime = ::localtime(&tNow);
					INT nYear = pNowTime->tm_year;
					INT nDay = pNowTime->tm_yday;
					TIME_T tLastTime = it->dwTime;
					tm *pLastTime = ::localtime(&tLastTime);

					if (nYear != pLastTime->tm_year || nDay > pLastTime->tm_yday)
					{
						pLastInvest = &(*it);
						break;
					}
				}
				bGet = TRUE;
			}
		}
	}
	return pLastInvest;
}

BankSaveData* PlayerBank::GetInvestData(INT nId)
{
	BankSaveData* pLastInvest = NULL;
	for (std::vector<BankSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (nId == it->nId && it->nCoins > 0)
		{
			pLastInvest = &(*it);
			break;
		}
	}

	return pLastInvest;

}

//获取每日投宝收益(测试用)
//str: 2014-08-02 12:12:12
BOOL PlayerBank::DoGetIncomeTest(INT nId, CHAR* str)
{
	BankSaveData* pSaveData = GetInvestData(nId);
	BankSaveData* pLastGetInvest = GetLastSaveDataTest(nId, str);
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	DWORD tNow = KSysService::StringToDateTime(str) + 8 * 3600;
	if (pSaveData == NULL || pLastGetInvest == NULL)
		return FALSE;

	if (pLastGetInvest->nCoins == 0 && tNow - pLastGetInvest->dwTime + pLastGetInvest->dwTime % 86400 <= 86400)
		return FALSE;

	INT nTotalDay = 0;
	INT nRate = 0;
	INT nDay = (tNow - pSaveData->dwTime + pSaveData->dwTime % 86400) / 86400;

	if (nId == BNAK_HONOR_ID)
	{
		nTotalDay = BNAK_HONOR_DAYS;
		nRate = g_cBankSetting.GetHonorRate(nDay+1);
	}
	else
	{
		nTotalDay = BNAK_NORMAL;
		nRate = g_cBankSetting.GetNoramlRate(nDay+1);
	}

	if (tNow > pSaveData->dwTime + nTotalDay * 86400)
		return FALSE;

	KPlayerPurse* pPlayerPurse = m_rcPlayer.GetCtrl<KPlayerPurse>();
	QCONFIRM_RET_FALSE(pPlayerPurse);
	pPlayerPurse->AddMoney(EM_MONEY_COIN, pSaveData->nCoins * nRate / 100, &nResult);

	BankSaveData sSaveData;
	sSaveData.nId = nId;
	sSaveData.nCoins = 0;
	sSaveData.nGetCoins = pSaveData->nCoins * nRate / 100;
	sSaveData.dwTime = tNow;

	m_vecSaveData.push_back(sSaveData);


	SyncMyInvestInfo();
EXIT0:
	return TRUE;
}

BankSaveData* PlayerBank::GetLastSaveDataTest(INT nId, CHAR* str)
{
	BOOL bGet = FALSE;
	BankSaveData* pLastInvest = NULL;
	for (std::vector<BankSaveData>::reverse_iterator it = m_vecSaveData.rbegin(); it != m_vecSaveData.rend(); ++it)
	{
		if (nId == it->nId)
		{
			//投资
			if (it->nCoins > 0)
			{
				if (bGet == FALSE)
				{
					pLastInvest = &(*it);
					break;
				}
			}
			//获取
			else
			{
				if (bGet == FALSE)
				{
					TIME_T tNow = KSysService::StringToDateTime(str) + 8 * 3600;
					tm *pNowTime = ::localtime(&tNow);
					INT nYear = pNowTime->tm_year;
					INT nDay = pNowTime->tm_yday;
					TIME_T tLastTime = it->dwTime;
					tm *pLastTime = ::localtime(&tLastTime);

					if (nYear != pLastTime->tm_year || nDay > pLastTime->tm_yday)
					{
						pLastInvest = &(*it);
						break;
					}
				}
				bGet = TRUE;
			}
		}
	}
	return pLastInvest;
}