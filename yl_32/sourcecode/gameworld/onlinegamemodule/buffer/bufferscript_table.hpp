/* -------------------------------------------------------------------------
//	文件名		：	bufferscript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	// 客户端/服务端0长度不能编译通过，放个没用的进来
	{"...", NULL, "", "", "", ""},

#ifdef GAME_SERVER
	{"GetBufferNumber", LuaGetBufferNumber, "获得Buffer数值", "dds", "nSkillId;nSkillLevel;szBufferCommand;", ""},
	{"AddBufferToCharacter", LuaAddBufferToCharacter, "添加buffer到角色", "ddddd", "释放者ID;释放技能模版ID;接受者ID;添加BUFFER;是否同步;", ""},
	{"AddGroundBuffer", LuaAddGroundBuffer, "添加地面buffer", "dddddddd", "BUFFER ID;释放者ID;释放技能模版ID;场景ID;buffer坐标X;buffer坐标Y;buffer坐标Z;是否同步;", ""},
	{"RemoveGroundBufferBySceneId", LuaRemoveGroundBufferBySceneId, "删除场景里所有groundbuffer", "d", "nSceneId;", ""},
	{"RemoveGroundBufferByCasterId", LuaRemoveGroundBufferByCasterId, "删除由某个角色释放的GroundBuffer", "d", "nCasterId;", ""},
#endif // #ifdef GAME_SERVER
};
