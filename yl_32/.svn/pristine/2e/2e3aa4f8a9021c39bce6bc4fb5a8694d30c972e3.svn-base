/* -------------------------------------------------------------------------
//	文件名		：	kchatmodule_gc.cpp
//	创建者		：	liuchang
//	创建时间	：	2010-6-24 11:10:59
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#ifdef GAME_CENTER

#include "stdafx.h"
#include "kchatmodule_gc.h"
#include "onlinegamebase/kscriptmanager.h"
#include "kchatapply_gc.h"
#include "kchatprotocolprocess_gc.h"
#include "gclogicbase/kgclogicbasehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IKGcQuery* g_pGcQuery = NULL;
IKGcServer* g_pGcServer = NULL;
IKGcScriptEnv* g_pScriptEnv = NULL;
IKOnlinedPlayerMgr* g_pOnlinePlayerMgr = NULL;

// -------------------------------------------------------------------------
GCMODULE_API IKGC_Module* CreateModuleInstance(IKGcQuery* piQuery)
{
	QCONFIRM_RET_NULL(piQuery);
	g_pGcQuery = piQuery;
	return &g_ChatChannelManager;
}




// -------------------------------------------------------------------------
// 聊天管理
KGC_ChatChannel g_ChatChannelManager;
KGC_ChatChannel::KGC_ChatChannel()
:m_pModuleConnectorFactory(NULL), m_pClientAcceptor(NULL)
{

}

BOOL KGC_ChatChannel::OnInit()
{
	QCONFIRM_RET_FALSE(g_pGcQuery);
	g_GcModuleInitHelper_Log(GetModuleName());

	BOOL hr = g_pGcQuery->QueryInterface(IID_IKG_GCServer, (void**)&g_pGcServer);
	QCONFIRM_RET_NULL(SUCCEEDED(hr));

	hr = g_pGcQuery->QueryInterface(IID_IKG_GCScriptEnv, (void**)&g_pScriptEnv);
	QCONFIRM_RET_NULL(SUCCEEDED(hr));

	g_GcModuleInitHelper_Script(GetModuleName(), g_pScriptEnv);

	hr = g_pGcQuery->QueryInterface(IID_IKG_OnlinePlayerMgr, (void**)&g_pOnlinePlayerMgr);
	QCONFIRM_RET_NULL(SUCCEEDED(hr));

	// 可挪到上层来设置
	SetClientAcceptor(&g_ChatProcessAcceptor);
	SetConnectorFactory(&g_cChatModuleConnectorFactory);

	QCONFIRM_RET_FALSE(m_pClientAcceptor && m_pModuleConnectorFactory);

	g_pGcServer->RegisterGsProcessor(m_pClientAcceptor);

	SetProcessorFactory(&g_cChatChannelProcessorFactory_GC);

	if (!KChatChannel::Init())
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	
	for (KT_CHANNEL_INFO::iterator it = m_mapChannel.begin(); it != m_mapChannel.end(); ++it)
	{
		INT nChannelId = it->first;
		ScriptSafe cSafeScript = g_pScriptEnv->GetScriptManager().GetSafeScript();
		VERIFY(cSafeScript->CallTableFunction("Chat", "GetDispathInterfaceId", 1, "d", nChannelId));
		if (!cSafeScript->IsNumber(-1))
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		KE_GC_MODULE_ID emModule = (KE_GC_MODULE_ID)cSafeScript->GetInt(-1);
		_ASSERT(m_pModuleConnectorFactory);
		if (emModule != emKGCMODULE_NONE && m_pModuleConnectorFactory)
		{
			IKProcessConnector* pConnector = m_pModuleConnectorFactory->GetModuleChatProcess(emModule);
			if (pConnector)
			{
				IKInterfaceConnector* pInterface = g_pGcServer->RegisterGcConnector((BYTE)emModule, pConnector, TRUE);
				_ASSERT(pInterface);
				it->second.pInterface = pInterface;
			}
		}

	}

	return TRUE;
}


BOOL KGC_ChatChannel::SetConnectorFactory(IKModuleConnectorFactory* pModuleConnectorFactory)
{
	_ASSERT(pModuleConnectorFactory);
	QCONFIRM_RET_FALSE(!m_pModuleConnectorFactory);
	m_pModuleConnectorFactory = pModuleConnectorFactory;

	return TRUE;
}

BOOL KGC_ChatChannel::SetClientAcceptor(IKProcessServer* pClientAcceptor)
{
	_ASSERT(pClientAcceptor);
	QCONFIRM_RET_FALSE(!m_pClientAcceptor);
	m_pClientAcceptor = pClientAcceptor;

	return TRUE;
}
#endif /* GAME_CENTER */
