
if not MODULE_GAMESERVER then
	return
end

Player.KILL_COUNT_GID = 5006;
Player.KILL_PLAYER_COUNT_ID = 1;


function Player:ClearTempRevive(pPlayer)
	local tbTempTable = pPlayer.GetCharacterTempTable();
	local tbTempRevive = tbTempTable.tbTempRevive;
	if tbTempTable.tbTempRevive then
		tbTempTable.tbTempRevive = nil;
	end
end

function Player:OnDeath(nKillerId, nPlayerId)
	print("~~Player:OnDeath(nKillerId, nPlayerId)",nKillerId, nPlayerId);
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	if not pPlayer then
		return;
	end
	
	local pKiller = KGameBase.GetPlayerById(nKillerId)
	if pKiller ~= nil then
	   pKiller.SysMsg(Lang.player.str42[Lang.Idx] ..pPlayer.szName)
	   pPlayer.SysMsg(Lang.player.str43[Lang.Idx]..pKiller.szName..Lang.player.str44[Lang.Idx])
	   
	   local nKillCount = pKiller.pTask.GetTask(Player.KILL_COUNT_GID, Player.KILL_PLAYER_COUNT_ID) or 0;
	   pKiller.pTask.DirectSetTask(Player.KILL_COUNT_GID, Player.KILL_PLAYER_COUNT_ID, nKillCount+1);
	   DailyTasks:commitTaskValue(pKiller,"",DailyTasks.TYPE_KILL_PLAYER,1);
	end
	local pKillerNpc = KGameBase.GetNpcById(nKillerId)
	if pKillerNpc ~= nil then
	   pPlayer.SysMsg(Lang.player.str45[Lang.Idx] .. pKillerNpc.szName .. Lang.player.str44[Lang.Idx])
	end
	
	local tbTempTable = pPlayer.GetCharacterTempTable();
	local tbTempRevive = tbTempTable.tbTempRevive;
	if not tbTempTable.tbTempRevive then
		print("~~not tbTempTable.tbTempRevive");
		tbTempTable.tbTempRevive = 
		{
			bWait = true;
			nWaitTimes = 1;
			bUseItem = true;
			bReturn = false;
			dwEffectSceneId = pPlayer.dwSceneId;
			dwSceneId = pPlayer.dwSceneId;
		};
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetReviveInfo", true, true, false, 0});
	elseif tbTempRevive and tbTempRevive.dwEffectSceneId == pPlayer.dwSceneId then
		print("~~tbTempRevive and tbTempRevive.dwEffectSceneId == pPlayer.dwSceneId");
		print("~~open relive panal", tbTempRevive.bWait, tbTempRevive.bUseItem, tbTempRevive.bReturn, tbTempRevive.nWaitTimes);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetReviveInfo", tbTempRevive.bWait, tbTempRevive.bUseItem, tbTempRevive.bReturn, tbTempRevive.nWaitTimes});
		tbTempRevive.nWaitTimes = tbTempRevive.nWaitTimes + 1;
		tbTempRevive.nDeathTime = KGameBase.GetTime();
	else
		print("~~else");
		Lib:CallBack(tbTempRevive);
	end
	
end

-- 回满血
function Player:FullHealth(pPlayer)
	local nMaxLife = pPlayer.GetMaxLife();
	pPlayer.SetCurrentLife(nMaxLife);
	
	local nMaxMana = pPlayer.GetMaxMana();
	pPlayer.SetCurrentMana(nMaxMana);
end

function Player:CheckRevive(pPlayer, nChoice)
	print("..Player:CheckRevive nChoice=" .. nChoice);
	local tbTempTable = pPlayer.GetCharacterTempTable();
	local tbTempRevive = tbTempTable.tbTempRevive;
	local tbReviveItem = {nGenre = 18, nDetail = 1, nParticular = 213};		-- 普通还魂丹
	local tbReviveProItem = {nGenre = 18, nDetail = 1, nParticular = 212};	-- 高级还魂丹

	if (nChoice == 1) then
		--pPlayer.ReturnToLastCity();
		--Scene:c2s_LeaveMission(pPlayer);
		--return true;
		return tbTempRevive.bWait;
	elseif (nChoice == 2) then
		local tbNeedItem;
		local nMapId = pPlayer.dwSceneTemplateId;
		if not MultiInstance.VIP_MAP_IDS[nMapId] then
			tbNeedItem = tbReviveItem;
		else
			-- vip多人副本要高级还魂丹
			tbNeedItem = tbReviveProItem;
		end
		
		local tbFind = pPlayer.pItem.FindItemInBags(tbNeedItem.nGenre, tbNeedItem.nDetail, tbNeedItem.nParticular, 0);
		local nCount = Item:GetItemAmount(tbFind);
		if nCount < 1 then
			local tbBaseProp = KItem.GetItemBaseProp(tbNeedItem.nGenre, tbNeedItem.nDetail, tbNeedItem.nParticular, 0);
			pPlayer.SendBlackMsg(string.format(Lang.player.str46[Lang.Idx], tbBaseProp.szName));
			return false;
		end
		pPlayer.pItem.ConsumeItemInBags(1, tbNeedItem.nGenre, tbNeedItem.nDetail, tbNeedItem.nParticular);
		return true;
	elseif (nChoice == 3) then
		return tbTempRevive.bReturn;
	elseif(nChoice == 4) then
		--消耗金钱
		local nNeedCoin = 0;
		local nMapId = pPlayer.dwSceneTemplateId;	
		if(not MultiInstance.VIP_MAP_IDS[nMapId])then
			nNeedCoin = 20;
		else
			nNeedCoin = 100;
		end
		local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
		if (nCurCoin < nNeedCoin) then
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", nNeedCoin});
			return false;
		end
		pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -1 * nNeedCoin);
		--复活
		return true;
	else
		return false;
	end
end

function Player:SetTempRevive(pPlayer, bWait, bUseItem, bReturn, nScene, ...)
	print("..Player:SetTempRevive", bWait, bUseItem, bReturn);
	local tbTempTable = pPlayer.GetCharacterTempTable();
	
	tbTempTable.tbTempRevive = 
	{
		bWait = bWait;
		nWaitTimes = 0;
		bUseItem = bUseItem;
		bReturn = bReturn;
		tbPos = { ... };
		dwSceneId = (nScene == 0 and pPlayer.dwSceneId or nScene);
		dwEffectSceneId = pPlayer.dwSceneId;
	};
end

function Player:SetTempReviveFunc(pPlayer, func)
	local tbTempTable = pPlayer.GetCharacterTempTable();
	tbTempTable.tbTempRevive = func;
end

function Player:Follow_OnTeleport(dwPlayerId, nTeamId, dwDesSceneId, nX, nY, nZ, dwSrcSceneId, IsFork)
	if nTeamId == 0 then
		return
	end
	if IsFork == 1 then
		return
	end
	
	local pPlayer = KGameBase.GetPlayerById(dwPlayerId)
	if pPlayer.IsCaptain() == 0 then
		return
	end
	
	local dwNextPlayer = pPlayer.GetFollowedBy()
	while dwNextPlayer ~= 0 do
		local pNextPlayer = KGameBase.GetPlayerById(dwNextPlayer)
		if pNextPlayer then
			pNextPlayer.TeleportTo(dwDesSceneId, nX, nY, nZ)
			dwNextPlayer = pNextPlayer.GetFollowedBy()
		else
			dwNextPlayer = 0
			break
		end
	end
end

function Player:c2s_CommitSurveyResult(pPlayer, nLevel, bPlayerDo)
	do	--临时屏蔽该功能
		return
	end
	if (not pPlayer or not nLevel) then
		return
	end
	
	--新手调查问卷，在这4个级别让玩家做问卷
	local tbLevel2Var = Player:GetFreshManSurveyLevel2VarTable() or {}
	local wVarId = tbLevel2Var[nLevel]
	if (not wVarId) then
		return
	end
	local nValue = pPlayer.GetVar(Character.emKCUSTOM_VARGROUP_FRESHMAN, wVarId)
	if (nValue > 0) then	-- 已经做过这个问卷了，提示玩家
		pPlayer.SysMsg(Lang.player.str47[[Lang.Idx]])
		return
	end
	if (nValue == 0) then
		pPlayer.SetVar(Character.emKCUSTOM_VARGROUP_FRESHMAN, wVarId, 1)	--设置为已做过
		if (bPlayerDo == 1) then
			self:SendSurveyAward(pPlayer, nLevel)
			pPlayer.SetVar(Character.emKCUSTOM_VARGROUP_FRESHMAN, wVarId, 2)	--设置为已发奖
		end
	end
end

function Player:SendSurveyAward(pPlayer, nLevel)
	if (not pPlayer or not nLevel) then
		return
	end
	
	local tbAward = self.tbFreshManSurveyAward[nLevel]
	if (not tbAward) then
		return
	end
	
	local tbMailInfo = {}
	tbMailInfo.nIconId = Mail.ICONID_PLAYER
	tbMailInfo.nType = 0		-- 系统邮件类型
	tbMailInfo.tbReceiver = {pPlayer.szName}
	tbMailInfo.szContent = string.format(Lang.player.str48[Lang.Idx],nLevel)
	tbMailInfo.szCaption = Lang.player.str49[Lang.Idx]
	tbMailInfo.tbItem = {}
	
	for i, nItemId in ipairs(tbAward) do
		local tbOne = {}
		tbOne.nClassId = nItemId
		tbOne.nCount = 1
		tbOne.nTimeout = 0
		tbOne.nPrice = 0
		tbOne.szGuid = ""
		tbMailInfo.tbItem[i] = tbOne
	end
	local nIndex = Mail:SendMailBySystem(tbMailInfo)
	return nIndex or -1
end


--临时的玩家复活处理
function Player:OnPlayerRevive(dwCharacterId, nChoice)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return
	end

	if nChoice == 3 then
		local tbTempTable = pPlayer.GetCharacterTempTable();
		local tbTempRevive = tbTempTable.tbTempRevive;
		if (tbTempRevive) then
			if type(tbTempRevive.tbPos[1]) ~= "table" then
				tbTempRevive.tbPos = { tbTempRevive.tbPos };
			end
			local nPosIdx = KUnify.MathRandom(1, #tbTempRevive.tbPos);
			local tbPos = tbTempRevive.tbPos[nPosIdx];
			pPlayer.TeleportTo(tbTempRevive.dwSceneId, tbPos[1], tbPos[2], tbPos[3], 0, 0, 1);
		end
	end

	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:CloseDeathPanel"});
	
	-- 如果是大神形态变回普通状态
	if pPlayer.GetAngerLeftTime() > 0 then
		KGameBase.CheckRemoveBladeState(pPlayer);
	end
end

function Player:CalcFightScore(dwCharacterId)
	print("......"..dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return 0;
	end
	pPlayer.CalcFightScore();
	return 0;
end

function Player:GetDegreeValue(pPlayer, szName)
	return Player.tbDegree:GetDegree(pPlayer, szName);
end

function Player:GetMaxDegreeValue(pPlayer, szName)
	return Player.tbDegree:GetMaxDegree(pPlayer, szName);
end

function Player:AddDegree(pPlayer, szName, nDegree)
	Player.tbDegree:AddDegree(pPlayer, szName, nDegree);
end

function Player:AddMaxDegree(pPlayer, szName, nDegree)
	Player.tbDegree:AddMaxDegree(pPlayer, szName, nDegree);
end

function Player:DelayCalcFightScore(dwCharacterId)
	Timer:Register(Env.GAME_FPS * 4, self.CalcFightScore, self, dwCharacterId);
end

if Player.nFollowTeleportEventId then
	CallCenter:UnRegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_TELEPORT, Player.nFollowTeleportEventId)
end
Player.nFollowTeleportEventId =
	CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_TELEPORT, Player.Follow_OnTeleport, Player)


--注册复活事件
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_REVIVE,Player.OnPlayerRevive,Player);

function Player:OnLevelUp(pPlayer)
	local nLevel = pPlayer.GetLevel();
	
	-- 角色升级回满血
	local nMaxLife = pPlayer.GetMaxLife();
	pPlayer.SetCurrentLife(nMaxLife);
	
	local nMaxMana = pPlayer.GetMaxMana();
	pPlayer.SetCurrentMana(nMaxMana);
	
	-- 检查可接的支线
	Task:CheckCanAcceptBranchTask_LevelUp(pPlayer, nLevel)
	
	-- 升级奖励
	if KAward.HasAssetAward("LevelUpGift", nLevel) == 1 then
		KAward.AddPlayerAward(pPlayer, "LevelUpGift", nLevel);
	end
	
	-- 活动提醒
	Activity:CheckActivityRemind(pPlayer);
	
	--Player:TestGiveVip(pPlayer);
end

function Player:TestGiveVip(pPlayer)
	local nLevel = pPlayer.GetLevel();
	if nLevel >= 10 and nLevel < 30 then
		pPlayer.SetVipLevel(3);
	elseif nLevel == 30 and nLevel < 35 then
		pPlayer.SetVipLevel(5);
	elseif nLevel == 35 and nLevel < 40 then
		pPlayer.SetVipLevel(7);
	elseif nLevel >= 40 then
		pPlayer.SetVipLevel(10);
	end
end

function Player:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	DailyTasks:resetDailyTaskWhenChangeDay(pPlayer);
	--if pPlayer then
		--Player:TestGiveVip(pPlayer);
	--end
end

function Player:OnEnterScene(nPlayerId)
	local pPlayer = KGameBase.GetCharacterById(nPlayerId);
	
	if pPlayer.IsNpc() == 1 then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	local szTypeName = KScene.GetSceneTypeName(nMapId);
	if szTypeName == "arena" 
	or szTypeName == "escort" 
	or szTypeName == "tower" then
		pPlayer.LeaveTeam();
	end
	
end

function Player:c2s_BroadcastCreateTeamMsg(pPlayer, szMsg)
	if pPlayer.GetSysTeamId() > 0 then
		return;
	end
	local tNow = KGameBase.GetTime();
	pPlayer.SetLastTeamMsgTime(tNow);
	-- 附近发送创建队伍信息
	KChat.BroadcastSysMsg(szMsg, 1, pPlayer.dwId, pPlayer.szName);
end

function Player:c2s_BroadcastCreateOverteamMsg(pPlayer, szMsg)
	-- 世界发送创建队伍信息
	local tNow = KGameBase.GetTime();
	pPlayer.SetLastTeamMsgTime(tNow);
	
	KChat.BroadcastSysMsg(szMsg, 21, pPlayer.dwId, pPlayer.szName);
end

function Player:c2s_BroadcastEscortHelpMsg(pPlayer, szMsg)
	KChat.BroadcastSysMsg(szMsg, 21, pPlayer.dwId, pPlayer.szName);
end

function Player:checkAssetAward(pPlayer,num)
	--[[if num or num = 4;]]
	if KAward.HasAssetAward("LevelUpGift", num) == 1 then
		KAward.AddPlayerAward(pPlayer, "LevelUpGift", num);
	end
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, Player.OnEnterScene, Player);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Player.OnLogin, Player);
