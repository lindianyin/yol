
#pragma once

#include "StatDataDef.h"
#include "KRole.h"

#define MAX_STAT_DATA_COUNT 1024 * 512

struct KMapInfo;

#pragma pack(1)
struct KStatData_DB
{
    int     nDataCount;
    INT64 nData[0];
};
#pragma pack()

class KStatDataManager
{
public:
    KStatDataManager();
    ~KStatDataManager();

    BOOL Init();
    void UnInit();
    
    void Activate();
    BOOL OnGSUpdate(KSTAT_DATA_MODIFY* pData, int nCount);
    int  GetNameID(const char cszName[]);
    BOOL SetDataValue(const char szName[], INT64 nValue);
    BOOL SaveAll();
    BOOL LoadDataName(const char szName[], int nID);
    BOOL LoadData(time_t nTime, BYTE* pbyData, size_t uDataLen);

    BOOL Update(const char cszName[], INT64 nValue);

private:
    void UpdateRoleStat();

public:
    void UpdateCreateMap(KMapInfo* pMapInfo);

    void UpdataAuctionItemStat(DWORD dwTabType, DWORD dwTabIndex, int nMoney);
    void UpdateMoneyStat(KRole* pRole, int nMoney, const char cszMethod[]);

    void UpdataCreateOrDeleteRole(BOOL bCreate);
    void UpdateGameCardDealAmmount(int nCoin, int nMoney);

    void UpdateMentorCreateStat(DWORD dwMentorID, DWORD dwApprenticeID);
    void UpdateMentorDeleteStat();
    void UpdateMentorGraduateStat(DWORD dwMentorID, DWORD dwApprenticeID);
    void UpdateMentorBrokenStat(DWORD dwMentorID, DWORD dwApprenticeID);

private:
    time_t  m_nStatTime;
    int     m_nMaxUsedID;
    INT64  m_StatValues[MAX_STAT_DATA_COUNT];

	typedef std::map<std::string, int> KDATA_NAME_TABLE;

    KDATA_NAME_TABLE m_DataNameTable;
};

