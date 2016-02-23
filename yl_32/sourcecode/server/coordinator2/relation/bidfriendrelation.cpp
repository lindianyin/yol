#include "stdafx.h"
#include "bidfriendrelation.h"
#include "relationfactorylist.h"
#include "relationmanager.h"
#include <math.h>

KBidFriendRelation::KBidFriendRelation(INT nType, IKRelationFactory *pFactory) :
	m_nType(nType),
	m_pFactory(pFactory),
	m_pInfo(NULL)
{
	ASSERT(pFactory);
	m_nMaster = m_nTarget = 0;
}

KBidFriendRelation::~KBidFriendRelation()
{
}

KBidFriendRelation::KFRIEND_INFO KBidFriendRelation::GetDefaultValue()
{
	KFRIEND_INFO sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.sDBInfo.nFavor = g_pGameCenter->m_RelationManager.GetMinFavor();
	return sInfo;
}

KBidFriendRelation::KFRIEND_INFO KBidFriendRelation::Load(LPVOID pData, INT nLen)
{
	ASSERT(pData && nLen == sizeof(KFRIEND_DBINFO));
	KFRIEND_INFO sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.sDBInfo = *(KFRIEND_DBINFO*)pData;
	return sInfo;
}

VOID KBidFriendRelation::Init(INT nMaster, INT nTarget, KFRIEND_INFO *pInfo)
{
	m_nMaster = nMaster;
	m_nTarget = nTarget;
	m_pInfo = pInfo;
}

// 查询子接口
BOOL KBidFriendRelation::QueryRelation(INT nType, LPVOID *ppv)
{
	if (nType == m_nType || nType == emKPLAYERRELATION_TYPE_BINDFRIEND)
	{
		*ppv = static_cast<IKBidFriendRelation*>(this);
		return TRUE;
	}
	return FALSE;
}

// 查询关系类型
INT KBidFriendRelation::GetType()
{
	return m_nType;
}

// 获取附加信息长度
INT KBidFriendRelation::GetInfLen()
{
	return sizeof(KFRIEND_INFO);
}

// 保存附加数据到指定缓冲区（用于同步好友信息）
BOOL KBidFriendRelation::SaveInfo(CHAR *pBuf, INT nLen)
{
	BOOL bRetCode = FALSE;
	LOG_PROCESS_ERROR(pBuf);
	LOG_PROCESS_ERROR(nLen == sizeof(KFRIEND_INFO));

	memcpy(m_pInfo, pBuf, nLen);
	bRetCode = TRUE;

EXIT0:
	return bRetCode;
}

// 加载附加数据
BOOL KBidFriendRelation::LoadInfo(CHAR *pBuf, INT nLen)
{
	BOOL bRetCode = FALSE;
	LOG_PROCESS_ERROR(nLen == sizeof(KFRIEND_INFO));

	memcpy(pBuf, m_pInfo, nLen);
	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

// 获取主位/次位玩家（bAsMaster为TRUE表示主位，FALSE表示次位）
INT KBidFriendRelation::GetRole(BOOL bAsMaster)
{
	return bAsMaster ? m_nMaster : m_nTarget;
}

INT KBidFriendRelation::GetOtherPlayerId(INT nSelfId)
{
	INT nPlayerId = m_nMaster;
	if (nSelfId == m_nMaster)
		nPlayerId = m_nTarget;
	return nPlayerId;
}

INT KBidFriendRelation::GetFavor()
{
	return m_pInfo->sDBInfo.nFavor;
}

DWORD KBidFriendRelation::GetLastTalkTime()
{
	return m_pInfo->sDBInfo.dwLastTalkTime;
}

BOOL KBidFriendRelation::CheckFavor(INT nPoint, INT nMethod, KFRIEND_INFO *pInfo)
{
	BOOL bRetCode = FALSE;
	INT nResPoint = 0;
	INT nMaxPoint = 0;

	LOG_PROCESS_ERROR(pInfo);
	LOG_PROCESS_ERROR(nPoint > 0 && nMethod >= 0 && nMethod < emKMETHOD_COUNT);
	
	// 检查最大值
	nResPoint = pInfo->sDBInfo.nFavor + nPoint;
	LOG_PROCESS_ERROR(nResPoint >= 0 && nResPoint <= g_pGameCenter->m_RelationManager.GetMaxFavor());
	
	// 检查每日亲密度限额
	switch (nMethod)
	{
	case emKMETHOD_NORMAL:
		nMaxPoint = g_pGameCenter->m_RelationManager.GetMaxFavorEveryDayWithoutIB();
		break;
	case emKMETHOD_IBITEM:
		nMaxPoint = g_pGameCenter->m_RelationManager.GetMaxFavorEveryDayWithIB();
		break;
	default:
		LOG_PROCESS_ERROR(0);
		break;
	}

	LOG_PROCESS_ERROR(pInfo->sDBInfo.aryDayPoint[nMethod] + pInfo->aryPrePoint[nMethod] + nPoint <= nMaxPoint);
	
	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}


INT KBidFriendRelation::GetFavorLevel(INT nPoint)
{
	INT nFavor = m_pInfo->sDBInfo.nFavor;
	if (nFavor == 0)
		return 1;
	INT nPrvLevel = (INT)ceil(sqrt(nFavor / 100.0)); 
	INT nCurLevel = (INT)ceil(sqrt((nFavor + nPoint) / 100.0));
	if (nPrvLevel < nCurLevel)
		return nCurLevel;
	return -1;
}

BOOL KBidFriendRelation::AddFavor(INT nPoint, INT nMethod, INT& nAwardLevel, BOOL bByHand)
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(nPoint > 0);
	LOG_PROCESS_ERROR(bByHand || CheckFavor(nPoint, nMethod, m_pInfo));

	nAwardLevel = GetFavorLevel(nPoint);
	m_pInfo->sDBInfo.aryDayPoint[nMethod] += nPoint;
	m_pInfo->sDBInfo.nFavor += nPoint;
	m_pInfo->btModify = 1;
	// g_cPlayerSaveLoadManager.AddRelayDirty(m_nMaster);
	// g_cPlayerSaveLoadManager.AddRelayDirty(m_nTarget);
	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KBidFriendRelation::SetLastTalkTime(DWORD dwTime)
{
	BOOL bResult = FALSE;
	m_pInfo->sDBInfo.dwLastTalkTime = dwTime;
	m_pInfo->btModify = 1;

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KBidFriendRelation::PreAddFavor(INT nPoint, INT nMethod)
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(nPoint > 0);

	bResult = CheckFavor(nPoint, nMethod, m_pInfo);
	LOG_PROCESS_ERROR(bResult);

	m_pInfo->aryPrePoint[nMethod] += nPoint;
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KBidFriendRelation::CancelPreAddFavor(INT nPoint, INT nMethod)
{
	BOOL bRetCode = FALSE;
	LOG_PROCESS_ERROR(nPoint > 0 && nMethod >= 0 && nMethod < emKMETHOD_COUNT);

	LOG_PROCESS_ERROR(m_pInfo->aryPrePoint[nMethod] >= nPoint);
	m_pInfo->aryPrePoint[nMethod] -= nPoint;

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}


BOOL KBidFriendRelation::ActAddFavor(INT nPoint, INT nMethod)
{
	BOOL bRetCode = FALSE;

	bRetCode = CheckFavor(nPoint, nMethod, m_pInfo);
	LOG_PROCESS_ERROR(bRetCode);

	LOG_PROCESS_ERROR(m_pInfo->aryPrePoint[nMethod] >= nPoint);
	// KGC_CONFIRM_RET_FALSE(m_pInfo->aryPrePoint[nMethod] >= nPoint);
	m_pInfo->sDBInfo.aryDayPoint[nMethod] += nPoint;
	m_pInfo->sDBInfo.nFavor += nPoint;
	m_pInfo->aryPrePoint[nMethod] -= nPoint;
	m_pInfo->btModify = 1;
	// g_cPlayerSaveLoadManager.AddRelayDirty(m_nMaster);
	// g_cPlayerSaveLoadManager.AddRelayDirty(m_nTarget);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KBidFriendRelation::_ClearFavorLimit()
{
	ZeroMemory(m_pInfo->sDBInfo.aryDayPoint, sizeof(m_pInfo->sDBInfo.aryDayPoint));
	return TRUE;
}

BOOL KBidFriendRelation::ClearFavorLimit(INT nPlayerId)
{
	BOOL					bRetCode = FALSE;
	IKRelationFactory*		pFactory = NULL;
	IKEnumRelation*			pEnumRelation = NULL;
	IKRelation*				pRelation = NULL;

	KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFactory);
	LOG_PROCESS_ERROR(pFactory);
	// KGC_CONFIRM_RET_FALSE(pFactory);
	pFactory->GetAllRelations(TRUE, nPlayerId, &pEnumRelation);
	LOG_PROCESS_ERROR(pEnumRelation);
	// KGC_CONFIRM_RET_FALSE(pEnumRelation);
	while(pEnumRelation->Next(&pRelation))
	{
		IKBidFriendRelation *pFriend = NULL;
		if (pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend))
		{
			((KBidFriendRelation* )pFriend)->_ClearFavorLimit();
		}
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}


