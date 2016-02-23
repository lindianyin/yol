/* -------------------------------------------------------------------------
//	文件名		：	kgc_mailmodule.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/17 14:39:41
//	功能描述	：	邮件系统模块
//
// -----------------------------------------------------------------------*/
#ifndef __KGC_MAILMODULE_H__
#define __KGC_MAILMODULE_H__

// -------------------------------------------------------------------------
#include "serverbase/kgcmoduleinterface.h"
interface IKOnlinedPlayerMgr;

class KGC_MailModule : public IKGC_Module, public IKProcessServer
{
public:
	KGC_MailModule(): m_pIKGcQuery(NULL), 
					  m_pIKGcServer(NULL),
					  m_pIKOnlinePlayerMgr(NULL)
	{}
	~KGC_MailModule(){}
	
	VOID Init(IKGcQuery* piGcQuery) { m_pIKGcQuery = piGcQuery; }
	static KGC_MailModule* Instance() {return &ms_inst;}

public:
	//IKGC_Module
	// 获取模块名字
	LPCSTR GetModuleName();
	// 每循环调用
	BOOL Loop();
	// 每帧调用
	BOOL Breath();
	//  初始化调用
	BOOL OnInit();
	// 初始化完毕进入运行状态时调用
	BOOL OnStartUp();
	//反初始化调用前调用, 关闭
	BOOL OnShutDown();
	// 反初始化调用
	BOOL OnUninit();

	//IKProcessServer
	BOOL OnConnectClose(INT nConnectId) { return TRUE; }
	BOOL OnConnected(INT nConnectId) { return TRUE; }
	BOOL ProcessData(INT nConnectId, LPBYTE pData, UINT uSize);

	IKGcQuery* GetIKGcQuery() { return m_pIKGcQuery; }
	IKGcServer* GetIKGcServer() { return m_pIKGcServer; }
	IKOnlinedPlayerMgr* GetOnlinePlayerMgr() { return m_pIKOnlinePlayerMgr; }

private:
	static KGC_MailModule ms_inst;

private:
	IKGcQuery*  m_pIKGcQuery;
	IKGcServer* m_pIKGcServer;
	IKOnlinedPlayerMgr*  m_pIKOnlinePlayerMgr;
};

// -------------------------------------------------------------------------

#endif /* __KGC_MAILMODULE_H__ */
