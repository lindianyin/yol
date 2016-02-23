
#include "stdafx.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "kitemsetting.h"
#include "kmagicattribmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

BOOL KItemSetting::Init()
{
	if (!InitCdType())
		return FALSE;

	if (!InitEquipScore())
		return FALSE;

	if (!InitStoneScore())
		return FALSE;

	return TRUE;
}

BOOL KItemSetting::InitCdType()
{
	QTabFile cTabFile;
	if (!cTabFile.Load(KD_STR_F_ITEM_CDTYPE))
		return	FALSE;

	INT nCount = cTabFile.GetHeight() - 1;
	for (INT i = 0; i < nCount; ++i)
	{
		INT nID = 0;
		INT nTime = 0UL;
		cTabFile.GetInteger(i + 2,	"ID",	0,	&nID);
		cTabFile.GetInteger(i + 2,	"Time",	0,	&nTime);
		_ASSERT(!m_mapCdType.count(nID));
		m_mapCdType[nID] = (DWORD)nTime;
	}
	return	TRUE;
}

BOOL KItemSetting::InitEquipScore()
{
	BOOL bRet = FALSE;
	BOOL bResult = FALSE;
	INT nCount = 0;
	ITabFile* pTabFile = g_OpenTabFile(KD_STR_F_EQUIP_SCORE);
	LOG_PROCESS_ERROR(pTabFile);

	nCount = pTabFile->GetHeight();
	for (INT i = 3; i <= nCount; ++i)
	{
		CHAR szBuf[MAX_NAME_LEN] = "";

		bRet = pTabFile->GetString(i, "Attrib", "", szBuf, countof(szBuf));
		LOG_PROCESS_ERROR(bRet);

		INT nAttrib = g_cMagicAttribMgr.GetIdFormString(szBuf);
		LOG_PROCESS_ERROR(nAttrib > 0);

		INT nScore = 0;
		bRet = pTabFile->GetInteger(i, "Score", 0, &nScore);
		LOG_PROCESS_ERROR(bRet);

		m_mapEquipScore[nAttrib] = nScore;
	}

EXIT0:
	return TRUE;
}

INT KItemSetting::GetAttribScore(INT nAttrib)
{
	KMAP_EQUIP_SCORE::iterator it = m_mapEquipScore.find(nAttrib);

	if(it != m_mapEquipScore.end())
	{
		return it->second;
	}

	return 0;
}

BOOL KItemSetting::InitStoneScore()
{
	BOOL bRet = FALSE;
	BOOL bResult = FALSE;
	ITabFile* pTabFile = g_OpenTabFile(KD_STR_F_STONE_SCORE);
	INT nCount = 0;
	LOG_PROCESS_ERROR(pTabFile);

	nCount = pTabFile->GetHeight();
	for (INT i = 3; i <= nCount; ++i)
	{
		CHAR szBuf[MAX_NAME_LEN] = "";

		bRet = pTabFile->GetString(i, "Attrib", "", szBuf, countof(szBuf));
		LOG_PROCESS_ERROR(bRet);

		INT nAttrib = g_cMagicAttribMgr.GetIdFormString(szBuf);
		LOG_PROCESS_ERROR(nAttrib > 0);

		INT nScore = 0;
		bRet = pTabFile->GetInteger(i, "Score", 0, &nScore);
		LOG_PROCESS_ERROR(bRet);

		m_mapStoneScore[nAttrib] = nScore;
	}

EXIT0:
	return TRUE;
}

INT KItemSetting::GetStoneAttribScore(INT nAttrib)
{
	KMAP_EQUIP_SCORE::iterator it = m_mapStoneScore.find(nAttrib);

	if(it != m_mapStoneScore.end())
	{
		return it->second;
	}

	return 0;
}

// -------------------------------------------------------------------------
