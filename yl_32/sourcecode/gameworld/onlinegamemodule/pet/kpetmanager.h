
/* -------------------------------------------------------------------------
//	文件名		：	kpetmanager.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-11-28 18:13:00
//	功能描述	：  
// -------------------------------------------------------------------------*/

#ifndef __KPETMANAGER_H__
#define __KPETMANAGER_H__

#include "petdef.h"
#include "onlinegameworld/kidobjpool.h"
#include "kpet.h"
#include "kride.h"
#include "Misc/qindexlist.h"

class KPlayer;

class KPetManager
{
public:
	KPetManager();
	~KPetManager();

public:
	BOOL	Init();
	BOOL	UnInit();

	INT		AddPet(DWORD dwId = 0);

	BOOL	ContactPetWithOwner(KPet* pPet, DWORD dwOwnerId);

	BOOL	RemovePet(DWORD dwId);

	KPet*	GetPetByIndex(INT nIndex);

	KPet*	GetPetById(DWORD dwId);

	// 得到找到的第一只相应类型和法系的神仙
	KPet*   GetPetByTypeAndNature(INT nType, INT nNature);

	KPet*	GetPetByOwnerId(DWORD dwOwnerId);

	KPet*	AddPlayerPet(KPlayer* pPlayer, INT nType, INT nCharacter = 0);

	INT		AddRide(DWORD dwId = 0);

	BOOL	RemoveRide(DWORD dwId);

	KRide*	GetRideByIndex(INT nIndex);

	KRide*	GetRideById(DWORD dwId);

	KRide* AddPlayerRide(KPlayer* pPlayer, 
		INT nGenre, 
		INT nDetail, 
		INT nParticular, 
		INT nLevel, 
		INT nHungry, 
		INT nMagicRate);

	BOOL ContactRideWithOwner(KRide* pRide, DWORD dwOwnerId);

	KRide* GetRideByOwnerId(DWORD dwOwnerId);

private:
	TKIdObjPool<KPet> m_cPetPool;				// 玩家身上的宠物

	TKIdObjPool<KRide> m_cRidePool;				// 玩家的坐骑

	std::map<DWORD, DWORD> m_mapCharacterId2PetId;		// 角色ID到宠物ID映射(只映射战斗的宠物)

	std::map<DWORD, DWORD> m_mapCharacterId2RideId;

	QIndexList m_cPetIdList;			// 已经使用的id列表
	QIndexList m_cRideIdList;		
};

extern KPetManager g_cPetMgr;

#endif
