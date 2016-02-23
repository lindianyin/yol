
#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "Protocol/AccountLoginDef.h"
#include "kluaitem.h"
#include "kitem.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "kitemmgr.h"
#include "kmagicattribmanager.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "onlinegamemodule/trade/kplayerpurse_i.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KItem* KLuaItem::GetIt(QLuaScript& sc, INT nIndex)
{
	KLuaItem* pLuaItem = dynamic_cast<KLuaItem *>(GetCObj(sc.GetLuaState(), nIndex));
	if (pLuaItem)
		return pLuaItem->m_pIt;

	return NULL;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Name
// @Description		: 道具名称（包含后缀名）
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getName(VOID) CONST
{
	return	m_pIt->GetOrgName();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: OrgName
// @Description		: 道具原名（不含后缀名）
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getOrgName(VOID) CONST
{
	return	m_pIt->GetOrgName();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Suffix
// @Description		: 道具后缀名
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getSuffix(VOID) CONST
{
	return	m_pIt->GetSuffix();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Genre
// @Description		: 道具种类 (武器? 药品? 矿石?)
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getGenre(VOID) CONST
{
	return	m_pIt->GetGenre();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Detail
// @Description		: 道具详细类别
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getDetail(VOID) CONST
{
	return	m_pIt->GetDetailType();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Particular
// @Description		: 道具具体类别
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getParticular(VOID) CONST
{
	return	m_pIt->GetParticular();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Level
// @Description		: 道具等级
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getLevel(VOID) CONST
{
	return	m_pIt->GetLevel();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: ReqLevel
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getReqLevel(VOID) CONST
{
	return	m_pIt->GetReqLevel();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Quality
// @Description		: 道具品质
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getQuality(VOID) CONST
{
	return	m_pIt->GetQuality();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: FightPower
// @Description		: 战斗力
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getFightPower(VOID) CONST
{
	return	m_pIt->GetFightPower();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: RandSeed
// @Description		: 道具随机种子
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaItem::getRandSeed(VOID) CONST
{
	return	m_pIt->GetRandSeed();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: RandSeedEx
// @Description		: 道具随机种子
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaItem::getRandSeedEx(VOID) CONST
{
	return	m_pIt->GetRandSeedEx();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Count
// @Description		: 道具当前叠加数目
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getCount(VOID) CONST
{
	return	m_pIt->GetCount();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: MaxCount
// @Description		: 道具最大叠加数目
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getMaxCount(VOID) CONST
{
	return	m_pIt->GetMaxCount();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Price
// @Description		: 道具价格
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getPrice(VOID) CONST
{
	return	m_pIt->GetPrice();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: BindType
// @Description		: 道具绑定类型
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getBindType(VOID) CONST
{
	return	m_pIt->GetBindType();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: EnhTimes
// @Description		: 道具已强化次数
// @Property(R/RW)	: RW
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getEnhTimes(VOID) CONST
{
	return	m_pIt->GetEnhanceTimes();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: SlotSize
// @Description		: 装备的宝石孔数量
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getSlotSize(VOID) CONST
{
	return	m_pIt->GetStoneSlot();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Strengthen
// @Description		: 道具改造等级
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getStrengthen(VOID) CONST
{
	return	m_pIt->GetStrengthen();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: EquipPos
// @Description		: 装备应在身上装备的位置
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getEquipPos(VOID) CONST
{
	if (!KD_ISEQUIP(m_pIt->GetGenre()))
		return	-1;
	return	KPlayerItem::GetEquipPos(m_pIt->GetDetailType());
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Value
// @Description		: 道具的价值量
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getValue(VOID) CONST
{
	return	m_pIt->GetValue();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: StarLevel
// @Description		: 道具的价值量星级
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getStarLevel(VOID) CONST
{
	return	m_pIt->GetStarLevel();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: NameColor
// @Description		: 道具的名字颜色
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getNameColor(VOID) CONST
{
	return	m_pIt->GetNameColor();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: CdType
// @Description		: 道具的CD时间类型
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getCdType(VOID) CONST
{
	return	m_pIt->GetCDType();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: CustomType
// @Description		: 道具的自定义字符串类型
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getCustomType(VOID) CONST
{
	return	m_pIt->GetCustomType();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: EquipCategory
// @Description		: 装备类别
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getEquipCategory(VOID) CONST
{
	return	m_pIt->GetEquipCategory();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Id
// @Description		: 道具ID
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaItem::getId(VOID) CONST
{
	return	m_pIt->GetID();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Class
// @Description		: Class名字
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getClass(VOID) CONST
{
	return	m_pIt->GetClass();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: CustomString
// @Description		: 道具自定义字符串
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getCustomString(VOID) CONST
{
	return	m_pIt->GetCustomString();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: ForbidType
// @Description		: 限制类型
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getForbidType(VOID) CONST
{
	return	m_pIt->GetForbidType();
}

VOID KLuaItem::setEnhTimes(INT nEnhTimes)
{
	m_pIt->SetEnhanceTimes(nEnhTimes);
}

BOOL KLuaItem::ClearTempTable(VOID)
{
	return ClearScriptTempTable(g_cScriptManager.GetSafeScript()->GetLuaState(), s_szClassName, m_pIt->GetID());
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetTempTable
// @Description		: 取得脚本用的临时Table
// @ReturnCode		: 与此类绑定的临时Table
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetTempTable(QLuaScript& sc)
{
	if (!GetScriptTempTable(sc.GetLuaState(), s_szClassName, m_pIt->GetID()))
	{
		_ASSERT(FALSE);
		return 0;
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsEquip
// @Description		: 判断该道具是否一件装备
// @ReturnCode		: 1-是装备
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsEquip(QLuaScript& sc)
{
	sc.PushNumber(KD_ISEQUIP(m_pIt->GetGenre()));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsStone
// @Description		: 判断该道具是否宝石
// @ReturnCode		: 1-是宝石
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsStone(QLuaScript& sc)
{
	sc.PushNumber(KD_ISSTONE(m_pIt->GetGenre(), m_pIt->GetDetailType()));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsInvalid
// @Description		: 判断该道具是否处于失效状态
// @ReturnCode		: 1-失效
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsInvalid(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsInvalid());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsDuped
// @Description		: 判断是否复制道具
// @ReturnCode		: 1-是复制道具
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsDuped(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsDupedItem());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsTemp
// @Description		: 判断是否临时道具
// @ReturnCode		: 1-是临时道具
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsTemp(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsTemporary());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetLockIntervale
// @Description		: 取出解绑周期，为0表示不能自动解绑
// @ReturnCode		: dwTime，秒数
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetLockIntervale(QLuaScript& rcScript)
{
	rcScript.PushNumber(m_pIt->GetLockIntervale());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsLock
// @Description		: 判断是否已锁定
// @ReturnCode		: 1-已锁定
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsLock(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsLock());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsBind
// @Description		: 判断是否已绑定
// @ReturnCode		: 1-已绑定
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsBind(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsBind());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: CanBeShortcut
// @Description		: 是否可以放进快捷栏
// @ReturnCode		: 1可以
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaCanBeShortcut(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->CanBeShortcut());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetBaseAttrib
// @Description		: 获取物品的基础属性
// @ReturnCode		: 魔法属性表
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetBaseAttrib(QLuaScript& sc)
{
	sc.PushTable();
	for (INT i = 0; i < emITEM_COUNT_BASE; ++i)
	{
		sc.PushTable();
		::g_PushMagicAttribAsTable(sc, &m_pIt->m_aryBaseAttrib[i]);
		sc.SetTableIndex(i + 1);
	}
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetReqAttrib
// @Description		: 获取物品的需求属性
// @ReturnCode		: 魔法属性表
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetReqAttrib(QLuaScript& sc)
{
	sc.PushTable();
	for (INT i = 0; i < emITEM_COUNT_REQ; ++i)
	{
		CONST KItem::KREQUIRE_ATTR *pReq = &m_pIt->m_aryReqAttrib[i];
		sc.PushTable();
		sc.PushNumber(pReq->eRequire);
		sc.SetTableField("nReq");
		sc.PushNumber(pReq->nValue);
		sc.SetTableField("nValue");
		sc.SetTableIndex(i + 1);
	}
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetExtParam
// @Description		: 获取物品的扩展参数表
// @ReturnCode		: 指定扩展参数或扩展参数表
// @ArgumentFlag	: [d]
// @ArgumentComment	: nIndex	返回第nIndex个扩展参数，0表示返回整个扩展参数表
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetExtParam(QLuaScript& sc)
{
	INT nIndex = sc.GetInt(1);
	if (!nIndex)
	{
		sc.PushTable();
		for (INT i = 0; i < emITEM_COUNT_EXTPARAM; ++i)
		{
			sc.PushNumber(m_pIt->GetExtParam(i));
			sc.SetTableIndex(i + 1);
		}
		return	1;
	}
	else
	{
		sc.PushNumber(m_pIt->GetExtParam(nIndex - 1));
			return	1;
	}
	return	0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsStackable
// @Description		: 是否可叠放物品
// @ReturnCode		: 1可叠放物品
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsStackable(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsStackable());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsDestoriable
// @Description		: 可否销毁
// @ReturnCode		: 1可销毁
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsDestoriable(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsDestoriable());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsForbitThrow
// @Description		: 可否丢弃
// @ReturnCode		: 1不可丢弃
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsForbitThrow(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsForbitThrow());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: IsForbitSell
// @Description		: 可否卖店
// @ReturnCode		: 1不可卖店
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsForbitSell(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->IsForbitSell());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetOwner
// @Description		: 获得道具所属的角色对象
// @ReturnCode		: 不属于任何角色则返回nil
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetOwner(QLuaScript& rcScript)
{
	INT nPlayerIdx = m_pIt->GetBelong();
	if (nPlayerIdx < 0 || nPlayerIdx >= MAX_PLAYER)
		return	0;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	if (pPlayer)
		rcScript.PushObj((QLunaBase*)pPlayer->GetScriptInterface());
	else
		rcScript.PushNull();
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Remove
// @Description		: 将道具对象从游戏世界删除（道具此时应该不属于任何一个角色）
// @ReturnCode		: 成功返回1
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaRemove(QLuaScript& rcScript)
{
	if (m_pIt->GetBelong() > 0)
		return	0;

	BOOL bRet = g_cItemMgr.Remove(m_pIt->GetGameIndex(), emITEM_DATARECORD_REMOVE);
	rcScript.PushNumber(bRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: SetTimeOut
// @Description		: 设置过期时间
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 过期类型：0绝对时间（秒数），1相对时间（也是秒数）
// @ArgumentComment	: 时间
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaSetTimeOut(QLuaScript& sc)
{
	INT nType = sc.GetInt(1);
	UINT uTime = (UINT)sc.GetInt(2);
	// 相对时间须转换为帧数
	if (nType == emKITEM_TIMEOUTTYPE_RELATIVE)
		uTime *= GAME_FPS;
	KITEM_TIMEOUT timeOut;
	timeOut.emType = (KE_ITEM_TIMEOUTTYPE)nType;
	timeOut.dwTimeout = uTime;
	m_pIt->SetTimeout(&timeOut);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: SetCount
// @Description		: 设置物品的个数
// @ReturnCode		: 1成功
// @ArgumentFlag	: d[d]
// @ArgumentComment	: 设置的个数
// @ArgumentComment	: 数据埋点类型途径
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaSetCount(QLuaScript& sc)
{
	INT nRet = 0;
	INT nWay = emKADDITEM_BYSCRIPT;
	INT nCount = sc.GetInt(1);

#ifndef GAME_SERVER
	if (!m_pIt->IsTemporary())
		return 0;			// 客户端只能修改临时道具的个数
#endif

	INT nPlayerIdx = m_pIt->GetBelong();
	if (nPlayerIdx <= 0)
		return 0;

	KE_ITEM_DATARECORD_WAY eRecordWay = emITEM_DATARECORD_NORMAL;
	if (sc.IsNumber(2))
		eRecordWay = (KE_ITEM_DATARECORD_WAY)sc.GetInt(2);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	QCONFIRM_RET_FALSE(pPlayer);
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	nRet = pPlayerItem->SetItemStackCount(
		m_pIt->GetGameIndex(), nCount, eRecordWay,
		(KE_LOSE_ITEM_WAY)nWay, (KE_ADD_ITEM_WAY)nWay
		);

	sc.PushNumber(nRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: SetGenerateMoneyType
// @Description		: 设置道具操作时选择的金钱类型
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 设置金钱类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaSetGenerateMoneyType(QLuaScript& sc)
{
	INT nMoneyType = sc.GetInt(1);
	QCONFIRM_RET_FALSE(nMoneyType == EM_MONEY_SILVER || nMoneyType == EM_MONEY_COIN);
	m_pIt->SetGenerateMoneyType(nMoneyType);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetSellMoneyType
// @Description		: 返回卖店可获得的银两类型
// @ReturnCode		: 0:绑定银两; 1:普通银两
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetSellMoneyType(QLuaScript& rcScript)
{
	rcScript.PushNumber((INT)m_pIt->GetSellMoneyType());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: HasStone
// @Description		: 返回装备孔位置上是否有镶嵌宝石，如果有，返回宝石等级
// @ReturnCode		: 1:有 0:没有
// @ArgumentFlag	: d
// @ArgumentComment	: 宝石孔位置1-3
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaHasStone(QLuaScript& rcScript)
{
	INT nPos = rcScript.GetInt(1);
	QCONFIRM_RET_NULL(nPos > 0 && nPos <= m_pIt->GetStoneSlot());
	rcScript.PushNumber(m_pIt->HasStone(nPos));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetEquipScore
// @Description		: 
// @ReturnCode		: 装备评分
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetEquipScore(QLuaScript& rcScript)
{
	KPlayerItem* pPlayerItem = NULL;
	KPlayer* pOwner = g_cPlayerMgr.GetByIndex(m_pIt->GetBelong());
	QCONFIRM_RET_NULL(pOwner);
	pPlayerItem = pOwner ? pOwner->GetCtrl<KPlayerItem>() : NULL;
	QCONFIRM_RET_NULL(pPlayerItem);

	INT nScore = pPlayerItem->CalcEquipScore(m_pIt);
	rcScript.PushNumber(nScore);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetResourceId
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetResourceId(QLuaScript& rcScript)
{
	BOOL bSex = rcScript.GetInt(1);
	INT nResId = m_pIt->GetResourceID(bSex);
	rcScript.PushNumber(nResId);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetChangeColorScheme
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetChangeColorScheme(QLuaScript& rcScript)
{
	//INT nColorScheme = m_pIt->GetChangeColorScheme();
	//rcScript.PushNumber(nColorScheme);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetTimeOut
// @Description		: 获得装备超时时间
// @ReturnCode		: 0:绝对时间（秒数）; 1:相对时间（也是秒数）
// @ReturnCode		: d:秒数
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetTimeOut(QLuaScript& sc)
{
	CONST KITEM_TIMEOUT *timeOut	= m_pIt->GetTimeout();
	INT nResult						= 0;
	INT nType						= (INT)timeOut->emType;
	DWORD dwTimeout					= timeOut->dwTimeout;
	
	if (nType == emKITEM_TIMEOUTTYPE_RELATIVE)
		dwTimeout = timeOut->dwTimeout / GAME_FPS;
	
	sc.PushNumber(nType);
	nResult ++;
	sc.PushNumber(dwTimeout);
	nResult ++;

	return nResult;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: Bind
// @Description		: 绑定道具
// @ReturnCode		: 1成功绑定
// @ArgumentFlag	: d
// @ArgumentComment	: bForce:强制
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaBind(QLuaScript& sc)
{
	BOOL bForce = sc.GetInt(1);
	sc.PushNumber(m_pIt->Bind(bForce));

#ifdef GAME_SERVER
	KPlayerItem* pPlayerItem = NULL;
	KPlayer* pOwner = g_cPlayerMgr.GetByIndex(m_pIt->GetBelong());
	LOG_PROCESS_ERROR(pOwner);
	pPlayerItem = pOwner ? pOwner->GetCtrl<KPlayerItem>() : NULL;
	LOG_PROCESS_ERROR(pPlayerItem);
	pPlayerItem->SyncItem(m_pIt->GetGameIndex());
#endif

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetGDPLStr
// @Description		: 得到物品的gdpl字符串
// @ReturnCode		: 1 成功
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetGDPLStr(QLuaScript& sc)
{
	CHAR buf[32];
	KPlayerItem* pPlayerItem = NULL;
	KPlayer* pOwner = g_cPlayerMgr.GetByIndex(m_pIt->GetBelong());
	QCONFIRM_RET_NULL(pOwner);
	pPlayerItem = pOwner ? pOwner->GetCtrl<KPlayerItem>() : NULL;
	QCONFIRM_RET_NULL(pPlayerItem);

	snprintf(buf, sizeof(buf), "%d,%d,%d,%d", m_pIt->GetGenre(), m_pIt->GetDetailType(), 
		m_pIt->GetParticular(), m_pIt->GetLevel());

	sc.PushString(buf);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: StoneClean
// @Description		: 宝石摘取后整理（把后面的宝石提到前面）
// @ReturnCode		: 1 成功
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaStoneClean(QLuaScript& sc)
{
	KPlayerItem* pPlayerItem = NULL;
	KPlayer* pOwner = g_cPlayerMgr.GetByIndex(m_pIt->GetBelong());
	QCONFIRM_RET_NULL(pOwner);
	pPlayerItem = pOwner ? pOwner->GetCtrl<KPlayerItem>() : NULL;
	QCONFIRM_RET_NULL(pPlayerItem);

	KItem* pEquip = m_pIt;
	QCONFIRM_RET_NULL(pEquip);
	if(pPlayerItem->CleanStone(pEquip))
		sc.PushNumber(1);
	else
		sc.PushNumber(2);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: StonePunch
// @Description		: 宝石打孔
// @ReturnCode		: 1 成功
// @ArgumentFlag	: d
// @ArgumentComment	: 宝石孔位置
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaStonePunch(QLuaScript& sc)
{
	INT nSlot = sc.GetInt(1);
	KPlayerItem* pPlayerItem = NULL;
	KPlayer* pOwner = g_cPlayerMgr.GetByIndex(m_pIt->GetBelong());
	QCONFIRM_RET_NULL(pOwner);
	pPlayerItem = pOwner ? pOwner->GetCtrl<KPlayerItem>() : NULL;
	QCONFIRM_RET_NULL(pPlayerItem);

	KItem* pEquip = m_pIt;

	sc.PushNumber(pPlayerItem->StonePunch(pEquip, nSlot));

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetStoneInfo
// @Description		: 得到一件装备的宝石信息
// @ReturnCode		: 宝石信息 { {nStoneId = ?, nStoneLevel = ?}, 0, -1 } // 0:未打宝石的孔，-1:没有孔
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetStoneInfo(QLuaScript& sc)
{
	INT nSlot = 0;
	KItem* pEquip = m_pIt;
	QCONFIRM_RET_NULL(pEquip);
	QCONFIRM_RET_NULL(KD_ISEQUIP(pEquip->GetGenre()));

	sc.PushTable();
		
	for(; nSlot < emSTONE_MAX_SLOT; ++nSlot)
	{
		sc.PushTable();

		if(nSlot < pEquip->GetStoneSlot())
		{
			sc.PushNumber(pEquip->m_aryStoneInfo[nSlot].byStoneId);
		}
		else
		{
			sc.PushNumber(-1);
		}
		sc.SetTableField("nStoneId");

		if(nSlot < pEquip->GetStoneSlot())
		{
			sc.PushNumber(pEquip->m_aryStoneInfo[nSlot].byStoneLevel);
		}
		else
		{
			sc.PushNumber(-1);
		}
		sc.SetTableField("nStoneLevel");

		sc.SetTableIndex(nSlot + 1);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetAppendAttr
// @Description		: 得到一件装备的附加属性信息
// @ReturnCode		: 附加属性信息 { {nAttr = ?, nValue = ?}, ...}
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetAppendAttr(QLuaScript& sc)
{
	KItem* pEquip = m_pIt;
	QCONFIRM_RET_NULL(pEquip && KD_ISEQUIP(pEquip->GetGenre()));

	sc.PushTable();

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		sc.PushTable();

		sc.PushNumber(pEquip->m_aryAppendAttrib[i].nAttribType);
		sc.SetTableField("nAttr");

		sc.PushNumber(pEquip->m_aryAppendAttrib[i].nValue[0]);
		sc.SetTableField("nValue");

		sc.SetTableIndex(i + 1);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetUpgradeItems
// @Description		: 得到装备升级所需材料信息
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetUpgradeItems(QLuaScript& sc)
{
	QCONFIRM_RET_FALSE(KD_ISEQUIP(m_pIt->GetGenre()));

	CONST KLibOfBPT* pLibBPT = g_cItemMgr.GetGenerator()->GetLibOfBPT();
	QCONFIRM_RET_FALSE(pLibBPT);
	KITEM_INDEX sIndex = { m_pIt->GetGenre(), m_pIt->GetDetailType(), m_pIt->GetParticular(), m_pIt->GetLevel() + 1};
	CONST KBASICPROP_EQUIPMENT* pEq = static_cast<CONST KBASICPROP_EQUIPMENT*>(pLibBPT->GetBasicProp(sIndex));
	QCONFIRM_RET_FALSE(pEq);

	sc.PushTable();

	for(INT i = 0; i < emITEM_COUNT_UPGRADE; ++i)
	{
		sc.PushTable();
		sc.PushNumber(pEq->aryNeedItems[i].nGenre);
		sc.SetTableField("nGenre");
		sc.PushNumber(pEq->aryNeedItems[i].nDetail);
		sc.SetTableField("nDetail");
		sc.PushNumber(pEq->aryNeedItems[i].nParticular);
		sc.SetTableField("nParticular");
		sc.PushNumber(pEq->aryNeedItems[i].nAmount);
		sc.SetTableField("nAmount");

		sc.SetTableIndex(i+1);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetSaveAppendAttr
// @Description		: 得到一件装备的洗炼后的附加属性信息
// @ReturnCode		: 附加属性信息 { {nAttr = ?, nValue = ?}, ...}
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetSaveAppendAttr(QLuaScript& sc)
{
	KItem* pEquip = m_pIt;
	QCONFIRM_RET_NULL(pEquip && KD_ISEQUIP(pEquip->GetGenre()));

	sc.PushTable();

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		sc.PushTable();

		sc.PushNumber(pEquip->m_arySaveAppendAttrib[i].nAttribType);
		sc.SetTableField("nAttr");

		sc.PushNumber(pEquip->m_arySaveAppendAttrib[i].nValue[0]);
		sc.SetTableField("nValue");

		sc.SetTableIndex(i + 1);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetBagPosLimit
// @Description		: 获得背包位置限制
// @ReturnCode		: 1成功绑定
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetBagPosLimit(QLuaScript& sc)
{
	sc.PushNumber(m_pIt->GetBagPosLimit());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetNextReqLevel
// @Description		: 获得下一等级装备的需求等级
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetNextReqLevel(QLuaScript& sc)
{
	QCONFIRM_RET_FALSE(KD_ISEQUIP(m_pIt->GetGenre()));
	CONST KLibOfBPT* pLibBPT = g_cItemMgr.GetGenerator()->GetLibOfBPT();
	QCONFIRM_RET_FALSE(pLibBPT);
	KITEM_INDEX sIndex = { m_pIt->GetGenre(), m_pIt->GetDetailType(), m_pIt->GetParticular(), m_pIt->GetLevel() + 1};
	CONST KBASICPROP_EQUIPMENT* pEq = static_cast<CONST KBASICPROP_EQUIPMENT*>(pLibBPT->GetBasicProp(sIndex));
	QCONFIRM_RET_FALSE(pEq);

	sc.PushNumber(pEq->nReqLevel);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaItem
// @LuaApiName		: GetStoneType
// @Description		: 获得宝石类型
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaGetStoneType(QLuaScript& sc)
{
	QCONFIRM_RET_FALSE(KD_ISSTONE(m_pIt->GetGenre(), m_pIt->GetDetailType()));
	CONST KLibOfBPT* pLibBPT = g_cItemMgr.GetGenerator()->GetLibOfBPT();
	QCONFIRM_RET_FALSE(pLibBPT);
	KITEM_INDEX sIndex = { m_pIt->GetGenre(), m_pIt->GetDetailType(), m_pIt->GetParticular(), m_pIt->GetLevel()};
	CONST KBASICPROP_STONE* pEq = static_cast<CONST KBASICPROP_STONE*>(pLibBPT->GetBasicProp(sIndex));
	QCONFIRM_RET_FALSE(pEq);

	sc.PushNumber(pEq->nStoneType);

	return 1;
}

#ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: BuyPrice
// @Description		: Ib价值
// @Property(R/RW)	: RW
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getBuyPrice(VOID) CONST
{
	return	m_pIt->GetBuyPrice();
}

VOID KLuaItem::setBuyPrice(INT nPrice)
{
	m_pIt->SetBuyPrice(nPrice);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: GUID
// @Description		: GUID的字符串
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getGUID(VOID) CONST
{
	return m_pIt->GetGuidString();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: GenTime
// @Description		: 道具生成时间
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaItem::getGenTime(VOID) CONST
{
	return	m_pIt->GetGenTime();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: IsIbItem
// @Description		: 是否IB物品
// @ReturnCode		: 1:是,0:否
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaIsIbItem(QLuaScript& sc)
{
	sc.PushNumber(IB_IS_IB_ITEM(m_pIt->GetItemGUID()));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: Regenerate
// @Description		: 重新生成一个道具
// @ReturnCode		: 成功返回1
// @ArgumentFlag	: ddddd???uu
// @ArgumentComment	: nGenre		
// @ArgumentComment	: nDetailType	
// @ArgumentComment	: nParticularType
// @ArgumentComment	: nLevel:道具等级
// @ArgumentComment	: nEnhTimes:道具已强化次数
// @ArgumentComment	: 若是table类型，则为StoneInfo的Table
// @ArgumentComment	: 若是table类型，则为附加属性信息
// @ArgumentComment	: 若是table类型，则为洗炼时锁住的属性( {0, 0, 1, 1, 0, 0} 表示第3和4条附加属性锁住了 )
// @ArgumentComment	: uRandSeed
// @ArgumentComment	: uRandSeedEx
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaRegenerate(QLuaScript& sc)
{
	INT nGenre			= sc.GetInt(1);
	INT nDetailType		= sc.GetInt(2);
	INT nParticularType	= sc.GetInt(3);
	INT nLevel			= sc.GetInt(4);
	INT nEnhTimes		= sc.GetInt(5);
	UINT uRandSeed		= sc.GetInt(9);
	UINT uRandSeedEx	= sc.GetInt(10);
	
	KItem::KREFINE_LOCK	aryLock[emITEM_COUNT_APPEND];
	KItem::KREFINE_LOCK* pRefineLock = NULL;

	KItem::KSTONE_INFO	aryStoneInfo[emSTONE_MAX_SLOT];
	KItem::KSTONE_INFO	*pStoneInfo = NULL;

	KMagicAttrib aryAppendAttr[emITEM_COUNT_APPEND];
	KMagicAttrib* pAppendAttr = NULL;

	INT nPlayerIdx = m_pIt->GetBelong();
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	QCONFIRM_RET_FALSE(pPlayer);

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	if (sc.IsTable(6))
	{	// 是StoneInfo的Table
		pStoneInfo = aryStoneInfo;
		for (INT i = 0; i < emSTONE_MAX_SLOT; i++)
		{
			sc.GetTableIndex(6, i + 1);
			INT nIndex = sc.GetTopIndex();
			sc.GetTableField(nIndex, "nStoneId");
			pStoneInfo[i].byStoneId = sc.GetInt(nIndex + 1);

			sc.GetTableIndex(6, i + 1);
			nIndex = sc.GetTopIndex();
			sc.GetTableField(nIndex, "nStoneLevel");
			pStoneInfo[i].byStoneLevel = sc.GetInt(nIndex + 1);
		}
	}

	if (sc.IsTable(7))
	{
		pAppendAttr = aryAppendAttr;
		for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
		{
			sc.GetTableIndex(7, i + 1);
			INT nIndex = sc.GetTopIndex();
			sc.GetTableField(nIndex, "nAttr");
			pAppendAttr[i].nAttribType = sc.GetInt(nIndex + 1);

			sc.GetTableIndex(7, i + 1);
			nIndex = sc.GetTopIndex();
			sc.GetTableField(nIndex, "nValue");
			pAppendAttr[i].nValue[0] = sc.GetInt(nIndex + 1);
		}
	}

	if (sc.IsTable(8))
	{
		for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
		{
			pRefineLock = aryLock;
			sc.GetTableIndex(8, i + 1);
			INT nIndex = sc.GetTopIndex();
			INT nLock = sc.GetInt(nIndex);
			pRefineLock[i] = nLock;
		}
	}

	BOOL bRet = m_pIt->Regenerate(
		nGenre,
		nDetailType,
		nParticularType,
		nLevel,
		nEnhTimes,
		pStoneInfo,
		pAppendAttr,
		pRefineLock,
		uRandSeed,
		uRandSeedEx
	);

	if(KD_ISEQUIP(m_pIt->GetGenre()))
	{
		KITEM_POS sPos;
		pPlayerItem->GetItemPos(m_pIt->GetGameIndex(), sPos);

		if(sPos.eRoom == emROOM_EQUIP || sPos.eRoom == emROOM_EQUIPEX)
		{
			// 重新计算人物战斗力
			g_cCharacterHelper.CalcFightScore(*pPlayer);
			pPlayer->FireEvent(emKOBJEVENTTYPE_PLAYER_EQUIP_OPS, pPlayer->GetId(), 0, 0);
		}
	}
	
	sc.PushNumber(bRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: ClearSaveAppendAttr
// @Description		: 清除已保存的洗炼附加属性
// @ReturnCode		: 1成功
// @ArgumentFlag	: d
// @ArgumentComment	: nSaveOri
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaClearSaveAppendAttr(QLuaScript& sc)
{
	INT nSaveOri = sc.GetInt(1);

	m_pIt->ClearSaveAppendAttr(nSaveOri);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: Sync
// @Description		: 同步道具数据到客户端
// @ReturnCode		: 1成功
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaSync(QLuaScript& sc)
{
	INT nRet = 0;
	INT nPlayerIdx = m_pIt->GetBelong();
	if (nPlayerIdx <= 0)
		return	0;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	QCONFIRM_RET_FALSE(pPlayer);
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);
	nRet = pPlayerItem->SyncItem(m_pIt->GetGameIndex());

	sc.PushNumber(nRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: ChangeColorScheme
// @Description		: 设置装备换色方案
// @ReturnCode		: 1成功
// @ArgumentFlag	: d
// @ArgumentComment	: nScheme:新的换色方案
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaChangeColorScheme(QLuaScript& sc)
{
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: SetCustom
// @Description		: 设置道具自定义字符串
// @ReturnCode		: 1成功
// @ArgumentFlag	: ds
// @ArgumentComment	: nCustomType:类型（）
// @ArgumentComment	: szCustom:自定义字符串
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaSetCustom(QLuaScript& rcScript)
{
	KItem::KE_CUSTOM_TYPE eCustomType = (KItem::KE_CUSTOM_TYPE)rcScript.GetInt(1);
	LPCSTR pszCustom = rcScript.GetStr(2);
	rcScript.PushNumber(m_pIt->SetCustom(eCustomType, pszCustom));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: SetExtParam
// @Description		: 设置道具ExtParam
// @ReturnCode		: 1成功
// @ArgumentFlag	: dd
// @ArgumentComment	: 位置
// @ArgumentComment	: 数据
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaSetExtParam(QLuaScript& rcScript)
{
	INT nIdx = rcScript.GetInt(1);
	INT nValue = rcScript.GetInt(2);

	// 最多设置3个
	QCONFIRM_RET_NULL(nIdx >= 1 && nIdx <= 3);

	m_pIt->SetExtParam(nIdx - 1, nValue);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: ApplyMagicAttrib
// @Description		: 应用魔法属性（应该移到技能魔法属性里）
// @ReturnCode		: 
// @ArgumentFlag	: s{dd}[d]
// @ArgumentComment	: szAttrib:魔法属性名
// @ArgumentComment	: nValue1:魔法属性参数1
// @ArgumentComment	: nValue2:魔法属性参数2
// @ArgumentComment	: bSync:是否同步客户端
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaApplyMagicAttrib(QLuaScript &sc)
{
	INT nValue = 0;
	INT nAttrib = g_cMagicAttribMgr.GetIdFormString(sc.GetStr(1));
	if (nAttrib <= KD_MAGIC_INVALID)
		return	0;

	KCharacter* pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_pIt->GetBelong());
	if(!pPlayer) return 0;

	KMagicAttrib sAttrib;
	sAttrib.nAttribType = nAttrib;
	for (INT i = 0; i < KD_MAGIC_VALUE_NUM; ++i)
	{
		sc.GetTableIndex(2, i + 1);
		nValue = sc.GetInt(-1);
		if(KD_ISMEDICINE(m_pIt->GetGenre()))
		{
			INT nMedicineEffect = pPlayer->GetMedicineEffect();
			// 药品的实际回复量 = int（药品的描述回复量 * （1 + 药效 / 100））
			nValue = (nValue * 100 + nMedicineEffect * nValue) / 100;
		}
		sAttrib.nValue[i] = nValue;
	}

	g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, sAttrib, FALSE);

	//if (sc.GetInt(4))
	//{
	//	// 同步给客户端
	//	KSYNC_NPC_ATTRIB sSync;
	//	sSync.cProtocol		= (PROTOCOL_MSG_TYPE)s2c_npcattrib;
	//	sSync.dwId			= m_pHim->GetId();
	//	sSync.sAttrib		= sAttrib;
	//	m_pHim->SendDataToNearRegion(&sSync, sizeof(sSync));
	//}

	return	0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaItem
// @LuaApiName		: RemoveMagicAttrib
// @Description		: 移除NPC的魔法属性
// @ReturnCode		: 无
// @ArgumentFlag	: s{dd}
// @ArgumentComment	: szAttrib:魔法属性名
// @ArgumentComment	: nValue1:魔法属性参数1
// @ArgumentComment	: nValue2:魔法属性参数2
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::LuaRemoveMagicAttrib(QLuaScript &sc)
{
	INT nAttrib = g_cMagicAttribMgr.GetIdFormString(sc.GetStr(1));
	if (nAttrib <= KD_MAGIC_INVALID)
		return	0;

	KCharacter* pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_pIt->GetBelong());
	if(!pPlayer) return 0;

	KMagicAttrib sAttrib;
	sAttrib.nAttribType = nAttrib;
	for (INT i = 0; i < KD_MAGIC_VALUE_NUM; ++i)
	{
		sc.GetTableIndex(2, i + 1);
		sAttrib.nValue[i] = sc.GetInt(-1);
	}

	g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, sAttrib, TRUE);
	return	0;
}

#else	// #ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaItem
// @LuaApiName		: Intro
// @Description		: 道具的描述信息
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getIntro(VOID) CONST
{
	return	m_pIt->GetIntro();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaItem
// @LuaApiName		: Help
// @Description		: 道具的描述信息
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getHelp(VOID) CONST
{
	return	m_pIt->GetHelp();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaItem
// @LuaApiName		: Index
// @Description		: 道具的全局索引号
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaItem::getIndex(VOID) CONST
{
	return	m_pIt->GetGameIndex();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaItem
// @LuaApiName		: IconImage
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getIconImage(VOID) CONST
{
	return	m_pIt->GetIconImage();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaItem
// @LuaApiName		: ViewImage
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getViewImage(VOID) CONST
{
	return	m_pIt->GetViewImage();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaItem
// @LuaApiName		: TransparencyIcon
// @Description		: 道具透明图层图标
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getTransparencyIcon(VOID) CONST
{
	return	m_pIt->GetTransparencyIcon();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaItem
// @LuaApiName		: MaskLayerIcon
// @Description		: 道具掩图层图标
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaItem::getMaskLayerIcon(VOID) CONST
{
	return m_pIt->GetMaskLayerIcon();
}

#endif

#include "kluaitem_table.hpp"
// -------------------------------------------------------------------------
