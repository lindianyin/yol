/* -------------------------------------------------------------------------
//	文件名		：	kmailplayermanager.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/19 15:04:00
//	功能描述	：	与玩家相关的邮件状态管理
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kmailplayermanager.h"
#include "kgc_mailmodule.h"

#define KD_MAIL_TABLE_MAILPLAYER		"tblmailplayer"
#define KD_MAIL_FIELD_PLAYER_ID			"playerid"
#define KD_MAIL_FIELD_SYSMAILID			"sysmailid"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailPlayerManager KMailPlayerManager::ms_inst;

KMailPlayerManager::KMailPlayerManager() : m_pIKGcDataBase(NULL)
{
	memset(m_szSQL, 0, sizeof(m_szSQL));
}

KMailPlayerManager::~KMailPlayerManager()
{
}

BOOL KMailPlayerManager::Init(INT nPlayerInitCount, INT nPlayerBatchCount)
{
	QCONFIRM_RET_FALSE(nPlayerInitCount > 0 && nPlayerBatchCount > 0);
	
	IKGcQuery* pIKGcQuery = KGC_MailModule::Instance()->GetIKGcQuery();
	QCONFIRM_RET_FALSE(NULL != pIKGcQuery);
	BOOL bRet = pIKGcQuery->QueryInterface(IID_IKG_GCDatabase, (void**)&m_pIKGcDataBase);
	QCONFIRM_RET_FALSE(bRet);

	m_poolKey.Init(1, 1);
	m_poolMailPlayer.Init(nPlayerInitCount, nPlayerBatchCount);
	m_mapMailPlayer.clear();

	if (!InitMailPlayerDataBase() || !Load())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL KMailPlayerManager::InitMailPlayerDataBase()
{
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	LPCSTR pszSQL = "create table if not exists %s("
		"%s INT unsigned primary key,"
		"%s BIGINT unsigned not null,"
		"index(%s))";
	sprintf(m_szSQL, pszSQL,
		KD_MAIL_TABLE_MAILPLAYER,
		KD_MAIL_FIELD_PLAYER_ID,
		KD_MAIL_FIELD_SYSMAILID,
		KD_MAIL_FIELD_PLAYER_ID);
	if (!rcDataBase.DoQuery(m_szSQL, (DWORD)strlen(m_szSQL)))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailPlayerManager Create MailPlayer Table Failed!");
		return FALSE;
	}

	return TRUE;
}

BOOL KMailPlayerManager::Load()
{
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	INT nLen = sprintf(m_szSQL,
		"select %s,%s from %s",
		KD_MAIL_FIELD_PLAYER_ID,
		KD_MAIL_FIELD_SYSMAILID,
		KD_MAIL_TABLE_MAILPLAYER);
	if (!rcDataBase.DoQuery(m_szSQL, (DWORD)nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager Load MailPlayerInfo exc sql failed!");
		return FALSE;
	}

	KMySqlResult sResult = rcDataBase.GetQueryResult();
	if (!sResult.IsValid() || sResult.GetFieldCount() != 2)
	{
		if (sResult.IsValid())
			sResult.Release();

		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager Load MailPlayerInfo Result Error!");
		return FALSE;
	}
	
	INT nRowCount = sResult.GetRowCount();
	for (INT i = 0; i < nRowCount; ++i)
	{
		KMySqlRow sRow = sResult.GetNextRow();
		QCONFIRM_BREAK(sRow.IsValid());
		
		KMAIL_PLAYER* pInfo = m_poolMailPlayer.New();
		QCONFIRM_CONTINUE(pInfo);

		INT nIndex = 0;
		pInfo->dwPlayerId = atoi(sRow.aFieldData[nIndex++]);
		sscanf(sRow.aFieldData[nIndex], "%lld", &(pInfo->nSysMailId));

		std::pair<KMAILPLAYER_MAP::iterator, bool> sRes = m_mapMailPlayer.insert(std::make_pair(pInfo->dwPlayerId, pInfo));
		QCONFIRM_CONTINUE(sRes.second);
	}
	sResult.Release();

	return TRUE;
}

BOOL KMailPlayerManager::UnInit()
{
	return SaveAll();
}

BOOL KMailPlayerManager::OnPlayerLogin(DWORD dwPlayerId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		// 玩家第一次登录游戏
		KMAIL_PLAYER* pInfo = m_poolMailPlayer.New();
		if (!pInfo)
		{
			QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager::OnPlayerLogin Allocate MailPlayer Failed!");
			return FALSE;
		}

		pInfo->dwPlayerId = dwPlayerId;
		std::pair<KMAILPLAYER_MAP::iterator, bool> sRes = m_mapMailPlayer.insert(std::make_pair(pInfo->dwPlayerId, pInfo));
		if (!sRes.second)
		{
			m_poolMailPlayer.Free(pInfo);
			
			QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager::OnPlayerLogin insert map Failed!");
			return FALSE;
		}

		// 存档
		KKEY_INFO* pKey = m_poolKey.New();
		memset(pKey, 0, sizeof(KKEY_INFO));

		pKey->dwPlayerId = pInfo->dwPlayerId;
		pKey->nSysMailId = pInfo->nSysMailId;

		m_pIKGcDataBase->PushDBAffair(this, emKCMD_ADDINFO, pKey);
	}
	
	return TRUE;
}

BOOL KMailPlayerManager::OnPlayerLogout(DWORD dwPlayerId)
{
	return Save(dwPlayerId);
}

BOOL KMailPlayerManager::OnShutDown()
{
	return SaveAll();
}

BOOL KMailPlayerManager::SaveAll()
{
	BOOL bResult = TRUE;
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.begin();
	for (; it != m_mapMailPlayer.end(); it++)
	{
		KMAIL_PLAYER* sInfo = it->second;
		if (!SaveOne(sInfo))
		{
			QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SaveAll PlayerId:%d Save failed!", sInfo->dwPlayerId);
			bResult = FALSE;
			continue;
		}
	}

	return bResult;
}

BOOL KMailPlayerManager::SaveOne(KMAIL_PLAYER* pInfo)
{
	KKEY_INFO* pKey = m_poolKey.New();
	memset(pKey, 0, sizeof(KKEY_INFO));

	pKey->dwPlayerId = pInfo->dwPlayerId;
	pKey->nSysMailId = pInfo->nSysMailId;

	return m_pIKGcDataBase->PushDBAffair(this, emKCMD_MODINFO, pKey);
}

BOOL KMailPlayerManager::SetSysMailId(DWORD dwPlayerId, INT64 nSysMailId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetSysMailId Not Found Id:%d player!", dwPlayerId);
		return FALSE;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetSysMailId Id:%d playerInfo null!", dwPlayerId);
		return FALSE;
	}
	
	pInfo->nSysMailId = nSysMailId;
	return TRUE;
}

INT64 KMailPlayerManager::GetSysMailId(DWORD dwPlayerId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetSysMailId Not Found Id:%d player!", dwPlayerId);
		return 0;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetSysMailId Id:%d playerInfo null!", dwPlayerId);
		return 0;
	}

	return pInfo->nSysMailId;
}

BOOL KMailPlayerManager::SetMailLoading(DWORD dwPlayerId, BYTE byMailLoading)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetMailLoading Not Found Id:%d player!", dwPlayerId);
		return FALSE;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetMailLoading Id:%d playerInfo null!", dwPlayerId);
		return FALSE;
	}

	pInfo->byMailLoading = byMailLoading;
	return TRUE;
}

BYTE KMailPlayerManager::GetMailLoading(DWORD dwPlayerId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetMailLoading Not Found Id:%d player!", dwPlayerId);
		return 0;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetMailLoading Id:%d playerInfo null!", dwPlayerId);
		return 0;
	}

	return pInfo->byMailLoading;
}

BOOL KMailPlayerManager::SetMailLoaded(DWORD dwPlayerId, BYTE byMailLoaded)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetMailLoaded Not Found Id:%d player!", dwPlayerId);
		return FALSE;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetMailLoaded Id:%d playerInfo null!", dwPlayerId);
		return FALSE;
	}

	pInfo->byMailLoaded = byMailLoaded;
	return TRUE;
}

BYTE KMailPlayerManager::GetMailLoaded(DWORD dwPlayerId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetMailLoaded Not Found Id:%d player!", dwPlayerId);
		return 0;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetMailLoaded Id:%d playerInfo null!", dwPlayerId);
		return 0;
	}

	return pInfo->byMailLoaded;
}

BOOL KMailPlayerManager::SetMailFetchLock(DWORD dwPlayerId, BYTE byMailFetchLock)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetMailFetchLock Not Found Id:%d player!", dwPlayerId);
		return FALSE;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetMailFetchLock Id:%d playerInfo null!", dwPlayerId);
		return FALSE;
	}

	pInfo->byMailFetchLock = byMailFetchLock;
	return TRUE;
}

BYTE KMailPlayerManager::GeMailFetchLock(DWORD dwPlayerId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GeMailFetchLock Not Found Id:%d player!", dwPlayerId);
		return 0;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GeMailFetchLock Id:%d playerInfo null!", dwPlayerId);
		return 0;
	}

	return pInfo->byMailFetchLock;
}

BOOL KMailPlayerManager::SetPlayerMailCount(DWORD dwPlayerId, INT nCount)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetPlayerMailCount Not Found Id:%d player!", dwPlayerId);
		return FALSE;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::SetPlayerMailCount Id:%d playerInfo null!", dwPlayerId);
		return FALSE;
	}

	pInfo->nPlayerMailCount = nCount;
	return TRUE;
}

INT KMailPlayerManager::GetPlayerMailCount(DWORD dwPlayerId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetPlayerMailCount Not Found Id:%d player!", dwPlayerId);
		return 0;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::GetPlayerMailCount Id:%d playerInfo null!", dwPlayerId);
		return 0;
	}

	return pInfo->nPlayerMailCount;
}

BOOL KMailPlayerManager::Save(DWORD dwPlayerId)
{
	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(dwPlayerId);
	if (it == m_mapMailPlayer.end())
		return FALSE;

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		QLogPrintf(LOG_ERR, "[mail] KMailPlayerManager::Save Id:%d playerInfo null!", dwPlayerId);
		return FALSE;
	}

	pInfo->byMailLoaded = 0;
	pInfo->byMailLoading = 0;
	pInfo->byMailFetchLock = 0;

	return SaveOne(pInfo);
}

BOOL KMailPlayerManager::DBThreadCallback(INT nParam, LPVOID pParam)
{
	BOOL bResult = TRUE;
	switch (nParam)
	{
	case emKCMD_GETINFO:
		Push_GetInfo(nParam, pParam);
		break;
	case emKCMD_ADDINFO:
		Push_AddInfo(nParam, pParam);
		break;
	case emKCMD_DELINFO:
		Push_DeleteInfo(nParam, pParam);
		break;
	case emKCMD_MODINFO:
		Push_ModifyInfo(nParam, pParam);
		break;
	default:
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager::DBThreadCallback Error Type!");
		bResult = FALSE;
		break;
	}

	return bResult;
}

BOOL KMailPlayerManager::MainThreadCallback(INT nParam, LPVOID pParam)
{
	BOOL bResult = TRUE;
	switch (nParam)
	{
	case emKCMD_GETINFO:
		Pop_GetInfo(nParam, pParam);
		break;
	case emKCMD_ADDINFO:
		Pop_AddInfo(nParam, pParam);
		break;
	case emKCMD_DELINFO:
		Pop_DeleteInfo(nParam, pParam);
		break;
	case emKCMD_MODINFO:
		Pop_ModifyInfo(nParam, pParam);
		break;
	default:
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager::MainThreadCallback Error Type!");
		bResult = FALSE;
		break;
	}

	return bResult;
}

BOOL KMailPlayerManager::Push_GetInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	INT nLen = sprintf(m_szSQL,
		"select %s,%s from %s where %s=%d",
		KD_MAIL_FIELD_PLAYER_ID,
		KD_MAIL_FIELD_SYSMAILID,
		KD_MAIL_TABLE_MAILPLAYER,
		KD_MAIL_FIELD_PLAYER_ID,
		pKey->dwPlayerId);
	if (rcDataBase.DoQuery(m_szSQL, (DWORD)nLen))
	{
		pKey->sResult = rcDataBase.GetQueryResult();
		pKey->bResult = TRUE;
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailPlayerManager::Pop_GetInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	if (!pKey->bResult || !pKey->sResult.IsValid() || pKey->sResult.GetFieldCount() != 8)
	{
		if (pKey->sResult.IsValid())
			pKey->sResult.Release();
		// 执行SQL语句失败，释放资源
		m_poolKey.Free(pKey);
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager GetInfo exc sql failed!");
		return FALSE;
	}

	KMAILPLAYER_MAP::iterator it = m_mapMailPlayer.find(pKey->dwPlayerId);
	if (it == m_mapMailPlayer.end())
	{
		pKey->sResult.Release();
		m_poolKey.Free(pKey);
		return FALSE;
	}

	KMAIL_PLAYER* pInfo = it->second;
	if (!pInfo)
	{
		pKey->sResult.Release();
		m_poolKey.Free(pKey);
		return FALSE;
	}

	KMySqlRow sRow = pKey->sResult.GetNextRow();
	if (!sRow.IsValid())
	{
		pKey->sResult.Release();
		m_poolKey.Free(pKey);
		return FALSE;
	}

	INT nIndex = 0;
	pInfo->dwPlayerId = atoi(sRow.aFieldData[nIndex++]);
	sscanf(sRow.aFieldData[nIndex++], "%lld", &(pInfo->nSysMailId));
	pKey->sResult.Release();
	m_poolKey.Free(pKey);
	
	return TRUE;
}

BOOL KMailPlayerManager::Push_AddInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	INT nLen = sprintf(m_szSQL,
		"insert into %s(%s,%s) values(%d,%lld)",
		KD_MAIL_TABLE_MAILPLAYER,
		KD_MAIL_FIELD_PLAYER_ID,
		KD_MAIL_FIELD_SYSMAILID,
		pKey->dwPlayerId,
		pKey->nSysMailId);
	if (!rcDataBase.DoQuery(m_szSQL, (DWORD)nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager AddInfo exc sql failed!");
	}

	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailPlayerManager::Pop_AddInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	pKey->sResult.Release();
	m_poolKey.Free(pKey);
	return TRUE;
}

BOOL KMailPlayerManager::Push_DeleteInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	INT nLen = sprintf(m_szSQL,
		"delete from %s where %s=%d",
		KD_MAIL_TABLE_MAILPLAYER,
		KD_MAIL_FIELD_PLAYER_ID,
		pKey->dwPlayerId);
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager DeleteInfo exc sql failed!");
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailPlayerManager::Pop_DeleteInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	pKey->sResult.Release();
	m_poolKey.Free(pKey);
	return TRUE;
}

BOOL KMailPlayerManager::Push_ModifyInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	INT nLen = sprintf(m_szSQL,
		"update %s set %s=%d, %s=%lld where %s=%d",
		KD_MAIL_TABLE_MAILPLAYER,
		KD_MAIL_FIELD_PLAYER_ID,
		pKey->dwPlayerId,
		KD_MAIL_FIELD_SYSMAILID,
		pKey->nSysMailId,
		KD_MAIL_FIELD_PLAYER_ID,
		pKey->dwPlayerId);
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailPlayerManager ModifyInfo exc sql failed!");
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailPlayerManager::Pop_ModifyInfo(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_INFO* pKey = (KKEY_INFO*)pParam;
	pKey->sResult.Release();
	m_poolKey.Free(pKey);
	return TRUE;
}

// -------------------------------------------------------------------------

