
-------------------------------------------------------------------------
-- 关于进度条目标
function Task:GetProgressData()
	local tbPlayerData				= me.GetTempTable("Task");
	local tbPlayerProgressTagData	= tbPlayerData.tbProgressTagData;
	if (not tbPlayerProgressTagData) then
		tbPlayerProgressTagData	= {};
		tbPlayerData.tbProgressTagData	= tbPlayerProgressTagData;
	end;
	
	return tbPlayerProgressTagData;
end


function Task:SetProgressTag(tbTag, pPlayer)
	local oldPlayer = me;
	me = pPlayer;
	local tbPlayerProgressTagData = self:GetProgressData();
	tbPlayerProgressTagData.tbTag = tbTag;
	me = oldPlayer;
end;

function Task:OnProgressTagFinish()
	local tbPlayerProgressTagData = self:GetProgressData();
	tbPlayerProgressTagData.tbTag.OnProgressFull(tbPlayerProgressTagData.tbTag);
	tbPlayerProgressTagData.tbTag = nil;
end


-------------------------------------------------------------------------
-- 根据地图ID获得地图名字
function Task:GetMapName(nMapId)
	if (not nMapId or nMapId <= 0) then
		return "";
	end
	return KScene.GetNameByTemplateId(nMapId);
end


------------ 以下函数可能不应该属于任务系统，暂时放在这里

-- 取得物品拥有数量
function Task:GetItemCount(pPlayer, tbItemId, nRoom)
	if (not nRoom) then
		local tbItemList;
		if (not tbItemId[5] or tbItemId[5] < 0) then
			tbItemList = self:GetPlayerItemList(pPlayer, {tbItemId[1], tbItemId[2], tbItemId[3], tbItemId[4]});
		else
			tbItemList = self:GetPlayerItemList(pPlayer, {tbItemId[1], tbItemId[2], tbItemId[3], tbItemId[4], tbItemId[5]});
		end
		local nCount = 0;
		for i = 1, #tbItemList do
			nCount = nCount + tbItemList[i].nCount;
		end
		return nCount;
	else
		return pPlayer.pItem.GetItemCount(nRoom, {tbItemId[1], tbItemId[2], tbItemId[3], tbItemId[4]});
	end
end;


-- 删除物品
function Task:DelItem(pPlayer, tbItemId, nDelCount)
--	assert(tbItemId[1] == 20);
	if (not nDelCount) then
		nDelCount	= 1;
	end;
	
	assert(type(nDelCount) == "number");

	local tbItemList = self:GetPlayerItemList(pPlayer, tbItemId);
	local i = 1;
	while (nDelCount >= 1 ) do
		if (not tbItemList[i]) then
			return;
		end
		local nItemCount = tbItemList[i].nCount;
		if (nItemCount <= 0) then
			return 0;
		end
		if (nItemCount > nDelCount) then
			tbItemList[i].SetCount(nItemCount - nDelCount, Item.emITEM_DATARECORD_REMOVE);
			break;
		else
			nDelCount = nDelCount - nItemCount;
			tbItemList[i].Delete(pPlayer);
		end
		i = i + 1;
	end
	
	return 1;
end;

-- 获得玩家指定物品列表
function Task:GetPlayerItemList(pPlayer, tbItemId)
	local tbItemList = {};
	local tbNeedSearchRoom = {
			Item.ROOM_EQUIP,
			Item.ROOM_EQUIPEX,
			Item.ROOM_MAINBAG,		-- 主背包
			Item.ROOM_REPOSITORY,	-- 贮物箱
			Item.ROOM_EXTBAG1,		-- 扩展背包1
			Item.ROOM_EXTREP1,		-- 扩展贮物箱1
			Item.ROOM_EXTREP2,		-- 扩展贮物箱2
			Item.ROOM_EXTREP3,		-- 扩展贮物箱3
			Item.ROOM_EXTREP4,		-- 扩展贮物箱4
			Item.ROOM_EXTREP5,		-- 扩展贮物箱5
		};
	for _,room in pairs(tbNeedSearchRoom) do
		local tbRoomItemList = pPlayer.pItem.FindItem(room, unpack(tbItemId));
		for _, item in ipairs(tbRoomItemList) do
			tbItemList[#tbItemList + 1] = item.pItem;
		end
	end	
	
	return tbItemList;
end

function Task:IsSameItem(tbItem1, tbItem2)
	if (tbItem1[1] ~= tbItem2[1] or 
		tbItem1[2] ~= tbItem2[2] or 
		tbItem1[3] ~= tbItem2[3] or 
		tbItem1[4] ~= tbItem2[4] or 
		tbItem1[5] ~= tbItem2[5]) then
		
		return 0;
	end
	
	return 1;
end

-- 解析字符串<npc=xxx><playername>
function Task:ParseTag(szMsg)
	local nCurIdx = 1;
	while true do
		local nNpcTagStart, nNpcIdStart	= string.find(szMsg, "<npc=");
		local nNpcTagEnd, nNpcIdEnd			= string.find(szMsg, ">", nNpcIdStart);
		if (not nNpcIdStart or not nNpcIdEnd) then
			break;
		end
		local nNpcTempId 		= tonumber(string.sub(szMsg, nNpcIdStart+1, nNpcIdEnd-1));
		
		if (nNpcTempId) then
			local szNpcName = KGameBase.GetNameByTemplateId(nNpcTempId);
			szMsg = Lib:ReplaceStrFormIndex(szMsg, nNpcTagStart, nNpcTagEnd, szNpcName);
		end
		nCurIdx = nNpcTagStart + 1; --不能是nNpcIdEnd + 1,因为字符串被替换了 
	end
	
	szMsg = Lib:ReplaceStr(szMsg, "<playername>", "<color=yellow><playername>"..me.szName.."<color>");
	
	return szMsg;
end



-------------------------------------------------------------------------
--新人直接得到新手任务
function Task:OnAskBeginnerTask()
	local bFresh = me.pTask.GetTask(Task.nFirstTaskValueGroup, Task.nFirstTaskValueId);
	if (bFresh ~= 1) then
		me.pTask.SetTask(Task.nFirstTaskValueGroup, Task.nFirstTaskValueId, 1, 1);
		local tbTaskData	= Task.tbTaskDatas[Task.nFirstTaskId];
		
		if (tbTaskData) then
			local nReferId 		= tbTaskData.tbReferIds[1];
			local nSubTaskId	= Task.tbReferDatas[nReferId].nSubTaskId;
			local tbSubData		= Task.tbSubDatas[nSubTaskId];
			
			local szMsg = "";
			if (tbSubData.tbAttribute.tbDialog.Start.szMsg) then -- 未分步骤
					szMsg = tbSubData.tbAttribute.tbDialog.Start.szMsg;
			else
					szMsg = tbSubData.tbAttribute.tbDialog.Start.tbSetpMsg[1];
			end

			TaskAct:TalkInDark(szMsg, Task.AskAccept, Task, Task.nFirstTaskId, nReferId);
		else
			print("新手任务不存在!")
		end
	end
end


-------------------------------------------------------------------------
-- 判断两个玩家是否是近距离
function Task:AtNearDistance(pPlayer1, pPlayer2)
	return pPlayer1.IsNearBy(pPlayer2.dwId); -- 参数2不填默认1Region
	--[[	
	local nMapId1, nPosX1, nPosY1, nPosZ1 = pPlayer1.GetWorldPos();
	local nMapId2, nPosX2, nPosY2, nPosZ2 = pPlayer2.GetWorldPos();
	
	if (nMapId1 == nMapId2) then
		local nMyR	= ((nPosX1-nPosX2)^2 + (nPosY1-nPosY2)^2)^0.5;
		if (nMyR < self.nNearDistance) then
			return 1;
		end;
	end;
	]]--
end

-------------------------------------------------------------------------

function Task:AddItems(pPlayer, tbItemId, nCount)
	if (nCount <= 0) then
		return;
	end
	
	for i = 1, nCount do
		Task:AddItem(pPlayer, tbItemId);
	end
end

-- 加物品
function Task:AddItem(pPlayer, tbItemId, nTaskId)
	--[[
	local tbItemInfo = {};
	tbItemInfo.nEnhTimes	= 0;
	tbItemInfo.tbGenInfo	= nil;
	tbItemInfo.tbRandomInfo	= nil;
	tbItemInfo.uRandSeed	= 0;
	tbItemInfo.bForceBind	= self:IsNeedBind(tbItemId);
	--]]
	
	local nWay = Player.emKITEMLOG_TYPE_FINISHTASK;
	
	local pItem = pPlayer.AddItemEx(tbItemId[1], tbItemId[2], tbItemId[3], tbItemId[4], nWay);
	
	if (not pItem) then
		print("添加物品失败", "Name："..pPlayer.szName.."..\n", unpack(tbItemId))		
		return;
	end

	nTaskId = nTaskId or Item.TASKID_INVALID;
	--Item:CheckXJRecord(Item.emITEM_XJRECORD_TASK, nTaskId, pItem);
	
	return pItem;
end;

function Task:AddObjAtPos(pPlayer, nDoodadId, tbItemId, nSceneId, nPosX, nPosY, nPosZ)
	local pDoodad = KGameBase.CallDoodad(nDoodadId, nSceneId, nPosX, nPosY, nPosZ, tbItemId[1], tbItemId[2], tbItemId[3])
	pDoodad.SetOwnerId(me.dwId);
end

function Task:IsNpcExist(dwNpcId, tb)
	if (not dwNpcId) then
		return 0;
	end
	
	local pNpc = KGameBase.GetNpcById(dwNpcId);
	if (not pNpc) then
		return 0;
	end
	
	return 1;
end

-------------------------------------------------------------------------
-- 以下函数为临时添加用于测试
-------------------------------------------------------------------------
function Task:SetStep(nTaskId, nStep)
	if (type(nTaskId) == "string") then
		nTaskId = tonumber(nTaskId, 16);
	end
	
	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTask	= tbPlayerTask.tbTasks[nTaskId];
	if (not tbTask) then
		return nil;
	end;
	tbTask:CloseCurStep("finish");
	tbTask:SetCurStep(nStep);
end


-------------------------------------------------------------------------
-- 获取目前正在进行的任务数目
function Task:GetMyRunTaskCount()
	local tbPlayerTask = self:GetPlayerTask(me);
	return tbPlayerTask.nCount;
end


-------------------------------------------------------------------------
-- 获取当前可接任务，不包括物品触发任务
function Task:GetCanAcceptTaskCount()
	local nCanAcceptCount = 0;
	local tbPlayerTasks	= self:GetPlayerTask(me).tbTasks;
	for _, tbTaskData in pairs(self.tbTaskDatas) do
		if (not tbPlayerTasks[tbTaskData.nId]) then
			local nReferIdx		= self:GetFinishedIdx(tbTaskData.nId) + 1;			-- +1表示将要继续的任务
			local nReferId		= tbTaskData.tbReferIds[nReferIdx];
			if (nReferId) then				
				local tbReferData	= self.tbReferDatas[nReferId];
				local tbAccept	= tbReferData.tbAccept;
				local tbVisable = tbReferData.tbVisable
				local nTaskType = tbTaskData.tbAttribute.TaskType;
				if (Lib:DoTestFuncs(tbVisable) and Lib:DoTestFuncs(tbAccept) and  (nTaskType == 1 or nTaskType == 2)) then	-- 满足可见和可接条件
					nCanAcceptCount = nCanAcceptCount + 1;	
				end
			end
		end
	end
	
	return nCanAcceptCount;
end


-- 获得当前可接的最小等级的引用子任务数据
function Task:GetMinCanAcceptRefDataList(pPlayer, nTaskType)
	if (not nTaskType) then
		nTaskType = Task.emType_Main;
	end
	
	return self:GetCanAcceptRefDataList(pPlayer, 1, 2);
end

-- 0 All
-- 1 Max
-- 2 Min
function Task:GetCanAcceptRefDataList(pPlayer, nTaskType, nLevelType)
	if (not nTaskType) then
		nTaskType = Task.emType_Main;
	end
	
	local tbRefSubDataList = {};
	local nLevel = nil;
	local tbPlayerTasks	= self:GetPlayerTask(pPlayer).tbTasks;
	for _, tbTaskData in pairs(self.tbTaskDatas) do
		if (not tbPlayerTasks[tbTaskData.nId]) then
			local nReferIdx		= self:GetFinishedIdx(tbTaskData.nId) + 1;			-- +1表示将要继续的任务
			local nReferId		= tbTaskData.tbReferIds[nReferIdx];
			if (nReferId) then				
				local tbReferData	= self.tbReferDatas[nReferId];
				if (tbReferData.nAcceptNpcId > 0) then
					local tbAccept	= tbReferData.tbAccept;
					local tbVisable = tbReferData.tbVisable;
					
					if (Lib:DoTestFuncs(tbVisable) and Lib:DoTestFuncs(tbAccept) and tbTaskData.tbAttribute.TaskType == nTaskType) then	-- 满足可见和可接条件						
						if (nLevelType == 1 and (not nLevel or tbReferData.nLevel > nLevel)) then
							tbRefSubDataList = {};
							tbRefSubDataList[#tbRefSubDataList + 1] = tbReferData;
							nLevel = tbReferData.nLevel;						
						elseif (nLevelType == 2 and (not nLevel or tbReferData.nLevel < nLevel)) then
							tbRefSubDataList = {};
							tbRefSubDataList[#tbRefSubDataList + 1] = tbReferData;
							nLevel = tbReferData.nLevel;
						elseif (not nLevel or tbReferData.nLevel == nLevel) then
							tbRefSubDataList[#tbRefSubDataList + 1] = tbReferData;							
						end
					end
				end
			end
		end
	end
	
	return tbRefSubDataList;
end


function Task:GetMinAcceptRefData(pPlayer)
	local nMinLevel = 1000;
	local tbRefSubData = nil;
	local tbPlayerTasks = self:GetPlayerTask(pPlayer).tbTasks;
	for _, tbTask in pairs(tbPlayerTasks) do
		local tbReferData = self.tbReferDatas[tbTask.nReferId]
		local tbTaskData = tbTask.tbTaskData;
		if (tbTaskData.tbAttribute.TaskType == 1) then
			if (tbReferData.nLevel < nMinLevel) then
				nMinLevel = tbReferData.nLevel;
				tbRefSubData = tbReferData;
			end
		end
	end
	
	return tbRefSubData;
end

-- 获得等级段描述
-- 先找已接任务最低等级段的任务描述，再找可接任务最低等级段描述
function Task:GetLevelRangeDesc(pPlayer)
	local nLevel = pPlayer.nLevel;
	local tbAcceptRefSubData = self:GetMinAcceptRefData(pPlayer);
	local tbRefSubData = self:GetMinCanAcceptRefDataList(pPlayer);
	
	if (tbAcceptRefSubData) then
		nLevel = tbAcceptRefSubData.nLevel;
	elseif (tbRefSubData and tbRefSubData[1]) then
		nLevel = tbRefSubData[1].nLevel;
	end
	
	for _, item in ipairs(self.tbLevelRangeInfo) do
		if (item.level_range_max >= nLevel) then
			return item.level_range_desc;
		end
	end
	
	return "";
end



-- 获得当前可接的最小等级主线任务指引描述
function Task:GetMinLevelMainTaskInfo(pPlayer)
	local tbRet = {};
	local tbRefSubDataList = self:GetMinCanAcceptRefDataList(pPlayer);
	if (tbRefSubDataList) then
		for _, tbRefSubData in ipairs(tbRefSubDataList) do
			tbRet[#tbRet + 1] = {tbRefSubData.nLevel, tbRefSubData.szName, tbRefSubData.szIntrDesc};
		end
	end
	
	return tbRet;
end

function Task:GetMaxLevelCampTaskInfo(pPlayer)
	local tbRet = {};
	local tbRefSubDataList = self:GetCanAcceptRefDataList(me, Task.emType_Camp, 1);
	if (tbRefSubDataList) then
		for _, tbRefSubData in ipairs(tbRefSubDataList) do
			tbRet[#tbRet + 1] = {tbRefSubData.nLevel, tbRefSubData.szName, tbRefSubData.szIntrDesc};
		end
	end
	
	return tbRet;	
end

-- 获得当前所有可接的支线任务列表
--
--{
--		{szName, szDesc = ""},
--		{szName, szDesc = ""},
--}
function Task:GetBranchTaskTable(pPlayer)
	local tbRet = {};
	local tbPlayerTasks	= self:GetPlayerTask(pPlayer).tbTasks;
	for _, tbTaskData in pairs(self.tbTaskDatas) do
		if (not tbPlayerTasks[tbTaskData.nId]) then
			local nReferIdx		= self:GetFinishedIdx(tbTaskData.nId) + 1;			-- +1表示将要继续的任务
			local nReferId		= tbTaskData.tbReferIds[nReferIdx];
			if (nReferId) then				
				local tbReferData	= self.tbReferDatas[nReferId];
				if (tbReferData.nAcceptNpcId > 0) then
				local tbAccept	= tbReferData.tbAccept;
				local tbVisable = tbReferData.tbVisable
				if (Lib:DoTestFuncs(tbVisable) and Lib:DoTestFuncs(tbAccept) and tbTaskData.tbAttribute.TaskType == 2 and (not tbTaskData.tbAttribute["Repeat"])) then	-- 满足可见和可接条件
					tbRet[#tbRet + 1] = {tbReferData.nLevel, tbReferData.szName, tbReferData.szIntrDesc}
					end
				end
			end
		end
	end
	
	table.sort(tbRet, self.CompLevel);
	
	return tbRet;
end


function Task.CompLevel(tbTaskA, tbTaskB)
	if (tbTaskA and tbTaskB) then
		return tbTaskA[1] < tbTaskB[1];
	end
end


-- 获得难度描述
function Task:GetRefSubTaskDegreeDesc(nRefSubId)
	if (not self.tbReferDatas[nRefSubId]) then
		return "";
	end
	
	local nDegree = self.tbReferDatas[nRefSubId].nDegree or 1;
	if (nDegree <= 1) then
		return "";
	elseif (nDegree == 2) then
		return "<color=Yellow>"..Lang.task.str100[Lang.Idx].."<color=White>";
	elseif (nDegree == 3) then
		return "<color=Yellow>"..Lang.task.str101[Lang.Idx].."<color=White>";
	elseif (nDegree == 4) then
		return"<color=Yellow>".. Lang.task.str102[Lang.Idx].."<color=White>";
	end
	
	return "";
end


-- 玩家是否做过指定引用子任务，对于重复任务无效
function Task:HaveDoneSubTask(pPlayer, nTaskId, nRefId)
	local tbTaskData = self.tbTaskDatas[nTaskId];
	if (not tbTaskData) then
		return 0;
	end
	local nLastRefId = pPlayer.pTask.GetTask(1000, nTaskId);
	
	if (nLastRefId == 0) then
		return 0;
	end
	if not self.tbReferDatas[nLastRefId] or not self.tbReferDatas[nRefId] then
		return 0;
	end
	if (self.tbReferDatas[nLastRefId].nReferIdx >= self.tbReferDatas[nRefId].nReferIdx) then
		return 1;
	end
	
	return 0;
end

-- 玩家现在身上是否有某个任务
function Task:HaveTask(pPlayer, nTaskId)
	local tbPlayerTasks	= self:GetPlayerTask(pPlayer).tbTasks;
	if (tbPlayerTasks[nTaskId]) then
		return 1;
	end
	
	return 0;
end


-- 显示所有任务，调试用
function Task:ShowAllTasks()
	local function fnCompEarlier(tbTask1, tbTask2)
		return tbTask1.nAcceptDate < tbTask2.nAcceptDate;
	end;

	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTasks	= {};
	for _, tbTask in pairs(tbPlayerTask.tbTasks) do
		tbTasks[#tbTasks+1]	= tbTask;
	end;
	me.SysMsg("My Tasks: ("..tbPlayerTask.nCount..")");
	table.sort(tbTasks, fnCompEarlier);
	for _, tbTask in ipairs(tbTasks) do
		me.SysMsg("  "..tbTask:GetName().." ["..os.date("%y/%m/%d %H:%M:%S", tbTask.nAcceptDate).."] ");
	end;
end


-- 以后补将可能用到，不删先
function Task:ModifTaskItem(pPlayer)
end

function Task:IsNeedBind(tbItem)
	return 1;
end

function Task:OnLoadMapFinish(nMapId, nMapCopy, nParam)
	self.tbArmyCampInstancingManager:OnLoadMapFinish(nMapId, nMapCopy, nParam);
end


function Task:RepairTaskValue()
end

function Task:SetTaskValueWithStepCondition(pPlayer, nTaskId, nReferId, nMinStep, nMaxStep, nGroupId, nRowId, nValue)
	if (not pPlayer or not nTaskId or not nReferId or not nMinStep or not nMaxStep) then
		return;
	end;
	if (not nGroupId or not nRowId or not nValue) then
		return;
	end;
	
	local tbPlayerTasks	= self:GetPlayerTask(pPlayer).tbTasks;
	local tbTask = tbPlayerTasks[nTaskId];
	if (tbTask and tbTask.nReferId == nReferId and tbTask.nCurStep >= nMinStep and tbTask.nCurStep <= nMaxStep) then
		if (nValue ~= pPlayer.pTask.GetTask(nGroupId, nRowId)) then
			Dbg:WriteLog("Task", "Player TaskValue Error!", nGroupId, nRowId);
			pPlayer.pTask.SetTask(nGroupId, nRowId, nValue, 1);
		end
	end;
end

function Task:DailyEvent()
end;

--PlayerSchemeEvent:RegisterGlobalDailyEvent({Task.DailyEvent, Task}); -- todo simon

