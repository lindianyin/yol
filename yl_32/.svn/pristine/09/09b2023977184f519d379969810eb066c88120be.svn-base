Include("script/mission/multiinstance/multi_base.lua")

MultiInstance.tbGames = {};

function MultiInstance:c2s_DoEnterGameSingle(pPlayer, nMapId)
	assert(pPlayer.GetTeamId() == 0);
	KTeam.CreateTeam(pPlayer.dwId);
	Timer:Register(1 * Env.GAME_FPS, self.DoEnterGame, self, {pPlayer.dwId}, nMapId);
end

function MultiInstance:IsDegreeEnough(pPlayer, nMapId)
	local szDegreeName = self:GetMultiDegreeName(nMapId);
	if szDegreeName == "None" then
		return 1;
	end
	
	if Player.tbDegree:GetDegree(pPlayer, szDegreeName) <= 0 then
		return 0;
	end
	
	return 1;
end

function MultiInstance:ReduceMultiDegree(pPlayer, nMapId)
	local szDegreeName = self:GetMultiDegreeName(nMapId);
	
	if szDegreeName == "None" then
		return;
	end
	
	if Player.tbDegree:GetDegree(pPlayer, szDegreeName) <= 0 then
		assert(false);
	end
	
	Player.tbDegree:ReduceDegree(pPlayer, szDegreeName);
end

function MultiInstance:DoEnterGame(tbPlayerIds, nMapId)

	assert( type(tbPlayerIds) == "table" );
	
	if #tbPlayerIds <= 0 then
		return;
	end
	
	for _, nPlayerId in ipairs(tbPlayerIds) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer and pPlayer.GetTeamId() > 0 then
			
			if self:IsDegreeEnough(pPlayer, nMapId) ~= 1 then
				assert(false);
			end
			
			KInstance.SetMultiRushStar(pPlayer, 0);
			KInstance.SetMultiState(pPlayer, 1);
			
			-- 设置开始时间
			local nTime = KGameBase.GetTime();
			KInstance.SetMultiStartTime(pPlayer, nTime);
			
			self:DoEnterInstance(pPlayer, nMapId);
		end
	end
	
	return 0;
end

function MultiInstance:PrepareToTransfer(pPlayer, nMapId)
	if self:IsDegreeEnough(pPlayer, nMapId) ~= 1 then
		assert(false);
	end
	
	-- 消耗进副本令牌
	self:ConsumeEnterNeedItems(pPlayer, nMapId);
			
	pPlayer.DoMultiTransferRequest();
end

function MultiInstance:ConsumeEnterNeedItems(pPlayer, nMapId)
	local tbNeedItem = self:GetEnterNeedItem(nMapId);
	if not tbNeedItem then
		return;
	end
	
	local tbFind = pPlayer.pItem.FindItemInBags(tbNeedItem.nGenre, tbNeedItem.nDetail, tbNeedItem.nParticular, 0);
	local nCount = Item:GetItemAmount(tbFind);
	if nCount < tbNeedItem.nAmount then
		assert(false);
	end
	
	pPlayer.pItem.ConsumeItemInBags(tbNeedItem.nAmount, tbNeedItem.nGenre, tbNeedItem.nDetail, tbNeedItem.nParticular);
end

function MultiInstance:DoEnterInstance(pPlayer, nMapId)
	local tbPos = self.tbSetting[nMapId].tbBeginPos;
	
	if type(tbPos[1]) == "table" then
		-- 多个点就随机一个点
		tbPos = tbPos[KUnify.MathRandom(1, #tbPos)];
	end
	
	Pet:GetDownRide(pPlayer);	-- 下马
	
	-- 消耗进副本令牌
	self:ConsumeEnterNeedItems(pPlayer, nMapId);
	
	pPlayer.TeleportTo(nMapId, tbPos[1], tbPos[2], tbPos[3]);
	
	--统计点  ldy
	local szStr = string.format("nMapId=%d",nMapId);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,64,szStr);
end

function MultiInstance:OnEnterScene(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if pPlayer.IsNpc() == 1 then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "city" and pPlayer.GetOverallTeamIdx() > 0 then
		KGameBase.LeaveOverallTeam(pPlayer);
	end
	
	if KScene.GetSceneTypeName(nMapId) ~= "multiinstance" then
		local nState = KInstance.GetMultiState(pPlayer);
		
		if nState ~= 0 then
			pPlayer.LeaveTeam();
			KInstance.SetMultiState(pPlayer, 0);
		end
		
		return;
	end
	
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId <= 0 then
		-- TODO:应该是要做退出判断
		return;
	end
	
	Player:FullHealth(pPlayer);	-- 回满血
	
	if self:IsDegreeEnough(pPlayer, nMapId) ~= 1 then
		assert(false);
	end
	
	KInstance.SetMultiRushStar(pPlayer, 0);
	KInstance.SetMultiState(pPlayer, 1);
	
	KCrossSv.ClearDropItems(pPlayer);
	
	-- 设置开始时间
	local nTime = KGameBase.GetTime();
	KInstance.SetMultiStartTime(pPlayer, nTime);
	
	local pGame = self.tbGames[nTeamId];
	
	if pGame then
	    pGame:_JoinPlayer(pPlayer, 1);
		KInstance.SetMultiRushMapId(pPlayer, nMapId);
		return;
	end
	
	local nGameType = self.tbSetting[nMapId].nGameType;
	if nGameType and self.GAME_FACTORY[nGameType] then
		pGame = self.GAME_FACTORY:Create(nGameType);
	else
		pGame = Lib:NewClass(self.BaseGame);
	end
	
	pGame:InitGame(nMapId, pPlayer.dwSceneId, nTeamId, self.tbSetting[nMapId]);
	pGame:_JoinPlayer(pPlayer, 1);
	
	KInstance.SetMultiRushMapId(pPlayer, nMapId);
	
	self.tbGames[nTeamId] = pGame;
end

function MultiInstance:BroadcastNoticeMsg(pPlayer)
	local nTeamId = pPlayer.GetTeamId();
	if KTeam.GetTeamLeader(nTeamId) ~= pPlayer.dwId then
		return;
	end
	local pGame = self.tbGames[nTeamId];
	local szMultiName = MultiInstance:GetMultiName(pGame.nMapId);
	local szMsg = string.format(Lang.mission.str979[Lang.Idx], pPlayer.szName, szMultiName);
	KChat.BroadcastNoticeMsg(szMsg);
end

-- 给奖励并弹出奖励结算面板
function MultiInstance:GivePlayerAward(pPlayer, nDefaultStar, nDelayShow)
	if KInstance.GetMultiState(pPlayer) ~= 1 then
		return;
	end
	
	local nStar, nLastTime = self:CalcRushStar(pPlayer);
	if nDefaultStar then
		nStar = nDefaultStar;
	end
	
	KInstance.SetMultiRushStar(pPlayer, nStar);
	
	local nMapId = KInstance.GetMultiRushMapId(pPlayer);
	
	local nTeamId = pPlayer.GetTeamId();
	local pGame = self.tbGames[nTeamId];
	if pGame.nDifficulty and pGame.nDifficulty == 2 then
		Target:DoTarget(pPlayer, 7);
	end
	
	-- 星星大于1级才算通过
	if nStar == 5 then
		self:BroadcastNoticeMsg(pPlayer);
	end
	
	local szCallFunc = "YoulongApp.UI.Controller.QUIClientScript:ShowTaskAwardPanel";
	if nDelayShow and nDelayShow == 1 then
		szCallFunc = "YoulongApp.UI.Controller.QUIClientScript:DelayShowTaskAwardPanel";
	end
	
	pPlayer.CallClientScript({szCallFunc, self.AWARD_MODULE_NAME, nMapId * 10 + nStar, nLastTime, nStar, Reward.emTASK_AWARD_MULTI});
end

-- ?gs me.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowTaskAwardPanel", MultiInstance.AWARD_MODULE_NAME, 300 * 10 + 5, 120, 3, Reward.emTASK_AWARD_MULTI});


-- 点击领取奖励
function MultiInstance:c2s_GetAward(pPlayer)
	local nMapId = KInstance.GetMultiRushMapId(pPlayer);
	if nMapId <= 0 then
		return;
	end
	
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId <= 0 then
		return;
	end
	
	local pGame = self.tbGames[nTeamId];
	if not pGame or not pGame.bFinish then
		return;
	end
	
	local nStar = KInstance.GetMultiRushStar(pPlayer);
	
	KAward.AddPlayerAward(pPlayer, self.AWARD_MODULE_NAME, nMapId * 10 + nStar);
	
	KInstance.SetMultiRushMapId(pPlayer, 0);
	
	KInstance.SetMultiState(pPlayer, 0);
	
	-- 领奖后扣次数
	if nStar > 1 then
		if self:IsDegreeEnough(pPlayer, nMapId) ~= 1 then
			assert(false);
		end
		self:ReduceMultiDegree(pPlayer, nMapId);
	end
	
	self:LeaveGame(pPlayer);
end

-- ?gs KAward.AddPlayerAward(me, MultiInstance.AWARD_MODULE_NAME, 5502);

function MultiInstance:AddMultiAward(pPlayer, nMapId, nStar)
	KAward.AddPlayerAward(pPlayer, self.AWARD_MODULE_NAME, nMapId * 10 + nStar);
	
	KInstance.SetMultiRushMapId(pPlayer, 0);
	
	KInstance.SetMultiState(pPlayer, 0);
	
	if self:IsDegreeEnough(pPlayer, nMapId) ~= 1 then
		assert(false);
	end
	
	self:ReduceMultiDegree(pPlayer, nMapId);
end

-- 关闭多人副本
function MultiInstance:CloseGame(nTeamId)
	local pGame = self.tbGames[nTeamId];
	pGame:CloseGame(1);
	
	self.tbGames[nTeamId] = nil;
end

function MultiInstance:DelayCloseGame(nTeamId)
	self:CloseGame(nTeamId);
	return 0;
end

function MultiInstance:DoStartGame(pPlayer)
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId <= 0 then
		return;
	end
	local pGame = self.tbGames[nTeamId];
	if pGame then
		pGame:StartGame();
	end
end

function MultiInstance:c2s_StartGame(pPlayer)
	self:DoStartGame(pPlayer);
end

function MultiInstance:LeaveGame(pPlayer)
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId <= 0 then
		return;
	end
	
	local pGame = self.tbGames[nTeamId];
	if pGame then
		pGame:DoLeaveGame(pPlayer, 1);
	end
end

function MultiInstance:OnDeath(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if not pPlayer or pPlayer.IsNpc() == 1 then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "multiinstance" then
		return;
	end
	
	local nTeamId = pPlayer.GetTeamId();
	assert(nTeamId > 0);
	local pGame = self.tbGames[nTeamId];
	if pGame then
		pGame:OnPlayerDeath(pPlayer);
	end
end

function MultiInstance:NpcUnLock(pNpc)
	local tbTmp = pNpc.GetTempTable("MultiInstance")
	if not tbTmp then
		return 0;
	end
	if (not tbTmp.tbInstance) or (not tbTmp.nLock) then
		return 0;
	end
	if not tbTmp.tbInstance.tbLock[tbTmp.nLock] then
		return 0;
	end
	
	tbTmp.tbInstance.tbLock[tbTmp.nLock]:UnLockMulti();
	
	-- KK:传递给这个Instance,记录杀敌数改变
	-- 判断锁的杀敌数要求是否大于0, 是则传送TargetInfo
	if (tbTmp.tbInstance.tbLock[tbTmp.nLock]:IsClose() ~= 1) then
		if (tbTmp.tbInstance.tbLock[tbTmp.nLock].nInitMultiNum > 0) then
			-- 更新targetInfo，显示杀人数量
			tbTmp.tbInstance:SyncTargetInfo(tbTmp.tbInstance.szTargetInfo, tbTmp.nLock);
		end
	end
end

function MultiInstance:OnLogin(dwCharacterId)

	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer or pPlayer.IsBZonePlayer() == 1 then
		return;
	end
	
	pPlayer.DoGetMultiAwardRequest();
	
	pPlayer.DoGetCrossSvExDataRequest();
end

function MultiInstance:GiveDropItems(pPlayer, nG, nD, nP, nL, nAmount)
	KItem.AddPlayerItem(pPlayer, nG, nD, nP, nL, nAmount);
end

function MultiInstance:OnLogout(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "multiinstance" then
		return;
	end
	
	local nTeamId = pPlayer.GetTeamId();
	local pGame = self.tbGames[nTeamId];
	
	-- 退出队伍 不能在这里退出队伍，因为玩家可能是跨gs调用OnLogout
	--pPlayer.LeaveTeam();
	
	local tbLastCity = pPlayer.GetLastSaveCity();
	pPlayer.SetPositionForSaving(tbLastCity.nMapId, tbLastCity.nX, tbLastCity.nY, tbLastCity.nZ);
	
	if pGame then
		pGame:OnLogout(pPlayer);
	end
end

-- 评判本次星级
function MultiInstance:CalcRushStar(pPlayer)
	local nLastTime = KGameBase.GetTime() - KInstance.GetMultiStartTime(pPlayer);
	local nStar = 1;
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId <= 0 then
		return 1, nLastTime;
	end
	
	local pGame = self.tbGames[nTeamId];
	if not pGame then
		return 1, nLastTime;
	end
	
	local nBloodPercent = 0;
	local nNum = 0;
	for _, nPlayerId in ipairs(pGame.tbGamePlayers) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			local nMaxLife = pPlayer.GetMaxLife();
			if pPlayer.GetAngerLeftTime() > 0 then
				nMaxLife = math.floor(nMaxLife / 2);
			end
			nNum = nNum + 1;
			nBloodPercent = nBloodPercent + math.floor(pPlayer.GetCurrentLife() * 100 / nMaxLife);
		end
	end
	
	if nNum > 0 then
		nBloodPercent = nBloodPercent / nNum;
	end
	
	-- 最低2星
	local nBloodStar = 2;
	local nTimeStar = 2;
	local nDeathStar = 5;
	
	-- 计算剩余血量
	if nBloodPercent >= 70 then
		nBloodStar = 5;
	elseif nBloodPercent >= 50 then
		nBloodStar = 4;
	elseif nBloodPercent >= 30 then
		nBloodStar = 3;
	else
		nBloodStar = 2;
	end
	
	if nLastTime <= 10 * 60 then
		nTimeStar = 5;
	elseif nLastTime <= 15 * 60 then
		nTimeStar = 4;
	elseif nLastTime <= 20 * 60 then
		nTimeStar = 3;
	else
		nTimeStar = 2;
	end
	
	local nDeathTimes = pGame:GetDeathTimes(pPlayer);
	
	if nDeathTimes <= 0 then
		nDeathStar = 5;
	elseif nDeathTimes == 1 then
		nDeathStar = 4;
	elseif nDeathTimes == 2 then
		nDeathStar = 3;
	else
		nDeathStar = 2;
	end
	
	-- 取小的
	if nBloodStar < nTimeStar then
		nStar = nBloodStar;
	else
		nStar = nTimeStar;
	end
	
	if nStar > nDeathStar then
		nStar = nDeathStar;
	end
	
	return nStar, nLastTime;
end

function MultiInstance:c2s_BuyRushTimes(pPlayer)
	
	if Vip:IsOpen(pPlayer, Vip.ID_MULTIINSTANCE) ~= 1 then
		return 0;
	end
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_MULTIINSTANCE, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.mission.str980[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = self.BUY_RUSH_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str981[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.mission.str982[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"MultiInstance:DoBuyMultiTimes", pPlayer}), "BuyMultiTimes", 0);
end

function MultiInstance:DoBuyMultiTimes(pPlayer)
	if Vip:CanBuyCount(pPlayer, Vip.ID_MULTIINSTANCE, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.mission.str980[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = self.BUY_RUSH_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str981[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,124);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_MULTIINSTANCE, 1);
end

function MultiInstance:OnRegenerateNpc(dwSceneId, tbRetNpc, dwExValue, szGroup)
	local pGame = self.tbGames[dwExValue];
	if not pGame or pGame.nSceneId ~= dwSceneId then
		return;
	end
	
	-- 将生成的npc重新加入到group里面
	for _, pNpc in ipairs(tbRetNpc) do
		pGame:AddNpcInGroup(pNpc, szGroup);
	end
	
	if pGame.tbDelRegenerateNpc[szGroup] then
		pGame:DelNpc(szGroup);
	end
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, MultiInstance.OnLogin, MultiInstance);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, MultiInstance.OnLogout, MultiInstance);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, MultiInstance.OnEnterScene, MultiInstance);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_DEATH, MultiInstance.OnDeath, MultiInstance);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_LEAVE_TEAM, MultiInstance.OnLeaveTeam, MultiInstance);