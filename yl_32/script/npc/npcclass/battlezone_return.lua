
local tbNpc = Npc:GetClass("battlezone_return")

function tbNpc:OnDialog()
	local tbOpt = {
		{Lang.npc.str31[Lang.Idx], self.BattleZoneReturn, self},
		{Lang.npc.str32[Lang.Idx]},
		};
	
	Dialog:Say(Lang.npc.str33[Lang.Idx], tbOpt);
end

function tbNpc:BattleZoneReturn()
	me.BattleZoneReturn(100, 4378, 2819, 1048576);
end
