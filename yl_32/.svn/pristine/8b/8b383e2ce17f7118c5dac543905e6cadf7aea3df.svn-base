/* -------------------------------------------------------------------------
//	文件名		：	kpet.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-2-27
//	功能描述	：	宠物
//
// -----------------------------------------------------------------------*/

#ifndef kpet_h__
#define kpet_h__


#include "kpetsetting.h"

// -------------------------------------------------------------------------

class KLuaPet;

// -------------------------------------------------------------------------

class KPet
{
public:
	DWORD m_dwId;

public:
	KPet(void);
	~KPet(void);

	KLuaPet* GetScriptInterface() { return m_pLuna; }

	BOOL Init(KPET_INDEX sIndex, UINT uRandSeed = 0);

	INT GetMaxRank();

	BOOL Breath();

	BOOL Gen_Pet(KPET_INDEX sIndex,
		INT nLevel,
		INT nRankLevel,
		UINT uRandSeed);

	BOOL Regenerate(INT nType,
		INT nQuality,
		INT nCharacter,
		INT nLevel, 
		INT nRankLevel,
		UINT uRandSeed);

	VOID SetIndex(INT nIndex) { m_nIndex = nIndex; }

	BOOL UnLockEquipPos(INT nPos);

	DWORD GetId() { return m_dwId; }

	DWORD GetRandSeed() { return m_dwRandSeed; }

	INT GetIndex() {return m_nIndex; }

	INT GetType() { return m_nType; }

	INT GetQuality() { return m_nQuality; }

	INT GetCharacter() { return m_nCharacter; }

	INT GetHungry() { return m_byHungry; }

	INT GetHungryConsume() { return m_nHungryConsume; }

	BOOL SetHungry(INT nHungry);

	INT GetLevel() { return m_nLevel; }

	INT GetSkillLevel(INT nSkillId);

	INT GetRankLevel() { return m_nRankLevel; }

	DWORD GetExp() { return m_dwExp; }

	DWORD GetOwnerId() { return m_dwOwnerId; }

	LPCTSTR GetPetName() { return m_szPetName; }

	INT GetPetState() { return m_byState; }

	INT GetFightScore() { return m_nFightScore; }

	VOID SetFightScore(INT nFightScore) { m_nFightScore = nFightScore; }

	INT GetBaseAttrValue(INT nIdx);

public:
	BOOL AddExp(DWORD dwExp);

	BOOL UpgradeSkill(INT nSkillId);

	BOOL SetExp(DWORD& rValue, DWORD dwNewValue);

	BOOL PutOnEquip(BYTE byPos, INT nParticular, INT nNumberIndex);

	BOOL TakeOffEquip(BYTE byPos);

	BOOL HasEquip(BYTE byPos);

	BOOL IsEquipLocked(BYTE byPos);

	INT	GetEquip(BYTE byPos);

	WORD GetEquipNumberIndex(BYTE byPos);

	INT GetBaseAttrScore();

	INT GetPetSkillScore();

	BOOL CalcFightScore();

	DWORD GetTotalExp(INT nLevel);		// 得到宠物指定等级总经验，0为得到宠物当前总经验

	DWORD GetTotalSkillBookNum();  // 得到宠物使用的技能书数量

	DWORD GetTotalUpRankBallNum(); // 得到宠物使用的升阶丸数量

	VOID SetSkillTargetId(DWORD dwTargetId) 
	{ 
		if (m_dwTargetId == 0)
		{
			m_dwTargetId = dwTargetId; 
			if (m_dwTargetId == 0)
			{
				m_nCurrentCastDelay = 0;
			}
		}
	}

	VOID SetTactic(INT nCharacter);

	INT GetTactic() { return m_nTactic;}

	BOOL Get_TacticAttrib(INT nQuality, INT nRankLevel, KBASEATTR& Attr, BOOL bIsBlade = FALSE);

	INT GetSkillValue(INT nSkillId, INT nValueIndex);

	VOID GetSkillIds(std::vector<INT>& vecSkillIds);	// 得到宠物技能id列表

	INT  GetBuffIdBySkillId(INT nSkillId);

	INT GetSkillIdByIndex(INT nIndex);

	VOID SetLastState(INT nState) { m_nLastState = nState; }

	VOID SetPosition(INT nX, INT nY);

	INT GetLastState() { return m_nLastState; }

	BOOL ClearSkillLevel();

	BOOL ClearExp();

private:
	BOOL Init_BaseAttrib();  // 初始化基础属性

	BOOL SetSkillValues(INT nSkillId, KSKILL_DATA& sSkillData);

	BOOL Init_TacticAttrib();

	BOOL Init_EquipAttrib(); // 初始化装备属性

	BOOL Init_UprankAttrib(); // 初始化升阶属性 

	BOOL Init_Skills();

	DWORD GetNextLvExp(INT nNextLevel);

	BOOL CopyPetData(CONST KPet* pPet);				// 为了不再使用operator =
	
	CONST CHAR* GetMagicEffectStrBySkillType(INT nSkillType); 

private:
	BOOL IsSkillInCD(INT nSkillId);
	BOOL CheckCastDistance();
	BOOL GetCastSkillId();
	BOOL CastSkill();

private:
	INT m_nType;
	INT m_nQuality;
	INT m_nCharacter;
	INT m_nX;
	INT m_nY;

	CHAR m_szPetName[MAX_NAME_LEN];

	INT m_nLevel;
	DWORD m_dwExp;				// 当前经验
	INT m_nRankLevel;		// 阶数
	DWORD m_dwOwnerId;

	DWORD m_dwRandSeed;
	INT m_nRename;

	BYTE m_byHungry;	// 饱食度

	INT m_nHungryConsume;

	INT m_nFightScore;	// 战斗力

	KBASEATTR m_aryBaseAttr[emPET_COUNT_BASE];	// 基础属性
	KBASEATTR m_TacticAttr;		// 阵法属性


	typedef std::map<INT, KSKILL_DATA> SKILL_DATA_MAP;
	SKILL_DATA_MAP m_mapSkill;	// 技能数据
	std::vector<INT> m_arrSkills;
	int m_nCurSkillIndex;

	INT m_nEquipSlot;		// 装备孔个数
	KPETEQUIP m_aryEquip[emPET_COUNT_EQUIP];	// 宠物装备

	KPetRandom	m_cRandom;

	BYTE m_byState;		// 出战状态

	DWORD m_dwTargetId;

	INT m_nTactic;		// 阵位信息，保存阵位中的性格，不在阵位为0

	INT m_nCurrentSkillId;
	INT m_nCurrentCastDelay;
	INT m_nWaitAfterCast;

	KPetTemplate* m_pPetTemplate;

	INT m_nLastState;		// 上次状态

protected:
	INT m_nIndex; // PetIndex
	std::map<INT, INT> m_mapSkillLevels;

	KLuaPet* m_pLuna; // Lua对象指针

	friend class KPetCtrl;
	friend class KPetModule;
};


#endif