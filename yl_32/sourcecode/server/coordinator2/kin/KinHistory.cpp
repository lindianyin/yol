#include "stdafx.h"
#include "GameCenter.h"
#include "KinHistory.h"
#include "kinhandler.h"

#define REGISTER_HISTORY_RECORD(RecordType, RecordDataStruct)   \
{                                                               \
    s_uRecordSize[RecordType] = sizeof(RecordDataStruct);       \
}

static size_t s_uRecordSize[hrtTotal];

void KTongHistory::Config()
{
    memset(s_uRecordSize, 0, sizeof(s_uRecordSize));

    REGISTER_HISTORY_RECORD(hrtChangeGroupBaseRight, KTONG_HISTORY_CHANGE_GROUP_BASE_RIGHT);
    REGISTER_HISTORY_RECORD(hrtChangeGroupAdvanceRight, KTONG_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT);
    REGISTER_HISTORY_RECORD(hrtChangeMemberGroup, KTONG_HISTORY_CHANGE_MEMBER_GROUP);
    REGISTER_HISTORY_RECORD(hrtJoin, KTONG_HISTORY_JOIN_OR_QUIT);
    REGISTER_HISTORY_RECORD(hrtQuit, KTONG_HISTORY_JOIN_OR_QUIT);
    REGISTER_HISTORY_RECORD(hrtKickOut, KTONG_HISTORY_KICK_OUT_MEMBER);

    REGISTER_HISTORY_RECORD(hrtChangeGroupWage, KTONG_HISTORY_CHANGE_GROUP_WAGE);
    REGISTER_HISTORY_RECORD(hrtPaySalary, KTONG_HISTORY_PAY_SALARY);
    REGISTER_HISTORY_RECORD(hrtSaveFund, KTONG_HISTORY_SAVE_FUND);

    REGISTER_HISTORY_RECORD(hrtActivateTechtree, KTONG_HISTORY_ACTIVATE_TECHTREE);
    REGISTER_HISTORY_RECORD(hrtProductItem, KTONG_HISTORY_PRODUCT_ITEM);

    REGISTER_HISTORY_RECORD(hrtMissionStart, KTONG_HISTORY_MISSION_START);
    REGISTER_HISTORY_RECORD(hrtMissionComplete, KTONG_HISTORY_MISSION_END);
    REGISTER_HISTORY_RECORD(hrtMissionFailed, KTONG_HISTORY_MISSION_END);
    REGISTER_HISTORY_RECORD(hrtWarStart, KTONG_HISTORY_WAR_START);
    REGISTER_HISTORY_RECORD(hrtWarWin, KTONG_HISTORY_WAR_END);
    REGISTER_HISTORY_RECORD(hrtWarLose, KTONG_HISTORY_WAR_END);
    REGISTER_HISTORY_RECORD(hrtAchievement, KTONG_HISTORY_ACHIEVEMENT);

    REGISTER_HISTORY_RECORD(hrtRepertoryTakeItem, KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM);
    REGISTER_HISTORY_RECORD(hrtRepertoryPutItem, KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM);
}

BOOL KTongHistory::Init()
{
    BOOL bResult = false;

    memset(m_dwIndex, 0, sizeof(m_dwIndex));
    
    bResult = true;
// EXIT0:
    return bResult;
}

void KTongHistory::UnInit()
{
    for (int i = 0; i < ohtTotal; i++)
    {
        KTONG_HISTORY_LIST& rList = m_RecordList[i];
        for (KTONG_HISTORY_LIST::iterator it = rList.begin(); it != rList.end(); ++it)
        {
			SAFE_DELETE(*it);
        }
        rList.clear();
    }
}

BOOL KTongHistory::Load(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    BYTE*                       pbyOffset   = pbyData;
    size_t                      uLeftSize   = uDataLen;
    KTONG_HISTORY*              pSrcRecord  = NULL;
    KTONG_HISTORY*              pDstRecord  = NULL;
    KTONG_HISTORY_LIST*         pRecordList = NULL;
    KTONG_DB_HISTORY_CATEGORY*  pCategory   = NULL;
    size_t                      uRecordSize = 0;

    for (int i = 0; i < ohtTotal; i++)
    {
        LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_HISTORY_CATEGORY));
        pCategory = (KTONG_DB_HISTORY_CATEGORY*)pbyOffset;
        uLeftSize -= sizeof(KTONG_DB_HISTORY_CATEGORY);
        pbyOffset += sizeof(KTONG_DB_HISTORY_CATEGORY);

        LOG_PROCESS_ERROR(pCategory->byType < ohtTotal);
        pRecordList = &m_RecordList[pCategory->byType];

        LOG_PROCESS_ERROR(pRecordList->empty());

        for (int j = 0 ; j < (int)pCategory->wRecordCount; j++)
        {
            LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_HISTORY));
            pSrcRecord = (KTONG_HISTORY*)pbyOffset;
            LOG_PROCESS_ERROR(pSrcRecord->byRecordType < hrtTotal);

            uRecordSize = s_uRecordSize[pSrcRecord->byRecordType];
            LOG_PROCESS_ERROR(uRecordSize);
            LOG_PROCESS_ERROR(uLeftSize >= uRecordSize);

            uLeftSize -= uRecordSize;
            pbyOffset += uRecordSize;

            pDstRecord = (KTONG_HISTORY*)malloc(uRecordSize);
            LOG_PROCESS_ERROR(pDstRecord);

            memcpy(pDstRecord, pSrcRecord, uRecordSize);

			assert(s_uHistoryListMaxSize[pCategory->byType] > 0);
			if (pRecordList->size() >= s_uHistoryListMaxSize[0])
			{
				pRecordList->pop_front();
			}

			pRecordList->push_back(pDstRecord);
			m_dwIndex[pCategory->byType]++;  // 只使用一个历史列表

            //AddRecord((KTONG_OPERATION_HISTORY_TYPE)pCategory->byType, pDstRecord);
            pDstRecord = NULL;
        }
    }

    bResult = true;
EXIT0:
    if (!bResult)
    {
        SAFE_DELETE(pDstRecord);
    }
    return bResult;
}

BOOL KTongHistory::Sava(BYTE* pbyBuffer, size_t uBufferSize, size_t* puRetUsedSize)
{
    BOOL                        bResult     = false;
    BYTE*                       pbyOffset   = pbyBuffer;
    size_t                      uLeftSize   = uBufferSize;
    size_t                      uRecordSize = 0;
    KTONG_HISTORY_LIST*         pRecordList = NULL;
    KTONG_DB_HISTORY_CATEGORY*  pCategory   = NULL;

    assert(pbyOffset);
    assert(uBufferSize);
    assert(puRetUsedSize);

    for (int i = 0; i < ohtTotal; i++)
    {
        pRecordList = &m_RecordList[i];

        LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_DB_HISTORY_CATEGORY));
        pCategory = (KTONG_DB_HISTORY_CATEGORY*)pbyOffset;
        pbyOffset += sizeof(KTONG_DB_HISTORY_CATEGORY);
        uLeftSize -= sizeof(KTONG_DB_HISTORY_CATEGORY);

        pCategory->byType = (BYTE)i;
        pCategory->wRecordCount = (WORD)pRecordList->size();
        
        for (KTONG_HISTORY_LIST::const_iterator it = pRecordList->begin(); it != pRecordList->end(); ++it)
        {
            KTONG_HISTORY* pRecord = *it;
            LOG_PROCESS_ERROR(pRecord);
            LOG_PROCESS_ERROR(pRecord->byRecordType < hrtTotal);

            uRecordSize = s_uRecordSize[pRecord->byRecordType];
            LOG_PROCESS_ERROR(uRecordSize);

            LOG_PROCESS_ERROR(uRecordSize <= uLeftSize);
            memcpy(pbyOffset, pRecord, uRecordSize);
            pbyOffset += uRecordSize;
            uLeftSize -= uRecordSize;
        }
    }

    *puRetUsedSize = uBufferSize - uLeftSize;

    bResult = true;
EXIT0:
    return bResult;
}

void KTongHistory::AddRecord(KTONG_OPERATION_HISTORY_TYPE eType, KTONG_HISTORY* pRecord)
{
    KTONG_HISTORY_LIST* pHistoryList = NULL;

    assert(pRecord);

    pRecord->nTime = g_pGameCenter->m_nTimeNow;
    pHistoryList = &m_RecordList[0]; // 不区分记录类型，统一一个队列

    assert(s_uHistoryListMaxSize[0] > 0);
    if (pHistoryList->size() >= s_uHistoryListMaxSize[0])
    {
        pHistoryList->pop_front();
    }

    pHistoryList->push_back(pRecord);
    m_dwIndex[0]++;  // 只使用一个历史列表
}

// ------------ 人事变动 ----------------
BOOL KTongHistory::RecordChangeGroupBaseRight(DWORD dwOperator, int nTargetGroup, int nRightOffset, BOOL bIsOpen)
{
    BOOL                                    bResult = false;
    KTONG_HISTORY_CHANGE_GROUP_BASE_RIGHT*  pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(nTargetGroup >= 0 && nTargetGroup < MAX_TONG_GROUP);
    LOG_PROCESS_ERROR(nRightOffset >= 0 && nRightOffset < TONG_OPERATION_NUM);

    pRecord = new KTONG_HISTORY_CHANGE_GROUP_BASE_RIGHT();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtChangeGroupBaseRight;
    pRecord->dwOperator     = dwOperator;
    pRecord->byTargetGroup  = (BYTE)nTargetGroup;
    pRecord->byRightOffset  = (BYTE)nRightOffset;
    pRecord->byIsOpen       = bIsOpen ? 1 : 0;

    AddRecord(ohtMemberManage, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
    {
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordChangeGroupAdvanceRight(DWORD dwOperator, int nSrcGroup, int nDstGroup, int nRightOffset, BOOL bIsOpen)
{
    BOOL                                        bResult = false;
    KTONG_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT*   pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(nSrcGroup >= 0 && nSrcGroup < MAX_TONG_GROUP);
    LOG_PROCESS_ERROR(nDstGroup >= 0 && nDstGroup < MAX_TONG_GROUP);
    LOG_PROCESS_ERROR(nRightOffset >= 0 && nRightOffset < TONG_OPERATION_NUM);

    pRecord = new KTONG_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtChangeGroupAdvanceRight;
    pRecord->dwOperator     = dwOperator;
    pRecord->bySrcGroup     = (BYTE)nSrcGroup;
    pRecord->byDstGroup     = (BYTE)nDstGroup;
    pRecord->byRightOffset  = (BYTE)nRightOffset;
    pRecord->bIsOpen        = bIsOpen ? 1 : 0;

    AddRecord(ohtMemberManage, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
    {
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordChangeMemberGroup(DWORD dwOperator, DWORD dwTargetMember, int nSrcGroup, int nDstGroup)
{
    BOOL                                bResult = false;
    KTONG_HISTORY_CHANGE_MEMBER_GROUP*  pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(dwTargetMember != KD_BAD_ID);
    LOG_PROCESS_ERROR(nSrcGroup >= 0 && nSrcGroup < MAX_TONG_GROUP);
    LOG_PROCESS_ERROR(nDstGroup >= 0 && nDstGroup < MAX_TONG_GROUP);

    pRecord = new KTONG_HISTORY_CHANGE_MEMBER_GROUP();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtChangeMemberGroup;
    pRecord->dwOperator     = dwOperator;
    pRecord->dwTargetMember = dwTargetMember;
    pRecord->bySrcGroup     = (BYTE)nSrcGroup;
    pRecord->byDstGroup     = (BYTE)nDstGroup;
    
    AddRecord(ohtMemberManage, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordJoin(DWORD dwMember)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_JOIN_OR_QUIT* pRecord = NULL;

    LOG_PROCESS_ERROR(dwMember != KD_BAD_ID);

    pRecord = new KTONG_HISTORY_JOIN_OR_QUIT();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType = hrtJoin;
    pRecord->dwMemberID = dwMember;

    AddRecord(ohtMemberManage, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordQuit(DWORD dwMember)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_JOIN_OR_QUIT* pRecord = NULL;

    LOG_PROCESS_ERROR(dwMember != KD_BAD_ID);

    pRecord = new KTONG_HISTORY_JOIN_OR_QUIT();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType = hrtQuit;
    pRecord->dwMemberID = dwMember;

    AddRecord(ohtMemberManage, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordKickout(DWORD dwOperator, DWORD dwTargetMember)
{
    BOOL                            bResult = false;
    KTONG_HISTORY_KICK_OUT_MEMBER*  pRecord = NULL;
    
    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(dwTargetMember != KD_BAD_ID);

    pRecord = new KTONG_HISTORY_KICK_OUT_MEMBER();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtKickOut;
    pRecord->dwOperator     = dwOperator;
    pRecord->dwTargetMember = dwTargetMember;

    AddRecord(ohtMemberManage, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

// ------------ 资金变动 ----------------
BOOL KTongHistory::RecordChangeGroupWage(DWORD dwOperator, int nTargetGroup, int nOldWage, int nNewWage)
{
    BOOL                                bResult = false;
    KTONG_HISTORY_CHANGE_GROUP_WAGE*    pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(nTargetGroup >= 0 && nTargetGroup < MAX_TONG_GROUP);
    LOG_PROCESS_ERROR(nOldWage >= 0 && nOldWage < MAX_TONG_GROUP);
    LOG_PROCESS_ERROR(nNewWage >= 0);

    pRecord = new KTONG_HISTORY_CHANGE_GROUP_WAGE();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtChangeGroupWage;
    pRecord->dwOperator     = dwOperator;
    pRecord->byTargetGroup  = (BYTE)nTargetGroup;
    pRecord->dwOldWage      = nOldWage;
    pRecord->dwNewWage      = nNewWage;

    AddRecord(ohtFund, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordPaySalary(DWORD dwOperator, int nTotalSalary)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_PAY_SALARY*   pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(nTotalSalary >= 0 && nTotalSalary <= MAX_TONG_FUND_BY_SILVER);

    pRecord = new KTONG_HISTORY_PAY_SALARY();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtPaySalary;
    pRecord->dwOperator     = dwOperator;
    pRecord->dwTotalSalary  = nTotalSalary;

    AddRecord(ohtFund, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordSaveFund(DWORD dwOperator, DWORD dwPealNum, DWORD dwJadeiteNum, DWORD dwAgateNum, DWORD dwDiamondNum)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_SAVE_FUND*    pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(dwPealNum >= 1 || dwJadeiteNum >= 1|| dwAgateNum >= 1 || dwDiamondNum >= 1);

    pRecord = new KTONG_HISTORY_SAVE_FUND();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType = hrtSaveFund;
    pRecord->dwOperator   = dwOperator;
    pRecord->dwPealNum = dwPealNum;
	pRecord->dwJadeiteNum = dwJadeiteNum;
	pRecord->dwAgateNum = dwAgateNum;
	pRecord->dwDiamondNum = dwDiamondNum;

    AddRecord(ohtFund, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

// -------- 科技树和道具出产 ------------  
BOOL KTongHistory::RecordActivateTechTree(DWORD dwOperator, int nNode, int nLevel, int nDevelopmentPoint, int nFund)
{
    BOOL                                bResult = false;
    KTONG_HISTORY_ACTIVATE_TECHTREE*    pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(nNode >= 0 && nNode <= MAX_TONG_TECHTREE_NODE_NUM);
    LOG_PROCESS_ERROR(nLevel > 0 && nLevel <= CHAR_MAX);
    LOG_PROCESS_ERROR(nDevelopmentPoint >= 0);
    LOG_PROCESS_ERROR(nFund >= 0);

    pRecord = new KTONG_HISTORY_ACTIVATE_TECHTREE();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtActivateTechtree;
    pRecord->dwOperator     = dwOperator;
    pRecord->byNode         = (BYTE)nNode;
    pRecord->byLevel        = (BYTE)nLevel;
    pRecord->dwConsumeDevelopmentPoint = nDevelopmentPoint;
    pRecord->dwConsumeFund  = nFund;

    AddRecord(ohtTechTree, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordProductItem(DWORD dwOperator, DWORD dwRecipeID, int nCount, int nDevelopmentPoint, int nFund)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_PRODUCT_ITEM* pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    // LOG_PROCESS_ERROR(dwRecipeID);
    LOG_PROCESS_ERROR(nCount > 0);
    LOG_PROCESS_ERROR(nDevelopmentPoint >= 0);
    LOG_PROCESS_ERROR(nFund >= 0);

    pRecord = new KTONG_HISTORY_PRODUCT_ITEM();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtProductItem;
    pRecord->dwOperator     = dwOperator;
    pRecord->dwRecipeID     = dwRecipeID;
    pRecord->dwCount        = nCount;
    pRecord->dwConsumeDevelopmentPoint = nDevelopmentPoint;
    pRecord->dwConsumeFund  = nFund;

    AddRecord(ohtTechTree, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

// ------------ 帮会活动 ----------------
BOOL KTongHistory::RecordMissionStart(DWORD dwOperator, DWORD dwMission ,time_t nMissionStartTime)
{
    BOOL                            bResult = false;
    KTONG_HISTORY_MISSION_START*    pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    // LOG_PROCESS_ERROR(dwMission);
    // LOG_PROCESS_ERROR(dwMissionStartTime);

    pRecord = new KTONG_HISTORY_MISSION_START();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType       = hrtMissionStart;
    pRecord->dwOperator         = dwOperator;
    pRecord->dwMission          = dwMission;
    pRecord->nMissionStartTime  = nMissionStartTime;

    AddRecord(ohtActivity, pRecord);
    
    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordMissionComplete(DWORD dwMission)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_MISSION_END*  pRecord = NULL;

    // LOG_PROCESS_ERROR(dwMission);

    pRecord = new KTONG_HISTORY_MISSION_END();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtMissionComplete;
    pRecord->dwMission      = dwMission;

    AddRecord(ohtActivity, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordMissionFailed(DWORD dwMission)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_MISSION_END*  pRecord = NULL;

    // LOG_PROCESS_ERROR(dwMission);

    pRecord = new KTONG_HISTORY_MISSION_END();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtMissionFailed;
    pRecord->dwMission      = dwMission;

    AddRecord(ohtActivity, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordWarStart(DWORD dwOperator, DWORD dwEnemyTong)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_WAR_START*    pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    LOG_PROCESS_ERROR(dwEnemyTong != KD_BAD_ID);

    pRecord = new KTONG_HISTORY_WAR_START();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtWarStart;
    pRecord->dwOperator     = dwOperator;
    pRecord->dwEnemyTong    = dwEnemyTong;

    AddRecord(ohtActivity, pRecord);
    
    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordWarWin(DWORD dwEnemyTong)
{
    BOOL                    bResult = false;
    KTONG_HISTORY_WAR_END*  pRecord = NULL;

    LOG_PROCESS_ERROR(dwEnemyTong != KD_BAD_ID);

    pRecord = new KTONG_HISTORY_WAR_END();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtWarWin;
    pRecord->dwEnemyTong    = dwEnemyTong;

    AddRecord(ohtActivity, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordWarLose(DWORD dwEnemyTong)
{
    BOOL                    bResult = false;
    KTONG_HISTORY_WAR_END*  pRecord = NULL;

    LOG_PROCESS_ERROR(dwEnemyTong != KD_BAD_ID);

    pRecord = new KTONG_HISTORY_WAR_END();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtWarLose;
    pRecord->dwEnemyTong    = dwEnemyTong;

    AddRecord(ohtActivity, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordAchievement(DWORD dwAchievement)
{
    BOOL                        bResult = false;
    KTONG_HISTORY_ACHIEVEMENT*  pRecord = NULL;

    // LOG_PROCESS_ERROR(dwAchievement);
    pRecord = new KTONG_HISTORY_ACHIEVEMENT();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtAchievement;
    pRecord->dwAchievement  = dwAchievement;

    AddRecord(ohtActivity, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

// ------------ 帮会仓库 ----------------
BOOL KTongHistory::RecordTakeRepertoryItem(DWORD dwOperator, DWORD dwItemType, DWORD dwItemIndex, int nStackNum)
{
    BOOL                                        bResult = false;
    KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM*   pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    // LOG_PROCESS_ERROR(dwItemIndex > ittInvalid && dwItemIndex < ittTotal);
    // LOG_PROCESS_ERROR(dwItemIndex);
    LOG_PROCESS_ERROR(nStackNum >= 0);

    pRecord = new KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtRepertoryTakeItem;
    pRecord->dwOperator     = dwOperator;
    pRecord->byTabType      = (BYTE)dwItemType;
    pRecord->wTabIndex      = (WORD)dwItemIndex;
    pRecord->wStackNum      = (WORD)nStackNum;

    AddRecord(ohtRepertory, pRecord);
    
    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

BOOL KTongHistory::RecordPutRepertoryItem(DWORD dwOperator, DWORD dwItemType, DWORD dwItemIndex, int nStackNum)
{
    BOOL                                        bResult = false;
    KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM*   pRecord = NULL;

    LOG_PROCESS_ERROR(dwOperator != KD_BAD_ID);
    // LOG_PROCESS_ERROR(dwItemIndex > ittInvalid && dwItemIndex < ittTotal);
    // LOG_PROCESS_ERROR(dwItemIndex);
    LOG_PROCESS_ERROR(nStackNum >= 0);

    pRecord = new KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM();
    LOG_PROCESS_ERROR(pRecord);

    pRecord->byRecordType   = hrtRepertoryPutItem;
    pRecord->dwOperator     = dwOperator;
    pRecord->byTabType      = (BYTE)dwItemType;
    pRecord->wTabIndex      = (WORD)dwItemIndex;
    pRecord->wStackNum      = (WORD)nStackNum;

    AddRecord(ohtRepertory, pRecord);

    bResult = true;
EXIT0:
    if (!bResult && pRecord)
	{
		SAFE_DELETE(pRecord);
    }
    return bResult;
}

static BOOL GetRoleName(char szBuffer[], const size_t uBufferSize, DWORD dwRoleID)
{
    BOOL    bResult = false;
    KRole*  pRole   = NULL;

    assert(szBuffer);
    assert(uBufferSize > 0);

    szBuffer[0] = '\0';
    pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
    PROCESS_ERROR(pRole);

    strncpy(szBuffer, pRole->m_szName, uBufferSize);
    szBuffer[uBufferSize - 1] = '\0';

    bResult = true;
EXIT0:
    return bResult;
}

static BOOL ConvertRecordData(KTONG_HISTORY* pRecord, BYTE* pbyBuffer, size_t uLeftSize, size_t* puRetUsedSize)
{
    BOOL                        bResult     = false;
    KTONG_SYNC_HISTORY*         pSyncHead   = NULL;
    KTONG_HISTORY_RECORD_TYPE   eType       = hrtInvalied;

    assert(pRecord);
    assert(pbyBuffer);
    assert(puRetUsedSize);
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_SYNC_HISTORY));

    PROCESS_ERROR(pRecord->byRecordType > hrtInvalied && pRecord->byRecordType < hrtTotal);

    eType = (KTONG_HISTORY_RECORD_TYPE)pRecord->byRecordType;

    pSyncHead               = (KTONG_SYNC_HISTORY*)pbyBuffer;
    pSyncHead->byRecordType = pRecord->byRecordType;
    pSyncHead->nTime        = pRecord->nTime;

    switch(eType)
    {
    case hrtChangeGroupBaseRight:
        {
            KTONG_HISTORY_CHANGE_GROUP_BASE_RIGHT*      pRecordData = (KTONG_HISTORY_CHANGE_GROUP_BASE_RIGHT*)pRecord;
            KTONG_SYNC_HISTORY_CHANGE_GROUP_BASE_RIGHT* pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_CHANGE_GROUP_BASE_RIGHT) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_CHANGE_GROUP_BASE_RIGHT*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->byTargetGroup    = pRecordData->byTargetGroup;
            pSyncData->byRightOffset    = pRecordData->byRightOffset;
            pSyncData->byIsOpen         = pRecordData->byIsOpen;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_CHANGE_GROUP_BASE_RIGHT);
        }
        break;
    case hrtChangeGroupAdvanceRight: 
        {
            KTONG_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT*       pRecordData = (KTONG_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT*)pRecord;
            KTONG_SYNC_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT*  pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->bySrcGroup       = pRecordData->bySrcGroup;
            pSyncData->byDstGroup       = pRecordData->byDstGroup;
            pSyncData->byRightOffset    = pRecordData->byRightOffset;
            pSyncData->bIsOpen          = pRecordData->bIsOpen;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT);
        }
        break;
    case hrtChangeMemberGroup: 
        {
            KTONG_HISTORY_CHANGE_MEMBER_GROUP*      pRecordData = (KTONG_HISTORY_CHANGE_MEMBER_GROUP*)pRecord;
            KTONG_SYNC_HISTORY_CHANGE_MEMBER_GROUP* pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_CHANGE_MEMBER_GROUP) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_CHANGE_MEMBER_GROUP*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            GetRoleName(pSyncData->szTargetMember, sizeof(pSyncData->szTargetMember), pRecordData->dwTargetMember);
            pSyncData->bySrcGroup = pRecordData->bySrcGroup;
            pSyncData->byDstGroup = pRecordData->byDstGroup;
            
            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_CHANGE_MEMBER_GROUP);
        }
        break;
    case hrtJoin:
    case hrtQuit:
        {
            KTONG_HISTORY_JOIN_OR_QUIT*         pRecordData = (KTONG_HISTORY_JOIN_OR_QUIT*)pRecord;
            KTONG_SYNC_HISTORY_JOIN_OR_QUIT*    pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_JOIN_OR_QUIT) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_JOIN_OR_QUIT*)pSyncHead;

            GetRoleName(pSyncData->szMember, sizeof(pSyncData->szMember), pRecordData->dwMemberID);

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_JOIN_OR_QUIT);
        }
        break;
    case hrtKickOut:
        {
            KTONG_HISTORY_KICK_OUT_MEMBER*      pRecordData = (KTONG_HISTORY_KICK_OUT_MEMBER*)pRecord;
            KTONG_SYNC_HISTORY_KICK_OUT_MEMBER* pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_KICK_OUT_MEMBER) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_KICK_OUT_MEMBER*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            GetRoleName(pSyncData->szTargetMember, sizeof(pSyncData->szTargetMember), pRecordData->dwTargetMember);

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_KICK_OUT_MEMBER);
        }
        break;

    case hrtChangeGroupWage:
        {
            KTONG_HISTORY_CHANGE_GROUP_WAGE*        pRecordData = (KTONG_HISTORY_CHANGE_GROUP_WAGE*)pRecord;
            KTONG_SYNC_HISTORY_CHANGE_GROUP_WAGE*   pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_CHANGE_GROUP_WAGE) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_CHANGE_GROUP_WAGE*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->byTargetGroup    = pRecordData->byTargetGroup;
            pSyncData->dwOldWage        = pRecordData->dwOldWage;
            pSyncData->dwNewWage        = pRecordData->dwNewWage;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_CHANGE_GROUP_WAGE);
        }
        break;
    case hrtPaySalary:
        {
            KTONG_HISTORY_PAY_SALARY*       pRecordData = (KTONG_HISTORY_PAY_SALARY*)pRecord;
            KTONG_SYNC_HISTORY_PAY_SALARY*  pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_PAY_SALARY) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_PAY_SALARY*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->dwTotalSalary = pRecordData->dwTotalSalary;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_PAY_SALARY);
        }
        break;
    case hrtSaveFund:
        {
            KTONG_HISTORY_SAVE_FUND*        pRecordData = (KTONG_HISTORY_SAVE_FUND*)pRecord;
            KTONG_SYNC_HISTORY_SAVE_FUND*   pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_SAVE_FUND) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_SAVE_FUND*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->dwSavePealNum = pRecordData->dwPealNum;
			pSyncData->dwSaveJadeiteNum = pRecordData->dwJadeiteNum;
			pSyncData->dwSaveAgateNum = pRecordData->dwAgateNum;
			pSyncData->dwSaveDiamondNum = pRecordData->dwDiamondNum;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_SAVE_FUND);
        }
        break;
    case hrtActivateTechtree: 
        {
            KTONG_HISTORY_ACTIVATE_TECHTREE*        pRecordData = (KTONG_HISTORY_ACTIVATE_TECHTREE*)pRecord;
            KTONG_SYNC_HISTORY_ACTIVATE_TECHTREE*   pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_ACTIVATE_TECHTREE) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_ACTIVATE_TECHTREE*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->byNode   = pRecordData->byNode;
            pSyncData->byLevel  = pRecordData->byLevel;
            pSyncData->dwConsumeDevelopmentPoint = pRecordData->dwConsumeDevelopmentPoint;
            pSyncData->dwConsumeFund = pRecordData->dwConsumeFund;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_ACTIVATE_TECHTREE);
        }
        break;
    case hrtProductItem:
        {
            KTONG_HISTORY_PRODUCT_ITEM*         pRecordData = (KTONG_HISTORY_PRODUCT_ITEM*)pRecord;
            KTONG_SYNC_HISTORY_PRODUCT_ITEM*    pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_PRODUCT_ITEM) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_PRODUCT_ITEM*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->dwRecipeID   = pRecordData->dwRecipeID;
            pSyncData->dwCount      = pRecordData->dwCount;
            pSyncData->dwConsumeDevelopmentPoint = pRecordData->dwConsumeDevelopmentPoint;
            pSyncData->dwConsumeFund = pRecordData->dwConsumeFund;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_PRODUCT_ITEM);
        }
        break;
    case hrtMissionStart:
        {
            KTONG_HISTORY_MISSION_START*        pRecordData = (KTONG_HISTORY_MISSION_START*)pRecord;
            KTONG_SYNC_HISTORY_MISSION_START*   pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_MISSION_START) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_MISSION_START*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            pSyncData->dwMission = pRecordData->dwMission;
            pSyncData->nMissionStartTime = pRecordData->nMissionStartTime; 

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_MISSION_START);
        }
        break;
    case hrtMissionComplete:
    case hrtMissionFailed: 
        {
            KTONG_HISTORY_MISSION_END*      pRecordData = (KTONG_HISTORY_MISSION_END*)pRecord;
            KTONG_SYNC_HISTORY_MISSION_END* pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_MISSION_END) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_MISSION_END*)pSyncHead;

            pSyncData->dwMission = pRecordData->dwMission;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_MISSION_END);
        }
        break;
    case hrtWarStart:
        {
            KTong*                          pTong       = NULL;
            KTONG_HISTORY_WAR_START*        pRecordData = (KTONG_HISTORY_WAR_START*)pRecord;
            KTONG_SYNC_HISTORY_WAR_START*   pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_WAR_START) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_WAR_START*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);
            
            pSyncData->szEnemyTong[0] = '\0';
            pTong = g_pGameCenter->m_TongManager.GetTong(pRecordData->dwEnemyTong);
            if (pTong)
            {
                strncpy(pSyncData->szEnemyTong, pTong->m_szName, sizeof(pSyncData->szEnemyTong));
                pSyncData->szEnemyTong[sizeof(pSyncData->szEnemyTong) - 1] = '\0';
            }

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_WAR_START);
        }
        break;
    case hrtWarWin:
    case hrtWarLose: 
        {
            KTong*                      pTong       = NULL;
            KTONG_HISTORY_WAR_END*      pRecordData = (KTONG_HISTORY_WAR_END*)pRecord; 
            KTONG_SYNC_HISTORY_WAR_END* pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_WAR_END) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_WAR_END*)pSyncHead;

            pSyncData->szEnemyTong[0] = '\0';
            pTong = g_pGameCenter->m_TongManager.GetTong(pRecordData->dwEnemyTong);
            if (pTong)
            {
                strncpy(pSyncData->szEnemyTong, pTong->m_szName, sizeof(pSyncData->szEnemyTong));
                pSyncData->szEnemyTong[sizeof(pSyncData->szEnemyTong) - 1] = '\0';
            }

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_WAR_END);
        }
        break;
    case hrtAchievement: 
        {
            KTONG_HISTORY_ACHIEVEMENT*      pRecordData = (KTONG_HISTORY_ACHIEVEMENT*)pRecord;
            KTONG_SYNC_HISTORY_ACHIEVEMENT* pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_ACHIEVEMENT) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_ACHIEVEMENT*)pSyncHead;

            pSyncData->dwAchievement = pRecordData->dwAchievement;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_ACHIEVEMENT);
        }
        break;
    case hrtRepertoryTakeItem:
    case hrtRepertoryPutItem: 
        {
            KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM*       pRecordData = (KTONG_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM*)pRecord;
            KTONG_SYNC_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM*  pSyncData   = NULL;

            LOG_PROCESS_ERROR(sizeof(KTONG_SYNC_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM) <= uLeftSize);
            pSyncData = (KTONG_SYNC_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM*)pSyncHead;

            GetRoleName(pSyncData->szOperator, sizeof(pSyncData->szOperator), pRecordData->dwOperator);

            pSyncData->byTabType = pRecordData->byTabType;
            pSyncData->wTabIndex = pRecordData->wTabIndex;
            pSyncData->wStackNum = pRecordData->wStackNum;

            *puRetUsedSize = sizeof(KTONG_SYNC_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM);
        }
        break;
    default:
        LOG_PROCESS_ERROR(0);
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTongHistory::SyncRecord(DWORD dwPlayerID, DWORD dwRangeButton, DWORD dwRangeTop)
{
    BOOL                bResult             = false;
    BOOL                bRetCode            = false;
    int                 nSyncRecordCount    = 0;
    DWORD               dwHeadRecord        = 0;
    DWORD               dwTailRecord        = 0;
    KTONG_HISTORY_LIST* pHistoryList        = NULL;
    int                 nCount              = 0;
    size_t              uLeftSize           = 0;
    size_t              uUsedSize           = 0;
    size_t              uTotalSize          = 0;
    BYTE*               pbyOffset           = NULL;
    int                 nRecordNum          = 0;
    BYTE                byBuffer[1024];

    //LOG_PROCESS_ERROR(eType >= 0 && eType < ohtTotal);
    LOG_PROCESS_ERROR(dwRangeTop >= dwRangeButton);

    pHistoryList = &m_RecordList[0];
    dwHeadRecord = m_dwIndex[0];
    nRecordNum = (int)pHistoryList->size();
    LOG_PROCESS_ERROR(dwHeadRecord >= (DWORD)nRecordNum);
    PROCESS_ERROR(nRecordNum > 0);
    dwTailRecord = dwHeadRecord + 1 - nRecordNum;

	dwRangeTop = dwRangeTop <= dwHeadRecord ? dwRangeTop : dwHeadRecord;
	dwRangeTop = dwRangeTop >= dwTailRecord ? dwRangeTop : dwTailRecord;

    nSyncRecordCount = dwRangeTop - dwRangeButton + 1;
    LOG_PROCESS_ERROR(nSyncRecordCount <= TONG_OPERATION_HISTORY_PAGE_SIZE);

    if (dwRangeButton < dwTailRecord)
    {
        dwRangeButton = dwTailRecord;
    }

    nSyncRecordCount = dwRangeTop - dwRangeButton + 1;
    assert(nSyncRecordCount <= nRecordNum);

    nCount      = 0;
    pbyOffset   = byBuffer;
    uLeftSize   = sizeof(byBuffer);
    
    for(KTONG_HISTORY_LIST::iterator it = pHistoryList->end() - nSyncRecordCount; it != pHistoryList->end(); ++it)
    {
        KTONG_HISTORY* pRecord = *it;
        assert(pRecord);

        uUsedSize = 0;
        bRetCode = ConvertRecordData(pRecord, pbyOffset, uLeftSize, &uUsedSize);
        LOG_PROCESS_ERROR(bRetCode);

        LOG_PROCESS_ERROR(uLeftSize >= uUsedSize);
        uLeftSize -= uUsedSize;
        pbyOffset += uUsedSize;

        nCount++;
        if (nCount == nSyncRecordCount)
            break;
    }

    LOG_PROCESS_ERROR(uLeftSize <= sizeof(byBuffer));
    uTotalSize = sizeof(byBuffer) - uLeftSize;

    bRetCode = KinHandler::DoSyncTongHistoryRespond(dwPlayerID, 0, dwRangeButton, nSyncRecordCount, byBuffer, uTotalSize);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

