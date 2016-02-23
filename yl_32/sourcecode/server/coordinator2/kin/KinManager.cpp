
#include "stdafx.h"
#include "StrMap.h"
#include "KRole.h"
#include "KinManager.h"
#include "GameCenter.h"
#include "Kin.h"
#include "DBTools.h"
#include "kinhandler.h"
#include "onlinegamebase/ikbadwordfilter.h"
#include "onlinegamebase/kscriptmanager.h"
#include "serverbase/RankDef.h"
#include "Runtime/TypeDef.h"

#pragma pack(1)
struct KTongSchemaFileHeader
{
    int nVersion;
    int nCount;
};
#pragma pack()

#define TONG_SCHEMA_FILE_VERSION    2
#define MAX_TONG_SCHEMA_TEMPLATE    64

KTongManager::KTongManager()
{
    for (int i = 0; i < (int)sizeof(m_nOperationIndex)/sizeof(int); i++)
        m_nOperationIndex[i] = INVALID_TONG_OPERATION;

	m_TongStackLastUpdateCounter = 0;
}

BOOL KTongManager::Init()
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;

    KTongHistory::Config();

    bRetCode = LoadOperationConfig();
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = LoadKinLevelConfig();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = LoadKinActivityConfig();
	LOG_PROCESS_ERROR(bRetCode);

	CreateSchema();
//    bRetCode = LoadSchemaTemplate();
//    LOG_PROCESS_ERROR(bRetCode);

	m_TongStackLastUpdateCounter = g_pGameCenter->m_nWorkLoop;

    bResult = true;
EXIT0:
    return bResult;
}

void KTongManager::UnInit()
{
    KTong* pTong = NULL;
    
    for (KTONG_TABLE::iterator it = m_TongTable.begin(); it != m_TongTable.end(); ++it)
    {
        pTong = &it->second;

        if (pTong->m_bUpdateFlag)
        {
            g_pGameCenter->m_MiscDB.DoSaveTongData(pTong->m_dwID, pTong);
            pTong->m_bUpdateFlag = false;
        }

        it->second.UnInit();
    }

    m_TongTable.clear();
}

void KTongManager::Activate()
{   
    if (g_pGameCenter->m_nWorkLoop % (GAME_FPS * 10) == 0)
    {
        RemoveTimeOutInvite();
    }
    
    if (g_pGameCenter->m_nWorkLoop % (GAME_FPS * 10) == 2)
    {
        ProcessCheckTongState();
    }

    if (g_pGameCenter->m_nWorkLoop % (GAME_FPS * 10) == 8)
    {
        for (KTONG_TABLE::iterator it = m_TongTable.begin(); it != m_TongTable.end(); ++it)
        {
            it->second.Activate();
        }
    }
}

void KTongManager::RemoveTimeOutInvite()
{
    time_t                                  nTimeNow = g_pGameCenter->m_nTimeNow;
    KRole*                                  pInviter = NULL;
    KRole*                                  pPlayer  = NULL;
    KTONG_JOIN_APPLICATION_TABLE::iterator  it;

    for (it = m_InviteApplication.begin(); it != m_InviteApplication.end(); NULL)
    {
        if (it->second.nTimeOut < nTimeNow)
        {
            pInviter = g_pGameCenter->m_RoleManager.GetRole(it->second.InviterID);
            pPlayer  = g_pGameCenter->m_RoleManager.GetRole(it->first);

            m_InviteApplication.erase(it++);

            if (pInviter && pPlayer && pInviter->IsOnline())
            {
                SendTongEventNotify(
                    pInviter->m_nConnIndex, pInviter->m_dwPlayerID, tenInviteTimeOutError,
                    pPlayer->m_szName
                );
            }

            continue;
        }

        ++it;
    }
}

void KTongManager::ProcessCheckTongState()
{
    BYTE byTongState = 0;

    for (KTONG_TABLE::iterator it = m_TongTable.begin(); it != m_TongTable.end(); NULL)
    {
        KTong*  pTong               = &it->second;
        BOOL    bTongStateChange    = false;
		
        //Q_Printl("..tong id="<<it->first);

		switch (pTong->m_eState)
        {
        case tsTrialTong:
            if (pTong->m_Roster.size() >= MIN_TONG_MEMBER_COUNT)
            {
                pTong->m_eState         = tsNormalTong;
                pTong->m_nStateTimer    = 0;
                pTong->m_bUpdateFlag    = true;
                pTong->m_nLastUpdateFrameTable[tdtBaseInfo] = g_pGameCenter->m_nWorkLoop;
                bTongStateChange        = true;
            }
            else if (g_pGameCenter->m_nTimeNow > pTong->m_nStateTimer)
            {

				//Q_Printl("..g_pGameCenter->m_nTimeNow > pTong->m_nStateTimer");
                //byTongState = (BYTE)tsInvalid;
                //KinHandler::DoTongBroadcastMessage(pTong, tbmTongStateChange, KD_BAD_ID, &byTongState, sizeof(byTongState));
                //pTong->Disband();
                //pTong->UnInit();
				//KTONG_STACK::iterator it_stack = std::find(m_TongStack.begin(), m_TongStack.end(), it->first);
				//解散家族时删除排行榜的该家族信息
				//g_pGameCenter->m_RankManager.EraseRankValue(emRANK_TONG_FIGHTSCORE, pTong->m_dwID);
				//g_pGameCenter->m_RankManager.EraseRankValue(emRANK_TONG_SALARY, pTong->m_dwID);
				//m_TongStack.erase(it_stack);
                //m_TongTable.erase(it++);
                //continue;   // for (KTONG_TABLE::iterator it ..
            }
            break;          // switch (pTong->m_eState)

        case tsNormalTong:
            if (pTong->m_Roster.size() < MIN_TONG_MEMBER_COUNT)
            {
                pTong->m_eState         = tsDisbandTong;
                pTong->m_nStateTimer    = g_pGameCenter->m_nTimeNow + TONG_DISBAND_TIME;
                pTong->m_bUpdateFlag    = true;
                pTong->m_nLastUpdateFrameTable[tdtBaseInfo] = g_pGameCenter->m_nWorkLoop;
                bTongStateChange        = true;
            }
            break;

        case tsDisbandTong:
            if (pTong->m_Roster.size() >= MIN_TONG_MEMBER_COUNT)
            {
                pTong->m_eState         = tsNormalTong;
                pTong->m_nStateTimer    = 0;
                pTong->m_bUpdateFlag    = true;
                pTong->m_nLastUpdateFrameTable[tdtBaseInfo] = g_pGameCenter->m_nWorkLoop;
                bTongStateChange        = true;
            }
            else if (g_pGameCenter->m_nTimeNow > pTong->m_nStateTimer)
            {
            	//Q_Printl("..g_pGameCenter->m_nTimeNow > pTong->m_nStateTimer");
				
                //byTongState = (BYTE)tsInvalid;
                //KinHandler::DoTongBroadcastMessage(pTong, tbmTongStateChange, KD_BAD_ID, &byTongState, sizeof(byTongState));
                //pTong->Disband();
                //pTong->UnInit();
				//KTONG_STACK::iterator it_stack = std::find(m_TongStack.begin(), m_TongStack.end(), it->first);
				//解散家族时删除排行榜的该家族信息
				//g_pGameCenter->m_RankManager.EraseRankValue(emRANK_TONG_FIGHTSCORE, pTong->m_dwID);
				//g_pGameCenter->m_RankManager.EraseRankValue(emRANK_TONG_SALARY, pTong->m_dwID);
				//m_TongStack.erase(it_stack);
                //m_TongTable.erase(it++);
                //continue;
            }
            break;

       default:
            assert(false);
            break;
        }

        if (bTongStateChange)
        {
            byTongState = (BYTE)pTong->m_eState;
            KinHandler::DoTongBroadcastMessage(pTong,  tbmTongStateChange, KD_BAD_ID,&byTongState, sizeof(byTongState));
        }

        ++it; 
    }
}

BOOL KTongManager::CreateTong(DWORD dwMasterID, const char cszName[], int nTemplateIndex, BYTE byCamp)
{
    BOOL                                bResult     = false;
    BOOL                                bRetCode    = false;
    KRole*                              pMaster     = NULL;
    KTong*                              pTong       = NULL;
    KTong*                              pTongRet    = NULL;
    DWORD                               dwTongID    = KD_BAD_ID;
    unsigned long long                  ullID       = KD_BAD_ID;
    BOOL                                bInsertFlag = false;
	std::pair<KTONG_TABLE::iterator, BOOL>   TongInsRet;
	std::map<DWORD, KinActivity>::iterator it_kinact;
	//BYTE    							byBuffer[32];
	
    assert(cszName);
	QLogPrintf(LOG_INFO,"..KTongManager::CreateTong()");
    LOG_PROCESS_ERROR(nTemplateIndex >= 0 && nTemplateIndex < (int)m_SchemaTemplate.size());

    pMaster = g_pGameCenter->m_RoleManager.GetRole(dwMasterID);
    LOG_PROCESS_ERROR(pMaster);
    
    if (pMaster->m_dwTongID != KD_BAD_ID)
    {
        SendTongEventNotify(pMaster->m_nConnIndex, dwMasterID, tenCreateTongAlreadyInTongError);
        goto EXIT0;
    }

    // 帮会名关键字过滤
//	bRetCode = g_pBadwordFilter->HasBadword(cszName);
//	if (bRetCode)
//	{
//		SendTongEventNotify(pMaster->m_nConnIndex, dwMasterID, tenCreateTongNameIllegalError);
//		goto EXIT0;
//	}

 //   bRetCode = g_pGameCenter->m_piTextFilter->Check(cszName);
 //   if (!bRetCode)
 //   {
 //       SendTongEventNotify(pMaster->m_nConnIndex, dwMasterID, tenCreateTongNameIllegalError);
 //       goto EXIT0;
 //   }

    pTongRet = GetTongByName(cszName);
    
    if (pTongRet)
    {
        SendTongEventNotify(pMaster->m_nConnIndex, dwMasterID, tenCreateTongNameConflictError);
        goto EXIT0;
    }

    bRetCode = g_pGameCenter->m_piSequence->GenerateID("TongID", 8, &ullID);
    LOG_PROCESS_ERROR(bRetCode);
    
    dwTongID = (DWORD)ullID;

	//插入家族列表
	TongInsRet = m_TongTable.insert(std::make_pair(dwTongID, KTong()));
    LOG_PROCESS_ERROR(TongInsRet.second);
    bInsertFlag = true;

    pTong = &TongInsRet.first->second;
    pTong->Init();

    pTong->m_dwID               = dwTongID;
    pTong->m_dwMaster           = pMaster->m_dwPlayerID;
	pTong->m_byCamp             = byCamp;
    pTong->m_nMaxMemberCount    = TONG_INIT_MAX_MEMBER_COUNT;
    pTong->m_nDevelopmentPoint  = 0;
    pTong->m_eState             = tsTrialTong;          // 帮会进入筹备状态
    pTong->m_nStateTimer        = g_pGameCenter->m_nTimeNow + TONG_TRIAL_TIME; 
	
	for (it_kinact = m_mapKinActivities.begin(); it_kinact != m_mapKinActivities.end(); ++it_kinact)
	{
		// 默认取最后一个时间
		pTong->m_ActivityOpenTimeMap[it_kinact->second.nKinActivityIndex].nKinActivityIndex = it_kinact->first;
		pTong->m_ActivityOpenTimeMap[it_kinact->second.nKinActivityIndex].dwActivityOpenTime = it_kinact->second.vecKinOpenTimes[it_kinact->second.vecKinOpenTimes.size() - 1];
	}

    strncpy(pTong->m_szName, cszName, sizeof(pTong->m_szName));
    pTong->m_szName[sizeof(pTong->m_szName) - 1] = '\0';

    pTong->m_Schema = m_SchemaTemplate[nTemplateIndex];
	// added by liyun，第0页放绑定装备，第5页放非绑定装备道具
    pTong->m_Repertory.m_Page[0].bEnable = true; // 开启仓库第一页
	pTong->m_Repertory.m_Page[5].bEnable = true; // 开启仓库第五页

    bRetCode = pTong->AddNewMember(pMaster, m_SchemaTemplate[nTemplateIndex].nMasterGroup);
    LOG_PROCESS_ERROR(bRetCode);

    QLogPrintf(LOG_INFO, "Player '%s'(id:%d) create tong '%s'(id:%d) success!\n", 
        pMaster->m_szName, dwMasterID, pTong->m_szName, pTong->m_dwID
    );

    // 同步帮会ID
    KinHandler::DoChangePlayerTongNotify(pMaster->m_nConnIndex, dwMasterID, dwTongID, ctrCreate, pTong->m_szName);

    pTong->m_bUpdateFlag = true;

	pTong->m_bUpdateTongRankFlag = false;
    
    bRetCode = OnJoinTong(dwMasterID, dwTongID);
    LOG_CHECK_ERROR(bRetCode);

	m_TongStack.push_back(dwTongID);
	OnTongMemberChanged(dwTongID);

	{
		ScriptSafe safeScript = g_cScriptManager.GetSafeScript();
		safeScript->CallTableFunction("Kin", "CreateKinActivitiesInGS", 0, "dd", dwTongID, -1);
	}

	g_pGameCenter->m_RankManager.ModifyRankValue(emRANK_TONG_FIGHTSCORE, pTong->m_dwID, pTong->GetTotalFightScore());
	g_pGameCenter->m_RankManager.ModifyRankValue(emRANK_TONG_SALARY, pTong->m_dwID, pTong->GetTotalSalary());


	//byBuffer[0] = '\0';
   	//g_pGameCenter->m_MailManager.PostRequestSendMail(
    //        0, pMaster->m_dwPlayerID, 0, eMailType_System, 
    //        g_pGameCenter->m_Settings.m_szSystemMailSender, 
    //        g_pGameCenter->m_Settings.m_szTongReturnMoneyMailTitle,
    //        1024,1025,1026,
    //        byBuffer, 1,
    //        NULL, 0, 0);
	
	
    bResult = true;
EXIT0:
    if (!bResult)
    {
        if (bInsertFlag)
        {
            if (pTong)
                pTong->UnInit();

            m_TongTable.erase(dwTongID);
            bInsertFlag = false;
        }
    }

    return bResult;
}

KTong* KTongManager::GetTong(DWORD dwID)
{
    KTONG_TABLE::iterator it = m_TongTable.find(dwID);
    if (it != m_TongTable.end())
    {
        return &it->second;
    }
    return NULL;
}

KTong* KTongManager::GetTongByName(const char* pszTongName)
{
    KTong* pTongResult = NULL;

    for (KTONG_TABLE::iterator it = m_TongTable.begin(); it != m_TongTable.end(); ++it)
    {
        if (strncmp(pszTongName, it->second.m_szName, sizeof(it->second.m_szName)) == 0)
        {
            pTongResult = &it->second;
            break;
        }
    }

    return pTongResult;
}

KTong* KTongManager::GetTongById(DWORD dwToneId)
{
	KTong* pTongResult = NULL;

	KTONG_TABLE::iterator it = m_TongTable.find(dwToneId);
	PROCESS_ERROR(it != m_TongTable.end());

	pTongResult = &it->second;

EXIT0:
	return pTongResult;
}

int KTongManager::GetOperationIndex(KTONG_OPERATION_TYPE eOperationType)
{
    assert(eOperationType > totInvalid && eOperationType < totTotal);
    return m_nOperationIndex[eOperationType];
}

BOOL KTongManager::LoadTongData(DWORD dwTongID, BYTE* pbyTongData, size_t uDataLen)
{
    BOOL                                    bResult     = false;
    BOOL                                    bRetCode    = false;
    KTong*                                  pTong       = NULL;
    BOOL                                    bInsertFlag = false;
    std::pair<KTONG_TABLE::iterator, bool>  InsertRet;

    assert(pbyTongData);
    
	InsertRet = m_TongTable.insert(std::make_pair(dwTongID, KTong()));
    LOG_PROCESS_ERROR(InsertRet.second);
    bInsertFlag = true;

    pTong = &InsertRet.first->second;
    LOG_PROCESS_ERROR(pTong);
    pTong->Init();

    pTong->m_dwID = dwTongID;

    bRetCode = pTong->Load(dwTongID, pbyTongData, uDataLen);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;

	m_TongStack.push_back(dwTongID);
	OnTongMemberChanged(dwTongID);

EXIT0:
    if (!bResult)
    {
        if (bInsertFlag)
        {
            pTong->UnInit();
            m_TongTable.erase(dwTongID);
            bInsertFlag = false;
        }
    }

    return bResult;
}

void KTongManager::ProcessSetTongActivityOpenTime(DWORD dwPlayerID, DWORD dwTongID, DWORD dwTongActivityIndex, DWORD dwTongActivityOpenTime)
{
	BOOL                        bRetCode    = FALSE;
	KRole*                      pPlayer     = NULL;
	KTong*                      pTong       = NULL;

	pPlayer = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	PROCESS_ERROR(pPlayer->m_dwTongID != KD_BAD_ID);
	PROCESS_ERROR(pPlayer->m_dwTongID == dwTongID);

	pTong = GetTong(dwTongID);
	LOG_PROCESS_ERROR(pTong);

	PROCESS_ERROR(pTong->m_dwMaster == dwPlayerID); // 只有族长有权限设置活动时间

	bRetCode = pTong->SetActivityOpenTime(dwTongActivityIndex, dwTongActivityOpenTime);
	if (bRetCode)
	{
		QTONG_ACTIVITY_TIME_SET_BROADCAST  cActivityTimeSet;
		cActivityTimeSet.dwOpenTime = dwTongActivityOpenTime;
		cActivityTimeSet.dwTongActivityIndex = dwTongActivityIndex;

		KinHandler::DoTongBroadcastMessage(pTong, tbmSetTongActivityTime, KD_BAD_ID, (BYTE*)&cActivityTimeSet, sizeof(cActivityTimeSet));

		pTong->m_History.RecordMissionStart(pTong->m_dwMaster, dwTongActivityIndex, dwTongActivityOpenTime);
	}

EXIT0:;
}

void KTongManager::ProcessApplyTongInfoRequest(DWORD dwPlayerID, int nLastUpdateFrame)
{
    BOOL                        bRetCode    = false;
    DWORD                       dwTongID    = KD_BAD_ID;
    KRole*                      pPlayer     = NULL;
    KTong*                      pTong       = NULL;

    pPlayer = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pPlayer);

    PROCESS_ERROR(pPlayer->m_dwTongID != KD_BAD_ID);

    pTong = GetTong(pPlayer->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtBaseInfo])
    {
        KinHandler::DoUpdateTongBaseInfo(pPlayer->m_nConnIndex, dwPlayerID, pTong);
		KinHandler::DoSyncTongActivityOpenTimes(pPlayer->m_nConnIndex, pPlayer->GetId(), pTong->m_dwID);
		pTong->m_History.SyncRecord(dwPlayerID, 0, TONG_OPERATION_HISTORY_PAGE_SIZE - 1);
    }

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtDevelopmentInfo])
    {
        KinHandler::DoUpdateTongDevelopmentInfo(pPlayer->m_nConnIndex, dwPlayerID, pTong);
    }

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtAnnouncement])
    {
        KinHandler::DoUpdateTongTextInfo(
            pPlayer->m_nConnIndex, dwPlayerID, 
            tdtAnnouncement, pTong->m_szAnnouncement
        );
    }

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtOnlineMessage])
    {
        KinHandler::DoUpdateTongTextInfo(
            pPlayer->m_nConnIndex, dwPlayerID, 
            tdtOnlineMessage, pTong->m_szOnlineMessage
        );
    }

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtIntroduction])
    {
        KinHandler::DoUpdateTongTextInfo(
            pPlayer->m_nConnIndex, dwPlayerID, 
            tdtIntroduction, pTong->m_szIntroduction
        );
    }

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtRules])
    {
        KinHandler::DoUpdateTongTextInfo(
            pPlayer->m_nConnIndex, dwPlayerID, 
            tdtRules, pTong->m_szRules
        );
    }

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtSchemaInfo])
    {
        KinHandler::DoUpdateTongSchemaInfo(pPlayer->m_nConnIndex, dwPlayerID, &pTong->m_Schema);
    }

    if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtMemorabilia])
    {
        KinHandler::DoUpdateTongMemorabilia(pPlayer->m_nConnIndex, dwPlayerID, pTong);
    }

	if (nLastUpdateFrame < pTong->m_nLastUpdateFrameTable[tdtApplyersInfo])
	{
		KinHandler::DoSyncTongApplyJoinList(pTong->m_dwID);
	}

    KinHandler::DoUpdateTongClientDataVersion(
        pPlayer->m_nConnIndex, dwPlayerID, tufInfo, g_pGameCenter->m_nWorkLoop
    );

EXIT0:
    return;
}

void KTongManager::ProcessApplyRosterRequest(DWORD dwPlayerID, int nClientLastUpdateFrame, DWORD* pdwMemberList, int nMemberCount)
{
    BOOL                            bRetCode        = false;
    DWORD                           dwTongID        = KD_BAD_ID;
    KRole*                          pRole           = NULL;
    const KTong*                    pTong           = NULL;

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);

    pTong = GetTong(pRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    for (KTong::KROSTER::const_iterator MemberIter = pTong->m_Roster.begin(); MemberIter != pTong->m_Roster.end(); ++MemberIter)
    {
        if (MemberIter->second.nLastUpdateFrame <= nClientLastUpdateFrame)
            continue;

        bRetCode = KinHandler::DoSyncTongMemberInfo(
            pRole->m_nConnIndex, dwPlayerID, MemberIter->first, MemberIter->second
        );
        LOG_PROCESS_ERROR(bRetCode);
    }

    for (int i = 0; i < nMemberCount; i++)
    {
        if (pTong->m_Roster.find(pdwMemberList[i]) == pTong->m_Roster.end())
        {
            KinHandler::DoTongDeleteMemberNotify(pRole->m_nConnIndex, dwPlayerID, pdwMemberList[i]);
        }
    }

    bRetCode = KinHandler::DoUpdateTongClientDataVersion(
        pRole->m_nConnIndex, dwPlayerID, tufRoster, g_pGameCenter->m_nWorkLoop
    );
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    return;
}

void KTongManager::ProcessInvitePlayerJoinTongRequest(DWORD dwPlayerID, char* pszNewMemberName)
{
    BOOL                    bRetCode    = false;
    KRole*                  pPlayer     = NULL;
    KRole*                  pNewMember  = NULL;
    KTong*                  pTong       = NULL;
    KTongMember*            pMember     = NULL;
    KTONG_JOIN_APPLICATION  Apply;

    assert(pszNewMemberName);

    pPlayer = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pPlayer);

    PROCESS_ERROR(pPlayer->m_dwTongID != KD_BAD_ID);
       
    pTong   = GetTong(pPlayer->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    pNewMember = g_pGameCenter->m_RoleManager.GetRole(pszNewMemberName);
    if (pNewMember == NULL)
    {
        // 角色名不存在
        // send Message
        SendTongEventNotify(
            pPlayer->m_nConnIndex, dwPlayerID, tenInviteNameNotFoundError, pszNewMemberName
        );
        goto EXIT0;
    }
    
    if (pNewMember->m_dwTongID != KD_BAD_ID)
    {
        // 对方已加入别的帮会
        // Send Message
        SendTongEventNotify(
            pPlayer->m_nConnIndex, dwPlayerID, tenInviteTargetAlreadyJoinTongError, pszNewMemberName
        );
        goto EXIT0; 
    }

    if (m_InviteApplication.find(pNewMember->m_dwPlayerID) != m_InviteApplication.end())
    {
        // 对方正在被邀请
        // Send Message
        SendTongEventNotify(
            pPlayer->m_nConnIndex, dwPlayerID, tenInviteTargetBusyError, pszNewMemberName
        );
        goto EXIT0;
    }

    if (!pNewMember->IsOnline())
    {
        // 对方不在线
        // Send Message
        SendTongEventNotify(
            pPlayer->m_nConnIndex, dwPlayerID, tenInviteTargetOfflineError, pszNewMemberName
        );
        goto EXIT0;
    }

    if ((int)pTong->m_Roster.size() >= pTong->m_nMaxMemberCount)
    {
        // 人满
        // Send Message to Inviter
        SendTongEventNotify(pPlayer->m_nConnIndex, dwPlayerID, tenInviteMemberNumberLimitError);
        goto EXIT0;
    }

    pMember = pTong->GetMember(dwPlayerID);

    // 检查权限
    bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totAllowOrDenyApplyer);
    if (!bRetCode)
    {
        // 未授权
        // Send Message
        SendTongEventNotify(pPlayer->m_nConnIndex, dwPlayerID, tenInviteNoPermissionError);
        goto EXIT0;
    }

    Apply.dwTongID      = pPlayer->m_dwTongID;
    Apply.InviterID     = dwPlayerID;
    Apply.nTimeOut      = g_pGameCenter->m_nTimeNow + TONG_INVITE_TIME;

    m_InviteApplication[pNewMember->m_dwPlayerID] = Apply;

    //  发送确认请求
    KinHandler::DoInvitePlayerJoinTongRequest(
        pNewMember->m_nConnIndex, pNewMember->m_dwPlayerID, dwPlayerID, pTong->m_dwID, pTong->m_byCamp,
        pPlayer->m_szName, pTong->m_szName
    );

EXIT0:
    return;
}

void KTongManager::ProcessInvitePlayerJoinTongRespond(DWORD dwPlayerID, DWORD dwInviterID, DWORD dwTongID, INVITE_TONG_RESULT_CODE eAccept)
{
    BOOL                                    bRetCode        = false;
    KRole*                                  pRole           = NULL;
    KRole*                                  pInviterRole    = NULL;
    KTong*                                  pTong           = NULL;
    KTongMember*                            pInviterMember  = NULL;
	KMapInfo*                               pMapInfo        = NULL;
    KTONG_JOIN_APPLICATION*                 pInviteRecord   = NULL;
    KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST     ChangeInfo;
    KTONG_JOIN_APPLICATION_TABLE::iterator  it;

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    pInviterRole =  g_pGameCenter->m_RoleManager.GetRole(dwInviterID);
    LOG_PROCESS_ERROR(pInviterRole);

    it = m_InviteApplication.find(pRole->m_dwPlayerID);
    if (it == m_InviteApplication.end())
    {
        // 请求不存在或已失效
        if (eAccept == itrSuccess && pRole->m_nConnIndex != 0)
        {
            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenInviteInvaildError);
        }

        goto EXIT0;
    }

    pInviteRecord = &it->second;
    LOG_PROCESS_ERROR(pInviteRecord->dwTongID == dwTongID);
    LOG_PROCESS_ERROR(pInviteRecord->InviterID == dwInviterID);

    m_InviteApplication.erase(it);
	if (eAccept == itrInvalid)
	{
		if (pInviterRole->m_nConnIndex != 0)
		{
			SendTongEventNotify(pInviterRole->m_nConnIndex, dwInviterID, tenInviteInvaildError);
		}

		goto EXIT0;
	}

	if (eAccept == itrCampLimit)
	{
		if (pInviterRole->m_nConnIndex != 0)
		{
			SendTongEventNotify(pInviterRole->m_nConnIndex, dwInviterID, tenInviteCampLimit);
		}

		goto EXIT0;
	}

	if (eAccept == itrLevelLimit)
	{
		if (pInviterRole->m_nConnIndex != 0)
		{
			SendTongEventNotify(pInviterRole->m_nConnIndex, dwInviterID, tenInviteLevelLimit);
		}

		goto EXIT0;
	}

    if (eAccept == itrRefuse)
    {
        // 拒绝
        // Send Message to inviter
        if (pInviterRole->m_nConnIndex != 0)
        {
            SendTongEventNotify(
                pInviterRole->m_nConnIndex, dwInviterID, tenInviteTargetRefuse, pRole->m_szName
            );
        }
        goto EXIT0;
    }

    if (eAccept == itrChargeLimit)
    {
        // 拒绝：因为免费阶段
        if (pInviterRole->m_nConnIndex != 0)
        {
            SendTongEventNotify(
                pInviterRole->m_nConnIndex, dwInviterID, tenInviteTargetChargeLimit, pRole->m_szName
            );
        }
        goto EXIT0;
    }

    pTong = GetTong(dwTongID);
    if(!pTong)  // 帮会已解散
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenInviteInvaildError);
        goto EXIT0;
    }

    //// 目标帮会是敌对阵营
    //bRetCode = pTong->CheckCampCompatible(pRole->m_eCamp);
    //if (!bRetCode)
    //{
    //    SendTongEventNotify(
    //        pInviterRole->m_nConnIndex, dwInviterID, tenTargetTongIsRivalCampError, pTong->m_szName
    //    );

    //    SendTongEventNotify(
    //        pInviterRole->m_nConnIndex, dwInviterID, tenInviteTargetInRivalCampError, pRole->m_szName
    //    );
    //    goto EXIT0; 
    //}

    if ((int)pTong->m_Roster.size() >= pTong->m_nMaxMemberCount)
    {
        // 人满
        // Send Message to player and inviter
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenInviteMemberNumberLimitError);
        SendTongEventNotify(pInviterRole->m_nConnIndex, dwInviterID, tenInviteMemberNumberLimitError);
        goto EXIT0;
    }

    pInviterMember = pTong->GetMember(dwInviterID);
    if(!pInviterMember) //邀请人已删号或退出了帮会
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenInviteInvaildError);
        goto EXIT0;
    }

    // 再次检查邀请人权限
    bRetCode = pTong->CanBaseOperate(pInviterMember->nGroupID, totAllowOrDenyApplyer);
    if (!bRetCode)
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenInviteInvaildError);
        goto EXIT0;
    }

	// 检查目标是否在活动地图中
	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pInviterRole->m_dwMapID);
	if (pMapInfo->m_nType == emtBattleField)
	{
		SendTongEventNotify(
			pRole->m_nConnIndex, dwPlayerID, tenTargetInActivity, pInviterRole->m_szName
			);
		goto EXIT0;
	}

    bRetCode = pTong->AddNewMember(pRole, pTong->m_Schema.nDefaultGroup);
    LOG_PROCESS_ERROR(bRetCode);

	OnTongMemberChanged(pTong->m_dwID);

    bRetCode = pTong->m_History.RecordJoin(dwPlayerID);
    LOG_CHECK_ERROR(bRetCode);

    KinHandler::DoChangePlayerTongNotify(pRole->m_nConnIndex, dwPlayerID, dwTongID, ctrJoin, pTong->m_szName);

    SendTongEventNotify(
        pInviterRole->m_nConnIndex, dwInviterID, tenInviteSuccess, pRole->m_szName
    );

    strncpy(ChangeInfo.szMemberName, pRole->m_szName, sizeof(ChangeInfo));
    ChangeInfo.szMemberName[sizeof(ChangeInfo.szMemberName) - 1] = '\0';

    KinHandler::DoTongBroadcastMessage(pTong, tbmMemberJoin, dwPlayerID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));
        
    bRetCode = OnJoinTong(dwPlayerID, dwTongID);
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    return;
}

void KTongManager::ProcessKickOutTongMemberRequest(DWORD dwPlayerID, DWORD dwMemberID)
{
    BOOL                                bRetCode        = false;
    KTong*                              pTong           = NULL;
    KRole*                              pSrcRole        = NULL;
    KRole*                              pDstRole        = NULL;
    KTongMember*                        pSrcMember      = NULL;
    KTongMember*                        pDstMember      = NULL;
	KMapInfo*                           pMapInfo        = NULL;
    KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST ChangeInfo;

    pSrcRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pSrcRole);

    pDstRole = g_pGameCenter->m_RoleManager.GetRole(dwMemberID);
    LOG_PROCESS_ERROR(pDstRole);

    PROCESS_ERROR(pSrcRole->m_dwTongID != KD_BAD_ID);
    PROCESS_ERROR(pSrcRole->m_dwTongID == pDstRole->m_dwTongID);

    pTong = GetTong(pSrcRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    pSrcMember = pTong->GetMember(dwPlayerID);
    LOG_PROCESS_ERROR(pSrcMember);

    pDstMember = pTong->GetMember(dwMemberID);
    if (pDstMember == NULL)
    {
        SendTongEventNotify(pSrcRole->m_nConnIndex, dwPlayerID, tenKickOutTargetNotFoundError);
        goto EXIT0;
    }
	
    // 检查权限
    bRetCode = pTong->CanAdvanceOperate(pSrcMember->nGroupID, pDstMember->nGroupID, totMoveFromGroup);
    if (!bRetCode)
    {
    	char buff[256];
		ZeroMemory(buff,sizeof(buff));
		sprintf(buff,LANG_043,pDstRole->m_szName);
        SendTongEventNotify(
            pSrcRole->m_nConnIndex, dwPlayerID, tenKickOutTargetNoPermissionError, buff
        );
        goto EXIT0;
    }

	// 检查目标是否在活动地图中
	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pDstRole->m_dwMapID);
	if (pMapInfo != NULL && pMapInfo->m_nType == emtBattleField)
	{
		SendTongEventNotify(
			pSrcRole->m_nConnIndex, dwPlayerID, tenTargetInActivity, pDstRole->m_szName
			);
		goto EXIT0;
	}

    // 移除
    bRetCode = pTong->RemoveMember(pDstRole);
    LOG_PROCESS_ERROR(bRetCode);

	OnTongMemberChanged(pTong->m_dwID);

	// 被T了，标记他借的装备可以借出，放入的装备没借出则删除，有借出则标记过期删除
	for (int i = 0; i < TONG_REPERTORY_PAGE_NUM; ++i)
	{
		KTONG_REPERTORY_PAGE* pPage = &pTong->m_Repertory.m_Page[i];
		if (pPage->bEnable)
		{
			for (int j = 0; j < TONG_REPERTORY_PAGE_CAPACITY; ++j)
			{
				KTONG_REPERTORY_GRID* pGrid = &pPage->Grids[j];
				if (pGrid->dwPutPlayerId == dwPlayerID)
				{
					if (pGrid->dwTakePlayerId == KD_BAD_ID ||
						(pGrid->dwTakePlayerId != KD_BAD_ID && pGrid->nExpireTime < g_pGameCenter->m_nTimeNow)) 
					{
						pTong->m_Repertory.LockGrid(i, j);
						pTong->m_Repertory.FreeItem(i, j);
					}
					else {
						pGrid->bRemoveOnExpire = TRUE;
						pGrid->nLastUpdateCounter = ++pTong->m_Repertory.m_nUpdateCounter;
					}
				}
				if (pGrid->dwTakePlayerId == dwPlayerID)
				{
					pGrid->dwTakePlayerId = KD_BAD_ID;
					pGrid->nTakeTime = 0;
					pGrid->nExpireTime = 0;
					pGrid->nLastUpdateCounter = ++pTong->m_Repertory.m_nUpdateCounter;
				}
			}
		}
	}

    bRetCode = pTong->m_History.RecordKickout(dwPlayerID, dwMemberID);
    LOG_CHECK_ERROR(bRetCode);

    // Send Message
    KinHandler::DoChangePlayerTongNotify(pDstRole->m_nConnIndex, dwMemberID, pDstRole->m_dwTongID, ctrFired, pTong->m_szName);

    SendTongEventNotify(
        pSrcRole->m_nConnIndex, dwPlayerID, tenKickOutSuccess, pDstRole->m_szName
    );

    strncpy(ChangeInfo.szMemberName, pDstRole->m_szName, sizeof(ChangeInfo));
    ChangeInfo.szMemberName[sizeof(ChangeInfo.szMemberName) - 1] = '\0';

    KinHandler::DoTongBroadcastMessage(pTong, tbmMemberFired, dwPlayerID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));
EXIT0:
    return;
}

void KTongManager::ProcessModifyTongInfoRequest(DWORD dwPlayerID, int nInfoType, BYTE* pbyData, size_t uDataLen)
{
    BOOL            bRetCode    = false;
    KRole*          pRole       = NULL;
    KTong*          pTong       = NULL;
    KTongMember*    pMember     = NULL;
	char			szBuf[TONG_RULES_LEN] = {0};

    assert(pbyData);
    
    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);

    pTong = GetTong(pRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    pMember = pTong->GetMember(dwPlayerID);
    LOG_PROCESS_ERROR(pMember);

    switch (nInfoType)
    {
    //case tmrTongName:
    //    LOG_PROCESS_ERROR(uDataLen >= 4 && uDataLen <= sizeof(pTong->m_szName));

    //    // 只有帮主才能修改帮会名称
    //    if (dwPlayerID != pTong->m_dwMaster)
    //    {
    //        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyTongNameNoPermissionError);
    //        goto EXIT0;
    //    }

    //    strncpy(pTong->m_szName, (char*)pbyData, sizeof(pTong->m_szName));
    //    pTong->m_szName[sizeof(pTong->m_szName) - 1] = '\0';
    //    pTong->m_nLastUpdateFrameTable[tdtBaseInfo] = g_pGameCenter->m_nWorkLoop;

    //    SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyTongNameSuccess);

    //    break;

    case tmrAnnouncement:
        LOG_PROCESS_ERROR(uDataLen <= sizeof(pTong->m_szAnnouncement));

        bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totModifyAnnouncement);

        if (!bRetCode)
        {
            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyAnnouncementNoPermissionError);
            goto EXIT0;
        }

		strncpy(szBuf, (char*)pbyData, sizeof(szBuf));
		szBuf[sizeof(szBuf) - 1] = '\0';
//		if (g_pBadwordFilter->HasBadword(szBuf))
//		{
//			SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyAnnouncementBadWordsError);
//			goto EXIT0;
//		}

        strncpy(pTong->m_szAnnouncement, (char*)pbyData, sizeof(pTong->m_szAnnouncement));
        pTong->m_szAnnouncement[sizeof(pTong->m_szAnnouncement) - 1] = '\0';
        pTong->m_nLastUpdateFrameTable[tdtAnnouncement] = g_pGameCenter->m_nWorkLoop;

        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyAnnouncementSuccess);

        break;

    case tmrOnlineMessage:
        LOG_PROCESS_ERROR(uDataLen <= sizeof(pTong->m_szOnlineMessage));

        bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totModifyOnlineMessage);

        if (!bRetCode)
        {
            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyOnlineMessageNoPermissionError);
            goto EXIT0;
        }

		strncpy(szBuf, (char*)pbyData, sizeof(szBuf));
		szBuf[sizeof(szBuf) - 1] = '\0';
//		if (g_pBadwordFilter->HasBadword(szBuf))
//		{
//			SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyOnlineMessageBadWordsError);
//			goto EXIT0;
//		}

        strncpy(pTong->m_szOnlineMessage, (char*)pbyData, sizeof(pTong->m_szOnlineMessage));
        pTong->m_szOnlineMessage[sizeof(pTong->m_szOnlineMessage) - 1] = '\0';
        pTong->m_nLastUpdateFrameTable[tdtOnlineMessage] = g_pGameCenter->m_nWorkLoop;
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyOnlineMessageSuccess);

        break;

    case tmrIntroduction:
        LOG_PROCESS_ERROR(uDataLen <= sizeof(pTong->m_szIntroduction));

        bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totModifyIntroduction);

        if (!bRetCode)
        {
            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyIntroductionNoPermissionError);
            goto EXIT0;
        }

		strncpy(szBuf, (char*)pbyData, sizeof(szBuf));
		szBuf[sizeof(szBuf) - 1] = '\0';
//		if (g_pBadwordFilter->HasBadword(szBuf))
//		{
//			SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyIntroductionBadWordsError);
//			goto EXIT0;
//		}

        strncpy(pTong->m_szIntroduction, (char*)pbyData, sizeof(pTong->m_szIntroduction));
        pTong->m_szIntroduction[sizeof(pTong->m_szIntroduction) - 1] = '\0';
        pTong->m_nLastUpdateFrameTable[tdtIntroduction] = g_pGameCenter->m_nWorkLoop;
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyIntroductionSuccess);

        break;

    case tmrRules:
        LOG_PROCESS_ERROR(uDataLen <= sizeof(pTong->m_szRules));

        bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totModifyRules);
        
        if (!bRetCode)
        {
            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyRulesNoPermissionError);
            goto EXIT0;
        }

		strncpy(szBuf, (char*)pbyData, sizeof(szBuf));
		szBuf[sizeof(szBuf) - 1] = '\0';
//		if (g_pBadwordFilter->HasBadword(szBuf))
//		{
//			SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyRulesBadWordsError);
//			goto EXIT0;
//		}

        strncpy(pTong->m_szRules, (char*)pbyData, sizeof(pTong->m_szRules));
        pTong->m_szRules[sizeof(pTong->m_szRules) - 1] = '\0';
        pTong->m_nLastUpdateFrameTable[tdtRules] = g_pGameCenter->m_nWorkLoop;
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyRulesSuccess);

        break;
    case tmrMemorabilia:

        bRetCode = pTong->CanBaseOperate(pMember->nGroupID, totModifyMemorabilia);
        // Send Message to player
        LOG_PROCESS_ERROR(bRetCode);

        LOG_PROCESS_ERROR(uDataLen > sizeof(KTONG_MODIFY_MEMORABILIA_INFO));
        {
            KTONG_MODIFY_MEMORABILIA_INFO* pInfo = (KTONG_MODIFY_MEMORABILIA_INFO*)pbyData;
            LOG_PROCESS_ERROR(pInfo->byOperationType <= mmtDelete);

            bRetCode = ProcessModifyMemorabiliarequest((KTONG_MODIFY_MEMORABILIA_TYPE)pInfo->byOperationType, pInfo, uDataLen, pTong);
            
            if(!bRetCode)
            {
                SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyMemorabiliaError);
                goto EXIT0;
            }

            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyMemorabiliaSuccess);


            pTong->m_nLastUpdateFrameTable[tdtMemorabilia] = g_pGameCenter->m_nWorkLoop;
        }
        break;

    default:
        QLogPrintf(LOG_DEBUG, "[KTongManager] ProcessModifyTongTextInfoRequest unrecognized Text Info Type!\n");
        goto EXIT0;
    }

    pTong->m_bUpdateFlag = true;

EXIT0:
    return;
}

BOOL KTongManager::SetApplyerState(DWORD dwOperator, DWORD dwApplyerId, DWORD dwTongId, BOOL bAllow)
{
	KTong* pTong = NULL;
	KRole* pRole = NULL;
	KTongMember* pOperator = NULL;
	std::vector<DWORD>::iterator it;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwApplyerId);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(dwTongId);
	LOG_PROCESS_ERROR(pTong);

	pOperator = pTong->GetMember(dwOperator);
	LOG_PROCESS_ERROR(pOperator);

	PROCESS_ERROR(pTong->CanBaseOperate(pOperator->nGroupID, totAllowOrDenyApplyer));

	if (bAllow)
	{
		if (pRole->m_dwTongID != 0)
		{
            SendTongEventNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenApplyerJoinedOtherKin, pRole->m_dwTongID);
		}
		else
		{
			AddRoleToTong(pRole, pTong);
			SendTongEventNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenApplyBeAllowed, pTong->m_dwID);
		}
        
	}

	// 移除申请者
	pTong->m_nLastUpdateFrameTable[tdtApplyersInfo]++;
	for (it = pTong->m_ApplyJoinTongPlayers.begin(); it != pTong->m_ApplyJoinTongPlayers.end(); ++it)
	{
		if (*it == dwApplyerId)
		{
			pTong->m_ApplyJoinTongPlayers.erase(it);
			break;
		}
	}
    KinHandler::DoSyncTongApplyJoinList(pTong->m_dwID);

EXIT0:
	return TRUE;
}

BOOL KTongManager::ProcessModifyMemorabiliarequest(
        KTONG_MODIFY_MEMORABILIA_TYPE eType, 
        KTONG_MODIFY_MEMORABILIA_INFO* pInfo, 
        size_t uDataLen, 
        KTong* pTong
)
{
    BOOL bResult = false;

    switch (eType)
    {
    case mmtAdd:
        LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_ADD_MEMORABILIA));
        PROCESS_ERROR(pTong->m_Memorabilia.size() < MAX_TONG_MEMORABILIA_COUNT);
        {
            KTONG_ADD_MEMORABILIA*  pAdd = (KTONG_ADD_MEMORABILIA*)pInfo;
            KTongIncidentRecord     Record;
            Record.dwID = ++(pTong->m_dwMemorabiliaCount);
            Record.nTime = pAdd->nTime;
            strncpy(Record.szDescription, pAdd->szDescription, sizeof(Record.szDescription));
            Record.szDescription[sizeof(Record.szDescription) - 1] = '\0';

            pTong->m_Memorabilia.push_back(Record);
        }

        break;
    case mmtModify:
        LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_MODIFY_MEMORABILIA));
        {
            KTONG_MODIFY_MEMORABILIA*   pModify = (KTONG_MODIFY_MEMORABILIA*)pInfo;
            BOOL                        bFound  = false;
          
            for (KTong::KINCIDENT_TABLE::iterator it = pTong->m_Memorabilia.begin(); it != pTong->m_Memorabilia.end(); ++it)
            {
                if (it->dwID == pModify->dwID)
                {
                    it->nTime = pModify->nTime;
                    strncpy(it->szDescription, pModify->szDescription, sizeof(it->szDescription));
                    it->szDescription[sizeof(it->szDescription) - 1] = '\0';
                    bFound = true;
                    break;
                }
            }
            // 该记录已不存在的情况
            PROCESS_ERROR(bFound);
        }

        break;
    case mmtDelete:
        LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_DELETE_MEMORABILIA));
        {
            KTONG_DELETE_MEMORABILIA*   pDelete = (KTONG_DELETE_MEMORABILIA*)pInfo;
            BOOL                        bFound  = false;
            for (KTong::KINCIDENT_TABLE::iterator it = pTong->m_Memorabilia.begin(); it != pTong->m_Memorabilia.end(); ++it)
            {
                if (it->dwID == pDelete->dwID)
                {
                    pTong->m_Memorabilia.erase(it);
                    bFound = true;
                    break;
                }
            }
            // 该记录已不存在的情况
            PROCESS_ERROR(bFound);
        }

        break;
    }

    bResult = true;
EXIT0:
    return bResult;
}

void KTongManager::ProcessModifyTongSchemaRequest(DWORD dwPlayerID, int nGroupIndex, int nModifyType, BYTE* pbyData, size_t uDataLen)
{
    BOOL            bRetCode    = false;
    KRole*          pRole       = NULL;
    KTong*          pTong       = NULL;
    KTongGroup*     pGroup      = NULL;
    KTongMember*    pMember     = NULL;

    assert(pbyData);
    LOG_PROCESS_ERROR(nGroupIndex >= 0 && nGroupIndex < MAX_TONG_GROUP);
    
    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);

    pTong = GetTong(pRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    pMember = pTong->GetMember(dwPlayerID);
    LOG_PROCESS_ERROR(pMember);

    pGroup = &(pTong->m_Schema.Data[nGroupIndex]);
    assert(pGroup);

    switch (nModifyType)
    {
    case tmsGroupName:

        LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_MODIFY_GROUP_NAME));

        {
            KTONG_MODIFY_GROUP_NAME*            pModify    = (KTONG_MODIFY_GROUP_NAME*)pbyData;
            KTONG_CHANGE_GROUP_NAME_BROADCAST   ChangeInfo;

            bRetCode = pTong->CanAdvanceOperate(pMember->nGroupID, nGroupIndex, totModifyGroupName);
            if (!bRetCode)
            {
                SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyGroupNameNoPermissionError);
                goto EXIT0;
            }

			char szBuf[_NAME_LEN] = {0};
			strncpy(szBuf, pModify->szGroupName, sizeof(szBuf));
			szBuf[sizeof(szBuf) - 1] = '\0';
//			if (g_pBadwordFilter->HasBadword(szBuf)){
//				SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyGroupNameBadWordsError);
//				goto EXIT0;
//			}

            strncpy(ChangeInfo.szOldGroupName, pGroup->szName, sizeof(ChangeInfo.szOldGroupName));
            ChangeInfo.szOldGroupName[sizeof(ChangeInfo.szOldGroupName) - 1] = '\0';
            strncpy(ChangeInfo.szNewGroupName, pModify->szGroupName, sizeof(ChangeInfo.szNewGroupName));
            ChangeInfo.szNewGroupName[sizeof(ChangeInfo.szNewGroupName) - 1] = '\0';

            strncpy(pGroup->szName, pModify->szGroupName, sizeof(pGroup->szName));
            pGroup->szName[sizeof(pGroup->szName) - 1] = '\0';

            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyGroupNameSuccess);
            KinHandler::DoTongBroadcastMessage(pTong, tbmGroupNameChange, KD_BAD_ID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));
        }
        break;

    case tmsBaseOperation:

        LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_MODIFY_BASE_OPERATION_MASK));

        {
            KTONG_MODIFY_BASE_OPERATION_MASK*   pModify = (KTONG_MODIFY_BASE_OPERATION_MASK*)pbyData;
            KTONG_CHANGE_GROUP_RIGHT_BROADCAST  ChangeInfo;
            
            LOG_PROCESS_ERROR(pModify->byOffset < TONG_OPERATION_NUM);

            bRetCode = pTong->CanBaseGrant(pMember->nGroupID, nGroupIndex, (int)pModify->byOffset);
            if (!bRetCode)
            {
                SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyBaseOperationMaskNoPermissionError);
                goto EXIT0;
            }

            //if (pModify->bValue)
            //{
            //    pGroup->byBaseOperationMask |= (1 << (pModify->byOffset));
            //}
            //else
            //{
            //    pGroup->byBaseOperationMask &= ~(1 << (pModify->byOffset));
            //}

            bRetCode = pTong->m_History.RecordChangeGroupBaseRight(
                dwPlayerID, nGroupIndex, pModify->byOffset, pModify->bValue
            );
            LOG_CHECK_ERROR(bRetCode);

            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyBaseOperationMaskSuccess);

            strncpy(ChangeInfo.szGroupName, pGroup->szName, sizeof(ChangeInfo.szGroupName));
            ChangeInfo.szGroupName[sizeof(ChangeInfo.szGroupName) - 1] = '\0';
            KinHandler::DoTongBroadcastMessage(pTong, tbmGroupRightChange, KD_BAD_ID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));
        }

        break;

    case tmsAdvanceOperation:

        LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_MODIFY_ADVANCE_OPERATION_MASK));

        {
            KTONG_MODIFY_ADVANCE_OPERATION_MASK*    pModify = (KTONG_MODIFY_ADVANCE_OPERATION_MASK*)pbyData;
            KTONG_CHANGE_GROUP_RIGHT_BROADCAST      ChangeInfo;
            
            LOG_PROCESS_ERROR(pModify->byIndex < MAX_TONG_GROUP);
            LOG_PROCESS_ERROR(pModify->byOffset < TONG_OPERATION_NUM);

            bRetCode = pTong->CanAdvanceGrant(pMember->nGroupID, nGroupIndex, (int)pModify->byIndex, (int)pModify->byOffset);
            if (!bRetCode)
            {
                SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyAdvanceOperationMaskNoPermissionError);
                goto EXIT0;
            }

            if (pModify->bValue)
            {
                pGroup->byAdvanceOperationMask[pModify->byIndex] |= (1 << (pModify->byOffset));
            }
            else
            {
                pGroup->byAdvanceOperationMask[pModify->byIndex] &= ~(1 << (pModify->byOffset));
            }

            bRetCode = pTong->m_History.RecordChangeGroupAdvanceRight(
                dwPlayerID, nGroupIndex, pModify->byIndex, pModify->byOffset, pModify->bValue
            );
            LOG_CHECK_ERROR(bRetCode);

            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyAdvanceOperationMaskSuccess);
            strncpy(ChangeInfo.szGroupName, pGroup->szName, sizeof(ChangeInfo.szGroupName));
            ChangeInfo.szGroupName[sizeof(ChangeInfo.szGroupName) - 1] = '\0';
            KinHandler::DoTongBroadcastMessage(pTong, tbmGroupRightChange, KD_BAD_ID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));
        }
        break;

    case tmsGroupWage:

        PROCESS_ERROR(uDataLen == sizeof(int));

        {
            int                                 nOldWage    = 0;
            int                                 nWage       = 0;
            KTONG_CHANGE_GROUP_WAGE_BROADCAST   ChangeInfo;

            bRetCode = pTong->CanAdvanceOperate(pMember->nGroupID, nGroupIndex, totModifyWage);
            if (!bRetCode)
            {
                SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyGroupWageNoPermissionError);
                goto EXIT0;
            }

            memcpy(&nWage, pbyData, sizeof(nWage));
            LOG_PROCESS_ERROR(nWage >= 0);

            nOldWage        = pGroup->nWage;
            pGroup->nWage   = nWage;
            
            bRetCode = pTong->m_History.RecordChangeGroupWage(dwPlayerID, nGroupIndex, nOldWage, nWage);
            LOG_CHECK_ERROR(bRetCode);

            SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenModifyGroupWageSuccess);

            ChangeInfo.nWage = nWage;
            strncpy(ChangeInfo.szGroupName, pGroup->szName, sizeof(ChangeInfo.szGroupName));
            ChangeInfo.szGroupName[sizeof(ChangeInfo.szGroupName) - 1] = '\0';
            KinHandler::DoTongBroadcastMessage(pTong, tbmGroupWageChange, KD_BAD_ID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));
        }

        break;

    default:
        QLogPrintf(LOG_DEBUG, "[KTongManager] Process unrecognized Modify Tong Schema Request!\n");
        goto EXIT0;
    }

    pTong->m_nLastUpdateFrameTable[tdtSchemaInfo] = g_pGameCenter->m_nWorkLoop;
    pTong->m_bUpdateFlag = true;

EXIT0:
    return;
}

void KTongManager::ProcessQuitRequest(DWORD dwPlayerID)
{
    BOOL                                bRetCode    = false;
    KRole*                              pRole       = NULL;
    KTong*                              pTong       = NULL;
	KMapInfo*                           pMapInfo    = NULL;
    KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST ChangeInfo;

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    if (pRole->m_dwTongID == KD_BAD_ID)
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenQuitNotInTongError);
        goto EXIT0;
    }
	// 检查目标是否在活动地图中
	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRole->m_dwMapID);
	if (pMapInfo->m_nType == emtBattleField)
	{
		goto EXIT0;
	}

    pTong =  GetTong(pRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    if (dwPlayerID == pTong->m_dwMaster)
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenQuitIsMasterError);
        goto EXIT0;
    }

	// 离开家族时要删除掉这个玩家放入的所有装备，只删除这个玩家放入的共享装备
	for (int i = 0; i < TONG_REPERTORY_PAGE_NUM; ++i)
	{
		if (i >= 5) // 大于5以上为非绑定区，不作删除处理
			continue;

		KTONG_REPERTORY_PAGE* pPage = &pTong->m_Repertory.m_Page[i];
		if (pPage && pPage->bEnable)
		{
			for (int j = 0; j < TONG_REPERTORY_PAGE_CAPACITY; ++j)
			{
				KTONG_REPERTORY_GRID* pGrid = &pPage->Grids[j];
				if (pGrid->dwPutPlayerId == dwPlayerID)
				{
					if (pGrid->dwTakePlayerId == KD_BAD_ID ||
						(pGrid->dwTakePlayerId != KD_BAD_ID && pGrid->nExpireTime < g_pGameCenter->m_nTimeNow)) 
					{
						pTong->m_Repertory.LockGrid(i, j);
						pTong->m_Repertory.FreeItem(i, j);
					}
					else {
						pGrid->bRemoveOnExpire = TRUE;
						pGrid->nLastUpdateCounter = ++pTong->m_Repertory.m_nUpdateCounter;
					}
				}
				if (pGrid->dwTakePlayerId == dwPlayerID)
				{
					pGrid->dwTakePlayerId = KD_BAD_ID;
					pGrid->nTakeTime = 0;
					pGrid->nExpireTime = 0;
					pGrid->nLastUpdateCounter = ++pTong->m_Repertory.m_nUpdateCounter;
				}
			}
		}
	}

    bRetCode = pTong->RemoveMember(pRole);
    LOG_PROCESS_ERROR(bRetCode);

	OnTongMemberChanged(pTong->m_dwID);

    bRetCode = pTong->m_History.RecordQuit(dwPlayerID);
    LOG_CHECK_ERROR(bRetCode);

    assert(pRole->m_dwTongID == KD_BAD_ID);
    KinHandler::DoChangePlayerTongNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, pRole->m_dwTongID, ctrQuit, pTong->m_szName);

    strncpy(ChangeInfo.szMemberName, pRole->m_szName, sizeof(ChangeInfo.szMemberName));
    ChangeInfo.szMemberName[sizeof(ChangeInfo.szMemberName) - 1] = '\0';
    KinHandler::DoTongBroadcastMessage(pTong, tbmMemberQuit, KD_BAD_ID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));
EXIT0:
    return;
}

void KTongManager::ProcessChangeMemberGroupRequest(DWORD dwPlayerID, DWORD dwTargetID, int nTargetGroup)
{
    BOOL                                bRetCode            = false;
    BOOL                                bHavePermissoion    = false;
    KRole*                              pRole               = NULL;
    KRole*                              pTargetRole         = NULL;
    KTong*                              pTong               = NULL;
    KTongMember*                        pMember             = NULL;
    KTongMember*                        pTargetMember       = NULL;
    int                                 nCurrentGroup       = 0;
    KTONG_MEMBER_CHANGE_GROUP_BROADCAST ChangeInfo;

    LOG_PROCESS_ERROR(nTargetGroup >= 0 && nTargetGroup < MAX_TONG_GROUP);

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    pTargetRole = g_pGameCenter->m_RoleManager.GetRole(dwTargetID);
    LOG_PROCESS_ERROR(pTargetRole);

    PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);
    
    pTong = GetTong(pRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    PROCESS_ERROR(pTong->m_dwMaster != dwTargetID);
    LOG_PROCESS_ERROR(pTong->m_Schema.Data[nTargetGroup].bEnable);

    pMember = pTong->GetMember(dwPlayerID);
    LOG_PROCESS_ERROR(pMember);

    pTargetMember = pTong->GetMember(dwTargetID);
    LOG_PROCESS_ERROR(pTargetMember);

    nCurrentGroup = pTargetMember->nGroupID;
    LOG_PROCESS_ERROR(nCurrentGroup >= 0 && nCurrentGroup < MAX_TONG_GROUP);
    PROCESS_ERROR(nCurrentGroup != nTargetGroup);

    bRetCode = pTong->CanAdvanceOperate(pMember->nGroupID, nCurrentGroup, totMoveFromGroup);
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = pTong->CanAdvanceOperate(pMember->nGroupID, nTargetGroup, totAddToGroup);
    LOG_PROCESS_ERROR(bRetCode);

    bHavePermissoion = true;

    pTargetMember->nGroupID = nTargetGroup;
    pTargetMember->nLastUpdateFrame = g_pGameCenter->m_nWorkLoop;

    bRetCode = pTong->m_History.RecordChangeMemberGroup(dwPlayerID, dwTargetID, nCurrentGroup, nTargetGroup);
    LOG_CHECK_ERROR(bRetCode);

    strncpy(ChangeInfo.szMemberName, pTargetRole->m_szName, sizeof(pTargetRole->m_szName));
    ChangeInfo.szMemberName[sizeof(ChangeInfo.szMemberName) - 1] = '\0';
    strncpy(ChangeInfo.szOldGroupName, pTong->m_Schema.Data[nCurrentGroup].szName, sizeof(ChangeInfo.szOldGroupName));
    ChangeInfo.szOldGroupName[sizeof(ChangeInfo.szOldGroupName) - 1] = '\0';
    strncpy(ChangeInfo.szNewGroupName, pTong->m_Schema.Data[nTargetGroup].szName, sizeof(ChangeInfo.szNewGroupName));
    ChangeInfo.szNewGroupName[sizeof(ChangeInfo.szNewGroupName) - 1] = '\0';

    SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenChangeMemberGroupSuccess);
    KinHandler::DoTongBroadcastMessage(pTong, tbmMemberChangeGroup, KD_BAD_ID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));

    pTong->m_bUpdateFlag = true;

EXIT0:
    if (!bHavePermissoion && pRole)
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenChangeMemberGroupError);
    }

    return;
}

void KTongManager::ProcessChangeMasterRequest(DWORD dwPlayerID, DWORD dwNewMasterID, BOOL bImpeached)
{
    KTong*                          pTong       = NULL;
    KRole*                          pRole       = NULL;
    KRole*                          pTargetRole = NULL;
    KTongMember*                    pMember     = NULL;
    KTongMember*                    pNewMaster  = NULL;
	KMapInfo*                       pMapInfo    = NULL;
    KTONG_CHANGE_MASTER_BROADCAST   ChangeInfo;
	INT nLasuGroupID = 0;
    
    LOG_PROCESS_ERROR(dwPlayerID != dwNewMasterID);

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);
    LOG_PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);

    pTargetRole = g_pGameCenter->m_RoleManager.GetRole(dwNewMasterID);
    LOG_PROCESS_ERROR(pTargetRole);
    LOG_PROCESS_ERROR(pTargetRole->m_dwTongID == pRole->m_dwTongID);
    
    pTong = GetTong(pRole->m_dwTongID);
    if (dwPlayerID != pTong->m_dwMaster)
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenChangeMasterError);
        goto EXIT0;
    }
	// 检查目标是否在活动地图中
//	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRole->m_dwMapID);
//	LOG_PROCESS_ERROR(pMapInfo);
//	if (pMapInfo->m_nType == emtBattleField)
//	{
//		if (bImpeached)
//		{
//			SendTongEventNotify(
//				pTargetRole->m_nConnIndex, dwPlayerID, tenTargetInActivity, pTargetRole->m_szName
//				);
//		}
//		goto EXIT0;
//	}

    pMember = pTong->GetMember(dwPlayerID);
    LOG_PROCESS_ERROR(pMember);

    pNewMaster = pTong->GetMember(dwNewMasterID);
    LOG_PROCESS_ERROR(pNewMaster);

    strncpy(ChangeInfo.szOldMasterName, pRole->m_szName, sizeof(ChangeInfo.szOldMasterName));
    ChangeInfo.szOldMasterName[sizeof(ChangeInfo.szOldMasterName) - 1] = '\0';
    strncpy(ChangeInfo.szNewMasterName, pTargetRole->m_szName,sizeof(ChangeInfo.szNewMasterName));
    ChangeInfo.szNewMasterName[sizeof(ChangeInfo.szNewMasterName) - 1] = '\0';

	nLasuGroupID        = pNewMaster->nGroupID;
    pNewMaster->nGroupID    = pTong->m_Schema.nMasterGroup;
    pMember->nGroupID       = pTong->m_Schema.nDefaultGroup;
    pTong->m_dwMaster       = dwNewMasterID;

    pMember->nLastUpdateFrame       = g_pGameCenter->m_nWorkLoop;
    pNewMaster->nLastUpdateFrame    = g_pGameCenter->m_nWorkLoop;

    pTong->m_nLastUpdateFrameTable[tdtBaseInfo] = g_pGameCenter->m_nWorkLoop;
	
	LOG_CHECK_ERROR(pTong->m_History.RecordChangeMemberGroup(dwPlayerID, dwNewMasterID, nLasuGroupID, pTong->m_Schema.nMasterGroup));

    SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenChangeMasterSuccess, bImpeached);
    KinHandler::DoTongBroadcastMessage(pTong, tbmMasterChange, KD_BAD_ID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));

    pTong->m_bUpdateFlag = true;

	QLogPrintf(LOG_INFO,"kin ProcessChangeMasterRequest");

EXIT0:
    return;
}

void KTongManager::ProcessChangeTongMemberRemarkRequest(DWORD dwPlayerID, DWORD dwTargetID, char szRemark[])
{
    BOOL            bRetCode        = false;
    KTong*          pTong           = NULL;
    KRole*          pRole           = NULL;
    KTongMember*    pCurMember      = NULL;
    KTongMember*    pTargetMember   = NULL;

    assert(szRemark);

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
    LOG_PROCESS_ERROR(pRole);
    LOG_PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);

    pTong = GetTong(pRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

    pCurMember = pTong->GetMember(dwPlayerID);
    LOG_PROCESS_ERROR(pCurMember);

    pTargetMember = pTong->GetMember(dwTargetID);
    LOG_PROCESS_ERROR(pTargetMember);

    bRetCode = pTong->CanAdvanceOperate(pCurMember->nGroupID, pTargetMember->nGroupID, totModifyMemberRemark);
    if (!bRetCode)
    {
        SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenChangeMemberRemarkError);
        goto EXIT0;
    }

    strncpy(pTargetMember->szRemark, szRemark, sizeof(pTargetMember->szRemark));
    pTargetMember->nLastUpdateFrame = g_pGameCenter->m_nWorkLoop;

    SendTongEventNotify(pRole->m_nConnIndex, dwPlayerID, tenChangeMemberRemarkSuccess);

    pTong->m_bUpdateFlag = true;

EXIT0:
    return;
}

void KTongManager::ProcessSaveMoneyInTong(KRole* pRole, DWORD dwPealNum, DWORD dwJadeiteNum, DWORD dwAgateNum, DWORD dwDiamondNum, DWORD dwContributionAdd)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    int     nSilver     = 0;
	DWORD   dwParams[4];
    KTong*  pTong       = NULL;
	KTongMember* pMember = NULL;
    BYTE    byBuffer[32];

    assert(pRole);

    PROCESS_ERROR(pRole->m_dwTongID != KD_BAD_ID);

    pTong = GetTong(pRole->m_dwTongID);
    LOG_PROCESS_ERROR(pTong);

	LOG_PROCESS_ERROR(dwPealNum >= 1 || dwJadeiteNum >= 1 || dwAgateNum >= 1 || dwDiamondNum >= 1);

    bRetCode = pTong->AddFund(dwPealNum, dwJadeiteNum, dwAgateNum, dwDiamondNum);
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = pTong->m_History.RecordSaveFund(pRole->m_dwPlayerID, dwPealNum, dwJadeiteNum, dwAgateNum, dwDiamondNum);
    LOG_CHECK_ERROR(bRetCode);

	//pTong->SyncRecords();

	pMember = pTong->GetMember(pRole->m_dwPlayerID);
	LOG_CHECK_ERROR(pMember);

    pMember->nSalary += dwContributionAdd;
	pMember->nDailyContribution += dwContributionAdd;

    SendTongEventNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenSaveMoneySuccess, pTong->m_nFund);

	pTong->m_bUpdateTongRankFlag = true;
    
    bResult = true;
EXIT0:
    if (!bResult)
    {
        byBuffer[0] = '\0'; // 信件正文部分至少以1个'\0'结尾
        g_pGameCenter->m_MailManager.PostRequestSendMail(
            0, pRole->m_dwPlayerID, 0, eMailType_System, 
            g_pGameCenter->m_Settings.m_szSystemMailSender, 
            g_pGameCenter->m_Settings.m_szTongReturnMoneyMailTitle,
            0,0,0,
            byBuffer, 1,
            NULL, 0, 0
        );
    }
    return;
}

void KTongManager::ProcessRenameConflictTong(
    int nConnIndex, DWORD dwPlayerID, DWORD dwTongID, const char cszNewName[]
)
{
    BOOL        bRetCode        = false;
    KTong*      pTong           = NULL;
    KTong*      pConflictTong   = NULL;
    size_t      uNewNameLen     = 0;

    LOG_PROCESS_ERROR(dwPlayerID != KD_BAD_ID);
    LOG_PROCESS_ERROR(dwTongID != KD_BAD_ID);
    LOG_PROCESS_ERROR(cszNewName);

    uNewNameLen = strlen(cszNewName);
    LOG_PROCESS_ERROR(uNewNameLen < _NAME_LEN);

    pTong = g_pGameCenter->m_TongManager.GetTong(dwTongID);
    LOG_PROCESS_ERROR(pTong); 

    if (pTong->m_dwMaster != dwPlayerID)
    {
        SendTongEventNotify(nConnIndex, dwPlayerID, tenRenameNoRightError);
        goto EXIT0;
    }

    //bRetCode = g_pGameCenter->m_piTextFilter->Check(pTong->m_szName);
    //if (bRetCode)
    //{
    //    SendTongEventNotify(nConnIndex, dwPlayerID, tenRenameUnnecessaryError);
    //    goto EXIT0;
    //}

    pConflictTong = g_pGameCenter->m_TongManager.GetTongByName(cszNewName);
    if (pConflictTong != NULL)
    {
        SendTongEventNotify(nConnIndex, dwPlayerID, tenRenameConflictError);
        goto EXIT0;
    }

    //bRetCode = g_pGameCenter->m_piTextFilter->Check(cszNewName);
    //if (!bRetCode)
    //{
    //    SendTongEventNotify(nConnIndex, dwPlayerID, tenRenameIllegalError);
    //    goto EXIT0;
    //}

    strncpy(pTong->m_szName, cszNewName, sizeof(pTong->m_szName));
    pTong->m_szName[sizeof(pTong->m_szName) - 1] = '\0';
    pTong->m_nLastUpdateFrameTable[tdtBaseInfo] = g_pGameCenter->m_nWorkLoop;

    pTong->m_bUpdateFlag = true;

    SendTongEventNotify(nConnIndex, dwPlayerID, tenRenameSuccess);

EXIT0:
    return;
}

void KTongManager::ProcessGetFreeJoinTongList( DWORD dwPlayerID, INT nLastUpdateCounter )
{
	BOOL					bRetCode	= FALSE;
	KRole*					pRole		= NULL;
	KRole*					pMaster		= NULL;
	INT						nTongNum	= 0;
	DWORD					dwTongID	= 0;
	KTong*					pTong		= NULL;
	KTONG_LIST_ITEM*		pListItem	= NULL;
	BYTE					byBuffer[sizeof(KTONG_LIST_ITEM) * MAX_TONG_LIST_ITEM_NUM];
	BYTE*					pByOffset	= &byBuffer[0];
	INT						nDataLen	= 0;
	KTONG_STACK::iterator	it;

	LOG_PROCESS_ERROR(dwPlayerID != KD_BAD_ID);
	LOG_PROCESS_ERROR(nLastUpdateCounter <= m_TongStackLastUpdateCounter);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	if (nLastUpdateCounter < m_TongStackLastUpdateCounter)
	{
		for (it = m_TongStack.begin(); it != m_TongStack.end(); ++it)
		{
			dwTongID = *it;
			LOG_PROCESS_ERROR(dwTongID);

			pTong = g_pGameCenter->m_TongManager.GetTong(dwTongID);
			LOG_PROCESS_ERROR(pTong);

			if (pTong->m_bFreeJoin && (INT)pTong->m_Roster.size() < pTong->m_nMaxMemberCount) // 开启自由招募的帮派才显示在列表
			{
				++nTongNum;

				pListItem = (KTONG_LIST_ITEM *)pByOffset;

				pListItem->dwTongID			= pTong->m_dwID;
				pListItem->byLevel			= pTong->m_nLevel;
				pListItem->byCamp           = pTong->m_byCamp;
				pListItem->byMaxMemberNum	= pTong->m_nMaxMemberCount;
				pListItem->byMemberNum		= pTong->m_Roster.size();

				memset(pListItem->szTongName, 0, sizeof(pListItem->szTongName));
				memcpy(pListItem->szTongName, pTong->m_szName, sizeof(pTong->m_szName));
				
				LOG_PROCESS_ERROR(pTong->m_dwMaster);
				pMaster = g_pGameCenter->m_RoleManager.GetRole(pTong->m_dwMaster);
				LOG_PROCESS_ERROR(pMaster);

				memset(pListItem->szMasterName, 0, sizeof(pListItem->szMasterName));
				memcpy(pListItem->szMasterName, pMaster->m_szName, sizeof(pMaster->m_szName));

				pByOffset += sizeof(KTONG_LIST_ITEM);
				nDataLen += sizeof(KTONG_LIST_ITEM);

				if (nTongNum >= MAX_TONG_LIST_ITEM_NUM)
					break;
			}
		}
	}

	bRetCode = KinHandler::DoGetFreeJoinTongListRespond(dwPlayerID, nTongNum, byBuffer, nDataLen, m_TongStackLastUpdateCounter);
	LOG_PROCESS_ERROR(bRetCode);
	
EXIT0:
	return;
}

BOOL KTongManager::AddRoleToTong(KRole* pRole, KTong* pTong)
{
	KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST     ChangeInfo;

	LOG_CHECK_ERROR(pRole);
	LOG_CHECK_ERROR(pTong);

	BOOL bRetCode = pTong->AddNewMember(pRole, pTong->m_Schema.nDefaultGroup);
	LOG_CHECK_ERROR(bRetCode);

	OnTongMemberChanged(pTong->m_dwID);

	bRetCode = pTong->m_History.RecordJoin(pRole->m_dwPlayerID);
	LOG_CHECK_ERROR(bRetCode);

	KinHandler::DoChangePlayerTongNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, pTong->m_dwID, ctrJoin, pTong->m_szName);

	strncpy(ChangeInfo.szMemberName, pRole->m_szName, sizeof(ChangeInfo));
	ChangeInfo.szMemberName[sizeof(ChangeInfo.szMemberName) - 1] = '\0';

	KinHandler::DoTongBroadcastMessage(pTong, tbmMemberJoin, 0, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));

	bRetCode = OnJoinTong(pRole->m_dwPlayerID, pTong->m_dwID);
	LOG_CHECK_ERROR(bRetCode);

	return TRUE;
}

void KTongManager::ProcessApplyJoinTongRequest( DWORD dwPlayerID, DWORD dwTongID )
{
	BOOL									bRetCode	= FALSE;
	KTong*									pTong		= NULL;
	KRole*									pRole		= NULL;
	KMapInfo*                               pMapInfo    = NULL;
	//KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST     ChangeInfo;
	
	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pRole->m_dwTongID == KD_BAD_ID);

	pTong = GetTong(dwTongID);
	LOG_PROCESS_ERROR(pTong);

	if (pTong->m_bFreeJoin)
	{
		// 检查目标是否在活动地图中
		pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRole->m_dwMapID);
		if (pMapInfo->m_nType == emtBattleField)
		{
			goto EXIT0;
		}

		AddRoleToTong(pRole, pTong);
		/*bRetCode = pTong->AddNewMember(pRole, pTong->m_Schema.nDefaultGroup);
		LOG_PROCESS_ERROR(bRetCode);

		OnTongMemberChanged(pTong->m_dwID);

		bRetCode = pTong->m_History.RecordJoin(dwPlayerID);
		LOG_CHECK_ERROR(bRetCode);

		KinHandler::DoChangePlayerTongNotify(pRole->m_nConnIndex, dwPlayerID, dwTongID, ctrJoin, pTong->m_szName);

		strncpy(ChangeInfo.szMemberName, pRole->m_szName, sizeof(ChangeInfo));
		ChangeInfo.szMemberName[sizeof(ChangeInfo.szMemberName) - 1] = '\0';

		KinHandler::DoTongBroadcastMessage(pTong, tbmMemberJoin, dwPlayerID, (BYTE*)&ChangeInfo, sizeof(ChangeInfo));

		bRetCode = OnJoinTong(dwPlayerID, dwTongID);
		LOG_PROCESS_ERROR(bRetCode);*/
	}
	else
	{
		if ((INT)pTong->m_ApplyJoinTongPlayers.size() < TONG_MAX_APPLY_JOIN_PLAYER_NUM)
		{
			std::vector<DWORD>::iterator it_find = std::find(pTong->m_ApplyJoinTongPlayers.begin(), pTong->m_ApplyJoinTongPlayers.end(), pRole->m_dwPlayerID);
			if (it_find == pTong->m_ApplyJoinTongPlayers.end())
			{
				pTong->m_nLastUpdateFrameTable[tdtApplyersInfo]++;
				pTong->m_ApplyJoinTongPlayers.push_back(pRole->m_dwPlayerID);
				KinHandler::DoSyncTongApplyJoinList(pTong->m_dwID);
			}
			else
			{
                SendTongEventNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenAlreadyApplyJoin);
			}
		}
		else
		{
             SendTongEventNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, tenJoinListFullError);
		}
	}

EXIT0:
	return;
}

void KTongManager::ProcessSetFreeJoin( DWORD dwPlayerID, DWORD dwTongID, BOOL bValue )
{
	BOOL									bRetCode	= FALSE;
	KTong*									pTong		= NULL;
	KRole*									pRole		= NULL;
	//KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST     ChangeInfo;
	BYTE									byValue		= 0;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pRole->m_dwTongID == dwTongID);

	pTong = GetTong(dwTongID);
	LOG_PROCESS_ERROR(pTong);
	LOG_PROCESS_ERROR(pTong->m_dwMaster == dwPlayerID);

	pTong->m_bFreeJoin = bValue ? TRUE : FALSE;
	byValue = pTong->m_bFreeJoin;
	m_TongStackLastUpdateCounter = g_pGameCenter->m_nWorkLoop;

	KinHandler::DoTongBroadcastMessage(pTong, tbmTongFreeJoinStateChange, KD_BAD_ID, &byValue, sizeof(byValue));

EXIT0:
	return;
}

void KTongManager::OnGSConnected(INT nConnIndex)
{
	KTONG_TABLE::iterator it;
	for (it = m_TongTable.begin(); it != m_TongTable.end(); ++it)
	{
		ScriptSafe safeScript = g_cScriptManager.GetSafeScript();
		safeScript->CallTableFunction("Kin", "CreateKinActivitiesInGS", 0, "dd", it->first, nConnIndex);
	}
}

void KTongManager::OnPlayerEnterGame(KRole* pRole)
{
    KTong*                                  pTong       = NULL;
    KTongMember*                            pMember     = NULL;

    assert(pRole);
    
    if (pRole->m_dwTongID != KD_BAD_ID)
    {
        KTONG_MEMBER_ONLINE_STATE_CHANGE_BROADCAST Broadcast;
        pTong = GetTong(pRole->m_dwTongID);
        LOG_PROCESS_ERROR(pTong);

        pMember = pTong->GetMember(pRole->m_dwPlayerID);
        LOG_PROCESS_ERROR(pMember);

        pMember->bIsOnline          = true;
        pMember->nLastUpdateFrame   = g_pGameCenter->m_nWorkLoop;

        ScriptSafe safeScript = g_cScriptManager.GetSafeScript();
		safeScript->CallTableFunction("Kin", "OnPlayerLogin", 0, "d", pRole->m_dwPlayerID);

        // 无需存盘
        strncpy(Broadcast.szMemberName, pRole->m_szName, sizeof(Broadcast.szMemberName));
        Broadcast.szMemberName[sizeof(Broadcast.szMemberName) - 1] = '\0';
        KinHandler::DoTongBroadcastMessage(pTong, tbmMemberOnline, pRole->m_dwPlayerID, 
            (BYTE*)&Broadcast, sizeof(Broadcast)
        );
    }
EXIT0:
    return;
}

void KTongManager::OnPlayerLeaveGame(KRole* pRole)
{
    KTong*                                  pTong       = NULL;
    KTongMember*                            pMember     = NULL;
    DWORD                                   dwInviterID = 0;
    KRole*                                  pInviter    = NULL;
    KTONG_JOIN_APPLICATION_TABLE::iterator  it;

    assert(pRole);
    
    if (pRole->m_dwTongID != KD_BAD_ID)
    {
        KTONG_MEMBER_ONLINE_STATE_CHANGE_BROADCAST Broadcast;

        pTong = GetTong(pRole->m_dwTongID);
        LOG_PROCESS_ERROR(pTong);

        pMember = pTong->GetMember(pRole->m_dwPlayerID);
        LOG_PROCESS_ERROR(pMember);

        pMember->nLastOfflineTime   = g_pGameCenter->m_nTimeNow;
        pMember->nLastUpdateFrame   = g_pGameCenter->m_nWorkLoop;

        pTong->m_bUpdateFlag        = true;

        strncpy(Broadcast.szMemberName, pRole->m_szName, sizeof(Broadcast.szMemberName));
        Broadcast.szMemberName[sizeof(Broadcast.szMemberName) - 1] = '\0';
        KinHandler::DoTongBroadcastMessage(pTong, tbmMemberOffline, pRole->m_dwPlayerID, 
            (BYTE*)&Broadcast, sizeof(Broadcast)
        );
    }
    else
    {
        it = m_InviteApplication.find(pRole->m_dwPlayerID);
        PROCESS_ERROR(it != m_InviteApplication.end());

        dwInviterID = it->second.InviterID;
        m_InviteApplication.erase(it++);

        pInviter = g_pGameCenter->m_RoleManager.GetRole(dwInviterID);
        PROCESS_ERROR(pInviter);

        if (pInviter->IsOnline())
        {
            SendTongEventNotify(
                pInviter->m_nConnIndex, pInviter->m_dwPlayerID, tenInviteTargetOfflineError, pRole->m_szName
            );
        }        
    }

EXIT0:
    return;
}

void KTongManager::SaveAll()
{
    for (KTONG_TABLE::iterator it = m_TongTable.begin(); it != m_TongTable.end(); ++it)
    {
        KTong* pTong = &it->second;       
        
        if (!pTong->m_bUpdateFlag)
           continue;

        g_pGameCenter->m_MiscDB.DoSaveTongData(it->first, pTong);
        pTong->m_bUpdateFlag = false;
    }
}

void KTongManager::SendTongEventNotify(int nConnIndex, DWORD dwPlayerID, TONG_EVENT_NOTIFY_CODE eCode)
{
	KinHandler::DoKinMessageNotify(nConnIndex, dwPlayerID, eCode, NULL);
    //g_pGameCenter->m_RemoteScript.Reset();
    //g_pGameCenter->m_RemoteScript.PushNumber(dwPlayerID);
    //g_pGameCenter->m_RemoteScript.PushNumber(eCode);
    //g_pGameCenter->m_RemoteScript.Call(nConnIndex, TONG_EVENT_NOTIFY_FUNCTION);
}

void KTongManager::SendTongEventNotify(int nConnIndex, DWORD dwPlayerID, TONG_EVENT_NOTIFY_CODE eCode, const char cszMsg[])
{
	KinHandler::DoKinMessageNotify(nConnIndex, dwPlayerID, eCode, cszMsg);
    //g_pGameCenter->m_RemoteScript.Reset();
    //g_pGameCenter->m_RemoteScript.PushNumber(dwPlayerID);
    //g_pGameCenter->m_RemoteScript.PushNumber(eCode);
    //g_pGameCenter->m_RemoteScript.PushString(cszMsg);
    //g_pGameCenter->m_RemoteScript.Call(nConnIndex, TONG_EVENT_NOTIFY_FUNCTION);
}

void KTongManager::SendTongEventNotify(int nConnIndex, DWORD dwPlayerID, TONG_EVENT_NOTIFY_CODE eCode, int nParam)
{
	char szMsg[8] = {0};
	memcpy(szMsg, &nParam, sizeof(nParam));
	szMsg[sizeof(nParam)] = 0;
	KinHandler::DoKinMessageNotify(nConnIndex, dwPlayerID, eCode, szMsg, sizeof(nParam));
    //g_pGameCenter->m_RemoteScript.Reset();
    //g_pGameCenter->m_RemoteScript.PushNumber(dwPlayerID);
    //g_pGameCenter->m_RemoteScript.PushNumber(eCode);
    //g_pGameCenter->m_RemoteScript.PushNumber(nParam);
    //g_pGameCenter->m_RemoteScript.Call(nConnIndex, TONG_EVENT_NOTIFY_FUNCTION);
}

//字符串分割函数
std::vector<std::string> split(std::string str,std::string pattern)
{           
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
	    {
		    std::string s=str.substr(i,pos-i);
		    result.push_back(s);
	        i=pos+pattern.size()-1;
	     }
	 }
    return result;
}

BOOL KTongManager::LoadKinActivityConfig()
{
	INT nHeight = 0;
	ITabFile* piTabFile = g_OpenTabFile("/setting/kin/kinactivity.txt");
	LOG_PROCESS_ERROR(piTabFile);

	nHeight = piTabFile->GetHeight();

	for (int nIndex = 2; nIndex < nHeight; nIndex++)
	{
		KinActivity cAct;

		piTabFile->GetInteger(nIndex + 1, "Index", 0, &cAct.nKinActivityIndex);
		piTabFile->GetInteger(nIndex + 1, "Persist", 0, &cAct.nKinActivityPersist);
		
		CHAR szOpenTimesTemp[_MAX_PATH];
		piTabFile->GetString(nIndex + 1, "OpenTimes", "", szOpenTimesTemp, _MAX_PATH);

		std::string szOpenTimes = szOpenTimesTemp;
		std::string::size_type pos;
		std::vector<std::string> timeResult;
		szOpenTimes += "|";//扩展字符串以方便操作
		int size = szOpenTimes.size();

		for(INT i = 0; i < size; i++)
		{
			pos = szOpenTimes.find("|", i);
			if(pos < size)
			{
				std::string s = szOpenTimes.substr(i, pos - i);
				timeResult.push_back(s);
			}

			i = pos;
		}

		for (INT i = 0; i < timeResult.size(); i++)
		{
			INT nTime = atoi(timeResult[i].c_str());
			DWORD dwTimeOfDayInSec = (nTime / 100) * 3600 + (nTime % 100) * 60;
			cAct.vecKinOpenTimes.push_back(dwTimeOfDayInSec);
		}

		m_mapKinActivities[cAct.nKinActivityIndex] = cAct;
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL KTongManager::LoadKinLevelConfig()
{
	INT nHeight = 0;
	ITabFile* piTabFile = g_OpenTabFile("/setting/kin/kinlevel.txt");
	LOG_PROCESS_ERROR(piTabFile);

	nHeight = piTabFile->GetHeight();

	for (int nIndex = 2; nIndex <= nHeight; nIndex++)
	{
		QTongUpgradeNeed cTongUpgradeNeed;
		INT nKinLevel;
		piTabFile->GetInteger(nIndex, "KinLevel", 0, &nKinLevel);
		piTabFile->GetInteger(nIndex, "PealNum", 0, &cTongUpgradeNeed.nPealNeed);
		piTabFile->GetInteger(nIndex, "JadeiteNum", 0, &cTongUpgradeNeed.nJadeiteNeed);
		piTabFile->GetInteger(nIndex, "AgateNum", 0, &cTongUpgradeNeed.nAgateNeed);
		piTabFile->GetInteger(nIndex, "DiamondNum", 0, &cTongUpgradeNeed.nDiamondNeed);

		m_mapTongUpgradeNeed[nKinLevel] = cTongUpgradeNeed;
		Q_Printl("..nKinLevel="<<nKinLevel);
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL KTongManager::LoadOperationConfig()
{
    BOOL        bResult         = false;
    BOOL        bRetCode        = false;
    int         nHeight         = 0;
    int         nOperationType  = 0;
    int         nOperationIndex = 0;
    ITabFile*   piTabFile       = NULL;
    char        szValue [32];
    char        szFileName[PATH_MAX];
    
    DECLARE_STRING_MAP_BEGIN(KTONG_OPERATION_TYPE)
        REGISTER_STR_TO_VALUE("Speak", totSpeak)
        REGISTER_STR_TO_VALUE("JoinManagerChannel", totJoinManagerChannel)
        REGISTER_STR_TO_VALUE("SpeakInManagerChannel", totSpeakInManagerChannel)
        REGISTER_STR_TO_VALUE("CheckOperationRecord", totCheckOperationRecord)
        REGISTER_STR_TO_VALUE("ModifyAnnouncement", totModifyAnnouncement)
        REGISTER_STR_TO_VALUE("ModifyOnlineMessage", totModifyOnlineMessage)
        REGISTER_STR_TO_VALUE("ModifyIntroduction", totModifyIntroduction)
        REGISTER_STR_TO_VALUE("ModifyRules", totModifyRules)
        REGISTER_STR_TO_VALUE("ModifyMemorabilia", totModifyMemorabilia)
        REGISTER_STR_TO_VALUE("DevelopTechnology", totDevelopTechnology)
        REGISTER_STR_TO_VALUE("ProductItem", totProductItem)
		REGISTER_STR_TO_VALUE("AllowOrDenyApplyer", totAllowOrDenyApplyer)

        REGISTER_STR_TO_VALUE("PutRepositoryPage2", totPutRepositoryPage2)
        REGISTER_STR_TO_VALUE("PutRepositoryPage3", totPutRepositoryPage3)
        REGISTER_STR_TO_VALUE("PutRepositoryPage4", totPutRepositoryPage4)
        REGISTER_STR_TO_VALUE("PutRepositoryPage5", totPutRepositoryPage5)
        REGISTER_STR_TO_VALUE("PutRepositoryPage6", totPutRepositoryPage6)
        REGISTER_STR_TO_VALUE("PutRepositoryPage7", totPutRepositoryPage7)
        REGISTER_STR_TO_VALUE("PutRepositoryPage8", totPutRepositoryPage8)
        REGISTER_STR_TO_VALUE("PutRepositoryPage9", totPutRepositoryPage9)
        REGISTER_STR_TO_VALUE("PutRepositoryPage10", totPutRepositoryPage10)

        REGISTER_STR_TO_VALUE("GetFromRepositoryPage2", totGetFromRepositoryPage2)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage3", totGetFromRepositoryPage3)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage4", totGetFromRepositoryPage4)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage5", totGetFromRepositoryPage5)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage6", totGetFromRepositoryPage6)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage7", totGetFromRepositoryPage7)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage8", totGetFromRepositoryPage8)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage9", totGetFromRepositoryPage9)
        REGISTER_STR_TO_VALUE("GetFromRepositoryPage10", totGetFromRepositoryPage10)

        REGISTER_STR_TO_VALUE("ModifyCalendar", totModifyCalendar)
        REGISTER_STR_TO_VALUE("ManageTask", totManageTask)
        REGISTER_STR_TO_VALUE("ManageVote", totManageVote)
        REGISTER_STR_TO_VALUE("AddToGroup", totAddToGroup)
        REGISTER_STR_TO_VALUE("MoveFromGroup", totMoveFromGroup)
        REGISTER_STR_TO_VALUE("ModifyWage", totModifyWage)
        REGISTER_STR_TO_VALUE("PaySalary", totPaySalary)
        REGISTER_STR_TO_VALUE("ModifyMemberRemark", totModifyMemberRemark)
        REGISTER_STR_TO_VALUE("ModifyGroupName", totModifyGroupName)
    DECLARE_STRING_MAP_END(KTONG_OPERATION_TYPE)

    snprintf(szFileName, sizeof(szFileName), "/setting/kin/kinoperation.txt");
    szFileName[sizeof(szFileName) - 1] = '\0';

    piTabFile = g_OpenTabFile(szFileName);
    LOG_PROCESS_ERROR(piTabFile);

    nHeight = piTabFile->GetHeight();
    LOG_PROCESS_ERROR(nHeight == totTotal);

    for (int nIndex = 1; nIndex < nHeight; nIndex++)
    {
        bRetCode = piTabFile->GetString(nIndex + 1, "Operation", "", szValue, sizeof(szValue));
        LOG_PROCESS_ERROR(bRetCode);
        
        MAP_STRING_LOCAL(KTONG_OPERATION_TYPE, szValue, nOperationType);

        bRetCode = piTabFile->GetInteger(nIndex + 1, "Index", 0, &nOperationIndex);
        LOG_PROCESS_ERROR(bRetCode);

        LOG_PROCESS_ERROR(nOperationIndex >= 0 && nOperationIndex < TONG_OPERATION_NUM);

        LOG_PROCESS_ERROR(m_nOperationIndex[nOperationType] == INVALID_TONG_OPERATION);
        m_nOperationIndex[nOperationType] = nOperationIndex;
    }

    bResult = true;
EXIT0:
    SAFE_RELEASE(piTabFile);
    return bResult;
}

BOOL KTongManager::LoadSchemaTemplate()
{
    BOOL                    bResult     = false;
    IFile*                  piFile      = NULL;
    size_t                  uFileSize   = 0;
    size_t                  uReadBytes  = 0;
    char                    szFileName[PATH_MAX];
    KTongSchemaFileHeader   FileHeader;

    snprintf(szFileName, sizeof(szFileName), "/setting/kin/kintemplate.txt");
    szFileName[sizeof(szFileName) - 1] = '\0';

    piFile = g_OpenFile(szFileName);
    LOG_PROCESS_ERROR(piFile);

    uFileSize = piFile->Size();
    LOG_PROCESS_ERROR(uFileSize >= sizeof(KTongSchemaFileHeader));

    uReadBytes = piFile->Read(&FileHeader, sizeof(FileHeader));
    LOG_PROCESS_ERROR(uReadBytes == sizeof(FileHeader));

    LOG_PROCESS_ERROR(FileHeader.nVersion == TONG_SCHEMA_FILE_VERSION);
    LOG_PROCESS_ERROR(FileHeader.nCount >= 0 && FileHeader.nCount <= MAX_TONG_SCHEMA_TEMPLATE);
    LOG_PROCESS_ERROR(uFileSize == sizeof(KTongSchemaFileHeader) + sizeof(KTongSchemaData) * FileHeader.nCount);

    m_SchemaTemplate.resize(FileHeader.nCount);

    uReadBytes = piFile->Read(&m_SchemaTemplate[0], sizeof(KTongSchemaData) * FileHeader.nCount);
    LOG_PROCESS_ERROR(uReadBytes == sizeof(KTongSchemaData) * FileHeader.nCount);

    for (int i = 0; i < FileHeader.nCount; i++)
    {
        KTongSchemaData* pTemplate = &m_SchemaTemplate[i];
        
        LOG_PROCESS_ERROR(pTemplate->nMasterGroup >= 0);
        LOG_PROCESS_ERROR(pTemplate->nMasterGroup < MAX_TONG_GROUP);
        LOG_PROCESS_ERROR(pTemplate->Data[pTemplate->nMasterGroup].bEnable);

        LOG_PROCESS_ERROR(pTemplate->nDefaultGroup >= 0);
        LOG_PROCESS_ERROR(pTemplate->nDefaultGroup < MAX_TONG_GROUP);
        LOG_PROCESS_ERROR(pTemplate->Data[pTemplate->nDefaultGroup].bEnable);

        // 预处理,防止模板数据错误,字符串溢出
        for (int j = 0; j < MAX_TONG_GROUP; j++)
        {
            KTongGroup* pGroup = &pTemplate->Data[j];

            pGroup->szName[sizeof(pGroup->szName) - 1] = '\0';
        }
    }

    bResult = true;
EXIT0:
    SAFE_RELEASE(piFile);
    return bResult;
}

BOOL KTongManager::OnJoinTong(DWORD dwPlayerID, DWORD dwTongID)
{
    BOOL            bResult         = false;
    BOOL            bRetCode        = false;
    int             nLuaTopIndex    = 0;
    //KScriptManager* pScriptManager  = NULL;

    //assert(g_pGameCenter);
    //pScriptManager = &g_pGameCenter->m_ScriptManager;

    //bRetCode = pScriptManager->IsFuncExist(TONG_SCRIPTS, JOIN_TONG_FUNCTION);
    //LOG_PROCESS_ERROR(bRetCode);

    //pScriptManager->SafeCallBegin(&nLuaTopIndex);

    //pScriptManager->Push(dwPlayerID);
    //pScriptManager->Push(dwTongID);

    //pScriptManager->Call(TONG_SCRIPTS, JOIN_TONG_FUNCTION, 0);

    //pScriptManager->SafeCallEnd(nLuaTopIndex);
    bResult = true;
EXIT0:
    if (!bResult)
    {
 /*       QLogPrintf(
            LOG_INFO, "Failed to Call Scripts:%s function:%s RoleID:%lu TongID:%lu\n", 
            TONG_SCRIPTS, JOIN_TONG_FUNCTION, dwPlayerID, dwTongID
        );*/
    }

    return bResult;
}


void KTongManager::OnTongMemberChanged( DWORD dwTongID )
{
	int		nStackIndex		= -1;
	KTong*	pTong			= NULL;
	KTong*	pTongUp			= NULL;
	KTong*	pTongDown		= NULL;

	pTong = GetTong(dwTongID);
	LOG_PROCESS_ERROR(pTong != NULL);

	for (std::size_t i = 0; i < m_TongStack.size(); ++i)
	{
		if (m_TongStack[i] == dwTongID)
		{
			nStackIndex = i;
			break;
		}
	}
	LOG_PROCESS_ERROR(nStackIndex != -1);

	for (std::size_t i = nStackIndex; i > 0; --i)
	{
		pTongUp		= GetTong(m_TongStack[i - 1]);
		pTongDown	= pTong;
		LOG_PROCESS_ERROR(pTongUp);
		LOG_PROCESS_ERROR(pTongDown);

		if (pTongDown->m_Roster.size() > pTongUp->m_Roster.size())
		{
			std::swap(m_TongStack[i - 1], m_TongStack[i]);
			continue;
		}
		break;
	}

	for (int i = nStackIndex; i < (int)m_TongStack.size() - 1; ++i)
	{
		pTongUp		= pTong;
		pTongDown	= GetTong(m_TongStack[i + 1]);
		LOG_PROCESS_ERROR(pTongUp);
		LOG_PROCESS_ERROR(pTongDown);

		if (pTongUp->m_Roster.size() < pTongDown->m_Roster.size())
		{
			std::swap(m_TongStack[i], m_TongStack[i + 1]);
			continue;
		}
		break;
	}

	m_TongStackLastUpdateCounter = g_pGameCenter->m_nWorkLoop;

EXIT0:
	return;
}

// 族长组设置
// ============================================================================



void KTongManager::CreateSchema()
{
	m_SchemaTemplate.resize(1);
	memset(&m_SchemaTemplate[0], 0, sizeof(m_SchemaTemplate[0]));
	KTongSchemaData& tongSchema = m_SchemaTemplate[0];

	// 有三个组，分别为族长，精英，普通帮众
	const int nGroupNum = USE_TONG_GROUP;
	char szName[16] = {0}; // buf

	tongSchema.nMasterGroup = MASTER_GROUP_INDEX;
	tongSchema.nDefaultGroup = DEFAULT_GROUP_INDEX;

	// ===================================================
	// 设置族长权限
	KTongGroup& masterGroup = tongSchema.Data[MASTER_GROUP_INDEX];
	masterGroup.bEnable = true;
	masterGroup.nWage = 0;
	strcpy(szName, LANG_031 );
	ModifyTongGroup(masterGroup, tmsGroupName, (BYTE*)szName, strlen(szName) + 1);

	// 基本操作权限
	const int nMasterBaseMaskNum = 7;
	const int nMasterAdvnaceMaskNum = 5;
	KTONG_MODIFY_BASE_OPERATION_MASK masterBaseMask[nMasterBaseMaskNum] = {
		{m_nOperationIndex[totSpeak], true}, 
		{m_nOperationIndex[totJoinManagerChannel], true}, 
		{m_nOperationIndex[totModifyAnnouncement], true}, 
		{m_nOperationIndex[totDevelopTechnology], true}, 
		{m_nOperationIndex[totPutRepositoryPage2], true}, 
		{m_nOperationIndex[totModifyCalendar], true},
		{m_nOperationIndex[totAllowOrDenyApplyer], true}
	};
	for (int i = 0; i < nMasterBaseMaskNum; ++i)
	{
		ModifyTongGroup(masterGroup, tmsBaseOperation, (BYTE*)&masterBaseMask[i], sizeof(masterBaseMask[i]));
	}

	// 高级操作权限
	KTONG_MODIFY_ADVANCE_OPERATION_MASK masterAdvanceMask[nGroupNum][nMasterAdvnaceMaskNum] = {
		{
			{MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], true}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], true}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], true}
		},
		{
			{ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], true}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], true}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], true}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], true}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], true}
		},
		{
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], true}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nMasterAdvnaceMaskNum; ++j)
		{
			ModifyTongGroup(masterGroup, tmsAdvanceOperation, (BYTE*)&masterAdvanceMask[i][j], sizeof(masterAdvanceMask[i][j]));
		}
	}

	// 基本授权
	KTONG_MODIFY_BASE_GRANT_MASK masterBaseGrantMask[nGroupNum][nMasterBaseMaskNum] = {
		{
			{MASTER_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		},
		{
			{ELITE_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], true}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], true}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], true}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], true}
		},
		{
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nMasterBaseMaskNum; ++j)
		{
			ModifyTongGroup(masterGroup, tmsBaseGrant, (BYTE*)&masterBaseGrantMask[i][j], sizeof(masterBaseGrantMask[i][j]));
		}
	}

	// 高级授权
	KTONG_MODIFY_ADVANCE_GRANT_MASK masterAdvanceGrantMask[nGroupNum][nGroupNum][nMasterAdvnaceMaskNum] = {
		{
			{
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		},
		{
			{
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], true}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], true}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], true}
			},
			{
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], true}
			},
			{
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		},
		{
			{
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nGroupNum; ++j)
		{
			for (int k = 0; k < nMasterAdvnaceMaskNum; ++k)
			{
				ModifyTongGroup(masterGroup, tmsAdvanceGrant, (BYTE*)&masterAdvanceGrantMask[i][j][k], sizeof(masterAdvanceGrantMask[i][j][k]));
			}
		}
	}

	// ===========================================================
	// 设置精英权限
	KTongGroup& eliteGroup = tongSchema.Data[ELITE_GROUP_INDEX];
	eliteGroup.bEnable = true;
	eliteGroup.nWage = 0;
	memset(szName, 0, sizeof(szName));
	strcpy(szName, LANG_032);
	ModifyTongGroup(eliteGroup, tmsGroupName, (BYTE*)szName, strlen(szName) + 1);

	// 基本操作权限
	const int nEliteBaseMaskNum = 7;
	const int nEliteAdvanceMaskNum = 5;
	KTONG_MODIFY_BASE_OPERATION_MASK eliteBaseMask[nEliteBaseMaskNum] = {
		{m_nOperationIndex[totSpeak], true}, 
		{m_nOperationIndex[totJoinManagerChannel], true}, 
		{m_nOperationIndex[totModifyAnnouncement], true}, 
		{m_nOperationIndex[totDevelopTechnology], true}, 
		{m_nOperationIndex[totPutRepositoryPage2], true}, 
		{m_nOperationIndex[totModifyCalendar], true},
		{m_nOperationIndex[totAllowOrDenyApplyer], true}
	};
	for (int i = 0; i < nEliteBaseMaskNum; ++i)
	{
		ModifyTongGroup(eliteGroup, tmsBaseOperation, (BYTE*)&eliteBaseMask[i], sizeof(eliteBaseMask[i]));
	}

	// 高级操作权限
	KTONG_MODIFY_ADVANCE_OPERATION_MASK eliteAdvanceMask[nGroupNum][nEliteAdvanceMaskNum] = {
		{
			{MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
		    {MASTER_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
		},
		{
			{ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
		},
		{
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], true}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nEliteAdvanceMaskNum; ++j)
		{
			ModifyTongGroup(eliteGroup, tmsAdvanceOperation, (BYTE*)&eliteAdvanceMask[i][j], sizeof(eliteAdvanceMask[i][j]));
		}
	}

	// 基本授权
	KTONG_MODIFY_BASE_GRANT_MASK eliteBaseGrantMask[nGroupNum][nEliteBaseMaskNum] = {
		{
			{MASTER_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		},
		{
			{ELITE_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		},
		{
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nEliteBaseMaskNum; ++j)
		{
			ModifyTongGroup(eliteGroup, tmsBaseGrant, (BYTE*)&eliteBaseGrantMask[i][j], sizeof(eliteBaseGrantMask[i][j]));
		}
	}

	// 高级授权
	KTONG_MODIFY_ADVANCE_GRANT_MASK eliteAdvanceGrantMask[nGroupNum][nGroupNum][nEliteAdvanceMaskNum] = {
		{
			{
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false},
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		},
		{
			{
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false},
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		},
		{
			{
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nGroupNum; ++j)
		{
			for (int k = 0; k < nEliteAdvanceMaskNum; ++k)
			{
				ModifyTongGroup(eliteGroup, tmsAdvanceGrant, (BYTE*)&eliteAdvanceGrantMask[i][j][k], sizeof(eliteAdvanceGrantMask[i][j][k]));
			}
		}
	}

	//============================================================
	// 设置普通帮众权限
	KTongGroup& defaultGroup = tongSchema.Data[DEFAULT_GROUP_INDEX];
	defaultGroup.bEnable = true;
	defaultGroup.nWage = 0;
	memset(szName, 0, sizeof(szName));
	strcpy(szName, LANG_033);
	ModifyTongGroup(defaultGroup, tmsGroupName, (BYTE*)szName, strlen(szName) + 1);

	// 基本操作权限
	const int nDefaultBaseMaskNum = 7;
	const int nDefaultAdvanceMaskNum = 5;
	KTONG_MODIFY_BASE_OPERATION_MASK defaultBaseMask[nDefaultBaseMaskNum] = {
		{m_nOperationIndex[totSpeak], true}, 
		{m_nOperationIndex[totJoinManagerChannel], false}, 
		{m_nOperationIndex[totModifyAnnouncement], false}, 
		{m_nOperationIndex[totDevelopTechnology], false}, 
		{m_nOperationIndex[totPutRepositoryPage2], true}, 
		{m_nOperationIndex[totModifyCalendar], false},
		{m_nOperationIndex[totAllowOrDenyApplyer], false}
	};
	for (int i = 0; i < nDefaultBaseMaskNum; ++i)
	{
		ModifyTongGroup(defaultGroup, tmsBaseOperation, (BYTE*)&defaultBaseMask[i], sizeof(defaultBaseMask[i]));
	}

	// 高级操作权限
	KTONG_MODIFY_ADVANCE_OPERATION_MASK defaultAdvanceMask[nGroupNum][nDefaultAdvanceMaskNum] = {
		{
			{MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}},
		{
			{ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
		},
		{
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totMoveFromGroup], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nDefaultAdvanceMaskNum; ++j)
		{
			ModifyTongGroup(defaultGroup, tmsAdvanceOperation, (BYTE*)&defaultAdvanceMask[i][j], sizeof(defaultAdvanceMask[i][j]));
		}
	}

	// 基本授权
	KTONG_MODIFY_BASE_GRANT_MASK defaultBaseGrantMask[nGroupNum][nDefaultBaseMaskNum] = {
		{
			{MASTER_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{MASTER_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		},
		{
			{ELITE_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{ELITE_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		},
		{
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totSpeak], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totJoinManagerChannel], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyAnnouncement], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totDevelopTechnology], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totPutRepositoryPage2], false}, 
			{DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyCalendar], false}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nDefaultBaseMaskNum; ++j)
		{
			ModifyTongGroup(defaultGroup, tmsBaseGrant, (BYTE*)&defaultBaseGrantMask[i][j], sizeof(defaultBaseGrantMask[i][j]));
		}
	}

	// 高级授权
	KTONG_MODIFY_ADVANCE_GRANT_MASK defaultAdvanceGrantMask[nGroupNum][nGroupNum][nDefaultAdvanceMaskNum] = {
		{
			{
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{MASTER_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		},
		{
			{
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{ELITE_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{ELITE_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{ELITE_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		},
		{
			{
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, MASTER_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, ELITE_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			},
			{
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totAddToGroup], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyWage], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyMemberRemark], false}, 
				{DEFAULT_GROUP_INDEX, DEFAULT_GROUP_INDEX, m_nOperationIndex[totModifyGroupName], false}
			}
		}
	};
	for (int i = 0; i < nGroupNum; ++i)
	{
		for (int j = 0; j < nGroupNum; ++j)
		{
			for (int k = 0; k < nDefaultAdvanceMaskNum; ++k)
			{
				ModifyTongGroup(defaultGroup, tmsAdvanceGrant, (BYTE*)&defaultAdvanceGrantMask[i][j][k], sizeof(defaultAdvanceGrantMask[i][j][k]));
			}
		}
	}
}

void KTongManager::ModifyTongGroup( KTongGroup& tongGroup, int nModifyType, BYTE* pbyData, int nLen )
{
	assert(nModifyType > tmsBegin && nModifyType < tmsEnd);
	assert(pbyData);
	assert(nLen > 0);

	switch (nModifyType)
	{
	case tmsGroupName:
		{
			char* pszNewName = (char*)pbyData;
			strncpy(tongGroup.szName, pszNewName, nLen);
		}
		break;
	case tmsBaseOperation:
		{
			KTONG_MODIFY_BASE_OPERATION_MASK* pModify = (KTONG_MODIFY_BASE_OPERATION_MASK*)pbyData;
			if (pModify->bValue)
			{
			    tongGroup.byBaseOperationMask |= (1 << (pModify->byOffset));
			}
			else
			{
			    tongGroup.byBaseOperationMask &= ~(1 << (pModify->byOffset));
			}
		}
		break;
	case tmsAdvanceOperation:
		{
			KTONG_MODIFY_ADVANCE_OPERATION_MASK* pModify = (KTONG_MODIFY_ADVANCE_OPERATION_MASK*)pbyData;
			if (pModify->bValue)
			{
				tongGroup.byAdvanceOperationMask[pModify->byIndex] |= (1 << (pModify->byOffset));
			}
			else
			{
				tongGroup.byAdvanceOperationMask[pModify->byIndex] &= ~(1 << (pModify->byOffset));
			}
		}
		break;
	case tmsGroupWage:
		{
			assert(nLen == sizeof(int));
			int* pnWage = (int*)pbyData;
			tongGroup.nWage = *pnWage;
		}
		break;
	case tmsBaseGrant:
		{
			KTONG_MODIFY_BASE_GRANT_MASK* pModify = (KTONG_MODIFY_BASE_GRANT_MASK*)pbyData;
			if (pModify->bValue)
			{
				tongGroup.byBaseGrantMask[pModify->byIndex] |= (1 << (pModify->byOffset));
			}
			else
			{
				tongGroup.byBaseGrantMask[pModify->byIndex] &= ~(1 << (pModify->byOffset));
			}
		}
		break;
	case tmsAdvanceGrant:
		{
			KTONG_MODIFY_ADVANCE_GRANT_MASK* pModify = (KTONG_MODIFY_ADVANCE_GRANT_MASK*)pbyData;
			if (pModify->bValue)
			{
				tongGroup.byAdvanceGrantMask[pModify->byIndex1][pModify->byIndex2] |= (1 << (pModify->byOffset));
			}
			else
			{
				tongGroup.byAdvanceGrantMask[pModify->byIndex1][pModify->byIndex2] &= ~(1 << (pModify->byOffset));
			}
		}
		break;
	default:
		assert(0);
	}
}
