/* -------------------------------------------------------------------------
//	文件名		：	kgc_mailprotocalhandler.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/17 16:49:14
//	功能描述	：	处理来自GS的协议
//
// -----------------------------------------------------------------------*/
#ifndef __KGC_MAILPROTOCALHANDLER_H__
#define __KGC_MAILPROTOCALHANDLER_H__

// -------------------------------------------------------------------------
#ifdef GAME_CENTER
#include "kmaildatabase.h"
#include "kmailbox.h"
#include "gcmodule/mail/kmail_datadef.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "gclogicbase/kgclogicextinterface.h"
#include "serverbase/kgcmoduleinterface.h"
#include "Misc/objectpool.h"
interface IKOnlinedPlayerMgr;

class KMailCountNotifier
{
public:
	BOOL Init(CONST KMAIL_INFO *pInfo);
	virtual BOOL NotifyMailCount(INT64 nMailCount);
private:
	BOOL SendMail(DWORD dwPlayerId, KMAIL_INFO *pMail);
private:
	KMAIL_INFO	m_sInfo;
	CHAR		m_szBuffer[KD_MAIL_MAX_SIZE];
};

class KGC_MailProtocalHandler : public IKMailCB_MailCount
{
public:
	struct KMAIL
	{
		LPSTR				pCaption;
		INT					nCaptionLen;
		LPSTR				pContent;
		INT					nContentLen;
		KItemData_Common*	pItemData;
		INT					nItemCount;
		INT					nIconId;
	};

public:
	KGC_MailProtocalHandler();
	~KGC_MailProtocalHandler();

public:
	static KGC_MailProtocalHandler* Instance() {return &ms_inst;}

public:
	BOOL Init();
	BOOL UnInit();
	BOOL ProcessData(INT nConnectId, LPBYTE pData, UINT uSize);

	// 发送系统邮件
	BOOL PostSysMail(DWORD dwReceiverId, LPCSTR pszCaption, LPCSTR pszContent, 
					 KItemData_Common* pItemData, INT nItemCount, INT nIconId);
	// 直接推送邮件
	BOOL DeliveryMailByInfo(KMAIL_INFO* pMail, INT nConnectId);
	BOOL DeliveryMailByMail(KMail* pMail, INT nConnectId);

public:
	// 玩家上线处理
	BOOL Process_PlayerLogin(LPBYTE pData, UINT uSize, INT nConnectId);
	// 玩家下线处理
	BOOL Process_PlayerLogout(LPBYTE pData, UINT uSize, INT nConnectId);
	// 邮件列表邮箱加载完成时申请
	BOOL Process_FetchInfo(LPBYTE pData, UINT uSize, INT nConnectId);
	// 系统邮件为邮箱打开时或新邮件通知且邮箱为开启状态时申请
	BOOL Process_FetchCondInfo(LPBYTE pData, UINT uSize, INT nConnectId);
	// 打开邮件时申请
	BOOL Process_FetchMail(LPBYTE pData, UINT uSize, INT nConnectId);
	BOOL Process_FetchItem(LPBYTE pData, UINT uSize, INT nConnectId);
	BOOL Process_DeleteMail(LPBYTE pData, UINT uSize, INT nConnectId);
	BOOL Process_DeleteItem(LPBYTE pData, UINT uSize, INT nConnectId);
	BOOL Process_SendMail(LPBYTE pData, UINT uSize, INT nConnectId);
	BOOL Process_SendCondMail(LPBYTE pData, UINT uSize, INT nConnectId);
	BOOL Process_CheckReceiver(LPBYTE pData, UINT uSize, INT nConnectId);

private:
	BOOL FetchMailList(IKEnumMail *pEnum, DWORD dwPlayerId, INT nConnectId);
	BOOL SendMail(CONST KMAIL_S2G_PROTO_SENDMAIL *pMsg, KMAIL_INFO *pMail);
	BOOL SendPlayerMail(DWORD dwSenderId, KGcPlayerCommon* pReceiver, KMAIL_INFO* pMail);
	BOOL SendSystemMail(KGcPlayerCommon* pReceiver, KMAIL_INFO* pMail);
	BOOL SendDeliveryMail(DWORD dwSenderId, KGcPlayerCommon* pReceiver, KMAIL_INFO* pMail);
	BOOL InitMailInfo(CONST KMAIL_S2G_PROTO_SENDMAIL* pMsg, KMAIL_INFO* pInfo);
	KMAIL_DBINFO* InitMailData(CONST KMAIL* pMail);
	BOOL NotifyMailCount(DWORD dwOwnerId, INT64 nMailCount, LPVOID pParam);

private:
	static KGC_MailProtocalHandler	ms_inst;
	IKGcServer*						m_pIKGcServer;
	IKOnlinedPlayerMgr*				m_pIKOnlinePlayerMgr;
	CHAR							m_szBuffer[32 * 1024];
	KIncObjPool<KMailCountNotifier>	m_poolNotifiers;
};

#endif /* GAME_CENTER */
// -------------------------------------------------------------------------

#endif /* __KGC_MAILPROTOCALHANDLER_H__ */
