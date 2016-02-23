
#ifdef GAME_CLIENT
#include "stdafx.h"
#include "onlinegameworld/kcharacterclient.h"
#include "onlinegameworld/kcharacter.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kbaseprotocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
BOOL KCharacterClient::LoadSyncVarSelf( KCHARACTER_VAR_SYNC_SELF& sSyncVar )
{
	for (UINT i = 0; i < countof(csByteVarSyncSelf); ++i)
		m_cCharacter.m_cContainer.m_cByArrayValue.SetValue(csByteVarSyncSelf[i], sSyncVar.aByVarSet[i]);
	for (UINT i = 0; i < countof(csWordVarSyncSelf); ++i)
		m_cCharacter.m_cContainer.m_cWArrayValue.SetValue(csWordVarSyncSelf[i], sSyncVar.aWVarSet[i]);
	for (UINT i = 0; i < countof(csDWordVarSyncSelf); ++i)
		m_cCharacter.m_cContainer.m_cDwArrayValue.SetValue(csDWordVarSyncSelf[i], sSyncVar.aDwVarSet[i]);
	for (UINT i = 0; i < sSyncVar.wMapVarNum; ++i)
	{
		m_cCharacter.m_cContainer.SetMapValue
		(
		KD_VAR_KEY(sSyncVar.aMapVarSet[i].byVarGroup, sSyncVar.aMapVarSet[i].byVarId),
		sSyncVar.aMapVarSet[i].dwVarValue);
	}
	m_cCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_UPDATE_ALL_ATTRIBUTE, m_cCharacter.m_dwId, 0, 0);
	return TRUE;
}

BOOL KCharacterClient::LoadSyncVarOther( KCHARACTER_VAR_SYNC_OTHER& sSyncVar )
{
	for (UINT i = 0; i < countof(csByteVarSyncOther); ++i)
		m_cCharacter.m_cContainer.m_cByArrayValue.SetValue(csByteVarSyncOther[i], sSyncVar.aByVarSet[i]);
	for (UINT i = 0; i < countof(csWordVarSyncOther); ++i)
		m_cCharacter.m_cContainer.m_cWArrayValue.SetValue(csWordVarSyncOther[i], sSyncVar.aWVarSet[i]);
	for (UINT i = 0; i < countof(csDWordVarSyncOther); ++i)
		m_cCharacter.m_cContainer.m_cDwArrayValue.SetValue(csDWordVarSyncOther[i], sSyncVar.aDwVarSet[i]);
	for (UINT i = 0; i < sSyncVar.wMapVarNum; ++i)
	{
		m_cCharacter.m_cContainer.SetMapValue
			(
			KD_VAR_KEY(sSyncVar.aMapVarSet[i].byVarGroup, sSyncVar.aMapVarSet[i].byVarId),
			sSyncVar.aMapVarSet[i].dwVarValue);
	}
	return TRUE;
}
// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT
