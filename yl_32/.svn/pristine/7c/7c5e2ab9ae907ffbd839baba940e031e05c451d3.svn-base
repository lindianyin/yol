/* -------------------------------------------------------------------------
//	文件名		：	kclosefriendrelation.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "closefriendrelation.h"
#include "GameCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


KCloseFriendRelation::KCLOSEFRIEND_INFO KCloseFriendRelation::GetDefaultValue()
{
	KCLOSEFRIEND_INFO sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.sDBInfo.wVersion = 2;
	sInfo.sDBInfo.bySendMail				= 0;
	sInfo.sDBInfo.nGainBindCoinMaster		= 0;
	sInfo.sDBInfo.nGainBindCoinTarget		= 0;
	sInfo.sDBInfo.tCreateTime				= g_pGameCenter->m_nTimeNow;
	return sInfo;
}

KCloseFriendRelation::~KCloseFriendRelation()
{

}

VOID KCloseFriendRelation::Init(INT nMaster, INT nTarget, KCLOSEFRIEND_INFO *pInfo)
{
	m_nMaster = nMaster;
	m_nTarget = nTarget;
	m_pInfo = pInfo;
}

KCloseFriendRelation::KCLOSEFRIEND_INFO KCloseFriendRelation::Load( LPVOID pData, INT nLen )
{
	ASSERT(pData && nLen == sizeof(KCLOSEFRIEND_DBINFO));
	KCLOSEFRIEND_INFO sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.sDBInfo = *(KCLOSEFRIEND_DBINFO*)pData;
	if (sInfo.sDBInfo.wVersion == 1)
	{
		sInfo.sDBInfo.nGainBindCoinMaster	= sInfo.sDBInfo.nGainBindCoinTarget;
		sInfo.sDBInfo.wVersion = 2;
	}
	return sInfo;
}

KCloseFriendRelation::KCloseFriendRelation(INT nType, IKRelationFactory *pFactory):
	m_nType(nType),
	m_pFactory(pFactory),
	m_pInfo(NULL)
{
	ASSERT(pFactory);
	m_nMaster = 0;
	m_nTarget = 0;
}

BOOL KCloseFriendRelation::QueryRelation( INT nType, LPVOID *ppv )
{
	if (nType == m_nType)
	{
		*ppv = static_cast<IKCloseFriendRelation*>(this);
		return TRUE;
	}
	return FALSE;
}

INT KCloseFriendRelation::GetType()
{
	return m_nType;
}

INT KCloseFriendRelation::GetInfLen()
{
	return sizeof(KCLOSEFRIEND_INFO);
}

BOOL KCloseFriendRelation::SaveInfo( CHAR *pBuf, INT nLen )
{
	ASSERT(pBuf);
	ASSERT(nLen == sizeof(KCLOSEFRIEND_INFO));
	memcpy(m_pInfo, pBuf, nLen);
	return TRUE;
}

BOOL KCloseFriendRelation::LoadInfo(CHAR *pBuf, INT nLen )
{
	ASSERT(nLen == sizeof(KCLOSEFRIEND_INFO));
	memcpy(pBuf, m_pInfo, nLen);
	return TRUE;
}

INT KCloseFriendRelation::GetRole( BOOL bAsMaster )
{
	return bAsMaster ? m_nMaster : m_nTarget;
}

INT KCloseFriendRelation::GetBindCoin(BOOL bMaster)
{
	INT nRet	= m_pInfo->sDBInfo.nGainBindCoinTarget;
	if (bMaster == TRUE)
		nRet	= m_pInfo->sDBInfo.nGainBindCoinMaster;

	return nRet * 5 / 100; // ５%的返回
}

BOOL KCloseFriendRelation::AddBindCoin(INT nCoin, BOOL bMaster)
{
	ASSERT(m_pInfo);
	if (bMaster)
		m_pInfo->sDBInfo.nGainBindCoinMaster	+= nCoin;
	else
		m_pInfo->sDBInfo.nGainBindCoinTarget	+= nCoin;
	m_pInfo->btModify	= 1;

// 	g_cPlayerSaveLoadManager.AddRelayDirty(m_nMaster);
// 	g_cPlayerSaveLoadManager.AddRelayDirty(m_nTarget);
// 	// 平台数据统计LOG
// 	g_piStatLog->ModifyAdd(KD_STTAB_BINDCOIN, "[产出]密友返还", "总量", (INT)(nCoin / 20)); // ５%的返回

	return TRUE;
}

#define ONEYEAR	31536000
BOOL KCloseFriendRelation::CheckOverTime()
{
	BOOL bRet	= FALSE;
	time_t tNowTime	= g_pGameCenter->m_nTimeNow;
	if (tNowTime - m_pInfo->sDBInfo.tCreateTime > ONEYEAR)
		bRet	= TRUE;
	return bRet;
}

INT KCloseFriendRelation::GetRemanentTime()
{
	time_t tNowTime	= g_pGameCenter->m_nTimeNow;
	INT nRemanentTime = (m_pInfo->sDBInfo.tCreateTime + ONEYEAR) - tNowTime;
	nRemanentTime = nRemanentTime > 0 ? nRemanentTime : 0;
	return nRemanentTime;
}

BOOL KCloseFriendRelation::CanSendMail()
{
	return m_pInfo->sDBInfo.bySendMail == 0;
}

BOOL KCloseFriendRelation::SetSendMail(BOOL bSend)
{
	m_pInfo->sDBInfo.bySendMail	= 1;
	return TRUE;
}

BOOL KCloseFriendRelation::GainBindCoin(BOOL bMaster)
{
	GetBindCoin(bMaster);
	if (bMaster == TRUE)
		m_pInfo->sDBInfo.nGainBindCoinMaster	= 0;
	else
		m_pInfo->sDBInfo.nGainBindCoinTarget	= 0;
	m_pInfo->btModify	= 1;

// 	g_cPlayerSaveLoadManager.AddRelayDirty(m_nMaster);
// 	g_cPlayerSaveLoadManager.AddRelayDirty(m_nTarget);

	return TRUE;
}

INT KCloseFriendRelation::GetOtherPlayerId(INT nSelfId)
{
	INT nPlayerId	= m_nMaster;
	if (nSelfId == m_nMaster)
		nPlayerId	= m_nTarget;
	return nPlayerId;
}
