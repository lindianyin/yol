
#pragma once

#if defined(GAME_SERVER)

#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "onlinegamebase/performancestat.h"
#include <list>

struct IRecorderFactory;
class KPlayer;
class KScene;
class KItem;

typedef void (*MODULE_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen);

class KRelayClient
{
public:
	KRelayClient();

	BOOL Init(/*IRecorderFactory* piFactory*/);
	void UnInit();

    void Activate();
    BOOL ProcessPackage();

    int  GetWorldIndex() { return m_nWorldIndex; }
    float GetUpTraffic() { return m_fUpTraffic; }
    float GetDownTraffic() { return m_fDownTraffic; }

    BOOL SaveRoleData(KPlayer* pPlayer);

    void PrepareToQuit() { m_bQuiting = true; m_nNextQuitingSaveTime = 0; }

	BOOL Send(IMemBlock* piBuffer);

	VOID SendGMCMDAward(BYTE* pbyData, size_t uDataLen);
	BOOL DoGMSendSuccess(DWORD id,DWORD dwRoleId, char* GMCMD);
protected:

    ISocketStream*		m_piSocketStream;
    BOOL                m_bSendErrorFlag;
    BOOL                m_bQuiting;
    BOOL                m_bSyncDataOver;
    time_t              m_nNextQuitingSaveTime;

	typedef void (KRelayClient::*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[r2s_protocol_end];
	MODULE_PROTOCOL_FUNC	m_ModuleProtocolFuns[r2s_protocol_end];

    //协议大小数组
    size_t m_uProtocolSize[r2s_protocol_end];

private:
	int    m_nPingCycle;
    time_t m_nLastSendPacketTime;
    int    m_nWorldIndex;

    DWORD  m_dwSyncRoleID;
    BYTE*  m_pbySyncRoleBuffer;
    size_t m_uSyncRoleOffset;

    BYTE*  m_pbySaveRoleBuffer;

    int    m_nUpTraffic;
    int    m_nDownTraffic;
    float  m_fUpTraffic;
    float  m_fDownTraffic;
	QProtocolStat m_ProtocolStat;

public:
	BOOL DoHandshakeRequest();

	BOOL DoPingSignal();

    BOOL DoUpdatePerformance();

	BOOL DoPlayerLoginRespond(DWORD dwPlayerID, BOOL bPermit, GUID Guid, DWORD dwPacketIdentity);

	BOOL DoConfirmPlayerLoginRequest(DWORD dwPlayerID);

    BOOL DoPlayerLeaveGS(DWORD dwPlayerID);

	BOOL DoCreateMapRespond(DWORD dwMapID, int nMapCopyIndex, BOOL bSucceed);

	BOOL DoSearchMapRequest(DWORD dwPlayerID, DWORD dwMapID, int nMapCopyIndex, int nX, int nY, int nZ, BOOL bOnThisGs = FALSE);

	BOOL DoLoadMapRequest(DWORD dwMapID, DWORD dwMapOwnerId, DWORD dwParam1, BOOL bToThisServer);
	BOOL DoDeleteBattleMapRequest(DWORD dwMapID, DWORD dwMapCopyIndex);
	BOOL DoMapCopyKeepPlayer(KPlayer* pPlayer);

	BOOL DoTransferPlayerRequest(KPlayer* pPlayer);
	BOOL DoTransferPlayerRespond(DWORD dwPlayerID, BOOL bSucceed, GUID Guid);

	BOOL DoBZoneTransferRequest(KPlayer* pPlayer, DWORD uRoleDataLen, INT nBZoneId = 0);
	BOOL DoBZoneReturnRequest(KPlayer* pPlayer, INT nMapId, INT nPosX, INT nPosY, INT nPosZ);
	BOOL ReturnToLastServer(KPlayer* pPlayer, INT nExValue);
	BOOL DoGetMultiAwardRequest(KPlayer* pPlayer);
	BOOL DoGetCrossSvExDataRequest(KPlayer* pPlayer);
    // --------------------- 聊天相关 --------------------------------------

    BOOL DoChatMessage(
        int nMsgType, 
        DWORD dwTalkerID, const char cszTalker[],
        DWORD dwReceiverID, const char cszReceiver[],
        size_t uDataLen, const BYTE*  pbyTalkData, BYTE byShownType
    );

	BOOL DoPlayerChatError(DWORD dwPlayerID, int nErrorCode);

	BOOL DoBroadcastWorldMessage(size_t uDataLen, const BYTE* pbyData);
    //-----------------------------------------------------------------------
    
    BOOL DoSaveScenePlayerList(KScene* pScene);
    BOOL DoSaveSceneData(KScene* pScene);
    BOOL DoPlayerEnterSceneNotify(DWORD dwPlayerID, DWORD dwMapID, int nMapCopyIndex);
	BOOL DoResetMapCopyRequest(DWORD dwPlayerID, DWORD dwMapID, int nMapCopyIndex);
	

    // ----------------------- 角色相关操作 -------------------------------------    
    BOOL DoLoadRoleDataRequest(DWORD dwRoleID);
	BOOL DoLoadDelayRoleDataRequest(INT nModuleId, DWORD dwSrcId, DWORD dwRoleID, LPCTSTR szRoleName = NULL);

    BOOL DoChangeRoleLevelRequest(KPlayer* pPlayer);
    BOOL DoChangeRoleForceIDRequest(KPlayer* pPlayer);

    BOOL DoSendGmMessage(const char cszGmName[], const char cszMessage[]);
    BOOL DoSendGmChn(const char cszAccount[], const char cszRole[], const char cszMessage[]);

    BOOL DoSendGMCommandGS(const char cszRolename[], const char cszCommand[]);
	BOOL DoSendGMCommandGC(const char cszCommand[]);

	BOOL DoRemoteLuaCall(BYTE* pbyParam, size_t uParamLen);

    BOOL DoSyncRoleData(DWORD dwID, BYTE* pbyData, size_t uOffset, size_t uDataLen);
    BOOL DoSaveRoleData(KPlayer* pPlayer, size_t uRoleDataLen);

	BOOL DoCreateDelayRoleData(KPlayer* pPlayer);
	BOOL DoSaveDelayRoleData(KPlayer* pPlayer);


	//-------------------------支付-----------------------------------------
	void OnPayOrderRespond(BYTE* pbyData, size_t uDataLen);

	BOOL DoPayOrderRequest(DWORD dwRoleID);

	BOOL DoPayOrderFinishRequest(DWORD dwRoleID, CHAR* szOrderno);


	//支付完成发奖励
	void OnPayFinish(DWORD dwRoleId, INT nType,DWORD dwGold);


    //-----------------------------------------------------------------------


    //AutoCode:-发送协议函数结束-
private:
	void OnHandshakeRespond(BYTE* pbyData, size_t uDataLen);
	void OnBZoneHandshakeRespond(BYTE* pbyData, size_t uDataLen);
    void OnQuitNotify(BYTE* pbyData, size_t uDataLen);

	void OnCreateMapNotify(BYTE* pbyData, size_t uDataLen);
	void OnLoadMapRespond(BYTE* pbyData, size_t uDataLen);
    void OnFinishCreateMapNotify(BYTE* pbyData, size_t uDataLen);
	void OnDeleteMapNotify(BYTE* pbyData, size_t uDataLen);
	void OnSearchMapRespond(BYTE* pbyData, size_t uDataLen);

	void OnTransferPlayerRequest(BYTE* pbyData, size_t uDataLen);
	void OnTransferPlayerRespond(BYTE* pbyData, size_t uDataLen);
	void OnBZoneTransferPlayerRespond(BYTE* pbyData, size_t uDataLen);
	void OnBZoneReturnRespond(BYTE* pbyData, size_t uDataLen);
	void OnGetMultiAwardRespond(BYTE* pbyData, size_t uDataLen);
	void OnGetCrossSvExDataRespond(BYTE* pbyData, size_t uDataLen);

	void OnPlayerLoginRequest(BYTE* pbyData, size_t uDataLen);

	void OnConfirmPlayerLoginRespond(BYTE* pbyData, size_t uDataLen);

	void OnKickAccountNotify(BYTE* pbyData, size_t uDataLen);

    // ------------------ 聊天相关 ------------------------------------------

    void OnPlayerChatError(BYTE* pbyData, size_t uDataLen);
    void OnPlayerChatWhisperSuccessRespond(BYTE* pbyData, size_t uDataLen);
    void OnChatMessage(BYTE* pbyData, size_t uDataLen);
    
    // ----------------------------------------------------------------------
    void OnMapCopyDoClearPlayerPrepare(BYTE* pbyData, size_t uDataLen);
    void OnMapCopyDoClearPlayer(BYTE* pbyData, size_t uDataLen);
    void OnSetMapCopyOwner(BYTE* pbyData, size_t uDataLen);

    // ----------------------- 角色相关操作 -------------------------------------
    void OnSaveRoleDataRespond(BYTE* pbyData, size_t uDataLen);

	void OnGmCommand(BYTE* pbyData, size_t uDataLen);
	void OnRemoteLuaCall(BYTE* pbyData, size_t uDataLen);

    void OnBroadcastSyncPQ(BYTE* pbyData, size_t uDataLen);
    void OnBroadcastDeletePQ(BYTE* pbyData, size_t uDataLen);
    void OnBroadcastPQValueChange(BYTE* pbyData, size_t uDataLen);
    void OnBroadcastPQFinish(BYTE* pbyData, size_t uDataLen);

    void OnSyncRoleData(BYTE* pbyData, size_t uDataLen);
    void OnLoadRoleData(BYTE* pbyData, size_t uDataLen);

	void OnDelayLoadRoleData(BYTE* pbyData, size_t uDataLen);

	void OnChangeExtPointRespond(BYTE* pbyData, size_t uDataLen);
    // ------------------------------------------------------------------------
	void OnBZoneTransferNotify(BYTE* pbyData, size_t uDataLen);
	//void OnBZoneTransferRequest(BYTE* pbyData, size_t uDataLen);

    //AutoCode:-处理协议函数结束-
};

extern KRelayClient g_RelayClient;

#endif	//_SERVER
