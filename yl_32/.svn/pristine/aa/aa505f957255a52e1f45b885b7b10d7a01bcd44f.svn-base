
-- 神族争霸福利奖励相关

-- 三界神皇套装gdpl
KinBattle.tbWinnerSuit = {1, 11, 1, 0};

KinBattle.BUFF_ATTACK = 1164;		-- 攻方头顶buff（纯效果）
KinBattle.BUFF_DEF = 1165;			-- 守方头顶buff

-- 加载神皇雕像
function KinBattle:OnLoadWinnerComplete(pNpc)
	KGameBase.AddNpc(pNpc, 65541, 3989, 4171, 1048576, 30);
	
	local nWinnerId = KActivity.GetKinBattleWinnerId();
	
	-- 公告
	local tbKinInfo = GetPlayerKinInfo(nWinnerId);
	KChat.BroadcastNoticeMsg(string.format(Lang.mission.str29[Lang.Idx], tbKinInfo.szKinName));
	
	local pWinner = KGameBase.GetPlayerById(nWinnerId);
	if not pWinner then
		return;
	end
	
	Timer:Register(3 * Env.GAME_FPS, self.BroadcastWinnerMsg, self, tbKinInfo.szKinName, pWinner.szName);
	
	-- 这里给套装
	KinBattle:CheckWinnerSuit(pWinner);
	
	-- 给禁言权限
	self:CheckChatDisableAuth(pWinner);
	
	local tbPlayers = KGameBase.GetAllPlayers()
	for _, pPlayer in pairs(tbPlayers) do
		if pPlayer.GetKinMasterId() == nWinnerId then
			-- 给称号
			KinBattle:CheckTitle(pPlayer);
		end
	end
end

function KinBattle:BroadcastWinnerMsg(szKinName, szWinnerName)
	KChat.BroadcastNoticeMsg(string.format(Lang.mission.str30[Lang.Idx], szKinName, szWinnerName));
	return 0;
end

-- 加载默认npc
function KinBattle:LoadDefaultWinner()
	local pNpc = KGameBase.CallNpc(2602, 65541, 4023, 4162, 1048576, 30);
	if pNpc then
		KActivity.SetKinBattleWinNpcId(pNpc.dwId);
	end
end

function KinBattle:ReloadKinBattleInfo()
	KGameBase.CenterExecute{"KinBattle:ReloadKinBattleInfo_GC"};
	return 0;
end

function KinBattle:DoReloadKinBattleInfo()
	KActivity.ReloadKinBattleInfo();
end

-- 是否三界神皇
function KinBattle:IsWinner(pPlayer)
	local nWinnerId = KActivity.GetKinBattleWinnerId();
	if pPlayer.dwId == nWinnerId then
		return 1;
	end
	return 0;
end

-- 是否跟三界神皇同一家族
-- 目前此函数不能在登陆时候使用
function KinBattle:IsWinnerMember(pPlayer)
	if pPlayer.GetKinId() <= 0 then
		return 0;
	end
	
	local nWinnerId = KActivity.GetKinBattleWinnerId();
	
	if pPlayer.GetKinMasterId() == nWinnerId and pPlayer.dwId ~= pPlayer.GetKinMasterId() then
		return 1;
	end
	
	return 0;
end

-- 检查三界神皇套装，给予或者删除
function KinBattle:CheckWinnerSuit(pPlayer)
	if self:IsWinner(pPlayer) ~= 1 then
		Item:DelPlayerItem(pPlayer, unpack(KinBattle.tbWinnerSuit));
		return;
	end
	
	if Item:IfHasItem(pPlayer, unpack(KinBattle.tbWinnerSuit)) == 1 then
		return;
	end
	
	-- 给套装
	KItem.AddPlayerItem(pPlayer, unpack(KinBattle.tbWinnerSuit));

end

-- 检查禁言权限
function KinBattle:CheckChatDisableAuth(pPlayer)
	if self:IsWinner(pPlayer) ~= 1 then
		KChat.SetChatDisableAuth(pPlayer, 0);
		return;
	end
	
	KChat.SetChatDisableAuth(pPlayer, 1);
end

function KinBattle:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	self:CheckWinnerSuit(pPlayer);
	self:CheckTitle(pPlayer);
	self:CheckChatDisableAuth(pPlayer);
	
	if self:IsWinner(pPlayer) == 1 and KActivity.GetKinBattleWinTimes(pPlayer) < 2 then
		KActivity.SetKinBattleWinTimes(pPlayer, 2);
	end

	if self:IsWinner(pPlayer) == 1 then 
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str1216[Lang.Idx],pPlayer.szName));
	end
	
end

-- 给称号
function KinBattle:CheckTitle(pPlayer)
	if self:IsWinner(pPlayer) == 1 then
		Title:AddTitleToCharacter(pPlayer, Title.KIN_BATTLE_WINNER, 1, 0);
	else
		Title:RemoveTitleToCharacter(pPlayer, Title.KIN_BATTLE_WINNER, 1, 0);
	end
	
	if self:IsWinnerMember(pPlayer) == 1 then
		Title:AddTitleToCharacter(pPlayer, Title.KIN_BATTLE_WINNER, 2, 0);
	else
		Title:RemoveTitleToCharacter(pPlayer, Title.KIN_BATTLE_WINNER, 2, 0);
	end
	
	-- 删除头顶攻守特效buff
	pPlayer.pBufferEffect.ClearBuff(KinBattle.BUFF_ATTACK);
	pPlayer.pBufferEffect.ClearBuff(KinBattle.BUFF_DEF);
end

-- ?gs Title:AddTitleToCharacter(me, Title.KIN_BATTLE_WINNER, 2, 0);

function KinBattle:DoCheckOldWinnerAward_GS(nOldWinnerId, nWinKinId)
	local pOldWinner = KGameBase.GetPlayerById(nOldWinnerId);
	if not pOldWinner or pOldWinner.GetKinId() == nWinKinId then
		return;
	end
	
	-- 删三界神皇套装
	Item:DelPlayerItem(pOldWinner, unpack(KinBattle.tbWinnerSuit));
	
	-- 删称号
	Title:RemoveTitleToCharacter(pOldWinner, Title.KIN_BATTLE_WINNER, 1, 0);
	
	-- 删禁言权限
	KChat.SetChatDisableAuth(pOldWinner, 0);
	
	KActivity.SetKinBattleWinTimes(pOldWinner, 0);
end

-- 删除原来三界神皇成员称号
function KinBattle:DoCheckOldWinnerMemberAward(nWinKinId)
	local tbPlayers = KGameBase.GetAllPlayers()
	for _, pPlayer in pairs(tbPlayers) do
		if pPlayer and pPlayer.GetKinId() ~= nWinKinId then
			Title:RemoveTitleToCharacter(pPlayer, Title.KIN_BATTLE_WINNER, 1, 0);
			KActivity.SetKinBattleWinTimes(pPlayer, 0);
		end
	end
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, KinBattle.OnLogin, KinBattle);
