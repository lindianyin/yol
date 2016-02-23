
function Escort:c2s_EnterEscortMap(pPlayer)
	pPlayer.TeleportTo(self.ESCORT_SCENE, 0, 0, 0, 1);
end

function Escort:OnLoadPlayerComplete(pPlayer, pNpc, pPetNpc, nPosIdx)
	local nSceneId = pPlayer.dwSceneId;
	
	local tbPos = self.STAND_POS[nPosIdx];
	
	KGameBase.AddNpc(pNpc, nSceneId, tbPos[1], tbPos[2], tbPos[3]);

	-- 宠物npc
	if pPetNpc ~= nil then
		KGameBase.AddNpc(pPetNpc, nSceneId, tbPos[1] + 100, tbPos[2], tbPos[3]);
	end
end

function Escort:c2s_SpeedUpEscort(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_FINISH_ESCORT, 0) == 1 then
		KEscort.SpeedUpEscort(pPlayer.dwId, 0);
	elseif Vip:IsOpen(pPlayer, Vip.ID_SPEEDUP_ESCORT, 0) == 1 then
		KEscort.SpeedUpEscort(pPlayer.dwId, 1);
	else
		pPlayer.SendBlackMsg(Lang.escort.str1[Lang.Idx]);
	end
end

function Escort:OnStartEscort(pPlayer)
	if Player.tbDegree:GetDegree(pPlayer, "Escort") <= 0 then
		return 0;
	end
	
	Player.tbDegree:ReduceDegree(pPlayer, "Escort");

	--统计点 ldy
	local szStr = string.format("Degree=%d",Player.tbDegree:GetDegree(pPlayer, "Escort"));
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,67,szStr);
	return 1;
end

function Escort:OnStartRobFight(pPlayer)
	if Player.tbDegree:GetDegree(pPlayer, "EscortAttack") <= 0 then
		return 0;
	end
	
	Player.tbDegree:ReduceDegree(pPlayer, "EscortAttack");
	
	return 1;
end

function Escort:OnStartRescueFight(pPlayer)
	if Player.tbDegree:GetDegree(pPlayer, "EscortHelp") <= 0 then
		return 0;
	end
	
	Player.tbDegree:ReduceDegree(pPlayer, "EscortHelp");
	
	return 1;
end

function Escort:c2s_BuyEscortTimes(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_BUY_ESCORT) ~= 1 then
		return 0;
	end
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_BUY_ESCORT, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_ESCORT_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str3[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.escort.str4[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Escort:DoBuyEscortTimes", pPlayer}), "BuyEscortTimes", 0);
end

function Escort:DoBuyEscortTimes(pPlayer)
	if Vip:CanBuyCount(pPlayer, Vip.ID_BUY_ESCORT, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_ESCORT_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str3[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,108);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_BUY_ESCORT, 1);
end

function Escort:c2s_BuyRefreshTimes(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_BUY_ESCORT_REFRESH) ~= 1 then
		return 0;
	end
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_BUY_ESCORT_REFRESH, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_REFRESH_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str5[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.escort.str6[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Escort:DoBuyRefreshTimes", pPlayer}), "BuyEscortRefreshTimes", 0);
end

function Escort:DoBuyRefreshTimes(pPlayer)
	if Vip:CanBuyCount(pPlayer, Vip.ID_BUY_ESCORT_REFRESH, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_REFRESH_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str5[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,109);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_BUY_ESCORT_REFRESH, 1);
end

function Escort:c2s_BuyRobTimes(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_ESCORT_ROB_TIMES) ~= 1 then
		return 0;
	end
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_ESCORT_ROB_TIMES, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_ROB_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str7[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.escort.str8[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Escort:DoBuyRobTimes", pPlayer}), "BuyEscortRobTimes", 0);
end

function Escort:DoBuyRobTimes(pPlayer)
	if Vip:CanBuyCount(pPlayer, Vip.ID_ESCORT_ROB_TIMES, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_ROB_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str9[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,110);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_ESCORT_ROB_TIMES, 1);
end


function Escort:c2s_BuyHelpTimes(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_ESCORT_HELP_TIMES) ~= 1 then
		return 0;
	end
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_ESCORT_HELP_TIMES, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_HELP_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str10[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.escort.str11[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Escort:DoBuyHelpTimes", pPlayer}), "BuyEscortHelpTimes", 0);
end

function Escort:DoBuyHelpTimes(pPlayer)
	if Vip:CanBuyCount(pPlayer, Vip.ID_ESCORT_HELP_TIMES, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.escort.str2[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Escort.BUY_HELP_TIMES_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str10[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,111);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_ESCORT_HELP_TIMES, 1);
end

function Escort:OnLoadSelfPlayerComplete(pPlayer, pNpc, pPetNpc)
	local nSceneId = pPlayer.dwSceneId;
	
	local tbPos = {0, 0, 1056168};
	
	KGameBase.AddNpc(pNpc, nSceneId, tbPos[1], tbPos[2], tbPos[3]);

	-- 宠物npc
	if pPetNpc ~= nil then
		KGameBase.AddNpc(pPetNpc, nSceneId, tbPos[1] + 100, tbPos[2], tbPos[3]);
	end
end

function Escort:OnLoadRobPlayerComplete(pPlayer, pHimNpc, pHimPetNpc, dwHimId, dwMeNpcId, dwMePetNpcId)
	local nSceneId = pPlayer.dwSceneId;
	
	local tbPos = {0, 0, 1056168};
	
	KGameBase.AddNpc(pHimNpc, nSceneId, tbPos[1], tbPos[2], tbPos[3]);

	-- 宠物npc
	if pPetNpc ~= nil then
		KGameBase.AddNpc(pHimPetNpc, nSceneId, tbPos[1] + 100, tbPos[2], tbPos[3]);
	end
	
	KGameBase.StartOfflineFight(pPlayer.dwId, dwHimId, dwMeNpcId, dwMePetNpcId, pHimNpc.dwId, (pHimPetNpc ~= nil and pHimPetNpc.dwId) or 0);
end

function Escort:OnCharacterEnterScene(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if pPlayer.IsNpc() == 1 then
		return;
	end
	
	if pPlayer.dwSceneTemplateId ~= self.ESCORT_SCENE then
		KEscort.RemoveEscortWatch(dwCharacterId);
		KEscort.ClearEscortData(pPlayer.dwId);
		return;
	end
	
	KEscort.RefreshEscortPlayers(pPlayer.dwId);
	
	local tLastRefreshTime = KEscort.GetLastRefreshTime(pPlayer);
	local tNow = KGameBase.GetTime();
	
	-- 隔天重置刷新类型
	if Lib:GetLocalDay(tLastRefreshTime) ~= Lib:GetLocalDay(tNow) then
		pPlayer.SetEscortType(1);
		KEscort.SetLastRefreshTime(pPlayer, tNow);
	end
end

function Escort:OnTakeRobAward(pPlayer, nEscortLevel)
	KAward.AddPlayerAward(pPlayer, "RobEscort", nEscortLevel);
	local tbAwardData = KAward.GetAssetAwardData("RobEscort", nEscortLevel);
	--pPlayer.SendBlackMsg(string.format(Lang.escort.str12[Lang.Idx], tbAwardData.nEnergy));
end

function Escort:c2s_OnRescuePlayer(pPlayer, nRescuePlayerId, szResName, nEscortType)
	KGameBase.CenterExecute({"Escort:SendRescueMsg_GC", nRescuePlayerId, szResName});
	
	KAward.AddPlayerAward(pPlayer, "RescueEscort", nEscortType);
	local tbAwardData = KAward.GetAssetAwardData("RescueEscort", nEscortType);
	pPlayer.SendBlackMsg(string.format(Lang.escort.str12[Lang.Idx], tbAwardData.nEnergy));
end

function Escort:OnRescuePlayerRespond(pPlayer, nRobPlayerId, szPlayerName, bCanResue)
	if bCanResue == 0 then
		pPlayer.SendBlackMsg(string.format(Lang.escort.str13[Lang.Idx], szPlayerName));
		return;
	end
	
	pPlayer.CallClientScript({"QClientScript.Escort:OnRescuePlayerRespond", nRobPlayerId, szPlayerName});
end

function Escort:SendRescueMsg_GS(nRescuePlayerId, szResName)
	local pPlayer = KGameBase.GetPlayerById(nRescuePlayerId);
	if not pPlayer then
		return;
	end
	
	pPlayer.CallClientScript({"QClientScript.Escort:SendRescueMsg", szResName});
end

function Escort:OnTakeEscortAward(pPlayer, nEscortState, nEscortLevel)
	local nAwardLevel = nEscortLevel;
	if nEscortState == Escort.emESCORT_STATE_COMPLETE_BEHIT then
		--nAwardLevel = 10 + nEscortLevel;
	end
	
	--KAward.AddPlayerAward(pPlayer, "Escort", nAwardLevel,1,100);
	local tbAwardData = KAward.GetAssetAwardData("Escort", nAwardLevel);
	pPlayer.pPlayerPurse.AddMoney(3, tbAwardData.nEnergy);
	--pPlayer.SendBlackMsg(string.format(Lang.escort.str12[Lang.Idx], tbAwardData.nEnergy));
end

function Escort:OnLogout(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if nMapId ~= self.ESCORT_SCENE then
		return;
	end
	
	KEscort.RemoveEscortWatch(dwCharacterId);
	
	Timer:Register(3 * Env.GAME_FPS, self.ResetEscortMap, self, pPlayer.dwSceneTemplateId, pPlayer.dwSceneId);
end

function Escort:c2s_LeaveEscort(pPlayer)
	pPlayer.ReturnToLastCity();
	
	KEscort.RemoveEscortWatch(pPlayer.dwId);
	
	Timer:Register(3 * Env.GAME_FPS, self.ResetEscortMap, self, pPlayer.dwSceneTemplateId, pPlayer.dwSceneId);
end

function Escort:ResetEscortMap(nMapId, nSceneId)
	KScene.ResetMap(nMapId, nSceneId);
	
	return 0;
end

function Escort:c2s_RefreshEscort(pPlayer, nAuto)
	local nTimes = Player.tbDegree:GetDegree(pPlayer, "EscortRefresh");
	assert(nTimes > 0);
	
	local nRefreshTimes = 1;
	if nAuto == 1 then
		nRefreshTimes = nTimes;
	end
	
	local nConsumeTimes = 0;
	local nCurType = pPlayer.GetEscortType();
	local nType = nCurType;
	
	for i = 1, nRefreshTimes do
		-- 已经刷到最高
		if nType >= 5 then
			break;
		end
		
		local tbProb = {};
		for nIdx = nType, 5 do
			tbProb[nIdx] = self.REFRESH_PROBABILTY[nIdx];
		end
		
		nType = Lib:RandProb(tbProb);
		
		nConsumeTimes = nConsumeTimes + 1;
	end
	
	Player.tbDegree:ReduceDegree(pPlayer, "EscortRefresh", nConsumeTimes);
	
	pPlayer.SetEscortType(nType);
end

function Escort:c2s_DoInviteFriend(pPlayer, nFriendId)
	KGameBase.CenterExecute({"Escort:DoInviteFriend_GC", pPlayer.dwId, pPlayer.szName, nFriendId});
end

function Escort:DoInviteFriend_GS(nInviteId, szPlayerName, nFriendId)
	local pPlayer = KGameBase.GetPlayerById(nFriendId);
	if not pPlayer then
		return;
	end
	
	-- 护驾次数不足
	if Player.tbDegree:GetDegree(pPlayer, "EscortProtect") <= 0 then
		return 0;
	end
	
	pPlayer.CallClientScript({"QClientScript.Escort:InviteEscortProtect", nInviteId, szPlayerName});
	--Dialog:_AskForSure(pPlayer.dwId, string.format(Lang.escort.str14[Lang.Idx], szPlayerName), ({"Escort:OnInviteFriend", nInviteId, pPlayer.szName, nFriendId}));
end

function Escort:c2s_OnInviteFriend(pPlayer, nInviteId, szFriendName, nFriendId)
	KGameBase.CenterExecute({"Escort:OnInviteFriend_GC", nInviteId, szFriendName, nFriendId});
end

function Escort:OnInviteFriend_GS(nInviteId, szFriendName, nFriendId)
	local pPlayer = KGameBase.GetPlayerById(nInviteId);
	if not pPlayer then
		return;
	end
	
	if pPlayer.GetEscortState() > 0 then
		KGameBase.CenterExecute({"Escort:OnInviteFriendFailed_GC", pPlayer.szName, nFriendId});
		return;
	end
	
	KGameBase.CenterExecute({"Escort:OnInviteFriendSuccess_GC", nFriendId});
	
	pPlayer.CallClientScript({"QClientScript.Escort:SetProtectPlayerId", nFriendId, szFriendName});
	
	pPlayer.SendBlackMsg(string.format(Lang.escort.str15[Lang.Idx], szFriendName));
end

function Escort:OnInviteFriendFailed_GS(szPlayerName, nFriendId)
	local pPlayer = KGameBase.GetPlayerById(nFriendId);
	if not pPlayer then
		return;
	end
	
	pPlayer.SendBlackMsg(string.format(Lang.escort.str16[Lang.Idx], szPlayerName));
end

function Escort:OnInviteFriendSuccess_GS(nFriendId)
	local pPlayer = KGameBase.GetPlayerById(nFriendId);
	if not pPlayer then
		return;
	end
	
	if Player.tbDegree:GetDegree(pPlayer, "EscortProtect") <= 0 then
		assert(false);
	end
	
	Player.tbDegree:ReduceDegree(pPlayer, "EscortProtect");
end

function Escort:OnFinishEscort(nProtecterId, nEscortLevel)
	KGameBase.CenterExecute({"Escort:OnFinishEscort_GC", nProtecterId, nEscortLevel});
end

function Escort:OnFinishEscort_GS(nProtecterId, nEscortLevel)
	local pPlayer = KGameBase.GetPlayerById(nProtecterId);
	if not pPlayer then
		return;
	end
	
	KAward.AddPlayerAward(pPlayer, "ProtectEscort", nEscortLevel);
	local tbAwardData = KAward.GetAssetAwardData("ProtectEscort", nEscortLevel);
	pPlayer.SendBlackMsg(string.format(Lang.escort.str17[Lang.Idx], tbAwardData.nEnergy));
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, Escort.OnCharacterEnterScene, Escort);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, Escort.OnLogout, Escort)