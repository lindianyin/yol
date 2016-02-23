
#pragma once

#include <map>
#include <list>
#include "ProtocolBasic.h"
#include "Kin.h"
#include "config/gamedef.h"

#define JOIN_TONG_FUNCTION "OnJoinTong"

class KTongManager
{
public:
    KTongManager();

    BOOL Init();
    void UnInit();

    void Activate();
    void RemoveTimeOutInvite();
    void ProcessCheckTongState();

	BOOL SetApplyerState(DWORD dwOperator, DWORD dwApplyerId, DWORD dwTongId, BOOL bAllow);

    BOOL CreateTong(DWORD dwMasterID, const char cszName[], int nTemplateIndex, BYTE byCamp);

    KTong* GetTong(DWORD dwID);
    KTong* GetTongByName(const char* pszTongName);
	KTong* GetTongById(DWORD dwToneId);
    int    GetOperationIndex(KTONG_OPERATION_TYPE eOperationType);

	CONST QTongUpgradeNeed& GetTongUpgradeNeed(INT nTongLevel)
	{
		ASSERT(nTongLevel <= TONG_MAX_LEVEL);
		return m_mapTongUpgradeNeed[nTongLevel];
	}

    BOOL LoadTongData(DWORD dwTongID, BYTE* pbyTongData, size_t uDataLen);

    void ProcessApplyTongInfoRequest(DWORD dwPlayerID, int nLastUpdateFrame);
    void ProcessApplyRosterRequest(DWORD dwPlayerID, int nClientLastUpdateFrame, DWORD* pdwMemberList, int nMemberCount);

    void ProcessInvitePlayerJoinTongRequest(DWORD dwPlayerID, char* pszNewMemberName);
    void ProcessInvitePlayerJoinTongRespond(DWORD dwPlayerID, DWORD dwInviterID, DWORD dwTongID, INVITE_TONG_RESULT_CODE eAccept);
    void ProcessKickOutTongMemberRequest(DWORD dwPlayerID, DWORD dwMemberID);

    void ProcessModifyTongInfoRequest(DWORD dwPlayerID, int nInfoType, BYTE* pbyData, size_t uDataLen);
    BOOL ProcessModifyMemorabiliarequest(
            KTONG_MODIFY_MEMORABILIA_TYPE eType, 
            KTONG_MODIFY_MEMORABILIA_INFO* pInfo, 
            size_t uDataLen, 
            KTong* pTong
        );

    void ProcessModifyTongSchemaRequest(DWORD dwPlayerID, int nGroupIndex, int nModifyType, BYTE* pbyData, size_t uDataLen);
    
    void ProcessQuitRequest(DWORD dwPlayerID);
    void ProcessChangeMemberGroupRequest(DWORD dwPlayerID, DWORD dwTargetID, int nTargetGroup);
    void ProcessChangeMasterRequest(DWORD dwPlayerID, DWORD dwNewMasterID, BOOL bImpeached);
    void ProcessChangeTongMemberRemarkRequest(DWORD dwPlayerID, DWORD dwTargetID, char szRemark[]);
    void ProcessSaveMoneyInTong(KRole* pRole, DWORD dwPealNum, DWORD dwJadeiteNum, DWORD dwAgateNum, DWORD dwDiamondNum, DWORD dwContributionAdd);
    void ProcessRenameConflictTong(int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, const char cszNewName[]);

	void ProcessGetFreeJoinTongList(DWORD dwPlayerID, INT nLastUpdateCounter);
	void ProcessApplyJoinTongRequest(DWORD dwPlayerID, DWORD dwTongID);
	void ProcessSetFreeJoin(DWORD dwPlayerID, DWORD dwTongID, BOOL bValue);
	void ProcessSetTongActivityOpenTime(DWORD dwPlayerID, DWORD dwTongID, DWORD dwTongActivityIndex, DWORD dwTongActivityOpenTime);

	void OnGSConnected(INT nConnIndex);

    void OnPlayerEnterGame(KRole* pRole);
    void OnPlayerLeaveGame(KRole* pRole);
    void SaveAll();

    void SendTongEventNotify(int nConnIndex, DWORD dwPlayerID, TONG_EVENT_NOTIFY_CODE eCode);
    void SendTongEventNotify(int nConnIndex, DWORD dwPlayerID, TONG_EVENT_NOTIFY_CODE eCode, const char cszMsg[]);
    void SendTongEventNotify(int nConnIndex, DWORD dwPlayerID, TONG_EVENT_NOTIFY_CODE eCode, int nParam);

	void CreateSchema();
	void ModifyTongGroup(KTongGroup& tongGroup, int nModifyType, BYTE* pbyData, int nLen);
    
private:
    BOOL LoadOperationConfig();
	BOOL LoadKinLevelConfig();
	BOOL LoadKinActivityConfig();
    BOOL LoadSchemaTemplate();

	BOOL AddRoleToTong(KRole* pRole, KTong* pTong);

    BOOL OnJoinTong(DWORD dwPlayerID, DWORD dwTongID);

	void OnTongMemberChanged(DWORD dwTongID);


private:



    
	std::map<DWORD, QTongUpgradeNeed> m_mapTongUpgradeNeed;
	std::map<DWORD, KinActivity>    m_mapKinActivities;
    
    std::vector<KTongSchemaData>    m_SchemaTemplate;

	typedef std::vector<DWORD>		KTONG_STACK;
	KTONG_STACK						m_TongStack;
	INT								m_TongStackLastUpdateCounter;

public:
	typedef std::map<DWORD, KTong>  KTONG_TABLE;
	KTONG_TABLE                     m_TongTable;//家族列表

	int                             m_nOperationIndex[totTotal];

    struct KTONG_JOIN_APPLICATION
    {
        DWORD   dwTongID;
        DWORD   InviterID;
        time_t  nTimeOut;
    };
    typedef std::map<DWORD, KTONG_JOIN_APPLICATION> KTONG_JOIN_APPLICATION_TABLE;

    KTONG_JOIN_APPLICATION_TABLE    m_InviteApplication;
};

