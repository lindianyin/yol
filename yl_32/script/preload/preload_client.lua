--固定预先加载的脚本，会是所有脚本加载的第一个

if not MODULE_GAMECLIENT then
	return
end
print("Preload client script files...");

Include("/script/npc/npc.lua");
Include("/script/doodad/doodad.lua");

Lib:DelayInherit(_KLuaCharacter, _KLuaPlayer);
Lib:DelayInherit(_KLuaCharacter, _KLuaNpc);

Include("/ui/script/ui.lua");
Include("/script/represent/public/representpublic.lua");
Include("/script/represent/define.lua");
