

#include "stdafx.h"

#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegamebase/kscriptmanager.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/synchelper.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "onlinegamemodule/chat/kplayerchat.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CHAR _pszTempText[KD_CHANNEL_MSG_MAX_LEN];		// chat消息临时字符串

// -------------------------------------------------------------------------
namespace kchatscript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: HasBadWords
	// @Description		: 是否含有chat敏感词
	// @ReturnCode		: 
	// @ArgumentFlag	: s
	// @ArgumentComment	: 输入文字
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaHasBadWords(QLuaScript& sc)
	{
		BOOL bRet = g_pChatFilter->HasBadword(sc.GetStr(1));
		sc.PushNumber(bRet);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: KillBadWords
	// @Description		: 屏蔽chat敏感词
	// @ReturnCode		: 
	// @ArgumentFlag	: s
	// @ArgumentComment	: 输入文字
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaKillBadWords(QLuaScript& sc)
	{
		BOOL bRet = g_pChatFilter->KillBadword(sc.GetStr(1), _pszTempText, KD_CHANNEL_MSG_MAX_LEN);
		sc.PushString(_pszTempText);

		return 1;
	}

#ifdef GAME_SERVER
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: SendMsg
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: dddss
// @ArgumentComment	: 频道Id
// @ArgumentComment	: 发送者Id
// @ArgumentComment	: 接收对象
// @ArgumentComment	: 发送者Head
// @ArgumentComment	: 消息内容
// @LuaMarkEnd
// -------------------------------------------------------------------------
	INT LuaSendMsg(QLuaScript& sc)
	{
		BOOL bRet = g_ChatChannelManager.RequestSendMsg(sc.GetInt(1) , sc.GetInt(2), sc.GetInt(3), sc.GetStr(4), sc.GetStr(5), emQCHAT_PANEL);
		sc.PushNumber(bRet);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: BroadcastSysMsg
	// @Description		: 向所有玩家发送系统消息
	// @ReturnCode		: 
	// @ArgumentFlag	: s[dds]
	// @ArgumentComment	: 系统消息
	// @ArgumentComment	: 系统消息类型
	// @ArgumentComment	: nSenderId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaBroadcastSysMsg(QLuaScript& sc)
	{
		LPCSTR pszMsg = sc.GetStr(1);
		LPCTSTR szSenderName = NULL;
		INT nChannelId = trGlobalSys;
		DWORD dwSenderId = 0;
		if(sc.IsNumber(2))
		{
			nChannelId = sc.GetInt(2);
		}
		if(sc.IsNumber(3))
		{
			dwSenderId = sc.GetInt(3);
		}
		if(sc.IsString(4))
		{
			szSenderName = sc.GetStr(4);
		}
		//KE_SYSTEMMSG_TYPE nType = emKSYSTEMMSG_NORMAL;
		g_ChatChannelManager.RequestBroadcastSystemMsg(nChannelId, pszMsg, dwSenderId, szSenderName);
		/*static BYTE bySyncPackBuffer[KD_CHANNEL_PACKET_MAX_LEN];

		ZeroMemory(bySyncPackBuffer, sizeof(bySyncPackBuffer));

		if (sc.GetTopIndex() >= 2)
			nType = (KE_SYSTEMMSG_TYPE)sc.GetInt(2);

		QCONFIRM_RET_FALSE(pszMsg);

		KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
		QCONFIRM_RET_FALSE(pProtocolProcessor);

		KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )bySyncPackBuffer;
		cSendData = KCHATPROTO_S2C_MSG();
		cSendData.nChannelId = trLocalSys;
		cSendData.nSenderId = nType;
		ZeroMemory(cSendData.szSenderName, sizeof(cSendData.szSenderName));
		cSendData.nReceiverId = 0;

		size_t uMsgLen = strlen(pszMsg) + 1;
		QCONFIRM_RET_FALSE(uMsgLen < sizeof(bySyncPackBuffer) - sizeof(cSendData));
		QStrCpyLen(cSendData.szMsg, pszMsg, uMsgLen);

		KCharacterServer::BroadcastThisServer(
			(LPCBYTE)bySyncPackBuffer, sizeof(cSendData) + uMsgLen, NULL, pProtocolProcessor);*/

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: BroadcastBlackMsg
	// @Description		: 向所有玩家发送黑条消息
	// @ReturnCode		: 
	// @ArgumentFlag	: s
	// @ArgumentComment	: 系统消息
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaBroadcastBlackMsg(QLuaScript& sc)
	{
		INT nLen = 0;
		LPCSTR pszString = sc.GetLStr(1, nLen);
		nLen++;

		g_ChatChannelManager.RequestBroadcastBlackMsg(pszString);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: BroadcastKinNoticeMsg
	// @Description		: 向所有家族玩家发送公告消息
	// @ReturnCode		: 
	// @ArgumentFlag	: ds
	// @ArgumentComment	: 玩家id
	// @ArgumentComment	: 系统消息
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaBroadcastKinNoticeMsg(QLuaScript& sc)
	{
		INT nLen = 0;
		INT nPlayerId = sc.GetInt(1);
		LPCSTR pszString = sc.GetLStr(2, nLen);
		nLen++;

		g_ChatChannelManager.RequestSendKinNoticeMsg(nPlayerId, pszString);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: BroadcastNoticeMsg
	// @Description		: 向所有玩家发送公告消息
	// @ReturnCode		: 
	// @ArgumentFlag	: s
	// @ArgumentComment	: 系统消息
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaBroadcastNoticeMsg(QLuaScript& sc)
	{
		INT nLen = 0;
		LPCSTR pszString = sc.GetLStr(1, nLen);
		nLen++;

		g_ChatChannelManager.RequestBroadcastNoticeMsg(pszString);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetFreeWorldSpeakerCount
	// @Description		: 获取每日赠送线聊喇叭数
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @ArgumentComment	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetFreeWorldSpeakerCount(QLuaScript& sc)
	{
		INT nCount = g_ChatChannelManager.GetFreeWorldSpeaker();
		sc.PushNumber(nCount);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetFreeWorldSpeakerCount
	// @Description		: 设置每日赠送线聊喇叭数
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @ArgumentComment	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetFreeWorldSpeakerCount(QLuaScript& sc)
	{
		INT nCount = sc.GetInt(1);
		g_ChatChannelManager.SetFreeWorldSpeaker(nCount);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetChatDisableAuth
	// @Description		: 设置是否有禁言权限
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 0或1
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetChatDisableAuth(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		KPlayerChat* pPlayerChat = NULL;
		INT nHasAuth = sc.GetInt(2) ? 1 : 0;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
		LOG_PROCESS_ERROR(pPlayerChat);

		pPlayerChat->SetDisableAuthority(nHasAuth);

		pPlayerChat->SyncDisableInfo();

	EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetChatDisableAuth
	// @Description		: 是否有禁言权限
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetChatDisableAuth(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		KPlayerChat* pPlayerChat = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
		LOG_PROCESS_ERROR(pPlayerChat);

		sc.PushNumber(pPlayerChat->HasDisableAuthority());

	EXIT0:
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: DisablePlayerChat
	// @Description		: 禁言
	// @ReturnCode		: 
	// @ArgumentFlag	: o[dd]
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 禁言频道
	// @ArgumentComment	: 禁言时长
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaDisablePlayerChat(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		KPlayerChat* pPlayerChat = NULL;
		INT nChannelId = trNearby;
		INT nLastTime = 30;		// 默认30分钟

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
		LOG_PROCESS_ERROR(pPlayerChat);

		if(sc.IsNumber(2))
		{
			nChannelId = sc.GetInt(2);
		}

		if(sc.IsNumber(3))
		{
			nLastTime = sc.GetInt(3);
		}

		pPlayerChat->SetDisableChannel(nChannelId);
		pPlayerChat->SetDisableEndTime(nLastTime);

		pPlayerChat->SyncDisableInfo();

	EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetChatDisableStartTime
	// @Description		: 设置开始禁别人言的时间
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetChatDisableStartTime(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		KPlayerChat* pPlayerChat = NULL;
		time_t tNow = KSysService::Time(NULL);

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
		LOG_PROCESS_ERROR(pPlayerChat);

		pPlayerChat->SetDisableStartTime(tNow);

EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetChatDisableStartTime
	// @Description		: 得到开始禁言的时间
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetChatDisableStartTime(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		KPlayerChat* pPlayerChat = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
		LOG_PROCESS_ERROR(pPlayerChat);

		sc.PushNumber(pPlayerChat->GetDisableStartTime());
EXIT0:
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: ModifyLastBroadcastTime
	// @Description		: 修改上次玩家个人公告时间
	// @ReturnCode		: 
	// @ArgumentFlag	: odd
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: nId
	// @ArgumentComment	: dwLastTime
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaModifyLastBroadcastTime(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		KPlayerChat* pPlayerChat = NULL;
		INT nId = sc.GetInt(2);
		DWORD dwLastTime = sc.GetInt(3);

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
		LOG_PROCESS_ERROR(pPlayerChat);

		pPlayerChat->ModifyBroadcastInfo(nId, dwLastTime);
EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLastBroadcastTime
	// @Description		: 得到上次玩家个人公告时间
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: nId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLastBroadcastTime(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		KPlayerChat* pPlayerChat = NULL;
		INT nId = sc.GetInt(2);
		DWORD dwLastTime = 0;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
		LOG_PROCESS_ERROR(pPlayerChat);

		dwLastTime = pPlayerChat->GetLastBroadcastTime(nId);
		sc.PushNumber(dwLastTime);
EXIT0:
		return 1;
	}

#endif

	#include "kchatscript_table.hpp"
}

BOOL g_RegisterChatScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("KChat", kchatscript::arFunction, countof(kchatscript::arFunction));
}



// -------------------------------------------------------------------------
