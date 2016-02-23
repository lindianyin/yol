/* -------------------------------------------------------------------------
//	文件名		：	kmailboxmanager.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/16 18:26:26
//	功能描述	：	邮箱管理器
//
// -----------------------------------------------------------------------*/
#ifndef __KMAILBOXMANAGER_H__
#define __KMAILBOXMANAGER_H__

// -------------------------------------------------------------------------
#ifdef GAME_CENTER

#include "kmaildatabase.h"
#include "kmailbox.h"
#include "kmail.h"
#include "Misc/objectpool.h"
#include "gclogicbase/kgcplayercommon.h"
#include <map>
#include <vector>
#include <functional>

class KMailBoxManager : public IKMailCB_FetchMail
{
private:
	typedef std::map<DWORD, KMailBox*>	KMAILBOX_MAP;

public:
	KMailBoxManager();
	~KMailBoxManager();

	static KMailBoxManager *Instance() { return &ms_inst; }

	BOOL Init(INT nMailBoxInitCount, INT nMailBoxBatchCount, INT nMailInitCount, INT nMailBatchCount, INT nMaxOnlineMailBoxCount);
	BOOL OnPlayerLogin(DWORD dwPlayerId);
	BOOL OnPlayerLogout(DWORD dwPlayerId);

	// 将玩家邮件加载到内存中
	BOOL Load(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD* pParam);
	BOOL Unload(DWORD dwPlayerId);
	BOOL SaveAllMail(DWORD dwPlayerId);

	KMailBox *FindMailBox(DWORD dwPlayerId);
	KMail *FindMail(DWORD dwPlayerId, INT64 nMailID, KMailBox **ppMailBox = NULL);
	KMail* AllocateMail();
	VOID FreeMail(KMail* pMail);
	BOOL OnMailLoaded(DWORD dwPlayerId);

private:
	BOOL _Load(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD* pParam);
	BOOL _AllocateMailBox(DWORD dwPlayerId);
	BOOL _Process_FetchInfo(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHINFO* pParam);
	BOOL _Process_FetchCondInfo(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHCONDINFO* pParam);
	BOOL _Process_FetchMail(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL* pParam);
	BOOL _Process_DeleteMail(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL* pParam);
	BOOL _Process_FetchItem(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM* pParam);
	BOOL _Process_DeleteItem(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM* pParam);
	BOOL _Process_FindNew(DWORD dwPlayerId, KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW* pParam);

private:
	virtual BOOL FetchMail(DWORD dwOwnerId, KMAIL_INFO *pMail);
	virtual BOOL Finish(DWORD dwOwnerId, LPVOID pParam);

private:
	static KMailBoxManager	ms_inst;

private:
	KMAILBOX_MAP			m_mapMailBox;
	KIncObjPool<KMailBox>	m_cBoxPool;		// 邮箱对象池
	KIncObjPool<KMail>		m_cMailPool;	// 邮件对象池

	KIncObjPool<KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHINFO>		m_cFetchInfoPool;
	KIncObjPool<KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHCONDINFO>	m_cFechCondInfoPool;
	KIncObjPool<KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL>		m_cFetchMailPool;
	KIncObjPool<KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL>		m_cDeleteMailPool;
	KIncObjPool<KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM>		m_cFetchItemPool;
	KIncObjPool<KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM>		m_cDeleteItemPool;
	KIncObjPool<KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW>			m_cFindNewPool;

	INT	m_nMaxOnlineMailBoxCount;		// 内存中最多存储的邮箱数
};

#endif /* GAME_CENTER */
// -------------------------------------------------------------------------

#endif /* __KMAILBOXMANAGER_H__ */
