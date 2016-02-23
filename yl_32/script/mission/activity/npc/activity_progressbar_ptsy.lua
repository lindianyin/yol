local ActivityProgressBar_PTSY = Npc:GetClass("activity_progressbar_ptsy")

Include("script/misc/progressbar_define.lua")

local tbBreakEvent = {
		ProgressBar.EVENT_MOVE,
		ProgressBar.EVENT_LOGOUT,
		ProgressBar.EVENT_BE_HIT,
		ProgressBar.EVENT_DEATH,
		ProgressBar.EVENT_CAST_SKILL,
		ProgressBar.EVENT_COMBAT_START,
		ProgressBar.EVENT_PLOT_PLAY_START,
		ProgressBar.EVENT_NEW_PROGRESS,
		ProgressBar.EVENT_CLIENT_CANCEL,
		ProgressBar.EVENT_ITEM_OPS,
		ProgressBar.EVENT_CAPTAIN_CHANGE,
		ProgressBar.EVENT_LEAVESCENE,
	};

	
function ActivityProgressBar_PTSY:OnDialog(pFlagNpc)
	local nProgressTime = 5 * Env.GAME_FPS;	--进度条时间
	local szDescription = Lang.mission.str83[Lang.Idx];	--进度条描述

	local nNpcId = pFlagNpc.dwTemplateId;  
	local pNpc = KGameBase.GetNpcById(him.dwId);
	local pMePlayer = KGameBase.GetPlayerById(me.dwId);
	
	if self.mapProgressStates == nil then
	   self.mapProgressStates = {}
	end
	
	if self.mapProgressStates[him.dwId] ~= nil then
	   pMePlayer.SendBlackMsg(Lang.mission.str84[Lang.Idx])
	   return
	end

	ProgressBar:Init();
	ProgressBar:StartProcessOnPlayer(me, szDescription, nProgressTime, 
		{self.FinishedCallback, self, me.dwId, him.dwId}, {self.CancelCallback, self, me.dwId, him.dwId}, tbBreakEvent);

  self.mapProgressStates[him.dwId] = 1;
end

function ActivityProgressBar_PTSY:FinishedCallback(nPlayerId, dwNpcId)
	Activity:OnProgressBarFinished(nPlayerId, dwNpcId);
	self.mapProgressStates[dwNpcId] = nil
end

function ActivityProgressBar_PTSY:CancelCallback(nPlayerId, dwNpcId)
	Activity:OnProgressBarCancel(nPlayerId);
	self.mapProgressStates[dwNpcId] = nil
end