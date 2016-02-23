#include "stdafx.h"
#include "serverbase/StrMap.h"
#include "Kin.h"
#include "KRole.h"
#include "GameCenter.h"
//#include "ItemDef.h"
#include "kinhandler.h"
#include "onlinegamebase/kscriptmanager.h"
#include "serverbase/RankDef.h"
#include "onlinegamebase/ksysservice.h"

KTong::KTong()
{
    m_dwID                  = KD_BAD_ID;
    m_dwMaster              = KD_BAD_ID;
    m_nFund                 = 0;
    m_nMaxWageRate          = 0;
    m_nLevel                = 1;
    m_nDevelopmentPoint     = 0;
    m_nMaxMemberCount       = 0;

    m_szName[0]             = '\0';
    m_szAnnouncement[0]     = '\0';
    m_szOnlineMessage[0]    = '\0';
    m_szIntroduction[0]     = '\0';
    m_szRules[0]            = '\0';

    m_bUpdateFlag           = false;
	m_bUpdateTongRankFlag	= false;
    m_eState                = tsInvalid;
    m_nStateTimer           = 0;
	m_bFreeJoin				= TRUE; // 默认打开自由招募

    //m_eCamp                 = cNeutral;
    m_dwMemorabiliaCount    = 0;

    m_nUsedDevelopmentPoint = 0;
    memset(m_byTechTree, 0, sizeof(m_byTechTree));
    memset(m_nLastUpdateFrameTable, 0, sizeof(m_nLastUpdateFrameTable));
}

BOOL KTong::Init()
{
    BOOL bResult            = false;
    BOOL bHistoryInitFlag   = false;
    BOOL bRepertoryInitFlag = false;

    bHistoryInitFlag = m_History.Init();
    LOG_PROCESS_ERROR(bHistoryInitFlag);

    bRepertoryInitFlag = m_Repertory.Init();
    LOG_PROCESS_ERROR(bRepertoryInitFlag);

	bResult = true;

EXIT0:
    if (!bResult)
    {
        if (bHistoryInitFlag)
            m_History.UnInit();
        if (bRepertoryInitFlag)
            m_Repertory.UnInit();
    }
    return bResult;
}

void KTong::UnInit()
{
    m_Repertory.UnInit();
    m_History.UnInit();
}

void KTong::Activate()
{
    if (m_bUpdateFlag)
    {
        g_pGameCenter->m_MiscDB.DoSaveTongData(m_dwID, this);
        m_bUpdateFlag = false;
    }

	if(m_bUpdateTongRankFlag)
	{
		DWORD dwTotalFightScore = GetTotalFightScore();
		DWORD dwTotalSalary = GetTotalSalary();
		g_pGameCenter->m_RankManager.ModifyRankValue(emRANK_TONG_FIGHTSCORE, m_dwID, dwTotalFightScore);
		g_pGameCenter->m_RankManager.ModifyRankValue(emRANK_TONG_SALARY, m_dwID, dwTotalSalary);
		m_bUpdateTongRankFlag = false;
	}

    for (KROSTER::iterator it = m_Roster.begin(); it != m_Roster.end(); ++it)
    {
        BOOL            bUpdateFlag = false;
        KRole*          pRole       = NULL;
        KTongMember*    pMember     = &it->second;

        pRole = g_pGameCenter->m_RoleManager.GetRole(it->first);
        if (!pRole)
            continue;
        
        bUpdateFlag |= (pMember->nLevel != pRole->m_byLevel);
        bUpdateFlag |= (pMember->dwForceID != pRole->m_dwForceID);
        bUpdateFlag |= (pMember->dwMapID != pRole->m_dwMapID);
        bUpdateFlag |= (pMember->bIsOnline != pRole->IsOnline());
		bUpdateFlag |= (pMember->dwFightScore != pRole->m_nFightScore);
        
        if (!bUpdateFlag)
            continue;
        
        pMember->nLevel			= pRole->m_byLevel;
        pMember->dwForceID		= pRole->m_dwForceID;
        pMember->dwMapID		= pRole->m_dwMapID;
        pMember->bIsOnline		= pRole->IsOnline();
		pMember->dwFightScore	= pRole->m_nFightScore;

        pMember->nLastUpdateFrame = g_pGameCenter->m_nWorkLoop;

		m_bUpdateTongRankFlag = true;
    }
}

BOOL KTong::Load(DWORD dwTongID, BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = false;
    KTONG_DB_HEADER*    pHeader     = (KTONG_DB_HEADER*)pbyData;
    BYTE*               pbyOffSet   = pbyData;
    size_t              uLeftSize   = uDataLen;
    
    assert(pbyData);

    LOG_PROCESS_ERROR(uDataLen >= sizeof(KTONG_DB_HEADER));
    LOG_PROCESS_ERROR(pHeader->nVersion <= TONG_DB_VERSION);

    m_dwID = dwTongID;

    pbyOffSet += sizeof(KTONG_DB_HEADER);
    uLeftSize -= sizeof(KTONG_DB_HEADER);

    while (uLeftSize)
    {
        KTONG_DB_SECTION* pSection = (KTONG_DB_SECTION*)pbyOffSet;
        pbyOffSet += sizeof(KTONG_DB_SECTION);
        uLeftSize -= sizeof(KTONG_DB_SECTION);

        LOG_PROCESS_ERROR(uLeftSize >= pSection->uDataLen);

        switch (pSection->nType)
        {
        case eTongBaseInfo:
            {
                LOG_PROCESS_ERROR(pHeader->nVersion == TONG_DB_VERSION);
                bRetCode = LoadBaseInfo(pSection->byData, pSection->uDataLen);
                LOG_PROCESS_ERROR(bRetCode);
            }
            break;

        case eTongSchema:
            bRetCode = LoadSchema(pSection->byData, pSection->uDataLen);
            LOG_PROCESS_ERROR(bRetCode);
            break;

        case eTongRoster:
            {
                LOG_PROCESS_ERROR(pHeader->nVersion == TONG_DB_VERSION);
                bRetCode = LoadRoster(pSection->byData, pSection->uDataLen);
                LOG_PROCESS_ERROR(bRetCode);
            }
            break;

        case eTongAnnouncement:
            strncpy(m_szAnnouncement, (const char*)pSection->byData, sizeof(m_szAnnouncement));
            m_szAnnouncement[sizeof(m_szAnnouncement) - 1] = '\0';
            break;

        case eTongOnlineMessage:
            strncpy(m_szOnlineMessage, (const char*)pSection->byData, sizeof(m_szOnlineMessage));
            m_szOnlineMessage[sizeof(m_szOnlineMessage) - 1] = '\0';
            break;

        case eTongIntroduction:
            strncpy(m_szIntroduction, (const char*)pSection->byData, sizeof(m_szIntroduction));
            m_szIntroduction[sizeof(m_szIntroduction) - 1] = '\0';
            break;

        case eTongRules:
            strncpy(m_szRules, (const char*)pSection->byData, sizeof(m_szRules));
            m_szRules[sizeof(m_szRules) - 1] = '\0';
            break;

        case eTongMemorabilia:
            bRetCode = LoadMemorabilia(pSection->byData, pSection->uDataLen);
            LOG_PROCESS_ERROR(bRetCode);
            break;

        case eTongRepertory:
            bRetCode = m_Repertory.Load(pSection->byData, pSection->uDataLen);
            LOG_PROCESS_ERROR(bRetCode);
            break;

        case eTongDevelopmentInfo:
            bRetCode = LoadDevelopmentInfo(pSection->byData, pSection->uDataLen);
            LOG_PROCESS_ERROR(bRetCode);
            break;
        case eTongHistory:
            bRetCode = m_History.Load(pSection->byData, pSection->uDataLen);
            LOG_PROCESS_ERROR(bRetCode);
            break;

        default:
            QLogPrintf(LOG_ERR, "[KTong] DB error, can not recognize section type\n");
            goto EXIT0;
        }

        pbyOffSet += pSection->uDataLen;
        uLeftSize -= pSection->uDataLen;
    }

    if (pHeader->nVersion == 1)
    {
        // 旧版本数据开启仓库默认页
        m_Repertory.m_Page[0].bEnable = true;
    }

    if (pHeader->nVersion < TONG_DB_VERSION) // 不同版本的数据格式转换之后尽快存盘
    {
        m_bUpdateFlag = true;
    }

	m_bUpdateTongRankFlag = false;
 
    bResult = true;
EXIT0:
    return bResult;
}


BOOL KTong::Save(size_t* puUseSize, BYTE* pbyBuffer, size_t uBufferSize)
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = false;
    int                 nRetCode    = 0;
    KTONG_DB_HEADER*    pHeader     = NULL;
    size_t		        uUsedSize   = 0;
    size_t              uLeftSize   = uBufferSize;
    size_t              uStrLen     = 0;
    BYTE*               pbyOffset   = NULL;
    KTONG_DB_SECTION*   pSection    = NULL;

    assert(pbyBuffer);
    assert(puUseSize);

    assert(uBufferSize >=
        sizeof(KTONG_DB_HEADER) +
        sizeof(KTONG_DB_SECTION) + sizeof(KTONG_DB_BASEINFO) +
        sizeof(KTONG_DB_SECTION) + sizeof(KTONG_DB_SCHEMA) + 
        sizeof(KTONG_DB_SECTION) + sizeof(KTONG_DB_ROSTER) + sizeof(KTONG_DB_MEMBER_DATA) * MAX_TONG_MEMBER_COUNT +
        sizeof(KTONG_DB_SECTION) + sizeof(m_szAnnouncement) +
        sizeof(KTONG_DB_SECTION) + sizeof(m_szOnlineMessage) +
        sizeof(KTONG_DB_SECTION) + sizeof(m_szIntroduction) +
        sizeof(KTONG_DB_SECTION) + sizeof(m_szRules) +
        sizeof(KTONG_DB_SECTION) +  (sizeof(KTONG_DB_MEMORABILIA) + TONG_MEMORABILIA_RECORD_LEN) * MAX_TONG_MEMORABILIA_COUNT +
        sizeof(KTONG_DB_SECTION) + TONG_REPERTORY_PAGE_NUM *
            (sizeof(KTONG_REPERTORY_DB_PAGE) + TONG_REPERTORY_PAGE_CAPACITY * (sizeof(KTONG_REPERTORY_DB_ITEM) + MAX_ITEM_DATA_SIZE)) +
        sizeof(KTONG_DB_SECTION) + sizeof(KTONG_DB_DEVELOPMENT_DATA) +
        sizeof(KTONG_DB_SECTION) + sizeof(KTONG_DB_HISTORY_CATEGORY) * ohtTotal + sizeof(KTONG_MAX_SIZE_HISTORY_DATA) * TOTAL_RECORD_COUNT
    );

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_HEADER));

    pHeader = (KTONG_DB_HEADER*)pbyBuffer;
    pbyOffset = pbyBuffer + sizeof(KTONG_DB_HEADER);
    uLeftSize -= sizeof(KTONG_DB_HEADER);

    pHeader->nVersion = TONG_DB_VERSION;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongBaseInfo;

    bRetCode = SaveBaseInfo(pSection->byData, uLeftSize, &uUsedSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    pSection->uDataLen = uUsedSize;
    pbyOffset += uUsedSize;
    uLeftSize -= uUsedSize;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongSchema;
    
    bRetCode = SaveSchema(pSection->byData, uLeftSize, &uUsedSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    pSection->uDataLen = uUsedSize;
    pbyOffset += uUsedSize;
    uLeftSize -= uUsedSize;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongRoster;
    
    bRetCode = SaveRoster(pSection->byData, uLeftSize, &uUsedSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    pSection->uDataLen = uUsedSize;
    pbyOffset += uUsedSize;
    uLeftSize -= uUsedSize;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongAnnouncement;

    uStrLen = strlen(m_szAnnouncement) + 1;
    LOG_PROCESS_ERROR(uStrLen <= uLeftSize);
    memcpy(pSection->byData, m_szAnnouncement, uStrLen);

    pSection->uDataLen = uStrLen;
    pbyOffset += uStrLen;
    uLeftSize -= uStrLen;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongOnlineMessage;

    uStrLen = strlen(m_szOnlineMessage) + 1;
    LOG_PROCESS_ERROR(uStrLen <= uLeftSize);
    memcpy(pSection->byData, m_szOnlineMessage, uStrLen);

    pSection->uDataLen = uStrLen;
    pbyOffset += uStrLen;
    uLeftSize -= uStrLen;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongIntroduction;

    uStrLen = strlen(m_szIntroduction) + 1;
    LOG_PROCESS_ERROR(uStrLen <= uLeftSize);
    memcpy(pSection->byData, m_szIntroduction, uStrLen);

    pSection->uDataLen = uStrLen;
    pbyOffset += uStrLen;
    uLeftSize -= uStrLen;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongRules;

    uStrLen = strlen(m_szRules) + 1;
    LOG_PROCESS_ERROR(uStrLen <= uLeftSize);
    memcpy(pSection->byData, m_szRules, uStrLen);

    pSection->uDataLen = uStrLen;
    pbyOffset += uStrLen;
    uLeftSize -= uStrLen;
    // --------------------------------------------------------------------

    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongMemorabilia;
    bRetCode = SaveMemorabilia(pSection->byData, uLeftSize, &uUsedSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    pSection->uDataLen = uUsedSize;
    pbyOffset += uUsedSize;
    uLeftSize -= uUsedSize;

    // --------------------------------------------------------------------

    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongRepertory;

    bRetCode = m_Repertory.Save(pSection->byData, uLeftSize, &uUsedSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    pSection->uDataLen = uUsedSize;
    pbyOffset += uUsedSize;
    uLeftSize -= uUsedSize;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);

    pSection->nType = eTongDevelopmentInfo;

    bRetCode = SaveDevelopmentInfo(pSection->byData, uLeftSize, &uUsedSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    pSection->uDataLen = uUsedSize;
    pbyOffset += uUsedSize;
    uLeftSize -= uUsedSize;

    // --------------------------------------------------------------------
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_SECTION));

    pSection = (KTONG_DB_SECTION*)pbyOffset;
    pbyOffset += sizeof(KTONG_DB_SECTION);
    uLeftSize -= sizeof(KTONG_DB_SECTION);
        
    pSection->nType = eTongHistory;
    bRetCode = m_History.Sava(pSection->byData, uLeftSize, &uUsedSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    pSection->uDataLen = uUsedSize;
    pbyOffset += uUsedSize;
    uLeftSize -= uUsedSize;
    // --------------------------------------------------------------------
    *puUseSize = uBufferSize - uLeftSize;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::AddNewMember(KRole* pRole, int nGroupID)
{
    BOOL                            bResult     = false;
    KTongMember*                    pMember     = NULL;
    std::pair<KROSTER::iterator, BOOL>   MemberInsRet;
	ScriptSafe safeScript = g_cScriptManager.GetSafeScript();

    assert(pRole);

    LOG_PROCESS_ERROR(pRole->m_dwTongID == KD_BAD_ID);
    LOG_PROCESS_ERROR(nGroupID >= 0 && nGroupID < MAX_TONG_GROUP);
    LOG_PROCESS_ERROR(m_nMaxMemberCount > (int)m_Roster.size());
    LOG_PROCESS_ERROR(m_Schema.Data[nGroupID].bEnable);

	MemberInsRet = m_Roster.insert(std::make_pair(pRole->m_dwPlayerID, KTongMember()));
    LOG_PROCESS_ERROR(MemberInsRet.second);

    pMember                     = &MemberInsRet.first->second;

    pMember->dwID               = pRole->m_dwPlayerID;
    pMember->nGroupID           = nGroupID;
    pMember->nJoinTime          = g_pGameCenter->m_nTimeNow;
    pMember->szRemark[0]        = '\0';
    pMember->nLastOfflineTime   = 0;
    pMember->nSalary            = 0;

    pMember->nLevel             = pRole->m_byLevel;
    pMember->dwForceID          = pRole->m_dwForceID;
    pMember->dwMapID            = pRole->m_dwMapID;
    pMember->bIsOnline          = pRole->IsOnline();
	pMember->dwFightScore		= pRole->m_nFightScore;

    pMember->nLastUpdateFrame   = g_pGameCenter->m_nWorkLoop;

    pRole->m_dwTongID           = m_dwID;

    m_bUpdateFlag               = true;

	safeScript->CallTableFunction("Kin", "OnPlayerJoinTong", 0, "dd", pRole->m_dwPlayerID, pRole->m_dwTongID);

    bResult = true;
EXIT0:
    return bResult;
}


BOOL KTong::SetActivityOpenTime(DWORD dwActivityIndex, DWORD dwActivityOpenTime)
{
	BOOL bResult = FALSE;
	std::map<DWORD, SyncKinActivityInfo>::iterator it;
	INT nCycleTimeInSeconds = (g_pGameCenter->m_nTimeNow % (24 * 3600)) + 8 * 3600;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

 	PROCESS_ERROR(nCycleTimeInSeconds < (INT)dwActivityOpenTime);

	it = m_ActivityOpenTimeMap.find(dwActivityIndex);
	if (it != m_ActivityOpenTimeMap.end())
	{
		SyncKinActivityInfo& activeInfo = m_ActivityOpenTimeMap[dwActivityIndex];

		time_t tNow = KSysService::Time(NULL);
		time_t tLastChangeTime = (time_t)activeInfo.dwLastChangeTime;
		tm* pTimeNow = ::localtime(&tNow);
		INT nYDayNow = pTimeNow->tm_yday;

		tm* pTimeQuitTong = ::localtime(&tLastChangeTime);
		INT nYDayLastChange = pTimeQuitTong->tm_yday;

        if (nYDayNow == nYDayLastChange) // 最后修改时间为当天
		{
			PROCESS_ERROR(nCycleTimeInSeconds <= (INT)activeInfo.dwActivityOpenTime); // 如果已过此前设置的活动时间，说明活动已开过
		}
		PROCESS_ERROR((activeInfo.dwActivityOpenTime - nCycleTimeInSeconds) > 20 * 60); // 在活动开启前20分钟内不能再更改活动时间

		activeInfo.dwActivityOpenTime = dwActivityOpenTime;

		activeInfo.dwLastChangeTime = (DWORD)tNow;
	}
	else
	{
		SyncKinActivityInfo activeInfo;
		activeInfo.dwActivityOpenTime = dwActivityOpenTime;
        m_ActivityOpenTimeMap[dwActivityIndex] = activeInfo;
	}
    
	cSafeScript->CallTableFunction("Kin", "SetKinActivityOpenTime", 0, "ddd", m_dwID, dwActivityIndex, dwActivityOpenTime);

	m_bUpdateFlag = TRUE;
	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KTong::RemoveMember(KRole* pRole)
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = false;
    KTongMember*        pMember     = NULL;
    KROSTER::iterator   it;

    assert(pRole);

    // 会长必须移交之后才可退出帮会
    PROCESS_ERROR(pRole->m_dwPlayerID != m_dwMaster);

    it = m_Roster.find(pRole->m_dwPlayerID);
    PROCESS_ERROR(it != m_Roster.end());

    pMember = &it->second;

    //bRetCode = AddFund(pMember->nSalary); //zzx
    //LOG_PROCESS_ERROR(bRetCode);
    pMember->nSalary = 0;

    m_Roster.erase(it);
    pRole->m_dwTongID = KD_BAD_ID;

    m_bUpdateFlag = true;

    bResult = true;
EXIT0:
    return bResult;
}

void KTong::Disband()
{
    KRole*  pRole       = NULL;

    for (KTong::KROSTER::iterator it = m_Roster.begin(); it != m_Roster.end(); ++it)
    {
        pRole = g_pGameCenter->m_RoleManager.GetRole(it->first);
        if (pRole)
        {
            pRole->m_dwTongID = KD_BAD_ID;
			KinHandler::DoChangePlayerTongNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID, KD_BAD_ID, ctrDisband, m_szName);
        }
    }

    g_pGameCenter->m_MiscDB.DoRemoveTongData(m_dwID);
}

BOOL KTong::CanBaseOperate(int nGroup, int nOperation)
{
    BOOL    bResult     = false;
    int     nMaskOffset = 0;

    assert(nGroup >= 0 && nGroup < MAX_TONG_GROUP);
    assert(nOperation > totInvalid && nOperation < totTotal);

    nMaskOffset = g_pGameCenter->m_TongManager.m_nOperationIndex[nOperation];
    LOG_PROCESS_ERROR(nMaskOffset >= 0 && nMaskOffset < TONG_OPERATION_NUM);

    bResult = m_Schema.Data[nGroup].byBaseOperationMask & (1 << nMaskOffset);
EXIT0:
    return bResult;
}

BOOL KTong::CanAdvanceOperate(int nGroup, int nTargetGroup, int nOperation)
{
    BOOL    bResult     = false;
    int     nMaskOffset = 0;

    assert(nGroup >= 0 && nGroup < MAX_TONG_GROUP);
    assert(nTargetGroup >= 0 && nTargetGroup < MAX_TONG_GROUP);
    assert(nOperation > totInvalid && nOperation < totTotal);

    nMaskOffset = g_pGameCenter->m_TongManager.m_nOperationIndex[nOperation];
    LOG_PROCESS_ERROR(nMaskOffset >= 0 && nMaskOffset < TONG_OPERATION_NUM);

    bResult = m_Schema.Data[nGroup].byAdvanceOperationMask[nTargetGroup] & (1 << nMaskOffset);
EXIT0:
    return bResult;
}

BOOL KTong::CanBaseGrant(int nGroup, int nTargetGroup, int nMaskOffset)
{
    BOOL    bResult     = false;

    assert(nGroup >= 0 && nGroup < MAX_TONG_GROUP);
    assert(nTargetGroup >= 0 && nTargetGroup < MAX_TONG_GROUP);

    LOG_PROCESS_ERROR(nMaskOffset >= 0 && nMaskOffset < TONG_OPERATION_NUM);

    bResult = m_Schema.Data[nGroup].byBaseGrantMask[nTargetGroup] & (1 << nMaskOffset);
EXIT0:
    return bResult;
}

BOOL KTong::CanAdvanceGrant(int nGroup, int nTargetGroupA, int nTargetGroupB, int nMaskOffset)
{
    BOOL    bResult     = false;

    assert(nGroup >= 0 && nGroup < MAX_TONG_GROUP);
    assert(nTargetGroupA >= 0 && nTargetGroupA < MAX_TONG_GROUP);
    assert(nTargetGroupB >= 0 && nTargetGroupB < MAX_TONG_GROUP);

    LOG_PROCESS_ERROR(nMaskOffset >= 0 && nMaskOffset < TONG_OPERATION_NUM);

    bResult = m_Schema.Data[nGroup].byAdvanceGrantMask[nTargetGroupA][nTargetGroupB] & (1 << nMaskOffset);
EXIT0:
    return bResult;
}

BOOL KTong::CanSpeak(DWORD dwMemberID)
{
    BOOL            bResult     = false;
    BOOL            bRetCode    = false;
    int             nMaskOffset = 0;
    KTongMember*    pMember     = NULL;

    pMember = GetMember(dwMemberID);
    LOG_PROCESS_ERROR(pMember);

    bRetCode = CanBaseOperate(pMember->nGroupID, totSpeak);
    PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::PaySalary(DWORD dwMemberID, int nTotalSalary)
{
    BOOL            bResult     = false;
    BOOL            bRetCode    = false;
    KTongMember*    pMember     = NULL;
    KTongMember*    pDstMember  = NULL;
    int             nLeftMoney  = 0;
    int             nSilver     = 0;
    
    pMember = GetMember(dwMemberID);
    LOG_PROCESS_ERROR(pMember);

    bRetCode = CanBaseOperate(pMember->nGroupID, totPaySalary);
    LOG_PROCESS_ERROR(bRetCode);

    nLeftMoney = m_nFund;
    for (KROSTER::iterator it = m_Roster.begin(); it != m_Roster.end(); ++it)
    {
        pDstMember = &it->second;
        nLeftMoney -= m_Schema.Data[pDstMember->nGroupID].nWage;
        LOG_PROCESS_ERROR(nLeftMoney >= 0);
    }

    LOG_PROCESS_ERROR(m_nFund - nTotalSalary == nLeftMoney);
    LOG_PROCESS_ERROR(nTotalSalary <= m_nFund * (m_nMaxWageRate / 100.0));

    for (KROSTER::iterator it = m_Roster.begin(); it != m_Roster.end(); ++it)
    {
        pDstMember = &it->second;
        nSilver = m_Schema.Data[pDstMember->nGroupID].nWage;
        LOG_PROCESS_ERROR(nSilver >= 0);

        if (nSilver > INT_MAX - pDstMember->nSalary)
            nSilver = INT_MAX - pDstMember->nSalary;

        assert(m_nFund >= nSilver);
        pDstMember->nSalary += nSilver;
        m_nFund -= nSilver;
    }

    bRetCode = m_History.RecordPaySalary(dwMemberID, nTotalSalary);
    LOG_CHECK_ERROR(bRetCode);

    m_nLastUpdateFrameTable[tdtDevelopmentInfo] = g_pGameCenter->m_nWorkLoop;
    m_bUpdateFlag = true;
	m_bUpdateTongRankFlag = true;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::AddFund(DWORD dwPealNum, DWORD dwJadeiteNum, DWORD dwAgateNum, DWORD dwDiamondNum)
{
    BOOL bResult = false;

	m_dwPearlNum += dwPealNum;
	m_dwJadeiteNum += dwJadeiteNum;
	m_dwAgateNum += dwAgateNum;
	m_dwDiamondNum += dwDiamondNum;
    /*if (nSilver >= 0)
    {
        if (m_nFund > MAX_TONG_FUND_BY_SILVER -  nSilver)
        {
            m_nFund = MAX_TONG_FUND_BY_SILVER;
        }
        else
        {
            m_nFund += nSilver;
        }
    }
    else
    {
        LOG_PROCESS_ERROR(m_nFund >= -nSilver);
        m_nFund += nSilver;
    }*/

    m_nLastUpdateFrameTable[tdtDevelopmentInfo] = g_pGameCenter->m_nWorkLoop;
    m_bUpdateFlag = true;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::LoadBaseInfo(BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult     = false;
    KTONG_DB_BASEINFO*  pBaseInfo   = NULL;
	DWORD* pdwApplyerIds = NULL;
	DWORD* pdwKinActOpenTimeData = NULL;
	SyncKinActivityInfo* pcKinActOpenTimeData = NULL;

    assert(pbyData);
    LOG_PROCESS_ERROR(uDataLen >= sizeof(KTONG_DB_BASEINFO));

    pBaseInfo           = (KTONG_DB_BASEINFO*)pbyData;

    m_dwMaster          = pBaseInfo->dwMaster;
    m_nLevel            = pBaseInfo->nLevel;
	m_byCamp            = pBaseInfo->byCamp;
	m_bFreeJoin			= pBaseInfo->bFreeJoin;
    m_nMaxMemberCount   = (int)pBaseInfo->wMaxMemberCount;
    m_eState            = (KTONG_STATE)pBaseInfo->byTongState;
    m_nStateTimer       = pBaseInfo->nStateTimer;
	m_dwPearlNum        = pBaseInfo->dwPearlNum;
	m_dwDiamondNum      = pBaseInfo->dwDiamondNum;
	m_dwJadeiteNum      = pBaseInfo->dwJadeiteNum;
	m_dwDiamondNum      = pBaseInfo->dwDiamondNum;

    strncpy(m_szName, pBaseInfo->szName, sizeof(m_szName));
    m_szName[sizeof(m_szName) - 1] = '\0';

	pdwApplyerIds = (DWORD*)(pbyData + sizeof(KTONG_DB_BASEINFO));
	for (INT i = 0; i < (INT)pBaseInfo->byApplyerNum; i++)
	{
		m_ApplyJoinTongPlayers.push_back(pdwApplyerIds[i]);
	}

	pcKinActOpenTimeData = (SyncKinActivityInfo*)(pbyData + sizeof(KTONG_DB_BASEINFO) + pBaseInfo->byApplyerNum * sizeof(DWORD));
	for (INT i = 0; i < (INT)pBaseInfo->byActOpenTimeNum; i++)
	{
		SyncKinActivityInfo& rActInfo = pcKinActOpenTimeData[i];
		m_ActivityOpenTimeMap[rActInfo.nKinActivityIndex] = pcKinActOpenTimeData[i];
	}

    bResult = true;
EXIT0:
    return bResult;
}

VOID KTong::SyncRecords()
{
	// 给所有在线玩家同步历史记录
	for(KROSTER::iterator Iter = m_Roster.begin(); Iter != m_Roster.end(); ++Iter)
	{
		if (Iter->second.bIsOnline)
		{
            m_History.SyncRecord(Iter->second.dwID, 0, TONG_OPERATION_HISTORY_PAGE_SIZE);
		}
	}
}

DWORD KTong::GetTotalFightScore()
{
	DWORD dwTotalScore = 0;
	for(KROSTER::iterator Iter = m_Roster.begin(); Iter != m_Roster.end(); ++Iter)
	{
		dwTotalScore += Iter->second.dwFightScore;
	}

	return dwTotalScore;
}

DWORD KTong::GetTotalSalary()
{
	DWORD dwTotalSalary = 0;
	for(KROSTER::iterator Iter = m_Roster.begin(); Iter != m_Roster.end(); ++Iter)
	{
		dwTotalSalary += Iter->second.nSalary;
	}

	return dwTotalSalary;
}

BOOL KTong::SaveBaseInfo(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize)
{
    BOOL                bResult     = false;
    KTONG_DB_BASEINFO*  pBaseInfo   = NULL;
	DWORD* pdwApplyerIds = NULL;
	SyncKinActivityInfo* pcKinActOpenTimesData = NULL;
	INT nKinActSaveIndex = 0;
	std::map<DWORD, SyncKinActivityInfo>::iterator it;

    assert(pbyBuffer);
    assert(puUsedSize);
    LOG_PROCESS_ERROR(uBufferSize >= sizeof(KTONG_DB_BASEINFO));

    pBaseInfo = (KTONG_DB_BASEINFO*)pbyBuffer;

    pBaseInfo->dwMaster             = m_dwMaster;
    pBaseInfo->nLevel               = m_nLevel;
	pBaseInfo->bFreeJoin			= m_bFreeJoin;
    pBaseInfo->wMaxMemberCount      = (WORD)m_nMaxMemberCount;
    pBaseInfo->byTongState          = (BYTE)m_eState;
	pBaseInfo->byCamp               = m_byCamp;
    pBaseInfo->nStateTimer          = m_nStateTimer;
	pBaseInfo->dwDiamondNum         = m_dwDiamondNum;
	pBaseInfo->dwAgateNum           = m_dwAgateNum;
	pBaseInfo->dwJadeiteNum         = m_dwJadeiteNum;
	pBaseInfo->dwPearlNum           = m_dwPearlNum;
	pBaseInfo->byApplyerNum         = (BYTE)m_ApplyJoinTongPlayers.size();
	pBaseInfo->byActOpenTimeNum     = (BYTE)m_ActivityOpenTimeMap.size();

    strncpy(pBaseInfo->szName, m_szName, sizeof(pBaseInfo->szName));
    pBaseInfo->szName[sizeof(pBaseInfo->szName) - 1] = '\0';

	pdwApplyerIds = (DWORD*)(pbyBuffer + sizeof(KTONG_DB_BASEINFO));
	for (INT i = 0; i < (INT)m_ApplyJoinTongPlayers.size(); i++)
	{
		pdwApplyerIds[i] = m_ApplyJoinTongPlayers[i];
	}

    pcKinActOpenTimesData = (SyncKinActivityInfo*)(pbyBuffer + sizeof(KTONG_DB_BASEINFO) + pBaseInfo->byApplyerNum * sizeof(DWORD));
	for (it = m_ActivityOpenTimeMap.begin(); it != m_ActivityOpenTimeMap.end(); ++it)
	{
		pcKinActOpenTimesData[nKinActSaveIndex++] = it->second;
	}

    *puUsedSize = sizeof(KTONG_DB_BASEINFO) + pBaseInfo->byApplyerNum * sizeof(DWORD) + pBaseInfo->byActOpenTimeNum * sizeof(SyncKinActivityInfo);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::LoadSchema(BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult         = false;
    KTONG_DB_SCHEMA*    pSchemaSection  = NULL;
    
    assert(pbyData);
    LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_DB_SCHEMA));

    pSchemaSection = (KTONG_DB_SCHEMA*)pbyData;

    memcpy(&m_Schema, pSchemaSection, sizeof(KTongSchemaData));

    for (int nGroupIndex = 0; nGroupIndex < MAX_TONG_GROUP; nGroupIndex++)
    {
        m_Schema.Data[nGroupIndex].szName[sizeof(m_Schema.Data[nGroupIndex].szName) - 1] = '\0';
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::SaveSchema(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize)
{
    BOOL                bResult         = false;
    KTONG_DB_SCHEMA*    pSchemaSection  = NULL;

    assert(pbyBuffer);
    assert(puUsedSize);
    LOG_PROCESS_ERROR(uBufferSize >= sizeof(KTONG_DB_SCHEMA));

    pSchemaSection = (KTONG_DB_SCHEMA*)pbyBuffer;

    memcpy(&pSchemaSection->Schema, &m_Schema, sizeof(pSchemaSection->Schema));

    *puUsedSize = sizeof(KTONG_DB_SCHEMA);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::LoadRoster(BYTE* pbyData, size_t uDataLen)
{
    BOOL                    bResult         = false;
    KTONG_DB_ROSTER*        pRosterSection  = NULL;
    int                     nMemberCount    = 0;
    KTONG_DB_MEMBER_DATA*   pMemberData     = NULL;
    KRole*                  pRole           = NULL;

    assert(pbyData);
    LOG_PROCESS_ERROR(uDataLen >= sizeof(KTONG_DB_ROSTER));

    pRosterSection = (KTONG_DB_ROSTER*)pbyData;
    nMemberCount = pRosterSection->nMemberCount;

    LOG_PROCESS_ERROR(nMemberCount >= 0);
    LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_DB_ROSTER) + sizeof(KTONG_DB_MEMBER_DATA) * nMemberCount);
    
    pMemberData = (KTONG_DB_MEMBER_DATA*)pRosterSection->byData;

    for (int i = 0; i < nMemberCount; i++)
    {
        KTongMember Member;

        pRole = g_pGameCenter->m_RoleManager.GetRole(pMemberData[i].dwID);
        if (pRole == NULL)
        {
            QLogPrintf(
                LOG_INFO, "[Tong] Tong member %u not exist, tong: (%u, %s)\n",
                pMemberData[i].dwID, m_dwID, m_szName
            );
            continue;
        }

        pRole->m_dwTongID       = m_dwID;

        Member.dwID             = pMemberData[i].dwID;
        Member.nGroupID         = pMemberData[i].nGroupID;
        Member.nJoinTime        = pMemberData[i].nJoinTime;
        Member.nLastOfflineTime = pMemberData[i].nLastOfflineTime;
        Member.nSalary          = pMemberData[i].nSalary;
		Member.nDailyContribution = pMemberData[i].nDailyContribution;

        Member.nLevel           = pRole->m_byLevel;
        Member.dwForceID        = pRole->m_dwForceID;
        Member.dwMapID          = pRole->m_dwMapID;
        Member.bIsOnline        = false;
		Member.dwFightScore		= pRole->m_nFightScore;
        
        Member.nLastUpdateFrame = 0;

        strncpy(Member.szRemark, pMemberData[i].szRemark, sizeof(Member.szRemark));
        Member.szRemark[sizeof(Member.szRemark) - 1] = '\0';

        m_Roster[Member.dwID] = Member;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::SaveRoster(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize)
{
    BOOL                    bResult     = false;
    KTONG_DB_ROSTER*        pRosterData = NULL;
    BYTE*                   pbyOffset   = NULL;

    assert(pbyBuffer);
    assert(puUsedSize);
    LOG_PROCESS_ERROR(uBufferSize >= sizeof(KTONG_DB_ROSTER) + sizeof(KTONG_DB_MEMBER_DATA) * m_Roster.size());
    
    pRosterData = (KTONG_DB_ROSTER*)pbyBuffer; 
    
    pRosterData->nMemberCount = (int)m_Roster.size();

    pbyOffset = pRosterData->byData;

    for(KROSTER::iterator Iter = m_Roster.begin(); Iter != m_Roster.end(); ++Iter)
    {
        KTONG_DB_MEMBER_DATA*   pData       = (KTONG_DB_MEMBER_DATA*)pbyOffset;
        const KTongMember&      crMember    = Iter->second;

        pData->dwID             = crMember.dwID;
        pData->nGroupID         = crMember.nGroupID;
        pData->nJoinTime        = crMember.nJoinTime;
        pData->nLastOfflineTime = crMember.nLastOfflineTime;
        pData->nSalary          = crMember.nSalary;
		pData->nDailyContribution = crMember.nDailyContribution;

        strncpy(pData->szRemark, crMember.szRemark, sizeof(pData->szRemark));
        pData->szRemark[sizeof(pData->szRemark) - 1] = '\0';

        pbyOffset += sizeof(KTONG_DB_MEMBER_DATA);
    }

    *puUsedSize =  sizeof(KTONG_DB_ROSTER) + sizeof(KTONG_DB_MEMBER_DATA) * m_Roster.size();

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::LoadMemorabilia(BYTE* pbyData, size_t uDataLen)
{
    BOOL                    bResult         = false;
    KTONG_DB_MEMORABILIA*   pMemorabilia    = NULL;
    BYTE*                   pbyOffset       = pbyData;
    size_t                  uLeftSize       = uDataLen;
    DWORD                   dwRecordCount   = 0;
    size_t                  uRecordSize     = 0;
    KTongIncidentRecord     Record;

    assert(pbyData);
    LOG_PROCESS_ERROR(m_Memorabilia.size() == 0);

    while(uLeftSize > 0)
    {
        LOG_PROCESS_ERROR(uLeftSize > sizeof(KTONG_DB_MEMORABILIA));
        pMemorabilia = (KTONG_DB_MEMORABILIA*)pbyOffset;

        uRecordSize = sizeof(KTONG_DB_MEMORABILIA) + pMemorabilia->byDataLen;
        LOG_PROCESS_ERROR(uLeftSize >= uRecordSize);

        Record.dwID     = ++dwRecordCount;
        Record.nTime    = pMemorabilia->nTime;

        strncpy(Record.szDescription, (const char*)pMemorabilia->byData, sizeof(Record.szDescription));
        Record.szDescription[sizeof(Record.szDescription) -1] = '\0';

        m_Memorabilia.push_back(Record);

        uLeftSize -= uRecordSize;
        pbyOffset += uRecordSize;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::SaveMemorabilia(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize)
{
    BOOL                    bResult         = false;
    BYTE*                   pbyOffset       = pbyBuffer;
    KTONG_DB_MEMORABILIA*   pMemorabilia    = NULL;
    size_t                  uStrLen         = 0;
    size_t                  uRecordSize     = 0;
    size_t                  uLeftsize       = uBufferSize;

    assert(pbyBuffer);
    assert(puUsedSize);

    for (int i = 0 ; i < (int)m_Memorabilia.size(); i++)
    {
        LOG_PROCESS_ERROR(uLeftsize >= sizeof(KTONG_DB_MEMORABILIA) + TONG_MEMORABILIA_RECORD_LEN);

        pMemorabilia = (KTONG_DB_MEMORABILIA*)pbyOffset;
        pMemorabilia->nTime = m_Memorabilia[i].nTime;

        uStrLen = strlen(m_Memorabilia[i].szDescription) + 1;
        LOG_PROCESS_ERROR(uStrLen <= TONG_MEMORABILIA_RECORD_LEN);

        memcpy(pMemorabilia->byData, m_Memorabilia[i].szDescription, uStrLen);

        pMemorabilia->byDataLen = (BYTE)uStrLen;
        uRecordSize = sizeof(KTONG_DB_MEMORABILIA) + uStrLen;
        
        uLeftsize -= uRecordSize;
        pbyOffset += uRecordSize;
    }

    *puUsedSize = uBufferSize - uLeftsize;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::LoadDevelopmentInfo(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    KTONG_DB_DEVELOPMENT_DATA*  pData       = NULL;

    assert(pbyData);
    LOG_PROCESS_ERROR(uDataLen == sizeof(KTONG_DB_DEVELOPMENT_DATA));
    pData = (KTONG_DB_DEVELOPMENT_DATA*)pbyData;

    m_nFund                 = pData->nFund;
    m_nMaxWageRate          = pData->nMaxWageRate;
    m_nDevelopmentPoint     = pData->nDevelopmentPoint;
    m_nUsedDevelopmentPoint = pData->nUsedDevelopmentPoint;

    LOG_PROCESS_ERROR(sizeof(m_byTechTree) == sizeof(pData->byTechTree));
    memcpy(m_byTechTree, pData->byTechTree, sizeof(m_byTechTree));

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTong::SaveDevelopmentInfo(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize)
{
    BOOL                        bResult = false;
    KTONG_DB_DEVELOPMENT_DATA*  pData   = NULL;

    assert(pbyBuffer);
    assert(puUsedSize);
    LOG_PROCESS_ERROR(uBufferSize >= sizeof(KTONG_DB_DEVELOPMENT_DATA));

    pData = (KTONG_DB_DEVELOPMENT_DATA*)pbyBuffer;
    pData->nFund                = m_nFund;
    pData->nMaxWageRate         = m_nMaxWageRate;
    pData->nDevelopmentPoint    = m_nDevelopmentPoint;
    pData->nUsedDevelopmentPoint = m_nUsedDevelopmentPoint;
    
    assert(sizeof(pData->byTechTree) == sizeof(m_byTechTree));
    memcpy(pData->byTechTree, m_byTechTree, sizeof(pData->byTechTree));
    
    *puUsedSize = sizeof(KTONG_DB_DEVELOPMENT_DATA);

    bResult = true;
EXIT0:
    return bResult;
}
