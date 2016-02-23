
local tbNpc = Npc:GetClass("getcardbook")

function tbNpc:OnDialog()
	local tbOpt = 
	{
		{Lang.npc.str71[Lang.Idx]},
		{Lang.npc.str72[Lang.Idx],self.GetCardBook, self},
	}
	return Dialog:Say(Lang.npc.str73[Lang.Idx],tbOpt);
end;

function tbNpc:GetCardBook()
	local tbGDPL = { 18, 1, 1 };	-- 幻化录
	
	local tbFind = me.pItem.FindItem(Item.ROOM_MAINBAG, unpack(tbGDPL));
	if (#tbFind >= 1) then
		me.SysMsg(Lang.npc.str74[Lang.Idx]);
		return;
	end
	
	if me.pItem.CountFreeBagCell() < 1 then
		me.SysMsg(Lang.npc.str75[Lang.Idx]);
		return;
	end
	
	KItem.AddPlayerItem(me, unpack(tbGDPL));
	me.SysMsg(Lang.npc.str76[Lang.Idx]);
end


--[[
function tbNpc:OnDialog()
	local tbOpt = 
	{
		{"领取适合装备",self.GetEquip,self},
		{"我要离开pk场",self.LeaveMap,self},
		{"取消"},
	}
	return Dialog:Say("你想要做什么",tbOpt);
end;

function tbNpc:LeaveMap()
	me.SetLevel(me.pTask.GetTask(2052,1) or me.GetLevel());
	me.TeleportTo(100,1904,7121,1048576);
	me.SetCurrentCamp(1);
end
--]]
