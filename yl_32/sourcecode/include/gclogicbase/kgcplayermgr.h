/* -------------------------------------------------------------------------
//	文件名		：	kgcplayermgr.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/30 14:47:36
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __GCLOGICBASE_KGCPLAYERMGR_H__
#define __GCLOGICBASE_KGCPLAYERMGR_H__

#include "Misc/objectpool.h"

// ------------------------------------------------------------------------
template<typename TGcPlayer>
class KGcPlayerMgr
{
public:
	struct CmpId : public std::binary_function<DWORD, DWORD, bool>
	{
		bool operator()(DWORD dwId1, DWORD dwId2) CONST
		{ 
			return dwId1 < dwId2;
		}
	};

public:
	// 增加玩家，如果已存在则返回原有的
	TGcPlayer* AddPlayer(DWORD dwPlayerId)
	{
		TGcPlayer& rcPlayer = m_mpGcPlayer[dwPlayerId];
		rcPlayer.Reset();
		rcPlayer.SetId(dwPlayerId);
		return &rcPlayer;
	}
	// 根据玩家ID获取玩家
	TGcPlayer* GetPlayer(DWORD dwPlayerId)
	{
		typename TMAP_GCPLAYER::iterator it = m_mpGcPlayer.find(dwPlayerId);
		if (it != m_mpGcPlayer.end())
			return &it->second;
		return NULL;
	}

protected:
	typedef std::map<DWORD, TGcPlayer, CmpId> TMAP_GCPLAYER;
	TMAP_GCPLAYER m_mpGcPlayer;
};

// -------------------------------------------------------------------------

#endif /* __GCLOGICBASE_KGCPLAYERMGR_H__ */
