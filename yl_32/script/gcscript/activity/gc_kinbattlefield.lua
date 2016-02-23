if not (MODULE_GAMECENTER) then
	return
end

Activity.MIN_JOIN_KIN_BATTLE_NUMS = 1;		-- 最小报名人数8人

Activity.tbApplyPlayers = {};

-- 重置报名人数
function Activity:ResetApplyPlayers()
	self.tbApplyPlayers = {};
end

-- 家族战报名
function Activity:ApplyJoinKinBattle_GC(nPlayerId, nKinId)
	if not self.tbApplyPlayers then
		self.tbApplyPlayers = {};
	end
	
	if not self.tbApplyPlayers[nKinId] then
		self.tbApplyPlayers[nKinId] = {};
	end
	
	local tbApply = self.tbApplyPlayers[nKinId];
	
	tbApply[nPlayerId] = 1;
	
	self:NoticeJoinKinBattle(nKinId);
end

function Activity:GetApplyKinBattlePlayerNum(nKinId, nPlayerId)
	local nHasJoined = 0;
	
	if not self.tbApplyPlayers or not self.tbApplyPlayers[nKinId] then
		return 0, nHasJoined;
	end
	
	local nCount = 0;
	local tbApply = self.tbApplyPlayers[nKinId];
	
	for nPlayerId, _ in pairs(tbApply) do
		nCount = nCount + 1;
	end
	
	if tbApply[nPlayerId] and tbApply[nPlayerId] == 1 then
		nHasJoined = 1;
	end
	
	return nCount, nHasJoined;
end

function Activity:NoticeJoinKinBattle(nKinId)
	local tbMembers = GetTongMembers(nKinId);
	
	for _, nPlayerId in ipairs(tbMembers) do
		local nJoinNum, nHasJoined = self:GetApplyKinBattlePlayerNum(nKinId, nPlayerId);
		GlobalExecute({"Activity:NoticeJoinKinBattle_GS", nPlayerId, nJoinNum, nHasJoined});
	end
end

function Activity:OnGetApplyKinBattleNum_GC(nPlayerId, nKinId)
	local nJoinNum, nHasJoined = self:GetApplyKinBattlePlayerNum(nKinId, nPlayerId);
	GlobalExecute({"Activity:OnGetApplyKinBattleNum_GS", nPlayerId, nJoinNum, nHasJoined});
end

function Activity:ApplyEnterKinBattle_GC(nPlayerId, nKinId)
	local nCount = self:GetApplyKinBattlePlayerNum(nKinId);
	
	if nCount < Activity.MIN_JOIN_KIN_BATTLE_NUMS then
		return;
	end
	
	GlobalExecute({"Activity:DoEnterKinBattle", nPlayerId});
end

function KinBattle:CheckOldWinnerAward_GC(nOldWinnerId, nWinKinId)
	GlobalExecute({"KinBattle:DoCheckOldWinnerAward_GS", nOldWinnerId, nWinKinId});
end

function KinBattle:CheckOldWinnerMemberAward_GC(nWinKinId)
	GlobalExecute({"KinBattle:DoCheckOldWinnerMemberAward", nWinKinId});
end

function KinBattle:ReloadKinBattleInfo_GC()
	GlobalExecute({"KinBattle:DoReloadKinBattleInfo"});
end