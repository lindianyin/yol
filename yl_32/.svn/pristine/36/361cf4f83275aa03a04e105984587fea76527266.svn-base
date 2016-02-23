#include "stdafx.h"
#include "playerawardlobby.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/characterbuffer.h"
#include "awardlobbysetting.h"
#include "awardlobbymodule.h"
#include "awardlobbyprotocol.h"
#include "award/kawardmanager.h"
#include "award/awarddef.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
//#include "../gameworld/nsworld/kcharacterhelper.h"


PlayerAwardLobby::PlayerAwardLobby(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerAwardLobby::~PlayerAwardLobby()
{

}

BOOL PlayerAwardLobby::Init()
{
	m_pLunaObj = new LuaPlayerAwardLobby(m_rcCharacter);
	m_nSignedIn = 0;
	m_dwLastSignedInTime = 0;
	m_nLoginDays = 0;
	m_nMaxLoginDays = 0;
	m_dwLastLoginTime = 0;
	m_mapActivity.clear();
	return TRUE;
}

BOOL PlayerAwardLobby::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	return TRUE;
}

BOOL PlayerAwardLobby::Active()
{
	// 每秒检测一次
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;

	return TRUE;
}


BOOL PlayerAwardLobby::OnLogin()
{
	return TRUE;
}

BOOL PlayerAwardLobby::OnLogout()
{
	return TRUE;
}


BOOL PlayerAwardLobby::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
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

	memcpy(&m_nSignedIn, pBuffer, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize -= sizeof(INT);

	memcpy(&m_dwLastSignedInTime, pBuffer, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize -= sizeof(DWORD);

	memcpy(&m_nLoginDays, pBuffer, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize -= sizeof(INT);

	memcpy(&m_nMaxLoginDays, pBuffer, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize -= sizeof(INT);

	memcpy(&m_dwLastLoginTime, pBuffer, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize -= sizeof(DWORD);

	while(uUsedSize > 0)
	{
		AwardLobbyActivity sActivity;
		memcpy(&sActivity, pBuffer, sizeof(AwardLobbyActivity));
		pBuffer += sizeof(AwardLobbyActivity);
		uUsedSize -= sizeof(AwardLobbyActivity);
		m_mapActivity[sActivity.nId] = sActivity;
	}

EXIT0:
	return TRUE;
}

BOOL PlayerAwardLobby::OnDbSave(LPBYTE pData, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	LPBYTE pBuffer = pData;
	LOG_PROCESS_ERROR(pBuffer);

	memcpy(pBuffer, &m_nSignedIn, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	memcpy(pBuffer, &m_dwLastSignedInTime, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize += sizeof(DWORD);

	memcpy(pBuffer, &m_nLoginDays, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	memcpy(pBuffer, &m_nMaxLoginDays, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	memcpy(pBuffer, &m_dwLastLoginTime, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize += sizeof(DWORD);

	for (std::map<INT, AwardLobbyActivity>::iterator it = m_mapActivity.begin(); it != m_mapActivity.end(); ++it)
	{
		memcpy(pBuffer, &(it->second), sizeof(AwardLobbyActivity));

		pBuffer += sizeof(AwardLobbyActivity);
		uUsedSize += sizeof(AwardLobbyActivity);
	}


EXIT0:
	return TRUE;
}

LuaPlayerAwardLobby* PlayerAwardLobby::GetScriptInterface() CONST
{
	return m_pLunaObj;
}

BOOL PlayerAwardLobby::PlayerSignedIn(INT nDay)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KAward* pAward = NULL;
	SignInAward* pSignInAward = NULL;
	INT nIndex = 0;
	INT nSignInCount = 0;

	TIME_T tNow = KSysService::GameTime(NULL);
	tm *pNowTime = ::localtime(&tNow);
	INT nYear = pNowTime->tm_year;
	INT nMonth = pNowTime->tm_mon;

	tm *pSignInTime = NULL;

	LOG_PROCESS_ERROR(nDay >= 1 && nDay <= pNowTime->tm_mday);
	pSignInTime = ::localtime(&m_dwLastSignedInTime);
	//不在同一个月
	if (nYear != pSignInTime->tm_year || nMonth != pSignInTime->tm_mon)
	{
		m_nSignedIn = 0;
	}

	LOG_PROCESS_ERROR(!IfHasSignedIn(nDay));

	m_nSignedIn |= (1 << (nDay - 1));
	m_dwLastSignedInTime = tNow;
	nSignInCount = GetSignedInCount();
	do 
	{
		pSignInAward = g_cAwardLobbySetting.GetSignInAwardByIndex(nIndex);
		++nIndex;
		if (pSignInAward != NULL)
		{
			if (pSignInAward->nSignInCount == nSignInCount)
				pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer,"SignAward", pSignInAward->nAwardId);
		}
	} while (pSignInAward != NULL);

	bRetCode = SyncSignedInInfo();
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerAwardLobby::IfHasSignedIn(INT nDay)
{
	BOOL bResult = FALSE;
	INT nHasSigned = 0;

	LOG_PROCESS_ERROR(nDay >= 1 && nDay <= 31);

	nHasSigned = m_nSignedIn & (1 << (nDay - 1));

	bResult = (nHasSigned != 0);
EXIT0:
	return bResult;
}

INT PlayerAwardLobby::GetSignedInCount()
{
	INT nCount = 0;
	for (INT i = 1; i <= 31; i++)
	{
		if (IfHasSignedIn(i))
			nCount++;
	}
	return nCount;
}

BOOL PlayerAwardLobby::SyncSignedInInfo()
{
	TIME_T tNow = KSysService::GameTime(NULL);
	tm *pNowTime = ::localtime(&tNow);
	INT nYear = pNowTime->tm_year;
	INT nMonth = pNowTime->tm_mon;

	tm *pSignInTime = ::localtime(&m_dwLastSignedInTime);

	//不在同一个月
	if (nYear != pSignInTime->tm_year || nMonth != pSignInTime->tm_mon)
	{
		m_nSignedIn = 0;
	}

	KS2C_SYNC_SIGNEDIN_INFO sSync;

	sSync.nSignedNum = m_nSignedIn;
	sSync.dwCreateTime = m_rcPlayer.m_cPlayerServer.m_dwCreateTime;

	return g_cAwardLobbyModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

VOID PlayerAwardLobby::SetLoginDays()
{
	TIME_T tNow = KSysService::GameTime(NULL);
	tm *pNowTime = ::localtime(&tNow);
	INT nYear = pNowTime->tm_year;
	INT nDay = pNowTime->tm_yday;
	TIME_T tLoginTime = m_dwLastLoginTime+86400;
	tm *pLastTime = ::localtime(&tLoginTime);

	//相差一天
	if (m_dwLastLoginTime == 0 || (nYear == pLastTime->tm_year && nDay == pLastTime->tm_yday))
	{
		++m_nLoginDays;
		if (m_nLoginDays > m_nMaxLoginDays)
		{
			m_nMaxLoginDays = m_nLoginDays;
			if (m_nMaxLoginDays <= 7)
			{
				SevenDaysAward* pDaysAward = g_cAwardLobbySetting.GetSevenDaysAwardByIndex(m_nMaxLoginDays-1);
				if (pDaysAward != NULL)
				{
					KAward* pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer,"SevenloginAward", pDaysAward->nAwardId);
				}
			}
		}
	}
	else if (nYear > pLastTime->tm_year || nDay > pLastTime->tm_yday)
	{
		m_nLoginDays = 1;
	}

	m_dwLastLoginTime = tNow;
}

BOOL PlayerAwardLobby::DoFinishActivity(INT nId, INT nAwardId)
{
	INT nAward = m_mapActivity[nId].nAward & (1 << (nAwardId % 31));
	KAward* pAward = NULL;
	AwardLobby* pAwardLobby = g_cAwardLobbySetting.GetAwardLobby(nId);
	TIME_T tNow = KSysService::GameTime(NULL);
	LOG_PROCESS_ERROR (pAwardLobby);
	//活动已过
	if (pAwardLobby->tStartTime > tNow || tNow > pAwardLobby->tEndTime)
	{
		return FALSE;
	}

	if (nAward == 0)
	{
		m_mapActivity[nId].nId = nId;
		m_mapActivity[nId].nAward |= (1 << (nAwardId % 31));
		SyncActivity();
		if (pAwardLobby != NULL)
		{
			pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer,pAwardLobby->szModuleName,nAwardId);
		}
	}
EXIT0:
	return TRUE;
}

//冲榜活动，活动结束时计算排名发放奖励
BOOL PlayerAwardLobby::DoFinishRankActivity(INT nId, INT nAwardId)
{
	INT nAward = m_mapActivity[nId].nAward;// & (1 << (nAwardId % 31));
	KAward* pAward = NULL;
	AwardLobby* pAwardLobby = g_cAwardLobbySetting.GetAwardLobby(nId);
	TIME_T tNow = KSysService::GameTime(NULL);
	PROCESS_ERROR (pAwardLobby);
	//活动结束前一分钟
	if (pAwardLobby->tEndTime - 60 > tNow || tNow > pAwardLobby->tEndTime || nAward > 0)
	{
		return FALSE;
	}

	m_mapActivity[nId].nId = nId;
	m_mapActivity[nId].nAward |= (1 << (nAwardId % 31));
	SyncActivity();
	if (pAwardLobby != NULL)
	{
		pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer,pAwardLobby->szModuleName,nAwardId);
	}
EXIT0:
	return TRUE;
}

BOOL PlayerAwardLobby::DoRecharge(INT nCoins)
{
	AwardLobby* pAwardLobby = NULL;
	TIME_T tNow = KSysService::GameTime(NULL);
	for (INT i = 1; i <= 2; ++i)
	{
		pAwardLobby = g_cAwardLobbySetting.GetAwardLobby(i);
		if (pAwardLobby == NULL || pAwardLobby->tStartTime > tNow || tNow > pAwardLobby->tEndTime) continue;
		
		for (std::map<INT, LobbyTarget*>::iterator it = pAwardLobby->mapLobbyTarget.begin(); it != pAwardLobby->mapLobbyTarget.end(); ++it)
		{
			if (nCoins >= it->second->nMinValue)
			{
				DoFinishActivity(i, it->first);
			}
		}
	}

EXIT0:
	return TRUE;
}

BOOL PlayerAwardLobby::SyncSevenDays()
{
	SetLoginDays();
	KS2C_SYNC_SEVENDAY_INFO sSync;

	sSync.nLoginDays = m_nLoginDays;
	sSync.nMaxLogindays = m_nMaxLoginDays;

	return g_cAwardLobbyModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL PlayerAwardLobby::SyncActivity()
{
	IMemBlock* piPackage					= NULL;
	KS2C_SYNC_ACTIVITY_INFO* ptcActivity	= NULL;
	INT nInx								= m_mapActivity.size();
	BOOL bOk								= FALSE;
	piPackage = QCreateMemBlock(sizeof(KS2C_SYNC_ACTIVITY_INFO)+sizeof(KLobbyActivityInfo) * nInx);
	ptcActivity = (KS2C_SYNC_ACTIVITY_INFO*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcActivity);

	ptcActivity->byProtocol					= (BYTE)s2c_sync_activity_info;
	ptcActivity->nCount						= nInx;

	nInx = 0;
	for (std::map<INT, AwardLobbyActivity>::iterator it = m_mapActivity.begin(); it != m_mapActivity.end(); ++it)
	{
		ptcActivity->arrInfo[nInx].nId = it->second.nId;
		ptcActivity->arrInfo[nInx].nAward = it->second.nAward;
		nInx++;

	}

	bOk =  g_cAwardLobbyModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcActivity, sizeof(KS2C_SYNC_ACTIVITY_INFO)+sizeof(KLobbyActivityInfo) * nInx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}
