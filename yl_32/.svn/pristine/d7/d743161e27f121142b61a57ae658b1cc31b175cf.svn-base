/* -------------------------------------------------------------------------
//	文件名		：	trainingfactory.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	师徒（未出师）工厂
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "trainingfactory.h"
#include "relationfactorylist.h"
#include "trainingsetting.h"

KTrainingFactory::KTrainingFactory() : SUPER_CLASS(emKPLAYERRELATION_TYPE_TRAINING)
{
}

KTrainingFactory::~KTrainingFactory()
{
}

BOOL KTrainingFactory::CreateRelation(INT nTeacherID,
									  INT nStudentID,
									  BOOL			bIsTeacherActive,
									  IKRelation**	ppRelation)
{
	BOOL						bRetCode			= FALSE;
	KRole*						pTeacher			= NULL;
	KRole*						pStudent			= NULL;

	LOG_PROCESS_ERROR(nTeacherID > 0 && nStudentID > 0);

	pTeacher = g_pGameCenter->m_RoleManager.GetRole(nTeacherID);
	pStudent = g_pGameCenter->m_RoleManager.GetRole(nStudentID);
	LOG_PROCESS_ERROR(pTeacher);
	LOG_PROCESS_ERROR(pStudent);

	bRetCode = SUPER_CLASS::CreateRelation(nTeacherID, nStudentID, bIsTeacherActive, ppRelation);
EXIT0:
	return bRetCode;

// 		KGC_CONFIRM_RET_FALSE(nTeacherID > 0 && nStudentID > 0);
// 		KGC_Player* pGCPlayerTeacher = g_pPlayerManager->GetPlayer(nTeacherID);
// 		KGC_Player* pGCPlayerStudent = g_pPlayerManager->GetPlayer(nStudentID);
// 		KGC_CONFIRM_RET_FALSE(pGCPlayerTeacher && pGCPlayerStudent);
// 
// 		if (!SUPER_CLASS::CreateRelation(nTeacherID, nStudentID, bIsTeacherActive, ppRelation))
// 			return FALSE;
	// 修改本月收徒数目
// 	INT nMonthCode = GetMonthCode();
// 	if (pGCPlayerTeacher->GetTrainingStudentMonth() == nMonthCode)
// 	{
// 		INT nCount = pGCPlayerTeacher->GetTrainingStudentCount();
// 		pGCPlayerTeacher->SetTrainingStudentCount(nCount + 1);
// 	}
// 	else
// 	{
// 		pGCPlayerTeacher->SetTrainingStudentMonth(nMonthCode);
// 		pGCPlayerTeacher->SetTrainingStudentCount(1);
// 	}
//	return TRUE;
}

BOOL KTrainingFactory::CanCreateRelation(INT nTeacherID, INT nStudentID)
{
	BOOL					bRetCode			= FALSE;
	KRole*					pTeacher			= NULL;
	KRole*					pStudent			= NULL;
	INT						nTeacherErr			= emKRELATION_TRAINING_ERR_NONE;
	INT						nStudentErr			= emKRELATION_TRAINING_ERR_NONE;
	IMemBlock*				piMemBlock			= NULL;
	R2S_RELATION_TRAINING_ERROR_RESPOND* pNotify	= NULL;

	LOG_PROCESS_ERROR(nTeacherID && nStudentID);
	pTeacher = g_pGameCenter->m_RoleManager.GetRole(nTeacherID);
	pStudent = g_pGameCenter->m_RoleManager.GetRole(nStudentID);
	LOG_PROCESS_ERROR(pTeacher);
	LOG_PROCESS_ERROR(pStudent);

	if (!CheckQualification(nTeacherID, nStudentID, &nTeacherErr, &nStudentErr))
	{
		piMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_TRAINING_ERROR_RESPOND));
		LOG_PROCESS_ERROR(piMemBlock);

		pNotify = (R2S_RELATION_TRAINING_ERROR_RESPOND*)piMemBlock->GetData();
		pNotify->wProtocolID = r2s_relation_training_error_respond;
		// 把错误通知发送给师父方
		if (nTeacherErr != emKRELATION_TRAINING_ERR_NONE &&
			pTeacher->m_nConnIndex > 0)
		{
			pNotify->nPlayerID = nTeacherID;
			pNotify->byError = nTeacherErr;
			g_pGameCenter->m_GameServer.Send(pTeacher->m_nConnIndex, piMemBlock);
		}
		// 把错误通知发送给弟子方
		if (nStudentErr != emKRELATION_TRAINING_ERR_NONE &&
			pStudent->m_nConnIndex > 0)
		{
			pNotify->wProtocolID = nStudentID;
			pNotify->byError = nStudentErr;
			g_pGameCenter->m_GameServer.Send(pStudent->m_nConnIndex, piMemBlock);
		}
		bRetCode = FALSE;
	}
	else
	{
		bRetCode = SUPER_CLASS::CanCreateRelation(nTeacherID,nStudentID);
	}

EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;

	// 检查建立师徒关系的条件
// 	KGC_Player* pGCPlayerTeacher = g_pPlayerManager->GetPlayer(nTeacherID);
// 	KGC_Player* pGCPlayerStudent = g_pPlayerManager->GetPlayer(nStudentID);
// 	KGC_CONFIRM_RET_FALSE(pGCPlayerTeacher && pGCPlayerStudent);
// 
// 	INT nTeacherErr = emKRELATION_TRAINING_ERR_NONE;
// 	INT nStudentErr = emKRELATION_TRAINING_ERR_NONE;
// 	INT nTeacherSvrID = pGCPlayerTeacher->GetOnlineServer();
// 	INT nStudentSvrID = pGCPlayerStudent->GetOnlineServer();
// 
// 	if (!CheckQualification(nTeacherID, nStudentID, &nTeacherErr, &nStudentErr))
// 	{
// 		R2S_RELATION_TRAINING_ERROR_RESPOND sNotify;
// 		sNotify.ProtocolType = r2s_relation_training_error_respond;
// 		// 把错误通知发送给师父方
// 		if (nTeacherErr != emKRELATION_TRAINING_ERR_NONE &&
// 			nTeacherSvrID > 0)
// 		{
// 			sNotify.nPlayerID = nTeacherID;
// 			sNotify.byError = nTeacherErr;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nTeacherSvrID);
// 		}
// 		// 把错误通知发送给弟子方
// 		if (nStudentErr != emKRELATION_TRAINING_ERR_NONE &&
// 			nStudentSvrID > 0)
// 		{
// 			sNotify.nPlayerID = nStudentID;
// 			sNotify.byError = nStudentErr;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nStudentSvrID);
// 		}
// 		return FALSE;
// 	}
// 	return SUPER_CLASS::CanCreateRelation(nTeacherID,nStudentID);
}
// 检查建立师徒的条件
BOOL KTrainingFactory::CheckQualification(INT nTeacherId, INT nStudentId, LPINT pnTeacherErr, LPINT pnStudentErr)
{
	KRole* pPlayerTeacher = g_pGameCenter->m_RoleManager.GetRole(nTeacherId);
	KRole* pPlayerStudent = g_pGameCenter->m_RoleManager.GetRole(nStudentId);
	if (!pPlayerStudent || !pPlayerTeacher)
		return FALSE;

	// 判断普通好友关系
	IKRelationFactory* pFriendFactory = NULL;
	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory))
		return FALSE;

	if (!(pFriendFactory->FindRelation(nTeacherId, nStudentId) &&
		pFriendFactory->FindRelation(nStudentId, nTeacherId)))
		return FALSE;

	return 	CheckTeacher(nTeacherId, pnTeacherErr, pnStudentErr) && CheckStudent(nStudentId, pnTeacherErr, pnStudentErr);
}

// 检查师父和弟子之间是否满足一定的条件
BOOL KTrainingFactory::CheckTeacherWithStudent(KGC_Player* pTeacher,
											   KGC_Player* pStudent,
											   LPINT		pnTeacherErr,
											   LPINT		pnStudentErr)
{
	assert(pTeacher && pStudent);
	// 师父与弟子的等级差必须在30级以上
	/*if (pTeacher->GetLevel() - pStudent->GetLevel() <= g_cTrainingSetting.GetGapMinLevel())
	{
		if (pnTeacherErr)
			*pnTeacherErr = emKRELATION_TRAINING_ERR3;
		if (pnStudentErr)
			*pnStudentErr = emKRELATION_TRAINING_ERR4;
		return FALSE;
	}
	*/
	return TRUE;
}

// 检查弟子的资格
BOOL KTrainingFactory::CheckStudent(INT nStudentId,
									LPINT		pnTeacherErr,
									LPINT		pnStudentErr)
{
	BOOL						bRetCode			= FALSE;
	KRole*						pStudent			= NULL;
	INT							nTeacherErr			= emKRELATION_TRAINING_ERR_NONE;
	INT							nStudentErr			= emKRELATION_TRAINING_ERR_NONE;
	IKEnumRelation*				pEnum				= NULL;
	IKRelationFactory*			pFactory			= NULL;

	pStudent = g_pGameCenter->m_RoleManager.GetRole(nStudentId);
	LOG_PROCESS_ERROR(pStudent);

	if (!pStudent->IsOnline())
	{
		nTeacherErr = emKRELATION_TRAINING_ERR_OFFLINE;
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 先关闭条件判断
	bRetCode = TRUE;
	goto EXIT0;

	// 弟子必须高于20级（含20级）
	if (pStudent->GetLevel() < g_cTrainingSetting.GetStudentMinLevel())
	{
		nStudentErr	= emKRELATION_TRAINING_ERR3;
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 弟子等级必须低于50级（不含50级）
	if (pStudent->GetLevel() >= g_cTrainingSetting.GetStudentMaxLevel())
	{
		nTeacherErr = emKRELATION_TRAINING_ERR5;
		nStudentErr = emKRELATION_TRAINING_ERR6;
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 弟子必须现在没有师父且没有出师记录
	if (GetAllRelations(false, nStudentId, &pEnum) && pEnum->GetCount() > 0)
	{
		nTeacherErr = emKRELATION_TRAINING_ERR13;
		bRetCode = FALSE;
		goto EXIT0;
	}

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pFactory);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFactory);

	if (pFactory->GetAllRelations(false, nStudentId, &pEnum) && pEnum->GetCount() > 0)
	{
		nTeacherErr = emKRELATION_TRAINING_ERR14;
		bRetCode = FALSE;
		goto EXIT0;
	}

	bRetCode = TRUE;

EXIT0:
	if (!bRetCode)
	{
		if (pnTeacherErr)
			*pnTeacherErr = nTeacherErr;
		if (pnStudentErr)
			*pnStudentErr = nStudentErr;
	}
	return bRetCode;


// 	KGC_Player* pPlayer = g_pPlayerManager->GetPlayer(nStudentId);
// 	KGC_CONFIRM_RET_FALSE(pPlayer);
// 
// 	BOOL bRet = FALSE;
// 	INT nTeacherErr = emKRELATION_TRAINING_ERR_NONE;
// 	INT nStudentErr = emKRELATION_TRAINING_ERR_NONE;
// 	do
// 	{
// 		// 是否在线
// 		if (pPlayer->GetOnlineServer() <= 0)
// 		{
// 			nTeacherErr = emKRELATION_TRAINING_ERR_OFFLINE;
// 			break;
// 		}
// 		CONST GUID* pGuid = pPlayer->GetGUID();
// 		INT nLevel = pPlayer->GetLevel();
// 		// 弟子必须高于20级（含20级）
// 		if (nLevel < g_cTrainingSetting.GetStudentMinLevel())
// 		{
// 			//KGC_Error("");
// 			nStudentErr	= emKRELATION_TRAINING_ERR3;
// 			break;
// 		}
// 		// 弟子等级必须低于50级（不含50级）
// 		if (nLevel >= g_cTrainingSetting.GetStudentMaxLevel())
// 		{
// 			nTeacherErr = emKRELATION_TRAINING_ERR5;
// 			nStudentErr = emKRELATION_TRAINING_ERR6;
// 			break;
// 		}
// 		// 弟子必须现在没有师父且没有出师记录
// 		IKEnumRelation* pEnum = NULL;
// 		if (GetAllRelations(false, nStudentId, &pEnum) && pEnum->GetCount() > 0)
// 		{
// 			nTeacherErr = emKRELATION_TRAINING_ERR13;
// 			break;
// 		}
// 		IKRelationFactory* pFactory = NULL;
// 		if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pFactory))
// 		{
// 			KGC_Error("");
// 			break;
// 		}
// 		if (pFactory->GetAllRelations(false, nStudentId, &pEnum) && pEnum->GetCount() > 0)
// 		{
// 			nTeacherErr = emKRELATION_TRAINING_ERR14;
// 			break;
// 		}
// 		bRet = TRUE;
// 	}
// 	while (0);
// 	
// 	if (!bRet)
// 	{
// 		if (pnTeacherErr)
// 			*pnTeacherErr = nTeacherErr;
// 		if (pnStudentErr)
// 			*pnStudentErr = nStudentErr;
// 	}
// 	return bRet;
}

// 检查师父的资格
BOOL KTrainingFactory::CheckTeacher(INT nTeacherId,
									LPINT		pnTeacherErr,
									LPINT		pnStudentErr)
{
	BOOL						bRetCode		= FALSE;
	KRole*						pTeacher		= NULL;
	INT							nStudentErr		= emKRELATION_TRAINING_ERR_NONE;
	INT							nTeacherErr		= emKRELATION_TRAINING_ERR_NONE;
	INT							nMonthCode		= 0;
	IKEnumRelation*				pEnum			= NULL;

	pTeacher = g_pGameCenter->m_RoleManager.GetRole(nTeacherId);
	LOG_PROCESS_ERROR(pTeacher);
	
	// 是否在线
	if (!pTeacher->IsOnline())
	{
		nStudentErr = emKRELATION_TRAINING_ERR_OFFLINE;
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 关闭限制条件
	bRetCode = TRUE;
	goto EXIT0;

	// 本月收徒数目限制
// 	nMonthCode = GetMonthCode();
// 	if (pPlayer->GetTrainingStudentMonth() == nMonthCode &&
// 		pPlayer->GetTrainingStudentCount() >= g_cTrainingSetting.GetMonthlyStudentMaxCount())
// 	{
// 		nStudentErr = emKRELATION_TRAINING_ERR11;
// 		nTeacherErr = emKRELATION_TRAINING_ERR12;
// 		break;
// 	}

	// 师父等级必须不低于69级
	if (pTeacher->GetLevel() < g_cTrainingSetting.GetTeacherMinLevel())
	{
		nStudentErr = emKRELATION_TRAINING_ERR4;
		bRetCode = FALSE;
		goto EXIT0;
	}
	// 师父当前弟子数量必须小于3
	if (GetAllRelations(true, nTeacherId, &pEnum) &&
		pEnum->GetCount() >= g_cTrainingSetting.GetCurrentStudentMaxCount())
	{
		nTeacherErr = emKRELATION_TRAINING_ERR1;
		nStudentErr = emKRELATION_TRAINING_ERR2;
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 师父不能是别人的当前弟子
	if (GetAllRelations(false, nTeacherId, &pEnum) &&
		pEnum->GetCount() > 0)
	{
		nTeacherErr = emKRELATION_TRAINING_ERR7;
		nStudentErr = emKRELATION_TRAINING_ERR8;
		bRetCode = FALSE;
		goto EXIT0;
	}

	bRetCode = TRUE;

EXIT0:
	if (!bRetCode)
	{
		if (pnStudentErr)
			*pnStudentErr = nStudentErr;
		if (pnTeacherErr)
			*pnTeacherErr = nTeacherErr;
	}
	return bRetCode;

// 	KGC_Player* pPlayer = g_pPlayerManager->GetPlayer(nTeacherId);
// 	KGC_CONFIRM_RET_FALSE(pPlayer);
// 	BOOL bRet = FALSE;
// 	INT nStudentErr = emKRELATION_TRAINING_ERR_NONE;
// 	INT nTeacherErr = emKRELATION_TRAINING_ERR_NONE;
// 	do
// 	{
// 		// 检查“允许收徒”选项
// 		//if ((pPlayer->GetTrainingOption() & 0x2) == 0)
// 		//{
// 		//	nStudentErr = emKRELATION_TRAINING_ERR9;
// 		//	break;
// 		//}
// 		// 是否在线
// 		if (pPlayer->GetOnlineServer() <= 0)
// 		{
// 			nStudentErr = emKRELATION_TRAINING_ERR_OFFLINE;
// 			break;
// 		}
// 
// 		// 本月收徒数目限制
// 		INT nMonthCode = GetMonthCode();
// 		if (pPlayer->GetTrainingStudentMonth() == nMonthCode &&
// 			pPlayer->GetTrainingStudentCount() >= g_cTrainingSetting.GetMonthlyStudentMaxCount())
// 		{
// 			nStudentErr = emKRELATION_TRAINING_ERR11;
// 			nTeacherErr = emKRELATION_TRAINING_ERR12;
// 			break;
// 		}
// 		// 师父等级必须不低于69级
// 		if (pPlayer->GetLevel() < g_cTrainingSetting.GetTeacherMinLevel())
// 		{
// 			nStudentErr = emKRELATION_TRAINING_ERR4;
// 			break;
// 		}
// 		// 师父当前弟子数量必须小于3
// 		IKEnumRelation* pEnum = NULL;
// 		if (GetAllRelations(true, nTeacherId, &pEnum) &&
// 			pEnum->GetCount() >= g_cTrainingSetting.GetCurrentStudentMaxCount())
// 		{
// 			nTeacherErr = emKRELATION_TRAINING_ERR1;
// 			nStudentErr = emKRELATION_TRAINING_ERR2;
// 			break;
// 		}
// 		// 师父不能是别人的当前弟子
// 		if (GetAllRelations(false, nTeacherId, &pEnum) &&
// 			pEnum->GetCount() > 0)
// 		{
// 			nTeacherErr = emKRELATION_TRAINING_ERR7;
// 			nStudentErr = emKRELATION_TRAINING_ERR8;
// 			break;
// 		}
// 		
// 		bRet = TRUE;
// 	}
// 	while (0);
// 
// 	if (!bRet)
// 	{
// 		if (pnStudentErr)
// 			*pnStudentErr = nStudentErr;
// 		if (pnTeacherErr)
// 			*pnTeacherErr = nTeacherErr;
// 	}
// 	return bRet;
}

// 检查师父的资格
BOOL KTrainingFactory::CheckTeacherQualification(INT nTeacherId, LPINT pnTeacherErr, LPINT pnStudentErr)
{
	if (nTeacherId)
		return CheckTeacher(nTeacherId, pnTeacherErr, pnStudentErr);
	return FALSE;
}

// 检查弟子的资格
BOOL KTrainingFactory::CheckStudentQualification(INT nStudentId, LPINT pnTeacherErr, LPINT pnStudentErr)
{
	if (nStudentId)
		return CheckStudent(nStudentId, pnTeacherErr, pnStudentErr);
	return FALSE;
}

// 检查师父和弟子之间是否符合一定关系
BOOL KTrainingFactory::CheckRelation(KGC_Player* pTeacher, KGC_Player* pStudent, LPINT pnTeacherErr, LPINT pnStudentErr)
{
	if (pTeacher && pStudent)
		return CheckTeacherWithStudent(pTeacher, pStudent, pnTeacherErr, pnStudentErr);
	return FALSE;
}

BOOL KTrainingFactory::QueryFactory(INT nType, LPVOID * ppFactory)
{
	assert(nType >= 0 && nType < emKPLAERRELATION_FACTORY_SUBTYPE_COUNT && ppFactory);
	if (nType == emKPLAERRELATION_FACTORY_SUBTYPE_TRAINING)
	{
		*ppFactory = static_cast<IKTrainingFactory*>(this);
		return TRUE;
	}
	*ppFactory = NULL;
	return FALSE;
}

