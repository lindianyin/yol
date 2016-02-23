/* -------------------------------------------------------------------------
//	文件名		：	kluapet_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaPet)
	REGISTER_LUA_STRING_READONLY(Name, "宠物名字")
	REGISTER_LUA_DWORD_READONLY(Id, "宠物Id")
	REGISTER_LUA_DWORD_READONLY(RandSeed, "宠物随机种子")
	REGISTER_LUA_DWORD_READONLY(OwnerId, "宠物主人Id")
	REGISTER_LUA_INTEGER_READONLY(Type, "宠物种类")
	REGISTER_LUA_INTEGER_READONLY(Quality, "宠物品质")
	REGISTER_LUA_INTEGER_READONLY(Character, "宠物性格")
	REGISTER_LUA_INTEGER_READONLY(Hungry, "宠物饱食度")
	REGISTER_LUA_INTEGER_READONLY(Level, "宠物等级")
	REGISTER_LUA_INTEGER_READONLY(RankLevel, "宠物阶级")
	REGISTER_LUA_INTEGER_READONLY(State, "宠物状态")
	REGISTER_LUA_FUNC(AddExp, "增加经验", "d", "增加经验值;", "")
	REGISTER_LUA_FUNC(GetTotalExp, "得到宠物的总经验", "", "", "")
	REGISTER_LUA_FUNC(GetBaseAttr, "得到宠物基础属性", "", "", "")
	REGISTER_LUA_FUNC(AddHungry, "增加饱食度", "d", "饱食度;", "")

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(Regenerate, "重新生成一只宠物", "dddddu", "nType		;nQuality	;nCharacter;nLevel;nRankLevel;uRandSeed;", "成功返回1")
	REGISTER_LUA_FUNC(Sync, "同步宠物数据到客户端", "", "", "1成功")
	REGISTER_LUA_FUNC(UnlockEquipPos, "解锁装备格子", "d", "装备槽位置;", "1成功")
	REGISTER_LUA_FUNC(GetSkillLevel, "得到技能等级", "d", "技能id;", "1成功")
	REGISTER_LUA_FUNC(GetBaseAttrScore, "得到基础属性战斗力", "", "", "")
	REGISTER_LUA_FUNC(SetFightScore, "设置宠物战斗力", "d", "nScore;", "")
	REGISTER_LUA_FUNC(DoRest, "设置当前出战神仙休息", "", "", "")
#endif // #ifdef GAME_SERVER
DEFINE_LUA_CLASS_END()
