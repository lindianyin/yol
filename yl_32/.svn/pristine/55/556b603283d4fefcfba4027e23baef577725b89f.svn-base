-- 逍遥谷挑战
XoyoGame.XoyoChallenge = XoyoGame.XoyoChallenge or {};
local XoyoChallenge = XoyoGame.XoyoChallenge;

-- NpcId索引的数据列表
--            打怪掉落率                                    随机卡开出来的概率
-- npcId --> {"nDropRate","tbCardGDPL", "id", "szCardGDPL", "nProbability"}
XoyoChallenge.tbNpcId2Data = {};
XoyoChallenge.tbNpcId2Data_id = {}; -- 内容与tbNpcId2Data一样，但用每行开头的id做索引

-- 物品GDPL索引的数据列表
-- "g,d,p,l" --> {"nNeededNum", "tbItemGDPL", "tbCardGDPL", "id", "szCardGDPL", "nProbability"}
XoyoChallenge.tbItem2Data = {};
XoyoChallenge.tbItem2Data_id = {}; -- 内容与tbItem2Data一样，但用每行开头的id做索引

-- 房间编号索引的数据列表
-- nRoomId --> {"tbCardGDPL", "id", "nDropRate", "szCardGDPL", "nProbability"}
XoyoChallenge.tbRoom2Data = {};
XoyoChallenge.tbRoom2Data_id = {}; -- 内容与tbRoom2Data一样，但用每行开头的id做索引

-- 反向索引，卡片储存在哪个地方
--                  [1]    [2]    [3]    [4]
-- "g,d,p,l" --> {nTaskId, nBit, nIndex, nId,
-- szDesc, tbCardGDPL}
XoyoChallenge.tbCardStorage = {};
-- {self.nProbabilitySum, v.tbCardStorage}
XoyoChallenge.tbCardStorage_probability = {};

-- 把卡片收集情况储存在任务变量中，一位表示一张卡片
-- 由第1位开始，一个任务变量存满后就用下一个
XoyoChallenge.TASKGID = 2050;
XoyoChallenge.TASK_NPC_BEGIN = 20; -- npc任务用，每个2位，1表示已收集卡片，2表示已使用卡片
XoyoChallenge.TASK_ITEM_BEGIN = 24; -- 收集物品用
XoyoChallenge.TASK_ROOM_BEGIN = 28; -- 过房间用
XoyoChallenge.TASK_END = 32; -- 这个不使用
XoyoChallenge.TASK_GET_XOYOLU_MONTH = 41; -- 记录获得逍遥录的年和月，如200903
XoyoChallenge.TASK_HANDUP_XOYOLU_MONTH = 42; -- 上交的是那个时候获得的逍遥录，如200903
XoyoChallenge.TASK_SPECIAL_CARD_DATE = 43; -- 获得特殊卡的日期，如20090312
XoyoChallenge.TASK_SPECIAL_CARD_NUM = 44; -- 当天成功换特殊卡的数量
XoyoChallenge.TASK_GET_AWARD_MONTH = 45; -- 领奖时间，例如玩家在200906拿了逍遥录，他在下个月领奖后，就会把这个变量记上200906（拿逍遥录的月份）
XoyoChallenge.TASK_GET_AWARD_MONTH_COPY = 46; --7月临时使用领奖时间任务变量

XoyoChallenge.tbSpecialCard = {18,1,314,1}; -- 特殊卡
XoyoChallenge.tbXoyolu = {18,1,1,1}; -- 逍遥录
XoyoChallenge.MAX_SPECIAL_CARD_NUM = 2; -- 每天最多换两张特殊卡

XoyoChallenge.MINUTE_OF_MONTH = 32*24*60;

-------------- load file ---------------------------

function XoyoChallenge:LoadCommonEntry(nRowNum, tbRow)
	local g,d,p,l = tonumber(tbRow.CARD_G), tonumber(tbRow.CARD_D), tonumber(tbRow.CARD_P), tonumber(tbRow.CARD_L);
	local id = tonumber(tbRow.Id);
	assert(id == nRowNum);
	local tb = {["tbCardGDPL"] = {g,d,p,l}, ["szCardGDPL"] = string.format("%d,%d,%d,%d",g,d,p,l),
			["id"] = id, ["szDesc"] = tbRow.CARD_DESC, ["nWeight"] = tonumber(tbRow.CARD_WEIGHT), ["nProbability"] = tbRow.PROBABILITY};
	return tb, id;
end

function XoyoChallenge:__LoadDropRate(tbRow, szKey)
	local tbId = Lib:SplitStr(tbRow[szKey], "|"); -- ROOM_ID或者NPC_ID
	for i = 1, #tbId do
		tbId[i] = assert(tonumber(tbId[i]));
	end

	local tbDropRate = Lib:SplitStr(tbRow.DROP_RATE, "|");
	for i = 1, #tbDropRate do
		tbDropRate[i] = assert(tonumber(tbDropRate[i]));
	end

	assert(#tbId == #tbDropRate);
	return tbId, tbDropRate;
end

function XoyoChallenge:LoadFile()
local tbFile = Lib:LoadTabFile("\\setting\\xoyogame\\card_kill_npc.txt");
for i = 1, #tbFile do
	local tbRow = tbFile[i];

	local tbNpcId, tbDropRate = self:__LoadDropRate(tbRow, "NPCID");

	for j = 1, #tbNpcId do
		local nNpcId = tbNpcId[j];
		local nDropRate = tbDropRate[j];
		assert(XoyoChallenge.tbNpcId2Data[nNpcId] == nil, tostring(nNpcId));
		local tb, id = XoyoChallenge:LoadCommonEntry(i, tbRow);
		tb["nDropRate"] = nDropRate;
		XoyoChallenge.tbNpcId2Data[nNpcId] = tb;
		XoyoChallenge.tbNpcId2Data_id[id] = tb;
	end
end

tbFile = Lib:LoadTabFile("\\setting\\xoyogame\\card_collect_item.txt");
for i = 1, #tbFile do
	local tbRow = tbFile[i];
	local szKey = string.format("%s,%s,%s,%s", tbRow.ITEM_G, tbRow.ITEM_D, tbRow.ITEM_P, tbRow.ITEM_L);
	local nNum = tonumber(tbRow.NUM);
	assert(XoyoChallenge.tbItem2Data[szKey] == nil);
	local tb, id = XoyoChallenge:LoadCommonEntry(i, tbRow);
	tb["nNeededNum"] = nNum;
	tb["tbItemGDPL"] = {tonumber(tbRow.ITEM_G), tonumber(tbRow.ITEM_D), tonumber(tbRow.ITEM_P), tonumber(tbRow.ITEM_L)};
	XoyoChallenge.tbItem2Data[szKey] = tb;
	XoyoChallenge.tbItem2Data_id[id] = tb;
end

tbFile = Lib:LoadTabFile("\\setting\\xoyogame\\card_room.txt");
for i = 1, #tbFile do
	local tbRow = tbFile[i];
	local tbRoomId, tbDropRate = self:__LoadDropRate(tbRow, "ROOM_ID");

	for j = 1, #tbRoomId do
		local nRoomId = tbRoomId[j];
		local nDropRate = tbDropRate[j];
		assert(XoyoChallenge.tbRoom2Data[nRoomId] == nil);
		local tb, id = XoyoChallenge:LoadCommonEntry(i, tbRow);
		local nCloseLock = tbRow.CLOSE_LOCK;
		tb["nDropRate"] = nDropRate;
		tb["nCloseLock"] = nCloseLock;
		XoyoChallenge.tbRoom2Data[nRoomId] = tb;
		XoyoChallenge.tbRoom2Data_id[id] = tb;
	end
end
end
--------------- load file end------------------------

XoyoChallenge.nCardNum = 0; -- 卡片总数
XoyoChallenge.nProbabilitySum = 0; -- 概率总和
function XoyoChallenge:InitCardStorage()
	local tbCtrl = {
		{self.tbNpcId2Data, self.TASK_NPC_BEGIN},
		{self.tbItem2Data,  self.TASK_ITEM_BEGIN},
		{self.tbRoom2Data,  self.TASK_ROOM_BEGIN},
		{nil, 				self.TASK_END}
	};

	for i = 1, #tbCtrl - 1 do
		local info = tbCtrl[i];

		for _, v in pairs(info[1]) do
			local szKey = string.format("%d,%d,%d,%d", unpack(v.tbCardGDPL));
			local nTaskId = info[2] + math.floor((v.id - 1)*2 / 32);
			local nBit = math.fmod((v.id - 1)*2, 32);

			assert(nTaskId < tbCtrl[i + 1][2], string.format("i:%d, id:%d", i, v.id)); -- 不能超过该类任务的数量限制(id太大)

			if self.tbCardStorage[szKey] and
				not (self.tbCardStorage[szKey][3] == i and self.tbCardStorage[szKey][4] == v.id) then -- npc表有多个npcId对应同一个卡片的情况

				assert(false, string.format("prev i:%d, prev id:%d, curr i:%d, curr id:%d, gdpl:%s",
					self.tbCardStorage[szKey][3], self.tbCardStorage[szKey][3], i, v.id, szKey)); -- 卡片gdpl不能重复
			end

			if not self.tbCardStorage[szKey] then
				self.nProbabilitySum = self.nProbabilitySum + v.nProbability;
				self.tbCardStorage[szKey] = {nTaskId, nBit, i, v.id}; -- i和v.id构成该行的唯一标识
				self.tbCardStorage[szKey]["szDesc"] = v.szDesc;
				self.tbCardStorage[szKey]["tbCardGDPL"] = v.tbCardGDPL;
				self.tbCardStorage[szKey]["nWeight"] = v.nWeight;
				table.insert(self.tbCardStorage_probability, {self.nProbabilitySum, v.tbCardGDPL});
				self.nCardNum = self.nCardNum + 1;
			end
		end
	end

	self.nTotalWeight = 0;
	for _, v in pairs(self.tbCardStorage) do
		self.nTotalWeight = self.nTotalWeight + v.nWeight;
	end
	self.__tbRange = self:TransRange({self.MINUTE_OF_MONTH, self.nTotalWeight, self.nCardNum});
end

function XoyoChallenge:PrintCardState()
	local tbCtrl = {
		self.tbNpcId2Data,
		self.tbItem2Data,
		self.tbRoom2Data,
		};
	for _, tbData in pairs(tbCtrl) do
		for _id, _info in pairs(tbData) do
			local nState = self:GetCardState(me, _info.szCardGDPL);
			local nTaskId = self.tbCardStorage[_info.szCardGDPL][1];
			local nTaskValue = me.pTask.GetTask(self.TASKGID, nTaskId);
			print("name: ", _info.szCardGDPL, "state: ", nState, "taskvalue: ", nTaskValue, "taskid:", nTaskId);
		end
	end
end

-- 获取卡片收集状况，已收集返回1，未收集返回0
function XoyoChallenge:GetCardState(pPlayer, szCardGDPL)
	local info = self.tbCardStorage[szCardGDPL];
	assert(info, szCardGDPL);
	local nTask = info[1];
	local nBit = info[2];

	local nVal = pPlayer.pTask.GetTask(self.TASKGID, nTask);
	return Lib:LoadBits(nVal, nBit, nBit+1);
end

-- 设置卡片收集状态
function XoyoChallenge:SetCardState(pPlayer, szCardGDPL, value)
	local info = self.tbCardStorage[szCardGDPL];
	assert(info, szCardGDPL);
	local nTask = info[1];
	local nBit = info[2];
	local nVal = pPlayer.pTask.GetTask(self.TASKGID, nTask);
	nVal = Lib:SetBits(nVal, value, nBit, nBit+1);
	pPlayer.pTask.DirectSetTask(self.TASKGID, nTask, nVal);
end

function XoyoChallenge:TransRange(tbRange, nMax)
	local tbRes = {}
	local n = 1
	for _, v in ipairs(tbRange) do
		n = n*(v+1)
		table.insert(tbRes,n)
	end
	local max = table.remove(tbRes)
	if not nMax then nMax = 2147483648 end
	assert(max < nMax)
	table.insert(tbRes, 1, 0)
	return tbRes
end

-- 杀死npc给队员卡片
function XoyoChallenge:KillNpcForCard(pPlayer, pNpc)
	if not pPlayer then
		return 0;
	end

	local nNpcTemplateId = pNpc.dwTemplateId;
	if not self.tbNpcId2Data[nNpcTemplateId] then
		return 0;
	end

	local tbPlayers = nil;
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId > 0 then
		tbPlayers = KTeam.GetTeamMemberList(nTeamId);
	else
		tbPlayers = {pPlayer.dwId}
	end

	for _, _Id in ipairs(tbPlayers) do
		local pMember = KGameBase.GetPlayerById(_Id);
		local nRes, tbData = self:_Check(pMember, nNpcTemplateId, self.tbNpcId2Data);
		if nRes == 1 then
			local nRand = KUnify.MathRandom(1, 100);
			if nRand <= tbData.nDropRate then
				self:_TryGiveCard(pMember, tbData);
			end
		end
	end
end

-- 过房间获得卡片
-- 如果得到卡片的话就作相应记录并返回1
-- 得不到卡片就返回0
function XoyoChallenge:PassRoomForCard(pPlayer, nRoomId, nCloseLock)
	local nRes, tbData = self:_Check(pPlayer, nRoomId, self.tbRoom2Data);
	if nRes == 0 then
		return 0;
	end
	
	if self.tbRoom2Data[nRoomId].nCloseLock ~= nCloseLock then
		return 0;
	end
	
	local nRand = KUnify.MathRandom(1, 100);
	if nRand <= tbData.nDropRate then
		self:_TryGiveCard(pPlayer, tbData);
	end
end

-- 找背包和储物箱看看有没有这个东西
function XoyoChallenge:HasItem(pPlayer, tbGDPL)
	local tbItems = Task:GetPlayerItemList(pPlayer, tbGDPL);
	if tbItems == nil or #tbItems == 0 then
		return 0
	end
	return 1
end

-- 获取玩家逍遥录状态
-- 那里都没有的话返回0, szMsg
-- 有的话返回1
function XoyoChallenge:GetXoyoluState(pPlayer, nCurrYearMonth)
	if not nCurrYearMonth then
		nCurrYearMonth = tonumber(GetLocalDate("%Y%m"));
	end

	local nGetXoyoluMonth = pPlayer.pTask.GetTask(self.TASKGID, self.TASK_GET_XOYOLU_MONTH);

	if nGetXoyoluMonth < nCurrYearMonth then
		return 0, Lang.mission.str852[Lang.Idx];
	end

	return 1;
end

-- 可否获取逍遥录
-- return 1 or 0, szMsg
function XoyoChallenge:CanGetXoyolu(pPlayer)
	--[[
	if TimeFrame:GetState("OpenXoyoGameTask") ~= 1 then
		return 0;
	end

	local nCurrYearMonth, nPrevMonth = XoyoChallenge:__GetYearMonth();
	local nPrevGetMonth = pPlayer.GetTask(self.TASKGID, self.TASK_GET_XOYOLU_MONTH);
	local nGetAwardMonth = pPlayer.GetTask(self.TASKGID, self.TASK_GET_AWARD_MONTH);

	if nCurrYearMonth == 200907 and Task.IVER_nXoyo_GetAward_Fix == 1 then
		local nGetAwardCopy = pPlayer.GetTask(self.TASKGID, self.TASK_GET_AWARD_MONTH_COPY);
		if nPrevGetMonth == nPrevMonth and nGetAwardCopy == 0 then
			return 0, "你上个月的奖励还没领取，先把奖励领了再来拿逍遥录吧！";
		end
	else
		if nPrevGetMonth == nPrevMonth and nGetAwardMonth < nPrevMonth then
			return 0, "你上个月的奖励还没领取，先把奖励领了再来拿逍遥录吧！";
		end
	end

	if nPrevGetMonth >= nCurrYearMonth then
		return 0, "这个月已经给过你一本了，本小姐记性可是好得很，别想忽悠我。";
	end
	--]]
	if self:HasXoyoluInBags(pPlayer) == 1 then
		return 0, Lang.mission.str853[Lang.Idx];
	end

	if pPlayer.pItem.CountFreeBagCell() < 1 then
		return 0, Lang.mission.str854[Lang.Idx];
	end

	return 1;
end

-- 清除卡片记录
function XoyoChallenge:ClearCardRecord(pPlayer)
	for i = self.TASK_NPC_BEGIN, self.TASK_END - 1 do
		pPlayer.pTask.DirectSetTask(self.TASKGID, i, 0);
	end
end

-- 获取逍遥录
-- return 1 or 0
function XoyoChallenge:GetXoyolu(pPlayer)
	local nRes, szMsg = self:CanGetXoyolu(pPlayer);
	if nRes == 0 then
		return 0, szMsg;
	end

	local nYearMonth = tonumber(GetLocalDate("%Y%m"));
	local pItem = KItem.AddPlayerItem(pPlayer, unpack(self.tbXoyolu));

	if not pItem then
		return 0;
	end

	self:ClearCardRecord(pPlayer);
	pPlayer.pTask.DirectSetTask(self.TASKGID, self.TASK_GET_XOYOLU_MONTH, nYearMonth);
	return 1;
end

-- 背包里有没有逍遥录
function XoyoChallenge:HasXoyoluInBags(pPlayer)
	local tbFind = Task:GetPlayerItemList(pPlayer, self.tbXoyolu);
	if tbFind == nil or #tbFind == 0 then
		return 0
	end
	return 1, tbFind[1];
end

function XoyoChallenge:_Check(pPlayer, key, tb)
	local tbData = tb[key];
	if not tbData then
		return 0;
	end

	if self:HasItem(pPlayer, tbData.tbCardGDPL) == 1 then -- 身上已有卡片
		return 0;
	end

	--[[
	local nCurrYearMonth = tonumber(os.date("%Y%m", GetTime()));

	if self:GetXoyoluState(pPlayer, nCurrYearMonth) == 0 then -- 没有逍遥录
		return 0;
	end
	--]]

	local nCardState = self:GetCardState(pPlayer, tbData.szCardGDPL);
	if nCardState == 2 then -- 已经放入逍遥录（已使用）
		return 0;
	end

	if pPlayer.pItem.CountFreeBagCell() < 1 then
		return 0;
	end

	return 1, tbData;
end

function XoyoChallenge:_TryGiveCard(pPlayer, tbData)
	local pItem = KItem.AddPlayerItem(pPlayer, unpack(tbData.tbCardGDPL));

	if not pItem then
		return 0;
	end

	--[[
	if self.tbCardStorage[tbData.szCardGDPL].nWeight >= 10 then
		pPlayer.SendMsgToFriend("您的好友["..pPlayer.szName.."]在逍遥录收集任务中获得一张" .. pItem.szName .. "。");
	end
	--]]

	return 1;
end

-- 使用卡片
-- return 1, or 0, szMsg
function XoyoChallenge:UseCard(pPlayer, pCard)

	if self:HasXoyoluInBags(pPlayer) == 0 then
		return 0, Lang.mission.str855[Lang.Idx];
	end

	if self:GetCardState(pPlayer, pCard.GetGDPLStr()) == 2 then
		return 0, Lang.mission.str856[Lang.Idx];
	end

	self:SetCardState(pPlayer, pCard.GetGDPLStr(), 2);
	KItem.DelPlayerItem(pPlayer, pCard, 1, Player.emKLOSEITEM_USE);
	--[[
	SpecialEvent.ActiveGift:AddCounts(pPlayer, 20);		--收集逍遥谷卡片活跃度
	local nPrevPoint = GetXoyoPointsByName(pPlayer.szName); -- 这个月的点数
	local nCurrPoint = self:GetTotalPoint(pPlayer);
	if nCurrPoint > nPrevPoint then
		PlayerHonor:SetPlayerXoyoPointsByName(pPlayer.szName, nCurrPoint);
	end
	--]]
	return 1;
end

XoyoChallenge:LoadFile();
XoyoChallenge:InitCardStorage();