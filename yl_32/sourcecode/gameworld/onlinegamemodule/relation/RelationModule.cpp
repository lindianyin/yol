

#include "stdafx.h"
#include "RelationModule.h"
#include "onlinegamemodule/relationprotocol.h"
#include "../gameworld/onlinegameworld/KRelayClient.h"
#include "onlinegameworld/kplayermgr.h"

QRelationModule g_RelationModule;

#define REGISTER_EELATION_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_anProtocolSizeC2S[ProtocolID] = ProtocolSize;}						


template<>
IKModuleInterface* GetModuleInterFace<emMODULE_RELATION>()
{
	return &g_RelationModule;
}


BYTE QRelationModule::m_byTempData[32*1024] = {0};


// ====================================================================================================

BOOL QRelationModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_DELRELATION);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_DEL_PLAYER);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_DELFAVOR);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_NOTIFYONLINE);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_NOTIFYRELATION);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_TRAININGAPPLY_TEACHER);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_TRAININGAPPLY_STUDENT);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_TRAININGOPTION);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_MYSELFLEFTCHECKINTIME);
	KD_PTC_FIXED_SIZE_S2C(KRELATION_PROTO_S2C_TRAININGAPPLYFORTEACHER);

	KD_PTC_LENGTNED_SIZE_S2C(KRELATION_PROTO_S2C_SYNCRELATION);
	KD_PTC_LENGTNED_SIZE_S2C(KRELATION_PROTO_S2C_START_SYNCRELATION);
	KD_PTC_LENGTNED_SIZE_S2C(KRELATION_PROTO_S2C_FRIENDINFO);
	KD_PTC_LENGTNED_SIZE_S2C(KRELATION_PROTO_S2C_SEARCHTEACHER);
	KD_PTC_LENGTNED_SIZE_S2C(KRELATION_PROTO_S2C_SEARCHSTUDENT);
	KD_PTC_LENGTNED_SIZE_S2C(KRELATION_PROTO_S2C_SUTDENTLEFTCHECKINTIME);

	return TRUE;
}

BOOL QRelationModule::OnInit()
{

	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_REQUESTTEACHER, &QRelationModule::C2SOnRequestTeacher, sizeof(KRELATION_PROTO_C2S_TRAININGREQUEST_TEACHER));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_REQUESTSTUDENT, &QRelationModule::C2SOnRequestStudent, sizeof(KRELATION_PROTO_C2S_TRAININGREQUEST_STUDENT));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_SEARCHTEACHER, &QRelationModule::C2SOnSearchTeacher, sizeof(KRELATION_PROTO_C2S_TRAININGSEARCH_TEACHER));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_SEARCHSTUDENT, &QRelationModule::C2SOnSearchStudent, sizeof(KRELATION_PROTO_C2S_TRAININGSEARCH_STUDENT));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_SETTRAININGOPTION, &QRelationModule::C2SOnSetTrainingOpt, sizeof(KRELATION_PROT_C2S_SETTRAININGOPTION));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_GETTRAININGOPTION, &QRelationModule::C2SOnGetTrainingOpt, sizeof(KRELATION_PROT_C2S_GETTRAININGOPTION));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_TRAININGVIEWINFO, &QRelationModule::C2SOnTrainingViewInfo, sizeof(KRELATION_PROTO_C2S_TRAININGVIEWINFO));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_TRAININGMAKEFRIEND, &QRelationModule::C2SOnTrainingMakeFriend, sizeof(KRELATION_PROTO_C2S_TRAININGMAKEFRIEND));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_TRAININGAPPLY, &QRelationModule::C2SOnTrainingApply, sizeof(KRELATION_PROTO_C2S_TRAININGAPPLY));
	REGISTER_EELATION_FUNC(emKRELATION_C2S_PROTO_WISHTEACHERCOMEHERE, &QRelationModule::C2SOnWishTheacherComeHere, sizeof(KRELATION_PROTO_C2S_WISHTHEACHERCOMEHERE));

	return TRUE;
}

BOOL QRelationModule::OnUnInit()
{
	return TRUE;
}

BOOL QRelationModule::ProcessProtocol( INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen )
{
	BOOL bResult = FALSE;
	PROCESS_PROTOCOL_FUNC       pFunc       = NULL;
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pbData;
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pbData);
	LOG_PROCESS_ERROR(uDataLen);

	pFunc = m_ProcessProtocolFuns[pHeader->byProtocol];
	LOG_PROCESS_ERROR(pFunc);

	(*pFunc)(pPlayer, (CHAR*)pbData, uDataLen);


	bResult = TRUE;
EXIT0:
	return bResult;
}

QRelationModule::QRelationModule( void ) : KModuleBase(emMODULE_RELATION)
{

}

QRelationModule::~QRelationModule( void )
{

}

BOOL QRelationModule::SendToClient( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return g_RelationModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL QRelationModule::SendLenToClient( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_RelationModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL QRelationModule::SendToCordinator( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}

// ====================================================================================================

void QRelationModule::C2SOnRequestTeacher(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                            bRet        = FALSE;
	KRELATION_PROTO_C2S_TRAININGREQUEST_TEACHER*	pMsg		= (KRELATION_PROTO_C2S_TRAININGREQUEST_TEACHER*)pData;
	S2R_RELATION_ADD_TRAINING_REQUEST*			pRequest	= NULL;
	IMemBlock*										pMemBlock	= NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROTO_C2S_TRAININGREQUEST_TEACHER));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_ADD_TRAINING_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_ADD_TRAINING_REQUEST*)pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_add_teacher_request;
	pRequest->nPlayerID = pPlayer->GetId();
	strncpy(pRequest->szTarget, pMsg->szTarget, _NAME_LEN - 1);
	pRequest->szTarget[_NAME_LEN - 1] = 0;
	
	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnRequestStudent(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                         bRet      = FALSE;
	KRELATION_PROTO_C2S_TRAININGREQUEST_STUDENT* pMessage  = (KRELATION_PROTO_C2S_TRAININGREQUEST_STUDENT*)pData;
	S2R_RELATION_ADD_TRAINING_REQUEST*         pRequest = (S2R_RELATION_ADD_TRAINING_REQUEST*)pData;
	IMemBlock*                                   pMemBlock = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROTO_C2S_TRAININGREQUEST_STUDENT));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_ADD_TRAINING_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_ADD_TRAINING_REQUEST*)pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_add_student_request;
	pRequest->nPlayerID = pPlayer->GetId();
	strncpy(pRequest->szTarget, pMessage->szTarget, _NAME_LEN);
	pRequest->szTarget[_NAME_LEN - 1] = 0;

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

// KRELATION_PROTO_C2S_TRAININGSEARCH_TEACHER
void QRelationModule::C2SOnSearchTeacher(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                         bRet      = FALSE;
	KRELATION_PROTO_C2S_TRAININGSEARCH_TEACHER*  pMessage  = (KRELATION_PROTO_C2S_TRAININGSEARCH_TEACHER*)pData;
	S2R_RELATION_SEARCH_PLAYER_REQUEST*          pRequest = (S2R_RELATION_SEARCH_PLAYER_REQUEST*)pData;
	IMemBlock*                                   pMemBlock = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROTO_C2S_TRAININGSEARCH_TEACHER));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_SEARCH_PLAYER_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_SEARCH_PLAYER_REQUEST*)pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_search_teacher_request;
	pRequest->nPlayerID = pPlayer->GetId();

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnSearchStudent(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                         bRet      = FALSE;
	KRELATION_PROTO_C2S_TRAININGSEARCH_STUDENT*  pMessage  = (KRELATION_PROTO_C2S_TRAININGSEARCH_STUDENT*)pData;
	S2R_RELATION_SEARCH_PLAYER_REQUEST*          pRequest = (S2R_RELATION_SEARCH_PLAYER_REQUEST*)pData;
	IMemBlock*                                   pMemBlock = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROTO_C2S_TRAININGSEARCH_STUDENT));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_SEARCH_PLAYER_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_SEARCH_PLAYER_REQUEST*)pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_search_student_request;
	pRequest->nPlayerID = pPlayer->GetId();

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnSetTrainingOpt(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                        bRet = FALSE;
	KRELATION_PROT_C2S_SETTRAININGOPTION*       pMessage = (KRELATION_PROT_C2S_SETTRAININGOPTION*)pData;
	S2R_RELATION_SET_TRAINING_OPTION_REQUEST*      pRequest = NULL;
	IMemBlock*                                  pMemBlock = NULL;
	
	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROT_C2S_SETTRAININGOPTION));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_SET_TRAINING_OPTION_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_SET_TRAINING_OPTION_REQUEST*)pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_set_training_option_request;
	pRequest->byTeacherOrStudent = pMessage->byTeacherOrStudent;
	pRequest->byOption = pMessage->byOption;
	pRequest->nPlayerID = pPlayer->GetId();

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnGetTrainingOpt(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                      bRet     = FALSE;
	KRELATION_PROT_C2S_GETTRAININGOPTION*     pMessage = (KRELATION_PROT_C2S_GETTRAININGOPTION*)pData;
	S2R_RELATION_PLAYER_ID_REQUEST*             pRequest = NULL;
	IMemBlock*                                pMemBlock = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROT_C2S_GETTRAININGOPTION));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_PLAYER_ID_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_PLAYER_ID_REQUEST*) pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_get_training_option_request;
	pRequest->nPlayerID = pPlayer->GetId();

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnTrainingViewInfo(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                      bRet     = FALSE;
	KRELATION_PROTO_C2S_TRAININGVIEWINFO*     pMessage = (KRELATION_PROTO_C2S_TRAININGVIEWINFO*)pData;
	S2R_RELATION_TRAINING_VIEW_INFO_REQUEST*     pRequest = NULL;
	IMemBlock*                                pMemBlock = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROTO_C2S_TRAININGVIEWINFO));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_TRAINING_VIEW_INFO_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_TRAINING_VIEW_INFO_REQUEST*) pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_training_view_info_request;
	pRequest->nPlayerID = pPlayer->GetId();
	strncpy(pRequest->szApplicant, pMessage->szApplicant, sizeof(pRequest->szApplicant));
	pRequest->szApplicant[sizeof(pRequest->szApplicant) - 1] = 0;

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnTrainingMakeFriend(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                      bRet     = FALSE;
	KRELATION_PROTO_C2S_TRAININGMAKEFRIEND*     pMessage = (KRELATION_PROTO_C2S_TRAININGMAKEFRIEND*)pData;
	S2R_RELATION_TRAINING_MAKE_FRIEND_REQUEST*     pRequest = NULL;
	IMemBlock*                                pMemBlock = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROTO_C2S_TRAININGMAKEFRIEND));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_TRAINING_MAKE_FRIEND_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_TRAINING_MAKE_FRIEND_REQUEST*) pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_training_make_friend_request;
	pRequest->nPlayerID = pPlayer->GetId();
	strncpy(pRequest->szRelation, pMessage->szRelation, sizeof(pRequest->szRelation));
	pRequest->szRelation[sizeof(pRequest->szRelation) - 1] = 0;

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnTrainingApply(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                      bRet     = FALSE;
	KRELATION_PROTO_C2S_TRAININGAPPLY*     pMessage = (KRELATION_PROTO_C2S_TRAININGAPPLY*)pData;
	S2R_RELATION_TRAINING_APPLY_REQUEST*     pRequest = NULL;
	IMemBlock*                                pMemBlock = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(KRELATION_PROTO_C2S_TRAININGAPPLY));

	pMemBlock = QCreateMemBlock(sizeof(S2R_RELATION_TRAINING_APPLY_REQUEST));
	LOG_PROCESS_ERROR(pMemBlock);

	pRequest = (S2R_RELATION_TRAINING_APPLY_REQUEST*) pMemBlock->GetData();
	pRequest->wProtocolID = s2r_relation_training_apply_request;
	pRequest->nPlayerID = pPlayer->GetId();
	strncpy(pRequest->szTeacher, pMessage->szTeacher, sizeof(pRequest->szTeacher));
	pRequest->szTeacher[sizeof(pRequest->szTeacher) - 1] = 0;

	bRet = SendToCordinator(pMemBlock);
	LOG_PROCESS_ERROR(bRet);

EXIT0:
	SAFE_RELEASE(pMemBlock);
	return;
}

void QRelationModule::C2SOnWishTheacherComeHere(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
// 	emKRELATION_C2S_PROTO_HOPETHEACHERCOMEHERE *pMsg = (emKRELATION_C2S_PROTO_HOPETHEACHERCOMEHERE*)pData;
// 	BYTE byAgree = pMsg->byHopeTeacherComeHere;
// 	KPlayer *pPlayer = &Player[nPlayerIndex];
// 	VERIFY(g_pMainScript->CallTableFunction("Item", "StudentAccredit", 0, "sd", pPlayer->m_PlayerName, pMsg->byHopeTeacherComeHere));
}

// ================================================================================
void QRelationModule::R2SOnStartSyncRelation(BYTE* pbyData, size_t uDataLen)
{
	BOOL										bRetCode = false;
	R2S_RELATION_SYNC_LIST_RESPOND*						pMessage = (R2S_RELATION_SYNC_LIST_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_SYNCRELATION*			pRespond = (KRELATION_PROTO_S2C_SYNCRELATION*)m_byTempData;
	KPlayer*									pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_SYNC_LIST_RESPOND));

	if (pMessage->byNotifyClient != 0)
	{
		memset(pRespond, 0, sizeof(KRELATION_PROTO_S2C_SYNCRELATION));
		pRespond->byProtocol	= emKRELATION_S2C_PROTO_START_SYNCRELATION;
		pRespond->nType			= pMessage->nType;
		pRespond->nCount		= pMessage->nCount;
		pRespond->byCreate		= pMessage->byCreate;

		pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
		LOG_PROCESS_ERROR(pPlayer);

		bRetCode = SendLenToClient(pPlayer->GetPlayerIndex(), *pRespond, 
			sizeof(KRELATION_PROTO_S2C_SYNCRELATION));
		LOG_PROCESS_ERROR(bRetCode);
	}

EXIT0:
	return;
}


void QRelationModule::R2SOnSyncRelation(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_SYNC_LIST_RESPOND*                pMessage = (R2S_RELATION_SYNC_LIST_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_SYNCRELATION*    pRespond = (KRELATION_PROTO_S2C_SYNCRELATION*)m_byTempData;
	KPlayer*                             pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + sizeof(KRELATION_INFO) * pMessage->nCount);

	if (pMessage->byNotifyClient != 0)
	{
		pRespond->byProtocol = emKRELATION_S2C_PROTO_SYNCRELATION;
		pRespond->nType = pMessage->nType;
		pRespond->nRole = pMessage->nRole;
		pRespond->nCount = pMessage->nCount;
		pRespond->byCreate = pMessage->byCreate;

		memcpy(pRespond->aryInf, pMessage->aryInfo, sizeof(KRELATION_INFO) * pMessage->nCount);

		pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
		LOG_PROCESS_ERROR(pPlayer);

		bRetCode = SendLenToClient(pPlayer->GetPlayerIndex(), *pRespond, 
			sizeof(KRELATION_PROTO_S2C_SYNCRELATION) + sizeof(KRELATION_INFO) * pMessage->nCount);
		LOG_PROCESS_ERROR(bRetCode);
	}

EXIT0:
	return;

// 	R2S_RELATION_SYNC_LIST_RESPOND *pMsg = (R2S_RELATION_SYNC_LIST_RESPOND*)pData;
// 	if (sizeof(R2S_RELATION_SYNC_LIST_RESPOND) > nSize ||
// 		pMsg->nCount == 0 ||
// 		sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + pMsg->nCount * sizeof(KRELATION_INFO) != nSize)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIndex = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIndex <= 0)
// 	{
// 		// 不在此GameServer上或者已经断线
// 		return;
// 	}
// 
// 	// 同步GameServer上的关系列表
// 	KSyncRelationList::Instance()->SyncRelationList(Player[nPlayerIndex].GetPlayerName(), pMsg);
// 
// 	// 同步到客户端
// 	if (pMsg->byNotifyClient != 0)
// 	{
// 		INT nBufLen = sizeof(KRELATION_PROTO_S2C_SYNCRELATION) + pMsg->nCount * sizeof(KRELATION_INFO);
// 		CHAR *pBuf = (CHAR*)alloca(nBufLen);
// 		if (!pBuf)
// 		{
// 			ASSERT(FALSE);
// 			return;
// 		}
// 		KRELATION_PROTO_S2C_SYNCRELATION *pCmd = (KRELATION_PROTO_S2C_SYNCRELATION*)pBuf;
// 		pCmd->ProtocolType = s2c_playerrelation;
// 		pCmd->nSize = nBufLen - 1;
// 		pCmd->nCmd = emKRELATION_S2C_PROTO_SYNCRELATION;
// 		pCmd->nCount = pMsg->nCount;
// 		pCmd->nRole = pMsg->nRole;
// 		pCmd->nType = pMsg->nType;
// 		memcpy(pCmd->aryInf, pMsg->aryInfo, pMsg->nCount * sizeof(KRELATION_INFO));
// 		g_pCoreServer->SendData(Player[nPlayerIndex].m_nNetConnectIdx, pBuf, nBufLen);
// 	}
}


void QRelationModule::R2SOnDelRelation(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_DEL_RELATION_RESPOND*     pMessage = (R2S_RELATION_DEL_RELATION_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_DELRELATION      cRespond;
	KPlayer*                             pPlayer  = NULL;

	cRespond.byProtocol = emKRELATION_S2C_PROTO_DELRELATION;
	cRespond.nType = pMessage->nType;
	cRespond.nRole = pMessage->nRelationRole;
	strncpy(cRespond.szRelation, pMessage->szRelation, sizeof(pMessage->szRelation));
	cRespond.szRelation[sizeof(cRespond.szRelation) - 1] = 0;

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
	LOG_PROCESS_ERROR(pPlayer);

EXIT0:
	return;

// 	R2S_RELATION_DEL_RELATION_RESPOND *pMsg = (R2S_RELATION_DEL_RELATION_RESPOND*)pData;
// 	if (nSize != sizeof(R2S_RELATION_DEL_RELATION_RESPOND))
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIndex = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIndex <= 0)
// 		return;
// 
// 	// 同步删除的关系
// 	KPlayer *pPlayer = &Player[nPlayerIndex];
// 	KSyncRelationList::Instance()->DelRelation(
// 		pPlayer->GetPlayerName(),
// 		pMsg->szRelation,
// 		pMsg->nType);
// 
// 	KRELATION_PROTO_S2C_DELRELATION sNotify;
// 	sNotify.ProtocolType = s2c_playerrelation;
// 	sNotify.nSize = sizeof(sNotify) - 1;
// 	sNotify.nCmd = emKRELATION_S2C_PROTO_DELRELATION;
// 	sNotify.nType = pMsg->nType;
// 	sNotify.nRole = pMsg->nRelationRole;
// 	strncpy(sNotify.szRelation, pMsg->szRelation, sizeof(sNotify.szRelation) - 1);
// 	sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 	g_pCoreServer->SendData(Player[nPlayerIndex].m_nNetConnectIdx, &sNotify, sizeof(sNotify));
}

void QRelationModule::R2SOnDeletePlayer(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_DEL_RELATION_RESPOND*     pMessage = (R2S_RELATION_DEL_RELATION_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_DELRELATION      cRespond;
	KPlayer*                             pPlayer  = NULL;

	cRespond.byProtocol = emKRELATION_S2C_PROTO_DELRELATION;
	cRespond.nType = pMessage->nType;
	cRespond.nRole = pMessage->nRelationRole;
	strncpy(cRespond.szRelation, pMessage->szRelation, sizeof(pMessage->szRelation));
	cRespond.szRelation[sizeof(cRespond.szRelation) - 1] = 0;

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
	LOG_PROCESS_ERROR(pPlayer);

EXIT0:
	return;

// 	R2S_RELATION_DEL_RELATION_RESPOND *pMsg = (R2S_RELATION_DEL_RELATION_RESPOND*)pData;
// 	if (nSize != sizeof(R2S_RELATION_DEL_RELATION_RESPOND))
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIndex = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIndex <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	// 同步删除的关系
// 	KPlayer *pPlayer = &Player[nPlayerIndex];
// 	KSyncRelationList::Instance()->DelRelation(
// 		pPlayer->GetPlayerName(),
// 		pMsg->szRelation,
// 		pMsg->nType);
// 
// 	KRELATION_PROTO_S2C_DELRELATION sNotify;
// 	sNotify.ProtocolType = s2c_playerrelation;
// 	sNotify.nSize = sizeof(sNotify) - 1;
// 	sNotify.nCmd = emKRELATION_S2C_PROTO_DELETEPLAYER;
// 	sNotify.nType = pMsg->nType;
// 	sNotify.nRole = pMsg->nRelationRole;
// 	strncpy(sNotify.szRelation, pMsg->szRelation, sizeof(sNotify.szRelation) - 1);
// 	sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 	g_pCoreServer->SendData(Player[nPlayerIndex].m_nNetConnectIdx, &sNotify, sizeof(sNotify));
}

void QRelationModule::R2SOnFriendInfo(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_FRIEND_INFO_RESPOND*      pMessage = (R2S_RELATION_FRIEND_INFO_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_FRIENDINFO*      pRespond = (KRELATION_PROTO_S2C_FRIENDINFO*)m_byTempData;
	KPlayer*                             pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO) * pMessage->nCount);

	pRespond->byProtocol = emKRELATION_S2C_PROTO_FRIENDINFO;
	pRespond->nCount = pMessage->nCount;
	memcpy(pRespond->aryInfo, pMessage->aryInfo, sizeof(KRELATION_PROTO_FRIENDINFO) * pMessage->nCount);

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendLenToClient(pPlayer->GetPlayerIndex(), *pRespond, 
		sizeof(KRELATION_PROTO_S2C_FRIENDINFO) + sizeof(KRELATION_PROTO_FRIENDINFO) * pMessage->nCount);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;


// 	R2S_RELATION_FRIEND_INFO_RESPOND *pMsg = (R2S_RELATION_FRIEND_INFO_RESPOND*)pData;
// 	if (nSize <= sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) ||
// 		nSize != sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO) * pMsg->nCount)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIndex = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIndex <= 0)
// 		return;	// 玩家掉线？
// 
// 	// 同步好友信息
// 	LPCSTR pszPlayerName = Player[nPlayerIndex].GetPlayerName();
// 	KSyncRelationList::Instance()->SyncFriendList(pszPlayerName, pMsg);
// 
// 	INT nBufLen = sizeof(KRELATION_PROTO_S2C_FRIENDINFO) + sizeof(KRELATION_PROTO_FRIENDINFO) * pMsg->nCount;
// 	KRELATION_PROTO_S2C_FRIENDINFO *pSync = (KRELATION_PROTO_S2C_FRIENDINFO*)alloca(nBufLen);
// 	if (!pSync)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	memcpy(pSync->aryInfo, pMsg->aryInfo, sizeof(KRELATION_PROTO_FRIENDINFO) * pMsg->nCount);
// 	pSync->ProtocolType = s2c_playerrelation;
// 	pSync->nCount = pMsg->nCount;
// 	pSync->nCmd = emKRELATION_S2C_PROTO_FRIENDINFO;
// 	pSync->nSize = nBufLen - 1;
// 	g_pCoreServer->SendData(Player[nPlayerIndex].m_nNetConnectIdx, pSync, nBufLen);
}

void QRelationModule::R2SOnNotifyOnline(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_ONLINE_NOTIFY*    pMessage = (R2S_RELATION_ONLINE_NOTIFY*)pbyData;
	KRELATION_PROTO_S2C_NOTIFYONLINE     cRespond;
	KPlayer*                             pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_ONLINE_NOTIFY) + sizeof(INT) * pMessage->nCount);
	cRespond.byProtocol = emKRELATION_S2C_PROTO_NOTIFYONLINE;
	cRespond.nRelationType = pMessage->nType;
	cRespond.nRole = pMessage->nRole;
	cRespond.nOnline = pMessage->nOnline;
	strncpy(cRespond.szRelation, pMessage->szName, sizeof(pMessage->szName));

	for (int i = 0; i < pMessage->nCount; ++i)
	{
		pPlayer = g_cPlayerMgr.GetById(pMessage->aryRelationID[i]);
		if (pPlayer)
		{
			bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}

EXIT0:
	return;

// 	R2S_RELATION_ONLINE_NOTIFY *pMsg = (R2S_RELATION_ONLINE_NOTIFY*)pData;
// 	if (nSize <= sizeof(R2S_RELATION_ONLINE_NOTIFY) ||
// 		nSize != sizeof(R2S_RELATION_ONLINE_NOTIFY) + pMsg->nCount * sizeof(INT) ||
// 		pMsg->nCount == 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 
// 	// 关系玩家上下线通知
// 	KRELATION_PROTO_S2C_NOTIFYONLINE sNotify;
// 	sNotify.ProtocolType = s2c_playerrelation;
// 	sNotify.nCmd = emKRELATION_S2C_PROTO_NOTIFYONLINE;
// 	sNotify.nSize = sizeof(sNotify) - 1;
// 	sNotify.nRole = pMsg->nRole;
// 	sNotify.nRelationType = pMsg->nType;
// 	sNotify.nOnline = pMsg->nOnline;
// 	strncpy(sNotify.szRelation, pMsg->szName, sizeof(sNotify.szRelation) - 1);
// 	sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 	for (INT i = 0; i < pMsg->nCount; i++)
// 	{
// 		INT nPlayerIndex = g_PlayerManager.FindLocalPlayer(pMsg->aryRelationID[i]);
// 		if (nPlayerIndex > 0)
// 		{
// 			g_pCoreServer->SendData(
// 				Player[nPlayerIndex].m_nNetConnectIdx,
// 				&sNotify,
// 				sizeof(sNotify));
// 		}
// 	}
}

void QRelationModule::R2SOnNotifyRelation(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_NOTIFY_RESPOND*  pMessage = (R2S_RELATION_NOTIFY_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_NOTIFYRELATION   cRespond;
	KPlayer*                             pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_NOTIFY_RESPOND));
	cRespond.byProtocol = emKRELATION_S2C_PROTO_NOTIFYRELATION;
	cRespond.nType = pMessage->nType;
	cRespond.nRole = pMessage->nRole;
	memcpy(cRespond.szRelation, pMessage->szRelation, sizeof(pMessage->szRelation));

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;

// 	R2S_RELATION_NOTIFY_RESPOND *pMsg = (R2S_RELATION_NOTIFY_RESPOND*)pData;
// 	if (nSize != sizeof(R2S_RELATION_NOTIFY_RESPOND))
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIndex = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIndex <= 0)
// 		return;
// 
// 	KRELATION_PROTO_S2C_NOTIFYRELATION sNotify;
// 	sNotify.byProtocol = emKRELATION_S2C_PROTO_NOTIFYRELATION;
// 	sNotify.ProtocolType = s2c_playerrelation;
// 	sNotify.nCmd = emKRELATION_S2C_PROTO_NOTIFYRELATION;
// 	sNotify.nSize = sizeof(sNotify) - 1;
// 	sNotify.nType = pMsg->nType;
// 	sNotify.nRole = pMsg->nRole;
// 	strncpy(sNotify.szRelation, pMsg->szRelation, sizeof(sNotify.szRelation) - 1);
// 	sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 	g_pCoreServer->SendData(Player[nPlayerIndex].m_nNetConnectIdx, &sNotify, sizeof(sNotify));
}

void QRelationModule::R2SOnTrainingError(BYTE* pbyData, size_t uDataLen)
{
// 	ASSERT(pData);
// 	static LPCSTR aryErrMsg[emKRELATION_TRAINING_ERR_COUNT] = {
// 		"",
// 		MSG_TRAINING_ERR_OFFLINE,
// 		MSG_TRAINING_ERR01,
// 		MSG_TRAINING_ERR02,
// 		MSG_TRAINING_ERR03,
// 		MSG_TRAINING_ERR04,
// 		MSG_TRAINING_ERR05,
// 		MSG_TRAINING_ERR06,
// 		MSG_TRAINING_ERR07,
// 		MSG_TRAINING_ERR08,
// 		MSG_TRAINING_ERR09,
// 		MSG_TRAINING_ERR10,
// 		MSG_TRAINING_ERR11,
// 		MSG_TRAINING_ERR12,
// 		MSG_TRAINING_ERR13,
// 		MSG_TRAINING_ERR14,
// 		MSG_TRAINING_ERR15,
// 		MSG_TRAINING_ERR16,
// 		MSG_TRAINING_ERR17,
// 		MSG_TRAINING_ERR18,
// 		MSG_TRAINING_ERR19,
// 		MSG_TRAINING_ERR20,
// 		MSG_TRAINING_ERR21,
// 	};
// 	KRELATION_PROTO_G2S_TRAININGERROR* pMsg = (KRELATION_PROTO_G2S_TRAININGERROR*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_G2S_TRAININGERROR) ||
// 		pMsg->byError >= emKRELATION_TRAINING_ERR_COUNT ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIdx <= 0)
// 		return;
// 	LPCSTR pszMessage = aryErrMsg[pMsg->byError];
// 	ASSERT(pszMessage && pszMessage[0] != 0);
// 	KPlayerChat::SendSystemInfo(emKSEND_MSG_PERSONAL,
// 		nPlayerIdx,
// 		K_MESSAGE_SYSTEM_MESSAGE_HEAD,
// 		pszMessage,
//		strlen(pszMessage));
}

void QRelationModule::R2SOnSearchStudent(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_SEARCH_STUDENT_RESPOND*   pMessage = (R2S_RELATION_SEARCH_STUDENT_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_SEARCHSTUDENT*   pRespond = (KRELATION_PROTO_S2C_SEARCHSTUDENT*)m_byTempData;
	KPlayer*                             pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_SEARCH_STUDENT_RESPOND) + sizeof(KRELATION_STUDENT) * pMessage->wCount);

	pRespond->byProtocol = emKRELATION_S2C_PROTO_SEARCHSTUDENT;
	pRespond->wCount = pMessage->wCount;
	memcpy(pRespond->aryResult, pMessage->aryResult, sizeof(KRELATION_STUDENT) * pMessage->wCount);

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendLenToClient(pPlayer->GetPlayerIndex(), *pRespond, 
		sizeof(KRELATION_PROTO_S2C_SEARCHSTUDENT) + sizeof(KRELATION_STUDENT) * pMessage->wCount);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;

// 	ASSERT(pData);
// 	R2S_RELATION_SEARCH_STUDENT_RESPOND* pResult = (R2S_RELATION_SEARCH_STUDENT_RESPOND*)pData;
// 	if (nSize < sizeof(R2S_RELATION_SEARCH_STUDENT_RESPOND) ||
// 		nSize != sizeof(R2S_RELATION_SEARCH_STUDENT_RESPOND) + pResult->wCount * sizeof(KRELATION_STUDENT) ||
// 		pResult->nPlayerID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pResult->nPlayerID);
// 	if (nPlayerIdx <= 0)
// 		return;
// 
// 	CHAR szBuf[4096];
// 	INT nBufLen = sizeof(KRELATION_PROTO_S2C_SEARCHSTUDENT) + pResult->wCount * sizeof(KRELATION_STUDENT);
// 	if (nBufLen > sizeof(szBuf))
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 
// 	KRELATION_PROTO_S2C_SEARCHSTUDENT* pNotify = (KRELATION_PROTO_S2C_SEARCHSTUDENT*)szBuf;
// 	pNotify->cProtocol	= s2c_playerrelation;
// 	pNotify->nCmd		= emKRELATION_S2C_PROTO_SEARCHSTUDENT;
// 	pNotify->nSize		= nBufLen - 1;
// 	pNotify->wCount		= pResult->wCount;
// 	//pNotify->nLastPlayerId	= pResult->nLastID;
// 	for (INT i = 0; i < pResult->wCount; i++)
// 	{
// 		pNotify->aryResult[i] = pResult->aryResult[i];
// 	}
// 	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, pNotify, nBufLen);
}

void QRelationModule::R2SOnSearchTeacher(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_SEARCH_TEACHER_RESPOND*   pMessage = (R2S_RELATION_SEARCH_TEACHER_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_SEARCHTEACHER*   pRespond = (KRELATION_PROTO_S2C_SEARCHTEACHER*)m_byTempData;
	KPlayer*                             pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_SEARCH_TEACHER_RESPOND) + sizeof(KRELATION_TEACHER) * pMessage->wCount);

	pRespond->byProtocol = emKRELATION_S2C_PROTO_SEARCHTEACHER;
	pRespond->wCount = pMessage->wCount;
	memcpy(pRespond->aryResult, pMessage->aryResult, sizeof(KRELATION_TEACHER) * pMessage->wCount);

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendLenToClient(pPlayer->GetPlayerIndex(), *pRespond, 
		sizeof(R2S_RELATION_SEARCH_TEACHER_RESPOND) + sizeof(KRELATION_TEACHER) * pMessage->wCount);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;

// 	ASSERT(pData);
// 	R2S_RELATION_SEARCH_TEACHER_RESPOND* pResult = (R2S_RELATION_SEARCH_TEACHER_RESPOND*)pData;
// 	if (nSize < sizeof(R2S_RELATION_SEARCH_TEACHER_RESPOND) ||
// 		nSize != sizeof(R2S_RELATION_SEARCH_TEACHER_RESPOND) + pResult->wCount * sizeof(KRELATION_TEACHER) ||
// 		pResult->nPlayerID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pResult->nPlayerID);
// 	if (nPlayerIdx <= 0)
// 		return;
// 
// 	CHAR szBuf[4096];
// 	INT nBufLen = sizeof(KRELATION_PROTO_S2C_SEARCHTEACHER) + pResult->wCount * sizeof(KRELATION_TEACHER);
// 	if (nBufLen > sizeof(szBuf))
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 
// 	KRELATION_PROTO_S2C_SEARCHTEACHER* pNotify = (KRELATION_PROTO_S2C_SEARCHTEACHER*)szBuf;
// 	pNotify->cProtocol	= s2c_playerrelation;
// 	pNotify->nCmd		= emKRELATION_S2C_PROTO_SEARCHTEACHER;
// 	pNotify->nSize		= nBufLen - 1;
// 	pNotify->wCount		= pResult->wCount;
// 	for (INT i = 0; i < pResult->wCount; i++)
// 	{
// 		pNotify->aryResult[i] = pResult->aryResult[i];
// 	}
// 	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, pNotify, nBufLen);
}

void QRelationModule::R2SOnTrainingOption(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                 bRetCode = false;
	R2S_RELATION_TRAINING_OPTION_RESPOND*   pMessage = (R2S_RELATION_TRAINING_OPTION_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_TRAININGOPTION   cRespond;
	KPlayer*                             pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_TRAINING_OPTION_RESPOND));

	cRespond.byProtocol = emKRELATION_S2C_PROTO_TRAININGOPTION;
	cRespond.nOption = pMessage->nOption;

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;

// 	ASSERT(pData);
// 	R2S_RELATION_TRAINING_OPTION_RESPOND* pMsg = (R2S_RELATION_TRAINING_OPTION_RESPOND*)pData;
// 	if (nSize != sizeof(R2S_RELATION_TRAINING_OPTION_RESPOND) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIdx <= 0)
// 		return;
// 	KRELATION_PROTO_S2C_TRAININGOPTION sNotify;
// 	sNotify.ProtocolType = s2c_playerrelation;
// 	sNotify.nCmd		 = emKRELATION_S2C_PROTO_TRAININGOPTION;
// 	sNotify.nOption		 = pMsg->nOption;
// 	sNotify.nSize		 = sizeof(sNotify) - 1;
// 	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, &sNotify, sizeof(sNotify));
}

void QRelationModule::R2SOnApplyingTeacher(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                        bRetCode = false;
	R2S_RELATION_ADD_TEACHER_STUDENT_RESPOND*          pMessage = (R2S_RELATION_ADD_TEACHER_STUDENT_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_TRAININGAPPLY_TEACHER   cRespond;
	KPlayer*                                    pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_ADD_TEACHER_STUDENT_RESPOND));

	cRespond.byProtocol = emKRELATION_S2C_PROTO_APPLYINGTEACHER;
	memcpy(cRespond.szApplicant, pMessage->szApplicant, sizeof(pMessage->szApplicant));

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;

// 	ASSERT(pData);
// 	R2S_RELATION_ADD_TRAINING_RESPOND* pMsg = (R2S_RELATION_ADD_TRAINING_RESPOND*)pData;
// 	if (nSize != sizeof(R2S_RELATION_ADD_TRAINING_RESPOND) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	pMsg->szApplicant[sizeof(pMsg->szApplicant) - 1] = 0;
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIdx <= 0)
// 		return;
// 	KRELATION_PROTO_S2C_TRAININGAPPLY sNotify;
// 	sNotify.ProtocolType = s2c_playerrelation;
// 	sNotify.nCmd = emKRELATION_S2C_PROTO_APPLYINGTEACHER;
// 	sNotify.nSize = sizeof(sNotify) - 1;
// 	strncpy(sNotify.szApplicant, pMsg->szApplicant, sizeof(sNotify.szApplicant) - 1);
// 	sNotify.szApplicant[sizeof(sNotify.szApplicant) - 1] = 0;
// 	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, &sNotify, sizeof(sNotify));
}

void QRelationModule::R2SOnApplyingStudent(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                        bRetCode = false;
	R2S_RELATION_ADD_TEACHER_STUDENT_RESPOND*          pMessage = (R2S_RELATION_ADD_TEACHER_STUDENT_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_TRAININGAPPLY_STUDENT   cRespond;
	KPlayer*                                    pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_ADD_TEACHER_STUDENT_RESPOND));

	cRespond.byProtocol = emKRELATION_S2C_PROTO_APPLYINGSTUDENT;
	memcpy(cRespond.szApplicant, pMessage->szApplicant, sizeof(pMessage->szApplicant));

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;

// 	ASSERT(pData);
// 	R2S_RELATION_ADD_TRAINING_RESPOND* pMsg = (R2S_RELATION_ADD_TRAINING_RESPOND*)pData;
// 	if (nSize != sizeof(R2S_RELATION_ADD_TRAINING_RESPOND) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	pMsg->szApplicant[sizeof(pMsg->szApplicant) - 1] = 0;
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 	if (nPlayerIdx <= 0)
// 		return;
// 	KRELATION_PROTO_S2C_TRAININGAPPLY sNotify;
// 	sNotify.ProtocolType = s2c_playerrelation;
// 	sNotify.nCmd = emKRELATION_S2C_PROTO_APPLYINGSTUDENT;
// 	sNotify.nSize = sizeof(sNotify) - 1;
// 	strncpy(sNotify.szApplicant, pMsg->szApplicant, sizeof(sNotify.szApplicant) - 1);
// 	sNotify.szApplicant[sizeof(sNotify.szApplicant) - 1] = 0;
// 	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, &sNotify, sizeof(sNotify));
}

void QRelationModule::R2SOnTrainingViewInfo(BYTE* pbyData, size_t uDataLen)
{
// 	ASSERT(pData);
// 	KRELATION_PROTO_G2S_TRAININGVIEWINFO* pMsg = (KRELATION_PROTO_G2S_TRAININGVIEWINFO*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_G2S_TRAININGVIEWINFO) ||
// 		pMsg->nViewerID <= 0 ||
// 		pMsg->nTargetID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nTargetID);
// 	if (nPlayerIdx <= 0)
// 	{
// 		// 玩家下线
// 		return;
// 	}
// 
// 	KPlayer* pPlayer = &Player[nPlayerIdx];
// 	KNpc* pNpc = &Npc[pPlayer->m_nIndex];
// 
// 	CHAR szBuff[sizeof(KRELATION_PROTO_S2G_PLAYERINFO) + sizeof(KVIEWITEMINFO) * (emEQUIPPOS_NUM + emPARTNER_NUM)];
// 	KRELATION_PROTO_S2G_PLAYERINFO* pInfo = (KRELATION_PROTO_S2G_PLAYERINFO*)szBuff;
// 	pInfo->ProtocolType = emKRELATION_S2G_PROTO_TRAININGPLAYERINFO;
// 	pInfo->nViewerID = pMsg->nViewerID;
// 	pPlayer->m_cPlayerItem.GetEquipView(&pInfo->sPlayerInfo);
// 	INT nProSize = sizeof(KRELATION_PROTO_S2G_PLAYERINFO) + sizeof(KVIEWITEMINFO) * pInfo->sPlayerInfo.byItemCount - 1;
// 	pInfo->Length = nProSize;		// 这个与GS与客户端的协议不同，不需要减去协议头的1个长度
// 
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nProSize, 0);
// 	memcpy(sBuf.GetBuf(), szBuff, nProSize);
// 	m_pProtocolProcessor->SendPacket(sBuf);
}

void QRelationModule::R2SOnTrainingRoleInfo(BYTE* pbyData, size_t uDataLen)
{
// 	ASSERT(pData);
// 	KRELATION_PROTO_G2S_PLAYERINFO* pMsg = (KRELATION_PROTO_G2S_PLAYERINFO*)pData;
// 	if (nSize != pMsg->Length || pMsg->nViewerID <= 0)
// 	{
// 		ASSERT(FALSE);
// 		return;
// 	}
// 	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nViewerID);
// 	if (nPlayerIdx <= 0)
// 		return;
// 
// 	CHAR szBuff[sizeof(KVIEW_EQUIP_SYNC) + sizeof(KVIEWITEMINFO) * (emEQUIPPOS_NUM + emPARTNER_NUM)];
// 	KVIEW_EQUIP_SYNC* pView = (KVIEW_EQUIP_SYNC*)szBuff;
// 	pView->ProtocolType = s2c_viewequip;
// 	INT nSendSize = pMsg->Length - sizeof(INT) - 1;		// 多了一个INT，客户端分析变长协议时没有计算协议头的那一个长度,再减1
// 	memcpy(&pView->sView, &pMsg->sPlayerInfo, nSendSize);
// 	pView->Length = nSendSize;
// 
// 	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, pView, pView->Length + 1);
}

void QRelationModule::R2SOnApplyForTeacher(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                          bRetCode = false;
	R2S_RELATION_ADD_TRAINING_RESPOND*  pMessage = (R2S_RELATION_ADD_TRAINING_RESPOND*)pbyData;
	KRELATION_PROTO_S2C_TRAININGAPPLYFORTEACHER   cRespond;
	KPlayer*                                      pPlayer  = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_RELATION_ADD_TRAINING_RESPOND));

	cRespond.byProtocol = emKRELATION_S2C_PROTO_TRAININGAPPLY;
	memcpy(cRespond.szApplicant, pMessage->szStudent, sizeof(pMessage->szStudent));

	pPlayer = g_cPlayerMgr.GetById(pMessage->nPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	bRetCode = SendToClient(pPlayer->GetPlayerIndex(), cRespond);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;

// 	do
// 	{
// 		R2S_RELATION_ADD_TRAINING_RESPOND* pMsg = (R2S_RELATION_ADD_TRAINING_RESPOND*)pData;
// 		if (nSize != sizeof(R2S_RELATION_ADD_TRAINING_RESPOND) ||
// 			pMsg->nPlayerID <= 0)
// 		{
// 			ASSERT(FALSE);
// 			break;
// 		}
// 		INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerID);
// 		if (nPlayerIdx <= 0)
// 			break;
// 
// 		pMsg->szStudent[sizeof(pMsg->szStudent) - 1] = '\0';
// 		KRELATION_PROTO_S2C_TRAININGAPPLYFORTEACHER sNotify;
// 		sNotify.cProtocol = s2c_playerrelation;
// 		sNotify.nCmd	  = emKRELATION_S2C_PROTO_TRAININGAPPLY;
// 		sNotify.nSize	  = sizeof(sNotify) - 1;
// 		strncpy(sNotify.szApplicant, pMsg->szStudent, sizeof(sNotify.szApplicant) - 1);
// 		sNotify.szApplicant[sizeof(sNotify.szApplicant) - 1] = '\0';
// 		g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, &sNotify, sizeof(sNotify));
// 	}
// 	while (0);
}
// 申请ib积分信息反馈
void QRelationModule::R2SOnResultApplyIbCoin(BYTE* pbyData, size_t uDataLen)
{
// 	KRELATION_PROTO_G2S_RESULTAPPLYIBCOIN* pMsg	= (KRELATION_PROTO_G2S_RESULTAPPLYIBCOIN*)pData;
// 
// 	do 
// 	{
// 
// 		INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(pMsg->nPlayerId);
// 		if (nPlayerIdx <= 0)
// 			break;
// 		KScriptFunctionCallHelper cHelper(*g_pMainScript);
// 		cHelper.SetMe(&Player[nPlayerIdx]);
// 		g_pMainScript->PushTable();
// 
// 		INT nTopIndex	= g_pMainScript->GetTopIndex();
// 		for (INT i = 0; i < (INT)pMsg->wCount; ++i)
// 		{
// 			g_pMainScript->PushTable();
// 			g_pMainScript->PushString(pMsg->aCloseRelation[i].szTargeter);
// 			g_pMainScript->SetTableField("szName");
// 			g_pMainScript->PushNumber(pMsg->aCloseRelation[i].nCoin);
// 			g_pMainScript->SetTableField("nBindCoin");
// 			g_pMainScript->SetTableIndex(i + 1);
// 		}
// 		g_pMainScript->CallTableFunction("Relation","SyncIbBinCoinInfo",0,"vd", nTopIndex, pMsg->byFirst);
// 	} while(FALSE);
}

//  领取ib返还绑定金币结果
void QRelationModule::R2SOnResultGainIbCoin(BYTE* pbyData, size_t uDataLen)
{
// 	KRELATION_PROTO_S2G_ADDBINDCOIN*  pMsg	= (KRELATION_PROTO_S2G_ADDBINDCOIN*)pData;
// 	INT nBindCoin	= pMsg->nCoin;
// 	INT nPlayerIdx	= g_PlayerManager.FindLocalPlayer(pMsg->nPlayerId);
// 	if (nBindCoin > 0 && nPlayerIdx > 0)
// 	{
// 		Player[nPlayerIdx].m_cPlayerItem.AddBindCoin(nBindCoin, emKBINDCOINADD_FRIEDN);
// 		// 日志
// 		CHAR szMsg[256];
// 		sprintf(szMsg, STR_PLAYERLOG_COMMENT_GETFRIENDCOINBACK, pMsg->szCloseFriend, pMsg->nCoin);
// 		KLogWritter::GetInstance()->Goddess_PlayerLog(
// 			&Player[nPlayerIdx],
// 			emKPLAYERLOG_TYPE_GETFRIENDCOINBACK,
// 			szMsg);
// 	}
}

// 发放好友亲密度奖励
void QRelationModule::R2SOnProcessAwardFriendFavor(BYTE* pbyData, size_t uDataLen)
{
// 	KRELATION_PROTO_G2S_AWARDBYFRIENDFAVOR*  pData = (KRELATION_PROTO_G2S_AWARDBYFRIENDFAVOR*)pMsg;
// 
// 	KGC_Player* pDstPlayer = g_pPlayerManager->GetPlayer(pData->nPlayerDstId);
// 	LPCSTR pszDstName = pDstPlayer->GetPlayerName();
// 	INT nAppIdx	= g_PlayerManager.FindLocalPlayer(pData->nPlayerAppId);
// 	INT nBindCoin = 0;
// 	if (nAppIdx > 0)
// 	{
// 
// 		INT nHaveGetCount = pData->byMaxNum - pData->byLeftCountApp;
// 		KScriptFunctionCallHelper cHelper(*g_pMainScript);
// 		g_pMainScript->PushTable();
// 
// 		INT nTopIndex	= g_pMainScript->GetTopIndex();
// 
// 		g_pMainScript->PushNumber(pData->nLevel);
// 		g_pMainScript->SetTableField("nLevel");
// 		g_pMainScript->PushNumber(pData->nAwardCoin);
// 		g_pMainScript->SetTableField("nAwardCoin");
// 
// 		g_pMainScript->PushNumber(pData->byAwardApp);
// 		g_pMainScript->SetTableField("nAwardApp");
// 		g_pMainScript->PushNumber(pData->byLeftCountApp);
// 		g_pMainScript->SetTableField("nAppLeftCount");
// 		g_pMainScript->PushNumber(pData->byMaxNum - pData->byLeftCountApp);
// 		g_pMainScript->SetTableField("nAppHaveCount");
// 		g_pMainScript->PushNumber(pData->byGiveCountApp);
// 		g_pMainScript->SetTableField("nAppGiveCount");
// 
// 		g_pMainScript->CallTableFunction("Relation", "AddAwared", 1, "osv", Player[nAppIdx].m_pLuna, pszDstName, nTopIndex);
// 	}
}
