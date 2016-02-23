#include "stdafx.h"
#include "titlesetting.h"
#include "titledef.h"

TitleSetting g_cTitleSetting;
TitleSetting::TitleSetting()
{

}

TitleSetting::~TitleSetting()
{

}

BOOL TitleSetting::Init()
{
	ReadTitleTemplate();
	return TRUE;
}

BOOL TitleSetting::UnInit()
{
	for (TITLE_TEMPLATE_VECTOR::iterator it = m_vecTitleTemplate.begin(); it != m_vecTitleTemplate.end(); ++it)
	{
		SAFE_DELETE(*it);
		*it = NULL;
	}
	m_vecTitleTemplate.clear();

	return TRUE;
}

BOOL TitleSetting::ReadTitleTemplate()
{
	m_vecTitleTemplate.clear();
	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(TITLE_FILE_PATH);
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 3; i <= nFileHeight; i++)  // µÚ3ÐÐ
	{
		TitleTemplate* titleTemplate = new TitleTemplate();
		tabFile->GetInteger(i, "Id",			0, &titleTemplate->nId);
		tabFile->GetInteger(i, "getWay",		0, &titleTemplate->nGetWay);
		tabFile->GetInteger(i, "type",			0, &titleTemplate->nType);
		tabFile->GetInteger(i, "mutex",			0, &titleTemplate->nMutex);
		tabFile->GetInteger(i, "minLevel",		0, &titleTemplate->nMinLevel);
		tabFile->GetInteger(i, "maxLevel",		0, &titleTemplate->nMaxLevel);
		tabFile->GetInteger(i, "effectiveTime", 0, &titleTemplate->nEffectiveTime);
		tabFile->GetInteger(i, "buffId",		0, &titleTemplate->nBuffId);

		m_vecTitleTemplate.push_back(titleTemplate);
	}

	return TRUE;

EXIT0:
	return FALSE;
}

TitleTemplate* TitleSetting::GetTitleTemplate(INT nGetWay, INT nLevel, INT nType)
{
	for (INT i = 0; i < m_vecTitleTemplate.size(); ++i)
	{
		if (m_vecTitleTemplate[i]->nGetWay == nGetWay && m_vecTitleTemplate[i]->nType == nType)
		{
			if (m_vecTitleTemplate[i]->nMinLevel <= nLevel && nLevel <= m_vecTitleTemplate[i]->nMaxLevel)
				return m_vecTitleTemplate[i];
		}
	}
	return NULL;
}

TitleTemplate* TitleSetting::GetTitleTemplate(INT nGetWay, INT nType)
{
	for (INT i = 0; i < m_vecTitleTemplate.size(); ++i)
	{
		if (m_vecTitleTemplate[i]->nGetWay == nGetWay && m_vecTitleTemplate[i]->nType == nType)
		{
			return m_vecTitleTemplate[i];
		}
	}
	return NULL;
}

TitleTemplate* TitleSetting::GetTitleTemplate(INT nId)
{
	for (INT i = 0; i < m_vecTitleTemplate.size(); ++i)
	{
		if (m_vecTitleTemplate[i]->nId == nId)
		{
			return m_vecTitleTemplate[i];
		}
	}
	return NULL;
}