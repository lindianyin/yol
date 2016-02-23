local tbMultiPick = Npc:GetClass("multi_pickup")
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

function tbMultiPick:OnDialog(pPickNpc)
	local nNpcId = pPickNpc.dwTemplateId;
	local nPickTime = 5;
	local nNeedRoom = nil;

	if MultiInstance.PICK_CD_TIME[nNpcId] then
		local tbTmp = pPickNpc.GetTempTable("MultiInstance");
		local pGame = tbTmp.tbInstance;
		assert(pGame);
		if not pGame.dwLastPickTime then
		    pGame.dwLastPickTime = 0;
		end

		local tNow = KGameBase.GetTime();
		if tNow - pGame.dwLastPickTime < MultiInstance.PICK_CD_TIME[nNpcId] then
			me.SendBlackMsg(MultiInstance.PICK_CD_TIPS[nNpcId]);
			return;
		end
	end

	if MultiInstance.PICK_TIME[nNpcId] ~= nil then
		nPickTime = MultiInstance.PICK_TIME[nNpcId]
	end

	if MultiInstance.PICK_NEED_ROOM[nNpcId] ~= nil then
		nNeedRoom = MultiInstance.PICK_NEED_ROOM[nNpcId]
	end

	self:PickUpItem(nPickTime, nNeedRoom);
end

MultiInstance.PICK_REMOVE_NPC =
{
	[1304] = 1,
	[1305] = 1,
	[1306] = 1,
	[1307] = 1,
	[1319] = 1,
	[1320] = 1,
	[1321] = 1,
	[1322] = 1,
	[1334] = 1,
	[1335] = 1,
	[1336] = 1,
	[1337] = 1,
	[1380] = 1,
};

MultiInstance.PICK_TIME =  --采集时间（秒）
{ --[npcId] = 采集时间（默认时间5秒）
	[1265] = 5,
	[1266] = 5,
	[1267] = 5,

	[1272] = 5,
	[1273] = 5,
	[1274] = 5,

	[1275] = 5,
	[1276] = 5,
	[1277] = 5,
};

MultiInstance.PICK_CD_TIME = 	-- 采集CD时间
{
	[1265] = 10,
	[1266] = 10,
	[1267] = 10,

	[1272] = 10,
	[1273] = 10,
	[1274] = 10,

	[1275] = 10,
	[1276] = 10,
	[1277] = 10,
};

MultiInstance.PICK_CD_TIPS =
{
	[1265] = Lang.mission.str983[Lang.Idx],
	[1266] = Lang.mission.str983[Lang.Idx],
	[1267] = Lang.mission.str983[Lang.Idx],

	[1272] = Lang.mission.str983[Lang.Idx],
	[1273] = Lang.mission.str983[Lang.Idx],
	[1274] = Lang.mission.str983[Lang.Idx],

	[1275] = Lang.mission.str983[Lang.Idx],
	[1276] = Lang.mission.str983[Lang.Idx],
	[1277] = Lang.mission.str983[Lang.Idx],
};

MultiInstance.PICK_NEED_ROOM =
{ --[npcid] = 每次采集收集道具数量（默认数量0）
};

MultiInstance.PICK_ADD_NPC =
{
		--		要删除的npc组id集合		AddNpc参数
	[1265] = { {"13", "14", "15"}, 		{13, 1, 1, "13", "paota1"} },	-- 单攻炮台1
	[1266] = { {"13", "14", "15"}, 		{14, 1, 1, "14", "paota1"} },	-- 群攻炮台1
	[1267] = { {"13", "14", "15"}, 		{15, 1, 1, "15", "paota1"} },	-- 减速炮台1

	[1272] = { {"16", "17", "18"}, 		{13, 1, 1, "16", "paota2"} },	-- 单攻炮台2
	[1273] = { {"16", "17", "18"}, 		{14, 1, 1, "17", "paota2"} },	-- 群攻炮台2
	[1274] = { {"16", "17", "18"}, 		{15, 1, 1, "18", "paota2"} },	-- 减速炮台2

	[1275] = { {"19", "20", "21"}, 		{13, 1, 1, "19", "paota3"} },	-- 单攻炮台3
	[1276] = { {"19", "20", "21"}, 		{14, 1, 1, "20", "paota3"} },	-- 群攻炮台3
	[1277] = { {"19", "20", "21"}, 		{15, 1, 1, "21", "paota3"} },	-- 减速炮台3

};

function tbMultiPick:PickUpItem(nTime, nNeedBagRoom)
	assert(nTime > 0);
	--nTime = MultiInstance.PICK_TIME[nPickUpTime];
	if nNeedBagRoom and me.pItem.CountFreeBagCell() < nNeedBagRoom then
		me.SysMsg(Lang.task.target.str47[Lang.Idx]);
		return;
	end

	if me.IsDead() == 1 then
		return;
	end

	ProgressBar:Init();
	ProgressBar:StartProcessOnPlayer(me, Lang.mission.str558[Lang.Idx], nTime * Env.GAME_FPS,
		{self.PickCallback, self, me.dwId, him.dwId}, {self.CancelCallback, self, me.dwId}, tbBreakEvent);
end

function tbMultiPick:PickCallback(nPlayerId, dwNpcId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local pNpc = KGameBase.GetNpcById(dwNpcId);
	if not pPlayer or not pNpc then
		return;
	end

	MultiInstance:NpcUnLock(pNpc);

	if MultiInstance.PICK_REMOVE_NPC[pNpc.dwTemplateId]  then
		KGameBase.RemoveNpc(pNpc.dwId);
	end

	local tbTmp = pNpc.GetTempTable("MultiInstance");
	local pGame = tbTmp.tbInstance;
	assert(pGame);

	local tNow = KGameBase.GetTime();
	pGame.dwLastPickTime = tNow;

	local nNpcId = pNpc.dwTemplateId;
	if MultiInstance.PICK_ADD_NPC[nNpcId] then
		local tbAdd = MultiInstance.PICK_ADD_NPC[nNpcId];
		for _, szGroup in ipairs(tbAdd[1]) do
			pGame:DelNpc(szGroup);
		end

		pGame:AddNpc(unpack(tbAdd[2]));
	end
end

function tbMultiPick:CancelCallback(nId)
end

