/* -------------------------------------------------------------------------
//	文件名		：	kplayerchatchannel_gs.h
//	创建者		：	zhangdeheng
//	创建时间	：	2009-11-30 14:46:15
//	功能描述	：
//
// -----------------------------------------------------------------------*/
#ifndef __KGS_PLAYERCHATCHANNEL_H__
#define __KGS_PLAYERCHATCHANNEL_H__

#ifdef KD_OP_GAME_SERVER
#include"stdafx.h"
#include <vector>
#include "gc_core/kchatchanneldef.h"
#include "gc_core/ikplayerchatchannel.h"
// -------------------------------------------------------------------------
class KEnumPlayer
{
public:
	typedef std::vector<INT> KT_PLAYERSET;

public:
	static KEnumPlayer* GetEnumPlayer();

public:
	BOOL Push(INT nIndex)
	{
		m_mpPlayerSet.push_back(nIndex);
		return TRUE;
	}
	BOOL Clear()
	{
		m_mpPlayerSet.clear();
		m_nCurPlayer = 0;
		return TRUE;
	}
	INT Next()
	{
		INT nIndex = 0;
		if (m_nCurPlayer < m_mpPlayerSet.size())
		{
			nIndex = m_mpPlayerSet[m_nCurPlayer];
		}
		m_nCurPlayer++;
		return nIndex; 
	}
protected:
	static KEnumPlayer m_scInst;
private:
	KT_PLAYERSET		m_mpPlayerSet;
	UINT				m_nCurPlayer;
};

class KGS_PlayerChatChannel : public IKPlayerChatChannel
{
public:
	KGS_PlayerChatChannel();
	~KGS_PlayerChatChannel() {};

	virtual BOOL Init() { return TRUE; };

	virtual BOOL CheckCanUseChannel(INT nPlayerId, INT nChannelId);

	virtual BOOL CheckChannelMsgInterval(INT nPlayerId, INT nChannelId, INT nIntervalTime);

	KEnumPlayer* GetEnumPlayer(INT nPlayerId, INT nChannelId);

protected:
	KEnumPlayer* GetGMPlayer(INT nPlayerId);
	KEnumPlayer* GetTeamPlayer(INT nPlayerId);
	KEnumPlayer* GetFactionPlayer(INT nPlayerId);
	KEnumPlayer* GetKinPlayer(INT nPlayerId);
	KEnumPlayer* GetTongPlayer(INT nPlayerId);
	KEnumPlayer* GetCityPlayer(INT nPlayerId);
	KEnumPlayer* GetWorldPlayer(INT nPlayerId);
	KEnumPlayer* GetFriendPlayer(INT nPlayerId);
	KEnumPlayer* GetServerPlayer(INT nPlayerId);

private:
	KEnumPlayer* (KGS_PlayerChatChannel::*GetPlayer[emKCHANNEL_ALL])(INT nPlayerId);
};

// -------------------------------------------------------------------------

#endif /* _SERVER */
#endif /* __KGS_PLAYERCHATCHANNEL_H__ */
