#pragma once

#include "serverbase/KinDef.h"

class KItem;

class KinRelayClient
{
public:
	KinRelayClient(void);
	~KinRelayClient(void);


	static inline BOOL Send(IMemBlock* piBuffer);

	static BOOL DoApplyTongRosterRequest(DWORD dwPlayerID, int nLastUpdateFrame, DWORD dwMemberList[], int nMemberCount);
	static BOOL DoApplyTongInfoRequest(DWORD dwPlayerID, int nLastUpdateFrame);
	static BOOL DoApplyCreateTongRequest(DWORD dwPlayerID, const char cszTongName[], int nTemplateIndex);
	static BOOL DoInvitePlayerJoinTongRequest(DWORD dwPlayerID, const char cszNewMemberName[]);
	static BOOL DoInvitePlayerJoinTongRespond(DWORD dwPlayerID, DWORD dwInviterID, DWORD dwTongID, INVITE_TONG_RESULT_CODE eAccept);
	static BOOL DoApplyKickOutTongMemberRequest(DWORD dwPlayerID, DWORD dwMemberID);
	static BOOL DoModifyTongInfoRequest(DWORD dwPlayerID, BYTE byInfoType, BYTE* pbyData, size_t uDataLen);
	static BOOL DoModifyTongSchemaRequest(DWORD dwPlayerID, BYTE byGroupIndex, BYTE byModifyType, BYTE* pbyData, size_t uDataLen);
	static BOOL DoApplyQuitTongRequest(DWORD dwPlayerID);

	static BOOL DoChangeTongMemberGroupRequest(DWORD dwPlayerID, DWORD dwTargetMemberID, int nTargetGroup);
	static BOOL DoChangeTongMasterRequest(DWORD dwPlayerID, DWORD dwNewMasterID);
	static BOOL DoChangeTongMemberRemarkRequest(DWORD dwPlayerID, DWORD dwTargetMemberID, char szRemark[]);
	static BOOL DoGetTongDescriptionRequest(DWORD dwPlayerID, DWORD dwTongID);
	static BOOL DoSaveMoneyInTongRequest(DWORD dwPlayerID, DWORD dwPealNum, DWORD dwJadeiteNum, DWORD dwAgateNum, DWORD dwDiamondNum, DWORD dwContributionAdd);
	static BOOL DoPayTongSalaryRequest(DWORD dwPlayerID, int nTotalSalary);
	static BOOL DoGetTongSalaryRequest(DWORD dwPlayerID);
	static BOOL DoGetTongRankInfoRequest(DWORD dwTongId, INT nRankId);
	static BOOL DoGetTongSalaryFailRespond(DWORD dwPlayerID, DWORD dwTongID, int nSilver);
	static BOOL DoGetFreeJoinTongListRequest(DWORD dwPlayerID, INT nLastUpdateCounter);
	static BOOL DoApplyJoinTongRequest(DWORD dwPlayerID, DWORD dwTongID);
	static BOOL DoSetTongFreeJoinRequest(DWORD dwPlayerID, DWORD dwTongID, BOOL bValue);
	static BOOL DoSetTongActivityOpenTimeRequest(DWORD dwPlayerID, DWORD dwTongID, DWORD dwTongActivityIndex, DWORD dwTongActivityOpenTime);


	//--------------------------°ï»á²Ö¿â--------------------------------
	static BOOL DoApplyTongRepertoryPageRequest(DWORD dwPlayerID, BYTE byPageIndex, int nLastUpdateCounter);
	static BOOL DoApplyOpenTongRepertoryRequest(DWORD dwPlayerID);

	static BOOL DoTakeTongRepertoryItemToPosRequest(
		DWORD dwPlayerID, BYTE byPageIndex, BYTE byRepertoryPos,
		BYTE byBoxIndex, BYTE byBoxPos, int nLastUpdateCounter
		);
	static BOOL DoTakeTongRepertoryItemRequest(DWORD dwPlayerID, BYTE byPageIndex, BYTE byRepertoryPos, BYTE byBoxIndex, BYTE byBoxPos, int nLastUpdateCounter);
	static BOOL DoTakeTongRepertoryItemRespond(DWORD dwPlayerID, DWORD dwTongID, BYTE byPageIndex, BYTE byRepertoryPos, BOOL bSuccess);

	static BOOL DoPutTongRepertoryItemToPosRequest(
		DWORD dwPlayerID, DWORD dwItemID, BYTE byBoxIndex, BYTE byBoxPos, BYTE byPageIndex, BYTE byPos
		);
	static BOOL DoPutTongRepertoryItemRequest(DWORD dwPlayerID, DWORD dwItemID, BYTE byBoxIndex, BYTE byBoxPos, BYTE byPageIndex, BYTE byPagePos, INT nItemDataLen, BYTE* pItemData);
	static BOOL DoPutTongRepertoryItemRespond(DWORD dwPlayerID, BYTE byPageIndex, BYTE byPagePos, BOOL bFindItem, BYTE* pbyItemData, BYTE byItemDataLen);
	static BOOL DoUnlockTongRepertoryGrid(DWORD dwTongID, BYTE byPageIndex, BYTE byPagePos);
	static BOOL DoExchangeTongRepertoryItemPosRequest(
		DWORD dwPlayerID, BYTE bySrcPageIndex, BYTE bySrcPagePos, int nSrcUpdateCounter,
		BYTE byDstPageIndex, BYTE byDstPagePos, int nDstUpdateCounter
		);

	static BOOL DoApplyStackTongRepertoryItemRequest(
		DWORD dwPlayerID, DWORD dwTongID, 
		BYTE byPageIndex, BYTE byPagePos, BYTE byBoxIndex, BYTE byBoxPos, DWORD dwItemID,
		BYTE byTabType, WORD wTabIndex, WORD wStackNum, WORD wMaxStackNum,
		int nLastUpdateCounter
		);

	static BOOL DoStackTongRepertoryItemRequest(
		DWORD dwPlayerID, DWORD dwTongID,
		BYTE byPageIndex, BYTE byPagePos, BYTE byTabType, WORD wTabIndex,
		WORD wStackNum, WORD WMaxStackNum
		);
	static BOOL DoRemoveTongRepertoryItemRequest(
		DWORD dwPlayerID, BYTE byPageIndex, BYTE byRepertoryPos, int nLastUpdateCounter
		);
	static BOOL DoGetBackItemFromRepertoryRequest(
		DWORD dwPlayerID, DWORD dwTongID, BYTE byBoxIndex, BYTE byBoxPos, const GUID& sGuid, INT nLastUpdateCounter
		);



	static void OnSyncTongMemberInfo(BYTE* pbyData, size_t uDataLen);
	static void OnDeleteTongMemberNotify(BYTE* pbyData, size_t uDataLen);
	static void OnApplyTongInfoRespond(BYTE* pbyData, size_t uDataLen);
	static void OnUpdateTongClientDataVersion(BYTE* pbyData, size_t uDataLen);

	static void OnSyncTongRepertoryPageRespond(BYTE* pbyData, size_t uDataLen);
	static void OnSyncTongApplyJoinList(BYTE* pbyData, size_t uDataLen);

	static void OnSyncTongActivityOpenTimes(BYTE* pbyData, size_t uDataLen);
	static void OnInvitePlayerJoinTongRequest(BYTE* pbyData, size_t uDataLen);
	static void OnChangePlayerTongNotify(BYTE* pbyData, size_t uDataLen);
	static void OnBroadcastTongMessage(BYTE* pbyData, size_t uDataLen);
	static void OnGetTongDescriptionRespond(BYTE* pbyData, size_t uDataLen);
	static void OnSyncTongOnlineMessage(BYTE* pbyData, size_t uDataLen);
	static void OnTakeTongRepertoryItemToPosRespond(BYTE* pbyData, size_t uDataLen);
	static void OnTakeTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen);
	static void OnPutTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen);
	static void OnApplyStackTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen);
	static void OnApplyOpenTongRepertoryRespond(BYTE* pbyData, size_t uDataLen);
	static void OnGetTongSalaryRespond(BYTE* pbyData, size_t uDataLen);
	static void OnSyncTongHistoryRespond(BYTE* pbyData, size_t uDataLen);
	static void OnKinMessageNotify(BYTE* pbyData, size_t uDataLen);
	static void OnGetBackItemFromRepertoryRespond(BYTE* pbyData, size_t uDataLen);
	static void OnGetFreeJoinTongListRespond(BYTE* pbyData, size_t uDataLen);
	static void OnGetTongRankInfoRespond(BYTE* pbyData, size_t uDataLen);
};
