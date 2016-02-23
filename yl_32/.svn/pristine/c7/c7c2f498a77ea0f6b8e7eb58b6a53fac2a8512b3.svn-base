
#pragma once

#include "onlinegameworld/kcharacter.h"

#define KD_FACTION_FIGHT_SCORE_TABLE  "\\setting\\faction\\score.txt"

// -------------------------------------------------------------------------
#define KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, _Name, nValue)							\
	BOOL Change##_Name##Base(KCharacter& rcCharacter, INT nValue) {						\
	rcCharacter.Add##_Name##Base(nValue);												\
	INT nBaseValue = rcCharacter.Get##_Name##Base();									\
	INT nPACValue  = rcCharacter.Get##_Name##PAC();										\
	rcCharacter.Set##_Name(nBaseValue + nBaseValue * nPACValue/1000);					\
	return TRUE;																		\
}																						\
	BOOL Change##_Name##PAC(KCharacter& rcCharacter, INT nValue) {						\
	rcCharacter.Add##_Name##PAC(nValue);												\
	INT nBaseValue = rcCharacter.Get##_Name##Base();									\
	INT nPACValue  = rcCharacter.Get##_Name##PAC();										\
	rcCharacter.Set##_Name(nBaseValue + nBaseValue * nPACValue/1000);					\
	return TRUE;																		\
}																						\

#define KD_CHANGE_CHARACTER_ATTR(rcCharacter, _Name, nValue)							\
	BOOL Change##_Name(KCharacter& rcCharacter, INT nValue) {							\
	rcCharacter.Add##_Name(nValue);														\
	return TRUE;																		\
}																						\


// 属性的评分
struct CHARACTER_ATTRIB_SCORE
{
	INT nLife;            // 生命
	INT nMana;            // 内力
	INT nAttack;          // 攻击
	INT nDefend;          // 防御
	INT nCritical;        // 暴击
	INT nCriticalResist;  // 抗暴
	INT nHit;             // 命中
	INT nMiss;            // 闪避
	INT nSkill;           // 主动技能
	INT nPassiveSkill;    // 被动技能
	INT nIgnoreDef;       // 破防
	INT nIgnoreHarm;      // 免疫伤害
};


class KCharacterHelper
{
public:
	std::map<INT, CHARACTER_ATTRIB_SCORE*> m_mapFactionScore;
public:
	BOOL Init();
	/*KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, PhysicsAttackPower, nValue)
	KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, MagicAttackPower, nValue)
	KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, PhysicsDefence, nValue)
	KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, MagicDefence, nValue)
	KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, Hit, nValue)
	KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, Dodge, nValue)
	//KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, Critical, nValue)
	KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, DeCritical, nValue)

	KD_CHANGE_CHARACTER_PV_ATTR(rcCharacter, MaxLife, nValue)

	KD_CHANGE_CHARACTER_ATTR(rcCharacter, ForbitMove, nValue)
	KD_CHANGE_CHARACTER_ATTR(rcCharacter, ForbitUseItem, nValue)
	KD_CHANGE_CHARACTER_ATTR(rcCharacter, ForbitCastSkill, nValue)
	KD_CHANGE_CHARACTER_ATTR(rcCharacter, ForbitDodge, nValue)
	KD_CHANGE_CHARACTER_ATTR(rcCharacter, Sleeping, nValue)*/

	// 杀死
	BOOL Kill( KCharacter& rcDeathCharacter, DWORD dwKillerCharacterId );
	BOOL SetCurrentLife(KCharacter& rcCharacter, INT nValue);
	BOOL ChangeCurrentLife(KCharacter& rcCharacter, INT nValue = 0, INT nPermillage = 0);
	BOOL SetCurrentMana(KCharacter& rcCharacter, INT nValue);
	BOOL ChangeCurrentMana(KCharacter& rcCharacter, INT nValue = 0, INT nPermillage = 0);
	BOOL SetCombatCamp(KCharacter& rcCharacter, INT nCamp);
	BOOL SetCurrentCombatCamp(KCharacter& rcCharacter, INT nCamp);
	BOOL Revive(KCharacter& rcCharacter, INT nLifePermillage, INT nManaPermillage, int nChoice);
	BOOL OnHurt(KCharacter& rcCharacter, KCharacter& rcCaster, INT nDamage);
	BOOL OnRecoverLife(KCharacter& rcCharacter, KCharacter& rcCaster, INT nRecover);
	BOOL OnDeath(KCharacter& rcCharacter, KCharacter& rcCaster);
	BOOL ChangeMaxMana(KCharacter& rcCharacter, INT nValue);
	BOOL CalcFightScore(KCharacter& rcCharacter);

	DWORD GetNextLvExp(INT nNextLevel);
};
extern KCharacterHelper g_cCharacterHelper;
// -------------------------------------------------------------------------

