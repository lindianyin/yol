#pragma once
#include "config/kplayerbasedbdef.h"

// 最多保留4条竞技场战斗记录
#define MAX_ARENA_RECORD_NUM 4

class ArenaManager
{
public:
	ArenaManager(void);
	~ArenaManager(void);

	BOOL Init();
	void UnInit();

private:
	typedef std::list<KArenaRecord> RECORD_LIST;
	typedef std::map<DWORD, RECORD_LIST> KRECORD_TABLE;

	KRECORD_TABLE m_mapArenaRecord;

public:
	BOOL LoadRecord(INT nConnIndex, DWORD dwRoleId);

	BOOL OnLoadRecordFromDB(INT nConnIndex, DWORD dwRoleId, BYTE* pData, size_t uDataLen);

	BOOL AddRecord(INT nConnIndex, DWORD dwRoleId, BOOL bAttack, BOOL bFightResult, LPCHAR szName, INT nTargetIdx);

	VOID SaveAll();

private:
	VOID SaveRecord(DWORD dwRoleId, RECORD_LIST& vecRecord);

	BOOL LoadRecordRespond(INT nConnIndex, DWORD dwRoleId, RECORD_LIST& vecRecord);
};
