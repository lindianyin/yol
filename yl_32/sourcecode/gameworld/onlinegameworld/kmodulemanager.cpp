

#include "stdafx.h"
#include "onlinegameworld/krequireinterface.h"
#include "onlinegameworld/kmoduleinterface.h"
#include "kmodulemanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KModuleManager g_cModuleManager;
// -------------------------------------------------------------------------
BOOL KModuleManager::Init()
{

	ITPModule itPModule = m_vecPiModule.begin();
	for (; itPModule != m_vecPiModule.end(); ++itPModule)
	{
		(*itPModule)->OnInit();
	}
	return TRUE;
}

BOOL KModuleManager::Breath()
{
	ITPModule itPModule = m_vecPiModule.begin();
	for (; itPModule != m_vecPiModule.end(); ++itPModule)
	{
		(*itPModule)->Breath();
	}
	return TRUE;
}

BOOL KModuleManager::UnInit()
{
	ITPModule itPModule = m_vecPiModule.begin();
	for (; itPModule != m_vecPiModule.end(); ++itPModule)
	{
		(*itPModule)->OnUnInit();
	}
	return TRUE;
}

BOOL KModuleManager::PreInit()
{
	ZeroStruct(m_anModuleInterface);
	IKModuleInterface** ppModuleInterface = g_piNsBaseRequire->GetModuleList();
	QCONFIRM_RET_FALSE(ppModuleInterface);

	for (INT i = 0; ppModuleInterface[i]; ++i)
	{
		m_vecPiModule.push_back(ppModuleInterface[i]);
		m_anModuleInterface[ppModuleInterface[i]->GetModuleId()] = ppModuleInterface[i];
		BYTE byGcModuleId = ppModuleInterface[i]->GetGcModuleId();
		if (byGcModuleId)
		{
			if (m_anGcModuleInterface[byGcModuleId])
			{
				Q_Error("Gc ModuleÖØ¸´×¢²á£¡");
			}
			else
			{
				m_anGcModuleInterface[byGcModuleId] = ppModuleInterface[i];
			}
		}
	}

	ITPModule itPModule = m_vecPiModule.begin();
	for (; itPModule != m_vecPiModule.end(); ++itPModule)
	{
		(*itPModule)->InitProtocol();
		if (!(*itPModule)->PreInit())
			return FALSE;
	}
	return TRUE;
}
// -------------------------------------------------------------------------
