/* -------------------------------------------------------------------------
//	文件名		：	relationcounter.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	玩家好友计数器（防止超出好友数目限制）
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "relationcounter.h"
#include "relationmanager.h"
#include "relationfactorylist.h"

KRelationCounter KRelationCounter::ms_isnt;

KRelationCounter::KRelationCounter()
{
}

KRelationCounter::~KRelationCounter()
{
}

BOOL KRelationCounter::CheckReachMaxCount(INT nType, INT nPlayerId)
{
	// CHAR szMsg[256];
	INT nCount	= 0;
// 	KGC_Player* pGCPlayer	= g_pPlayerManager->GetPlayer(nPlayerId);
// 	KGC_CONFIRM_RET_FALSE(pGCPlayer);
	
	
	if (emKPLAYERRELATION_TYPE_ENEMEY == nType)
	{
		nCount	= GetOneTypeCount(nPlayerId, nType);
		if (nCount < emKRELATION_ENEMYRELATION_COUNT)
			return TRUE;
		else
			return FALSE;
	}
	else if (emKPLAYERRELATION_TYPE_BLACKLIST == nType)
	{
		nCount	= GetOneTypeCount(nPlayerId, nType);
		if (nCount < emKRELATION_BLACKRELATION_COUNT)
			return TRUE;
//		sprintf(szMsg, KD_STR_BLACKLIST_UPTO_MAX);
	}
	else if (emKPLAYERRELATION_TYPE_BINDFRIEND == nType || emKPLAYERRELATION_TYPE_TMPFRIEND == nType)
	{
//		INT nMaxCount	= pGCPlayer->GetMaxFriendNum();
		INT nMaxCount	= 0;
		nMaxCount	= (nMaxCount == 0) ? emKRELATION_FRIENDRELATION_COUNT : nMaxCount;

		nCount	= GetOneTypeCount(nPlayerId, emKPLAYERRELATION_TYPE_BINDFRIEND) + 
			GetOneTypeCount(nPlayerId, emKPLAYERRELATION_TYPE_TMPFRIEND);

		if (nCount < nMaxCount)
			return TRUE;
//		sprintf(szMsg, KD_STR_FRIEND_UPTO_MAX);
	}
	else if (emKPLAYERRELATION_TYPE_COUPLE == nType)
	{
		nCount = GetOneTypeCount(nPlayerId, emKPLAYERRELATION_TYPE_COUPLE);
		if (nCount < emKRELATION_COUPLE_COUNT)
			return TRUE;
		
//		::sprintf(szMsg, KD_STR_COUPLE_UPTO_MAX);
	}
	else if(emKPLAYERRELATION_TYPE_BUDDY == nType || emKPLAYERRELATION_TYPE_TRAINED == nType || emKPLAYERRELATION_TYPE_TRAINING == nType || 
		emKPLAYERRELATION_TYPE_INTRODUCTION == nType)
	{
		return TRUE;
	}

//	INT nMasterSvrID = pGCPlayer->GetOnlineServer();
//	g_ChatChannelManager.PostSystemMessage(nPlayerId, KD_STR_SYSTEM, szMsg, nMasterSvrID);
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("Relation", "NotifyReachMaxCount_GC", 0, "dd", nPlayerId, nType);
 	return FALSE;
}

INT KRelationCounter::GetTrainAndBuddyCount(INT nPlayerId)
{
	INT nCount	= 0;
	nCount	= GetOneTypeCount(nPlayerId, emKPLAYERRELATION_TYPE_TRAINING, true);
	nCount	+= GetOneTypeCount(nPlayerId, emKPLAYERRELATION_TYPE_TRAINED, true);
	nCount	+= GetOneTypeCount(nPlayerId, emKPLAYERRELATION_TYPE_INTRODUCTION, true);
	nCount	+=  GetOneTypeCount(nPlayerId, emKPLAYERRELATION_TYPE_BUDDY, true);	
	return nCount;
}


INT KRelationCounter::GetOneTypeCount(INT nPlayerId, BYTE btType, BOOL bMaster /*=TRUE*/)
{
	IKRelationFactory*			pFactory		= NULL;
	IKEnumRelation*				pEnum			= NULL;
	INT							nCount			= 0;
	BOOL						bRetCode		= FALSE;

	LOG_PROCESS_ERROR(KRelationFactoryList::Instance()->CreateFactory(btType, &pFactory));
	// 现在此种关系的人数
	if (pFactory->GetAllRelations(bMaster, nPlayerId, &pEnum))
		nCount	= pEnum->GetCount();

	bRetCode = nCount;
EXIT0:
	return bRetCode;

// 	do 
// 	{
// 		if (!KRelationFactoryList::Instance()->CreateFactory(btType, &pFactory))
// 		{
// 			KGC_Error("");
// 			break;
// 		}	
// 		// 现在此种关系的人数
// 		if (pFactory->GetAllRelations(bMaster, nPlayerId, &pEnum))
// 			nCount	= pEnum->GetCount();
// 	} while(FALSE);
// 	return nCount;
}