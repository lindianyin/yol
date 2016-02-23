/* -------------------------------------------------------------------------
//	文件名		：	kpetscript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"GetPetById", LuaGetPetById, "通过Id得到Pet", "d", "PetId;", "o"},
	{"FreePet", LuaFreePet, "放生宠物", "d", "PetId;", ""},

#ifdef GAME_SERVER
	{"AddPlayerPet", LuaAddPlayerPet, "给指定角色一只新的宠物", "?d[d]", "nPlayerId/pPlayer:角色Id或者对象;nType:宠物表现id;nCharacter;", "成功返回宠物对象，失败返回nil"},
	{"GetPlayerPetList", LuaGetPlayerPetList, "得到角色宠物id列表", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"GetPlayerPetMaxPower", LuaGetPlayerPetMaxPower, "得到角色宠物最大战斗力", "?", "nPlayerId/pPlayer:角色Id或者对象;", "" },
	{"GetFightPet", LuaGetFightPet, "得到玩家当前正在出战的宠物", "?", "nPlayerId/pPlayer:角色Id或者对象;", "成功返回宠物对象，失败返回nil"},
	{"AddPlayerRide", LuaAddPlayerRide, "给指定角色一只新的坐骑", "?dddd[dd]", "nPlayerId/pPlayer:角色Id或者对象;nGenre;nDetail;nParticular;nLevel;nHungry;nMagicRate;", "成功返回坐骑对象，失败返回nil"},
	{"TakeOffRide", LuaTakeOffRide, "卸下人物坐骑", "?d", "nPlayerId/pPlayer:角色Id或者对象;坐骑ID;", "成功返回1"},
	{"GetRide", LuaGetRide, "得到玩家的当前坐骑", "?", "nPlayerId/pPlayer:角色Id或者对象;", "成功返回坐骑对象，失败返回nil"},
	{"GetPetNpcIdByType", LuaGetPetNpcIdByType, "获取宠物对应的NPC编号", "d", "nType 宠物ID;", "返回INT"},
	{"GetPetIslandRefreshRate", LuaGetPetIslandRefreshRate, "获取宠物岛刷新成功率", "?d", "nPlayerId/pPlayer:角色Id或者对象;nQuality指定品质;", "成功返回成功率，失败返回nil"},
	{"SetPetIslandRefreshRate", LuaSetPetIslandRefreshRate, "设置宠物岛刷新成功率", "?dd", "nPlayerId/pPlayer:角色Id或者对象;nQuality指定品质;nValue指定值;", "成功返回1，失败返回0"},
	{"GetPetIslandRefreshCount", LuaGetPetIslandRefreshCount, "获取宠物岛刷新过的次数", "?", "nPlayerId/pPlayer:角色Id或者对象;", "成功返回刷新次数，失败返回nil"},
	{"UpgradePetSkill", LuaUpgradePetSkill, "升级神仙技能", "dd", "神仙Id;神仙技能Id;", "成功返回1,失败返回0"},
	{"AddPetIslandRefreshCount", LuaAddPetIslandRefreshCount, "增加宠物岛刷新过的次数+1", "?", "nPlayerId/pPlayer:角色Id或者对象;", "成功返回刷新次数，失败返回nil"},
	{"RecordPetIslandCurrentInfo", LuaRecordPetIslandCurrentInfo, "记录此次召唤神仙信息", "?dddd", "nPlayerId/pPlayer:角色Id或者对象;当前刷出的索引;品质;法系;是否在战斗状态;", ""},
	{"GetPetIslandLastInfo", LuaGetPetIslandLastInfo, "记录此次召唤神仙信息", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"SendRefreshPetInfo", LuaSendRefreshPetInfo, "同步此次翻牌的结果", "??", "nPlayerId/pPlayer:角色Id或者对象;刷出的神仙卡牌", ""},
	{"RecordPetsLastState", LuaRecordPetsLastState, "记录身上宠物的状态", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"ResumePetsLastState", LuaResumePetsLastState, "恢复之前身上宠物状态", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"GetPetByTypeAndNature", LuaGetPetByTypeAndNature, "根据表现id和法系得到神仙", "dd", "nType:宠物表现id;nCharacter:神仙法系", ""},
#endif // #ifdef GAME_SERVER
};
