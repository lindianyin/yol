/* -------------------------------------------------------------------------
//	文件名		：	kldapclient.h
//	创建者		：	simon
//	创建时间	：	2011/4/15 21:25:54
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KLDAPCLIENT_H__
#define __KLDAPCLIENT_H__

#ifdef WIN32
#include <Winldap.h>
#else
#define LDAP_DEPRECATED 1
#include <ldap.h>
#endif // WIN32

class KLdapClient
{
public:
	static KLdapClient& Inst();
	BOOL Connect(PCSTR pszServer, PCSTR pszDomain = NULL, PCSTR pszAuthMethod = NULL);
	BOOL VerifyAccount(PCSTR pszAccount, PCSTR pszPassword);
	BOOL Disconnect();

private:
	KLdapClient(void);
	virtual ~KLdapClient(void);
private:
	LDAP* m_hLdap;
	CHAR m_szServer[MAX_PATH];
	CHAR m_szAuthMethod[MAX_PATH];
	CHAR m_szDomain[MAX_PATH];
};

// -------------------------------------------------------------------------

#endif /* __KLDAPCLIENT_H__ */
