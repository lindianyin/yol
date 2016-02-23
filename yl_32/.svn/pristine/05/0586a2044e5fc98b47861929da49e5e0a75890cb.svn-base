#include "stdafx.h"
#include "ArenaManager.h"
#include "ArenaServerHandler.h"
#include "KRole.h"
#include "GameCenter.h"
#include "onlinegamebase/ksysservice.h"

ArenaManager::ArenaManager(void)
{

}

ArenaManager::~ArenaManager(void)
{

}

BOOL ArenaManager::Init()
{
	BOOL bResult    = false;

	m_mapArenaRecord.clear();

	bResult = true;
EXIT0:
	return bResult;
}

VOID ArenaManager::UnInit()
{
	SaveAll();
}

VOID ArenaManager::SaveAll()
{
	for(KRECORD_TABLE::iterator it = m_mapArenaRecord.begin(); it != m_mapArenaRecord.end(); ++it)
	{
		SaveRecord(it->first, it->second);
	}
}

VOID ArenaManager::SaveRecord(DWORD dwRoleId, RECORD_LIST& vecRecord)
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;

	PROCESS_ERROR(vecRecord.size());

	uPakSize = vecRecord.size() * sizeof(KArenaRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pBuffer = (LPBYTE)piMem->GetData();
	for(RECORD_LIST::iterator it = vecRecord.begin(); it != vecRecord.end(); ++it)
	{
		KArenaRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KArenaRecord));
		pBuffer += sizeof(KArenaRecord);
	}

	bRetCode = g_pGameCenter->m_MiscDB.DoSaveArenaRecord(dwRoleId, piMem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piMem);
	return;
}

BOOL ArenaManager::LoadRecordRespond(INT nConnIndex, DWORD dwRoleId, RECORD_LIST& vecRecord)
{
	BOOL				bResult		= FALSE;
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMem		= NULL;
	size_t				uPakSize	= 0;
	LPBYTE				pBuffer		= NULL;
	LPBYTE				pData		= NULL;

	PROCESS_ERROR(vecRecord.size());

	uPakSize = vecRecord.size() * sizeof(KArenaRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);

	pData = (LPBYTE)piMem->GetData();
	pBuffer = pData;
	for(RECORD_LIST::iterator it = vecRecord.begin(); it != vecRecord.end(); ++it)
	{
		KArenaRecord* pRecord = &(*it);
		memcpy(pBuffer, pRecord, sizeof(KArenaRecord));
		pBuffer += sizeof(KArenaRecord);
	}

	bRetCode = KArenaHandler::DoLoadRecordRespond(nConnIndex, dwRoleId, pData, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL ArenaManager::LoadRecord(INT nConnIndex, DWORD dwRoleId)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	KRECORD_TABLE::iterator it = m_mapArenaRecord.find(dwRoleId);
	if(it != m_mapArenaRecord.end())
	{
		RECORD_LIST& vecRecord = it->second;
		LoadRecordRespond(nConnIndex, dwRoleId, vecRecord);
	}
	else
	{
		bRetCode = g_pGameCenter->m_MiscDB.DoLoadArenaRecord(nConnIndex, dwRoleId);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaManager::OnLoadRecordFromDB(INT nConnIndex, DWORD dwRoleId, BYTE* pData, size_t uDataLen)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	RECORD_LIST vecRecord;
	BYTE*		pBuffer = pData;
	size_t		uPakSize = uDataLen;

	while(uPakSize >= sizeof(KArenaRecord))
	{
		KArenaRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KArenaRecord));
		vecRecord.push_back(sRecord);
		pBuffer += sizeof(KArenaRecord);
		uPakSize -= sizeof(KArenaRecord);
	}

	LOG_PROCESS_ERROR(uPakSize == 0);

	m_mapArenaRecord[dwRoleId] = vecRecord;

	bRetCode = KArenaHandler::DoLoadRecordRespond(nConnIndex, dwRoleId, pData, uDataLen);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaManager::AddRecord(INT nConnIndex, DWORD dwRoleId, BOOL bAttack, BOOL bFightResult, LPCHAR szName, INT nTargetIdx)
{
	BOOL					bResult = FALSE;
	BOOL					bRetCode = FALSE;
	KArenaRecord			sRecord;
	KRECORD_TABLE::iterator it;

	sRecord.dwTimestamp = KSysService::Time(NULL);
	sRecord.bAttack = bAttack;
	sRecord.bResult = bFightResult;
	::strncpy(sRecord.szTargetName, szName, _NAME_LEN);
	sRecord.nTargetIdx = nTargetIdx;

	it = m_mapArenaRecord.find(dwRoleId);
	if(it != m_mapArenaRecord.end())
	{
		RECORD_LIST& vecList = it->second;
		vecList.push_back(sRecord);

		if(vecList.size() > MAX_ARENA_RECORD_NUM)
		{
			vecList.pop_front();
		}
	}
	else
	{
		RECORD_LIST vecList;
		vecList.push_back(sRecord);
		m_mapArenaRecord[dwRoleId] = vecList;
	}
	
	bRetCode = LoadRecordRespond(nConnIndex, dwRoleId, m_mapArenaRecord[dwRoleId]);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}