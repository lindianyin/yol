
#pragma once

#define NPC_FACTION_RANDOM_RANGE 20
#define NPC_DROP_LIST_ITEM_NUM 15

struct NPC_INTENSITY
{
	INT nLevel;			  // 等级
	INT nLevelRate;			// 等级随机几率
	INT nNpcHp;           // 生命值
	INT nNpcHpReplenish;  // 生命值回复速度
	INT nMana;            // 真气
	INT nManaReplenish;   // 真气回复速度
	INT nExp;             // 经验值
	INT nExpType;         // 经验分配方式

	INT nAttack;			// 攻击
    INT nHit;				// 命中
	INT nDuck;				// 闪避
	INT nArmor;				// 防御
	INT nCritical;			// 暴击
	INT nDeCritical;		// 抗暴
	
	INT nWalkSpeed;
	INT nRunSpeed;

	INT nIgnoreDef;			// 免控
};

struct KNS_NPC_TEMPLATE
{
	INT nPrimaryFactionId[NPC_FACTION_RANDOM_RANGE];
	INT nSecondFactionId[NPC_FACTION_RANDOM_RANGE];
	INT nRelation;
	INT nAIType;
	INT nKind;           // NPC类型
	INT nCamp;           // 阵营

	INT nPropId;         // 强度表ID
};

struct NPC_DROP_LIST
{
    DWORD dwBoxID;
	INT nGenre[NPC_DROP_LIST_ITEM_NUM];
	INT nDetailType[NPC_DROP_LIST_ITEM_NUM];
	INT nParticularType[NPC_DROP_LIST_ITEM_NUM];
	INT nDropRate[NPC_DROP_LIST_ITEM_NUM];
};

// -------------------------------------------------------------------------
class KNpc;
class KNpcHelper
{
public:
	BOOL Init();
	BOOL LoadNpcTemplate(KNpc& rcNpc);
	INT GetNpcPropId(INT nTemplateId);

	INT GetMatchLevel(DWORD dwNpcPropId, INT nLevel);
	BOOL ResetNpcIntensity(KNpc& rcNpc, INT nPropId, INT nLevel = 0);

	INT GetNpcExp(DWORD dwNpcPropId, INT nLevel);
	INT GetNpcExpType(DWORD dwNpcPropId, INT nLevel);

private:
	BOOL ReadNpcTemplate();

	BOOL ReadNpcIntensityTable();

	const NPC_INTENSITY* GetNpcIntensity(DWORD dwNpcPropId, INT nLevel = 0);

private:
	typedef std::map<DWORD, KNS_NPC_TEMPLATE> MP_NPC_TEMPLATE;
	MP_NPC_TEMPLATE m_mapNpcTemplate;
	std::map<DWORD, std::map<DWORD, NPC_INTENSITY> > m_mapNpcIntensityTable;

	std::map<DWORD, NPC_DROP_LIST> m_mapNpcDropList;
};

extern KNpcHelper g_cNpcHelper;
// -------------------------------------------------------------------------

