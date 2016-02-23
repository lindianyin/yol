/* -------------------------------------------------------------------------
//	文件名		：	kcondmailmanager.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/17 14:23:29
//	功能描述	：	系统邮件管理器，用于管理要发送给指定条件玩家的系统邮件
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kcondmailmanager.h"
#include "kmailboxmanager.h"
#include "kgc_mailmodule.h"
#include "kmailplayermanager.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "gcmodule/mail/kmail_datadef.h"
#include "kcondchecker.h"
#include "gclogicbase/kgclogicextinterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KCondMailManager KCondMailManager::ms_inst;

KCondMailManager::KCondMailManager()
{
}

KCondMailManager::~KCondMailManager()
{
}

BOOL KCondMailManager::FetchSysMail(CONST KCONDMAIL_INFO* pSysMail)
{
	return AddMail(pSysMail);
}

BOOL KCondMailManager::AddMail(CONST KCONDMAIL_INFO* pSysMail)
{
	QCONFIRM_RET_FALSE(pSysMail && pSysMail->nMailID > 0);
	QCONFIRM_RET_FALSE((pSysMail->nCondLen > 0 && pSysMail->pCond) || pSysMail->nCondLen == 0);
	QCONFIRM_RET_FALSE(pSysMail->pMail && pSysMail->nMailLen > 0);
	QCONFIRM_RET_FALSE(pSysMail->nCondLen <= KD_MAIL_MAX_COND_LENGTH && pSysMail->nMailLen <= KD_MAIL_MAX_SIZE);

	KSYSMAIL sMail;
	memset(&sMail, 0, sizeof(sMail));
	sMail.nCondLen	= pSysMail->nCondLen;
	sMail.nMailLen	= pSysMail->nMailLen;
	sMail.sSendTime = pSysMail->sSendTime;
	memcpy(sMail.szCond, pSysMail->pCond, pSysMail->nCondLen);
	memcpy(sMail.szMail, pSysMail->pMail, pSysMail->nMailLen);
	std::pair<std::map<INT64, KSYSMAIL>::iterator, BOOL> sRes = m_mapMail.insert(
		std::make_pair(pSysMail->nMailID, sMail));
	if (!sRes.second)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KCondMailManager AddMail Failed!");
		return FALSE;
	}
	return TRUE;
}

// 获取新邮件起始位置
BOOL KCondMailManager::GetNewMailPosition(INT64 nLastID, std::map<INT64, KSYSMAIL>* pMap, std::map<INT64, KSYSMAIL>::const_iterator &it)
{
	QCONFIRM_RET_FALSE(nLastID >= 0 && pMap);
	INT64 nEndMailID = pMap->rbegin()->first;
	INT64 nBeginMailID = pMap->begin()->first;
	QCONFIRM_RET_FALSE(nLastID <= nEndMailID);

	// 已经接受过系统邮件
	if (nLastID == nEndMailID)
		return FALSE;
	// 定位未接收的邮件
	if (nLastID < nBeginMailID)
	{
		it = pMap->begin();
		return TRUE;
	}
	std::map<INT64, KSYSMAIL>::const_iterator itFind = pMap->find(nLastID);
	QCONFIRM_RET_FALSE(itFind != pMap->end());
	itFind++;
	if (itFind == pMap->end())
		return FALSE;

	it = itFind;
	return TRUE;
}

// 投递系统邮件（如果玩家符合条件）
BOOL KCondMailManager::PostMail(DWORD dwPlayerId)
{
	IKOnlinedPlayerMgr* pPlayerMgr = KGC_MailModule::Instance()->GetOnlinePlayerMgr();
	QCONFIRM_RET_NULL(pPlayerMgr);
	KRole* pPlayer = pPlayerMgr->GetPlayerById(dwPlayerId);
	QCONFIRM_RET_NULL(pPlayer);

	if (m_mapMail.empty())
		return FALSE;

	std::map<INT64, KSYSMAIL>::const_iterator it;
	if (!GetNewMailPosition(KMailPlayerManager::Instance()->GetSysMailId(dwPlayerId), &m_mapMail, it))
		return FALSE;

	// 投递系统邮件
	for (; it != m_mapMail.end(); ++it)
	{
		INT64 nMailID = it->first;
		CONST KSYSMAIL* pMail = &it->second;
		if (!CheckCondition(pPlayer, (KMAIL_CONDHEAD*)pMail->szCond, pMail->nCondLen))
			continue;
		PostMail(dwPlayerId, pMail);
	}

	INT64 nEndMailID = m_mapMail.rbegin()->first;
	KMailPlayerManager::Instance()->SetSysMailId(dwPlayerId, nEndMailID);
	return TRUE;
}


BOOL KCondMailManager::NotifyMail(DWORD dwPlayerId)
{
	BOOL bRet = TRUE;
	KMAIL_G2S_PROTO_ADDMAIL sNotify;

	IKOnlinedPlayerMgr* pPlayerMgr = KGC_MailModule::Instance()->GetOnlinePlayerMgr();
	QCONFIRM_RET_FALSE(pPlayerMgr);
	KRole* pPlayer = pPlayerMgr->GetPlayerById(dwPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);
	IKGcServer* pGcServer = KGC_MailModule::Instance()->GetIKGcServer();
	QCONFIRM_RET_FALSE(pGcServer);

	std::map<INT64, KSYSMAIL>::const_iterator it;
	if (GetNewMailPosition(KMailPlayerManager::Instance()->GetSysMailId(dwPlayerId), &m_mapMail, it))
	{
		for (; it != m_mapMail.end(); it++)
		{
			INT64 nMailID = it->first;
			CONST KSYSMAIL* pMail = &it->second;
			if (!CheckCondition(pPlayer, (KMAIL_CONDHEAD*)pMail->szCond, pMail->nCondLen))
				continue;

			//通知在线玩家有新邮件
			INT nServerID = pPlayer->GetOnlineServer();
			if (nServerID > 0)
			{
				sNotify.dwPlayerId = dwPlayerId;
				sNotify.nMailID = 0;
				pGcServer->Send(nServerID, &sNotify, sizeof(sNotify));
				break;
			}
		}
	}
	else
	{
		bRet = FALSE;
	}
	return bRet;
}

BOOL KCondMailManager::CheckCondition(KRole* pPlayer, CONST KMAIL_CONDHEAD* pCond, INT nCondLen)
{
	QCONFIRM_RET_FALSE(pPlayer);
	QCONFIRM_RET_FALSE((nCondLen > 0 && pCond) || nCondLen == 0);

	if (nCondLen == 0)
		return TRUE;
	else
		return KCondChecker::Instance()->CheckCondition(pPlayer, pCond, nCondLen);
}

INT64 KCondMailManager::PostMail(DWORD dwPlayerId, CONST KSYSMAIL* pMail)
{
	QCONFIRM_RET_NULL(pMail);
	KMailBox* pMailBox = KMailBoxManager::Instance()->FindMailBox(dwPlayerId);
	QCONFIRM_RET_NULL(pMailBox);

	KMAIL_INFO sMail;
	sMail.nMailID	= 0;
	sMail.nMailSize = pMail->nMailLen;
	sMail.nStat		= emKMAIL_STAT_UNREAD;
	sMail.nType		= emKMAIL_TYPE_SYSTEM;
	sMail.pMailData = pMail->szMail;
	sMail.sSendTime = pMail->sSendTime;
	sMail.dwReceiverId = dwPlayerId;
	sMail.dwSenderId = 0;
	sMail.szSender[0] = '\0';
    sMail.nCondLen = pMail->nCondLen;
	memcpy(sMail.szCond, pMail->szCond, sMail.nCondLen);
	QCONFIRM_RET_FALSE(pMailBox->CreateMail(&sMail));
	QCONFIRM_RET_FALSE(sMail.nMailID > 0);
	return sMail.nMailID;
}

// -------------------------------------------------------------------------

