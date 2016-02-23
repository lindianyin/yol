/* -------------------------------------------------------------------------
//	文件名		：	kmailboxmanager.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/16 18:28:59
//	功能描述	：	邮箱管理器
//
// -----------------------------------------------------------------------*/

#ifdef GAME_CENTER
#include "stdafx.h"
#include "kmailboxmanager.h"
#include "kcondmailmanager.h"
#include "kgc_mailmodule.h"
#include "kmailplayermanager.h"
#include "kgc_mailprotocalhandler.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "gclogicbase/kgclogicextinterface.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailBoxManager KMailBoxManager::ms_inst;

KMailBoxManager::KMailBoxManager()
{
}

KMailBoxManager::~KMailBoxManager()
{
}

BOOL KMailBoxManager::Init(INT nMailBoxInitCount, INT nMailBoxBatchCount, INT nMailInitCount, INT nMailBatchCount, INT nMaxOnlineMailBoxCount)
{
	QCONFIRM_RET_FALSE(nMailBoxInitCount > 0 && nMailBoxBatchCount > 0);
	QCONFIRM_RET_FALSE(nMailInitCount > 0 && nMailBatchCount > 0);
	QCONFIRM_RET_FALSE(nMaxOnlineMailBoxCount > 0);
	
	m_cBoxPool.Init(nMailBoxInitCount, nMailBoxBatchCount);
	m_cMailPool.Init(nMailInitCount, nMailBatchCount);

	m_cFetchInfoPool.Init(1, 1);
	m_cFechCondInfoPool.Init(1, 1);
	m_cFetchMailPool.Init(1, 1);
	m_cFetchItemPool.Init(1, 1);
	m_cDeleteItemPool.Init(1, 1);
	m_cFindNewPool.Init(1, 1);

	m_nMaxOnlineMailBoxCount = nMaxOnlineMailBoxCount;

	return TRUE;
}

KMail* KMailBoxManager::AllocateMail()
{
	KMail* pMail = m_cMailPool.New();
	if (pMail)
		pMail->Clear();
	return pMail;
}

VOID KMailBoxManager::FreeMail(KMail* pMail)
{
	if (pMail)
	{
		pMail->Clear();
		m_cMailPool.Free(pMail);
	}
}

BOOL KMailBoxManager::OnPlayerLogin(DWORD dwPlayerId)
{
	// 玩家登陆只通知邮箱加载成功，减少GC宕机重连时的压力	
	IKOnlinedPlayerMgr* pPlayerMgr = KGC_MailModule::Instance()->GetOnlinePlayerMgr();
	QCONFIRM_RET_FALSE(pPlayerMgr);
	KGcPlayerCommon* pPlayer = pPlayerMgr->GetPlayerById(dwPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);
	IKGcServer* pGcServer = KGC_MailModule::Instance()->GetIKGcServer();
	QCONFIRM_RET_FALSE(pGcServer);

	INT nServerID = pPlayer->GetOnlineServer();
	if (nServerID > 0)
	{
		// 通知客户端邮件加载完成
		KMAIL_G2S_PROTO_LOADOK sMsg;
		sMsg.dwPlayerId = pPlayer->GetId();
		sMsg.byShowNotify = 1;

		pGcServer->Send(nServerID, &sMsg, sizeof(sMsg));
	}
	
	return TRUE;
}

BOOL KMailBoxManager::OnPlayerLogout(DWORD dwPlayerId)
{
	KMailPlayerManager* pMailPlayerManager = KMailPlayerManager::Instance();
	if (pMailPlayerManager->GetMailLoaded(dwPlayerId) == 1)
	{
		QCONFIRM(Unload(dwPlayerId));
		pMailPlayerManager->SetMailLoaded(dwPlayerId, 0);
	}

	return TRUE;
}

BOOL KMailBoxManager::Load(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD* pParam)
{	
	QCONFIRM_RET_FALSE(pParam);

	BOOL bFindMailBox = FALSE;
	if (FindMailBox(dwPlayerId))
		bFindMailBox = TRUE;

	// 加载邮箱（避免重复加载）
	KMailPlayerManager* pMailPlayerManager = KMailPlayerManager::Instance();
	if (!bFindMailBox &&
		pMailPlayerManager->GetMailLoading(dwPlayerId) == 0 &&
		pMailPlayerManager->GetMailLoaded(dwPlayerId) == 0)
	{
		return _Load(dwPlayerId, pParam);
	}

	if (bFindMailBox && pMailPlayerManager->GetMailLoaded(dwPlayerId) == 1)
	{
		return Finish(dwPlayerId, pParam);
	}

	return TRUE;
}

BOOL KMailBoxManager::Unload(DWORD dwPlayerId)
{
	KMAILBOX_MAP::iterator it = m_mapMailBox.find(dwPlayerId);
	if (it != m_mapMailBox.end())
	{
		// 保存邮件到数据库
		KMailBox* pMailBox = it->second;
		pMailBox->Save();
		pMailBox->UnInit();
		m_cBoxPool.Free(pMailBox);
		m_mapMailBox.erase(it);
		return TRUE;
	}
	return FALSE;
}

BOOL KMailBoxManager::SaveAllMail(DWORD dwPlayerId)
{
	KMAILBOX_MAP::iterator it = m_mapMailBox.find(dwPlayerId);
	if (it != m_mapMailBox.end())
	{
		// 保存邮件到数据库
		KMailBox* pMailBox = it->second;
		pMailBox->Save();
	}
	return TRUE;
}

KMailBox* KMailBoxManager::FindMailBox(DWORD dwPlayerId)
{
	KMAILBOX_MAP::iterator it = m_mapMailBox.find(dwPlayerId);
	if (it == m_mapMailBox.end())
		return NULL;
	return it->second;
}

KMail* KMailBoxManager::FindMail(DWORD dwPlayerId, INT64 nMailID, KMailBox** ppMailBox)
{
	QCONFIRM_RET_NULL(nMailID > 0);
	KMailBox* pMailBox = FindMailBox(dwPlayerId);
	if (!pMailBox)
		return NULL;
	KMail* pMail = pMailBox->FindMail(nMailID);
	if (!pMail)
		return NULL;
	if (ppMailBox)
		*ppMailBox = pMailBox;
	return pMail;
}

BOOL KMailBoxManager::FetchMail(DWORD dwOwnerId, KMAIL_INFO* pMail)
{
	QCONFIRM_RET_FALSE(pMail);
	
	KMailBox* pMailBox = FindMailBox(dwOwnerId);
	if (!pMailBox)
	{
		// 玩家在获取到邮件之前下线
		return TRUE;
	}
	if (!pMailBox->LoadMail(pMail))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::FetchMail LoadMail failed!");
		return FALSE;
	}
	return TRUE;
}

BOOL KMailBoxManager::_Load(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD* pParam)
{	
	// 分配邮箱
	if (!_AllocateMailBox(dwPlayerId))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load AllocateMailBox failed!");
		return FALSE;
	}

	// 收取全区系统条件邮件
	KCondMailManager::Instance()->PostMail(dwPlayerId);

	// 分配参数空间
	KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD* pCBParam = NULL;
	switch (pParam->byType)
	{
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHINFO:
		{
			pCBParam = m_cFetchInfoPool.New();
			if (!pCBParam)
			{
				QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Allocate CallBackParam Failed!");
				return FALSE;
			}
			pCBParam->nConnectId = pParam->nConnectId;
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHCONDINFO:
		{
			pCBParam = m_cFechCondInfoPool.New();
			if (!pCBParam)
			{
				QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Allocate CallBackParam Failed!");
				return FALSE;
			}
			pCBParam->nConnectId = pParam->nConnectId;
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHMAIL:
		{
			pCBParam = m_cFetchMailPool.New();
			if (!pCBParam)
			{
				QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Allocate CallBackParam Failed!");
				return FALSE;
			}
			pCBParam->nConnectId = pParam->nConnectId;
			((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL*)pCBParam)->nMailId = ((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL*)pParam)->nMailId;
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEMAIL:
		{
			pCBParam = m_cDeleteMailPool.New();
			if (!pCBParam)
			{
				QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Allocate CallBackParam Failed!");
				return FALSE;
			}
			pCBParam->nConnectId = pParam->nConnectId;
			((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL*)pCBParam)->nMailId = ((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL*)pParam)->nMailId;
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHITEM:
		{
			pCBParam = m_cFetchItemPool.New();
			if (!pCBParam)
			{
				QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Allocate CallBackParam Failed!");
				return FALSE;
			}
			pCBParam->nConnectId = pParam->nConnectId;
			((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM*)pCBParam)->nMailId = ((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM*)pParam)->nMailId;
			((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM*)pCBParam)->byItemIndex = ((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM*)pParam)->byItemIndex;
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEITEM:
		{
			pCBParam = m_cDeleteItemPool.New();
			if (!pCBParam)
			{
				QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Allocate CallBackParam Failed!");
				return FALSE;
			}
			pCBParam->nConnectId = pParam->nConnectId;
			((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM*)pCBParam)->nMailId = ((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM*)pParam)->nMailId;
			((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM*)pCBParam)->byItemIndex = ((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM*)pParam)->byItemIndex;
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FINDNEW:
		{
			pCBParam = m_cFindNewPool.New();
			if (!pCBParam)
			{
				QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Allocate CallBackParam Failed!");
				return FALSE;
			}
			pCBParam->nConnectId = pParam->nConnectId;
		} break;
	default:
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_Load Error CallBackParam Type!");
		return FALSE;
	}

	// 从数据库加载邮件
	KMailDatabase *pDatabase = KMailDatabase::Instance();
	pDatabase->FetchMail(dwPlayerId, this, pCBParam);

	// 设置正在加载
	KMailPlayerManager::Instance()->SetMailLoading(dwPlayerId, 1);
	return TRUE;
}

// 分配邮箱
BOOL KMailBoxManager::_AllocateMailBox(DWORD dwPlayerId)
{
	while (m_mapMailBox.size() >= m_nMaxOnlineMailBoxCount)
	{
		// 达到内存中邮箱数上限
		KMAILBOX_MAP::iterator itFind = m_mapMailBox.begin();
		for (KMAILBOX_MAP::iterator it = m_mapMailBox.begin(); it != m_mapMailBox.end(); ++it)
		{
			INT nTime = it->second->GetInitTime();
			INT nFindTime = itFind->second->GetInitTime();
			if (nTime < nFindTime)
				itFind = it;
		}

		KMailPlayerManager::Instance()->Save(itFind->first);
		Unload(itFind->first);
	}

	// 重新分配邮箱
	KMailBox* pMailBox = m_cBoxPool.New();
	if (!pMailBox)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_AllocateMailBox Allocate MailBox Failed!");
		return FALSE;
	}

	INT nCurTime = (INT)KSysService::Time(NULL);
	pMailBox->Init(dwPlayerId, nCurTime);
	std::pair<KMAILBOX_MAP::iterator, bool> sRes = m_mapMailBox.insert(std::make_pair(dwPlayerId, pMailBox));
	if (!sRes.second)
	{
		m_cBoxPool.Free(pMailBox);
		
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::_AllocateMailBox insert map Failed!");
		return FALSE;
	}

	return TRUE;
}

BOOL KMailBoxManager::Finish(DWORD dwOwnerId, LPVOID pParam)
{
	QCONFIRM_RET_FALSE(pParam);
	QCONFIRM_RET_FALSE(KMailPlayerManager::Instance()->SetMailLoading(dwOwnerId, 0));
	QCONFIRM_RET_FALSE(KMailPlayerManager::Instance()->SetMailLoaded(dwOwnerId, 1));

	BOOL bRet = FALSE;
	KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD* pCBParam = (KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD*)pParam;
	switch (pCBParam->byType)
	{
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHINFO:
		{
			bRet = _Process_FetchInfo(dwOwnerId, (KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHINFO*)pCBParam);
			m_cFetchInfoPool.Free((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHINFO*)pCBParam);
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHCONDINFO:
		{
			bRet = _Process_FetchCondInfo(dwOwnerId, (KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHCONDINFO*)pCBParam);
			m_cFechCondInfoPool.Free((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHCONDINFO*)pCBParam);
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHMAIL:
		{
			bRet = _Process_FetchMail(dwOwnerId, (KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL*)pCBParam);
			m_cFetchMailPool.Free((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL*)pCBParam);
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEMAIL:
		{
			bRet = _Process_DeleteMail(dwOwnerId, (KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL*)pCBParam);
			m_cDeleteMailPool.Free((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL*)pCBParam);
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHITEM:
		{
			bRet = _Process_FetchItem(dwOwnerId, (KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM*)pCBParam);
			m_cFetchItemPool.Free((KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM*)pCBParam);
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEITEM:
		{
			bRet = _Process_DeleteItem(dwOwnerId, (KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM*)pCBParam);
			m_cDeleteItemPool.Free((KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM*)pCBParam);
		} break;
	case emKMAIL_FETCHMAIL_CALLBACK_TYPE_FINDNEW:
		{
			bRet = _Process_FindNew(dwOwnerId, (KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW*)pCBParam);
			m_cFindNewPool.Free((KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW*)pCBParam);
		} break;
	default:
		QLogPrintf(LOG_ERR, "[mail] %s", "KMailBoxManager::Finish Error CallBackParam Type!");
		return FALSE;
	}
	
	return bRet;
}

// 获取玩家邮件信息
BOOL KMailBoxManager::_Process_FetchInfo(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHINFO* pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KMAIL_S2G_PROTO_FETCHINFO sInfo;
	sInfo.dwPlayerId = dwPlayerId;

	return KGC_MailProtocalHandler::Instance()->Process_FetchInfo(
		(LPBYTE)&sInfo,
		sizeof(sInfo),
		pParam->nConnectId
	);
}

// 获取玩家邮件信息（包括全区邮件）
BOOL KMailBoxManager::_Process_FetchCondInfo(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHCONDINFO* pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KMAIL_S2G_PROTO_FETCHCONDINFO sInfo;
	sInfo.dwPlayerId = dwPlayerId;

	return KGC_MailProtocalHandler::Instance()->Process_FetchCondInfo(
		(LPBYTE)&sInfo,
		sizeof(sInfo),
		pParam->nConnectId
	);
}

// 获取指定邮件内容
BOOL KMailBoxManager::_Process_FetchMail(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL* pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KMAIL_S2G_PROTO_FETCHMAIL sInfo;
	sInfo.dwPlayerId = dwPlayerId;
	sInfo.nMailID = pParam->nMailId;

	return KGC_MailProtocalHandler::Instance()->Process_FetchMail(
		(LPBYTE)&sInfo,
		sizeof(sInfo),
		pParam->nConnectId
	);
}

// 删除指定邮件
BOOL KMailBoxManager::_Process_DeleteMail(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL* pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KMAIL_S2G_PROTO_DELMAIL sInfo;
	sInfo.dwPlayerId = dwPlayerId;
	sInfo.nMailID = pParam->nMailId;

	return KGC_MailProtocalHandler::Instance()->Process_DeleteMail(
		(LPBYTE)&sInfo,
		sizeof(sInfo),
		pParam->nConnectId
	);
}

// 获取指定附件信息
BOOL KMailBoxManager::_Process_FetchItem(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM* pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KMAIL_S2G_PROTO_FETCHITEM sInfo;
	sInfo.dwPlayerId = dwPlayerId;
	sInfo.nMailID = pParam->nMailId;
	sInfo.nItemIdx = pParam->byItemIndex;

	return KGC_MailProtocalHandler::Instance()->Process_FetchItem(
		(LPBYTE)&sInfo,
		sizeof(sInfo),
		pParam->nConnectId
	);
}

// 删除指定附件
BOOL KMailBoxManager::_Process_DeleteItem(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM* pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	KMAIL_S2G_PROTO_DELETEITEM sInfo;
	sInfo.dwPlayerId = dwPlayerId;
	sInfo.nMailID = pParam->nMailId;
	sInfo.nItemIdx = pParam->byItemIndex;

	return KGC_MailProtocalHandler::Instance()->Process_DeleteItem(
		(LPBYTE)&sInfo,
		sizeof(sInfo),
		pParam->nConnectId
	);
}

// 查看有无新邮件
BOOL KMailBoxManager::_Process_FindNew(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW* pParam)
{
	QCONFIRM_RET_FALSE(pParam);

	IKOnlinedPlayerMgr* pPlayerMgr = KGC_MailModule::Instance()->GetOnlinePlayerMgr();
	IKGcServer* pGcServer = KGC_MailModule::Instance()->GetIKGcServer();
	KMailBox* pMailBox = KMailBoxManager::Instance()->FindMailBox(dwPlayerId);
	if (pMailBox)
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
					QLogPrintf(LOG_ERR, "[mail] KMailBoxManager::_Process_FindNew playerid:%d player not Exist!");
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

	return TRUE;
}

// -------------------------------------------------------------------------
#endif /* GAME_CENTER */
