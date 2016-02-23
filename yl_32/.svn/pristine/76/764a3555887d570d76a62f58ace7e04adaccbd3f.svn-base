/* -------------------------------------------------------------------------
//	文件名		：	kmail.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/16 16:06:37
//	功能描述	：	邮件
//
// -----------------------------------------------------------------------*/

#ifdef GAME_CENTER
#include "stdafx.h"
#include "kmail.h"
#include "kmaildatabase.h"
#include "kgc_mailmodule.h"
#include "kcondchecker.h"
#include "gclogicbase/kgcplayercommon.h"
#include "gclogicbase/kgclogicextinterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMail::KMail() : m_bDirty(FALSE)
{
	memset(&m_sMailInfo, 0, sizeof(m_sMailInfo));
	memset(m_strCond, 0, sizeof(m_strCond));
	m_strCaption.clear();
	m_strContent.clear();
	m_nCondLen = 0;
}

KMail::~KMail()
{
}

BOOL KMail::Load(CONST KMAIL_INFO *pData)
{
	static CHAR g_szCaption[KD_MAIL_MAX_CAPTION_LENGTH];
	static CHAR g_szContent[KD_MAIL_MAX_CONTENT_LENGTH];
	
	QCONFIRM_RET_FALSE(pData && pData->nMailID > 0);
	QCONFIRM_RET_FALSE(pData->pMailData && pData->nMailSize > 0);
	KMAIL_DBINFO* pInfo = (KMAIL_DBINFO*)pData->pMailData;
	if (pData->nMailSize <= sizeof(KMAIL_DBINFO) ||
		pData->nMailSize != pInfo->wSize)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMail::Load failed!Total size error!");
		return FALSE;
	}
	if (pInfo->wCaptionOffset >= pInfo->wContentOffset ||
		pInfo->wContentOffset >= pInfo->wAttachOffet ||
		pInfo->wAttachOffet > pInfo->wSize)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMail::Load failed! size error!");
		return FALSE;
	}

	// 加载邮件接收条件
	if (pData->nCondLen > 0)
	{
		if (pData->nCondLen > KD_MAIL_MAX_COND_LENGTH)
		{
			QLogPrintf(LOG_ERR, "[mail] %s", "KMail::Load failed! CondLength Too Large!");
			return FALSE;
		}

		memcpy(m_strCond, pData->szCond, pData->nCondLen);
		m_nCondLen = pData->nCondLen;
	}

	// 加载标题和正文
	INT nCaptionLen = pInfo->wContentOffset - pInfo->wCaptionOffset;
	INT nContentLen = pInfo->wAttachOffet - pInfo->wContentOffset;
	if (nCaptionLen > KD_MAIL_MAX_CAPTION_LENGTH ||
		nContentLen > KD_MAIL_MAX_CONTENT_LENGTH)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMail::Load failed! Caption or Content size too large!");
		return FALSE;
	}
	QStrCpyLen(g_szCaption, (LPSTR)(pInfo) + pInfo->wCaptionOffset, nCaptionLen);
	m_strCaption = g_szCaption;
	QStrCpyLen(g_szContent, (LPSTR)(pInfo) + pInfo->wContentOffset, nContentLen);
	m_strContent = g_szContent;
	
	// 加载附带信息
	m_sMailInfo.nType = pData->nType;
	m_sMailInfo.nIconId = pInfo->nIconId;
	m_sMailInfo.nStat = pData->nStat;
	m_sMailInfo.nMailID = pData->nMailID;
	m_sMailInfo.dwReceiverId = pData->dwReceiverId;
	m_sMailInfo.dwSenderId = pData->dwSenderId;
	QStrCpyLen(m_sMailInfo.szSender, pData->szSender, sizeof(m_sMailInfo.szSender) - 1);
	memcpy(&m_sMailInfo.sSendTime, &pData->sSendTime, sizeof(KMAIL_DATETIME));
	
	// 加载附件
	m_mapItems.clear();
	INT nAttachLen = pInfo->wSize - pInfo->wAttachOffet;
	if (nAttachLen == 0)
		return TRUE;
	if (!LoadAttachment((CHAR*)(pInfo) + pInfo->wAttachOffet, nAttachLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMail::Load failed! LoadAttachment failed!");
		return FALSE;
	}
	return TRUE;
}

LPCTSTR KMail::GetCaption(INT &nSize)
{
	nSize = (INT)m_strCaption.size() + 1;
	return m_strCaption.c_str();
}

LPCTSTR KMail::GetContent(INT &nSize)
{
	nSize = (INT)m_strContent.size() + 1;
	return m_strContent.c_str();
}

LPCTSTR KMail::GetCondition(INT &nSize)
{
	nSize = m_nCondLen;
	return m_strCond;
}

BOOL KMail::LoadAttachment(CHAR *pData, INT nLen)
{
	QCONFIRM_RET_FALSE(pData);
	KMAIL_DBINFO_ATTACHMENT *pAttach = (KMAIL_DBINFO_ATTACHMENT*)pData;
	if (sizeof(KMAIL_DBINFO_ATTACHMENT) > nLen ||
		sizeof(KMAIL_DBINFO_ATTACHMENT) + pAttach->wCount * sizeof(KItemData_Common) != nLen)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMail::LoadAttachment failed! KMAIL_DBINFO_ATTACHMENT size error!");
		return FALSE;
	}

	QCONFIRM_RET_FALSE(m_mapItems.empty());
	for (INT i = 0; i < pAttach->wCount; ++i)
	{
		KMAILITEM sItem;
		sItem.sItemData = pAttach->aryItems[i];
		sItem.bLock = FALSE;
		m_mapItems[i + 1] = sItem;
	}
	return TRUE;
}

BOOL KMail::RemoveItem(INT nIndex)
{
	QCONFIRM_RET_FALSE(nIndex > 0);
	KDB_MAP::iterator it = m_mapItems.find(nIndex);
	QCONFIRM_RET_FALSE(it != m_mapItems.end());
	m_mapItems.erase(it);
	m_bDirty = TRUE;
	return TRUE;
}

INT KMail::GetAttachBufLen()
{
	if (m_mapItems.size() == 0)
		return 0;
	return sizeof(KMAIL_DBINFO_ATTACHMENT) + sizeof(KItemData_Common) * (INT)m_mapItems.size();
}

BOOL KMail::SaveAttachment(CHAR *pData, INT nLen)
{
	QCONFIRM_RET_FALSE(pData);
	if (nLen != sizeof(KMAIL_DBINFO_ATTACHMENT) + sizeof(KItemData_Common) * m_mapItems.size())
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMail::SaveAttachment size error!");
		return FALSE;
	}
	KMAIL_DBINFO_ATTACHMENT *pHead = (KMAIL_DBINFO_ATTACHMENT*)pData;
	pHead->wCount = (WORD)m_mapItems.size();
	KItemData_Common *pItem = pHead->aryItems;
	KDB_MAP::const_iterator it = m_mapItems.begin();
	for (; it != m_mapItems.end(); it++)
	{
		*pItem++ = it->second.sItemData;
	}
	return TRUE;
}

BOOL KMail::Save(CHAR *pBuffer, INT nBufLen, INT &nRetLen)
{
	QCONFIRM_RET_FALSE(pBuffer);
	INT nDataLen = sizeof(KMAIL_DBINFO) + sizeof(KMAIL_DBINFO_ATTACHMENT);
	nDataLen += (INT)(m_strCaption.size() + 1);
	nDataLen += (INT)(m_strContent.size() + 1);
	nDataLen += (INT)(m_mapItems.size() * sizeof(KItemData_Common));
	if (nDataLen > nBufLen || nDataLen > KD_MAIL_MAX_SIZE)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMail::Save failed! size too large!");
		return FALSE;
	}
	KMAIL_DBINFO *pDBInfo = (KMAIL_DBINFO*)pBuffer;
	// 标题
	CHAR *pCaption = &pBuffer[sizeof(KMAIL_DBINFO)];
	QStrCpyLen(pCaption, m_strCaption.c_str(), (INT)(m_strCaption.size() + 1));
	// 正文
	CHAR *pContent = &pCaption[m_strCaption.size() + 1];
	QStrCpyLen(pContent, m_strContent.c_str(), (INT)(m_strContent.size() + 1));
	// 附件
	KMAIL_DBINFO_ATTACHMENT *pAttach = (KMAIL_DBINFO_ATTACHMENT*)(&pContent[m_strContent.size() + 1]);
	pAttach->wCount = (WORD)m_mapItems.size();
	KDB_MAP::const_iterator it = m_mapItems.begin();
	KItemData_Common *pItem = pAttach->aryItems;
	for (; it != m_mapItems.end(); it++)
	{
		*pItem++ = it->second.sItemData;
	}
	KMAIL_DBINFO *pHead = (KMAIL_DBINFO*)pBuffer;
	pHead->wSize = nDataLen;
	pHead->wVersion = 1;
	pHead->nIconId = m_sMailInfo.nIconId;
	pHead->wCaptionOffset = (WORD)(pCaption - pBuffer);
	pHead->wContentOffset = (WORD)(pContent - pBuffer);
	pHead->wAttachOffet = (WORD)((CHAR*)pAttach - pBuffer);
	nRetLen = nDataLen;
	return TRUE;
}

CONST KItemData_Common* KMail::GetAttachItem(INT nIndex)
{
	QCONFIRM_RET_NULL(nIndex >= 0);
	KDB_MAP::const_iterator it = m_mapItems.find(nIndex);
	if (it == m_mapItems.end())
		return NULL;
	return &it->second.sItemData;
}

BOOL KMail::DeleteAttachItem(INT nIndex)
{
	QCONFIRM_RET_FALSE(nIndex >= 0);
	KDB_MAP::iterator it = m_mapItems.find(nIndex);
	QCONFIRM_RET_FALSE(it != m_mapItems.end());
	m_mapItems.erase(it);
	m_bDirty = TRUE;
	return TRUE;
}

time_t KMail::MkTime(CONST KMAIL_DATETIME* pTime)
{
	QCONFIRM_RET(pTime, 0);
	tm t;
	memset(&t, 0, sizeof(t));
	t.tm_year = pTime->nYear - 1900;
	t.tm_mon  = pTime->nMonth - 1;
	t.tm_mday = pTime->nDay;
	t.tm_hour = pTime->nHour;
	t.tm_min  = pTime->nMinute;
	t.tm_sec  = pTime->nSecond;
	return mktime(&t);
}

VOID KMail::LockItem(INT nIndex)
{
	ASSERT(nIndex >= 0);
	KDB_MAP::iterator it = m_mapItems.find(nIndex);
	if (it != m_mapItems.end())
	{
		ASSERT(!it->second.bLock);
		it->second.bLock = TRUE;
	}
	else
	{
		ASSERT(FALSE);
	}
}

BOOL KMail::IsItemLocked(INT nIndex)
{
	ASSERT(nIndex >= 0);
	// 如果找不到此附件，也认为附件物品锁定
	BOOL bResult = TRUE;
	KDB_MAP::const_iterator it = m_mapItems.find(nIndex);
	if (it != m_mapItems.end())
	{
		bResult = it->second.bLock;
	}
	return bResult;
}

VOID KMail::Clear()
{
	memset(&m_sMailInfo, 0, sizeof(m_sMailInfo));		// 邮件信息
	memset(m_strCond, 0, sizeof(m_strCond));
	m_strCaption.clear();
	m_strContent.clear();
	m_mapItems.clear();		// 附带物品
	m_bDirty = FALSE;		// 邮件是否修改过
	m_nCondLen = 0;
}

BOOL KMail::CanPost()
{
	if (m_nCondLen == 0)
		return TRUE;

	IKOnlinedPlayerMgr* pPlayerMgr = KGC_MailModule::Instance()->GetOnlinePlayerMgr();
	QCONFIRM_RET_FALSE(pPlayerMgr);
	KGcPlayerCommon* pPlayer = pPlayerMgr->GetPlayerById(m_sMailInfo.dwReceiverId);
	QCONFIRM_RET_FALSE(pPlayer);
	
	return KCondChecker::Instance()->CheckCondition(pPlayer, (KMAIL_CONDHEAD*)m_strCond, m_nCondLen);
}

// -------------------------------------------------------------------------

#endif /* GAME_CENTER */
