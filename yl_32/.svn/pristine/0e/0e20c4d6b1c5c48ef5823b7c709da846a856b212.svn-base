
#include "stdafx.h"
#include "kescorthelper.h"

KEscortHelper g_cEscortHelper;

KEscortHelper::KEscortHelper(void)
{
	
}

KEscortHelper::~KEscortHelper(void)
{
}

BOOL KEscortHelper::SetNpcWithDelayData(KNpc* pNpc, KDelayRoleData* pData)
{
	BOOL bResult = FALSE;
	KDelayBaseData* pBaseData = &(pData->sBaseRoleData);
	LOG_PROCESS_ERROR(pBaseData);

	pNpc->SetSex(pBaseData->bySex);

	pNpc->SetMaxLife(pBaseData->nMaxLife);
	pNpc->SetCurrentLife(pBaseData->nMaxLife);
	//Íâ¹Û
	pNpc->SetHatRepresentId(pBaseData->wHatRepresentId);
	pNpc->SetBodyRepresentId(pBaseData->wBodyRepresentId);
	pNpc->SetLeftWeaponRepresentId(pBaseData->wLeftWeaponRepresentId);
	pNpc->SetLeftShoulderRepresentId(pBaseData->wLeftShoulderRepresentId);
	pNpc->SetRightWeaponRepresentId(pBaseData->wRightWeaponRepresentId);
	pNpc->SetRightShoulerRepresentId(pBaseData->wRightShoulerRepresentId);
	pNpc->SetRideRepresentId(pBaseData->wRideRepresentId);
	pNpc->SetHeadPendantRepresentId(pBaseData->wHeadPendantRepresentId);
	pNpc->SetBottomPendantRepresentId(pBaseData->wBottomPendantRepresentId);
	pNpc->SetBackPendantRepresentId(pBaseData->wBackPendantRepresentId);
	pNpc->SetPetRepresentId(pBaseData->wPetRepresentId);

	pNpc->SetFightScore(pBaseData->dwFightScore);

	CHAR szNewName[64];
	sprintf(szNewName, "%s", pBaseData->m_szRoleName);
	pNpc->SetStrName(szNewName);

	bResult = TRUE;
EXIT0:
	return bResult;
}