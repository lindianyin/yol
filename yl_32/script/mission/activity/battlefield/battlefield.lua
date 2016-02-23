Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.BattleField = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.BATTLE_FIELD] = Activity.BattleField -- 注册到活动工厂

local BattleField = Activity.BattleField

BattleField.ONE_NPC_SCORE 		= 5;	--击杀一个npc获得积分数
BattleField.ONE_PLAYER_SCORE 	= 10;	--击杀一个玩家获得积分数
BattleField.ONE_TOWER_SCORE 	= 15;	--击杀一个炮塔获得积分数
BattleField.ONE_BASE_SCORE 		= 30;	--击杀一个基地获得积分数
BattleField.NPC_START_TEMPLATE_ID 	= 2571;	--战场npc起始ID
BattleField.NPC_END_TEMPLATE_ID 	= 2710;	--战场npc结束ID
BattleField.TOWER_START_TEMPLATE_ID = 2546; --战场炮塔起始ID
BattleField.TOWER_END_TEMPLATE_ID 	= 2570;	--战场炮塔结束ID
BattleField.BASE_START_TEMPLATE_ID 	= 2540;	--战场基地起始ID
BattleField.BASE_END_TEMPLATE_ID 	= 2545;	--战场基地结束ID

function BattleField:OnInit()	
	self.tbPlayerNpcScores = {};	--玩家击杀npc得分
	self.tbPlayerPlayerScores = {};	--玩家击杀玩家得分
	self.tbPlayerTowerScores = {};	--玩家击杀炮塔得分
	self.tbPlayerBaseScores = {};	--玩家击杀基地得分
	self.tbPlayerRanks = {};			--玩家积分排行榜
	self.tbPlayerScore = {};		--玩家总积分表
	self.nProtossCampScores = 0;	--神族总积分
	self.nEvilCampScores = 0;		--魔族总积分
	self.dwProtossCampScoresUpdateTime = 0;	--神族总积分更新时间
	self.dwEvilCampScoresUpdateTime = 0;	--魔族总积分更新时间
	self.dwProtossBaseKilledTime = 0; --神族基地被击杀时间 
	self.dwEvilBaseKilledTime = 0;	--魔族基地被击杀时间
	self.nProtossCampCount = 0;		-- 神族人数
	self.nEvilCampCount = 0;		-- 魔族人数
end

-- 加入战场，传递信息, 初始化
function BattleField:OnEnterActivityScene(nPlayerId)
	print("!!!!!!!!!!!BattleField JoinPlayer. Group: ", nGroup);
	self.tbPlayerNpcScores[nPlayerId] = 0;
	self.tbPlayerPlayerScores[nPlayerId] = 0;
	self.tbPlayerTowerScores[nPlayerId] = 0;
	self.tbPlayerBaseScores[nPlayerId] = 0;
	self.tbPlayerScore[nPlayerId] = {nScore = 0, dwUpateTime = 0};
	
	-- 战场积分清0
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	pPlayer.SetGlobalBattleScore(0);
end

function BattleField:OnStartStep()
	self:BroadcastCampCount();
	-- 全部已经加入，排名刷新
	self:RefreshAllPlayerRanks() 
	-- 发送开始信息和排名给客户端
	for nPlayerId, tbScore in pairs(self.tbPlayerScore) do
		--self:SendPlayerRankRecord(nPlayerId);排名刷新时已经发送过
		--self:SendPlayerMeInfo(nPlayerId);
		self:SendPlayerMeRecord(nPlayerId, 0);
		self:SendPlayerCampRecord(nPlayerId);
		self:SendCampCount(nPlayerId);
	end
  
  for _, tbPlayer in pairs(self.tbPlayers) do
		-- 加入神、魔buff特效
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
	
		if pPlayer then
	    if pPlayer.GetCurrentCamp() == 4 then
	      self:SetPlayerActivityInfo(tbPlayer.nPlayerId, nil, Lang.mission.str11[Lang.Idx]);	
	      BufferApi.AddBufferToCharacter(pPlayer.dwId, 0, pPlayer.dwId, 1170, 1)
	    elseif pPlayer.GetCurrentCamp() == 5 then
	      self:SetPlayerActivityInfo(tbPlayer.nPlayerId, nil, Lang.mission.str12[Lang.Idx]);	
	      BufferApi.AddBufferToCharacter(pPlayer.dwId, 0, pPlayer.dwId, 1171, 1)
	    end
	    
		end
	end
	
	--播开始特效
	--self:ShowStartEffect();
end

function BattleField:OnPlayerKill(nKillerCharId, nDeathCharId)
	local pKiller = KGameBase.GetPlayerById(nKillerCharId);
	local pDeath = KGameBase.GetPlayerById(nDeathCharId);
	
	if pKiller and pDeath then
		self:AddPlayerBattleScore(pKiller.dwId, 1);
	end
end

function BattleField:AddPlayerBattleScore(nPlayerId, nScoreType)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer or self.IsOpen == 0 then
		return;
	end
	
	local nScore = 0;
	if nScoreType == 1 then	--敌方玩家
		nScore = self.ONE_PLAYER_SCORE;
		self.tbPlayerPlayerScores[nPlayerId] = self.tbPlayerPlayerScores[nPlayerId] + nScore;
	elseif nScoreType == 2 then --敌方npc
		nScore = self.ONE_NPC_SCORE;
		self.tbPlayerNpcScores[nPlayerId] = self.tbPlayerNpcScores[nPlayerId] + nScore;
	elseif nScoreType == 3 then	--敌方炮塔
		nScore = self.ONE_TOWER_SCORE;
		self.tbPlayerTowerScores[nPlayerId] = self.tbPlayerTowerScores[nPlayerId] + nScore;
	elseif nScoreType == 4 then --敌方基地
		nScore = self.ONE_BASE_SCORE;
		self.tbPlayerBaseScores[nPlayerId] = self.tbPlayerBaseScores[nPlayerId] + nScore;
	end
	
	if pPlayer.GetCurrentCamp() == 4 then	--神族阵营总积分计算
		self.nProtossCampScores = self.nProtossCampScores + nScore;
		self.dwProtossCampScoresUpdateTime = KGameBase.GetTime();
	elseif pPlayer.GetCurrentCamp() == 5 then -- 魔族阵营总积分计算
		self.nEvilCampScores = self.nEvilCampScores + nScore;
		self.dwEvilCampScoresUpdateTime = KGameBase.GetTime();	
	end
	self.tbPlayerScore[nPlayerId].nScore = self.tbPlayerScore[nPlayerId].nScore + nScore;
	self.tbPlayerScore[nPlayerId].dwUpateTime = KGameBase.GetTime();
	--旧的接口：pPlayer.SetGlobalBattleScore(nCurrScore)
	
	--计算连斩数
	local nKillPlayerCount = self.tbPlayerPlayerScores[nPlayerId] / self.ONE_PLAYER_SCORE;
	local nKillNpcCount = self.tbPlayerNpcScores[nPlayerId] / self.ONE_NPC_SCORE;
	local nKillTowerCount = self.tbPlayerTowerScores[nPlayerId] / self.ONE_TOWER_SCORE;
	local nKillBaseCount = self.tbPlayerBaseScores[nPlayerId] / self.ONE_BASE_SCORE;
	local nKillCount = nKillPlayerCount + nKillTowerCount + nKillBaseCount;
	if nScoreType ~= 2 then
		if nKillCount == 5 then
			KChat.BroadcastNoticeMsg(string.format(Lang.mission.str13[Lang.Idx],pPlayer.szName,nKillCount));
		elseif nKillCount == 10 then
			KChat.BroadcastNoticeMsg(string.format(Lang.mission.str14[Lang.Idx],pPlayer.szName,nKillCount));
		elseif nKillCount == 15 then
			KChat.BroadcastNoticeMsg(string.format(Lang.mission.str15[Lang.Idx],pPlayer.szName,nKillCount));
		elseif nKillCount == 20 then
			KChat.BroadcastNoticeMsg(string.format(Lang.mission.str16[Lang.Idx],pPlayer.szName,nKillCount));
		end
	end
	--刷新当前排行榜
	self:RefreshAllPlayerRanks();
	self:SendPlayerMeRecord(nPlayerId, nScoreType);
	self:BroadcastCampRecord();
	
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SHENMO_SCORE, nScore)
	KRank.ModifyRankValue(pPlayer.dwId, Rank.SMZC_RANK, pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_SHENMO_SCORE));
end

function BattleField:OnJoinPlayer(nPlayerId, nGroup)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	if pPlayer ~= nil then
	   pPlayer.SetCurrentLife(pPlayer.GetMaxLife()) -- 加入战场时恢复满血	 
	end
end


-- 玩家下线或被踢时
function BattleField:OnKickPlayer(nPlayerId)
	if self.IsOpen ~= 1 then
		return;
	end
	
	-- 恢复玩家阵营
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if pPlayer then
		if pPlayer.GetCurrentCamp() == 4 and self.nProtossCampCount > 0 then
			self.nProtossCampCount = self.nProtossCampCount - 1;
		elseif pPlayer.GetCurrentCamp() == 5 and self.nEvilCampCount > 0 then
			self.nEvilCampCount = self.nEvilCampCount - 1;
		end
		
		--pPlayer.SysMsg("此次神魔战场排名："..self:GetPlayerRank(nPlayerId))
		
		-- 清除头顶标志
		if pPlayer.GetCurrentCamp() == 4 then
	      pPlayer.pBufferEffect.ClearBuff(1170)
	  elseif pPlayer.GetCurrentCamp() == 5 then
	      pPlayer.pBufferEffect.ClearBuff(1171)
	  end
	    
		pPlayer.SetCurrentCamp(1);		
		--移除该玩家数据
		self.tbPlayerNpcScores[nPlayerId] = nil;
		self.tbPlayerPlayerScores[nPlayerId] = nil;
		self.tbPlayerTowerScores[nPlayerId] = nil;
		self.tbPlayerBaseScores[nPlayerId] = nil;
		self.tbPlayerScore[nPlayerId] = nil;
		--移除玩家则移除排名，但是总积分不移除
		self:RefreshAllPlayerRanks();
		self:BroadcastCampCount();
	end
end

-- 统计所有玩家的排名状态 (玩家总分改变时执行)
function BattleField:RefreshAllPlayerRanks()
	self.tbPlayerRanks = {} -- array
	for PlayerId, tbScore in pairs(self.tbPlayerScore) do
		-- 将这个玩家积分插入数组
		table.insert(self.tbPlayerRanks, {nPlayerId = PlayerId, nPlayerScore = tbScore.nScore, dwUpdateTime = tbScore.dwUpateTime})
	end
	
	-- 对数组进行排序
	local sortFunc = function(a, b) if b.nPlayerScore ~= a.nPlayerScore then  return a.nPlayerScore > b.nPlayerScore  else return  a.dwUpdateTime < b.dwUpdateTime end end
	table.sort(self.tbPlayerRanks, sortFunc);
	
	-- 发送现有排名给客户端
	for nPlayerId, tbScore in pairs(self.tbPlayerScore) do
		self:SendPlayerRankRecord(nPlayerId);
		self:SendPlayerMeInfo(nPlayerId);
	end
end
-- 获取排名数
function BattleField:GetPlayerRank(nPlayerId)
	for rankId, tbPlayerRank in pairs(self.tbPlayerRanks) do
		if (tbPlayerRank.nPlayerId == nPlayerId) then
			return rankId
		end
	end
	return -1; -- 没有排名！怎么可能！
end

-- 传递玩家个人基本信息:  个人积分、排名
function BattleField:SendPlayerMeInfo(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local nPlayerScore = self.tbPlayerScore[nPlayerId].nScore;	--	玩家积分	
	local nPlayerRank = self:GetPlayerRank(nPlayerId); -- 玩家排名
	--pPlayer.SysMsg(string.format("设置个人信息，共%d分，排第%d名", nPlayerScore, nPlayerRank));
	pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeInfo", nPlayerScore, nPlayerRank});	--个人积分和排名
	local szCampName = "";
	if pPlayer.GetCurrentCamp() == 4 then
		szCampName = szCampName.."神族";
	elseif pPlayer.GetCurrentCamp() == 5 then
		szCampName = szCampName.."魔族";
	end
	self:SetPlayerActivityInfo(nPlayerId, nil, string.format(Lang.mission.str17[Lang.Idx],
	 szCampName, nPlayerRank, nPlayerScore, self.nProtossCampScores, self.nEvilCampScores));	
end

-- 传递玩家阵营战况, 两阵营积分总计
function BattleField:SendPlayerCampRecord(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	--pPlayer.SysMsg(string.format("神族阵营积分:%d, 魔族阵营积分:%d", self.nProtossCampScores, self.nEvilCampScores));
	pPlayer.CallClientScript({"QClientScript.Activity:SetBFCampRecord", self.nProtossCampScores, self.nEvilCampScores});
end

-- 发送双方阵营当前人数
function BattleField:SendCampCount(nPlayerId)
	local pReceivePlayer = KGameBase.GetPlayerById(nPlayerId)  -- 脚本命令接收者
	if self.nProtossCampCount == 0 and self.nEvilCampCount == 0 then
		for nGroup, tbPlayerGroup in pairs(self.tbPlayerGroups) do
			if nGroup > 0 then
				for _, nPlayerId in pairs(tbPlayerGroup) do
					local pPlayer = KGameBase.GetPlayerById(nPlayerId)
					if pPlayer then
						if pPlayer.GetCurrentCamp() == 4 then
							self.nProtossCampCount = self.nProtossCampCount + 1;
						elseif pPlayer.GetCurrentCamp() == 5 then
							self.nEvilCampCount = self.nEvilCampCount + 1;
						end
					end
				end
			end
		end
	end
	
	--pReceivePlayer.SysMsg(string.format("神族:%d人, 魔族:%d人", self.nProtossCampCount, self.nEvilCampCount));
	pReceivePlayer.CallClientScript({"QClientScript.Activity:SetBFCampCount", self.nProtossCampCount, self.nEvilCampCount});
end

--广播战场人数
function BattleField:BroadcastCampCount()
	for _, tbPlayer in pairs(self.tbPlayers) do
		self:SendCampCount(tbPlayer.nPlayerId);
	end
end

--广播战场积分
function BattleField:BroadcastCampRecord()
	for _, tbPlayer in pairs(self.tbPlayers) do
		self:SendPlayerCampRecord(tbPlayer.nPlayerId);
	end
end

-- 传递玩家排名状况, 排名、势力、门派路数、玩家姓名、伤敌玩家、夺旗奖励、护旗奖励、个人总积分
function BattleField:SendPlayerRankRecord(nPlayerId)
	local pReceivePlayer = KGameBase.GetPlayerById(nPlayerId)  -- 脚本命令接收者
	local nRank = 1;
	for _, tbPlayerRank in pairs(self.tbPlayerRanks) do
		if nRank <= 10 then
			-- 获取排行榜玩家的信息
			local pPlayer = KGameBase.GetPlayerById(tbPlayerRank.nPlayerId)
			local nCamp = pPlayer.GetCurrentCamp();
			local szCampName = "";
			if nCamp == 4 then
				szCampName = szCampName..Lang.mission.str18[Lang.Idx];
			elseif nCamp == 5 then
				szCampName = szCampName..Lang.mission.str19[Lang.Idx];
			end
			local szPlayerName = pPlayer.szName;
			local nKillPlayerCount = self.tbPlayerPlayerScores[tbPlayerRank.nPlayerId] / self.ONE_PLAYER_SCORE;
			local nKillNpcCount = self.tbPlayerNpcScores[tbPlayerRank.nPlayerId] / self.ONE_NPC_SCORE;
			local nKillTowerCount = self.tbPlayerTowerScores[tbPlayerRank.nPlayerId] / self.ONE_TOWER_SCORE;
			local nKillBaseCount = self.tbPlayerBaseScores[tbPlayerRank.nPlayerId] / self.ONE_BASE_SCORE;
			local nPlayerScore = self.tbPlayerScore[tbPlayerRank.nPlayerId].nScore 	-- 个人总积分
			
			-- 前10排名逐条发送
			--pReceivePlayer.SysMsg(string.format("收到排名信息, 排名%d, 玩家%s", nRank, szPlayerName));
			pReceivePlayer.CallClientScript({"QClientScript.Activity:SetBFRankRecord", nRank, szCampName, szPlayerName, nKillPlayerCount, nKillNpcCount, nKillTowerCount, nKillBaseCount, nPlayerScore});
		end
		nRank = nRank + 1;
	end
	
	--排名不足10位
	while nRank <= 10 do
		pReceivePlayer.CallClientScript({"QClientScript.Activity:SetBFRankRecord", nRank,"", "", 0, 0, 0, 0, 0});
		nRank = nRank + 1;
	end
end

-- 1表示击杀玩家 2表示击杀npc 3表示摧毁炮塔 4表示摧毁基地
function BattleField:SendPlayerMeRecord(nPlayerId, nType)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if pPlayer then
		local nCount = 0;
		local nScore = 0;
		if nType == 1 then	--敌方玩家
			nScore = self.tbPlayerPlayerScores[nPlayerId];
			nCount = nScore / self.ONE_PLAYER_SCORE;
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", nType, nCount, nScore});
		elseif nType == 2 then --敌方npc
			nScore = self.tbPlayerNpcScores[nPlayerId];
			nCount = nScore / self.ONE_NPC_SCORE;
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", nType, nCount, nScore});			
		elseif nType == 3 then --敌方炮塔
			nScore = self.tbPlayerTowerScores[nPlayerId];
			nCount = nScore / self.ONE_TOWER_SCORE;
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", nType, nCount, nScore});			
		elseif nType == 4 then	--敌方基地
			nScore = self.tbPlayerBaseScores[nPlayerId];	
			nCount = nScore / self.ONE_BASE_SCORE; 
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", nType, nCount, nScore});
		elseif nType == 0 then  --处理全部
			nScore = self.tbPlayerPlayerScores[nPlayerId];
			nCount = nScore / self.ONE_PLAYER_SCORE;
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", 1, nCount, nScore});
			nScore = self.tbPlayerNpcScores[nPlayerId];
			nCount = nScore / self.ONE_NPC_SCORE;
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", 2, nCount, nScore});
			nScore = self.tbPlayerTowerScores[nPlayerId];
			nCount = nScore / self.ONE_TOWER_SCORE;
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", 3, nCount, nScore});
			nScore = self.tbPlayerBaseScores[nPlayerId];	
			nCount = nScore / self.ONE_BASE_SCORE; 
			pPlayer.CallClientScript({"QClientScript.Activity:SetBFMeRecord", 4, nCount, nScore});			
		end
	end
end

function BattleField:OnNpcDeath(pKiller, pNpc)
	assert(pKiller and pNpc);
	if pNpc.IsNpc() ~= 1 then
		return;
	end
	
	if pKiller.GetCurrentCamp() == pNpc.GetCurrentCamp() then
		return;
	end
	
	if pNpc.dwTemplateId >= self.BASE_START_TEMPLATE_ID and pNpc.dwTemplateId <= self.BASE_END_TEMPLATE_ID then
		if pNpc.GetCurrentCamp() == 4 then	--神族基地被击杀
			self.dwProtossBaseKilledTime = KGameBase.GetTime();
		elseif pNpc.GetCurrentCamp() == 5 then --魔族基地被击杀
			self.dwEvilBaseKilledTime = KGameBase.GetTime();
		end
		self:AddPlayerBattleScore(pKiller.dwId, 4);
		self:EndStep();
	elseif pNpc.dwTemplateId >= self.TOWER_START_TEMPLATE_ID and pNpc.dwTemplateId <= self.TOWER_END_TEMPLATE_ID then
		self:AddPlayerBattleScore(pKiller.dwId, 3);
	elseif pNpc.dwTemplateId >= self.NPC_START_TEMPLATE_ID and pNpc.dwTemplateId <= self.NPC_END_TEMPLATE_ID then
		self:AddPlayerBattleScore(pKiller.dwId, 2);
	end
end

function BattleField:OnPlayerDamage(nCasterId, nInjuredId, nDamage)
    local pInjuredChar = KGameBase.GetCharacterById(nInjuredId)
    if pInjuredChar.IsNpc() == 1 then
      local pBaseNpc = KGameBase.GetNpcById(nInjuredId)
	    if pBaseNpc.dwTemplateId >= self.BASE_START_TEMPLATE_ID and pBaseNpc.dwTemplateId <= self.BASE_END_TEMPLATE_ID then
	        local nCurrentLife = pBaseNpc.GetCurrentLife();
	        local nMaxLife = pBaseNpc.GetMaxLife();
	        local fScale = nCurrentLife / nMaxLife;
	        local fLastScale = (nCurrentLife + nDamage) / nMaxLife;
	        local nTargetGroup = 2;
	        
	        local pPlayer = KGameBase.GetPlayerById(nCasterId)
	        if pPlayer.GetCurrentCamp() == 5 then
	           nTargetGroup = 1;
	        end
	        
	        if (fScale < 0.8 and fLastScale >= 0.8) then
	           self:BroadcastActivityNotice(Lang.mission.str20[Lang.Idx], nTargetGroup)
	        elseif (fScale < 0.6  and fLastScale >= 0.6) then
	           self:BroadcastActivityNotice(Lang.mission.str20[Lang.Idx], nTargetGroup)
	        elseif (fScale < 0.4 and fLastScale >= 0.4) then
	           self:BroadcastActivityNotice(Lang.mission.str20[Lang.Idx], nTargetGroup)
	        elseif (fScale < 0.2 and fLastScale >= 0.2) then
	           self:BroadcastActivityNotice(Lang.mission.str20[Lang.Idx], nTargetGroup)
	        end
	    end
	  end
end

function BattleField:ShowStartEffect()
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pPlayer then
			self:ShowEffect(pPlayer, 1);	--开始特效
		end
	end
end

function BattleField:OnEndStep()
	--胜负方判断
	local nWinnerCamp = 0;
	if self.dwProtossBaseKilledTime ~= 0 then
		if self.dwEvilBaseKilledTime ~= 0 then
			if self.dwEvilBaseKilledTime > self.dwProtossBaseKilledTime then
				nWinnerCamp = 4;	--神族胜利
			else
				nWinnerCamp = 5;	--魔族胜利
			end
		else
			nWinnerCamp = 5;	--魔族胜利
		end
	else 
		if self.dwEvilBaseKilledTime ~= 0 then
			nWinnerCamp = 4;	--神族胜利
		else
			if self.nProtossCampScores > self.nEvilCampScores then
				nWinnerCamp = 4;
			elseif  self.nProtossCampScores < self.nEvilCampScores then
				nWinnerCamp = 5;
			else	--积分相等
				if self.nProtossCampScores == 0 and self.nEvilCampScores == 0 then	--打酱油，则随机
					nWinnerCamp = KUnify.MathRandom(4, 5);
				else
					if self.dwProtossCampScoresUpdateTime > self.dwEvilCampScoresUpdateTime then	--时间先后判断
						nWinnerCamp = 5;
					else
						nWinnerCamp = 4;
					end
				end
			end
		end
	end
	
	--self.nWinnerCamp = nWinnerCamp;
	
	--发阵营奖励
	if nWinnerCamp ~= 0 then
		for _, tbPlayer in pairs(self.tbPlayers) do
			local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
			if pPlayer then
				--[[if pPlayer.GetCurrentCamp() == nWinnerCamp then
					--self:ShowEffect(pPlayer,4);	--成功特效
					KAward.AddPlayerAward(pPlayer, "BattleFieldCamp", 1);
				else
					--self:ShowEffect(pPlayer,3);	--失败特效
				end--]]
				-- 清除头顶标志
				if pPlayer.GetCurrentCamp() == 4 then
			      pPlayer.pBufferEffect.ClearBuff(1170)
			  elseif pPlayer.GetCurrentCamp() == 5 then
			      pPlayer.pBufferEffect.ClearBuff(1171)
			  end
			  
			  if self:GetPlayerRank(tbPlayer.nPlayerId) == -1 then
			  	pPlayer.SysMsg(Lang.mission.str21[Lang.Idx]..self.tbPlayerScore[tbPlayer.nPlayerId].nScore)
			  else
			  	pPlayer.SysMsg(Lang.mission.str21[Lang.Idx]..self.tbPlayerScore[tbPlayer.nPlayerId].nScore..Lang.mission.str22[Lang.Idx]..self:GetPlayerRank(tbPlayer.nPlayerId))
			  end
	  
			  if pPlayer.IsDead() == 1 then  -- 如果角色死亡，自动重生		  
			  	KCharacter.Revive(pPlayer, 1000, 1000);
				else   --进入擂台赛，自动满血
				  pPlayer.SetCurrentLife(pPlayer.GetMaxLife());
			  end
				--pPlayer.SetCurrentCamp(1);	--恢复玩家阵营
				--设胜利阵营
				pPlayer.CallClientScript({"QClientScript.Activity:SetBFWinCamp", nWinnerCamp - 3});
				--弹个人战报
				pPlayer.CallClientScript({"QClientScript.Activity:OpenBFPanel"});		
			end
		end
		
		local szWinnerCamp = Lang.mission.str18[Lang.Idx];
		if nWinnerCamp ~= 4 then
			szWinnerCamp = Lang.mission.str19[Lang.Idx];
		end
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str23[Lang.Idx],szWinnerCamp));
			
		if #self.tbPlayerRanks >= 1 then
			local pFirstPlayer = KGameBase.GetPlayerById(self.tbPlayerRanks[1].nPlayerId);
			KChat.BroadcastNoticeMsg(string.format(Lang.mission.str24[Lang.Idx],pFirstPlayer.szName));
		end
	end
	
	local tbPlayersCopy = Lib:CopyTBN(self.tbPlayers)
	local tbPlayerRanksCopy = Lib:CopyTBN(self.tbPlayerRanks)
	Timer:Register(3 * Env.GAME_FPS, self.SendAward, self, tbPlayersCopy, tbPlayerRanksCopy, nWinnerCamp) -- 延迟几秒发奖励，避免面板一起出来
	
	self:ResetData();
	self:DeleteAllNpc();	--删除所有npc 待测试
end

function BattleField:SendAward(tbPlayers, tbPlayerRanks, nWinnerCamp)			
  --发排行奖励
	for nRank, tbPlayerRank in pairs(tbPlayerRanks) do
		if nRank <= 10 then
			local pPlayer = KGameBase.GetPlayerById(tbPlayerRank.nPlayerId);
			if pPlayer then
				KAward.AddPlayerAward(pPlayer, "BattleFieldRank", nRank);
				
				Title:AddTitleToCharacter(pPlayer, 12, nRank, 0) -- 给称号奖励
			end
		end	
	end  
	
	if nWinnerCamp ~= 0 then
		for _, tbPlayer in pairs(tbPlayers) do
			local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
			if pPlayer then
				if pPlayer.GetCurrentCamp() == nWinnerCamp then					
					KAward.AddPlayerAward(pPlayer, "BattleFieldCamp", 1);
				end
				
				pPlayer.SetCurrentCamp(1);	--恢复玩家阵营
			end
		end
	end
	
	return 0
end

function BattleField:ResetData()
	--重置数据
	self.tbPlayerNpcScores = {};
	self.tbPlayerPlayerScores = {};
	self.tbPlayerTowerScores = {};
	self.tbPlayerBaseScores = {};
	self.tbPlayerScore = {};
	self.nProtossCampScores = 0;	
	self.nEvilCampScores = 0;
	self.dwProtossCampScoresUpdateTime = 0;
	self.dwEvilCampScoresUpdateTime = 0;		
	self.dwProtossBaseKilledTime = 0;  
	self.dwEvilBaseKilledTime = 0;	
	self.nProtossCampCount = 0;	
	self.nEvilCampCount = 0;		
end
