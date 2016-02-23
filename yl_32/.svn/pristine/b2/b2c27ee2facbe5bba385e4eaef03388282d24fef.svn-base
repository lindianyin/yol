
#pragma once

#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "config/gamedef.h"

class KRole;
struct KPARTY;
struct KMail;
struct KMapInfo;
struct KACCOUNT_LOGIN_INFO; 
enum SWITCH_MAP_RESULT_CODE;
class KGPQ;
class KTong;
struct KTeam;


typedef void (*MODULE_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen, int nConnIndex);

struct KGS_INFO 
{
    int                 nConnIndex;
    ISocketStream*      piSocket;
    int                 nWorldIndex;
    float               fImmediatePerformance;  // 短期性能指数
    float               fAveragePerformance;    // 长期性能指数
    size_t              uMemory;
    int                 nPlayerCount;
	int	                nConnectionCount;
    size_t              uNetworkFlux;
	time_t              nLastRecvPacketTime;
    BOOL                bQuiting;

    DWORD               dwRoleID;
    size_t              uRoleDataOffset;
    BYTE                byRoleDataBuffer[MAX_ROLE_DATA_SIZE];
};

class KGameServer
{
public:
	KGameServer();

	BOOL Init();
    void UnInit();

    void Activate();

    void PrepareToQuit() { m_bQuiting = true; }
    BOOL IsAnyGSConnected();

    KGS_INFO* GetGSInfoByConnectionIndex(int nConnIndex);

    int  GetRandomGS();
    int  GetBestGSConnectionForCreateMap(KMapInfo* pMapInfo);
    
    // 检查玩家登陆时,GS性能是否构成性能限制
    BOOL IsPlayerLoginPerformanceLimit(int nConnIndex);
    
    BOOL ProcessRoleLogin(
        KACCOUNT_LOGIN_INFO* pAccInfo, 
		DWORD dwRoleID, 
		KRoleBaseInfo* pBaseInfo, 
		DWORD dwQueueID
    );
    
    BOOL GetGameWorldProtocolVersion(
		int* pnGameWorldLowerProtocolVersion, 
		int* nGameWorldUpperProtocolVersion
	);

    size_t GetFluxTotal();

    BOOL SendGlobalSysMsg(const char cszMsg[]);

    BOOL LoadRoleData(int nConnIndex, DWORD dwPlayerID, BOOL bSucceed, BYTE* pbyRoleData, size_t uRoleDataLen);

	BOOL Send(int nConnIndex, IMemBlock* piBuffer);
	void Broadcast(IMemBlock* piBuffer, int nExceptConnIndex = 0);
private:
    void ProcessAcceptor();
    BOOL ProcessConnection(int nConnIndex, ISocketStream* piSocket);

    BOOL CloseConnection(int nConnIndex);

    BOOL CallGSConnectScript(int nConnIndex);

	VOID OnGSConnected(int nConnIndex);

private:

    QSocketAcceptor	m_SocketAcceptor;
    int                 m_nLastConnIndex;
    BOOL                m_bQuiting;

	BOOL                m_bGameWorldProtocolValidFlag;
    int                 m_nGameWorldLowerProtocolVersion;
    int                 m_nGameWorldUpperProtocolVersion;

	typedef void (KGameServer::*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[s2r_protocol_end];
    
    //协议大小数组
	size_t m_uProtocolSize[s2r_protocol_end];

	MODULE_PROTOCOL_FUNC	m_ModuleProtocolFuns[s2r_protocol_end];

    KGS_INFO m_GSList[MAX_GS_COUNT];

    int      m_nConnectionTimeout;
    DWORD    m_dwNextLoginQueueID;

    struct KTALK_TONG_TRAVERSE 
    {
        BOOL operator () (KTongMember* pMember);

        IMemBlock*         piSendBuff;
        R2S_CHAT_MESSAGE*   pDownMsg;
        KGameServer*        pGameServer;
    };

private:
	void OnHandshakeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnPingSignal(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnUpdatePerformance(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	void OnPlayerLoginRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnBZonePlayerLoginRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnBZoneSlaveLoginRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	void OnCreateMapRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnSearchMapRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnLoadMapRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnDeleteBattleMapRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	void OnTransferPlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnBZoneTransferRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnTransferPlayerRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	VOID OnBZoneReturnRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex );

	VOID OnGetMultiAwardRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex );

	VOID OnGetCrossSvExDataRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex );

	void OnConfirmPlayerLoginRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	void OnPlayerLeaveGS(BYTE* pbyData, size_t uDataLen, int nConnIndex);

 //   // ----------------------- 聊天相关 ------------------------------

    void OnChatMessage(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnPlayerChatError(BYTE* pbyData, size_t uDataLen, int nConnIndex);
 
    // ----------------------------------------------------------------
    void OnSaveScenePlayerList(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnSaveSceneData(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnPlayerEnterSceneNotify(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnResetMapCopyNotify(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	
    // --------------------- 角色操作相关 -----------------------------
    void OnSaveRoleDataRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnLoadRoleDataRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	void OnLoadDelayRoleDataRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnSyncRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnSaveRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnChangeRoleLevelRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnChangeRoleForceIDRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	void OnCreateDelayRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnSaveDelayRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnSendGmMessage(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnSendGmChn(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnSendGMCommandGS(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnSendGMCommandGC(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	void OnRemoteLuaCall(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnJoinBattleFieldQueueRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnLeaveBattleFieldQueueRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnAcceptJoinBattleField(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnGetBattleFieldList(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnLeaveBattleFieldRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnApplyCreatePQRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnApplyDeletePQRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnApplyChangePQValueRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    
    void OnAddCampScore(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnApplySetCampRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	// ----------------------------------------------------------------
    void OnUpdateStatDataRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnQueryStatIDRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnChangeExtPointRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnSetChargeFlagRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

    void OnMapCopyKeepPlayer(BYTE* pbyData, size_t uDataLen, int nConnIndex);
    void OnActivePresentCodeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	// --------------------- 邮件相关 --------------------------------
	void OnSendMailRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnSendGlobalMailRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnGetMailListRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnQueryMailContent(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnAcquireMailMoneyRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnAcquireMailItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnSetMailRead(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnDeleteMail(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnReturnMail(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnGiveMailMoneyToPlayerRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void OnGiveMailItemToPlayerRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	// -------------------------ArenaPVE竞技场相关---------------------------------------
	// 收到Server请求进入信息
	VOID OnEnterArenaRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);


	//----------------------------------支付相关-------------------------------------------
	VOID OnPayOrderRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	VOID OnPayOrderFinishRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	
	VOID ChangeGMCMDMarkRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	VOID onLoginFindSqlRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	VOID SyncInfoToSqlRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex); 
    // ----------------------------------------------------------------
    //AutoCode:-处理协议函数结束-
public:
	BOOL DoHandshakeRespond(int nConnIndex);
    BOOL DoQuitNotify(int nConnIndex);

	BOOL DoPlayerLoginRequest(
        int nConnIndex, const KACCOUNT_LOGIN_INFO& crAccInfo, DWORD dwRoleID, const KRoleBaseInfo& crBaseInfo
    );

	BOOL DoConfirmPlayerLoginRespond(int nConnIndex, DWORD dwPlayerID, BOOL bPermit);
	
	BOOL DoKickAccountNotify(DWORD dwPlayerID);

	BOOL DoCreateMapNotify(int nConnIndex, DWORD dwMapID, int nMapCopyIndex);

	BOOL DoLoadMapRespond(int nConnIndex, DWORD dwMapID, int nMapCopyIndex, int nCode, int nParam1);

    BOOL DoFinishCreateMapNotify(int nConnIndex, DWORD dwPlayerID);

	BOOL DoDeleteMapNotify(int nConnIndex, DWORD dwMapID, int nMapCopyIndex);

	BOOL DoSearchMapRespond(
        int nConnIndex, DWORD dwPlayerID, SWITCH_MAP_RESULT_CODE eResult, BOOL bSwitchServer, 
        DWORD dwDestMapID, int nDestMapCopyIndex,
        int nX = 0, int nY = 0, int nZ = 0
    );

    BOOL DoTransferPlayerRespond(
        int nConnIndex, DWORD dwPlayerID, BOOL bSucceed, 
        DWORD dwAddress = 0, int nPort = 0, const GUID* pcGuid = NULL
    );

	BOOL DoBZoneTransferPlayerRespond(
		int nConnIndex, DWORD dwDstPlayerID, DWORD dwPlayerID, BOOL bSucceed, 
		DWORD dwAddress = 0, int nPort = 0, const GUID* pcGuid = NULL
		);

	BOOL DoBZoneReturnRespond(
		int nConnIndex, DWORD dwDstPlayerID, DWORD dwPlayerID, BOOL bSucceed, 
		DWORD dwAddress = 0, int nPort = 0, const GUID* pcGuid = NULL
		);

	BOOL DoGetMultiAwardRespond(INT nConIdx, DWORD dwRoleId, INT nMapId, INT nStar);

	BOOL DoGetCrossSvExDataRespond(INT nConIdx, DWORD dwRoleId, LPBYTE byExData, DWORD uExDataLen);
    
    // ------------------------ 聊天相关 --------------------------------------

    BOOL DoPlayerChatError(int nConnectionIndex, DWORD dwReceiverID, int nErrorCode);
    BOOL DoPlayerChatWhisperSuccessRespond(int nConnectionIndex, DWORD dwTalkerID, DWORD dwReceiverID);

    // ----------------------------------------------------------------
    BOOL DoMapCopyDoClearPlayerPrepare(int nConnIndex, DWORD dwMapID, int nMapCopyIndex, int nType, int nDelayTime);
    BOOL DoMapCopyDoClearPlayer(int nConnIndex, DWORD dwMapID, int nMapCopyIndex);
    BOOL DoSetMapCopyOwner(int nConnIndex, DWORD dwMapID, int nMapCopyIndex, DWORD dwOwnerID);
    
    // --------------------- 角色操作相关 -----------------------------
    BOOL DoSaveRoleDataRespond(int nConnIndex, DWORD dwPlayerID, BOOL bSucceed, DWORD uRoleDataLen, int nUserValue, int nUserValue2);

    BOOL DoSyncRoleData(int nConndex, DWORD dwRoleID, BYTE* pbyData, size_t uOffset, size_t uDataLen);
    BOOL DoLoadRoleData(int nConndex, DWORD dwRoleID, BOOL bSucceed, size_t uRoleDataLen);

	BOOL DoDelayLoadRoleData(int nConnIndex, INT nModuleId, DWORD dwPlayerID, DWORD dwRequestId, BOOL bSucceed, BYTE* pbyRoleData, size_t uDelayDataLen);

    // -----------------------------------------------------------------

	BOOL DoGmCommand(int nConnIndex, DWORD dwPlayerID, const char cszGmName[], const char cszGmCommand[]);
	BOOL DoRemoteLuaCall(int nConnIndex, BYTE* pbyData, size_t uDataLen);

    BOOL DoSyncGlobalSystemValue(int nConnIndex);
    BOOL DoQueryStatIDRespond(int nConnIndex, const char szName[], int nID);
    BOOL DoChangeExtPointRespond(int nConnIndex, DWORD dwPlayerID, BOOL bSucceed);
    BOOL DoSetChargeFlagRespond(
        int nConnIndex, DWORD dwPlayerID, int nChargeFlag, const tagExtPointInfo& rExtPointInfo,
        DWORD dwEndTimeOfFee
    );

    BOOL DoSyncZoneChargeFlag(int nConnIndex, int nZoneChargeFlag);
    BOOL DoActivePresentCodeRespond(
        int nConnIndex, DWORD dwPlayerID, BOOL bSucceed, const char* pszPresentCode, DWORD dwPresentType
    );

	// --------------------- 邮件相关 --------------------------------
	BOOL DoSendMailRespond(int nRespondID, KRole* pRole, int nResult, KMail* pMail, size_t uMailLen, const char cszReceiver[]);
	BOOL DoGetMailListRespond(DWORD dwPlayerID, KMailListInfo MailList[], int nMailCount);
	BOOL DoSyncMailContent(
		int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byResult, KMailContent* pContent, size_t uContentDataLen
		);
	BOOL DoGiveMailMoneyToPlayer(
		int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byMailType, int nMoney,int nSilver,int nEnergy, const char cszSender[]
	);
	BOOL DoGiveMailItemToPlayer(
		int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byMailType, BYTE byResult, 
		int nItemIndex, int nItemPrice, BYTE* pbyItemData, size_t uItemDataLen, const char cszSender[]
	);
	BOOL DoNewMailNotity(int nConnIndex, DWORD dwPlayerID, KMailListInfo* pMailListInfo);
	BOOL DoMailGeneralRespond(int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byResult);

	// ---------------------   拍卖行   -----------------------------
	BOOL DoAuctionLookupRespond(DWORD dwPlayerID, BYTE byRespondID, BYTE byCode, KAUCTION_PAGE_HEAD* pPage);
	BOOL DoAuctionBidRespond(DWORD dwPlayerID, BYTE byCode, int nBidPrice);
	BOOL DoAuctionSellRespond(
		DWORD dwPlayerID, BYTE byCode, int nStartPrice, int nBuyItNowPrice, 
		int nCustodyCharges, size_t uItemDataLen, BYTE* pbyItemData
		);
	BOOL DoAuctionCancelRespond(DWORD dwPlayerID, BYTE byCode);
	BOOL DoAuctionMessageNotify(DWORD dwPlayerID, BYTE byCode, const char* pszSaleName, int nPrice);

	// -------------------------ArenaPVE竞技场相关---------------------------------------
	BOOL DoEnterArenaResponse(INT nConnIndex, DWORD dwPlayerID, DWORD* dwPlayerRanks, BYTE* pbyData, SIZE_T uDataLen);

    // -----------------------------------------------------------------
    //AutoCode:-发送协议函数结束-
};
