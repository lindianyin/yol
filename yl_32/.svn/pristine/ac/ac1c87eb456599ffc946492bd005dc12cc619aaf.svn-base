
-- ¿Õ°×NpcÄ£°å

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

local tbNpc	= Npc:GetClass("manhuangzhidi_trap");

function tbNpc:OnDialog()
	Dialog:_AskForSure(me.dwId, Lang.npc.str79[Lang.Idx], ({"Activity:LeaveGame", me}));
end

function tbNpc:OnDeath()
end
