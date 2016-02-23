
#include "stdafx.h"
#include "kmasocketset.h"
#include "kitemmgr.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"

#include "kmagicattribmanager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// KMASocketSet

KMASocketSet::KMASocketSet(INT nCount) : m_nCount(nCount), m_pSocket(new KSOCKET[nCount])
{
	ASSERT(m_nCount > 0);
	m_bInit = FALSE;
	for (INT i = 0; i < nCount; ++i)
		ClearSocket(i);
}

KMASocketSet::~KMASocketSet()
{
	delete [] m_pSocket;
}

KMASocketSet &KMASocketSet::operator = (CONST KMASocketSet &cMASS)
{
	if (!PreAssign(cMASS))
		return	*this;

	INT i = 0;

	for (; i < m_nCount && i < cMASS.GetCount(); ++i)
		m_pSocket[i] = cMASS.m_pSocket[i];

	// 如果两边大小不等，清掉剩余项
	for (; i < m_nCount; ++i)
		ClearSocket(i);

	return	*this;
}

BOOL KMASocketSet::Init(VOID)
{
	for (INT i = 0; i < m_nCount; ++i)
	{
		m_pSocket[i].bActive = FALSE;		// 禁用全部魔法属性
		m_pSocket[i].bVisible = TRUE;		// 魔法属性全部可见
	}

	m_bInit = TRUE;
	return	TRUE;
}

BOOL KMASocketSet::Reset(VOID)
{
	m_bInit = FALSE;
	for (INT i = 0; i < m_nCount; ++i)	// 清除所有魔法属性
		ClearSocket(i);

	return	TRUE;
}

CONST KMagicAttrib *KMASocketSet::GetMagicAttrib(INT nIndex) CONST
{
	if (!m_bInit)
		return	NULL;
	if (nIndex < 0 || nIndex >= m_nCount)
		return	NULL;
	return	&m_pSocket[nIndex].sMagicAttrib;
}

BOOL KMASocketSet::IsActive(INT nIndex) CONST
{
	if (!m_bInit)
		return	FALSE;

	if (nIndex < 0 || nIndex >= m_nCount)
	{	// 下标不对
		_ASSERT(FALSE);
		return	FALSE;
	}

	return	m_pSocket[nIndex].bActive;
}

BOOL KMASocketSet::IsVisible(INT nIndex) CONST
{
	if (!m_bInit)
		return	FALSE;

	if (nIndex < 0 || nIndex >= m_nCount)
	{	// 下标不对
		_ASSERT(FALSE);
		return	FALSE;
	}

	return	m_pSocket[nIndex].bVisible;
}

VOID KMASocketSet::UpdateState(INT nPlayerIdx /* = 0 */, BOOL bActive/* = FALSE*/)
{
	if (!m_bInit)
		return;

	Init();
	Update(nPlayerIdx, bActive);
}

BOOL KMASocketSet::PreAssign(CONST KMASocketSet &cMASS)
{
	ASSERT(m_nCount == cMASS.GetCount());	// 正常情况下要保证源和目标大小一致，不过即使不一致后面也会作相应的处理
	if (!m_bInit && !cMASS.m_bInit)
	{	// 左右都没初始化就什么也不管
		return	FALSE;
	}
	else if (cMASS.m_bInit)
	{	// 右边已经初始化了，那就先清掉左边的
		Reset();
	}
	else
	{	// 左边已经初始化，右边却还没有，显然有问题！
		_ASSERT(FALSE);
		return	FALSE;
	}

	m_bInit = TRUE;
	return	TRUE;
}

BOOL KMASocketSet::ClearSocket(INT nIndex)
{
	if (nIndex < 0 || nIndex >= m_nCount)
	{	// 下标不对
		_ASSERT(FALSE);
		return	FALSE;
	}

	KSOCKET* pSocket = &m_pSocket[nIndex];
	pSocket->bActive = FALSE;
	pSocket->bVisible = TRUE;
	pSocket->sMagicAttrib.nAttribType = KD_MAGIC_INVALID;
	for (INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
		pSocket->sMagicAttrib.nValue[j] = 0;

	return TRUE;
}

// ----------------------------------------------------------------------------
// KMASS_Item

KMASS_Item::KMASS_Item(KItem& cItem, INT nCount) : KMASocketSet(nCount), m_pItem(&cItem)
{

}

KMASS_Item &KMASS_Item::operator = (CONST KMASS_Item &cMASS)
{	// 重载这个运算符的原因是为防止修改m_pItem的值
	KMASocketSet::operator = (cMASS);
	return	*this;
}

BOOL KMASS_Item::Init(VOID)
{
	if (!KMASocketSet::Init())
		return	FALSE;

	if (m_pItem->IsUseRandom())				// 参与随机属性激活的装备
		return	TRUE;

	for (INT i = 0; i < m_nCount; ++i)
		m_pSocket[i].bActive = FALSE;

	return	TRUE;
}

VOID KMASS_Item::Update(INT nPlayerIdx,  BOOL bActive/* = FALSE*/)
{
	if (!m_pItem->IsInvalid())
		return;

	if (nPlayerIdx <= 0)
	{	// 没在身上却无效，脱下装备时
		m_pItem->SetInvalid(FALSE);		// 使装备有效
		UpdateState(0, bActive);		// 更新激活属性
	}
}

// ----------------------------------------------------------------------------
// KMASS_Random

KMASS_Random::KMASS_Random(KItem& cItem) : KMASS_Item(cItem, emITEM_COUNT_RANDOM)
{

}

BOOL KMASS_Random::Init(VOID)
{
	if (!::KMASS_Item::Init())
		return	FALSE;

	if (!m_pItem->IsUseRandom())
		return	TRUE;

	if (m_pItem->IsInvalid())					// 无效装备所有属性都不激活
		return	FALSE;

	//if (m_pItem->GetDetailType() != equip_ride)
	//{
	//	for (INT i = 0; i < m_nCount / 2; ++i)
	//		m_pSocket[i].bActive = TRUE;			// 明属性必然被激活，即使没有被装备（在客户端显示的时候有用）
	//}
	return	TRUE;
}

VOID KMASS_Random::Update(INT nPlayerIdx /* = 0 */,  BOOL bActive /* = FALSE*/)
{
	KMASS_Item::Update(nPlayerIdx, bActive);

	if (m_pItem->IsInvalid())
		return;

	if (nPlayerIdx <= 0)							// 不装备到身上没用
		return;

//	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
//	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
//
//	KPlayer	*pPlayer = &Player[nPlayerIdx];
//	KNpc *pNpc = &Npc[pPlayer->m_nIndex];
//	KPlayerItem *pPlayerItem = &pPlayer->m_cPlayerItem;
//
//// 激活乘骑属性 ——————————————————
//	if (m_pItem->GetDetailType() == equip_horse)
//	{
//		BOOL bRide = pNpc->GetRideHorse();
//		for (INT i = 0; i < m_nCount; i++)
//			m_pSocket[i].bActive = bRide;
//		return;
//	}
// —————————————————————————

	//INT nActive = 0;							// 激活暗属性计数器
	//INT nSeries = m_pItem->GetSeries();
	//INT nPlace = KPlayerItem::GetEquipPos(m_pItem->GetDetailType());

	//if (::g_IsAccrue(pNpc->m_Series, nSeries))	// 如果玩家五行与当前装备相生
	//	++nActive;

	//for (INT j = 0; j < KD_ITEM_MAX_SERIES_ACTIVE; j++)
	//{	// 看看玩家的装备中是否有装备相生当前装备
	//	INT nEquipIdx = pPlayerItem->GetEquip(g_anEquipActive[nPlace][j]);
	//	if (!nEquipIdx)							// 该位置没穿装备就不管
	//		continue;
	//	if (::g_IsAccrue(Item[nEquipIdx].GetSeries(), nSeries))
	//		++nActive;
	//}
	//if (bActive)
	//	nActive = m_nCount / 2;
	//for (INT i = 0;  i < m_nCount / 2 && i < nActive; i++)
	//	m_pSocket[i + m_nCount / 2].bActive = TRUE;		// 激活nActive个暗属性
}

// ----------------------------------------------------------------------------
// KMASS_Enhance

KMASS_Enhance::KMASS_Enhance(KItem& cItem) : KMASS_Item(cItem, emITEM_COUNT_ENHANCE)
{
	m_pBPT = NULL;
}

INT KMASS_Enhance::GetTimes(INT nIndex) CONST
{
	if (!m_pBPT || nIndex < 0 || nIndex >= GetCount())
		return	0;
	return	m_pBPT[nIndex].nTimes;
}

BOOL KMASS_Enhance::Init(VOID)
{
	/*
	if (!KMASocketSet::Init())
		return	FALSE;

	if (m_pBPT)
		return	TRUE;				// 已经初始化过了，注意这个用法要求必须在Reset时把m_pBPActive置NULL

	if (!KD_ISEQUIP(m_pItem->GetGenre()))
	{	// 非装备不读配置文件
		m_pBPT = NULL;
		return	TRUE;
	}

	// 初始化配置文件关于装备的强化激活属性信息
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	if (!pItemGen)
	{
		ASSERT(FALSE);
		return	FALSE;
	}
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	KITEM_INDEX sIndex = m_pItem->GetIndex();
	CONST KBASICPROP_EQUIPMENT* pBPTEquip = (CONST KBASICPROP_EQUIPMENT*)pLibBPT->GetBasicProp(sIndex);
	ASSERT(pBPTEquip);
	m_pBPT = pBPTEquip->aryEnh;
	*/

	return	TRUE;
}

BOOL KMASS_Enhance::Reset(VOID)
{
	m_pBPT = NULL;
	return	KMASS_Item::Reset();
}

VOID KMASS_Enhance::Update(INT nPlayerIdx /* = 0 */,  BOOL bActive /* = FALSE*/)
{	// 即使没有装备，也可以根据道具本身的属性判断是否可以激活，这里nPlayerIdx参数没有用处
	KMASS_Item::Update(nPlayerIdx, bActive);

	if (!m_pBPT)
	{	// 能走到这里八成是没有正确Init
		_ASSERT(FALSE);
		return;
	}

	for (INT i = 0; i < m_nCount; i++)
	{	// 遍历所有属性，看是否达到激活条件
		INT nReqTimes = m_pBPT[i].nTimes;
		if (nReqTimes <= 0)
			continue;

		if (m_pItem->GetEnhanceTimes() >= nReqTimes)
		{
			if (nPlayerIdx <= 0 || !m_pItem->IsInvalid())
				m_pSocket[i].bActive = TRUE;		// 如果是身上的装备，失效时总不激活
		}
	}
}

// ----------------------------------------------------------------------------
// KMASS_Suite

KMASS_Suite::KMASS_Suite(CONST KSUITE_INFO &sSuiteInfo, KE_SUITE_TYPE eSuiteType) : KMASocketSet(emITEM_COUNT_SUITE)
{
	ASSERT(eSuiteType != emITEM_SUITE_ERROR);
	m_pSuiteInfo = &sSuiteInfo;
	m_eType = eSuiteType;
	CONST KBASICPROP_SUITEINFO *pSIF = m_pSuiteInfo->pInfo;
	for (INT i = 0; i < m_nCount; i++)
	{	// 初始化魔法属性值
		m_pSocket[i].sMagicAttrib.nAttribType = pSIF->arySuite[i].nAttrib;
		for (INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
			m_pSocket[i].sMagicAttrib.nValue[j] = pSIF->arySuite[i].aryValue[j];
	}
}

VOID KMASS_Suite::Update(INT nPlayerIdx /* = 0 */,  BOOL bActive /* = FALSE*/)
{
	if (nPlayerIdx <= 0)							// 没装备必然不激活
		return;

	INT nSuiteID = m_pSuiteInfo->pInfo->nSuiteID;
	if (nSuiteID <= KD_ITEM_INVALID_SUITEID)	// 无效的套装号
		return;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	KPlayerItem *pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	KITEM_SUITE_COUNT_MAP *pSuiteCount = NULL;
	INT nMagicActiveNum = 0;

	switch (m_eType)
	{
	case emITEM_SUITE_GOLD:
		pSuiteCount = pPlayerItem->GetGoldSuiteCount();
		break;
	case emITEM_SUITE_GREEN:
		pSuiteCount = pPlayerItem->GetGreenSuiteCount();
		nMagicActiveNum = pPlayerItem->GetMagicActiveSuit(nSuiteID);
		break;
	default:
		_ASSERT(FALSE);
		return;
	}

	for (INT i = 0; i < m_nCount; i++)
	{	// 遍历所有属性，看是否达到激活条件
		INT nReqNum = m_pSuiteInfo->pInfo->arySuite[i].nNum;
		if ((*pSuiteCount)[nSuiteID] >= nReqNum)
		{
			m_pSocket[i].bActive = TRUE;
		}
		else if (nMagicActiveNum > 0)
		{
			m_pSocket[i].bActive = TRUE;
			nMagicActiveNum-- ;
		}
	}
}

CONST KMASS_Suite::KItemVec& KMASS_Suite::GetSuiteParts(INT nEquipType) CONST
{
	_ASSERT(nEquipType > 0 && nEquipType <= equip_detailnum);
	return	m_pSuiteInfo->aryPart[KPlayerItem::GetEquipPos(nEquipType)];
}

// ----------------------------------------------------------------------------
// KMASS_Strengthen
KMASS_Strengthen::KMASS_Strengthen(KItem& cItem) : KMASS_Item(cItem, emITEM_COUNT_STRENGTHEN)
{
	m_pBPT = NULL;
}

BOOL KMASS_Strengthen::Init()
{
	if (!KMASocketSet::Init())
		return	FALSE;

	if (m_pBPT)
		return	TRUE;				// 已经初始化过了，注意这个用法要求必须在Reset时把m_pBPActive置NULL

	if (!KD_ISEQUIP(m_pItem->GetGenre()))
	{	// 非装备不读配置文件
		m_pBPT = NULL;
		return	TRUE;
	}

	// 初始化配置文件关于装备的强化激活属性信息
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	if (!pItemGen)
	{
		ASSERT(FALSE);
		return	FALSE;
	}
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	KITEM_INDEX sIndex = m_pItem->GetIndex();
	CONST KBASICPROP_EQUIPMENT* pBPTEquip = (CONST KBASICPROP_EQUIPMENT*)pLibBPT->GetBasicProp(sIndex);
	ASSERT(pBPTEquip);
	m_pBPT = pBPTEquip->aryStr;
	return TRUE;
}

INT KMASS_Strengthen::GetTimes(INT nIndex) CONST
{
	if (!m_pBPT || nIndex < 0 || nIndex >= GetCount())
		return	0;
	return	m_pBPT[nIndex].nTimes;
}

BOOL KMASS_Strengthen::Reset(VOID)
{
	m_pBPT = NULL;
	return	KMASS_Item::Reset();
}

VOID KMASS_Strengthen::Update(INT nPlayerIdx /* = 0 */,  BOOL bActive /* = FALSE*/)
{	// 即使没有装备，也可以根据道具本身的属性判断是否可以激活，这里nPlayerIdx参数没有用处
	KMASS_Item::Update(nPlayerIdx, bActive);

	if (!m_pBPT)
	{	// 能走到这里八成是没有正确Init
		_ASSERT(FALSE);
		return;
	}
	// 如果被改造过
	if (m_pItem->GetStrengthen() > 0)
	{
		for (INT i = 0; i < m_nCount; i++)
		{	// 遍历所有属性，看是否达到激活条件
			INT nReqTimes = m_pBPT[i].nTimes;
			if (nReqTimes <= 0)
				continue;
			if (m_pItem->GetEnhanceTimes() == nReqTimes)
			{
				if (nPlayerIdx <= 0 || !m_pItem->IsInvalid())
					m_pSocket[i].bActive = TRUE;		// 如果是身上的装备，失效时总不激活
			}
		}
	}
}

// -------------------------------------------------------------------------

/*	返回格式:
	szName  = 魔法属性名称,
	tbValue =
	{
		[1] = 魔法属性参数1,
		[2] = 魔法属性参数2,
		[3] = 魔法属性参数3,
	},
*/
VOID g_PushMagicAttribAsTable(QLuaScript &sc, CONST KMagicAttrib *pMA)
{
	ASSERT(pMA);
	sc.PushString(g_cMagicAttribMgr.GetStringFromId(pMA->nAttribType));
	sc.SetTableField("szName");
	sc.PushTable();
	for (INT i = 1; i <= KD_MAGIC_VALUE_NUM; ++i)
	{
		sc.PushNumber(pMA->nValue[i - 1]);
		sc.SetTableIndex(i);
	}
	sc.SetTableField("tbValue");
}

/*	返回格式:
	[1] =
	{
		szName  = 魔法属性名称,
		tbValue =
		{
			[1] = 魔法属性参数1,
			[2] = 魔法属性参数2,
			[3] = 魔法属性参数3,
		},
		bActive = 是否激活,
		bVisible = 是否可见,
	},
	[2] =
	{
		...
	},
	...
*/
VOID g_PushMASSAsTable(QLuaScript &sc, CONST KMASocketSet &cMASS)
{
	for (INT i = 0; i < cMASS.GetCount(); ++i)
	{
		CONST KMagicAttrib *pMA = cMASS.GetMagicAttrib(i);
		if (!pMA)
			return;
		sc.PushTable();
		::g_PushMagicAttribAsTable(sc,pMA);
		sc.PushNumber(cMASS.IsActive(i));
		sc.SetTableField("bActive");
		sc.PushNumber(cMASS.IsVisible(i));
		sc.SetTableField("bVisible");
		sc.SetTableIndex(i + 1);
	}
}

// -------------------------------------------------------------------------
