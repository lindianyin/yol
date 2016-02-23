#ifndef __KESCORTCTRL_H__
#define __KESCORTCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegamemodule/kescortdef.h"
#include "config/kplayerbasedbdef.h"

class KPlayer;

class KEscortCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_ESCORT,	};


public:

	explicit KEscortCtrl(KCharacter& rcPlayer);
	virtual ~KEscortCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }
	virtual BOOL Active();

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

public:
	VOID ClearEscortData(BOOL bClearSelf = TRUE);

	BOOL DoRefreshEscortNpcs();

	BOOL DoRefreshEscortNpcs2();

	BOOL DoRefreshEscortNpcs3();

	BOOL RemoveEscortNpc(DWORD dwPlayerId, DWORD dwNpcId);

	INT GetTotalEscortPage();

	BOOL OnSyncEscortPlayers(INT nMembersNum, KPlayerEscort* pEscort);

	BOOL InsertEscortPlayer(INT nPlayerId);

	BOOL OnSyncRobEscortPlayers(INT nMembersNum, KRobEscort* pRobEscort);

	BOOL GetPlayerEscort(INT nPlayerId, KPlayerEscort& sEscort);

	BOOL OnLoadDelayRoleDataRespond(DWORD dwSrcId, DWORD dwRoleId, KDelayRoleData* pDelayData);

	BOOL CreateSelfNpc(KDelayRoleData* pDelayData);

	BOOL CreateEscortNpc(DWORD dwRoleId, KDelayRoleData* pDelayData);

	BOOL OnRobFightRequest(INT nRobPlayerId);

	BOOL OnRescueFightRequest(INT nRobPlayerId);

	BOOL SyncAddNpc(DWORD dwRoleId, DWORD dwNpcId, DWORD dwPetNpcId, INT nPos, BOOL bCanFight);

	BOOL SyncRemoveNpc(DWORD dwNpcId, BOOL bIsPet = FALSE);

	BOOL DoSyncEscortMembers();

	BOOL DoSyncRobEscortMembers();

	BOOL SyncAddSelfNpc(DWORD dwRoleId, DWORD dwNpcId, DWORD dwPetNpcId);

	BOOL ClearNpcs();

	BOOL OnUpdatePlayerEscortState(INT nPlayerId, INT nEscortState, INT nLeftTime, DWORD dwRoberId, DWORD dwRoberFightscore, LPCTSTR szRoberName);

private:
	std::map<DWORD, DWORD> m_mapRole2Npcs;
	std::map<DWORD, DWORD> m_mapRole2PetNpcs;

	std::map<DWORD, KPlayerEscort> m_mapPlayerEscorts;

	std::map<DWORD, KRobEscort> m_mapRobEscorts;

	std::map<DWORD, INT> m_mapRole2Idxs;
	DWORD m_arrRoleIdxs[MAX_ESCORT_POS_NUM];

	INT m_nCurEscortPage;	// 当前页码
	INT m_nLastEscortPage;

private:
	DWORD m_dwLastRefreshTime;	// 上次刷新坐骑的时间

	DWORD m_dwCurFightPlayerId;		// 当前劫驾要挑战的玩家id

	DWORD m_dwCurSelfNpcId;
	DWORD m_dwCurSelfPetNpcId;

public:
	VOID SetLastRefreshTime(DWORD dwTime) { m_dwLastRefreshTime = dwTime; }

	DWORD GetLastRefreshTime() { return m_dwLastRefreshTime; }

	VOID SetCurEscortPage(INT nPage) { m_nLastEscortPage = m_nCurEscortPage; m_nCurEscortPage = nPage; }

private:
	DWORD GetNpcIdByRoleId(DWORD dwRoleId);
	DWORD GetPetNpcIdByRoleId(DWORD dwRoleId);

private:
	KPlayer& m_rcPlayer;	// 所属玩家

};

#endif