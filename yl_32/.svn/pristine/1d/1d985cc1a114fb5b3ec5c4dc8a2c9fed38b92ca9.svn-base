#ifndef _KSNDAAGENCY_H_
#define _KSNDAAGENCY_H_

#include "protocol/GatewayDef.h"

#ifdef USE_SDOA

#ifdef WIN32
#include "../../engine_depends/devenv/SDOA_SDK/Server/win32/bin/SDOA4Server1.1_Win32_Vc7.1_X86/sdoa4server.h"
#ifdef _DEBUG
#define SDOA_DLL_FILE "SDOA4ServerD.dll"
#else
#define SDOA_DLL_FILE "SDOA4Server.dll"
#endif
typedef HMODULE KG_HMODULE;
#else
#include "../SDOA_SDK/Server/linux/bin/SDOA4Server1.1_Linux2.6_Gcc4.1_X86/sdoa4server.h"
#define SDOA_DLL_FILE "./libSDOA4Server.so"
typedef void* KG_HMODULE;
#endif
#define SDOA_CONFIG_FILE "sdoa4server.ini"

#endif // USE_SDOA

struct KTOKEN_VERIFY_RESPOND
{
	int     nCode;
	int     nPlayerIndex;
	char    szSndaID[SNDA_ID_LEN];
	char    szIDCard[ID_CARD_LEN];
	BOOL    bAdult;
};

struct KPlayerAgency;
class KPlayerManager;
class KPaysysAgency;
class QGateway;

class KSndaAgency
{
public:
	KSndaAgency();
	~KSndaAgency();

	BOOL Init();
	void UnInit();

	BOOL LoadConfig();

	void Activate();

	void AttachModule(KPlayerManager* pModule) { m_pPlayerManager = pModule; }
	void AttachModule(KPaysysAgency* pModule) { m_pPaysysAgency = pModule; }
	void AttachModule(QGateway* pModule) { m_pGateway = pModule; }

	BOOL DoTokenVerifyRequest(KPlayerAgency* pPlayer);

	void OnTokenVerifyRespond(const KTOKEN_VERIFY_RESPOND* pRespond);

private:
#ifdef USE_SDOA
	KG_HMODULE              m_hSDOALibrary;
	ISDOAUserInfoAuthen*    m_piUserInfoAuthen;
#endif // USE_SDOA

	KPlayerManager*         m_pPlayerManager;
	KPaysysAgency*          m_pPaysysAgency;
	QGateway*            m_pGateway;

public:
	BOOL    m_bOpen;
	char    m_szAdultIDCard[ID_CARD_LEN];
};

#endif
