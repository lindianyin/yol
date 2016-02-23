/* -------------------------------------------------------------------------
//	文件名		：	kgc_relaydatasaveloader.cpp
//	创建者		：	luobaohang
//	创建时间	：	2008-5-21 9:19:53
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "datasaveloader.h"
#include "relationfactorylist.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma pack(push, 1)
struct KRelaionHead
{
	INT nLastAddRelayDay;
	WORD wMaxFriendNum;
	INT nTrainingStudentMonth;
	INT nTrainingCheckInTime;
	INT nTrainingStudentHintTime;
	INT nTrainingTeacherHintTime;
	WORD wTrainingAllStudentCount;
	BYTE btTrainingStudentCount;
	BYTE btTrainingOption;
};
struct KRelationTypeHead 
{
	BYTE btType;
	BYTE btFlag;
	BYTE btDataSize;
	WORD wRelayNum;
};
#pragma pack(pop)
// -------------------------------------------------------------------------

BOOL RelationSaveLoader::AddHead( INT nPlayerId )
{
// 	KGC_Player* pPlayer = g_pPlayerManager->GetPlayer(nPlayerId);
// 	KGC_CONFIRM_RET_FALSE(pPlayer);
 	KRelaionHead* pHead = (KRelaionHead*)m_pWritePos;
	memset(pHead, 0, sizeof(*pHead));
// 	cHead.nLastAddRelayDay = pPlayer->GetLastAddRelationDay();
// 	cHead.wMaxFriendNum = pPlayer->GetMaxFriendNum();
// 	cHead.nTrainingStudentMonth = pPlayer->GetTrainingStudentMonth();
// 	cHead.nTrainingCheckInTime = pPlayer->GetTrainingCheckInTime();
// 	cHead.nTrainingStudentHintTime = pPlayer->GetTrainingStudentHintTime();
// 	cHead.nTrainingTeacherHintTime = pPlayer->GetTrainingTeacherHintTime();
// 	cHead.wTrainingAllStudentCount = pPlayer->GetTrainingAllStudentCount();
// 	cHead.btTrainingStudentCount = pPlayer->GetTrainingStudentCount();
// 	cHead.btTrainingOption = pPlayer->GetTrainingOption();
 	m_pWritePos += sizeof(*pHead);

	return TRUE;
}

BOOL RelationSaveLoader::SaveBegin( INT nType, INT nDataLen, BOOL bMaster /*= TRUE*/ ) 
{
	assert(m_nCurType == 0 && (UINT)nType <= 255 && (UINT)nDataLen <= 255);

	m_nCurType = nType;
	m_nCurDataLen = nDataLen;
	KRelationTypeHead* pHead = (KRelationTypeHead*)m_pWritePos;
	pHead->btType = (BYTE)nType;
	pHead->btFlag = 0;
	if (bMaster)
		pHead->btFlag |= 1;
	pHead->btDataSize = (BYTE)nDataLen;
	pHead->wRelayNum = 0;
	m_pwRelayNum = &pHead->wRelayNum;
	m_pWritePos += sizeof(*pHead);
	return TRUE;
}

BOOL RelationSaveLoader::SaveEnd( INT nType ) 
{
	assert(m_nCurType == nType);

	m_nCurType = 0;
	m_nCurDataLen = 0;
	m_pwRelayNum = NULL;
	return TRUE;
}

BOOL RelationSaveLoader::AddRecord( INT nPlayerId, LPVOID pData /*= NULL*/) 
{
	assert(m_pwRelayNum > 0);
	KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	assert(pRole);
	assert(pData || m_nCurDataLen == 0);
	// memcpy(m_pWritePos, 0, sizeof(GUID));
	// memcpy(m_pWritePos, pPlayer->GetGUID(), sizeof(GUID));
	// m_pWritePos += sizeof(GUID);
	memcpy(m_pWritePos, &nPlayerId, sizeof(nPlayerId));
	m_pWritePos += sizeof(nPlayerId);
	if (m_nCurDataLen > 0)
	{
		memcpy(m_pWritePos, pData, m_nCurDataLen);
		m_pWritePos += m_nCurDataLen;
	}
	++(*m_pwRelayNum);
	if (*m_pwRelayNum > KRelationCounter::emKRELATION_FRIENDRELATION_COUNT)
	{
		assert(0);
		// KGC_Error(pPlayer->GetPlayerName() << "\t" << *m_pwRelayNum << "\t" << (INT)m_nCurType);
	}
	return TRUE;
}

BOOL RelationSaveLoader::OnLoadRelayData( INT nPlayerId, LPBYTE pData, UINT uDataSize ) 
{
	assert(nPlayerId);
	assert(pData && uDataSize >= sizeof(KRelationTypeHead));
	assert(uDataSize >= sizeof(KRelaionHead));
	LPBYTE pEndData = pData + uDataSize;
	KRelaionHead* pHead = (KRelaionHead*)pData;
	pData += sizeof(*pHead);
// 	pPlayer->SetLastAddRelationDay(cHead.nLastAddRelayDay);
// 	pPlayer->_SetMaxFriendNum(cHead.wMaxFriendNum);
// 	pPlayer->_SetTrainingStudentMonth(cHead.nTrainingStudentMonth);
// 	pPlayer->_SetTrainingCheckInTime(cHead.nTrainingCheckInTime);
// 	pPlayer->_SetTrainingStudentHintTime(cHead.nTrainingStudentHintTime);
// 	pPlayer->_SetTrainingTeacherHintTime(cHead.nTrainingTeacherHintTime);
// 	pPlayer->_SetTrainingAllStudentCount(cHead.wTrainingAllStudentCount);
// 	pPlayer->_SetTrainingStudentCount(cHead.btTrainingStudentCount);
// 	pPlayer->_SetTrainingOption(cHead.btTrainingOption);

	while (pData < pEndData - sizeof(KRelationTypeHead))
	{
		KRelationTypeHead& cHead = *(KRelationTypeHead*)pData;
		pData += sizeof(KRelationTypeHead);

		if (cHead.wRelayNum == 0)
			continue;

		// 简单做些校验，一个关系的个数不可能超过400
		if (cHead.wRelayNum >= 400)
		{
			// KGC_Error(pPlayer->GetPlayerName() << "\t" << (INT)cHead.wRelayNum << "\t" << (INT)cHead.btType);
			assert(0);
		}

		IKRelationFactory *pFactory = NULL;
		if (!KRelationFactoryList::Instance()->CreateFactory(cHead.btType, &pFactory) ||
			pFactory->GetRelationInfoLen() != cHead.btDataSize)
		{
			// KGC_Error( (INT(pFactory) ? pFactory->GetRelationInfoLen() : INT(pFactory)) << " " << cHead.btDataSize);
			pData += cHead.wRelayNum * (sizeof(INT) + cHead.btDataSize);
			continue;
		}
		assert(pData + cHead.wRelayNum * (sizeof(INT) + cHead.btDataSize) <= pEndData);
		for (INT i = 0; i < cHead.wRelayNum; ++i)
		{
// 			GUID& cGuid = *(GUID*)pData;
// 			pData += sizeof(GUID);
// 			INT nPlayerId2 = g_pPlayerManager->GetPlayerIDByGUID(cGuid);
// 			KGC_Player* pPlayer2 = g_pPlayerManager->GetPlayer(nPlayerId2);
// 			KGC_CONFIRM_CONTINUE(pPlayer2);
			INT nPlayerId2 = *(INT*)pData;
			pData += sizeof(nPlayerId2);
			assert(nPlayerId2 > 0);
			KRole* pPlayer2 = g_pGameCenter->m_RoleManager.GetRole(nPlayerId2);
			if (!pPlayer2)
			{
				pData += cHead.btDataSize;
				continue;
			}
			IKRelation *pRelation = NULL;
			// 如果pPlayer2已加载，就不用再Load
			// 如果好友数据库加载很慢，导致player2 unload后玩家好友还没加载好，有回档的可能，但忽略，把unload廷迟设大点比较好
			// if (cHead.btType != emKPLAYERRELATION_TYPE_ENEMEY  && cHead.btType >= emKPLAYERRELATION_TYPE_BINDFRIEND && pPlayer2->GetRelayLoaded() != 0)
			if (pPlayer2->IsOnline() && cHead.btType != emKPLAYERRELATION_TYPE_ENEMEY  && cHead.btType >= emKPLAYERRELATION_TYPE_BINDFRIEND)
			{
				pData += cHead.btDataSize;
				continue;
			}

			// 如果是仇人关系，只加载被谁杀了，不加载杀了谁
			if (cHead.btType == emKPLAYERRELATION_TYPE_ENEMEY && ((cHead.btFlag & 0x1) == 0))
			{
				pData += cHead.btDataSize;
				continue;
			}

			INT nMaster, nTarget;

			if ((cHead.btFlag & 0x1) == 1)
			{
				nMaster	= nPlayerId;
				nTarget	= nPlayerId2;
			}	
			else
			{
				nMaster	= nPlayerId2;
				nTarget	= nPlayerId;
			}
			
			if (!pFactory->Load(nMaster, nTarget, pData, cHead.btDataSize))
			{
				assert(0);
			}
			pData += cHead.btDataSize;
		}
	}
	return TRUE;
}

// -------------------------------------------------------------------------
