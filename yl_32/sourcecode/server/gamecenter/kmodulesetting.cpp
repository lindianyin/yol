/* -------------------------------------------------------------------------
//	文件名		：	kmodulesetting.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/9/6 17:55:44
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kkeysharemem.h"
#include "kmodulesetting.h"
#include "serverbase/kgcmoduledef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KModuleSetting::KModuleSetting()
{
	//m_pShareMemData = NULL;
}

KModuleSetting* KModuleSetting::Inst()
{
	static KModuleSetting _sInst;
	return &_sInst;
}

BOOL KModuleSetting::SetModuleInfo(INT nModuleNum, KGC_MODULE_INFO aModuleInfo[])
{
	QCONFIRM_RET_FALSE(nModuleNum <= countof(m_aModuleInfo));
	ZeroStruct(m_aModuleInfo);
	m_nModuleCount = nModuleNum;
	for (INT i = 0; i < nModuleNum; ++i)
	{
		m_aModuleInfo[aModuleInfo[i].byModuleId] = aModuleInfo[i];
	}
	return TRUE;
}

BYTE KModuleSetting::GetLoaderId( BYTE byModuleId )
{
	QCONFIRM_RET_FALSE(byModuleId > 0);
	return (BYTE)m_aModuleInfo[byModuleId].nLoaderId;
}

LPCSTR KModuleSetting::GetModuleName( BYTE byModuleId )
{
	QCONFIRM_RET_FALSE(byModuleId > 0/* && byModuleId < countof(g_aszGcModuleNames)*/);
	return g_aszGcModuleNames[byModuleId];
}

LPCSTR KModuleSetting::GetModulePath( BYTE byModuleId )
{
	QCONFIRM_RET_FALSE(byModuleId > 0 /*&& byModuleId < countof(g_aszGcModuleFiles)*/);
	return g_aszGcModuleFiles[byModuleId];
}

INT KModuleSetting::GetModuleNum()
{
	return m_nModuleCount;
}

INT KModuleSetting::Init()
{
	m_nDevModeFlag = 0;
	m_nModuleCount = 0;
	ZeroStruct(m_aModuleInfo);
	return TRUE;
}

INT KModuleSetting::GetGsPort( BYTE byLoaderId )
{
	for (INT i = 0; i < countof(m_aModuleInfo); ++i)
	{
		if (m_aModuleInfo[i].nLoaderId == byLoaderId)
		{
			return m_aModuleInfo[i].uGsPort;
		}
	}
	return 0;
}

// -------------------------------------------------------------------------
