/* -------------------------------------------------------------------------
//	文件名		：	kmailbox.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/16 17:56:37
//	功能描述	：	邮箱
//
// -----------------------------------------------------------------------*/

#ifdef GAME_CENTER
#include "stdafx.h"
#include "kmailbox.h"
#include "kmailboxmanager.h"
#include "kmaildatabase.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailBox::KMailBox() : m_bLoaded(FALSE), m_dwOwnerId(0)
{
}

KMailBox::~KMailBox()
{
}

VOID KMailBox::Init(DWORD dwOwnerId, INT nTime)
{
	ASSERT(m_mapMail.empty());
	m_dwOwnerId = dwOwnerId;
	m_nInitTime = nTime;
	m_mapMail.clear();
	m_bLoaded = FALSE;
}

VOID KMailBox::UnInit()
{
	KMailBoxManager* pManager = KMailBoxManager::Instance();
	std::map<INT64, KMail*>::iterator it = m_mapMail.begin();
	for (; it != m_mapMail.end(); it++)
	{
		KMail *pMail = it->second;
		ASSERT(pMail);
		pManager->FreeMail(pMail);
	}
	m_mapMail.clear();
	m_dwOwnerId = 0;
	m_nInitTime = 0;
	m_bLoaded = FALSE;
}

BOOL KMailBox::LoadMail(CONST KMAIL_INFO *pData, KMail **ppMail)
{
	QCONFIRM_RET_FALSE(pData && pData->nMailID > 0);
	KMail* pMail = KMailBoxManager::Instance()->AllocateMail();
	if (!pMail)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBox::LoadMail AllocateMail failed!");
		return FALSE;
	}

	std::pair<std::map<INT64, KMail*>::iterator, bool> sRes = m_mapMail.insert(std::make_pair(pData->nMailID, pMail));
	if (!sRes.second)
	{
		KMailBoxManager::Instance()->FreeMail(pMail);

		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBox::LoadMail insert map failed!");
		return FALSE;
	}
	
	if (!pMail->Load(pData))
	{
		m_mapMail.erase(sRes.first);
		KMailBoxManager::Instance()->FreeMail(pMail);
		
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBox::LoadMail LoadData failed!");
		return FALSE;
	}
	if (ppMail)
	{
		*ppMail = pMail;
	}
	return TRUE;
}

BOOL KMailBox::CreateMail(KMAIL_INFO *pData, KMail **ppMail)
{
	QCONFIRM_RET_FALSE(pData && pData->nMailID == 0);
	
	// 先把邮件保存到数据库
	KMailDatabase::Instance()->AddMail(m_dwOwnerId, pData);
	return LoadMail(pData, ppMail);
}

KMail *KMailBox::FindMail(INT64 nMailID)
{
	QCONFIRM_RET_NULL(nMailID > 0);
	std::map<INT64, KMail*>::iterator it = m_mapMail.find(nMailID);
	if (it == m_mapMail.end())
		return NULL;
	return it->second;
}

BOOL KMailBox::DeleteMail(INT64 nMailID)
{
	QCONFIRM_RET_FALSE(nMailID > 0);
	std::map<INT64, KMail*>::iterator it = m_mapMail.find(nMailID);
	if (it == m_mapMail.end())
		return FALSE;
	KMail* pMail = it->second;
	KMailBoxManager::Instance()->FreeMail(pMail);
	m_mapMail.erase(it);
	// 从数据库删除
	KMailDatabase::Instance()->DeleteMail(nMailID);
	return TRUE;
}

BOOL KMailBox::GetAllMail(IKEnumMail **ppEnum)
{
	QCONFIRM_RET_FALSE(ppEnum);
	if (m_mapMail.size() == 0)
		return FALSE;
	m_enumMail.Init(&m_mapMail);
	*ppEnum = &m_enumMail;
	return TRUE;
}

BOOL KMailBox::Save(KMail *pMail)
{
	QCONFIRM_RET_FALSE(pMail);
	INT nLen = 0;
	CHAR szBuf[KD_MAIL_MAX_SIZE];
	if (!pMail->Save(szBuf, sizeof(szBuf), nLen))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBox::Save failed!");
		return FALSE;
	}

	QCONFIRM_RET_FALSE(nLen > 0);
	CONST KMail::KMAILHEAD *pHead = pMail->GetInfo();
	KMAIL_MODINFO sInfo;
	memset(&sInfo, 0, sizeof(KMAIL_MODINFO));
	sInfo.nMailID = pHead->nMailID;
	sInfo.nStat = pHead->nStat;
	sInfo.pMailData = szBuf;
	sInfo.nMailSize = nLen;
	LPCSTR pszCond = pMail->GetCondition(sInfo.nCondLen);
	memcpy(sInfo.szCond, pszCond, sInfo.nCondLen);
	KMailDatabase::Instance()->ModifyMail(&sInfo);
	pMail->CleanDirtiness();
	return TRUE;
}

BOOL KMailBox::Save(INT64 nMailID)
{
	QCONFIRM_RET_FALSE(nMailID > 0);
	std::map<INT64, KMail*>::iterator it = m_mapMail.find(nMailID);
	QCONFIRM_RET_FALSE(it != m_mapMail.end());
	Save(it->second);
	return TRUE;
}

VOID KMailBox::Save()
{
	std::map<INT64, KMail*>::iterator it = m_mapMail.begin();
	for (; it != m_mapMail.end(); it++)
	{
		KMail *pMail = it->second;
		ASSERT(pMail);
		if (pMail != NULL && pMail->IsDirty())
		{
			Save(pMail);
		}
	}
}

BOOL KMailBox::Clear() 
{
	m_mapMail.clear();
	m_bLoaded = FALSE;
	m_dwOwnerId = 0;
	return TRUE;
}

INT KMailBox::GetInitTime()
{
	return m_nInitTime;
}

// -------------------------------------------------------------------------

#endif /* GAME_CENTER */
