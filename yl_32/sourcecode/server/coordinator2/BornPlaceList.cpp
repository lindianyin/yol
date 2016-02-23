

#include "stdafx.h"
#include <algorithm>
#include "BornPlaceList.h"
#include "MapManager.h"
#include "GameCenter.h"



QBornPlaceList::QBornPlaceList()
{
}

BOOL QBornPlaceList::Load()
{
    BOOL        bResult                 = false;
    int         nRetCode                = false;
    ITabFile*   piTabFile               = NULL;
    int         nTableHeight            = 0;
    char        szFileName[PATH_MAX];

    snprintf(szFileName, sizeof(szFileName), "/setting/scene/bornplacelist.txt");
    szFileName[sizeof(szFileName) - 1] = '\0';

    piTabFile = g_OpenTabFile(szFileName);
    if (!piTabFile)
    {
        goto EXIT0;
    }

    nTableHeight = piTabFile->GetHeight();

    for (int nLine = 2; nLine <= nTableHeight; nLine++)
    {
        KMapInfo*                       pMapInfo   = NULL;
        QBornPointInfo                  BornPoint;
        QBORNPLACE_MAP_TABLE::iterator   MapTabIter;

        nRetCode = piTabFile->GetInteger(nLine, "MapId", 0, (int*)&BornPoint.dwMapID);
        LOG_PROCESS_ERROR(nRetCode == 1);

        nRetCode = piTabFile->GetInteger(nLine, "RoleType", 0, &BornPoint.nRoleType);
        LOG_PROCESS_ERROR(nRetCode == 1);

        nRetCode = piTabFile->GetInteger(nLine, "X", 0, &BornPoint.nPosX);
        LOG_PROCESS_ERROR(nRetCode == 1);

        nRetCode = piTabFile->GetInteger(nLine, "Y", 0, &BornPoint.nPosY);
        LOG_PROCESS_ERROR(nRetCode == 1);

        nRetCode = piTabFile->GetInteger(nLine, "Z", 0, &BornPoint.nPosZ);
        LOG_PROCESS_ERROR(nRetCode == 1);

        nRetCode = piTabFile->GetInteger(nLine, "FaceDirection", 0, &BornPoint.nDirection);
        LOG_PROCESS_ERROR(nRetCode == 1);

        pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(BornPoint.dwMapID);
        LOG_PROCESS_ERROR(pMapInfo && pMapInfo->m_nType == emtBirthMap);

        MapTabIter = find(m_HometownMapTable.begin(), m_HometownMapTable.end(), BornPoint.dwMapID);
        if (MapTabIter == m_HometownMapTable.end())
        {
            m_HometownMapTable.push_back(BornPoint.dwMapID);
        }

        m_BornPointTable.push_back(BornPoint);
    }

    bResult = true;
EXIT0:
    if (!bResult)
    {
        QLogPrintf(LOG_ERR, "Load file %s failed !\n", szFileName);
    }
    SAFE_RELEASE(piTabFile);
    return bResult;
}

BOOL QBornPlaceList::GetBornPointInfo(QBornPointInfo* pBornPointInfo, DWORD dwMapID, int nRoleType)
{
    BOOL bResult = false;

    for (KBORN_POINT_TABLE::iterator it = m_BornPointTable.begin(); it != m_BornPointTable.end(); ++it)
    {
        if (it->dwMapID == dwMapID && it->nRoleType == nRoleType)
        {
            *pBornPointInfo = *it;
            bResult = true;
            break;
        }
    }

    return bResult;
}

