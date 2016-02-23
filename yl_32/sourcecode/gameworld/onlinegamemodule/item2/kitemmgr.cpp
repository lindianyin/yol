
#include "stdafx.h"
#include "kitemmgr.h"
#include <map>
#include <string>
#include <fstream>
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayermgr.h"
#include "kitemprotocolprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#ifdef GAME_SERVER
#define  KD_STR_DATARECORD_FILENAME		"\\setting\\misc\\datarecord.txt"
#endif
// -------------------------------------------------------------------------

extern QLogFile	g_SysAlarmLogFile;

KItemMgr g_cItemMgr;
QLogFile g_ItemLogFile;


template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_ITEM>()
{
	return &g_cItemMgr;
}

CONST KPlayerItem::KE_LIMIT_PRIORITY g_aeLimitPri[] =
{
	(KPlayerItem::KE_LIMIT_PRIORITY)NULL,
	KPlayerItem::emKLIMIT_PRIORITY_forbid,
	KPlayerItem::emKLIMIT_PRIORITY_proper,
};

// -------------------------------------------------------------------------

KItemMgr::KItemMgr() : KModuleBase(emKMODULE_ITEM)
{
	m_pItemGen = NULL;
	m_dwIDCreator = 100;
#ifdef GAME_SERVER
	m_nLastIdentity = 0;
#endif
}

BOOL KItemMgr::OnInit()
{
	Item = new KItem[MAX_ITEM];	
	QCONFIRM_RET_FALSE(Item);

	// 初始化切换处理函数表
	KSwitchItem::InitProc();

	extern BOOL gInitSysAlarmDeal();
	gInitSysAlarmDeal();

	if (!m_cSetting.Init())
		return FALSE;

	if (!m_cIdxList.Init(MAX_ITEM))
		return FALSE;

#ifdef GAME_SERVER
	g_ItemLogFile.InitialLogFile(F_LOG_PATH_ROOT, F_ItemSet_0);
	m_dupedItemDeal.SetMarkFlag(DupedDef::mark_dupe_limit);	// 与游戏世界中的物品有雷同复制)的标记
#endif

	m_pItemGen = new KItemGenerator;
	m_pItemGen->Init(1);

#ifdef GAME_SERVER
	QTabFile cFile;
	if (!cFile.Load(KD_STR_DATARECORD_FILENAME))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	for (INT i = 0; i < cFile.GetHeight() - 1; ++i)
	{
		INT nRow = i + 2;
		KDATARECORD* pRecord = new KDATARECORD;
		INT nG, nD, nP = 0;

		cFile.GetInteger(nRow, "G", 0, &nG);
		cFile.GetInteger(nRow, "D", 0, &nD);
		cFile.GetInteger(nRow, "P", 0, &nP);
		::sprintf(pRecord->szKey, "%d_%d_%d", nG, nD, nP);
	
		cFile.GetInteger(nRow, "bConsume", 0, &pRecord->bConsume);
		cFile.GetInteger(nRow, "bAdd", 0, &pRecord->bAdd);

		m_mpDataRecordList.insert(std::map<std::string, KDATARECORD*>::value_type(pRecord->szKey, pRecord));
	}
#endif

	for (INT i = 0; i < KPlayerItem::emKLIMITUSE_COUNT; ++i)
	{
		if (g_aeLimitPri[i])
		{
			KPlayerItem::vecLimitUseRule vecRule;
			if (m_mpLimitPri.find(g_aeLimitPri[i]) != m_mpLimitPri.end())
				vecRule = m_mpLimitPri.find(g_aeLimitPri[i])->second;

			vecRule.push_back((KPlayerItem::KE_LIMITUSE_TYPE)i);
			m_mpLimitPri[g_aeLimitPri[i]] = vecRule;
		}
	}

	return TRUE;
}

VOID KItemMgr::RegistLimitUse(LPCSTR szForbidType, KPlayerItem::KE_LIMITUSE_TYPE eType, LPCSTR szMap, INT bRegister)
{
	if (bRegister)  // 是注册
	{	
		KMP_LIMITUSE mpLimitUse;
		KMP_DATA mpData;

		if (m_mpLimitUseData.find(szForbidType) != m_mpLimitUseData.end())
		{
			mpLimitUse = m_mpLimitUseData.find(szForbidType)->second;
			if (mpLimitUse.find(eType) == mpLimitUse.end())
			{
				mpData[szMap] = bRegister;
				mpLimitUse.insert(KMP_LIMITUSE::value_type(eType, mpData));
			}
			else
			{
				KMP_DATA* pMpData = &(mpLimitUse.find(eType)->second);
				pMpData->insert(KMP_DATA::value_type(szMap, bRegister));	
			}
		}
		else
		{
			mpData.insert(KMP_DATA::value_type(szMap, bRegister));
			mpLimitUse.insert(KMP_LIMITUSE::value_type(eType, mpData));
		}		

		m_mpLimitUseData[szForbidType] = mpLimitUse;
	}
	else	// 是反注册
	{
		if (m_mpLimitUseData.find(szForbidType) == m_mpLimitUseData.end())
			return;

		KMP_LIMITUSE* pMpLimitUse = &(m_mpLimitUseData.find(szForbidType)->second);
		if (pMpLimitUse->find(eType) == pMpLimitUse->end())
			return;

		KMP_DATA* pMpData = &(pMpLimitUse->find(eType)->second);
		if (pMpData->find(szMap) == pMpData->end())
			return;

		pMpData->erase(pMpData->find(szMap));
	}
}

BOOL KItemMgr::OnUnInit()
{
	RemoveAll();
#ifdef GAME_SERVER
	for (std::map<std::string, KDATARECORD*>::iterator it = m_mpDataRecordList.begin(); it != m_mpDataRecordList.end(); ++it)
		SAFE_DELETE(it->second);
#endif

	SAFE_DELETE(m_pItemGen);
	SAFE_DELETE_ARRAY(Item);
	return TRUE;
}

INT KItemMgr::SearchID(DWORD dwID) CONST
{
	if (!dwID)
		return 0;

	INT nIdx = 0;

	std::map<DWORD, INT>::const_iterator it = m_mpIdToIdx.find(dwID);
	if (it != m_mpIdToIdx.end())
		nIdx = it->second;

	return nIdx;
}

KItemGenerator* KItemMgr::GetGenerator(INT nVersion /* = 0 */) CONST
{
	return m_pItemGen;
}

INT KItemMgr::Add(KItem* pcItem)
{
	_ASSERT(pcItem);
	INT i = m_cIdxList.FindFree();
	if (!i){
		assert(false);
		return 0;
	}

	Item[i].CopyItemData(pcItem);
#ifdef GAME_SERVER
	SetID(i);
	pcItem->SetIdentity(++m_nLastIdentity);
#endif // #ifdef GAME_SERVER
	m_cIdxList.Use(i);
	
	return	i;
}

INT KItemMgr::Add(INT						nItemGenre,
				  INT						nDetailType,
				  INT						nParticularType,
				  INT						nLevel,
				  INT						nEnhTimes,
				  KE_ITEM_DATARECORD_WAY	eWay,
				  CONST KItem::KSTONE_INFO* pStoneInfo,		// = NULL
				  CONST KMagicAttrib*		pAppendAttr,	// = NULL
				  UINT						uRandomSeed,	// = 0
				  UINT						uRandomSeedEx,	// = 0
				  IItemEnv*					pEnv)			// = NULL
{
	KPARAM sParam;
	ZeroStruct(sParam);
	sParam.nGenre		= nItemGenre;
	sParam.nDetail		= nDetailType;
	sParam.nParticular	= nParticularType;
	sParam.nLevel		= nLevel;
	sParam.nEnhTimes	= nEnhTimes;
	sParam.uRandSeed	= uRandomSeed;
	sParam.uRandSeedEx	= uRandomSeedEx;

	INT nIndex = Add_Impl(sParam, pStoneInfo, pAppendAttr);
	if (nIndex <= 0)
		return 0;
#ifdef GAME_SERVER
	KItem *pItem = &Item[nIndex];
	if (pEnv && CanLogThisItem(pItem))
	{
		g_ItemLogFile.LogRecordVar(
			LOG_LEVEL_INFO,
			"AddItem[%s] Genre:%d, EnhTimes:%d"
			"DetailType:%d, ParticularType:%d, RandomSeed:%d, RandomSeedEx:%d, GenTime:%u, Env:%s",
			pItem->GetOrgName(),
			nItemGenre,
			nEnhTimes,
			nDetailType,
			nParticularType,
			uRandomSeed,
			uRandomSeedEx,
			pItem->GetGenTime(),
			pEnv->GetEnvMsg()
		);
	}

#endif //_SERVER
	return nIndex;
}

INT	KItemMgr::Add_Impl(CONST KPARAM& rsParam,
					   CONST KItem::KSTONE_INFO* pStoneInfo,
					   CONST KMagicAttrib* pAppendAttr,
					   INT nExtParamMask /* = 0 */)
{
	INT i = m_cIdxList.FindFree();
	if (!i){
		assert(false);
		return	0;
	}

	KItem *pItem = &Item[i];

	KItemGenerator* pItemGen = GetGenerator();
	ASSERT(pItemGen);

	BOOL bOK = pItemGen->Gen_Item(
		pItem,
		rsParam.nGenre,
		rsParam.nDetail,
		rsParam.nParticular,
		rsParam.nLevel,
		rsParam.nEnhTimes,
		pStoneInfo,
		pAppendAttr,
		NULL,
		rsParam.uRandSeed,
		rsParam.uRandSeedEx,
		TRUE,
		FALSE,
		NULL
	);

	if (!bOK)
		return	0;

#ifdef GAME_SERVER
	SetID(i);
	pItem->SetIdentity(++m_nLastIdentity);
#endif // #ifdef GAME_SERVER

	m_cIdxList.Use(i);
	return	i;
}

BOOL KItemMgr::Remove(INT nIdx, 
					  KE_ITEM_DATARECORD_WAY eRecord /*= emITEM_DATARECORD_NORMAL*/,
					  KE_ITEM_CONSUMEMODE eConsumeMode/* = emITEM_CONSUMEMODE_NORMAL*/, 
					  KPlayer* pPlayer/* = NULL*/, 
					  BOOL bEraseInMap/* = TRUE*/)
{
	if (nIdx <= 0 || nIdx >= MAX_ITEM)
		return FALSE;

#ifdef GAME_SERVER
	m_dupedItemDeal.Remove(nIdx, bEraseInMap);	// 复制装备检查
	//if (pPlayer)
	//	g_cIBItemHelper.OnItemRemove(Item[nIdx], pPlayer, eConsumeMode);

#endif // #ifdef GAME_SERVER
	std::map<DWORD, INT>::iterator it = m_mpIdToIdx.find(Item[nIdx].GetID());
	if (it == m_mpIdToIdx.end())
	{
		ASSERT(FALSE);
	}
	else
	{
		m_mpIdToIdx.erase(it);
	}

	Item[nIdx].Remove();
	m_cIdxList.Free(nIdx);
	return TRUE;
}

VOID KItemMgr::RemoveAll(VOID)
{
	if (m_cIdxList.GetCount() > 0)
	{
#ifdef GAME_SERVER
		g_ItemLogFile.LogRecordVar(LOG_LEVEL_WARNING, "Item Leak! Total %d:", GetItemCount());
#endif
		ASSERT(FALSE);
	}

	INT nIdx = 0;

	for (INT nLogCount = 0; nLogCount < 100; ++nLogCount)	// 最多输出100个
	{
		nIdx = m_cIdxList.FindNext(0);
		if (!nIdx)
			break;

#ifdef GAME_SERVER
		m_dupedItemDeal.Remove(nIdx, TRUE);	// 复制装备检查
		// TODO: xyf 检查服务端Item泄露而加的临时日志
		time_t nGenTime = (time_t)Item[nIdx].GetGenTime();
		tm* psTime = localtime(&nGenTime);
		g_ItemLogFile.LogRecordVar(
			LOG_LEVEL_WARNING,
			"Leak : %s(%d,%d,%d) Index(%d) ID(%u) Owner(%d) CreateTime(%04d/%02d/%02d %02d:%02d%:%02d)",
			Item[nIdx].GetOrgName(),
			Item[nIdx].GetGenre(),
			Item[nIdx].GetDetailType(),
			Item[nIdx].GetParticular(),
			nIdx,
			Item[nIdx].GetID(),
			Item[nIdx].GetBelong(),
			psTime->tm_year + 1900,
			psTime->tm_mon + 1,
			psTime->tm_mday,
			psTime->tm_hour,
			psTime->tm_min,
			psTime->tm_sec
		);
#endif	// #ifdef GAME_SERVER
		std::map<DWORD, INT>::iterator it = m_mpIdToIdx.find(Item[nIdx].GetID());
		if (it == m_mpIdToIdx.end())
		{
			ASSERT(FALSE);
		}
		else
		{
			m_mpIdToIdx.erase(it);
		}

		Item[nIdx].Remove();
		m_cIdxList.Free(nIdx);
	}
}

VOID KItemMgr::SetID(INT nIdx)
{
	Item[nIdx].SetID(m_dwIDCreator);
	// 同步修改ID到索引的map表
	m_mpIdToIdx.insert(std::map<DWORD, INT>::value_type(Item[nIdx].GetID(), nIdx));
	m_dwIDCreator++;
}

#ifdef GAME_SERVER

/*
参数：bCheckDuped - 是否检查复制装备
	DUPED_OP_MARK& - 标记操作类型
返回值：> 0		正确加入物品，
		= -1	添加物品失败(正常流程)
		= -2	添加物品失败(非法复制物品标记)
*/
INT KItemMgr::Add(CONST KItem*			pcItem,
				  INT					nPlayerIndex,
				  DupedDef::OP_LEVEL	dupedOpLevel, 
				  DupedDef::OP_RESULT&	dupedRet, 
				  IItemEnv*				pcEnv)		// = NULL
{
	INT nIndex = Add_Impl(pcItem, nPlayerIndex, dupedOpLevel, dupedRet);
	if (nIndex <= 0)
		return	0;

	KItem *pcClone = &Item[nIndex];
	if (pcEnv && CanLogThisItem(pcClone))
	{
		g_ItemLogFile.LogRecordVar(
			LOG_LEVEL_INFO, 
			"CloneItem[%s] Genre:%d, DetailType:%d, ParticularType:%d, RandomSeed:%d, GenTime:%u, Env:%s",
			pcClone->GetOrgName(),
			pcClone->GetGenre(),
			pcClone->GetDetailType(),
			pcClone->GetParticular(),
			pcClone->GetRandSeed(),
			pcClone->GetGenTime(),
			pcEnv->GetEnvMsg()
		);
	}

	return	nIndex;
}

INT KItemMgr::Add_Impl(CONST KItem*			pcItem, 
					   INT					nPlayerIndex,
					   DupedDef::OP_LEVEL	dupedOpLevel, 
					   DupedDef::OP_RESULT&	dupedRet)
{
	_ASSERT(pcItem);
	INT i = m_cIdxList.FindFree();
	if (!i){
		assert(false);
		return 0;
	}

	Item[i].CopyItemData(pcItem);

#ifdef GAME_SERVER
	SetID(i);
	Item[i].SetIdentity(++m_nLastIdentity);
#endif

	m_cIdxList.Use(i);
	return	i;
}

BOOL KItemMgr::CanLogThisItem(KItem* pcItem)
{
	ASSERT(pcItem);
	return TRUE;
}

BOOL KItemMgr::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	return KItemProtocol::ProcessProtocol(nPlayerIndex, pbData, uDataLen);
}

BOOL KItemMgr::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KPLAYER_USEITEM);
	KD_PTC_FIXED_SIZE_C2S(KPLAYER_RESIZE_EXTBAG);
	KD_PTC_LENGTNED_SIZE_C2S(KITEM_REPAIR_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_SPLIT_ITEM);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_SWITCH_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_ARRANGE_BAG);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_ENHANCE_APPLY_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_UPGRADE_APPLY_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_REFINE_APPLY_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_SAVE_REFINE_ATTR);
	KD_PTC_LENGTNED_SIZE_C2S(KITEM_C2S_STONE_MIX_SYNC);
	KD_PTC_LENGTNED_SIZE_C2S(KITEM_C2S_SELL_ITEMS);
	KD_PTC_LENGTNED_SIZE_C2S(KITEM_C2S_STONE_MOUNT_SYNC);
	KD_PTC_LENGTNED_SIZE_C2S(KITEM_C2S_STONE_REMOVE_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_STONE_PUNCH);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_EQUIP_RERAND_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KITEM_C2S_EQUIP_IDENTIFY);
	KD_PTC_LENGTNED_SIZE_C2S(KSWITCH_EQUIP_PROTOCOL);
	KD_PTC_FIXED_SIZE_C2S(KITEM_THROWAWAY_SYNC);
	KD_PTC_FIXED_SIZE_C2S(C2S_CLOSE_REPOSITORY);
	KD_PTC_FIXED_SIZE_C2S(C2S_REAPPLY_PENDANTS_REQUEST);

	KD_PTC_FIXED_SIZE_S2C(KPLAYER_USEITEM_RESULT);
	KD_PTC_FIXED_SIZE_S2C(KSYNC_PREPARE_ITEMREPAIR);
	KD_PTC_LENGTNED_SIZE_S2C(KITEM_SYNC);
	KD_PTC_FIXED_SIZE_S2C(KITEM_S2C_SWITCH_SYNC);
	KD_PTC_FIXED_SIZE_S2C(KITEM_REMOVE_SYNC);
	KD_PTC_FIXED_SIZE_S2C(KITEM_COUNT_SYNC);
	KD_PTC_FIXED_SIZE_S2C(KITEM_S2C_ENHANCE_RESULT_SYNC);
	KD_PTC_FIXED_SIZE_S2C(KITEM_S2C_SWITCH_END);
	KD_PTC_FIXED_SIZE_S2C(KITEM_S2C_ARRANGE_END);
	KD_PTC_FIXED_SIZE_S2C(KITEM_S2C_SELLITEMS_END);
	KD_PTC_FIXED_SIZE_S2C(S2C_OPEN_REPOSITORY);
	KD_PTC_FIXED_SIZE_S2C(S2C_SYNC_EXT_REPO_STATE);
	KD_PTC_LENGTNED_SIZE_S2C(KVIEW_EQUIP_SYNC);

	return TRUE;
}

#endif	// #ifdef GAME_SERVER #else

// -------------------------------------------------------------------------
