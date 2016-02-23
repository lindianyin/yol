/* -------------------------------------------------------------------------
//	文件名		：	kmaildatabase.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/16 16:36:12
//	功能描述	：	邮件数据库（负责邮件的存取）
//
// -----------------------------------------------------------------------*/

#ifdef GAME_CENTER
#include "stdafx.h"
#include "kmaildatabase.h"
#include "kmailplayermanager.h"
#include "kgc_mailmodule.h"
#include "onlinegamebase/ksysservice.h"

#define KD_MAIL_TABLE_MAILBOX			"tblmailbox"
#define KD_MAIL_TABLE_SYSMAIL			"tblconditionmail"
#define KD_MAIL_FIELD_MAILID			"mailid"
#define KD_MAIL_FIELD_OWNER				"owner"
#define KD_MAIL_FIELD_SENDER			"sender"
#define KD_MAIL_FIELD_RECEIVER			"receiver"
#define KD_MAIL_FIELD_SENDER_NAME		"sendername"
#define KD_MAIL_FIELD_TYPE				"type"
#define KD_MAIL_FIELD_STATUS			"status"
#define KD_MAIL_FIELD_SENDTIME			"sendtime"
#define KD_MAIL_FIELD_MAIL				"mail"
#define KD_MAIL_FIELD_CONDITION			"cond"

#define KD_MAIL_MAX_SEARCH_COUNT		100000	// 最大轮询数据库的条数

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailDatabase KMailDatabase::ms_inst;

KMailDatabase::KMailDatabase() : m_nNextMailBoxID(0), m_nNextSysMailID(0), m_nMailExpireTime(30), m_pIKGcDataBase(NULL)
{
}

KMailDatabase::~KMailDatabase()
{
}

BOOL KMailDatabase::Init(INT nMailExpireTime, IKMailCB_FetchSysMail* pCallBack)
{
	QCONFIRM_RET_FALSE(nMailExpireTime > 0);
	
	m_poolQueryRes.Init(1, 1);
	m_poolQueryKey.Init(1, 1);
	m_poolInsertKey.Init(1, 1);
	m_poolMailIDKey.Init(1, 1);
	m_poolModifyKey.Init(1, 1);
	m_poolGuidKey.Init(1, 1);
	m_poolSysMailKey.Init(1, 1);
	
	m_nMailExpireTime = nMailExpireTime;
	
	IKGcQuery* pIKGcQuery = KGC_MailModule::Instance()->GetIKGcQuery();
	QCONFIRM_RET_FALSE(NULL != pIKGcQuery);
	BOOL bRet = pIKGcQuery->QueryInterface(IID_IKG_GCDatabase, (void**)&m_pIKGcDataBase);
	QCONFIRM_RET_FALSE(bRet);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	if (InitMailBoxDatabase(&rcDataBase) &&
		InitSysMailDatabase(&rcDataBase, pCallBack) &&
		LoadPlayerMailCount(&rcDataBase))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL KMailDatabase::UnInit()
{
	return TRUE;
}

// 初始化个人邮箱数据库
BOOL KMailDatabase::InitMailBoxDatabase(KMySqlDatabase* pDatabase)
{
	QCONFIRM_RET_FALSE(pDatabase);
	LPCSTR pszSQL = "create table if not exists %s("
		"%s BIGINT unsigned primary key,"
		"%s INT unsigned not null,"
		"%s INT unsigned not null,"
		"%s INT unsigned not null,"
		"%s varchar(32) not null,"
		"%s INT not null,"
		"%s INT not null,"
		"%s datetime not null,"
		"%s tinyblob not null,"
		"%s blob not null,"
		"index(%s), index(%s), index(%s))";
	sprintf(m_szSQL, pszSQL,
		KD_MAIL_TABLE_MAILBOX,
		KD_MAIL_FIELD_MAILID,
		KD_MAIL_FIELD_OWNER,
		KD_MAIL_FIELD_SENDER,
		KD_MAIL_FIELD_RECEIVER,
		KD_MAIL_FIELD_SENDER_NAME,
		KD_MAIL_FIELD_TYPE,
		KD_MAIL_FIELD_STATUS,
		KD_MAIL_FIELD_SENDTIME,
		KD_MAIL_FIELD_CONDITION,
		KD_MAIL_FIELD_MAIL,
		KD_MAIL_FIELD_OWNER,
		KD_MAIL_FIELD_SENDER,
		KD_MAIL_FIELD_SENDTIME);
	if (!pDatabase->DoQuery(m_szSQL, (DWORD)strlen(m_szSQL)))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDateBase Create Mail Table failed!");
		return FALSE;
	}

	// 删除过期邮件
	DBDeleteExpire();
	// 获取起始邮件ID
	return InitMailBoxID(pDatabase);
}

// 初始化系统邮件数据库
BOOL KMailDatabase::InitSysMailDatabase(KMySqlDatabase* pDatabase, IKMailCB_FetchSysMail* pCallBack)
{
	QCONFIRM_RET_FALSE(pDatabase);
	LPCSTR pszSQL = "create table if not exists %s("
		"%s BIGINT unsigned primary key,"
		"%s datetime not null,"
		"%s tinyblob not null,"
		"%s blob not null,"
		"index(%s))";
	sprintf(m_szSQL, pszSQL,
		KD_MAIL_TABLE_SYSMAIL,
		KD_MAIL_FIELD_MAILID,
		KD_MAIL_FIELD_SENDTIME,
		KD_MAIL_FIELD_CONDITION,
		KD_MAIL_FIELD_MAIL,
		KD_MAIL_FIELD_SENDTIME);
	if (!pDatabase->DoQuery(m_szSQL, (DWORD)strlen(m_szSQL)))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDateBase Create Sys Mail Table failed!");
		return FALSE;
	}

	// 获取起始邮件ID
	if (!InitSysMailID(pDatabase))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase InitSysMailId Failed!");
		return FALSE;
	}
	// 删除过期邮件
	if (!DelOutdatedCondMail(pDatabase))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase DelOutdatedCondMail Failed!");
		return FALSE;
	}
	// 获取条件邮件
	if (pCallBack != NULL && !GetCondMail(pDatabase, pCallBack))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase GetCondMail Failed!");
		return FALSE;
	}
	return TRUE;
}

// 统计个人邮箱的玩家邮件个数
BOOL KMailDatabase::LoadPlayerMailCount(KMySqlDatabase* pDatabase)
{
	QCONFIRM_RET_FALSE(pDatabase);

	LPCSTR pszSQL = "select %s,%s from %s where %s >= %lld and %s <= %lld";
	INT64 nStartMailId = 1;
	INT64 nEndMailId = KD_MAIL_MAX_SEARCH_COUNT;
	while (nStartMailId <= m_nNextMailBoxID)
	{
		if (nEndMailId > m_nNextMailBoxID)
			nEndMailId = m_nNextMailBoxID;

		sprintf(m_szSQL, pszSQL,
			KD_MAIL_FIELD_OWNER,
			KD_MAIL_FIELD_TYPE,
			KD_MAIL_TABLE_MAILBOX,
			KD_MAIL_FIELD_MAILID,
			nStartMailId,
			KD_MAIL_FIELD_MAILID,
			nEndMailId);
		if (!pDatabase->DoQuery(m_szSQL, (DWORD)strlen(m_szSQL)))
		{
			QLogPrintf(LOG_ERR, "[mail] %s", "MailDateBase LoadPlayerMailCount failed!");
			return FALSE;
		}

		KMySqlResult sResult = pDatabase->GetQueryResult();
		if (!sResult.IsValid())
		{
			QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase LoadPlayerMailCount exc sql failed!");
			sResult.Release();
			return FALSE;
		}

		INT nRowCount = sResult.GetRowCount();
		for (INT i = 0; i < nRowCount; ++i)
		{
			KMySqlRow sRow = sResult.GetNextRow();
			QCONFIRM_BREAK(sRow.IsValid());

			DWORD dwOwnerId = atoi(sRow.aFieldData[0]);
			INT nType = atoi(sRow.aFieldData[1]);
			if (nType == emKMAIL_TYPE_PLAYER)
			{
				INT nCount = KMailPlayerManager::Instance()->GetPlayerMailCount(dwOwnerId);
				KMailPlayerManager::Instance()->SetPlayerMailCount(dwOwnerId, nCount + 1);
			}
		}
		sResult.Release();

		nStartMailId = nStartMailId + KD_MAIL_MAX_SEARCH_COUNT;
		nEndMailId = nEndMailId + KD_MAIL_MAX_SEARCH_COUNT;
	}

	return TRUE;
}

// 删除过期邮件
BOOL KMailDatabase::DBDeleteExpire() 
{
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	time_t t = KSysService::Time(NULL) - m_nMailExpireTime * 24 * 3600;
	tm *pTime = ::localtime(&t);
	CHAR szTime[128];
	sprintf(szTime,
		"%4d-%02d-%02d %02d:%02d:%02d",
		pTime->tm_year + 1900,
		pTime->tm_mon + 1,
		pTime->tm_mday,
		pTime->tm_hour,
		pTime->tm_min,
		pTime->tm_sec);

	INT nLen = sprintf(m_szSQL, "delete from %s where %s < '%s'",
		KD_MAIL_TABLE_MAILBOX, KD_MAIL_FIELD_SENDTIME, szTime);
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase DeleteExpire exc sql failed!");
	}
	return TRUE;
}

// 删除过时的系统邮件
BOOL KMailDatabase::DelOutdatedCondMail(KMySqlDatabase* pDatabase)
{
	QCONFIRM_RET_FALSE(pDatabase);

	CHAR szBuf[2048];
	time_t t = KSysService::Time(NULL) - m_nMailExpireTime * 24 * 3600;
	tm* pTime = ::localtime(&t);
	CHAR szTime[128];
	sprintf(szTime,
		"%4d-%02d-%02d %02d:%02d:%02d",
		pTime->tm_year + 1900,
		pTime->tm_mon + 1,
		pTime->tm_mday,
		pTime->tm_hour,
		pTime->tm_min,
		pTime->tm_sec);

	sprintf(szBuf, "delete from %s where %s<'%s'",
		KD_MAIL_TABLE_SYSMAIL,
		KD_MAIL_FIELD_SENDTIME,
		szTime);

	if (!pDatabase->DoQuery(szBuf, (DWORD)strlen(szBuf)))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase DelOutdatedCondMail exc sql failed!");
		return FALSE;
	}
	return TRUE;
}

// 获取系统邮件
BOOL KMailDatabase::GetCondMail(KMySqlDatabase* pDatabase, IKMailCB_FetchSysMail* pCallBack)
{
	QCONFIRM_RET_FALSE(pDatabase && pCallBack);

	sprintf(m_szSQL, "select %s,%s,%s,%s from %s",
		KD_MAIL_FIELD_MAILID,
		KD_MAIL_FIELD_SENDTIME,
		KD_MAIL_FIELD_CONDITION,
		KD_MAIL_FIELD_MAIL,
		KD_MAIL_TABLE_SYSMAIL);
	if (!pDatabase->DoQuery(m_szSQL, (DWORD)strlen(m_szSQL)))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase SelectCondMail exc sql failed!");
		return FALSE;
	}

	KMySqlResult sResult = pDatabase->GetQueryResult();
	if (!sResult.IsValid() || sResult.GetFieldCount() != 4)
	{
		if (sResult.IsValid())
			sResult.Release();

		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase GetCondMail Result Error!");
		return FALSE;
	}

	INT nRowCount = sResult.GetRowCount();
	KCONDMAIL_INFO sSysMail;
	for (INT i = 0; i < nRowCount; ++i)
	{
		KMySqlRow sRow = sResult.GetNextRow();
		INT nIndex = 0;
		sscanf(sRow.aFieldData[nIndex++], "%lld", &(sSysMail.nMailID));
		DecodeDateTime(&sSysMail.sSendTime, sRow.aFieldData[nIndex++]);
		sSysMail.pCond = sRow.aFieldData[nIndex];
		sSysMail.nCondLen = sRow.aFieldLenth[nIndex++];
		sSysMail.pMail = sRow.aFieldData[nIndex];
		sSysMail.nMailLen = sRow.aFieldLenth[nIndex++];
		pCallBack->FetchSysMail(&sSysMail);
	}
	sResult.Release();
	return TRUE;
}

BOOL KMailDatabase::InitMailID(KMySqlDatabase* pDatabase, LPCSTR pszTable, LPCSTR pszField, INT64& nMailID)
{
	QCONFIRM_RET_FALSE(pDatabase && pszTable && pszField);

	CHAR szBuf[1024];
	sprintf(szBuf, "select max(%s) from %s", pszField, pszTable);
	if (!pDatabase->DoQuery(szBuf, (DWORD)strlen(szBuf)))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase Select Max MailId exc sql failed!");
		return FALSE;
	}

	KMySqlResult sResult = pDatabase->GetQueryResult();
	if (!sResult.IsValid() || sResult.GetRowCount() != 1 || sResult.GetFieldCount() != 1)
	{
		if (sResult.IsValid())
			sResult.Release();

		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase SelectMaxMailId Result Error!");
		return FALSE;
	}

	KMySqlRow sRow = sResult.GetNextRow();
	DWORD nLen = sRow.aFieldLenth[0];
	if (nLen == 0)
	{
		nMailID = 1;
	}
	else
	{
		sscanf(sRow.aFieldData[0], "%lld", &nMailID);
		nMailID = nMailID + 1;
	}
	sResult.Release();

	QCONFIRM_RET_FALSE(nMailID != 0);
	return TRUE;
}

BOOL KMailDatabase::InitMailBoxID(KMySqlDatabase* pDatabase)
{
	QCONFIRM_RET_FALSE(pDatabase);

	return InitMailID(pDatabase, KD_MAIL_TABLE_MAILBOX, KD_MAIL_FIELD_MAILID, m_nNextMailBoxID);
}

BOOL KMailDatabase::InitSysMailID(KMySqlDatabase* pDatabase)
{
	QCONFIRM_RET_FALSE(pDatabase);

	return InitMailID(pDatabase, KD_MAIL_TABLE_SYSMAIL, KD_MAIL_FIELD_MAILID, m_nNextSysMailID);
}

BOOL KMailDatabase::DBThreadCallback(INT nParam, LPVOID pParam)
{
	switch (nParam)
	{
	case emKCMD_GETMAIL:
		Push_FetchMail(nParam, pParam);
		break;
	case emKCMD_ADDMAIL:
		Push_CreateMail(nParam, pParam);
		break;
	case emKCMD_DELMAIL:
		Push_DeleteMail(nParam, pParam);
		break;
	case emKCMD_DELOWNER:
		Push_DeleteOwner(nParam, pParam);
		break;
	case emKCMD_MODMAIL:
		Push_ModifyMail(nParam, pParam);
		break;
	case emKCMD_ADDSYSMAIL:
		Push_AddSysMail(nParam, pParam);
		break;
	default:
		ASSERT(FALSE);
		QLogPrintf(LOG_ERR, "[Mail] %s", "MailDataBase Invalid DBThreadCallback!");
		return FALSE;
	}
	return TRUE;
}

BOOL KMailDatabase::MainThreadCallback(INT nParam, LPVOID pParam)
{
	switch (nParam)
	{
	case emKCMD_GETMAIL:
		Pop_FetchMail(nParam, pParam);
		break;
	case emKCMD_ADDMAIL:
		Pop_CreateMail(nParam, pParam);
		break;
	case emKCMD_DELMAIL:
		Pop_DeleteMail(nParam, pParam);
		break;
	case emKCMD_DELOWNER:
		Pop_DeleteOwner(nParam, pParam);
		break;
	case emKCMD_MODMAIL:
		Pop_ModifyMail(nParam, pParam);
		break;
	case emKCMD_ADDSYSMAIL:
		Pop_AddSysMail(nParam, pParam);
		break;
	default:
		ASSERT(FALSE);
		QLogPrintf(LOG_ERR, "[Mail] %s", "MailDataBase Invalid MainThreadCallback!");
		return FALSE;
	}
	return TRUE;
}

BOOL KMailDatabase::FetchMail(DWORD dwOwnerId, IKMailCB_FetchMail* pCallBack, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(m_pIKGcDataBase && pCallBack);
	
	KKEY_QUERYMAIL* pKey = m_poolQueryKey.New();
	pKey->dwOwnerId = dwOwnerId;
	pKey->pCallBack = pCallBack;
	pKey->pParam = pParam;
	pKey->bResult = FALSE;
	m_pIKGcDataBase->PushDBAffair(this, emKCMD_GETMAIL, pKey);
	return TRUE;
}

BOOL KMailDatabase::Push_FetchMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);
	
	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_QUERYMAIL* pKey = (KKEY_QUERYMAIL*)pParam;
	INT nLen = sprintf(m_szSQL,
		"select %s,%s,%s,%s,%s,%s,%s,%s,%s from %s where %s=%d",
		KD_MAIL_FIELD_MAILID,
		KD_MAIL_FIELD_SENDER,
		KD_MAIL_FIELD_RECEIVER,
		KD_MAIL_FIELD_SENDER_NAME,
		KD_MAIL_FIELD_TYPE,
		KD_MAIL_FIELD_STATUS,
		KD_MAIL_FIELD_SENDTIME,
		KD_MAIL_FIELD_CONDITION,
		KD_MAIL_FIELD_MAIL,
		KD_MAIL_TABLE_MAILBOX,
		KD_MAIL_FIELD_OWNER,
		pKey->dwOwnerId);
	if (rcDataBase.DoQuery(m_szSQL, (DWORD)nLen))
	{
		pKey->sResult = rcDataBase.GetQueryResult();
		pKey->bResult = TRUE;
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailDatabase::Pop_FetchMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KKEY_QUERYMAIL* pKey = (KKEY_QUERYMAIL*)pParam;
	if (!pKey->bResult || !pKey->sResult.IsValid() || pKey->sResult.GetFieldCount() != 9)
	{
		if (pKey->sResult.IsValid())
			pKey->sResult.Release();

		// 执行SQL语句失败，释放资源
		m_poolQueryKey.Free(pKey);
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase FetchMail exc sql failed!");
		return FALSE;
	}

	INT nRowCount = pKey->sResult.GetRowCount();
	for (INT i = 0; i < nRowCount; ++i)
	{
		KMySqlRow sRow = pKey->sResult.GetNextRow();
		QCONFIRM_BREAK(sRow.IsValid());
		KMAIL_INFO sInf;
		INT nIndex = 0;
		sscanf(sRow.aFieldData[nIndex++], "%lld", &(sInf.nMailID));
		sInf.dwSenderId = atoi(sRow.aFieldData[nIndex++]);
		sInf.dwReceiverId = atoi(sRow.aFieldData[nIndex++]);
		QStrCpyLen(sInf.szSender, sRow.aFieldData[nIndex], sizeof(sInf.szSender));
		nIndex++;
		sInf.nType = atoi(sRow.aFieldData[nIndex++]);
		sInf.nStat = atoi(sRow.aFieldData[nIndex++]);
		INT nDateTimeLen = sRow.aFieldLenth[nIndex];
		QCONFIRM_CONTINUE(nDateTimeLen == 19);
		DecodeDateTime(&sInf.sSendTime, sRow.aFieldData[nIndex]);
		nIndex++;
		memcpy(sInf.szCond, sRow.aFieldData[nIndex], sRow.aFieldLenth[nIndex]);
		sInf.nCondLen = sRow.aFieldLenth[nIndex];
		nIndex++;
		sInf.pMailData = (CHAR*)sRow.aFieldData[nIndex];
		sInf.nMailSize = sRow.aFieldLenth[nIndex];
		pKey->pCallBack->FetchMail(pKey->dwOwnerId, &sInf);
	}

	pKey->pCallBack->Finish(pKey->dwOwnerId, pKey->pParam);
	pKey->sResult.Release();
	m_poolQueryKey.Free(pKey);
	return TRUE;
}

BOOL KMailDatabase::AddCondMail(KCONDMAIL_INFO* pMail)
{
	QCONFIRM_RET_FALSE(pMail && pMail->pCond && pMail->pMail);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	if (pMail->nCondLen < 0 ||
		pMail->nCondLen > KD_MAIL_MAX_COND_LENGTH ||
		pMail->nMailLen <= 0 ||
		pMail->nMailLen > KD_MAIL_MAX_SIZE)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailDatabase::AddCondMail MailInfo Error!");
		return FALSE;
	}

	time_t t = KSysService::Time(NULL);
	tm* pTime = ::localtime(&t);
	pMail->sSendTime.nYear	 = pTime->tm_year + 1900;
	pMail->sSendTime.nMonth	 = pTime->tm_mon + 1;
	pMail->sSendTime.nDay	 = pTime->tm_mday;
	pMail->sSendTime.nHour	 = pTime->tm_hour;
	pMail->sSendTime.nMinute = pTime->tm_min;
	pMail->sSendTime.nSecond = pTime->tm_sec;
	pMail->nMailID = m_nNextSysMailID++;

	KKEY_SYSMAIL* pKey = m_poolSysMailKey.New();
	pKey->nMailID	= pMail->nMailID;
	pKey->sSendTime = pMail->sSendTime;
	pKey->nCondLen  = pMail->nCondLen;
	pKey->nMailLen	= pMail->nMailLen;
	memcpy(pKey->szCond, pMail->pCond, pMail->nCondLen);
	memcpy(pKey->szMail, pMail->pMail, pMail->nMailLen);

	return m_pIKGcDataBase->PushDBAffair(this, emKCMD_ADDSYSMAIL, pKey);
}

BOOL KMailDatabase::Push_AddSysMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_SYSMAIL* pKey = (KKEY_SYSMAIL*)pParam;
	CHAR szDateTime[128];
	sprintf(szDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
		pKey->sSendTime.nYear,
		pKey->sSendTime.nMonth,
		pKey->sSendTime.nDay,
		pKey->sSendTime.nHour,
		pKey->sSendTime.nMinute,
		pKey->sSendTime.nSecond);
	INT nLen = sprintf(m_szSQL, "insert into %s(%s,%s,%s,%s) values(%lld,'%s','",
		KD_MAIL_TABLE_SYSMAIL,
		KD_MAIL_FIELD_MAILID,
		KD_MAIL_FIELD_SENDTIME,
		KD_MAIL_FIELD_CONDITION,
		KD_MAIL_FIELD_MAIL,
		pKey->nMailID,
		szDateTime);
	if (pKey->szCond && pKey->nCondLen > 0)
		nLen += rcDataBase.EncodeString(&m_szSQL[nLen], pKey->szCond, pKey->nCondLen);
	strcpy(&m_szSQL[nLen], "','");
	nLen += 3;
	if (pKey->szMail && pKey->nMailLen > 0)
		nLen += rcDataBase.EncodeString(&m_szSQL[nLen], pKey->szMail, pKey->nMailLen);
	strcpy(&m_szSQL[nLen], "')");
	nLen += 2;

	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase AddSysMail exc sql failed!");
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, emKCMD_ADDSYSMAIL, pKey);
	return TRUE;
}

BOOL KMailDatabase::Pop_AddSysMail(INT nParam, LPVOID pParam)
{
	KKEY_SYSMAIL* pKey = (KKEY_SYSMAIL*)pParam;
	m_poolSysMailKey.Free(pKey);
	return TRUE;
}

// 生成邮件
BOOL KMailDatabase::AddMail(DWORD dwOwnerId, KMAIL_INFO* pMail)
{
	QCONFIRM_RET_FALSE(pMail && pMail->pMailData);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	// 发件人和收件人不能相同
	BOOL bFalse = pMail->nMailSize <= 0 ||
		pMail->nMailSize > KD_MAIL_MAX_SIZE ||
		pMail->dwReceiverId == pMail->dwSenderId ||
		((pMail->nType < 0 || pMail->nType >= emKMAIL_TYPE_COUNT) && pMail->szSender[0] == '\0');
	QCONFIRM_RET_FALSE(!bFalse);
	
	time_t t = KSysService::Time(NULL);
	tm* pTime = ::localtime(&t);
	pMail->sSendTime.nYear	 = pTime->tm_year + 1900;
	pMail->sSendTime.nMonth	 = pTime->tm_mon + 1;
	pMail->sSendTime.nDay	 = pTime->tm_mday;
	pMail->sSendTime.nHour	 = pTime->tm_hour;
	pMail->sSendTime.nMinute = pTime->tm_min;
	pMail->sSendTime.nSecond = pTime->tm_sec;

	KKEY_INSERTMAIL* pKey = m_poolInsertKey.New();
	pKey->dwOwnerId = dwOwnerId;
	pMail->nMailID = m_nNextMailBoxID++;
	pKey->sMail = *pMail;
	memcpy(pKey->szData, pMail->pMailData, pMail->nMailSize);
	pKey->sMail.pMailData = pKey->szData;
	return m_pIKGcDataBase->PushDBAffair(this, emKCMD_ADDMAIL, pKey);
}

BOOL KMailDatabase::Push_CreateMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_INSERTMAIL* pKey = (KKEY_INSERTMAIL*)pParam;
	KMAIL_INFO* pMail = &pKey->sMail;
	CHAR szSenderName[128];
	rcDataBase.EncodeString(szSenderName, pMail->szSender, (INT)(QStrLen(pMail->szSender) + 1));
	CHAR szDateTime[128];
	sprintf(szDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
		pMail->sSendTime.nYear,
		pMail->sSendTime.nMonth,
		pMail->sSendTime.nDay,
		pMail->sSendTime.nHour,
		pMail->sSendTime.nMinute,
		pMail->sSendTime.nSecond);
	CHAR szCond[KD_MAIL_MAX_COND_LENGTH];
	memset(szCond, 0, sizeof(szCond));
	if (pMail->nCondLen > 0 && pMail->szCond)
		rcDataBase.EncodeString(szCond, pMail->szCond, pMail->nCondLen);

	INT nLen = sprintf(m_szSQL,
		"insert into %s(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) values(%lld,%d,%d,%d,'%s',%d,%d,'%s','%s','",
		KD_MAIL_TABLE_MAILBOX,
		KD_MAIL_FIELD_MAILID,
		KD_MAIL_FIELD_OWNER,
		KD_MAIL_FIELD_SENDER,
		KD_MAIL_FIELD_RECEIVER,
		KD_MAIL_FIELD_SENDER_NAME,
		KD_MAIL_FIELD_TYPE,
		KD_MAIL_FIELD_STATUS,
		KD_MAIL_FIELD_SENDTIME,
		KD_MAIL_FIELD_CONDITION,
		KD_MAIL_FIELD_MAIL,
		pMail->nMailID,
		pKey->dwOwnerId,
		pMail->dwSenderId,
		pMail->dwReceiverId,
		szSenderName,
		pMail->nType,
		pMail->nStat,
		szDateTime,
		szCond);
	if (pMail->pMailData && pMail->nMailSize > 0)
		nLen += rcDataBase.EncodeString(&m_szSQL[nLen], pMail->pMailData, pMail->nMailSize);
	strcpy(&m_szSQL[nLen], "')");
	nLen += 2;
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase CreateMail exc sql failed!");
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, emKCMD_ADDMAIL, pKey);
	return TRUE;
}

BOOL KMailDatabase::Pop_CreateMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_INSERTMAIL* pKey = (KKEY_INSERTMAIL*)pParam;
	m_poolInsertKey.Free(pKey);
	return TRUE;
}

// 查询普通玩家邮件数量
BOOL KMailDatabase::QueryMailCount(DWORD dwOwnerId, IKMailCB_MailCount* pCallBack, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);
	QCONFIRM_RET_FALSE(pCallBack);

	// 普通玩家数量记录在KMailPlayerManager中，不在查询数据库，减少数据库压力
	INT nPlayerMailCount = KMailPlayerManager::Instance()->GetPlayerMailCount(dwOwnerId);
	pCallBack->NotifyMailCount(dwOwnerId, nPlayerMailCount, pParam);

	return TRUE;
}

// 删除邮件
BOOL KMailDatabase::DeleteMail(INT64 nMailID)
{
	QCONFIRM_RET_FALSE(nMailID > 0);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KKEY_MAILID* pKey = m_poolMailIDKey.New();
	pKey->nMailID = nMailID;
	m_pIKGcDataBase->PushDBAffair(this, emKCMD_DELMAIL, pKey);
	return TRUE;
}

BOOL KMailDatabase::Push_DeleteMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_MAILID* pKey = (KKEY_MAILID*)pParam;
	INT nLen = sprintf(m_szSQL,
		"delete from %s where %s=%lld",
		KD_MAIL_TABLE_MAILBOX,
		KD_MAIL_FIELD_MAILID,
		pKey->nMailID);
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase DeleteMail exc sql failed!");
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailDatabase::Pop_DeleteMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_MAILID* pKey = (KKEY_MAILID*)pParam;
	m_poolMailIDKey.Free(pKey);
	return TRUE;
}

// 删除和某个玩家有关的所有邮件（删号时使用）
BOOL KMailDatabase::DeleteOwner(DWORD dwOwnerId)
{
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KKEY_GUID* pKey = m_poolGuidKey.New();
	pKey->dwOwnerId = dwOwnerId;
	m_pIKGcDataBase->PushDBAffair(this, emKCMD_DELOWNER, pKey);
	return TRUE;
}

BOOL KMailDatabase::Push_DeleteOwner(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KKEY_GUID* pKey = (KKEY_GUID*)pParam;
	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	INT nLen = sprintf(m_szSQL,
		"delete from %s where %s=%d or %s=%d or %s=%d",
		KD_MAIL_TABLE_MAILBOX,
		KD_MAIL_FIELD_OWNER,
		pKey->dwOwnerId,
		KD_MAIL_FIELD_SENDER,
		pKey->dwOwnerId,
		KD_MAIL_FIELD_RECEIVER,
		pKey->dwOwnerId);
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBse DeleteOwner exc sql failed!");
	}

	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailDatabase::Pop_DeleteOwner(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_GUID* pKey = (KKEY_GUID*)pParam;
	m_poolGuidKey.Free(pKey);
	return TRUE;
}

// 清空玩家邮箱中的邮件（暂时只留给测试用）
BOOL KMailDatabase::ClearOwner(DWORD dwOwnerId)
{
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KKEY_GUID* pKey = m_poolGuidKey.New();
	pKey->dwOwnerId = dwOwnerId;
	m_pIKGcDataBase->PushDBAffair(this, emKCMD_CLEAROWNER, pKey);
	return TRUE;
}

BOOL KMailDatabase::Push_ClearOwner(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KKEY_GUID* pKey = (KKEY_GUID*)pParam;
	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	INT nLen = sprintf(m_szSQL,
		"delete from %s where %s=%d",
		KD_MAIL_TABLE_MAILBOX,
		KD_MAIL_FIELD_OWNER,
		pKey->dwOwnerId);
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBse ClearOwner exc sql failed!");
	}

	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailDatabase::Pop_ClearOwner(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_GUID* pKey = (KKEY_GUID*)pParam;
	m_poolGuidKey.Free(pKey);
	return TRUE;
}

// 修改邮件，nMask见KE_MASK_MODIFYMAIL
BOOL KMailDatabase::ModifyMail(KMAIL_MODINFO* pMail, INT nMask)
{ 
	QCONFIRM_RET_FALSE(pMail && pMail->nMailID > 0);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KKEY_MODIFYMAIL* pKey = m_poolModifyKey.New();
	if ((nMask & emKMASK_MODIFYMAIL_STAT) == 0 && (nMask & emKMASK_MODIFYMAIL_MAIL) == 0)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase ModifyMail Invalid Mask!");
		return FALSE;
	}
	if (pMail->pMailData == NULL || pMail->nMailSize > KD_MAIL_MAX_SIZE)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase ModifyMail MailSize Too Large!");
		return FALSE;
	}

	pKey->nMask = nMask;
	pKey->sMail = *pMail;
	memcpy(pKey->szData, pMail->pMailData, pMail->nMailSize);
	pKey->sMail.pMailData = pKey->szData;
	m_pIKGcDataBase->PushDBAffair(this, emKCMD_MODMAIL, pKey);
	return TRUE;
}

BOOL KMailDatabase::Push_ModifyMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(m_pIKGcDataBase);

	KMySqlDatabase& rcDataBase = m_pIKGcDataBase->GetDatabase();
	KKEY_MODIFYMAIL* pKey = (KKEY_MODIFYMAIL*)pParam;
	LPSTR pszBuf = m_szSQL;
	BOOL bHasComma = FALSE;		// 是否已经有逗号
	pszBuf += sprintf(pszBuf, "update %s set ", KD_MAIL_TABLE_MAILBOX);
	if ((pKey->nMask & emKMASK_MODIFYMAIL_STAT) != 0)
	{
		pszBuf += sprintf(pszBuf, "%s=%d", KD_MAIL_FIELD_STATUS, pKey->sMail.nStat);
	}
	if ((pKey->nMask & emKMASK_MODIFYMAIL_MAIL) != 0)
	{
		if ((pKey->nMask & emKMASK_MODIFYMAIL_STAT) != 0)
		{
			*pszBuf++ = ',';
			bHasComma = TRUE;
		}

		if (pKey->sMail.pMailData && pKey->sMail.nMailSize > 0)
		{
			pszBuf += sprintf(pszBuf, "%s='", KD_MAIL_FIELD_MAIL);
			pszBuf += rcDataBase.EncodeString(pszBuf, pKey->sMail.pMailData, pKey->sMail.nMailSize);
			*pszBuf++ = '\'';
			bHasComma = FALSE;
		}	

		if (pKey->sMail.szCond && pKey->sMail.nCondLen > 0)
		{
			if (!bHasComma)
				*pszBuf++ = ',';
			
			pszBuf += sprintf(pszBuf, "%s='", KD_MAIL_FIELD_CONDITION);
			pszBuf += rcDataBase.EncodeString(pszBuf, pKey->sMail.szCond, pKey->sMail.nCondLen);
			*pszBuf++ = '\'';
		}	
	}
	pszBuf += sprintf(pszBuf, " where %s=%lld", KD_MAIL_FIELD_MAILID, pKey->sMail.nMailID);
	DWORD nLen = (DWORD)(pszBuf - &m_szSQL[0]);
	if (!rcDataBase.DoQuery(m_szSQL, nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "MailDataBase ModifyMail exc sql failed!");
	}
	m_pIKGcDataBase->PushMainThreadCallBack(this, nParam, pKey);
	return TRUE;
}

BOOL KMailDatabase::Pop_ModifyMail(INT nParam, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	KKEY_MODIFYMAIL* pKey = (KKEY_MODIFYMAIL*)pParam;
	m_poolModifyKey.Free(pKey);
	return TRUE;
}

BOOL KMailDatabase::DecodeDateTime(KMAIL_DATETIME* pDateTime, LPCSTR pszTime)
{
	QCONFIRM_RET_FALSE(pDateTime && pszTime);

	KSTR_DATETIME sDateTime;
	strcpy((CHAR*)&sDateTime, pszTime);
	sDateTime.chSep1	= 0;
	sDateTime.chSep2	= 0;
	sDateTime.chSep3	= 0;
	sDateTime.chSep4	= 0;
	sDateTime.chSep5	= 0;
	sDateTime.chEnd		= 0;
	pDateTime->nYear	= atoi(sDateTime.szYear);
	pDateTime->nMonth	= atoi(sDateTime.szMonth);
	pDateTime->nDay		= atoi(sDateTime.szDay);
	pDateTime->nHour	= atoi(sDateTime.szHour);
	pDateTime->nMinute	= atoi(sDateTime.szMinute);
	pDateTime->nSecond	= atoi(sDateTime.szSecond);
	return TRUE;
}

// -------------------------------------------------------------------------

#endif /* GAME_CENTER */
