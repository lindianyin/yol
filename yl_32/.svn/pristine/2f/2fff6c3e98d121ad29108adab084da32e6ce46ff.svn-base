/* -------------------------------------------------------------------------
//	文件名		：	kldapclient.cpp
//	创建者		：	simon
//	创建时间	：	2011/4/15 21:25:58
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kldapclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
#ifndef WIN32

#define SASL_CB_LIST_END   0  /* end of list */
#define SASL_CB_AUTHNAME     0x4002  /* client authentication name */
#define SASL_CB_PASS         0x4004  /* client passphrase-based secret */

typedef struct sasl_interact {
	unsigned long id;		/* same as client/user callback ID */
	const char *challenge;	/* presented to user (e.g. OTP challenge) */
	const char *prompt;		/* presented to user (e.g. "Username: ") */
	const char *defresult;	/* default result string */
	const void *result;		/* set to point to result */
	unsigned len;		/* set to length of result */
} sasl_interact_t;

typedef struct ldap_sasl_defaults {
	const char *user;
	const char *passwd;
} ldap_sasl_defaults_t;

int LdapCallbackFun(LDAP *ld, unsigned flags, void *defaults, void *sasl_interact)
{
	sasl_interact_t* pSasl = (sasl_interact_t*)sasl_interact;
	ldap_sasl_defaults_t* pDefaults = (ldap_sasl_defaults_t*)defaults;

	while (pSasl && pSasl->id != SASL_CB_LIST_END)
	{
		if (pSasl->id == SASL_CB_AUTHNAME)
		{
			pSasl->defresult = pDefaults->user;
			pSasl->result = pSasl->defresult;
			pSasl->len = strlen((char*)pSasl->result);
		}
		if (pSasl->id == SASL_CB_PASS)
		{
			pSasl->defresult = pDefaults->passwd;
			pSasl->result = pSasl->defresult;
			pSasl->len = strlen((char*)pSasl->result);
		}

		pSasl++;
	}
	return LDAP_SUCCESS;
}
#endif // not WIN32

KLdapClient::KLdapClient(void) : m_hLdap(NULL)
{
	m_szAuthMethod[0] = 0;
	m_szServer[0] = 0;
	m_szDomain[0] = 0;
}

KLdapClient::~KLdapClient(void)
{
}

BOOL KLdapClient::Connect( PCSTR pszServer,  PCSTR pszDomain/* = NULL*/, PCSTR pszAuthMethod /*= NULL*/ )
{
	BOOL bResult = FALSE;
	ULONG uRetCode = 0;
	assert(pszServer);

	QStrCpyLen(m_szServer, pszServer, sizeof(m_szServer));
	if (pszAuthMethod)
		QStrCpyLen(m_szAuthMethod, pszAuthMethod, sizeof(m_szAuthMethod));
	if (pszDomain)
		QStrCpyLen(m_szDomain, pszDomain, sizeof(m_szDomain));

	m_hLdap = ldap_init((PCHAR)pszServer, LDAP_PORT);
	LOG_PROCESS_ERROR(m_hLdap);
#ifdef WIN32
	uRetCode = ldap_connect(m_hLdap, NULL);
#else
	uRetCode = m_hLdap ? LDAP_SUCCESS : errno;
	{
		INT version = LDAP_VERSION3;
		ldap_set_option(m_hLdap, LDAP_OPT_PROTOCOL_VERSION, &version);
	}
#endif // WIN32

	if (uRetCode != LDAP_SUCCESS)
	{
		QLogPrintf(LOG_ERR,
			"Failed to init/connect ldap server %s, code 0x%x %s",
			pszServer, uRetCode, ldap_err2string(uRetCode));
		goto EXIT0;
	}
	QLogPrintf(LOG_INFO,
		"Init/Connect ldap server %s ok, AuthMethod [%s]", pszServer, m_szAuthMethod);
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KLdapClient::VerifyAccount( PCSTR pszAccount, PCSTR pszPassword )
{
	Connect(m_szServer, m_szDomain, m_szAuthMethod);
#ifdef WIN32
	SEC_WINNT_AUTH_IDENTITY sCredential;
	sCredential.Flags = SEC_WINNT_AUTH_IDENTITY_ANSI;
	sCredential.Domain = 0;
	sCredential.DomainLength = 0;
	sCredential.Password = (PBYTE)pszPassword;
	sCredential.PasswordLength = (ULONG)strlen((PCHAR)sCredential.Password);
	sCredential.User = (PBYTE)pszAccount;
	sCredential.UserLength = (ULONG)strlen((PCHAR)sCredential.User);
	ULONG uRetCode = ldap_bind_s(m_hLdap, NULL, (PCHAR)&sCredential, LDAP_AUTH_DIGEST);
	QLogPrintf(LOG_INFO,
		"LDAP域验证 verification for %s, code 0x%x %s",
		pszAccount, uRetCode, ldap_err2string(uRetCode));
#else
	// method 1 (调用系统ldap工具)
	//CHAR szCommand[MAX_PATH] = "";
	//if (*m_szAuthMethod)
	//{
	//	snprintf(szCommand, sizeof(szCommand), 
	//		"ldapwhoami -Y %s -h%s -U%s -w%s",
	//		m_szAuthMethod, m_szServer, pszAccount, pszPassword);
	//}
	//else
	//{
	//	snprintf(szCommand, sizeof(szCommand), 
	//		"ldapwhoami -h%s -U%s -w%s",
	//		m_szServer, pszAccount, pszPassword);
	//}
	//ULONG uRetCode = system(szCommand);
	//uRetCode = WEXITSTATUS(uRetCode);
	//QLogPrintf(LOG_INFO, "LDAP verification for %s, code 0x%x", pszAccount, uRetCode);

	// method 2 (无加密验证)
	// todo prepend domain name
	CHAR szUserNameFull[MAX_PATH] = "";
	snprintf(szUserNameFull, sizeof(szUserNameFull), "%s%s%s", m_szDomain, *m_szDomain ? "\\" : "", pszAccount);
	ULONG uRetCode = ldap_simple_bind_s(m_hLdap, szUserNameFull, pszPassword);
	QLogPrintf(LOG_INFO,
		"LDAP verification [LDAP_AUTH_SIMPLE] for %s, code 0x%x %s",
		pszAccount, uRetCode, ldap_err2string(uRetCode));

	// method 3 (OpenLDAP和windows域有时不太兼容，暂时取消这种验证方法)
	//ldap_sasl_defaults_t sDefaults;
	//sDefaults.user = pszAccount;
	//sDefaults.passwd = pszPassword;
	//ULONG uRetCode = ldap_sasl_interactive_bind_s(m_hLdap, NULL, m_szAuthMethod, 0, 0,
	//		LDAP_SASL_QUIET, LdapCallbackFun, &sDefaults);
	//QLogPrintf(LOG_INFO,
	//	"LDAP verification [%s] for %s, code 0x%x %s", m_szAuthMethod,
	//	pszAccount, uRetCode, ldap_err2string(uRetCode));
#endif // WIN32
	ldap_unbind_s(m_hLdap);
	m_hLdap = NULL;
	return uRetCode == LDAP_SUCCESS;
}

BOOL KLdapClient::Disconnect()
{
	if (m_hLdap)
	{
		ldap_unbind_s(m_hLdap);
		m_hLdap = NULL;
	}
	return TRUE;
}

KLdapClient& KLdapClient::Inst()
{
	static KLdapClient s_ldapClient;
	return s_ldapClient;
}
