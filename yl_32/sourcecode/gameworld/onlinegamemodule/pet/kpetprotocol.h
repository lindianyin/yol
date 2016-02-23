/* -------------------------------------------------------------------------
//	文件名		：	kpetprotocol.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-11-28 18:13:00
//	功能描述	：  
// -------------------------------------------------------------------------*/

#pragma once

#include "onlinegameworld/kprotocol.h"
#include "petdef.h"

#pragma pack(1)

enum KS2C_PET_PROTOCOL
{
	s2c_pet_begin = 0, 

	s2c_sync_pet_info,	// 发送具体宠物信息
	s2c_sync_pet_state, // 发送改变状态结果
	s2c_sync_pet_equip,	// 同步宠物装备信息
	s2c_sync_free_result,	// 发送放生结果
	s2c_sync_pet_exp,	// 同步宠物经验
	s2c_pet_cast_skill,	// 释放技能
	s2c_pet_island_refresh_rate, // 宠物刷新成功率
	s2c_pet_island_refresh_petcards, // 同步刷出的神仙卡

	s2c_takeoff_ride,
	s2c_sync_ride_state,
	s2c_sync_ride_info,
	s2c_sync_ride_level,	// 同步坐骑等级信息
};

enum KC2S_PET_PROTOCOL
{
	c2s_pet_begin = 0,

	c2s_get_petinfo_request,		// 获得具体宠物信息
	c2s_change_petstate_request,	// 设置宠物状态请求
	c2s_pet_free_request,	// 放生宠物
	c2s_resize_pet_bag_request,		// 开宠物格子
	c2s_change_character_request,  // 驯化
	c2s_foster_request,			// 培养
	c2s_feed_request,		// 喂食
	c2s_up_rank_request, // 升阶
	c2s_change_name_request,
	c2s_puton_equip_request, // 宠物穿装备
	c2s_takeoff_equip_request, // 脱装备
	c2s_trans_exp_request,
	c2s_change_pet_tactic_request,
	c2s_compose_pet_card,     // 神仙卡合成道具
	
	c2s_pet_skill_state,    // 宠物释放技能状态

	c2s_get_rideinfo_request, // 获得坐骑的信息
	c2s_takeoff_ride_request, // 脱下坐骑
	c2s_ride_uprank_request, // 坐骑升阶
	c2s_change_ride_state, // 改变坐骑的状态
	c2s_feed_ride, // 喂食饲料
	c2s_add_ride_magicrate, // 喂食幻化果
	c2s_change_ride_represent, // 改变坐骑的表现
};

// s2c
struct KS2C_SYNC_PET_INFO : KPTC_HEADER<s2c_sync_pet_info>
{
	DWORD dwId;
	DWORD dwOwnerId;
	WORD wType;
	BYTE byQuality;
	BYTE byCharacter;
	CHAR szName[MAX_NAME_LEN];
	BYTE byLevel;
	BYTE byRankLevel;
	BYTE byState;
	BYTE byHungry;
	BYTE byEquipSlot;
	BYTE byTactic;
	DWORD dwFightScore;
	DWORD dwExp;
	//DWORD dwRandSeed;
	DWORD awSkill[emPET_COUNT_SKILL];
	DWORD  wSkillValue1[emPET_COUNT_SKILL * 2];  // 技能数值1
	DWORD  wSkillValue2[emPET_COUNT_SKILL * 2];  // 技能数值2
	DWORD  wSkillValue3[emPET_COUNT_SKILL * 2];  // 技能数值2
	
	DWORD awAttrValue[7];	// 基础属性//change
	LONGLONG awTacticValue[2];	// 阵位属性(当前和下一阶属性)//change
	DWORD awEquip[emPET_COUNT_EQUIP];//change
	BYTE abyEquipLockFlags[emPET_COUNT_EQUIP];
	WORD wbyNumberIndex[emPET_COUNT_EQUIP];
	INT  nRenameCount;
};

struct KS2C_SYNC_RIDE_INFO : KPTC_HEADER<s2c_sync_ride_info>
{
	DWORD dwId;
	DWORD dwOwnerId;
	BYTE byGenre;
	BYTE byDetail;
	WORD wParticular;
	BYTE byLevel;
	BYTE byHungry;
	BYTE byState;
	WORD wMagicRate;
	WORD wRepresentId;
	DWORD dwFightScore[2];
};

struct KS2C_SYNC_PET_STATE : KPTC_HEADER<s2c_sync_pet_state>
{
	DWORD dwId;
	DWORD dwOwnerId;
	BYTE byState;
};

struct KS2C_SYNC_PET_EQUIP : KPTC_HEADER<s2c_sync_pet_equip>
{
	DWORD dwId;
	WORD arywEquip[emPET_COUNT_EQUIP];
	BYTE abyEquipLockFlags[emPET_COUNT_EQUIP];
	WORD wbyNumberIndex[emPET_COUNT_EQUIP];
};

struct KS2C_SYNC_PET_EXP : KPTC_HEADER<s2c_sync_pet_exp>
{
	DWORD dwId;
	DWORD dwExp;
};

struct QS2C_PET_ISLAND_REFRESH_RATE : KPTC_HEADER<s2c_pet_island_refresh_rate>
{
	INT nRefreshProgress;
	INT nCurQuality;
};

struct KS2C_PET_CAST_SKILL : KPTC_HEADER<s2c_pet_cast_skill>
{
	DWORD dwId;
	DWORD dwTargetId;
	WORD wSkillId;
	WORD wMissileTime;
};

struct QS2C_PET_CARDS : KPTC_HEADER<s2c_pet_island_refresh_petcards>
{
	USHORT petInfos[20];
};

struct KS2C_SYNC_FREE_RESULT : KPTC_HEADER<s2c_sync_free_result>
{
	DWORD dwId;
	BYTE bResult;
};

struct KS2C_TAKEOFF_RIDE : KPTC_HEADER<s2c_takeoff_ride>
{
	DWORD dwRideId;
};

struct KS2C_SYNC_RIDE_STATE : KPTC_HEADER<s2c_sync_ride_state>
{
	DWORD dwRideId;
	BYTE byState;
};

struct KS2C_SYNC_RIDE_LEVEL : KPTC_HEADER<s2c_sync_ride_level>
{
	DWORD dwRideId;
	BYTE byLevel;
};


// c2s
struct KC2S_GET_PETINFO_REQUEST : KPTC_HEADER<c2s_get_petinfo_request>
{
	DWORD dwCharacterId;
};

struct KC2S_CHANGE_PETSTATE_REQUEST : KPTC_HEADER<c2s_change_petstate_request>
{
	DWORD dwId;
	BYTE byState;
};

struct KC2S_PET_FREE_REQUEST : KPTC_HEADER<c2s_pet_free_request>
{
	DWORD dwId;
};

struct KC2S_RESIZE_PET_BAG_REQUEST : KPTC_HEADER<c2s_resize_pet_bag_request>
{

};

struct KC2S_CHANGE_CHARACTER_REQUEST : KPTC_HEADER<c2s_change_character_request>
{
	DWORD dwId;
	BYTE byCharacter;
};

struct KC2S_CHANGE_NAME_REQUEST : KPTC_HEADER<c2s_change_name_request>
{
	DWORD dwId;
	CHAR szName[MAX_NAME_LEN];
};

struct KC2S_UP_RANK_REQUEST : KPTC_HEADER<c2s_up_rank_request>
{
	DWORD dwId;
};

struct KC2S_FOSTER_PET_REQUEST : KPTC_HEADER<c2s_foster_request>
{
	DWORD dwId;
	BYTE byAmount;
};

struct KC2S_FEED_PET_REQUEST : KPTC_HEADER<c2s_feed_request>
{
	DWORD dwPetId;
	DWORD dwItemId;
};

struct KC2S_PUTON_EQUIP_REQUEST : KPTC_HEADER<c2s_puton_equip_request>
{
	DWORD dwPetId;
	DWORD dwItemId;
	BYTE byPos;
};

struct KC2S_TAKEOFF_EQUIP_REQUEST : KPTC_HEADER<c2s_takeoff_equip_request>
{
	DWORD dwPetId;
	BYTE byPos;
};

struct KC2S_TRANS_EXP_REQUEST : KPTC_HEADER<c2s_trans_exp_request>
{
	DWORD dwSrcPetId;
	DWORD dwDstPetId;
	BYTE  byUseGold; // 使用元宝
};

struct KC2S_CHANGE_PET_TACTIC_REQUEST : KPTC_HEADER<c2s_change_pet_tactic_request>
{
	DWORD dwPetId;
	BYTE byCharacter;
};

struct QC2S_COMPOSE_PET_CARD : KPTC_HEADER<c2s_compose_pet_card>
{
	DWORD dwPetIds[12];
};

struct KC2S_PET_SKILL_STATE : KPTC_HEADER<c2s_pet_skill_state>
{
	DWORD dwPetId;
	DWORD dwCharacterId;
	INT nPetPosX;
	INT nPetPosY;
};

struct KC2S_GET_RIDEINFO_REQUEST : KPTC_HEADER<c2s_get_rideinfo_request>
{
	DWORD dwCharacterId;
};

// 坐骑卸下变成一个道具
struct KC2S_TAKEOFF_RIDE_REQUEST : KPTC_HEADER<c2s_takeoff_ride_request>
{
	DWORD dwRideId;
};

struct KC2S_RIDE_UPRANK_REQUEST : KPTC_HEADER<c2s_ride_uprank_request>
{
	DWORD dwRideId;
};

struct KC2S_CHANGE_RIDE_STATE : KPTC_HEADER<c2s_change_ride_state>
{
	DWORD dwRideId;
	BYTE byState;
};

struct KC2S_FEED_RIDE : KPTC_HEADER<c2s_feed_ride>
{
	DWORD dwRideId;
	BYTE byCostNum;
};

struct KC2S_ADD_RIDE_MAGICRATE : KPTC_HEADER<c2s_add_ride_magicrate>
{
	DWORD dwRideId;
	INT   nCostNum;
};

struct KC2S_CHANGE_RIDE_REPRESENT : KPTC_HEADER<c2s_change_ride_represent>
{
	DWORD dwRideId;
	INT   nRepresentId;
};

#pragma pack()