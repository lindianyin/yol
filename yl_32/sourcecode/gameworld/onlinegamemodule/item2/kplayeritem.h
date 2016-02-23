
#ifndef __KPLAYERITEM_H__
#define __KPLAYERITEM_H__

#include "kitem.h"
#include "kitemroom.h"
#include "kcdtime.h"
#include "kdupeditemdeal.h"
#include "kswitchitem.h"
#include "kitemtimeoutctrl.h"
#include <set>
#include <list>
#include <string>
#include <map>
#include <vector>
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"

#ifndef GAME_SERVER
#include "client_core/iinlinectrldata.h"
#endif


class KLuaPlayerItem;

// 20100325：对绑银的消耗和使用也做获得和失去途径的统计，其在程序中的相关枚举应用与银两的一致
// 在脚本中才有所不同
enum KE_MONEY_EARN_TYPE
{
	emKEARN_GM = 0,
	emKEARN_PLAYER_TRADE,	// 玩家交易
	emKEARN_SELL,			// 卖店
	emKEARN_FARM,			// 打怪获得
	emKEARN_NPC_DROP,		// 怪物提落
	emKEARN_PICK,			// 玩家拾取
	emKEARN_EXCHANGE_BACK,	// 跨服归还
	emKEARN_GIVE,			// Give给予
	emKEARN_STALL_SELL,		// 贩卖获得
	emKEARN_STALL_BUY,		// 回购卖出获得
	emKEARN_MAIL,			// 邮件附件
	emKEARN_JBEXCHANGE_CASH, // 金币交易所帐户
	emKEARN_JBEXCHANGE_BACK, // 金币交易所返还
	// 100以上留给脚本
	emKEARN_SCRIPT_BEGIN = 100,
	emKEARN_SCRIPT_TONG_FUN = 110,	// 脚本操作，帮会取钱
	emKEARN_SCRIPT_TONG_DISPAND = 111,	// 脚本操作，帮会发钱
	emKEARN_SCRIPT_EXCHANGE_BUYFAIL = 121,	// 金币交易所购买金币失败，银两返还
	emKEARN_SCRIPT_DRAWBANK	= 123,	// 从钱庄取出银两
};

enum KE_MONEY_PAY_TYPE
{
	emKPAY_NONE = 0,
	emKPAY_PLAYER_TRADE,	// 玩家交易
	emKPAY_STALL_SELL,		
	emKPAY_STALL_BUY,		 
	emKPAY_SHOP,		
	emKPAY_GIVE,			// 直接给?
	emKPAY_PREPAY,			// 付订金
	emKPAY_SPEECH,
	emKPAY_DIC_GAME,
	emKPAY_IBSHOP,
	emKPAY_BUY_RECYCLE,
	emKPAY_SHITU,
	emKPAY_SENDMAIL,		// 通过邮件发钱给某人
	emKPAY_KINMAIL,			// 发家族邮件时的消耗
	emKPAY_AUCTION_TAX,
	emKPAY_AUCTION_PRICE,
	emKPAY_MAIL_COST,		// 发邮件的消耗

	// 100以上留给脚本
	emKPAY_SCRIPT_BEGIN = 100,
	emKPAY_SCRIPT_JBEXCHANGE = 108,	  // 脚本操作，金币交易所
	emKPAY_SCRIPT_TONGFUD = 118,	// 存入帮会资金
	emKPAY_SCRIPT_PEEL = 120,	// 玄晶剥离
	emKPAY_SCRIPT_RESTOREBANK	= 122,	// 银两存入钱庄
};

enum KE_ADD_ITEM_WAY
{
	emKADDITEM_NONE = 0,
	emKADDITEM_LOAD,		// 加载
	emKADDITEM_BUY,			// 普通商店
	emKADDITEM_IBSHOP,		// 奇珍阁
	emKADDITEM_IBCOIN,		// 奇珍阁金币购买
	emKADDITEM_MAIL,		// 邮件
	emKADDITEM_TRADE,		// 交易
	emKADDITEM_PICK,		// 捡起
	emKADDITEM_RECYCLE,		// 回购
	emKADDITEM_TO_RECYCLE,	// 加入到回购
	emKADDITEM_STALL_SELL,	// 摆摊购买
	emKADDITEM_STALL_RETURN,// 摆摊归还
	emKADDITEM_STALL_STACK,	// KPlayerStall::BuyStackableItem
	emKADDITEM_STALL_BUY,	// 收购
	emKADDITEM_TEAMSHARE,	// 组队分配
	emKADDITEM_NPCDROP,		// npc掉到身上
	emKADDITEM_REGEN,		// Regenerate
	emKADDITEM_SPLIT,		// 拆分
	emKADDITEM_MANTLE,		// 披风
	emKADDITEM_MYSTERY_SHOP_BUY,	// 神秘商店
	emKADDITEM_KIN,         // 家族仓库
	emKADDITEM_PET_EQUIP,	// 宠物装备
	emKADDITEM_PET_ITEM,	// 宠物道具
	emKADDITEM_BATTLE_ZONE,	// 跨服道具

	emKADDITEM_BYSCRIPT = 100, // 脚本增加
	emKADDITEM_BYSCRIPT_PEEL_EQUIP,		//装备剥离
	emKADDITEM_BYSCRIPT_COMPOSE,		// 玄晶合成
	emKADDITEM_BYSCRIPT_PRODUCE,		// 生活技能制造
	emKADDITEM_BYSCRIPT_FINISHTASK,		// 任务完成奖励
	emKADDITEM_BYSCRIPT_BREAKUP = 107,	 // 玄晶、装备拆解
	emKADDITEM_BYSCRIPT_PEEL_PARTNER,	// 剥离同伴
	emKADDITEM_BYSCRIPT_CYSTAL_COMPOSE,	// 水晶合成
	emKADDITEM_BYSCRIPT_MANTLE_SHOP,	// 披风换魂石
};

enum KE_LOSE_ITEM_WAY
{
	emKLOSEITEM_NONE = 0,
	emKLOSEITEM_SELL,
	emKLOSEITEM_TRADE,
	emKLOSEITEM_RECYCLE,
	emKLOSEITEM_BUYRECYCLE,
	emKLOSEITEM_USE,
	emKLOSEITEM_THROW,
	emKLOSEITEM_MAIL,		// 邮件
	emKLOSEITEM_TIMEOUT,
	emKLOSEITEM_STALL_SELL,
	emKLOSEITEM_STALL_BUY,
	emKLOSEITEM_STALL_BUY_STACK,
	emKLOSEITEM_PICK,		// 拾起删除
	emKLOSEITEM_BUYFAIL,
	emKLOSEITEM_STACK,
	emKLOSEITEM_SET_STACK,
	emKLOSEITEM_CONSUME,
	emKLOSEITEM_REGENE,		// Regenerate
	emKLOSEITEM_FAVOR,		// 亲密度道具
	emKLOSEITEM_DUP,
	emKLOSEITEM_BYGDP,		// Remove by g,d,p
	emKLOSEITEM_FEATURE,
	emKLOSEITEM_MANTLE,
	emKLOSEITEM_AUCTION,
	emKLOSEITEM_MYSTERY_SHOP_SALE,
	emKLOSEITEM_RETURN_TO_KIN_REPERTORY,
	emKLOSEITEM_PUTON_PET_EQUIP,	// 穿宠物装备


	emKLOSEITEM_SCRIPT = 100,
	emKLOSEITEM_SCRIPT_COMPOSE,		//玄晶合成
	emKLOSEITEM_SCRIPT_ENHANCE,		//强化时扣除的玄晶
	emKLOSEITEM_SCRIPT_EVENTUSE,	// 活动扣除
	emKLOSEITEM_SCRIPT_TASKUSE,		// 任务扣除
	emKLOSEITEM_SCRIPT_BREAKUP = 106,		// 玄晶、装备拆解
	emKLOSEITEM_SCRIPT_CHANGE_HUN,		// 兑换魂石
	emKLOSEITEM_SCRIPT_REPAIR,			// 修理品消耗
	emKLOSEITEM_SCRIPT_SIGNET,			// 升级五行印
	emKLOSEITEM_SCRIPT_KILLER,			// 官府通缉任务兑换
	emKLOSEITEM_SCRIPT_MANTLE_SHOP = 112,		// 披风换魂石
	emKLOSEITEM_SCRIPT_STRENGTHEN =113 ,  // 改造扣除玄晶
	emKLOSEITEM_SCRIPT_PARTNER_TALENT,  //喂同伴领悟度
	emKLOSEITEM_SCRIPT_CYSTAL_COMPOSE,	// 水晶合成
	emKLOSEITEM_SCRIPT_STONE_MIX = 117,		// 宝石合成
	emKLOSEITEM_SCRIPT_STONE_MOUNT,			// 宝石镶嵌
};

// 绑金变化枚举值
enum KE_BINDCOIN_ADD_WAY
{
	emKBINDCOINADD_NONE = 0,
	emKBINDCOINADD_FRIEDN,	// 密友返还

	emKBINDCOINADD_SCRIPT = 100,
};

enum KE_BINDCOIN_COST_WAY
{
	emKBINDCOINCOST_NONE = 0,
	emKBINDCOINCOST_SHOP,		// 奇珍阁绑金区购买

	emKBINDCOINCOST_SCRIPT = 100,
};

//---------------------------------------------------------------------------

// 该宏把切换装备的位置转化为实际装对应的位置
#define KD_EQEXPOS2EQPOS(equipexpos)	((KE_EQUIP_POSITION)equipexpos)	// 目前定义中两者是相等的(见KE_EQUIP_POSITION，KE_EQUIPEX_POSITION定义)

struct BuySellInfo	// BuyInfo除用于商店外，还用于储存开箱状态，给予界面状态等
	// BuyInfo随时会Clear，因此使用时要检验
{
	enum
	{
		BUY_INDEX_NULL			= -1,			// 空值状态
		BUY_INDEX_REPOSITORY	= -2,			// 开储物箱状态
		BUY_INDEX_GIFT			= -3,			// 给予界面状态
		BUY_INDEX_AUCTION		= -4,			// 拍卖行界面状态
	};

	INT				m_nBuyIdx;
	DWORD			m_dwSceneId; // 和npc对话时的位置
	INT				m_nPosX;
	INT				m_nPosY;
	INT				m_nPosZ;
	//INT				m_nScale;
	//KCURRENCYTYPE	m_eCurrencyType;
	//CHAR			m_szCallBackFunc[100]; //回调脚本函数名

	VOID Clear()
	{
		m_nBuyIdx			= BUY_INDEX_NULL;
		m_dwSceneId		= (DWORD)-1;
		m_nPosX				= 0;
		m_nPosY				= 0;
		//m_nScale			= 100;
	}
	BOOL InShop() CONST
	{
		return m_nBuyIdx != BUY_INDEX_NULL;
	}
};
//---------------------------------------------------------------------------

class KPlayerItem : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_ITEM	};

	enum KE_HOLDSTATUS				// 物品拥有状态
	{
		emHOLDSTATUS_NONE,			// 不拥有此物品
		emHOLDSTATUS_ROOM,			// 此物品在箱子或物品栏里
		emHOLDSTATUS_EQUIP,			// 此物品穿在身上
	};

	enum KE_ENHANCE_MODE			// 强化操作类型
	{
		emENHANCE_MODE_NONE,		// 无操作/取消操作
		emENHANCE_MODE_ENHANCE,		// 装备强化
		emENHANCE_MODE_PEEL,		// 玄晶剥离
		emENHANCE_MODE_COMPOSE,		// 玄晶合成
		emENHANCE_MODE_UPGRADE,		// 印鉴升级
		emENHANCE_MODE_REFINE,		// 炼化装备
		emENHANCE_MODE_STRENGTHEN,	// 装备改造
	};

	enum KE_CONSUME_TYPE			//消耗物品的种类
	{
		emCONSUME_ALL = -1,				//不分辨消耗物品的类型,一视同仁
		emCONSUME_BIND_ONLY = 1,		//只消耗绑定物品
		emCONSUME_NOBIND_ONLY = 0,		//只消耗非绑定物品
	};

	enum KE_LIMITUSE_TYPE
	{
		emKLIMITUSE_NONE = 0,			// 什么也不做
		emKLIMITUSE_MAPFORBID = 1,		// 本地图禁止使用
		emKLIMITUSE_MAPPROPER = 2,		// 专属某地图使用
		emKLIMITUSE_COUNT = 3,
	};

	// 道具使用限制的优先级，枚举值越大，优先级越大
	enum KE_LIMIT_PRIORITY
	{
		emKLIMIT_PRIORITY_forbid = 1,	//	地图禁止用
		emKLIMIT_PRIORITY_proper,		//  地图专属用
		emKLIMIT_PRIORITY_count,
	};

	typedef std::vector<KE_LIMITUSE_TYPE> vecLimitUseRule;			// 道具使用限制规则组织vector（还需要考虑更好的实现）
	typedef BOOL (*CHECKCALLBACK)(LPCSTR, DWORD, CHAR*);	// 单个道具限制规则的检查回调函数指针

	struct KHOLD_ITEM
	{
		KITEM_INDEX		sIndex;
		BOOL			bBind;
		INT				nCount;
		BOOL			bTimeOut;
	};

	struct KSP_ACTIVE
	{
		BYTE bActiveOrnament;	// 激活所有首饰
		BYTE bActiveArmor;		// 激活所有防具
		BYTE bActiveWeapon;		// 激活武器
		BYTE _;					// 为对齐32位
	};

	typedef std::vector<KMASS_Suite*>	KMassSuiteVec;
	typedef std::list<KHOLD_ITEM>		KHoldItemList;
	typedef std::list<CONST KItem*>		KItemList;

private:

	friend class KSwitchItem;
	friend class KSwitchNotify;
	friend class KJxPlayerPartner;

	typedef std::set<INT>				KINDEX_SET;

	static CHECKCALLBACK m_apfnLimitUseCheck[emKLIMITUSE_COUNT];

public:

	KCDTime				m_cCDTime;
	KItemRoom			m_acRoom[emROOM_NUM];
	KMassSuiteVec		m_vecGoldMASS;							// 黄金装备套装激活魔法属性表
	KMassSuiteVec		m_vecGreenMASS;							// 绿色装备套装激活魔法属性表
	KSP_ACTIVE			m_sActiveSpState;						// 特殊激活状态
#ifdef GAME_SERVER
	INT					m_nViewEquipTime;						// 最后一次察看他人装备的时间
	KDupedItemDeal		m_dupedItemDeal;						// 玩家身上的复制装备查找并执行删除处理
	INT					m_nPrePayMoney;

	DWORD				m_dwCheckMoney;	// TODO: 临时增加，fanzai

	std::vector<CONST TDBItemData*> m_vtrLoadItemBuffer;	// 物品加载缓冲
	BuySellInfo			m_BuyInfo; // 应该移到商店系统
#endif

public:

	explicit KPlayerItem(KCharacter& rcPlayer);
	virtual ~KPlayerItem();

	KLuaPlayerItem* GetScriptInterface() const { return m_pLuna; }

public:
	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL OnLogin();	//首次登陆时刷新神秘商店
	//virtual BOOL OnLogout();
	virtual BOOL Active();
	virtual BOOL OnClientReady();
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	BOOL Release();

	INT		MakeItem(CONST TDBItemData *pItemData);
	BOOL	LoadItemData(CONST TDBItemData *pPlayerItem, INT nCount = 1);
	VOID	SaveItemData(CONST KItem *pItem, TDBItemData *pItemData, KLISTITEM* pListItem);
	VOID	SaveItemData(CONST KItem *pItem, TDBItemData *pItemData, INT nPlace = 0, INT nX = 0, INT nY = 0);

	INT		GetEquip(INT nEquipPos) CONST		{ return m_acRoom[emROOM_EQUIP].GetCell(nEquipPos); }
	INT		GetEquipEx(INT nEquipExPos) CONST	{ return m_acRoom[emROOM_EQUIPEX].GetCell(nEquipExPos); }
	INT		GetExtBag(INT nExtBagPos) CONST		{ return m_acRoom[emROOM_EXTBAGBAR].GetCell(nExtBagPos); }
	INT		Add(INT nIdx, KE_ITEM_ROOM eRoom, KE_ADD_ITEM_WAY eWay, INT nX = 0, INT nY = 0, INT nPrice = 0, INT nCount = 0, BOOL bIsSell = 0, LPCSTR pszCurrencyType = NULL);	// 成功返回加入后m_asList的索引，失败返回0
	INT		Add(INT nIdx, CONST KITEM_POS& rsPos, KE_ADD_ITEM_WAY eWay, INT nPrice = 0, INT nCount = 0, BOOL bIsSell = 0, LPCSTR pszCurrencyType = NULL);	// 成功返回加入后m_asList的索引，失败返回0
	BOOL	RemoveOnly(INT nItemIdx, KE_LOSE_ITEM_WAY eWay);											// 仅执行删除操作，不发同步事件或协议
	BOOL	Remove(INT nItemIdx, KE_LOSE_ITEM_WAY eWay);												// nIdx指游戏世界中道具数组的编号
	VOID	RemoveAll(VOID);
	BOOL	CheckItemUsable(INT nIdx) CONST;								// 是否可以使用道具
	INT		FindSame(INT nGameIdx) CONST;									// nGameIdx指游戏世界中道具数组的编号
	BOOL	CanUseItem(CONST KItem& rcItem) CONST;								// 检查是否可以使用/装备某道具
	BOOL	CanUnequip() CONST;
	BOOL	CanEquip(CONST KItem& rcEquip, INT nEquipPos = -1) CONST;			// 检查装备是否能够穿上, -1表示自动匹配道具位置
	BOOL	SatisfyRequire(KE_ITEM_REQUIREMENT eRequire, INT nValue) CONST;
	INT		SearchID(DWORD dwID, LPINT pnListIdx = NULL) CONST;
	INT		SearchGUID(const GUID& guid, LPINT pnListIdx = NULL) CONST;
	VOID	ExchangeMoney(KE_PURSE_TYPE eSrcPurse, KE_PURSE_TYPE eDesPurse, INT nMoney);
	BOOL	SwitchItem(CONST KITEM_POS& sPickPos, CONST KITEM_POS& sDropPos);	// 直接交换两个空间的道具
	INT		GetItemByPos(CONST KITEM_POS& rsPos) CONST;
	INT		GetItemByPos(KE_ITEM_ROOM eRoom, INT nX = 0, INT nY = 0) CONST;
	INT		GetTotalMoney(VOID) CONST;											// 得到包裹和储物箱的总钱数
	INT		GetCashMoney(VOID) CONST	{ return m_aMoney[emPURSE_CASH]; }		// 得到现金数
	INT		GetSaveMoney(VOID) CONST	{ return m_aMoney[emPURSE_SAVE]; }		// 得到存款数
	INT		GetCoin() CONST;
	BOOL	CostBindMoney(INT nMoney, INT nCostWay);							//  消耗绑定银两数量
	INT		GetMachineCoin();													//  获取机关力耐久力
	BOOL	CanResizeExtBag(KE_ITEM_ROOM emRoom, INT nWidth, INT nHeight);
	BOOL	ResizeExtBag(KE_ITEM_ROOM emRoom, INT nWidth, INT nHeight);

	INT		GetBankStoreCoin() CONST;											//  存款金币数
	BOOL	DecMoney(INT nMoney);
	VOID	SetMoney(INT nMoney1, INT nMoney2);
	VOID	SetMoney(KE_PURSE_TYPE ePurse, INT nMoney);
	VOID	RestoreAttrib(VOID);
	VOID	UpdateEquipInvalid(BOOL bForce = FALSE);							// 装备失效处理。bForce = TRUE即使没有装备的失效状态发生变化也依然更新所有装备的效果，仅用于角色DB初始化

	BOOL	IsInBags(INT nItemIdx) CONST;										// 看指定道具是否在背包里
	INT		GetExtRepState(VOID) CONST		{ return m_nExtRepState; }			// 获得当前扩展储物箱激活状态
	BOOL	SetExtRepState(INT nState);											// 设置扩展储物箱激活状态，同时会激活可激活的扩展储物箱
	VOID	SetMaxCarryMoney(INT nMaxMoney)		{ m_nMaxCarryMoney = nMaxMoney; }
	INT		GetMaxCarryMoney()		{ return m_nMaxCarryMoney;}
	INT		CheckCanAddMoney(KE_PURSE_TYPE ePurse = emPURSE_CASH) { return m_nMaxCarryMoney - m_aMoney[ePurse];}
	VOID	SetIdxArrNum(INT nCount,INT nIdx)		{ m_nItemIdxArr[nCount] = nIdx; }
	INT		GetIdxArrNum(INT nCount)		{ return m_nItemIdxArr[nCount];}

	KItem*		GetItemById(DWORD dwItemId);
	KLISTITEM* GetItem(INT nListIdx = 0);
	QLinkArray* GetUseIdx(VOID)				{ return &m_UseIdx; }
	KLISTITEM* GetFirstItem(VOID);
	KLISTITEM* GetNextItem(VOID);
	KE_HOLDSTATUS CheckItemHoldStatus(CONST KITEM_INDEX &sItemIdx);

	KITEM_SUITE_COUNT_MAP* GetGoldSuiteCount(VOID)	{ return &m_mapGoldCount; }	// 获得当前所穿黄金装备套装持有情况
	KITEM_SUITE_COUNT_MAP* GetGreenSuiteCount(VOID)	{ return &m_mapGreenCount; }// 获得当前所穿绿色装备套装持有情况
	INT		GetMagicActiveSuit(INT nSuitId);									// 获取属性激活套装属性

	INT		GetCashCoin(INT nCoinItemIndex);								// 获得玩家物品箱的铜钱个数（默认的索引为1表示的是五行魂石；2表示的是月影之石）
	INT		GetValueCoin(INT nValueCoinIndex);								// 获取玩家数值货币的数量
	BOOL	SetItemStackCount(INT nItemGameIdx, INT nCount, INT eRecordWay, KE_LOSE_ITEM_WAY eWay1, KE_ADD_ITEM_WAY eWay2);	// 设置物品的叠放数（若叠放数等于0则删除该物品）
	VOID	SetPlayerIdx(INT nIdx);												// 设定玩家索引
	VOID	ClearRoom(KE_ITEM_ROOM eRoom);
	BOOL	GetItemPos(INT nGameIndex, KITEM_POS& rsItemPos);
	INT		GetMarkPrice(INT nObjIdx);
	BOOL	ValidateIndex(INT nItemIdx);										// 验证Item index的合法性
	BOOL	FindRoom(KE_ITEM_ROOM eRoom, KItem* pItem, KPos& sPos, INT &nIdx);	// 检查指定空间是否能放下指定物品
	BOOL	CheckRoom(KE_ITEM_ROOM eRoom, KItem* pItem, INT nCount);			// 检查指定空间是否能够容纳指定数量的物品
	VOID	PrepareItemRepair(DWORD dwUseItemID);								// 准备道具修理
	BOOL	GetBagFreeCell(KITEM_POS& sPos) CONST;								// 在背包（主背包和扩展背包）里寻找一个空格
	INT		CountBagFreeCell(VOID) CONST;										// 获取背包（主背包和扩展背包）的空闲格子数
	BOOL	GetRepFreeCell(KITEM_POS& sPos);									// 在指定的储物箱（主储物箱或扩展储物箱）里寻找一个空格
	INT		CountRepFreeCell(KE_ITEM_ROOM eRoom) CONST;							// 获取指定的储物箱（主储物箱或扩展储物箱）的空闲格子数
	BOOL	UseItem(INT nIdx, INT nParam = 0);										// 使用指定道具
	BOOL	ThrowAwayItem(DWORD dwId);											// 扔掉指定物品
	BOOL	CheckAutoGive(INT nGenre, INT nDetail, INT nParticular, INT nLevel, INT nAmount);
	VOID	ModifyEquipRes(KPlayer* pPlayer, KE_NPCRES_PART ePart, KItem* pItem, BOOL bPutOn);					// 将物品的相关信息同步到人物身上
	VOID	ModifyBodyRepresent(KPlayer* pPlayer, KItem* pItem, BOOL bPutOn);
	VOID	ModifyExRepresent(KPlayer* pPlayer, KItem* pItem, BOOL bPutOn);
	VOID	ModifySuitRepresent(KPlayer* pPlayer, KItem* pSuit, BOOL bPutOn);
	VOID	ModifyHeadRepresent(KPlayer* pPlayer, KItem* pSuit, BOOL bPutOn);
	VOID	ModifyShoulderRepresent(KPlayer* pPlayer, KItem* pItem, BOOL bPutOn);
	INT		GetEquipByDetail(INT nDetail);
	INT		CalcEquipScore(KItem* pEquip);										// 装备评分，返回分数1-100
	static INT CalcEquipFightScore(KItem* pEquip);									// 计算装备战斗力
	static INT CalcStoneFightScore(KItem* pStone);									// 计算宝石战斗力

	BOOL	CleanStone(KItem* pEquip);											// 宝石整理

	BOOL	StonePunch(KItem* pEquip, INT nSlot);								// 宝石打孔

	BOOL	CheckItemInBag(KItem* pItem);										// 检查物品是否在背包里
	INT		CalcAllEquipValue(VOID);											// 计算全身装备价值量
	VOID	SetTimeout(INT nItemIdx, CONST KITEM_TIMEOUT* pTimeout);
	BOOL	DelTimeout(INT nItemIdx);
	VOID	CheckTimeout(time_t tNow, INT nInterval);							// 检查超时物品
	BOOL	CheckTimeoutXuanJing(INT nIntervalDay, BOOL bNotify = TRUE);		// 检查玄晶是否超时
	VOID	SetRecycleItemDate(INT nListIdx, WORD wRecycleDate);				// 设置进入回购的日期
	CONST KITEM_TIMEOUT* GetTimeout(INT nItemIdx) CONST;

	VOID	ApplyNpcRes(INT nEquipPos);											// 应用NPC换装效果
	VOID	RemoveNpcRes(INT nEquipPos, BOOL bForce = FALSE);					// 移除NPC换装效果

	VOID	ReApplyEquipRes();

	// 检查当前背包是否能够放得下指定的东西（会将各种复杂的叠加情况也计算在内）
	BOOL	CanAddItemIntoBag(CONST KHoldItemList& rlstHoldItem) CONST;
	BOOL	CanAddItemIntoBag(CONST KItemList& rlstItem) CONST;
	BOOL	CanAddItemIntoBag(CONST KITEM_INDEX& rsIndex, BOOL bBind = FALSE, INT nCount = 1, BOOL bTimeOut = FALSE) CONST;
	BOOL	CanAddItemIntoBag(CONST KItem& rcItem) CONST;

	// 计算所有背包空间中还可放置某个物品的数量
	INT		CalcFreeItemCountInBags(CONST KITEM_INDEX& rsIndex, BOOL bBind) CONST;
	INT		CalcFreeItemCountInBags(KItem& rcItem) CONST;
	BOOL	ClearItemSellPrice(INT nItemIdx);



	INT GetItemCount(
		KE_ITEM_ROOM		eRoom,
		CONST KITEM_INDEX&	sIndex,
		INT					nBind		= KD_ITEM_NO_BIND_LIMIT,
		INT					bTimeOut	= KD_ITEM_NO_TIMEOUT_LIMIT
	);	// 获得一个空间里的一类道具的总数量（包括叠加数目）

	INT GetItemCountInBags(
		CONST KITEM_INDEX&	sIndex,
		INT					nBind = KD_ITEM_NO_BIND_LIMIT
	);	// 获得所有背包中一类道具的总数量（包括叠加数目）

	INT GetItemCount(
		KE_ITEM_ROOM	eRoom,
		INT				nGenre		= ITEM_INVALID_TYPE,
		INT				nDetail		= ITEM_INVALID_TYPE,
		INT				nParticular	= ITEM_INVALID_TYPE,
		INT				nBind		= KD_ITEM_NO_BIND_LIMIT,
		INT				bTimeOut	= KD_ITEM_NO_TIMEOUT_LIMIT
	);	// 获得一个空间的一类道具的总数量（包括叠加数目）

	INT GetItemCountInBags(
		INT nGenre = ITEM_INVALID_TYPE,
		INT nDetail = ITEM_INVALID_TYPE,
		INT nParticular = ITEM_INVALID_TYPE,
		INT	 nBind = KD_ITEM_NO_BIND_LIMIT
	);	// 获得所有背包中一类道具的总数量（包括叠加数目）

	static BOOL CheckUseLimited(LPCSTR szItemForbitType, DWORD dwMapId, CHAR* szRet = NULL);		// 限制使用规则回调总入口
	static BOOL GetLimitUseRule(LPCSTR szItemForbitType, vecLimitUseRule& rcRule);		// 获取道具限制使用的规则组合，目前所有道具受限规则统一。（到时要怎么组织这个限制规则还有待考虑）
	
	static BOOL CheckMapForbid(LPCSTR, DWORD, CHAR*);				// 地图使用限制检查回调
	static BOOL CheckMapProperUse(LPCSTR, DWORD, CHAR*);			// 地图专属道具检查回调
#ifdef GAME_SERVER
	INT		AddBankStoreCoin(INT nValue);

	BOOL    AddMachineCoin(INT nMachineCoin);
	BOOL	AddBindCoin(INT nValue, INT nWay);
	INT		AddBindMoney(INT nMoney, INT nAddWay, BOOL bNoNeedMsg = 0);											// 增加绑定银两，超上限则加到最大值，返回加成功的银两数字
	BOOL	OnCoinChanged(INT nCoinType, INT nCount);
	BOOL	c2sApplyEnhance(CONST KITEM_C2S_ENHANCE_APPLY_SYNC* pSync);

	BOOL	StackItem(INT& nStackPick, INT nStackDrop, KE_ADD_ITEM_WAY eWay = emKADDITEM_NONE,INT nItemCount = 1);							// 叠放物品
	BOOL	SplitItem(INT nItemIdx, INT nSplitCount);							// 拆分物品
	BOOL	PayCoin(INT nPayNum, INT nItemIndex = 1);							// 减少玩家物品箱的铜钱数，第二个参数默认是1，表示是五行魂石，其他的参看gamesetting.ini
	BOOL	PayValueCoin(INT nPayNum, INT nValueCoinIndex);					// 扣除指定数量的数值货币，第二个参数表示是哪种数值货币，详情参阅gemesetting.ini
	INT		Add_AutoStack(KE_ITEM_ROOM eRoom, INT nItemIdx, KE_ADD_ITEM_WAY eWay, BOOL bStack = TRUE);// 把指定物品自动叠放到指定箱子
	INT		FindAndStackInBag(INT nItemIdx, KE_ADD_ITEM_WAY eWay,INT nItemCount = 1);				// 寻找并叠加物品
	INT		FindStackRoom(INT nItemIdx, KE_ADD_ITEM_WAY eWay, INT nGenre);                  //检测是否可以叠加此物品
	BOOL	AutoEquip(INT nItemIdx);											// 自动穿上物品，要是已穿了同类物品，身上又有足够的位置，就自动脱下原有物品
	VOID	Abrade(VOID);
	VOID	SendMoneySync(VOID);												// 服务器发money同步信息给客户端
	BOOL	IsItemExist(INT nGenreType, INT nDetailType, INT nParticularType);
	BOOL	RemoveItem(INT nGenreType, INT nDetailType, INT nParticularType);	// 删除某个物品，如果nDetailType或者nParticularType为-1的话，则不检查该项是否匹配。
	BOOL	SyncItem(INT nGameItemIdx);
	VOID	SyncLockState(INT nItem);
	BOOL	IsMyItem(INT nItemIndex);											// 这个ItemIndex的物品是否是属于我的

	VOID	c2sSyncSwitchEquip(CONST KSWITCH_EQUIP_PROTOCOL* pProtocol);

	INT		ConsumeItemInBags(INT nCount, CONST KITEM_INDEX& sIndex);	// 消耗减少一定数量的指定类型的物品（个数为物品叠放数），物品必须在背包中

	INT		ConsumeItemInBags(
		INT 	nCount,
		INT 	nGenre		= ITEM_INVALID_TYPE,
		INT 	nDetailType	= ITEM_INVALID_TYPE,
		INT 	nParticular	= ITEM_INVALID_TYPE,
		INT		nLevel		= KD_ITEM_NO_LEVEL_LIMIT
	);	// 消耗、减少一定数量的指定类型的物品（个数为物品叠放数），物品必须在背包中
	INT		ConsumeItemInBagsByBindOrder(
		INT 	nCount,
		INT*	pnCosumeBindCount,
		INT 	nGenre		= ITEM_INVALID_TYPE,
		INT 	nDetailType	= ITEM_INVALID_TYPE,
		INT 	nParticular	= ITEM_INVALID_TYPE
		);	// 消耗、减少一定数量的指定类型的物品（个数为物品叠放数），会先消耗非绑定物品,再消耗绑定物品,物品必须在背包中

	VOID	c2sSplitItem(DWORD dwItemID, INT nSplitCount);					// 收到客户端消息拆分物品
	INT		AddItemOnBody(INT nItemIdx, KE_ADD_ITEM_WAY eWay, BOOL bDropIfFull = TRUE, BOOL bMsg2Player = FALSE, BOOL bStack = TRUE, INT nItemCount = 1);		// 增加物品到身上（顺序：道具栏、地上），bDropIfFull是否丢弃到地上如果身上已满，bMsg2Player是否给玩家发信息
	INT		AddItemIntoBag(INT nItemIdx, KE_ADD_ITEM_WAY eWay, BOOL bMsg2Player = FALSE, BOOL bStack = TRUE, LPCSTR pszCurrencyType = NULL, INT nItemCount = 1);			// 增加物品到背包，如果格子不够则不加且返回0，否则返回叠加后道具的索引，bMsg2Player是否给玩家发信息
	INT     AddItemIntoKinBag(INT nItemIdx);                                // 增加物品到家族背包
	VOID	SendEquipItemInfo(INT nTargetPlayer);							// 发送自己装备在身上的装备信息给别人看
	VOID	GetEquipView(KPLAYER_EQUIPVIEW* pView);							// 获取装备信息
	BOOL	PrePay(INT nMoney);												// 付订金
	BOOL	Pay(INT nMoney, INT nPayType);
	BOOL	Earn(INT nMoney, INT nEarnType);
	INT     Give(DWORD dwTarID, INT nType, LPDWORD pIDs, INT nCount);		// 给予某人某些东西，这些东西一定要是自己身上的钱或物品栏上的东西
	VOID	SetGiftTable(LPCSTR pszTableName);

	UINT	GetAddedMoney() { return m_uAddedMoney; }
	UINT	GetAddedMoneyTime() { return m_uAddedMoneyTime; }
	UINT	GetAddedTimes() { return m_uAddedTimes; }
	VOID	SetAddedMoney(INT nValue);
	VOID	SetAddedMoneyTime(INT nValue) { m_uAddedMoneyTime = (UINT)nValue; }
	VOID	SetAddedTimes(INT nValue) { m_uAddedTimes = (UINT)nValue; }

	BOOL	NeedRestore(){	return m_bStoreAcutionSearchRet;}
	VOID	SetRestoreFlag(BOOL bFlag);
	BOOL	CheckMoney(LPCTSTR pszReason);
	BOOL	SetCheckMoney(DWORD dwNowMoney);
	VOID	SetForbidUse(INT nForbid){m_nForbidUse = nForbid;}

	VOID	ArrangeBagItem();		// 整理背包

	BOOL    IsKinFull(){return m_acRoom[emROOM_KIN].CountFree() < 1;};  // 家族背包是否已满
	void    RemoveAllRentItem();

	static  INT GetDataRecordWay(KE_ADD_ITEM_WAY eWay1 = emKADDITEM_NONE, KE_LOSE_ITEM_WAY eWay2 = emKLOSEITEM_NONE);

	VOID	ApplyAppendAttrib(INT nEquipPos);
	VOID	RemoveAppendAttrib(INT nEquipPos);
#else	// #ifdef GAME_SERVER

#endif	// #ifdef GAME_SERVER #else

	static INT		GetEquipPos(INT nType);		// 取得某类型装备应该放的位置
	static KItem*	GetItemFromItemPos(CONST KITEM_POS* pPos, KPlayer *pPlayer);
	static BOOL		IsBagRoom(KE_ITEM_ROOM eRoom);

private:

	KLuaPlayerItem* m_pLuna;
	KPlayer&	m_rcPlayer;													// 所属玩家
	KLISTITEM	m_asList[KD_MAX_PLAYER_ITEM];								// 玩家拥有的所有道具（包括装备着的和箱子里放的，对应游戏世界中道具数组的索引）
	QLinkArray	m_FreeIdx;
	QLinkArray	m_UseIdx;
	INT			m_nListCurIdx;												// 用于 GetFirstItem 和 GetNextItem
	INT			m_aMoney[emPURSE_NUM];										// 角色银两
	INT			m_nJbCoin;													// 金币数量
//	INT			m_nBindCoin;												// 绑定金币数
	KSwitchItem	m_cSwitch;													// 道具位置切换
	INT			m_nExtRepState;												// 扩展储物箱激活状态
	CHAR		m_szGiftTable[256];											// 纪录当前的Gift Table
	INT			m_nMaxCarryMoney;											// 最大银两携带量
	INT			m_nItemIdxArr[KD_ROOM_BAGSUM_SIZE];                         // 存放唯一码数组

	KITEM_SUITE_COUNT_MAP m_mapGoldCount;									// 所穿黄金装备套装持有情况
	KITEM_SUITE_COUNT_MAP m_mapGreenCount;									// 所穿绿色装备套装持有情况
	KITEM_SUITE_COUNT_MAP m_mapBaseActiveSuit;								// 基础属性增加套装激活情况
	KItemTimeoutCtrl	  m_cTimeoutCtrl;									// 超时物品控制


#ifdef GAME_SERVER
	time_t		m_nLastSwithTime;											// 记录最后一次装备切换的时间
	BOOL		m_bAbrade;													// 记录磨损状态
	UINT		m_uAddedMoney;												// 总共增加的金钱
	UINT		m_uAddedMoneyTime;											// 增加金钱开始计时
	UINT		m_uAddedTimes;												// 增加金钱次数
	INT			m_nNextAlarmLevel;											// 当前的增加金钱报警等级
	INT			m_nForbidUse;												// 禁止使用物品
	BOOL		m_bStoreAcutionSearchRet;									// 是否需要保存拍卖行搜索返回结果
#else
	BOOL		m_bLockOperation;
	INT			m_nRepairUseItem;											// 用于记录道具修理时使用的消耗品的索引
	KITEM_SYNC	m_sCurItemInfo_Temp;										// 客户端正在处理的当前同步过来的物品同步信息
	std::map<INT, KITEM_SYNC>	m_mapUnknownItemInfo;					    // 未知道具的位置列表
#endif
	
private:
	BOOL	CostMoney(INT nMoney, INT nPayType);
	BOOL	AddMoney(KE_PURSE_TYPE ePurse, INT nMoney);						// 增加银两，超上限则不加，返回时候成功
	BOOL	Fit(CONST KItem& rcEquip, INT nEquipPos) CONST;					// 检查是否可以装备到指定位置
	VOID	ClearSuiteMass(VOID);											// 回收套装信息的内存
	VOID	RefreshSuiteState(VOID);										// 检查套装装备持有情况，同时初始化套装激活属性表
	VOID	CalcActiveMA(VOID);												// 重新计算并应用所有激活魔法属性
	VOID	ApplyBaseAttrib(INT nEquipPos);									// 应用装备基础属性
	VOID	RemoveBaseAttrib(INT nEquipPos);								// 移除装备基础属性
	VOID	ApplyEnhanceAttrib(INT EquipPos);							// 应用装备强化属性（新）
	VOID	RemoveEnhanceAttrib(INT nEquipPos);							// 移除装备强化属性（新）
	VOID	ModifyStoneAttribArr(INT nEquipPos, BOOL bIsEndProcess);			// 宝石属性操作接口
	VOID	ApplyStoneAttribArr(INT nEquipPos);								// 应用宝石属性
	VOID	RemoveStoneAttribArr(INT nEquipPos);							// 移除宝石属性
	VOID	ApplyActiveMA(CONST KMASocketSet* pMASS);						// 应用激活魔法属性
	VOID	RemoveActiveMA(CONST KMASocketSet* pMASS);						// 移除激活魔法属性
	VOID	ApplyMagicActiveSuit(INT nSuitId, INT nActiveNum);				// 属性激活套装属性
	VOID	RemoveMagicActiveSuit(INT nSuitId, INT nActiveNum);				// 移除属性激活套装属性
	BOOL	EquipResForNPC(INT nEquipPos);
	INT		FindFree(VOID) CONST;
	BOOL	ResizeRoom(KE_ITEM_ROOM eRoom, INT nWidth, INT nHeight);		// 改变一个空间的大小，自动调整内部道具
	BOOL	TestInvalid(CONST KItem& rcItem) CONST;							// 检测道具是否失效
	VOID	CheckHoldStatckItemInBags(const KHOLD_ITEM& rsHold, INT nStackMax, KHoldItemList& rlstVirtual, INT& rnFreeCell) CONST;	// 辅助CanAddItemIntoBag

	// 可将下面的函数抽象成一个回调接口
	BOOL	AddBase(INT nItem, INT nListIdx, CONST KITEM_POS& sPos);
	BOOL	AddEquip(INT nItem, INT nListIdx, INT nPos = -1);				// 穿上装备 nPos = -1自动找位置
	BOOL	AddExtBag(INT nItem, INT nListIdx, INT nPos = -1);				// 穿上扩充背包, nPos = -1自动找空闲位置
	
	BOOL	AddEquipEx(INT nItem, INT nListIdx, INT nPos);

	BOOL	RemoveBase(INT nListIdx, INT nX = -1, INT nY = -1);
	BOOL	RemoveEquip(INT nListIdx, INT nPos = -1);						// 卸下装备 nPos = -1自动找位置
	BOOL	RemoveExtBag(INT nListIdx, INT nPos = -1);						// 卸下扩充背包, nPos = -1自动自动识别

#ifdef GAME_SERVER
	//VOID	DoMysteryShopRefresh(VOID);									// 神秘商店刷新处理
	BOOL	CompareItems(CONST KINDEX_SET& lhs, CONST KINDEX_SET& rhs);		// 比较物品列表，相同返TRUE，否则返回FALSE
	BOOL	CheckRoomItems(KE_ITEM_ROOM eRoom);								// 检查物品合法性，如果合法则返回TRUE
	VOID	SyncItem(CONST KLISTITEM& rsListItem) CONST;					// 同步道具
	INT		ConsumeRoomItem(KE_ITEM_ROOM eRoom, INT nCount, CONST KITEM_INDEX& sIndex, KE_CONSUME_TYPE eConsumeType = emCONSUME_ALL);	// 移除指定空间中指定数目的道具，返回差额

	// 根据p和l得到宝石的某条魔法属性
	static BOOL GetStoneAttrib(KMagicAttrib* pAttrib, INT nParticular, INT nLevel, INT i);

	VOID GetBagUsedItem(std::vector<INT>& vecUsedItem);

public:
	BOOL SaveEquipDelayData(KDelayRoleData* pDelayData);
	
#endif	// #ifdef GAME_SERVER

};

extern CONST INT g_anEquipPos[equip_detailnum + 1];
extern CONST KE_ITEM_ROOM g_aeExtBagPos2Room[];
extern CONST KE_ITEM_ROOM g_aeBagRoom[];
// -------------------------------------------------------------------------

#endif /* __KPLAYERITEM_H__ */
