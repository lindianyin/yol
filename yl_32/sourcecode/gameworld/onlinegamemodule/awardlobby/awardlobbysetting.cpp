#include "stdafx.h"
#include "awardlobbysetting.h"
#include "awardlobbydef.h"
#include "Runtime/qstring.h"
#include "trade/kbuysellsetting.h"

AwardLobbySetting g_cAwardLobbySetting;
AwardLobbySetting::AwardLobbySetting()
{

}

AwardLobbySetting::~AwardLobbySetting()
{

}

BOOL AwardLobbySetting::Init()
{
	ReadSignIdAward();
	ReadSevenDaysAward();
	ReadAwardLobby();
	return TRUE;
}

BOOL AwardLobbySetting::UnInit()
{
	ClearSignIdAward();
	ClearSevenDaysAward();
	ClearAwardLobby();
	return TRUE;
}

SignInAward* AwardLobbySetting::GetSignInAwardByIndex(INT nIndex)
{
	if (m_signIdAwardVec.size() > nIndex && nIndex >= 0)
		return m_signIdAwardVec[nIndex];

	return NULL;
}

SevenDaysAward* AwardLobbySetting::GetSevenDaysAwardByIndex(INT nIndex)
{
	if (m_sevenDaysAwardVec.size() > nIndex && nIndex >= 0)
		return m_sevenDaysAwardVec[nIndex];

	return NULL;
}

AwardLobby* AwardLobbySetting::GetAwardLobby(INT key)
{
	std::map<INT, AwardLobby*>::iterator it = m_awardLobbyMap.find(key);
	if (it != m_awardLobbyMap.end())
		return it->second;

	return NULL;
}

BOOL AwardLobbySetting::ReadSignIdAward()
{
	ClearSignIdAward();

	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(SIGNIN_AWARD_PATH);

	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 2; i <= nFileHeight; i++)  // 第2行
	{
		SignInAward* pSignIn = new SignInAward();
		tabFile->GetInteger(i, "id",			0,	&pSignIn->nId);
		tabFile->GetInteger(i, "awardId",		0,	&pSignIn->nAwardId);
		tabFile->GetInteger(i, "signInCount",		0,	&pSignIn->nSignInCount);

		m_signIdAwardVec.push_back(pSignIn);
	}

	return TRUE;

EXIT0:
	return FALSE;

}

VOID AwardLobbySetting::ClearSignIdAward()
{
	for (std::vector<SignInAward*>::iterator it = m_signIdAwardVec.begin(); it != m_signIdAwardVec.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_signIdAwardVec.clear();
}

BOOL AwardLobbySetting::ReadSevenDaysAward()
{
	ClearSevenDaysAward();

	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(SEVENDAYS_AWARD_PATH);

	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 2; i <= nFileHeight; i++)  // 第2行
	{
		SevenDaysAward* pSevenDays = new SevenDaysAward();
		tabFile->GetInteger(i, "id",			0,	&pSevenDays->nId);
		tabFile->GetInteger(i, "awardId",		0,	&pSevenDays->nAwardId);

		m_sevenDaysAwardVec.push_back(pSevenDays);
	}

	return TRUE;

EXIT0:
	return FALSE;

}

VOID AwardLobbySetting::ClearSevenDaysAward()
{
	for (std::vector<SevenDaysAward*>::iterator it = m_sevenDaysAwardVec.begin(); it != m_sevenDaysAwardVec.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_sevenDaysAwardVec.clear();
}


BOOL AwardLobbySetting::ReadAwardLobby()
{
	ClearAwardLobby();
	KBuySellSetting tmpBuysell;

	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(AWARD_LOBBY_PATH);

	CHAR szFilePath[_MAX_PATH];
	QString szFileTemp;
	ITabFile* pFile = NULL;
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 2; i <= nFileHeight; i++)  // 第3行
	{
		INT nId = 0;
		CHAR szRegion[20];
		AwardLobby* pAwardLobby = new AwardLobby();
		tabFile->GetInteger(i, "id",			0,	&pAwardLobby->nId);
		tmpBuysell.TryGetDateTime(tabFile, i, "startTime", pAwardLobby->tStartTime);
		tmpBuysell.TryGetDateTime(tabFile, i, "endTime", pAwardLobby->tEndTime);
		tabFile->GetString(i, "targetPath",	"", szFilePath, _MAX_PATH);
		tabFile->GetString(i, "awardModuleName",	"", pAwardLobby->szModuleName, countof(pAwardLobby->szModuleName));
		tabFile->GetString(i, "regionId", "", szRegion, 20);

		std::string strRegion(szRegion);

		if(!strRegion.size())
		{
			continue;
		}
		INT nStartLine = 1, nTotalLine = 1;
		std::basic_string<char>::size_type iBegin = 0, iEnd = 0;
		iEnd = strRegion.find(',', iBegin);
		if (iEnd != std::string::npos)
		{
			nStartLine = atoi((strRegion.substr(iBegin, iEnd - iBegin)).c_str());
			iBegin = iEnd + 1;
		}
		nTotalLine = atoi((strRegion.substr(iBegin, strRegion.size() - iBegin)).c_str());

		szFileTemp = LOBBY_TARGET_FOLD;
		szFileTemp = szFileTemp + szFilePath;
		pFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pFile);
		for (int j = nStartLine+1; j <= nTotalLine+1; j++)
		{
			LobbyTarget* pLobby = new LobbyTarget();
			pFile->GetInteger(j, "id",			0,	&pLobby->nId);
			pFile->GetInteger(j, "awardId",		0,	&pLobby->nAwardId);
			pFile->GetInteger(j, "minValue",		0,	&pLobby->nMinValue);

			pAwardLobby->mapLobbyTarget.insert(std::make_pair(pLobby->nId, pLobby));
		}

		m_awardLobbyMap.insert(std::make_pair(pAwardLobby->nId, pAwardLobby));
	}

	return TRUE;

EXIT0:
	return FALSE;

}

VOID AwardLobbySetting::ClearAwardLobby()
{
	for (std::map<INT, AwardLobby*>::iterator it = m_awardLobbyMap.begin(); it != m_awardLobbyMap.end(); ++it)
	{
		AwardLobby* pAward = it->second;
		for (std::map<INT, LobbyTarget*>::iterator it_target = pAward->mapLobbyTarget.begin(); it_target != pAward->mapLobbyTarget.end(); ++it_target)
		{
			SAFE_DELETE(it_target->second);
		}
		pAward->mapLobbyTarget.clear();
		SAFE_DELETE(it->second);
	}

	m_awardLobbyMap.clear();
}

