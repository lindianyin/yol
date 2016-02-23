local ActivityProgressBar = Npc:GetClass("activity_progressbar")
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

	
function ActivityProgressBar:OnDialog(pFlagNpc)
	local szDescription = Activity:GetProgressBarDescription(me.dwId)
	local nProgressTime = Activity:GetProgressBarTime(me.dwId)
	local nNpcId = pFlagNpc.dwTemplateId;  
	
	-- 当前只用在战场抢旗，如果在其他活动也要滚动条对话，移到ActivityManager
	local pNpc = KGameBase.GetNpcById(him.dwId);
	local pMePlayer = KGameBase.GetPlayerById(me.dwId);
	if (pNpc.GetCurrentCamp() ~= pMePlayer.GetCurrentCamp()) then  -- 玩家与战场旗帜阵营不一致，出现抢旗滚动条	
		ProgressBar:Init();
		ProgressBar:StartProcessOnPlayer(me, szDescription, nProgressTime, 
			{self.FinishedCallback, self, me.dwId, him.dwId}, {self.CancelCallback, self, me.dwId}, tbBreakEvent);
	end
end

function ActivityProgressBar:FinishedCallback(nPlayerId, dwNpcId)
	Activity:OnProgressBarFinished(nPlayerId, dwNpcId)
end

function ActivityProgressBar:CancelCallback(nId)
	Activity:OnProgressBarCancel(nId)
end

