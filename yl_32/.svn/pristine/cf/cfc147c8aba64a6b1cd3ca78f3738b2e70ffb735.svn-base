
#pragma once

#include "KPaysysAgency.h"
#include "KPlayerManager.h"
#include "KRelayAgency.h"
#include "KQueueManager.h"
#include "KGatewayEyes.h"
#include "KSndaAgency.h"
#include "UnityPolicyServer.h"

#define GATEWAY_CONFIG_FILE "gateway_cfg.ini"

class QGateway
{
public:
	QGateway();

	BOOL Init();
	void UnInit();

	void Run();

	void Quit() { m_bRunFlag = false; }

public:
	time_t              m_nTimeNow;

private:
	BOOL                LoadConfig();

private:
	char                m_szLocale[64];
	BOOL                m_bRunFlag;
	KPaysysAgency       m_PaysysAgency;
	KRelayAgency        m_RelayAgency;
	KPlayerManager      m_PlayerManager;
	KQueueManager       m_QueueManager;
	UnityPolicyServer m_UnityPolicyServer;
	KGatewayEyes        m_Eyes;
	KSndaAgency         m_SndaAgency;
	KE_ACCOUNT_VERIFIER m_eAccountVerifier;
	CHAR m_szLdapServer[MAX_PATH];
	CHAR m_szLdapDomain[MAX_PATH];
	CHAR m_szLdapAuthMethod[MAX_PATH];
};

