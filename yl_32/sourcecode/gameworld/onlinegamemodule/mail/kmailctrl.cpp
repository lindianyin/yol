/* -------------------------------------------------------------------------
//	文件名		：	kmailctrl.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/24 9:48:47
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegamemodule/mail/kmailctrl.h"
#include "onlinegamemodule/mail/kmailmodule.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
#pragma warning(disable : 4355) // 'this' : used in base member initializer list

KMailCtrl::KMailCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer),
											 m_cLunaObj(*this)
{
	m_bMailBoxLoaded = FALSE;
}
#pragma warning(default : 4355)

CONST KLuaPlayerMail& KMailCtrl::GetScriptInterface() CONST
{
	return m_cLunaObj;
}

BOOL KMailCtrl::Init()
{	
	time_t t = KSysService::Time(NULL);
	tm* pTime = ::localtime(&t);
	m_sSaveData.sSendTime.nYear = pTime->tm_year + 1900;
	m_sSaveData.sSendTime.nMonth = pTime->tm_mon + 1;
	m_sSaveData.sSendTime.nDay = pTime->tm_mday;
	m_sSaveData.nSendCount = 0;
	
	return TRUE;
}

BOOL KMailCtrl::UnInit()
{
	return TRUE;
}

BOOL KMailCtrl::OnLogin()
{
	return TRUE;
}

BOOL KMailCtrl::OnLogout()
{
#ifdef GAME_SERVER

	KMAIL_S2G_PROTO_PLAYER_LOGOUT sNotify;
	sNotify.dwPlayerId = m_rcCharacter.GetId();
	g_cMailModule.SendGcMsg(&sNotify, sizeof(sNotify));

#endif /* GAME_SERVER */
	
	return TRUE;
}

#ifdef GAME_SERVER

BOOL KMailCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	QCONFIRM_RET_FALSE(sizeof(KMailSaveData) <= uBuffSize);

	// 检测发送时间
	time_t t = KSysService::Time(NULL);
	tm* pTime = ::localtime(&t);
	KSendTime sSendTime;
	sSendTime.nYear = pTime->tm_year + 1900;
	sSendTime.nMonth = pTime->tm_mon + 1;
	sSendTime.nDay = pTime->tm_mday;
	if (!m_sSaveData.sSendTime.IsSame(sSendTime))
	{
		// 当前时间已经过了记录的发送时间，发送限制次数清0
		m_sSaveData.sSendTime = sSendTime;
		m_sSaveData.nSendCount = 0;
	}

	*(KMailSaveData*)pBuffer = m_sSaveData;
	uUsedSize = sizeof(KMailSaveData);
	
	return TRUE;
}

BOOL KMailCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	QCONFIRM_RET_FALSE(pData && uSize == sizeof(KMailSaveData));
	
	time_t t = KSysService::Time(NULL);
	tm* pTime = ::localtime(&t);
	m_sSaveData.sSendTime.nYear = pTime->tm_year + 1900;
	m_sSaveData.sSendTime.nMonth = pTime->tm_mon + 1;
	m_sSaveData.sSendTime.nDay = pTime->tm_mday;

	KMailSaveData* pMailData = (KMailSaveData*)pData;
	if (pMailData->sSendTime.IsSame(m_sSaveData.sSendTime))
	{
		m_sSaveData.nSendCount = pMailData->nSendCount;
	}
	else
	{
		m_sSaveData.nSendCount = 0;
	}

	return TRUE;
}

BOOL KMailCtrl::OnClientReady()
{
	KMAIL_S2G_PROTO_PLAYER_LOGIN sNotify;
	sNotify.dwPlayerId = m_rcCharacter.GetId();
	g_cMailModule.SendGcMsg(&sNotify, sizeof(sNotify));

	return TRUE;
}

#endif /* GAME_SERVER */

VOID KMailCtrl::SetMailBoxLoaded(BOOL bMailBoxLoaded)
{
	m_bMailBoxLoaded = bMailBoxLoaded;
}

BOOL KMailCtrl::IsMailBoxLoaded()
{
	return m_bMailBoxLoaded;
}

VOID KMailCtrl::AddMailSendTime()
{
	// 检测发送时间
	time_t t = KSysService::Time(NULL);
	tm* pTime = ::localtime(&t);
	KSendTime sSendTime;
	sSendTime.nYear = pTime->tm_year + 1900;
	sSendTime.nMonth = pTime->tm_mon + 1;
	sSendTime.nDay = pTime->tm_mday;
	if (!m_sSaveData.sSendTime.IsSame(sSendTime))
	{
		// 当前时间已经过了记录的发送时间，发送限制次数清0
		m_sSaveData.sSendTime = sSendTime;
		m_sSaveData.nSendCount = 1;
	}
	else
	{
		m_sSaveData.nSendCount++;
	}
}

BOOL KMailCtrl::CanSendMail(INT nSendLimit)
{
	// 检测发送时间
	time_t t = KSysService::Time(NULL);
	tm* pTime = ::localtime(&t);
	KSendTime sSendTime;
	sSendTime.nYear = pTime->tm_year + 1900;
	sSendTime.nMonth = pTime->tm_mon + 1;
	sSendTime.nDay = pTime->tm_mday;
	if (!m_sSaveData.sSendTime.IsSame(sSendTime))
	{
		// 当前时间已经过了记录的发送时间，发送限制次数清0
		m_sSaveData.sSendTime = sSendTime;
		m_sSaveData.nSendCount = 0;
		return TRUE;
	}
	else
	{
		return m_sSaveData.nSendCount < nSendLimit;
	}
}

DWORD KMailCtrl::GetPlayerId()
{
	return m_rcCharacter.GetId();
}

// -------------------------------------------------------------------------
