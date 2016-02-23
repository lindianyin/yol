/* -------------------------------------------------------------------------
//	文件名		：	kluaai_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaAI)
	REGISTER_LUA_FUNC(SetAIType, "设置Npc的AI", "d", "nAIType AI类型;", "")
	REGISTER_LUA_FUNC(GetAIType, "获取AI类型", "", "", "nAIType AI类型")
	REGISTER_LUA_FUNC(FireAIEvent, "触发AI事件", "ddd", "nEventID 事件类型;dwEventSrc 触发事件者ID;dwEventParam;", "")
	REGISTER_LUA_FUNC(GetAIOriginPos, "获取Npc记录的原点", "dddd", "posX;posY;posZ;faceDirection;", "")
	REGISTER_LUA_FUNC(SetAIOriginPos, "记录Npc原点", "dddd", "posX;posY;posZ;faceDirection;", "")
	REGISTER_LUA_FUNC(GetAISelectSkill, "获取释放技能Id", "", "", "dwSkillID,dwSkillLevel")
	REGISTER_LUA_FUNC(GetAIEventSrcId, "获取事件触发者ID", "", "", "nEventSrcId")
	REGISTER_LUA_FUNC(EnableDebug, "启动AI调试输出信息", "d", "nEnable 1 开启 0 关闭;", "")
	REGISTER_LUA_FUNC(SetPatrolPath, "设置巡逻路径", "dd[d]", "nPatrolPath;nNextPointIndex;nOrderIndex;", "")
	REGISTER_LUA_FUNC(SetBattleZone, "设置Npc战斗区域，离开区域会触发AI事件", "ddddd", "x;y;z;radius;height;", "")
DEFINE_LUA_CLASS_END()
