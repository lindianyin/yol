
-- 

local XoyoNpc_Death = Npc:GetClass("xnhjnpc_death")

function XoyoNpc_Death:OnDeath(pKiller, pNpc)
	XoyoGame:NpcUnLock(pNpc);
	XoyoGame.XoyoChallenge:KillNpcForCard(pKiller, pNpc);
	
	--³É¾Í
	--[[local pPlayer = pKiller.GetPlayer(); --zzx
	if pPlayer then
		Achievement:FinishAchievement(pPlayer, 200);
	end]]
	
	self:DropItems(pKiller, pNpc);
end

function XoyoNpc_Death:DropItems(pKiller, pNpc)
	local pGame = XoyoGame:GetMyGame(pKiller);
	
	if not pGame then
		return;
	end
	
	local mPos = pNpc.GetPosition();
	
	local tbSetting = XoyoGame.RoomSetting.tbRoom[pGame.nCurRoomId];
	local bTeamDrop = 0;
	for _, Info in ipairs(tbSetting.NPC) do
		if Info.nTemplate == pNpc.dwTemplateId then
			bTeamDrop = Info.bTeamDrop;
			break;
		end
	end
	
	local mPos = pNpc.GetPosition();
	
	if bTeamDrop and bTeamDrop == 0 then
		Npc:DropItems(pNpc.dwTemplateId, pKiller.dwId, mPos, 1);
	else
		local nTeamId = pKiller.GetTeamId();
		local tbMembers, nCount = KTeam.GetTeamMemberList(nTeamId);
		for _, nId in pairs(tbMembers) do
			Npc:DropItems(pNpc.dwTemplateId, nId, mPos, 1);
		end
	end
end

--?pl DoScript("\\script\\mission\\xoyogame\\npc\\xoyonpc_death.lua")

