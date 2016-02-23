#include "stdafx.h"
#include "kpetmanager.h"
#include "kpetctrl.h"
#include "kridectrl.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/kplayer.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"
#include "onlinegamemodule/trade/kplayerpurse.h"

KPetManager g_cPetMgr;

KPetManager::KPetManager()
{

}

KPetManager::~KPetManager()
{
	UnInit();
}

BOOL KPetManager::Init()
{
	m_cPetIdList.Init(PET_POOL_MAX_COUNT);
	m_cRideIdList.Init(RIDE_POOL_MAX_COUNT);

	m_cPetPool.Init(PET_POOL_MAX_COUNT);
	m_cRidePool.Init(RIDE_POOL_MAX_COUNT);

	return TRUE;
}

BOOL KPetManager::UnInit()
{
	m_cPetPool.Uninit();
	m_cRidePool.Uninit();

	m_mapCharacterId2PetId.clear();
	m_mapCharacterId2RideId.clear();
	return TRUE;
}

INT KPetManager::AddPet(DWORD dwId /*= 0*/)
{
	BOOL bRet = FALSE;
	INT nIndex = 0;
	KPet* pPet = NULL;

	dwId = m_cPetIdList.FindFree();

	nIndex = m_cPetPool.Add(dwId);

	m_cPetIdList.Use(dwId);

	pPet = m_cPetPool.GetByIndex(nIndex);
	LOG_PROCESS_ERROR(pPet);

	pPet->SetIndex(nIndex);

EXIT0:
	return nIndex;
}


BOOL KPetManager::ContactPetWithOwner(KPet* pPet, DWORD dwOwnerId)
{
	DWORD dwId = pPet->GetId();

	m_mapCharacterId2PetId[dwOwnerId] = dwId;

	return TRUE;
}

BOOL KPetManager::ContactRideWithOwner(KRide* pRide, DWORD dwOwnerId)
{
	DWORD dwId = pRide->GetId();

	m_mapCharacterId2RideId[dwOwnerId] = dwId;

	return TRUE;
}

BOOL KPetManager::RemovePet(DWORD dwId)
{
	KPet* pPet = g_cPetMgr.GetPetById(dwId);
	QCONFIRM_RET_FALSE(pPet);

	DWORD dwOwnerId = pPet->GetOwnerId();

	std::map<DWORD, DWORD>::const_iterator it = m_mapCharacterId2PetId.find(dwOwnerId);
	if(it != m_mapCharacterId2PetId.end() && it->second == pPet->GetId())
	{
		m_mapCharacterId2PetId.erase(dwOwnerId);
	}

	// 宠物ID回收
	QCONFIRM_RET_FALSE(m_cPetIdList.Free(dwId));

	return m_cPetPool.Remove(dwId);
}

KPet* KPetManager::GetPetByIndex(INT nIndex)
{
	return m_cPetPool.GetByIndex(nIndex);
}

KPet* KPetManager::GetPetByOwnerId(DWORD dwOwnerId)
{
	DWORD dwId;
	KPet* pPet = NULL;
	std::map<DWORD, DWORD>::const_iterator it = m_mapCharacterId2PetId.find(dwOwnerId);
	if(it != m_mapCharacterId2PetId.end())
	{
		dwId = it->second;
		pPet = m_cPetPool.GetById(dwId);
	}
	return pPet;
}

KPet* KPetManager::GetPetById(DWORD dwId)
{
	return m_cPetPool.GetById(dwId);
}

KPet* KPetManager::GetPetByTypeAndNature(INT nType, INT nNature)
{
    for (INT i = 0; i < m_cPetPool.GetUsedCount(); i++)
	{
		KPet* pPet = m_cPetPool.GetByIndex(i);
		if (pPet != NULL)
		{
			if (pPet->GetType() == nType && pPet->GetCharacter() == nNature)
			{
				return pPet;
			}
		}
	}
	return NULL;
}

// 给角色一只新宠物
KPet* KPetManager::AddPlayerPet(KPlayer* pPlayer, INT nType, INT nCharacter /* = 0 */)
{
	INT nIndex = 0;
	KPet* pPet = NULL;
	KPetCtrl* pPetCtrl = NULL;
	INT nQuality = 0;
	KPET_INDEX sIndex;
	time_t tNow = KSysService::Time(NULL);
	KPlayerPurse *pPlayerPurse = NULL;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	char szStr[1024];

	KPetTemplate* petTemplate = g_cPetSetting.GetPetTemplate(nType);
	LOG_PROCESS_ERROR(petTemplate);
	nQuality = petTemplate->nQuality;// 读取宠物所属品质

	sIndex.nType = nType;
	sIndex.nQuality = nQuality;
	sIndex.nCharacter = nCharacter;

	// 如果性格为0就随机一个性格
	if(!nCharacter)
		nCharacter = 1 + KSysService::Rand(emPET_COUNT_BASE);
	LOG_PROCESS_ERROR(nCharacter >= emPET_CHARACTER_LIFE && nCharacter <= emPET_CHARACTER_DODGE);
	sIndex.nCharacter = nCharacter;

	nIndex = AddPet();

	pPet = GetPetByIndex(nIndex);
	LOG_PROCESS_ERROR(pPet);

	pPet->Init(sIndex);

	pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	LOG_PROCESS_ERROR(pPetCtrl);

	pPetCtrl->Add(pPet, TRUE);

	//统计点 ldy
	pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);
	ZeroMemory(szStr,sizeof(szStr));
	sprintf(szStr,"nType=%d,nQuality=%d,nCharacter=%d",nType,nQuality,nCharacter);
	
	pPlayerPurse->LogMoney(EM_MONEY_COIN,0,59,&nResult,1,szStr);
	// 修改排行榜排名
	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LEVEL, pPlayer->GetId(), pPet->GetLevel(), tNow);

	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LV_OS, pPlayer->GetId(), pPet->GetLevel(), tNow);

EXIT0:
	return pPet;
}

INT KPetManager::AddRide(DWORD dwId /*= 0*/)
{
	BOOL bRet = FALSE;
	INT nIndex = 0;
	KRide* pRide = NULL;

	dwId = m_cRideIdList.FindFree();

	nIndex = m_cRidePool.Add(dwId);

	m_cRideIdList.Use(dwId);

	pRide = m_cRidePool.GetByIndex(nIndex);
	LOG_PROCESS_ERROR(pRide);

	pRide->SetIndex(nIndex);

EXIT0:
	return nIndex;
}

BOOL KPetManager::RemoveRide(DWORD dwId)
{
	KRide* pRide = g_cPetMgr.GetRideById(dwId);
	QCONFIRM_RET_FALSE(pRide);

	DWORD dwOwnerId = pRide->GetOwnerId();

	std::map<DWORD, DWORD>::const_iterator it = m_mapCharacterId2RideId.find(dwOwnerId);
	if(it != m_mapCharacterId2RideId.end() && it->second == pRide->GetId())
	{
		m_mapCharacterId2RideId.erase(dwOwnerId);
	}

	QCONFIRM_RET_FALSE(m_cRideIdList.Free(dwId));

	return m_cRidePool.Remove(dwId);
}

KRide* KPetManager::GetRideByIndex(INT nIndex)
{
	return m_cRidePool.GetByIndex(nIndex);
}

KRide* KPetManager::GetRideById(DWORD dwId)
{
	return m_cRidePool.GetById(dwId);
}

KRide* KPetManager::AddPlayerRide(KPlayer* pPlayer, 
								  INT nGenre, 
								  INT nDetail, 
								  INT nParticular, 
								  INT nLevel, 
								  INT nHungry,
								  INT nMagicRate)
{
	KITEM_INDEX sIndex = {nGenre, nDetail, nParticular, nLevel};

	INT nIndex = AddRide();

	KRide* pRide = GetRideByIndex(nIndex);
	QCONFIRM_RET_FALSE(pRide);

	pRide->Init(sIndex, nHungry, nMagicRate);

	KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
	QCONFIRM_RET_FALSE(pRideCtrl);

	pRideCtrl->Add(pRide, TRUE);

	ContactRideWithOwner(pRide, pPlayer->GetId());

	g_cCharacterHelper.CalcFightScore(*pPlayer);




	

	return pRide;
}

KRide* KPetManager::GetRideByOwnerId(DWORD dwOwnerId)
{
	DWORD dwId;
	KRide* pRide = NULL;

	std::map<DWORD, DWORD>::const_iterator it = m_mapCharacterId2RideId.find(dwOwnerId);
	if(it != m_mapCharacterId2RideId.end())
	{
		dwId = it->second;
		pRide = GetRideById(dwId);
	}

	return pRide;
}