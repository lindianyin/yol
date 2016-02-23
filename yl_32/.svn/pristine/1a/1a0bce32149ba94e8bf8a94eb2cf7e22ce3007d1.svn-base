local tbNpc = Npc:GetClass("dengtiantajieyin")

function tbNpc:OnDialog()
	local tbOpt = 
	{
		{Lang.npc.str62[Lang.Idx]},
		{Lang.npc.str63[Lang.Idx],self.LeaveMap,self},
	}
	return Dialog:Say(Lang.npc.str64[Lang.Idx],tbOpt);
end;

function tbNpc:LeaveMap()
	XoyoGame:c2s_TryEndDTTGame(me);
	--me.TeleportTo(100,1904,7121,1048576); --´«ËÍ
	--me.SetCurrentCamp(1);
end
