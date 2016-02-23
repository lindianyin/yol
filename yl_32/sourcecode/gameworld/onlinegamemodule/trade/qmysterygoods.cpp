
#include "stdafx.h"
#include "qmysterygoods.h"
#include "item2/kitemmgr.h"

QMysteryGoods::QMysteryGoods()
{
	m_sIndex.nGenre		 		= ITEM_INVALID_TYPE;
	m_sIndex.nDetailType 		= ITEM_INVALID_TYPE;
	m_sIndex.nParticular 		= ITEM_INVALID_TYPE;
	m_sIndex.nGenre		 		= 0;
	
	m_nId						= -1;
	m_nBind						= -1;
	m_nPrice					= 0;
	m_dwTimeout					= 0;
	m_nCurrencyType				= -1;
	m_nRate						= 0;
	m_nValueable				= 0;
	m_nIsOpen					= 0;
}

BOOL QMysteryGoods::IsGetBind()
{
	return 1 == m_nBind;
}

INT QMysteryGoods::GetId(VOID) CONST
{
	return m_nId;
}

KITEM_INDEX QMysteryGoods::GetIndex(VOID) CONST
{
	return m_sIndex;
}

INT QMysteryGoods::GetBindType(VOID) CONST
{
	return m_nBind;
}

INT QMysteryGoods::GetGenre(VOID) CONST
{
	return m_sIndex.nGenre;
}

VOID QMysteryGoods::SetId(INT nId)
{
	if (nId < 0)
	{
		m_nId = -1;
	}
	else
	{
		m_nId = nId;
	}
}

LPCSTR QMysteryGoods::GetName(VOID) CONST
{
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	QCONFIRM_RET(pItemGen, NULL);

	CONST KLibOfBPT *pLibBPT = pItemGen->GetLibOfBPT();
	QCONFIRM_RET(pLibBPT, NULL);

	CONST KBASICPROP_BASEDATA *pBaseData = pLibBPT->GetBasicProp(m_sIndex);
	QCONFIRM_RET(pBaseData, NULL);

	return	pBaseData->szName;

}

DWORD QMysteryGoods::GetTimeout() CONST
{
	return m_dwTimeout;
}

VOID QMysteryGoods::SetTimeout( DWORD dwTimeout )
{
	m_dwTimeout = dwTimeout;
}


time_t QMysteryGoods::GetTimeSaleStart()
{
	return m_timeSaleStart;
}

VOID QMysteryGoods::SetTimeSaleStart( time_t timeSaleStart )
{
	m_timeSaleStart = timeSaleStart;
}

time_t QMysteryGoods::GetTimeSaleClose()
{
	return m_timeSaleClose;
}

VOID QMysteryGoods::SetTimeSaleClose( time_t timeSaleClose )
{
	m_timeSaleClose = timeSaleClose;
}

INT QMysteryGoods::GetTimeFrameStartDay()
{
	return m_nTimeFrameStartDay;
}

VOID QMysteryGoods::SetTimeFrameStartDay( INT nTimeFrameStartDay )
{
	m_nTimeFrameStartDay = nTimeFrameStartDay;
}