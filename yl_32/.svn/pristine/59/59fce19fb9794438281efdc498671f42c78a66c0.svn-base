/* -------------------------------------------------------------------------
//	文件名		：	kcondmailmanager.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/17 14:14:47
//	功能描述	：	系统邮件管理器，用于管理要发送给指定条件玩家的系统邮件
//
// -----------------------------------------------------------------------*/
#ifndef __KCONDMAILMANAGER_H__
#define __KCONDMAILMANAGER_H__

// -------------------------------------------------------------------------
#ifdef GAME_CENTER

#include "kmaildatabase.h"
#include "gclogicbase/kgcplayercommon.h"
#include <map>

class KCondMailManager : public IKMailCB_FetchSysMail
{
public:
	struct KSYSMAIL
	{
		KMAIL_DATETIME	sSendTime;
		INT				nCondLen;
		INT				nMailLen;
		CHAR			szCond[KD_MAIL_MAX_COND_LENGTH];
		CHAR			szMail[KD_MAIL_MAX_SIZE];
	};
	typedef BOOL (KCondMailManager::*fnCheckCondition)(KRole*, CONST KMAIL_CONDHEAD*, INT);
	struct KCOND_INFO
	{
		INT					nSize;
		fnCheckCondition	pfnCheckCondition;
	};
public:
	KCondMailManager();
	~KCondMailManager();
	static KCondMailManager *Instance() {return &ms_inst;}
	BOOL AddMail(CONST KCONDMAIL_INFO* pSysMail);		// 增加系统邮件
	BOOL PostMail(DWORD dwPlayerId);	// 投递系统邮件（如果玩家符合条件）
	BOOL NotifyMail(DWORD dwPlayerId);// 通知客户端有新邮件

private:
	virtual BOOL FetchSysMail(CONST KCONDMAIL_INFO* pSysMail);
	BOOL CheckCondition(KRole* pPlayer, CONST KMAIL_CONDHEAD* pCond, INT nCondLen);
	// 投递邮件，返回新的邮件ID，返回0表示失败
	INT64 PostMail(DWORD dwPlayerId, CONST KSYSMAIL* pMail);
	// 获取新邮件起始位置
	BOOL GetNewMailPosition(INT64 nLastID, std::map<INT64, KSYSMAIL>* pMap, std::map<INT64, KSYSMAIL>::const_iterator &it);
private:
	static KCondMailManager		ms_inst;
	std::map<INT64, KSYSMAIL>	m_mapMail;
};


#endif /* GAME_CENTER */
// -------------------------------------------------------------------------

#endif /* __KCONDMAILMANAGER_H__ */
