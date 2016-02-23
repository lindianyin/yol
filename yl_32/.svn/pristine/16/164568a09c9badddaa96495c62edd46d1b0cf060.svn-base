/* -------------------------------------------------------------------------
//	文件名		：	fightskillsetting.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2011.5.21
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef fightskillsetting_h__
#define fightskillsetting_h__
#include "stdafx.h"
#include <map>
#include "onlinegameworld/kcharacter.h"
#include <string>
using std::string;

enum FIGHT_SKILL_TYPE
{
    FIGHT_SKILL_PASSIVE,         // 被动技能
	FIGHT_SKILL_BUFFER,          // buffer技能 
	FIGHT_SKILL_MISSILE,         // 导弹技能
	FIGHT_SKILL_OTHER,  // 闪电链技能
	FIGHT_SKILL_MEDICINE,  // 吃药技能

	emSKILL_TYPE_RESIST, // 抗体技能
};

enum FIGHT_SKILL_STARTPOS
{
    FIGHT_SKILL_STARTPOS_SELF,      // 自身
	FIGHT_SKILL_STARTPOS_TARGET,    // 目标
};

enum FIGHT_SKILL_KIND
{
    FIGHT_SKILL_KIND_FACTION = 1,   // 门派技能
    FIGHT_SKILL_KIND_GENERAL = 2,   // 通用技能
	FIGHT_SKILL_KIND_ITEM = 3,       // 道具技能
	FIGHT_SKILL_KIND_PET = 4,		// 宠物技能
};

// 技能消耗类型
enum FIGHT_SKILL_COST_TYPE
{
    FIGHT_SKILL_COST_TYPE_MANA, // 真气
	FIGHT_SKILL_COST_TYPE_POWER, // 集中值
    FIGHT_SKILL_COST_TYPE_LIFE   // 生命
};

// 技能引导类型
enum FIGHT_SKILL_CHANNEL_TYPE
{
    FIGHT_SKILL_CHANNEL_NO, // 非引导技能
	FIGHT_SKILL_CHANNEL_FORWARD, // 正向引导技能
	FIGHT_SKILL_CHANNEL_REVERSE  // 反向引导技能
};

enum FIGHT_SKILL_STATE_TYPE
{
	FIGHT_SKILL_STATE_CHANNEL,	// 技能引导类型
};

// 技能数值
struct BaseSkillNumber
{
	INT nType; // 5 or 6
	INT nSkillLevel;
	INT nValue;
};

// 吃药技能数值
struct MedicineSkillNumber : BaseSkillNumber{
	INT nNeedMedicineCount;
};
// 被动技能数值
struct PassiveSkillNumber  : BaseSkillNumber {};
// 抗体技能数值
struct ResistSkillNumber  : BaseSkillNumber 
{
	INT nResistSkillId;
};

struct InactiveSkillNumber
{
	INT nSkillId;
	INT nResistSkillId;
	INT nSkillLevel;
	INT nBufferIncroRate;
	INT nBufferExistTime;
	INT nBufferValue;
	INT nBufferId;
	INT nManaCost;
	INT nAddBuffToSelf;
    INT nSkillCoefficient;
	INT nHarmValue;
	INT nAddAnger;
};

struct FightSkillTemplate
{
    INT		nSkillID;
	INT		nFightSkillType;
	INT     nPassiveSkillType;			// 被动技能类型 1-7
    BOOL	bNeedTarget;				// 是否需要目标
	BOOL	bIsImmediately;				// 是否立即释放
	INT		nFightSkillStartPos;		// 技能开始位置
	INT		nFightSkillKind;			// 技能类别
	INT		nMissileHeight;				// 导弹相对起始位置的高度
	INT		nMissileTargetHeight;		// 导弹到达目标时的高度
	//INT		nMissileHorizontalSpeed;	// 导弹水平速度(以cell长度为最小单位的每帧移动速度)
	//INT		nMissileVerticalSpeed;		// 导弹垂直速度(以cell长度为最小单位的每帧移动速度)
	FLOAT	fAttackRadius;				// 施法距离(米)
	BOOL	bAura;						// 是否光环
	BOOL	nFactionLimit[5];				// 门派限制
	INT		nRouteLimit;				// 路线限制
	BOOL	bMelee;						// 是否近身攻击
    INT		nSkillCostType;				// 技能消耗类型
	INT		nCostPercent;				// 技能消耗百分比
	INT		nTimePerCast;				// 技能冷却时间
	FLOAT	nTimeBeforeCast;			// 释放前的准备时间
	BOOL	bIsPhysical;				// 是否物理攻击
	INT		bDmgOrRvv;					// 是否造成伤害
    INT		nTargetRelation;			// 与技能目标关系
	BOOL	bUseAR;						// 是否考虑命中
	INT		nFightSkillChannel;			// 引导技能类型
	INT		nChannelTime;				// 引导时间
	INT		nChannelFreq;				// 引导频率
	FLOAT	fRepelPercent;				// 引导击退率
	FLOAT	fRepelValue;				// 引导击退量
	INT		nReqLevel;					// 技能需求等级
	INT		nMaxLevel;					// 技能最大等级
    INT		nWeaponLimit;				// 武器限制
	BOOL	bHorseLimit;				// 骑马限制
	BOOL	bPeachCanUse;				// 安全状态下能否使用
	BOOL	bExpSkill;					// 是否为熟练度技能
	INT     nChainLightningTargetNum;   // 闪电链技能目标数量
	FLOAT   fChainLightningRange;       // 闪电链影响范围
	INT		nCategory;
	BOOL	abRelation[RELATION_NUM];	// 关系判断([1]自己、[2]队友、[3]敌人、[4]友方)
	INT		nIncorBuff1Id;				// buff1
	INT		nIncorBuff2Id;				// buff2
	INT		nIncorBuff3Id;				// buff3
	FLOAT   fMissileSpeed;				// 导弹飞行速度
	INT		nSlot;						// 槽位
	//INT     nRune;						// 符文
	//INT		nSkillLevel;				// 技能等级
	INT     nYlEnergy;					// 本技能需要消耗多少游龙真气才能升上来
    BOOL    bNeedJumpTo;                // 是否跳砸类技能
	INT		nCoefficientMin;			// 技能系数最小值
	INT		nCoefficientMax;			// 技能系数最大值
	FLOAT	fSpecialTimeScale;			// 特殊技能模式下的时间缩放
	INT		nSpecialCdTime;				// 特殊技能cd缩放（百分比）
	FLOAT	nSpecialTimeBeforeCast;			// 释放前的准备时间
	CHAR	szPlugins[64];	// 技能插件
};

class FightSkillSetting
{
public:
    FightSkillSetting();

	~FightSkillSetting();

	BOOL Init();

	FightSkillTemplate* GetFightSkillTemplate(WORD dwTemplateID);

	PassiveSkillNumber* GetPassiveSkillNumber(INT nPassiveType, INT nSkillLevel);
	MedicineSkillNumber* GetMedicineSkillNumber(INT nMedicineType, INT nSkillLevel);
	ResistSkillNumber* GetResistSkillNumber(INT nResistType, INT nSkillLevel);
	ResistSkillNumber* GetResistSkillNumberByResistSkillId(INT nResistSkillId, INT nSkillLevel);
	InactiveSkillNumber* GetInactiveSkillNumber(INT nSkillId, INT nSkillLevel);

	INT GetSkillCdTime(INT nSkillTemplateId);  // 获取技能冷却时间
	INT GetSkillCastTime(INT nSkillTemplateId, INT nDistance = 0); // 获取技能释放总时长（参数可选是否包括导弹技能， 填距离）

	std::map<INT, FightSkillTemplate*>* GetFightSkillSettings() { return &m_mapFightSkills;}

private:
	
	std::map<INT, FightSkillTemplate*> m_mapFightSkills;

	std::map<INT, PassiveSkillNumber*> m_mapPassiveSkillNumbers;
	std::map<INT, MedicineSkillNumber*> m_mapMedicineSkillNumbers;
	std::map<INT, ResistSkillNumber*> m_mapResistSkillNumbers;
	std::map<INT, ResistSkillNumber*> m_mapResistSkillNumbersBySkillIds;
	std::map<INT, InactiveSkillNumber*> m_mapInactiveSkillNumbers;

	BOOL SplitString(const std::string &src, const std::string &separator, std::vector<string> &dest);
	
};

extern FightSkillSetting g_cFightSkillSetting;

#endif // fightskillsetting_h__