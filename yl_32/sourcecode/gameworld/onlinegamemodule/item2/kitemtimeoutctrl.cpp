
#include "stdafx.h"
#include "kitemtimeoutctrl.h"
#include "kitemmgr.h"
#include "kplayeritem.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "onlinegameworld/kplayermgr.h"



bool KItemTimeoutCtrl::KimeoutLess::operator()(CONST INT& rcIdx1, CONST INT& rcIdx2) CONST
{
	ASSERT(rcIdx1 > 0 && rcIdx2 > 0);
	CONST KITEM_TIMEOUT* pTimeout1 = Item[rcIdx1].GetTimeout();
	CONST KITEM_TIMEOUT* pTimeout2 = Item[rcIdx2].GetTimeout();
	ASSERT(pTimeout1->emType == pTimeout2->emType);
	return pTimeout1->dwTimeout != pTimeout2->dwTimeout ?
		pTimeout1->dwTimeout < pTimeout2->dwTimeout : rcIdx1 < rcIdx2;
}

KItemTimeoutCtrl::KItemTimeoutCtrl() : m_bNotify(FALSE)
{

}

KItemTimeoutCtrl::~KItemTimeoutCtrl()
{
	ASSERT(m_pItemList == NULL);
}
#define XUANJINGLEVEL 6
VOID KItemTimeoutCtrl::Init(KPlayerItem* pItemList)
{
	ASSERT(pItemList);
	for (INT i = 0; i < emKITEM_TIMEOUTTYPE_COUNT; ++i)
	{
		SET_TIMEOUT* pSet = &m_aTimeout[i];
		pSet->clear();
	}
	m_pItemList = pItemList;
}

VOID KItemTimeoutCtrl::UnInit()
{
	for (INT i = 0; i < emKITEM_TIMEOUTTYPE_COUNT; ++i)
	{
		SET_TIMEOUT* pSet = &m_aTimeout[i];
		pSet->clear();
	}
	m_pItemList = NULL;
}

VOID KItemTimeoutCtrl::CheckTimeout(time_t tNow, INT nInterval)
{
#ifdef GAME_SERVER
	CheckAbsTime(&m_aTimeout[emKITEM_TIMEOUTTYPE_ABSOLUTE], tNow);
#endif
	CheckRelTime(&m_aTimeout[emKITEM_TIMEOUTTYPE_RELATIVE], nInterval);
}

VOID KItemTimeoutCtrl::CheckRelTime(SET_TIMEOUT* pSet, INT nInterval)
{
	ASSERT(nInterval > 0);
	SET_TIMEOUT::iterator it = pSet->begin();
	while (it != pSet->end())
	{
		SET_TIMEOUT::iterator cur = it++;
		INT nItemIdx = *cur;
		if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		{
			ASSERT(FALSE);
			break;
		}
		KItem* pItem = &Item[nItemIdx];
		CONST KITEM_TIMEOUT* pTimeout = pItem->GetTimeout();
		INT nLeftTime = pTimeout->dwTimeout - nInterval;

		if (nLeftTime <= 0)
		{
			pSet->erase(cur);
#ifdef GAME_SERVER
			RemoveItem(nItemIdx);
#else
			KITEM_TIMEOUT sTimeout = *pTimeout;
			sTimeout.dwTimeout = 0;
			pItem->SetTimeout(&sTimeout);
#endif
		}
		else
		{
			KITEM_TIMEOUT sTimeout = *pTimeout;
			sTimeout.dwTimeout = (DWORD)nLeftTime;
			pItem->SetTimeout(&sTimeout);
		}
	}
}

#ifdef GAME_SERVER
VOID KItemTimeoutCtrl::CheckAbsTime(SET_TIMEOUT* pSet, time_t tNow)
{
	ASSERT(tNow > 0);
	SET_TIMEOUT::iterator it = pSet->begin();
	while (it != pSet->end())
	{
		SET_TIMEOUT::iterator cur = it++;
		INT nItemIdx = *cur;
		if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		{
			ASSERT(FALSE);
			break;
		}
		if (Item[nItemIdx].GetTimeout()->dwTimeout <= (DWORD)tNow)
		{
			pSet->erase(cur);
			RemoveItem(nItemIdx);
		}
	}
}

VOID KItemTimeoutCtrl::RemoveItem(INT nItemIdx)
{
	ASSERT(nItemIdx > 0);
	do
	{
		KItem* pItem = &Item[nItemIdx];
		INT nPlayerIdx = pItem->GetBelong();
		if (nPlayerIdx <= 0)
		{
			ASSERT(FALSE);
			break;
		}
	
		CHAR szMsg[1024];
		sprintf(szMsg, "您的%s超过有效期已被删除！", pItem->GetOrgName());
		KPlayer* pOwner = g_cPlayerMgr.GetByIndex(nPlayerIdx);

		if (m_pItemList->Remove(nItemIdx, emKLOSEITEM_TIMEOUT))
		{
			g_cItemMgr.Remove(
				nItemIdx,
				emITEM_DATARECORD_REMOVE,
				emITEM_CONSUMEMODE_USINGTIMEOUT,
				pOwner
				);

			g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
				pOwner->GetId(), "系统", szMsg, emQCHAT_PANEL);
		}
		else
		{
			QLogPrintf(LOG_ERR, "KItemTimeoutCtrl::RemoveItem failed Idx % Owner %s GDP %d,%d,%d",
				pItem->GetGameIndex(), pOwner->GetName(),
				pItem->GetGenre(), pItem->GetDetailType(), pItem->GetParticular());
		}
	}
	while (0);
}
#endif

BOOL KItemTimeoutCtrl::AddTimeout(INT nItemIdx)
{
	ASSERT(nItemIdx > 0);
	BOOL bRet = FALSE;
	do
	{
		CONST KITEM_TIMEOUT* pTimeout = Item[nItemIdx].GetTimeout();
		if (pTimeout->emType < 0 ||	pTimeout->emType >= emKITEM_TIMEOUTTYPE_COUNT)
		{
			ASSERT(FALSE);
			break;
		}
		bRet = m_aTimeout[pTimeout->emType].insert(nItemIdx).second;
	}
	while (0);
	return bRet;
}

VOID KItemTimeoutCtrl::DelTimeout(INT nItemIdx)
{
	ASSERT(nItemIdx > 0);
	CONST KITEM_TIMEOUT* pTimeout = Item[nItemIdx].GetTimeout();
	if (pTimeout->emType >= 0 && pTimeout->emType < emKITEM_TIMEOUTTYPE_COUNT)
		m_aTimeout[pTimeout->emType].erase(nItemIdx);
}

VOID KItemTimeoutCtrl::SetTimeout(INT nItemIdx, CONST KITEM_TIMEOUT* pTimeout)
{
	ASSERT(nItemIdx > 0 && pTimeout);
	DelTimeout(nItemIdx);
	Item[nItemIdx].SetTimeout(pTimeout);
	AddTimeout(nItemIdx);
}

