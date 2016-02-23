#include "stdafx.h"
#include "opensesetting.h"
#include "opensedef.h"
#include "Runtime/qstring.h"

OpenseSetting g_cOpenseSetting;
OpenseSetting::OpenseSetting()
{

}

OpenseSetting::~OpenseSetting()
{

}

BOOL OpenseSetting::Init()
{
	ReadOpenseTemplate();
	return TRUE;
}

BOOL OpenseSetting::UnInit()
{
	ClearOpenseCfg();
	return TRUE;
}

BOOL OpenseSetting::ReadOpenseTemplate()
{
	m_MapOpenseCfg.clear();
	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(OPENSE_FILE_PATH);

	CHAR szFilePath[_MAX_PATH];
	QString szFileTemp;
	ITabFile* pFile = NULL;
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 3; i <= nFileHeight; i++)  // 第3行
	{
		OpenseCfg* openseTemplate = new OpenseCfg();
		tabFile->GetInteger(i, "id", 0, &openseTemplate->nId);
		tabFile->GetInteger(i, "awardId", 0, &openseTemplate->nAwardId);
		tabFile->GetInteger(i, "autoGive", 0, &openseTemplate->nAutoGive);
		tabFile->GetString(i, "conditionPath", "", szFilePath, _MAX_PATH);

		szFileTemp = OPENSE_DAY_FOLD;
		szFileTemp = szFileTemp + szFilePath;
		pFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pFile);
		for (int j = 3; j <= pFile->GetHeight(); j++)
		{
			OpenseDayCfg* openseDay = new OpenseDayCfg();
			pFile->GetInteger(j, "id", 0, &openseDay->nId);
			pFile->GetInteger(j, "awardId", 0, &openseDay->nAwardId);
			pFile->GetInteger(j, "finishCount", 0, &openseDay->nFinishCount);
			pFile->GetInteger(j, "autoGive", 0, &openseDay->nAutoGive);

			openseTemplate->mapOpenseDay.insert(std::make_pair(openseDay->nId, openseDay));
		}
		m_MapOpenseCfg.insert(std::make_pair(openseTemplate->nId, openseTemplate));
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL OpenseSetting::ClearOpenseCfg()
{
	for (std::map<INT, OpenseCfg*>::iterator it = m_MapOpenseCfg.begin(); it != m_MapOpenseCfg.end(); ++it)
	{
		OpenseCfg* pDayMap = it->second;
		for (std::map<INT, OpenseDayCfg*>::iterator it_day = pDayMap->mapOpenseDay.begin(); it_day != pDayMap->mapOpenseDay.end(); ++it_day)
		{
			SAFE_DELETE(it_day->second);
		}
		pDayMap->mapOpenseDay.clear();
		SAFE_DELETE(it->second);
	}
	m_MapOpenseCfg.clear();
	return TRUE;
}

OpenseCfg* OpenseSetting::GetOpenseCfg(INT nDay)
{
	if (m_MapOpenseCfg.find(nDay) != m_MapOpenseCfg.end())
		return m_MapOpenseCfg[nDay];

	return NULL;
}

OpenseDayCfg* OpenseSetting::GetOpenseDayCfg(INT nDay, INT nId)
{
	if (m_MapOpenseCfg.find(nDay) != m_MapOpenseCfg.end())
	{
		std::map<INT, OpenseDayCfg*> pOpenseCfg = m_MapOpenseCfg[nDay]->mapOpenseDay;
		if (pOpenseCfg.find(nId) != pOpenseCfg.end())
			return pOpenseCfg[nId];
	}
	return NULL;
}

//奖励数量()
INT OpenseSetting::GetOpenseAwardCount()
{
	INT count = 0;
	for (std::map<INT, OpenseCfg*>::iterator it = m_MapOpenseCfg.begin(); it != m_MapOpenseCfg.end(); ++it)
	{
		OpenseCfg* pDayMap = it->second;
		count += pDayMap->mapOpenseDay.size();
	}
	return count;
}
