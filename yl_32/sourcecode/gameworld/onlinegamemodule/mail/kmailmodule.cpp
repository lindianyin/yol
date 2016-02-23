/* -------------------------------------------------------------------------
//	文件名		：	kmailmodule.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/23 15:28:06
//	功能描述	：	邮件模块
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegamemodule/mail/kmailmodule.h"
#include "serverbase/kgcmoduledef.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "onlinegameworld/kplayer.h"

#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/mail/kmailmgr_gs.h"
#else
#include "onlinegameworld/kplayermanager_client.h"
#include "onlinegamemodule/mail/kmailmgr_client.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailModule g_cMailModule;

// template<>
// IKModuleInterface* GetModuleInterFace<emKMODULE_MAIL>()
// {
// 	return &g_cMailModule;
// }

KMailModule::KMailModule() : KGcModuleBase(emKMODULE_NONE, emKGCMODULE_NONE)
{
}

KMailModule::~KMailModule()
{
}

BOOL KMailModule::OnInit()
{
	return TRUE;
}

BOOL KMailModule::OnUnInit()
{
	return TRUE;
}

BOOL KMailModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(KMAIL_S2C_MAILLOADOK);
	KD_PTC_LENGTNED_SIZE_S2C(KMAIL_S2C_PROTO_FETCHINFO);
	KD_PTC_LENGTNED_SIZE_S2C(KMAIL_S2C_PROTO_FETCHMAIL);
	KD_PTC_LENGTNED_SIZE_S2C(KMAIL_S2C_PROTO_SENDRESULT);
	KD_PTC_FIXED_SIZE_S2C(KMAIL_S2C_PROTO_NEWMAIL);
	KD_PTC_FIXED_SIZE_S2C(KMAIL_S2C_PROTO_FETCHITEMOK);

	KD_PTC_FIXED_SIZE_C2S(KMAIL_C2S_PROTO_FETCHINFO);
	KD_PTC_FIXED_SIZE_C2S(KMAIL_C2S_PROTO_FETCHCONDINFO);
	KD_PTC_FIXED_SIZE_C2S(KMAIL_C2S_PROTO_FETCHMAIL);
	KD_PTC_FIXED_SIZE_C2S(KMAIL_C2S_PROTO_FETCHITEM);
	KD_PTC_FIXED_SIZE_C2S(KMAIL_C2S_PROTO_DELMAIL);
	KD_PTC_LENGTNED_SIZE_C2S(KMAIL_C2S_PROTO_SENDMAIL);

	return TRUE;
}

#ifdef GAME_SERVER

//处理GC发过来协议
BOOL KMailModule::ProcessGcProtocol(LPCBYTE pbData, UINT uDataLen)
{
	return KMailManager_GS::Instance()->ProcessGcProtocol(pbData, uDataLen);
}

// 处理玩家客户端协议
BOOL KMailModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	return KMailManager_GS::Instance()->ProcessClientProtocol(nPlayerIndex, pbData, uDataLen);
}

BOOL KMailModule::SendGcMsg(LPCVOID pData, UINT uSize)
{
	BOOL bRet = FALSE;
	LOG_PROCESS_ERROR(m_piGcSvcClient);
	bRet = m_piGcSvcClient->SendData(pData, uSize);
	PROCESS_ERROR(bRet);
	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KMailModule::SendClientMsg(DWORD dwPlayerId, KPTC_HEADER_BASE& rPtcData )
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);

	QCONFIRM_RET_FALSE(this->GetProtocolProcessor());
	return this->GetProtocolProcessor()->SendData(pPlayer->GetPlayerIndex(), rPtcData);
}

BOOL KMailModule::SendVarLenClientMsg(DWORD dwPlayerId, KLENPTC_HEADER_BASE& rPtcData, UINT uDataLen )
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);

	QCONFIRM_RET_FALSE(this->GetProtocolProcessor());
	return this->GetProtocolProcessor()->SendLenData(pPlayer->GetPlayerIndex(), rPtcData, uDataLen);
}

#endif

#ifdef GAME_CLIENT

// 处理GS发过来协议
BOOL KMailModule::ProcessProtocol(LPCBYTE pbData, UINT uDataLen)
{
	return KMailManager_Client::Instance()->ProcessGsProtocal(pbData, uDataLen);
}

BOOL KMailModule::SendGsMsg(KPTC_HEADER_BASE& rPtcData)
{
	QCONFIRM_RET_FALSE(this->GetProtocolProcessor());
	return this->GetProtocolProcessor()->SendData(rPtcData);
}

BOOL KMailModule::SendVarLenGsMsg(KLENPTC_HEADER_BASE& rPtcData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(this->GetProtocolProcessor());
	return this->GetProtocolProcessor()->SendLenData(rPtcData, uDataLen);
}

#endif

// -------------------------------------------------------------------------
