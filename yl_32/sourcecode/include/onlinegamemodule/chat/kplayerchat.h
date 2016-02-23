
#pragma once

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "kluaplayerchat.h"
#include "gcmodule/chat/kchanneldef.h"

class KPlayer;
// -------------------------------------------------------------------------
class KPlayerChat : public KCharacterCtrlBase
{
public:
	enum { emKCtrlId = emKCHARACTER_CTRL_PLAYER_CHAT };
	KPlayerChat(KCharacter& rcPlayer);

public:
	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL OnLogin();
	virtual BOOL OnLogout();

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	CONST KLuaPlayerChat& GetScriptInterface() CONST;
	
	BOOL SendMsg(INT nChannelId, INT nReceiverId, LPCSTR pszMessage, BYTE byShownType = emQCHAT_PANEL);

#ifdef GAME_CLIENT
	BOOL SendMsgToPlayer(INT nChannelId, const char* pszReceiver, LPCSTR pszMessage);
#endif //#ifdef GAME_CLIENT

private:
	struct KCHAT_DATE 
	{
		INT nYear;
		INT nMonth;
		INT nMonDay;
	};
	BOOL IsToday(time_t tTime);

private:
	KLuaPlayerChat m_cLunaObj;
	KPlayer& m_rcPlayer;

private:
	// 禁言结构
	struct KCHAT_DISABLE
	{
		BOOL bDisableAuthority;		// 是否有禁言权限
		INT nDisableChannel;		// 被禁言的频道
		DWORD dwDisableStartTime;		// 开始禁别人言的时间，用来计算禁言时间间隔
		DWORD dwDisableEndTime;			// 禁言结束时间
	};

	KCHAT_DISABLE sChatDisable;

public:
	VOID SetDisableAuthority(BOOL bEnable) { sChatDisable.bDisableAuthority = bEnable; }

	BOOL HasDisableAuthority() { return sChatDisable.bDisableAuthority; }

	VOID SetDisableChannel(INT nChannel) { sChatDisable.nDisableChannel = nChannel; }

	INT GetDisableChannel() { return sChatDisable.nDisableChannel; }

	VOID SetDisableEndTime(INT nLastTime);

	DWORD GetDisableEndTime() { return sChatDisable.dwDisableEndTime; }

	VOID SetDisableStartTime(DWORD dwTime) { sChatDisable.dwDisableStartTime = dwTime; }

	DWORD GetDisableStartTime() { return sChatDisable.dwDisableStartTime; }

	BOOL SyncDisableInfo();

	BOOL IfCanSendMsg(INT nChannelId);

private:
	struct KBROADCAST_INFO
	{
		DWORD dwLastBroadcastTime;
	};

	std::map<INT, KBROADCAST_INFO> m_mapBroadcastInfo;		// 公告信息，是否已经发公告

public:
	VOID ModifyBroadcastInfo(INT nId, DWORD dwBroadcastTime);
	
	DWORD GetLastBroadcastTime(INT nId);
};

