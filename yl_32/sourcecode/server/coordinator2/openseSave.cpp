#include "stdafx.h"
#include "openseSave.h"
#include "GameCenter.h"
#include "DBTools.h"
#include "RoleManager.h"
#include "serverbase/RankDef.h"
#include "onlinegamebase/ksysservice.h"


#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#else
#include "onlinegameworld/kplayermanager_client.h"
#endif

OpenseSave::OpenseSave()
{
	m_pDBGMCMD = NULL;
	mapOpenseSave.clear();
}

OpenseSave::~OpenseSave()
{

}

BOOL OpenseSave::Init()
{
	BOOL	bRetCode = FALSE;
	bRetCode = connect();
	LOG_PROCESS_ERROR(bRetCode);
	return TRUE;
EXIT0:
	return FALSE;
}

void	OpenseSave::Activate()
{
	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_MASTER)
	{
		AutoGiveOpenServerReward();
	}
}

void OpenseSave::UnInit()
{

}

BOOL OpenseSave::connect()
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



BOOL OpenseSave::FindTable(const char* search)
{
	BOOL             bResult         = false;
	BOOL             bRetCode        = false;
	int              nRetCode        = false;
	MYSQL_RES*       pQueryRes       = NULL;
	MYSQL_ROW        QueryRow;
	DWORD			 playerID		 = 0;
	assert(m_pDBGMCMD);
	bRetCode = DoQuery(search);
	LOG_PROCESS_ERROR(bRetCode);

	pQueryRes = mysql_use_result(m_pDBGMCMD);
	LOG_PROCESS_ERROR(pQueryRes);

	while (true)
	{
		DWORD id		  = 0;
		//DWORD playerID_DB = 0;
		DWORD day 		  = 0;
		//char* GMCMD		  =	NULL;
		time_t LastTime	  = 0;		//最后时间
		DWORD mark = 0;
		QueryRow = mysql_fetch_row(pQueryRes);
		if (!QueryRow)
			break;
		id				   = (DWORD)strtoul(QueryRow[0], NULL, 10);
		day 		       = (DWORD)strtoul(QueryRow[1], NULL, 10);
		mark			   = (DWORD)strtoul(QueryRow[2], NULL, 10);
		//day 			   = (DWORD)strtoul(QueryRow[3], NULL, 10);
		//GMCMD			   = QueryRow[3];
		LastTime		   = (DWORD)strtoul(QueryRow[4], NULL, 10);
		//QLogPrintf(LOG_INFO,"id = %d，playerID_DB = %d，GMCMD = %s，mark = %d，LastTime = %d",id,playerID_DB,GMCMD,mark,LastTime);
		//if (mark == 1 /*&& LastTime > g_pGameCenter->m_nTimeNow*/ && playerID_DB == playerID)
		//{
			//BOOL res = DoCommand((LPCSTR)GMCMD);
		//	INT connectID = g_pGameCenter->m_RoleManager.GetRole(playerID)->m_nConnIndex;
		//	m_RespondGMCMD.Lock();
			//QLogPrintf(LOG_INFO,"开始下行协议：connect = %d",playerID);
			//BOOL result = R2S_SendCMD(id,connectID,playerID,GMCMD);
			//QLogPrintf(LOG_INFO,"GMCMD playerID:%d GMCMD:%s connectID:%d",playerID,GMCMD,connectID);		
		//	m_RespondGMCMD.Unlock();
		//}
		if(mapOpenseSave.find(day)==mapOpenseSave.end())
		{
			openseSav saveA = {day,mark};
			mapOpenseSave[day] = saveA;
		}
	}

	bResult = true;
EXIT0:

	if (pQueryRes)
	{
		mysql_free_result(pQueryRes);
		pQueryRes = NULL;
	}
	return bResult;
}


BOOL OpenseSave::DoQuery(const char cszSQL[])
{
	BOOL     bResult            = false;
	BOOL     bRetCode           = false;
	unsigned ulMySQLErrorCode   = 0;

	//assert(cszSQL);
	//assert(m_pDBGMCMD);
	if (m_pDBGMCMD == NULL)
	{
		//printf("**********m_pDBGMCMD  IS NULL \n");
	}

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

BOOL OpenseSave::DoCommand( LPCSTR pszCommand )
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

BOOL OpenseSave::R2S_SendCMD(DWORD id, INT nConnIndex, DWORD dwRoleId, char* GMCMD)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_GM_SEND_AWARD*				pRespond = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_GM_SEND_AWARD));
	pRespond = (R2S_GM_SEND_AWARD*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_GM_send_award;
	pRespond->id		 = id;
	pRespond->dwRoleId = dwRoleId;
	memcpy(pRespond->GMCMD, GMCMD, strlen(GMCMD)+1);	
	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL OpenseSave::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}

void OpenseSave::ChangeGMCMDMark(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	/*R2S_GM_SEND_AWARD*			pRequest            = (R2S_GM_SEND_AWARD*)pbyData;
	BOOL             bResult         = false;
	BOOL             bRetCode        = false;
	int              nRetCode        = false;
	DWORD			 id		 = 0;
	assert(m_pDBGMCMD);

	id = pRequest->id;
	LOG_PROCESS_ERROR(id);

	nRetCode = snprintf(
		m_szSQL, sizeof(m_szSQL), "update %s set mark=0 where id = %d", 
		GM_SEND_AWARD_TABLE_NAME,id
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

	bRetCode = DoQuery(m_szSQL);

	//QLogPrintf(LOG_INFO,"-------------上行上行 %s",m_szSQL);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;*/
}


void OpenseSave::signalRun(int nDay)
{ 
	int ti = 0;
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	char saveBuffer[1024];
	memset(buffer,0,sizeof(saveBuffer));
	//g_pGameCenter->m_OpenseSave.FindTable("select * from OpenseSave");
	
	switch(nDay)
	{ 
		case 1:
				//if(g_pGameCenter->m_OpenseSave.mapOpenseSave[g_pGameCenter->gDay-1].mar==0)
				//{
					for(ti = 0;ti!=10;++ti)
					{ 
						if(g_pGameCenter->m_RankManager.GetRankPool(emRANK_ROLE_LEVEL_OPEN_SERVER)->GetIdByIdx(ti)!=0)
						{ 

							switch(ti)
							{
								case 0:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1111)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 1:	
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1112)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 2:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1112)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 3:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1113)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 4:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1113)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 5:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1113)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 6:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1113)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 7:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1113)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 8:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1113)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 9:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1113)',0)",g_pGameCenter->m_RankManager.GetRankPool(11)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								default:
									break;
							}
						}

					}
					//sprintf(saveBuffer,"INSERT INTO OpenseSave VALUE (0,%d,1,NOW())",g_pGameCenter->gDay-1);
					//g_pGameCenter->m_OpenseSave.DoQuery(saveBuffer);
				//}
				
			break;
		case 2: 
				//if(g_pGameCenter->m_OpenseSave.mapOpenseSave[g_pGameCenter->gDay-1].mar==0)
				//{
					for(ti = 0;ti!=10;++ti)
					{ 
						if(g_pGameCenter->m_RankManager.GetRankPool(emRANK_RIDE_LV)->GetIdByIdx(ti)!=0)
						{ 
							switch(ti)
							{
								case 0:	
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1121)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 1:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1122)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 2:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1122)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 3:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1123)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 4:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1123)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 5:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1123)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 6:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1123)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 7:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1123)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 8:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1123)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 9:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1123)',0)",g_pGameCenter->m_RankManager.GetRankPool(5)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								default:
									break;
							}
						}

					}
					//sprintf(saveBuffer,"INSERT INTO OpenseSave VALUE (0,%d,1,NOW())",g_pGameCenter->gDay-1);
					//g_pGameCenter->m_OpenseSave.DoQuery(saveBuffer);
				//}
				
			break;
		case 3:
				//if(g_pGameCenter->m_OpenseSave.mapOpenseSave[g_pGameCenter->gDay-1].mar==0)
				//{
					for(ti = 0;ti!=10;++ti)
					{ 
						if(g_pGameCenter->m_RankManager.GetRankPool(emRANK_AREAN_OS)->GetIdByIdx(ti)!=0)
						{ 
							switch(ti)
							{
								case 0:	
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1131)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 1:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1132)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 2:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1132)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 3:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1133)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 4:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1133)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 5:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1133)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 6:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1133)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 7:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1133)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 8:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1133)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 9:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1133)',0)",g_pGameCenter->m_RankManager.GetRankPool(1)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								default:
									break;
							}
						}

					}
					//sprintf(saveBuffer,"INSERT INTO OpenseSave VALUE (0,%d,1,NOW())",g_pGameCenter->gDay-1);
					//g_pGameCenter->m_OpenseSave.DoQuery(saveBuffer);
				//}
			break;
		case 4: 
				//if(g_pGameCenter->m_OpenseSave.mapOpenseSave[g_pGameCenter->gDay-1].mar==0)
				//{
					for(ti = 0;ti!=10;++ti)
					{ 
						if(g_pGameCenter->m_RankManager.GetRankPool(emRANK_PET_LV_OS)->GetIdByIdx(ti)!=0)
						{ 
							switch(ti)
							{
								case 0:	
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1141)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 1:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1142)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 2:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1142)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 3:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1143)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 4:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1143)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 5:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1143)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 6:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1143)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 7:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1143)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 8:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1143)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 9:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1143)',0)",g_pGameCenter->m_RankManager.GetRankPool(3)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								default:
									break;
							}
						}

					}
					//sprintf(saveBuffer,"INSERT INTO OpenseSave VALUE (0,%d,1,NOW())",g_pGameCenter->gDay-1);
					//g_pGameCenter->m_OpenseSave.DoQuery(saveBuffer);
				//}				
			break;
		case 5: 
				//if(g_pGameCenter->m_OpenseSave.mapOpenseSave[g_pGameCenter->gDay-1].mar==0)
				//{
					for(ti = 0;ti!=10;++ti)
					{ 
						//g_pGameCenter->m_RankManager.GetRankPool(5);
						if(g_pGameCenter->m_RankManager.GetRankPool(emRANK_LG_OS)->GetIdByIdx(ti)!=0)
						{ 
							switch(ti)
							{
								case 0:	
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1151)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 1:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1152)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 2:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1152)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 3:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1153)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 4:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1153)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 5:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1153)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 6:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1153)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 7:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1153)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 8:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1153)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 9:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1153)',0)",g_pGameCenter->m_RankManager.GetRankPool(7)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								default:
									break;
							}
						}

					}
					//sprintf(saveBuffer,"INSERT INTO OpenseSave VALUE (0,%d,1,NOW())",g_pGameCenter->gDay-1);
					//g_pGameCenter->m_OpenseSave.DoQuery(saveBuffer);
				//}				
			break;
		case 6: 
				//if(g_pGameCenter->m_OpenseSave.mapOpenseSave[g_pGameCenter->gDay-1].mar==0)
				//{
					for(ti = 0;ti!=10;++ti)
					{ 
						if(g_pGameCenter->m_RankManager.GetRankPool(emRANK_TOWER)->GetIdByIdx(ti)!=0)
						{ 
							switch(ti)
							{
								case 0:	
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1161)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 1:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1162)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 2:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1162)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 3:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1163)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 4:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1163)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 5:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1163)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 6:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1163)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 7:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1163)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 8:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1163)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 9:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1163)',0)",g_pGameCenter->m_RankManager.GetRankPool(16)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								default:
									break;
							}
						}

					}
					//sprintf(saveBuffer,"INSERT INTO OpenseSave VALUE (0,%d,1,NOW())",g_pGameCenter->gDay-1);
					//g_pGameCenter->m_OpenseSave.DoQuery(saveBuffer);
				//}
			break;
		case 7: 
				//if(g_pGameCenter->m_OpenseSave.mapOpenseSave[g_pGameCenter->gDay-1].mar==0)
				//{
					for(ti = 0;ti!=10;++ti)
					{ 
						if(g_pGameCenter->m_RankManager.GetRankPool(emRANK_ROLE_FIGSHTSCORE_OS)->GetIdByIdx(ti)!=0)
						{ 
							switch(ti)
							{
								case 0:	
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1171)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 1:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1172)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 2:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1172)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 3:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1173)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 4:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1173)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 5:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1173)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 6:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1173)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 7:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1173)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 8:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1173)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								case 9:
									sprintf(buffer,"INSERT INTO gmcmdaward(id,playerID,mark,GMCMD,LastTime) VALUE (0,%d,1,'?gs Player:checkAssetAward(me,1173)',0)",g_pGameCenter->m_RankManager.GetRankPool(2)->GetIdByIdx(ti));
									g_pGameCenter->m_GMSendAward.DoQuery(buffer);
									break;
								default:
									break;
							}
						}

					}
					//sprintf(saveBuffer,"INSERT INTO OpenseSave VALUE (0,%d,1,NOW())",g_pGameCenter->gDay-1);
					//g_pGameCenter->m_OpenseSave.DoQuery(saveBuffer);
				//}				
			break;
		default:
			break;
	}
}


//by ldy
void OpenseSave::AutoGiveOpenServerReward()
{
	DWORD * openServerData = NULL;
	int i = 0;
	DWORD dDay = 0;
	DWORD dwOpenServerTime =  0;
	time_t dwTimeNow = 0;
	int nDeltaTime = 0;
	dwOpenServerTime = g_pGameCenter->m_RecordManager.GetOpenServerTime();
	openServerData = g_pGameCenter->m_RecordManager.GetOpenServerData();
	dwTimeNow = time(NULL);
	//nDeltaTime = (dwTimeNow + 8*60*60) / 86400 - (dwOpenServerTime + 8*60*60) / 86400;
	nDeltaTime =  KSysService::GetDayDiff(dwOpenServerTime,dwTimeNow);
	if(nDeltaTime < 7)
	{
		for(i ; i< nDeltaTime + 1 ;i++)
		{	
			struct tm localtm = *localtime(&dwTimeNow); 
			if(localtm.tm_hour != 23  && i == nDeltaTime)
			{
				continue;
			}
			
			dDay = openServerData[i];
			if(dDay == 0)
			{
				openServerData[i] = dwTimeNow;
				signalRun(i+1);
			}
		}
		
	}


}






