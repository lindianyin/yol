Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.MENPAIJINGJI_YUXUANSAI = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.MENPAIJINGJI_YUXIANSAI] = Activity.MENPAIJINGJI_YUXUANSAI -- 注册到活动工厂

local MENPAIJINGJI_YUXUANSAI = Activity.MENPAIJINGJI_YUXUANSAI

function MENPAIJINGJI_YUXUANSAI:OnInit()
	self.tbPlayerKillCount = {}
end

function MENPAIJINGJI_YUXUANSAI:OnJoinPlayer(nPlayerId, nGroup)
	for _, tbKillCount in pairs(self.tbPlayerKillCount) do
	    if tbKillCount.nPlayerId == nPlayerId then -- 已经存在
	    	print(Lang.mission.str76[Lang.Idx], nPlayerId)
	    	return
	    end
	end
	table.insert(self.tbPlayerKillCount, {nPlayerId = nPlayerId, nKillCount = 0})
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	pPlayer.SetFactionBattleRank(32) -- 参加预选赛但是还没晋级的玩家统一设为32
	self:SetPlayerActivityInfo(nPlayerId, nil, Lang.mission.str79[Lang.Idx])
	print(Lang.mission.str77[Lang.Idx], nPlayerId)
end

function MENPAIJINGJI_YUXUANSAI:OnKickPlayer(nPlayerId)
	self:SetPlayerActivityInfo(nPlayerId, "", "")
	for index, tbKillCount in pairs(self.tbPlayerKillCount) do
	    if tbKillCount.nPlayerId == nPlayerId then
	    	table.remove(self.tbPlayerKillCount, index)
	    	break;
	    end
    end
end

function MENPAIJINGJI_YUXUANSAI:AddPlayerKillCount(nPlayerId, nNum)
	for _, tbKillCount in pairs(self.tbPlayerKillCount) do
	    if tbKillCount.nPlayerId == nPlayerId then
	    	tbKillCount.nKillCount = tbKillCount.nKillCount + nNum
	    	return tbKillCount.nKillCount
	    end
    end
    return 0
end

function MENPAIJINGJI_YUXUANSAI:OnPlayerKill(nKillerCharId, nDeathCharId)
	local nCurrKillCount = self:AddPlayerKillCount(nKillerCharId, 1)
	--self.tbPlayerKillCount[nKillerCharId] =  self.tbPlayerKillCount[nKillerCharId] + 1
	
	local getNum = function(tbMap) 
		local count = 0; 
		for id, val in pairs(tbMap) do 
			count = count + 1 
		end;
	    return count; 
	end
	local rankIndex = 0
   
	local sortFunc = function(a, b) return b.nKillCount < a.nKillCount end
	table.sort(self.tbPlayerKillCount, sortFunc)
	
	for rankIndex, tbKillCount in pairs(self.tbPlayerKillCount) do
		self:SetPlayerActivityInfo(tbKillCount.nPlayerId, nil, string.format(Lang.mission.str78[Lang.Idx], tbKillCount.nKillCount * 5, rankIndex)) -- 杀一人得5积分
    end
end

function MENPAIJINGJI_YUXUANSAI:OnEndStep()
	local winnerCount = 0;
	for _, tbKillCount in pairs(self.tbPlayerKillCount) do
		winnerCount = winnerCount + 1;
		self:AddWinner(tbKillCount.nPlayerId, 0);
		if winnerCount >= 16 then
			break;
		end
    end
	--数据重置
	self.tbPlayerKillCount = {};	
end

-- 播放结束特效
function MENPAIJINGJI_YUXUANSAI:ShowEndEffect()
	local winnerCount = 0;
	for _, tbKillCount in pairs(self.tbPlayerKillCount) do
		winnerCount = winnerCount + 1;
		if winnerCount < 16 then
			local pPlayer = KGameBase.GetPlayerById(tbKillCount.nPlayerId);
			if pPlayer then
				self:ShowEffect(pPlayer, 2);	--成功晋级特效
				KActivity.SetLastMPJJRank(pPlayer, 16);
			end
		else
			local pPlayer = KGameBase.GetPlayerById(tbKillCount.nPlayerId);
			if pPlayer then
				self:ShowEffect(pPlayer, 3);	--失败淘汰特效
				KActivity.SetLastMPJJRank(pPlayer, 17);--16强开外都统一为17名
				pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_MPJJ_SCORE, 5);	--16强开外都统一奖励5积分
				KRank.ModifyRankValue(pPlayer.dwId, Rank.MPJJ_RANK, pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_MPJJ_SCORE));
			end
		end
    end
end

function MENPAIJINGJI_YUXUANSAI:ShowStartEffect()
	for _, tbKillCount in pairs(self.tbPlayerKillCount) do
		local pPlayer = KGameBase.GetPlayerById(tbKillCount.nPlayerId);
		if pPlayer then
			self:ShowEffect(pPlayer, 1);	--开始特效
		end
	end
end
