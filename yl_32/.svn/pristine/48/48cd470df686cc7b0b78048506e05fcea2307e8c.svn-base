
local tbNpc = Npc:GetClass("battlezone")

function tbNpc:OnDialog()
	local tbOpt = {
		{Lang.npc.str27[Lang.Idx], self.BattleZoneTransfer, self},
		{Lang.npc.str28[Lang.Idx]},
		};
	if me.GetLevel() < 10 then	
		return Dialog:Say(Lang.npc.str29[Lang.Idx]);
	end
	Dialog:Say(Lang.npc.str30[Lang.Idx], tbOpt);
end

function tbNpc:BattleZoneTransfer()
	me.BattleZoneTransfer(2007, 1756, 1068108);
end
