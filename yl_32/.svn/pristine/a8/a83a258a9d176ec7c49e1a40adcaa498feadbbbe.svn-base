
-- 
local ActivityNpc_Drop = Npc:GetClass("activitynpc_drop")

function ActivityNpc_Drop:OnDeath(pKiller, pNpc)
	self:DropItems(pKiller, pNpc);
end

function ActivityNpc_Drop:DropItems(pKiller, pNpc)
	local mPos = pNpc.GetPosition();
	
	Npc:DropItems(pNpc.dwTemplateId, 0, mPos, {});
end