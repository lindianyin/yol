#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "itemenv.h"

#ifdef GAME_SERVER

LPCSTR KItemEnvPlayer::GetEnvMsg()
{
	snprintf(
		m_szBuff, sizeof(m_szBuff) - 1,
		"Account:%s, Role:%s",
		GetAccount(),
		GetRole());
	m_szBuff[sizeof(m_szBuff) - 1] = 0;
	return m_szBuff;
}

LPCSTR KItemEnvPlayer::GetAccount()
{
	if (m_pPlayer)
		return m_pPlayer->m_cPlayerServer.GetAccount();
	return "";
}

LPCSTR KItemEnvPlayer::GetRole()
{
	if (m_pPlayer)
		return m_pPlayer->GetName();
	return "";
}

#endif //_SERVER
