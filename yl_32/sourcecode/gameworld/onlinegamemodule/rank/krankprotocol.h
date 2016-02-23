
#ifndef	__KRANKPROTOCOL_H__
#define __KRANKPROTOCOL_H__

#include "onlinegameworld/kprotocol.h"
#include "rankdef.h"

#pragma pack(1)

enum KC2S_RANK_PROTOCOL
{
	c2s_rank_protocol_start = 0,
	c2s_get_lastupdatetime_request,
	c2s_get_rank_request,
};

enum KS2C_RANK_PROTOCOL
{
	s2c_sync_lastupdatetime = 0,
	s2c_sync_arena,
	s2c_sync_role_rank,			// 发送战斗力榜
	s2c_sync_pet_lv_rank,
	s2c_sync_pet_fightscore_rank,
	s2c_sync_ride_rank,
	s2c_sync_mpjj_rank,
	s2c_sync_lg_rank,
	s2c_sync_smzc_rank,

	s2c_sync_tong_fs_rank,
	s2c_sync_tong_sr_rank,

	//下面为 开服活动发送的排行榜只发前10名
	s2c_sync_player_level_open_server = 11,//玩家等级
	s2c_sync_ride_lv_open_server,//坐骑等级
	s2c_sync_arean_open_server,//竞技场
	s2c_sync_pet_lv_open_server,//宠物等级
	s2c_sync_lg_open_server,//炼卦
	s2c_sync_tower_open_server,//伏魔塔
	s2c_role_fightscore_open_server,//战斗力
	s2c_sync_end,//同步结束
};

struct KC2S_GET_ROLE_RANK : KPTC_HEADER<c2s_get_rank_request>
{
	INT nRankId;
};

struct KC2S_GET_LASTUPDATETIME_REQUEST : KPTC_HEADER<c2s_get_lastupdatetime_request>
{
	INT nRankId;
};


struct KS2C_SYNC_LASTUPDATETIME : KPTC_HEADER<s2c_sync_lastupdatetime>
{
	INT nRankId;
	DWORD dwLastUpdateTime;
};

struct KS2C_SYNC_ROLE_RANK : KLENPTC_HEADER<s2c_sync_role_rank>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_AREAN_OS : KLENPTC_HEADER<s2c_sync_arean_open_server>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_ROLE_FIGHTSCORE_OS : KLENPTC_HEADER<s2c_role_fightscore_open_server>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_PET_LV_RANK : KLENPTC_HEADER<s2c_sync_pet_lv_rank>
{
	INT nPetRankNum;
	INT nMeRank;
	KPetRankInfo arrRanks[];
};

struct KS2C_SYNC_PET_LV_OS : KLENPTC_HEADER<s2c_sync_pet_lv_open_server>
{
	INT nPetRankNum;
	INT nMeRank;
	KPetRankInfo arrRanks[];
};

struct KS2C_SYNC_PET_FS_RANK : KLENPTC_HEADER<s2c_sync_pet_fightscore_rank>
{
	INT nPetRankNum;
	INT nMeRank;
	KPetRankInfo arrRanks[];
};

struct KS2C_SYNC_RIDE_RANK : KLENPTC_HEADER<s2c_sync_ride_rank>
{
	INT nRideRankNum;
	INT nMeRank;
	KRideRankInfo arrRanks[];
};

struct KS2C_SYNC_RIDE_LV_OS : KLENPTC_HEADER<s2c_sync_ride_lv_open_server>
{
	INT nRideRankNum;
	INT nMeRank;
	KRideRankInfo arrRanks[];
};

struct KS2C_SYNC_MPJJ_RANK : KLENPTC_HEADER<s2c_sync_mpjj_rank>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_LG_RANK : KLENPTC_HEADER<s2c_sync_lg_rank>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_LG_OS : KLENPTC_HEADER<s2c_sync_lg_open_server>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_SMZC_RANK : KLENPTC_HEADER<s2c_sync_smzc_rank>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_TONG_FS_RANK : KLENPTC_HEADER<s2c_sync_tong_fs_rank>
{
	INT nTongRankNum;
	INT nMeRank;
	KTongRankInfo arrRanks[];
};

struct KS2C_SYNC_TONG_SR_RANK : KLENPTC_HEADER<s2c_sync_tong_sr_rank>
{
	INT nTongRankNum;
	INT nMeRank;
	KTongRankInfo arrRanks[];
};

struct KS2C_SYNC_PLAYER_LEVEL_OPEN_SERVICE : KLENPTC_HEADER<s2c_sync_player_level_open_server>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};

struct KS2C_SYNC_TOWER_OPEN_SERVICE : KLENPTC_HEADER<s2c_sync_tower_open_server>
{
	INT nRoleRankNum;
	INT nMeRank;
	KRoleRankInfo arrRanks[];
};


struct KS2C_SYNC_END : KLENPTC_HEADER<s2c_sync_end>
{
	INT nRankId;
};







#pragma pack()

#endif
