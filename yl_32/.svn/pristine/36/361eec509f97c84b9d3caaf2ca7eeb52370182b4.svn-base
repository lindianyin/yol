
//	功能描述	：	Item的相应指令，不针对单个已知Item


#include "stdafx.h"
#include "kmagicattribmanager.h"
#include "kitemmgr.h"
#include "kluaitem.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegamemodule/chat/kchatmodule.h"

#ifdef GAME_SERVER
#include "itemenv.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#ifdef GAME_SERVER
extern INT g_nUseFailedFrameInterval;
extern INT g_nUseFailedLimitTime;
#endif	// #ifndef GAME_SERVER

// -------------------------------------------------------------------------

#ifndef GAME_SERVER
/*
	{
		szName		= 套装名称（客户端）,
		tbPos		= （客户端）
		{
			[Item.EQUIPPOS_HEAD] =
			{
				szName		= 头部装备名称,
				nHoldStatus = 持有情况,
			}
			[Item.EQUIPPOS_BODY] = 
			{
				...
			}
			....
		},
		nCount		= 套装件数,
		nEquip		= 已装备件数,
		tbSuiteMA	=
		{
			[1]	=
			{
				szName	= 魔法属性名称,
				tbValue	=
				{
					[1] = 魔法属性参数1,
					[2] = 魔法属性参数2,
					[3] = 魔法属性参数3,
				},
				bActive	= 是否被激活,
				nNum    = 激活所需件数,
			},
			[2]	=
			{
				...
			},
			...
			[4] =
			{
				...
			},
		},
	}
*/

template<typename KBPT_SuiteEquip>
static INT LuaGetSuiteAttrib(QLuaScript							&sc,
							 INT								nSuiteId,
							 KPlayerItem						*pPlayerItem,
							 CONST KSUITE_INFO					*pInfo,
							 CONST KPlayerItem::KMassSuiteVec	&vMASS,
							 CONST KBPT_SuiteEquip				&cBPT)
{
	if (!pInfo)
		return	0;

	KMASS_Suite *pMASS = NULL;
	KPlayerItem::KMassSuiteVec::const_iterator it = vMASS.begin();
	for (; it < vMASS.end(); ++it)
	{
		if ((*it)->GetSuiteID() == nSuiteId)
		{
			pMASS = *it;
			break;
		}
	}
	// pMASS == NULL表示装备没装在身上

	INT nEquipNum = 0;
	sc.PushTable();
	sc.PushString(pInfo->pInfo->szName);
	sc.SetTableField("szName");			// 套装名称
	sc.PushTable();
	for (INT i = 0; i < emEQUIPPOS_NUM; ++i)
	{
		KPlayerItem::KE_HOLDSTATUS eHoldStatus = KPlayerItem::emHOLDSTATUS_NONE;
		CONST std::vector<KITEM_INDEX>* pIdxs = &pInfo->aryPart[i];

		std::vector<KITEM_INDEX>::const_iterator it = pIdxs->begin();
		for (; it != pIdxs->end(); it++)
		{
			CONST KBASICPROP_EQUIPMENT *pEq = cBPT.GetData(*it);
			if (!pEq)
			{
				_ASSERT(FALSE);
				continue;
			}

			KPlayerItem::KE_HOLDSTATUS eHS = pPlayerItem->CheckItemHoldStatus(*it);
			if (eHoldStatus < eHS)
			{
				eHoldStatus = eHS;
				if (eHoldStatus == KPlayerItem::emHOLDSTATUS_EQUIP)
					++nEquipNum;
			}
		}

		sc.PushTable();
		sc.PushString(pInfo->pInfo->aryName[i]);
		sc.SetTableField("szName");
		sc.PushNumber(eHoldStatus);
		sc.SetTableField("nHoldStatus");// 注意这里的索引值是ITEM_PART枚举
		sc.SetTableIndex(i);
	}
	sc.SetTableField("tbPos");			// 各件名称
	sc.PushNumber(pInfo->nCount);
	sc.SetTableField("nCount");			// 套装总件数
	sc.PushNumber(nEquipNum);
	sc.SetTableField("nEquip");			// 已装备件数
	sc.PushTable();
	for (INT i = 0; i < emITEM_COUNT_SUITE; ++i)
	{
		KMagicAttrib sMA;
		BOOL bActive = FALSE;

		if (pMASS)
			bActive = pMASS->IsActive(i);

		sMA.nAttribType = pInfo->pInfo->arySuite[i].nAttrib;
		for (INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
			sMA.nValue[j] = pInfo->pInfo->arySuite[i].aryValue[j];

		sc.PushTable();
		sc.PushString(g_cMagicAttribMgr.GetStringFromId(sMA.nAttribType));
		sc.SetTableField("szName");
		sc.PushTable();
		for (INT j = 1; j <= KD_MAGIC_VALUE_NUM; ++j)
		{
			sc.PushNumber(sMA.nValue[j - 1]);
			sc.SetTableIndex(j);
		}
		sc.SetTableField("tbValue");
		sc.PushNumber(bActive);
		sc.SetTableField("bActive");
		sc.PushNumber(pInfo->pInfo->arySuite[i].nNum);
		sc.SetTableField("nNum");
		sc.SetTableIndex(i + 1);
	}
	sc.SetTableField("tbSuiteMA");			// 套装激活属性相关

	return	1;
}
#endif

// -------------------------------------------------------------------------

namespace KItemScriptNameSpace
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetItemObj
	// @Description		: 根据道具索引，得到道具对象
	// @ReturnCode		: 返回道具对象，失败返回nil
	// @ArgumentFlag	: d
	// @ArgumentComment	: nIdx:道具索引
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetItemObj(QLuaScript& sc)
	{
		INT nIdx = sc.GetInt(1);
		if (nIdx <= 0 || nIdx >= MAX_ITEM)
			return	0;
		sc.PushObj(Item[nIdx].m_pLuna);
		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetItemCount
	// @Description		: 获得道具总数
	// @ReturnCode		: 道具总数
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetItemCount(QLuaScript& sc)
	{
		return sc.PushNumber(g_cItemMgr.GetItemCount());
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetNameById
	// @Description		: 根据道具ID，得到道具名称
	// @ReturnCode		: 成功返回Item名称，失败返回空
	// @ArgumentFlag	: dddd
	// @ArgumentComment	: nGenre:主类型
	// @ArgumentComment	: nDetailType:详细类别
	// @ArgumentComment	: nParticular:具体类别
	// @ArgumentComment	: nLevel:级别
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetNameById(QLuaScript& sc)
	{
		KITEM_INDEX sIndex;
		sc.ReadList(1, "dddd", &sIndex.nGenre, &sIndex.nDetailType, &sIndex.nParticular, &sIndex.nLevel);
		if (sIndex.nLevel < 0)
			sIndex.nLevel = 0;

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
		CONST KBASICPROP_BASEDATA* pBaseData = pLibBPT->GetBasicProp(sIndex);
		if (!pBaseData)
			return	0;

		sc.PushString(pBaseData->szName);
		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetMagicValue
	// @Description		: 获得指定魔法属性的原始价值量
	// @ReturnCode		: 价值量，出错看作0
	// @ArgumentFlag	: sd
	// @ArgumentComment	: szAttrib 魔法属性名
	// @ArgumentComment	: nLevel 魔法属性等级
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMagicValue(QLuaScript& sc)
	{
		INT nValue = 0;
		INT nAttrib = g_cMagicAttribMgr.GetIdFormString(sc.GetStr(1));

		if (nAttrib > 0)
		{
			CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
			CONST KLibOfBPT *pLibBPT = pItemGen->GetLibOfBPT();
			KMAGIC_INDEX sIndex = { nAttrib, sc.GetInt(2) };
			CONST KBASICPROP_MAGICATTRIB *pMagic = pLibBPT->m_cMagicAttrib.GetData(sIndex);
			if (pMagic)
				nValue = pMagic->nValue;
		}

		sc.PushNumber(nValue);
		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: CanPlayerUseItem
	// @Description		: 检查指定角色是否可以使用/装备指定道具
	// @ReturnCode		: 1可以使用
	// @ArgumentFlag	: oo
	// @ArgumentComment	: pPlayer:指定角色
	// @ArgumentComment	: pItem:指定道具
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaCanPlayerUseItem(QLuaScript& sc)
	{
		KLuaPlayer* pLuaPlayer = dynamic_cast<KLuaPlayer*>(sc.GetObj(1));
		KLuaItem* pLuaItem = dynamic_cast<KLuaItem*>(sc.GetObj(2));
		if (!pLuaPlayer || !pLuaItem)
			return 0;

		KItem* pItem = pLuaItem->GetIt();
		QCONFIRM_RET_FALSE(pItem);
		KPlayer* pPlayer = pLuaPlayer->GetHim();
		QCONFIRM_RET_FALSE(pPlayer);
		KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
		QCONFIRM_RET_FALSE(pPlayerItem);

		sc.PushNumber(pPlayerItem->CanUseItem(*pItem));
		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: CheckLimitUse
	// @Description		: 检查指定角色在当前地图是否受限使用（CanPlayerUseItem是检查装备的）
	// @ReturnCode		: 1可以使用
	// @ArgumentFlag	: ds[d]
	// @ArgumentComment	: nMapId:地图ID
	// @ArgumentComment	: szForbitType:禁用类型
	// @ArgumentComment	: bChat:是否发送失败信息给角色
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaCheckLimitUse(QLuaScript& sc)
	{
		DWORD dwMapId = (DWORD)sc.GetInt(1);
		LPCSTR szForbitType = sc.GetStr(2);
		BOOL bChat = 0;
		if (sc.GetTopIndex() >= 3)
			bChat = sc.GetInt(3);

		CHAR szRet[128] = {0};

		sc.PushNumber(KPlayerItem::CheckUseLimited(szForbitType, dwMapId, szRet));
		//if (bChat && szRet[0])
		//{
//#ifndef GAME_SERVER
//			::g_CoreEventNotify(emCOREEVENT_SYSTEM_MESSAGE, "s", szRet);
//#endif
		//}

		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: EquipType2EquipPos
	// @Description		: 从装备类型获得装备位置
	// @ReturnCode		: 成功返回装备位置，失败返回-1
	// @ArgumentFlag	: d
	// @ArgumentComment	: nType:装备类型
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaEquipType2EquipPos(QLuaScript& sc)
	{
		INT nType = sc.GetInt(1);
		sc.PushNumber(KPlayerItem::GetEquipPos(nType));
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetItemBaseProp
	// @Description		: 获得道具基础信息
	// @ReturnCode		: 返回基础信息表，失败返回nil
	// @ArgumentFlag	: dddd
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetItemBaseProp(QLuaScript& sc)
	{
		KITEM_INDEX sIndex;
		sIndex.nGenre		= sc.GetInt(1);
		sIndex.nDetailType	= sc.GetInt(2);
		sIndex.nParticular	= sc.GetInt(3);
		sIndex.nLevel		= sc.GetInt(4);

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
		CONST KBASICPROP_BASEDATA* pBPTBase = (CONST KBASICPROP_BASEDATA*)pLibBPT->GetBasicProp(sIndex);
		if (!pBPTBase)
			return	0;

		sc.PushTable();
		sc.PushNumber(pBPTBase->nItemGenre);
		sc.SetTableField("nGenre");
		sc.PushNumber(pBPTBase->nDetailType);
		sc.SetTableField("nDetail");
		sc.PushNumber(pBPTBase->nParticular);
		sc.SetTableField("nParticular");
		sc.PushNumber(pBPTBase->nLevel);
		sc.SetTableField("nLevel");
		sc.PushNumber(pBPTBase->nBindType);
		sc.SetTableField("nBindType");
		sc.PushNumber(pBPTBase->nPrice);
		sc.SetTableField("nPrice");
		sc.PushNumber(pBPTBase->bValuable);
		sc.SetTableField("bValuable");
		sc.PushString(pBPTBase->szName);
		sc.SetTableField("szName");
		sc.PushString(pBPTBase->szClass);
		sc.SetTableField("szClass");
		sc.PushString(pBPTBase->szKind);
		sc.SetTableField("szKind");
#ifndef GAME_SERVER
		sc.PushString(pBPTBase->szIconImage);
		sc.SetTableField("szIconImage");
		sc.PushString(pBPTBase->szViewImage);
		sc.SetTableField("szViewImage");
		sc.PushString(pBPTBase->szIntro);
		sc.SetTableField("szIntro");
		sc.PushString(pBPTBase->szHelp);
		sc.SetTableField("szHelp");
#endif

		sc.PushTable();
		for (INT i = 0; i < emITEM_COUNT_EXTPARAM; ++i)
		{
			sc.PushNumber(pBPTBase->aryExtParam[i]);
			sc.SetTableIndex(i + 1);
		}
		sc.SetTableField("tbExtParam");

		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetEquipBaseProp
	// @Description		: 获得装备基础属性信息
	// @ReturnCode		: 返回装备基础属性信息表，出错返回nil
	// @ArgumentFlag	: dddd
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetEquipBaseProp(QLuaScript& sc)
	{
		KITEM_INDEX sIndex;
		sIndex.nGenre		= sc.GetInt(1);
		sIndex.nDetailType	= sc.GetInt(2);
		sIndex.nParticular	= sc.GetInt(3);
		sIndex.nLevel		= sc.GetInt(4);

		if (!KD_ISEQUIP(sIndex.nGenre))
			return	0;

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
		CONST KBASICPROP_EQUIPMENT* pBPTEquip = (CONST KBASICPROP_EQUIPMENT*)pLibBPT->GetBasicProp(sIndex);
		if (!pBPTEquip)
			return	0;

		CONST KBPT_EQUIP_BASEATTR* pBaseAttrib = pBPTEquip->aryBasic;

		sc.PushTable();
		sc.PushNumber(pBPTEquip->nPandent1);
		sc.SetTableField("Pandent1ID");
		sc.PushNumber(pBPTEquip->nPandent2);
		sc.SetTableField("Pandent2ID");
		sc.PushNumber(pBPTEquip->nCategory);
		sc.SetTableField("nCategory");

		sc.PushTable();
		for (INT i = 0; i < emITEM_COUNT_BASE; ++i)
		{
			CONST KBPT_EQUIP_BASEATTR* pBaseAttr = &pBPTEquip->aryBasic[i];
			sc.PushTable();
			sc.PushString(g_cMagicAttribMgr.GetStringFromId(pBaseAttr->nAttrib));
			sc.SetTableField("szName");
			sc.PushTable();
			for (INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
			{
				sc.PushTable();
				sc.PushNumber(pBaseAttr->aryRange[j].nMin);
				sc.SetTableField("nMin");
				sc.PushNumber(pBaseAttr->aryRange[j].nMax);
				sc.SetTableField("nMax");
				sc.SetTableIndex(j + 1);
			}
			sc.SetTableField("tbRange");
			sc.SetTableIndex(i + 1);
		}
		sc.SetTableField("tbBaseAttrib");

		sc.PushTable();
		for (INT i = 0; i < emITEM_COUNT_REQ; ++i)
		{
			CONST KBPT_EQUIP_REQATTR* pReqAttr = &pBPTEquip->aryReq[i];
			sc.PushTable();
			sc.PushNumber(pReqAttr->eType);
			sc.SetTableField("nType");
			sc.PushNumber(pReqAttr->nValue);
			sc.SetTableField("nValue");
			sc.SetTableIndex(i + 1);
		}
		sc.SetTableField("tbReqAttrib");

		if (!KD_ISSUITE(sIndex.nGenre))
			return	1;

		CONST KBASICPROP_EQUIP_SUITE* pBPTSuite = (CONST KBASICPROP_EQUIP_SUITE*)pBPTEquip;
		INT nSuiteId = pBPTSuite->nSuiteID;
		if (nSuiteId <= 0)
			return	1;

		//CONST KBASICPROP_SUITEINFO* pBPTInfo = NULL;

		//switch (sIndex.nGenre)
		//{
		//case item_equip_green:
		//	pBPTInfo = pLibBPT->m_cGreenInfo.GetData(nSuiteId);
		//	break;
		//case item_equip_gold:
		//	pBPTInfo = pLibBPT->m_cGoldInfo.GetData(nSuiteId);
		//	break;
		//}

		//if (!pBPTInfo)
		//	return	1;

		//CONST KBPT_EQUIP_SUITEMA* pSuiteMA = pBPTInfo->arySuite;

		//sc.PushTable();
		//for (INT i = 0; i < emITEM_COUNT_SUITE; ++i)
		//{
		//	sc.PushTable();
		//	sc.PushString(g_cMagicAttribMgr.GetStringFromId(pSuiteMA[i].nAttrib));
		//	sc.SetTableField("szName");
		//	sc.PushTable();
		//	for (INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
		//	{
		//		sc.PushNumber(pSuiteMA[i].aryValue[j]);
		//		sc.SetTableIndex(j + 1);
		//	}
		//	sc.SetTableField("tbValue");
		//	sc.PushNumber(pSuiteMA[i].nNum);
		//	sc.SetTableField("nNum");
		//	sc.SetTableIndex(i + 1);
		//}
		//sc.SetTableField("tbSuiteMA");


		//for (INT i = 0; i < emITEM_COUNT_STRENGTHEN; ++i)
		//{
		//	CONST KBPT_EQUIP_ENHMA* pStrMA = &pBPTEquip->aryStr[i];
		//	sc.PushTable();
		//	sc.PushNumber(pStrMA->nTimes);
		//	sc.SetTableField("nTimes");
		//	sc.PushString(g_cMagicAttribMgr.GetStringFromId(pStrMA->nAttrib));
		//	sc.SetTableField("szName");
		//	sc.PushTable();
		//	for (INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
		//	{
		//		sc.PushTable();
		//		sc.PushNumber(pStrMA->aryRange[j].nMin);
		//		sc.SetTableField("nMin");
		//		sc.PushNumber(pStrMA->aryRange[j].nMax);
		//		sc.SetTableField("nMax");
		//		sc.SetTableIndex(j + 1);
		//	}
		//	sc.SetTableField("tbRange");
		//	sc.SetTableIndex(i + 1);
		//}
		//sc.SetTableField("tbStrengthenMA");

		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetOtherBaseProp
	// @Description		: 获得非装备基础属性信息
	// @ReturnCode		: 返回非装备基础属性信息表，出错返回nil
	// @ArgumentFlag	: ddd[d]
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetOtherBaseProp(QLuaScript& sc)
	{
		KITEM_INDEX sIndex;
		sIndex.nGenre		= sc.GetInt(1);
		sIndex.nDetailType	= sc.GetInt(2);
		sIndex.nParticular	= sc.GetInt(3);
		sIndex.nLevel = 0;
		if(sc.IsNumber(4))
		{
			sIndex.nLevel = sc.GetInt(4);
		}

		if (KD_ISEQUIP(sIndex.nGenre))
			return	0;

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
		CONST KBASICPROP_OTHER* pBPTOther = (CONST KBASICPROP_OTHER*)pLibBPT->GetBasicProp(sIndex);
		if (!pBPTOther)
			return	0;

		sc.PushTable();
		sc.PushNumber(pBPTOther->nStackMax);
		sc.SetTableField("nStackMax");
		sc.PushNumber(pBPTOther->nCDType);
		sc.SetTableField("nCdType");
		sc.PushNumber(pBPTOther->nReqLevel);
		sc.SetTableField("nReqLevel");
		sc.PushNumber(pBPTOther->bShortcut);
		sc.SetTableField("bShortcut");
		sc.PushString(pBPTOther->szForbidType);
		sc.SetTableField("szForbidType");
		sc.PushNumber(pBPTOther->nPrice);
		sc.SetTableField("nPrice");
		return	1;
	}
	
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetOtherForbidType
	// @Description		: 获得非装备道具禁止类型
	// @ReturnCode		: 返回非装备道具禁止类型，出错返回nil
	// @ArgumentFlag	: dddd
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetOtherForbidType(QLuaScript& sc)
	{
		KITEM_INDEX sIndex;
		sIndex.nGenre		= sc.GetInt(1);
		sIndex.nDetailType	= sc.GetInt(2);
		sIndex.nParticular	= sc.GetInt(3);
		sIndex.nLevel		= sc.GetInt(4);
		INT nVersion		= sc.GetInt(5);

		if (KD_ISEQUIP(sIndex.nGenre))
			return	0;

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
		CONST KBASICPROP_OTHER* pBPTOther = (CONST KBASICPROP_OTHER*)pLibBPT->GetBasicProp(sIndex);
		if (!pBPTOther)
			return	0;

		sc.PushString(pBPTOther->szForbidType);
		return	1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: IsItemBindByBindType
	// @Description		: 根据绑定类型判断道具新添加到玩家身上时是否是绑定的
	// @ReturnCode		: 1绑定
	// @ArgumentFlag	: d
	// @ArgumentComment	: nBindType:绑定类型
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaIsItemBindByBindType(QLuaScript& sc)
	{
		KE_ITEM_BINDTYPE eBindType = (KE_ITEM_BINDTYPE)sc.GetInt(1);
		sc.PushNumber(KItem::IsBindByBindType(eBindType));
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetObjById
	// @Description		: 根据道具ID获得道具对象
	// @ReturnCode		: pItem，失败返回nil
	// @ArgumentFlag	: d
	// @ArgumentComment	: dwId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetObjById(QLuaScript& sc)
	{
		DWORD dwId = (DWORD)sc.GetInt(1);
		INT nIndex = g_cItemMgr.SearchID(dwId);
		if (nIndex <= 0)
			return 0;

		sc.PushObj(Item[nIndex].m_pLuna);
		return 1;
	}

#ifdef GAME_SERVER

	static INT AddItemByScript(QLuaScript& sc, INT nBeginIndex, KE_ITEM_DATARECORD_WAY eItemAddWay)
	{
		INT nGenre			= sc.GetInt(nBeginIndex);
		nBeginIndex++;
		INT nDetailType		= sc.GetInt(nBeginIndex);
		nBeginIndex++;
		INT nParticularType	= sc.GetInt(nBeginIndex);

		KE_ITEM_DATARECORD_WAY eWay	= eItemAddWay;

		INT nLevel			= 0;
		INT nEnhTimes		= 0;
		UINT uRandSeed		= 0U;
		UINT uRandSeedEx	= 0U;
		BYTE* pRandomMa		= NULL;
		BYTE byRandom[emITEM_COUNT_RANDOM] = {0};
		KItem::KSTONE_INFO	aryStoneInfo[emSTONE_MAX_SLOT];
		KItem::KSTONE_INFO	*pStoneInfo = NULL;
		KMagicAttrib sIdentify;
		KMagicAttrib* pIdentify = NULL;;

		nBeginIndex++;
		if (sc.IsNumber(nBeginIndex))
			nLevel = sc.GetInt(nBeginIndex);

		nBeginIndex += 2;
		if (sc.IsNumber(nBeginIndex))
			nEnhTimes = sc.GetInt(nBeginIndex);

		nBeginIndex++;
		if (sc.IsTable(nBeginIndex))
		{	// 是StoneInfo的Table
			pStoneInfo = aryStoneInfo;
			for (INT i = 0; i < emSTONE_MAX_SLOT; i++)
			{
				sc.GetTableIndex(nBeginIndex, i + 1);
				INT nIndex = sc.GetTopIndex();
				sc.GetTableField(nIndex, "nStoneId");
				pStoneInfo[i].byStoneId = sc.GetInt(nIndex + 1);

				sc.GetTableIndex(nBeginIndex, i + 1);
				nIndex = sc.GetTopIndex();
				sc.GetTableField(nIndex, "nStoneLevel");
				pStoneInfo[i].byStoneLevel = sc.GetInt(nIndex + 1);
			}
		}

		nBeginIndex++;
		if (sc.IsTable(nBeginIndex))
		{
			pIdentify = &sIdentify;
			INT nIdx = 0;
			sc.GetTableIndex(nBeginIndex, ++nIdx);
			INT nIndex = sc.GetTopIndex();
			sIdentify.nAttribType = g_cMagicAttribMgr.GetIdFormString(sc.GetStr(nIndex));

			sc.GetTableIndex(nBeginIndex, ++nIdx);
			nIndex = sc.GetTopIndex();
			sIdentify.nValue[0] = sc.GetInt(nIndex);
			sIdentify.nValue[1] = sIdentify.nValue[0];
		}

		nBeginIndex++;
		if (sc.IsTable(nBeginIndex))
		{	// 参数是byRandom
			pRandomMa = byRandom;
			for (INT i = 0; i < emITEM_COUNT_RANDOM; i++)
			{
				sc.GetTableIndex(nBeginIndex, i + 1);			// LUA的Table下标是从1开始的
				INT nIndex = sc.GetTopIndex();
				byRandom[i] = sc.IsNumber(nIndex) ? sc.GetInt(nIndex) : 0;
			}
		}

		nBeginIndex++;
		if (sc.IsNumber(nBeginIndex))
			uRandSeed = (UINT)sc.GetInt(nBeginIndex);

		nBeginIndex++;
		if (sc.IsNumber(nBeginIndex))
			uRandSeedEx = (UINT)sc.GetInt(nBeginIndex);

		INT nItemIdx = g_cItemMgr.Add( 
			nGenre,
			nDetailType,
			nParticularType,
			nLevel,
			nEnhTimes,
			eWay,
			pStoneInfo,
			NULL,
			uRandSeed,
			uRandSeedEx
			);

		return nItemIdx;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddPlayerItem
	// @Description		: 给指定角色增加一个新道具
	// @ReturnCode		: 成功返回物品对象，失败返回nil
	// @ArgumentFlag	: ?ddd[ddd??uudddd]
	// @ArgumentComment	: nPlayerIndex/pPlayer:角色索引或者对象
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nLevel:道具等级，默认为0
	// @ArgumentComment	: nCount:道具数量，默认为1
	// @ArgumentComment	: nEnhTimes:道具已强化次数，默认为0
	// @ArgumentComment	: tbStoneInfo:道具生成参数，默认为nil
	// @ArgumentComment	: tbRandomInfo:道具生成随机参数，默认为nil
	// @ArgumentComment	: uRandSeed:随机种子，默认为自动分配(0)
	// @ArgumentComment	: uRandSeedEx:第二随机种子，默认为自动分配(0)
	// @ArgumentComment	: nBind:是否强制绑定，默认不强制绑定(0)
	// @ArgumentComment	: bIsTimeOut:是否会超时,默认叠加(FALSE)
	// @ArgumentComment	: bMsg2Player:是否提示获得
	// @ArgumentComment	: nWay: 获取物品的途径, 默认100
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddPlayerItem(QLuaScript &sc)
	{
		KPlayer *pPlayer	= NULL;
		INT nItemIdx		= 0;
		INT nBind			= 0;
		BOOL bIsTimeOut		= FALSE;

		if (sc.IsNumber(1))
		{
			INT nPlayerIdx = sc.GetInt(1);
			if (nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER)
				pPlayer = &pPlayer[nPlayerIdx];
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}

		if (!pPlayer)		// 角色参数不正确
			return	0;

		KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
		QCONFIRM_RET_FALSE(pPlayerItem);

		{
			INT nGenre = sc.GetInt(2);
			INT nDetail = sc.GetInt(3);
			INT nParticular = sc.GetInt(4);
			INT nLevel = 0;
			INT nAmount = 1;
			if(sc.IsNumber(5))
			{
				nLevel = sc.GetInt(5);
			}
			if(sc.IsNumber(6))
			{
				nAmount = sc.GetInt(6);
			}

			if(pPlayerItem->CheckAutoGive(nGenre, nDetail, nParticular, nLevel, nAmount))
			{
				return 0;
			}

			KITEM_INDEX sIndex = {nGenre, nDetail, nParticular, nLevel};
			if (!pPlayerItem->CanAddItemIntoBag(sIndex, emITEM_BIND_GET, nAmount))
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, pPlayer->GetId(), LANG_044, LANG_006);
				return 0;
			}
		}

		KE_ADD_ITEM_WAY nWay = emKADDITEM_BYSCRIPT;
		if (sc.IsNumber(15))
			nWay = (KE_ADD_ITEM_WAY)sc.GetInt(15);
		KE_ITEM_DATARECORD_WAY nItemAddWay = (KE_ITEM_DATARECORD_WAY)KPlayerItem::GetDataRecordWay((KE_ADD_ITEM_WAY)nWay);

		nItemIdx = AddItemByScript(sc, 2, nItemAddWay);

		INT nIndex = 13;	// 其他参数的起始索引
		if (sc.IsNumber(nIndex))
			nBind = sc.GetInt(nIndex);
		
		nIndex++;
		if (sc.IsNumber(nIndex))
			bIsTimeOut = sc.GetInt(nIndex);

		nIndex++;
		BOOL bMsg2Player = TRUE;
		if (sc.IsNumber(nIndex))
			bMsg2Player = sc.GetInt(nIndex);

		INT nCount = 1;
		if(sc.IsNumber(6))
		{
			nCount = sc.GetInt(6);
		}

		if (nItemIdx > 0)
		{
			if (nBind)
				Item[nItemIdx].Bind(TRUE);

			KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
			QCONFIRM_RET_FALSE(pPlayerItem);
			
			if(nCount > (INT)Item[nItemIdx].GetMaxCount())
			{
				INT nNumSerail = nCount / Item[nItemIdx].GetMaxCount() + (nCount % Item[nItemIdx].GetMaxCount()?1:0);
				if(nNumSerail > pPlayerItem->CountBagFreeCell())
				{
					_ASSERT(FALSE);
				}
				for(INT i = 0;i < nNumSerail;i++)
				{
					pPlayerItem->SetIdxArrNum(i, AddItemByScript(sc, 2, nItemAddWay));

					//printf("唯一码序列：",pPlayerItem->GetIdxArrNum(i));
				}
			}

			INT nIdx = pPlayerItem->AddItemOnBody(nItemIdx, nWay, FALSE, bMsg2Player, !bIsTimeOut, nCount);
			
			if (nIdx <= 0)
			{	// 加到人身上失败，从游戏世界中删掉
				g_cItemMgr.Remove(nItemIdx);
				return	0;
			}
			sc.PushObj(Item[nIdx].m_pLuna);
			return	1;
		}

		return	0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: IsEquipExist
	// @Description		: 根据GDPL判断一件装备是否存在
	// @ReturnCode		: 
	// @ArgumentFlag	: dddd
	// @ArgumentComment	: G
	// @ArgumentComment	: D
	// @ArgumentComment	: P
	// @ArgumentComment	: L
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaIsEquipExist(QLuaScript& sc)
	{
		INT nGenre = sc.GetInt(1);
		INT nDetail = sc.GetInt(2);
		INT nParticular = sc.GetInt(3);
		INT nLevel = sc.GetInt(4);

		QCONFIRM_RET_FALSE(KD_ISEQUIP(nGenre));
		CONST KLibOfBPT* pLibBPT = g_cItemMgr.GetGenerator()->GetLibOfBPT();
		QCONFIRM_RET_FALSE(pLibBPT);
		KITEM_INDEX sIndex = { nGenre, nDetail, nParticular, nLevel};
		CONST KBASICPROP_EQUIPMENT* pEq = static_cast<CONST KBASICPROP_EQUIPMENT*>(pLibBPT->GetBasicProp(sIndex));
		if(pEq)
		{
			sc.PushNumber(1);
		}
		else
		{
			sc.PushNumber(2);
		}

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetStackInfo
	// @Description		: 判断物品是否可叠加
	// @ReturnCode		: 成功返回插入成功的值，失败返回0
	// @ArgumentFlag	: ?ddd[dd???uudddd]
	// @ArgumentComment	: nPlayerIndex/pPlayer:角色索引或者对象
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nLevel:道具等级，默认为0
	// @ArgumentComment	: nEnhTimes:道具已强化次数，默认为0
	// @ArgumentComment	: tbGenInfo:道具生成参数，默认为nil
	// @ArgumentComment	: tbStoneInfo:道具生成参数，默认为nil
	// @ArgumentComment	: tbRandomInfo:道具生成随机参数，默认为nil
	// @ArgumentComment	: uRandSeed:随机种子，默认为自动分配(0)
	// @ArgumentComment	: uRandSeedEx:第二随机种子，默认为自动分配(0)
	// @ArgumentComment	: nBind:是否强制绑定，默认不强制绑定(0)
	// @ArgumentComment	: bIsTimeOut:是否会超时,默认叠加(FALSE)
	// @ArgumentComment	: bMsg2Player:是否提示获得
	// @ArgumentComment	: nWay: 获取物品的途径, 默认100
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetStackInfo(QLuaScript &sc){
		KPlayer *pPlayer	= NULL;
		if (sc.IsNumber(1))
		{
			INT nPlayerIdx = sc.GetInt(1);
			if (nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER)
				pPlayer = &pPlayer[nPlayerIdx];
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}

		if (!pPlayer)		// 角色参数不正确
			return	0;

		KE_ADD_ITEM_WAY nWay = emKADDITEM_BYSCRIPT;
		if (sc.IsNumber(14))
			nWay = (KE_ADD_ITEM_WAY)sc.GetInt(14);
		KE_ITEM_DATARECORD_WAY nItemAddWay = (KE_ITEM_DATARECORD_WAY)KPlayerItem::GetDataRecordWay((KE_ADD_ITEM_WAY)nWay);

		INT nItemIdx = AddItemByScript(sc, 2, nItemAddWay);

		KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
		QCONFIRM_RET_FALSE(pPlayerItem);
		
		KItem& rcItem = Item[nItemIdx];
		INT nBindType = rcItem.GetBindType();
		if (emITEM_BIND_GET == nBindType || emITEM_BIND_OWN == nBindType)
			rcItem.Bind();

		if (Item[nItemIdx].IsStackable())
		{	
			if(pPlayerItem->FindStackRoom(nItemIdx, nWay, rcItem.GetGenre()))
				return TRUE;
			else 
				return FALSE;
			
		}
		return FALSE;

	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetFailedFrameInterval
	// @Description		: 设置使用道具失败判断的帧间隔
	// @ReturnCode		: 无
	// @ArgumentFlag	: d
	// @ArgumentComment	: nNewInterval, 新的帧间隔
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetFailedFrameInterval(QLuaScript& sc)
	{
		INT nNewInterval = sc.GetInt(1);
		g_nUseFailedFrameInterval = nNewInterval;
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: ArrangeBag
	// @Description		: 整理背包
	// @ReturnCode		: 无
	// @ArgumentFlag	: ?
	// @ArgumentComment	: player
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaArrangeBag(QLuaScript& sc)
	{
		KPlayer *pPlayer	= NULL;
		KPlayerItem* pPlayerItem = NULL;
		if (sc.IsNumber(1))
		{
			INT nPlayerIdx = sc.GetInt(1);
			if (nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER)
				pPlayer = &pPlayer[nPlayerIdx];
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}

		pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
		LOG_PROCESS_ERROR(pPlayerItem);

		pPlayerItem->ArrangeBagItem();

	EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SplitItem
	// @Description		: 拆分道具
	// @ReturnCode		: 无
	// @ArgumentFlag	: ?dd
	// @ArgumentComment	: player
	// @ArgumentComment	: itemid
	// @ArgumentComment	: 拆分数量
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSplitItem(QLuaScript& sc)
	{
		INT nItemId = sc.GetInt(2);
		INT nSplitCount = sc.GetInt(3);

		KPlayer *pPlayer	= NULL;
		KPlayerItem* pPlayerItem = NULL;
		if (sc.IsNumber(1))
		{
			INT nPlayerIdx = sc.GetInt(1);
			if (nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER)
				pPlayer = &pPlayer[nPlayerIdx];
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}

		pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
		LOG_PROCESS_ERROR(pPlayerItem);

		pPlayerItem->c2sSplitItem(nItemId, nSplitCount);

	EXIT0:
		return 0;
	}


	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetFailedTimeLimit
	// @Description		: 设置使用道具连续失败次数限制
	// @ReturnCode		: 无
	// @ArgumentFlag	: d
	// @ArgumentComment	: nNweLimit, 新的限制次数
	// @ArgumentComment	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetFailedTimeLimit(QLuaScript& sc)
	{
		INT nNewLimit = sc.GetInt(1);
		g_nUseFailedLimitTime = nNewLimit;
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddItemInPos
	// @Description		: 在某个地点掉落一件道具
	// @ReturnCode		: 成功返回物品对象，失败返回nil
	// @ArgumentFlag	: dddddd[d??uo]
	// @ArgumentComment	: nMapId
	// @ArgumentComment	: nX
	// @ArgumentComment	: nY
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nEnhTimes:道具已强化次数，默认为0
	// @ArgumentComment	: tbGenInfo:道具生成参数，默认为nil
	// @ArgumentComment	: tbRandomInfo:道具生成随机参数，默认为nil
	// @ArgumentComment	: uRandSeed:随机种子，默认为自动分配(0)
	// @ArgumentComment	: pBelong:归属人
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddItemInPos(QLuaScript& sc)
	{
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: DelPlayerItem
	// @Description		: 将指定角色身上的某物品删除
	// @ReturnCode		: 1成功
	// @ArgumentFlag	: ??[dd]
	// @ArgumentComment	: nPlayerIndex/pPlayer:角色索引或对象
	// @ArgumentComment	: nItemIndex/pItem:道具索引或对象
	// @ArgumentComment	: 道具个数
	// @ArgumentComment	: nWay:失去途径
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaDelPlayerItem(QLuaScript &sc)
	{
		KPlayer *pPlayer = NULL;
		INT nItem = 0;
		INT nItemCount = 1;
		BOOL bOK = FALSE;

		INT nG = 0;
		INT nD = 0;
		INT nP = 0;
		INT nL = 0;

		if (sc.IsNumber(1))
		{
			INT nPlayerIdx = sc.GetInt(1);
			if (nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER)
				pPlayer = &pPlayer[nPlayerIdx];
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}

		if (!pPlayer)		// 角色参数不正确
			return	0;

		if (sc.IsNumber(2))
		{
			INT nItemIdx = sc.GetInt(2);
			if (nItemIdx > 0 && nItemIdx < MAX_ITEM)
				nItem = nItemIdx;
		}
		else
		{
			KLuaItem *pLuaItem = dynamic_cast<KLuaItem *>(sc.GetObj(2));
			nG = pLuaItem->getGenre();
			nD = pLuaItem->getDetail();
			nP = pLuaItem->getParticular();
			nL = pLuaItem->getLevel();

			
			if (pLuaItem)
				nItem = pLuaItem->GetIt()->GetGameIndex();
		}

		if(sc.IsNumber(3))
		{
			nItemCount = sc.GetInt(3);
		}

		KE_LOSE_ITEM_WAY nWay = emKLOSEITEM_SCRIPT;
		if (sc.IsNumber(4))
			nWay = (KE_LOSE_ITEM_WAY)sc.GetInt(4);
		
		KE_ITEM_DATARECORD_WAY eRecordWay = (KE_ITEM_DATARECORD_WAY)KPlayerItem::GetDataRecordWay(emKADDITEM_NONE, nWay);

		if (nItem <= 0)		// 道具参数不正确
			return	0;

		KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
		QCONFIRM_RET_FALSE(pPlayerItem);
		
		KItem& rcItem = Item[nItem];
		INT nCurCount = rcItem.GetCount();

		QCONFIRM_RET_FALSE(nCurCount >= nItemCount);

		if (nCurCount <= nItemCount)
		{
			if (!pPlayerItem->Remove(nItem, nWay))
				return	0;

			bOK = g_cItemMgr.Remove(nItem, eRecordWay, emITEM_CONSUMEMODE_COMMONSCRIPT, pPlayer);
		}
		else
		{
			bOK = pPlayerItem->SetItemStackCount(nItem, rcItem.GetCount() - nItemCount, emITEM_DATARECORD_REMOVE, emKLOSEITEM_CONSUME, emKADDITEM_NONE);
		}

		/*
		// 日志
		KLogWritter::GetInstance()->ItemHistory_RemoveItem(
			&Item[nItem],
			pPlayer,
			emKLOG_REASON_REMOVEITEM_SCRIPT);
		*/

		sc.PushNumber(bOK);
		if(bOK)
		{
			pPlayer->FireEvent(emKOBJEVENTTYPE_PLAYER_ITEM_OPS, pPlayer->GetId(), 0, 0);
		}

		//char szParam1[256];
		//ZeroMemory(szParam1,sizeof(szParam1));
		//sprintf(szParam1,"%d#%d#%d#%d",nG,nD,nP,nL);
		//ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		//cSafeScript->CallTableFunction( "DailyTasks", "commitTaskValue", 0, "osdd",pPlayer->GetScriptInterface(),szParam1,4,nItemCount);
		
		return 1;
	}

#else	// #ifdef GAME_SERVER

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: C
	// @ClassName		: --
	// @LuaApiName		: IsGetBindType
	// @Description		: 根据goods的类型返回对应物品的是否为绑定类型
	// @ReturnCode		: 1:获取绑定，0：不绑定
	// @ArgumentFlag	: d
	// @ArgumentComment	: Goods的Index
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaIsGetBindType(QLuaScript& sc)
	{
		INT nRet	= 0;
		//INT nIndex	= sc.GetInt(1);
		//KGoods* pGood = g_cBuySellMgr.GetGoods(nIndex);
		//if (pGood)
		//{
		//	nRet	= pGood->IsGetBind();
		//}
		return sc.PushNumber(nRet);
	}
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: C
	// @ClassName		: --
	// @LuaApiName		: GetPlayerGoldSuiteAttrib
	// @Description		: 根据套装ID获得黄金装备激活魔法属性
	// @ReturnCode		: 返回套装的名称、总数、各件名称、激活属性等信息表
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer:玩家对象
	// @ArgumentComment	: nSuiteId:套装ID
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPlayerGoldSuiteAttrib(QLuaScript &sc)
	{
		KLuaPlayer* pLuaPlayer = dynamic_cast<KLuaPlayer*>(sc.GetObj(1));
		if (!pLuaPlayer)
			return	0;

		INT nSuiteId = sc.GetInt(2);
		if (nSuiteId <= 0)
			return	0;

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibOfBPT = pItemGen->GetLibOfBPT();
		CONST KSUITE_INFO* pInfo = pLibOfBPT->GetGoldSuiteInfo(nSuiteId);
		KPlayerItem* pPlayerItem = &pLuaPlayer->GetHim()->GetCtrl<KPlayerItem>();
		CONST KPlayerItem::KMassSuiteVec& rvMASS = pPlayerItem->m_vecGoldMASS;

		return	::LuaGetSuiteAttrib(sc, nSuiteId, pPlayerItem, pInfo, rvMASS, pLibOfBPT->m_cGoldEquip);
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: C
	// @ClassName		: --
	// @LuaApiName		: GetPlayerGreenSuiteAttrib
	// @Description		: 根据套装ID获得绿色装备激活魔法属性
	// @ReturnCode		: 返回套装的名称、总数、各件名称、激活属性等信息表
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer:玩家对象
	// @ArgumentComment	: nSuiteId:套装ID
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPlayerGreenSuiteAttrib(QLuaScript &sc)
	{
		KLuaPlayer* pLuaPlayer = dynamic_cast<KLuaPlayer*>(sc.GetObj(1));
		if (!pLuaPlayer)
			return	0;

		INT nSuiteId = sc.GetInt(2);
		if (nSuiteId <= 0)
			return	0;

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibOfBPT = pItemGen->GetLibOfBPT();
		CONST KSUITE_INFO* pInfo = pLibOfBPT->GetGreenSuiteInfo(nSuiteId);
		KPlayerItem* pPlayerItem = &pLuaPlayer->GetHim()->GetCtrl<KPlayerItem>();
		CONST KPlayerItem::KMassSuiteVec& rvMASS = pPlayerItem->m_vecGreenMASS;

		return	::LuaGetSuiteAttrib(sc, nSuiteId, pPlayerItem, pInfo, rvMASS, pLibOfBPT->m_cGreenEquip);
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: C
	// @ClassName		: --
	// @LuaApiName		: GetRandAttribInfo
	// @Description		: 根据随机魔法属性名字/ID和级别获得相关信息
	// @ReturnCode		: 随机魔法属性信息表，出错返回nil。
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: szAttrib/nAttrib:魔法属性名/魔法属性ID
	// @ArgumentComment	: nLevel:魔法属性级别
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetRandAttribInfo(QLuaScript& sc)
	{
		KMAGIC_INDEX sMAIndex;
		sMAIndex.nAttrib = KD_MAGIC_INVALID;
		sMAIndex.nLevel  = sc.GetInt(2);

		if (sc.IsNumber(1))
			sMAIndex.nAttrib = sc.GetInt(1);
		else
			sMAIndex.nAttrib = g_cMagicAttribMgr.GetIdFormString(sc.GetStr(1));

		if (sMAIndex.nAttrib <= KD_MAGIC_INVALID)
			return	0;

		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
		CONST KBASICPROP_MAGICATTRIB* pBPTMA = pLibBPT->m_cMagicAttrib.GetData(sMAIndex);
		if (!pBPTMA)
			return	0;

		CONST KBPT_EQUIP_RANDMA* pRandMA = &pBPTMA->sMA;

		sc.PushTable();
		for (INT i = 0; i < KD_MAGIC_VALUE_NUM; ++i)
		{
			sc.PushTable();
			sc.PushNumber(pRandMA->aryRange[i].nMin);
			sc.SetTableField("nMin");
			sc.PushNumber(pRandMA->aryRange[i].nMax);
			sc.SetTableField("nMax");
			sc.SetTableIndex(i + 1);
		}

		return	1;
	}

#endif	// #ifdef GAME_SERVER #else

	#include "kitemscript_table.hpp"
	// -------------------------------------------------------------------------
}


//注册KItem
BOOL g_RegisterItemScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("KItem", KItemScriptNameSpace::arFunction, countof(KItemScriptNameSpace::arFunction));
}
// -------------------------------------------------------------------------
