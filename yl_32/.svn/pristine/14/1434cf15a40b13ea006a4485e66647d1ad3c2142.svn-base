
-- 

local ActivityNpc_Death = Npc:GetClass("activitynpc_death")

function ActivityNpc_Death:OnDeath(pKiller, pNpc)
	Activity:NpcUnLock(pNpc);
	if pKiller.IsNpc() == 1 then
		return;
	end
	Activity:OnNpcDeath(pKiller, pNpc);
	self:DropItems(pKiller, pNpc);
end

function ActivityNpc_Death:DropItems(pKiller, pNpc)

end