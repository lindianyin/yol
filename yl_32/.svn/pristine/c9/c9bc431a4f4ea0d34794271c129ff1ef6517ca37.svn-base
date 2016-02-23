#ifndef __KPETCTRL_H__
#define __KPETCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "config/kplayerbasedbdef.h"
#include "../onlinegamemodule/item2/kmagicattribmanager.h"
#include "petdef.h"

class KPlayer;
class KPet;

class KPetCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_PET	};

	struct LastPetIslandInfo
	{
		WORD wLastPetQuality;
		INT wLastPetIndex;
		WORD wLastPetNature;
		WORD wLastFightState;
	};

private:
	struct PetSaveData
	{
		WORD wType;
		WORD wQuality;
		WORD wCharacter;
		CHAR szName[32];
		BYTE byLevel;
		BYTE byRankLevel;
		BYTE byState;
		BYTE byLastState;
		BYTE byHungry;
		BYTE byEquipSlot;
		BYTE byTactic;
		DWORD dwExp;
		DWORD dwRandSeed;
		BYTE byRename;
		INT  anSkillLevels[emPET_COUNT_SKILL];
		KPETEQUIP awEquip[emPET_COUNT_EQUIP];
	};

public:
	typedef std::vector<KPet*> PET_ARRAY;

	explicit KPetCtrl(KCharacter& rcPlayer);
	virtual ~KPetCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL Active();
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);
	virtual BOOL OnLogout();
	virtual BOOL OnLogin();

private:
	BOOL SavePetData(PetSaveData* pBuffer, KPet* pPet);

	BOOL MakePet(KPet* pPet, PetSaveData* pSaveData);

	INT GetPetBaseAttrValue(KPet* pPet, INT nIdx);

	BOOL CalcPetAttrib(KPet* pPet, KMagicAttrib& sMagicAttib, INT nIdx);

	// 将宠物属性生效到角色身上
	BOOL ApplyPetAttrib(KPet* pPet);

	BOOL RemovePetAttrib(KPet* pPet);

	BOOL CanChangePetState(KPet* pPet, INT nState);

public:
	INT GetMaxPower();
	
	BOOL Add(KPet* pPet, BOOL bSync = FALSE);

	BOOL UpgradePetSkill(KPet* pet, INT nSkillId);

	BOOL Insert(KPet* pPet, INT nPos);

	BOOL ApplyTacticAttr(KPet* pPet);

	BOOL RemoveTacticAttr(KPet* pPet);

	BOOL ApplyPetProb(KPet* pPet);

	BOOL RemovePetProb(KPet* pPet);

	BOOL SyncPetInfo(KPet* pPet);

	BOOL SyncPetCardInfo(std::vector<QPET_CARD>& vecCardInfos);

	BOOL SetPetFightState(KPet* pPet, INT nState);

	VOID ReducePetHungry(KPet* pPet);

	BOOL OnChangeStateRequest(KPet* pPet, INT nState);

	BOOL SyncPetExp(KPet* pPet);

	BOOL SyncPetIslandRefreshState(INT nCurQuality);  // 同步幸运条进度

	BOOL BroadcastPetEquipInfo(KPet* pPet);

	BOOL BroadcastPetState(KPet* pPet);

	BOOL BroadcastCastSkill(KPet* pPet, INT nSkillId, INT nTargetId, INT nMissileTime);

	BOOL OnPutOnEquipRequest(KPet* pPet, DWORD dwItemId, BYTE byPos);

	BOOL OnTakeOffEquipRequest(KPet* pPet, BYTE byPos);

	INT GetPetPos(DWORD dwId);

	KPet* GetPetByPos(INT nPos);

	BOOL RemoveOnly(INT nPos);

	// 放生宠物
	BOOL FreePet(DWORD dwId);

    BOOL TransExpToPet(KPet* pSrcPet, KPet* pTargetPet, INT nPercent);

	BOOL OnChangeTactic(INT nCharacter, KPet* pPet);

	KPet* GetPetByTactic(INT nCharacter);

	BOOL RemoveFromTactic(INT nCharacter);

	VOID BindPetTarget(DWORD dwPetId, DWORD dwTargetId, INT nPetX, INT nPetY);

	BOOL ChangeName(KPet* pPet, LPCTSTR szName);

	VOID SetLastPetIslandInfo(CONST LastPetIslandInfo& info) { m_cLastPetIslandInfo = info; }

	CONST LastPetIslandInfo& GetLastPetIslandInfo() { return m_cLastPetIslandInfo; }

	// 得到宠物数量
	INT GetPetsCount() { return m_arrPets.size(); }

	// 根据位置得到宠物
	KPet* GetPetByIdx(INT nIdx);

	KPet* GetPetById(INT nId);

	KPet* GetCurrentFightPet();

	INT m_arrPetRefreshRate[6];  // 神仙岛刷新成功率 6个
	INT m_nPetIslandRefreshCount; // 玩家一共刷新的次数 宠物岛

	BOOL SavePetDelayData(KDelayPetData* pDelayPetData);

public:
	BOOL RecordPetsLastState();
	BOOL ResumePetsLastState();

private:
	KPlayer& m_rcPlayer;	// 所属玩家
	PET_ARRAY m_arrPets;	// 玩家拥有的宠物列表
    LastPetIslandInfo m_cLastPetIslandInfo;

	typedef std::map<INT, KPet*> PET_TACTIC_MAP;
	PET_TACTIC_MAP m_mapTactic;	// 宠物阵位
};


#endif