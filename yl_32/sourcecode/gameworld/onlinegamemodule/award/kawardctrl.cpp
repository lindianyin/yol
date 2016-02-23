/* -------------------------------------------------------------------------
//	文件名		：	kawardctrl.cpp
//	创建者		：	zhaoyu
//	创建时间	：	2013-8-12
//	功能描述	：	奖励管理
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kawardctrl.h"
#include "kawardprotocol.h"
#include "kawardmodule.h"
#include "kawardmanager.h"
#include "onlinegameworld/kplayer.h"
#include "kaward.h"
#include "onlinegamebase/ksysservice.h"

KAwardCtrl::KAwardCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KAwardCtrl::~KAwardCtrl()
{

}

BOOL KAwardCtrl::Init()
{
	return TRUE;
}

BOOL KAwardCtrl::UnInit()
{
	m_arrAwards.clear();

	return TRUE;
}

BOOL KAwardCtrl::OnLogout()
{
	for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
	{
		QCONFIRM_RET_FALSE(g_cAwardMgr.RemoveAward((*it)->GetId()));
	}

	return TRUE;
}

BOOL KAwardCtrl::Add(KAward* pAward, BOOL bSync /* = TRUE */)
{
	m_arrAwards.push_back(pAward);

	if(bSync)
	{
		SyncAwardInfo(pAward);
	}

	// 直接领取不弹面板
	if(pAward->m_bAutoGive)
	{
		TakeAward(pAward->GetId(),NULL);
		goto EXIT0;
	}

EXIT0:
	return TRUE;
}

BOOL KAwardCtrl::SyncAwardInfo(KAward* pAward)
{
	BOOL					bRetCode		= FALSE;
	BOOL					bResult			= FALSE;
	IMemBlock*				piAwardSync		= NULL;
	size_t					uPakSize		= 0;
	KS2C_SYNC_AWARD*		pSync			= NULL;

	uPakSize = sizeof(KS2C_SYNC_AWARD) + pAward->m_nItemNum * sizeof(KAwardItem);
	piAwardSync = QCreateMemBlock(uPakSize);

	pSync = (KS2C_SYNC_AWARD*)piAwardSync->GetData();

	pSync->byProtocol = s2c_sync_award;
	pSync->dwId = pAward->GetId();
	pSync->nRankIdx = pAward->m_nRankIdx;

	strncpy(pSync->szModuleName, pAward->m_szModuleName, sizeof(pAward->m_szModuleName));
	pSync->nLevel = pAward->m_nLevel;

	pSync->nEnergy = pAward->m_nEnergy;
	pSync->nSilver = pAward->m_nSilver;
	pSync->nFightingSpirit = pAward->m_nFightingSpirit;
    pSync->nPrestige = pAward->m_nPrestige;

	pSync->nItemNum = pAward->m_nItemNum;

	memcpy((VOID*)pSync->Items, pAward->m_arrItems, pAward->m_nItemNum * sizeof(KAwardItem));
	
	bRetCode = g_cAwardModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);
	QLogPrintf(LOG_INFO,"..KAwardCtrl::SyncAwardInfo pAward->GetModuleName()=%s pAward->GetId()=%d pAward->GetIndex()=%d", pAward->GetModuleName()
				,pAward->GetId(),pAward->GetIndex());
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piAwardSync);
	return bResult;
}

BOOL KAwardCtrl::DoTakeAwardRespond(DWORD dwId, BOOL bSucceed)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	KS2C_TAKE_AWARD_RESPOND sSync;

	sSync.byProtocol = s2c_take_award_respond;
	sSync.dwId = dwId;
	sSync.bSucceed = (BYTE)bSucceed;

	bRetCode = g_cAwardModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KAwardCtrl::SaveAwardData(CONST KAward* pAward, KAwardSaveData* pSaveData)
{
	strncpy(pSaveData->szModuleName, pAward->m_szModuleName, sizeof(pAward->m_szModuleName));
	pSaveData->nLevel = pAward->m_nLevel;

	pSaveData->dwOutdateTime = (DWORD)pAward->m_tOutdateTime;
	pSaveData->nRankIdx = pAward->m_nRankIdx;

	pSaveData->nEnergy = pAward->m_nEnergy;
	pSaveData->nFightingSpirit = pAward->m_nFightingSpirit;
	pSaveData->nSilver = pAward->m_nSilver;

	pSaveData->nItemNum = pAward->m_nItemNum;

	memcpy((VOID*)pSaveData->Items, pAward->m_arrItems, pAward->m_nItemNum * sizeof(KAwardItem));

	return TRUE;
}

BOOL KAwardCtrl::LoadAwardData(KAward* pAward, KAwardSaveData* pSaveData)
{
	strncpy(pAward->m_szModuleName, pSaveData->szModuleName, sizeof(pSaveData->szModuleName));
	pAward->m_nLevel = pSaveData->nLevel;

	pAward->m_tOutdateTime = (time_t)pSaveData->dwOutdateTime;
	pAward->m_nRankIdx = pSaveData->nRankIdx;

	pAward->m_nEnergy = pSaveData->nEnergy;
	pAward->m_nFightingSpirit = pSaveData->nFightingSpirit;
	pAward->m_nSilver = pSaveData->nSilver;
	pAward->m_nItemNum = pSaveData->nItemNum;

	ZeroMemory(pAward->m_arrItems, sizeof(pAward->m_arrItems));

	memcpy((VOID*)pAward->m_arrItems, pSaveData->Items, pAward->m_nItemNum * sizeof(KAwardItem));

	return TRUE;
}

BOOL KAwardCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	KAwardSaveData* pSaveData = NULL;
	time_t tNow = KSysService::Time(NULL);
	size_t uSize = 0;

	for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
	{
		KAward* pAward = *it;
		// 不存盘或者奖励已过期
		if(!pAward->m_bSave || (pAward->m_tOutdateTime > 0 && tNow > pAward->m_tOutdateTime))
			continue;

		pSaveData = (KAwardSaveData*)pBuffer;

		SaveAwardData(pAward, pSaveData);

		uSize = sizeof(KAwardSaveData) + sizeof(KAwardItem) * pAward->m_nItemNum;

		pBuffer += uSize;
		uUsedSize += uSize;
	}

	return TRUE;
}

BOOL KAwardCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	size_t uUsedSize = 0;

	if (uSize == 0)
		return TRUE;

	if (pData == NULL || uSize < sizeof(KAwardSaveData))
	{
		QLogPrintf(LOG_ERR, "[KAwardCtrl::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(KAwardSaveData");
		return FALSE;
	}

	INT nIdx = 0;
	SIZE_T uLoadSize = 0;
	while(uLoadSize < uSize)
	{
		KAwardSaveData* pAwardData = (KAwardSaveData*)pData;
		CONST QAssetAward* pAssetAward = NULL;

		INT nIndex = g_cAwardMgr.AddAward();
		QCONFIRM_RET_FALSE(nIndex >= 0);

		KAward* pAward = g_cAwardMgr.GetAwardByIndex(nIndex);
		QCONFIRM_RET_FALSE(pAward);

		LoadAwardData(pAward, pAwardData);

		pAssetAward = g_cAwardSetting.RequestAssetAward(pAward->m_szModuleName, pAward->m_nLevel);
		QCONFIRM_RET_FALSE(pAssetAward);

		pAward->m_bAutoGive = pAssetAward->nAutoGive;
		pAward->m_bSync = pAssetAward->nSync;
		pAward->m_bSave = pAssetAward->nSave;

		BOOL bRet = Add(pAward, pAward->IsSync());
		QCONFIRM_RET_FALSE(bRet);

		uUsedSize = sizeof(KAwardSaveData) + sizeof(KAwardItem) * pAward->m_nItemNum;

		pData += uUsedSize;
		uLoadSize += uUsedSize;
	}

	return TRUE;
}

BOOL KAwardCtrl::TakeAward(DWORD dwId,const CHAR *szModuleName)
{
	INT nRet = 0;
	DWORD dwRemoveId = 0;
	KAward* pAward = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
	{
		pAward = *it;
		if(pAward->GetId() != dwId)
		{
			continue;
		}

		if(NULL != szModuleName && 0 != strcmp(szModuleName,"") && 0 != strcmp(pAward->GetModuleName(),szModuleName))
		{
			continue;
		}
		
		cSafeScript->CallTableFunction("Reward", "TakeAward", 1, "od", m_rcPlayer.GetScriptInterface(), pAward->GetId());
		nRet = cSafeScript->GetInt(-1);
		
		if(nRet)
		{
			g_cAwardMgr.RemoveAward(dwId);
			dwRemoveId = dwId;
		}

		DoTakeAwardRespond(dwId, nRet);
		break;
	}

	for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
	{
		pAward = *it;
		if(pAward->GetId() != dwRemoveId)
			continue;
		m_arrAwards.erase(it);
		break;
	}

	return TRUE;
}

BOOL KAwardCtrl::DelAward(DWORD dwId /* = 0 */)
{
	KAward* pAward = NULL;

	if(!dwId)
	{
		m_arrAwards.clear();
	}
	else
	{
		for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
		{
			pAward = *it;
			if(pAward->GetId() == dwId)
			{
				g_cAwardMgr.RemoveAward(dwId);
				m_arrAwards.erase(it);
				break;
			}
		}
	}

	return TRUE;
}

BOOL KAwardCtrl::GetPlayerAwards(std::vector<DWORD>& arrAwards, LPCTSTR szModuleName /* = NULL */)
{
	arrAwards.clear();
	KAward* pAward = NULL;

	for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
	{
		pAward = *it;
		if(szModuleName != NULL)
		{
			if(strcmp(pAward->GetModuleName(), szModuleName) != 0)
				continue;
		}

		arrAwards.push_back(pAward->GetId());
	}

	return TRUE;
}

BOOL KAwardCtrl::TakeAllAward(LPCSTR szModuleName /* = NULL */)
{
	KAward* pAward = NULL;
	std::vector<DWORD> arrAwardIds;

	for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
	{
		pAward = *it;
		if(szModuleName != NULL)
		{
			if(strcmp(pAward->GetModuleName(), szModuleName) != 0)
				continue;
		}
		arrAwardIds.push_back(pAward->GetId());
	}

	for(std::vector<DWORD>::iterator it = arrAwardIds.begin(); it != arrAwardIds.end(); ++it)
	{
		QCONFIRM_RET_FALSE(TakeAward(*it,NULL));
	}

	return TRUE;
}

BOOL KAwardCtrl::GetAwardListByModuleName(std::vector<DWORD>& vecAwardIds, LPCHAR szModuleName)
{
	KAward* pAward = NULL;
	vecAwardIds.clear();

	for(std::vector<KAward*>::iterator it = m_arrAwards.begin(); it != m_arrAwards.end(); ++it)
	{
		pAward = *it;
		QCONFIRM_RET_FALSE(pAward);

		if(strcmp(szModuleName, pAward->m_szModuleName) == 0)
		{
			vecAwardIds.push_back(pAward->GetId());
		}
	}

	return TRUE;
}