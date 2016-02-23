
#ifndef	KItemH
#define	KItemH

#include "klibofbpt.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "config/knsbaserequire.h"
#include "kmasocketset.h"
#include "../award/kawardsetting.h"

#include "kitemdatadef.h"

#ifndef GAME_SERVER
#include "represent/krepresentunit.h"
#endif //_SERVER

class KGuidString
{
public:
	KGuidString(const GUID* pGuid = NULL)
	{
		if (pGuid)
			MakeString(pGuid);
		else
			m_szGuid[0] = '\0';
	}
	VOID Clear()
	{
		m_szGuid[0] = '\0';
	}
	LPCSTR GetString() const
	{
		return m_szGuid;
	}
	LPCSTR MakeString(const GUID* pGuid)
	{
		INT nIndex = 0;
		unsigned char* pData = (unsigned char*)pGuid;
		m_szGuid[nIndex++] = '{';
		for (UINT i = 0; i < sizeof(GUID); i++)
		{
			unsigned char ch[2];
			ch[0] = (pData[i] >> 4) & 0xf;
			ch[1] = pData[i] & 0xf;
			for (INT j = 0; j < 2; j++)
			{
				UCHAR c = ch[j];
				if (c < 10)
					m_szGuid[nIndex++] = '0' + c;
				else
					m_szGuid[nIndex++] = 'A' + c - 10;
			}
		}
		m_szGuid[nIndex++] = '}';
		m_szGuid[nIndex] = '\0';
		return m_szGuid;
	}
private:
	char m_szGuid[sizeof(GUID) * 2 + 3];
};

class KGuid
{
public:
	KGuid(CONST GUID* pGuid = NULL)
	{
		if (pGuid)
		{
			SetGuid(pGuid);
		}
		else
		{
			memset(&m_sGuid, 0, sizeof(m_sGuid));
		}
	}
	inline VOID SetGuid(CONST GUID *pGuid)
	{
		m_sGuid = *pGuid;
		m_cString.MakeString(pGuid);
	}
	inline CONST GUID& GetGuid() CONST
	{
		return m_sGuid;
	}
	inline LPCSTR GetString() CONST
	{
		return m_cString.GetString();
	}
	inline VOID Clear()
	{
		memset(&m_sGuid, 0, sizeof(m_sGuid));
		m_cString.Clear();
	}
private:
	GUID			m_sGuid;
	KGuidString		m_cString;
};

//---------------------------------------------------------------------------

class KRandom;
class KPlayer;
class QIniFile;

#ifdef GAME_SERVER
#define	MAX_ITEM					g_piNsBaseRequire->GetEnvConfig()->nMaxItem
#else
#define	MAX_ITEM					4000
#endif
#define	INVALID_PRICE				1000000000					// 10亿 MAX = 99999999
#define MAX_ENH_LV(nQuality)		((nQuality) * 12)							// 最高强化等级
#define KD_ITEM_MAX_QUALITY			7

#define	KD_ITEM_MAX_SERIES_ACTIVE	2							// 装备五行相生最多可以激活魔法属性数
#define KD_ITEM_GENINFO_COUNT		(sizeof(KMAGIC_INDEX) / sizeof(INT) * emITEM_COUNT_RANDOM)
#define KD_ITEM_MAX_DURABILITY		200						// 道具的耐久值上限


#define KD_ITEM_MAX_CUSTOM_LENGTH	32							// 道具自定义字符串长度限制
#define	MAX_AMOUNT					2000000000					// 最大金额20亿

#define	ENHANCE_MASK			0x0000003f						// 强化的掩码，强化的值在m_nEnhTimes的后6位（存在数据库就8位）
#define	STRENGTHEN_MASK			0x000000c0						// 改造的掩码，改造的值在m_nEnhTimes的前2位
#define	STRENGTHEN_OFFSET		6								// 改造的位移量
//---------------------------------------------------------------------------
// KItem

class KLuaItem;

class KItem
{
	friend class	KLuaItem;
	friend class	KItemGenerator;
	friend class	KPlayerItem;

public:

	enum KEITEM_GENINFO_TASK // 物品geninfo用途
	{
		emKITEM_GENINFO_OWNER_TONGID = 1, // 与其绑定的帮会id
	};

	struct KBASEDATA
	{
		LPCSTR				pszName;							// 名称
		LPCSTR				pszClass;							// CLASS
		KITEM_INDEX			sIndex;								// 道具索引
		INT					nPrice;								// 价格
		BYTE				byCurrencyType;						// 货币类型
		INT					nQuality;							// 品质（颜色）
		INT					nBindType;							// 道具绑定形式 (KE_ITEM_BINDTYPE枚举)
		BOOL				bValuable;							// 是否贵重道具 (防刷及写LOG)
		INT					aryExtParam[emITEM_COUNT_EXTPARAM];	// 扩展参数
#ifndef GAME_SERVER	
		LPCSTR				pszIconImage;						// 道具图标动画文件名
		LPCSTR				pszViewImage;						// 道具预览动画文件名
		LPCSTR				pszIntro;							// 说明文字
		LPCSTR				pszHelp;							// 帮助文字
#endif
	};

	struct KREQUIRE_ATTR
	{
		KE_ITEM_REQUIREMENT	eRequire;							// 需求类型
		INT					nValue;								// 参数值
	};

	struct KSTONE_INFO
	{
		BYTE				byStoneId;							// 宝石的particular
		BYTE				byStoneLevel;
	};

	typedef INT	KREFINE_LOCK;		// 附加属性锁，用0和1表示，0代表没锁，1代表锁住

	struct KGEN_PARAM
	{
		UINT				uRandomSeed;						// 随机种子
		UINT				uRandomSeedEx;						// 专门用来随机装备强化宝石属性的随机种子
	};

	enum KE_CUSTOM_TYPE											// 自定义字符串类型
	{
		emCUSTOM_TYPE_NONE,										// 未使用
		emCUSTOM_TYPE_MAKER,									// 制造者名字
		emCUSTOM_TYPE_EVENT,									// 活动定制
	};

public:

	KItem();
	~KItem();

	KLuaItem		*m_pLuna;

	KBASEDATA		m_sBaseData;								// 基本配置
	KGEN_PARAM		m_GeneratorParam;							// 生成参数
	KMagicAttrib	m_aryBaseAttrib[emITEM_COUNT_BASE];			// 基础属性
	KMagicAttrib	m_sEnhAttrib;								// 强化属性
	KMagicAttrib	m_aryAppendAttrib[emITEM_COUNT_APPEND];		// 附加属性
	KMagicAttrib	m_arySaveAppendAttrib[emITEM_COUNT_APPEND];	// 尚未保存的附加属性
	BYTE			m_aryAppendQuality[emITEM_COUNT_APPEND];
	BYTE			m_arySaveAppendQuality[emITEM_COUNT_APPEND];	// 附加属性品质
	KREQUIRE_ATTR	m_aryReqAttrib[emITEM_COUNT_REQ];			// 需求属性
	KSTONE_INFO		m_aryStoneInfo[emSTONE_MAX_SLOT];			// 宝石信息
	INT				m_nUseFailedTimes;							// 使用道具连续失败次数 added by dengyong 20100128
	DWORD			m_dwLastFailedFrame;						// 上次使用失败的帧数 added by dengyong 20100128	
	INT				m_nFightScore;								// 装备战斗力
	INT				m_nGenerateMoneyType;							// 装备操作时选择的金钱类型

private:

	DWORD			m_dwID;										// 独立的ID，用于客户端与服务器端的交流
	DWORD			m_dwCurCount;								// 当前物品的叠放数
	DWORD			m_dwMaxCount;								// 物品的最大叠放数
	INT				m_nFightPower;								// 原始战斗力
	INT				m_nEnhTimes;									// 已强化次数（0-15）
	INT				m_nResMale;									// 男性装备的资源ID
	INT				m_nResFemale;								// 女性装备的资源ID
	INT				m_nPandent1;								// 外观1
	INT				m_nPandent2;								// 外观2
	INT				m_arrExPandents[emITEM_COUNT_EX_PENDANT];
	INT				m_arrSuitExPandents[emITEM_COUNT_SUIT_EX_PENDANT];
	INT				m_nResEffect;								// 特效资源ID
	//INT				m_nChangeColorScheme;						// 换色方案
	//INT				m_nEffectChangeColorScheme;					// 特效换色方案
	INT				m_nCDType;									// 冻结时间类型
	BOOL			m_bShortcut;								// 是否可放快捷栏
	BOOL			m_bBind;									// 是否已绑定
	BOOL            m_bRentFromKin;                             // 是否从家族仓库借来
	DWORD           m_dwKinRepertoryId;                         // added by liyun 记录这个装备被借出到家族仓库的家族Id
	BOOL			m_bLock;									// 是否已锁定
	BOOL			m_bTemporary;								// 是否临时装备 (临时装备不存盘)
	BOOL			m_bInvalid;									// 装备失效标志，该标志仅对穿在身上的装备有效，其余情况均不用考虑失效
	BOOL			m_bCanUseInBZone;							// 跨服能否使用标志
	CHAR			m_szFullName[256];							// 道具的全名（包括后缀名）
	CHAR			m_szCustom[KD_ITEM_MAX_CUSTOM_LENGTH + 1];	// 自定义字符串类型
	KE_CUSTOM_TYPE	m_eCustomType;								// 自定义字符串类型
	INT				m_nOwner;									// 所属角色的索引
	KITEM_TIMEOUT	m_sTimeout;									// 超时时间
	INT				m_nReqLevel;								// 所有道具的使用等级需求
	INT				m_nEquipLevel;								// 跟装备相关的道具对装备的等级要求
	INT				m_nCurValue;								// 实际价值量（单件）
	INT				m_nStarLevel;								// 价值量星级
	INT				m_nStoneSlot;									// 宝石孔数量

	INT				m_nLockInterval;							// 解锁间隔时间，为0表示不能解锁，否则表示间隔时间
	DWORD			m_dwNameColor;								// 名字颜色
	KE_EQUIP_WEAPON_CATEGORY m_eEquipCategory;					// 装备类别
	DWORD			m_dwPrice;									// 摆摊标价
	DWORD			m_dwCount;									// 摆摊数量
	INT				m_nAssignMode;								// 分配方式
	INT				m_nBagPos;									// 背包放置位置限制

#ifdef GAME_SERVER
	KGuid			m_cGuid;									// GUID类
	INT64			m_nItemGUID;								// 物品GUID(KGuid将废弃)
	INT				m_nIdentity;								// 物品流水号（不存盘），每个物品都有不同的流水号，每次产生一个新物品该流水号递增。
	DWORD			m_dwGenTime;								// 生成时间
	BYTE			m_byBitFlag;								// 位标志，各位含义见KE_ITEMDATA_FLAG
	DWORD			m_dwBuyPrice;								// 购买时价格（奇珍阁）
#else
	KRUImage		m_Image;
	CHAR			m_szTransparencyIcon[MAX_PATH];				// 道具的透明图层
	CHAR			m_szMaskLayerIcon[MAX_PATH];				// 道具的掩膜图层
#endif

public:

	BOOL	CopyItemData(CONST KItem* pItem);				// 为了不再使用operator =
	BOOL	Regenerate(INT, INT, INT, INT, INT, CONST KSTONE_INFO*, CONST KMagicAttrib*, CONST KREFINE_LOCK*, UINT, UINT);

	DWORD	GetID(VOID) CONST						{ return m_dwID; }
	KITEM_INDEX GetIndex(VOID) CONST				{ return m_sBaseData.sIndex; }
	INT		GetGenre(VOID) CONST					{ return m_sBaseData.sIndex.nGenre; }
	INT		GetDetailType(VOID) CONST				{ return m_sBaseData.sIndex.nDetailType; }
	INT		GetParticular(VOID) CONST				{ return m_sBaseData.sIndex.nParticular; }
	INT		GetLevel(VOID) CONST					{ return m_sBaseData.sIndex.nLevel; }

	LPCSTR	GetName(VOID) CONST						{ return m_szFullName; }								// 获得名称(包含后缀)
	VOID	SetName(LPCSTR szName)					{ strcpy(m_szFullName, szName); }
	LPCSTR	GetOrgName(VOID) CONST					{ return m_sBaseData.pszName; }							// 获得原名(不含后缀)
	LPCSTR	GetSuffix(VOID) CONST;																			// 获得后缀名，不存在时返回NULL
	LPCSTR	GetClass(VOID) CONST					{ return m_sBaseData.pszClass; }						// 获得CLASS名
	INT		GetBindType(VOID) CONST					{ return m_sBaseData.nBindType; }						// 获得绑定类型
	INT		GetQuality(VOID) CONST					{ return m_sBaseData.nQuality; }						// 获得道具品质
	INT		GetReqLevel(VOID) CONST					{ return m_nReqLevel; }									// 获得非装备的使用等级需求
	BOOL	IsValuable(VOID) CONST					{ return m_sBaseData.bValuable; }						// 是否贵重物品
	INT		GetExtParam(INT nIndex) CONST;																	// 获得扩展参数
	INT		GetPandent1Id(VOID)CONST				{ return m_nPandent1; }
	INT		GetCDType(VOID) CONST					{ return m_nCDType; }									// 获得CD类型
	DWORD	GetCDTime(VOID) CONST;																			// 获得CD时间
	DWORD	GetCount(VOID) CONST					{ return m_dwCurCount; }								// 获取物品的叠放数
	DWORD	GetMaxCount(VOID) CONST					{ return m_dwMaxCount; }								// 获取物品可叠放的最大值
	INT		GetEnhanceStrengthen(VOID) CONST				{ return m_nEnhTimes & ENHANCE_MASK; }				// 获得改造和已强化次数的属性
	INT		GetStoneSlot(VOID) CONST				{ return m_nStoneSlot;	}									// 获得宝石孔数量
	INT		GetEnhanceTimes(VOID) CONST				{ return m_nEnhTimes; }									// 获得已强化次数
	INT		GetGenerateMoneyType(VOID) CONST		{ return m_nGenerateMoneyType; }
	// 获得改造信息	
	INT		GetStrengthen(VOID) CONST				{ return (m_nEnhTimes & STRENGTHEN_MASK) >> STRENGTHEN_OFFSET; }	
	// 计算改造等级
	INT		CalStrengthen(CONST INT nEnhanceStrengthen);
	// 计算强化等级
	INT		CalEnhTimes(CONST INT nEnhanceStrengthen);
	// 计算改造强化存储值
	static INT CalEnhanceStrengthen( CONST INT nStrengthen, CONST INT nEnhTimes);
	// 地图禁止类型
	LPCSTR GetForbidType();

	UINT	GetRandSeed(VOID) CONST					{ return m_GeneratorParam.uRandomSeed;}					// 获得随机种子
	UINT	GetRandSeedEx(VOID) CONST					{ return m_GeneratorParam.uRandomSeedEx;}
	
	LPCSTR	GetCustomString(VOID) CONST				{ return m_szCustom; }									// 获得自定义字符串
	KE_CUSTOM_TYPE GetCustomType(VOID) CONST		{ return m_eCustomType; }								// 获得自定义字符串类型
	INT		GetPrice(VOID) CONST;																			// 获得价格
	INT		GetRecyclePrice(VOID) CONST;																	// 获得回购价格
	INT		GetSalePrice() CONST					{ return GetUnitSalePrice() * GetCount(); }				// 获得出售给NPC的价格
	INT		GetUnitPrice() CONST;																			// 获取单位价格
	INT		GetUnitSalePrice() CONST				{ return GetUnitPrice() / 2; }							// 获得出售给NPC的单位价格
	INT		GetGameIndex(VOID) CONST;																		// 获得游戏世界道具索引
	INT		GetSingleValue(VOID) CONST				{ return m_nCurValue;}									// 获得单个道具的价值量
	INT		GetValue(VOID) CONST					{ return m_nCurValue * m_dwCurCount; }					// 获得总价值量

	INT		GetBelong(VOID) CONST					{ return m_nOwner; }									// 获得所属角色索引
	BOOL	CanBeShortcut(VOID) CONST				{ return m_bShortcut; }									// 该道具是否可放进快捷栏
	BOOL	IsBind(VOID) CONST						{ return m_bBind; }										// 是否已绑定
	BOOL    IsRentFromKin(VOID) CONST               { return m_bRentFromKin; }                              // 是否从家族仓库借来
	INT		HasStone(INT nSlot);																			// 判断装备的某个孔是否有宝石
	BOOL	HasStoneLocked(INT nSlot);																		// 判断宝石孔是否上锁
	INT		GetLockIntervale(VOID)					{ return m_nLockInterval; }
	BOOL	IsLock(VOID) CONST						{ return m_bLock; }										// 是否已锁定
	BOOL	IsTemporary(VOID) CONST					{ return m_bTemporary; }								// 是否临时装备
	BOOL	IsDestoriable(VOID) CONST				{ return !m_bBind && !m_bLock; }						// 是否可销毁物品
	BOOL	IsForbitThrow(VOID) CONST				{ return /*m_sBaseData.bValuable || m_bBind || */m_bLock; }	// 是否可丢弃
	BOOL	IsForbitSell(VOID) CONST				{ return m_sBaseData.nBindType == emITEM_BIND_OWN || m_sBaseData.nBindType == emITEM_BIND_NONE_OWN; }	// 是否可卖店
	BOOL	IsStackable(VOID) CONST;																		// 是否可叠放物品
	BOOL	IsInvalid(VOID) CONST;																			// 装备是否失效
	BOOL	CanUseInBZone(VOID) CONST				{ return m_bCanUseInBZone; }								// 装备是否在跨服战场中失效
	BOOL	IsUseRandom(VOID) CONST					{ return IsUseRandom(GetIndex()); }						// 该道具是否使用随机属性（关系到GenInfo存档意义是否魔法属性索引）
	INT		GetStarLevel(VOID) CONST				{ return m_nStarLevel; }								// 获得道具星级
	DWORD	GetNameColor(VOID) CONST				{ return m_dwNameColor; }								// 获得道具颜色
	INT		GetAssignMode(VOID) CONST				{ return m_nAssignMode; }								// 获取物品的分配方式
	INT		GetBagPosLimit(VOID) CONST				{ return m_nBagPos;		}		

	INT GetBaseAttrValue(CONST QNumericalAward* pAward);
	
	KE_MONEY_TYPE GetSellMoneyType(VOID) CONST;

	KE_EQUIP_WEAPON_CATEGORY GetEquipCategory(VOID) CONST;

	CONST KMagicAttrib*		GetBaseAttrib(INT i) CONST;
	CONST KMagicAttrib*		GetEnhAttribArr() CONST;
	CONST KREQUIRE_ATTR*	GetReqAttrib(INT i) CONST;

	VOID	SetIndex(CONST KITEM_INDEX& rsIndex)	{ m_sBaseData.sIndex = rsIndex; }						// 设定道具索引
	VOID	SetID(DWORD dwID)						{ m_dwID = dwID; }
	VOID	SetTemporary(BOOL bTemp = TRUE)			{ m_bTemporary = bTemp; }
	BOOL	SetLevel(INT nLevel);

	VOID	SetInvalid(BOOL bInvalid = TRUE)		{ m_bInvalid = bInvalid; }								// 设置装备失效状态，仅仅设值，不做对其影响的处理
	VOID	SetBZoneInvalid(BOOL bCanUse = FALSE)	{ m_bCanUseInBZone = bCanUse; }
	BOOL	SetCount(DWORD dwCount);																		// 设置物品的叠放数
	VOID	SetEnhanceTimes(INT nEnhTimes);
	VOID	SetFightScore(INT nScore)				{ m_nFightScore = nScore; }
	INT		GetFightScore()							{ return m_nFightScore; }
	INT		GetFightPower()							{ return m_nFightPower; }
	VOID	SetFightPower(INT nFightPower)			{ m_nFightPower = nFightPower; }
	VOID	SetRandSeed(UINT uRandSeed)				{ m_GeneratorParam.uRandomSeed = uRandSeed; }			// 设定随机种子
	VOID	SetRandSeedEx(UINT uRandSeedEx)				{ m_GeneratorParam.uRandomSeedEx = uRandSeedEx; }	
	VOID	SetBelong(INT nBelong)					{ m_nOwner = nBelong; }									// 设定所有者
	BOOL	SetCustom(KE_CUSTOM_TYPE eType, LPCSTR pszCustom, INT nBufSize = 0);							// 设定自定义字符串
	VOID	SetAssignMode(INT nMode)				{ m_nAssignMode = nMode;}								// 设置物品的分配方式
	DWORD   GetKinReptoryID() CONST                 { return m_dwKinRepertoryId; }                          // added by liyun
	VOID    SetKinReptoryID(DWORD dwKinReptoryId)   { m_dwKinRepertoryId = dwKinReptoryId; }                // 装备被借出到家族仓库后记录家族ID
	BOOL	SetExtParam(INT nIndex, INT nValue);															// 设置道具额外数据
	VOID	SetGenerateMoneyType(INT nMoneyType)	{ m_nGenerateMoneyType = nMoneyType; }

	VOID	Remove();
	VOID	Clear();
	BOOL	Bind(BOOL bForce = FALSE);																		// 绑定道具
	VOID    SetRentFromKin(BOOL bRent = FALSE)      { m_bRentFromKin = bRent; }                             // 从家族仓库借来
	BOOL	SetBaseAttrib(CONST KMagicAttrib* pAttrib, INT nIndex = -1);
	BOOL	SetRequireAttrib(CONST KREQUIRE_ATTR* pRequire, INT nIndex = -1);
	BOOL	SubCount(INT nCount);																			// 减少物品的叠放数
	BOOL	AddCount(INT nCount);																			// 增加物品的叠放数
	BOOL	CanStackWith(CONST KItem& rcItem) CONST;														// 判断两物品是否是可以叠放到一起的相同的物品
	BOOL	GetExtBagSize(INT& rnWidth, INT& rnHeight) CONST;												// 获得扩展背包的大小

	INT		GetResourceID(BOOL bSex) CONST				{ return bSex ? m_nResFemale : m_nResMale; }		// 获得资源ID
	INT		GetEffectResourceID() CONST					{ return m_nResEffect; }							// 获得装备特效资源ID

	VOID	SetTimeout(CONST KITEM_TIMEOUT* pTimeout)	{ ASSERT(pTimeout); m_sTimeout = *pTimeout; }		// 设置超时时间
	CONST KITEM_TIMEOUT* GetTimeout() CONST				{ return &m_sTimeout; }								// 获取超时时间

	WORD	GetRandSeed2() { return 0; }
	BOOL	IsDupedItem();

#ifdef GAME_SERVER
	VOID	SetBuyPrice(DWORD dwPrice)					{ m_dwBuyPrice = dwPrice; }
	DWORD	GetBuyPrice() CONST							{ return m_dwBuyPrice; }
	BOOL	ForBitForDupedItem();
	// 物品GUID===============>
	VOID	SetGuid(CONST GUID* pGuid);
	CONST GUID& GetGuid() CONST							{ return m_cGuid.GetGuid(); }
	LPCSTR	GetGuidString() CONST						{ return m_cGuid.GetString(); }
	VOID	SetItemGUID(CONST INT64 nGUID);
	CONST INT64 GetItemGUID() CONST						{ return m_nItemGUID; }
	// 物品GUID<===============
	
	INT		MakeSyncInfo(KVIEWITEMINFO& rsViewInfoEx);	// 生成基础同步信息,返回实际使用大小(扣除Custom无用字节)
	VOID	SetIdentity(INT nIdentity)					{ m_nIdentity = nIdentity; }
	INT		GetIdentity() CONST							{ return m_nIdentity; }
	VOID	SetGenTime(DWORD dwTime)					{ m_dwGenTime = dwTime; }
	DWORD	GetGenTime() CONST							{ return m_dwGenTime; }

	INT		GetEnhanceCost();
	BOOL	GetAppendAttribNum();
	VOID	ClearSaveAppendAttr(BOOL bSaveOri);
	BOOL	RerandAppendAttrib(CONST KREFINE_LOCK* pLock);
#else
	LPCSTR	GetIntro(VOID) CONST						{ return m_sBaseData.pszIntro; }
	LPCSTR	GetHelp(VOID) CONST							{ return m_sBaseData.pszHelp; }
	LPCSTR	GetIconImage(VOID) CONST					{ return m_sBaseData.pszIconImage; }
	LPCSTR	GetViewImage(VOID) CONST					{ return m_sBaseData.pszViewImage; }
	LPCSTR	GetTransparencyIcon(VOID) CONST				{ return m_szTransparencyIcon; }
	LPCSTR  GetMaskLayerIcon(VOID) CONST				{ return m_szMaskLayerIcon; }

	VOID	GetPrimalData(IInlineCtrlData* pItemData);
	INT		GennerateItemByData(IInlineCtrlData* pItemData, KItem* pItem);
	VOID	GetLinkTipByData(IInlineCtrlData* pItemData, KGameObjDesc *pDescStr);
	VOID	GetLinkNameByData(IInlineCtrlData* pItemData, LPSTR pszLinkName, DWORD& dwLinkNameColor);
	VOID	GetLinkImageByData(IInlineCtrlData* pItemData, LPSTR pszImage);

	BOOL	GetMaskActionSpr(INT nAction, LPSTR pszActionSpr);					// 获得面具预览效果图标
	VOID	Paint(INT nX, INT nY, KITEM_DRAW_PARAM* psParam);
	VOID	SetBind(BOOL bBind)							{ m_bBind = bBind ? 1 : 0; }
	static BOOL GetMaskActionSpr(INT nNpcSetting, INT nAction, LPSTR pszActionSpr);
#endif

	static BOOL IsBindByBindType(KE_ITEM_BINDTYPE eBindType);
	static BOOL IsUseRandom(CONST KITEM_INDEX &sIndex);
	static BOOL IsUseRandom(CONST KItem &cItem) { return IsUseRandom(cItem.GetIndex()); };
	static BOOL GetExtBagSize(INT nDetail, INT& rnWidth, INT& rnHeight);		// 获得扩展背包的大小

private:

	BOOL	Init_BaseData(CONST KBASICPROP_BASEDATA* pBPT);						// 初始化基本配置
	BOOL	Init_BaseAttrib(CONST KBPT_EQUIP_BASEATTR* pBase, KRandom& rcRandom); // 初始化基础属性
	BOOL	InitEnhAttrib();
	BOOL	InitAppendAttrib();
	BOOL	Init_ReqAttrib(CONST KBPT_EQUIP_REQATTR* pReq);						  // 初始化需求属性
	BOOL	Init_WeaponDmg(CONST KBASICPROP_WEAPON* pBPT, KRandom& rcRandom);		// 初始化武器攻击力
	BOOL	Init_Equip(CONST KBASICPROP_EQUIPMENT* pBPT, KRandom& rcRandom, KRandomEx& rcRandomEx, BOOL bNew);	// 初始化装备属性
	BOOL	Init_SuitEquip(CONST KBASICPROP_SUIT* pBPT, KRandom& rcRandom, BOOL bNew);
	BOOL	Init_Other(CONST KBASICPROP_OTHER* pBPT);							// 初始化非装备属性
	BOOL	Init_Medicine(CONST KBASICPROP_MEDICINE* pBPT);						// 初始化药品属性
	BOOL	Init_Stone(CONST KBASICPROP_STONE* pBPT);							// 初始化宝石属性
	BOOL	Init_Script(CONST KBASICPROP_SCRIPT* pBPT);							// 初始化脚本物品属性
	BOOL	Init_Skill(CONST KBASICPROP_SKILL* pBPT);							// 初始化技能物品属性
	BOOL	Init_Quest(CONST KBASICPROP_QUEST* pBPT);							// 初始化任务物品属性
	BOOL	Init_ExtBag(CONST KBASICPROP_EXTBAG* pBPT);							// 初始化扩展背包属性
	BOOL	Init_Stuff(CONST KBASICPROP_STUFF* pBPT);							// 初始化生活技能材料属性
	BOOL	Init_Plan(CONST KBASICPROP_PLAN* pBPT);								// 初始化生活技能配方属性
	BOOL	Init_FullName(VOID);												// 初始化道具全名
	BOOL	Init_ResColor(CONST KBASICPROP_EQUIPMENT* pBPT);					// 初始化装备资源和配色
	BOOL	Init_SuitResColor(CONST KBASICPROP_SUIT* pBPT);
	BOOL	EndGenerate(VOID);													// 结束道具生成后所需要做的工作

#ifndef GAME_SERVER
	VOID	InitIconImage(LPCSTR lpszIconImage);								// 初始化图标图像数据
#endif

};

extern KItem* Item;

#endif
