
#ifndef __KLIBOFBPT_H__
#define __KLIBOFBPT_H__

#include "kbasproptbl.h"

class KItem;

//---------------------------------------------------------------------------
// 该数据结构用于输出角色装备中对套装的持有情况（穿在身上的）

typedef std::map<INT, INT>	KITEM_SUITE_COUNT_MAP;		// Key是套装ID, Value是持有的套装件数

//---------------------------------------------------------------------------
// 该数据结构用来记录套装的完整信息

struct KSUITE_INFO
{
	KSUITE_INFO() : pInfo(NULL), nCount(0) {}
	CONST KBASICPROP_SUITEINFO*	pInfo;
	INT							nCount;
	std::vector<KITEM_INDEX>	aryPart[emEQUIPPOS_NUM];
};

//---------------------------------------------------------------------------
// KLibOfBPT

class KLibOfBPT
{
public:
	typedef std::map<INT, KSUITE_INFO>		KSuiteInfoMap;		// Suite Info Map 下标是Suite ID
	typedef std::vector<KMAGIC_INDEX>		KMagicIndexVec;		// 魔法属性索引表


public:
	BOOL Init(INT nVersion);

	CONST KMagicIndexVec*		GetRandMAT(BOOL bDarkness, INT nEquipType) CONST;
	CONST KBASICPROP_BASEDATA*	GetBasicProp(CONST KITEM_INDEX &sIndex) CONST;

	CONST KSUITE_INFO*			GetGoldSuiteInfo(INT nSuiteId) CONST;
	CONST KSUITE_INFO*			GetGreenSuiteInfo(INT nSuiteId) CONST;
	CONST KBASICPROP_MAGICACTIVERULE*  GetActiveRuleInfo(INT nRuleId) CONST;

private:
	BOOL InitRandMAT(INT nVersion);
	VOID InitExternSetting(INT nVersion);					// 初始化外部扩充的道具配置

	VOID InitSuiteInfo(INT nVersion);						// 整理套装数据：统计包含的套装装备，以及总件数
	template<typename KBPT_Equip, typename KBASICPROP_SUITE, typename KBPT_Info>
	VOID InitSuiteInfoT(KSuiteInfoMap &mapSuiteInfo, KBPT_Equip &cBPTEquip, KBPT_Info &cBPTInfo);

	CONST KBASICPROP_BASEDATA*	GetBasicPropStrictly(CONST KITEM_INDEX &sIndex) CONST;


public:
	KBPT_GE_Sword				m_cGE_Sword;
	KBPT_GE_Spear				m_cGE_Spear;
	KBPT_GE_Necklace			m_cGE_Necklace;						// 项链
	KBPT_GE_Ring				m_cGE_Ring;							// 戒指
	KBPT_GE_Pendant				m_cGE_Pendant;						// 玉佩
	KBPT_GE_Head				m_cGE_Head;
	KBPT_GE_Armor				m_cGE_Armor;						// 衣服
	KBPT_GE_Belt				m_cGE_Belt;							// 腰带
	KBPT_GE_Cloak				m_cGE_Cloak;						// 披风
	KBPT_GE_Foot				m_cGE_Foot;							// 鞋子
	
	KBPT_GE_Suit				m_cGE_Suit;							// 套装（外装）
	KBPT_GE_Bottom				m_cGE_Bottom;						// 脚底（外装）
	KBPT_GE_Back				m_cGE_Back;							// 背（外装）

	// 非装备道具
	KBPT_Medicine				m_cMedicine;						// 药品
	KBPT_Script_Item			m_cScript;							// 触发脚本的物品
	KBPT_Upgrade_Material		m_cUpgradeMaterial;					// 升级材料
	KBPT_Box					m_cBox;
	KBPT_Stone					m_cStone;							// 宝石
	KBPT_Ride					m_cRide;							// 坐骑卡片
	KBPT_Pet_Equip				m_cPetEquip;							// 宠物装备
	KBPT_Pet_Item				m_cPetItem;								// 宠物相关道具
	//KBPT_Skill_Item				m_cSkill;							// 触发技能的物品
	KBPT_Task_Quest				m_cQuest;							// 任务物品
	//KBPT_Extend_Bag				m_cExtBag;							// 扩展包裹
	//KBPT_Stuff_Item				m_cStuff;							// 生活技能材料
	//KBPT_Plan_Item				m_cPlan;							// 生活技能配方

	// 相关配置
	KBPT_MagicAttrib			m_cMagicAttrib;						// 随机魔法属性
	KBPT_GoldInfo				m_cGoldInfo;						// 黄金套装信息
	KBPT_GreenInfo				m_cGreenInfo;						// 绿色套装信息
	KBPT_EquipEffect			m_cEquipEffect;						// 装备特效信息
	KBPT_MagicActiveRule		m_cMagicActiveRule;					// 魔法属性激活规则

protected:
	KSuiteInfoMap				m_mapSIGold;						// 黄金套装初始信息
	KSuiteInfoMap				m_mapSIGreen;						// 绿色套装初始信息

	KMagicIndexVec				m_RandMAT[emMATF_COUNT_BRIGHTDARK][emMATF_COUNT_EQUIPTYPE];
};

// -------------------------------------------------------------------------

#endif /* __KLIBOFBPT_H__ */
