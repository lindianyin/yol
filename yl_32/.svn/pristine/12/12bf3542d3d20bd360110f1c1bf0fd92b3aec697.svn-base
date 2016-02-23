Include("script/mission/tower/tower_base.lua")

Tower.tbGames = {};
Tower.tbFirstEnter = {};
Tower.tbTotalExp = {};		-- 累计获得的经验奖励

function Tower:c2s_DoChallenge(pPlayer)
	print("..Tower:c2s_DoChallenge pPlayer.dwId",pPlayer.dwId);
	self:DoChallenge(pPlayer);
end

function Tower:DoChallenge(pPlayer)
	local nLayer = pPlayer.GetTowerLayer() + 1;
	
	if not self.tbSetting[nLayer] then
		assert(false);
	end
	
	local tbPos = self.tbSetting[nLayer].tbBeginPos;
	local nMapId = self.tbSetting[nLayer].nMapId;
	
	pPlayer.SetMissionState(PVEMatch.STATE_NONE);		-- 重置状态
	
	Pet:GetDownRide(pPlayer);	-- 下马
	
	pPlayer.TeleportTo(nMapId, tbPos[1], tbPos[2], tbPos[3], 1);
	
	--统计点  ldy
	local szStr = string.format("nMapId=%d",nMapId);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,66,szStr);
	
end

function Tower:SetStartLayer(pPlayer, nLayer)
	local tbTempTable = pPlayer.GetCharacterTempTable();
	if not tbTempTable.tbTowerInfo then
		tbTempTable.tbTowerInfo = {};
	end
	
	local tbTowerInfo = tbTempTable.tbTowerInfo;
	tbTowerInfo.nStartLayer = nLayer;
end

function Tower:GetStartLayer(pPlayer)
	local nStartLayer = 0;
	local tbTempTable = pPlayer.GetCharacterTempTable();
	local tbTowerInfo = tbTempTable.tbTowerInfo;
	
	nStartLayer = tbTowerInfo.nStartLayer;
	--tbTowerInfo.nStartLayer = nil;
	
	return nStartLayer;
end

function Tower:SetFailedFlag(pPlayer)
	local tbTempTable = pPlayer.GetCharacterTempTable();
	if not tbTempTable.tbTowerInfo then
		tbTempTable.tbTowerInfo = {};
	end
	
	local tbTowerInfo = tbTempTable.tbTowerInfo;
	if not tbTowerInfo.nFailed then
		tbTowerInfo.nFailed = 1;
	end
end

function Tower:GetFailedFlag(pPlayer)
	local nFailed = 0;
	local tbTempTable = pPlayer.GetCharacterTempTable();
	local tbTowerInfo = tbTempTable.tbTowerInfo;
	
	if tbTowerInfo then
	    nFailed = tbTowerInfo.nFailed;
	    tbTowerInfo.nFailed = nil;
	end
	
	return nFailed;
end

function Tower:OnEnterScene(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if pPlayer.IsNpc() == 1 then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) == "city" then
		Player:SetTempReviveFunc(pPlayer, nil);
	end
	
	if KScene.GetSceneTypeName(nMapId) ~= "tower" then
		return;
	end
	
	local nLayer = pPlayer.GetTowerLayer() + 1;
	
	local pGame = self.tbGames[dwCharacterId];
	
	if pGame then
		return;
	end
	
	pGame = Lib:NewClass(self.BaseGame);
	pGame:InitGame(nMapId, pPlayer.dwSceneId, pPlayer.dwId, self.tbSetting[nLayer]);
	pGame:JoinPlayer(pPlayer, 1);
	
	if not self.tbTotalExp[dwCharacterId] then
		self.tbTotalExp[dwCharacterId] = 0;
	end
	
	if not self.tbFirstEnter[dwCharacterId] then
		self:SetStartLayer(pPlayer, nLayer);
		self.tbFirstEnter[dwCharacterId] = 1;
		self:SetRewardInfo(pPlayer, nLayer - 1);
	end
	
	self.tbGames[dwCharacterId] = pGame;
	
	Player:SetTempReviveFunc(pPlayer, {"Tower:OnDeath", pPlayer});
end

function Tower:OnDeath(pPlayer)
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "tower" then
		return;
	end
	
	self:OnFailed(pPlayer);
end

function Tower:ShowFailedPanel(nPlayerId)
	-- 弹失败面板
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	local nStartLayer = self:GetStartLayer(pPlayer);
	pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:ShowTowerFailPanel", nStartLayer});
	
	return 0;
end

function Tower:OnFailed(pPlayer)
	self:SetFailedFlag(pPlayer);
	self:ShowFailedPanel(pPlayer.dwId);
	Timer:Register(10 * Env.GAME_FPS, self.DoLeaveGame, self, pPlayer.dwId);
end

function Tower:c2s_DoLeaveGame(pPlayer)
	print("..Tower:c2s_DoLeaveGame pPlayer.dwId",pPlayer.dwId);
	self:DoLeaveGame(pPlayer.dwId);
end

function Tower:DoLeaveGame(nPlayerId)
	local pGame = self.tbGames[nPlayerId];
	
	if pGame then
		pGame:CloseGame(1);
	end
	
	return 0;
end

function Tower:GoNextLayer(nOwnerId)
	local pPlayer = KGameBase.GetPlayerById(nOwnerId);
	local pGame = self.tbGames[pPlayer.dwId];
	
	self:OnChallengeLayerDone(pPlayer);
	
	pGame:CloseGame();
	
	local nLayer = pPlayer.GetTowerLayer() + 1;
	pPlayer.SetTowerLayer(nLayer);
	KTower.SyncTowerRank(nOwnerId, nLayer);
	Player:FullHealth(pPlayer);		-- 回满血
	
	pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:OnNextLayerBegin"});
end

function Tower:OnAllFinish(nOwnerId)
	local pPlayer = KGameBase.GetPlayerById(nOwnerId);
	local pGame = self.tbGames[pPlayer.dwId];
	
	self:OnChallengeLayerDone(pPlayer);
	
	pGame:CloseGame(1);
	
	local nLayer = pPlayer.GetTowerLayer() + 1;
	pPlayer.SetTowerLayer(nLayer);
	KTower.SyncTowerRank(nOwnerId, nLayer);
end

function Tower:c2s_DoStartNextLayer(pPlayer)
	print("..Tower:c2s_DoStartNextLayer pPlayer.dwId",pPlayer.dwId);
	self:DoStartNextLayer(pPlayer);
end

function Tower:DoStartNextLayer(pPlayer)
	local nLayer = pPlayer.GetTowerLayer();
	
	local nTargetMapId = self.tbSetting[nLayer + 1].nMapId;
	
	-- 如果下一层跟本层地图一样就直接刷个过场动画然后开始，否则先传送
	if nTargetMapId == pPlayer.dwSceneTemplateId then
		local tbPos = self.tbSetting[nLayer].tbBeginPos;
		pPlayer.TeleportTo(pPlayer.dwSceneId, tbPos[1], tbPos[2], tbPos[3], 1);
		self:OnEnterScene(pPlayer.dwId);
		self:DoStartGame(pPlayer);
	else
		self:DoChallenge(pPlayer);
	end
end

function Tower:DoStartGame(pPlayer)
	local pGame = self.tbGames[pPlayer.dwId];
	if pGame then
		pGame:StartGame();
	end
end

function Tower:c2s_StartGame(pPlayer)
	print("..Tower:c2s_StartGame pPlayer.dwId",pPlayer.dwId);
	self:DoStartGame(pPlayer);
end

function Tower:NpcUnLock(pNpc)
	local tbTmp = pNpc.GetTempTable("Tower")
	if not tbTmp then
		return 0;
	end
	if (not tbTmp.tbTower) or (not tbTmp.nLock) then
		return 0;
	end
	if not tbTmp.tbTower.tbLock[tbTmp.nLock] then
		return 0;
	end
	
	tbTmp.tbTower.tbLock[tbTmp.nLock]:UnLockMulti();
	
	-- KK:传递给这个Instance,记录杀敌数改变
	-- 判断锁的杀敌数要求是否大于0, 是则传送TargetInfo
	if (tbTmp.tbTower.tbLock[tbTmp.nLock]:IsClose() ~= 1) then
		if (tbTmp.tbTower.tbLock[tbTmp.nLock].nInitMultiNum > 0) then
			-- 更新targetInfo，显示杀人数量
			tbTmp.tbTower:SyncTargetInfo(tbTmp.tbTower.szTargetInfo, tbTmp.nLock);
		end
	end
end

-- 得到每一层应该得到的经验总值
function Tower:GetLayerTotalExp(nLayer, nPlayerLevel)
	-- 60层总经验
	--local nTotalExp = KAward.GetExpByLevel(Tower.EXP_AWARD_ID, nPlayerLevel + 1);
	local tbCfg = Tower.LayerCfgs[nLayer];
	--local nLayerExp = math.floor(nTotalExp * tbCfg.nExpPercent / Tower.nTotalExpPercent);
	--return nLayerExp;
	return tbCfg.nExpPercent;
end

function Tower:OnChallengeLayerDone(pPlayer)
	-- 发当前层的挑战奖励
	local nCurLayer = pPlayer.GetTowerLayer() + 1;
	local tbCfg = Tower.LayerCfgs[nCurLayer];
	
	local nTotalExp = Tower:GetLayerTotalExp(nCurLayer, pPlayer.GetLevel());
	local nExp = math.floor(nTotalExp);
	pPlayer.AddExp(nExp);
	
	--self.tbTotalExp[pPlayer.dwId] = self.tbTotalExp[pPlayer.dwId] + nTotalExp;

	self.tbTotalExp[pPlayer.dwId] = self.tbTotalExp[pPlayer.dwId] + nExp;
	
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, tbCfg.nMoney);
	
	if nCurLayer == 20 then
		Target:DoTarget(pPlayer, 1);
	elseif nCurLayer == 40 then
		Target:DoTarget(pPlayer, 3);
	elseif nCurLayer == 60 then
		Target:DoTarget(pPlayer, 6);
	end
	
	if nCurLayer == 10 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1053[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 20 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1054[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 30 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1055[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 40 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1056[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 50 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1057[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 60 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1058[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 70 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1231[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 80 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1232[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 90 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1233[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 100 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1234[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 110 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1235[Lang.Idx], pPlayer.szName));
	elseif nCurLayer == 120 then
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1236[Lang.Idx], pPlayer.szName));
	end
end

function Tower:SetRewardInfo(pPlayer, nLayer)
	if not nLayer then
		nLayer = pPlayer.GetTowerLayer() + 1;
	end
	
	local szInfo = "";
	local nTotalMoney = 0;
	local nStartLayer = self:GetStartLayer(pPlayer);
	local nNextLevel = pPlayer.GetLevel() + 1;
	
	for i = nStartLayer, nLayer do
		local tbCfg = Tower.LayerCfgs[i];
		nTotalMoney = nTotalMoney + tbCfg.nMoney;
	end
	
	szInfo = string.format(Lang.mission.str1059[Lang.Idx], nTotalMoney, self.tbTotalExp[pPlayer.dwId]);
	
	pPlayer.CallClientScript({"QClientScript.Mission:SetMissionExInfo", szInfo});
end

function Tower:LeaveGame(pPlayer)
	self:SetFailedFlag(pPlayer);
	local pGame = self.tbGames[pPlayer.dwId];
	if pGame then
		pGame:CloseGame(1);
	end
end

function Tower:OnLogout(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "tower" then
		return;
	end
	
	local pGame = self.tbGames[pPlayer.dwId];
	if pGame then
		pGame:CloseGame(1);
	end
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, Tower.OnLogout, Tower)
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, Tower.OnEnterScene, Tower);
