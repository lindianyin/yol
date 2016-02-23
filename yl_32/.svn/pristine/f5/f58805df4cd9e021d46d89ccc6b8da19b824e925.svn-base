/********************************************************************
	created:	2012/06/20 11:58
	file base:	luabuffereffect.h
	author:		zhangjunjie
*********************************************************************/

#ifndef luabuffereffect_h__
#define luabuffereffect_h__


class CharacterBuffer;
class KCharacter;

// -------------------------------------------------------------------------
class LuaCharacterBufferEffect : public QLunaBase
{
public:
	LuaCharacterBufferEffect(KCharacter& rcCharacter)
		:m_rcCharacter(rcCharacter){}

	static INT Register2Character(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaCharacterBufferEffect);

	// -------------------------------------------------------------------------
	INT	LuaDamageLifeV(QLuaScript& sc);
	INT	LuaDamageLifeP(QLuaScript& sc);
	INT	LuaDamageManaV(QLuaScript& sc);
	INT	LuaDamageManaP(QLuaScript& sc);
	INT	LuaStun(QLuaScript& sc);
	INT	LuaFix(QLuaScript& sc);
	INT	LuaFear(QLuaScript& sc);
	INT	LuaSilence(QLuaScript& sc);
	INT	LuaCrazy(QLuaScript& sc);
	INT	LuaSlowP(QLuaScript& sc);

	INT	LuaKnock(QLuaScript& sc);
	INT	LuaJumpTo(QLuaScript& sc);
	INT	LuaAddCurLifeV(QLuaScript& sc);
	INT	LuaAddCurLifeP(QLuaScript& sc);
	INT	LuaAddCurManaV(QLuaScript& sc);
	INT	LuaAddCurManaP(QLuaScript& sc);
	INT	LuaAddCurPowerV(QLuaScript& sc);
	INT	LuaAddCurPowerP(QLuaScript& sc);
	INT	LuaDoDeath(QLuaScript& sc);
	INT	LuaDoRevive(QLuaScript& sc);
	INT	LuaGetCurrentBufferStack(QLuaScript& sc);
	INT	LuaClearBuff(QLuaScript& sc);
	INT	LuaHasBuff(QLuaScript& sc);
	INT	LuaClearCurrentBuff(QLuaScript& sc);
	INT	LuaSetCurrentBuffer(QLuaScript& sc);
	INT	LuaClearBuffByType(QLuaScript& sc);

	INT	LuaClearCD(QLuaScript& sc);
	INT	LuaPullTarget(QLuaScript& sc);
	INT	LuaSetMapPos(QLuaScript& sc);
	INT	LuaTp(QLuaScript& sc);
	INT	LuaIgnorarmorV(QLuaScript& sc);
	INT	LuaAddBuffTime(QLuaScript& sc);
	INT	LuaRefreshBuffTime(QLuaScript& sc);
	INT	LuaModifyDmgP(QLuaScript& sc);
	INT	LuaModifyDmgV(QLuaScript& sc);

	INT	LuaAddConst(QLuaScript& sc);
	INT	LuaAddStrth(QLuaScript& sc);
	INT	LuaAddAgile(QLuaScript& sc);
	INT	LuaAddIntel(QLuaScript& sc);
	INT	LuaAddSpirt(QLuaScript& sc);
	INT	LuaAddAllAttributeV(QLuaScript& sc);
	INT	LuaAddAllAttributeP(QLuaScript& sc);
	INT	LuaAddMaxLifeV(QLuaScript& sc);
	INT	LuaAddMaxLifeP(QLuaScript& sc);
	INT	LuaAddMaxManaV(QLuaScript& sc);

	INT	LuaAddMaxManaP(QLuaScript& sc);
	INT	LuaAddMaxPowerV(QLuaScript& sc);
	INT	LuaAddMaxPowerP(QLuaScript& sc);
	INT	LuaAddHitRateV(QLuaScript& sc);
	INT	LuaAddCriticalRateV(QLuaScript& sc);
	INT	LuaAddSkillCriticalRateV(QLuaScript& sc);
	INT	LuaAddCriticalDmgP(QLuaScript& sc);
	INT	LuaAddRunSpeedV(QLuaScript& sc);
	INT	LuaAddArmorV(QLuaScript& sc);
	INT	LuaAddDodgeV(QLuaScript& sc);

	INT	LuaHideToNpc(QLuaScript& sc);
	INT	LuaHideToPlayer(QLuaScript& sc);
	INT	LuaAddThreat(QLuaScript& sc);
	INT	LuaReboundDmgP(QLuaScript& sc);
	INT	LuaReboundDmgV(QLuaScript& sc);
	INT	LuaReduceDmg(QLuaScript& sc);
	INT	LuaReduceDmgByMana(QLuaScript& sc);
	INT	LuaTransDmgToLife(QLuaScript& sc);
	INT	LuaTransDmgToMana(QLuaScript& sc);
	INT	LuaForceAttack(QLuaScript& sc);

	INT	LuaChangeChannelType(QLuaScript& sc);
	INT	LuaReduceChannelTime(QLuaScript& sc);
	INT	LuaAddBuff(QLuaScript& sc);
	INT	LuaAddBuffToSender(QLuaScript& sc);
	INT	LuaKillSelf(QLuaScript& sc);
	INT	LuaIgnorBuff(QLuaScript& sc);
	INT	LuaIgnorBuffGroup(QLuaScript& sc);
	INT	LuaIgnorBuffCate(QLuaScript& sc);
	INT	LuaCastSkill(QLuaScript& sc);
	INT	LuaAddTreatLifeV(QLuaScript& sc);

	INT	LuaAddTreatLifeP(QLuaScript& sc);
	INT	LuaAddTreatManaV(QLuaScript& sc);
	INT	LuaAddTreatManaP(QLuaScript& sc);
	INT	LuaReduceCostV(QLuaScript& sc);
	INT	LuaReduceCostP(QLuaScript& sc);
	INT	LuaIgnorDmg(QLuaScript& sc);
	INT	LuaAddExtSkillBuff(QLuaScript& sc);
	INT	LuaDelExtSkillBuff(QLuaScript& sc);
	INT	LuaAddMagnification(QLuaScript& sc);
	INT	LuaAddWeaponDmgP(QLuaScript& sc);

	INT LuaGetCurrProcBuffFightInfo(QLuaScript& sc);
	INT LuaGetGroundBuffPosition(QLuaScript& sc);
	INT LuaResetBufferTime(QLuaScript& sc);
	INT LuaRefreshBufferTime(QLuaScript& sc);
	INT LuaAppendData(QLuaScript& sc);
	INT LuaGetAppendData(QLuaScript& sc);
	INT LuaSetTransParam(QLuaScript& sc);
	INT LuaGetTransParam(QLuaScript& sc);
	INT LuaGetDestPosition(QLuaScript& sc);
private:
	KCharacter& m_rcCharacter;
};

// -------------------------------------------------------------------------


#endif // luabuffereffect_h__
