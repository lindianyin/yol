#include "stdafx.h"
#include "ThreatList.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/knpcmgr.h"
#include "../onlinegameworld/scene/kregion.h"
#include "../onlinegameworld/scene/kscene.h"
#include "onlinegamemodule/ai/kaivm.h"


#ifdef GAME_SERVER

BOOL KThreatLink::AddMember(DWORD dwThreatLinkID, KCharacter* pCharacter)
{
    BOOL                    bResult = false;
    MEMBER_VECTOR&          rLink   = m_ThreatLinkMap[dwThreatLinkID];
    MEMBER_VECTOR::iterator it;

    PROCESS_ERROR(dwThreatLinkID);
    assert(pCharacter);

    it = std::find(rLink.begin(), rLink.end(), pCharacter->m_dwId);
    PROCESS_SUCCESS(it != rLink.end());

    rLink.push_back(pCharacter->m_dwId);

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KThreatLink::DelMember(DWORD dwThreatLinkID, KCharacter* pCharacter)
{
    BOOL                    bResult = false;
    MEMBER_VECTOR&          rLink   = m_ThreatLinkMap[dwThreatLinkID];
    MEMBER_VECTOR::iterator it;

    PROCESS_ERROR(dwThreatLinkID);
    assert(pCharacter);

    it = std::find(rLink.begin(), rLink.end(), pCharacter->m_dwId);
    PROCESS_SUCCESS(it == rLink.end());

    rLink.erase(it);

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KThreatLink::NewThreat(DWORD dwThreatLinkID, KCharacter* pCharacter)
{
    BOOL                    bResult = false;
    MEMBER_VECTOR&          rLink   = m_ThreatLinkMap[dwThreatLinkID];
    MEMBER_VECTOR::iterator itEnd   = rLink.end();
    MEMBER_VECTOR::iterator it;

    PROCESS_ERROR(dwThreatLinkID);
    assert(pCharacter);

    for (it = rLink.begin(); it != itEnd; ++it)
    {
        DWORD       dwCharacterID   = *it;
        KCharacter* pDstCharacter   = NULL;

        if (IS_NPC(dwCharacterID))
            pDstCharacter = g_cNpcMgr.GetById(dwCharacterID);
        else
            pDstCharacter = g_cPlayerMgr.GetById(dwCharacterID);

        if (!pDstCharacter)
            continue;

        if (!pDstCharacter->GetScene())
            continue;

        if (pDstCharacter->m_eMoveState == cmsOnDeath)
            continue;

        pDstCharacter->m_SimpThreatList.ModifyThreat(thtSpecialThreat, pCharacter, 0);
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KThreatLink::SyncAllThreat(DWORD dwThreatLinkID, KCharacter* pDstCharacter)
{
    BOOL                    bResult = false;
    MEMBER_VECTOR&          rLink   = m_ThreatLinkMap[dwThreatLinkID];
    MEMBER_VECTOR::iterator itEnd   = rLink.end();
    MEMBER_VECTOR::iterator it;

    PROCESS_ERROR(dwThreatLinkID);
    assert(pDstCharacter);

    for (it = rLink.begin(); it != itEnd; ++it)
    {
        DWORD                   dwCharacterID       = *it;
        KCharacter*             pCharacter          = NULL;
        const KSimpThreatNode*  cpSimpThreatNode    = NULL;

        if (IS_NPC(dwCharacterID))
            pCharacter = g_cNpcMgr.GetById(dwCharacterID);
        else
            pCharacter = g_cPlayerMgr.GetById(dwCharacterID);

        if (!pCharacter || pCharacter->m_eMoveState == cmsOnDeath)
            continue;

        cpSimpThreatNode = pCharacter->m_SimpThreatList.GetFirstThreat(thtMainThreat);
        while (cpSimpThreatNode)
        {
            if (cpSimpThreatNode->pCharacter)
                pDstCharacter->m_SimpThreatList.ModifyThreat(thtSpecialThreat, cpSimpThreatNode->pCharacter, 0);

            cpSimpThreatNode = pCharacter->m_SimpThreatList.GetNextThreat(thtMainThreat, cpSimpThreatNode);
        }
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KThreatLink::ClearAllMemberThreat(DWORD dwThreatLinkID)
{
    BOOL                    bResult     = false;
    BOOL                    bRetCode    = false;
    MEMBER_VECTOR&          rLink       = m_ThreatLinkMap[dwThreatLinkID];
    MEMBER_VECTOR::iterator itEnd       = rLink.end();
    MEMBER_VECTOR::iterator it;

    PROCESS_ERROR(dwThreatLinkID);

    for (it = rLink.begin(); it != itEnd; ++it)
    {
        DWORD           dwCharacterID   = *it;
        KCharacter*     pCharacter      = NULL;

        if (IS_NPC(dwCharacterID))
            pCharacter = g_cNpcMgr.GetById(dwCharacterID);
        else
            pCharacter = g_cPlayerMgr.GetById(dwCharacterID);

        if (!pCharacter || pCharacter->m_eMoveState == cmsOnDeath)
            continue;

        bRetCode = pCharacter->m_SimpThreatList.ClearAllThreat();
        LOG_CHECK_ERROR(bRetCode);
    }

    bResult = true;
EXIT0:
    return bResult;
}

#endif // _SERVER 

#ifdef _CLIENT

KThreatList::KThreatList(void)
{
	m_pSelf = NULL;
}

BOOL KThreatList::Init(KCharacter* pSelf)
{
    BOOL bResult = false;

	LOG_PROCESS_ERROR(pSelf);
	LOG_PROCESS_ERROR(m_ThreatList.IsEmpty());
	LOG_PROCESS_ERROR(m_ThreatenList.IsEmpty());

	m_pSelf = pSelf;

	bResult = true;
EXIT0:
	return bResult;
}

void KThreatList::UnInit(void)
{	
	BOOL				bRetCode		= false;
	KThreatLiteNode*	pThreatNode		= NULL;
	KThreatLiteNode*	pThreatenNode	= NULL;

	pThreatNode = (KThreatLiteNode*)m_ThreatList.GetHead();
	while (pThreatNode)
	{
		KThreatLiteNode* pNext = (KThreatLiteNode*)pThreatNode->GetNext();

		bRetCode = ClearThreat(pThreatNode);
		LOG_CHECK_ERROR(bRetCode);

		pThreatNode = pNext;
	}

	pThreatenNode = (KThreatLiteNode*)m_ThreatenList.GetHead();
	while (pThreatenNode)
	{
		KThreatLiteNode* pNext = (KThreatLiteNode*)pThreatenNode->GetNext();

		bRetCode = ClearThreat(pThreatenNode);
		LOG_CHECK_ERROR(bRetCode);

		pThreatenNode = pNext;
	}

	return;
}

BOOL KThreatList::ModifyThreat(DWORD dwCharacterID)
{
    BOOL bResult  = false;
	BOOL bRetCode = false;
	KThreatLiteNode* pNode = NULL;

	LOG_PROCESS_ERROR(dwCharacterID != ERROR_ID);

	bRetCode = IsInThreatList(dwCharacterID);

	if (!bRetCode)
	{
		pNode = KMemory::New<KThreatLiteNode>();
		pNode->dwCharacterID = dwCharacterID;
		m_ThreatList.AddTail(pNode);
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KThreatList::ModifyThreaten(DWORD dwCharacterID)
{
    BOOL bResult  = false;
	BOOL bRetCode = false;
	KThreatLiteNode* pNode = NULL;

	LOG_PROCESS_ERROR(dwCharacterID != ERROR_ID);

	bRetCode = IsInThreatenList(dwCharacterID);

	if (!bRetCode)
	{
		pNode = KMemory::New<KThreatLiteNode>();
		pNode->dwCharacterID = dwCharacterID;
		m_ThreatenList.AddTail(pNode);
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KThreatList::ClearThreat(DWORD dwCharacterID)
{
    BOOL                bResult     = false;
	BOOL                bRetCode    = false;
	KThreatLiteNode*    pNode       = NULL;
    KThreatLiteNode*    pNext       = NULL;

	pNode = (KThreatLiteNode*)m_ThreatList.GetHead();
	while (pNode)
	{
        pNext = (KThreatLiteNode*)pNode->GetNext();

		if (pNode->dwCharacterID == dwCharacterID)
		{
			bRetCode = ClearThreat(pNode);
			LOG_PROCESS_ERROR(bRetCode);
			break;
		}

		pNode = pNext;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KThreatList::ClearThreaten(DWORD dwCharacterID)
{
    BOOL                bResult     = false;
	BOOL                bRetCode    = false;
	KThreatLiteNode*    pNode       = NULL;
    KThreatLiteNode*    pNext       = NULL;

	pNode = (KThreatLiteNode*)m_ThreatenList.GetHead();
	while (pNode)
	{
        pNext = (KThreatLiteNode*)pNode->GetNext();

		if (pNode->dwCharacterID == dwCharacterID)
		{
            bRetCode = ClearThreat(pNode);
			LOG_PROCESS_ERROR(bRetCode);
			break;
		}

		pNode = pNext;
	}

	bResult = true;
EXIT0:
	return bResult;
}

const KThreatLiteNode* KThreatList::GetFirstThreat()
{
	KThreatLiteNode* pNode = NULL;

	pNode = (KThreatLiteNode*)m_ThreatList.GetHead();

	return pNode;
}

const KThreatLiteNode* KThreatList::GetFirstThreaten()
{
	KThreatLiteNode* pNode = NULL;

    pNode = (KThreatLiteNode*)m_ThreatenList.GetHead();   
    
    return pNode;
}

BOOL KThreatList::ClearThreat(KThreatLiteNode* pNode)
{
	LOG_PROCESS_ERROR(pNode);
	
	pNode->Remove();
    KMemory::Delete(pNode);
    pNode = NULL;

	return true;
EXIT0:
	return false;
}

BOOL KThreatList::IsInThreatList(DWORD dwCharacterID)
{
	KThreatLiteNode *pNode = NULL;

	LOG_PROCESS_ERROR(dwCharacterID != ERROR_ID);

	pNode = (KThreatLiteNode*)m_ThreatList.GetHead();
	while (pNode)
	{
		if (pNode->dwCharacterID == dwCharacterID)
			return true;

		pNode = (KThreatLiteNode*)pNode->GetNext();
	}

EXIT0:
	return false;
}

BOOL KThreatList::IsInThreatenList(DWORD dwCharacterID)
{
	KThreatLiteNode *pNode = NULL;

	LOG_PROCESS_ERROR(dwCharacterID != ERROR_ID);

	pNode = (KThreatLiteNode*)m_ThreatenList.GetHead();
	while (pNode)
	{
		if (pNode->dwCharacterID == dwCharacterID)
			return true;

		pNode = (KThreatLiteNode*)pNode->GetNext();
	}
EXIT0:
	return false;
}

#endif // _CLIENT

//////////////////////////////////////////////////////////////////////////

#ifdef GAME_SERVER

KSimpThreatNode::KSimpThreatNode()
{
    pCharacter              = NULL;
    pPairNode               = NULL;
    nInitFrame              = g_cOrpgWorld.m_dwLogicFrames;
    nLastRefreshFrame       = nInitFrame;
    nLastInThreatFieldFrame = nInitFrame;
    bTopFlag                = false;
    
    for (int i = thtInvalid + 1; i < thtTotal; i++)
        nThreat[i] = 0;
}

KSimpThreatList::KSimpThreatList(KCharacter& rcCharacter) : KCharacterCtrlBase(rcCharacter)
{
    m_pSelf                 = NULL;

    m_nNextCheckLoop        = 0;

    m_nThreatPointX         = 0;
    m_nThreatPointY         = 0;

    m_nBattleZoneCenterX    = 0;
    m_nBattleZoneCenterY    = 0;
    m_nBattleZoneCenterZ    = 0;
    m_nBattleZoneRadius     = 0;
    m_nBattleZoneHeight     = 0;

	m_pSelf = &rcCharacter;
    m_nMainThreatSum        = 0;
}

//BOOL KSimpThreatList::Init(KCharacter* pSelf)
//{
//    assert(pSelf);
//
//    m_pSelf = pSelf;
//    m_nMainThreatSum = 0;
//
//    return true;
//}

BOOL KSimpThreatList::Init()
{
	m_nMainThreatSum = 0;
	return true;
}


BOOL KSimpThreatList::UnInit()
{
    ClearAllThreat();
    ClearAllThreaten();

    //m_pSelf = NULL;
	return TRUE;
}

BOOL KSimpThreatList::ModifyThreat(THREAT_TYPE eThreatType, KCharacter* pCharacter, int nThreat)
{
    BOOL                    bResult             = false;
    int                     nOldMainThreat      = 0;
    KSimpThreatNode*        pSimpThreatNode     = NULL;
    const KSimpThreatNode*  cpTempThreatNode    = NULL;

	assert(m_pSelf);

    LOG_PROCESS_ERROR(eThreatType > thtInvalid);
    LOG_PROCESS_ERROR(eThreatType < thtTotal);
    LOG_PROCESS_ERROR(pCharacter);
    PROCESS_ERROR(pCharacter->m_dwId != m_pSelf->m_dwId);

    // 自己已经被移除了，就不要再加仇恨了，否则仇恨表不Active会导致别人出不了战斗。
    PROCESS_SUCCESS(!m_pSelf->GetScene());

    LOG_PROCESS_ERROR(m_pSelf->m_pRegion);

    // 不仇恨已经死了的人。这样，对方死了之后，即使DOT跳也不会进战斗了。
    PROCESS_SUCCESS(pCharacter->m_eMoveState == cmsOnDeath);

    PROCESS_SUCCESS(pCharacter->m_pScene != m_pSelf->m_pScene);

	PROCESS_SUCCESS(m_pSelf->IsSkillHidden(pCharacter)); // 是否对方对自己隐身

    nThreat = pCharacter->AdjustThreat(eThreatType, nThreat);

    pSimpThreatNode = _FindThreat(pCharacter);
    if (!pSimpThreatNode)
    {
        PROCESS_SUCCESS(nThreat < 0);

        pSimpThreatNode = new KSimpThreatNode();
        assert(pSimpThreatNode);

        pSimpThreatNode->pCharacter         = pCharacter;
        pSimpThreatNode->nInitFrame         = g_cOrpgWorld.m_dwLogicFrames;
        pSimpThreatNode->nLastRefreshFrame  = g_cOrpgWorld.m_dwLogicFrames;

        // 判断谁先动手
        cpTempThreatNode = pCharacter->m_SimpThreatList.FindThreat(m_pSelf);
        if (cpTempThreatNode)
            pSimpThreatNode->bPrimacord = !cpTempThreatNode->bPrimacord;
        else
            pSimpThreatNode->bPrimacord = true;

        for (int i = thtInvalid + 1; i < thtTotal; i++)
            m_SortedList[i].AddTail(&(pSimpThreatNode->SortedListNode[i]));

        pSimpThreatNode->pPairNode = new KSimpThreatenNode();
        assert(pSimpThreatNode->pPairNode);

        pSimpThreatNode->pPairNode->pCharacter = m_pSelf;
        pSimpThreatNode->pPairNode->pPairNode = pSimpThreatNode;

        pCharacter->m_SimpThreatList.m_ThreatenList.AddTail(pSimpThreatNode->pPairNode);

        if (IS_NPC(m_pSelf->m_dwId))
        {
            KNpc* pNpc = (KNpc*)m_pSelf;

            if (pNpc->m_nNpcTeamID && pNpc->m_pScene)
                pNpc->m_pScene->m_ThreatLink.NewThreat(pNpc->m_nNpcTeamID, pCharacter);
        }

        //if (IS_PLAYER(m_pSelf->m_dwId))
        //    g_PlayerServer.DoModifyThreat(((KPlayer*)m_pSelf)->m_nConnIndex, pCharacter->m_dwId);

        //if (IS_PLAYER(pCharacter->m_dwId))
        //    g_PlayerServer.DoModifyThreaten(((KPlayer*)pCharacter)->m_nConnIndex, m_pSelf->m_dwId);

        if (IS_NPC(m_pSelf->m_dwId))
            OnCharacterEnterNpcThreatList(pCharacter);
        
        if (IS_PLAYER(m_pSelf->m_dwId) && IS_PLAYER(pCharacter->m_dwId))
            OnPlayerEnterPlayerThreatList((KPlayer*)pCharacter);

        m_pSelf->m_pRegion->SetActive();
    }

    nOldMainThreat = pSimpThreatNode->nThreat[thtMainThreat];

    pSimpThreatNode->nThreat[eThreatType] += nThreat;

    if (eThreatType != thtMainThreat)
        pSimpThreatNode->nThreat[thtMainThreat] += nThreat;

    pSimpThreatNode->nLastRefreshFrame = g_cOrpgWorld.m_dwLogicFrames;

    _AdjustThreatList(pSimpThreatNode);

    if (nOldMainThreat > 0)
        m_nMainThreatSum -= nOldMainThreat;

    if (pSimpThreatNode->nThreat[thtMainThreat] > 0)
        m_nMainThreatSum += pSimpThreatNode->nThreat[thtMainThreat];

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::ModifyThreaten(THREAT_TYPE eThreatType, KCharacter* pCharacter, int nThreat)
{
    BOOL                bResult = false;
    KSimpThreatenNode*  pNode   = NULL;

    LOG_PROCESS_ERROR(eThreatType > thtInvalid);
    LOG_PROCESS_ERROR(eThreatType < thtTotal);
    LOG_PROCESS_ERROR(pCharacter);

    pNode = (KSimpThreatenNode*)m_ThreatenList.GetHead();
    while (pNode)
    {
        assert(pNode->pPairNode);
        assert(pNode->pCharacter);

        pNode->pCharacter->m_SimpThreatList.ModifyThreat(eThreatType, pCharacter, nThreat);

        pNode = (KSimpThreatenNode*)pNode->GetNext();
    }

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::ClearThreat(KCharacter* pCharacter)
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = false;
    KSimpThreatNode*    pSimpThreatNode = NULL;

    LOG_PROCESS_ERROR(pCharacter);

    pSimpThreatNode = _FindThreat(pCharacter);
    PROCESS_ERROR(pSimpThreatNode);

    bRetCode = _ClearThreat(pSimpThreatNode);
    LOG_CHECK_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::ClearAllThreat()
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    QNode*  pNext       = NULL;

    pNext = m_SortedList[thtMainThreat].GetHead();
    while (pNext)
    {
        KSimpThreatNode* pSimpThreatNode = KTHREAT_CONTAINING_RECORD(pNext, KSimpThreatNode, SortedListNode[thtMainThreat]);
        pNext = pNext->GetNext();

        bRetCode = _ClearThreat(pSimpThreatNode);
        LOG_CHECK_ERROR(bRetCode);
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::ClearAllThreaten()
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    QNode*  pNext       = NULL;
    
    pNext = m_ThreatenList.GetHead();
    while (pNext)
    {
        KSimpThreatenNode* pSimpThreatenNode = (KSimpThreatenNode*)pNext;
        pNext = pNext->GetNext();

        bRetCode = pSimpThreatenNode->pCharacter->m_SimpThreatList._ClearThreat(pSimpThreatenNode->pPairNode);
        LOG_CHECK_ERROR(bRetCode);
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::StickThreat(KCharacter* pCharacter)
{
    BOOL                bResult = false;
    KSimpThreatNode*    pNode   = NULL;

    LOG_PROCESS_ERROR(pCharacter);

    pNode = _FindThreat(pCharacter);
    if (!pNode)
    {
        ModifyThreat(thtMainThreat, pCharacter, 0);
        pNode = _FindThreat(pCharacter);
    }
    LOG_PROCESS_ERROR(pNode);

    // 由于BUFF机制限制，保证同一时间只有一个仇恨置顶。
    UnstickThreat();

    pNode->bTopFlag = true;

    _AdjustThreatList(pNode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::UnstickThreat()
{
    BOOL                bResult = false;
    KSimpThreatNode*    pNode   = NULL;

    pNode = _GetFirstThreat(thtMainThreat);
    PROCESS_ERROR(pNode);
    PROCESS_ERROR(pNode->bTopFlag);

    pNode->bTopFlag = false;

    _AdjustThreatList(pNode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::ZeroThreat(KCharacter* pCharacter)
{
    BOOL                bResult = false;
    KSimpThreatNode*    pNode   = NULL;

    PROCESS_ERROR(pCharacter);

    pNode = _FindThreat(pCharacter);
    PROCESS_ERROR(pNode);

    if (pNode->nThreat[thtMainThreat] > 0)
        m_nMainThreatSum -= pNode->nThreat[thtMainThreat];

    for (int i = thtInvalid + 1; i < thtTotal; i++)
        pNode->nThreat[i] = 0;

    _AdjustThreatList(pNode);

    bResult = true;
EXIT0:
    return bResult;
}

const KSimpThreatNode* KSimpThreatList::GetFirstThreat(THREAT_TYPE eThreatType)
{
    return _GetFirstThreat(eThreatType);
}

const KSimpThreatNode* KSimpThreatList::GetNextThreat(THREAT_TYPE eThreatType, const KSimpThreatNode* cpThreatNode)
{
    const KSimpThreatNode*  cpResult    = NULL;
    QNode*                  pNode       = NULL;

    LOG_PROCESS_ERROR(eThreatType > thtInvalid);
    LOG_PROCESS_ERROR(eThreatType < thtTotal);
    LOG_PROCESS_ERROR(cpThreatNode);

    pNode = ((KSimpThreatNode*)cpThreatNode)->SortedListNode[eThreatType].GetNext();    // What the fuck! The QNode doesn't accept a const pointer for GetNext()! 
    PROCESS_ERROR(pNode);

    cpResult = KTHREAT_CONTAINING_RECORD(pNode, KSimpThreatNode, SortedListNode[eThreatType]);
EXIT0:
    return cpResult;
}

const KSimpThreatNode* KSimpThreatList::GetNthThreat(THREAT_TYPE eThreatType, int n)
{
    const KSimpThreatNode*  cpResult    = NULL;
    QNode*                  pNode       = NULL;

    LOG_PROCESS_ERROR(eThreatType > thtInvalid);
    LOG_PROCESS_ERROR(eThreatType < thtTotal);
    LOG_PROCESS_ERROR(n > 0);

    pNode = m_SortedList[eThreatType].GetHead();
    PROCESS_ERROR(pNode);

    for (int i = 2; i <= n; i++)
    {
        pNode = pNode->GetNext();
        PROCESS_ERROR(pNode);
    }

    cpResult = KTHREAT_CONTAINING_RECORD(pNode, KSimpThreatNode, SortedListNode[eThreatType]);
EXIT0:
    return cpResult;
}

const KSimpThreatNode* KSimpThreatList::FindThreat(KCharacter* pCharacter)
{
    return _FindThreat(pCharacter);
}

const KSimpThreatenNode* KSimpThreatList::GetFirstThreaten()
{
    return (const KSimpThreatenNode*)m_ThreatenList.GetHead();
}

const KSimpThreatenNode* KSimpThreatList::GetNextThreaten(const KSimpThreatenNode* cpThreatenNode)
{
    const KSimpThreatenNode*    cpResult    = NULL;
    QNode*                      pNode       = NULL;

    LOG_PROCESS_ERROR(cpThreatenNode);

    pNode = ((KSimpThreatenNode*)cpThreatenNode)->GetNext();    // What the fuck! The QNode doesn't accept a const pointer for GetNext()! 
    PROCESS_ERROR(pNode);

    cpResult = (const KSimpThreatenNode*)pNode;
EXIT0:
    return cpResult;
}

BOOL KSimpThreatList::IsInThreatList(KCharacter* pCharacter)
{
    BOOL                bResult = false;
    KSimpThreatNode*    pNode   = NULL;

    pNode = _FindThreat(pCharacter);
    PROCESS_ERROR(pNode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::IsInThreatenList(KCharacter* pCharacter)
{
    BOOL                bResult = false;
    KSimpThreatenNode*  pNode   = NULL;

    pNode = (KSimpThreatenNode*)m_ThreatenList.GetHead();
    while (pNode)
    {
        if (pNode->pCharacter == pCharacter)
            break;

        pNode = (KSimpThreatenNode*)pNode->GetNext();
    }

    PROCESS_ERROR(pNode);

    bResult = true;
EXIT0:
    return bResult;
}

void KSimpThreatList::UpdateKeepThreatField()
{
    assert(m_pSelf);

    m_nThreatPointX = m_pSelf->m_nX;
    m_nThreatPointY = m_pSelf->m_nY;

    return;
}

void KSimpThreatList::Activate()
{
    BOOL    bIsPlayer               = false;
    QNode*  pNext                   = NULL;
    BOOL    bOutOfBattleZoneFlag    = false;

    PROCESS_ERROR(m_pSelf);
    PROCESS_ERROR(m_nNextCheckLoop < (INT)g_cOrpgWorld.m_dwLogicFrames);
    m_nNextCheckLoop = g_cOrpgWorld.m_dwLogicFrames + CHECK_THREAT_INTERVAL;

    assert(m_pSelf->m_pScene);

    bIsPlayer = IS_PLAYER(m_pSelf->m_dwId);

    pNext = m_SortedList[thtMainThreat].GetHead();
    while (pNext)
    {
        KSimpThreatNode*    pNode               = KTHREAT_CONTAINING_RECORD(pNext, KSimpThreatNode, SortedListNode[thtMainThreat]);
        KCharacter*         pCharacter          = pNode->pCharacter;
        int                 nDistance2          = 0;
        int                 nKeepThreatRange    = 0;
        BOOL                bInRange            = false;

        assert(pNode->pPairNode);

        pNext = pNext->GetNext();

        if (
            pCharacter == NULL || 
            pCharacter->m_pScene == NULL || 
            pCharacter->m_pScene->m_dwTemplateId != m_pSelf->m_pScene->m_dwTemplateId || 
            pCharacter->m_pScene->m_nCopyIndex != m_pSelf->m_pScene->m_nCopyIndex
        )
        {
            _ClearThreat(pNode);
            continue;
        }

        if (pCharacter->m_eMoveState == cmsOnDeath)
        {
            _ClearThreat(pNode);

			m_pSelf->m_AIVM.FireEvent(aevOnTargetKilled, pCharacter->m_dwId, 0);
            continue;
        }

		 // 隐身
		 if(pCharacter->IsSkillHidden(m_pSelf))
		 {
			 _ClearThreat(pNode);
			 continue;
		 }

        // 检测仇恨消退时间,玩家为6秒规则,Npc的消退时间填在模板表里
        if ((INT)g_cOrpgWorld.m_dwLogicFrames >= (pNode->nLastRefreshFrame + m_pSelf->m_nThreatTime))
        {
            _ClearThreat(pNode);
            continue;
        }

        // 后面的流程只针对NPC --------------------------------------------------------
        if (bIsPlayer)
            continue;

        // 如果这个NPC定义了战斗区域，那么只要有玩家跑出仇恨区域，就立即通知AI
        if (IS_PLAYER(pCharacter->m_dwId) && m_nBattleZoneCenterX && m_nBattleZoneCenterY && !bOutOfBattleZoneFlag)
        {
            BOOL bInRadius = g_InRange(m_nBattleZoneCenterX, m_nBattleZoneCenterY, 0, pCharacter->m_nX, pCharacter->m_nY, 0, m_nBattleZoneRadius);
            BOOL bInHeight = ((pCharacter->m_nZ >= m_nBattleZoneCenterZ) && (pCharacter->m_nZ <= (m_nBattleZoneCenterZ + m_nBattleZoneHeight)));

            if (!bInRadius || !bInHeight)
                bOutOfBattleZoneFlag = true;
        }

        nKeepThreatRange = 1/*g_pWorld->m_Settings.m_NpcTemplateList.GetTemplate(0)->nPursuitRange*/;

        // NPC 模板定义的追击范围
        if (((KNpc*)m_pSelf)->m_pTemplate->nPursuitRange)
            nKeepThreatRange = ((KNpc*)m_pSelf)->m_pTemplate->nPursuitRange;

        bInRange = g_InRange(m_nThreatPointX, m_nThreatPointY, 0, pCharacter->m_nX, pCharacter->m_nY, 0, nKeepThreatRange);

        // 检测玩家是否从保持仇恨的范围外又回到了范围内
        if (bInRange || nKeepThreatRange == -1)
        {
            pNode->nLastInThreatFieldFrame = g_cOrpgWorld.m_dwLogicFrames;
            continue;
        }

        // 玩家如果在保持仇恨范围外并且超时，则清除该仇恨
        if ((g_cOrpgWorld.m_dwLogicFrames - pNode->nLastInThreatFieldFrame) >= CHECK_THREAT_INTERVAL + THREAT_FADE_FRAME)
        {
            _ClearThreat(pNode);
            continue;
        }
    }

    if (bOutOfBattleZoneFlag)
        m_pSelf->m_AIVM.FireEvent(aevOnOutOfBattleZone, m_pSelf->m_dwId, 0);

EXIT0:
    return;
}

BOOL KSimpThreatList::GetThreatListCount(THREAT_TYPE eThreatType, int& nCount)
{
    BOOL    bResult = false;
    QNode*  pNode   = NULL;

    LOG_PROCESS_ERROR(eThreatType > thtInvalid);
    LOG_PROCESS_ERROR(eThreatType < thtTotal);

    nCount = 0;
    pNode = m_SortedList[eThreatType].GetHead();
    while (pNode)
    {
        nCount++;
        pNode = pNode->GetNext();
    }

    bResult = true;
EXIT0:
    return bResult;
}

int KSimpThreatList::GetMainThreatSum()
{
    return m_nMainThreatSum;
}

BOOL KSimpThreatList::SetBattleZone(int nX, int nY, int nZ, int nRadius, int nHeight)
{
    BOOL bResult = false;

    PROCESS_ERROR(nX >= 0);
    PROCESS_ERROR(nY >= 0);
    PROCESS_ERROR(nZ >= 0);
    PROCESS_ERROR(nRadius >= 0);
    PROCESS_ERROR(nHeight >= 0);

    m_nBattleZoneCenterX = nX;
    m_nBattleZoneCenterY = nY;
    m_nBattleZoneCenterZ = nZ;
    m_nBattleZoneRadius  = nRadius;
    m_nBattleZoneHeight  = nHeight;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KSimpThreatList::IsDropCandidate(KPlayer* pPlayer)
{
    BOOL bResult = false;

    assert(m_pSelf);
    assert(pPlayer);

    PROCESS_ERROR(m_pSelf->m_pScene);
    PROCESS_ERROR(pPlayer->m_eMoveState != cmsOnDeath);
    PROCESS_ERROR(pPlayer->m_pScene == m_pSelf->m_pScene);

    bResult = true;
EXIT0:
    return bResult;
}

void KSimpThreatList::OnCharacterEnterNpcThreatList(KCharacter* pEnterCharacter)
{
    KNpc*   pNpc          = (KNpc*)m_pSelf;
    DWORD   dwEnterTeamID = ERROR_ID;
    DWORD   dwDropTeamID  = ERROR_ID;

    assert(pEnterCharacter);
    assert(pNpc);

    LOG_PROCESS_ERROR(IS_NPC(m_pSelf->m_dwId));

    //if (IS_PLAYER(pEnterCharacter->m_dwID) && pNpc->m_dwDropTargetPlayerID != ERROR_ID)
    //{
    //    KPlayer* pEnterPlayer = (KPlayer*)pEnterCharacter;
    //    KPlayer* pDropTarget = g_cPlayerMgr.GetById(pNpc->m_dwDropTargetPlayerID);

    //    dwEnterTeamID = pEnterPlayer->GetTeamId();
    //    if (pDropTarget)
    //        dwDropTeamID  = pDropTarget->GetTeamId();
    //    if (
    //        dwEnterTeamID == ERROR_ID || 
    //        dwDropTeamID != dwEnterTeamID
    //    )
    //    {
    //        pNpc->m_nAllAttackerLevel += pEnterPlayer->m_nLevel;
    //    }
    //}
    //else if (IS_NPC(pEnterCharacter->m_dwID))
    //{
    //    KNpc* pEnterNpc = (KNpc*)pEnterCharacter;

    //    if (pEnterNpc->m_pTemplate && !pEnterNpc->m_pTemplate->bDropNotQuestItemFlag)
    //    {
    //        pNpc->m_bDropNotQuestItemFlag = false;
    //    }

    //    pNpc->m_nAllAttackerLevel += pEnterCharacter->m_nLevel;
    //}

EXIT0:
    return;
}

void KSimpThreatList::OnCharacterLeaveNpcThreatList(KCharacter* pLeaveCharacter)
{
    BOOL            bRetCode      = false;
    KNpc*           pNpc          = (KNpc*)m_pSelf;
    DWORD           dwDropTeamID  = ERROR_ID;
    KPlayer*        pDropTarget   = NULL;

    assert(pLeaveCharacter);
    assert(pNpc);

    LOG_PROCESS_ERROR(IS_NPC(m_pSelf->m_dwId));

    //PROCESS_ERROR(pNpc->m_dwDropTargetPlayerID == pLeaveCharacter->m_dwID);
    PROCESS_ERROR(m_pSelf->m_eMoveState != cmsOnDeath);

    //pDropTarget = g_cPlayerMgr.GetById(pNpc->m_dwDropTargetPlayerID);
    //LOG_PROCESS_ERROR(pDropTarget);

    //pNpc->m_dwDropTargetPlayerID = ERROR_ID;

    //dwDropTeamID = pDropTarget->GetTeamId();
    //if (dwDropTeamID != ERROR_ID)
    //{
    //    const KSimpThreatNode* cpThreatNode = NULL;

    //    cpThreatNode = GetFirstThreat(thtMainThreat);
    //    while (cpThreatNode)
    //    {
    //        assert(cpThreatNode->pCharacter);

    //        if (IS_PLAYER(cpThreatNode->pCharacter->m_dwId))
    //        {
    //            KPlayer* pThreatPlayer  = (KPlayer*)cpThreatNode->pCharacter;
    //            DWORD    dwThreatTeamID = pThreatPlayer->GetTeamId();
    //            if (
    //                dwThreatTeamID == dwDropTeamID &&
    //                pThreatPlayer->m_dwID != pDropTarget->m_dwID
    //            )
    //            {
    //                bRetCode = IsDropCandidate(pThreatPlayer);
    //                if (bRetCode)
    //                {
    //                    pNpc->m_dwDropTargetPlayerID = pThreatPlayer->m_dwID;
    //                    break;
    //                }
    //            }
    //        }

    //        cpThreatNode = GetNextThreat(thtMainThreat, cpThreatNode);
    //    }
    //}

    //if (pNpc->m_dwDropTargetPlayerID == ERROR_ID)
    //{
    //    // 如果已经清掉了掉落目标，同时还要清掉其他信息
    //    pNpc->ClearDropTarget();
    //}

EXIT0:
    return;
}

void KSimpThreatList::OnPlayerEnterPlayerThreatList(KPlayer* pTarget)
{
    KPlayer* pSelf      = NULL;
    BOOL     bRetCode   = false;

    assert(pTarget);
    assert(IS_PLAYER(m_pSelf->m_dwId));
    
    pSelf = (KPlayer*)m_pSelf;

    //PROCESS_ERROR(pSelf->m_eCamp != cNeutral && pTarget->m_eCamp != cNeutral);
    //PROCESS_ERROR(pSelf->m_eCamp != pTarget->m_eCamp); // 我和他在同一阵营不用开开关
    //PROCESS_ERROR(pSelf->m_bCampFlag); // 我没有开开关，他就不用开
    //PROCESS_ERROR(!pTarget->m_bCampFlag); // 他的开关已经开了就不处理了
    //
    //bRetCode = pSelf->m_PK.IsOnSlay();
    //PROCESS_ERROR(!bRetCode); // 我开了屠杀，他就不用开开关

    //bRetCode = pSelf->IsFoe(pTarget->m_dwID);
    //PROCESS_ERROR(!bRetCode); // 他是我的仇人

    //bRetCode = pSelf->m_PK.IsDuel();
    //if (bRetCode)
    //{
    //    DWORD dwTargetID = pSelf->m_PK.GetTargetID();
    //    PROCESS_ERROR(dwTargetID != pTarget->m_dwID);
    //}
    //
    //PROCESS_ERROR(!pSelf->m_bRedName); // 我是红名，他不用开
    //
    //pTarget->SetCampFlag(true);

EXIT0:
    return;
}

KSimpThreatNode* KSimpThreatList::_GetFirstThreat(THREAT_TYPE eThreatType)
{
    KSimpThreatNode*    pResult = NULL;
    QNode*              pNode   = NULL;

    LOG_PROCESS_ERROR(eThreatType > thtInvalid);
    LOG_PROCESS_ERROR(eThreatType < thtTotal);

    pNode = m_SortedList[eThreatType].GetHead();
    PROCESS_ERROR(pNode);

    pResult = KTHREAT_CONTAINING_RECORD(pNode, KSimpThreatNode, SortedListNode[eThreatType]);
EXIT0:
    return pResult;
}

KSimpThreatNode* KSimpThreatList::_FindThreat(KCharacter* pCharacter)
{
    KSimpThreatNode*    pResult = NULL;
    QNode*              pNext   = NULL;

    pNext = m_SortedList[thtMainThreat].GetHead();
    while (pNext)
    {
        KSimpThreatNode* pNode = KTHREAT_CONTAINING_RECORD(pNext, KSimpThreatNode, SortedListNode[thtMainThreat]);

        pNext = pNext->GetNext();

        if (pNode->pCharacter != pCharacter)
            continue;

        pResult = pNode;
        break;
    }

EXIT0:
    return pResult;
}

void KSimpThreatList::_AdjustThreatList(KSimpThreatNode* pSimpThreatNode)
{
    assert(pSimpThreatNode);
    
    for (int i = thtInvalid + 1; i < thtTotal; i++)
    {
        QNode*  pPrev   = NULL;
        QNode*  pNext   = NULL;
        QNode*  pAdjust = NULL;

        pPrev = pSimpThreatNode->SortedListNode[i].GetPrev();
        while (pPrev)
        {
            KSimpThreatNode* pPrevSimpThreatNode = KTHREAT_CONTAINING_RECORD(pPrev, KSimpThreatNode, SortedListNode[i]);

            if (!pSimpThreatNode->bTopFlag && pPrevSimpThreatNode->bTopFlag)
                break;

            if (pSimpThreatNode->bTopFlag == pPrevSimpThreatNode->bTopFlag && pSimpThreatNode->nThreat[i] <= pPrevSimpThreatNode->nThreat[i])
                break;

            pAdjust = pPrev;
            pPrev = pPrev->GetPrev();
        }

        if (pAdjust)
        {
            pSimpThreatNode->SortedListNode[i].Remove();
            pAdjust->InsertBefore(&pSimpThreatNode->SortedListNode[i]);

            pAdjust = NULL;
        }

        pNext = pSimpThreatNode->SortedListNode[i].GetNext();
        while (pNext)
        {
            KSimpThreatNode* pNextSimpThreatNode = KTHREAT_CONTAINING_RECORD(pNext, KSimpThreatNode, SortedListNode[i]);

            if (pSimpThreatNode->bTopFlag && !pNextSimpThreatNode->bTopFlag)
                break;

            if (pSimpThreatNode->bTopFlag == pNextSimpThreatNode->bTopFlag && pSimpThreatNode->nThreat[i] >= pNextSimpThreatNode->nThreat[i])
                break;

            pAdjust = pNext;
            pNext = pNext->GetNext();
        }

        if (pAdjust)
        {
            pSimpThreatNode->SortedListNode[i].Remove();
            pAdjust->InsertAfter(&pSimpThreatNode->SortedListNode[i]);

            pAdjust = NULL;
        }
    }
}

BOOL KSimpThreatList::_ClearThreat(KSimpThreatNode *pSimpThreatNode)
{
    BOOL        bResult     = false;
    KCharacter* pThreatDst  = NULL;

    LOG_PROCESS_ERROR(pSimpThreatNode);

    pThreatDst = pSimpThreatNode->pCharacter;
    LOG_PROCESS_ERROR(pThreatDst);
    
    if (IS_NPC(m_pSelf->m_dwId))
        OnCharacterLeaveNpcThreatList(pThreatDst);

    //if (IS_PLAYER(m_pSelf->m_dwId))
    //    g_PlayerServer.DoClearThreat(((KPlayer*)m_pSelf)->m_nConnIndex, pThreatDst->m_dwID);

    if (IS_PLAYER(pThreatDst->m_dwId))
    {
        //g_PlayerServer.DoClearThreaten(((KPlayer*)pThreatDst)->m_nConnIndex, m_pSelf->m_dwId);

        //npc对玩家清除仇恨的同时,玩家也必定清除Npc的仇恨
        if (IS_NPC(m_pSelf->m_dwId))
            pThreatDst->m_SimpThreatList.ClearThreat(m_pSelf);
    }

    if (pSimpThreatNode->nThreat[thtMainThreat] > 0)
        m_nMainThreatSum -= pSimpThreatNode->nThreat[thtMainThreat];

    pSimpThreatNode->pPairNode->Remove();
    SAFE_DELETE(pSimpThreatNode->pPairNode);

    for (int i = thtInvalid + 1; i < thtTotal; i++)
        pSimpThreatNode->SortedListNode[i].Remove();

	SAFE_DELETE(pSimpThreatNode);

    bResult = true;
EXIT0:
    return bResult;
}

#endif // GAME_SERVER
