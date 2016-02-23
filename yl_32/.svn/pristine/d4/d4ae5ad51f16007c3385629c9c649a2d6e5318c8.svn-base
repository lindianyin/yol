local ActivityProgressBar = Npc:GetClass("activity_progressbar_sjzb")
Include("script/misc/progressbar_define.lua")

local tbBreakEvent = {
		ProgressBar.EVENT_MOVE,
		ProgressBar.EVENT_LOGOUT,
		--ProgressBar.EVENT_BE_HIT,		-- 被击不打断
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
	
ActivityProgressBar.nCurDialogId = 0;

local function GetCurHoldInfo(nPlayerId)
	local tbActivity = Activity.tbActivities[26];
	assert(tbActivity);
	
	local tbStep = tbActivity.tbSteps[1];
	
	return tbStep.tbCurHoldInfo;
end
	
function ActivityProgressBar:OnDialog(pFlagNpc)
	assert(me.GetKinId() > 0);
	
	-- 当前只用在战场抢旗，如果在其他活动也要滚动条对话，移到ActivityManager
	local tbHoldInfo = GetCurHoldInfo(me.dwId);
	local tbKinInfo = GetPlayerKinInfo(me.dwId);
	
	assert(tbHoldInfo);
	
	if tbKinInfo.dwKinMasterID ~= me.dwId then
		me.SendBlackMsg(Lang.mission.str85[Lang.Idx]);
		return;
	end
	
	if me.GetKinId() == tbHoldInfo.nKinId then
		return;
	end
	
	if self.nCurDialogId ~= 0 and self.nCurDialogId ~= me.dwId then
		local pCurHoldPlayer = KGameBase.GetPlayerById(self.nCurDialogId);
		me.SendBlackMsg(string.format(Lang.mission.str86[Lang.Idx], pCurHoldPlayer.szName));
		return;
	end
	
	ProgressBar:Init();
	ProgressBar:StartProcessOnPlayer(me, Lang.mission.str87[Lang.Idx], 20 * Env.GAME_FPS, 
		{self.FinishedCallback, self, me.dwId, him.dwId}, {self.CancelCallback, self, me.dwId}, tbBreakEvent);
		
	self.nCurDialogId = me.dwId;
end

function ActivityProgressBar:FinishedCallback(nPlayerId, dwNpcId)
	Activity:OnProgressBarFinished(nPlayerId, dwNpcId)
	self.nCurDialogId = 0;
end

function ActivityProgressBar:CancelCallback(nId)
	Activity:OnProgressBarCancel(nId);
	self.nCurDialogId = 0;
end

