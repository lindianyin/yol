#ifndef PETDEF_H__
#define PETDEF_H__

#define PET_TABLE_FILE "\\setting\\pet\\pet.txt"
#define PET_SKILL_TABLE_FILE "\\setting\\pet\\petskill.txt"
#define PET_ATTR_SCORE_FILE "\\setting\\pet\\petattrscore.txt"
#define RIDE_ATTR_SCORE_FILE "\\setting\\pet\\rideattrscore.txt"

#define RIDE_TABLE_FILE "\\setting\\pet\\ride.txt"
#define RIDE_TABLE_RANK_FILE "\\setting\\pet\\riderank.txt"

#define PET_BASE_MODULE_NAME "pet_base"
#define PET_LEVEL_MODULE_NAME "pet_level"		// 宠物等级奖励模块名称
#define PET_TACTIC_MODULE_NAME "pet_tactic"     //
#define PET_UPRANK_MODULE_NAME "pet_uprank"		//升阶
#define PET_BLADE_MODULE_NAME "pet_blade"		// 变身神仙阵法加成百分比

#define KD_PET_TACTIC_NUM 7		// 阵位个数

// 玩家宠物栏最小和最大值
#define KD_MIN_PET_COUNT 6
#define KD_MAX_PET_COUNT 30

#define KD_PET_INIT_LEVEL 1
#define KD_PET_INIT_RANKLEVEL 1

#define KD_PET_MAX_QUALITY 6

// 释放技能最大距离
#define KD_PET_CAST_DISTANCE (10 * CELL_LENGTH)

#define KD_PET_HUNGRY_MAX 100	// 饱食度上限

#define KD_RIDE_HUNGRY_MAX 100	// 坐骑饱食度上限

#define KD_PET_ABRADE_SECOND (6000)			// 每5分钟处理一次宠物饱食度和亲密度

#define KD_RIDE_ABRADE_SECOND (300)		

#define PET_POOL_MAX_COUNT	20000	// 当前服务器最多的玩家宠物数量

#define RIDE_POOL_MAX_COUNT	3000		// 当前服务器最多的玩家坐骑数量

#define PET_CAST_SKILL_FRAME 1	// 每0.5秒施放一次技能

#define PET_INIT_EQUIP_SLOT_NUM 3  // 初始有3个可用槽位

enum KE_PET_FIGHT_STATE  // 宠物出战状态
{
	emPET_STATE_REST = 0,	// 休息
	emPET_STATE_FIGHT = 1,  // 出战
};

enum KE_RIDE_STATE
{
	emRIDE_STATE_REST = 0,		// 休息
	emRIDE_STATE_UP = 1,		// 骑乘
};

enum KE_PET_CHARACTER	// 宠物性格
{
	emPET_CHARACTER_LIFE = 1,	// 天真
	emPET_CHARACTER_ATTACK = 2,	// 凶猛
	emPET_CHARACTER_HIT	= 3,	// 坚定
	emPET_CHARACTER_CRIT = 4,	// 暴躁
	emPET_CHARACTER_RESIST = 5,	// 温顺
	emPET_CHARACTER_DEF = 6,	// 刚毅
	emPET_CHARACTER_DODGE = 7,	// 灵敏
	
};

enum KE_PET_COUNT_ATTR_CONST					// 宠物属性数目常量
{
	emPET_COUNT_BASE		= 7,				// 基本属性的数目
	emPET_MAX_SKILL_LEVEL   = 5,				// 技能最大等级
	emPET_COUNT_SKILL		= 2,				// 最大技能槽个数
	emPET_COUNT_VALUE		= 3,				// 技能数值数量
	emPET_COUNT_EQUIP		= 7,				// 最大装备槽个数
};

enum KE_RIDE_COUNT_ATTR_CONST				// 坐骑属性数目常量
{
	emRIDE_COUNT_BASE		= 6,		// 坐骑最大基本属性的数目
	emRIDE_COUNT_RIDE		= 2,		// 骑乘属性最大数目
};

// 宠物基础属性定义顺序
enum QE_PET_BASE_ATTR
{
	emPET_ATTR_LIFE,
	emPET_ATTR_ATTACK,
	emPET_ATTR_HIT,
	emPET_ATTR_CRIT,  // 暴击
	emPET_ATTR_RESIST,  // 抗暴
	emPET_ATTR_DEFENSE,
	emPET_ATTR_DODGE,
};

enum QE_PET_SKILL_TYPE
{
	emPET_NORMAL_ATTACK,    // 普通攻击
	emPET_BUFF_ADD_BLOOD,   // 加血buff技能
	emPET_BUFF_SUB_BLOOD,   // 减血buff技能
	emPET_BUFF_ADD_ATTACK,  // 加攻buff技能
	emPET_BUFF_SUB_DEFENCE, // 减防buff技能
};

enum QE_PET_SKILLBOOK
{
	emPET_SKILL_BOOK_G = 24,
	emPET_SKILL_BOOK_D = 8,
	emPET_SKILL_BOOK_P = 1
};

enum QE_PET_UPRANKBALL
{
	emPET_UPRANKBALL_G = 24,
	emPET_UPRANKBALL_D = 2,
	emPET_UPRANKBALL_P = 8
};

//enum KE_PET_SKILL_ID
//{
//	emPET_SKILL_LL = 1,		// 凌厉
//	emPET_SKILL_TB = 2,		// 铁壁
//	emPET_SKILL_WR = 3,		// 温润
//	emPET_SKILL_EC = 4,		// 恩赐
//	emPET_SKILL_CR = 5,		// 残忍
//};

//CONST KE_PET_SKILL_ID g_aePetSkill[] =
//{
//	emPET_SKILL_LL,
//	emPET_SKILL_TB,
//	emPET_SKILL_WR,
//	emPET_SKILL_EC,
//	emPET_SKILL_CR,
//};

struct KBASEATTR
{
	LONGLONG nAttrib;
	LONGLONG nValue;
};

struct KPETEQUIP
{
	INT   nParticular;
	WORD  wNumberIndex; // 装备附加属性的数值索引
	BYTE  byLock;       // 此装备槽是否锁定
};

struct KSKILL_DATA
{
	INT		nBuffId;

	INT nSkillLevel;        // 技能等级
	INT nSkillType;         // 技能类型
	INT nSkillValues[emPET_COUNT_VALUE * 2];    // 技能相关数值，根据技能类型代表不同的意义
	INT nCDTime;			// CD时间
	INT nCurrentCDTime;		// 当前cd时间
	BOOL bStartCalcCD;		// 是否开始计算CD时间
	BOOL bToSelf;		// 是否加给自己
};

struct KPET_INDEX								// 标识配置表中的一个宠物
{
	INT		nType;				// 类型ID，对应Npc表里的NpcId
	INT		nQuality;		// 品质
	INT		nCharacter;		// 性格

	bool operator < (CONST KPET_INDEX &sIndex) CONST
	{
		if (nType != sIndex.nType)
			return	nType < sIndex.nType;
		if (nQuality != sIndex.nQuality)
			return	nQuality < sIndex.nQuality;
		return	nCharacter < sIndex.nCharacter;
	}

	bool operator == (CONST KPET_INDEX &sIndex) CONST
	{
		return	nType == sIndex.nType && nQuality == sIndex.nQuality &&
			nCharacter == sIndex.nCharacter;
	}

	bool operator != (CONST KPET_INDEX &sIndex) CONST
	{
		return !operator == (sIndex);
	}
};

struct QPET_CARD  // 神仙卡片信息
{
    int nPetId;       // 神仙ID
	int nPetNature;   // 神仙法系
};

class KPetRandom			// 随机序列生成器
{

public:

	explicit KPetRandom(UINT uRandSeed = 42) : m_uRandSeed(uRandSeed) {}

	UINT GetSeed(VOID) CONST		{ return m_uRandSeed; }
	VOID SetSeed(UINT uRandSeed)	{ m_uRandSeed = uRandSeed; }
	UINT Random(VOID)				{ return m_uRandSeed = m_uRandSeed * 3877 + 29573; }
	INT  Random(INT nMin, INT nMax)	
	{
		UINT uRandom = Random();
		if (nMax - nMin >= 0)
		{
			return uRandom % (nMax - nMin + 1) + nMin;
		}
		else
			return nMin;
	}

private:

	UINT				m_uRandSeed;
};

#endif // petdef_h__