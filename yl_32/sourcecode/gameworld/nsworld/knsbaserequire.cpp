
#include "stdafx.h"
#include "config/kconfigfile.h"
#include "onlinegamemodule/kmoduledef.h"
#include "config/knsbaserequire.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KNsBaseRequire* g_InitNsBaseRequire()
{
	static KNsBaseRequire s_NsBaseRequire;
	s_NsBaseRequire.Init();
	return &s_NsBaseRequire;
}

// -------------------------------------------------------------------------

BOOL KNsBaseRequire::Init( )
{
	QIniFile* pLoadedIniFile = NULL;

	memset(&m_sEnvCfg, 0, sizeof(m_sEnvCfg));
#ifdef GAME_SERVER
	IIniFile* pIniFile = g_OpenIniFile(KDF_SERVER_CFG);
#else
	IIniFile* pIniFile = g_OpenIniFile(KDF_CLIENT_CFG);
#endif
	CHAR szBuf[32];
	QCONFIRM_RET_FALSE(pIniFile);

	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "MaxPlayer", KD_DEFAULT_MAX_PLAYER, &m_sEnvCfg.nMaxPlayer);
	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "MaxNpc", KD_DEFAULT_MAX_NPC, &m_sEnvCfg.nMaxNpc);
	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "MaxDoodad", KD_DEFAULT_MAX_DOODAD, &m_sEnvCfg.nMaxDoodad);
	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "MaxItem", 0, &m_sEnvCfg.nMaxItem);
	pIniFile->GetString(KD_SERVER_OVERLOAD_CFG, "StartSeverTime", KD_DEFAULT_START_SEVER_TIME, szBuf, sizeof(szBuf));
	m_sEnvCfg.dwStartSeverTime = KSysService::StringToDateTime(szBuf);
#ifdef GAME_SERVER
	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "MaxWaitPlayer", KD_DEFAULT_MAX_WAIT_PLAYER, &m_sEnvCfg.nMaxWaitPlayer);
	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "MaxVipPlayer", KD_DEFAULT_VIP_RESERVATION, &m_sEnvCfg.nMaxVipPlayer);

	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "CharacterArrayValueMaxIndexBy", 10,
		&m_sEnvCfg.nCharacterArrayValueMaxIndexBy);
	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "CharacterArrayValueMaxIndexW", 10,
		&m_sEnvCfg.nCharacterArrayValueMaxIndexW);
	pIniFile->GetInteger(KD_SERVER_OVERLOAD_CFG, "CharacterArrayValueMaxIndexDw", 10,
		&m_sEnvCfg.nCharacterArrayValueMaxIndexDw);
#endif // GAME_SERVER

	return TRUE;
}

const KENV_CONFIG* KNsBaseRequire::GetEnvConfig()
{
	return &m_sEnvCfg;
}

IKModuleInterface** KNsBaseRequire::GetModuleList()
{
	return g_GetModuleList();
}

