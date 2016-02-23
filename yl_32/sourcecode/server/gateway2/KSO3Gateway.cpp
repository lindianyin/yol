#include "stdafx.h"
#include "KSO3Gateway.h"
#include "kldapclient.h"

KSO3Gateway::KSO3Gateway()
{
	m_szLocale[0] = '\0';
	m_szLdapAuthMethod[0] = '\0';
	m_szLdapDomain[0] = '\0';
	m_szLdapServer[0] = '\0';
}

BOOL KSO3Gateway::Init()
{
	BOOL            bResult             = false;
	BOOL            bRetCode            = false;
	const char*     pszLocale           = NULL;
	BOOL            bPaysysAgencyInit   = false;
	BOOL            bLdapInit   = false;
	BOOL            bRelayAgencyInit    = false;
	BOOL            bPlayerManagerInit  = false;
	BOOL            bEyesInit           = false;
	BOOL            bSDOAInit           = false;

	g_SetRootPath(NULL);
	m_eAccountVerifier = emACCOUNT_VERIFIER_PAYSYS;

	bRetCode = LoadConfig();
	LOG_PROCESS_ERROR(bRetCode);

	pszLocale = setlocale(LC_ALL, m_szLocale);
	LOG_PROCESS_ERROR(pszLocale);

	QLogPrintf(LOG_INFO, "Set locale: '%s'\n", pszLocale);

	m_PaysysAgency.AttachModule(&m_RelayAgency);
	m_PaysysAgency.AttachModule(&m_PlayerManager);
	m_PaysysAgency.AttachModule(&m_Eyes);

	m_RelayAgency.AttachModule(&m_PaysysAgency);
	m_RelayAgency.AttachModule(&m_PlayerManager);
	m_RelayAgency.AttachModule(this);
	m_RelayAgency.AttachModule(&m_QueueManager);
	m_RelayAgency.AttachModule(&m_SndaAgency);

	m_PlayerManager.AttachModule(&m_PaysysAgency);
	m_PlayerManager.AttachModule(&m_RelayAgency);
	m_PlayerManager.AttachModule(&m_QueueManager);
	m_PlayerManager.AttachModule(this);
	m_PlayerManager.AttachModule(&m_SndaAgency);

	m_QueueManager.AttachModule(&m_PlayerManager);
	m_QueueManager.AttachModule(&m_RelayAgency);

	m_Eyes.AttachModule(&m_PlayerManager);
	m_Eyes.AttachModule(&m_PaysysAgency);
	m_Eyes.AttachModule(&m_QueueManager);

	m_SndaAgency.AttachModule(&m_PlayerManager);
	m_SndaAgency.AttachModule(&m_PaysysAgency);
	m_SndaAgency.AttachModule(this);

	if (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS)
	{
		bRetCode = m_PaysysAgency.Init();
		LOG_PROCESS_ERROR(bRetCode);
		bPaysysAgencyInit = true;
	}
	else if (m_eAccountVerifier & emACCOUNT_VERIFIER_LDAP)
	{
		bRetCode = KLdapClient::Inst().Connect(m_szLdapServer, m_szLdapDomain, m_szLdapAuthMethod);
		LOG_PROCESS_ERROR(bRetCode);
		bLdapInit = true;
	}

	bRetCode = m_RelayAgency.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bRelayAgencyInit = true;

	bRetCode = m_PlayerManager.Init(m_eAccountVerifier);
	LOG_PROCESS_ERROR(bRetCode);
	bPlayerManagerInit = true;

	bRetCode = m_Eyes.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bEyesInit = true;

	bRetCode = m_SndaAgency.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bSDOAInit = true;

	m_UnityPolicyServer.Init();

	m_bRunFlag = true;

	bResult = true;
EXIT0:
	if (!bResult)
	{
		if (bPlayerManagerInit)
		{
			m_PlayerManager.UnInit();
			bPlayerManagerInit = false;
		}

		if (bRelayAgencyInit)
		{
			m_RelayAgency.UnInit();
			bRelayAgencyInit = false;
		}

		if (bPaysysAgencyInit)
		{
			m_PaysysAgency.UnInit();
			bPaysysAgencyInit = false;
		}
		if (bLdapInit)
		{
			KLdapClient::Inst().Disconnect();
			bLdapInit = false;
		}

		if (bEyesInit)
		{
			m_Eyes.UnInit();
			bEyesInit = false;
		}

		if (bSDOAInit)
		{
			m_SndaAgency.UnInit();
			bSDOAInit = false;
		}
	}

	return bResult;
}

void KSO3Gateway::UnInit()
{
	m_UnityPolicyServer.Uninit();
	m_PlayerManager.UnInit();
	m_RelayAgency.UnInit();
	m_PaysysAgency.UnInit();
	m_Eyes.UnInit();
	m_SndaAgency.UnInit();
}


void KSO3Gateway::Run()
{
	BOOL bRetCode = false;

	while (m_bRunFlag)
	{
		m_nTimeNow = time(NULL);

		m_PaysysAgency.Activate();

		m_RelayAgency.Activate();

		m_PlayerManager.Activate();

		m_QueueManager.Activate();

		m_Eyes.Activate();

		m_SndaAgency.Activate();

		m_UnityPolicyServer.Activate();

		bRetCode = m_Eyes.GetQuitFlag();
		if (bRetCode)
			break;

		QThread_Sleep(1);
	}

	QLogPrintf(LOG_INFO, "Gateway exit !");
}

BOOL KSO3Gateway::LoadConfig()
{
	BOOL        bResult     = false;
	BOOL        bRetCode    = false;
	IIniFile*   piIniFile   = NULL;
	CHAR szTmp[MAX_PATH] = "";

	piIniFile = g_OpenIniFile(GATEWAY_CONFIG_FILE);
	LOG_PROCESS_ERROR(piIniFile);

	bRetCode = piIniFile->GetString("Global", "Locale", "", m_szLocale, sizeof(m_szLocale));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piIniFile->GetString("Global", "AccountServer", "", szTmp, sizeof(szTmp));
	LOG_PROCESS_ERROR(bRetCode);

	if (strncmp(szTmp, "none", sizeof(szTmp)) == 0)
		m_eAccountVerifier = emACCOUNT_VERIFIER_NONE;
	else if (strncmp(szTmp, "paysys", sizeof(szTmp)) == 0 || strncmp(szTmp, "sdoa", sizeof(szTmp)) == 0)
		m_eAccountVerifier = emACCOUNT_VERIFIER_PAYSYS;
	else if (strncmp(szTmp, "ldap", sizeof(szTmp)) == 0)
		m_eAccountVerifier = emACCOUNT_VERIFIER_LDAP;
	else
		QLogPrintf(LOG_WARNING, "Unknown config AccountServer=%s", szTmp);

	bRetCode = piIniFile->GetString("Global", "UseWhiteList", "", szTmp, sizeof(szTmp));
	LOG_PROCESS_ERROR(bRetCode);

	if (strncmp(szTmp, "1", sizeof(szTmp)) == 0 || strncmp(szTmp, "yes", sizeof(szTmp)) == 0)
		m_eAccountVerifier = KE_ACCOUNT_VERIFIER(m_eAccountVerifier | emACCOUNT_VERIFIER_WHITE_LIST);

	bRetCode = piIniFile->GetString("LDAP", "URI", "", m_szLdapServer, sizeof(m_szLdapServer));
	bRetCode = piIniFile->GetString("LDAP", "Domain", "", m_szLdapDomain, sizeof(m_szLdapDomain));
	bRetCode = piIniFile->GetString("LDAP", "SASL", "", m_szLdapAuthMethod, sizeof(m_szLdapAuthMethod));

	bResult = true;
EXIT0:
	SAFE_RELEASE(piIniFile);
	return bResult;
}
