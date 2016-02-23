

#ifdef GAME_SERVER
#include "stdafx.h"
#include "kchatapply_gs.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "onlinegameworld/kplayerprotocolprocess.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "onlinegameworld/synchelper.h"

#include "onlinegameworld/kcharactermgr.h"
#include "onlinegameworld/korpgworld.h"
#include "../onlinegameworld/KRelayClient.h"
#include "qteamserver.h"
#include "Runtime/TypeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KGSChatProcess_Default	g_cGSChatDefault;
KGSChatProcess_Nearby	g_cGSChatNearby;
KGSChatProcess_Private	g_cGSChatPrivate;
KGSChatProcess_World	g_cGSChatWorld;
KGSChatProcess_Scene	g_cGSChatScene;
KGSChatProcess_Team		g_cGSChatTeam;
KGSChatProcess_Faction	g_cGsChatFaction;
KGSChatProcess_Kin		g_cGSChatKin;
KGSChatProcess_Group	g_cGSChatGroup;
KGSChatProcess_GM		g_cGSChatGM;
KGSChatProcess_Global	g_cGSChatGlobal;


KProcessorFactory_GS g_cChatChannelProcessorFactory_GS;

static BYTE _sbyChatPacketBuffer[KD_CHANNEL_PACKET_MAX_LEN]; // chat网络包缓冲区
// -------------------------------------------------------------------------
// KGSChatProcess_Default
BOOL KGSChatProcess_Default::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{

	return FALSE;
}

BOOL KGSChatProcess_Default::TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	size_t uMsgLen = strlen(pszMessage) + 1;
	g_RelayClient.DoChatMessage(nChannelId, nSenderId, pszSenderName, nReceiverId, "", uMsgLen, (LPCBYTE)pszMessage, byShownType);

	return TRUE;
}


// -------------------------------------------------------------------------
// KGSChatProcess_Nearby
BOOL KGSChatProcess_Nearby::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	g_ChatChannelManager.ProcessRespond(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage);
	return TRUE;
}

BOOL KGSChatProcess_Nearby::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_S2C_MSG();

	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KCharacter *pSender = g_cCharacterMgr.GetById(nSenderId);
	QCONFIRM_RET_FALSE(pSender);
	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	pSender->m_cCharacterServer.BroadcastNearby(&cSendData, sizeof(cSendData) + uMsgLen, TRUE, pProtocolProcessor);

	return TRUE;
}

// -------------------------------------------------------------------------
// KGSChatProcess_Private
BOOL KGSChatProcess_Private::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	KPlayer *pPlayer = g_cPlayerMgr.GetById(nReceiverId);
	if (!pPlayer)
		return FALSE;

	if (strcmp(pszSenderName, pPlayer->GetName()) == 0)
		return TRUE;

	g_ChatChannelManager.ProcessRespond(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage);

	return FALSE;
}

BOOL KGSChatProcess_Private::ProcessSendMsgToPlayer(INT nChannelId, INT nSenderId, LPCSTR pszReceiver, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	KPlayer *pPlayer = g_cPlayerMgr.GetById(nSenderId);
	if (!pPlayer)
		return FALSE;

	size_t uMsgLen = strlen(pszMessage) + 1;

	CHAR receiverName[MAX_NAME_LEN];
	INT i = 0;
	CHAR lastChar;

	for (i = 0; i < MAX_NAME_LEN; i++)
	{
		lastChar = pszMessage[i];
		if (lastChar == ' ' || lastChar == '\0')
			break;

		receiverName[i] = lastChar;
	}

	if (i == MAX_NAME_LEN || lastChar == '\0')
	{
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
			pPlayer->GetId(), "DEBUG", LANG_036);
		return FALSE;
	}
	else
	{
		receiverName[i] = '\0';
		
		//KPlayer* pReceiver = g_cPlayerMgr.GetByName(receiverName);
		//if (pReceiver == NULL)
		//{
		//	QString errorStr;
		//	errorStr.Format(LANG_037, receiverName);
		//	g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
		//		pPlayer->GetId(), "DEBUG", errorStr.CStr());
		//	return FALSE;
		//}

		//if (pReceiver->GetId() == nSenderId)
		//{
		//	g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
		//		pPlayer->GetId(), "DEBUG", LANG_038);
		//	return FALSE;
		//}
	}

	g_RelayClient.DoChatMessage(nChannelId, nSenderId, pszSenderName, KD_BAD_ID, receiverName, uMsgLen, (LPCBYTE)(pszMessage + i + 1), byShownType);
	//g_RelayClient.DoChatMessage(nChannelId, nSenderId, pszSenderName, KD_BAD_ID, pszSenderName, uMsgLen, (LPCBYTE)(pszMessage + i + 1), byShownType);
	return TRUE;
}

BOOL KGSChatProcess_Private::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_S2C_MSG();
	QLogPrintf(LOG_INFO,"..KGSChatProcess_Private::TransferToClient(INT nChannelId=%d, INT nSenderId=%d, INT nReceiverId=%d, LPCSTR pszSenderName=%s, LPCSTR pszMessage=%s, BYTE byShownType=%d)",
							nChannelId,nSenderId,nReceiverId,pszSenderName,pszMessage,byShownType);
	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KPlayer* pReceiver = g_cPlayerMgr.GetById(nReceiverId);
	QCONFIRM_RET_FALSE(pReceiver);
	pProtocolProcessor->SendLenData(pReceiver->GetPlayerIndex(), cSendData, sizeof(cSendData)+uMsgLen);

	KPlayer* pSender = g_cPlayerMgr.GetById(nSenderId);
	if(pSender)
	{
		//pProtocolProcessor->SendLenData(pSender->GetPlayerIndex(), cSendData, sizeof(cSendData)+uMsgLen);
	}

	return TRUE;
}


// -------------------------------------------------------------------------
// KGSChatProcess_World

BOOL KGSChatProcess_World::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{	
	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_S2C_MSG();

	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KCharacterServer::BroadcastThisServer(
		(LPCBYTE)&cSendData, sizeof(cSendData) + uMsgLen, NULL, pProtocolProcessor);

	return TRUE;
}

// -------------------------------------------------------------------------
// KGSChatProcess_Global

BOOL KGSChatProcess_Global::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	// Global 需要转发到gc上 
	/*g_ChatChannelManager.ProcessRespond(
		nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage);*/
	return FALSE;
}
BOOL KGSChatProcess_Global::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));

	cSendData = KCHATPROTO_S2C_MSG();

	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	KCharacterServer::BroadcastThisServer(
		(LPCBYTE)&cSendData, sizeof(cSendData) + uMsgLen, NULL, pProtocolProcessor);

	// 扣喇叭数
	if (nChannelId != trGmMessage)
	{
		KPlayer *pPlayer = g_cPlayerMgr.GetById(nSenderId);
		if (pPlayer != NULL)  // 发送者可能不在这个服
		{
			INT nGlobalSpeakerCount = pPlayer->GetGlobalSpeakerCount();
			nGlobalSpeakerCount = nGlobalSpeakerCount - 1;
			nGlobalSpeakerCount = nGlobalSpeakerCount < 0 ? 0: nGlobalSpeakerCount;
			pPlayer->SetGlobalSpeakerCount(nGlobalSpeakerCount);
		}
	}

	return TRUE;
}
// -------------------------------------------------------------------------
// KGSChatProcess_Scene add by NN
BOOL KGSChatProcess_Scene::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	g_ChatChannelManager.ProcessRespond(
		nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage);

	return TRUE;
}

BOOL KGSChatProcess_Scene::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{	
	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_S2C_MSG();

	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	KPlayer* pPlayer = g_cPlayerMgr.GetById(nSenderId);
	QCONFIRM_RET_FALSE(pPlayer);
	pPlayer->m_cCharacterServer.BroadcastScene(
		(LPCBYTE)&cSendData, sizeof(cSendData) + uMsgLen, TRUE, pProtocolProcessor);

	// 扣喇叭数
	if (nChannelId != trGmMessage)
	{
		KPlayer *pPlayer = g_cPlayerMgr.GetById(nSenderId);
		QCONFIRM_RET_FALSE(pPlayer);

		INT nWorldFreeSpeakerCount = pPlayer->GetWorldFreeSpeakerCount();
		if (nWorldFreeSpeakerCount > 0)
		{
			nWorldFreeSpeakerCount = nWorldFreeSpeakerCount - 1;
			pPlayer->SetWorldFreeSpeakerCount(nWorldFreeSpeakerCount);
		}
		else
		{
			INT nWorldSpeakerCount = pPlayer->GetWorldSpeakerCount();
			nWorldSpeakerCount = nWorldSpeakerCount - 1;
			pPlayer->SetWorldSpeakerCount(nWorldSpeakerCount);
		}
	}

	return TRUE;
}
// -------------------------------------------------------------------------
// KGSChatProcess_Faction

BOOL KGSChatProcess_Faction::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{	
	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_S2C_MSG();

	std::vector<KPlayer*> vPlayers;
	g_cPlayerMgr.GetAllPlayers(vPlayers);

	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	INT nFaction = nReceiverId;
	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		if (pPlayer->GetPrimaryFaction() == nFaction)
		{
			pProtocolProcessor->SendLenData(pPlayer->GetPlayerIndex(), cSendData, sizeof(cSendData) + uMsgLen);
		}
	}

	return TRUE;
}
// -------------------------------------------------------------------------
// KGSChatProcess_Team

BOOL KGSChatProcess_Team::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	QTeam* pTeam = g_cTeamServer.GetTeam(nReceiverId);
	QCONFIRM_RET_FALSE(pTeam);

	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_S2C_MSG();

	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		for (
			QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
			it != pTeam->MemberGroup[i].MemberList.end(); ++it
			)
		{

			KPlayer* pPlayer = g_cPlayerMgr.GetById(it->dwMemberID);
			if (pPlayer)
				pProtocolProcessor->SendLenData(pPlayer->GetPlayerIndex(), cSendData, sizeof(cSendData) + uMsgLen);
		}
	}

	return TRUE;
}

// -------------------------------------------------------------------------
// KGSChatProcess_Kin

BOOL KGSChatProcess_Kin::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_S2C_MSG();

	cSendData.nChannelId = nChannelId;
	cSendData.byShownType = byShownType;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KPlayer* pPlayer = g_cPlayerMgr.GetById(nReceiverId);
	QCONFIRM_RET_FALSE(pPlayer);

	pProtocolProcessor->SendLenData(pPlayer->GetPlayerIndex(), cSendData, sizeof(cSendData) + uMsgLen);

	return TRUE;
}

// -------------------------------------------------------------------------
// KGSChatProcess_Group
BOOL KGSChatProcess_Group::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{

	return TRUE;
}

BOOL KGSChatProcess_GM::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
{
	LPCSTR pszPrefix = GM_CMD_PREFIX_GAMESERVER;
	SIZE_T nPrefixLen = sizeof(GM_CMD_PREFIX_GAMESERVER) - 1;

	KPlayer* pSender = g_cPlayerMgr.GetByName(pszSenderName);
	QCONFIRM_RET(pSender, TRUE); // consume this msg

	if ( ! g_cOrpgWorld.IsGm(pSender))
	{
		Q_Error("Player [" << pSender->GetId() << "][" <<
			pSender->m_cPlayerServer.GetAccount() << "] not in gm list, cmd " << pszMessage);
		return TRUE; // consume this msg
	}

	//QCONFIRM_RET(strlen(pszMessage) < KD_CHANNEL_MSG_MAX_LEN, TRUE); // 允许gm指令长消息

	if (QStrCmpLen(pszMessage, pszPrefix, nPrefixLen))
	{
		QLunaBase* pOldMe = g_cScriptManager.SetMe((QLunaBase*)pSender->GetScriptInterface());

		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->DoBuffer(pszMessage + nPrefixLen);

		QLogPrintf(LOG_INFO, "[GM] [%s][%s] '%s'",
			pSender->m_cPlayerServer.GetAccount(),
			pSender->GetName(),
			pszMessage);

		g_cScriptManager.SetMe(pOldMe);
		return TRUE;
	}

	return FALSE;
}
#endif
