#include "stdafx.h"
#include "targetsetting.h"
#include "targetdef.h"
#include "Runtime/qstring.h"

TargetSetting g_cTargetSetting;
TargetSetting::TargetSetting()
{

}

TargetSetting::~TargetSetting()
{

}

BOOL TargetSetting::Init()
{
	ReadTargetTemplate();
	return TRUE;
}

BOOL TargetSetting::UnInit()
{
	ClearTargetCfg();
	return TRUE;
}

BOOL TargetSetting::ReadTargetTemplate()
{
	m_MapTargetCfg.clear();
	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(TARGET_FILE_PATH);

	CHAR szFilePath[_MAX_PATH];
	QString szFileTemp;
	ITabFile* pFile = NULL;
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 3; i <= nFileHeight; i++)  // 第3行
	{
		TargetCfg* targetTemplate = new TargetCfg();
		tabFile->GetInteger(i, "id",			0,	&targetTemplate->nId);
		tabFile->GetInteger(i, "awardId",		0,	&targetTemplate->nAwardId);
		tabFile->GetInteger(i, "autoGive",		0,	&targetTemplate->nAutoGive);
		tabFile->GetString(i, "conditionPath",	"", szFilePath, _MAX_PATH);

		szFileTemp = TARGET_DAY_FOLD;
		szFileTemp = szFileTemp + szFilePath;
		pFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pFile);
		for (int j = 3; j <= pFile->GetHeight(); j++)
		{
			TargetDayCfg* targetDay = new TargetDayCfg();
			pFile->GetInteger(j, "id",			0,	&targetDay->nId);
			pFile->GetInteger(j, "awardId",		0,	&targetDay->nAwardId);
			pFile->GetInteger(j, "finishCount",	0,	&targetDay->nFinishCount);
			pFile->GetInteger(j, "autoGive",	0,	&targetDay->nAutoGive);

			targetTemplate->mapTargetDay.insert(std::make_pair(targetDay->nId, targetDay));
		}

		m_MapTargetCfg.insert(std::make_pair(targetTemplate->nId, targetTemplate));
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL TargetSetting::ClearTargetCfg()
{
	for (std::map<INT, TargetCfg*>::iterator it = m_MapTargetCfg.begin(); it != m_MapTargetCfg.end(); ++it)
	{
		TargetCfg* pDayMap = it->second;
		for (std::map<INT, TargetDayCfg*>::iterator it_day = pDayMap->mapTargetDay.begin(); it_day != pDayMap->mapTargetDay.end(); ++it_day)
		{
			SAFE_DELETE(it_day->second);
		}
		pDayMap->mapTargetDay.clear();
		SAFE_DELETE(it->second);
	}
	m_MapTargetCfg.clear();

	return TRUE;
}

TargetCfg* TargetSetting::GetTargetCfg(INT nDay)
{
	if (m_MapTargetCfg.find(nDay) != m_MapTargetCfg.end())
		return m_MapTargetCfg[nDay];

	return NULL;
}

TargetDayCfg* TargetSetting::GetTargetDayCfg(INT nDay, INT nId)
{
	if (m_MapTargetCfg.find(nDay) != m_MapTargetCfg.end())
	{
		std::map<INT, TargetDayCfg*> pTargetCfg = m_MapTargetCfg[nDay]->mapTargetDay;
		if (pTargetCfg.find(nId) != pTargetCfg.end())
			return pTargetCfg[nId];
	}
	return NULL;
}

//奖励数量()
INT TargetSetting::GetTargetAwardCount()
{
	INT count = 0;
	for (std::map<INT, TargetCfg*>::iterator it = m_MapTargetCfg.begin(); it != m_MapTargetCfg.end(); ++it)
	{
		
		TargetCfg* pDayMap = it->second;
		count += pDayMap->mapTargetDay.size();
	}
	return count;
}
