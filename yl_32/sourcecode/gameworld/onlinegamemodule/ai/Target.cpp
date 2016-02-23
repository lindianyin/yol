#include "stdafx.h"
#include "onlinegamemodule/ai/Target.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kdoodadmgr.h"

KTarget::KTarget()
{
    m_eTargetType   = ttInvalid;
    m_dwID          = ERROR_ID;
    memset(&m_Coordination, 0, sizeof(m_Coordination));
}

KTarget::~KTarget()
{
    m_eTargetType   = ttInvalid;
    m_dwID          = ERROR_ID;
}

void KTarget::SetTargetData(KTargetData const& TargetData)
{
	assert(sizeof(KTarget) == sizeof(KTargetData));
	memcpy(this, &TargetData, sizeof(KTargetData));
}

void KTarget::GetTargetData(KTargetData& TargetData)
{
	assert(sizeof(KTarget) == sizeof(KTargetData));
	memcpy(&TargetData, this, sizeof(KTargetData));
}

TARGET_TYPE KTarget::GetTargetType()
{
    return m_eTargetType;
}

DWORD KTarget::GetTargetID()
{
	return m_dwID;
}

BOOL KTarget::SetTarget(KCharacter* pCharacter)
{
    BOOL bResult = false;

    PROCESS_ERROR(pCharacter);
    
    m_dwID  = pCharacter->m_dwId;
    m_eTargetType = ttNpc;

    if (IS_PLAYER(pCharacter->m_dwId))
    {
        m_eTargetType = ttPlayer;
    }
    
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::SetTarget(KDoodad* pDoodad)
{
    BOOL bResult = false;
    
    PROCESS_ERROR(pDoodad);

    m_eTargetType = ttDoodad;
    m_dwID        = pDoodad->m_dwId;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::SetTarget(int nX, int nY, int nZ)
{
    m_eTargetType   = ttCoordination;
    m_Coordination.nX = nX;
    m_Coordination.nY = nY;
    m_Coordination.nZ = nZ;

    return true;
}

BOOL KTarget::SetTarget(TARGET_TYPE eType, DWORD dwID)
{
    BOOL bResult = false;

    switch (eType)
    {
    case ttNoTarget:
        break;
    
    case ttNpc:
        {
            KNpc* pNpc = NULL;
            LOG_PROCESS_ERROR(IS_NPC(dwID));

            pNpc = g_cNpcMgr.GetById(dwID);
            PROCESS_ERROR(pNpc);
            
            break;
        }
    case ttPlayer:
        {
            KPlayer* pPlayer = NULL;

            LOG_PROCESS_ERROR(IS_PLAYER(dwID));
            pPlayer = g_cPlayerMgr.GetById(dwID);
            PROCESS_ERROR(pPlayer);

            break;
        }
    case ttDoodad:
        {
            KDoodad* pDoodad = NULL;

            pDoodad = g_cDoodadMgr.GetById(dwID);
            PROCESS_ERROR(pDoodad);

            break;
        }
    case ttItem:
        {
            KItem* pItem = NULL;
            //pItem = g_pWorld->m_ItemSet.GetObj(dwID);
            PROCESS_ERROR(pItem);
            
            break;
        }
    default:
        LOG_PROCESS_ERROR(false && "Unexecpt Target Type!");
    }
    
    m_eTargetType = eType;
    m_dwID        = dwID;

    bResult = true;
EXIT0:
    if (!bResult)
    {
        m_eTargetType = ttNoTarget;
    }
    return bResult;
}

BOOL KTarget::GetTarget(KCharacter** ppCharacter)
{
    BOOL bResult = false;

    switch (m_eTargetType)
    {
    case ttNpc:
        {
            KNpc* pNpc = g_cNpcMgr.GetById(m_dwID);
            PROCESS_ERROR(pNpc);
            *ppCharacter = (KCharacter*)pNpc;
            break;
        }
    case ttPlayer:
        {
            KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwID);
            PROCESS_ERROR(pPlayer);
            *ppCharacter = (KCharacter*)pPlayer;
            break;
        }
    default:
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::GetTarget(KNpc** ppNpc)    
{
    BOOL    bResult = false;
    KNpc*   pNpc    = NULL;
    
    assert(ppNpc);

    PROCESS_ERROR(m_eTargetType == ttNpc);

    pNpc = g_cNpcMgr.GetById(m_dwID);
    PROCESS_ERROR(pNpc);

    *ppNpc = pNpc;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::GetTarget(KPlayer** ppPlayer)
{
    BOOL        bResult = false;
    KPlayer*    pPlayer = NULL;

    assert(ppPlayer);
    PROCESS_ERROR(m_eTargetType == ttPlayer);
    
    pPlayer = g_cPlayerMgr.GetById(m_dwID);
    PROCESS_ERROR(pPlayer);

    *ppPlayer = pPlayer;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::GetTarget(KDoodad** ppDoodad)
{
    BOOL        bResult = false;
    KDoodad*    pDoodad = NULL;

    assert(ppDoodad);
    
    PROCESS_ERROR(m_eTargetType == ttDoodad);
    
    pDoodad = g_cDoodadMgr.GetById(m_dwID);
    PROCESS_ERROR(pDoodad);

    *ppDoodad = pDoodad;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::GetTarget(KSceneObject** ppSceneObject)
{
    BOOL bResult = false;

    switch (m_eTargetType)
    {
    case ttNpc:
        {
            KNpc* pNpc = g_cNpcMgr.GetById(m_dwID);
            PROCESS_ERROR(pNpc);
            *ppSceneObject = (KSceneObject*)pNpc;
            break;
        }
    case ttPlayer:
        {
            KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwID);
            PROCESS_ERROR(pPlayer);
            *ppSceneObject = (KSceneObject*)pPlayer;
            break;
        }
    case ttDoodad:
        {
            KDoodad* pDoodad = g_cDoodadMgr.GetById(m_dwID);
            PROCESS_ERROR(pDoodad);
            *ppSceneObject = (KSceneObject*)pDoodad;
            break;
        }
    default:
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::GetTarget(int* pnX, int* pnY, int* pnZ)
{
    BOOL        bResult  = false;
    BOOL        bRetCode = false;
    KPlayer*    pPlayer  = NULL;
    KNpc*       pNpc     = NULL;
    KDoodad*    pDoodad  = NULL;

    switch (m_eTargetType)
    {
    case ttNpc:
        pNpc = g_cNpcMgr.GetById(m_dwID);
        PROCESS_ERROR(pNpc);
        bRetCode = pNpc->GetPosition(*pnX, *pnY, *pnZ);
        PROCESS_ERROR(bRetCode);
        break;
    
    case ttPlayer:
        pPlayer = g_cPlayerMgr.GetById(m_dwID);
        PROCESS_ERROR(pPlayer);
        bRetCode = pPlayer->GetPosition(*pnX, *pnY, *pnZ);
        PROCESS_ERROR(bRetCode);
        break;

    case ttDoodad:
        pDoodad = g_cDoodadMgr.GetById(m_dwID);
        PROCESS_ERROR(pDoodad);
        bRetCode = pDoodad->GetPosition(*pnX, *pnY, *pnZ);
        PROCESS_ERROR(bRetCode);
        break;

    case ttCoordination:
        *pnX = m_Coordination.nX;
        *pnY = m_Coordination.nY;
        *pnZ = m_Coordination.nZ;
        break;

    case ttItem:
        goto EXIT0;

	case ttItemPos:
		//pPlayer = g_pWorld->m_PlayerSet.GetObj(m_dwID);
		//LOG_PROCESS_ERROR(pPlayer);
  //      bRetCode = pPlayer->GetAbsoluteCoordinate(pnX, pnY, pnZ);
  //      LOG_PROCESS_ERROR(bRetCode);
		break;

    default:
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTarget::ClearTarget()
{
    m_eTargetType = ttNoTarget;
    return true;
}
