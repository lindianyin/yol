/* -------------------------------------------------------------------------
//	文件名		：	kmailcenter.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/17 18:01:43
//	功能描述	：	邮件中心
//
// -----------------------------------------------------------------------*/

#ifdef GAME_CENTER
#include "stdafx.h"
#include "kmailcenter.h"
#include "kmaildatabase.h"
#include "kcondmailmanager.h"
#include "kmailplayermanager.h"
#include "kmailboxmanager.h"
#include "kgc_mailprotocalhandler.h"
#include "kgc_mailmodule.h"
#include "onlinegamebase/ksysservice.h"

#define KDF_MAIL_CONFIG	"gamecenter_cfg.ini"			// 邮件配置文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailCenter KMailCenter::ms_inst;

KMailCenter::KMailCenter() : m_bStartSearch(TRUE),m_dwLastPlayerId(0),m_nSearchMailBoxCount(0),m_nSearchTimeSpace(0)
{
	m_nSearchTime = (INT)KSysService::Time(NULL);
}

KMailCenter::~KMailCenter()
{
}

BOOL KMailCenter::Init()
{
	BOOL bLoadConfig = FALSE;
	IIniFile* pIniFile = NULL;
	
	INT nMailBoxInitCount = 0;		// 初始分配的邮箱数目
	INT nMailBoxBatchCount = 0;		// 增量分配的邮箱数目
	INT nMailInitCount = 0;			// 初始分配的邮件数目
	INT nMailBatchCount = 0;		// 增量分配的邮件数目
	INT nMaxOnlineMailBoxCount = 0;	// 内存中最多存储的邮箱数

	INT nMailPlayerInitCount = 0;	// 初始分配的玩家数目（全区玩家数）
	INT nMailPlayerBatchCount = 0;	// 增量分配的玩家数目

	INT nMailExpireTime = 0;		// 邮件过期时间，单位（天）

	pIniFile = g_OpenIniFile(KDF_MAIL_CONFIG);
	if (!pIniFile)
	{
		QLogPrintf(LOG_ERR, "[mail] Read mailconfig %s Failed!", KDF_MAIL_CONFIG);
		return FALSE;
	}

	pIniFile->GetInteger("Mail", "MailBoxInitCount", 10000, &nMailBoxInitCount);
	pIniFile->GetInteger("Mail", "MailBoxBatchCount", 10000, &nMailBoxBatchCount);
	pIniFile->GetInteger("Mail", "MailInitCount", 200000, &nMailInitCount);
	pIniFile->GetInteger("Mail", "MailBatchCount", 10000, &nMailBatchCount);
	pIniFile->GetInteger("Mail", "MaxOnlineMailBoxCount", 10000, &nMaxOnlineMailBoxCount);

	pIniFile->GetInteger("Mail", "MailPlayerInitCount", 200000, &nMailPlayerInitCount);
	pIniFile->GetInteger("Mail", "MailPlayerBatchCount", 10000, &nMailPlayerBatchCount);

	pIniFile->GetInteger("Mail", "MailExpireTime", 7, &nMailExpireTime);
	
	pIniFile->GetInteger("Mail", "SearchMailBoxCount", 100, &m_nSearchMailBoxCount);
	pIniFile->GetInteger("Mail", "SearchTimeSpace", 3600, &m_nSearchTimeSpace);
	
	QCONFIRM_RET_FALSE(KGC_MailProtocalHandler::Instance()->Init());
	QCONFIRM_RET_FALSE(KMailBoxManager::Instance()->Init(nMailBoxInitCount, nMailBoxBatchCount, nMailInitCount, nMailBatchCount, nMaxOnlineMailBoxCount));
	// 以下两个初始化的顺序不要颠倒，有问题找ZZ
	QCONFIRM_RET_FALSE(KMailPlayerManager::Instance()->Init(nMailPlayerInitCount, nMailPlayerBatchCount));
	QCONFIRM_RET_FALSE(KMailDatabase::Instance()->Init(nMailExpireTime, KCondMailManager::Instance()));

	return TRUE;
}

BOOL KMailCenter::UnInit()
{
	QCONFIRM(KMailPlayerManager::Instance()->UnInit());
	QCONFIRM(KMailDatabase::Instance()->UnInit());
	QCONFIRM(KGC_MailProtocalHandler::Instance()->UnInit());

	return TRUE;
}

BOOL KMailCenter::ProcessData(INT nConnectId, LPBYTE pData, UINT uSize)
{
	return KGC_MailProtocalHandler::Instance()->ProcessData(nConnectId, pData, uSize);
}

BOOL KMailCenter::OnPlayerLogin(DWORD dwPlayerId)
{
	QCONFIRM(KMailPlayerManager::Instance()->OnPlayerLogin(dwPlayerId));
	QCONFIRM(KMailBoxManager::Instance()->OnPlayerLogin(dwPlayerId));

	return TRUE;
}

BOOL KMailCenter::OnPlayerLogout(DWORD dwPlayerId)
{
	QCONFIRM(KMailBoxManager::Instance()->OnPlayerLogout(dwPlayerId));
	QCONFIRM(KMailPlayerManager::Instance()->OnPlayerLogout(dwPlayerId));

	return TRUE;
}

BOOL KMailCenter::OnShutDown()
{
	QCONFIRM(KMailPlayerManager::Instance()->OnShutDown());

	return TRUE;
}

BOOL KMailCenter::Breath()
{
	if (!m_bStartSearch)
	{
		INT nCurTime = (INT)KSysService::Time(NULL);
		if (nCurTime - m_nSearchTime < m_nSearchTimeSpace)
			return TRUE;

		m_bStartSearch = TRUE;
	}
	
	// 遍历在线玩家的邮箱，看有没有可以接收的邮件
	IKOnlinedPlayerMgr* pPlayerMgr = KGC_MailModule::Instance()->GetOnlinePlayerMgr();
	IKGcServer* pGcServer = KGC_MailModule::Instance()->GetIKGcServer();
	IKEnumPlayerId* pPlayerEnum = pPlayerMgr->EnumPlayer(m_dwLastPlayerId);
	QCONFIRM_RET_FALSE(pPlayerEnum);

	INT nCount = 0;
	while (TRUE)
	{
		if (nCount >= m_nSearchMailBoxCount)
		{
			// 遍历完一轮
			break;
		}
		
		if (!pPlayerEnum->Next(m_dwLastPlayerId))
		{
			// 遍历完最后一个玩家
			m_dwLastPlayerId = 0;
			m_nSearchTime = (INT)KSysService::Time(NULL);
			m_bStartSearch = FALSE;

			break;
		}

		KGcPlayerCommon* pPlayer = pPlayerMgr->GetPlayerById(m_dwLastPlayerId);
		if (!pPlayer)
		{
			QLogPrintf(LOG_ERR, "[mail] playerid:%d player not Exist!");
			continue;
		}
		else if (pPlayer->GetOnlineServer() == 0)
		{
			// 玩家不在线
			continue;
		}

		// 遍历玩家邮箱
		KMailBox* pMailBox = KMailBoxManager::Instance()->FindMailBox(m_dwLastPlayerId);
		if (!pMailBox)
		{
			// 没有在内存里，要重新加载玩家邮箱
			KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW sParam;
			sParam.nConnectId = pPlayer->GetOnlineServer();
			return KMailBoxManager::Instance()->Load(m_dwLastPlayerId, &sParam);
		}
		else
		{
			IKEnumMail* pEnum = NULL;
			if (pMailBox->GetAllMail(&pEnum))
			{
				KMail* pMail = NULL;
				BOOL bHasNewMail = FALSE;
				INT nServerID = 0;
				INT64 nNewMailId = 0;
				DWORD dwReceiverId = 0;
				while (pEnum->NextMail(&pMail))
				{
					QCONFIRM_CONTINUE(pMail && pMail->GetInfo());

					CONST KMail::KMAILHEAD *pMailHead = pMail->GetInfo();
					KGcPlayerCommon* pReceiver = pPlayerMgr->GetPlayerById(pMailHead->dwReceiverId);
					if (!pReceiver)
					{
						QLogPrintf(LOG_ERR, "[mail] KMailCenter::Breath playerid:%d player not Exist!");
						continue;
					}

					if (pMail->GetInfo()->nType == emKMAIL_TYPE_DELIVERY)
					{
						// 如果是急件，直接推送，不需要发新邮件通知
						nServerID = pReceiver->GetOnlineServer();
						if (nServerID > 0)
						{
							KGC_MailProtocalHandler::Instance()->DeliveryMailByMail(pMail, nServerID);
						}
					}

					if (pMailHead->nStat == emKMAIL_STAT_UNREAD && pMail->CanPost())
					{
						nServerID = pReceiver->GetOnlineServer();
						if (nServerID > 0)
						{
							bHasNewMail = TRUE;
							nNewMailId = pMailHead->nMailID;
							dwReceiverId = pReceiver->GetId();
						}
					}
				}

				if (bHasNewMail)
				{
					// 通知玩家有新邮件
					KMAIL_G2S_PROTO_ADDMAIL sNotify;
					sNotify.dwPlayerId	 = dwReceiverId;
					sNotify.nMailID		 = nNewMailId;
					return pGcServer->Send(nServerID, &sNotify, sizeof(sNotify));
				}
			}
		}

		nCount++;
	}

	return TRUE;
}

BOOL KMailCenter::PostSysMail(DWORD dwReceiverId, LPCSTR pszCaption, LPCSTR pszContent, 
							  KItemData_Common* pItemData, INT nItemCount, INT nIconId)
{
	return KGC_MailProtocalHandler::Instance()->PostSysMail(dwReceiverId, pszCaption, pszContent, pItemData, nItemCount, nIconId);
}

// -------------------------------------------------------------------------
#endif /* GAME_CENTER */
