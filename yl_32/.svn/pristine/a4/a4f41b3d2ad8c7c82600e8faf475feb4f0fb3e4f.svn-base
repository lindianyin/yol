
#pragma once

#include <vector>
#include <algorithm>

typedef std::vector<DWORD> QBORNPLACE_MAP_TABLE;

struct QBornPointInfo
{
    DWORD   dwMapID;
    int     nRoleType;
    int     nPosX;
    int     nPosY;
    int     nPosZ;
    int     nDirection;
};

class QBornPlaceList
{
public:
    QBornPlaceList();

    BOOL Load();

    BOOL GetBornPointInfo(QBornPointInfo* pBornPointInfo, DWORD dwMapID, int nRoleType);

    BOOL IsMapInHowntownList(DWORD dwMapID)
    {
        QBORNPLACE_MAP_TABLE::iterator it = std::find(m_HometownMapTable.begin(), m_HometownMapTable.end(), dwMapID);
        return (it != m_HometownMapTable.end());
    }

    const QBORNPLACE_MAP_TABLE* GetBornPlaceTable()
    {
        return &m_HometownMapTable;
    }

private:

    typedef std::vector<QBornPointInfo> KBORN_POINT_TABLE;

    KBORN_POINT_TABLE    m_BornPointTable;
    QBORNPLACE_MAP_TABLE  m_HometownMapTable;
};


