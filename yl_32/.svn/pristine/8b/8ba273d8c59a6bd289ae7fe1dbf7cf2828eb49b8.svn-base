
#ifndef	KBasPropTblH
#define	KBasPropTblH

#include "kitemdatadef.h"
#include "config/knpcdef.h"
#include <vector>

//------------------------------------------------------------------------

#define	SZBUFLEN_0					64							// 典型的字符串缓冲区长度
#define	SZBUFLEN_1					256							// 典型的字符串缓冲区长度

#define	KD_ITEM_SETTING_PATH		"\\setting\\item"

//------------------------------------------------------------------------

struct KBPT_RANGE												// 记录区间值
{
	INT			nMin;
	INT			nMax;
};

struct KBPT_EQUIP_BASEATTR										// 装备的基础属性
{
	INT			nAttrib;										// 基础属性ID
	KBPT_RANGE	aryRange[KD_MAGIC_VALUE_NUM];					// 取值范围
};

struct KBPT_EQUIP_RIDEATTR
{
	INT			nAttrib;										// 乘骑属性ID
	KBPT_RANGE	aryRange[KD_RIDE_VALUE_NUM];					// 取值范围  add by NiuNiu
};

struct KBPT_EQUIP_REQATTR										// 装备的需求属性
{
	KE_ITEM_REQUIREMENT	eType;									// 需求属性类型
	INT					nValue;									// 数值
};

struct KBPT_EQUIP_RANDMA										// 非套装的随机魔法属性
{
	INT			nAttrib;										// 随机魔法属性ID
	KBPT_RANGE	aryRange[KD_MAGIC_VALUE_NUM];					// 魔法属性取值范围
};

struct KBPT_SUITE_RANDMA										// 套装的随机魔法属性
{
	INT			nAttrib;										// 随机魔法属性ID
	KBPT_RANGE	aryRange[KD_MAGIC_VALUE_NUM];					// 魔法属性取值范围
};

struct KBPT_ENH_ATTR
{
	INT			nAttrib;										// 强化激活魔法属性ID
	KBPT_RANGE	aryRange[KD_MAGIC_VALUE_NUM];					// 魔法属性取值范围
	KBPT_RANGE	aryGrow;										// 魔法属性成长值
};

struct KBPT_EQUIP_ENHMA											// 装备的强化激活魔法属性
{
	INT			nTimes;											// 激活条件：最少强化次数
	KBPT_ENH_ATTR aryAttr[emITEM_COUNT_ENHATTR];
};

struct KBPT_EQUIP_SUITEMA										// 装备的套装激活魔法属性
{
	INT			nNum;											// 激活条件：最少套装件数
	INT			nAttrib;										// 套装激活魔法属性ID
	INT			aryValue[KD_MAGIC_VALUE_NUM];					// 魔法属性确定数值
};

struct KBPT_MED_ATTRIB											// 描述药品属性的特性：数值与时间
{
	INT			nAttrib;										// 药品魔法属性ID
	INT			nValue;											// 药品魔法值参数
	INT			nTime;											// 药品魔法持续时间
};

struct KBPT_STONE_ATTRIB										// 宝石基本属性：魔法属性和1-20级的数值
{
	INT			nAttrib;										// 宝石魔法属性ID
	INT			aryParam;										// 宝石魔法属性ID对应的参数，目前只有1个
};

struct KBPT_EQUIP_ACTIVEMA : public KBPT_EQUIP_BASEATTR
{
	INT			nRuleId;										// 激活规则ID，与KBASICPROP_MAGICACTIVERULE中的nRuleId对应
};

//------------------------------------------------------------------------
// 物品基本属性的基本数据(公共数据 14项)

struct KBASICPROP_BASEDATA
{
	CHAR		szName[SZBUFLEN_0];								// 名称
	CHAR		szKind[SZBUFLEN_1];								// 自定义分类
	CHAR		szClass[SZBUFLEN_0];							// CLASS
	INT			nItemGenre;										// 道具种类
	INT			nDetailType;									// 具体类别
	INT			nParticular;									// 详细类别
	INT			nLevel;											// 等级
	INT			nCanDestroy;									// 是否可摧毁
	INT			nPrice;											// 价格
	BYTE		byCurrencyType;									// 货币类型
	INT			nQuality;										// 道具品质（颜色）
	INT			nBindType;										// 道具绑定形式 (KE_ITEM_BINDTYPE枚举)
	BOOL		bValuable;										// 是否贵重道具 (防刷及写LOG)
	INT			aryExtParam[emITEM_COUNT_EXTPARAM];				// 扩展参数
#ifndef GAME_SERVER	
	CHAR		szIconImage[SZBUFLEN_0];						// 道具图标动画文件名
	CHAR		szViewImage[SZBUFLEN_0];						// 道具预览动画文件名
	CHAR		szIntro[SZBUFLEN_1*2];							// 说明文字
	CHAR		szHelp[SZBUFLEN_1];								// 帮助文字
#endif
};

//------------------------------------------------------------------------
// 以下结构用于描述非装备类道具

struct KBASICPROP_OTHER : KBASICPROP_BASEDATA
{
	INT			nStackMax;										// 叠放的最大值 (1表示不可叠放)
	INT			nReqLevel;										// 使用道具所需级别
	INT			nCDType;										// 冻结时间类型
	BOOL		bShortcut;										// 是否可放快捷栏
	CHAR		szForbidType[SZBUFLEN_0];						// 禁用类型
};

typedef KBASICPROP_OTHER	KBASICPROP_SCRIPT;					// 脚本物品属性
typedef KBASICPROP_OTHER	KBASICPROP_SKILL;					// 技能物品属性
typedef KBASICPROP_OTHER	KBASICPROP_QUEST;					// 任务物品属性
typedef KBASICPROP_OTHER	KBASICPROP_STUFF;					// 生活技能材料属性
typedef KBASICPROP_OTHER	KBASICPROP_PLAN;					// 生活技能配方属性

struct KBASICPROP_EXTBAG : KBASICPROP_OTHER				// 扩展背包属性
{
	INT			nPos;
};

struct KBASICPROP_MEDICINE : KBASICPROP_OTHER					// 药品基本属性
{
	INT			nCanUseInBZone;									// 能否在跨服战场使用
	KBPT_MED_ATTRIB		aryAttrib[emMEDICINE_COUNT_ATTRIB];		// 药品的属性
};

struct KBASICPROP_STONE : KBASICPROP_BASEDATA					// 宝石基本属性
{
	INT			nColor;											// 宝石颜色
	INT			nStackMax;										// 叠放的最大值 (1表示不可叠放)
	INT			nReqLevel;										// 人物等级需求
	INT			nEquipLevel;									// 装备等级需求
	INT			nStoneType;										// 宝石类型
	KBPT_STONE_ATTRIB	aryAttrib[emSTONE_COUNT_ATTRIB];		// 宝石的3个魔法属性
};

//------------------------------------------------------------------------
// 以下结构用于描述装备的初始属性

struct KUPGRADE_NEEDITEM
{
	INT nGenre;
	INT nDetail;
	INT nParticular;
	INT nAmount;
};

// modified by dengyong 20100429, 给装备加上解锁时间
struct KBASICPROP_EQUIPMENT : KBASICPROP_BASEDATA
{
	KBPT_EQUIP_BASEATTR	aryBasic[emITEM_COUNT_BASE];			// 基础属性
	KBPT_EQUIP_REQATTR	aryReq[emITEM_COUNT_REQ];				// 需求属性
	KBPT_EQUIP_ENHMA	aryStr[emITEM_COUNT_STRENGTHEN];		// 装备改造激活属性
	KBPT_RANGE			arySlot;								// 宝石孔
	/*
	INT					nResMale;								// 男性装备外观
	INT					nResFemale;								// 女性装备外观
	*/
	INT					nPandent1;								// 左边外观
	INT					nPandent2;								// 右边外观
	INT					nCategory;								// 装备类别
	INT					nFightPower;							// 装备原始战斗力
	INT					nReqLevel;								// 装备需求等级
	INT					aryExPandents[emITEM_COUNT_EX_PENDANT];	// 装备特殊挂件
	KUPGRADE_NEEDITEM	aryNeedItems[emITEM_COUNT_UPGRADE];		// 装备升级所需材料
};

typedef KBASICPROP_EQUIPMENT	KBASICPROP_GENERALEQUIP;		// 普通装备属性

//------------------------------------------------------------------------

// 以下结构用于描述武器攻击力属性
struct KBASICPROP_WEAPON : KBASICPROP_EQUIPMENT
{
	KBPT_EQUIP_BASEATTR	aryDmg;									// 武器攻击力下限值和区间
};

struct KBASICPROP_SUIT : KBASICPROP_BASEDATA
{
	KBPT_EQUIP_BASEATTR	aryBasic[emITEM_COUNT_BASE];			// 基础属性
	INT					nPandent1;								// 左边外观
	INT					nPandent2;								// 右边外观
	INT					nReqLevel;								// 装备需求等级
	INT					aryExPandents[emITEM_COUNT_SUIT_EX_PENDANT];			// 额外挂件
};

//------------------------------------------------------------------------
// 以下结构用于描述紫装的初始属性

struct KBASICPROP_PURPLEEQUIP : KBASICPROP_EQUIPMENT
{
	KMAGIC_INDEX		aryRand[emITEM_COUNT_RANDOM];			// 随机魔法属性索引表
};

//------------------------------------------------------------------------
// 以下结构用于描述制定套装（绿装）的初始属性
struct KBASICPROP_GREENSUITE : KBASICPROP_PURPLEEQUIP 
{
	INT					nSuiteID;
};


//------------------------------------------------------------------------
// 以下结构用于描述套装的初始属性

struct KBASICPROP_EQUIP_SUITE : KBASICPROP_EQUIPMENT
{
	KBPT_SUITE_RANDMA	aryRand[emITEM_COUNT_RANDOM];			// 随机魔法属性表
	INT					nSuiteID;								// 所属套装的ID，大于0有效
};

typedef KBASICPROP_EQUIP_SUITE	KBASICPROP_GOLDEQUIP;			// 黄金装备属性

//------------------------------------------------------------------------
// 以下结构用于描述配置文件中给出的魔法属性

struct KBASICPROP_MAGICATTRIB
{
	CHAR				szSuffix[SZBUFLEN_0];					// 后缀名
	INT					nLevel;									// 魔法属性等级
	BOOL				bDarkness;								// 是否暗属性
	//INT					nSeries;								// 五行要求
	INT					aryDropRate[equip_detailnum];			// 出现概率
	INT					nValue;									// 价值量
	KBPT_EQUIP_RANDMA	sMA;									// 核心参数
};

//------------------------------------------------------------------------
// 以下结构用于描述配置文件中给出的魔法属性激活规则
struct KBASICPROP_MAGICACTIVERULE
{
	INT		nRuleId;				// 规则序列号
	INT		nSkillId;				// 激活BUFF的技能ID
	INT		nBeginLevel;			// 激活起效等级,开始值
	INT		nEndLevel;				// 激活起效等级,最末值
	CHAR	szDesc[SZBUFLEN_1];		// 描述信息
};

//------------------------------------------------------------------------
// 以下结构用于保存套装信息

struct KBASICPROP_SUITEINFO
{
	INT					nSuiteID;								// 套装ID
	KBPT_EQUIP_SUITEMA	arySuite[emITEM_COUNT_SUITE];			// 套装激活属性
#ifndef GAME_SERVER
	CHAR				szName[SZBUFLEN_0];						// 套装名称
	CHAR				aryName[emEQUIPPOS_NUM][SZBUFLEN_0];	// 该套套装各件的名字（可能为空串）
#endif
};

//------------------------------------------------------------------------
// 以下结构用于描述装备特效配置
struct KBASICPROP_EQUIPEFFECT
{
	//INT					nSeries;								// 五行属性
	INT					nCategory;								// 装备类别
	INT					anEquipEffectRes[1/*series_num*/];			// 不同五行的特效资源Id
};


//------------------------------------------------------------------------
// KBasicPropertyTable

class KBasicPropertyTable										// 缩写: BPT，用于派生类
{

public:

	KBasicPropertyTable();
	~KBasicPropertyTable();

	virtual BOOL Load(INT nVersion);        					// 从tabfile中读出初始属性值, 填入属性表

protected:

	LPBYTE m_pBuf;												// 指向属性表缓冲区的指针
																// 属性表是一个结构数组,
																// 其具体类型由派生类决定
	INT		m_nNumOfEntries;									// 属性表含有多少项数据
    INT     m_nSizeOfEntry;										// 每项数据的大小(即结构的大小)
	CHAR	m_szTabFile[MAX_PATH];								// tabfile的文件名

	VOID	GetMemory(VOID);
	VOID	ReleaseMemory(VOID);
	VOID	SetCount(INT);
	virtual	VOID LoadRecord(INT i, QTabFile *pTF) = 0;

};

//------------------------------------------------------------------------
// KBPT_BaseT	基础类模板

template<typename KINDEXT, typename KBASICPROPT>
class KBPT_BaseT : public KBasicPropertyTable
{

public:

	typedef std::map<KINDEXT, KBASICPROPT*> KINDEX_MAP;

	KINDEX_MAP& GetIndexMap(VOID) { return m_mapIndex; }
	CONST KBASICPROPT* GetData(CONST KINDEXT& rcIndex) CONST
	{
		typename KINDEX_MAP::const_iterator it = m_mapIndex.find(rcIndex);
		if (it == m_mapIndex.end())
			return	NULL;
		return	it->second;
	}

protected:

	KINDEX_MAP	m_mapIndex;

	explicit KBPT_BaseT(PCSTR pszTabFile)
	{
		m_nSizeOfEntry = sizeof(KBASICPROPT);
		::strncpy(m_szTabFile, pszTabFile, sizeof(m_szTabFile));
		m_szTabFile[sizeof(m_szTabFile) - 1] = 0;
	}

	CONST KBASICPROPT* GetRecord(INT nIndex) CONST
	{
		if (nIndex >= 0 && nIndex < m_nNumOfEntries)
			return	(((KBASICPROPT*)m_pBuf) + nIndex);
		return	NULL;
	}

};

//------------------------------------------------------------------------
// KBPT_BaseT类实例声明

// 配置文件路径
#define _KD_TABFILE_GE_SWORD			"equip\\sword"
#define _KD_TABFILE_GE_SPEAR			"equip\\spear"
#define	_KD_TABFILE_GE_NECKLACE			"equip\\necklace"
#define	_KD_TABFILE_GE_RING				"equip\\ring"
#define	_KD_TABFILE_GE_PENDANT			"equip\\pendant"
#define	_KD_TABFILE_GE_HEAD				"equip\\head"
#define	_KD_TABFILE_GE_ARMOR			"equip\\armor"
#define	_KD_TABFILE_GE_BELT				"equip\\belt"
#define	_KD_TABFILE_GE_CLOAK			"equip\\cloak"
#define	_KD_TABFILE_GE_FOOT				"equip\\foot"

#define	_KD_TABFILE_GE_SUIT				"equip\\suit"
#define	_KD_TABFILE_GE_BOTTOM			"equip\\bottom"
#define	_KD_TABFILE_GE_BACK				"equip\\back"


#define _KD_TABFILE_STONE				"other\\gem"		// 宝石

#define _KD_TABFILE_GE_GARMENT			"equip\\general\\garment"		// 外衣
#define _KD_TABFILE_GE_OUTHAT			"equip\\general\\outhat"		// 外帽
#define	_KD_TABFILE_PE_MELEEWEAPON		"equip\\purple\\meleeweapon"
#define	_KD_TABFILE_PE_RANGEWEAPON		"equip\\purple\\rangeweapon"
#define	_KD_TABFILE_PE_ARMOR			"equip\\purple\\armor"
#define	_KD_TABFILE_PE_HELM				"equip\\purple\\helm"
#define	_KD_TABFILE_PE_BOOTS			"equip\\purple\\boots"
#define	_KD_TABFILE_PE_BELT				"equip\\purple\\belt"
#define	_KD_TABFILE_PE_AMULET			"equip\\purple\\amulet"
#define	_KD_TABFILE_PE_RING				"equip\\purple\\ring"
#define	_KD_TABFILE_PE_NECKLACE			"equip\\purple\\necklace"
#define	_KD_TABFILE_PE_CUFF				"equip\\purple\\cuff"
#define	_KD_TABFILE_PE_PENDANT			"equip\\purple\\pendant"
#define	_KD_TABFILE_MEDICINE			"other\\medicine"
#define	_KD_TABFILE_SCRIPT				"other\\scriptitem"
#define _KD_TABFILE_UPGRADE_MATERIAL	"material\\upgrade_material"
#define _KD_TABFILE_BOX					"other\\box"
#define _KD_TABFILE_PET_EQUIP			"pet\\equip"
#define _KD_TABFILE_PET_ITEM			"pet\\other"
#define	_KD_TABFILE_RIDE				"other\\rideitem"
#define _KD_TABFILE_SKILL				"other\\skillitem"
#define _KD_TABFILE_QUEST				"other\\taskitem"
#define _KD_TABFILE_EXTBAG				"other\\extbag"
#define _KD_TABFILE_STUFF				"other\\stuffitem"
#define _KD_TABFILE_PLAN				"other\\planitem"
#define	_KD_TABFILE_GOLDEQUIP			"equip\\goldequip"
#define	_KD_TABFILE_GREENEQUIP			"equip\\greenequip"
#define	_KD_TABFILE_MAGICATTRIB			"magic\\magicattrib"
#define _KD_TABFILE_MAGICACTIVERULE		"magic\\magicactiverule"
#define _KD_TABFILE_GOLDINFO			"equip\\goldinfo"
#define _KD_TABFILE_GREENINFO			"equip\\greeninfo"
#define _KD_TABFILE_EQUIPEFFECT			"equip\\effect"

#define _DECLARE_BPTCLASS(CLASSNAME, INDEX, BASICPROP, TABFILE, LOADRECORDPROC)			\
	class CLASSNAME : public KBPT_BaseT<INDEX, BASICPROP>								\
	{																					\
	public:																				\
		CLASSNAME() : KBPT_BaseT<INDEX, BASICPROP>(TABFILE) {}							\
	protected:																			\
		virtual	VOID LoadRecord(INT i, QTabFile* pTF)									\
		{																				\
			extern VOID LOADRECORDPROC(INT, QTabFile*, BASICPROP*, KINDEX_MAP&);		\
			LOADRECORDPROC(i, pTF, (BASICPROP*)(m_pBuf), m_mapIndex);					\
		}																				\
	}


// 普通装备
_DECLARE_BPTCLASS(KBPT_GE_Sword,		KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_SWORD,		_BPT_GeneralEquip_LoadRecord);		// 剑
_DECLARE_BPTCLASS(KBPT_GE_Spear,		KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_SPEAR,		_BPT_GeneralEquip_LoadRecord);		// 枪
_DECLARE_BPTCLASS(KBPT_GE_Necklace,		KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_NECKLACE,	_BPT_GeneralEquip_LoadRecord);		// 项链
_DECLARE_BPTCLASS(KBPT_GE_Ring,			KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_RING,		_BPT_GeneralEquip_LoadRecord);		// 戒指
_DECLARE_BPTCLASS(KBPT_GE_Pendant,		KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_PENDANT,		_BPT_GeneralEquip_LoadRecord);		// 玉佩
_DECLARE_BPTCLASS(KBPT_GE_Head,			KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_HEAD,		_BPT_GeneralEquip_LoadRecord);		// 头饰
_DECLARE_BPTCLASS(KBPT_GE_Armor,		KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_ARMOR,		_BPT_GeneralEquip_LoadRecord);		// 衣服
_DECLARE_BPTCLASS(KBPT_GE_Belt,			KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_BELT,		_BPT_GeneralEquip_LoadRecord);		// 腰带
_DECLARE_BPTCLASS(KBPT_GE_Cloak,		KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_CLOAK,		_BPT_GeneralEquip_LoadRecord);		// 披风
_DECLARE_BPTCLASS(KBPT_GE_Foot,			KITEM_INDEX,	KBASICPROP_GENERALEQUIP,	_KD_TABFILE_GE_FOOT,		_BPT_GeneralEquip_LoadRecord);		// 鞋子


_DECLARE_BPTCLASS(KBPT_GE_Suit,			KITEM_INDEX,	KBASICPROP_SUIT,	_KD_TABFILE_GE_SUIT,		_BPT_Suit_LoadRecord);		// 套装（外装）
_DECLARE_BPTCLASS(KBPT_GE_Bottom,		KITEM_INDEX,	KBASICPROP_SUIT,	_KD_TABFILE_GE_BOTTOM,		_BPT_Suit_LoadRecord);		// 脚底（外装）
_DECLARE_BPTCLASS(KBPT_GE_Back,			KITEM_INDEX,	KBASICPROP_SUIT,	_KD_TABFILE_GE_BACK,		_BPT_Suit_LoadRecord);		// 背（外装）


// 非装备道具
_DECLARE_BPTCLASS(KBPT_Medicine,		KITEM_INDEX,	KBASICPROP_MEDICINE,		_KD_TABFILE_MEDICINE,		_BPT_Medicine_LoadRecord);		// 药品
_DECLARE_BPTCLASS(KBPT_Stone,			KITEM_INDEX,	KBASICPROP_STONE,			_KD_TABFILE_STONE,			_BPT_Stone_LoadRecord);			// 宝石
_DECLARE_BPTCLASS(KBPT_Script_Item,		KITEM_INDEX,	KBASICPROP_SCRIPT,			_KD_TABFILE_SCRIPT,			_BPT_Script_LoadRecord);		// 脚本道具
_DECLARE_BPTCLASS(KBPT_Pet_Equip,		KITEM_INDEX,	KBASICPROP_SCRIPT,			_KD_TABFILE_PET_EQUIP,		_BPT_Script_LoadRecord);		// 宠物装备
_DECLARE_BPTCLASS(KBPT_Pet_Item,		KITEM_INDEX,	KBASICPROP_SCRIPT,			_KD_TABFILE_PET_ITEM,		_BPT_Script_LoadRecord);		// 宠物道具
_DECLARE_BPTCLASS(KBPT_Ride,			KITEM_INDEX,	KBASICPROP_SCRIPT,			_KD_TABFILE_RIDE,			_BPT_Ride_LoadRecord);			// 坐骑卡 
_DECLARE_BPTCLASS(KBPT_Skill_Item,		KITEM_INDEX,	KBASICPROP_SKILL,			_KD_TABFILE_SKILL,			_BPT_Skill_LoadRecord);			// 技能道具
_DECLARE_BPTCLASS(KBPT_Task_Quest,		KITEM_INDEX,	KBASICPROP_QUEST,			_KD_TABFILE_QUEST,			_BPT_Quest_LoadRecord);			// 任务道具
_DECLARE_BPTCLASS(KBPT_Extend_Bag,		KITEM_INDEX,	KBASICPROP_EXTBAG,			_KD_TABFILE_EXTBAG,			_BPT_ExtBag_LoadRecord);		// 扩展背包
_DECLARE_BPTCLASS(KBPT_Stuff_Item,		KITEM_INDEX,	KBASICPROP_STUFF,			_KD_TABFILE_STUFF,			_BPT_Stuff_LoadRecord);			// 生活技能材料
_DECLARE_BPTCLASS(KBPT_Plan_Item,		KITEM_INDEX,	KBASICPROP_PLAN,			_KD_TABFILE_PLAN,			_BPT_Plan_LoadRecord);			// 生活技能配方
_DECLARE_BPTCLASS(KBPT_Upgrade_Material, KITEM_INDEX,	KBASICPROP_SCRIPT,			_KD_TABFILE_UPGRADE_MATERIAL, _BPT_Script_LoadRecord);		// 升级装备材料
_DECLARE_BPTCLASS(KBPT_Box,				KITEM_INDEX,	KBASICPROP_SCRIPT,			_KD_TABFILE_BOX,			_BPT_Script_LoadRecord);		// 宝箱

// 相关配置
_DECLARE_BPTCLASS(KBPT_MagicAttrib,		KMAGIC_INDEX,	KBASICPROP_MAGICATTRIB,		_KD_TABFILE_MAGICATTRIB,	_BPT_MagicAttrib_LoadRecord);	// 蓝色装备魔法属性表
_DECLARE_BPTCLASS(KBPT_GoldInfo,		INT,			KBASICPROP_SUITEINFO,		_KD_TABFILE_GOLDINFO,		_BPT_SuiteInfo_LoadRecord);		// 黄金装备套装名称表
_DECLARE_BPTCLASS(KBPT_GreenInfo,		INT,			KBASICPROP_SUITEINFO,		_KD_TABFILE_GREENINFO,		_BPT_SuiteInfo_LoadRecord);		// 绿色装备套装名称表
_DECLARE_BPTCLASS(KBPT_EquipEffect,		INT,			KBASICPROP_EQUIPEFFECT,		_KD_TABFILE_EQUIPEFFECT,	_BPT_EquipEffect_LoadRecord);	// 装备特效表
_DECLARE_BPTCLASS(KBPT_MagicActiveRule,	INT,			KBASICPROP_MAGICACTIVERULE,	_KD_TABFILE_MAGICACTIVERULE,_BPT_MagicActiveRule_LoadReord);// 魔法属性激活规则表

#undef _DECLARE_BPTCLASS

#undef _KD_TABFILE_GE_MELEEWEAPON
#undef _KD_TABFILE_GE_RANGEWEAPON
#undef _KD_TABFILE_GE_ARMOR
#undef _KD_TABFILE_GE_HELM
#undef _KD_TABFILE_GE_BOOT
#undef _KD_TABFILE_GE_BELT
#undef _KD_TABFILE_GE_AMULET
#undef _KD_TABFILE_GE_RING
#undef _KD_TABFILE_GE_NECKLACE
#undef _KD_TABFILE_GE_BRACERS
#undef _KD_TABFILE_GE_PENDANT
#undef _KD_TABFILE_PE_MELEEWEAPON
#undef _KD_TABFILE_PE_RANGEWEAPON
#undef _KD_TABFILE_PE_ARMOR
#undef _KD_TABFILE_PE_HELM
#undef _KD_TABFILE_PE_BOOTS
#undef _KD_TABFILE_PE_BELT
#undef _KD_TABFILE_PE_AMULET
#undef _KD_TABFILE_PE_RING
#undef _KD_TABFILE_PE_NECKLACE
#undef _KD_TABFILE_PE_CUFF
#undef _KD_TABFILE_PE_PENDANT
#undef _KD_TABFILE_PE_BOTTOM


#undef _KD_TABFILE_MEDICINE
#undef _KD_TABFILE_SCRIPT
#undef _KD_TABFILE_SKILL
#undef _KD_TABFILE_QUEST
#undef _KD_TABFILE_EXTBAG
#undef _KD_TABFILE_STUFF
#undef _KD_TABFILE_PLAN
#undef _KD_TABFILE_GOLDEQUIP
#undef _KD_TABFILE_GREENEQUIP
#undef _KD_TABFILE_MAGICATTRIB
#undef _KD_TABFILE_GOLDINFO
#undef _KD_TABFILE_GREENINFO
#undef _KD_TABFILE_EQUIPEFFECT

//------------------------------------------------------------------------

#endif		// #ifndef KBasPropTblH
