#include "stdafx.h"
//#include "knpc.h"
#include "kitem.h"
#include "kitemmgr.h"
#include "kmagicattrib.h"
#include "kluaitem.h"
#include "kitemgenerator.h"
#include "kdupeditemdeal.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamebase/ksysservice.h"
#include "Protocol/AccountLoginDef.h"
#include "kmagicattribmanager.h"
#include "onlinegamemodule/trade/kplayerpurse_i.h"
//#include "klogwritter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

extern QLogFile	g_SysAlarmLogFile;

KItem*	Item = NULL;

// ----------------------------------------------------------------------------

#define _KD_DEFAULT_BASEDATA		g_DefaultBaseData

static KItem::KBASEDATA g_DefaultBaseData =
{
	"",								// pszName
	"",								// pszClass
	{
		ITEM_INVALID_TYPE,		// sIndex.nGenre
		ITEM_INVALID_TYPE,		// sIndex.nDetailType
		ITEM_INVALID_TYPE,		// sIndex.nParticular
		KD_ITEM_NO_LEVEL_LIMIT,		// sIndex.nLevel
	},								// sIndex
	//0,								// nObjIdx
	0,								// nPrice
	0,								// nCurrencyType
	1,								// nQuality
	//0,								// nOrgValue 
	emITEM_BIND_NONE,				// eBindType
	FALSE,							// bValuable
	{0},							// aryExtParam
#ifndef GAME_SERVER
	"",								// pszIconImage
	"",								// pszViewImage
	"",								// pszIntro
	"",								// pszHelp
#endif
};

// ----------------------------------------------------------------------------
// KItem

KItem::KItem()
{
	m_pLuna	= NULL;
	m_nAssignMode = 0;

	Clear();
#ifdef GAME_SERVER
	m_nItemGUID = 0;
	m_nIdentity = 0;

	m_nUseFailedTimes = 0;	// added by dengyong 20100128
	m_dwLastFailedFrame = 0;	// added by dengyong 20100128
#endif
	m_nLockInterval = 0;
	memset(&m_sTimeout, 0, sizeof(m_sTimeout));
	m_pLuna	= new KLuaItem(this);
}

KItem::~KItem()
{
	SAFE_DELETE(m_pLuna);
}

BOOL KItem::CopyItemData(CONST KItem* pItem)
{
	if (!pItem)
		return	FALSE;

	KLuaItem* pLuna = m_pLuna;
	*this = *pItem;			// 往该类里增加成员要注意根据情况重载=号
	m_pLuna	= pLuna;
	m_nOwner = 0;
	return	TRUE;
}

BOOL KItem::Regenerate(INT				nGenre,
					   INT				nDetail,
					   INT				nParticular,
					   INT				nLevel,
					   INT				nEnhTimes,
					   CONST KSTONE_INFO* pStoneInfo,
					   CONST KMagicAttrib* pAppendAttr,
					   CONST KREFINE_LOCK* pRefineLock,
					   UINT				uRandSeed,
					   UINT				uRandSeedEx
					  )
{
	KItem cNewItem;
	if (!cNewItem.CopyItemData(this))
		return	FALSE;

	CONST GUID *pGuid = NULL;
#ifdef GAME_SERVER
	pGuid = &m_cGuid.GetGuid();
	//BOOL bNeedLog = KLogWritter::GetInstance()->ShouldLog(this);
#endif

	KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();

	if (!pItemGen->Gen_Item(
		&cNewItem,
		nGenre,
		nDetail,
		nParticular,
		nLevel,
		nEnhTimes,
		pStoneInfo,
		pAppendAttr,
		pRefineLock,
		uRandSeed,
		uRandSeedEx,
		FALSE,
		FALSE,
		pGuid))
	{
		return	FALSE;
	}

	INT nItemIdx  = GetGameIndex();
	KPlayer* pOwner = m_nOwner ? g_cPlayerMgr.GetByIndex(m_nOwner) : NULL;
	KPlayerItem* pPlayerItem = pOwner ? pOwner->GetCtrl<KPlayerItem>() : NULL;
	KITEM_POS sItemPos;// = { emROOM_NONE, 0, 0 };

	if (pPlayerItem)
	{
		if (!pPlayerItem->GetItemPos(nItemIdx, sItemPos))
			return FALSE;
		if (!pPlayerItem->RemoveOnly(nItemIdx, emKLOSEITEM_REGENE))		// 先将原道具从人身上临时删除
			return FALSE;
	}

	KItem cOldItem;
	cOldItem.CopyItemData(this);	// 备份原道具
	CopyItemData(&cNewItem);		// 使用新道具

	if (pPlayerItem && !pPlayerItem->Add(nItemIdx, sItemPos, emKADDITEM_REGEN))	// 再把新道具加到人身上
	{
		CopyItemData(&cOldItem);	// 如果失败则恢复原道具
		if (!pPlayerItem->Add(nItemIdx, sItemPos, emKADDITEM_REGEN))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	return	TRUE;
}


INT KItem::CalStrengthen(CONST INT nEnhanceStrengthen) 
{
	return  (nEnhanceStrengthen & STRENGTHEN_MASK) >> STRENGTHEN_OFFSET;
}

INT	KItem::CalEnhTimes(CONST INT nEnhanceStrengthen) 
{ 
	return  nEnhanceStrengthen & ENHANCE_MASK;
}

INT	KItem::CalEnhanceStrengthen( CONST INT nStrengthen, CONST INT nEnhTimes)
{
	INT nEnhanceStrengthen = (nStrengthen << STRENGTHEN_OFFSET) & STRENGTHEN_MASK;
	return nEnhanceStrengthen = nEnhTimes | nEnhanceStrengthen;
}

LPCSTR KItem::GetForbidType()
{
	KITEM_INDEX sIndex;
	sIndex.nGenre = GetGenre();
	sIndex.nDetailType = GetDetailType();
	sIndex.nParticular = GetParticular();
	sIndex.nLevel = GetLevel();

	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	CONST KBASICPROP_OTHER* pBPTOther = (CONST KBASICPROP_OTHER*)pLibBPT->GetBasicProp(sIndex);

	if (!pBPTOther)
		return NULL;

	return strcmp(pBPTOther->szForbidType, "") == 0 ? GetClass() : pBPTOther->szForbidType;
}

BOOL KItem::Init_BaseData(CONST KBASICPROP_BASEDATA* pBPT)		// 初始化基本配置
{
	m_sBaseData.pszName				= pBPT->szName;
	m_sBaseData.pszClass			= pBPT->szClass;
	m_sBaseData.sIndex.nGenre		= pBPT->nItemGenre;
	m_sBaseData.sIndex.nDetailType	= pBPT->nDetailType;
	m_sBaseData.sIndex.nParticular	= pBPT->nParticular;
	m_sBaseData.sIndex.nLevel		= pBPT->nLevel;
	m_sBaseData.nPrice				= pBPT->nPrice;
	m_sBaseData.byCurrencyType		= pBPT->byCurrencyType;
	m_sBaseData.nQuality			= pBPT->nQuality;
	m_sBaseData.nBindType			= pBPT->nBindType;
	m_sBaseData.bValuable			= pBPT->bValuable;

#ifndef GAME_SERVER
	m_sBaseData.pszIconImage		= pBPT->szIconImage;
	m_sBaseData.pszViewImage		= pBPT->szViewImage;
	m_sBaseData.pszIntro			= pBPT->szIntro;
	m_sBaseData.pszHelp				= pBPT->szHelp;
#endif
	for(INT i = 0; i < emITEM_COUNT_EXTPARAM; ++i)
	{
		if(!m_sBaseData.aryExtParam[i])
			m_sBaseData.aryExtParam[i] = pBPT->aryExtParam[i];
	}
	//::memcpy(m_sBaseData.aryExtParam, pBPT->aryExtParam, sizeof(m_sBaseData.aryExtParam));
	return	TRUE;
}

BOOL KItem::Init_BaseAttrib(CONST KBPT_EQUIP_BASEATTR* pBase, KRandom& rcRandom)
{
	// 根据入口参数, 确定装备的基本数据
	CONST KLibOfBPT* pLibBPT = g_cItemMgr.GetGenerator()->GetLibOfBPT();
	KITEM_INDEX sIndex = { item_equip_general, GetDetailType(), GetParticular(), GetLevel() + 1 };
	CONST KBASICPROP_GENERALEQUIP* pNextEq = static_cast<CONST KBASICPROP_GENERALEQUIP*>(pLibBPT->GetBasicProp(sIndex));

	KMagicAttrib* pDst = &m_aryBaseAttrib[0];
	CONST KBPT_EQUIP_BASEATTR *pSrc = &pBase[0];
	pDst->nAttribType = pSrc->nAttrib;

	ASSERT(countof(pDst->nValue) == countof(pSrc->aryRange));
	pDst->nValue[0] = rcRandom.Random(pSrc->aryRange[0].nMin, pSrc->aryRange[0].nMax);
	if(pNextEq)
	{
		pSrc = &(pNextEq->aryBasic[0]);
		pDst->nValue[1] = rcRandom.Random(pSrc->aryRange[0].nMin, pSrc->aryRange[0].nMax);
	}

	return TRUE;
}

INT KItem::GetBaseAttrValue(CONST QNumericalAward* pAward)
{
	INT nValue = 0;
	switch(GetDetailType())
	{
	case equip_sword:
	case equip_spear:
		nValue = pAward->nAttack;
		break;
	case equip_necklace:
		nValue = pAward->nCrit;
		break;
	case equip_ring:
		nValue = pAward->nHit;
		break;
	case equip_pendant:
		nValue = pAward->nResist;
		break;
	case equip_head:
	case equip_armor:
		nValue = pAward->nLife;
		break;
	case equip_belt:
	case equip_cloak:
		nValue = pAward->nDefense;
		break;
	case equip_foot:
		nValue = pAward->nDodge;
		break;
	case equip_suit:
	case equip_bottom:
	case equip_back:
		nValue = 0;
		break;
	default:
		ASSERT(FALSE);
	}

	return nValue;
}

INT KItem::GetEnhanceCost()
{
	INT nCost = 0;
	if(!KD_ISEQUIP(GetGenre()))
		return 0;
	INT nQuality = GetQuality();
	INT nEnhTimes = GetEnhanceTimes();
	if(nEnhTimes >= MAX_ENH_LV(nQuality))
	{
		++nQuality;
	}

	if(nQuality <= KD_ITEM_MAX_QUALITY)
	{
		CONST QNumericalAward* pAward = g_cAwardSetting.RequestNumericalAward(EQUIP_ENH_MODULE_NAME, nQuality, nEnhTimes + 1);
		QCONFIRM_RET_FALSE(pAward);

		nCost = pAward->nCost;
	}

	return nCost;
}

BOOL KItem::InitEnhAttrib()
{
	BOOL bResult = FALSE;
	INT nQuality = GetQuality();
	INT nEnhTimes = GetEnhanceTimes();
	CONST QNumericalAward* pAward = NULL;
	CONST QNumericalAward* pNextAward = NULL;
	m_sEnhAttrib.nAttribType = m_aryBaseAttrib[0].nAttribType;
	if(nEnhTimes)
	{
		pAward = g_cAwardSetting.RequestNumericalAward(EQUIP_ENH_MODULE_NAME, nQuality, nEnhTimes);
		LOG_PROCESS_ERROR(pAward);
		// 属性与基础属性一致
		m_sEnhAttrib.nValue[0] = GetBaseAttrValue(pAward);
	}
	else
	{
		m_sEnhAttrib.nValue[0] = 0;
	}
	
	if(nEnhTimes >= MAX_ENH_LV(nQuality))
	{
		++nQuality;
	}

	if(nQuality <= KD_ITEM_MAX_QUALITY)
	{
		pNextAward = g_cAwardSetting.RequestNumericalAward(EQUIP_ENH_MODULE_NAME, nQuality, nEnhTimes + 1);
		LOG_PROCESS_ERROR(pNextAward);
		m_sEnhAttrib.nValue[1] = GetBaseAttrValue(pNextAward);
	}
	else
	{
		m_sEnhAttrib.nValue[1] = 0;
	}
	bResult = TRUE;
EXIT0:
	return bResult;
}


// 获得附加属性条数
BOOL KItem::GetAppendAttribNum()
{
	INT nNum = 0;
	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		if(m_aryAppendAttrib[i].nAttribType == 0)
			break;
		++nNum;
	}

	return nNum;
}

// 装备升级+1条附加属性
BOOL KItem::InitAppendAttrib()
{
	INT							i					= 0;
	INT							j					= 0;
	INT							nAppendAttr			= 0;
	INT							nAppendValue		= 0;
	BOOL						bResult				= FALSE;
	LPCSTR*						pAttr				= NULL;
	CONST QNumericalAward*		pAward				= NULL;
	INT							nAppendNum			= GetAppendAttribNum();
	INT							nLevel				= GetLevel();

	LPCSTR aryOtherAttr[emITEM_COUNT_APPEND] =
	{
		"modifydmg_v",
		"adddef_v",
		"addcriticalrate_v",
		"addresist_v",
		"addhitrate_v"
	};
	LPCSTR aryLifeAttr[emITEM_COUNT_APPEND] = 
	{
		"addmaxlife_v",
		"addmaxmana_v",
		"addcriticalrate_v",
		"addresist_v",
		"addhitrate_v"
	};

	// TODO zhaoyu:写个宏 附加属性数目 = 装备等级 - 2
	if(nLevel <= nAppendNum + 1 || nLevel >= KD_EQUIP_MAX_LEVEL)
	{
		goto EXIT0;
	}
	ASSERT(nLevel == nAppendNum + 2);
	
	if(GetDetailType() == equip_head || GetDetailType() == equip_armor)
	{
		pAttr = aryLifeAttr;
	}
	else
	{
		pAttr = aryOtherAttr;
	}
	// TODO:循环pAttr，当发现pAttr中的属性在appendAttr中没有时，就作为当前升级加的属性，属性值取数值奖励1级
	for(i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		INT j = 0;
		INT nAttrType = g_cMagicAttribMgr.GetIdFormString(pAttr[i]);
		for(j = 0; j < emITEM_COUNT_APPEND; ++j)
		{
			if(m_aryAppendAttrib[j].nAttribType == nAttrType)
				break;
		}

		if(j >= emITEM_COUNT_APPEND)
		{
			nAppendAttr = nAttrType;
			break;
		}
	}

	pAward = g_cAwardSetting.RequestNumericalAward(EQUIP_APPEND_MODULE_NAME, 1, 1);
	LOG_PROCESS_ERROR(pAward);
	nAppendValue = g_cAwardSetting.GetAwardValueByMA(nAppendAttr, pAward);

	for(i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		if(m_aryAppendAttrib[i].nAttribType == 0)
		{
			m_aryAppendAttrib[i].nAttribType = nAppendAttr;
			m_aryAppendAttrib[i].nValue[0] = nAppendValue;
			m_aryAppendAttrib[i].nValue[1] = nAppendValue;

			m_aryAppendQuality[i] = 1;
			break;
		}
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KItem::RerandAppendAttrib(CONST KREFINE_LOCK* pLock)
{
	INT nAwardLevel = 0;
	INT nAppendAttr = 0;
	INT nAppendValue = 0;
	std::vector<INT> vecAttr;
	std::map<INT, INT> mapLockAttr;
	CONST QNumericalAward* pAward = NULL;
	INT* pRate = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nMaxIdx = 0;

	ASSERT(m_nGenerateMoneyType == EM_MONEY_COIN || m_nGenerateMoneyType == EM_MONEY_SILVER);

	LPCSTR aryAppendAttr[] =
	{
		"addmaxlife_v",
		"modifydmg_v",
		"adddef_v",
		"addcriticalrate_v",
		"addresist_v",
		"addhitrate_v",
		"adddodge_v"
	};

	// 随机几率，一共9个等级
	INT arySilverRate[] = {500, 250, 150, 50, 27, 10, 1, 1, 1};
	INT aryGoldRate[] = {300, 200, 200, 100, 100, 60, 25, 10, 5};

	if(GetGenerateMoneyType() == EM_MONEY_COIN)
	{
		pRate = aryGoldRate;
	}
	else
	{
		pRate = arySilverRate;
	}

	memcpy(m_arySaveAppendAttrib, m_aryAppendAttrib, sizeof(m_aryAppendAttrib));
	memcpy(m_arySaveAppendQuality, m_aryAppendQuality, sizeof(m_arySaveAppendQuality));

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		if(pLock[i] > 0)
		{
			mapLockAttr[m_aryAppendAttrib[i].nAttribType] = 1;
		}
	}

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		if(m_arySaveAppendAttrib[i].nAttribType == 0)
			break;
		// 被锁住了
		if(pLock[i] > 0)
			continue;

		// 随机属性(与前面及被锁属性不重复)
		vecAttr.clear();
		for(INT j = 0; j < countof(aryAppendAttr); ++j)
		{
			INT k = 0;
			INT nAttrType = g_cMagicAttribMgr.GetIdFormString(aryAppendAttr[j]);
			for(k = 0; k < i; ++k)
			{
				if(m_arySaveAppendAttrib[k].nAttribType == nAttrType)
					break;
			}

			std::map<INT, INT>::iterator it = mapLockAttr.find(nAttrType);
			if(k >= i && it == mapLockAttr.end())
			{
				vecAttr.push_back(nAttrType);
			}
		}

		m_arySaveAppendAttrib[i].nAttribType = vecAttr[KSysService::Rand(vecAttr.size())];
		nAppendAttr = m_arySaveAppendAttrib[i].nAttribType;

		// 随机数值
		INT nRand = KSysService::Rand(1000) + 1;
		INT nSum = 0;
		for(INT nIdx = 0; nIdx < countof(aryGoldRate); ++nIdx)
		{
			nSum += pRate[nIdx];
			if(nRand <= nSum)
			{
				pAward = g_cAwardSetting.RequestNumericalAward(EQUIP_APPEND_MODULE_NAME, 1, nIdx + 1);
				QCONFIRM_RET_FALSE(pAward);

				m_arySaveAppendQuality[i] = nIdx + 1;

				nAppendValue = g_cAwardSetting.GetAwardValueByMA(nAppendAttr, pAward);

				if(nIdx > nMaxIdx)
				{
					nMaxIdx = nIdx;
				}
				break;
			}
		}

		m_arySaveAppendAttrib[i].nValue[0] = nAppendValue;
		m_arySaveAppendAttrib[i].nValue[1] = nAppendValue;
	}

	cSafeScript->CallTableFunction("Item", "OnEquipRefined", 1, "od", m_pLuna, nMaxIdx + 1);

	return TRUE;
}

// 保存附加属性
VOID KItem::ClearSaveAppendAttr(BOOL bSaveOri)
{
	memset(m_arySaveAppendAttrib, 0, sizeof(m_arySaveAppendAttrib));

	if(!bSaveOri)
	{
		memcpy(m_aryAppendQuality, m_arySaveAppendQuality, sizeof(m_aryAppendQuality));
	}

	memset(m_arySaveAppendQuality, 0, sizeof(m_arySaveAppendQuality));
}

BOOL KItem::Init_ReqAttrib(CONST KBPT_EQUIP_REQATTR* pReq)
{
	for (INT i = 0; i < emITEM_COUNT_REQ; i++)
	{
		m_aryReqAttrib[i].eRequire	= pReq[i].eType;
		m_aryReqAttrib[i].nValue	= pReq[i].nValue;
	}
	return TRUE;
}

BOOL KItem::Init_Equip(CONST KBASICPROP_EQUIPMENT* pBPT, KRandom& rcRandom, KRandomEx& rcRandomEx, BOOL bNew)
{
	Init_BaseData(pBPT);							// 基础配置

	// 设置叠放值（装备不能叠放）
	m_dwMaxCount = 1;
	m_dwCurCount = 1;
	m_nAssignMode = 0;

	m_eEquipCategory = (KE_EQUIP_WEAPON_CATEGORY)pBPT->nCategory;	// 设置装备类别
	m_nLockInterval = 0;
	m_nFightPower = pBPT->nFightPower;
	m_nReqLevel		= pBPT->nReqLevel;				// 使用道具所需级别
	m_nStoneSlot			= rcRandomEx.Random(pBPT->arySlot.nMin, pBPT->arySlot.nMax);		// 设置宝石孔数量
	QCONFIRM_RET_FALSE(m_nStoneSlot <= emSTONE_MAX_SLOT);

	Init_BaseAttrib(pBPT->aryBasic, rcRandom);		// 赋值基本属性
	Init_ResColor(pBPT);							// 初始化装备资源和配色

#ifndef GAME_SERVER
	InitIconImage(m_sBaseData.pszIconImage);
#endif
	return TRUE;
}

BOOL KItem::Init_SuitEquip(CONST KBASICPROP_SUIT* pBPT, KRandom& rcRandom, BOOL bNew)
{
	Init_BaseData(pBPT);							// 基础配置

	// 设置叠放值（装备不能叠放）
	m_dwMaxCount = 1;
	m_dwCurCount = 1;
	m_nAssignMode = 0;

	m_nReqLevel		= pBPT->nReqLevel;				// 使用道具所需级别

	Init_BaseAttrib(pBPT->aryBasic, rcRandom);		// 赋值基本属性
	Init_SuitResColor(pBPT);

#ifndef GAME_SERVER
	InitIconImage(m_sBaseData.pszIconImage);
#endif
	return TRUE;
}

BOOL KItem::Init_Other(CONST KBASICPROP_OTHER* pBPT)
{
	Init_BaseData(pBPT);							// 基础配置

	m_nReqLevel		= pBPT->nReqLevel;				// 使用道具所需级别
	m_nCDType		= pBPT->nCDType;
	m_bShortcut		= pBPT->bShortcut;
	m_nAssignMode = 0;

	// 设置叠放值
	m_dwMaxCount = (pBPT->nStackMax > 1) ? pBPT->nStackMax : 1;
	if (m_dwCurCount > m_dwMaxCount)				// 如果是在LoadDB时创建物品，m_nCurCount可能不为1
		m_dwCurCount = m_dwMaxCount;

	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	ZeroMemory(m_aryReqAttrib,  sizeof(m_aryReqAttrib));

#ifndef GAME_SERVER
	InitIconImage(m_sBaseData.pszIconImage);
#endif

	return	TRUE;
}

INT KItem::HasStone(INT nSlot)
{
	QCONFIRM_RET_FALSE(m_nStoneSlot > 0 && m_nStoneSlot <= emSTONE_MAX_SLOT);
	QCONFIRM_RET_FALSE(nSlot > 0 && nSlot <= GetStoneSlot());

	if(m_aryStoneInfo[nSlot-1].byStoneId > 0 && m_aryStoneInfo[nSlot-1].byStoneId < emSTONE_LOCKED_ID)
	{
		return m_aryStoneInfo[nSlot-1].byStoneLevel;
	}

	return 0;
}

BOOL KItem::HasStoneLocked(INT nSlot)
{
	QCONFIRM_RET_FALSE(m_nStoneSlot > 0 && m_nStoneSlot <= emSTONE_MAX_SLOT);
	QCONFIRM_RET_FALSE(nSlot > 0 && nSlot <= GetStoneSlot());

	return (m_aryStoneInfo[nSlot-1].byStoneId == emSTONE_LOCKED_ID);
}

BOOL KItem::Init_Stone(CONST KBASICPROP_STONE* pBPT)
{
	QCONFIRM_RET_FALSE(pBPT->nLevel >= 1 && pBPT->nLevel <= emSTONE_MAX_LV);
	Init_BaseData(pBPT);							// 基础配置
	m_dwNameColor	= pBPT->nColor;
	m_dwMaxCount	= pBPT->nStackMax;
	m_nReqLevel		= pBPT->nReqLevel;				// 使用道具所需级别
	m_nEquipLevel	= pBPT->nEquipLevel;			// 装备等级要求

	/*
	for(INT i = 0; i < emSTONE_COUNT_ATTRIB; ++i)
	{
		m_aryBaseAttrib[i].nAttribType = pBPT->aryAttrib[i].nAttrib;
		// 目前宝石属性只有1个参数
		for(INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
		{
			m_aryBaseAttrib[i].nValue[j] = pBPT->aryAttrib[i].aryParam;
		}
	}
	*/

	return TRUE;
}

BOOL KItem::Init_Medicine(CONST KBASICPROP_MEDICINE* pBPT)
{
	Init_Other(pBPT);
	m_bCanUseInBZone	= pBPT->nCanUseInBZone;
	for(INT i = 0; i < emMEDICINE_COUNT_ATTRIB; ++i)
	{
		m_aryBaseAttrib[i].nAttribType = pBPT->aryAttrib[i].nAttrib;
		m_aryBaseAttrib[i].nValue[0]   = pBPT->aryAttrib[i].nValue;
		m_aryBaseAttrib[i].nValue[1]   = pBPT->aryAttrib[i].nTime;
	}
	return	TRUE;
}

BOOL KItem::Init_Script(CONST KBASICPROP_SCRIPT* pBPT)
{
	return	Init_Other(pBPT);
}

BOOL KItem::Init_Skill(CONST KBASICPROP_SKILL* pBPT)
{
	return	Init_Other(pBPT);
}

BOOL KItem::Init_Quest(CONST KBASICPROP_QUEST* pBPT)
{
	return	Init_Other(pBPT);
}

BOOL KItem::Init_ExtBag(CONST KBASICPROP_EXTBAG* pBPT)
{
	m_nBagPos = pBPT->nPos;
	return	Init_Other(pBPT);
}

BOOL KItem::Init_Stuff(CONST KBASICPROP_STUFF* pBPT)
{
	return	Init_Other(pBPT);
}

BOOL KItem::Init_Plan(CONST KBASICPROP_PLAN* pBPT)
{
	return	Init_Other(pBPT);
}

LPCSTR KItem::GetSuffix(VOID) CONST
{ 
	/*
	LPCSTR pszRet = NULL;
	CONST KMAGIC_INDEX* psKeyMA = NULL;
	CONST KItemGenerator* pItemGen = NULL;
	CONST KLibOfBPT* pcLibBPT = NULL;
	CONST KBASICPROP_MAGICATTRIB* pBaseMA = NULL;

	PROCESS_ERROR(KD_ISEQUIP(GetGenre()) && !IsWhite() && !KD_ISSUITE(GetGenre()));

	for (INT i = 0; i < emITEM_COUNT_RANDOM; ++i)
	{
		CONST KMAGIC_INDEX* psMAIndex = &m_GeneratorParam.unInfo.asRand[i];
		if (psMAIndex->nAttrib <= KD_MAGIC_INVALID)
			continue;

		if (!psKeyMA || psMAIndex->nLevel >= psKeyMA->nLevel)
			psKeyMA = psMAIndex;
	}
	PROCESS_ERROR(psKeyMA);

	pItemGen = g_cItemMgr.GetGenerator();
	PROCESS_ERROR(pItemGen);

	pcLibBPT = pItemGen->GetLibOfBPT();
	pBaseMA = pcLibBPT->m_cMagicAttrib.GetData(*psKeyMA);
	PROCESS_ERROR(pBaseMA);
	PROCESS_ERROR(*pBaseMA->szSuffix);// 后缀是空串，认为无后缀

	pszRet = pBaseMA->szSuffix;
EXIT0:
	return pszRet;
	*/
	return NULL;
}

BOOL KItem::Init_FullName(VOID)
{
	strcpy(m_szFullName, m_sBaseData.pszName);
	LPCSTR pszSuffix = GetSuffix();
	if (!pszSuffix)
		return	TRUE;

	strcat(m_szFullName, "·");
	strcat(m_szFullName, pszSuffix);

	return	TRUE;
}

BOOL KItem::Init_ResColor(CONST KBASICPROP_EQUIPMENT* pBPT)
{
	ASSERT(pBPT);

	m_nPandent1 = pBPT->nPandent1;
	m_nPandent2 = pBPT->nPandent2;

	for(INT i = 0; i < emITEM_COUNT_EX_PENDANT; ++i)
	{
		m_arrExPandents[i] = pBPT->aryExPandents[i];
	}

	return	TRUE;
}

BOOL KItem::Init_SuitResColor(CONST KBASICPROP_SUIT* pBPT)
{
	ASSERT(pBPT);

	m_nPandent1 = pBPT->nPandent1;
	m_nPandent2 = pBPT->nPandent2;

	for(INT i = 0; i < emITEM_COUNT_SUIT_EX_PENDANT; ++i)
	{
		m_arrSuitExPandents[i] = pBPT->aryExPandents[i];
	}

	return	TRUE;
}

BOOL KItem::EndGenerate(VOID)
{
	INT nRet = TRUE;

	/*
	if (!Init_FullName())
		nRet = FALSE;

	if (!Init_Value())
		nRet = FALSE;
	*/

	//for (INT i = 0; i < emITEM_COUNT_BASE; ++i)		// 对magic_damage_series_resist属性作特殊处理
	//{
	//	static INT anDamage[] = { -1, damage_poison, damage_light, damage_fire, damage_physics, damage_cold };
	//	KMagicAttrib& rsMagic = m_aryBaseAttrib[i];
	//	if (rsMagic.nAttribType != magic_damage_series_resist)
	//		continue;
	//	INT nSeries = GetSeries();
	//	if (nSeries <= 0 || nSeries >= countof(anDamage))
	//	{
	//		::memset(&rsMagic, 0, sizeof(rsMagic));
	//		continue;
	//	}
	//	rsMagic.nValue[2] = anDamage[nSeries];
	//}

	return	nRet;
}

BOOL KItem::SetCustom(KE_CUSTOM_TYPE eType, LPCSTR pszCustom, INT nBufSize /* = 0 */)
{
	if (eType == emCUSTOM_TYPE_NONE)
	{
		*m_szCustom = '\0';
		return TRUE;
	}

	if (m_eCustomType == emCUSTOM_TYPE_NONE || eType == m_eCustomType)
	{
		m_eCustomType = eType;
		INT nSize = nBufSize;
		if (nSize <= 0)
			nSize = strlen(pszCustom);
		if (nSize >= sizeof(m_szCustom))
			nSize = sizeof(m_szCustom) - 1;
		memcpy(m_szCustom, pszCustom, nSize);
		m_szCustom[nSize] = '\0';
		return TRUE;
	}

	ASSERT(FALSE);
	return FALSE;
}

VOID KItem::Remove()
{
#ifdef GAME_SERVER
	ASSERT(!m_nOwner);
	if (m_nOwner != 0)
	{
		//g_SysAlarmLogFile.LogRecordVar(
		//	LOG_LEVEL_INFO,
		//	L_ITEM_0, 
		//	Player[m_nOwner].m_AccountName,
		//	Player[m_nOwner].m_PlayerName,
		//	GetName(),
		//	GetGenre(),
		//	GetDetailType(),
		//	GetParticular()
		//);
	}
#endif

	Clear();
}

VOID KItem::Clear()
{
	::memset(m_aryBaseAttrib,	0, sizeof(m_aryBaseAttrib));
	::memset(m_aryAppendAttrib, 0, sizeof(m_aryAppendAttrib));
	::memset(m_arySaveAppendAttrib, 0, sizeof(m_arySaveAppendAttrib));
	::memset(m_aryAppendQuality, 0, sizeof(m_aryAppendQuality));
	::memset(m_arySaveAppendQuality, 0, sizeof(m_arySaveAppendQuality));
	::memset(m_aryReqAttrib,	0, sizeof(m_aryReqAttrib));
	::memset(&m_GeneratorParam,	0, sizeof(m_GeneratorParam));

	if (m_pLuna)
		m_pLuna->ClearTempTable();

	m_sBaseData					= _KD_DEFAULT_BASEDATA;
	m_dwID						= 0;
	m_dwCurCount				= 1;
	m_dwMaxCount				= 1;
	m_bTemporary				= FALSE;
	m_nResMale					= 0;
	m_nResFemale				= 0;
	m_nResEffect				= 0;
	m_nCDType					= 0;
	m_nReqLevel					= 0;
	m_bShortcut					= FALSE;
	m_bInvalid					= FALSE;
	m_bCanUseInBZone			= FALSE;
	m_bBind						= FALSE;
	m_bRentFromKin              = FALSE;
	m_bLock						= FALSE;
	m_nEnhTimes					= 0;
	m_nCurValue					= 0;
	m_nStarLevel				= 0;
	m_dwNameColor				= 0;
	m_nOwner					= 0;
	m_nFightPower				= 0;
	m_nGenerateMoneyType		= 0;
	m_eEquipCategory			= emKEQUIP_WEAPON_CATEGORY_ALL;
	*m_szFullName				= '\0';
	*m_szCustom					= '\0';
	m_eCustomType				= emCUSTOM_TYPE_NONE;
	memset(&m_sTimeout, 0, sizeof(m_sTimeout));
	m_dwPrice					= 0;
	m_dwCount					= 0;

#ifdef GAME_SERVER
	m_dwGenTime					= 0;
	m_byBitFlag					= 0;
	m_dwBuyPrice				= 0;

	m_nUseFailedTimes			= 0;	// added by dengyong 20100128
	m_dwLastFailedFrame			= 0;	// added by dengyong 20100128
#else
	*m_szTransparencyIcon		= '\0';
	*m_szMaskLayerIcon			= '\0';
	ZeroMemory(&m_Image, sizeof(KRUImage));
#endif
}

BOOL KItem::Bind(BOOL bForce /* = FALSE */)
{
	m_bBind = FALSE;

	if (emITEM_BIND_NONE != GetBindType() || bForce)
	{
		m_bBind = TRUE;
//#ifdef GAME_SERVER
//		KPlayer* pOwner = g_cPlayerMgr.GetByIndex(m_nOwner);
//		QCONFIRM_RET_FALSE(pOwner);
//		KPlayerItem* pPlayerItem = pOwner ? pOwner->GetCtrl<KPlayerItem>() : NULL;
//		QCONFIRM_RET_FALSE(pPlayerItem);
//		pPlayerItem->SyncItem(GetGameIndex());
//#endif
		return TRUE;
	}

	return FALSE;
}

BOOL KItem::SetBaseAttrib(CONST KMagicAttrib* pAttrib, INT nIndex /* = -1 */)
{
	if (!pAttrib)
		return FALSE;

	if (nIndex < 0)
	{
		for (INT i = 0; i < countof(m_aryBaseAttrib); i++)
			m_aryBaseAttrib[i] = pAttrib[i];
		return TRUE;
	}

	if (nIndex >= emITEM_COUNT_BASE)
		return	FALSE;

	m_aryBaseAttrib[nIndex] = *pAttrib;
	return	TRUE;
}

BOOL KItem::SetRequireAttrib(CONST KREQUIRE_ATTR* pRequire, INT nIndex /* = -1 */)
{
	if (!pRequire)
		return FALSE;

	if (nIndex < 0)
	{
		for (INT i = 0; i < countof(m_aryReqAttrib); i++)
			m_aryReqAttrib[i] = pRequire[i];
		return TRUE;
	}

	if (nIndex >= emITEM_COUNT_REQ)
		return	FALSE;

	m_aryReqAttrib[nIndex] = *pRequire;
	return	TRUE;
}

#ifdef GAME_SERVER

// 返回实际使用大小(扣除Custom无用字节)
INT KItem::MakeSyncInfo(KVIEWITEMINFO& rsViewInfo)
{
	rsViewInfo.dwID			= GetID();
	rsViewInfo.byGenre		= (BYTE)GetGenre();	
	rsViewInfo.wDetail		= (WORD)GetDetailType();
	rsViewInfo.wParticular	= (WORD)GetParticular();
	rsViewInfo.dwLevel		= (DWORD)GetLevel();
	rsViewInfo.wCount		= (WORD)GetCount();
	rsViewInfo.byEnhance	= (BYTE)GetEnhanceTimes();
	rsViewInfo.bBind		= (BYTE)IsBind();
	rsViewInfo.dwRandomSeed	= GetRandSeed();
	rsViewInfo.dwRandomSeedEx	= GetRandSeedEx();
	rsViewInfo.byTimeoutType= (BYTE)m_sTimeout.emType;
	rsViewInfo.dwTimeout	= m_sTimeout.dwTimeout;
	rsViewInfo.dwFightScore = m_nFightScore;

	rsViewInfo.byBaseAttrib = (BYTE)m_aryBaseAttrib[0].nAttribType;
	rsViewInfo.wBaseValue = (WORD)m_aryBaseAttrib[0].nValue[0];
	rsViewInfo.wBaseNextValue = (WORD)m_aryBaseAttrib[0].nValue[1];
	rsViewInfo.wEnhValue = (WORD)m_sEnhAttrib.nValue[0];
	rsViewInfo.wEnhNextValue = (WORD)m_sEnhAttrib.nValue[1];
	rsViewInfo.dwEnhanceCost = (DWORD)GetEnhanceCost();
	for(INT i = 0; i < 5; ++i)
	{
		rsViewInfo.aryAppendInfo[i].byAttrib = (BYTE)m_aryAppendAttrib[i].nAttribType;
		rsViewInfo.aryAppendInfo[i].wValue = (WORD)m_aryAppendAttrib[i].nValue[0];
	}

	for(INT i = 0; i < 5; ++i)
	{
		rsViewInfo.arySaveAppendInfo[i].byAttrib = (BYTE)m_arySaveAppendAttrib[i].nAttribType;
		rsViewInfo.arySaveAppendInfo[i].wValue = (WORD)m_arySaveAppendAttrib[i].nValue[0];
	}

	memcpy(rsViewInfo.aryAppendQuality, m_aryAppendQuality, sizeof(m_aryAppendQuality));
	memcpy(rsViewInfo.arySaveAppendQuality, m_arySaveAppendQuality, sizeof(m_arySaveAppendQuality));

	memcpy(rsViewInfo.byStoneInfo, m_aryStoneInfo, sizeof(rsViewInfo.byStoneInfo));

	INT nUseSize = sizeof(rsViewInfo);
	return nUseSize;
}

#endif

BOOL KItem::SetLevel(INT nLevel)
{
	if (KD_ITEM_NO_LEVEL_LIMIT == nLevel)
	{
		if (KD_ITEM_NO_LEVEL_LIMIT == m_sBaseData.sIndex.nLevel)
		{
			_ASSERT(FALSE);
			return	FALSE;
		}
		return	TRUE;
	}
	if (m_sBaseData.sIndex.nLevel != KD_ITEM_NO_LEVEL_LIMIT)
		return	FALSE;
	m_sBaseData.sIndex.nLevel = nLevel;
	return	TRUE;
}

BOOL KItem::SetCount(DWORD dwCount)
{
	if (dwCount < 0)
		return	FALSE;
	m_dwCurCount = dwCount;
	return	TRUE;
}

VOID KItem::SetEnhanceTimes(INT nEnhTimes)
{
	if (!KD_ISEQUIP(GetGenre()))
		return;

	ASSERT(nEnhTimes >= 0 && nEnhTimes <= MAX_ENH_LV(GetQuality()));

	m_nEnhTimes = nEnhTimes;
}

INT KItem::GetExtParam(INT nIndex) CONST
{
	if (nIndex < 0 || nIndex >= emITEM_COUNT_EXTPARAM)
		return	0;
	return m_sBaseData.aryExtParam[nIndex];
}

BOOL KItem::SetExtParam(INT nIndex, INT nValue)
{
	if (nIndex < 0 || nIndex >= emITEM_COUNT_EXTPARAM)
		return	0;

	m_sBaseData.aryExtParam[nIndex] = nValue;

	return TRUE;
}

INT KItem::GetPrice(VOID) CONST 
{
	return	(m_sBaseData.nPrice > 0) ? m_sBaseData.nPrice * GetCount() : 0;
}

INT KItem::GetRecyclePrice(VOID) CONST
{
	INT nPrice = GetPrice();
	//if (nPrice < KD_ITEM_RECYCLE_MIN_PRICE)
	//	nPrice = KD_ITEM_RECYCLE_MIN_PRICE;
	return nPrice;
}

INT KItem::GetUnitPrice() CONST
{
	return m_sBaseData.nPrice > 0 ? m_sBaseData.nPrice : 0;
}

INT KItem::GetGameIndex(VOID) CONST
{
	INT nIdx = this - Item;
	if (nIdx < 0 || nIdx >= MAX_ITEM)
		return	0;
	return	nIdx;
}

DWORD KItem::GetCDTime(VOID) CONST
{
	if (!g_cItemMgr.m_cSetting.m_mapCdType.count(m_nCDType))
		return	0UL;
	return	g_cItemMgr.m_cSetting.m_mapCdType[m_nCDType];
}

BOOL KItem::IsStackable(VOID) CONST
{
	if (m_dwMaxCount <= 1)			// 不能叠加
		return FALSE;

	if (m_sTimeout.dwTimeout)		// 计时道具
		return FALSE;

	return TRUE;
}

BOOL KItem::IsInvalid(VOID) CONST
{
	if (!KD_ISEQUIP(GetGenre()))
		return	FALSE;					// 不是装备都返回FALSE
	return m_bInvalid;
}

KE_EQUIP_WEAPON_CATEGORY KItem::GetEquipCategory(VOID) CONST
{
	if (!KD_ISEQUIP(GetGenre()))
		return emKEQUIP_WEAPON_CATEGORY_ALL;

	return m_eEquipCategory;
}

BOOL KItem::IsBindByBindType(KE_ITEM_BINDTYPE eBindType)
{
	if (eBindType == emITEM_BIND_GET || eBindType == emITEM_BIND_OWN)
		return	TRUE;		// 绑定
	return	FALSE;			// 不绑定
}

BOOL KItem::IsUseRandom(CONST KITEM_INDEX &sIndex)
{
	if (!KD_ISEQUIP(sIndex.nGenre))
		return	FALSE;

	return TRUE;
}

BOOL KItem::GetExtBagSize(INT nDetail, INT& rnWidth, INT& rnHeight)
{
	switch (nDetail)
	{
	case extbag_4cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_4CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_4CELL;
		break;
	case extbag_6cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_6CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_6CELL;
		break;
	case extbag_8cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_8CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_8CELL;
		break;
	case extbag_10cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_10CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_10CELL;
		break;
	case extbag_12cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_12CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_12CELL;
		break;
	case extbag_15cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_15CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_15CELL;
		break;
	case extbag_18cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_18CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_18CELL;
		break;
	case extbag_20cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_20CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_20CELL;
		break;
	case extbag_24cell:
		rnWidth  = KD_ROOM_EXTBAG_WIDTH_24CELL;
		rnHeight = KD_ROOM_EXTBAG_HEIGHT_24CELL;
		break;
	default:
		return	FALSE;
	}

	return	TRUE;
}

CONST KMagicAttrib* KItem::GetBaseAttrib(INT i) CONST
{
	if (i < 0 || i > emITEM_COUNT_BASE)
		return	NULL;
	return	m_aryBaseAttrib + i;
}

CONST KMagicAttrib* KItem::GetEnhAttribArr() CONST
{
	return &m_sEnhAttrib;
}

CONST KItem::KREQUIRE_ATTR* KItem::GetReqAttrib(INT i) CONST
{
	if (i < 0 || i > emITEM_COUNT_REQ)
		return	NULL;
	return	m_aryReqAttrib + i;
}

BOOL KItem::AddCount(INT nCount)
{
	ASSERT(nCount > 0);
	if (GetMaxCount() <= 1)
		return	FALSE;
	DWORD dwCurCount = m_dwCurCount + nCount;
	if (dwCurCount > m_dwMaxCount || dwCurCount < 0)
		return	FALSE;
	m_dwCurCount = dwCurCount;
	return	TRUE;
}

BOOL KItem::SubCount(INT nCount)
{
	ASSERT(nCount > 0);
	if (GetMaxCount() <= 1)
		return	FALSE;
	DWORD dwCurCount = m_dwCurCount - nCount;
	if (dwCurCount < 0)
		return	FALSE;
	m_dwCurCount = dwCurCount;
	return	TRUE;
}

BOOL KItem::CanStackWith(CONST KItem& rcItem) CONST
{	// 注意：这里的逻辑不能随便改动！在判断背包是否能放得下东西的叠加逻辑和此处需保持一致（参看KPlayerItem::KHOLD_ITEM和KPlayerItem::CanAddItemIntoBag()）
	return	IsStackable()	&& rcItem.IsStackable()		&&
			GetGenre()		== rcItem.GetGenre()		&&
			GetDetailType()	== rcItem.GetDetailType()	&&
			GetParticular()	== rcItem.GetParticular()	&&
			IsBind()		== rcItem.IsBind();			
}

BOOL KItem::GetExtBagSize(INT& rnWidth, INT& rnHeight) CONST
{
	if (GetGenre() != item_extbag)
		return	FALSE;
	return	GetExtBagSize(GetDetailType(), rnWidth, rnHeight);
}

KE_MONEY_TYPE KItem::GetSellMoneyType(VOID) CONST
{
	KE_MONEY_TYPE eRet = emNORMAL_MONEY;
	 //KE_MONEY_TYPE eRet = (GetGenre() == item_equip_purple ? emNORMAL_MONEY : emBIND_MONEY);
	 //if (IsBind() == TRUE)
		// eRet = emBIND_MONEY;
	 return eRet;
}

BOOL KItem::IsDupedItem()
{
	// UNDONE: Fanghao_Wu 使用别的变量存复制物品参数，恢复该功能
	return FALSE;
}

#ifdef GAME_SERVER

BOOL KItem::ForBitForDupedItem()
{
	return FALSE;
	
	DupedDef::FORBIT_LEVEL level = g_dupedItemSetting.GetForbitLevel();
	if (DupedDef::forbit_none == level)
		return FALSE;

	if (DupedDef::forbit_for_all == level && IsDupedItem())
	{
		return TRUE;
	}

	// UNDONE: Fanghao_Wu 使用别的变量存复制物品参数，恢复该功能
	/*
	BYTE flag = GetExParam(1);
	if (DupedDef::forbit_for_inworld == level && DupedDef::mark_dupe_free == flag)
		return TRUE;
	
	if (DupedDef::forbit_for_inown == level && DupedDef::mark_dupe_limit == flag)
		return TRUE;
	*/
	
	return FALSE;
}

#else

#endif // #ifndef GAME_SERVER

#ifdef GAME_SERVER
VOID KItem::SetGuid(CONST GUID* pGuid)					
{
	ASSERT(pGuid); 
	m_cGuid.SetGuid(pGuid);
	INT64* pnItemGUID = (INT64*)pGuid;
	if (IB_IS_IB_ITEM(pnItemGUID[0]) && 0 == pnItemGUID[1])
	{
		m_nItemGUID = pnItemGUID[0];
	}
	else
	{
		m_nItemGUID = 0;
	}
}

VOID KItem::SetItemGUID(CONST INT64 nGUID)
{
	m_nItemGUID = nGUID;
	GUID sGUID;
	INT64* pnItemGUID = (INT64*)&sGUID;
	pnItemGUID[0] = nGUID;
	pnItemGUID[1] = 0;

	m_cGuid.SetGuid(&sGUID);
}

#endif // #ifdef GAME_SERVER
