
local tbNpc = Npc:GetClass("createtongnpc")
function tbNpc:OnDialog()
	local tbOpt =
	{
		{Lang.npc.str56[Lang.Idx],self.ShowInputTongNamePanel,self},
		{Lang.npc.str57[Lang.Idx]},
	}
	Dialog:Say(Lang.npc.str58[Lang.Idx],tbOpt);
end

local KIN_OPEN_MIN_LV = 10;

function tbNpc:ShowInputTongNamePanel()
	-- 判断家族开启等级
	if me.GetLevel() < KIN_OPEN_MIN_LV then
		me.SysMsg(string.format(Lang.npc.str59[Lang.Idx], KIN_OPEN_MIN_LV));
		return;
	end
	-- 判断玩家是否有家族
	if me.dwKinId ~= 0 then
		me.SysMsg(Lang.npc.str60[Lang.Idx]);
		return;
	end
	Dialog:AskString(Lang.npc.str61[Lang.Idx], 3, 8, "Kin:c2s_CreateTong");
end

