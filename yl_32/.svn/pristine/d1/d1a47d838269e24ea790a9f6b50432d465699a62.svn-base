

#include "stdafx.h"
#include "onlinegamemodule/chat/kplayerchat.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegamebase/ksysservice.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "kchatapply_client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
#pragma warning(disable : 4355) // 'this' : used in base member initializer list

KPlayerChat::KPlayerChat(KCharacter& rcPlayer)
: KCharacterCtrlBase(rcPlayer), m_cLunaObj(*this),
m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}
#pragma warning(default : 4355)

CONST KLuaPlayerChat& KPlayerChat::GetScriptInterface() CONST
{
	return m_cLunaObj;
}

BOOL KPlayerChat::Init()
{
	memset(&sChatDisable, 0, sizeof(KCHAT_DISABLE));
	m_mapBroadcastInfo.clear();

	return TRUE;
}

BOOL KPlayerChat::UnInit()
{
	return TRUE;
}

BOOL KPlayerChat::OnLogin()
{
	// 查看是否赠送过线聊喇叭
	time_t tResetTime = (time_t)m_rcPlayer.GetWorldFreeSpeakerResetTime();
	if (!IsToday(tResetTime))
	{
		INT nFreeWorldSpeakerCount = g_ChatChannelManager.GetFreeWorldSpeaker();
		INT nCurTime = (INT)KSysService::Time(NULL);
		m_rcPlayer.SetWorldFreeSpeakerCount(nFreeWorldSpeakerCount);
		m_rcPlayer.SetWorldFreeSpeakerResetTime(nCurTime);
	}
	
	return TRUE;
}

BOOL KPlayerChat::OnLogout()
{
	return TRUE;
}

BOOL KPlayerChat::SendMsg(INT nChannelId, INT nReceiverId, LPCSTR pszMessage, BYTE byShownType)
{
	return g_ChatChannelManager.RequestSendMsg(nChannelId, m_rcPlayer.GetId(), nReceiverId, m_rcPlayer.GetName(), pszMessage, byShownType);
}

#ifdef GAME_CLIENT

//给玩家发消息
BOOL KPlayerChat::SendMsgToPlayer(INT nChannelId, const char* pszReceiver, LPCSTR pszMessage)
{
	QCONFIRM_RET_FALSE(NULL != pszReceiver);
	QCONFIRM_RET_FALSE(NULL != pszMessage);
	
	return g_cClientChatPrivate.TransferToServerByPlayerName(
										nChannelId, 
										m_rcPlayer.GetPlayerId(), 
										pszReceiver, 
										m_rcPlayer.GetName(), 
										pszMessage, 
										FALSE);
}
#endif //#ifdef GAME_CLIENT

BOOL KPlayerChat::IsToday(time_t tTime)
{
	BOOL bRet = FALSE;
	
	KCHAT_DATE sDate;
	time_t tCurTime = KSysService::Time(NULL);
	tm* pTime = ::localtime(&tTime);
	sDate.nYear = pTime->tm_year;
	sDate.nMonth = pTime->tm_mon;
	sDate.nMonDay = pTime->tm_mday;
	tm* pCurTime = ::localtime(&tCurTime);

	PROCESS_ERROR(pTime && pCurTime && "[kplayerchat] localtime failed!!!!");

	if (sDate.nYear == pCurTime->tm_year &&
		sDate.nMonth == pCurTime->tm_mon &&
		sDate.nMonDay == pCurTime->tm_mday)
	{
		bRet = TRUE;
	}

EXIT0:
	return bRet;
}

BOOL KPlayerChat::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	BOOL bResult = FALSE;

	memcpy(pBuffer, &sChatDisable, sizeof(KCHAT_DISABLE));
	uUsedSize += sizeof(KCHAT_DISABLE);
	pBuffer += sizeof(KCHAT_DISABLE);

	for(std::map<INT, KBROADCAST_INFO>::iterator it = m_mapBroadcastInfo.begin(); it != m_mapBroadcastInfo.end(); ++it)
	{
		memcpy(pBuffer, &(it->first), sizeof(INT));
		pBuffer += sizeof(INT);
		memcpy(pBuffer, &(it->second), sizeof(KBROADCAST_INFO));
		pBuffer += sizeof(KBROADCAST_INFO);
		uUsedSize += sizeof(INT) + sizeof(KBROADCAST_INFO);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerChat::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;

	if(uSize <= 0)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	ASSERT(uSize >= sizeof(KCHAT_DISABLE));

	memcpy(&sChatDisable, pData, sizeof(KCHAT_DISABLE));

	SyncDisableInfo();

	uSize -= sizeof(KCHAT_DISABLE);
	pData += sizeof(KCHAT_DISABLE);

	while(uSize >= sizeof(INT) + sizeof(KBROADCAST_INFO))
	{
		INT nId = 0;
		KBROADCAST_INFO info;
		memcpy(&nId, pData, sizeof(INT));
		pData += sizeof(INT);
		memcpy(&info, pData, sizeof(KBROADCAST_INFO));
		pData += sizeof(KBROADCAST_INFO);
		m_mapBroadcastInfo[nId] = info;
		uSize -= (sizeof(INT) + sizeof(KBROADCAST_INFO));
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KPlayerChat::SetDisableEndTime(INT nLastTime)
{
	time_t tNow = KSysService::Time(NULL);
	time_t tDisableEndTime = tNow + nLastTime;

	sChatDisable.dwDisableEndTime = tDisableEndTime;
}

BOOL KPlayerChat::IfCanSendMsg(INT nChannelId)
{
	BOOL bRetCode = FALSE;
	time_t tNow = KSysService::Time(NULL);
	
	// 禁言时间过期了
	if(tNow > sChatDisable.dwDisableEndTime)
	{
		bRetCode = TRUE;
		goto EXIT0;
	}

	if(tNow < sChatDisable.dwDisableEndTime && sChatDisable.nDisableChannel == nChannelId)
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KPlayerChat::SyncDisableInfo()
{
	KCHATPROTO_S2C_DISABLE_INFO sSync;

	sSync.bDisableAuth = sChatDisable.bDisableAuthority;
	sSync.nDisableChannel = sChatDisable.nDisableChannel;
	sSync.dwDisableEndTime = sChatDisable.dwDisableEndTime;

	return g_ChatChannelManager.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

VOID KPlayerChat::ModifyBroadcastInfo(INT nId, DWORD dwBroadcastTime)
{
	std::map<INT, KBROADCAST_INFO>::iterator it = m_mapBroadcastInfo.find(nId);
	if(it != m_mapBroadcastInfo.end())
	{
		it->second.dwLastBroadcastTime = dwBroadcastTime;
	}
	else
	{
		KBROADCAST_INFO info;
		info.dwLastBroadcastTime = dwBroadcastTime;
		m_mapBroadcastInfo[nId] = info;
	}
}

DWORD KPlayerChat::GetLastBroadcastTime(INT nId)
{
	DWORD dwLastBroadcastTime = 0;
	std::map<INT, KBROADCAST_INFO>::iterator it = m_mapBroadcastInfo.find(nId);
	if(it != m_mapBroadcastInfo.end())
	{
		dwLastBroadcastTime = it->second.dwLastBroadcastTime;
	}

	return dwLastBroadcastTime;
}

// -------------------------------------------------------------------------
