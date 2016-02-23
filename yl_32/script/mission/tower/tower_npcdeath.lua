
-- 
local TowerNpc_Death = Npc:GetClass("towernpc_death")

function TowerNpc_Death:OnDeath(pKiller, pNpc)
	Tower:NpcUnLock(pNpc);
	
	self:DropItems(pKiller, pNpc);
end

function TowerNpc_Death:DropItems(pKiller, pNpc)
	local pGame = Tower.tbGames[pKiller.dwId];
	
	if not pGame then
		return;
	end
	
	local mPos = pNpc.GetPosition();
	
	self:GiveExp(pKiller, pNpc);
	
	Npc:DropItems(pNpc.dwTemplateId, pKiller.dwId, mPos, {pKiller.dwId});
end

function TowerNpc_Death:GiveExp(pKiller, pNpc)
	local tbTmp = pNpc.GetTempTable("Tower");
	if not tbTmp then
		return;
	end
	local tbTower = tbTmp.tbTower;
	local nEnemyNum = 10;	-- 默认10
	if tbTower.nEnemyNum then
		nEnemyNum = tbTower.nEnemyNum;
	end
	
	--local nExp = math.floor(Tower:GetLayerTotalExp(pKiller.GetTowerLayer() + 1, pKiller.GetLevel()) / 2);
	local nPropId = KCharacter.GetNpcPropId(pNpc.dwTemplateId);
	local nExp = KCharacter.GetNpcExp(nPropId,pNpc.GetLevel())
	nExp = math.floor(nExp / nEnemyNum);	-- 经验平均分
	pKiller.AddExp(nExp);
end