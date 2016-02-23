
#pragma once

#include <list>
#include <map>
#include "config/gamedef.h"

class KRole;

#define EMPTY_TEAM_TIME_OUT         (60 * 60) // 一个小时
#define LOGOUT_TEAM_TIME_OUT			(60 * 5) // 掉线5分钟
#define INVITE_AND_APPLY_TIME_OUT   (30) // 30秒

struct KTEAM_MEMBER_INFO
{
    DWORD dwMemberID;
    BOOL  bIsOnline;
};

typedef std::list<KTEAM_MEMBER_INFO> KTEAM_MEMBER_LIST;

struct KGroup 
{
    DWORD             dwFormationLeader;
    KTEAM_MEMBER_LIST MemberList;
};

struct KTeam
{
    BOOL    bDelete;
    BOOL    bSystem;
    DWORD   dwAuthority[tatTotal];
    //KCAMP   eCamp;
    int     nLootMode;
    int     nRollQuality;
    int     nGroupNum;
    time_t  nLastActiveTime;
    DWORD   dwTeamMark[TEAM_MARK_COUNT];
    KGroup  MemberGroup[MAX_TEAM_GROUP_NUM];
};

typedef std::map<DWORD, KTeam, std::less<DWORD> > KTEAM_TABLE;

class QTeamCenter
{
public:
    QTeamCenter();

    BOOL Init();
    void UnInit();

    void Activate();

    DWORD CreateTeam(BOOL bSystem);
    BOOL DestroyTeam(DWORD dwTeamID);

    KTeam* GetTeam(DWORD dwTeamID);

    BOOL AddMember(DWORD dwTeamID, DWORD dwRoleID, BOOL bSystem = 0);
    BOOL DelMember(DWORD dwTeamID, DWORD dwRoleID);
    BOOL ChangeGroup(DWORD dwTeamID, int nDstGroupIndex, DWORD dwDstRoleID, DWORD dwRoleID);

    BOOL SetAuthority(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID);
    BOOL SetFormationLeader(DWORD dwTeamID, int nGroupIndex, DWORD dwRequestSrcID, DWORD dwTargetID);

    BOOL SetMark(DWORD dwTeamID, int nMarkType, DWORD dwTargetID);

    void SyncNewServer(int nConnIndex);

    int GetTeamMemberCount(DWORD dwTeamID);
    BOOL IsRoleInTeam(DWORD dwTeamID, DWORD dwRoleID);

    BOOL RegisterInvite(DWORD dwInviteSrc, DWORD dwInviteDst);
    BOOL UnregisterInvite(DWORD dwInviteSrc, DWORD dwInviteDst);
    BOOL ClearInvite(DWORD dwInviteSrc);
    BOOL IsInviteExist(DWORD dwInviteSrc, DWORD dwInviteDst);

    BOOL RegisterApply(DWORD dwApplySrc, DWORD dwApplyDst);
    BOOL UnregisterApply(DWORD dwApplySrc, DWORD dwApplyDst);
    BOOL ClearApply(DWORD dwApplySrc);
    BOOL IsApplyExist(DWORD dwApplySrc, DWORD dwApplyDst);

    int GetTeamCount();

	void ProcessTeam(DWORD dwTeamID, KTeam* pTeam);
private:
    KTEAM_TABLE m_TeamTable;

    DWORD       m_dwTeamSNGenerator;

    typedef std::map<INT64, time_t> KINVITE_TABLE;
    KINVITE_TABLE m_InviteTable;

    typedef std::map<INT64, time_t> KAPPLY_TABLE;
    KAPPLY_TABLE m_ApplyTable;

private:
    void ProcessAuthority(DWORD dwTeamID, KTeam* pTeam);
    void ProcessFormation(DWORD dwTeamID, KTeam* pTeam);
    void ProcessMember(DWORD dwTeamID, KTeam* pTeam);
    void ProcessInviteAndApplyTimeOut();

    void OnChangeLeader(DWORD dwLeaderID);

    void SetRoleTeamID(KRole* pRole, BOOL bSystem, DWORD dwTeamID);
};


