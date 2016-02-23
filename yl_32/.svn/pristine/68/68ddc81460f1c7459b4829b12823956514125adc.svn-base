/* -------------------------------------------------------------------------
//	文件名		：	tmpfriendfactory.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	临时好友工厂
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "tmpfriendfactory.h"
#include "relationfactorylist.h"
#include "relationmanager.h"

KTmpFriendFactory::KTmpFriendFactory() : SUPER_CLASS(emKPLAYERRELATION_TYPE_TMPFRIEND)
{
}

KTmpFriendFactory::~KTmpFriendFactory()
{
}

BOOL KTmpFriendFactory::CanCreateRelation(INT nMaster, INT nTarget)
{
	BOOL					bRetCode	= FALSE;
	IKRelationFactory*		pBidFactory = NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pBidFactory);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pBidFactory);

	if (pBidFactory->FindRelation(nMaster, nTarget))
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	bRetCode = SUPER_CLASS::CanCreateRelation(nMaster, nTarget);
EXIT0:
	return bRetCode;

	// 如果已经是普通好友，则不允许建立临时好友关系
// 	if (pBidFactory->FindRelation(nMaster, nTarget))
// 	{
// 		// KGC_Error("");
// 		return FALSE;
// 	}
// 
// 	if (!SUPER_CLASS::CanCreateRelation(nMaster, nTarget))
// 		return FALSE;
// 
// 	return TRUE;
}


BOOL KTmpFriendFactory::CreateRelation(INT nMaster,
									   INT nTarget,
									   BOOL			bIsMasterActive,
									   IKRelation**	ppRelation)
{
	BOOL						bRetCode	= FALSE;
	IKRelation*					pRelation	= NULL;
	IKRelationFactory*			pBidFactory = NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	//不自动转换为双向关系
	//if (FindRelation(nTarget, nMaster))
	//{
	//	// 如果已经是反向临时好友，则自动转为普通好友
	//	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pBidFactory);
	//	LOG_PROCESS_ERROR(bRetCode);
	//	LOG_PROCESS_ERROR(pBidFactory);

	//	bRetCode = DeleteRelation(nTarget, nMaster);
	//	LOG_PROCESS_ERROR(bRetCode);

	//	bRetCode = pBidFactory->CreateRelation(nMaster, nTarget, bIsMasterActive, &pRelation);
	//	LOG_PROCESS_ERROR(bRetCode);

	//	// todo 通知自己与对方成为普通好友
	//}
	//else 
	{
		bRetCode = SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive);
		LOG_PROCESS_ERROR(bRetCode);
	}


	bRetCode = TRUE;
EXIT0:
	return bRetCode;

	// KGC_CONFIRM_RET_FALSE(nMaster && nTarget && m_pSender);
	
// 	KGC_Player* pGCPlayerMaster = g_pPlayerManager->GetPlayer(nMaster);
// 	KGC_Player* pGCPlayerTarget = g_pPlayerManager->GetPlayer(nTarget);
// 	// KGC_CONFIRM_RET_FALSE(pGCPlayerMaster && pGCPlayerTarget);
// 	if (FindRelation(nTarget, nMaster))
// 	{
// 		IKRelation *pRelation = NULL;
// 		IKRelationFactory *pBidFactory = NULL;
// 		if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pBidFactory))
// 		{
// 			// KGC_Error("");
// 			return FALSE;
// 		}
// 		// KGC_CONFIRM_RET_FALSE(pBidFactory);
// 		// 如果已经是反向临时好友，则自动转为普通好友
// 		if (!DeleteRelation(nTarget, nMaster) ||
// 			!pBidFactory->CreateRelation(nMaster, nTarget, bIsMasterActive, &pRelation))
// 		{
// 			// KGC_Error("");
// 			return FALSE;
// 		}
// 		// 通知自己与对方成为普通好友
// 		INT nServerId	= pGCPlayerMaster->GetOnlineServer();
// 		INT nPlayerId	= g_pPlayerManager->GetPlayerIDByName(pGCPlayerMaster->GetPlayerName());
// 		CHAR szBuf[1024];
// 
// 		sprintf(szBuf, KD_STR_MAKE_BIDRELATION, pGCPlayerTarget->GetPlayerName());
// 		
// 		g_ChatChannelManager.PostSystemMessage(
// 			nPlayerId,
// 			KD_STR_SYSTEM,
// 			szBuf,
// 			nServerId);
// /*
// 		INT nTargetPlayerServer = pGCPlayerTarget->GetOnlineServer();
// 		if (nTargetPlayerServer > 0)
// 		{
// 			// 通知被加为好友
// 			KRELATION_PROTO_G2S_NOTIFYRELATION sNotify;
// 			sNotify.ProtocolType = emKRELATION_G2S_PROTO_NOTIFYRELATION;
// 			sNotify.nType = emKPLAYERRELATION_TYPE_TMPFRIEND;
// 			sNotify.nRole = 0;
// 			sNotify.nPlayerID = nTarget;
// 			strncpy(sNotify.szRelation, pGCPlayerMaster->GetPlayerName(), sizeof(sNotify.szRelation) - 1);
// 			sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nTargetPlayerServer);
// 		}
// */
// 
// 	}
// 	else
// 	{
// 		// 如果不存在反向临时好友关系，则直接建立临时好友关系（不通知被加为临时好友的玩家）
// 		if (!SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive))
// 		{
// 			KGC_INFO("");
// 			return FALSE;
// 		}
// 	}
// 	return TRUE;
}

