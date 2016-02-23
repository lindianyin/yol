--
-- FileName: complementalnpc.lua
-- Author: panhui
-- Time: 2010/11/19 9:59
-- Comment: 补足NPC管理类
--

if not MODULE_GAMESERVER then
	return
end

Include("/script/npc/define.lua")

if not Npc.ComplementalNpc then
	Npc.ComplementalNpc = {}
end

local ComplementalNpc = Npc.ComplementalNpc

function ComplementalNpc:GetComplementalNpc4Plot(nAddNpcGroupId, tbCharacter, nRandomSeed)
	local tbAddNpc = {}
	local nHCount = 0
	local nDCount = 0
	local nCharacterCount = 0
	local nSeed = nRandomSeed
	local nNeedHealth = Npc.CHARACTERCOUNT.emHEALTH_COUNT
	local nNeedDamage = Npc.CHARACTERCOUNT.emDAMAGE_COUNT	
	
	tbAddNpc = self:GetComplementalNpc(nAddNpcGroupId, tbCharacter, nRandomSeed,  nNeedHealth, nNeedDamage) or {}
	return tbAddNpc
end

function ComplementalNpc:GetComplementalNpc4Battle(nAddNpcGroupId, tbCharacter, nRandomSeed)
	local tbAddNpc = {}
	local nNeedHealth = Npc.CHARACTERCOUNT.emHEALTH_COUNT
	local nNeedDamage = Npc.CHARACTERCOUNT.emDAMAGE_COUNT
	local nNeedCharacterCount = 0
	
	-- 临时：根据角色buffer判断添加的助手个数和类型		by：zhaokan
	local pPlayer = KGameBase.GetPlayerById(tbCharacter[1])
	if pPlayer then
		for _, BufferId in pairs(pPlayer.pBufferList.GetBufferList()) do
			if BufferId == 50001 then
				nNeedDamage = nNeedDamage - 1
			end
			
			if BufferId == 50002 then
				nNeedHealth = nNeedHealth - 1
			end
			
			if BufferId == 50003 then
				nNeedDamage = nNeedDamage - 1
			end
		end
	end
	
	tbAddNpc = self:GetComplementalNpc(nAddNpcGroupId, tbCharacter, nRandomSeed,  nNeedHealth, nNeedDamage) or {}
	return tbAddNpc
end

function ComplementalNpc:GetComplementalNpc(nAddNpcGroupId, tbCharacter, nRandomSeed, nNeedHealth, nNeedDamage)
	local tbAddNpc = {}
	local nHCount = 0
	local nDCount = 0
	local nCharacterCount = 0
	local nSeed = nRandomSeed	
	local nNeedCharacterCount = nNeedDamage + nNeedHealth
	
	for __, nPlayerId in pairs(tbCharacter) do
		local pMemPlayer = KGameBase.GetPlayerById(nPlayerId)
		if pMemPlayer then
			nCharacterCount = nCharacterCount + 1
			local nPriFactionId = pMemPlayer.GetPrimaryFaction()
			local nFactionType = Faction:GetFactionType(nPriFactionId)
			if (nFactionType == Faction.emFACTION_TYPE_HEALTH) then
				nHCount = nHCount + 1
			elseif (nFactionType == Faction.emFACTION_TYPE_DAMAGE) then
				nDCount = nDCount + 1
			end
		else
			Dbg:WriteLogEx(Dbg.LOG_INFO, "NPC.ComplementalNpc", "忽略, 发现离线或者死亡的玩家", nPlayerId);
		end
	end
	
	if nCharacterCount < nNeedCharacterCount then
		local tbAddNpcGroup = Npc.tbAddGroups[nAddNpcGroupId]
		if tbAddNpcGroup then
			if nHCount < nNeedHealth then
				local tbHNpc = Npc:GetNpcIdByGroup(nAddNpcGroupId, 1, Npc.emNPCGROUPTYPE_ADDGROUPS)
				if tbHNpc and #tbHNpc > 0 then
					for i = nHCount + 1, nNeedHealth do
						local nIdx = nSeed % #tbHNpc + 1
						table.insert(tbAddNpc, tbHNpc[nIdx])
						nCharacterCount = nCharacterCount + 1
						nSeed = nSeed + 1
					end
				end
			end
			nSeed = nRandomSeed
			if nCharacterCount < nNeedCharacterCount then
				local tbDNpc = Npc:GetNpcIdByGroup(nAddNpcGroupId, 2, Npc.emNPCGROUPTYPE_ADDGROUPS)
				if tbDNpc and #tbDNpc > 0 then
					for i = 1, nNeedCharacterCount - nCharacterCount do
						local nIdx = nSeed % #tbDNpc + 1
						table.insert(tbAddNpc, tbDNpc[nIdx])
						nSeed = nSeed + 1
					end
				end
			end
		end
	end
	
	return tbAddNpc
end