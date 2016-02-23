
#pragma once

#include <list>
#include <map>

#include "ProtocolBasic.h"

#ifdef GAME_SERVER

struct QTEAM_MEMBER_INFO;

struct QTeamBroadcast
{
	DWORD  m_dwTeamID;
	DWORD  m_dwExceptID;
	void*  m_pvData;
	size_t m_uSize;

	QTeamBroadcast();
	BOOL operator () (QTEAM_MEMBER_INFO* pMemberInfo);
};

struct QTEAM_MEMBER_INFO
{
    CHAR            szRoleName[_NAME_LEN];
    DWORD           dwMemberID;
    BOOL            bOnlineFlag;
    DWORD           dwMapID;
    int             nMapCopyIndex;
    DWORD           dwForceID;
    int             nLevel;
    int             nMaxLife; 
    int             nMaxMana; 
    int             nMaxRage;
    int             nLifePercent;
    int             nManaPercent;
    int             nRagePercent;
    int             nPosX;
    int             nPosY;
	int		        nRoleType;
    BOOL            bDeathFlag;
};

typedef std::list<QTEAM_MEMBER_INFO> QTEAM_MEMBER_LIST;

struct QTeamGroup
{
    DWORD               dwFormationLeader;
    QTEAM_MEMBER_LIST   MemberList;
};


struct QTeam
{
    BOOL        bSystem;
    DWORD       dwAuthority[tatTotal];
    int         nLootMode;
    int         nRollQuality;
    int         nGroupNum;
    DWORD       dwMark[TEAM_MARK_COUNT];
    QTeamGroup  MemberGroup[MAX_TEAM_GROUP_NUM];
};

typedef std::map<DWORD, QTeam> QTEAM_TABLE;

class QTeamServer
{
public:
    QTeamServer();

    BOOL Init();
    void UnInit();

    QTeam* GetTeam(DWORD dwTeamID);

    BOOL Create(DWORD dwTeamID);
    BOOL Destroy(DWORD dwTeamID);

    BOOL AddMember(DWORD dwTeamID, int nGroupIndex, DWORD dwPlayerID, BOOL bSystem = 0);
    BOOL DelMember(DWORD dwTeamID, int nGroupIndex, DWORD dwPlayerID);
    BOOL ChangeMember(
        DWORD dwTeamID, int nSrcGroupIndex, int nDstGroupIndex, DWORD dwSrcPlayerID, DWORD dwDstPlayerID
    );

    BOOL IsPlayerInTeam(DWORD dwTeamID, DWORD dwPlayerID);
    QTEAM_MEMBER_INFO* GetMember(DWORD dwTeamID, DWORD dwPlayerID);

    BOOL SetAuthority(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID);
    BOOL SetFormationLeader(DWORD dwTeamID, int nGroupIndex, DWORD dwTargetID);

    BOOL SetMark(DWORD dwTeamID, int nMarkType, DWORD dwTargetID);
    int GetMemberGroupIndex(DWORD dwTeamID, DWORD dwMemberID);

	BOOL TeamMessageNotifyToAll(DWORD dwTeamID, INT nMsgId, PVOID pData, UINT uDataLen);
	BOOL SendTeamInfoToAll(DWORD dwTeamID);

    template <class TFunc> BOOL Traverse(TFunc& Func);

private:
    QTEAM_TABLE m_TeamTable;
};

struct QGetAllTeamMember
{
    QGetAllTeamMember()
    {
        m_dwTeamID      = KD_BAD_ID;
        m_dwExceptID    = KD_BAD_ID;
        m_vecResult.clear();
    }

    DWORD m_dwTeamID;
    DWORD m_dwExceptID;
    std::vector<DWORD> m_vecResult;

    BOOL operator () (QTEAM_MEMBER_INFO* pMemberInfo);
};

template <class TFunc>
BOOL QTeamServer::Traverse(TFunc& Func)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    QTeam*  pTeam       = NULL;

    pTeam = GetTeam(Func.m_dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (int i = 0; i < pTeam->nGroupNum; i++)
    {
        for (
            QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
            it != pTeam->MemberGroup[i].MemberList.end(); ++it
        )
        {
            if (it->dwMemberID == Func.m_dwExceptID)
                continue;

            bRetCode = Func(&(*it));
            PROCESS_ERROR(bRetCode);
        }
    }

    bResult = true;
EXIT0:
    return bResult;
}

extern QTeamServer g_cTeamServer;

#endif // GAME_SERVER
