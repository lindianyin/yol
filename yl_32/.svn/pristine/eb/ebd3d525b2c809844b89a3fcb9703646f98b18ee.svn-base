

#include "stdafx.h"
#include "config/kcharacterdef.h"
#include "kvarsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KVarSetting g_cCharacterVarSetting;
// -------------------------------------------------------------------------
KVarSetting::KVarSetting()
{
	m_pTaskProp = new TASK_PROP[65536];
	ZeroMemory(m_pTaskProp, sizeof(TASK_PROP) * 65536);
	//LoadBaseVardef();
}

KVarSetting::~KVarSetting()
{
	delete[] m_pTaskProp;
}

BOOL KVarSetting::LoadTaskDef(LPCSTR szFileName)
{
	ITabFile* pTabFile = g_OpenTabFile(szFileName);
	QCONFIRM_RET_FALSE(pTabFile);
	INT nRow = pTabFile->GetHeight();
	INT nValue;
	INT i = 2;
	for (; i <= nRow; i++)
	{
		// 读ID
		INT nGroupId, nIdFirst, nIdLast, nOverDate;
		pTabFile->GetInteger(i, "VarGroup", -1, &nGroupId); 
		pTabFile->GetInteger(i, "VarFirst", -1, &nIdFirst);
		pTabFile->GetInteger(i, "VarLast", -1, &nIdLast);
		QCONFIRM_CONTINUE(nGroupId >= KD_MIN_EXTERN_VARGROUP && nGroupId < 256
			&& nIdFirst <= nIdLast && nIdFirst >=0 && nIdLast < 256);
		// 读属性
		TASK_PROP prop = { 0 };
		prop.m_Flag |= emKVARFLAG_DEF;
		pTabFile->GetInteger(i, "SyncSelf", 0, &nValue);
		if (nValue == 1)
			prop.m_Flag |= emKVARFLAG_SYNC_SELF;
		pTabFile->GetInteger(i, "SyncOther", 0, &nValue);
		if (nValue == 1)
			prop.m_Flag |= emKVARFLAG_SYNC_OTHER;
		pTabFile->GetInteger(i, "ClientFlag", 0, &nValue);
		if (nValue == 1)
			prop.m_Flag |= emKVARFLAG_CLIENT;
		pTabFile->GetInteger(i, "ResetDate", 0, &nOverDate);
		prop.m_dwReSetDate = (DWORD)nOverDate;

		WORD wGroupMask = nGroupId << 8;
		for (INT nId = nIdFirst; nId <= nIdLast; ++nId)
		{
			m_pTaskProp[wGroupMask | nId] = prop;
			VAR_KEY vKey = { (BYTE)nGroupId, (BYTE)(nId) };
			if (prop.m_Flag & emKVARFLAG_SYNC_SELF)
				m_vecSyncSelfTask.push_back(vKey);
			if (prop.m_Flag & emKVARFLAG_SYNC_OTHER)
				m_vecSyncOtherTask.push_back(vKey);
		}
	}

	SAFE_RELEASE(pTabFile);
	return TRUE;
}

BOOL KVarSetting::LoadBaseVardef()
{
	// 设置自动同步的变量
	for (UINT i = 0; i < countof(csByteVarSyncSelf); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_BYTE << 8) | csByteVarSyncSelf[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_SELF;
	}
	for (UINT i = 0; i < countof(csByteVarSyncOther); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_BYTE << 8) | csByteVarSyncOther[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_OTHER;
	}

	for (UINT i = 0; i < countof(csWordVarSyncSelf); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_WORD << 8) | csWordVarSyncSelf[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_SELF;
	}
	for (UINT i = 0; i < countof(csWordVarSyncOther); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_WORD << 8) | csWordVarSyncOther[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_OTHER;
	}

	for (UINT i = 0; i < countof(csDWordVarSyncSelf); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_DWORD << 8) | csDWordVarSyncSelf[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_SELF;
	}
	for (UINT i = 0; i < countof(csDWordVarSyncOther); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_DWORD << 8) | csDWordVarSyncOther[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_OTHER;
	}
	
	for (UINT i = 0; i < countof(csMapVarSyncSelf); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_MAP << 8) | csMapVarSyncSelf[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_SELF;
		g_cCharacterVarSetting.SetTaskSyncSelf(emKVARGROUP_CHARACTER_MAP, (BYTE)csMapVarSyncSelf[i]);
	}
	for (UINT i = 0; i < countof(csMapVarSyncOther); ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_MAP << 8) | csMapVarSyncOther[i];
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_SYNC_OTHER;
		g_cCharacterVarSetting.SetTaskSyncOther(emKVARGROUP_CHARACTER_MAP, (BYTE)csMapVarSyncOther[i]);
	}


	// 把程序枚举的变量变为有定义
	for (INT i = 0; i < emKCHARACTER_TASK_BYTE_MAX; ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_BYTE << 8) | i;
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_DEF;
	}
	for (INT i = 0; i < emKCHARACTER_TASK_WORD_MAX; ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_WORD << 8) | i;
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_DEF;
	}
	for (INT i = 0; i < emKCHARACTER_TASK_DWORD_MAX; ++i)
	{
		WORD wVarKey = (emKVARGROUP_CHARACTER_DWORD << 8) | i;
		m_pTaskProp[wVarKey].m_Flag |= emKVARFLAG_DEF;
	}
	return TRUE;
}

// -------------------------------------------------------------------------
