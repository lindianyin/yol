/* -------------------------------------------------------------------------
//	文件名		：	kchatmodule_gc.h
//	创建者		：	liuchang
//	创建时间	：	2010-6-24 11:10:24
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KCHATMODULE_GC_H__
#define __KCHATMODULE_GC_H__

// -------------------------------------------------------------------------

#ifdef GAME_CENTER

#include "gcmodule/chat/kchatchannel.h"
#include "serverbase/kgcmoduleinterface.h"
#include "serverbase/kinterfacedef.h"
#include "gclogicbase/kgclogicextinterface.h"
#include "serverbase/kgcmoduledef.h"


extern "C"
{
	GCMODULE_API IKGC_Module* CreateModuleInstance(IKGcQuery* piQuery);
};

extern IKGcQuery* g_pGcQuery;


class IKModuleConnectorFactory
{
public:
	virtual IKProcessConnector* GetModuleChatProcess(KE_GC_MODULE_ID emModuleId) = 0;
};

// -------------------------------------------------------------------------
// 聊天管理
class KGC_ChatChannel : public KChatChannel, public IKGC_Module
{
public:
	KGC_ChatChannel();

public:
	virtual LPCSTR GetModuleName() { return "Chat"; };
	virtual BOOL Loop() { return TRUE; };
	virtual BOOL Breath() { return TRUE; };
	virtual BOOL OnInit();
	virtual BOOL OnStartUp() { return TRUE; };
	virtual BOOL OnUninit() { return TRUE; };

public:
	BOOL SetConnectorFactory(IKModuleConnectorFactory* pModuleConnectorFactory);
	BOOL SetClientAcceptor(IKProcessServer* pClientAcceptor);

private:
	IKModuleConnectorFactory* m_pModuleConnectorFactory;	// 处理主动连接的其它GC模块的类的工厂
	IKProcessServer* m_pClientAcceptor;	// 处理被GS连接与消息
};

extern KGC_ChatChannel g_ChatChannelManager;
extern IKGcServer* g_pGcServer;
extern IKGcScriptEnv* g_pScriptEnv;
extern IKOnlinedPlayerMgr* g_pOnlinePlayerMgr;

#endif /* GAME_CENTER */


// -------------------------------------------------------------------------

#endif /* __KCHATMODULE_GC_H__ */
