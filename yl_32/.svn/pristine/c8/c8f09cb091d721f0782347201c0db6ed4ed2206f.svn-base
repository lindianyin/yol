#include "stdafx.h"
#include "GMSendAwardDB.h"
#include "GameCenter.h"
#include "DBTools.h"
#include "RoleManager.h"
#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#else
#include "onlinegameworld/kplayermanager_client.h"
#endif

GMSendAwardDB::GMSendAwardDB()
{
	m_pDBGMCMD = NULL;
}

GMSendAwardDB::~GMSendAwardDB()
{

}

BOOL GMSendAwardDB::Init()
{
	BOOL	bRetCode = FALSE;
	bRetCode = connect();
	LOG_PROCESS_ERROR(bRetCode);
	return TRUE;
EXIT0:
	return FALSE;
}

void GMSendAwardDB::UnInit()
{
	if (m_pDBGMCMD)
    {
        MySQL_Disconnect(m_pDBGMCMD);
        m_pDBGMCMD = NULL;
    }
}

BOOL GMSendAwardDB::connect()
{
	BOOL    bResult    = false;

	assert(m_pDBGMCMD == NULL);

	m_pDBGMCMD = MySQL_Connect(
		g_pGameCenter->m_Settings.m_szDBIP,
		g_pGameCenter->m_Settings.m_szDBName, 
		g_pGameCenter->m_Settings.m_szDBAcc,
		g_pGameCenter->m_Settings.m_szDBPsw    
		);
	PROCESS_ERROR(m_pDBGMCMD);

	bResult = true;
EXIT0:
	if (!bResult)
	{
		if (m_pDBGMCMD)
		{
			MySQL_Disconnect(m_pDBGMCMD);
			m_pDBGMCMD = NULL;
		}
	}
	return bResult;
}


BOOL GMSendAwardDB::OnLogin( DWORD playerID )
{
	FindTable(playerID);  
	return TRUE;
}

void GMSendAwardDB::Activate()
{
	KRole *pRole = NULL;
	//防止过长时间没有操作数据库断开连接
	if (g_pGameCenter->m_nTimeNow > m_nNextPingTime)
    {
        if (m_pDBGMCMD)
        {
            mysql_ping(m_pDBGMCMD);
        }
        m_nNextPingTime = g_pGameCenter->m_nTimeNow + PING_CYCLE;
    }
	
	g_pGameCenter->m_RoleManager.m_lock.Lock();

	std::vector<DWORD> playerIds;
	g_pGameCenter->m_RoleManager.GetOnlineRoleIds(playerIds);
	for(std::vector<DWORD>::iterator it = playerIds.begin();it != playerIds.end();++it)
	{
		pRole = g_pGameCenter->m_RoleManager.GetRole(*it);
		if(NULL == pRole)
		{
			continue;
		}
		FindTable(*it);
	}
	
	//std::map<DWORD, KRole, std::less<DWORD> > m_RoleTable = g_pGameCenter->m_RoleManager.GetAllRole();
	//for (std::map<DWORD, KRole, std::less<DWORD> >::iterator it = m_RoleTable.begin();it != m_RoleTable.end();++it)
	//{
	//	KRole&  pRole = it->second;
	//	//LOG_PROCESS_ERROR(pRole);
	//	if (!pRole.IsOnline())
	//	{
	//		continue;
	//	}
	//	FindTable(pRole.m_dwPlayerID);
	//}
	LoadRoleBlackList();
EXIT0:
	g_pGameCenter->m_RoleManager.m_lock.Unlock();
	return;
}




BOOL GMSendAwardDB::LoadRoleBlackList()
{
    BOOL        bResult         = false;
    BOOL        bRetCode        = false;
    int         nRetCode        = false;
    MYSQL_RES*  pQueryRes       = NULL;
    MYSQL_ROW   QueryRow        = NULL;
    DWORD       dwRoleID        = 0;
    time_t      nFreezeTime     = 0;
    int         nBlackListCount = 0;
	ZeroMemory(m_szSQL, sizeof(m_szSQL));
    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL), "select ID, UNIX_TIMESTAMP(FreezeTime) from %s", 
        ROLE_BLACK_LIST_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBGMCMD);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        dwRoleID = strtol(QueryRow[0], NULL, 10);
        nFreezeTime = strtol(QueryRow[1], NULL, 10);

        g_pGameCenter->m_RoleManager.FreezeRole(dwRoleID, nFreezeTime);

        nBlackListCount++;
    }

    //QLogPrintf(LOG_DEBUG, "[DB] %d roles in black list loaded !", nBlackListCount);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}




BOOL GMSendAwardDB::FindTable(DWORD	dwPlayerId)
{
	BOOL             bResult         = false;
	BOOL             bRetCode        = false;
	int              nRetCode        = false;
	MYSQL_RES*       pQueryRes       = NULL;
	MYSQL_ROW        queryRow;
	time_t			 nCMDTime		 = -1;
	KRole*           pRole           = NULL;
	unsigned long *  pulLengths      = NULL;

	
	PROCESS_ERROR(m_pDBGMCMD);
	LOG_PROCESS_ERROR(dwPlayerId > 0);
	ZeroMemory(m_szSQL, sizeof(m_szSQL));
	nRetCode = snprintf(
		m_szSQL, sizeof(m_szSQL), "select * from %s where playerID = %d and mark=1", 
		GM_SEND_AWARD_TABLE_NAME,dwPlayerId);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));
	bRetCode = DoQuery(m_szSQL);
	LOG_PROCESS_ERROR(bRetCode);

	pQueryRes = mysql_use_result(m_pDBGMCMD);
	LOG_PROCESS_ERROR(pQueryRes);

	while (true)
	{
		DWORD 	id		  		= 0;
		DWORD 	playerID_DB 	= 0;
		char* 	pszGMCMD		= NULL;
		char* 	LastTime	  	= NULL;		//最后时间
		DWORD 	mark 			= 0;
		queryRow = mysql_fetch_row(pQueryRes);
		if (!queryRow)
		{
			break;
		}
		pulLengths = mysql_fetch_lengths(pQueryRes);
		LOG_PROCESS_ERROR(pulLengths);
		
		id				   = (DWORD)strtoul(queryRow[0], NULL, 10);
		playerID_DB        = (DWORD)strtoul(queryRow[1], NULL, 10);
		mark			   = (DWORD)strtoul(queryRow[2], NULL, 10);
		pszGMCMD		   = queryRow[3];
		LastTime		   = queryRow[4];
		
		LOG_PROCESS_ERROR(pszGMCMD);
		LOG_PROCESS_ERROR(LastTime);
		LOG_PROCESS_ERROR(GetTimeStampByStr(LastTime,&nCMDTime));
		if (mark == 1 && (nCMDTime > g_pGameCenter->m_nTimeNow  || nCMDTime <=0) && playerID_DB == dwPlayerId && pszGMCMD != NULL)
		{
			pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
			LOG_PROCESS_ERROR(pRole);
			m_RespondGMCMD.Lock();
			BOOL result = R2S_SendCMD(id,pRole->m_nConnIndex,dwPlayerId,pszGMCMD);
			m_RespondGMCMD.Unlock();
		}
	}
	bResult = TRUE;
EXIT0:
	if (pQueryRes)
	{
		mysql_free_result(pQueryRes);
		pQueryRes = NULL;
	}
	return bResult;
}


BOOL GMSendAwardDB::DoQuery(const char cszSQL[])
{
	BOOL     bResult            = false;
	BOOL     bRetCode           = false;
	unsigned ulMySQLErrorCode   = 0;
	PROCESS_ERROR(m_pDBGMCMD);
	bRetCode = MySQL_Query(m_pDBGMCMD, cszSQL, &ulMySQLErrorCode);
	LOG_PROCESS_ERROR(bRetCode);
	bResult = true;
EXIT0:
	if (ulMySQLErrorCode != 0 && ulMySQLErrorCode != ER_DUP_ENTRY) // 主键冲突不用断开链接
	{
		MySQL_Disconnect(m_pDBGMCMD);
		m_pDBGMCMD = NULL;
	}
	return bResult;
}

BOOL GMSendAwardDB::DoCommand( LPCSTR pszCommand )
{
	//QCONFIRM_RET_FALSE(pszCommand);
	//PCSTR pszLuaCommand = NULL;
	//if (*pszCommand != '?')
	//	pszLuaCommand = pszCommand;
	//else if (strstr(pszCommand, GM_CMD_PREFIX_GAMESERVER))
	//	pszLuaCommand = pszCommand + sizeof(GM_CMD_PREFIX_GAMESERVER) - 1;

	//QLogPrintf(LOG_INFO, "[CMD]: %s", pszCommand);

	//ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	//if (pszLuaCommand)
	//	return cSafeScript->DoBuffer(pszLuaCommand);
	//else
	//	return cSafeScript->CallTableFunction("ServerEvent", "OnConsoleCommand", 0, "s", pszCommand);
	return FALSE;
}

BOOL GMSendAwardDB::R2S_SendCMD(DWORD id, INT nConnIndex, DWORD dwRoleId, char* pszGMCMD)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_GM_SEND_AWARD*				pRespond = NULL;
	IMemBlock*						piPackage = NULL;
	INT								nLength  = 0;

	
	piPackage = QCreateMemBlock(sizeof(R2S_GM_SEND_AWARD));
	pRespond = (R2S_GM_SEND_AWARD*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	ZeroMemory(pRespond->GMCMD,sizeof(pRespond->GMCMD));
	pRespond->wProtocolID = r2s_GM_send_award;
	pRespond->id		  = id;
	pRespond->dwRoleId 	  = dwRoleId;
	
	nLength = strlen(pszGMCMD)+1;
	LOG_PROCESS_ERROR(sizeof(pRespond->GMCMD) >= nLength);
	memcpy(pRespond->GMCMD, pszGMCMD, nLength);
	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL GMSendAwardDB::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}

void GMSendAwardDB::ChangeGMCMDMark(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	R2S_GM_SEND_AWARD*			pRequest            = (R2S_GM_SEND_AWARD*)pbyData;
	BOOL             bResult         = false;
	BOOL             bRetCode        = false;
	int              nRetCode        = false;
	DWORD			 id		 		 = 0;
	PROCESS_ERROR(m_pDBGMCMD);
	id = pRequest->id;
	LOG_PROCESS_ERROR(id);
	nRetCode = snprintf(
		m_szSQL, sizeof(m_szSQL), "update %s set mark = 0 ,LastTime = NOW() where id = %d", 
		GM_SEND_AWARD_TABLE_NAME,id
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));
	bRetCode = DoQuery(m_szSQL);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

BOOL GMSendAwardDB::GetTimeStampByStr(char* pDate,time_t *pTime)
{
	BOOL	bResult	 = FALSE;	
	const char* pStart = pDate;
	char szYear[5], szMonth[3], szDay[3], szHour[3], szMin[3], szSec[3];
	ZeroMemory(szYear,sizeof(szYear));
	ZeroMemory(szMonth,sizeof(szMonth));
	ZeroMemory(szDay,sizeof(szDay));
	ZeroMemory(szHour,sizeof(szHour));
	ZeroMemory(szMin,sizeof(szMin));
	ZeroMemory(szSec,sizeof(szSec));
	LOG_PROCESS_ERROR(pStart);
	LOG_PROCESS_ERROR(pDate);
	LOG_PROCESS_ERROR(strlen(pDate) >= 19);
	
	szYear[0]   = *pDate++; 
	szYear[1]   = *pDate++; 
	szYear[2]   = *pDate++; 
	szYear[3]   = *pDate++; 
	szYear[4]   = 0x0; 
	++pDate; 
	szMonth[0]  = *pDate++; 
	szMonth[1]  = *pDate++; 
	szMonth[2]  = 0x0; 
	++pDate; 
	szDay[0]    = *pDate++;   
	szDay[1]    = *pDate++; 
	szDay[2]    = 0x0; 
	++pDate; 
	szHour[0]   = *pDate++; 
	szHour[1]   = *pDate++; 
	szHour[2]   = 0x0; 
	++pDate; 
	szMin[0]    = *pDate++; 
	szMin[1]    = *pDate++; 
	szMin[2]    = 0x0; 
	++pDate; 
	szSec[0]    = *pDate++; 
	szSec[1]    = *pDate++; 
	szSec[2]    = 0x0; 
	tm tmObj; 
	tmObj.tm_year = atoi(szYear)-1900; 
	tmObj.tm_mon  = atoi(szMonth)-1; 
	tmObj.tm_mday = atoi(szDay); 
	tmObj.tm_hour = atoi(szHour); 
	tmObj.tm_min  = atoi(szMin); 
	tmObj.tm_sec  = atoi(szSec); 
	tmObj.tm_isdst= -1; 
	*pTime = mktime(&tmObj);
	bResult = TRUE;
EXIT0:
	return bResult;
}
