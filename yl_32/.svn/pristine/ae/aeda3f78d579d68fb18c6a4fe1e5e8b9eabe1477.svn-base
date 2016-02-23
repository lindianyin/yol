
-- 
local MultiInstanceNpc_Death = Npc:GetClass("multiinstance_npc_death")

function MultiInstanceNpc_Death:OnDeath(pKiller, pNpc)
	local tbTmp = pNpc.GetTempTable("MultiInstance");
	local pGame = tbTmp.tbInstance;
	assert(pGame);
	pGame:OnKillNpc(pKiller, pNpc);
	
	for _, nPlayerId in ipairs(pGame.tbGamePlayers) do
        local pPlayer = KGameBase.GetPlayerById(nPlayerId)
		if pPlayer then
			self:DropItems(pPlayer, pNpc);
		end
	end
	
	MultiInstance:NpcUnLock(pNpc);
end

function MultiInstanceNpc_Death:DropItems(pKiller, pNpc)
	local mPos = pNpc.GetPosition();
	
	Npc:DropItems(pNpc.dwTemplateId, pKiller.dwId, mPos, {pKiller.dwId});
end
