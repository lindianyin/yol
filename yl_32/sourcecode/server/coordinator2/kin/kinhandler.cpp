#include "stdafx.h"
#include "kinhandler.h"
#include "GameCenter.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "KinHistory.h"


void KinHandler::OnApplyTongRosterRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_APPLY_TONG_ROSTER_REQUEST*  pPackage = (S2R_APPLY_TONG_ROSTER_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_APPLY_TONG_ROSTER_REQUEST) + sizeof(DWORD) * pPackage->nMemberCount);

	g_pGameCenter->m_TongManager.ProcessApplyRosterRequest(
		pPackage->dwPlayerID, 
		pPackage->nLastUpdateFrame, 
		pPackage->dwMemberList,
		pPackage->nMemberCount
		);

EXIT0:
	return;
}

void KinHandler::OnApplyTongInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_APPLY_TONG_INFO_REQUEST* pRequest    = (S2R_APPLY_TONG_INFO_REQUEST*)pbyData;

	g_pGameCenter->m_TongManager.ProcessApplyTongInfoRequest(pRequest->dwPlayerID, pRequest->nLastUpdateFrame);
}

void KinHandler::OnApplyCreateTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode        = false;
	S2R_APPLY_CREATE_TONG_REQUEST*  pApplyRequest   = (S2R_APPLY_CREATE_TONG_REQUEST*)pbyData;

	pApplyRequest->szTongName[sizeof(pApplyRequest->szTongName) - 1] = '\0';

	bRetCode = g_pGameCenter->m_TongManager.CreateTong(
		pApplyRequest->dwPlayerID, pApplyRequest->szTongName, (int)pApplyRequest->byTemplateIndex,
		pApplyRequest->byCamp);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KinHandler::OnInvitePlayerJoinTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_INVITE_PLAYER_JOIN_TONG_REQUEST*    pInviteRequest  = (S2R_INVITE_PLAYER_JOIN_TONG_REQUEST*)pbyData;

	pInviteRequest->szNewMemberName[sizeof(pInviteRequest->szNewMemberName) - 1] = '\0';

	g_pGameCenter->m_TongManager.ProcessInvitePlayerJoinTongRequest(
		pInviteRequest->dwPlayerID, pInviteRequest->szNewMemberName
		);
}

void KinHandler::OnInvitePlayerJoinTongRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_INVITE_PLAYER_JOIN_TONG_RESPOND*    pRespond   = (S2R_INVITE_PLAYER_JOIN_TONG_RESPOND*)pbyData;

	g_pGameCenter->m_TongManager.ProcessInvitePlayerJoinTongRespond(
		pRespond->dwPlayerID, pRespond->dwInviterID, pRespond->dwTongID, (INVITE_TONG_RESULT_CODE)pRespond->byAccept
		);
}

void KinHandler::OnApplyKickOutTongMemberRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_APPLY_KICK_OUT_TONG_MEMBER* pRequest = (S2R_APPLY_KICK_OUT_TONG_MEMBER*)pbyData;

	g_pGameCenter->m_TongManager.ProcessKickOutTongMemberRequest(pRequest->dwPlayerID, pRequest->dwMemberID);
}

void KinHandler::OnModifyTongInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_MODIFY_TONG_INFO_REQUEST*   pRequest = (S2R_MODIFY_TONG_INFO_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_MODIFY_TONG_INFO_REQUEST) + pRequest->wDataLen);

	g_pGameCenter->m_TongManager.ProcessModifyTongInfoRequest(
		pRequest->dwPlayerID, (int)pRequest->byInfoType, pRequest->byData, (size_t)pRequest->wDataLen
		);

EXIT0:
	return;
}

void KinHandler::OnModifyTongSchemaRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_MODIFY_TONG_SCHEMA_REQUEST* pRequest = (S2R_MODIFY_TONG_SCHEMA_REQUEST*)pbyData;

	g_pGameCenter->m_TongManager.ProcessModifyTongSchemaRequest(
		pRequest->dwPlayerID, pRequest->byGroupIndex, pRequest->byModifyType, 
		pRequest->byData, uDataLen - sizeof(S2R_MODIFY_TONG_SCHEMA_REQUEST)
		);
}

void KinHandler::OnApplyQuitTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{ 
	S2R_APPLY_QUIT_TONG_REQUEST*   pRequest = (S2R_APPLY_QUIT_TONG_REQUEST*)pbyData;

	g_pGameCenter->m_TongManager.ProcessQuitRequest(pRequest->dwPlayerID);
}

void KinHandler::OnChangeTongMemberGroupRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_CHANGE_TONG_MEMBER_GROUP_REQUEST*   pRequest = (S2R_CHANGE_TONG_MEMBER_GROUP_REQUEST*)pbyData;

	g_pGameCenter->m_TongManager.ProcessChangeMemberGroupRequest(
		pRequest->dwPlayerID, pRequest->dwTargetMemberID, (int)pRequest->byTargetGroup
		);
}

void KinHandler::OnChangeTongMasterRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	QLogPrintf(LOG_INFO,"kin OnChangeTongMasterRequest");
	S2R_CHANGE_TONG_MASTER_REQUEST* pRequest = (S2R_CHANGE_TONG_MASTER_REQUEST*)pbyData;

	g_pGameCenter->m_TongManager.ProcessChangeMasterRequest(pRequest->dwPlayerID, pRequest->dwNewMasterID, FALSE);
}

void KinHandler::OnChangeTongMemberRemarkRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_CHANGE_TONG_MEMBER_REMARK_REQUEST*  pRequest = (S2R_CHANGE_TONG_MEMBER_REMARK_REQUEST*)pbyData;

	pRequest->szRemark[sizeof(pRequest->szRemark) - 1] = '\0';

	g_pGameCenter->m_TongManager.ProcessChangeTongMemberRemarkRequest(
		pRequest->dwPlayerID, pRequest->dwTargetMemberID, pRequest->szRemark
		);
}

void KinHandler::OnGetTongDescriptionRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_GET_TONG_DESCRIPTION_REQUEST*   pRequest    = (S2R_GET_TONG_DESCRIPTION_REQUEST*)pbyData;
	KTong*                              pTong       = NULL;

	pTong = g_pGameCenter->m_TongManager.GetTong(pRequest->dwTongID);
	LOG_PROCESS_ERROR(pTong);

	DoGetTongDescriptionRepond(nConnIndex, pRequest->dwPlayerID, pTong);

EXIT0:
	return;
}

void KinHandler::OnSaveMoneyInTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	KRole*                          pRole       = NULL;
	S2R_SAVE_MONEY_IN_TONG_REQUEST* pRequest    = (S2R_SAVE_MONEY_IN_TONG_REQUEST*)pbyData;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pRole->m_dwTongID);

	// 存入帮会的资金以银币为最小单位
	LOG_PROCESS_ERROR(pRequest->dwAgateNum >= 1 || pRequest->dwDiamondNum >= 1 || pRequest->dwJadeiteNum >= 1 || pRequest->dwPealNum >= 1);

	g_pGameCenter->m_TongManager.ProcessSaveMoneyInTong(pRole, pRequest->dwPealNum, pRequest->dwJadeiteNum, pRequest->dwAgateNum, pRequest->dwDiamondNum, pRequest->dwContributionAdd);

EXIT0:
	return;
}

void KinHandler::OnPayTongSalaryRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode    = false;
	KRole*                          pRole       = NULL;
	KTong*                          pTong       = NULL;
	S2R_PAY_TONG_SALARY_REQUEST*    pRequest    = (S2R_PAY_TONG_SALARY_REQUEST*)pbyData;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pRole->m_dwTongID);

	LOG_PROCESS_ERROR(pRequest->nTotalSalary > 0);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	bRetCode = pTong->PaySalary(pRequest->dwPlayerID, pRequest->nTotalSalary);
	if (bRetCode)
	{
		g_pGameCenter->m_TongManager.SendTongEventNotify(nConnIndex, pRequest->dwPlayerID, tenPaySalarySuccess, pTong->m_nFund);
	}
	else
	{
		g_pGameCenter->m_TongManager.SendTongEventNotify(nConnIndex, pRequest->dwPlayerID, tenPaySalaryFailedError);
	}
EXIT0:
	return;
}

void KinHandler::OnGetTongSalaryRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode        = false;
	KRole*                          pRole           = NULL;
	KTong*                          pTong           = NULL;
	KTongMember*                    pMember         = NULL;
	int                             nSalarySilver   = 0;
	S2R_GET_TONG_SALARY_REQUEST*    pRequest        = (S2R_GET_TONG_SALARY_REQUEST*)pbyData;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pRole->m_dwTongID);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pMember = pTong->GetMember(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pMember);

	nSalarySilver = pMember->nSalary;
	PROCESS_ERROR(nSalarySilver > 0);

	bRetCode = DoGetTongSalaryRespond(nConnIndex, pRequest->dwPlayerID, pTong->m_dwID, nSalarySilver);
	LOG_PROCESS_ERROR(bRetCode);

	pMember->nSalary = 0;
	pTong->m_bUpdateFlag = true;

EXIT0:
	return;
}

void KinHandler::OnGetTongRankInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode        = false;
	KTong*                          pTong           = NULL;
	S2R_GET_TONG_RANK_INFO_REQUEST* pRequest        = (S2R_GET_TONG_RANK_INFO_REQUEST*)pbyData;

	pTong = g_pGameCenter->m_TongManager.GetTong(pRequest->dwTongId);
	if(!pTong)
	{
		//printf("can't find tongId:%d\n", pRequest->dwTongId);
		goto EXIT0;
	}
	LOG_PROCESS_ERROR(pTong);
	bRetCode = DoSyncTongRankInfo(nConnIndex, pRequest->dwTongId, pRequest->nRankId);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

void KinHandler::OnGetTongSalaryFailRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	KRole*                              pRole       = NULL;
	KTong*                              pTong       = NULL;
	KTongMember*                        pMember     = NULL;
	S2R_GET_TONG_SALARY_FAIL_RESPOND*   pRespond    = (S2R_GET_TONG_SALARY_FAIL_RESPOND*)pbyData;
	BYTE                                byBuffer[32];

	pTong = g_pGameCenter->m_TongManager.GetTong(pRespond->dwTongID);
	PROCESS_ERROR(pTong);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	PROCESS_ERROR(pRole);

	PROCESS_ERROR(pRole->m_dwTongID == pRespond->dwTongID);

	pMember = pTong->GetMember(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pMember);

	if (INT_MAX - pMember->nSalary < pRespond->nSilver)
	{
		pMember->nSalary = INT_MAX;
	}
	else
	{
		pMember->nSalary += pRespond->nSilver;
	}

	// 修改工资存盘
	pTong->m_bUpdateFlag = true;

	pTong->m_bUpdateTongRankFlag = true;

	bResult = true;
EXIT0:
	if (!bResult)
	{
		if (pRole)
		{
			byBuffer[0] = '\0';
			int nMoney    = pRespond->nSilver * MONEY_CONVERSION_SILVER_RATE;
			g_pGameCenter->m_MailManager.PostRequestSendMail(
				0, pRole->m_dwPlayerID, 0, eMailType_System, g_pGameCenter->m_Settings.m_szSystemMailSender, 
				g_pGameCenter->m_Settings.m_szTongReturnMoneyMailTitle, 
				nMoney,0,0, byBuffer, 1, NULL, 0, 0
				);
		}
		else if (pTong)
		{
			//pTong->AddFund(pRespond->nSilver);
		}
	}
	return;
}

void KinHandler::OnChangeTongCampRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode    = false;
	KRole*                          pRole       = NULL;
	KTong*                          pTong       = NULL;
	S2R_CHANGE_TONG_CAMP_REQUEST*   pRequest    = (S2R_CHANGE_TONG_CAMP_REQUEST*)pbyData;


	//pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	//LOG_PROCESS_ERROR(pRole);

	//pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	//LOG_PROCESS_ERROR(pTong);

	//if (pTong->m_dwMaster != pRequest->dwPlayerID)
	//{
	//	// 不是帮主，没权限
	//	g_pGameCenter->m_TongManager.SendTongEventNotify(nConnIndex, pRequest->dwPlayerID, tenChangeCampPermissionError);
	//	goto EXIT0;
	//}

	//LOG_PROCESS_ERROR(pRequest->byCamp == cGood || pRequest->byCamp == cEvil);
	//bRetCode = pTong->ChangeCamp((KCAMP)pRequest->byCamp);

	//if (!bRetCode)
	//{
	//	// 阵营转换限制
	//	g_pGameCenter->m_TongManager.SendTongEventNotify(nConnIndex, pRequest->dwPlayerID, tenChangeCampLimitError);
	//	goto EXIT0;
	//}

	////  广播阵营变更
	//DoTongBroadcastMessage(pTong, tbmCampChange, KD_BAD_ID, &pRequest->byCamp, sizeof(BYTE));

EXIT0:
	return;
}

void KinHandler::OnApplyRepertoryPageRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_APPLY_TONG_REPERTORY_PAGE_REQUEST* pRequest = (S2R_APPLY_TONG_REPERTORY_PAGE_REQUEST*)pbyData;

	DoSyncTongRepertoryPageRespond(
		nConnIndex, pRequest->dwPlayerID, (int)pRequest->byPageIndex, pRequest->nLastUpdateCounter
		);
}

void KinHandler::OnApplyOpenTongRepertoryRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetcode = false;
	KRole*                                  pRole    = NULL;
	KTong*                                  pTong    = NULL;
	S2R_APPLY_OPEN_TONG_REPERTORY_REQUEST*  pRequest = (S2R_APPLY_OPEN_TONG_REPERTORY_REQUEST*)pbyData;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	LOG_PROCESS_ERROR(KTongRepertory::GetRentConfig().s_bKinRepertoryEnable);

	DoApplyOpenTongRepertoryRespond(nConnIndex, pRequest->dwPlayerID, pTong);

EXIT0:
	return;
}

void KinHandler::OnTakeTongRepertoryItemToPosRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                            bRetCode    = false;
	KRole*                                          pRole       = NULL;
	KTong*                                          pTong       = NULL;
	//S2R_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST*    pRequest    = (S2R_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST*)pbyData;
	//KTONG_REPERTORY_GRID*                           pGrid       = NULL;
	//KTongMember*                                    pMember     = NULL;

	//LOG_PROCESS_ERROR(pRequest->byPageIndex < TONG_REPERTORY_PAGE_NUM);
	//LOG_PROCESS_ERROR(pRequest->byRepertoryPos < TONG_REPERTORY_PAGE_CAPACITY);

	//pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	//LOG_PROCESS_ERROR(pRole);

	//pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	//LOG_PROCESS_ERROR(pTong);

	//pMember = pTong->GetMember(pRole->m_dwPlayerID);
	//LOG_PROCESS_ERROR(pMember);

	//LOG_PROCESS_ERROR(pTong->m_Repertory.m_Page[pRequest->byPageIndex].bEnable);

	//if (pRequest->byPageIndex > 0)  // 所有成员拥有第一页取权限
	//{
	//	bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totGetFromRepositoryPage2 + pRequest->byPageIndex - 1);
	//	if (!bRetCode)
	//	{
	//		g_pGameCenter->m_TongManager.SendTongEventNotify(
	//			nConnIndex, pRequest->dwPlayerID, tenTakeRepertoryItemPermissionDenyError
	//			);

	//		goto EXIT0;
	//	}
	//}

	//pGrid = pTong->m_Repertory.TakeItem(pRequest->byPageIndex, pRequest->byRepertoryPos, pRequest->nLastUpdateCounter);

	//if (pGrid->pbyItemData == NULL)
	//{
	//	// 道具获取失败
	//	g_pGameCenter->m_TongManager.SendTongEventNotify(
	//		nConnIndex, pRequest->dwPlayerID, tenItemNotInRepertoryError
	//		);
	//	goto EXIT0;
	//}

	//bRetCode = DoTakeTongRepertoryItemToPosRespond(
	//	nConnIndex, pRequest->dwPlayerID, pRole->m_dwTongID,
	//	pRequest->byPageIndex, pRequest->byRepertoryPos,
	//	pRequest->byBoxIndex, pRequest->byBoxPos,
	//	(size_t)pGrid->byItemDataLen, pGrid->pbyItemData
	//	);
	//LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KinHandler::OnTakeTongRepertoryItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode    = false;
	KRole*                                  pRole       = NULL;
	KTong*                                  pTong       = NULL;
	S2R_TAKE_TONG_REPERTORY_ITEM_REQUEST*   pRequest    = (S2R_TAKE_TONG_REPERTORY_ITEM_REQUEST*)pbyData;
	KTONG_REPERTORY_GRID*                   pGrid       = NULL;
	KTongMember*                            pMember     = NULL;
	TDBItemData*                            pItemData   = NULL;

	LOG_PROCESS_ERROR(pRequest->byPageIndex < TONG_REPERTORY_PAGE_NUM);
	LOG_PROCESS_ERROR(pRequest->byRepertoryPos < TONG_REPERTORY_PAGE_CAPACITY);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pTong->m_Repertory.CheckRentOnExpireItem();

	pMember = pTong->GetMember(pRole->m_dwPlayerID);
	LOG_PROCESS_ERROR(pMember);

	LOG_PROCESS_ERROR(pTong->m_Repertory.m_Page[pRequest->byPageIndex].bEnable);

	if (pRequest->byPageIndex > 0) // 所有成员拥有第一页取权限
	{
		bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totGetFromRepositoryPage2 + pRequest->byPageIndex - 1);
		if (!bRetCode)
		{
			g_pGameCenter->m_TongManager.SendTongEventNotify(
				nConnIndex, pRequest->dwPlayerID, tenTakeRepertoryItemPermissionDenyError
				);

			goto EXIT0;
		}
	}

	pGrid = pTong->m_Repertory.TakeItem(pRequest->byPageIndex, pRequest->byRepertoryPos, pRequest->nLastUpdateCounter, pRequest->dwPlayerID, g_pGameCenter->m_nTimeNow);
	if (pGrid == NULL)
	{
		// 道具获取失败
		g_pGameCenter->m_TongManager.SendTongEventNotify(
			nConnIndex, pRequest->dwPlayerID, tenItemNotInRepertoryError
			);
		goto EXIT0;
	}

	pItemData = (TDBItemData*)pGrid->pbyItemData;
	LOG_PROCESS_ERROR(pItemData);
	if (pItemData->byStateFlags & (0x01 << 1))
	{
		PROCESS_ERROR(KTongRepertory::GetRentConfig().s_bKinRepertoryRentEnable);
		LOG_PROCESS_ERROR(!pGrid->bRemoveOnExpire);
		if (pGrid->dwTakePlayerId != KD_BAD_ID && pGrid->nExpireTime >= g_pGameCenter->m_nTimeNow)
		{ // 装备已经借出
			if (pGrid->dwPutPlayerId == pRequest->dwPlayerID)
			{ // 主人想取回装备
				pGrid->bRemoveOnExpire = TRUE;
				DoKinMessageNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenSetItemRemoveOnExpireSuccess);
				goto EXIT0;
			}
			else
			{
				DoKinMessageNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenTakeItemAlreadyRentError);
				goto EXIT0;
			}
		}
		else
		{
			if (pGrid->dwPutPlayerId == pRequest->dwPlayerID)
			{
				// 主人取回装备
				pGrid->bRemoveOnExpire = TRUE;
			}
			else
			{
				// 借装备
				LOG_PROCESS_ERROR(pGrid->dwTakePlayerId == 0);
				pGrid->dwTakePlayerId = pRequest->dwPlayerID;
				pGrid->nTakeTime = g_pGameCenter->m_nTimeNow;
				pGrid->nExpireTime = g_pGameCenter->m_nTimeNow + KTongRepertory::GetRentConfig().s_tRentPeriod;
			}
		}
	}

	bRetCode = DoTakeTongRepertoryItemRespond(
		nConnIndex, pRequest->dwPlayerID, pRole->m_dwTongID, pGrid->dwPutPlayerId, KTongRepertory::GetRentConfig().s_tRentPeriod,
		(int)pRequest->byBoxIndex, (int)pRequest->byBoxPos,
		(int)pRequest->byPageIndex, (int)pRequest->byRepertoryPos,
		(size_t)pGrid->byItemDataLen, pGrid->pbyItemData
		);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

void KinHandler::OnTakeTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode        = false;
	S2R_TAKE_TONG_REPERTORY_ITEM_RESPOND*   pRespond        = (S2R_TAKE_TONG_REPERTORY_ITEM_RESPOND*)pbyData;
	KTong*                                  pTong           = NULL;
	TDBItemData*                            pItemData       = NULL;
	KTONG_REPERTORY_GRID*                   pGrid           = NULL;
	BOOL                                    bRemove         = FALSE;

	LOG_PROCESS_ERROR(pRespond->dwTongID);
	LOG_PROCESS_ERROR(pRespond->byPageIndex < TONG_REPERTORY_PAGE_NUM);
	LOG_PROCESS_ERROR(pRespond->byPagePos < TONG_REPERTORY_PAGE_CAPACITY);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRespond->dwTongID);
	LOG_PROCESS_ERROR(pTong);

	if (pRespond->bSuccess)
	{
		pGrid = pTong->m_Repertory.GetRepertoryGrid(pRespond->byPageIndex, pRespond->byPagePos);
		LOG_PROCESS_ERROR(pGrid);
		LOG_PROCESS_ERROR(pGrid->pbyItemData);

		pItemData = (TDBItemData*)pGrid->pbyItemData;
		if ((pItemData->byStateFlags >> 1) & 0x01)
		{
			if (pGrid->dwPutPlayerId == pRespond->dwPlayerID)
			{ // 拿回装备
				PROCESS_ERROR(pGrid->dwTakePlayerId == KD_BAD_ID);
				bRemove = TRUE;
			}
			else 
			{ // 借装备
				bRemove = FALSE;
				pTong->m_Repertory.UnlockGrid(pRespond->byPageIndex, pRespond->byPagePos);
				pGrid->nLastUpdateCounter = ++pTong->m_Repertory.m_nUpdateCounter;
			}
		}
		else
		{
			bRemove = TRUE;
		}
		
		if (bRemove)
		{
			bRetCode = pTong->m_History.RecordTakeRepertoryItem(
				pRespond->dwPlayerID, pItemData->wGenre, 0, pItemData->wStackCount
				);
			LOG_CHECK_ERROR(bRetCode);

			pTong->m_Repertory.FreeItem(pRespond->byPageIndex, pRespond->byPagePos);
		}

		g_pGameCenter->m_TongManager.SendTongEventNotify(
			nConnIndex, pRespond->dwPlayerID, tenTakeItemFromRepertorySuccess, (int)pRespond->byPageIndex
			);

		pTong->m_bUpdateFlag = true;
	}
	else
	{
		pTong->m_Repertory.RecoverItem(pRespond->byPageIndex, pRespond->byPagePos);
	}
EXIT0:
	return;
}

void KinHandler::OnPutTongRepertoryItemToPosRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                            bRetCode    = false;
	//S2R_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST*     pRequest    = (S2R_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST*)pbyData;
	//KTong*                                          pTong       = NULL;
	//KRole*                                          pRole       = NULL;
	//KTongMember*                                    pMember     = NULL;

	//LOG_PROCESS_ERROR(pRequest->byPageIndex < TONG_REPERTORY_PAGE_NUM);
	//LOG_PROCESS_ERROR(pRequest->byPagePos < TONG_REPERTORY_PAGE_CAPACITY);

	//pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	//LOG_PROCESS_ERROR(pRole);

	//pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	//LOG_PROCESS_ERROR(pTong);

	//pMember = pTong->GetMember(pRole->m_dwPlayerID);
	//LOG_PROCESS_ERROR(pMember);

	//LOG_PROCESS_ERROR(pTong->m_Repertory.m_Page[pRequest->byPageIndex].bEnable);

	//if (pRequest->byPageIndex > 0) // 仓库第一页不用检查权限
	//{
	//	bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totPutRepositoryPage2 + pRequest->byPageIndex - 1);
	//	if (!bRetCode)
	//	{
	//		g_pGameCenter->m_TongManager.SendTongEventNotify(
	//			nConnIndex, pRequest->dwPlayerID, tenPutItemToRepertoryPermissionDenyError
	//			);

	//		goto EXIT0;
	//	}
	//}

	//bRetCode = pTong->m_Repertory.ApplyEmptyGrid(pRequest->byPageIndex, pRequest->byPagePos);

	//if (!bRetCode)
	//{
	//	// 栏位已被占
	//	g_pGameCenter->m_TongManager.SendTongEventNotify(
	//		nConnIndex, pRequest->dwPlayerID, tenRepertoryGridFilledError
	//		);
	//	goto EXIT0;
	//}

	//bRetCode = DoPutTongRepertoryItemRespond(
	//	pRequest->dwPlayerID, nConnIndex, pTong->m_dwID,
	//	pRequest->dwServerItemID, pRequest->byBoxIndex, pRequest->byBoxPos,
	//	pRequest->byPageIndex, pRequest->byPagePos
	//	);
	//LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

void KinHandler::OnPutTongRepertoryItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode    = false;
	S2R_PUT_TONG_REPERTORY_ITEM_REQUEST*    pRequest    = (S2R_PUT_TONG_REPERTORY_ITEM_REQUEST*)pbyData;
	KTong*                                  pTong       = NULL;
	KRole*                                  pRole       = NULL;
	KTongMember*                            pMember     = NULL;
	TDBItemData*                            pItemData   = NULL;
	BOOL                                    bFindItem   = FALSE;
	INT                                     nPageIndex  = -1;
	INT                                     nPos        = -1;
	KTONG_REPERTORY_GRID*                   pGrid       = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pMember = pTong->GetMember(pRole->m_dwPlayerID);
	LOG_PROCESS_ERROR(pMember);

	LOG_PROCESS_ERROR(pTong->m_Repertory.m_Page[pRequest->byPageIndex].bEnable);

	if (pRequest->byPageIndex > 0) // 仓库第一页不用检查权限
	{
		bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totPutRepositoryPage2 + pRequest->byPageIndex - 1);
		if (!bRetCode)
		{
			g_pGameCenter->m_TongManager.SendTongEventNotify(
				nConnIndex, pRequest->dwPlayerID, tenPutItemToRepertoryPermissionDenyError
				);

			goto EXIT0;
		}
	}

	pItemData = (TDBItemData*)pRequest->byItemData;
	if (pItemData->byStateFlags & (0x01 << 1))
	{
		PROCESS_ERROR(KTongRepertory::GetRentConfig().s_bKinRepertoryRentEnable);
	}

	bFindItem = pTong->m_Repertory.FindItem(pRequest->byItemData, &nPageIndex, &nPos);
	if ((pItemData->byStateFlags & (0x01 << 1)) && bFindItem)
	{ // 玩家归还
		pRequest->byPageIndex = nPageIndex;
		pRequest->byPagePos = nPos;

		pGrid = pTong->m_Repertory.GetRepertoryGrid(nPageIndex, nPos);
		assert(pGrid);
		LOG_PROCESS_ERROR(pGrid->dwTakePlayerId == pRequest->dwPlayerID);
	}
	else
	{
		if (pRequest->byPagePos == -1) // 不指定位置
		{
			INT nEmpty = -1;
			bRetCode = pTong->m_Repertory.FindEmptyGrid(pRequest->byPageIndex, &nEmpty);
			if (!bRetCode)
			{
				g_pGameCenter->m_TongManager.SendTongEventNotify(
					nConnIndex, pRequest->dwPlayerID, tenRepertoryPageFullError
					);
				goto EXIT0;
			}

			bRetCode = pTong->m_Repertory.ApplyEmptyGrid(pRequest->byPageIndex, nEmpty);
			assert(bRetCode);
			pRequest->byPagePos = nEmpty;
		}
		else
		{
			bRetCode = pTong->m_Repertory.ApplyEmptyGrid(pRequest->byPageIndex, pRequest->byPagePos);

			if (!bRetCode)
			{
				// 栏位已被占
				g_pGameCenter->m_TongManager.SendTongEventNotify(
					nConnIndex, pRequest->dwPlayerID, tenRepertoryGridFilledError
					);
				goto EXIT0;
			}
		}
	}

	bRetCode = DoPutTongRepertoryItemRespond(
		pRequest->dwPlayerID, nConnIndex, pTong->m_dwID,
		pRequest->dwItemIdx, pRequest->byBoxIndex, pRequest->byBoxPos,
		pRequest->byPageIndex, pRequest->byPagePos, bFindItem
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KinHandler::OnPutTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode        = false;
 	S2R_PUT_TONG_REPERTORY_ITEM_RESPOND*    pRespond        = (S2R_PUT_TONG_REPERTORY_ITEM_RESPOND*)pbyData;
 	KTong*                                  pTong           = NULL;
	KRole*                                  pRole           = NULL;
 	TDBItemData*                            pItemData       = NULL;
 	KTONG_REPERTORY_GRID*                   pGrid           = NULL;
 
	pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);
 
	pGrid = pTong->m_Repertory.GetRepertoryGrid(pRespond->byPageIndex, pRespond->byPagePos);
	LOG_PROCESS_ERROR(pGrid);

	if (pRespond->bFindItem)
	{ // 玩家归还
		PROCESS_ERROR(pGrid->pbyItemData);
		pGrid->dwTakePlayerId = KD_BAD_ID;
		pGrid->nTakeTime = 0;
		pGrid->nExpireTime = 0;
		pGrid->bRemoveOnExpire = FALSE;
		pGrid->nLastUpdateCounter = ++pTong->m_Repertory.m_nUpdateCounter;
		if (pGrid->bRemoveOnExpire)
		{
			pTong->m_Repertory.LockGrid(pRespond->byPageIndex, pRespond->byPagePos);
			pTong->m_Repertory.FreeItem(pRespond->byPageIndex, pRespond->byPagePos);
		}
	}
	else
	{
		bRetCode = pTong->m_Repertory.PutItem(
			pRespond->byPageIndex, pRespond->byPagePos, pRespond->byItemData, (size_t)pRespond->byItemDataLen, 
			pRespond->dwPlayerID, g_pGameCenter->m_nTimeNow
			);
		LOG_PROCESS_ERROR(bRetCode);

		LOG_PROCESS_ERROR(pGrid->pbyItemData);
		pItemData = (TDBItemData*)pGrid->pbyItemData;
		bRetCode = pTong->m_History.RecordPutRepertoryItem(
			pRespond->dwPlayerID, pItemData->wGenre, 0, pItemData->wStackCount
			);
		LOG_PROCESS_ERROR(bRetCode);
	}

 
 	pTong->m_bUpdateFlag = true;
 
 	g_pGameCenter->m_TongManager.SendTongEventNotify(
 		nConnIndex, pRespond->dwPlayerID, tenPutItemInRepertorySuccess, (int)pRespond->byPageIndex
 		);

EXIT0:
	return;
}

void KinHandler::OnUnlockTongRepertoryGrid(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bRetCode    = false;
	//S2R_UNLOCK_TONG_REPERTORY_GRID*     pRespond    = (S2R_UNLOCK_TONG_REPERTORY_GRID*)pbyData;
	//KTong*                              pTong       = NULL;

	//LOG_PROCESS_ERROR(pRespond->byPageIndex < TONG_REPERTORY_PAGE_NUM);
	//LOG_PROCESS_ERROR(pRespond->byPagePos < TONG_REPERTORY_PAGE_CAPACITY);

	//pTong = g_pGameCenter->m_TongManager.GetTong(pRespond->dwTongID);
	//LOG_PROCESS_ERROR(pTong);

	//bRetCode = pTong->m_Repertory.UnlockGrid((int)pRespond->byPageIndex, (int)pRespond->byPagePos);
	//LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KinHandler::OnExchangeTongRepertoryItemPosRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                            bRetCode        = false;
	int                                             nSrcPageIndex   = 0;
	int                                             nDstPageIndex   = 0;
	int                                             nSrcPagePos     = 0;
	int                                             nDstPagePos     = 0;
	KRole*                                          pRole           = NULL;
	KTong*                                          pTong           = NULL;
	KTongMember*                                    pMember         = NULL;
	KTONG_REPERTORY_PAGE*                           pSrcPage        = NULL;
	KTONG_REPERTORY_PAGE*                           pDstPage        = NULL;
	S2R_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST*   pRequest        = (S2R_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST*)pbyData;


	LOG_PROCESS_ERROR(pRequest->bySrcPageIndex < TONG_REPERTORY_PAGE_NUM);
	LOG_PROCESS_ERROR(pRequest->bySrcPagePos < TONG_REPERTORY_PAGE_CAPACITY);
	LOG_PROCESS_ERROR(pRequest->byDstPageIndex < TONG_REPERTORY_PAGE_NUM);
	LOG_PROCESS_ERROR(pRequest->byDstPagePos < TONG_REPERTORY_PAGE_CAPACITY);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	LOG_PROCESS_ERROR(pRole->m_dwTongID);
	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pMember = pTong->GetMember(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pMember);

	nSrcPageIndex   = (int)pRequest->bySrcPageIndex;
	nDstPageIndex   = (int)pRequest->byDstPageIndex;
	nSrcPagePos     = (int)pRequest->bySrcPagePos;
	nDstPagePos     = (int)pRequest->byDstPagePos;

	pSrcPage = &pTong->m_Repertory.m_Page[nSrcPageIndex];
	pDstPage = &pTong->m_Repertory.m_Page[nDstPageIndex];

	LOG_PROCESS_ERROR(pSrcPage->bEnable);
	LOG_PROCESS_ERROR(pDstPage->bEnable);

	if (pSrcPage->Grids[nSrcPagePos].pbyItemData)
	{
		if (nSrcPageIndex > 0)
		{
			bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totGetFromRepositoryPage2 + nSrcPageIndex - 1);
			LOG_PROCESS_ERROR(bRetCode);
		}

		if (nDstPageIndex > 0)
		{
			bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totPutRepositoryPage2 + nDstPageIndex - 1);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}

	if (pDstPage->Grids[nDstPagePos].pbyItemData)
	{
		if (nDstPageIndex > 0)
		{
			bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totGetFromRepositoryPage2 + nDstPageIndex - 1);
			LOG_PROCESS_ERROR(bRetCode);
		}

		if (nSrcPageIndex > 0)
		{
			bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totPutRepositoryPage2 + nSrcPageIndex - 1);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}

	bRetCode = pTong->m_Repertory.ExchangeItemPos(
		nSrcPageIndex, nSrcPagePos, pRequest->nSrcUpdateCounter,
		nDstPageIndex, nDstPagePos, pRequest->nDstUpdateCounter
		);
	LOG_PROCESS_ERROR(bRetCode);
	pTong->m_bUpdateFlag = true;

	g_pGameCenter->m_TongManager.SendTongEventNotify(
		nConnIndex, pRequest->dwPlayerID, tenExchangeRepertoryItemSuccess, nDstPageIndex
		);

EXIT0:
	return;
}

void KinHandler::OnApplyStackTongRepertoryItemRequest(BYTE*pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                            bRetCode    = false;
	//S2R_APPLY_STACK_TONG_REPERTORY_ITEM_REQUEST*    pRequest    = (S2R_APPLY_STACK_TONG_REPERTORY_ITEM_REQUEST*)pbyData;
	//KTong*                                          pTong       = NULL;
	//KTongMember*                                    pMember     = NULL;

	//pTong = g_pGameCenter->m_TongManager.GetTong(pRequest->dwTongID);
	//LOG_PROCESS_ERROR(pTong);

	//pMember = pTong->GetMember(pRequest->dwPlayerID);
	//LOG_PROCESS_ERROR(pMember);

	//if (pRequest->byPageIndex > 0)
	//{
	//	bRetCode = pTong->CanBaseOperate(pMember->nGroupID, pRequest->byPageIndex + totPutRepositoryPage2 - 1);
	//	LOG_PROCESS_ERROR(bRetCode);

	//	if (!bRetCode)
	//	{
	//		g_pGameCenter->m_TongManager.SendTongEventNotify(
	//			nConnIndex, pRequest->dwPlayerID, tenPutItemToRepertoryPermissionDenyError
	//			);
	//		goto EXIT0;
	//	}
	//}

	//bRetCode = pTong->m_Repertory.ApplyStackItem(
	//	pRequest->byPageIndex, pRequest->byPagePos, 
	//	pRequest->byTabType, pRequest->wTabIndex, 
	//	pRequest->wStackNum, pRequest->wMaxStackNum,
	//	pRequest->nLastUpdateCounter
	//	);
	//if (!bRetCode)
	//{
	//	g_pGameCenter->m_TongManager.SendTongEventNotify(
	//		nConnIndex, pRequest->dwPlayerID, tenStackItemToRepertoryFailError
	//		);
	//	goto EXIT0;
	//}

	//bRetCode = DoApplyStackTongRepertoryItemRespond(
	//	nConnIndex, pRequest->dwPlayerID, pRequest->dwTongID,
	//	pRequest->byPageIndex, pRequest->byPagePos, 
	//	pRequest->byBoxIndex, pRequest->byBoxPos, pRequest->dwItemID,
	//	pRequest->byTabType, pRequest->wTabIndex,
	//	pRequest->wStackNum
	//	);
	//LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KinHandler::OnStackTongRepertoryItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode    = false;
	BOOL                                    bSuccess    = false;
	S2R_STACK_TONG_REPERTORY_ITEM_REQUEST*  pRequest    = (S2R_STACK_TONG_REPERTORY_ITEM_REQUEST*)pbyData;
	KTong*                                  pTong       = NULL;
	KRole*                                  pRole       = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRequest->dwTongID);
	LOG_PROCESS_ERROR(pTong);

	bRetCode = pTong->m_Repertory.StackItem(
		pRequest->byPageIndex, pRequest->byPagePos,
		pRequest->byTabType, pRequest->wTabIndex,
		pRequest->wStackNum, pRequest->wMaxStackNum
		);
	LOG_PROCESS_ERROR(bRetCode);

	pTong->m_bUpdateFlag = true;

	g_pGameCenter->m_TongManager.SendTongEventNotify(
		nConnIndex, pRequest->dwPlayerID, tenPutItemInRepertorySuccess, (int)pRequest->byPageIndex
		);

	bSuccess = true;
EXIT0:
	if (!bSuccess && pRole)
	{        
		g_pGameCenter->m_LogServerAgency.LogPlayerItemChange(
			pRole, ITEM_OPERATION_TYPE_OUT, 
			pRequest->byTabType, pRequest->wTabIndex, pRequest->wStackNum, "tong repertory stack fail"
			);
	}
	return;
}


void KinHandler::OnRemoveTongRepertoryItemRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL                                    bRetCode    = false;
	KRole*                                  pRole       = NULL;
	KTong*                                  pTong       = NULL;
	S2R_REMOVE_TONG_REPERTORY_ITEM_REQUEST* pRequest    = (S2R_REMOVE_TONG_REPERTORY_ITEM_REQUEST*)pbyData;
	KTONG_REPERTORY_GRID*                   pGrid       = NULL;
	KTongMember*                            pMember     = NULL;

	LOG_PROCESS_ERROR(pRequest->byPageIndex < TONG_REPERTORY_PAGE_NUM);
	LOG_PROCESS_ERROR(pRequest->byPagePos < TONG_REPERTORY_PAGE_CAPACITY);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pMember = pTong->GetMember(pRole->m_dwPlayerID);
	LOG_PROCESS_ERROR(pMember);

	LOG_PROCESS_ERROR(pTong->m_Repertory.m_Page[pRequest->byPageIndex].bEnable);

	if (pTong->m_dwMaster != pRequest->dwPlayerID)
	{
		g_pGameCenter->m_TongManager.SendTongEventNotify(
			nConnIndex, pRequest->dwPlayerID, tenRemoveRepertoryItemPermissionError
			);

		goto EXIT0;
	}

	pGrid = pTong->m_Repertory.GetRepertoryGrid(pRequest->byPageIndex, pRequest->byPagePos);
	LOG_PROCESS_ERROR(pGrid);

	if (pGrid->dwTakePlayerId > 0 && pGrid->nExpireTime >= g_pGameCenter->m_nTimeNow)
	{
		pGrid->bRemoveOnExpire = TRUE;
	}
	else
	{
		pTong->m_Repertory.LockGrid(pRequest->byPageIndex, pRequest->byPagePos);
		bRetCode = pTong->m_Repertory.FreeItem(pRequest->byPageIndex, pRequest->byPagePos);
		LOG_PROCESS_ERROR(bRetCode);
	}

	g_pGameCenter->m_TongManager.SendTongEventNotify(
		nConnIndex, pRequest->dwPlayerID, tenRemoveRepertoryItemSuccess
		);

EXIT0:
	return;
}

void KinHandler::OnGetBackItemFromRepertoryRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL                                      bRetCode    = false;
	HRESULT                                   hRetCode    = E_FAIL;
	KRole*                                    pRole       = NULL;
	KTong*                                    pTong       = NULL;
	S2R_GET_BACK_ITEM_FROM_REPERTORY_REQUEST* pRequest    = (S2R_GET_BACK_ITEM_FROM_REPERTORY_REQUEST*)pbyData;
	TDBItemData*                              pDBItemData = NULL;
	KTONG_REPERTORY_PAGE*                     pPage       = NULL;
	KTONG_REPERTORY_GRID*                     pGrid       = NULL;
	BOOL                                      bFindItem   = false;
	BOOL                                      bGetBackSuccess = FALSE;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRequest->dwTongID);
	if (pTong)
	{
		for (int i = 0; i < TONG_REPERTORY_PAGE_NUM; ++i)
		{
			pPage = &pTong->m_Repertory.m_Page[i];
			if (pPage->bEnable)
			{
				for (int j = 0; j < TONG_REPERTORY_PAGE_CAPACITY; ++j)
				{
					pGrid = &pPage->Grids[j];

					if (pGrid->pbyItemData)
					{
						pDBItemData = (TDBItemData*)pGrid->pbyItemData;
						if (pDBItemData->sGuid == pRequest->sGuid)
						{
							bFindItem = TRUE;
							// 借出状态则通知不能取回
							if (pGrid->dwTakePlayerId != KD_BAD_ID && pGrid->nExpireTime > g_pGameCenter->m_nTimeNow)
							{
								DoKinMessageNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenGetBackItemAleadyRentError);
								goto EXIT0;
							}
							else
							{
								bGetBackSuccess = TRUE;
								bRetCode = pTong->m_Repertory.LockGrid(i, j);
								LOG_PROCESS_ERROR(bRetCode);

								bRetCode = pTong->m_Repertory.FreeItem(i, j);
								LOG_PROCESS_ERROR(bRetCode);

								DoKinMessageNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenGetBackRepertoryItemSuccess);
							}
							break;
						}
					}
				}
				if (bFindItem) break;
			}
		}
	}

	// 没找到装备，说明已经移除，可直接拿回
	if (!bFindItem) bGetBackSuccess = TRUE;

	bRetCode = DoGetBackItemFromRepertoryRespond(
		pRequest->dwPlayerID, pRequest->byBoxIndex, pRequest->byBoxPos, bGetBackSuccess
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}


void KinHandler::OnGetFreeJoinTongListRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL                                      bRetCode    = FALSE;
	S2R_GET_FREEJOIN_TONG_LIST_REQUEST*		  pRequest    = (S2R_GET_FREEJOIN_TONG_LIST_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(uDataLen >= sizeof(S2R_GET_FREEJOIN_TONG_LIST_REQUEST));
	g_pGameCenter->m_TongManager.ProcessGetFreeJoinTongList(pRequest->dwPlayerID, pRequest->nLastUpdateCounter);

	bRetCode = TRUE;
EXIT0:
	return;
}

void KinHandler::OnApplyJoinTongRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL									bRetCode = FALSE;
	S2R_APPLY_JOIN_TONG_REQUEST*			pRequest = (S2R_APPLY_JOIN_TONG_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(uDataLen >= sizeof(S2R_APPLY_JOIN_TONG_REQUEST));
	g_pGameCenter->m_TongManager.ProcessApplyJoinTongRequest(pRequest->dwPlayerID, pRequest->dwTongID);

	bRetCode = TRUE;
EXIT0:
	return;
}

void KinHandler::OnSetTongActivityOpenTimeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL									  bRetCode = FALSE;
	S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST*	  pRequest = (S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(uDataLen >= sizeof(S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST));
	g_pGameCenter->m_TongManager.ProcessSetTongActivityOpenTime(pRequest->dwPlayerId, pRequest->dwTongId, pRequest->dwTongActivityIndex, pRequest->dwTongActivityOpenTime);

	bRetCode = TRUE;
EXIT0:
	return;
}

void KinHandler::OnSetTongFreeJoinRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL									bRetCode = FALSE;
	S2R_SET_TONG_FREEJOIN_REQUEST*			pRequest = (S2R_SET_TONG_FREEJOIN_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(uDataLen >= sizeof(S2R_SET_TONG_FREEJOIN_REQUEST));
	g_pGameCenter->m_TongManager.ProcessSetFreeJoin(pRequest->dwPlayerID, pRequest->dwTongID, pRequest->bValue);

	bRetCode = TRUE;
EXIT0:
	return;
}

// ----------------------------------------------------------------------------------------------------


BOOL KinHandler::DoSyncTongMemberInfo(int nConnIndex, int dwPlayerID, DWORD dwMemberID, const KTongMember& crMember)
{
	BOOL                        bResult     = false;
	BOOL                        bRetCode    = 0;
	IMemBlock*                 piPackage   = NULL;
	R2S_SYNC_TONG_MEMBER_INFO*  pR2SPak     = NULL; 
	KTongMemberInfo*            pMemberInfo = NULL;
	KRole*                      pRole       = NULL;

	piPackage = QCreateMemBlock((unsigned)sizeof(R2S_SYNC_TONG_MEMBER_INFO));
	LOG_PROCESS_ERROR(piPackage);

	pR2SPak = (R2S_SYNC_TONG_MEMBER_INFO*)piPackage->GetData();
	LOG_PROCESS_ERROR(pR2SPak);

	pR2SPak->wProtocolID    = r2s_sync_tong_member_info;
	pR2SPak->dwPlayerID     = dwPlayerID;

	pRole = g_pGameCenter->m_RoleManager.GetRole(crMember.dwID);
	LOG_PROCESS_ERROR(pRole);

	pMemberInfo                     = &pR2SPak->MemberInfo;

	pMemberInfo->dwID               = crMember.dwID;
	pMemberInfo->nGroupID           = crMember.nGroupID;
	pMemberInfo->nJoinTime          = crMember.nJoinTime;
	pMemberInfo->nLastOfflineTime   = crMember.nLastOfflineTime;

	pMemberInfo->byLevel            = (BYTE)crMember.nLevel;
	pMemberInfo->byForceID          = (BYTE)crMember.dwForceID;
	pMemberInfo->dwMapID            = crMember.dwMapID;
	pMemberInfo->bIsOnline          = crMember.bIsOnline ? true : false;
	pMemberInfo->nSalary            = crMember.nSalary;
	pMemberInfo->nDailyContribution = crMember.nDailyContribution;

	strncpy(pMemberInfo->szName, pRole->m_szName, sizeof(pMemberInfo->szName));
	pMemberInfo->szName[sizeof(pMemberInfo->szName) - 1] = '\0';

	strncpy(pMemberInfo->szRemark, crMember.szRemark, sizeof(pMemberInfo->szRemark));
	pMemberInfo->szRemark[sizeof(pMemberInfo->szRemark) - 1] = '\0';

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoTongDeleteMemberNotify(int nConnIndex, DWORD dwPlayerID, DWORD dwMemberID)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piBuffer    = NULL;
	R2S_DELETE_TONG_MEMBER_NOTIFY*  pPackage    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_DELETE_TONG_MEMBER_NOTIFY));
	LOG_PROCESS_ERROR(piBuffer);

	pPackage = (R2S_DELETE_TONG_MEMBER_NOTIFY*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pPackage);

	pPackage->wProtocolID   = r2s_delete_tong_member_notify;
	pPackage->dwPlayerID    = dwPlayerID;
	pPackage->dwMemberID    = dwMemberID;

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoSyncTongRepertoryPageRespond(int nConnIndex, int dwPlayerID, int nPageIndex, int nLastUpdateCounter)
{
	BOOL                                    bResult             = false;
	BOOL                                    bRetCode            = false;
	BOOL                                    bNeedUpdateGrid     = false;
	BOOL                                    bNeedFreeGrid       = false;
	int                                     nCount              = 0;
	HRESULT                                 hRetCode            = E_FAIL;
	DWORD                                   dwTongID            = KD_BAD_ID;
	KRole*                                  pRole               = NULL;
	KTong*                                  pTong               = NULL;
	KTongMember*                            pMember             = NULL;
	size_t                                  uUsedSize           = 0;
	IMemBlock*                             piBuffer            = NULL;
	IMemBlock_Resizable*                      piResize            = NULL;
	R2S_SYNC_TONG_REPERTORY_PAGE_RESPOND*   pSyncPageRespond    = NULL;
	size_t                                  uLeftSize           = 0;
	BYTE*                                   pbyOffset           = NULL;

	LOG_PROCESS_ERROR(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
	LOG_PROCESS_ERROR(nLastUpdateCounter >= -1);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	LOG_PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pMember = pTong->GetMember(dwPlayerID);
	LOG_PROCESS_ERROR(pMember);

	PROCESS_ERROR(pTong->m_Repertory.m_Page[nPageIndex].bEnable);
	pTong->m_Repertory.CheckRentOnExpireItem();

	// --------------------------------------------------------------------------------------------------
	piBuffer = QCreateMemBlock(MAX_SYNC_TONG_REPERTORY_PAGE_PAK_SIZE);
	LOG_PROCESS_ERROR(piBuffer);

	pSyncPageRespond = (R2S_SYNC_TONG_REPERTORY_PAGE_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pSyncPageRespond);

	pSyncPageRespond->wProtocolID       = r2s_sync_tong_repertory_page_respond;
	pSyncPageRespond->dwPlayerID        = dwPlayerID;
	pSyncPageRespond->byPageIndex       = (BYTE)nPageIndex;
	pSyncPageRespond->nServerCounter    = pTong->m_Repertory.m_nUpdateCounter;

	if (nPageIndex == 0)
	{
		pSyncPageRespond->byGetRight = true;
	}
	else
	{
		bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totGetFromRepositoryPage2 + nPageIndex - 1);
		pSyncPageRespond->byGetRight    = bRetCode ? 1 : 0;
	}

	if (nPageIndex == 0)
	{
		pSyncPageRespond->byPutRight = 1;
	}
	else
	{
		bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totPutRepositoryPage2 + nPageIndex - 1);
		pSyncPageRespond->byPutRight = bRetCode ? 1 : 0;
	}

	pbyOffset = pSyncPageRespond->byData;
	uLeftSize = MAX_SYNC_TONG_REPERTORY_PAGE_PAK_SIZE - sizeof(R2S_SYNC_TONG_REPERTORY_PAGE_RESPOND);
	LOG_PROCESS_ERROR(uLeftSize > 0);

	// --------------------------------------------------------------------------------------------------
	nCount      = 0;
	uUsedSize   = 0;
	bRetCode = pTong->m_Repertory.GetSyncPageUpdateGridData(nPageIndex, nLastUpdateCounter, pbyOffset, uLeftSize, &nCount, &uUsedSize);
	LOG_PROCESS_ERROR(bRetCode);

	pSyncPageRespond->byUpdateItemCount = (BYTE)nCount;

	pbyOffset += uUsedSize;
	uLeftSize -= uUsedSize;
	LOG_PROCESS_ERROR(uLeftSize > 0);

	// --------------------------------------------------------------------------------------------------
	nCount      = 0;
	uUsedSize   = 0;
	bRetCode = pTong->m_Repertory.GetSyncPageFreeGridData(nPageIndex, nLastUpdateCounter, pbyOffset, uLeftSize, &nCount, &uUsedSize);
	LOG_PROCESS_ERROR(bRetCode);

	pSyncPageRespond->byInvailedItemCount = (BYTE)nCount;

	pbyOffset += uUsedSize;
	uLeftSize -= uUsedSize;
	LOG_PROCESS_ERROR(uLeftSize >= 0);

	// --------------------------------------------------------------------------------------------------
	hRetCode = piBuffer->QueryInterface(IID_IMemBlock_Resizable, (void**)&piResize);
	LOG_COM_PROCESS_ERROR(hRetCode);

	bRetCode = piResize->SetSmallerSize((unsigned)(MAX_SYNC_TONG_REPERTORY_PAGE_PAK_SIZE - uLeftSize));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;

EXIT0:
	SAFE_RELEASE(piBuffer);
	SAFE_RELEASE(piResize);

	return bResult;
}

BOOL KinHandler::DoUpdateTongBaseInfo(int nConnIndex, DWORD dwPlayerID, KTong* pTong)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
	R2S_APPLY_TONG_INFO_RESPOND*    pApplyRespond   = NULL;
	KTONG_BASE_INFO*                pBaseInfo       = NULL;

	piPackage       = QCreateMemBlock(sizeof(R2S_APPLY_TONG_INFO_RESPOND) + sizeof(KTONG_BASE_INFO));
	LOG_PROCESS_ERROR(piPackage);

	pApplyRespond   = (R2S_APPLY_TONG_INFO_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApplyRespond);

	pApplyRespond->wProtocolID  = r2s_apply_tong_info_respond;
	pApplyRespond->dwPlayerID   = dwPlayerID;
	pApplyRespond->byInfoType   = (BYTE)tdtBaseInfo;

	pBaseInfo                       = (KTONG_BASE_INFO*)pApplyRespond->byData;
	pBaseInfo->dwMasterID           = pTong->m_dwMaster;
	pBaseInfo->byLevel              = (BYTE)pTong->m_nLevel;
	pBaseInfo->byCamp               = (BYTE)pTong->m_byCamp;
	pBaseInfo->nMaxMemberCount      = pTong->m_nMaxMemberCount;
	pBaseInfo->byState              = (BYTE)pTong->m_eState;
	pBaseInfo->nStateTimer			= 0;
	pBaseInfo->byFreeJoin			= (BYTE)pTong->m_bFreeJoin;
	pBaseInfo->dwPearlNum           = pTong->m_dwPearlNum;
	pBaseInfo->dwJadeiteNum         = pTong->m_dwJadeiteNum;
	pBaseInfo->dwAgateNum           = pTong->m_dwAgateNum;
	pBaseInfo->dwDiamondNum         = pTong->m_dwDiamondNum;
	//pBaseInfo->nStateTimerOffset    = 0;

	//if (pTong->m_nStateTimer)
	//{
	//	pBaseInfo->nStateTimerOffset    = pTong->m_nStateTimer - g_pGameCenter->m_nTimeNow;
	//}    

	strncpy(pBaseInfo->szTongName, pTong->m_szName, sizeof(pBaseInfo->szTongName));
	pBaseInfo->szTongName[sizeof(pBaseInfo->szTongName) - 1] = '\0';

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoUpdateTongDevelopmentInfo(int nConnIndex, DWORD dwPlayerID, KTong* pTong)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
	R2S_APPLY_TONG_INFO_RESPOND*    pApplyRespond   = NULL;
	KTONG_DEVELOPMENT_INFO*         pDevelopment    = NULL;

	piPackage       = QCreateMemBlock(sizeof(R2S_APPLY_TONG_INFO_RESPOND) + sizeof(KTONG_DEVELOPMENT_INFO));
	LOG_PROCESS_ERROR(piPackage);

	pApplyRespond   = (R2S_APPLY_TONG_INFO_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApplyRespond);

	pApplyRespond->wProtocolID          = r2s_apply_tong_info_respond;
	pApplyRespond->dwPlayerID           = dwPlayerID;
	pApplyRespond->byInfoType           = (BYTE)tdtDevelopmentInfo;

	pDevelopment                        = (KTONG_DEVELOPMENT_INFO*)pApplyRespond->byData;
	pDevelopment->nFound                = pTong->m_nFund;
	pDevelopment->nMaxWageRate          = pTong->m_nMaxWageRate;
	pDevelopment->nDevelopmentPoint     = pTong->m_nDevelopmentPoint;
	pDevelopment->nUsedDevelopmentPoint = pTong->m_nUsedDevelopmentPoint;

	assert(sizeof(pDevelopment->m_byTechTree) == sizeof(pTong->m_byTechTree));
	memcpy(pDevelopment->m_byTechTree, pTong->m_byTechTree, sizeof(pDevelopment->m_byTechTree));

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoUpdateTongTextInfo(int nConnIndex, DWORD dwPlayerID, 
									   int nDataType, const char cszText[]
)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	int                             nRetCode        = 0;
	HRESULT                         hRetCode        = E_FAIL;
	IMemBlock*                     piPackage       = NULL;
	R2S_APPLY_TONG_INFO_RESPOND*    pApplyRespond   = NULL;
	KTONG_TEXT_INFO*                pTextInfo       = NULL;
	size_t                          uStrLen         = 0;

	assert(cszText);

	uStrLen = strlen(cszText);

	piPackage = QCreateMemBlock((unsigned int)sizeof(R2S_APPLY_TONG_INFO_RESPOND) + sizeof(KTONG_TEXT_INFO) + uStrLen + 1);
	LOG_PROCESS_ERROR(piPackage);

	pApplyRespond = (R2S_APPLY_TONG_INFO_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApplyRespond);

	pApplyRespond->wProtocolID   = r2s_apply_tong_info_respond;
	pApplyRespond->dwPlayerID    = dwPlayerID;
	pApplyRespond->byInfoType    = (BYTE)nDataType;

	pTextInfo = (KTONG_TEXT_INFO*)pApplyRespond->byData;

	strcpy((char*)pTextInfo->byContent, cszText);

	pTextInfo->uDataLen = uStrLen + 1;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoUpdateTongSchemaInfo(int nConnIndex, DWORD dwPlayerID, KTongSchemaData* pData)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
	R2S_APPLY_TONG_INFO_RESPOND*    pApplyRespond   = NULL;
	KTONG_SCHEMA_DATA_INFO*         pUpdateGroup    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_APPLY_TONG_INFO_RESPOND) + sizeof(KTONG_SCHEMA_DATA_INFO));
	LOG_PROCESS_ERROR(piPackage);

	pApplyRespond = (R2S_APPLY_TONG_INFO_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApplyRespond);

	pApplyRespond->wProtocolID   = r2s_apply_tong_info_respond;
	pApplyRespond->dwPlayerID    = dwPlayerID;
	pApplyRespond->byInfoType    = (BYTE)tdtSchemaInfo;

	pUpdateGroup = (KTONG_SCHEMA_DATA_INFO*)pApplyRespond->byData;
	memcpy(&pUpdateGroup->SchemaData, pData, sizeof(pUpdateGroup->SchemaData));

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoUpdateTongMemorabilia(int nConnIndex, DWORD dwPlayerID, KTong* pTong)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
	R2S_APPLY_TONG_INFO_RESPOND*    pApplyRespond   = NULL;
	KTONG_MEMORABILIA_INFO*         pInfo           = NULL;
	int                             nCount          = 0;

	nCount = (int)pTong->m_Memorabilia.size();

	piPackage = QCreateMemBlock(
		sizeof(R2S_APPLY_TONG_INFO_RESPOND) + sizeof(KTONG_MEMORABILIA_INFO) + sizeof(KTongIncidentRecord) * nCount
		);
	LOG_PROCESS_ERROR(piPackage);

	pApplyRespond = (R2S_APPLY_TONG_INFO_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApplyRespond);

	pApplyRespond->wProtocolID  = r2s_apply_tong_info_respond;
	pApplyRespond->dwPlayerID   = dwPlayerID;
	pApplyRespond->byInfoType   = (BYTE)tdtMemorabilia;

	pInfo = (KTONG_MEMORABILIA_INFO*)pApplyRespond->byData;
	pInfo->byCount = (BYTE)nCount;

	if (nCount > 0)
	{
		memcpy(&pInfo->Memorabilia[0], &pTong->m_Memorabilia[0], sizeof(KTongIncidentRecord) * nCount);
	}

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoUpdateTongClientDataVersion(int nConnIndex, DWORD dwPlayerID, KTONG_UPDATE_FLAG eFrameType, int nUpdateFrame)
{
	BOOL                                    bResult             = false;
	BOOL                                    bRetcode            = false;
	IMemBlock*                             piPackage           = NULL;
	R2S_UPDATE_TONG_CLIENT_DATA_VERSION*    pServerDataVersion  = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_UPDATE_TONG_CLIENT_DATA_VERSION));
	LOG_PROCESS_ERROR(piPackage);

	pServerDataVersion = (R2S_UPDATE_TONG_CLIENT_DATA_VERSION*)piPackage->GetData();
	LOG_PROCESS_ERROR(pServerDataVersion);

	pServerDataVersion->wProtocolID     = r2s_update_tong_client_data_version;
	pServerDataVersion->dwPlayerID      = dwPlayerID;
	pServerDataVersion->byCountType		= eFrameType;
	pServerDataVersion->nUpdateCount	= nUpdateFrame;
	//pServerDataVersion->byFrameType     = (BYTE)eFrameType;
	//pServerDataVersion->nUpdateFrame    = nUpdateFrame;

	bRetcode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoInvitePlayerJoinTongRequest(
	int nConnIndex, DWORD dwPlayerID, DWORD dwInviterID, DWORD dwTongID, BYTE byTongCamp, char szInviterName[], char szTongName[]
)
{
	BOOL                                    bResult             = false;
	BOOL                                    bRetcode            = false;
	IMemBlock*                             piPackage           = NULL;
	R2S_INVITE_PLAYER_JOIN_TONG_REQUEST*    pRequest            = NULL;

	assert(szInviterName);
	assert(szTongName);

	piPackage = QCreateMemBlock(sizeof(R2S_INVITE_PLAYER_JOIN_TONG_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest  = (R2S_INVITE_PLAYER_JOIN_TONG_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = r2s_invite_player_join_tong_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->dwInviterID   = dwInviterID;
	pRequest->dwTongID      = dwTongID;
	pRequest->byTongCamp    = byTongCamp;

	strncpy(pRequest->szInviterName, szInviterName, sizeof(pRequest->szInviterName));
	pRequest->szInviterName[sizeof(pRequest->szInviterName) - 1] = '\0';

	strncpy(pRequest->szTongName, szTongName, sizeof(pRequest->szTongName));
	pRequest->szTongName[sizeof(pRequest->szTongName) - 1] = '\0';

	bRetcode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinHandler::DoChangePlayerTongNotify(int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, KCHANGE_TONG_REASON eReason, char szTongName[])
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piBuffer    = NULL;
	R2S_CHANGE_PLAYER_TONG_NOTIFY*  pNotify     = NULL;

	QLogPrintf(LOG_INFO,"..KinHandler::DoChangePlayerTongNotify eReason=%d",eReason);

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_CHANGE_PLAYER_TONG_NOTIFY));
	LOG_PROCESS_ERROR(piBuffer);

	pNotify = (R2S_CHANGE_PLAYER_TONG_NOTIFY*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID    = r2s_change_player_tong_notify;
	pNotify->dwPlayerID     = dwPlayerID;
	pNotify->dwTongID       = dwTongID;
	pNotify->byReason       = (BYTE)eReason;
	strncpy(pNotify->szTongName, szTongName, sizeof(pNotify->szTongName));
	pNotify->szTongName[sizeof(pNotify->szTongName) - 1] = '\0';

	pNotify->dwKinMasterId = 0;
	if(pNotify->dwTongID > 0)
	{
		KTong* pTong = g_pGameCenter->m_TongManager.GetTong(pNotify->dwTongID);
		LOG_PROCESS_ERROR(pTong);
		pNotify->dwKinMasterId = pTong->m_dwMaster;
	}

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::KTONG_BROADCAST_MESSAGE_TRAVERSE::operator () (KTongMember* pMember)
{
	KRole* pReceiver = NULL;

	PROCESS_ERROR(pMember->dwID != dwExceptID);

	pReceiver = g_pGameCenter->m_RoleManager.GetRole(pMember->dwID);
	LOG_PROCESS_ERROR(pReceiver);

	pMessage->dwPlayerID = pReceiver->m_dwPlayerID;
	
	g_pGameCenter->m_GameServer.Send(pReceiver->m_nConnIndex, piSendBuff);
EXIT0:
	return true;
}

BOOL KinHandler::KTONG_SYNC_KIN_LIST_TRAVERSE::operator () (KTongMember* pMember)
{
	KRole* pReceiver = NULL;

	PROCESS_ERROR(pMember->dwID != dwExceptID);

	pReceiver = g_pGameCenter->m_RoleManager.GetRole(pMember->dwID);
	LOG_PROCESS_ERROR(pReceiver);

	if (pMember->nGroupID == DEFAULT_GROUP_INDEX || !pMember->bIsOnline)
	{
		goto EXIT0;
	}

	pSynList->dwPlayerId = pMember->dwID;

	g_pGameCenter->m_GameServer.Send(pReceiver->m_nConnIndex, piSendBuff);
EXIT0:
	return true;
}

BOOL KinHandler::DoTongBroadcastMessage(KTong* pTong, int nCode, DWORD dwExceptID, BYTE* pbyData, size_t uDataLen)
{
	BOOL                                bResult     = false;
	IMemBlock*                         piBuffer    = NULL;
	R2S_TONG_BROADCAST_MESSAGE*         pMessage    = NULL;
	KTONG_BROADCAST_MESSAGE_TRAVERSE    Traverse;

	assert(pTong);
	LOG_PROCESS_ERROR(nCode > tbmInvalid && nCode < tbmTotal);

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_TONG_BROADCAST_MESSAGE) + uDataLen);
	LOG_PROCESS_ERROR(piBuffer);

	pMessage = (R2S_TONG_BROADCAST_MESSAGE*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pMessage);

	pMessage->wProtocolID   = r2s_tong_broadcast_message;
	pMessage->dwPlayerID    = 0;
	pMessage->byCode        = (BYTE)nCode;

	if (uDataLen)
	{
		assert(pbyData);
		memcpy(pMessage->byData, pbyData, uDataLen);
	}

	Traverse.piSendBuff  = piBuffer;
	Traverse.pMessage    = pMessage;
	Traverse.dwExceptID  = dwExceptID;

	pTong->TraverseMember(Traverse);

EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoGetTongDescriptionRepond(int nConnIndex, DWORD dwPlayerID, KTong* pTong)
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	IMemBlock*                         piBuffer    = NULL;
	R2S_GET_TONG_DESCRIPTION_RESPOND*   pRespond    = NULL;

	assert(pTong);

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_GET_TONG_DESCRIPTION_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_GET_TONG_DESCRIPTION_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = r2s_get_tong_description_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->dwTongID      = pTong->m_dwID;
	pRespond->byTongLevel   = (BYTE)pTong->m_nLevel;
	pRespond->dwMasterID	= pTong->m_dwMaster;

	strncpy(pRespond->szTongName, pTong->m_szName, sizeof(pRespond->szTongName));
	pRespond->szTongName[sizeof(pRespond->szTongName) - 1] = '\0';

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoSyncTongOnlineMessage(int nConnIndex, DWORD dwPlayerID)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	R2S_SYNC_TONG_ONLINE_MESSAGE*   pSync       = NULL;
	IMemBlock*                     piBuffer    = NULL;
	KRole*                          pRole       = NULL;
	KTong*                          pTong       = NULL;
	size_t                          uDataLen    = 0;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	PROCESS_ERROR(pRole->m_dwTongID);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	PROCESS_ERROR(pTong->m_szOnlineMessage[0] != '\0');

	uDataLen = strlen(pTong->m_szOnlineMessage) + 1;

	piBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_SYNC_TONG_ONLINE_MESSAGE) + uDataLen));
	LOG_PROCESS_ERROR(piBuffer);

	pSync = (R2S_SYNC_TONG_ONLINE_MESSAGE*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pSync);

	assert(uDataLen <= UCHAR_MAX);

	pSync->wProtocolID  = r2s_sync_tong_online_message;
	pSync->dwPlayerID   = dwPlayerID;
	pSync->byDataLen    = (BYTE)uDataLen;

	strncpy(pSync->szOnlineMessage, pTong->m_szOnlineMessage, uDataLen);
	pSync->szOnlineMessage[uDataLen - 1] = '\0';

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoTakeTongRepertoryItemToPosRespond(
	int nConnIndex, DWORD dwPlayerID, DWORD dwTongID,
	int nPageIndex, int nRepertoryPos, int nBoxIndex, int nBoxPos, 
	size_t uSize, BYTE* pbyData 
	)
{
	BOOL                                            bResult     = false;
	BOOL                                            bRetCode    = false;
	//R2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_RESPOND*    pRespond    = NULL;
	//IMemBlock*                                     piBuffer    = NULL;

	//assert(dwTongID);
	//assert(pbyData);

	//piBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_RESPOND) + uSize));
	//LOG_PROCESS_ERROR(piBuffer);

	//pRespond = (R2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_RESPOND*)piBuffer->GetData();
	//LOG_PROCESS_ERROR(pRespond);

	//pRespond->wProtocolID       = r2s_take_tong_repertory_item_to_pos_respond;
	//pRespond->dwPlayerID        = dwPlayerID;
	//pRespond->dwTongID          = dwTongID;
	//pRespond->byPageIndex       = (BYTE)nPageIndex;
	//pRespond->byRepertoryPos    = (BYTE)nRepertoryPos;
	//pRespond->byBoxIndex        = (BYTE)nBoxIndex;
	//pRespond->byBoxPos          = (BYTE)nBoxPos;
	//pRespond->bySize            = (BYTE)uSize;
	//memcpy(pRespond->byData, pbyData, uSize);

	//bRetCode = Send(nConnIndex, piBuffer);
	//LOG_PROCESS_ERROR(bRetCode);

	//bResult = true;
//EXIT0:
//	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoTakeTongRepertoryItemRespond(
	int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, DWORD dwPutPlayerID, time_t tRentPeriod,
	int nBoxIndex, int nBoxPos, int nPageIndex, int nPagePos, 
	size_t uSize, BYTE* pbyData)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND*   pRespond    = NULL;
	IMemBlock*                              piBuffer    = NULL;

	assert(dwTongID);
	assert(pbyData);

	piBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND) + uSize));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = r2s_take_tong_repertory_item_respond;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->dwPutItemPlayerID = dwPutPlayerID;
	pRespond->tRentPeriod         = tRentPeriod;
	pRespond->dwTongID          = dwTongID;
	pRespond->byBoxIndex        = nBoxIndex;
	pRespond->byBoxPos          = nBoxPos;
	pRespond->byPageIndex       = (BYTE)nPageIndex;
	pRespond->byPagePos         = (BYTE)nPagePos;
	pRespond->byItemDataLen     = (BYTE)uSize;
	memcpy(pRespond->byItemData, pbyData, uSize);

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoPutTongRepertoryItemRespond(
	DWORD dwPlayerID, int nConnIndex, DWORD dwTongID,
	DWORD dwItemIdx,BYTE byBoxIndex, BYTE byBoxPos, BYTE byPageIndex, BYTE byRepertoryPos,
	BOOL bFindItem
	)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	IMemBlock*                             piBuffer    = NULL;
	R2S_PUT_TONG_REPERTORY_ITEM_RESPOND*    pRespond    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_PUT_TONG_REPERTORY_ITEM_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_PUT_TONG_REPERTORY_ITEM_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = r2s_put_tong_repertory_item_respond;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->dwTongID          = dwTongID;
	pRespond->dwItemID          = dwItemIdx;
	pRespond->bFindItem         = bFindItem;
	pRespond->byBoxIndex        = byBoxIndex;
	pRespond->byBoxPos          = byBoxPos;
	pRespond->byPageIndex       = byPageIndex;
	pRespond->byPagePos         = byRepertoryPos;

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoApplyStackTongRepertoryItemRespond(
	int nConnIndex, DWORD dwPlayerID, DWORD dwTongID,
	BYTE byPageIndex, BYTE byPagePos, BYTE byBoxIndex, BYTE byBoxPos, DWORD dwItemID,
	BYTE byTabType, WORD wTabIndex, WORD wStackNum
	)
{
	BOOL                                            bResult    = false;
	BOOL                                            bRetCode   = false;
//	IMemBlock*                                     piBuffer = NULL;
//	R2S_APPLY_STACK_TONG_REPERTORY_ITEM_RESPOND*    pRespond = NULL;
//
//	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_APPLY_STACK_TONG_REPERTORY_ITEM_RESPOND));
//	LOG_PROCESS_ERROR(piBuffer);
//
//	pRespond = (R2S_APPLY_STACK_TONG_REPERTORY_ITEM_RESPOND*)piBuffer->GetData();
//	LOG_PROCESS_ERROR(pRespond);
//
//	pRespond->wProtocolID   = r2s_apply_stack_tong_repertory_item_respond;
//	pRespond->dwPlayerID    = dwPlayerID;
//	pRespond->dwTongID      = dwTongID;
//	pRespond->byPageIndex   = byPageIndex;
//	pRespond->byPagePos     = byPagePos;
//	pRespond->byBoxIndex    = byBoxIndex;
//	pRespond->byBoxPos      = byBoxPos;
//	pRespond->dwItemID      = dwItemID;
//	pRespond->wStackNum     = wStackNum;
//
//	bRetCode = Send(nConnIndex, piBuffer);
//	LOG_PROCESS_ERROR(bRetCode);
//
//	bResult = true;
//EXIT0:
//	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoApplyOpenTongRepertoryRespond(int nConnIndex, DWORD dwPlayerID, KTong* pTong)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	R2S_APPLY_OPEN_TONG_REPERTPRY_RESPOND*  pRespond    = NULL;
	IMemBlock*                             piBuffer    = NULL;
	int                                     nPageCount  = 0;

	assert(pTong);

	piBuffer  = QCreateMemBlock((unsigned)sizeof(R2S_APPLY_OPEN_TONG_REPERTPRY_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_APPLY_OPEN_TONG_REPERTPRY_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = r2s_apply_open_tong_repertpry_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	strncpy(pRespond->szTongName, pTong->m_szName, sizeof(pRespond->szTongName));
	pRespond->szTongName[sizeof(pRespond->szTongName) - 1] = '\0';

	nPageCount = 0;
	for (int i = 0; i < TONG_REPERTORY_PAGE_NUM; i++)
	{
		if (!pTong->m_Repertory.m_Page[i].bEnable)
		{
			break;
		}
		nPageCount++;
	}

	pRespond->byRepertoryPageNum = (BYTE)nPageCount;

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoGetTongSalaryRespond(int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, int nSilver)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	R2S_GET_TONG_SALARY_RESPOND*    pRespond    = NULL;
	IMemBlock*                     piBuffer    = NULL;

	piBuffer  = QCreateMemBlock((unsigned)sizeof(R2S_GET_TONG_SALARY_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_GET_TONG_SALARY_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = r2s_get_tong_salary_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->dwTongID      = dwTongID;
	pRespond->nSilver       = nSilver;

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoSyncTongRankInfo(int nConnIndex, DWORD dwTongID, INT nRankId)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	R2S_SYNC_TONG_RANK_INFO*		pRespond    = NULL;
	IMemBlock*						piBuffer    = NULL;
	KTong*							pTong		= NULL;
	KRole*							pMaster		= NULL;

	piBuffer  = QCreateMemBlock((unsigned)sizeof(R2S_SYNC_TONG_RANK_INFO));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_SYNC_TONG_RANK_INFO*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pTong = g_pGameCenter->m_TongManager.GetTongById(dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pMaster = g_pGameCenter->m_RoleManager.GetRole(pTong->m_dwMaster);
	LOG_PROCESS_ERROR(pMaster);

	pRespond->wProtocolID   = r2s_sync_tong_rank_info;
	pRespond->nRankId = nRankId;
	pRespond->dwTongId      = dwTongID;
	pRespond->nTongMemberNum = pTong->GetMemberNum();

	strncpy(pRespond->szTongName, pTong->m_szName, sizeof(pRespond->szTongName));
	pRespond->szTongName[sizeof(pRespond->szTongName) - 1] = '\0';

	strncpy(pRespond->szMasterName, pMaster->m_szName, sizeof(pRespond->szMasterName));
	pRespond->szMasterName[sizeof(pRespond->szMasterName) - 1] = '\0';

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoSyncTongHistoryRespond(DWORD dwPlayerID, int nType, DWORD dwStartIndex, int nSyncRecordCount, BYTE byBuffer[], size_t uBufferSize)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	R2S_SYNC_TONG_HISTORY_RESPOND*  pRespond    = NULL;
	IMemBlock*                     piBuffer    = NULL;
	KRole*                          pRole       = NULL;
	int                             nConnIndex  = 0;

	assert(byBuffer);
	LOG_PROCESS_ERROR(uBufferSize > 0);
	LOG_PROCESS_ERROR(dwPlayerID != KD_BAD_ID);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	nConnIndex = pRole->m_nConnIndex;
	PROCESS_ERROR(nConnIndex);

	piBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_SYNC_TONG_HISTORY_RESPOND) + uBufferSize));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_SYNC_TONG_HISTORY_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = r2s_sync_tong_history_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->byType        = (BYTE)nType;
	pRespond->dwStartIndex  = dwStartIndex;
	pRespond->byCount       = (BYTE)nSyncRecordCount;
	memcpy(pRespond->byData, byBuffer, uBufferSize);

	bRetCode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinHandler::DoKinMessageNotify(int nConnIndex, DWORD dwPlayerID, int nCode, const char* pszText /*= NULL*/, int nLen /*= 0*/)
{
	BOOL                                bResult             = false;
	BOOL                                bRetCode            = false;
	size_t                              uTextLen            = 0;
	IMemBlock*							piSendBuffer        = NULL;
	R2S_KIN_MESSAGE_NOTIFY*				pMsg                = NULL;

	if (nLen)
	{
		uTextLen = nLen;
	}
	else if (pszText)
	{
		uTextLen = strlen(pszText) + 1;
	}

	piSendBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_KIN_MESSAGE_NOTIFY) + uTextLen));
	LOG_PROCESS_ERROR(piSendBuffer);

	pMsg = (R2S_KIN_MESSAGE_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pMsg);

	pMsg->wProtocolID = r2s_kin_message_notify;
	pMsg->dwPlayerID  = dwPlayerID;
	pMsg->nCode       = nCode;

	if (pszText)
	{
		memcpy(pMsg->byData, pszText, uTextLen);
	}

	bRetCode = Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}


BOOL KinHandler::DoGetBackItemFromRepertoryRespond( DWORD dwPlayerID, BYTE byBoxIndex, BYTE byBoxPos, BOOL bSuccess )
{
	BOOL                                       bResult             = false;
	BOOL                                       bRetCode            = false;
	size_t                                     uTextLen            = 0;
	IMemBlock*							       piSendBuffer        = NULL;
	R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND*  pRespond            = NULL;
	KRole*                                     pRole               = NULL;
	int                                        nConnIndex          = 0;

	piSendBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND)));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	nConnIndex = pRole->m_nConnIndex;
	PROCESS_ERROR(nConnIndex);

	pRespond->wProtocolID = r2s_get_back_item_from_repertory_respond;
	pRespond->dwPlayerID  = dwPlayerID;
	pRespond->byBoxIndex  = byBoxIndex;
	pRespond->byBoxPos    = byBoxPos;
	pRespond->bySuccess   = bSuccess;

	bRetCode = Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KinHandler::DoSyncTongActivityOpenTimes(int nConnIndex, DWORD dwPlayerId, DWORD dwTongId)
{
	BOOL bRetCode = FALSE;
	INT nDataLen = 0;
	IMemBlock* piMemBlock = NULL;
	R2S_SYNC_KIN_ACTIVITY_OPENTIMES* pSyncData = NULL;
	SyncKinActivityInfo* pcOpenTimesData = NULL;
	INT nIndex = 0;
	std::map<DWORD, SyncKinActivityInfo>::iterator it;
	KTong* pTong = g_pGameCenter->m_TongManager.GetTong(dwTongId);
	LOG_PROCESS_ERROR(pTong);

	PROCESS_ERROR(pTong->m_ActivityOpenTimeMap.size() > 0);

    nDataLen = sizeof(R2S_SYNC_KIN_ACTIVITY_OPENTIMES) + pTong->m_ActivityOpenTimeMap.size() * sizeof(SyncKinActivityInfo);
    piMemBlock = QCreateMemBlock(nDataLen);
	LOG_PROCESS_ERROR(piMemBlock);

	pSyncData = (R2S_SYNC_KIN_ACTIVITY_OPENTIMES*)piMemBlock->GetData();
	pSyncData->wProtocolID = r2s_sync_kin_opentimes;
	pSyncData->dwPlayerId = dwPlayerId;
	pSyncData->nCount = (DWORD)pTong->m_ActivityOpenTimeMap.size();
    pcOpenTimesData = (SyncKinActivityInfo*)pSyncData->byOpenTimesData;

	for (it = pTong->m_ActivityOpenTimeMap.begin(); it != pTong->m_ActivityOpenTimeMap.end(); ++it)
	{
		pcOpenTimesData[nIndex++] = it->second;
	}

	bRetCode = Send(nConnIndex, piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL KinHandler::DoSyncTongApplyJoinList(DWORD dwTongId)
{
	Q_Printl("..KinHandler::DoSyncTongApplyJoinList dwTongId="<<dwTongId<<" m_TongTable.size()="<<g_pGameCenter->m_TongManager.m_TongTable.size());
	BOOL                bResult 	= FALSE;
    KTong* 				pTong 		= NULL;
	KRole* 				pRole 		= NULL;
	INT					nDataLen 	= 0;
	IMemBlock* 			piMemBlock 	= NULL;
	R2S_SYNC_TONG_APPLY_JOIN_LIST* 	pSynData = NULL;
	QTongApplyerInfo* 	pApplyerData = NULL;

	pTong = g_pGameCenter->m_TongManager.GetTong(dwTongId);
	LOG_PROCESS_ERROR(pTong);

    pRole = g_pGameCenter->m_RoleManager.GetRole(pTong->m_dwMaster);
	LOG_PROCESS_ERROR(pRole);

    nDataLen = sizeof(R2S_SYNC_TONG_APPLY_JOIN_LIST) + sizeof(QTongApplyerInfo) * (INT)pTong->m_ApplyJoinTongPlayers.size();
	piMemBlock = QCreateMemBlock(nDataLen);
	LOG_PROCESS_ERROR(piMemBlock);

	pSynData = (R2S_SYNC_TONG_APPLY_JOIN_LIST*)piMemBlock->GetData();
	pSynData->wProtocolID = r2s_sync_apply_join_tong_list;
	pSynData->dwCount = (DWORD)pTong->m_ApplyJoinTongPlayers.size();

	pApplyerData = (QTongApplyerInfo*)pSynData->byData;
	LOG_PROCESS_ERROR(pApplyerData);
	for (INT i = 0; i < pTong->m_ApplyJoinTongPlayers.size(); i++)
	{
		pApplyerData[i].nPlayerId = (INT)pTong->m_ApplyJoinTongPlayers[i];

		pRole = g_pGameCenter->m_RoleManager.GetRole(pTong->m_ApplyJoinTongPlayers[i]);
		LOG_PROCESS_ERROR(pRole);

		pApplyerData[i].nLevel = (INT)pRole->m_byLevel;
		pApplyerData[i].dwLastLogoutTime = (DWORD)pRole->m_nLastSaveTime;
		strcpy(pApplyerData[i].szName, pRole->m_szName);
		pApplyerData[i].nFightScore = pRole->m_nFightScore;
	}

	KTONG_SYNC_KIN_LIST_TRAVERSE cTraverse;
	cTraverse.piSendBuff  = piMemBlock;
	cTraverse.pSynList    = pSynData;
	cTraverse.dwExceptID  = KD_BAD_ID;

	pTong->TraverseMember(cTraverse);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bResult;
}

BOOL KinHandler::DoGetFreeJoinTongListRespond( DWORD dwPlayerID, BYTE byTongNum, BYTE* pbyData, INT nDataLen, INT nUpdateCounter )
{
	BOOL								bRetCode = FALSE;
	IMemBlock*							piMemBlock = NULL;
	R2S_GET_FREEJOIN_TONG_LIST_RESPOND* pRespond = NULL;
	KRole*								pRole = NULL;
	INT									nConnectIndex = 0;

	LOG_PROCESS_ERROR(dwPlayerID != KD_BAD_ID);
	LOG_PROCESS_ERROR(byTongNum >= 0);
	LOG_PROCESS_ERROR(pbyData);
	LOG_PROCESS_ERROR(nDataLen >= 0);

	piMemBlock = QCreateMemBlock(sizeof(R2S_GET_FREEJOIN_TONG_LIST_RESPOND) + nDataLen);
	LOG_PROCESS_ERROR(piMemBlock);

	pRespond = (R2S_GET_FREEJOIN_TONG_LIST_RESPOND*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	nConnectIndex = pRole->m_nConnIndex;
	LOG_PROCESS_ERROR(nConnectIndex);

	pRespond->wProtocolID		= r2s_get_freejoin_tong_list_respond;
	pRespond->dwPlayerID		= dwPlayerID;
	pRespond->byTongNum			= byTongNum;
	pRespond->nUpdateCounter	= nUpdateCounter;
	memcpy(pRespond->byData, pbyData, nDataLen);

	bRetCode = Send(nConnectIndex, piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL KinHandler::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}