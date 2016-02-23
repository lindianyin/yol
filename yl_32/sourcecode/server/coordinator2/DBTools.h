#ifndef _KDBTOOLS_H_
#define _KDBTOOLS_H_

class KDBTools
{
public:
    KDBTools();

    BOOL Init();
    void UnInit();

    BOOL CheckDatabase();

    BOOL LoadGameData();

	BOOL LoadRoleBlackList();
    
private:    
    BOOL CheckRoleTable();
    BOOL CheckRelationTable();
    BOOL CheckMapCopyTable();
    BOOL CheckRoleBlakListTable();
    BOOL CheckTongTable();
    BOOL CheckGlobalMailTable();
    BOOL CheckMailTable();
    BOOL CheckPQTable();
    BOOL CheckAuctionTable();
    BOOL CheckGlobalCustomDataTable();
    BOOL CheckGlobalSystemValueTable();
    BOOL CheckStatDataTable();
    BOOL CheckActivityTable();
    BOOL CheckGameCardTable();
    BOOL CheckRoleDeleteTable();
    BOOL CheckAntiFarmerTable();
    BOOL CheckMentorTable();
    BOOL CheckRenameRoleTable();
	BOOL CheckDelayLoadTable();
	BOOL CheckPayRecordTable();
	BOOL CheckRankPoolTable();
	BOOL CheckArenaRecordTable();
	BOOL CheckServerRecordTable();

    BOOL LoadRoleTable();
	BOOL LoadPayRecordTable();
    BOOL LoadMapCopyInfo();
    
    BOOL LoadRoleDeleteTable();
    BOOL LoadTongTable();
	BOOL LoadRankTable();
    BOOL LoadGlobalSystemValue();
    BOOL LoadGlobalMailTable();
    BOOL LoadGlobalCustomDataTable();
    BOOL LoadStatDataTable();
    BOOL LoadActivityTable();
    BOOL LoadAntiFarmerTable();
    BOOL LoadMentorTable();
    BOOL LoadRenameRoleTable();
	BOOL CheckGMSendAwardTale();
	BOOL CheckSaveOpenseTable();
	BOOL CheckPayListTable();
    // 返回-1表示失败,0表示未保存过,1表示成功
    int GetGlobalSystemValue(int* pnRetValue, const char cszKey[]);
    
    BOOL DoQuery(const char szSQL[]);
private:
    MYSQL*  m_pDBHandle;

    char m_szSQL[1024];
};

#endif // _KDBTOOLS_H_
