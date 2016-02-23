local ActivityProgressBar_MPJJ = Npc:GetClass("activity_progressbar_mpjj")
Include("script/misc/progressbar_define.lua")

local tbAllPos = {
{dwSceneId = 338, nX = 3879, nY = 3637, nZ = 1048576},
--{dwSceneId = 338, nX = 3707, nY = 3751, nZ = 1048576},
{dwSceneId = 338, nX = 3519, nY = 3925, nZ = 1048576},
--{dwSceneId = 338, nX = 3382, nY = 4150, nZ = 1048576},
{dwSceneId = 338, nX = 3365, nY = 4348, nZ = 1048576},
--{dwSceneId = 338, nX = 3425, nY = 4568, nZ = 1048576},
{dwSceneId = 338, nX = 3557, nY = 4780, nZ = 1048576},
--{dwSceneId = 338, nX = 3746, nY = 4939, nZ = 1048576},
{dwSceneId = 338, nX = 3984, nY = 5043, nZ = 1048576},
--{dwSceneId = 338, nX = 4205, nY = 5059, nZ = 1048576},
{dwSceneId = 338, nX = 4410, nY = 5016, nZ = 1048576},
--{dwSceneId = 338, nX = 4573, nY = 4905, nZ = 1048576},
{dwSceneId = 338, nX = 4709, nY = 4748, nZ = 1048576},
--{dwSceneId = 338, nX = 4755, nY = 4595, nZ = 1048576},
{dwSceneId = 338, nX = 4762, nY = 4473, nZ = 1048576},
{dwSceneId = 338, nX = 4766, nY = 4282, nZ = 1048576},
{dwSceneId = 338, nX = 4733, nY = 4170, nZ = 1048576},
{dwSceneId = 338, nX = 4610, nY = 3972, nZ = 1048576},
{dwSceneId = 338, nX = 4428, nY = 3790, nZ = 1048576},
{dwSceneId = 338, nX = 4192, nY = 3687, nZ = 1048576},
{dwSceneId = 338, nX = 4333, nY = 3912, nZ = 1048576},
{dwSceneId = 338, nX = 4560, nY = 4181, nZ = 1048576},
{dwSceneId = 338, nX = 4511, nY = 4543, nZ = 1048576},
{dwSceneId = 338, nX = 4193, nY = 4696, nZ = 1048576},
{dwSceneId = 338, nX = 3880, nY = 4563, nZ = 1048576},
{dwSceneId = 338, nX = 3697, nY = 4328, nZ = 1048576},
{dwSceneId = 338, nX = 3812, nY = 4057, nZ = 1048576},
{dwSceneId = 338, nX = 4068, nY = 3948, nZ = 1048576},
{dwSceneId = 338, nX = 4280, nY = 4101, nZ = 1048576},
{dwSceneId = 338, nX = 4320, nY = 4416, nZ = 1048576},
{dwSceneId = 338, nX = 3973, nY = 4417, nZ = 1048576},
{dwSceneId = 338, nX = 4037, nY = 4177, nZ = 1048576},


}; --掉落的位置
local tbItem = {nGenre = 18, nDetail = 1, nParticular = 122, nLevel = 0, nAmount = 1}; --掉落宝石箱
local tbItem2 = {nGenre = 18, nDetail = 1, nParticular = 123, nLevel = 0, nAmount = 1}; --掉落属性丹箱

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


function ActivityProgressBar_MPJJ:OnDialog(pFlagNpc)
	local nProgressTime = 5;	--进度条时间
	local szDescription = Lang.mission.str80[Lang.Idx];	--进度条描述

	local nNpcId = pFlagNpc.dwTemplateId;

	-- 当前只用在战场抢旗，如果在其他活动也要滚动条对话，移到ActivityManager
	local pNpc = KGameBase.GetNpcById(him.dwId);
	local pMePlayer = KGameBase.GetPlayerById(me.dwId);
	if Activity.dwMPJJChampionId and Activity.dwMPJJChampionId == pMePlayer.dwId then  -- 玩家与战神无双冠军ID
		ProgressBar:Init();
		ProgressBar:StartProcessOnPlayer(me, szDescription, nProgressTime,
			{self.FinishedCallback, self, me.dwId, him.dwId}, {self.CancelCallback, self, me.dwId}, tbBreakEvent);
	else
		pMePlayer.SendBlackMsg(Lang.mission.str81[Lang.Idx]);
	end
end

function ActivityProgressBar_MPJJ:FinishedCallback(nPlayerId, dwNpcId)
	local nCount = 0
	if Activity.dwMPJJChampionId and Activity.dwMPJJChampionId == nPlayerId then
		for _, tbPos in pairs(tbAllPos) do
		     nCount = nCount + 1;
		    if nCount % 2 == 1 then
				Doodad:DropItem(5, 0, {}, tbPos, tbItem);
			else
			    Doodad:DropItem(5, 0, {}, tbPos, tbItem2);
			end
		end
		KGameBase.RemoveNpc(dwNpcId);
	end
end

function ActivityProgressBar_MPJJ:CancelCallback(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	if pPlayer then
		pPlayer.SendBlackMsg(Lang.mission.str82[Lang.Idx]);
	end
end
