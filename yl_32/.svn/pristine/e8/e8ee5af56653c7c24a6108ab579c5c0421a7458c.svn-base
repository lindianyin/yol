#pragma once

#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kcharacterctrlbase.h"
#include "LuaPlayerArena.h"
#include "arenadef.h"
#include "config/kplayerbasedbdef.h"

class PlayerArena  : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_ARENA	};


	PlayerArena(KCharacter& rcPlayer);
	~PlayerArena();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 存盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 读盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);
	
	LuaPlayerArena* GetScriptInterface() CONST { return m_pLunaObj; }

private:
	KPlayer&		 m_rcPlayer;

	LuaPlayerArena* m_pLunaObj;


	INT m_nRankIdx;		// 自己在竞技场的排名

	BOOL m_bFirstLogin;

	BOOL m_bFrontArenaNpcsMode;				// 模式true，显示前五名，模式false，显示玩家前5
	std::map<DWORD, INT> m_mapRankPos;		// 角色id和竞技场的位置对应 位置从1开始，   0是玩家所在位置, 排名越高，pos越低,   实现从左到右，从大到小
	std::map<DWORD, INT> m_mapRankIdx;		// 角色id和排名的对应 排名从0开始

	std::map<DWORD, DWORD> m_mapNpc2Role;	// npcid和roleid对应表
	std::map<DWORD, DWORD> m_mapRole2Npc;
	std::map<DWORD, DWORD> m_mapNpc2Pets;  // Npc的宠物

	typedef std::map<DWORD, KDelayRoleData> KMAP_DELAY_DATA;
	KMAP_DELAY_DATA m_mapDelayData;

private:
	DWORD GetRoleIdByNpcId(DWORD dwNpcId);

	DWORD GetNpcIdByRoleId(DWORD dwRoleId);

	DWORD GetPetNpcIdByRoleNpcId(DWORD dwRoleNpcId);

	INT GetRankIdxByRoleId(DWORD dwRoleId);

	INT GetSkillIdOfCurrRound(INT nRound, INT nRoute);

	KDelayRoleData* GetDelayDataById(DWORD dwRoleId);

	VOID CalChallengeCDTime(INT nChallengeTime);

	VOID ProcessRankChangedWhenChallenge();

	VOID SetRankPos(DWORD dwRoleId, INT nRankIdx);

public:
	BOOL AddArenaRank();

	BOOL ClearNpcs();

	BOOL ResetData(); 

	BOOL StartArena();  // 进入竞技场时触发
	
	BOOL RefreshArenaPlayers();

	BOOL DoLoadArenaPlayers();

	BOOL DoLoadFrontArenaPlayers();

	BOOL DoGetPlayerIdxRequest(DWORD dwSrcPlayerId, DWORD dwRoleId);

	BOOL OnGetPlayerIdxRespond(DWORD dwRoleId, INT nRankIdx);

	BOOL OnGetPlayerIdRespond(DWORD dwRholeId, INT nRankIdx);

	BOOL OnLoadDelayRoleDataRespond(DWORD dwSrcId, DWORD dwRoleId, KDelayRoleData* pDelayData);

	BOOL CreateArenaNpc(DWORD dwRoleId, INT nPos, KDelayRoleData* pDelayData);

	BOOL OnSwapRankRespond(BOOL bSucceed);

private:
	DWORD m_dwPlayerRoleId;  // 主角的npc镜像ID
	DWORD m_dwCurrentFightRoleId;
	BOOL m_bFightResult;
	BOOL m_bCheckedAreaRecordFightResult; // 是否检查过离线期间的战斗记录（连胜清0）
	INT m_nWinCombo;	   // 连胜次数
    INT m_nFailCombo;      // 连败次数
	INT m_nFightNum;       // 战斗次数
	INT m_nCurrChallengeCDTime;  //当前CD时间

	DWORD m_dwLastTakeAwardTime;		// 上一次领取排名奖励的时间
	INT m_nTakeComboAward;		// 今天领取连胜奖励的连胜场次

	INT m_nCDTime;		// 挑战CD

	// 本场战斗过程记录
	std::list<KATTACK_RECORD> m_RecordList;

public:
	BOOL OnFightRequest(DWORD dwNpcId);

	BOOL DoFightProcess();

	BOOL SyncFightProcess();

	BOOL SyncArenaInfo();

	BOOL SyncAddNpc(DWORD dwRoleId, DWORD dwNpcId, DWORD dwPetNpcId,  INT nPos, BOOL bCanFight);

	INT GetCDTime() { return m_nCDTime; }

	VOID SetCDTime(INT nTime) { m_nCDTime = nTime; }

	VOID SetTakeAwardTime(DWORD dwTime) { m_dwLastTakeAwardTime = dwTime; }

	VOID SetWinCombo(INT nWinCombo){ m_nWinCombo = nWinCombo; }
	INT GetWinCombo() { return m_nWinCombo; }

	INT GetTakeComboAward() { return m_nTakeComboAward; }

	VOID SetTakeComboAward(INT nCombo) { m_nTakeComboAward = nCombo; }

private:
	struct KArenaSaveData
	{
		DWORD dwLastTakeAwardTime;
		WORD wCDTime;
		BYTE byTakeComboAward;
		WORD wCurrentChallengeCDTime;
		WORD wFightNum;
        BYTE byWinCombo;
	};

private:
	// 玩家竞技场战斗记录
	std::vector<KArenaRecord> m_vecArenaRecord;

public:
	BOOL OnLoadRecordRespond(BYTE* pRecordData, size_t uRecordDataLen);
	BOOL SyncArenaRecord();
};
