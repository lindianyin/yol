
#ifndef __KITEMMGR_H__
#define __KITEMMGR_H__

#include "itemenv_i.h"
#include "kitem.h"
#include "kdupeditemdeal.h"
#include "kitemsetting.h"
#include "Misc/qindexlist.h"
#include "kitemgenerator.h"
#include "kplayeritem.h"
#include <map>
#include "onlinegameworld/kgcmodulebase.h"


// 物品消耗方式
enum KE_ITEM_CONSUMEMODE
{
	emITEM_CONSUMEMODE_NORMAL = 0,

	emITEM_CONSUMEMODE_REALCONSUME_START,								// 真实消耗

	emITEM_CONSUMEMODE_SELL = emITEM_CONSUMEMODE_REALCONSUME_START,		// 卖商店
	emITEM_CONSUMEMODE_ENCHASER,										// 装备升级
	emITEM_CONSUMEMODE_USINGTIMESEND,									// 使用次数用完
	emITEM_CONSUMEMODE_USINGTIMEOUT,									// 使用时间到
	emITEM_CONSUMEMODE_EXPIREDTIMEOUT,									// 保值期到
	emITEM_CONSUMEMODE_EAT_QUICK,										// 通过右键或快捷键使用(吃掉)
	emITEM_CONSUMEMODE_EAT,												// 通过脚本使用(吃掉)
	emITEM_CONSUMEMODE_CONSUME,											// 通过脚本消耗
	emITEM_CONSUMEMODE_ERRORLOST_STACK,									// 因物品叠加异常删除
	emITEM_CONSUMEMODE_PICKUP,											// 捡起即消失
	emITEM_CONSUMEMODE_COMMONSCRIPT,									// 通过通用脚本删除
	emITEM_CONSUMEMODE_DUPEDITEM,										// 因复制品而被删除
	emITEM_CONSUMEMODE_ERRORLOST_PK,									// 因PK原因丢失，异常删除
	emITEM_CONSUMEMODE_ERRORLOST_THROWALLITEM,							// 因丢物品到地上，异常删除
	emITEM_CONSUMEMODE_ERRORLOST_ADDONBODY,								// 因向身上添加物品失败，异常删除
	emITEM_CONSUMEMODE_REALCONSUME_END = emITEM_CONSUMEMODE_ERRORLOST_ADDONBODY,	// 正常消耗

	emITEM_CONSUMEMODE_STACK,											// 因物品叠加
};

enum KE_ITEM_DATARECORD_WAY
{
	emITEM_DATARECORD_NORMAL,			// 默认途径，不记录
	emITEM_DATARECORD_SYSTEMADD,		// 系统产出(系统掉落、奇珍阁购买, 生活技能制作)
	emITEM_DATARECORD_ROLLOUT,			// 价值量转出添加(剥离装备，剥离同伴，玄晶拆解)
	emITEM_DATARECORD_ROLLIN,			// 价值量转入删除(强化装备、喂同伴，玄晶合成)
	emITEM_DATARECORD_REMOVE,			// 永久性删除(吃声望令牌、成品，物品在地上系统回收，在回购栏被挤掉，道具兑换)
};

// -------------------------------------------------------------------------
// KItemMgr

class KItemMgr : public KModuleBase
{
// 类型定义

private:

	struct KPARAM
	{
		INT		nGenre;
		INT		nDetail;
		INT		nParticular;
		INT		nLevel;
		INT		nEnhTimes;
		UINT	uRandSeed;
		UINT	uRandSeedEx;
	};

	struct KDATARECORD
	{
		CHAR szKey[MAX_NAME_LEN];	// 格式为G_D_P
		BOOL bConsume;
		BOOL bAdd;
	};
// 接口方法实现

public:
	LPCSTR	GetName() CONST	{ return "Item"; }

	virtual BOOL OnInit();
	virtual BOOL OnUnInit();

	virtual BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	virtual BOOL InitProtocol();

	virtual BOOL Breath() { return TRUE; }

// 成员方法定义

public:

	KItemMgr();

	INT		Add(
		CONST KITEM_INDEX&		sIdx,
		INT						nEnhTimes,
		KE_ITEM_DATARECORD_WAY	eWay,
		CONST KItem::KSTONE_INFO*	pStoneInfo	= NULL,
		CONST KMagicAttrib*		pAppendAttr = NULL,
		UINT					uRandomSeed	= 0,
		UINT					uRandomSeedEx	= 0,
		IItemEnv*				pEnv		= NULL)
	{
		return Add(
			sIdx.nGenre,
			sIdx.nDetailType,
			sIdx.nParticular,
			sIdx.nLevel,
			nEnhTimes,
			eWay,
			pStoneInfo,
			pAppendAttr,
			uRandomSeed,
			uRandomSeedEx,
			pEnv
		);
	}

	INT		Add(
		INT						nItemGenre,
		INT						nDetailType,
		INT						nParticularType,
		INT						nLevel,
		INT						nEnhTimes,
		KE_ITEM_DATARECORD_WAY	eWay,
		CONST KItem::KSTONE_INFO* pStoneInfo = NULL,
		CONST KMagicAttrib*		pAppendAttr = NULL,
		UINT					uRandomSeed	= 0,
		UINT					uRandomSeedEx = 0,
		IItemEnv*				pEnv		= NULL
	);

	INT		Add(KItem* pcItem);				// 从已有道具复制一个新的道具
	BOOL	Remove(INT nIdx, KE_ITEM_DATARECORD_WAY eRecord = emITEM_DATARECORD_NORMAL, KE_ITEM_CONSUMEMODE eConsumeMode = emITEM_CONSUMEMODE_NORMAL, 
						KPlayer* pPlayer = NULL, BOOL bEraseInMap = TRUE);
	VOID	RemoveAll(VOID);
	INT		GetItemCount(VOID)		{ return m_cIdxList.GetCount(); }
	INT		SearchID(DWORD dwID) CONST;
	KItemGenerator* GetGenerator(INT nVersion = 0) CONST;

	//VOID RegistLimitUse(QLuaScript& sc);
	VOID RegistLimitUse(LPCSTR szForbidType, KPlayerItem::KE_LIMITUSE_TYPE eType, LPCSTR szMap, INT bRegister);

#ifdef GAME_SERVER
	INT		Add(
		CONST KItem*			pcItem, 
		INT						nPlayerIndex,
		DupedDef::OP_LEVEL		dupedOpLevel, 
		DupedDef::OP_RESULT&	dupedRet, 
		IItemEnv*				pcEnv = NULL
	);
#else
	BOOL	IsUnknownItem(INT nVersion, KITEM_INDEX sIndex);			// 检查是否是客户端未知的道具
#endif

private:

	VOID	SetID(INT nIdx);
	INT		FindFree(VOID) CONST	{ return m_cIdxList.FindFree(); }
	INT		Add_Impl(CONST KPARAM& rsParam,
					CONST KItem::KSTONE_INFO* pStoneInfo,
					CONST KMagicAttrib* pAppendAttr,
					INT nExtParamMask = 0);

#ifdef GAME_SERVER
	INT		Add_Impl(CONST KItem* pcItem, INT nPlayerIndex, DupedDef::OP_LEVEL dupedOpLevel, DupedDef::OP_RESULT& dupedRet);
	BOOL	CanLogThisItem(KItem* pcItem);		// 是否需要记录该物件日志
#endif

// 成员数据定义

public:

	KItemSetting	m_cSetting;			// 物品设置
#ifdef GAME_SERVER
	std::map<std::string, KDATARECORD*> m_mpDataRecordList;	//需要记录数据埋点的道具信息列表
#endif
	std::map<DWORD, INT> m_mpIdToIdx;	// 键：道具的ID；值：道具的索引,这个map的数据一定要与m_)cIdxList的数据同步，不然会有问题
	
	typedef std::map<std::string, INT> KMP_DATA;
	struct KLIMITUSE_DATA
	{
		KMP_DATA mpData;
		CHAR szProperMap[32];
	};

	typedef std::map<KPlayerItem::KE_LIMITUSE_TYPE, KMP_DATA> KMP_LIMITUSE;
	std::map<std::string, KMP_LIMITUSE> m_mpLimitUseData;

	typedef std::map<KPlayerItem::KE_LIMIT_PRIORITY, KPlayerItem::vecLimitUseRule> mapLimitPriority;
	mapLimitPriority m_mpLimitPri;

private:

	DWORD			m_dwIDCreator;		// ID生成器，用于客户端与服务器端的交流
	QIndexList		m_cIdxList;			// 索引表
	KItemGenerator*	m_pItemGen;			// 道具生成器数组

#ifdef GAME_SERVER
	KDupedItemDeal	m_dupedItemDeal;	// 对复制装备的处理
	INT				m_nLastIdentity;	// 上一次使用的物品流水号
#endif

};

// -------------------------------------------------------------------------

extern KItemMgr	g_cItemMgr;
extern QLogFile	g_ItemLogFile;

// -------------------------------------------------------------------------

#endif /* __KITEMMGR_H__ */
