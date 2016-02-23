
local tbNpc = Npc:GetClass("pkchangjieyinren2")

function tbNpc:OnDialog()
	local tbOpt = 
	{
		{Lang.npc.str88[Lang.Idx],self.GoToPK,self},
		--{"我要进入60级战魂之滨",self.GoToPK,self},
		{Lang.npc.str84[Lang.Idx]},
	}
	Dialog:Say(Lang.npc.str90[Lang.Idx],tbOpt);
end;

function tbNpc:GoToPK()
	local tbOpt = 
	{
		{Lang.npc.str91[Lang.Idx],self.TeleportToPK,self,1},
		{Lang.npc.str92[Lang.Idx],self.TeleportToPK,self,2},
		{Lang.npc.str93[Lang.Idx],self.WatchPK,self},
	}
	if me.GetLevel() < 50 then
		Dialog:Say(Lang.npc.str94[Lang.Idx]);
	else
		Dialog:Say(Lang.npc.str95[Lang.Idx],tbOpt);
	end
end

function tbNpc:TeleportToPK(nIdx)
	--me.pTask.SetTask(2052,1,me.GetLevel());
	me.TeleportTo(unpack(self.tbPKTelePos[nIdx]));
	me.SetCurrentCamp(nIdx);
	--me.SetLevel(nLevel);
	--me.SetCurrentLife(me.GetMaxLife());
	--me.SetCurrentMana(me.GetMaxMana());
end

function tbNpc:WatchPK()
	--me.pTask.SetTask(2052,1,me.GetLevel());
	Dialog:Say(Lang.npc.str96[Lang.Idx]);
	--me.SetLevel(nLevel);
	--me.SetCurrentLife(me.GetMaxLife());
	--me.SetCurrentMana(me.GetMaxMana());
end

tbNpc.tbPKTelePos =
{
	[1] = {800,2723, 6287, 1048576},  --战神堂
	[2] = {800,6287, 5682, 1048576},  --武魂居
}
