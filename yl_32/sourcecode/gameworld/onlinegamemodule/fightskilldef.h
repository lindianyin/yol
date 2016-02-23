#pragma once

#define FIGHT_SKILL_CHANGE_ROUTE_CD_TIME   3  // 30秒的切换技能路线CD时间

enum QINACTIVE_SKILL_ID
{
	emNEAR_SINGLE_SKILL_ID = 1,    // 近单
	emFAR_SINGLE_SKILL_ID = 5,     // 远单
	emNEAR_GROUP_SKILL_ID = 2,     // 近群
	emFAR_GROUP_SKILL_ID = 6,      // 远群
	emNEAR_CONTROL_SKILL_ID = 3,   // 近控技能id
	emFAR_CONTROL_SKILL_ID = 7,    // 远控技能id
	emNEAR_DEATH_SKILL_ID = 4,     // 近程必杀技能id
	emFAR_DEATH_SKILL_ID = 4,     //  远程必杀技能id
	emGOD_SINGLE_SKILL_ID = 11,   //  神化单体技能
	emGOD_GROUP_SKILL_ID = 12,    //  神化群体技能
	emGOD_DEATH_SKILL_ID = 13,    //  神化必杀技能
	emRED_SKILL_ID = 9,
	emBLUE_SKILL_ID = 10,
};

// 技能槽位
enum FIGHT_SKILL_SLOT
{
	emSINGLE_SKILL = 1, // 单技 1
	emRANGE_SKILL, // 群技能 2
	emCONTROL_SKILL, // 控技 3 
	emULTIMATE_SKILL, // 必杀技 4

	emRED_SKILL = 5, // 补血技能 5
	emBLUE_SKILL = 6, // 补蓝技能 6

	emACTIVE_SKILL_SLOTS_ALL_COUNT = 6,  // 主动技能个数

	emPASSIVE_SKILL = 7, // 被动技能 7
	emRESIST_SKILL = 8,  // 6个抗性技能
	emPASSIVE_SKILL_COUNT = 7, // 被动技能个数  7个加成6个抗性
	enRESIST_SKILL_COUNT = 6,  // 抗性技能个数

	emHELPER_SKILL_COUNT = emPASSIVE_SKILL_COUNT + enRESIST_SKILL_COUNT,  // 辅助技能
	emALL_SKILL_COUNT = emACTIVE_SKILL_SLOTS_ALL_COUNT + emPASSIVE_SKILL_COUNT,  // 总共多少个槽
};

enum MEDICINE_SKILL_TYPE
{
	emMEDICINE_SKILL_RED=1,
	emMEDICINE_SKILL_BLUE,
};

// 参考QE_PET_BASE_ATTR
// 被动技能对应属性
enum E_PASSIVE_SKILL_ATTR
{
	emSKILL_ATTR_LIFE=1,
	emSKILL_ATTR_ATTACK,
	emSKILL_ATTR_DEFENSE, // 防御
	emSKILL_ATTR_HIT,     // 命中
	emSKILL_ATTR_DODGE,   // 闪避
	emSKILL_ATTR_CRIT,  // 暴击
	emSKILL_ATTR_RESIST,  // 抗暴

	emSKILL_ATTR_COUNT=7,  // 总共多少个
};

enum E_RESIST_SKILL_ID
{
	emRESIST_SKILL_SINGLE=27, // 抗近单
	emRESIST_SKILL_FAR,		// 抗远单
	emRESIST_SKILL_NEAR_GROUP,  // 抗近群
    emRESIST_SKILL_FAR_GROUP, // 抗远群
	emRESIST_SKILL_NEAR_CONTROL,
	emRESIST_SKILL_FAR_CONTROL,

	emRESIST_SKILL_COUNT = 6,
};

enum MEDICINE_ITEN_INDEX
{
    emMEDICINE_ITEM_INDEX_GENRE = 18,
	emMEDICINE_ITEM_INDEX_DETAIL = 1,
	emMEDICINE_ITEM_INDEX_PARTICULAR = 22,
};