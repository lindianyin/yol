/* -------------------------------------------------------------------------
//	文件名		：	luacharacterskill.h
//	创建者		：	zhangzhixiong
//	实现者		：	
//	创建时间	：	20012-7-6 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef luacharacterskill_h__
#define luacharacterskill_h__


class KCharacter;

// -------------------------------------------------------------------------
class LuaCharacterSkill : public QLunaBase
{
public:
	LuaCharacterSkill(KCharacter& rcCharacter)
		:m_rcCharacter(rcCharacter){}

	static INT Register2Character(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaCharacterSkill);

	INT LuaCastSkill(QLuaScript& sc);

	INT LuaCanCastSkill(QLuaScript& sc);

	INT LuaGetSkillFightInfo(QLuaScript& sc);

	INT LuaGetSkillCoefficient(QLuaScript& sc);

	INT LuaGetSkillCate(QLuaScript& sc);

	INT LuaGetSkillAttackType(QLuaScript& sc);

	INT LuaReduceSkillCost(QLuaScript& sc);

	INT LuaResetSkillCost(QLuaScript& sc);

	INT LuaResetSkillMagnification(QLuaScript& sc);

	INT LuaCancelChannelSkill(QLuaScript& sc);
	INT LuaGetResistSkillLevel(QLuaScript& sc);

	INT LuaResetSkillChannelTime(QLuaScript& sc);

	INT LuaAddSkillCriticalPercent(QLuaScript& sc);

	INT LuaClearSkillCD(QLuaScript& sc);

	INT LuaResetSkillChannelType(QLuaScript& sc);

	INT LuaAddBuffToSkill(QLuaScript& sc);

	INT LuaCheckObstacle(QLuaScript& sc);

private:
	KCharacter& m_rcCharacter;
};

// -------------------------------------------------------------------------

#endif // luacharacterskill_h__
