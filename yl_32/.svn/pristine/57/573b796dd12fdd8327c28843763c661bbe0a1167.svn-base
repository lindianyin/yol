-- File       : mapcopylevelrule.lua
-- Creator    : liyun
-- Date       : 2012-10-23 10:25:32
-- Description: 玩家匹配副本等级规则逻辑实现

-- c2s func
-- 获取这个玩家适合的副本索引，会考虑组队的情况
--------------------------------------------------------------------------------
function PVEMatch:c2s_GetSuitableInstanceId(pPlayer)
	local nTeamId = pPlayer.GetTeamId();
	if (nTeamId > 0) then
		local tbTeamLevel = {};
		local tbTeamMem = KTeam.GetTeamMemberList(nTeamId);
		for _, nPlayerId in pairs(tbTeamMem) do
			local pTeamPlayer = KGameBase.GetPlayerById(nPlayerId);
			if (pTeamPlayer) then
				table.insert(tbTeamLevel, pTeamPlayer.GetLevel());
			end
		end
		
		local tbResult = PVEMatch:GetTeamSuitableInstanceIndexes(tbTeamLevel);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowInstanceLevelMenu", tbResult});
	else
		local tbResult = PVEMatch:GetPlayerSuitableInstanceIndexes(pPlayer.GetLevel());
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowInstanceLevelMenu", tbResult});
	end
end

-- logic
--------------------------------------------------------------------------------
-- 获得单个玩家的适合副本
function PVEMatch:GetPlayerSuitableInstanceIndexes(nPlayerLevel, bInTeam)
	-- 遍历所有的任务配置文件，适合的加入到列表

	bInTeam = bInTeam or 0;

	local tbResult = {};
	local tbMapCopyLevelConfig = self:GetInstanceLevelConfig();
	for mapCopyId, tbConfig in pairs(tbMapCopyLevelConfig) do
		if ((bInTeam == 0 and tbConfig.Single == 1) or (bInTeam == 1 and tbConfig.Team == 1)) and
			(nPlayerLevel >= tbConfig.MinLevel and nPlayerLevel <= tbConfig.MaxLevel) 
		then
			table.insert(tbResult, mapCopyId);
		end
	end
	
	return tbResult;
end

-- 获得组队玩家的适合副本
-- tbTeamLevelSample = {10, 20, 30}，为玩家级别
function PVEMatch:GetTeamSuitableInstanceIndexes(tbTeamLevel)
	-- 遍历每个玩家然后再合并结果，并返回
	local tbResult = {};
	local bFirst = 1;
	for _, nLevel in pairs(tbTeamLevel) do 
		if bFirst == 1 then
			tbResult = self:GetPlayerSuitableInstanceIndexes(nLevel, 1);
			bFirst = 0;
		else
			if #tbResult > 0 then
				local r1 = self:GetPlayerSuitableInstanceIndexes(nLevel, 1);
				-- tbResult和r1取交，并存到tbResult
				-- todo 这个取交算法复杂度是n^2，数量多了可能会影响性能
				local inter = {};
				for _,v1 in pairs(tbResult) do
					for _,v2 in pairs(r1) do
						if v1 == v2 then
							table.insert(inter, v1);
						end
					end
				end
				tbResult = inter;
			else
				return {}; -- 如果没有适合的副本则不必继续比较了，作一个优化
			end
		end
	end
	return tbResult;
end

-- 取副本配置表
function PVEMatch:GetInstanceLevelConfig()
	if self.tbMapCopyConfig == nil then

		self.tbMapCopyConfig = {};
		local tbData = Lib:LoadTabFile("/setting/pvematch/instancename.txt");
		for _, row in pairs(tbData) do
			row.Id = tonumber(row.Id) or 0;
			row.MinLevel = tonumber(row.MinLevel) or 0;
			row.MaxLevel = tonumber(row.MaxLevel) or 0;
			row.Single = tonumber(row.Single) or 0;
			row.Team = tonumber(row.Team) or 0;
			self.tbMapCopyConfig[row.Id] = row;
		end
	end
	
	return self.tbMapCopyConfig;
end
