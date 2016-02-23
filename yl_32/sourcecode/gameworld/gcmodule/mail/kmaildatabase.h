/* -------------------------------------------------------------------------
//	文件名		：	kmaildatabase.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/16 16:16:05
//	功能描述	：	邮件数据库（负责邮件的存取）
//
// -----------------------------------------------------------------------*/
#ifndef __KMAILDATABASE_H__
#define __KMAILDATABASE_H__

// -------------------------------------------------------------------------
#ifdef GAME_CENTER

#include "Misc/mysqldatabase.h"
#include "Misc/objectpool.h"
#include "gcmodule/mail/kmail_datadef.h"

// 系统邮件（接受者为符合某些条件的玩家）
struct KCONDMAIL_INFO
{
	INT64				nMailID;		// 邮件ID
	KMAIL_DATETIME		sSendTime;		// 发送时间
	INT					nCondLen;		// 条件长度
	INT					nMailLen;		// 邮件长度
	LPSTR				pCond;			// 条件指针
	LPSTR				pMail;			// 邮件指针
};

// 回调接口：获取系统邮件
class IKMailCB_FetchSysMail
{
public:
	virtual BOOL FetchSysMail(CONST KCONDMAIL_INFO* pSysMail) = 0;
};

// 回调接口：获取邮件
class IKMailCB_FetchMail
{
public:
	virtual BOOL FetchMail(DWORD dwOwnerId, KMAIL_INFO* pMail) = 0;
	virtual BOOL Finish(DWORD dwOwnerId, LPVOID pParam) = 0;
};

// 回调接口：获取邮件数量
class IKMailCB_MailCount
{
public:
	virtual BOOL NotifyMailCount(DWORD dwOwnerId, INT64 nMailCount, LPVOID pParam) = 0;
};

/************************* 获取邮件回调，finish参数相关 ***************************/
enum KE_MAIL_FETCHMAIL_CALLBACK_TYPE
{
	emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHINFO,			// 获取普通邮件列表（除全区邮件）
	emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHCONDINFO,		// 获取全部邮件列表
	emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHMAIL,			// 获取邮件内容
	emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEMAIL,			// 删除指定邮件
	emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHITEM,			// 获取附件
	emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEITEM,			// 删除附件
	emKMAIL_FETCHMAIL_CALLBACK_TYPE_FINDNEW,			// 查看有无新邮件
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	BYTE byType; 
	INT nConnectId;
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHINFO : public KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHINFO()
	{
		byType = emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHINFO;
		nConnectId = 0;
	}
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHCONDINFO : public KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHCONDINFO()
	{
		byType = emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHCONDINFO;
		nConnectId = 0;
	}
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL : public KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	INT64 nMailId;

	KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHMAIL()
	{
		byType = emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHMAIL;
		nConnectId = 0;
		nMailId = 0;
	}
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL : public KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	INT64 nMailId;

	KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEMAIL()
	{
		byType = emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEMAIL;
		nConnectId = 0;
		nMailId = 0;
	}
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM : public KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	INT64 nMailId;
	BYTE byItemIndex;

	KMAIL_FETCHMAIL_CALLBACK_PARAM_FETCHITEM()
	{
		byType = emKMAIL_FETCHMAIL_CALLBACK_TYPE_FETCHITEM;
		nConnectId = 0;
		nMailId = 0;
		byItemIndex = 0;
	}
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM : public KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	INT64 nMailId;
	BYTE byItemIndex;

	KMAIL_FETCHMAIL_CALLBACK_PARAM_DELETEITEM()
	{
		byType = emKMAIL_FETCHMAIL_CALLBACK_TYPE_DELETEITEM;
		nConnectId = 0;
		nMailId = 0;
		byItemIndex = 0;
	}
};

struct KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW : public KMAIL_FETCHMAIL_CALLBACK_PARAM_HEAD
{
	KMAIL_FETCHMAIL_CALLBACK_PARAM_FINDNEW()
	{
		byType = emKMAIL_FETCHMAIL_CALLBACK_TYPE_FINDNEW;
		nConnectId = 0;
	}
};

/************************* 获取邮件回调，finish参数相关 ***************************/

#pragma pack(push, 1)
struct KSTR_DATETIME
{
	CHAR	szYear[4];
	CHAR	chSep1;
	CHAR	szMonth[2];
	CHAR	chSep2;
	CHAR	szDay[2];
	CHAR	chSep3;
	CHAR	szHour[2];
	CHAR	chSep4;
	CHAR	szMinute[2];
	CHAR	chSep5;
	CHAR	szSecond[2];
	CHAR	chEnd;
};
#pragma pack(pop)

class KMailDatabase : public IKDBThreadCallback, public IKMainThreadCallback
{
public:
	enum KE_COMMAND
	{
		emKCMD_GETMAIL = 1,		// 获取邮件
		emKCMD_ADDMAIL,			// 生成邮件
		emKCMD_DELMAIL,			// 删除邮件
		emKCMD_MODMAIL,			// 修改邮件
		emKCMD_DELOWNER,		// 删除某个玩家相关的所有邮件
		emKCMD_CLEAROWNER,		// 清除玩家邮箱所有邮件
		emKCMD_ADDSYSMAIL,		// 生成系统邮件（发送给符合指定条件的玩家）
	};
	// 修改邮件的掩码
	enum KE_MASK_MODIFYMAIL
	{
		emKMASK_MODIFYMAIL_STAT = 0x1,	// 修改邮件状态
		emKMASK_MODIFYMAIL_MAIL = 0x2,	// 修改邮件内容
	};
	struct KKEY_GUID
	{
		DWORD	dwOwnerId;
	};
	struct KKEY_QUERYMAIL : public KKEY_GUID
	{
		IKMailCB_FetchMail*	pCallBack;
		KMySqlResult		sResult;
		BOOL				bResult;
		LPVOID				pParam;
	};
	struct KKEY_INSERTMAIL : public KKEY_GUID
	{
		KMAIL_INFO		sMail;
		CHAR			szData[KD_MAIL_MAX_SIZE];
	};
	struct KKEY_MODIFYMAIL
	{
		INT					nMask;
		KMAIL_MODINFO		sMail;
		CHAR				szData[KD_MAIL_MAX_SIZE];
	};
	struct KKEY_MAILCOUNT : public KKEY_GUID
	{
		BOOL				bResult;
		IKMailCB_MailCount* pCallBack;
		KMySqlResult		sResult;
		LPVOID				pParam;
	};
	struct KKEY_MAILID
	{
		INT64				nMailID;
	};
	struct KKEY_SYSMAIL : public KKEY_MAILID
	{
		KMAIL_DATETIME		sSendTime;
		INT					nCondLen;
		INT					nMailLen;
		CHAR				szCond[KD_MAIL_MAX_COND_LENGTH];
		CHAR				szMail[KD_MAIL_MAX_SIZE];
	};
public:
	static KMailDatabase* Instance() {return &ms_inst;}
	KMailDatabase();
	~KMailDatabase();
	/***********************************************************
	 Function		: Initialize
	 Description	: 初始化
	 Return			: BOOL
	 ***********************************************************/
	BOOL Init(INT nMailExpireTime, IKMailCB_FetchSysMail* pCallBack = NULL);
	/***********************************************************
	 Function		: UnInitialize
	 Description	: 反初始化
	 Return			: BOOL
	 ***********************************************************/
	BOOL UnInit();
	/***********************************************************
	 Function		: FetchMail
	 Description	: 收取邮件（异步操作）
	 Return			: BOOL
	 Param			: dwOwnerId		玩家Id
	 Param			: pCallBack		回调接口
	 ***********************************************************/
	BOOL FetchMail(DWORD dwOwnerId, IKMailCB_FetchMail* pCallBack, LPVOID pParam = NULL);
	/***********************************************************
	 Function		: AddMail
	 Description	: 把邮件放入玩家邮箱
	 Return			: BOOL
	 Param			: dwOwnerId		玩家Id
	 Param			: pMail[in/out]	邮件信息，返回KMAIL_INFO::nMailID和KMAIL_INFO::sSendTime
	 ***********************************************************/
	BOOL AddMail(DWORD dwOwnerId, KMAIL_INFO* pMail);
	/***********************************************************
	 Function		: AddCondMail
	 Description	: 向数据库插入条件邮件（发送给符合指定条件的玩家）
	 Return			: BOOL
	 Param			: pMail[in/out]	邮件信息，返回KCONDMAIL_INFO::dwMailID和KMAIL_INFO::sSendTime
	 ***********************************************************/
	BOOL AddCondMail(KCONDMAIL_INFO* pMail);
	/***********************************************************
	 Function		: QueryMailCount
	 Description	: 查询某个玩家邮箱中的邮件数量（异步操作）
	 Return			: BOOL
	 Param			: dwOwnerId		玩家Id
	 Param			: pCallBack		回调接口
	 ***********************************************************/
	BOOL QueryMailCount(DWORD dwOwnerId, IKMailCB_MailCount* pCallBack, LPVOID pParam = NULL);
	/***********************************************************
	 Function		: DeleteMail
	 Description	: 删除邮件
	 Return			: BOOL
	 Param			: nMailID		邮件ID
	 ***********************************************************/
	BOOL DeleteMail(INT64 nMailID);
	/***********************************************************
	 Function		: DeleteOwner
	 Description	: 删除和某个玩家有关的所有邮件（删号时使用）
	 Return			: BOOL
	 Param			: dwOwnerId			玩家Id
	 ***********************************************************/
	BOOL DeleteOwner(DWORD dwOwnerId);
	/***********************************************************
	 Function		: ClearOwner
	 Description	: 清空玩家邮箱中的邮件（暂时只留给测试用）
	 Return			: BOOL
	 Param			: dwOwnerId			玩家Id
	 ***********************************************************/
	BOOL ClearOwner(DWORD dwOwnerId);
	/***********************************************************
	 Function		: ModifyMail
	 Description	: 修改邮件（只允许修改邮件状态和/或邮件内容）
	 Return			: BOOL
	 Param			: pMail			邮件
	 Param			: nMask			修改标志，见KE_MASK_MODIFYMAIL
	 ***********************************************************/
	BOOL ModifyMail(KMAIL_MODINFO* pMail, INT nMask = emKMASK_MODIFYMAIL_STAT | emKMASK_MODIFYMAIL_MAIL);
	/***********************************************************
	 Function		: DeleteExpire
	 Description	: 删除过期邮件
	 Return			: BOOL
	 Param			: 
	 ***********************************************************/
	BOOL DBDeleteExpire();

private:
	virtual BOOL DBThreadCallback(INT nParam, LPVOID pParam);
	virtual BOOL MainThreadCallback(INT nParam, LPVOID pParam);
	BOOL InitMailBoxDatabase(KMySqlDatabase* pDatabase);										// 初始化个人邮箱数据库
	BOOL InitSysMailDatabase(KMySqlDatabase* pDatabase, IKMailCB_FetchSysMail* pCallBack);		// 初始化系统邮件数据库
	BOOL LoadPlayerMailCount(KMySqlDatabase* pDatabase);										// 统计每个人的玩家邮件个数
	BOOL InitMailBoxID(KMySqlDatabase* pDatabase);												// 初始化个人邮箱可用邮件ID
	BOOL InitSysMailID(KMySqlDatabase* pDatabase);												// 初始化系统邮件可用邮件ID
	BOOL DelOutdatedCondMail(KMySqlDatabase* pDatabase);										// 删除过时的系统邮件
	BOOL GetCondMail(KMySqlDatabase* pDatabase, IKMailCB_FetchSysMail* pCallBack);				// 获取系统邮件
	BOOL InitMailID(KMySqlDatabase* pDatabase, LPCSTR pszTable, LPCSTR pszField, INT64& nMailID);
	BOOL DecodeDateTime(KMAIL_DATETIME* pDateTime, LPCSTR pszTime);
	
	BOOL Push_FetchMail(INT nParam, LPVOID pParam);
	BOOL Push_CreateMail(INT nParam, LPVOID pParam);
	BOOL Push_DeleteMail(INT nParam, LPVOID pParam);
	BOOL Push_ModifyMail(INT nParam, LPVOID pParam);
	BOOL Push_DeleteOwner(INT nParam, LPVOID pParam);
	BOOL Push_ClearOwner(INT nParam, LPVOID pParam);
	BOOL Push_AddSysMail(INT nParam, LPVOID pParam);
	
	BOOL Pop_FetchMail(INT nParam, LPVOID pParam);
	BOOL Pop_CreateMail(INT nParam, LPVOID pParam);
	BOOL Pop_DeleteMail(INT nParam, LPVOID pParam);
	BOOL Pop_ModifyMail(INT nParam, LPVOID pParam);
	BOOL Pop_DeleteOwner(INT nParam, LPVOID pParam);
	BOOL Pop_ClearOwner(INT nParam, LPVOID pParam);
	BOOL Pop_AddSysMail(INT nParam, LPVOID pParam);

private:
	static KMailDatabase			ms_inst;
	INT64							m_nNextMailBoxID;		// 个人邮箱表可用的下一个邮件ID
	INT64							m_nNextSysMailID;		// 系统邮件表可用的下一个邮件ID
	IKGcDatabase*					m_pIKGcDataBase;
	CHAR							m_szSQL[2 * 1024];
	INT								m_nMailExpireTime;		// 邮件过期时间，单位（天）

	KIncObjPool<KMySqlResult>			m_poolQueryRes;
	KIncObjPool<KKEY_QUERYMAIL>			m_poolQueryKey;
	KIncObjPool<KKEY_INSERTMAIL>		m_poolInsertKey;
	KIncObjPool<KKEY_MAILID>			m_poolMailIDKey;
	KIncObjPool<KKEY_MODIFYMAIL>		m_poolModifyKey;
	KIncObjPool<KKEY_GUID>				m_poolGuidKey;
	KIncObjPool<KKEY_SYSMAIL>			m_poolSysMailKey;
};

#endif /* GAME_CENTER */
// -------------------------------------------------------------------------

#endif /* __KMAILDATABASE_H__ */
