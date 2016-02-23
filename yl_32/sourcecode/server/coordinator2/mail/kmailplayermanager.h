
#pragma once

#ifdef GAME_CENTER
#include "Misc/mysqldatabase.h"
#include "Misc/objectpool.h"
#include <map>
#include "serverbase/kdbcallback.h"

class KMailPlayerManager : public IKDBThreadCallback, public IKMainThreadCallback
{
	struct KMAIL_PLAYER
	{
		DWORD dwPlayerId;
		INT64 nSysMailId;		// 上次获取的系统邮件ID
		BYTE byMailLoading;		// 是否正在加载邮件
		BYTE byMailLoaded;		// 是否完成邮件加载
		BYTE byMailFetchLock;	// 邮件是否锁住
		INT nPlayerMailCount;	// 普通玩家邮件个数

		KMAIL_PLAYER()
		{
			nSysMailId = 0;
			byMailLoaded = 0;
			byMailLoading = 0;
			byMailFetchLock = 0;
			nPlayerMailCount = 0;
		}
	};

	enum KE_COMMAND
	{
		emKCMD_GETINFO = 1,		// 获取玩家邮件信息
		emKCMD_ADDINFO,			// 添加新玩家邮件信息
		emKCMD_DELINFO,			// 删除玩家邮件信息
		emKCMD_MODINFO,			// 修改玩家邮件信息
	};

	struct KKEY_INFO : public KMAIL_PLAYER
	{
		KMySqlResult sResult;
		BOOL bResult;
	};

public:
	KMailPlayerManager();
	~KMailPlayerManager();

public:
	static KMailPlayerManager* Instance() {return &ms_inst;}

public:
	BOOL Init(INT nPlayerInitCount, INT nPlayerBatchCount);
	BOOL UnInit();

	BOOL OnPlayerLogin(DWORD dwPlayerId);
	BOOL OnPlayerLogout(DWORD dwPlayerId);
	BOOL OnShutDown();

	BOOL SetSysMailId(DWORD dwPlayerId, INT64 nSysMailId);
	INT64 GetSysMailId(DWORD dwPlayerId);
	BOOL SetMailLoading(DWORD dwPlayerId, BYTE byMailLoading);
	BYTE GetMailLoading(DWORD dwPlayerId);
	BOOL SetMailLoaded(DWORD dwPlayerId, BYTE byMailLoaded);
	BYTE GetMailLoaded(DWORD dwPlayerId);
	BOOL SetMailFetchLock(DWORD dwPlayerId, BYTE byMailFetchLock);
	BYTE GeMailFetchLock(DWORD dwPlayerId);
	BOOL SetPlayerMailCount(DWORD dwPlayerId, INT nCount);
	INT GetPlayerMailCount(DWORD dwPlayerId);

	BOOL Save(DWORD dwPlayerId);

private:
	virtual BOOL DBThreadCallback(INT nParam, LPVOID pParam);
	virtual BOOL MainThreadCallback(INT nParam, LPVOID pParam);
	BOOL InitMailPlayerDataBase();
	BOOL Load();
	BOOL SaveAll();
	BOOL SaveOne(KMAIL_PLAYER* pInfo);

	BOOL Push_GetInfo(INT nParam, LPVOID pParam);
	BOOL Push_AddInfo(INT nParam, LPVOID pParam);
	BOOL Push_DeleteInfo(INT nParam, LPVOID pParam);
	BOOL Push_ModifyInfo(INT nParam, LPVOID pParam);
	BOOL Pop_GetInfo(INT nParam, LPVOID pParam);
	BOOL Pop_AddInfo(INT nParam, LPVOID pParam);
	BOOL Pop_DeleteInfo(INT nParam, LPVOID pParam);
	BOOL Pop_ModifyInfo(INT nParam, LPVOID pParam);

private:
	typedef std::map<DWORD, KMAIL_PLAYER*> KMAILPLAYER_MAP;
	static KMailPlayerManager		ms_inst;
	KMAILPLAYER_MAP					m_mapMailPlayer;
	IKGcDatabase*					m_pIKGcDataBase;
	KIncObjPool<KKEY_INFO>			m_poolKey;
	KIncObjPool<KMAIL_PLAYER>		m_poolMailPlayer;
	CHAR							m_szSQL[1024];
};

#endif /* GAME_CENTER */
