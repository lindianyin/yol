#ifndef __KINSTANCECTRL_H__
#define __KINSTANCECTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "instancedef.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"

class KPlayer;

class KInstanceCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_INSTANCE,	};


public:

	explicit KInstanceCtrl(KCharacter& rcPlayer);
	virtual ~KInstanceCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }
	virtual BOOL Active();
	virtual BOOL OnLogout();

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

public:
	BOOL SyncMultiInsInfo();

	BOOL SyncOverallTeams(BYTE byTeamsNum, KTEAM_INFO* pTeams);

	BOOL SyncPlayerOverallTeam(DWORD dwLeaderCombinedIdx, INT nMemberNum, INT nRequireFightscore, KRS_MEMBER_INFO* arrMembers);

	BOOL SyncOverallTeamList(LIST_OVERALL_TEAMS& listTeams);

	BOOL DoSyncOverallTeamopResult(BYTE byTeamsOp, BYTE byResult);

	BOOL DoMemberCountDownNotify();

	VOID DoLeaveOverallTeam();

private:

	VOID ResetMultiInfo();

private:
	KPlayer& m_rcPlayer;	// 所属玩家

public:
	KMultiInstanceInfo sMultiInfo;

private:
	std::map<INT, INT> m_mapPassMultiMapIds;	// 记录已经通关的多人副本

public:
	VOID SetMultiMapPassed(INT nMapId);

	BOOL IsMultiMapPassed(INT nMapId);

public:
	DWORD m_dwOverallCombinedIdx;		// 记录自己的排队idx

};

#endif