local tbStagePick = Npc:GetClass("stage_pickup")
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


function tbStagePick:OnDialog(pPickNpc)
	local nNpcId = pPickNpc.dwTemplateId;
	local nPickTime = 3 * Env.GAME_FPS;
	local nNeedRoom = nil;

	if self.PICK_TIME[nNpcId] ~= nil then
		nPickTime = self.PICK_TIME[nNpcId]
	end

	if self.PICK_NEED_ROOM[nNpcId] ~= nil then
		nNeedRoom = self.PICK_NEED_ROOM[nNpcId]
	end

	self:PickUpItem(nPickTime, nNeedRoom);
end

tbStagePick.PICK_TIME =  --采集时间（秒）
{ --[npcId] = 采集时间（默认时间5秒）
};

tbStagePick.PICK_NEED_ROOM =
{ --[npcid] = 每次采集收集道具数量（默认数量0）
};

function tbStagePick:PickUpItem(nTime, nNeedBagRoom)
	assert(nTime > 0);
	--nTime = self.PICK_TIME[nPickUpTime];
	if nNeedBagRoom and me.pItem.CountFreeBagCell() < nNeedBagRoom then
		me.SysMsg(Lang.mission.str1069[Lang.Idx]);
		return;
	end

	if me.IsDead() == 1 then
		return;
	end

	ProgressBar:Init();
	ProgressBar:StartProcessOnPlayer(me, Lang.mission.str558[Lang.Idx], nTime,
		{self.PickCallback, self, me.dwId, him.dwId}, {self.CancelCallback, self, me.dwId}, tbBreakEvent);
end

function tbStagePick:PickCallback(nPlayerId, dwNpcId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local pNpc = KGameBase.GetNpcById(dwNpcId);
	if not pPlayer or not pNpc then
		return;
	end
	--if pPlayer.CountFreeBagCell() < 1 then
	--	pPlayer.Msg("你的背包已经满啦！");
	--	return;
	--end
	Stage:NpcUnLock(pNpc);
	KGameBase.RemoveNpc(pNpc.dwId);
end

function tbStagePick:CancelCallback(nId)
end

