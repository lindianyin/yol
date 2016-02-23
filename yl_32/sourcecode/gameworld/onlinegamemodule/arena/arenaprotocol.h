#pragma once

#include "config/kplayerbasedbdef.h"
#include "onlinegameworld/kprotocol.h"
#include "arenadef.h"

#pragma pack(push, 1)
enum ARENA_S2C_PROTOCOL
{
	s2c_arena_protocol_begin = 0,
	s2c_sync_arena_info,
	s2c_arena_add_npc,
	s2c_sync_fight_process,
	s2c_sync_arena_record,

	s2c_arena_protocol_end,
};

enum ARENA_C2S_PROTOCOL
{
	c2s_arena_protocol_begin = 0,
	c2s_begin_fight_request,
	c2s_refresh_arena_players, // 主动刷新竞技场排名
	c2s_start_offline_fight_request,
	c2s_arena_protocol_end,
};

struct KS2C_SYNC_ARENA_INFO : KPTC_HEADER<s2c_sync_arena_info>
{
	BYTE byChallengeTimes;		// 挑战次数
	BYTE byWinCombo;			// 连胜次数
	DWORD dwCDOverTime;				// 挑战CD时间
	BYTE byComboAward;			// 领取连胜奖励的场次

	DWORD dwCurRankAwardId;		// 当前排名奖励Id
	DWORD dwCurWinComboAwardId; // 当前连胜奖励id
};

struct KS2C_SYNC_ARENA_RECORD : KLENPTC_HEADER<s2c_sync_arena_record>
{
	INT nRecordNums;
	KArenaRecord arrRecord[0];
};

struct KS2C_ARENA_ADD_NPC : KPTC_HEADER<s2c_arena_add_npc>
{
	DWORD dwMappingRoleId;
	DWORD dwNpcId;
	DWORD dwPetNpcId;
	BYTE byPos;
	BYTE byCanFight;
};

struct KS2C_SYNC_FIGHT_PROCESS : KLENPTC_HEADER<s2c_sync_fight_process>
{
	BYTE bResult;
	DWORD dwFightNpcId;
	INT nMeSkillId;
	INT nMePetSkillId;
	INT nHimPetNpcId;
	INT nHimPetSkillId;

	INT nRecordCount;
	KATTACK_RECORD arrRecord[0];
};

struct QC2S_REFRESH_ARENA_PLAYERS : KPTC_HEADER<c2s_refresh_arena_players>
{

};

struct KC2S_BEGIN_FIGHT_REQUEST : KPTC_HEADER<c2s_begin_fight_request>
{
	DWORD dwNpcId;
};

struct KC2S_START_OFFLINE_FIGHT_REQUEST : KPTC_HEADER<c2s_start_offline_fight_request>
{
	DWORD dwStartPlayerId;
	DWORD dwOfflinePlayerId;
	DWORD dwStartRoleNpcId;
	DWORD dwStartPetNpcId;
	DWORD dwOfflineRoleNpcId;
	DWORD dwOfflinePetNpcId;
};


#pragma pack(pop)
