#include "stdafx.h"
#include "vipsetting.h"
#include "vipdef.h"
#include "Runtime/qstring.h"

VipSetting g_cVipSetting;
VipSetting::VipSetting()
{

}

VipSetting::~VipSetting()
{

}

BOOL VipSetting::Init()
{
	ReadVipGifts();
	ReadVipAttribute();
	return TRUE;
}

BOOL VipSetting::UnInit()
{
	ClearVipGifts();
	ClearVipAttribute();
	return TRUE;
}

VipOverview* VipSetting::GetVipAttribute(INT nLevel)
{
	if (m_MapVipGifts.find(nLevel) != m_MapVipGifts.end())
		return m_MapVipGifts[nLevel];

	return NULL;
}

BOOL VipSetting::GetVipGift(INT nLevel, std::map<INT, VipGift*>& mapVipGifts)
{
	if (m_MapVipGifts.find(nLevel) != m_MapVipGifts.end())
	{
		for (std::map<INT, VipGift*>::iterator it = m_MapVipGifts[nLevel]->mapVipGift.begin(); it != m_MapVipGifts[nLevel]->mapVipGift.end(); ++it)
		{
			mapVipGifts.insert(std::make_pair(it->first, it->second));
		}
		return TRUE;
	}

	return FALSE;
}

VipAttributeInfo* VipSetting::GetVipAttribute(INT nId, INT nLevel)
{
	if (m_MapVipAttribute.find(nId) != m_MapVipAttribute.end())
	{
		VipAttribute* pInfo = m_MapVipAttribute[nId];
		if (pInfo->mapVipInfo.find(nLevel) != pInfo->mapVipInfo.end())
			return pInfo->mapVipInfo[nLevel];
	}

	return NULL;
}

LPCSTR VipSetting::GetModuleName(INT nId)
{
	if (m_MapVipAttribute.find(nId) != m_MapVipAttribute.end())
	{
		return m_MapVipAttribute[nId]->szModuleName;
	}

	return NULL;
}


BOOL VipSetting::ReadVipGifts()
{
	ClearVipGifts();

	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(WELFARE_OVERVIEW_PATH);

	CHAR szFilePath[_MAX_PATH];
	QString szFileTemp;
	ITabFile* pFile = NULL;
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 3; i <= nFileHeight; i++)  // 第3行
	{
		INT nLevel = 0;
		INT nNeedCoins = 0;
		VipOverview* pVipoverview = new VipOverview();
		tabFile->GetInteger(i, "level",			0,	&nLevel);
		tabFile->GetInteger(i, "needCoins",		0,	&pVipoverview->nNeedCoins);
		tabFile->GetInteger(i, "awardId",		0,	&pVipoverview->nAwardId);
		tabFile->GetString(i, "giftsPath",	"", szFilePath, _MAX_PATH);

		szFileTemp = WELFARE_OVERVIEW_FOLD;
		szFileTemp = szFileTemp + szFilePath;
		pFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pFile);
		for (int j = 3; j <= pFile->GetHeight(); j++)
		{
			INT nId = 0;
			VipGift* pGift = new VipGift();
			pFile->GetInteger(j, "id",			0,	&nId);
			pFile->GetInteger(j, "Genre",		0,	&pGift->nGenre);
			pFile->GetInteger(j, "DetailType",	0,	&pGift->nDetail);
			pFile->GetInteger(j, "ParticularType",	0,	&pGift->nParticular);
			pFile->GetInteger(j, "Level",	0,	&pGift->nLevel);
			pFile->GetInteger(j, "awardId",	0,	&pGift->nAwardId);

			pVipoverview->mapVipGift.insert(std::make_pair(nId, pGift));
		}
		m_MapVipGifts.insert(std::make_pair(nLevel, pVipoverview));
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL VipSetting::ClearVipGifts()
{

	for (std::map<INT, VipOverview*>::iterator it = m_MapVipGifts.begin(); it != m_MapVipGifts.end(); ++it)
	{
		VipOverview* pInfo = it->second;
		for (std::map<INT, VipGift*>::iterator it_info = pInfo->mapVipGift.begin(); it_info != pInfo->mapVipGift.end(); ++it_info)
		{
			SAFE_DELETE(it_info->second);
		}
		pInfo->mapVipGift.clear();
		SAFE_DELETE(it->second);
	}
	m_MapVipGifts.clear();

	return TRUE;
}

BOOL VipSetting::ReadVipAttribute()
{
	ClearVipAttribute();

	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(WELFARE_CONSTRAST_PATH);

	CHAR szFilePath[_MAX_PATH];
	QString szFileTemp;
	ITabFile* pFile = NULL;
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 3; i <= nFileHeight; i++)  // 第3行
	{
		INT nId = 0;
		VipAttribute* pVipAttribute = new VipAttribute();
		tabFile->GetInteger(i, "id",			0,	&nId);
		tabFile->GetString(i, "moduleName",	"", pVipAttribute->szModuleName, countof(pVipAttribute->szModuleName));
		tabFile->GetString(i, "attributePath",	"", szFilePath, _MAX_PATH);

		szFileTemp = WELFARE_CONSTRAST_FOLD;
		szFileTemp = szFileTemp + szFilePath;
		pFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pFile);
		for (int j = 3; j <= pFile->GetHeight(); j++)
		{
			VipAttributeInfo* pVipInfo = new VipAttributeInfo();
			pFile->GetInteger(j, "level",			0,	&pVipInfo->nLevel);
			pFile->GetInteger(j, "num",		0,	&pVipInfo->nNum);
			pFile->GetInteger(j, "type",	0,	&pVipInfo->nType);
			pFile->GetInteger(j, "isOpen",	0,	&pVipInfo->nIsOpen);
			pFile->GetInteger(j, "rate",	0,	&pVipInfo->nRate);

			pVipAttribute->mapVipInfo.insert(std::make_pair(pVipInfo->nLevel, pVipInfo));
		}

		m_MapVipAttribute.insert(std::make_pair(nId, pVipAttribute));
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL VipSetting::ClearVipAttribute()
{
	for (std::map<INT, VipAttribute*>::iterator it = m_MapVipAttribute.begin(); it != m_MapVipAttribute.end(); ++it)
	{
		VipAttribute* pInfo = it->second;
		for (std::map<INT, VipAttributeInfo*>::iterator it_info = pInfo->mapVipInfo.begin(); it_info != pInfo->mapVipInfo.end(); ++it_info)
		{
			SAFE_DELETE(it_info->second);
		}
		pInfo->mapVipInfo.clear();
		SAFE_DELETE(it->second);
	}
	m_MapVipAttribute.clear();

	return TRUE;
}

