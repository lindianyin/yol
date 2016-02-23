
#ifndef itemdef_h__
#define itemdef_h__

#include "onlinegameworld/kbaseprotocol.h"

#define F_LOG_PATH_ROOT			"\\log"
#define F_ItemSet_0				        "itemset"
#define		KD_STR_F_ITEM_CDTYPE		"\\setting\\item\\cdtype.txt"
#define		KD_STR_F_EQUIP_SCORE		"\\setting\\item\\equipscore.txt"
#define		KD_STR_F_STONE_SCORE		"\\setting\\item\\stonescore.txt"

#define		EQUIP_ENH_MODULE_NAME		"equip_enhance"
#define		EQUIP_APPEND_MODULE_NAME	"equip_refine"

// 占用32位：genre-8位，detailtype/particular-12位
#define MAKE_ITEM_TYPEID(genre, detailtype, particular)						\
	((genre << 24) + (detailtype << 12) + (particular))

#define		KD_ITEM_RECYCLE_TIME_OUT			15			// 回购商品超时最大天数 15天
#define	KD_NPCRES_DEFAULT	0

#define KD_EQUIP_MAX_LEVEL	7	// 装备最大等级

#pragma pack(1)

struct TDBItemData
{
	BYTE 	byVersion;					// 版本
	WORD	wGenre;						// 大类
	WORD	wDetailType;				// 小类
	WORD	wParticularType;			// 小小类
	BYTE	byLevel;					// 等级
	DWORD	dwGenTime;					// 生成时间
	DWORD 	dwRandSeed;					// 随机种子
	DWORD	dwRandSeedEx;				// 随机种子EX
	WORD	wStackCount;				// 叠加数目
	BYTE	byStateFlags;				// 状态标志，第0位：锁定标志，1表示锁定中；第1位：绑定锁定，1表示绑定中，第2位：0表示买，1表示卖，对应于dwPrice，第3位：租借标志，1表示这个装备是从家族仓库借来的
	BYTE	byPlaceRoom;				// 放置箱位
	BYTE	byPlaceX;					// 放置箱位X坐标
	BYTE	byPlaceY;					// 放置箱位Y坐标
	BYTE	byCustomType;				// 自定义字符串类型
	BYTE	abyCustom[32];				// 自定义字符串
	// TODO: 可以把这下面的东西改为Union，不同大类物品可不同利用
	BYTE	byEnhance;					// 强化次数（仅装备）		
	BYTE	byTimeoutType;				// 超时类型（见KE_ITEM_TIMEOUTTYPE）
	DWORD	dwTimeout;					// 超时时间（内容由byTimeoutType决定）
	GUID	sGuid;						// 物品ID
	DWORD	dwPrice;					// 摆摊价格,大于零才有效（0表示不为买卖状态）,买、卖状态有byStateFlags的第二位确定， 
	DWORD	dwCount;					// 摆摊收购或贩卖的数量，0表示没有买卖状态
	DWORD	dwBuyPrice;					// 购买时价格（奇珍阁商品）
	DWORD	arydwStoneInfo[2];			// 装备镶嵌的宝石信息
	WORD	arywAppendData[10];			// 附加属性
	WORD	arywSaveAppendData[10];		// 保存的附加属性
	BYTE	aryAppendQuality[5];
	BYTE	arySaveAppendQuality[5];
	DWORD	dwKinRepteroryId;			// added by liyun 记录这个装备被放入到哪个家族仓库中了，当装备在玩家的家族背包中有效
	DWORD	arydwExtParam[3];
};

#pragma pack()

enum KE_ITEM_ROOM
{
	emROOM_NONE	= -1,				// 无
	emROOM_EQUIP,					// 装备着的
	emROOM_EQUIPEX,					// 装备切换空间
	emROOM_MAINBAG,					// 主背包
	emROOM_EXTBAGBAR,				// 扩展背包放置栏
	emROOM_EXTBAG,					// 扩展背包
	emROOM_MAIL,					// 邮件附件				// 现在的邮件走的另外一条机制
	emROOM_KIN,                     // 家族背包
	emROOM_NUM,
};

// 道具品质（颜色）
enum KE_ITEM_QUALITY
{
	emQUALITY_WHITE = 1,			// 白色
	emQUALITY_BLUE,					// 蓝色
	emQUALITY_PURPLE,				// 紫色
	emQUALITY_ORANGE,				// 橙色
};

enum KE_EQUIP_POSITION
{
	emEQUIPPOS_SWORD,
	emEQUIPPOS_SPEAR,
	emEQUIPPOS_NECKLACE,			// 项链
	emEQUIPPOS_RING,				// 戒指
	emEQUIPPOS_PENDANT,				// 腰坠
	emEQUIPPOS_HEAD,
	emEQUIPPOS_ARMOR,				// 衣服
	emEQUIPPOS_BELT,				// 腰带
	emEQUIPPOS_CLOAK,				// 披风
	emEQUIPPOS_FOOT,				// 鞋子

	emEQUIPPOS_SUIT,				// 套装
	emEQUIPPOS_BOTTOM,				// 脚下
	emEQUIPPOS_BACK,				// 背
	
	emEQUIPPOS_NUM,
};

enum KE_EQUIPEX_POSITION
{
	emEQUIPEXPOS_SWORD,
	emEQUIPEXPOS_SPEAR,
	emEQUIPEXPOS_NECKLACE,			// 项链
	emEQUIPEXPOS_RING,				// 戒指
	emEQUIPEXPOS_PENDANT,				// 腰坠
	emEQUIPEXPOS_HEAD,
	emEQUIPEXPOS_ARMOR,				// 衣服
	emEQUIPEXPOS_BELT,				// 腰带
	emEQUIPEXPOS_CLOAK,				// 披风
	emEQUIPEXPOS_FOOT,				// 鞋子

	emEQUIPEXPOS_SUIT,				// 套装
	emEQUIPEXPOS_BOTTOM,				// 脚下
	emEQUIPEXPOS_BACK,				// 背

	emEQUIPEXPOS_NUM,
};

enum KE_EXTBAG_POSITION
{
	emEXTBAGPOS_BAG1,				// 扩展背包1
	emEXTBAGPOS_BAG2,				// 扩展背包2
	emEXTBAGPOS_BAG3,				// 扩展背包3
	emEXTBAGPOS_NUM,				// 扩展背包总数
};

enum KE_EXTREP_POSITION
{
	emEXTREPPOS_REP1,				// 扩展储物箱1
	emEXTREPPOS_REP2,				// 扩展储物箱2
	emEXTREPPOS_REP3,				// 扩展储物箱3
	emEXTREPPOS_REP4,				// 扩展储物箱4
	emEXTREPPOS_REP5,				// 扩展储物箱5
	emEXTREPPOS_NUM,
};


// 各ROOM尺寸定义
#define KD_ROOM_EQUIP_WIDTH				emEQUIPPOS_NUM
#define KD_ROOM_EQUIP_HEIGHT			1
#define KD_ROOM_EQUIPEX_WIDTH			emEQUIPEXPOS_NUM
#define KD_ROOM_EQUIPEX_HEIGHT			1
#define	KD_ROOM_MAINBAG_WIDTH			7
#define	KD_ROOM_MAINBAG_HEIGHT			5
#define KD_ROOM_EXTBAGBAR_WIDTH			emEXTBAGPOS_NUM
#define KD_ROOM_EXTBAGBAR_HEIGHT		1
#define KD_ROOM_EXTBAG_WIDTH			1			
#define KD_ROOM_EXTBAG_HEIGHT			1
#define KD_ROOM_EXTBAG1_MAX_WIDTH		133					// 42 * 3 + 7
#define KD_ROOM_EXTBAG1_MAX_HEIGHT		1
#define KD_ROOM_MAIL_WIDTH				1
#define	KD_ROOM_MAIL_HEIGHT				1
#define KD_ROOM_KIN_WIDTH               9
#define KD_ROOM_KIN_HEIGHT              1

// ROOM总大小
#define KD_ROOM_EQUIP_SIZE				(KD_ROOM_EQUIP_WIDTH * KD_ROOM_EQUIP_HEIGHT)
#define KD_ROOM_EQUIPEX_SIZE			(KD_ROOM_EQUIPEX_WIDTH * KD_ROOM_EQUIPEX_HEIGHT)
#define	KD_ROOM_MAINBAG_SIZE			(KD_ROOM_MAINBAG_WIDTH * KD_ROOM_MAINBAG_HEIGHT)
#define KD_ROOM_EXTBAGBAR_SIZE			(KD_ROOM_EXTBAGBAR_WIDTH * KD_ROOM_EXTBAGBAR_HEIGHT)
#define KD_ROOM_EXTBAG_SIZE				(KD_ROOM_EXTBAG_WIDTH * KD_ROOM_EXTBAG_HEIGHT)
#define KD_ROOM_MAIL_SIZE				(KD_ROOM_MAIL_WIDTH * KD_ROOM_MAIL_HEIGHT)
#define KD_ROOM_KIN_SIZE                (KD_ROOM_KIN_WIDTH * KD_ROOM_KIN_HEIGHT)
#define KD_ROOM_EXTBAG1_SIZE		    (KD_ROOM_EXTBAG1_MAX_WIDTH * KD_ROOM_EXTBAG1_MAX_HEIGHT)
#define KD_ROOM_BAGSUM_SIZE             (KD_ROOM_MAINBAG_SIZE + KD_ROOM_EXTBAG1_SIZE)

#define	KD_PLAYER_ITEM_RESERVED			32

#define KD_MAX_PLAYER_ITEM				(KD_ROOM_EQUIP_SIZE + KD_ROOM_EQUIPEX_SIZE + KD_ROOM_MAINBAG_SIZE + \
	KD_ROOM_EXTBAGBAR_SIZE + emEXTBAGPOS_NUM * KD_ROOM_EXTBAG_SIZE + \
	emEXTREPPOS_NUM * KD_PLAYER_ITEM_RESERVED + KD_ROOM_EQUIPEX_SIZE + KD_ROOM_KIN_SIZE )			// 存档道具总数

// 扩展背包各种型号尺寸
#define	KD_ROOM_EXTBAG_WIDTH_4CELL		4			// 4格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_6CELL		6			// 6格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_8CELL		4			// 8格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_10CELL		5			// 10格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_12CELL		6			// 12格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_15CELL		5			// 15格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_18CELL		6			// 18格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_20CELL		5			// 20格背包宽度
#define	KD_ROOM_EXTBAG_WIDTH_24CELL		6			// 24格背包宽度
#define	KD_ROOM_EXTBAG_HEIGHT_4CELL		1			// 4格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_6CELL		1			// 6格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_8CELL		2			// 8格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_10CELL	2			// 10格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_12CELL	2			// 12格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_15CELL	3			// 15格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_18CELL	3			// 18格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_20CELL	4			// 20格背包高度
#define	KD_ROOM_EXTBAG_HEIGHT_24CELL	4			// 24格背包高度

enum KE_PURSE_TYPE						// 钱袋类型
{
	emPURSE_CASH,						// 现金（主背包）
	emPURSE_SAVE,						// 存款（储物箱）
	emPURSE_NUM,
};

enum KE_ENHANCE_MODE
{
	emENHANCE_ONCE = 1,
	emENHANCE_ALL,	// 一键强化
};

struct KITEM_POS						// 道具所在位置
{
	KE_ITEM_ROOM	eRoom;				// 空间
	INT				nX;					// X坐标
	INT				nY;					// Y坐标
	KITEM_POS() : eRoom(emROOM_NONE), nX(0), nY(0) {}

	bool operator == (CONST KITEM_POS &sPos) CONST
	{
		return eRoom == sPos.eRoom && nX == sPos.nX && nY == sPos.nY;
	}

	bool operator != (CONST KITEM_POS &sPos) CONST
	{
		return !operator == (sPos);
	}
};

#define KD_MAGIC_INVALID		0			// 无效的魔法属性ID
#define KD_MAGIC_VALUE_NUM		2			// 魔法属性参数个数
#define KD_RIDE_VALUE_NUM		2			// 乘骑属性参数个数

//	魔法属性
struct KMagicAttrib
{
	INT				nAttribType;					// 属性类型
	INT				nValue[KD_MAGIC_VALUE_NUM];		// 属性参数

	KMagicAttrib(INT nType = KD_MAGIC_INVALID, INT nValue1 = 0, INT nValue2 = 0, INT nValue3 = 0)
		:nAttribType(nType)
	{
		nValue[0] = nValue1;
		nValue[1] = nValue2;
		//nValue[2] = nValue3;
	}
};



struct KITEM_INDEX								// 标识配置表中的一个物品（一行）
{
	INT		nGenre;				// 8Bits
	INT		nDetailType;		// 12Bits
	INT		nParticular;		// 12Bits
	INT		nLevel;

	bool operator < (CONST KITEM_INDEX &sIndex) CONST
	{
		if (nGenre != sIndex.nGenre)
			return	nGenre < sIndex.nGenre;
		if (nDetailType != sIndex.nDetailType)
			return	nDetailType < sIndex.nDetailType;
		if (nParticular != sIndex.nParticular)
			return	nParticular < sIndex.nParticular;
		return	nLevel < sIndex.nLevel;
	}

	bool operator == (CONST KITEM_INDEX &sIndex) CONST
	{
		return	nGenre == sIndex.nGenre && nDetailType == sIndex.nDetailType &&
			nParticular == sIndex.nParticular && nLevel == sIndex.nLevel;
	}

	bool operator != (CONST KITEM_INDEX &sIndex) CONST
	{
		return !operator == (sIndex);
	}
};

struct KPos
{
	INT nX;
	INT nY;
};

enum KE_EQUIP_WEAPON_CATEGORY	// 武器类型，现用于技能对武器的判断
{
	emKEQUIP_WEAPON_CATEGORY_ALL		= 0,
	emKEQUIP_WEAPON_CATEGORY_UNLIMITED	= 0,

	emKEQUIP_WEAPON_CATEGORY_HAND			= 0,	// 空手
	emKEQUIP_WEAPON_CATEGORY_SWORD			= 1,	// 剑
	emKEQUIP_WEAPON_CATEGORY_BOOK			= 2,	// 书
	emKEQUIP_WEAPON_CATEGORY_DOUBLE_SWORD		= 3,	// 双手剑
	emKEQUIP_WEAPON_CATEGORY_SHIELD			= 4,	// 盾牌
	emKEQUIP_WEAPON_CATEGORY_DOUBLE_BLUNT		= 5,	// 双钝器
	emKEQUIP_WEAPON_CATEGORY_BLUNT			= 6,	// 钝器（双手持）
	emKEQUIP_WEAPON_CATEGORY_DOUBLE_KNIFE		= 7,	// 双匕首
	emKEQUIP_WEAPON_CATEGORY_CROSSBOW		= 8,	// 弩箭

	emKEQUIP_WEAPON_CATEGORY_NUM			= 9,
};

// 物品超时类型
enum KE_ITEM_TIMEOUTTYPE
{
	emKITEM_TIMEOUTTYPE_ABSOLUTE,		// 绝对超时时间 单位为秒
	emKITEM_TIMEOUTTYPE_RELATIVE,		// 相对超时时间 单位为游戏帧数
	emKITEM_TIMEOUTTYPE_COUNT,
};

struct KITEM_TIMEOUT
{
	KE_ITEM_TIMEOUTTYPE		emType;
	DWORD					dwTimeout;
};

enum KE_ITEMDATA_FLAG
{
	emKITEMDATA_FLAG_NULL	= 0,		// 
	emKITEMDATA_FLAG_IBSHOP = 0x1,		// 是否奇珍阁商品
};

struct KLISTITEM
{
	KLISTITEM() : nIdx(0)
	{
		sPos.eRoom	= emROOM_NONE;
		sPos.nX		= 0;
		sPos.nY		= 0;
	}
	INT			nIdx;
	KITEM_POS	sPos;
	WORD		wRecycleDate;	// 进入回购空间日期（回购空间有效）
};

enum KE_REPAIR_TYPE			// 道具修理类型
{
	emREPAIR_ERROR,
	emREPAIR_COMMON,		// 普通修理
	emREPAIR_SPECIAL,		// 特殊修理
	emREPAIR_ITEM,			// 使用道具修理
};

// Npc部件顺序枚举（必须跟部件配置文件对应）
enum KE_NPCRES_PART
{
	emNPCRES_PART_HEAD,				// 头部（外装）
	emNPCRES_PART_SHOULDER,			// 肩（外装）
	emNPCRES_PART_BACK,				// 肩（外装）
	emNPCRES_PART_BOTTOM,			// 脚（外装）
	emNPCRES_PART_WEAPON,			// 武器
	emNPCRES_PART_BOTTEM,			// 脚底
	emNPCRES_PART_BODY,				// 衣服
	emNPCRES_PART_CLOAK,			// 披风
	emNPCRES_PART_SUIT,				// 套装

	emNPCRES_PART_COUNT
};

enum KE_STONE_COUNT_ATTR_CONST					// 宝石属性数目常量
{
	emSTONE_MIX_MAX_COUNT = 3,					// 宝石合成最多合成数量
	emSTONE_MAX_LV = 20,						// 宝石最大等级
	emSTONE_COUNT_ATTRIB = 3,					// 宝石具有魔法属性个数
	emSTONE_MAX_SLOT = 3,						// 装备最大宝石孔个数

	emSTONE_LOCKED_ID = 255,					// 锁住的状态id
};

enum KE_REFINE_COUNT_CONST
{
	emREFINE_MAX_LEVEL = 9,		// 洗炼最大9个等级
};

enum KE_MEDICINE_COUNT_ATTR_CONST
{
	emMEDICINE_COUNT_ATTRIB = 3,				// 药品具有魔法属性个数
};

class KRandom			// 随机序列生成器
{

public:

	explicit KRandom(UINT uRandSeed = 42) : m_uRandSeed(uRandSeed) {}

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

class KRandomEx
{

public:

	explicit KRandomEx(UINT uRandSeed = 42) : m_uRandSeedEx(uRandSeed) {}

	UINT GetSeed(VOID) CONST		{ return m_uRandSeedEx; }
	VOID SetSeed(UINT uRandSeed)	{ m_uRandSeedEx = uRandSeed; }
	UINT Random(VOID)				{ return m_uRandSeedEx = m_uRandSeedEx * 3877 + 29573; }
	INT  Random(INT nMin, INT nMax)	
	{
		UINT uRandom = Random();
		if (nMax - nMin >= 0)
			return uRandom % (nMax - nMin + 1) + nMin;
		else
			return nMin;
	}

private:

	UINT				m_uRandSeedEx;
};

static inline INT GetRandomNumber(INT nMin, INT nMax)
{
	return g_Random(nMax - nMin + 1) + nMin;
}

//////////////////////////////////////////////////////////////////////////

enum KS2C_ITEM_PROTOCOL
{
	s2c_syncitem,
	s2c_removeitem,
	s2c_syncmoney,
	s2c_syncjbcoin,				// 同步金币数
	s2c_npcattrib,				// 同步NPC魔法属性到客户端

	s2c_itemride,
	s2c_switchitem,
	s2c_openrepository,			// 打开储物箱
	s2c_sync_ext_repo_state,
	s2c_viewequip,

	s2c_useitem_result,			// 使用道具结果
	s2c_switchitem_end,			// 道具交换结束
	s2c_arrange_end,			// 整理结束
	s2c_sellitems_end,			// 卖出道具结束

	s2c_prepare_itemrepair,		// 通知客户端准备道具修理
	s2c_enhance_result,			// 强化结果
	s2c_itemcountsync,

	s2c_waititemstate,			// 设置等待获取物品锁定状态
	s2c_stone_mix_result,		// 宝石合成结果

	s2c_end,
};

enum KC2S_ITEM_PROTOCOL
{
	c2s_switchitem,         
	c2s_playeruseitem,      //使用物品
	c2s_resizeextbag,       //解锁背包
	c2s_arrangebag,			// 背包整理
	//c2s_playerpickupitem,
	c2s_playersellitem,
	c2s_playerbuyitem,
	c2s_playerthrowawayitem,
	c2s_viewequip,
	c2s_switchequip,			// 装备切换
	c2s_repairitem,
	c2s_itemride,				// 上下马
	c2s_applyenhance,			// 装备强化请求
	c2s_upgrade,
	c2s_refine,					// 洗炼
	c2s_save_appendattr,		// 保存附加属性
	c2s_applybreakup,			// 装备拆解

	c2s_closerepository,		// 关闭储物箱
	c2s_splititem,				// 道具拆分请求
	c2s_playeruseitem_with_param,	// 带参数使用道具
	c2s_stone_mix,				// 宝石合成
	c2s_stone_mount,			// 宝石镶嵌
	c2s_stone_remove,			// 宝石拆除
	c2s_stone_punch,			// 打孔
	c2s_equip_rerand,			// 装备重铸
	c2s_equip_identify,			// 装备鉴定

	c2s_reapply_pendant_request,

	c2s_end,
};

#pragma pack(1)

struct KPROTO_ITEMPOS
{
	BYTE	byRoom;
	BYTE	byX;
	BYTE	byY;
};

struct KPLAYER_USEITEM : KPTC_HEADER<c2s_playeruseitem>		// 玩家鼠标右键点击使用物品
{
	BYTE	byRoom;			// 物品位置
	BYTE	byX;			// 物品x坐标
	BYTE	byY;			// 物品y坐标
};

struct KPLAYER_RESIZE_EXTBAG : KPTC_HEADER<c2s_resizeextbag>
{
	BYTE		byRoom;
	BYTE		byDestWidth;
	BYTE		byDestHeight;
};

struct KPLAYER_USEITEM_RESULT : KPTC_HEADER<s2c_useitem_result>		// 使用道具结果
{
	DWORD	dwID;			// 道具ID
	BYTE	bResult;		// 结果
};

struct KITEM_REPAIR_SYNC : KLENPTC_HEADER<c2s_repairitem>			// 修理
{
	BYTE	byType;
	BYTE	byCount;
	DWORD	dwUseItemID;
	DWORD	dwItemID[1];
};

struct KITEM_C2S_SPLIT_ITEM : KLENPTC_HEADER<c2s_splititem>			// 拆分
{
	DWORD dwItemId;
	BYTE bySplitCount;	// 拆分个数
};

struct KSYNC_PREPARE_ITEMREPAIR : KPTC_HEADER<s2c_prepare_itemrepair>	// 通知客户端使用道具对装备进行修理
{
	DWORD	dwUseItemID;	// 道具修理时的消耗品ID
};

struct KAPPEND_VIEW_INFO
{
	BYTE byAttrib;
	WORD wValue;
};

struct KVIEWITEMINFO
{
	DWORD	dwID;
	BYTE	byGenre;
	WORD	wDetail;
	WORD	wParticular;
	DWORD	dwLevel;
	DWORD	dwRandomSeed;
	DWORD	dwRandomSeedEx;
	WORD	wCount;
	BYTE	byEnhance;

	BYTE	byBaseAttrib;		// 基础属性
	WORD	wBaseValue;			// 基础属性值
	WORD	wBaseNextValue;		// 下一级基础属性值
	WORD	wEnhValue;			// 强化属性值
	WORD	wEnhNextValue;		// 下一级强化属性值
	DWORD	dwEnhanceCost;		// 强化费用
	KAPPEND_VIEW_INFO aryAppendInfo[5];	// 附加属性
	KAPPEND_VIEW_INFO arySaveAppendInfo[5];		// 已保存的附加属性
	BYTE	aryAppendQuality[5];
	BYTE	arySaveAppendQuality[5];
	BYTE	byStoneInfo[6];			// 宝石信息

	BYTE	bBind;		// TRUE, FALSE
	BYTE	byTimeoutType;		// 超时时间类型，0为绝对时间，1为相对时间（帧数） (KE_ITEM_TIMEOUTTYPE, 2)

	DWORD	dwTimeout;				// 超时时间
	DWORD	dwFightScore;			// 战斗力
};

struct KITEM_EXTINFO
{
	BYTE			byRoom;				// 空间
	BYTE			byX;				// X坐标
	BYTE			byY;				// Y坐标
};
struct KITEM_SYNC : KLENPTC_HEADER<s2c_syncitem>, KITEM_EXTINFO, KVIEWITEMINFO
{
};

struct KITEM_C2S_SWITCH_SYNC : KPTC_HEADER<c2s_switchitem>
{
	BYTE	byPickRoom;
	BYTE	byPickX;
	BYTE	byPickY;
	BYTE	byDropRoom;
	BYTE	byDropX;
	BYTE	byDropY;
};

struct KITEM_C2S_ARRANGE_BAG : KPTC_HEADER<c2s_arrangebag>
{

};

struct KITEM_S2C_SWITCH_SYNC : KPTC_HEADER<s2c_switchitem>
{
	BYTE	bSuccess;
	BYTE	byPickRoom;
	BYTE	byPickX;
	BYTE	byPickY;
	BYTE	byDropRoom;
	BYTE	byDropX;
	BYTE	byDropY;
};

struct KITEM_C2S_VIEW_EQUIP: KPTC_HEADER<c2s_viewequip>
{
	DWORD m_dwPlayerIdx;
};

struct KITEM_REMOVE_SYNC : KPTC_HEADER<s2c_removeitem>
{
	DWORD	dwID;				// 物品的ID
};

struct KITEM_COUNT_SYNC : KPTC_HEADER<s2c_itemcountsync>
{
	DWORD	dwId;
	WORD	wCount;
};

struct KITEM_C2S_ENHANCE_APPLY_SYNC : KPTC_HEADER<c2s_applyenhance>
{
	DWORD	dwEquipId;
	BYTE	byMode;
};

struct KITEM_C2S_UPGRADE_APPLY_SYNC : KPTC_HEADER<c2s_upgrade>
{
	DWORD	dwEquipId;
};

struct KITEM_C2S_REFINE_APPLY_SYNC : KPTC_HEADER<c2s_refine>
{
	DWORD dwEquipId;
	BYTE byMoneyType;

	BYTE aryLock[5];
};

struct KITEM_C2S_SAVE_REFINE_ATTR : KPTC_HEADER<c2s_save_appendattr>
{
	DWORD dwEquipId;
	BYTE bySaveOriginal;
};

struct KITEM_C2S_EQUIP_IDENTIFY : KPTC_HEADER<c2s_equip_identify>
{
	DWORD	dwEquipId;
};

struct KITEM_C2S_STONE_MIX_SYNC : KLENPTC_HEADER<c2s_stone_mix>
{
	BYTE byStoneCount;
	DWORD adwStoneId[emSTONE_MIX_MAX_COUNT];
};

struct KITEM_C2S_SELL_ITEMS : KLENPTC_HEADER<c2s_playersellitem>
{
	BYTE byItemCount;
	DWORD adwItemIds[0];
};

struct KITEM_C2S_EQUIP_RERAND_SYNC : KPTC_HEADER<c2s_equip_rerand>
{
	DWORD	dwEquipId;
	BYTE	byMode;				// 重铸模式,0=普通重铸,1=一键重铸
	BYTE	byNeedScore;		// 需要的分数，模式为1才需要
};

struct KITEM_C2S_STONE_MOUNT_SYNC : KLENPTC_HEADER<c2s_stone_mount>
{
	DWORD	dwEquipId;
	DWORD	adwStoneId[0];
};

struct KITEM_C2S_STONE_REMOVE_SYNC : KLENPTC_HEADER<c2s_stone_remove>
{
	DWORD	dwEquipId;
	BYTE	abySlot[0];
};

struct KITEM_C2S_STONE_PUNCH : KPTC_HEADER<c2s_stone_punch>
{
	DWORD dwEquipId;
	BYTE byPos;	// 从0开始
};

struct KITEM_S2C_ENHANCE_RESULT_SYNC : KPTC_HEADER<s2c_enhance_result>
{
	BYTE	byMode;
	BYTE	byResult;		// -1:无操作，0:强化/剥离失败，1:强化/剥离成功
};

struct KITEM_S2C_SWITCH_END : KPTC_HEADER<s2c_switchitem_end>
{
};

struct KITEM_S2C_ARRANGE_END : KPTC_HEADER<s2c_arrange_end>
{

};

struct KITEM_S2C_SELLITEMS_END : KPTC_HEADER<s2c_sellitems_end>
{

};

struct KITEM_THROWAWAY_SYNC : KPTC_HEADER<c2s_playerthrowawayitem>
{
	DWORD	dwId;
};

struct KSWITCH_EQUIP_PROTOCOL : KLENPTC_HEADER<c2s_switchequip>
{
	struct KPAIR
	{
		BYTE	byEquipPos;
		BYTE	byEquipExPos;
	};
	BYTE		byCount;		// 此次切换装备对数（也就是sPairs数组的大小）
	KPAIR		asPair[0];		// 不定大小数组
};	// 切换玩家装备（一次切换多个）

struct S2C_OPEN_REPOSITORY : KPTC_HEADER<s2c_openrepository>
{
};
struct C2S_CLOSE_REPOSITORY : KPTC_HEADER<c2s_closerepository>
{
};

struct C2S_REAPPLY_PENDANTS_REQUEST : KPTC_HEADER<c2s_reapply_pendant_request>
{

};

struct S2C_SYNC_EXT_REPO_STATE : KPTC_HEADER<s2c_sync_ext_repo_state>
{
	BYTE		byState;
};

struct KPLAYER_EQUIPVIEW
{
	//DWORD				dwNpcID;
	// 32Bits
	DWORD				bySex		:2;
	DWORD            	byPKValue	:4;	// (KD_MAX_PKVALUE, 10)
	DWORD				byFaction	:4;	// (KD_MAX_FACTION, 12)
	DWORD				nPortrait	:14;
	DWORD				byLevel		:8;	// 级别

	CHAR				szRoleName[_NAME_LEN];		// 玩家名字
	BYTE				byItemCount;
	CHAR				szBuffer[1];
};
struct KVIEW_EQUIP_SYNC : KLENPTC_HEADER<s2c_viewequip>
{
	KPLAYER_EQUIPVIEW sView;
	BYTE byData[]; // KVIEWITEMINFO[]
};

#pragma pack()

#endif // itemdef_h__
