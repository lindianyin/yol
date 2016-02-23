
#pragma once

#include "KinDef.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"

struct KTongMember;
class KTong;

class KinHandler
{
public:

	static BOOL DoSyncTongMemberInfo(int nConnIndex, int dwPlayerID, DWORD dwMemberID, const KTongMember& crMember);
	static BOOL DoTongDeleteMemberNotify(int nConnIndex, DWORD dwPlayerID, DWORD dwMemberID);

	static BOOL DoSyncTongRepertoryPageRespond(int nConnIndex, int dwPlayerID, int nPageIndex, int nLastUpdateCounter);
	static BOOL DoSyncTongApplyJoinList(DWORD dwTongId);
	static BOOL DoSyncTongActivityOpenTimes(int nConnIndex, DWORD dwPlayerId, DWORD dwTongId);

	static BOOL DoUpdateTongBaseInfo(int nConnIndex, DWORD dwPlayerID, KTong* pTong);
	static BOOL DoUpdateTongDevelopmentInfo(int nConnIndex, DWORD dwPlayerID, KTong* pTong);
	static BOOL DoUpdateTongTextInfo(int nConnIndex, DWORD dwPlayerID, int nDataType,  const char cszText[]);
	static BOOL DoUpdateTongSchemaInfo(int nConnIndex, DWORD dwPlayerID, KTongSchemaData* pData);
	static BOOL DoUpdateTongMemorabilia(int nConnIndex, DWORD dwPlayerID, KTong* pTong);

	static BOOL DoUpdateTongClientDataVersion(int nConnIndex, DWORD dwPlayerID, KTONG_UPDATE_FLAG eFrameType, int nUpdateFrame);

	static BOOL DoInvitePlayerJoinTongRequest(int nConnIndex, DWORD dwPlayerID, DWORD dwInviter, DWORD dwTongID, BYTE byTongCamp, char szInviterName[], char szTongName[]);
	static BOOL DoChangePlayerTongNotify(int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, KCHANGE_TONG_REASON eReason, char szTongName[]);
	static BOOL DoTongBroadcastMessage(KTong* pTong, int nCode, DWORD dwExceptID, BYTE* pbyData = NULL, size_t uDataLen = 0);
	static BOOL DoGetTongDescriptionRepond(int nConnIndex, DWORD dwPlayerID, KTong* pTong);

	static BOOL DoSyncTongOnlineMessage(int nConnIndex, DWORD dwPlayerID);
	static BOOL DoTakeTongRepertoryItemToPosRespond(
		int nConnIndex, DWORD dwPlayerID, DWORD dwTongID,
		int nPageIndex, int nRepertoryPos, int nBoxIndex, int nBoxPos, 
		size_t uSize, BYTE* pbyData
		);
	static BOOL DoTakeTongRepertoryItemRespond(
		int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, DWORD dwPutPlayerID, time_t tRentPeriod,
		int nBoxIndex, int nBoxPos, int nPageIndex, int nRepertoryPos, 
		size_t uSize, BYTE* pbyData
		);

	static BOOL DoPutTongRepertoryItemRespond(
		DWORD dwPlayerID, int nConnIndex, DWORD dwTongID, 
		DWORD dwItemIdx,BYTE byBoxIndex, BYTE byBoxPos, BYTE byPageIndex, BYTE byRepertoryPos,
		BOOL bFindItem
		);

	static BOOL DoApplyStackTongRepertoryItemRespond(
		int nConnIndex, DWORD dwPlayerID, DWORD dwTongID,
		BYTE byPageIndex, BYTE byPagePos, BYTE byBoxIndex, BYTE byBoxPos, DWORD dwItemID,
		BYTE byTabType, WORD wTabIndex, WORD wStackNum
		);

	static BOOL DoApplyOpenTongRepertoryRespond(int nConnIndex, DWORD dwPlayerID, KTong* pTong);

	static BOOL DoGetTongSalaryRespond(int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, int nSilver);
	static BOOL DoSyncTongHistoryRespond(DWORD dwPlayerID, int nType, DWORD dwStartIndex, int nSyncRecordCount, BYTE byBuffer[], size_t uBufferSize);
	static BOOL DoKinMessageNotify(int nConnIndex, DWORD dwPlayerID, int nCode, const char* pszText = NULL, int nLen = 0);
	static BOOL DoGetBackItemFromRepertoryRespond(DWORD dwPlayerID, BYTE byBoxIndex, BYTE byBoxPos, BOOL bSuccess);
	static BOOL DoSyncTongRankInfo(int nConnIndex, DWORD dwTongID, INT nRankId);

	static BOOL DoGetFreeJoinTongListRespond(DWORD dwPlayerID, BYTE byTongNum, BYTE* pbyData, INT nDataLen, INT nUpdateCounter);

	// --------------------- 帮会操作相关 -----------------------------
	static void OnApplyTongRosterRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnApplyTongInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex); 
	static void OnApplyCreateTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnInvitePlayerJoinTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnInvitePlayerJoinTongRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnApplyKickOutTongMemberRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnModifyTongInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnModifyTongSchemaRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnApplyQuitTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnChangeTongMemberGroupRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnChangeTongMasterRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnChangeTongMemberRemarkRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnGetTongDescriptionRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnSaveMoneyInTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnPayTongSalaryRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnGetTongSalaryRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnGetTongSalaryFailRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnChangeTongCampRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnGetFreeJoinTongListRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnApplyJoinTongRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnSetTongFreeJoinRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnSetTongActivityOpenTimeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);//设置帮会活动开启时间
	static void OnGetTongRankInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	// --------------------- 帮会仓库 -----------------------------
	static void OnApplyRepertoryPageRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnApplyOpenTongRepertoryRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTakeTongRepertoryItemToPosRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTakeTongRepertoryItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTakeTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnPutTongRepertoryItemToPosRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnPutTongRepertoryItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnPutTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnUnlockTongRepertoryGrid(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnExchangeTongRepertoryItemPosRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnApplyStackTongRepertoryItemRequest(BYTE*pbyData, size_t uDataLen, int nConnIndex);
	static void OnStackTongRepertoryItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnRemoveTongRepertoryItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnGetBackItemFromRepertoryRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	struct KTONG_BROADCAST_MESSAGE_TRAVERSE
	{
		BOOL operator () (KTongMember* pMember);

		IMemBlock*                 piSendBuff;
		R2S_TONG_BROADCAST_MESSAGE* pMessage;
		DWORD                       dwExceptID;
	};
	struct KTONG_SYNC_KIN_LIST_TRAVERSE
	{
		BOOL operator () (KTongMember* pMember);

		IMemBlock*                 piSendBuff;
		R2S_SYNC_TONG_APPLY_JOIN_LIST* pSynList;
		DWORD                       dwExceptID;
	};

	static inline BOOL Send(int nConnIndex, IMemBlock* piBuffer);
};
