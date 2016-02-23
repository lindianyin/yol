#ifndef __KRANKCTRL_H__
#define __KRANKCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "rankdef.h"

class KPlayer;

class KRankCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_RANK,	};


public:

	explicit KRankCtrl(KCharacter& rcPlayer);
	virtual ~KRankCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }
	virtual BOOL Active();
	virtual BOOL OnLogin();

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

public:
	//同步"个人排行"的"战斗力榜"
	BOOL SyncRoleRanks();
	
	BOOL SyncPetLvRanks();
	BOOL SyncPetFSRanks();
	BOOL SyncRideRanks();
	BOOL SyncMpjjRanks();
	BOOL SyncLgRanks();
	BOOL SyncSmzcRanks();
	BOOL SyncTongFSRanks();
	BOOL SyncTongSrRanks();


	
	BOOL SyncLastUpdateTime(INT nRankId);

	
	//同步"全民冲级"活动
	BOOL SyncRankRoleLvOfOpenServer();
	BOOL SyncRankRideLV();
	BOOL SyncRankArean();
	BOOL SyncPetLV();
	BOOL SyncLGRank_OS();
	BOOL SyncRankTower();
	BOOL SyncRoleFightScore_OS();

private:
	INT GetRoleRankNum();

	INT GetPetLvRankNum();

	INT GetPetFSRankNum();
	
	INT GetRideRankNum();

private:
	KPlayer& m_rcPlayer;	// 所属玩家

private:
	std::map<INT, DWORD> m_mapLastTakeAwardTime;		// 上次领奖时间

public:
	VOID InitLastTakeAwardTime();

	BOOL SetLastTakeAwardTime(INT nRankId, DWORD dwTime);

	DWORD GetLastTakeAwardTime(INT nRankId);


};

#endif