--固定预先加载的脚本，会是所有脚本加载的第一个

-- GS专用
if not MODULE_GAMESERVER then
	return
end

print("Preload gs script files...");

Include("/script/npc/npc.lua");
Include("/script/doodad/doodad.lua");
Include("/script/task/task.lua");

Lib:DelayInherit(_KLuaCharacter, _KLuaPlayer);
Lib:DelayInherit(_KLuaCharacter, _KLuaNpc);
