-- File       : test_trapscript1.lua
-- Creator    : simon
-- Date       : 2010-03-25 13:04:27
-- Description: 

Include("script/scene/scene.lua")

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

local tbTrap = Scene:GetTrapClass("test_script")

function tbTrap:OnPlayer()
	local tbOpt = 
	{
		{Lang.scene.str7[Lang.Idx]},
		{"...."},
	}
	Dialog:Say(Lang.scene.str8[Lang.Idx], tbOpt);
end
