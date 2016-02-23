
#include "stdafx.h"
#include "KinModule.h"
#include "KinRelayClient.h"
#include "onlinegamemodule/kinprotocol.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/kmoduledef.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "serverbase/kgcmoduledef.h"
#include "onlinegameworld/ikscene.h"
#include "item2/kplayeritem.h"
#include "onlinegamebase/ksysservice.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "Runtime/TypeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


QKinModule g_KinModule;

#define REGISTER_EXTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_anProtocolSizeC2S[ProtocolID] = ProtocolSize;}						


template<>
IKModuleInterface* GetModuleInterFace<emMODULE_KIN>()
{
	return &g_KinModule;
}

//QKinModule::PROCESS_PROTOCOL_FUNC	QKinModule::m_ProcessProtocolFuns[c2s_tong_end] = {0};
//size_t QKinModule::m_nProtocolSize[c2s_tong_end] = {0};

BYTE	QKinModule::m_byTempData[32*1024] = {0};

// -----------------------------------------------------------

class KinBroadcastHelper : public IKTraverseHelper
{
public:
	KinBroadcastHelper(S2C_BROADCAST_PLAYER_CHANGE_TONG& pkg)
		: m_pkg(pkg)
	{
	}

	virtual BOOL OnTraverse(KSceneObject* pObj)
	{
		ASSERT(pObj->GetId());
		KPlayer* pPlayer = (KPlayer*)pObj;
		g_KinModule.SendData(pPlayer->GetPlayerIndex(), m_pkg);
		return TRUE;
	}

protected:
	S2C_BROADCAST_PLAYER_CHANGE_TONG& m_pkg;
};

//------------------------------------------------------------

BOOL QKinModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(S2C_SYNC_TONG_MEMBER_INFO);
	KD_PTC_FIXED_SIZE_S2C(S2C_DELETE_TONG_MEMBER_NOTIFY);
	KD_PTC_FIXED_SIZE_S2C(S2C_UPDATE_TONG_CLIENT_DATA_VERSION);
	KD_PTC_FIXED_SIZE_S2C(S2C_SYNC_TONG_REPERTORY_PAGE_FRAME);
	KD_PTC_FIXED_SIZE_S2C(S2C_INVITE_PLAYER_JOIN_TONG_REQUEST);
	KD_PTC_FIXED_SIZE_S2C(S2C_CHANGE_PLAYER_TONG_NOTIFY);
	KD_PTC_FIXED_SIZE_S2C(S2C_GET_TONG_DESCRIPTION_RESPOND);
	KD_PTC_FIXED_SIZE_S2C(S2C_BROADCAST_PLAYER_CHANGE_TONG);
	KD_PTC_FIXED_SIZE_S2C(S2C_APPLY_OPEN_TONG_REPERTORY_RESPOND);

	KD_PTC_LENGTNED_SIZE_S2C(S2C_APPLY_TONG_INFO_RESPOND);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_SYNC_TONG_REPERTORY_PAGE_RESPOND);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_TONG_BROADCAST_MESSAGE);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_SYNC_TONG_ONLINE_MESSAGE);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_SYNC_TONG_HISTORY_RESPOND);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_KIN_MESSAGE_NOTIFY);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_GET_FREEJOIN_TONG_LIST_RESPOND);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_SYNC_APPLY_JOIN_TONG_LIST);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_SYNC_ACTIVITY_OPENTIMS);

	return TRUE;
}

BOOL QKinModule::OnInit()
{
	REGISTER_EXTERNAL_FUNC(c2s_update_tong_roster_request, &QKinModule::OnApplyTongRosterRequest, -1); // 变长
	REGISTER_EXTERNAL_FUNC(c2s_apply_tong_info_request, &QKinModule::OnApplyTongInfoRequest, sizeof(C2S_APPLY_TONG_INFO_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_apply_tong_repertory_page_request, &QKinModule::OnApplyTongRepertoryPageRequest, sizeof(C2S_APPLY_TONG_REPERTORY_PAGE_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_invite_player_join_tong_request, &QKinModule::OnInvitePlayerJoinTongRequest, sizeof(C2S_INVITE_PLAYER_JOIN_TONG_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_invite_player_join_tong_respond, &QKinModule::OnInvitePlayerJoinTongRespond, sizeof(C2S_INVITE_PLAYER_JOIN_TONG_RESPOND));
	REGISTER_EXTERNAL_FUNC(c2s_apply_kick_out_tong_member_request, &QKinModule::OnApplyKickOutTongMemberRequest, sizeof(C2S_APPLY_KICK_OUT_TONG_MEMBER_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_modify_tong_info_request, &QKinModule::OnModifyTongInfoRequest, -1); // 变长
	REGISTER_EXTERNAL_FUNC(c2s_modify_tong_schema_request, &QKinModule::OnModifyTongSchemaRequest, -1); // 变长
	REGISTER_EXTERNAL_FUNC(c2s_apply_quit_tong_request, &QKinModule::OnApplyQuitTongRequest, sizeof(C2S_APPLY_QUIT_TONG_REQUEST));

	REGISTER_EXTERNAL_FUNC(c2s_change_tong_member_group_request, &QKinModule::OnChangeTongMemberGroupRequest, sizeof(C2S_CHANGE_TONG_MEMBER_GROUP_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_change_tong_master_request, &QKinModule::OnChangeTongMasterRequest, sizeof(C2S_CHANGE_TONG_MASTER_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_change_tong_member_remark_request, &QKinModule::OnChangeTongMemberRemarkRequest, sizeof(C2S_CHANGE_TONG_MEMBER_REMARK_REQUEST));

	REGISTER_EXTERNAL_FUNC(c2s_get_tong_description_request, &QKinModule::OnGetTongDescriptionRequest, sizeof(C2S_GET_TONG_DESCRIPTION_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_save_money_in_tong_request, &QKinModule::OnSaveMoneyInTongRequest, sizeof(C2S_SAVE_MONEY_IN_TONG_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_pay_tong_salary_request, &QKinModule::OnPayTongSalaryRequest, sizeof(C2S_PAY_TONG_SALARY_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_get_tong_salary_request, &QKinModule::OnGetTongSalaryRequest, sizeof(C2S_GET_TONG_SALARY_REQUEST));

	REGISTER_EXTERNAL_FUNC(c2s_apply_open_tong_repertory_request, &QKinModule::OnApplyOpenTongRepertoryRequest, sizeof(C2S_APPLY_OPEN_TONG_REPERTORY_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_take_tong_repertory_item_request, &QKinModule::OnTakeTongRepertoryItemRequest, sizeof(C2S_TAKE_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_put_tong_repertory_item_request, &QKinModule::OnPutTongRepertoryItemRequest, sizeof(C2S_PUT_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_exchange_tong_repertory_item_pos_request, &QKinModule::OnExchangeTongRepertoryItemPosRequest, sizeof(C2S_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_stack_tong_repertory_item_request, &QKinModule::OnStackTongRepertoryItemRequest, sizeof(C2S_STACK_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_remove_repertory_item_request, &QKinModule::OnRemoveTongRepertoryItemRequest, sizeof(C2S_REMOVE_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_get_back_item_from_repertory_request, &QKinModule::OnGetBackItemFromRepertoryRequest, sizeof(C2S_GET_BACK_ITEM_FROM_REPERTORY_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_get_freejoin_tong_list_request, &QKinModule::OnGetFreeJoinTongListRequest, sizeof(C2S_GET_FREEJOIN_TONG_LIST_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_apply_join_tong_request, &QKinModule::OnApplyJoinTongRequest, sizeof(C2S_APPLY_JOIN_TONG_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_set_tong_freejoin_request, &QKinModule::OnSetTongFreeJoin, sizeof(C2S_SET_TONG_FREEJOIN_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_set_kin_activity_opentime, &QKinModule::OnSetKinActivityOpenTime, sizeof(C2S_SET_KIN_ACTIVITY_OPENTIME));

	ReadSetting();

	return TRUE;
}

BOOL QKinModule::OnUnInit()
{
	return TRUE;
}

BOOL QKinModule::ReadSetting()
{
	INT nHeight = 0;
	BOOL bRet = TRUE;

	QTabFile cContributionTabFile;
	bRet = cContributionTabFile.Load("/setting/kin/kincontribution.txt");
	LOG_PROCESS_ERROR(bRet);

    nHeight = cContributionTabFile.GetHeight();
	for (INT i = 2; i < nHeight; i++)
	{
		cContributionTabFile.GetInteger(i + 1, "PealExchange", 0, &m_cContributionValue.nPealExchange);
		cContributionTabFile.GetInteger(i + 1, "JadeiteExchange", 0, &m_cContributionValue.nJadeiteExchange);
		cContributionTabFile.GetInteger(i + 1, "AgateExchange", 0, &m_cContributionValue.nAgateExchange);
		cContributionTabFile.GetInteger(i + 1, "DiamondExchange", 0, &m_cContributionValue.nDiamondExchange);
		cContributionTabFile.GetInteger(i + 1, "TotalOneDay", 0, &m_cContributionValue.nTotalOneDay);
	}

EXIT0:
	return bRet;
}

BOOL QKinModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
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

	(this->*pFunc)(pPlayer, (CHAR*)pbData, uDataLen);


	bResult = TRUE;
EXIT0:
	return bResult;
}

QKinModule::QKinModule(void) : KModuleBase(emMODULE_KIN)
{
}

QKinModule::~QKinModule(void)
{
}


void QKinModule::OnApplyTongRosterRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_APPLY_TONG_ROSTER_REQUEST*  pRosterRequest  = (C2S_APPLY_TONG_ROSTER_REQUEST*)pData;

	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_APPLY_TONG_ROSTER_REQUEST) + sizeof(DWORD) * pRosterRequest->nMemberCount);
	PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	
	KinRelayClient::DoApplyTongRosterRequest(
		pPlayer->GetId(), pRosterRequest->nClientUpdateCount, 
		pRosterRequest->dwMemberList, pRosterRequest->nMemberCount
		);

EXIT0:
	return;
}

void QKinModule::OnApplyTongInfoRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_APPLY_TONG_INFO_REQUEST*    pInfoRequest  = (C2S_APPLY_TONG_INFO_REQUEST*)pData;

	PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	KinRelayClient::DoApplyTongInfoRequest(pPlayer->GetId(), pInfoRequest->nLastUpdateCount);

EXIT0:
	return;
}

void QKinModule::OnApplyTongRepertoryPageRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
//	KNpc*                                   pNpc        = NULL;
	BOOL                                    bRetCode    = false;
	C2S_APPLY_TONG_REPERTORY_PAGE_REQUEST*  pRequest    = (C2S_APPLY_TONG_REPERTORY_PAGE_REQUEST*)pData;

// 	PROCESS_ERROR(pPlayer->m_dwTongRepertoryNpcID);
// 	pNpc = g_pWorld->m_NpcSet.GetObj(pPlayer->m_dwTongRepertoryNpcID);
// 	LOG_PROCESS_ERROR(pNpc);
// 
// 	bRetCode = g_InRange(pPlayer, pNpc, COMMON_PLAYER_OPERATION_DISTANCE);
// 	LOG_PROCESS_ERROR(bRetCode);

	KinRelayClient::DoApplyTongRepertoryPageRequest(pPlayer->GetId(), pRequest->byPageIndex, pRequest->nLastUpdateCounter);

EXIT0:
	return;
}

void QKinModule::OnInvitePlayerJoinTongRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_INVITE_PLAYER_JOIN_TONG_REQUEST*    pInfoRequest    = (C2S_INVITE_PLAYER_JOIN_TONG_REQUEST*)pData;

	pInfoRequest->szNewMemberName[sizeof(pInfoRequest->szNewMemberName) - 1] = '\0';

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);

	KinRelayClient::DoInvitePlayerJoinTongRequest(pPlayer->GetId(), pInfoRequest->szNewMemberName);

EXIT0:
	return;
}

void QKinModule::OnInvitePlayerJoinTongRespond(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_INVITE_PLAYER_JOIN_TONG_RESPOND*    pConfirm    = (C2S_INVITE_PLAYER_JOIN_TONG_RESPOND*)pData;
	INVITE_TONG_RESULT_CODE                 eAccpet     = (INVITE_TONG_RESULT_CODE)pConfirm->byAccept;

	//if (pPlayer->m_bFreeLimitFlag)
	//{
	//	DoMessageNotify(nConnIndex, ectChargeLimitCode, cltYouJoinTong);
	//	eAccpet = itrChargeLimit;
	//}

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId == KD_BAD_ID);
	KinRelayClient::DoInvitePlayerJoinTongRespond(
		pPlayer->GetId(), pConfirm->dwInviterID, pConfirm->dwTongID, eAccpet
		);

EXIT0:
	return;
}

void QKinModule::OnApplyKickOutTongMemberRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_APPLY_KICK_OUT_TONG_MEMBER_REQUEST* pRequest    = (C2S_APPLY_KICK_OUT_TONG_MEMBER_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	KinRelayClient::DoApplyKickOutTongMemberRequest(pPlayer->GetId(), pRequest->dwMemberID);

EXIT0:
	return;
}

void QKinModule::OnModifyTongInfoRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_MODIFY_TONG_INFO_REQUEST*   pRequest    = (C2S_MODIFY_TONG_INFO_REQUEST*)pData;

	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_MODIFY_TONG_INFO_REQUEST) + pRequest->wDataLen);
	PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	KinRelayClient::DoModifyTongInfoRequest(pPlayer->GetId(), pRequest->byInfoType, pRequest->byData, (size_t)pRequest->wDataLen);

EXIT0:
	return;
}

void QKinModule::OnModifyTongSchemaRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_MODIFY_TONG_SCHEMA_REQUEST* pRequest    = (C2S_MODIFY_TONG_SCHEMA_REQUEST*)pData;

	LOG_PROCESS_ERROR(uDataLen > sizeof(C2S_MODIFY_TONG_SCHEMA_REQUEST));
	PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);

	KinRelayClient::DoModifyTongSchemaRequest(
		pPlayer->GetId(), pRequest->byGroupIndex, pRequest->byModifyType, 
		pRequest->byData, uDataLen - sizeof(C2S_MODIFY_TONG_SCHEMA_REQUEST)
		);

EXIT0:
	return;
}

void QKinModule::OnApplyQuitTongRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                    bResult = FALSE;
	KPlayerItem*							pPlayerItem = NULL;
	KItem*                                  pItem       = NULL;
	KE_ITEM_ROOM                            cItemRoom   = emROOM_NONE;

	LOG_PROCESS_ERROR(pPlayer);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);
	pPlayerItem->RemoveAllRentItem();

	PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	KinRelayClient::DoApplyQuitTongRequest(pPlayer->GetId());

EXIT0:
	return;
}

void QKinModule::OnChangeTongMemberGroupRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_CHANGE_TONG_MEMBER_GROUP_REQUEST*   pRequest    = (C2S_CHANGE_TONG_MEMBER_GROUP_REQUEST*)pData;

	PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	KinRelayClient::DoChangeTongMemberGroupRequest(pPlayer->GetId(), pRequest->dwTargetMemberID, (int)pRequest->byTargetGroup);

EXIT0:
	return;
}

void QKinModule::OnChangeTongMasterRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_CHANGE_TONG_MASTER_REQUEST* pRequest    = (C2S_CHANGE_TONG_MASTER_REQUEST*)pData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nRet = 0;

	cSafeScript->CallTableFunction("Kin", "DoCheckChangeMaster", 1, "o", pPlayer->GetScriptInterface());
	nRet = cSafeScript->GetInt(-1);
	LOG_PROCESS_ERROR(nRet != 0);

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	KinRelayClient::DoChangeTongMasterRequest(pPlayer->GetId(), pRequest->dwNewMasterID);
	QLogPrintf(LOG_INFO,"kin c2sOnChangeTongMasterRequest");
EXIT0:
	return;
}

void QKinModule::OnChangeTongMemberRemarkRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_CHANGE_TONG_MEMBER_REMARK_REQUEST*  pRequest    = (C2S_CHANGE_TONG_MEMBER_REMARK_REQUEST*)pData;

	pRequest->szRemark[sizeof(pRequest->szRemark) - 1] = '\0';

	PROCESS_ERROR(pPlayer->m_dwKinId != KD_BAD_ID);
	KinRelayClient::DoChangeTongMemberRemarkRequest(pPlayer->GetId(), pRequest->dwTargetMemberID, pRequest->szRemark);

EXIT0:
	return;
}

void QKinModule::OnGetTongDescriptionRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_GET_TONG_DESCRIPTION_REQUEST* pRequest = (C2S_GET_TONG_DESCRIPTION_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer);

	KinRelayClient::DoGetTongDescriptionRequest(pPlayer->GetId(), pRequest->dwTongID);
EXIT0:
	return;
}

void QKinModule::OnSaveMoneyInTongRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	int                             nOwnMoney   = 0;
    KPlayerItem*                    pPlayerItem = NULL;
	KPlayerPurse*                   pPlayerPurse = NULL;
	INT                             nPealCount  = 0;
	INT                             nJadeiteCount  = 0;
	INT                             nAgateCount  = 0;
	INT                             nDiamondCount  = 0;
	INT                             nContributon = 0;
	KE_PURSE_RESULT                 cResult;
	C2S_SAVE_MONEY_IN_TONG_REQUEST* pRequest = (C2S_SAVE_MONEY_IN_TONG_REQUEST*)pData;
	CHAR szMsgTmp[256];
	QString szKinNoticeMsg;

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);

	// 至少要有一个珠宝
	LOG_PROCESS_ERROR(pRequest->dwPealNum >= 1 || pRequest->dwJadeiteNum >= 1 || pRequest->dwDiamondNum >= 1 || pRequest->dwAgateNum >= 1);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);
	
	nPealCount = pPlayerItem->GetItemCountInBags(totPealGenre, totPealDetail, totPealParticular);
	printf("PealCountInBag : %d    PealCountConsumed: %d", nPealCount, (INT)pRequest->dwPealNum);
    LOG_PROCESS_ERROR(nPealCount >= (INT)pRequest->dwPealNum);
	nJadeiteCount = pPlayerItem->GetItemCountInBags(totJadeiteGenre, totJadeiteDetail, totJadeiteParticular);
	LOG_PROCESS_ERROR(nJadeiteCount >= (INT)pRequest->dwJadeiteNum);
	nAgateCount = pPlayerItem->GetItemCountInBags(totAgateGenre, totAgateDetail, totAgateParticular);
	LOG_PROCESS_ERROR(nAgateCount >= (INT)pRequest->dwAgateNum);
	nDiamondCount = pPlayerItem->GetItemCountInBags(totDiamondGenre, totDiamondDetail, totDiamondParticular);
	LOG_PROCESS_ERROR(nDiamondCount >= (INT)pRequest->dwDiamondNum);

	// 扣掉相应道具
	pPlayerItem->ConsumeItemInBags(pRequest->dwPealNum, totPealGenre, totPealDetail, totPealParticular);
	pPlayerItem->ConsumeItemInBags(pRequest->dwJadeiteNum, totJadeiteGenre, totJadeiteDetail, totJadeiteParticular);
	pPlayerItem->ConsumeItemInBags(pRequest->dwAgateNum, totAgateGenre, totAgateDetail, totAgateParticular);
	pPlayerItem->ConsumeItemInBags(pRequest->dwDiamondNum, totDiamondGenre, totDiamondDetail, totDiamondParticular);

	nContributon += pRequest->dwPealNum * m_cContributionValue.nPealExchange;  
	nContributon += pRequest->dwJadeiteNum * m_cContributionValue.nJadeiteExchange;
	nContributon += pRequest->dwAgateNum * m_cContributionValue.nAgateExchange; 
	nContributon += pRequest->dwDiamondNum * m_cContributionValue.nDiamondExchange; // 一钻石兑换200贡献度

	pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);
	pPlayerPurse->AddMoney(EM_MONEY_CONTRIBUTION, nContributon, &cResult);

	sprintf(szMsgTmp, LANG_012, pPlayer->GetName());
	szKinNoticeMsg = szMsgTmp;
	if (pRequest->dwPealNum > 0)
	{
		sprintf(szMsgTmp, LANG_013, pRequest->dwPealNum);
		szKinNoticeMsg = szKinNoticeMsg + szMsgTmp;
	}
	if (pRequest->dwJadeiteNum > 0)
	{
		sprintf(szMsgTmp, LANG_014, pRequest->dwJadeiteNum);
		szKinNoticeMsg = szKinNoticeMsg + szMsgTmp;
	}
	if (pRequest->dwAgateNum > 0)
	{
		sprintf(szMsgTmp, LANG_015, pRequest->dwAgateNum);
		szKinNoticeMsg = szKinNoticeMsg + szMsgTmp;
	}
	if (pRequest->dwDiamondNum > 0)
	{
		sprintf(szMsgTmp, LANG_016, pRequest->dwDiamondNum);
		szKinNoticeMsg = szKinNoticeMsg + szMsgTmp;
	}
	sprintf(szMsgTmp,LANG_017, nContributon);
	szKinNoticeMsg = szKinNoticeMsg + szMsgTmp;

	g_ChatChannelManager.RequestSendKinNoticeMsg((INT)pPlayer->GetId(), szKinNoticeMsg.CStr());

	bRetCode = KinRelayClient::DoSaveMoneyInTongRequest(pPlayer->GetId(), pRequest->dwPealNum, pRequest->dwJadeiteNum, pRequest->dwAgateNum, pRequest->dwDiamondNum, nContributon);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

void QKinModule::OnPayTongSalaryRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	C2S_PAY_TONG_SALARY_REQUEST*    pRequest    = (C2S_PAY_TONG_SALARY_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);
	LOG_PROCESS_ERROR(pRequest->nTotalSalary > 0);

	bRetCode = KinRelayClient::DoPayTongSalaryRequest(pPlayer->GetId(), pRequest->nTotalSalary);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

void QKinModule::OnGetTongSalaryRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	C2S_GET_TONG_SALARY_REQUEST*    pRequest    = (C2S_GET_TONG_SALARY_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);

	bRetCode = KinRelayClient::DoGetTongSalaryRequest(pPlayer->GetId());
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

void QKinModule::OnApplyOpenTongRepertoryRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_APPLY_OPEN_TONG_REPERTORY_REQUEST*  pRequest = (C2S_APPLY_OPEN_TONG_REPERTORY_REQUEST*)pData;
//	KPlayer*                                pPlayer = NULL;
//	KNpc*                                   pNpc    = NULL;
//
//	pPlayer = GetPlayerByConnection(nConnIndex);
//	LOG_PROCESS_ERROR(pPlayer);
//	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);
//	PROCESS_ERROR(pPlayer->m_eMoveState != cmsOnDeath);
//
//	pNpc = g_pWorld->m_NpcSet.GetObj(pRequest->dwNpcID);
//	LOG_PROCESS_ERROR(pNpc);
//	LOG_PROCESS_ERROR(pNpc->m_bTongRepertory);
//
//	pPlayer->m_dwTongRepertoryNpcID = pRequest->dwNpcID;

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);
	KinRelayClient::DoApplyOpenTongRepertoryRequest(pPlayer->GetId());
EXIT0:
	return;
}

void QKinModule::OnTakeTongRepertoryItemToPosRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{               
//	BOOL                                            bRetCode        = false;
//	KPlayer*                                        pPlayer         = NULL;
//	KItem*                                          pDstItem        = NULL;
//	int                                             nBoxSize        = 0;
//	C2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST*    pRequest        = (C2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST*)pData;
//	KItemList*                                      pItemList       = NULL;
//	int                                             nStackNum       = 0;
//	int                                             nMaxStackNum    = 0;
//	int                                             nExistAmount    = 0;
//	int                                             nMaxExistAmount = 0;
//
//	pPlayer = GetPlayerByConnection(nConnIndex);
//	LOG_PROCESS_ERROR(pPlayer);
//	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);
//
//	bRetCode = pPlayer->CanOpenTongRepertory();
//	PROCESS_ERROR(bRetCode);
//
//	LOG_PROCESS_ERROR(pRequest->byBoxIndex >= ibPackage && pRequest->byBoxIndex <= ibPackage4);
//
//	pItemList = &pPlayer->m_ItemList;
//	nBoxSize = pItemList->GetBoxSize(pRequest->byBoxIndex);
//	LOG_PROCESS_ERROR(pRequest->byBoxPos < nBoxSize);
//
//	pDstItem = pPlayer->m_ItemList.GetItem(pRequest->byBoxIndex, pRequest->byBoxPos);
//	if (pDstItem)
//	{
//		bRetCode = pDstItem->IsStackable();
//		PROCESS_ERROR(bRetCode);
//
//		nStackNum       = pDstItem->GetStackNum();
//		nMaxStackNum    = pDstItem->GetMaxStackNum();
//		nExistAmount    = pItemList->GetItemTotalAmount(pDstItem->m_GenParam.dwTabType, pDstItem->m_GenParam.dwIndex);
//		nMaxExistAmount = pDstItem->GetMaxExistAmount();
//
//		PROCESS_ERROR(nStackNum < nMaxStackNum);
//		PROCESS_ERROR(nMaxExistAmount == 0 || nExistAmount < nMaxExistAmount);
//	}
//
//	KinRelayClient::DoTakeTongRepertoryItemToPosRequest(
//		pPlayer->m_dwID, pRequest->byPageIndex, pRequest->byPagePos,
//		pRequest->byBoxIndex, pRequest->byBoxPos, pRequest->nLastUpdateCounter
//		);
EXIT0:
	return;
}

void QKinModule::OnTakeTongRepertoryItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                        bRetCode    = false;
	C2S_TAKE_TONG_REPERTORY_ITEM_REQUEST*       pRequest    = (C2S_TAKE_TONG_REPERTORY_ITEM_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);

	KinRelayClient::DoTakeTongRepertoryItemRequest(
		pPlayer->GetId(), pRequest->byPageIndex, pRequest->byPagePos, 
		pRequest->byBoxIndex, pRequest->byBoxPos, pRequest->nLastUpdateCounter
		);
EXIT0:
	return;
}

void QKinModule::OnPutTongRepertoryItemToPosRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
//	BOOL                                        bRetCode    = false;
//	KPlayer*                                    pPlayer     = NULL;
//	KItem*                                      pItem       = NULL;
//	C2S_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST* pRequest    = (C2S_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST*)pData;
//	KItemList*                                  pItemList   = NULL;
//	int                                         nBoxSize    = 0;
//	int                                         nGenre      = 0;
//
//	pPlayer = GetPlayerByConnection(nConnIndex);
//	LOG_PROCESS_ERROR(pPlayer);
//
//	bRetCode = pPlayer->CanOpenTongRepertory();
//	PROCESS_ERROR(bRetCode);
//
//	LOG_PROCESS_ERROR(pRequest->byBoxIndex >= ibPackage  && pRequest->byBoxIndex <= ibPackage4);
//
//	pItemList = &pPlayer->m_ItemList;
//
//	nBoxSize = pItemList->GetBoxSize(pRequest->byBoxIndex);
//	LOG_PROCESS_ERROR(pRequest->byBoxPos < nBoxSize);
//
//	pItem = pItemList->GetItem(pRequest->byBoxIndex, pRequest->byBoxPos);
//	PROCESS_ERROR(pItem);
//
//	bRetCode = pItem->IsBind();
//	PROCESS_ERROR(!bRetCode);
//
//	nGenre = pItem->GetGenre();
//	PROCESS_ERROR(nGenre != igTaskItem);
//
//	KinRelayClient::DoPutTongRepertoryItemToPosRequest(
//		pPlayer->m_dwID, pItem->m_dwID, pRequest->byBoxIndex ,pRequest->byBoxPos, 
//		pRequest->byPageIndex, pRequest->byPagePos
//		);
EXIT0:
	return;
}

void QKinModule::OnPutTongRepertoryItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                    bRetCode    = false;
	KItem*                                  pItem       = NULL;
	C2S_PUT_TONG_REPERTORY_ITEM_REQUEST*    pRequest    = (C2S_PUT_TONG_REPERTORY_ITEM_REQUEST*)pData;
	KPlayerItem*							pPlayerItem = NULL;
	INT                                     nItemIndex  = 0;
	int                                     nBoxSize    = 0;
	int                                     nGenre      = 0;
	TDBItemData								cItemData;
	KITEM_POS                               cItemPos;
	INT                                     nx          = 0;
	INT                                     ny          = 0;

	//bRetCode = pPlayer->CanOpenTongRepertory();
	//PROCESS_ERROR(bRetCode);

	LOG_PROCESS_ERROR(pRequest->byBoxIndex == emROOM_MAINBAG || pRequest->byBoxIndex == emROOM_EXTBAG);
	LOG_PROCESS_ERROR(pRequest->byBoxPos <= KD_ROOM_MAINBAG_SIZE);
	//LOG_PROCESS_ERROR(pRequest->byBoxIndex >= ibPackage  && pRequest->byBoxIndex <= ibPackage4);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	nx = pRequest->byBoxPos % KD_ROOM_MAINBAG_WIDTH;
	ny = pRequest->byBoxPos / KD_ROOM_MAINBAG_WIDTH;
	cItemPos.eRoom = (KE_ITEM_ROOM)pRequest->byBoxIndex;
	cItemPos.nX = nx;
	cItemPos.nY = ny;
	pItem = KPlayerItem::GetItemFromItemPos(&cItemPos, pPlayer);
	PROCESS_ERROR(pItem);

	nItemIndex = pItem->GetGameIndex();
	PROCESS_ERROR(nItemIndex);

	nGenre = pItem->GetGenre();
	PROCESS_ERROR(nGenre);

	if (pItem->IsBind())
	{
		PROCESS_ERROR(nGenre == item_equip_general);
		PROCESS_ERROR(!pPlayerItem->IsKinFull());

		if (pRequest->byPageIndex != 0)
		{
			BYTE byNotUse = 0;
			DoKinMessageNotify(pPlayer->GetPlayerIndex(), tenPutItemToRepertoryPermissionDenyError, &byNotUse, sizeof(byNotUse));
			goto EXIT0;
		}
	}
	else
	{
		// 非绑定装备必须放在第五页中
		if (pRequest->byPageIndex != 5)
		{
			BYTE byNotUse = 0;
			DoKinMessageNotify(pPlayer->GetPlayerIndex(), tenPutItemToRepertoryPermissionDenyError, &byNotUse, sizeof(byNotUse));
			goto EXIT0;
		}
	}

	memset(&cItemData, 0, sizeof(cItemData));
	pPlayerItem->SaveItemData(pItem, &cItemData, (INT)pRequest->byBoxIndex, nx, ny);

	KinRelayClient::DoPutTongRepertoryItemRequest(
		pPlayer->GetId(), nItemIndex, pRequest->byBoxIndex, pRequest->byBoxPos, 
		pRequest->byPageIndex, pRequest->byPagePos, sizeof(cItemData), (BYTE*)&cItemData
		);

	// nBoxSize = pItemList->GetBoxSize(pRequest->byBoxIndex);
	// LOG_PROCESS_ERROR(pRequest->byBoxPos < nBoxSize);

	// pItem = pItemList->GetItem(pRequest->byBoxIndex, pRequest->byBoxPos);
	// PROCESS_ERROR(pItem);

	//bRetCode = pItem->IsBind();
	//PROCESS_ERROR(!bRetCode);

	//nGenre = pItem->GetGenre();
	//PROCESS_ERROR(nGenre != igTaskItem);

	//KinRelayClient::DoPutTongRepertoryItemRequest(
	//	pPlayer->m_dwID, pItem->m_dwID, pRequest->byBoxIndex ,pRequest->byBoxPos, pRequest->byPageIndex
	//	);
EXIT0:
	return;
}

void QKinModule::OnExchangeTongRepertoryItemPosRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                                            bRetCode    = false;
	C2S_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST*   pRequest = (C2S_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer);

// 	bRetCode = pPlayer->CanOpenTongRepertory();
// 	PROCESS_ERROR(bRetCode);

	LOG_PROCESS_ERROR(
		pRequest->bySrcPageIndex != pRequest->byDstPageIndex ||
		pRequest->bySrcPagePos != pRequest->byDstPagePos
		);

	KinRelayClient::DoExchangeTongRepertoryItemPosRequest(
		pPlayer->GetId(), pRequest->bySrcPageIndex, pRequest->bySrcPagePos, pRequest->nSrcPageLastUpdateCounter,
		pRequest->byDstPageIndex, pRequest->byDstPagePos, pRequest->nDstPageLastUpdateCounter
		);
EXIT0:
	return;
}

void QKinModule::OnStackTongRepertoryItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
//	BOOL                                    bRetCode    = false;
//	KPlayer*                                pPlayer     = NULL;
//	KItem*                                  pItem       = NULL;
//	KItemList*                              pItemList   = NULL;
//	int                                     nBoxSize    = 0;
//	BYTE                                    byTabType   = 0;
//	WORD                                    wTabIndex   = 0;
//	int                                     nGenre      = 0;
//	C2S_STACK_TONG_REPERTORY_ITEM_REQUEST*  pRequest    = (C2S_STACK_TONG_REPERTORY_ITEM_REQUEST*)pData;
//
//	pPlayer = GetPlayerByConnection(nConnIndex);
//	LOG_PROCESS_ERROR(pPlayer);
//
//	bRetCode = pPlayer->CanOpenTongRepertory();
//	PROCESS_ERROR(bRetCode);
//
//	LOG_PROCESS_ERROR(pRequest->byBoxIndex >= ibPackage  && pRequest->byBoxIndex <= ibPackage4);
//
//	pItemList = &pPlayer->m_ItemList;
//	assert(pItemList);
//
//	nBoxSize = pItemList->GetBoxSize(pRequest->byBoxIndex);
//	LOG_PROCESS_ERROR(pRequest->byBoxPos < nBoxSize);
//
//	pItem = pItemList->GetItem(pRequest->byBoxIndex, pRequest->byBoxPos);
//	PROCESS_ERROR(pItem);
//
//	bRetCode = pItem->IsStackable();
//	LOG_PROCESS_ERROR(bRetCode);
//
//	bRetCode = pItem->IsBind();
//	PROCESS_ERROR(!bRetCode);
//
//	nGenre = pItem->GetGenre();
//	PROCESS_ERROR(nGenre != igTaskItem);
//
//	LOG_PROCESS_ERROR(pRequest->wStackNum <= pItem->m_nStackNum);
//	byTabType = (BYTE)pItem->m_GenParam.dwTabType;
//	wTabIndex = (WORD)pItem->m_GenParam.dwIndex;
//
//	bRetCode = KinRelayClient::DoApplyStackTongRepertoryItemRequest(
//		pPlayer->m_dwID, pPlayer->m_dwKinId, 
//		pRequest->byPageIndex, pRequest->byPagePos,
//		pRequest->byBoxIndex, pRequest->byBoxPos, pItem->m_dwID,
//		byTabType, wTabIndex, pRequest->wStackNum, (WORD)pItem->m_nMaxStackNum,
//		pRequest->nLastUpdateCounter
//		);
EXIT0:
	return;
}

void QKinModule::OnRemoveTongRepertoryItemRequest( KPlayer* pPlayer, char* pData, size_t uDataLen )
{
	BOOL                                        bRetCode    = false;
	C2S_TAKE_TONG_REPERTORY_ITEM_REQUEST*       pRequest    = (C2S_TAKE_TONG_REPERTORY_ITEM_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer->m_dwKinId);

	KinRelayClient::DoRemoveTongRepertoryItemRequest(
		pPlayer->GetId(), pRequest->byPageIndex, pRequest->byPagePos, pRequest->nLastUpdateCounter
		);
EXIT0:
	return;
}

void QKinModule::OnGetBackItemFromRepertoryRequest( KPlayer* pPlayer, char* pData, size_t uDataLen )
{
	BOOL                                           bRetCode    = false;
	C2S_GET_BACK_ITEM_FROM_REPERTORY_REQUEST*      pRequest    = (C2S_GET_BACK_ITEM_FROM_REPERTORY_REQUEST*)pData;
	KPlayerItem*                                   pPlayerItem = NULL;
	KItem*                                         pItem       = NULL;
	INT                                            nX          = 0;
	INT                                            nY          = 0;
	KITEM_POS                                      cItemPos;
	DWORD                                          dwKinId     = KD_BAD_ID;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pRequest->dwBoxPos < KD_ROOM_KIN_SIZE);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	memset(&cItemPos, 0, sizeof(cItemPos));
	cItemPos.eRoom = emROOM_KIN;
	cItemPos.nX = pRequest->dwBoxPos % KD_ROOM_KIN_WIDTH;
	cItemPos.nY = pRequest->dwBoxPos / KD_ROOM_KIN_WIDTH;
	pItem = KPlayerItem::GetItemFromItemPos(&cItemPos, pPlayer);
	LOG_PROCESS_ERROR(pItem);

	dwKinId = pItem->GetKinReptoryID();
	LOG_PROCESS_ERROR(dwKinId != KD_BAD_ID);

	// LOG_PROCESS_ERROR(pPlayer->m_dwKinId);
	// LOG_PROCESS_ERROR(pRequest->dwTongID != KD_BAD_ID);
	KinRelayClient::DoGetBackItemFromRepertoryRequest(
		pPlayer->GetId(), dwKinId, (BYTE)pRequest->dwTongID, (BYTE)pRequest->dwBoxPos, 
		pItem->GetGuid(), pRequest->nLastUpdateCounter);
EXIT0:
	return;
}

void QKinModule::OnGetFreeJoinTongListRequest( KPlayer* pPlayer, char* pData, size_t uDataLen )
{
	C2S_GET_FREEJOIN_TONG_LIST_REQUEST*      pRequest    = (C2S_GET_FREEJOIN_TONG_LIST_REQUEST*)pData;
	
	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pRequest);

	KinRelayClient::DoGetFreeJoinTongListRequest(pPlayer->GetId(), pRequest->nLastUpdateCounter);

EXIT0:
	return;
}

void QKinModule::OnApplyJoinTongRequest( KPlayer* pPlayer, char* pData, size_t uDataLen )
{
	time_t tNow = KSysService::Time(NULL);
	time_t tQuitTongTime = (time_t)pPlayer->GetLastQuitTongTime();
	C2S_APPLY_JOIN_TONG_REQUEST*		pRequest = (C2S_APPLY_JOIN_TONG_REQUEST*)pData;

	tm* pTimeNow = ::localtime(&tNow);
	INT nYearNow = pTimeNow->tm_year;
	INT nYDayNow = pTimeNow->tm_yday;

	tm* pTimeQuitTong = ::localtime(&tQuitTongTime);
	INT nYearLastQuit = pTimeQuitTong->tm_year;
	INT nYDayLastQuit = pTimeQuitTong->tm_yday;

	if (nYearNow == nYearLastQuit && nYDayNow == nYDayLastQuit)
	{
        g_cPlayerBaseProcess.DoSyncBlackMsg(pPlayer,LANG_018);
		return;
	}

	KinRelayClient::DoApplyJoinTongRequest(pPlayer->GetId(), pRequest->dwTongID);

EXIT0:
	return;
}

void QKinModule::OnSetTongFreeJoin( KPlayer* pPlayer, char* pData, size_t uDataLen )
{
	C2S_SET_TONG_FREEJOIN_REQUEST*		pRequest = (C2S_SET_TONG_FREEJOIN_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pRequest);

	KinRelayClient::DoSetTongFreeJoinRequest(pPlayer->GetId(), pRequest->dwTongID, pRequest->bFreeJoin);

EXIT0:
	return;
}

void QKinModule::OnSetKinActivityOpenTime(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	C2S_SET_KIN_ACTIVITY_OPENTIME* pRequest = (C2S_SET_KIN_ACTIVITY_OPENTIME*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pRequest);

	KinRelayClient::DoSetTongActivityOpenTimeRequest(pPlayer->GetId(), pRequest->dwKinID, pRequest->dwKinActivityIndex, pRequest->dwKinActivityOpenTime);

EXIT0:
	return;
}

BOOL QKinModule::DoSyncTongMemberInfo(DWORD dwPlayerID, const KTongMemberInfo& crTongMember)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	KPlayer*                    pPlayer         = NULL;
	S2C_SYNC_TONG_MEMBER_INFO   SyncInfo;

	pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
	PROCESS_ERROR(pPlayer);

	SyncInfo.byProtocol   = s2c_sync_tong_member_info;
	SyncInfo.TongMemberInfo = crTongMember;

	bRetCode = SendData(pPlayer->GetPlayerIndex(), SyncInfo);
	PROCESS_ERROR(bRetCode);

	bResult = true;

EXIT0:
	return bResult;
}

BOOL QKinModule::DoTongDeleteMemberNotify(DWORD dwPlayerID, DWORD dwMemberID)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	KPlayer*                        pPlayer     = NULL;
	S2C_DELETE_TONG_MEMBER_NOTIFY	Package;

	pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
	PROCESS_ERROR(pPlayer);

	Package.byProtocol  = s2c_delete_tong_member_notify;
	Package.dwMemberID   = dwMemberID;

	bRetCode = SendData(pPlayer->GetPlayerIndex(), Package);
	PROCESS_ERROR(bRetCode);

	bResult =  true;

EXIT0:
	return bResult;
}

BOOL QKinModule::DoApplyTongInfoRespond(DWORD dwPlayerID, BYTE byRespondType, BYTE* pbyRespondData, size_t uDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	KPlayer*                        pPlayer     = NULL;
	S2C_APPLY_TONG_INFO_RESPOND*    pRespond	= (S2C_APPLY_TONG_INFO_RESPOND*)m_byTempData;
	size_t                          uPakSize    = sizeof(S2C_APPLY_TONG_INFO_RESPOND) + uDataLen;

	// LOG_PROCESS_ERROR(uPakSize <= MAX_EXTERNAL_PACKAGE_SIZE);

	pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
	PROCESS_ERROR(pPlayer);

	pRespond->byProtocol  = s2c_apply_tong_info_respond;
	pRespond->byRespondType = byRespondType;
	// pRespond->wSize         = (WORD)uPakSize;
	memcpy(pRespond->byData, pbyRespondData, uDataLen);

	bRetCode = SendLenData(pPlayer->GetPlayerIndex(), *pRespond, uPakSize);
	PROCESS_ERROR(bRetCode);

	bResult = true;

EXIT0:
	return bResult;
}

BOOL QKinModule::DoUpdateTongClientDataVersion(DWORD dwPlayerID, KTONG_UPDATE_FLAG eFrameType, int nUpdateFrame)
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	KPlayer*                            pPlayer     = NULL;
	S2C_UPDATE_TONG_CLIENT_DATA_VERSION Package;

	pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
	PROCESS_ERROR(pPlayer);

	Package.byProtocol    = s2c_update_tong_client_data_version;
	Package.byFrameType   = (BYTE)eFrameType;
	Package.nUpdateCount  = nUpdateFrame;

	bRetCode = SendData(pPlayer->GetPlayerIndex(), Package);
	PROCESS_ERROR(bRetCode);

	bResult = true;

EXIT0:
	return bResult;
}

BOOL QKinModule::DoSyncTongRepertoryPageRespond(
	DWORD dwPlayerID, BYTE byPageIndex, int nServerCounter, BYTE byGetRight, BYTE byPutRight,
	BYTE byUpdateItemCount, BYTE byInvailedItemCount, size_t uDataLen, BYTE byData[]
)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	KPlayer*                                pPlayer     = NULL;
	S2C_SYNC_TONG_REPERTORY_PAGE_RESPOND*   pRespond    = (S2C_SYNC_TONG_REPERTORY_PAGE_RESPOND*)m_byTempData;
	UINT									nSize		= 0;

	LOG_PROCESS_ERROR(sizeof(S2C_SYNC_TONG_REPERTORY_PAGE_RESPOND) + uDataLen < sizeof(m_byTempData));

	pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
	PROCESS_ERROR(pPlayer);

	pRespond->byProtocol          = s2c_sync_tong_repertory_page_respond;
	pRespond->byPageIndex           = byPageIndex;
	pRespond->nServerCounter        = nServerCounter;
	pRespond->byGetRight            = byGetRight;
	pRespond->byPutRight            = byPutRight;
	pRespond->byUpdateItemCount     = byUpdateItemCount;
	pRespond->byInvailedItemCount   = byInvailedItemCount;
	memcpy(pRespond->byData, byData, uDataLen);

	nSize = (WORD)(sizeof(S2C_SYNC_TONG_REPERTORY_PAGE_RESPOND) + uDataLen);

	bRetCode = SendLenData(pPlayer->GetPlayerIndex(), *pRespond, nSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QKinModule::DoInvitePlayerJoinTongRequest(
	DWORD dwPlayerID, DWORD dwInviterID, DWORD dwTongID, BYTE byTongCamp, char szInviterName[], char szTongName[]
)
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	KPlayer*                            pPlayer     = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	S2C_INVITE_PLAYER_JOIN_TONG_REQUEST Package;

	assert(szInviterName);
	assert(szTongName);

	pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
	PROCESS_ERROR(pPlayer);

	//if (pPlayer->GetKinCamp() != byTongCamp) // 家族阵营不对
	//{
	//	KinRelayClient::DoInvitePlayerJoinTongRespond(
	//		pPlayer->GetId(), dwInviterID, dwTongID, itrCampLimit
	//		);
    //    goto EXIT0;
	//}

	Package.byProtocol    = s2c_invite_player_join_tong_request;
	Package.dwInviterID     = dwInviterID;
	Package.dwTongID        = dwTongID;

	strncpy(Package.szInviterName, szInviterName, sizeof(Package.szInviterName));
	Package.szInviterName[sizeof(Package.szInviterName) - 1] = '\0';

	strncpy(Package.szTongName, szTongName, sizeof(Package.szTongName));
	Package.szTongName[sizeof(Package.szTongName) - 1] = '\0';

	bRetCode = SendData(pPlayer->GetPlayerIndex(), Package);
	PROCESS_ERROR(bRetCode);

	bResult = true;

EXIT0:
	return bResult;
}

BOOL QKinModule::DoSyncTongActivityOpenTimes(int nConnIndex, BYTE* pbyOpenTimesData, INT nCount)
{
    INT nDataLen = sizeof(S2C_SYNC_ACTIVITY_OPENTIMS) + sizeof(SyncKinActivityInfo) * nCount;
	IMemBlock* piPackage = QCreateMemBlock(nDataLen);
	S2C_SYNC_ACTIVITY_OPENTIMS* pSyncOpenTims = (S2C_SYNC_ACTIVITY_OPENTIMS*)piPackage->GetData();
	pSyncOpenTims->byProtocol = s2c_sync_activity_opentimes;
	pSyncOpenTims->nCount = nCount;
	memcpy(pSyncOpenTims->byData, pbyOpenTimesData, sizeof(SyncKinActivityInfo) * nCount);

	BOOL bRet = SendLenData(nConnIndex, *pSyncOpenTims, nDataLen);
	SAFE_RELEASE(piPackage);

	return bRet;
}

BOOL QKinModule::DoSyncTongApplyJoinList(int nConnIndex, BYTE* pbyApplyerData, DWORD dwCount)
{
	INT nDataLen = sizeof(S2C_SYNC_APPLY_JOIN_TONG_LIST) + sizeof(QTongApplyerInfo) * dwCount;
	IMemBlock* piPackage = QCreateMemBlock(nDataLen);
	S2C_SYNC_APPLY_JOIN_TONG_LIST* pSyncApplyJoinTongList = (S2C_SYNC_APPLY_JOIN_TONG_LIST*)piPackage->GetData();
	pSyncApplyJoinTongList->byProtocol = s2c_sync_apply_join_tong_list;
	pSyncApplyJoinTongList->dwApplyerCount = dwCount;
	memcpy(pSyncApplyJoinTongList->byData, pbyApplyerData, sizeof(QTongApplyerInfo) * dwCount);

	BOOL bRet = SendLenData(nConnIndex, *pSyncApplyJoinTongList, nDataLen);
	SAFE_RELEASE(piPackage);

	return bRet;
}

BOOL QKinModule::DoChangePlayerTongNotify(int nConnIndex, DWORD dwTongID, BYTE byReason, char szTongName[])
{
	BOOL                            bResult  = false;
	BOOL                            bRetCode = false;
	S2C_CHANGE_PLAYER_TONG_NOTIFY   Notify;

	Notify.byProtocol = s2c_change_player_tong_notify;
	Notify.dwTongID     = dwTongID;
	Notify.byReason     = byReason;
	strncpy(Notify.szTongName, szTongName, sizeof(Notify.szTongName));
	Notify.szTongName[sizeof(Notify.szTongName) - 1] = '\0';

	bRetCode = SendData(nConnIndex, Notify);
	PROCESS_ERROR(bRetCode);

	bResult = true;

EXIT0:
	return bResult;
}

BOOL QKinModule::DoBroadcastTongMessage(int nConnIndex, BYTE byCode, BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bResult     = false;
	BOOL                        bRetCode    = false;
	S2C_TONG_BROADCAST_MESSAGE* pMessage    = (S2C_TONG_BROADCAST_MESSAGE*)m_byTempData;
	size_t                      uPackSize   = sizeof(S2C_TONG_BROADCAST_MESSAGE) + uDataLen;

	// LOG_PROCESS_ERROR(MAX_EXTERNAL_PACKAGE_SIZE >= uPackSize);

	pMessage->byProtocol  = s2c_tong_broadcast_message;
	// pMessage->wSize         = (WORD)uPackSize;
	pMessage->byCode        = byCode;
	memcpy(pMessage->byData, pbyData, uDataLen);

	bRetCode = SendLenData(nConnIndex, *pMessage, uPackSize);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QKinModule::UpdatePlayerKinInfo(DWORD dwPlayerId, CONST PLAYER_KIN_INFO& info)
{
	PLAYER_KIN_INFO& infoSet = m_mapPlayerKinInfos[dwPlayerId];
	infoSet = info;
	return TRUE;
}

BOOL QKinModule::UpdateKinLevel(DWORD dwKinId, INT nLevel)
{
	std::map<DWORD, PLAYER_KIN_INFO>::iterator it;
	for (it = m_mapPlayerKinInfos.begin(); it != m_mapPlayerKinInfos.end(); ++it)
	{
		if (it->second.dwKinId == dwKinId)
		{
			it->second.nKinLevel = nLevel;
		}
	}

	return TRUE;
}

BOOL QKinModule::DoGetTongDescriptionRespond( int nConnIndex, DWORD dwTongID, char szTongName[], DWORD dwMaster )
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	S2C_GET_TONG_DESCRIPTION_RESPOND    Respond;

	Respond.byProtocol = s2c_get_tong_description_respond;
	Respond.dwTongID = dwTongID;
	strncpy(Respond.szTongName, szTongName, sizeof(Respond.szTongName));
	Respond.szTongName[sizeof(Respond.szTongName) -  1] = '\0';
	Respond.dwMastarID = dwMaster;

	bRetCode = SendData(nConnIndex, Respond);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

// todo
BOOL QKinModule::DoBroadcastPlayerChangeTong(KPlayer* pPlayer)
{
	BOOL                                bResult = false;
	IKScene*                             pScene  = NULL;
//	KBroadcastFunc                      BroadcastFunc;
	S2C_BROADCAST_PLAYER_CHANGE_TONG    Pak;
	KinBroadcastHelper bcHelper(Pak);

	assert(pPlayer);

	Pak.byProtocol    = s2c_broadcast_player_change_tong;
	Pak.dwPlayerID      = pPlayer->GetId();
	Pak.dwTongID        = pPlayer->m_dwKinId;

//	BroadcastFunc.m_pvData = &Pak;
//	BroadcastFunc.m_uSize = sizeof(Pak);

	pScene = pPlayer->GetScene();
	PROCESS_ERROR(pScene);

	pScene->TraverseNearbyPlayers(bcHelper, pPlayer);
	// pScene->Broadcast(pPlayer->m_pRegion, BroadcastFunc);

	bResult = true;

EXIT0:    
	return bResult;
}

BOOL QKinModule::DoSyncTongOnlineMessage(int nConnIndex, char szOnlineMessage[], size_t uDataLen)
{
	BOOL                            bResult  = false;
	BOOL                            bRetCode = false;
	S2C_SYNC_TONG_ONLINE_MESSAGE*   pSync = (S2C_SYNC_TONG_ONLINE_MESSAGE*)m_byTempData;

	// LOG_PROCESS_ERROR(MAX_EXTERNAL_PACKAGE_SIZE >= sizeof(S2C_SYNC_TONG_ONLINE_MESSAGE) + uDataLen);

	pSync->byProtocol = s2c_sync_tong_online_message;
	// pSync->wSize        = (WORD)(sizeof(S2C_SYNC_TONG_ONLINE_MESSAGE) + uDataLen);
	strncpy(pSync->szOnlineMessage, szOnlineMessage, uDataLen);
	assert(uDataLen > 0);
	pSync->szOnlineMessage[uDataLen - 1] = '\0';

	bRetCode = SendLenData(nConnIndex, *pSync, sizeof(S2C_SYNC_TONG_ONLINE_MESSAGE) + uDataLen);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QKinModule::DoApplyOpenTongRepertoryRespond(int nConnIndex, DWORD dwPlayerID, const char szTongName[], BYTE byRepertoryPageNum)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	S2C_APPLY_OPEN_TONG_REPERTORY_RESPOND*  pRespond    = (S2C_APPLY_OPEN_TONG_REPERTORY_RESPOND*)m_byTempData;

	assert(szTongName);
//	assert(sizeof(S2C_APPLY_OPEN_TONG_REPERTORY_RESPOND) < MAX_EXTERNAL_PACKAGE_SIZE);

	pRespond->byProtocol          = s2c_apply_open_tong_repertory_respond;
	pRespond->byRepertoryPageNum    = byRepertoryPageNum;

	strncpy(pRespond->szTongName, szTongName, sizeof(pRespond->szTongName));
	pRespond->szTongName[sizeof(pRespond->szTongName) - 1] = '\0';

	bRetCode = SendData(nConnIndex, *pRespond);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QKinModule::DoSyncTongHistoryRespond(int nConnIndex, BYTE byType, DWORD dwStartIndex, int nCount, BYTE byData[], size_t uDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	S2C_SYNC_TONG_HISTORY_RESPOND*  pRespond    = (S2C_SYNC_TONG_HISTORY_RESPOND*)m_byTempData;
	UINT							nSize		= 0;

	assert(byData);
	PROCESS_ERROR(nCount > 0);
	PROCESS_ERROR(uDataLen > 0);

	pRespond->byProtocol  = s2c_sync_tong_history_respond;
	pRespond->byType        = byType;
	pRespond->dwStartIndex  = dwStartIndex;
	pRespond->byCount       = (BYTE)nCount;
	memcpy(pRespond->byData, byData, uDataLen);

	nSize = (WORD)(sizeof(S2C_SYNC_TONG_HISTORY_RESPOND) + uDataLen);

	bRetCode = SendLenData(nConnIndex, *pRespond, nSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QKinModule::DoKinMessageNotify(int nConnIndex, BYTE byCode, BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bResult     = false;
	BOOL                        bRetCode    = false;
	S2C_KIN_MESSAGE_NOTIFY*		pMessage    = (S2C_KIN_MESSAGE_NOTIFY*)m_byTempData;
	size_t                      uPackSize   = sizeof(S2C_KIN_MESSAGE_NOTIFY) + uDataLen;

	// LOG_PROCESS_ERROR(MAX_EXTERNAL_PACKAGE_SIZE >= uPackSize);

	pMessage->byProtocol  = s2c_kin_message_notify;
	// pMessage->wSize         = (WORD)uPackSize;
	pMessage->byCode        = byCode;
	memcpy(pMessage->byData, pbyData, uDataLen);

	bRetCode = SendLenData(nConnIndex, *pMessage, uPackSize);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QKinModule::DoGetFreeJoinTongListRespond( int nConnIndex, BYTE byTongNum, int nUpdateCounter, BYTE* pbyData, size_t uDataLen )
{
	BOOL								bRetCode	= FALSE;
	S2C_GET_FREEJOIN_TONG_LIST_RESPOND*	pRespond	= (S2C_GET_FREEJOIN_TONG_LIST_RESPOND*)m_byTempData;
	size_t								uSize		= sizeof(S2C_GET_FREEJOIN_TONG_LIST_RESPOND) + uDataLen;

	pRespond->byProtocol		= s2c_get_freejoin_tong_list_respond;
	pRespond->byTongNum			= byTongNum;
	pRespond->nUpdateCounter	= nUpdateCounter;
	memcpy(pRespond->byData, pbyData, uDataLen);

	bRetCode = SendLenData(nConnIndex, *pRespond, uSize);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}


BOOL QKinModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return g_KinModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL QKinModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_KinModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL QKinModule::SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize )
{
	return g_KinModule.GetProtocolProcessor()->SendUnknownData(nPlayerIndex, pData, uDataSize);
}

