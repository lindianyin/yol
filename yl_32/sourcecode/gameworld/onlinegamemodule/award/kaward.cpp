
#include "stdafx.h"
#include "awarddef.h"
#include "kaward.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/kplayer.h"

KAward::KAward()
{
	
}

KAward::~KAward()
{
	
}

BOOL KAward::Init(KPlayer* pPlayer, LPCTSTR szModuleName, INT nLevel, INT nTimes /* = 1 */, INT nAwardPercent /* = 100 */)
{
	INT nItemIdx = 0;
	BOOL bResult = FALSE;
	QString strModuleName(szModuleName);

	CONST QAssetAward* pAssetAward = g_cAwardSetting.RequestAssetAward(strModuleName, nLevel);
	LOG_PROCESS_ERROR(pAssetAward);

	strncpy(m_szModuleName, szModuleName, MAX_NAME_LEN);
	m_nLevel = nLevel;

	m_bSave = pAssetAward->nSave;
	m_bSync = pAssetAward->nSync;
	m_bAutoGive = pAssetAward->nAutoGive;
	m_nRankIdx = 0;

	if(pAssetAward->nOverlayDay < 0)
	{
		m_tOutdateTime = -1;
	}
	else
	{
		m_tOutdateTime = KSysService::Time(NULL) + pAssetAward->nOverlayDay * (24 * 3600);
	}

	m_nFightingSpirit = nTimes * pAssetAward->nFightingSpirit * nAwardPercent / 100;
	m_nSilver = nTimes * pAssetAward->nSilver * nAwardPercent / 100;
	m_nEnergy = nTimes * pAssetAward->nEnergy * nAwardPercent / 100;
	m_nPrestige = nTimes * pAssetAward->nPrestige* nAwardPercent / 100;

	m_nExp = 0;
	if(pAssetAward->nExpAwardId > 0)
	{
		INT nLevel = pPlayer->GetLevel();
		//奖励经验不需要百分比
		m_nExp = g_cAwardSetting.GetExpByLevel(pAssetAward->nExpAwardId, nLevel + 1);
		m_nExp = m_nExp * nTimes;
		//m_nExp = nTimes  * nAwardPercent /*/ 100* m_nExp*/;
	}

	ZeroMemory(m_arrItems, sizeof(m_arrItems));

	for(INT i = 0; i < pAssetAward->nItemNum; ++i)
	{
		INT nRand = KSysService::Rand(100) + 1;
		INT nMin = pAssetAward->arrAwardItem[i].nAmountMin;
		INT nMax = pAssetAward->arrAwardItem[i].nAmountMax;
		ASSERT(nMax >= nMin);

		if(nRand >= pAssetAward->arrAwardItem[i].nRate)
			continue;

		m_arrItems[nItemIdx].nGenre = pAssetAward->arrAwardItem[i].sIndex.nGenre;
		m_arrItems[nItemIdx].nDetail = pAssetAward->arrAwardItem[i].sIndex.nDetailType;
		m_arrItems[nItemIdx].nParticular = pAssetAward->arrAwardItem[i].sIndex.nParticular;
		m_arrItems[nItemIdx].nLevel = pAssetAward->arrAwardItem[i].sIndex.nLevel;
		m_arrItems[nItemIdx].nTimeOut = pAssetAward->arrAwardItem[i].nTimeOut;

		if(nMax == nMin)
		{
			m_arrItems[nItemIdx].nAmount = nTimes * nMin * nAwardPercent / 100;
		}
		else
		{
			m_arrItems[nItemIdx].nAmount = nTimes * (KSysService::Rand(nMax - nMin + 1) + nMin) * nAwardPercent / 100;
		}

		nItemIdx++;
	}

	m_nItemNum = nItemIdx;

	bResult = TRUE;

EXIT0:
	return bResult;
}