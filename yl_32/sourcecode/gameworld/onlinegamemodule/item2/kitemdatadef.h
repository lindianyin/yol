
#ifndef __KITEMDATADEF_H__
#define __KITEMDATADEF_H__

#include "onlinegamemodule/item2/itemdef.h"

// -------------------------------------------------------------------------

#define	ITEM_INVALID_TYPE				0			// 无效的装备类别号		Modify by NiuNiu 
#define	KD_ITEM_MAX_STACK_COUNT			1			// 默认的物品叠放最大值

#define KD_ITEM_NO_LEVEL_LIMIT			0			// 无级别限制
#define KD_ITEM_NO_BIND_LIMIT			-1			// 无绑定限制
#define KD_ITEM_NO_TIMEOUT_LIMIT		-1			// 无 物品过期时间 限制
#define KD_ITEM_INVALID_SUITEID			0			// 无效的套装号
#define	KD_ITEM_MIN_LEVEL				1
#define KD_ITEM_MAX_EQUIP_LEVEL			10			// 装备的最大级别
#define KD_ITEM_ABRADE_SECOND			5			// 每5秒做一次掉耐久操作
#define KD_MYSTERY_GOOD_UPDATE_SECOND	7200			// 神秘商店的固定刷新时间(s)
#define KD_ITEM_ABRADE_PRE_TIME			2			// 每次掉2点		

#define KD_ISEQUIP(g)				((g) == item_equip_general && (g) <= item_equip_max)	// 根据道具类型判断是否装备道具
#define KD_ISSTONE(g, d)			((g) == item_script && (d) == item_script_stone)
#define KD_ISPETEQUIP(g, d)			((g) == item_pet && (d) == item_pet_equip)
#define KD_ISSUITE(g)				FALSE/*((g) == item_equip_gold || (g) == item_equip_green)*/		// 根据道具类型判断是否套装装备
#define KD_ISWEAPON(d)				((d) == equip_sword)/*((d) == equip_meleeweapon || (d) == equip_rangeweapon)*/	// 根据道具细类判断是否武器（不检查大类）
#define KD_ISORNAMENT(nPos)			((nPos) >= emEQUIPPOS_AMULET && (nPos) <= emEQUIPPOS_PENDANT)	// 根据装备位置判断是否为首饰
#define KD_ISEQUIP_ROOM(nRoom)		((nRoom) == emROOM_EQUIP ||	(nRoom) == emROOM_EQUIPEX )			// 是否能是装备空间
#define KD_ISMEDICINE(g)			((g) == item_medicine)											// 是否药品


//------------------------------------------------------------------------

enum KE_ITEM_GENRE								// 道具类型
{
	item_equip_general		= 1,				// 白色装备和蓝色装备
	//item_equip_purple		= 2,				// 紫色装备
	//item_equip_gold			= 3,				// 黄金装备
	//item_equip_green		= 4,				// 绿色装备

	item_equip_max		= 16,

	// 5 - 16 为装备预留
	item_medicine			= 17,				// 药品
	item_script				= 18,				// 脚本物品
	item_skill				= 19,				// 技能物品
	item_quest				= 20,				// 任务物品
	item_extbag				= 21,				// 扩展背包
	item_stuff				= 22,				// 生活技能材料
	item_plan				= 23,				// 生活技能配方
	item_pet				= 24,				// 宠物道具
};

enum KE_ITEM_EQUIP_DETAILTYPE					// 装备详细类别
{
	equip_sword	= 1,				// 剑
	equip_spear = 2,				// 护符
	equip_necklace = 3,				// 项链
	equip_ring = 4,					// 戒指
	equip_pendant = 5,				// 腰坠
	equip_head = 6,					// 头饰
	equip_armor	= 7,				// 衣服
	equip_belt = 8,					// 腰带
	equip_cloak = 9,				// 披风
	equip_foot = 10,				// 鞋子

	equip_suit = 11,					// 套装(外装)
	equip_bottom = 12,					// 脚底（外装）
	equip_back = 13,					// 背（外装）

	equip_detailnum			= 15,
};

enum KE_ITEM_SCRIPT_DETAILTYPE					// 脚本道具详细类别
{
	item_script_other = 1, // 其他
	item_script_stone = 2,	// 宝石
	item_script_ride = 3,	// 坐骑卡
	item_upgrade_material = 4,	// 升级材料
	item_script_box = 5,		// 宝箱
};

enum KE_ITEM_PET_DETAILTYPE
{
	item_pet_equip = 1,		// 宠物装备
	item_pet_develop = 2,	// 宠物升品升阶物品
	item_pet_food = 3,		// 宠物食物
	item_pet_toy = 4,		// 宠物玩具
	item_pet_exp = 5,		// 经验
	item_pet_rerand = 6,	// 驯化
	item_pet_skill = 7,		// 技能书
};



enum KE_ITEM_MEDICINE_DETAILTYPE				// 药品详细类别
{
	medicine_blood			= 1,				// 补血
	medicine_mana			= 2,				// 补魔
	medicine_both			= 3,				// 补双
	medicine_stamina		= 4,				// 补体力
	medicine_antipoison		= 5,				// 解毒
	medicine_detailnum		= 5,
};

enum KE_ITEM_EXTBAG_DETAILTYPE					// 扩展背包详细类别
{
	extbag_4cell			= 1,				// 4格扩展背包
	extbag_6cell			= 2,				// 6格扩展背包
	extbag_8cell			= 3,				// 8格扩展背包
	extbag_10cell			= 4,				// 10格扩展背包
	extbag_12cell			= 5,				// 12格扩展背包
	extbag_15cell			= 6,				// 15格扩展背包
	extbag_18cell			= 7,				// 18格扩展背包
	extbag_20cell			= 8,				// 20格扩展背包
	extbag_24cell			= 9,				// 24格扩展背包
	extbag_detailnum		= 9,
};

enum KE_ITEM_TRADETYPE							// 交易类型
{
	emITEM_TRADE_NONE		= 0,				// 不可交易
	emITEM_TRADE_ALLOW		= 1,				// 可交易
};

enum KE_ITEM_DESTROYTYPE						// 摧毁类型
{
	emITEM_DESTROY_NONE		= 0,				// 不可摧毁
	emITEM_DESTROY_ALLOW	= 1,				// 可摧毁
};

enum KE_ITEM_BINDTYPE							// 绑定类型
{
	emITEM_BIND_NONE		= 0,				// 不绑定
	emITEM_BIND_GET			= 1,				// 获取时绑定，可以卖给NPC
	emITEM_BIND_EQUIP		= 2,				// 装备时绑定
	emITEM_BIND_OWN			= 3,				// 获取时绑定，不可以卖给NPC
	emITEM_BIND_NONE_OWN	= 4,				// 不绑定，不可以卖给NPC
};

enum KE_SUITE_TYPE								// 套装类型
{
	emITEM_SUITE_ERROR		= 0,
	emITEM_SUITE_GOLD		= 1,				// 黄金装备
	emITEM_SUITE_GREEN		= 2,				// 绿色装备
};

enum KE_ITEM_REQUIREMENT						// 道具需求属性
{
	emEQUIP_REQ_NONE		= 0,
	emEQUIP_REQ_ROUTE		= 1,				// 路线
	emEQUIP_REQ_LEVEL		= 2,				// 级别
	emEQUIP_REQ_FACTION		= 3,				// 门派
	emEQUIP_REQ_SEX			= 4,				// 性别
};

//------------------------------------------------------------------------

enum KE_ITEM_COUNT_ATTR_CONST					// 道具属性数目常量
{
	emITEM_COUNT_BASE		= 1,				// 基本属性的数目
	emITEM_COUNT_REQ		= 6,				// 需求属性的数目
	emITEM_COUNT_RANDOM		= 4,				// 道具随机魔法属性的数目
	emITEM_COUNT_ENHANCE	= 3,				// 道具档次的数目
	emITEM_COUNT_ENHATTR	= 1,				// 道具增强附加属性的数目
	emITEM_COUNT_RIDE		= 6,				// 上马激活属性的数目
	emITEM_COUNT_SUITE		= 4,				// 套装激活附加属性的数目
	emITEM_COUNT_EXTPARAM	= 10,				// 道具扩展参数数目
	emITEM_COUNT_STRENGTHEN	= 2,				// 道具改造魔法属性的数目
	emITEM_COUNT_ACITVEMA	= 10,				// 道具激活属性的数目, 加到10条，方便以后扩展
	emITEM_COUNT_APPEND		= 5,				// 附加属性最大数目
	emITEM_COUNT_UPGRADE	= 3,				// 升级所需材料最大数目
	emITEM_COUNT_EX_PENDANT = 3,				// 镶嵌宝石特效数目
	emITEM_COUNT_SUIT_EX_PENDANT = 4,			// 套装额外挂件数目
};

enum KE_MATF_COUNT_CONST
{
	emMATF_COUNT_EQUIPTYPE	= equip_detailnum,				// 装备类型数
	emMATF_COUNT_BRIGHTDARK	= 2,							// 明暗属性
};

enum KE_MONEY_TYPE
{
	emBIND_MONEY,				// 绑定银两
	emNORMAL_MONEY,				// 普通银两
};

//------------------------------------------------------------------------

struct KMAGIC_INDEX						// 标识配置表中的一个魔法属性（一行）
{
	INT		nAttrib;					// 魔法ID
	INT		nLevel;						// 魔法属性等级

	bool operator < (CONST KMAGIC_INDEX &sIndex) CONST
	{
		if (nAttrib != sIndex.nAttrib)
			return	nAttrib < sIndex.nAttrib;
		return	nLevel < sIndex.nLevel;
	}

	bool operator == (CONST KMAGIC_INDEX &sIndex) CONST
	{
		return	!(*this < sIndex) && !(sIndex < *this);
	}
};

// -------------------------------------------------------------------------

#endif /* __KITEMDATADEF_H__ */
