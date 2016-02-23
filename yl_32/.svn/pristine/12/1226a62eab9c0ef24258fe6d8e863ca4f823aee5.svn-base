
function Arena:OnCharacterEnterScene(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if pPlayer.IsNpc() == 1 then
		return;
	end
	
	if pPlayer.dwSceneTemplateId ~= self.ARENA_SCENE then  -- 必须是竞技场场景
		return;
	end
	
	pPlayer.pArena.SyncArenaInfo();
	
	pPlayer.pArena.StartArena();
end

function Arena:OnLogout(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if nMapId ~= self.ARENA_SCENE then
		return;
	end
	local _dwSceneTemplateId = pPlayer.dwSceneTemplateId;
	local _dwSceneId = pPlayer.dwSceneId;
	
	Timer:Register(3 * Env.GAME_FPS, self.ResetArenaMap, self, _dwSceneTemplateId, _dwSceneId);
end

function Arena:OnLoadPlayerComplete(pPlayer, pNpc, pPetNpc, nPos, bTop5Mode)
	local nSceneId = pPlayer.dwSceneId;
	local tbPos = self.ENTER_POS;
	
	if nPos > 0 then
		if bTop5Mode == 0 then
			tbPos = self.STAND_POS[nPos];  -- 非前5
		elseif bTop5Mode == 1 then
			tbPos = self.TOP5_POS[nPos];  -- 前5名
		end
	end
	
	KGameBase.AddNpc(pNpc, nSceneId, tbPos[1], tbPos[2], tbPos[3]);

	-- 宠物npc
	if pPetNpc ~= nil then
		KGameBase.AddNpc(pPetNpc, nSceneId, tbPos[1] + 100, tbPos[2], tbPos[3]);
	end
end

function Arena:ResetArenaMap(nMapId, nSceneId)
	KScene.ResetMap(nMapId, nSceneId);
	return 0;
end

function Arena:GetChallengeTimes(pPlayer)
	local nTimes = Player.tbDegree:GetDegree(pPlayer, "ArenaChallenge");
	return nTimes;
end

function Arena:BuyChallengeTimes(pPlayer)
	-- 扣元宝
	local nCurCoin = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
	local nNeedCoin = 10 -- 需求10个
	if (nCurCoin < nNeedCoin) then
		me.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", 10});
		return 0;
	end
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -1 * nNeedCoin);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -1 * nNeedCoin,100);
	return 1;
end

function Arena:BuyChallengeTimesCallback(pPlayer, dwNpcId)
	-- 抵达最大
	if Player.tbDegree:IsDegreeMax(pPlayer, "ArenaChallenge") == 1 then
		pPlayer.SysMsg(Lang.arena.str1[Lang.Idx])
		return
	end

	-- 判断次数上限
	if self:BuyChallengeTimes(pPlayer) ~= 1 then
		return;
	end
	
	Player.tbDegree:AddDegree(pPlayer, "ArenaChallenge", 1);
	pPlayer.pArena.SyncArenaInfo();	
	self:OnChallenge(pPlayer, dwNpcId);
end

function Arena:BuyCDTime(pPlayer)
	local nCDTime = pPlayer.pArena.GetCDTime() / Env.GAME_FPS;
	if nCDTime <= 0 then
		return 0;
    end
    
	-- 扣元宝
	local nCurCoin = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
	local nNeedCoin = 10 -- 需求10个
	if (nCurCoin < nNeedCoin) then
		me.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", 10});
		return 0;
	end
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -1 * nNeedCoin);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -1 * nNeedCoin,101);
	return 1;
end

function Arena:BuyCDTimeCallback(pPlayer, dwNpcId)
	if self:BuyCDTime(pPlayer) ~= 1 then
		return;
	end
	
	pPlayer.pArena.SetCDTime(0);
	self:OnChallenge(pPlayer, dwNpcId);
end

function Arena:CanChallenge(pPlayer, dwNpcId)
	local nTimes = self:GetChallengeTimes(pPlayer)
	if nTimes <= 0 then
		Dialog:_AskForSure(pPlayer.dwId, Lang.arena.str2[Lang.Idx], ({"Arena:BuyChallengeTimesCallback", pPlayer, dwNpcId}), "ArenaChallenge", 1);
		return 0;
	end
	
	local nCDTime = pPlayer.pArena.GetCDTime() / Env.GAME_FPS;
	if nCDTime > 0 then
		Dialog:_AskForSure(pPlayer.dwId, string.format(Lang.arena.str3[Lang.Idx], nCDTime), ({"Arena:BuyCDTimeCallback", pPlayer, dwNpcId}), "", 1);
		return 0;
	end
	
	return 1;
end

function Arena:OnChallenge(pPlayer, dwNpcId)
	if self:CanChallenge(pPlayer, dwNpcId) ~= 1 then
		return;
	end
	
	pPlayer.pArena.Challenge(dwNpcId);
end

function Arena:OnChallengeEnd(pPlayer, nResult, nWinCombo)
	-- 给奖励
	if nResult == 1 then
		self:GiveAward(pPlayer, self.emARENA_AWARD_WIN);
	else
		self:GiveAward(pPlayer, self.emARENA_AWARD_LOSE);
	end
	
	-- 扣次数
	Player.tbDegree:ReduceDegree(pPlayer, "ArenaChallenge", 1);
	self:c2s_TakeComboAward(pPlayer)
end

-- ============================C2S======================== 
function Arena:c2s_DoEnterArena(pPlayer)
	pPlayer.TeleportTo(self.ARENA_SCENE, self.ENTER_POS[1],self.ENTER_POS[2], self.ENTER_POS[3]);
end

function Arena:c2s_LoadArenaPlayers(pPlayer)
	pPlayer.pArena.LoadArenaPlayers();
end

function Arena:c2s_LoadFrontArenaPlayers(pPlayer)
	pPlayer.pArena.LoadFrontArenaPlayers();
end

function Arena:c2s_LeaveArena(pPlayer)
	--这些值会在返回city之后被修改 
	local _dwSceneTemplateId = pPlayer.dwSceneTemplateId;
	local _dwSceneId = pPlayer.dwSceneId;
	pPlayer.ReturnToLastCity();
	pPlayer.pArena.ClearData();	
	Timer:Register(3 * Env.GAME_FPS, self.ResetArenaMap, self, _dwSceneTemplateId, _dwSceneId);
end

function Arena:c2s_GetChallengeTimes(pPlayer)
	-- 抵达最大
	if Player.tbDegree:IsDegreeMax(pPlayer, "ArenaChallenge") == 1 then
		pPlayer.SendBlackMsg(Lang.arena.str4[Lang.Idx])
		return
	end
	
	if self:BuyChallengeTimes(pPlayer) ~= 1 then
		return;
	end
	
	Player.tbDegree:AddDegree(pPlayer, "ArenaChallenge", 1);
	pPlayer.pArena.SyncArenaInfo();
end

function Arena:c2s_ClearCDTime(pPlayer)
	if self:BuyCDTime(pPlayer) ~= 1 then
		return;
	end
	
	pPlayer.pArena.SetCDTime(0);
	pPlayer.pArena.SyncArenaInfo();
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, Arena.OnLogout, Arena)
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, Arena.OnCharacterEnterScene, Arena);