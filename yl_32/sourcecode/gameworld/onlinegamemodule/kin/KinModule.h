#pragma once

#include "onlinegamemodule/kinprotocol.h"
#include "onlinegameworld/kgcmodulebase.h"
#include "serverbase/KinDef.h"

class KPlayer;

class QKinModule : public KModuleBase
{
private:
	virtual BOOL InitProtocol();
	virtual BOOL OnInit();
	virtual BOOL OnUnInit();
	virtual BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
	BOOL ReadSetting();

public:
	QKinModule(void);
	~QKinModule(void);

	struct KIN_CONTRIBUTION_VALUE
	{
		INT nPealExchange;     // 一珍珠兑换的贡献度
		INT nJadeiteExchange;  // 一翡翠兑换的贡献度
		INT nAgateExchange;    // 一玛瑙兑换的贡献度
		INT nDiamondExchange;  // 一钻石兑换的贡献度
		INT nTotalOneDay;       // 一天贡献度的总产出
	};

	struct PLAYER_KIN_INFO
	{
		std::string szKinName;
		INT nKinLevel;
		DWORD dwKinId;
		DWORD dwKinMasterID;

	public:
		PLAYER_KIN_INFO() : szKinName(""), nKinLevel(0), dwKinMasterID(0), dwKinId(0)
		{
			
		}
	};

	BOOL DoSyncTongMemberInfo(DWORD dwPlayerID, const KTongMemberInfo& crTongMember);
	BOOL DoTongDeleteMemberNotify(DWORD dwPlayerID, DWORD dwMemberID);
	BOOL DoApplyTongInfoRespond(DWORD dwPlayerID, BYTE byRespondType, BYTE* pbyRespondData, size_t uDataLen);
	BOOL DoUpdateTongClientDataVersion(DWORD dwPlayerID, KTONG_UPDATE_FLAG eFrameType, int nUpdateFrame);    
	BOOL DoSyncTongRepertoryPageRespond(
		DWORD dwPlayerID, BYTE byPageIndex, int nServerCounter, BYTE byGetRight, BYTE byPutRight,
		BYTE byUpdateItemCount, BYTE byInvailedItemCount, size_t uDataLen, BYTE byData[]
	);
	BOOL DoInvitePlayerJoinTongRequest(DWORD dwPlayerID, DWORD dwInviterID, DWORD dwTongID, BYTE byTongCamp, char szInviterName[], char szTongName[]);
	BOOL DoChangePlayerTongNotify(int nConnIndex, DWORD dwTongID, BYTE byReason, char szTongName[]);
	BOOL DoBroadcastTongMessage(int nConnIndex, BYTE byCode, BYTE* pbyData, size_t uDataLen);
	BOOL DoGetTongDescriptionRespond(int nConnIndex, DWORD dwTongID, char szTongName[], DWORD dwMaster);
	BOOL DoBroadcastPlayerChangeTong(KPlayer* pPlayer);
	BOOL DoSyncTongOnlineMessage(int nConnIndex, char szOnlineMessage[], size_t uDataLen);
	BOOL DoApplyOpenTongRepertoryRespond(int nConnIndex, DWORD dwPlayerID, const char szTongName[], BYTE byRepertoryPageNum);
	BOOL DoSyncTongHistoryRespond(int nConnIndex, BYTE byType, DWORD dwStartIndex, int nCount, BYTE byData[], size_t uDataLen);
	BOOL DoKinMessageNotify(int nConnIndex, BYTE byCode, BYTE* pbyData, size_t uDataLen);
	BOOL DoGetFreeJoinTongListRespond(int nConnIndex, BYTE byTongNum, int nUpdateCounter, BYTE* pbyData, size_t uDataLen);
    BOOL DoSyncTongApplyJoinList(int nConnIndex, BYTE* pbyApplyerData, DWORD dwCount);
	BOOL DoSyncTongActivityOpenTimes(int nConnIndex, BYTE* pbyOpenTimesData, INT nCount);

	void OnApplyTongRosterRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnApplyTongInfoRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnApplyTongRepertoryPageRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnInvitePlayerJoinTongRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnInvitePlayerJoinTongRespond(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnApplyKickOutTongMemberRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnModifyTongInfoRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnModifyTongSchemaRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnApplyQuitTongRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);

	void OnChangeTongMemberGroupRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnChangeTongMasterRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnChangeTongMemberRemarkRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnGetTongDescriptionRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnSaveMoneyInTongRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnPayTongSalaryRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnGetTongSalaryRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnApplyOpenTongRepertoryRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);

	void OnTakeTongRepertoryItemToPosRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnTakeTongRepertoryItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnPutTongRepertoryItemToPosRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnPutTongRepertoryItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);

	void OnExchangeTongRepertoryItemPosRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnStackTongRepertoryItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnRemoveTongRepertoryItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnGetBackItemFromRepertoryRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);

	void OnGetFreeJoinTongListRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnApplyJoinTongRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnSetTongFreeJoin(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnSetKinActivityOpenTime(KPlayer* pPlayer, char* pData, size_t uDataLen);

public:
	INT GetTotalContributionOneDay() { return m_cContributionValue.nTotalOneDay; }

	BOOL UpdatePlayerKinInfo(DWORD dwPlayerId, CONST PLAYER_KIN_INFO& info);

	BOOL UpdateKinLevel(DWORD dwKinId, INT nLevel);

	CONST PLAYER_KIN_INFO& GetPlayerKinInfo(DWORD dwPlayerId){return m_mapPlayerKinInfos[dwPlayerId];}

public:
	// 发送固定长度协议
	static BOOL SendData(INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);
	// 发送变长协议
	static BOOL SendLenData(INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);
	static BOOL SendUnknownData(INT nPlayerIndex, LPSTR pData, UINT uDataSize);

	static BYTE	m_byTempData[32*1024];

private:
	typedef void (QKinModule::*PROCESS_PROTOCOL_FUNC)(KPlayer* pPlayer, char* pData, size_t uDataLen);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[c2s_tong_end];

	KIN_CONTRIBUTION_VALUE m_cContributionValue;
	std::map<DWORD, PLAYER_KIN_INFO> m_mapPlayerKinInfos;
	// size_t m_nProtocolSize[c2s_tong_end];

};

extern QKinModule g_KinModule;


