/* -------------------------------------------------------------------------
//	文件名		：	luacharacterskill_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(LuaCharacterSkill)
	REGISTER_LUA_FUNC(CastSkill, "释放技能", "dd[ddd]", "技能ID;接受者ID;导弹技的目标点X;导弹技的目标点Y;导弹技的目标点Z;", "")
	REGISTER_LUA_FUNC(GetResistSkillLevel, "获取指定抗性技能等级", "d", "抗性技能类型;", "")
	REGISTER_LUA_FUNC(CanCastSkill, "是否能释放技能", "ddddd", "技能ID;接受者ID;导弹技的目标点X;导弹技的目标点Y;导弹技的目标点Z;", "")
	REGISTER_LUA_FUNC(GetSkillCoefficient, "技能的計算信息(是否物理攻擊、成長值)", "d", "技能ID;", "")
	REGISTER_LUA_FUNC(GetSkillFightInfo, "技能的計算信息(是否物理攻擊、成長值)", "d", "技能ID;", "")
	REGISTER_LUA_FUNC(GetSkillCate, "当前Buff所属技能的Cate信息", "d", "技能ID;", "")
	REGISTER_LUA_FUNC(GetSkillAttackType, "技能的攻击类型(NeedTarget, IsImmediately)", "d", "技能ID;", "")
	REGISTER_LUA_FUNC(ReduceSkillCost, "修改技能的消耗", "dddd", "技能nCateType;技能nCateId;是值还是百分比;修改值;", "")
	REGISTER_LUA_FUNC(ResetSkillCost, "重置技能的消耗", "dd", "技能nCateType;技能nCateId;", "")
	REGISTER_LUA_FUNC(ResetSkillMagnification, "修改技能的攻击力系数", "ddd", "技能nCateType;技能nCateId;修改值;", "")
	REGISTER_LUA_FUNC(CancelChannelSkill, "打断当前正在读条的技能", "", "", "")
	REGISTER_LUA_FUNC(ResetSkillChannelTime, "修改技能的施法时间", "ddd", "技能nCateType;技能nCateId;修改值;", "")
	REGISTER_LUA_FUNC(AddSkillCriticalPercent, "修改技能的暴击率", "ddd", "技能nCateType;技能nCateId;修改值;", "")
	REGISTER_LUA_FUNC(ClearSkillCD, "修改技能CD", "dd", "技能nCateType;技能nCateId;", "")
	REGISTER_LUA_FUNC(ResetSkillChannelType, "修改技能引导类型", "ddd", "技能nCateType;技能nCateId;设置技能释放的类型(0、1、2、-1:设回默认);", "")
	REGISTER_LUA_FUNC(AddBuffToSkill, "修改技能引导类型", "ddd", "技能nCateType;技能nCateId;Buff ID(+:加Buff，-:减Buff);", "")
	REGISTER_LUA_FUNC(CheckObstacle, "判断目标或目标点障碍", "dddddd", "起始点X;起始点Y;起始点Z;结束点X;结束点Y;结束点Z;", "")
DEFINE_LUA_CLASS_END()
