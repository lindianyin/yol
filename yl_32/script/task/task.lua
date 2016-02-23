
-------------------------------------------------------------------------
 -- 目标库
if (not Task.tbTargetLib) then
	Task.tbTargetLib	= {};
end;

-------------------------------------------------------------------------------
-- 获取任务目标，主要用于目标定义
function Task:GetTarget(szTarget)
	local tbTarget	= self.tbTargetLib[szTarget];
	if (not tbTarget) then
		tbTarget	= {};
		self.tbTargetLib[szTarget]	= tbTarget;
	end;
	return tbTarget;
end;


-------------------------------------------------------------------------
-- 任务系统初始化，载入200个任务文件和600个子任务文件
function Task:OnInit()
	local nTaskCount	= 0;

	self:LoadText();
	for i = 1, 600 do
		if (self:LoadTask2(i)) then
			nTaskCount	= nTaskCount + 1;
		end
	end

	print(string.format("Task System Inited! %d task(s) loaded!", nTaskCount));

	self:LoadLevelRangeInfo();
	self:LoadTaskTypeFile();
	--LinkTask:InitFile();	--包万同任务
	--TreasureMap:OnInit();	--藏宝图任务
	--Merchant:InitFile();	--商会任务
	--Wanted:InitFile();		--官府通缉任务
end

-------------------------------------------------------------------------
-- 玩家上线，先载入这个玩家的任务数据
function Task:OnLogin(dwPlayerId)
	me = KGameBase.GetPlayerById(dwPlayerId);
	--Dialog:SetTrackTask(me, false);
	self:LoadData();
	self.bDataLoaded = 1;
	me.pTask.SyncTaskGroup(self.TASK_FINISH_RECORD); -- 同步已完成的任务,组1000,nGroupId = 1000, nTaskValueId = nTaskId, nTaskValue = nLastRefId
	--Dialog:SetTrackTask(me, true);

	self:AutoAcceptStoryTask(); -- 每次登陆，判断当前是否有主线任务，没有则强接一个
end

function Task:c2s_AutoAcceptStoryTask()
  self:AutoAcceptStoryTask() -- 接取可以接的主线
end

--- 自动强接主线任务 kk
function Task:AutoAcceptStoryTask()
	local bHasStoryTask = 0;  -- 有否在执行的主线任务
	local tbPlayerTask = self:GetPlayerTask(me);  -- 玩家所有在执行任务
	for _, tbTask in pairs(tbPlayerTask.tbTasks) do
		local tbTaskData = self.tbTaskDatas[tbTask.nTaskId]; -- 获取读表
		local nTaskTypeId = tbTaskData.tbAttribute.TypeID; -- 任务类型 0是主线
		if nTaskTypeId == self.emTskProType_Main then -- 主线任务  -- 判断是否主线任务
			bHasStoryTask = 1; -- 标记有主线任务
			break
		end
	end
	if bHasStoryTask == 0 then  -- 无主线任务，从表里取一个，强接
		for _, tbReferData in pairs(self.tbReferDatas) do --
			local tbTaskData = self.tbTaskDatas[tbReferData.nTaskId]; -- tbTaskData
			if tbTaskData.tbAttribute.TypeID == self.emTskProType_Main then
				-- 先判断，之前有没坐过
				local nHasFinished = me.pTask.GetTask(self.TASK_FINISH_RECORD, tbReferData.nTaskId);
				if nHasFinished <= 0 then  -- 要确保未做过这个主线任务
					-- 主线，强接
					local bOK, szMsg = Lib:DoTestFuncs(tbReferData.tbAccept);  -- ReferTaskData  可接条件
					if bOK == 1 then
						Task:DoAccept(tbReferData.nTaskId, tbReferData.nTaskId);
						print("自动接收主线任务: " .. tbReferData.nTaskId);
						break;
					end
				end
			end
		end
	end
end

-------------------------------------------------------------------------
-- 玩家下线，保存任务数据
function Task:OnLogout(dwPlayerId)
 	me = KGameBase.GetPlayerById(dwPlayerId);
	if (MODULE_GAMESERVER) then
		local tbPlayerTask	= self:GetPlayerTask(me);
		for _, tbTask in pairs(tbPlayerTask.tbTasks) do
			tbTask:SaveData();
			tbTask:CloseCurStep("logout");
		end

		-- self.bDataLoaded = 2;
		print(string.format("Player[%s] %d task(s) saved.", me.szName, tbPlayerTask.nCount));
	end
end



-------------------------------------------------------------------------
-- 根据玩家任务变量，载入已接任务，每个任务变量组前面4个都有特殊意义
function Task:LoadData()
	local nCount = 0;
	for nSaveGroup = self.TASK_GROUP_MIN, self.TASK_GROUP_MAX do
		local nReferId	= me.pTask.GetTask(nSaveGroup, self.emSAVEID_REFID);
		if (nReferId ~= 0) then
			local tbTask	= self:NewTask(nReferId);
			if (tbTask) then
				tbTask:LoadData(nSaveGroup);-- 载入这个任务的数据，比如当前目标和步骤
				nCount	= nCount + 1;
			end

		end;
	end;

	print(string.format("Player[%s] %d task(s) loaded.", me.szName, nCount));
end;




-------------------------------------------------------------------------
-- [S]通知客户端弹出接受任务对话框
function Task:AskAccept(nTaskId, nReferId, pSharedPlayer)
	print("Task:AskAccept", nTaskId, nReferId, pSharedPlayer)
	local nSharedPlayerId = -1;

	if (self.tbTaskDatas[nTaskId] and self.tbTaskDatas[nTaskId].tbAttribute["Repeat"]) then
		if (self:CanAcceptRepeatTask() ~= 1) then
			if (pSharedPlayer) then
				pSharedPlayer.Msg(me.szName..Lang.task.str56[Lang.Idx]);
			end

			return;
		end
	end

	if (pSharedPlayer) then
		nSharedPlayerId = pSharedPlayer.nId;
	end

	self:GetPlayerTask(me).tbAskAccept	= { -- 防止客户端作弊(没接任务也发送接任务请求)
		nTaskId			= nTaskId,
		nReferId		= nReferId,
		nAskDate		= KGameBase.GetTime(),
		nSharedPlayerId	= nSharedPlayerId;
	};
	print("Task:AskAccept", nTaskId, nReferId, pSharedPlayer)
	--KTask.SendAccept(me, nTaskId, nReferId , 1);
	self:DoAccept(nTaskId,nReferId,1)
	return 1;
end;

-------------------------------------------------------------------------
-- 接到客户端确认接受
function Task:OnAccept(nTaskId, nReferId, bAccept)
	print("Task:OnAccept", nTaskId, nReferId, bAccept)

	--防沉迷, 不健康时间不能领取任务
--	if (me.GetTiredDegree() == 2) then
--		return 0;
--	end
	local taskType = Task:GetTaskType(nTaskId);
	if taskType == "Escort" then
		for _, tbTask in pairs(self:GetPlayerTask(me).tbTasks) do
			local ownerTaskType = Task:GetTaskType(tbTask.nTaskId);
			if ownerTaskType == "Escort" then	-- 如果已经接到的任务有修仙任务
			    me.SysMsg(Lang.task.str57[Lang.Idx]);
				return nil;
			end
		end
		local tbTaskStartIdAndType = Escort.tbTaskId[nTaskId];
		me.pTask.SetTask(Escort.ESCORT_GROUP_ID, tbTaskStartIdAndType[1], tbTaskStartIdAndType[2]);
	end

	if (self.tbTaskDatas[nTaskId] and
		not self.tbTaskDatas[nTaskId].tbAttribute["Repeat"] and
		Task:HaveDoneSubTask(me, nTaskId, nReferId) == 1) then
		me.SysMsg(Lang.task.str58[Lang.Idx]);
		local szMsg = "TaskId: " .. nTaskId .. "ReferId"  .. nReferId;
		Dbg:WriteLog("Task", Lang.task.str59[Lang.Idx], me.szAccount, me.szName, szMsg);
		BlackSky:GiveMeBright(me);
		return;
	end;

	BlackSky:GiveMeBright(me);
	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbAskAccept	= tbPlayerTask.tbAskAccept;
	--[[if (not tbAskAccept or tbAskAccept.nTaskId ~= nTaskId or tbAskAccept.nReferId ~= nReferId) then -- 校验客户端是否作弊(没接任务也发送接任务请求) -- 这个先去掉，客户端有直接接任务的需求
		return;
	end --]]
	local nSharedPlayerId	= 0;

	if tbAskAccept ~= nil then
	    nSharedPlayerId	= tbAskAccept.nSharedPlayerId;
	end

	local pSharedPlayer = KGameBase.GetPlayerById(nSharedPlayerId);

	tbPlayerTask.tbAskAccept	= nil;

	-- 重复任务需要检查可接次数
	if (self.tbTaskDatas[nTaskId].tbAttribute["Repeat"]) then
		if (self:CanAcceptRepeatTask() ~= 1) then
			Dialog:SendInfoBoardMsg(me, Lang.task.str60[Lang.Idx]);
			return;
		end
	end

	if (bAccept == 0) then
		if (pSharedPlayer) then
			pSharedPlayer.Msg(me.szName..Lang.task.str61[Lang.Idx]..self.tbTaskDatas[nTaskId].szName);
		end

		return 1;
	end

	if (pSharedPlayer) then
		pSharedPlayer.Msg(me.szName..Lang.task.str62[Lang.Idx]..self.tbTaskDatas[nTaskId].szName);
	end

	return self:DoAccept(nTaskId, nReferId);
end

function Task:CheckCanAcceptBranchTask_LevelUp(pPlayer, nLevel)
   local tbSubTaskMap =
   {
      [17] = 256,
      [20] = 257,
	  [25] = 258,
	  [26] = 259,
	  [27] = 260,
	  [28] = 261,
	  [30] = {262,263,10,11,12,13,14,15,26,27},
	  [31] = {264,265},
	  [32] = {272,273,28},
	  [33] = {274,275},
	  [34] = {276,277,29},
	  [35] = {278,279,30},
	  [36] = {280,281,31},
	  [37] = {288,289,42},
	  [38] = {290,291,43},
	  [39] = {292,293,44},
	  [40] = {294,295,45},
	  [41] = {296,297,46},
	  [42] = {304,305},
	  [43] = {306,307,47},
	  [44] = {308,309,58},
	  [45] = {310,311,59},
	  [46] = {312,313,60},
	  [47] = {320,321},
	  [48] = {322,323,61},
	  [49] = {324,325,62},
	  [50] = {326,327,63},
	  [51] = {328,329,74},
	  [52] = {336,337,75},
	  [53] = {338,339,76},
	  [54] = {340,341},
	  [55] = {342,343,77},
	  [56] = {344,345,78},
	  [57] = {352,353},
	  [58] = {354,355,79},
	  [59] = {356,357,90},




   }

   if tbSubTaskMap[nLevel] ~= nil then
        me = pPlayer
       if type(tbSubTaskMap[nLevel]) == "table" then
	       for _,taskId in pairs(tbSubTaskMap[nLevel]) do
		       self:OnAccept(taskId, taskId, 1)
		   end
	   else
	       self:OnAccept(tbSubTaskMap[nLevel], tbSubTaskMap[nLevel], 1)
	   end
   end

   self:AutoAcceptStoryTask() -- 接取主线
end

function Task:CheckCanAcceptBranchTask(pPlayer, TaskId)
   local tbSubTaskMap =
   {
      [36] = 256,
      [39] = 257,
	  [50] = 258,
	  [51] = 259,
	  [52] = 260,
	  [53] = 261,
	  [55] ={262,10},
	  [262] =263,
	  [263] =264,
	  [264] =265,
	  [265] =272,
	  [272] =273,
	  [273] =274,
	  [274] =275,
	  [275] =276,
	  [276] =277,
	  [277] =278,
	  [278] =279,
	  [279] =280,
	  [280] =281,
	  [281] =288,
	  [288] =289,
	  [289] =290,
	  [290] =291,
	  [291] =292,
	  [292] =293,
	  [293] =294,
	  [294] =295,
	  [295] =296,
	  [296] =297,
	  [297] =304,
	  [304] =305,
	  [305] =306,
	  [306] =307,
	  [307] =308,
	  [308] =309,
	  [309] =310,
	  [310] =311,
	  [311] =312,
	  [312] =313,
	  [313] =320,
	  [320] =321,
	  [321] =322,
	  [322] =323,
	  [323] =324,
	  [324] =325,
	  [325] =326,
	  [326] =327,
	  [327] =328,
	  [328] =329,
	  [329] =336,
	  [336] =337,
	  [337] =338,
	  [338] =339,
	  [340] =341,
	  [341] =342,
	  [342] =343,
	  [343] =344,
	  [344] =345,
	  [345] =352,
	  [352] =353,
	  [352] =353,
	  [353] =354,
	  [354] =355,
	  [355] =356,
	  [356] =357,
	  [10] = 11,
	  [11] = 12,
	  [12] = 13,
	  [13] = 14,
	  [14] = 15,
	  [15] = 26,
	  [26] = 27,
	  [27] = 28,
	  [28] = 29,
	  [29] = 30,
	  [30] = 31,
	  [31] = 42,
	  [42] = 43,
	  [43] = 44,
	  [44] = 45,
	  [45] = 46,
	  [46] = 47,
	  [47] = 58,
	  [58] = 59,
	  [59] = 60,
	  [60] = 61,
	  [61] = 62,
	  [62] = 63,
	  [63] = 74,
	  [74] = 75,
	  [75] = 76,
	  [76] = 77,
	  [77] = 78,
	  [78] = 79,
	  [79] = 90,


   }

   if tbSubTaskMap[TaskId] ~= nil then
        me = pPlayer
       if type(tbSubTaskMap[TaskId]) == "table" then
	       for _,taskId in pairs(tbSubTaskMap[TaskId]) do
		       self:OnAccept(taskId, taskId, 1)
		   end
	   else
	       self:OnAccept(tbSubTaskMap[TaskId], tbSubTaskMap[TaskId], 1)
	   end
   end
end

function Task:c2s_DoAcceptTask(pPlayer, clearTaskId, acceptTaskId)
    me = pPlayer
    Task:CloseTask(clearTaskId, "Debug")
    self:DoAccept(acceptTaskId,acceptTaskId,1)
    KTask.SendRefresh(me, acceptTaskId, acceptTaskId, 0)
end

-------------------------------------------------------------------------
-- 立即接受任务
function Task:DoAccept(nTaskId, nReferId, bDebugMode)
	print("Task:DoAccept", nTaskId, nReferId)
	if (type(nTaskId) == "string") then
		nTaskId = tonumber(nTaskId, 16);
	end
	if (type(nReferId) == "string") then
		nReferId = tonumber(nReferId, 16);
	end

	if (not nTaskId or not nReferId) then
		assert(false);
		return;
	end

	local tbTaskData	= self.tbTaskDatas[nTaskId];
	local tbReferData	= self.tbReferDatas[nReferId];
	if (not tbReferData) then
		return;
	end

	--if me.GetTiredDegree1() == 2 then
	--	me.SysMsg("您太累了，还是休息下吧！");
	--	return;
	--end

	-- 判断可接条件
	if (tbReferData.tbAccept and not bDebugMode) then
		local bOK, szMsg	= Lib:DoTestFuncs(tbReferData.tbAccept);
		if (not bOK) then
			me.SysMsg(szMsg);
			return nil;
		end;
	end;

	local tbUsedGroup	= {};
	-- 标记已经使用过的Group
	for _, tbTask in pairs(self:GetPlayerTask(me).tbTasks) do
		tbUsedGroup[tbTask.nSaveGroup]	= 1;
	end;
	-- 找出空闲的可以使用的Group
	local nSaveGroup	= nil;
	for n = self.TASK_GROUP_MIN, self.TASK_GROUP_MAX do
		if (not tbUsedGroup[n]) then
			nSaveGroup	= n;
			break;
		end;
	end;
	if (not nSaveGroup) then
		me.SendBlackMsg(Lang.task.str63[Lang.Idx]);
		return nil;
	end;

	-- 若是物品触发，检查玩家身上是否有此物品，有则删除，没有就返回nil
	if (tbReferData.nParticular) then
		local tbItemId = {20,1,tbReferData.nParticular,1};
		if (not self:DelItem(me, tbItemId, 1)) then
			me.SendBlackMsg(Lang.task.str64[Lang.Idx]);
			return nil;
		end
	end

	-- 建立此任务的实例
	local tbTask	= self:NewTask(nReferId);
	if (not tbTask) then
		return nil;
	end;

	--Merchant:DoAccept(tbTask, nTaskId, nReferId);
	-- 重复任务需要设置已接次数
	if (self.tbTaskDatas[nTaskId].tbAttribute["Repeat"] and tbReferData.nSubTaskId < 10000) then
		-- 次数
		local nAcceptCount = me.pTask.GetTask(self.TASK_GROUP_REPEAT, 1);
		assert(nAcceptCount < self.nRepeatTaskAcceptMaxTime);
		me.pTask.SetTask(self.TASK_GROUP_REPEAT, 1, nAcceptCount + 1, 1);
	end

	tbTask.nAcceptDate	= KGameBase.GetTime();
	tbTask.nSaveGroup	= nSaveGroup;
	me.SysMsg(Lang.task.str65[Lang.Idx]..tbTask:GetName());

	me.pTask.SetTask(tbTask.nSaveGroup, self.emSAVEID_NEWFLAG, 1, 1);  -- 设置新任务标记，使客户端激活新任务事件

	--me.SysMsg("    "..tbTask.tbReferData.szMainDesc);
	tbTask:SetCurStep(1);
	--me.CastSkill(self.nAcceptTaskSkillId,1,-1, me.GetNpc().nIndex);

	local tbStartExecute = tbTask.tbSubData.tbStartExecute;
	if (tbStartExecute and #tbStartExecute > 0) then
		Lib:DoExecFuncs(tbStartExecute);
	end;

	self:LogAcceptTask(nTaskId);
	me.pTask.SetTask(tbTask.nSaveGroup, self.emSAVEID_NEWFLAG, 0, 1);  -- 恢复任务标记

	--写Log
--	if tbTask.tbTaskData.tbAttribute.TaskType == 1 then
--		local szTaskName = self:GetTaskName(tbTask.nTaskId);
--		local szSubTaskName = self:GetManSubName(tbTask.nReferId);
--		KStatLog.ModifyField("roleinfo", me.szName, Lang.task.str66[Lang.Idx]..szTaskName, szSubTaskName);
--	end
	return tbTask;
end;

--GM使用指令直接接取任务，不经过有没有做过的判断
function Task:GMDoAccept(nTaskId)
	local tbReferData	= self.tbReferDatas[nTaskId];
	local tbUsedGroup	= {};
		-- 标记已经使用过的Group
		for _, tbTask in pairs(self:GetPlayerTask(me).tbTasks) do
			tbUsedGroup[tbTask.nSaveGroup]	= 1;
		end;
		-- 找出空闲的可以使用的Group
		local nSaveGroup	= nil;
		for n = self.TASK_GROUP_MIN, self.TASK_GROUP_MAX do
			if (not tbUsedGroup[n]) then
				nSaveGroup	= n;
				break;
			end;
		end;
		if (not nSaveGroup) then
			me.SendBlackMsg(Lang.task.str63[Lang.Idx]);
			return nil;
		end;

		-- 若是物品触发，检查玩家身上是否有此物品，有则删除，没有就返回nil
		if (tbReferData.nParticular) then
			local tbItemId = {20,1,tbReferData.nParticular,1};
			if (not self:DelItem(me, tbItemId, 1)) then
				me.SendBlackMsg(Lang.task.str64[Lang.Idx]);
				return nil;
			end
		end

		-- 建立此任务的实例
		local tbTask	= self:NewTask(nTaskId);
		if (not tbTask) then
			return nil;
		end;

		--Merchant:DoAccept(tbTask, nTaskId, nReferId);
		-- 重复任务需要设置已接次数
		if (self.tbTaskDatas[nTaskId].tbAttribute["Repeat"] and tbReferData.nSubTaskId < 10000) then
			-- 次数
			local nAcceptCount = me.pTask.GetTask(self.TASK_GROUP_REPEAT, 1);
			assert(nAcceptCount < self.nRepeatTaskAcceptMaxTime);
			me.pTask.SetTask(self.TASK_GROUP_REPEAT, 1, nAcceptCount + 1, 1);
		end

		tbTask.nAcceptDate	= KGameBase.GetTime();
		tbTask.nSaveGroup	= nSaveGroup;
		me.SysMsg(Lang.task.str65[Lang.Idx]..tbTask:GetName());

		me.pTask.SetTask(tbTask.nSaveGroup, self.emSAVEID_NEWFLAG, 1, 1);  -- 设置新任务标记，使客户端激活新任务事件

		--me.SysMsg("    "..tbTask.tbReferData.szMainDesc);
		tbTask:SetCurStep(1);
		--me.CastSkill(self.nAcceptTaskSkillId,1,-1, me.GetNpc().nIndex);

		local tbStartExecute = tbTask.tbSubData.tbStartExecute;
		if (tbStartExecute and #tbStartExecute > 0) then
			Lib:DoExecFuncs(tbStartExecute);
		end;

		self:LogAcceptTask(nTaskId);
		me.pTask.SetTask(tbTask.nSaveGroup, self.emSAVEID_NEWFLAG, 0, 1);  -- 恢复任务标记
end
--完成nTaskId之前的全部任务
function Task:GMFinishTask(nTaskId)
	for i=1,nTaskId do
			local tbPlayerTask	= self:GetPlayerTask(me);
			local tbTask	= tbPlayerTask.tbTasks[i];
			if (tbTask) then
				tbTask:CloseCurStep("finish");
				tbPlayerTask.tbTasks[nTaskId]	= nil;
				tbPlayerTask.nCount	= tbPlayerTask.nCount - 1;
				me.pTask.ClearTaskGroup(tbTask.nSaveGroup,1);

				KTask.SendRefresh(me, nTaskId, tbTask.nReferId, 0);

				local tbFinishExecute = tbTask.tbSubData.tbFinishExecute;
				if (tbFinishExecute and #tbFinishExecute > 0) then
					Lib:DoExecFuncs(tbFinishExecute);
				end;

				local tbSubExecute = tbTask.tbSubData.tbExecute;
				if (tbSubExecute and #tbSubExecute > 0) then
					Lib:DoExecFuncs(tbSubExecute);
				end;
			end;
	end
end


function Task:LogAcceptTask(nTaskId)
end


function Task:LogFinishTask(nTaskId)
end
-------------------------------------------------------------------------
-- 建立玩家当前任务数据
function Task:NewTask(nReferId)
	local tbReferData	= self.tbReferDatas[nReferId];
	if (not tbReferData) then
		me.SysMsg(Lang.task.str67[Lang.Idx] .. nReferId);
		return nil;
	end

	local nTaskId		= tbReferData.nTaskId;
	local nSubTaskId	= tbReferData.nSubTaskId;
	local tbTaskData	= self.tbTaskDatas[nTaskId];
	local tbSubData		= self.tbSubDatas[nSubTaskId];

	-- 获得玩家的任务
	local tbPlayerTask	= self:GetPlayerTask(me);
	if (tbPlayerTask.tbTasks[nTaskId]) then
		--me.SysMsg(Lang.task.str68[Lang.Idx] .. tbTaskData.szName);
		return nil;
	end;

	local tbTask		= Lib:NewClass(self._tbClassBase);
	tbTask.nTaskId		= nTaskId;
	tbTask.nSubTaskId	= nSubTaskId;
	tbTask.nReferId		= nReferId;
	tbTask.tbTaskData	= tbTaskData;
	tbTask.tbSubData	= tbSubData;
	tbTask.tbReferData	= tbReferData;
	tbTask.tbCurTags	= {};
	tbTask.nAcceptDate	= 0;
	tbTask.nCurStep		= 0;
	tbTask.nSaveGroup	= 0;
	tbTask.me			= me;
	tbTask.tbNpcMenus	= {};

	tbTask.nLogMoney	= 0;

	tbPlayerTask.tbTasks[nTaskId]	= tbTask;
	tbPlayerTask.nCount	= tbPlayerTask.nCount + 1;
	return tbTask;
end;



-------------------------------------------------------------------------
-- 接到客户端放弃
function Task:OnGiveUp(nTaskId, nReferId)
	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTask	= tbPlayerTask.tbTasks[nTaskId];

	if (not tbTask or tbTask.nReferId ~= nReferId) then
		me.SysMsg(Lang.task.str69[Lang.Idx]);
		return;
	end

	if (not tbTask.tbReferData.bCanGiveUp) then
		me.SysMsg(Lang.task.str70[Lang.Idx]);
		return;
	end

	self:CloseTask(nTaskId, "giveup");
end


-------------------------------------------------------------------------
-- [S]接到客户端申请共享
function Task:OnShare(nTaskId, nReferId)
	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTask	= tbPlayerTask.tbTasks[nTaskId];

	if (not tbTask or tbTask.nReferId ~= nReferId) then
		me.SysMsg(Lang.task.str71[Lang.Idx]);
		return;
	end

	if (not tbTask.tbTaskData.tbAttribute["Share"]) then
		me.SysMsg(Lang.task.str72[Lang.Idx]);
		return;
	end

	local tbTeamMembers, nMemberCount	= KTeam.GetTeamMemberList(me.GetTeamId());
	if (not tbTeamMembers) then
		Dialog:SendInfoBoardMsg(me, Lang.task.str73[Lang.Idx]);
		return;
	end
	if (nMemberCount <= 0) then
		Dialog:SendInfoBoardMsg(me, Lang.task.str74[Lang.Idx]);
		return;
	end

	-- 只有玩家处于这个任务的第一个引用子任务的时候才能共享
	if (self:GetFinishedRefer(nTaskId) > 0) then
		me.SysMsg(Lang.task.str75[Lang.Idx]);
		return;
	end

	local tbReferData	= self.tbReferDatas[nReferId];
	local tbVisable	= tbReferData.tbVisable;

	local plOld	= me;
	local nOldPlayerIdx = me.nPlayerIndex;
	for i = 1, nMemberCount do
		me	= tbTeamMembers[i];
		if (me.nPlayerIndex ~= nOldPlayerIdx) then
			if (Task:AtNearDistance(me, plOld) == 1) then
				local tbPlayerTask = self:GetPlayerTask(me);
				if (not tbPlayerTask.tbTasks[nTaskId]) then
					if (self:GetFinishedRefer(nTaskId) <= 0) then  -- 只有从没接这个任务的队友才能接受
						if (Lib:DoTestFuncs(tbVisable)) then
							self:AskAccept(nTaskId, nReferId, plOld);
						else
							plOld.Msg(me.szName..Lang.task.str76[Lang.Idx])
						end
					else
						plOld.Msg(me.szName..Lang.task.str77[Lang.Idx]);
					end
				end
			else
				plOld.Msg(me.szName..Lang.task.str78[Lang.Idx]);
			end
		end
	end
	me	= plOld;
end


-------------------------------------------------------------------------
-- 接到客户端领奖
function Task:OnAward(nTaskId, nReferId, nChoice)
	-- 需判断是否会接下一个子任务
	BlackSky:GiveMeBright(me);
	if (nChoice == -1) then
		me.SysMsg(Lang.task.str79[Lang.Idx]);
		return;
	end;
	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTask	= tbPlayerTask.tbTasks[nTaskId];
	if (not tbTask or tbTask.nCurStep ~= -1) then --防止客户端舞弊
		me.SysMsg(Lang.task.str80[Lang.Idx]);
		return;
	end

	if (tbTask and tbTask.nReferId ~= nReferId) then -- 错误的ReferId
		return;
	end

	if (not TaskCond:HaveBagSpace(tbTask.tbReferData.nBagSpaceCount)) then
		Dialog:SendInfoBoardMsg(me, string.format(Lang.task.str81[Lang.Idx],tbTask.tbReferData.nBagSpaceCount));
		return;
	end

	local tbAwards	= self:GetAwardsForMe(nTaskId, true);
	--print("任务奖励为")
	--Lib:ShowTB(tbAwards)
	if (tbAwards.tbOpt[nChoice]) then
		-- 判断一下要领取的这个物品是不是需要扣除金币替代物才能领取，如果是的话，就先扣除
		-- 这里的nIndex 对应的是speoptaward.txt 文件中的nIndex 那一列，是用来标识物品的
		local nIndex = -1;
		if (tbAwards.tbOpt[nChoice].varValue and tbAwards.tbOpt[nChoice].varValue[1]) then
			nIndex = tbAwards.tbOpt[nChoice].varValue[1];
		end

		if (nIndex ~= -1 and self:IsSpeOptAward(nTaskId, nReferId, nIndex) == 1) then
			local bCanGetSpeOptAward, szErrMsg = self:GetSpeOpt_Cost(nTaskId, nReferId, nIndex);
			if (not bCanGetSpeOptAward or bCanGetSpeOptAward ~= 1) then
				if (szErrMsg and szErrMsg ~= "") then
					me.SysMsg(szErrMsg);
				end
				return;
			end
			self:GiveAward(tbAwards.tbOpt[nChoice], nTaskId);
		else
			self:GiveAward(tbAwards.tbOpt[nChoice], nTaskId);
		end
	end;
	for _, tbAward in pairs(tbAwards.tbFix) do
		self:GiveAward(tbAward, nTaskId);
	end;


	if (tbAwards.tbRand[1]) then
		local nSum	= 0;
		local nCurSum = 0;
		for _,tbAward in pairs(tbAwards.tbRand) do
			nSum = nSum + tbAward.nRate;
		end
		if (nSum >= 1) then
			local nRand	= MathRandom(nSum);
			for _, tbAward in pairs(tbAwards.tbRand) do
				nCurSum	= nCurSum + tbAward.nRate;
				if (nCurSum > nRand) then
					self:GiveAward(tbAward, nTaskId);
					break;
				end
			end
		end
	end

	self:SetFinishedRefer(nTaskId, nReferId);

	self:CloseTask(nTaskId, "finish");

	self:OnFinishTask(me, nTaskId) -- 任务完成事件，响应FinishTask Target by KK

	self:LogFinishTask(nTaskId);

	--统计点 by ldy
	local szTask = string.format("nTaskId=%d,nReferId=%d",nTaskId,nReferId);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,51,szTask);
	--print(".." .. szTask);
	
end

--===================================================

-- 在获取要扣除特定物品才能获得的可选奖励前，扣除物品
function Task:GetSpeOpt_Cost(nTaskId, nReferId, nIndex)
	local szErrMsg = "";

	if (not nTaskId or not nReferId or not nIndex or nTaskId <= 0 or nReferId <= 0) then
		return 0;
	end

	if (self:IsSpeOptAward(nTaskId, nReferId, nIndex) == 0) then
		return 0;
	end

	local tbSpeOptAward = self:GetSpeOptInfo(nTaskId, nReferId, nIndex);
	if (not tbSpeOptAward) then
		return 0;
	end

	local nCostNum = tbSpeOptAward.nCost;
	if (nCostNum == 0) then
		-- 扣除0个物品，直接认为扣除成功
		return 1;
	end
	local tbCostGDPL = tbSpeOptAward.tbCostGDPL;
	local szCostItem = KItem.GetNameById(unpack(tbSpeOptAward.tbCostGDPL));
	if (not szCostItem or nCostNum < 0) then
		return 0;
	end

	local tbFind = me.pItem.FindItemInBags(unpack(tbCostGDPL));
	if (#tbFind < nCostNum) then
		szErrMsg = string.format(Lang.task.str82[Lang.Idx], nCostNum, szCostItem);
		return 0, szErrMsg;
	end

	local bRet = me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));
	if (bRet ~= 0) then
		local szLog = string.format(Lang.task.str83[Lang.Idx], nCostNum, szCostItem);
		Dbg:WriteLog(Lang.task.str84[Lang.Idx], me.szName, szLog);
		return 0;
	end

	return 1;
end

-------------------------------------------------------------------------

-- 任务产出log
function Task:TskProduceLog(nTaskId, nType, nValue)
	if (not nTaskId or not nType or not nValue or nTaskId <= 0 or nValue <= 0) then
		return;
	end

	local nSubTaskId, nTskProType, szAwardType = self:__TskProduceLog_GetInfo(nType, nTaskId);
	if (not nSubTaskId or not nTskProType or not szAwardType) then
		return;
	end

	local szLog = string.format("%s,%s,%s,%s,%s", nTaskId, nSubTaskId, nTskProType, szAwardType, nValue);
	--StatLog:WriteStatLog("stat_info", "taskproduct", "currency", me.dwId, szLog);
end

function Task:__TskProduceLog_GetInfo(nMoneyType, nTaskId)
	local tbAwardType = {
		[self.TSKPRO_LOG_TYPE_MONEY] = "money",
		[self.TSKPRO_LOG_TYPE_BINDMONEY] = "bindmoney",
		[self.TSKPRO_LOG_TYPE_BINDCOIN] = "bindcoin",
		};

	local tbTskProType = {
		[self.emType_Main]		= self.emTskProType_Main,
		[self.emType_Branch]	= self.emTskProType_Branch,
		[self.emType_World]		= self.emTskProType_World,
		[self.emType_Random]	= self.emTskProType_Random,
		[self.emType_Camp]		= self.emTskProType_Camp,
		};

	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTask = tbPlayerTask.tbTasks[nTaskId];
	local nSubTaskId = tbTask.nReferId or 0;
	local nTaskType = tbTask.tbTaskData.tbAttribute.TaskType;
	local nTskProType = tbTskProType[nTaskType] or 0;
	--if (nTaskId == Merchant.TASKDATA_ID) then
	--	nTskProType = self.emTskProType_Merchant;
	--end

	return nSubTaskId, nTskProType, tbAwardType[nMoneyType];
end


-------------------------------------------------------------------------
-- 给与一组奖励，并提示获得奖品
function Task:GiveAward(tbAward, nTaskId)

	local szType	= tbAward.szType;
	local varValue	= tbAward.varValue;

	if (szType == "exp") then
		local nExpPercent = tbAward.varValue;
		if nExpPercent > 1 then
		  nExpPercent = 1
		end
		local tbTaskData = self.tbTaskDatas[nTaskId]
		local tbAward = KAward.GetAssetAwardData("levelexp", tbTaskData.nLevel+1); -- 得到角色在任务等级时的升级经验
		local nExp = tbAward.nExpAwardId * nExpPercent
		me.AddExp(nExp);
	elseif (szType == "money" or szType == "bindmoney") then
		  me.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, tbAward.varValue)
			self:TskProduceLog(nTaskId, self.TSKPRO_LOG_TYPE_BINDMONEY, tbAward.varValue);
	elseif (szType == "activemoney") then
		local tbPlayerTask	= self:GetPlayerTask(me);
		local tbTask		= tbPlayerTask.tbTasks[nTaskId];
		if (tbTask) then
			me.Earn(tbAward.varValue, Player.emKEARN_TASK_GIVE);
			tbTask.nLogMoney = tbAward.varValue;
			KStatLog.ModifyAdd("jxb", Lang.task.str85[Lang.Idx] ..self:GetTaskTypeName(nTaskId), Lang.task.str86[Lang.Idx], tbAward.varValue);
			self:TskProduceLog(nTaskId, self.TSKPRO_LOG_TYPE_MONEY, tbAward.varValue);
		end
	elseif (szType == "title") then
		me.AddTitle(tbAward.varValue[1], tbAward.varValue[2], tbAward.varValue[3], 0)
	elseif (szType == "taskvalue") then
		if (tbAward.varValue[1] == 2025 and tbAward.varValue[2] == 2) then
			Task.tbArmyCampInstancingManager.StatLog:WriteLog(12, tbAward.varValue[3]);
			me.AddMachineCoin(tbAward.varValue[3]);
		else
			me.pTask.SetTask(tbAward.varValue[1], tbAward.varValue[2], tbAward.varValue[3], 1);
		end
	elseif(szType == "script") then
		-- 直接执行脚本
		Lib:DoExecFuncs({tbAward.varValue}, nTaskId)
	elseif (szType == "item") then
		local nCount = tonumber(tbAward.szAddParam1) or 1;
		if (nCount < 1) then
			nCount = 1;
		end
		for i = 1, nCount do
			local pItem 	= Task:AddItem(me, tbAward.varValue, nTaskId);
			--self:WriteItemLog(pItem, me, nTaskId);
			if Task:IsAutoUse(pItem) == 1 then
				me.pItem.AutoUse(pItem);
			end
		end
	elseif (szType == "customizeitem") then
		Task:AddItem(me, tbAward.varValue, nTaskId);
	elseif (szType == "arrary") then
		for _, tbOneAward in ipairs(tbAward.varValue) do
			self:GiveAward(tbOneAward, nTaskId);
		end;
	elseif (szType == "bindcoin") then
		me.pItem.AddBindCoin(tbAward.varValue[1], Player.emKBINDCOIN_ADD_TASK);
		KStatLog.ModifyAdd("bindcoin",  Lang.task.str85[Lang.Idx] ..self:GetTaskTypeName(nTaskId),  Lang.task.str86[Lang.Idx], tbAward.varValue[1]);
		self:TskProduceLog(nTaskId, self.TSKPRO_LOG_TYPE_BINDCOIN, tbAward.varValue[1]);
	elseif (szType == "factionequip") then
		self:GetSpeOpt_FactionEquip(nTaskId, varValue[1]);
	elseif (szType == "ylenergy") then  -- 奖励游龙真气
		me.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, tbAward.varValue);
	elseif (szType == "buff") then
		BufferApi.AddBufferToCharacter(me.dwId, 0, me.dwId, tbAward.varValue, 1);  -- 任务领奖发一个buffer
  elseif (szType == "Spirit") then
		  me.pPlayerPurse.AddMoney(Purse.EM_MONEY_SPIRIT, tbAward.varValue)
  elseif (szType == "Prestige") then
		  me.pPlayerPurse.AddMoney(Purse.EM_MONEY_PRESTIGE, tbAward.varValue)
	elseif (szType == "pet") then
		-- 获取宠物的对应品质~ 在宠物岛
		Pet:AddPet(me, tbAward.varValue[1], tbAward.varValue[2])  -- 发放宠物
	end;
end;

function Task:IsAutoUse(pItem)
	if pItem.szClass == "task_pet_card" or
		pItem.szClass == "task_ride_card" or
		pItem.szClass == "autogive" then

		return 1;
	end
	return 0;
end

-- 获取特殊奖励（门派装备），注意，并不是所有的门派装备奖励都走到这里
-- 只有出现在可选奖励里面，并且需要扣除一些费用才能得到的门派装备才会执行到这里
function Task:GetSpeOpt_FactionEquip(nTaskId, nIndex)
	if (not nTaskId or not nIndex or  nTaskId <= 0) then
		return;
	end

	local tbPlayerTask = self:GetPlayerTask(me);
	local tbTask = tbPlayerTask.tbTasks[nTaskId];
	local nReferId = tbTask.nReferId;

	local tbAwardInfo = self:GetSpeOptInfo(nTaskId, nReferId, nIndex);
	local tbGDPL = tbAwardInfo.tbGDPL;
	if (not tbGDPL or #tbGDPL ~= 4) then
		return;
	end

	local nCostNum = tbAwardInfo.nCost;
	local tbCostGDPL = tbAwardInfo.tbCostGDPL;
	local szCostItem = KItem.GetNameById(unpack(tbCostGDPL));
	if (not nCostNum or not szCostItem) then
	end

	local pItem = me.AddItem(unpack(tbGDPL));
	if (pItem) then
		pItem.Bind(1);

		local szLog = string.format(Lang.task.str87[Lang.Idx], nCostNum, szCostItem, pItem.szName);
		Dbg:WriteLog("获得新手装备", me.szName, szLog);
	end
end

function Task:WriteItemLog(pItem, pPlayer, nTaskId)
	local tbPlayerTask	= self:GetPlayerTask(pPlayer);
	local tbTask		= tbPlayerTask.tbTasks[nTaskId];
	local szLogMsg		= "";
	if (tbTask) then
		szLogMsg	= string.format("完成%s，任务ID：%x，支线任务ID：%x", tbTask:GetName(), tbTask.nTaskId, tbTask.nReferId);
	else
		szLogMsg	= string.format("没有Id为 %x 的任务！", nTaskId);
	end;
	local bGiveSuc 	= 1;
	if (not pItem) then
		bGiveSuc = 0;
	end
--	me.ItemLog(pItem, bGiveSuc, Log.emKITEMLOG_TYPE_FINISHTASK, szLogMsg);
	local szLog = string.format("%s获得了1个%s", szLogMsg, pItem.szName);
	Dbg:WriteLog("Task", "任务获得物品", me.szAccount, me.szName, szLog);
end

-------------------------------------------------------------------------
-- 设定特定任务完成的最后一个引用子任务ID
function Task:SetFinishedRefer(nTaskId, nReferId)
	local nLogReferId = nReferId;
	local tbTaskData = self.tbTaskDatas[nTaskId];
	-- 废掉了referid，不作重复判断，游龙任务系统结构不一样
	if (tbTaskData.tbAttribute["Repeat"]) then  -- 如果重复
		local nReferIdx		= self:GetFinishedIdx(nTaskId) + 2;	--根据重复次数获取索引
		local nNextReferId	= tbTaskData.tbReferIds[nReferIdx];  -- 获取指定任务id
		if (not nNextReferId) then
			nReferId = 0;  -- 无子任务
			return; -- by kk  -- 重复任务，无需作完成记录
		end
	end

	me.pTask.SetTask(self.TASK_FINISH_RECORD, nTaskId, nTaskId, 1); -- Group1000保存了所有任务的完成情况,其中任务变量Id(nTaskId)也就是任务Id
end


-------------------------------------------------------------------------
-- 使任务失败
function Task:Failed(nTaskId)
	if (type(nTaskId) == "string") then
		nTaskId = tonumber(nTaskId, 16);
	end

	return self:CloseTask(nTaskId, "failed");
end;

-------------------------------------------------------------------------

-- 获取指定玩家的修仙任务
function Task:GetPlayerEscort(pPlayer)
	for _, tbTask in pairs(self:GetPlayerTask(pPlayer).tbTasks) do
		local ownerTaskType = Task:GetTaskType(tbTask.nTaskId);
		if ownerTaskType == "Escort" then	-- 如果已经接到的任务有修仙任务
			return tbTask.nTaskId
		end
	end
	return 0;
end

--[[
-- 刷新修仙任务, 增加修仙任务计数
function Task:RefreshEscort(nTaskId)
	local nEscortId = self:GetPlayerEscort(me);
	-- 不是当前的修仙任务
	if nTaskId ~= nEscortId then
		return -1;
	end
end
--]]

-- 关闭修仙任务
function Task:CloseEscort(nTaskId)
	local szType = self:GetTaskType(nTaskId);
	if szType == "Escort" then
		me.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:RemoveEscortClouds",});  -- ProtectNpc Target也会执行
	end
end
-- 关闭任务
function Task:CloseTask(nTaskId, szReason)
	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTask	= tbPlayerTask.tbTasks[nTaskId];
	if (not tbTask) then
		return nil;
	end;

	tbTask:CloseCurStep(szReason);
	if (szReason == "giveup") then
		self:CloseEscort(nTaskId);
		me.SysMsg(Lang.task.str92[Lang.Idx] ..tbTask:GetName());
		--me.SysMsg(tbTask.tbReferData.szGossip);
	elseif (szReason == "failed") then
		me.SysMsg(Lang.task.str93[Lang.Idx] ..tbTask:GetName());
		--me.SysMsg(tbTask.tbReferData.szGossip);
	elseif (szReason == "finish") then
		--self:RefreshEscort(nTaskId);
		-- 2. 低于50级以下角色的任务事件，不记入角色历程日志。
		if (me.GetLevel() >= 50) then
			--me.SysMsg("任务完成："..tbTask:GetName());
			--me.CastSkill(self.nFinishTaskSkillId, 1, -1, me.GetNpc().nIndex);
			local szLogMsg = string.format("完成%s，任务ID：%x，支线任务ID：%x", tbTask:GetName(), tbTask.nTaskId, tbTask.nReferId);
			if (tbTask.nLogMoney > 0) then
				szLogMsg = szLogMsg .. string.format(" 奖励%d银两", tbTask.nLogMoney)
				tbTask.nLogMoney = 0;
			end
			--me.PlayerLog(Log.emKPLAYERLOG_TYPE_FINISHTASK ,szLogMsg);
		end

	end

	tbPlayerTask.tbTasks[nTaskId]	= nil;
	tbPlayerTask.nCount	= tbPlayerTask.nCount - 1;
	if (tbPlayerTask.nCount <= 0) then

	end;

	me.pTask.ClearTaskGroup(tbTask.nSaveGroup,1);

	KTask.SendRefresh(me, nTaskId, tbTask.nReferId, 0);

	if (szReason == "finish") then
		local tbFinishExecute = tbTask.tbSubData.tbFinishExecute;
		if (tbFinishExecute and #tbFinishExecute > 0) then
			Lib:DoExecFuncs(tbFinishExecute);
		end;

		local tbSubExecute = tbTask.tbSubData.tbExecute;
		if (tbSubExecute and #tbSubExecute > 0) then
			Lib:DoExecFuncs(tbSubExecute);
		end;
	elseif (szReason == "failed" or szReason == "giveup") then
		local tbFailedExecute = tbTask.tbSubData.tbFailedExecute;
		if (tbFailedExecute and #tbFailedExecute > 0) then
			Lib:DoExecFuncs(tbFailedExecute);
		end;
	end

	return 1;
end;
-------------------------------------------------------------------------
-- 追加Npc对话选项
function Task:AppendNpcMenu(tbOption)
	local nNpcTempId	= him.dwTemplateId;
	local tbPlayerTasks	= self:GetPlayerTask(me).tbTasks;
	local bHaveRelation	= 0;
	-- 添加已有任务对话选项
	for _, tbTask in pairs(tbPlayerTasks) do
		if (tbTask:AppendNpcMenu(nNpcTempId, tbOption, him)) then
			bHaveRelation	= 1;
		end
	end
	-- 添加可见任务(未接任务)对话选项
	for _, tbTaskData in pairs(self.tbTaskDatas) do
		if (not tbPlayerTasks[tbTaskData.nId]) then  								-- 该玩家未接这任务
			local nReferIdx		= self:GetFinishedIdx(tbTaskData.nId) + 1;			-- +1表示将要继续的任务
			local nReferId		= tbTaskData.tbReferIds[nReferIdx];
			if (not tbTaskData.tbAttribute["Repeat"] or self:CanAcceptRepeatTask() == 1) then -- 非重复任务
				if (nReferId) then
					local tbReferData	= self.tbReferDatas[nReferId];
					if (tbReferData.nAcceptNpcId == nNpcTempId) then
						local tbVisable	= tbReferData.tbVisable;
						local bOK	= Lib:DoTestFuncs(tbVisable);						-- 可见条件测试
						if (bOK) then
							bHaveRelation	= 1;
							local tbSubData	= self.tbSubDatas[tbReferData.nSubTaskId];
							if (tbSubData) then
								local szMsg = "";
								if (tbSubData.tbAttribute.tbDialog.Start) then
									if (tbSubData.tbAttribute.tbDialog.Start.szMsg) then 		-- 未分步骤
										szMsg = tbSubData.tbAttribute.tbDialog.Start.szMsg;
									else
										szMsg = tbSubData.tbAttribute.tbDialog.Start.tbSetpMsg[1];
									end
								end

								local szTypeIDName = self:GetTaskIDName(tbReferData.nSubTaskId);  -- 取得该任务类型的中文名称

								tbOption[#tbOption + 1]	= {szTypeIDName .. "<color=green>"..tbReferData.szName,
									TaskAct.TalkInDark, TaskAct, szMsg,
									self.AskAccept, self, tbTaskData.nId, nReferId};
							end
						end
					end
				end
			end
		end
	end

	--Lib:SmashTable(tbOption);  -- 不打乱
	return bHaveRelation;
end;

-- 获取TypeID, 对应的任务名称 中文
function Task:GetTaskIDName(nTaskTemplateId)
    local tbTaskData = self.tbTaskDatas[nTaskTemplateId];
	local szTypeIDName = "";
	if (tbTaskData.tbAttribute.TypeID == self.emTskProType_Main) then
		szTypeIDName = Lang.task.str94[Lang.Idx];
	elseif (tbTaskData.tbAttribute.TypeID == self.emTskProType_Branch or tbTaskData.tbAttribute.TypeID == Task.emTskProType_Branch_New) then
		szTypeIDName = Lang.task.str95[Lang.Idx];
	elseif (tbTaskData.tbAttribute.TypeID == self.emTskProType_BodyGuard) then
		szTypeIDName = Lang.task.str96[Lang.Idx];
	elseif (tbTaskData.tbAttribute.TypeID == self.emTskProType_Run) then
		szTypeIDName = Lang.task.str97[Lang.Idx];
	end

	return szTypeIDName;
end;

-------------------------------------------------------------------------
-- 靠近一个NPC时触发，显示在小地图上的技能和头上的问号，叹号 (客户端）
function Task:OnApproachNpc()
	local tbTaskState = Task:CheckTaskOnNpc();

	self:ChangeNpcFlag(tbTaskState);
end;


-------------------------------------------------------------------------
-- 检测Npc任务标记，用于客户端显示  (客户端)
-- TODO: liuchang 之后可能有需求根据是否已完成目标添加新的技能
function Task:CheckTaskOnNpc()
	local tbPlayerTasks	= self:GetPlayerTask(me).tbTasks;

	-- 检测已有任务
	for _, tbTask in pairs(tbPlayerTasks) do
		if (tbTask:CheckTaskOnNpc() == 1) then
			if (tbTask.tbTaskData.tbAttribute["Repeat"]) then
				return self.CheckTaskFlagSkillSet.RepeatCanReply;
			elseif (tbTask.tbTaskData.tbAttribute.TaskType == self.emType_Main) then
				return self.CheckTaskFlagSkillSet.MainCanReply;
			elseif (tbTask.tbTaskData.tbAttribute.TaskType == self.emType_Branch) then
				return self.CheckTaskFlagSkillSet.BranchCanReply;
			elseif (tbTask.tbTaskData.tbAttribute.TaskType == self.emType_World) then
				return self.CheckTaskFlagSkillSet.WorldCanReply;
			elseif (tbTask.tbTaskData.tbAttribute.TaskType == self.emType_Random) then
				return self.CheckTaskFlagSkillSet.RandomCanReply;
			elseif (tbTask.tbTaskData.tbAttribute.TaskType == self.emType_Camp) then
				return self.CheckTaskFlagSkillSet.RandomCanReply;
			else
				assert(false);
			end
		end;
	end;

	-- 检测可见任务
	for _, tbTaskData in pairs(self.tbTaskDatas) do
		if (not tbPlayerTasks[tbTaskData.nId]) then
			local nReferIdx		= self:GetFinishedIdx(tbTaskData.nId) + 1;--+1表示将要做的任务
			local nReferId		= tbTaskData.tbReferIds[nReferIdx];
			if (nReferId) then
				local tbReferData	= self.tbReferDatas[nReferId];
				if (tbReferData.nAcceptNpcId == him.nTemplateId) then
					local tbVisable	= tbReferData.tbVisable;
					local bOK	= Lib:DoTestFuncs(tbVisable);
					if (bOK) then
						if (tbTaskData.tbAttribute["Repeat"]) then
							if (self:CanAcceptRepeatTask() ~= 1) then
								return;
							end
							return self.CheckTaskFlagSkillSet.RepeatCanAccept;
						elseif (tbTaskData.tbAttribute.TaskType == self.emType_Main) then
							return self.CheckTaskFlagSkillSet.MainCanAccept;
						elseif (tbTaskData.tbAttribute.TaskType == self.emType_Branch) then
							return self.CheckTaskFlagSkillSet.BranchCanAccept;
						elseif (tbTaskData.tbAttribute.TaskType == self.emType_World) then
							return self.CheckTaskFlagSkillSet.WorldCanAccept;
						elseif (tbTaskData.tbAttribute.TaskType == self.emType_Random) then
							return self.CheckTaskFlagSkillSet.RandomCanAccept;
						elseif (tbTaskData.tbAttribute.TaskType == self.emType_Camp) then
							return self.CheckTaskFlagSkillSet.RandomCanAccept;
						else
							assert(false);
						end
					end
				end;
			end;
		end;
	end;

	return;

end;



-------------------------------------------------------------------------
-- 改变NPC的任务状态显示,去除不需要的，添加需要的
function Task:ChangeNpcFlag(tbSkillId)

	local tbTempTotleSkill = {};
	for _,tbSkillSet in pairs(self.CheckTaskFlagSkillSet) do
		for _, Skill in pairs(tbSkillSet) do
			tbTempTotleSkill[Skill] = 1;
		end
	end

	local tbTotleSkill = {};
	for Skill,Item in pairs(tbTempTotleSkill) do
		tbTotleSkill[#tbTotleSkill+1] = Skill;
	end


	if (not tbSkillId) then
		for _,nSkillId in ipairs(tbTotleSkill) do
			him.RemoveTaskState(nSkillId);
		end
		return;
	end

	for _,nSkillId in ipairs(tbTotleSkill) do
		local bRemove = 1;
		for _, nRetainSkillId in ipairs(tbSkillId) do
			local tbBeRemoveSet = {};
			if (nRetainSkillId == nSkillId) then
				bRemove = 0;
			end
		end
		if (bRemove == 1) then
			him.RemoveTaskState(nSkillId);
		end
	end

	for _, nRetainSkillId in ipairs(tbSkillId) do
		him.AddTaskState(nRetainSkillId);
	end
end


-------------------------------------------------------------------------
-- 玩家使用任务物品时候触发
function Task:OnTaskItem(pItem)
	local nParticular	= pItem.nParticular;
	local tbPlayerTasks	= self:GetPlayerTask(me).tbTasks;

	-- 用于已有任务
	for _, tbTask in pairs(tbPlayerTasks) do
		if (tbTask:OnTaskItem(nParticular)) then
			return 1;
		end;
	end;

	-- 用于接新任务
	for _, tbTaskData in pairs(self.tbTaskDatas) do
		if (not tbPlayerTasks[tbTaskData.nId]) then
			local nReferIdx	= self:GetFinishedIdx(tbTaskData.nId) + 1;--+1表示将要做的任务
			local nReferId	= tbTaskData.tbReferIds[nReferIdx];
			if (nReferId) then
				local tbReferData = self.tbReferDatas[nReferId];
				local tbSubData	  = self.tbSubDatas[tbReferData.nSubTaskId];
				local szMsg = "";
				if (tbSubData.tbAttribute.tbDialog.Start) then
					if (tbSubData.tbAttribute.tbDialog.Start.szMsg) then -- 未分步骤
						szMsg = tbSubData.tbAttribute.tbDialog.Start.szMsg;
					else
						szMsg = tbSubData.tbAttribute.tbDialog.Start.tbSetpMsg[1];
					end
				end


				if (tbReferData.nParticular == pItem.nParticular) then
					local tbVisable	= tbReferData.tbVisable;
					local bOK, szMsg = Lib:DoTestFuncs(tbVisable);						-- 可见条件测试
					if (bOK) then
						TaskAct:TalkInDark(szMsg, self.AskAccept, self, tbTaskData.nId, nReferId);
						return 1
					else
						me.SysMsg(szMsg);
						return nil;
					end
				end;
			end;
		else
			local nReferIdx	= self:GetFinishedIdx(tbTaskData.nId) + 1;--+1表示将要做的任务
			local nReferId	= tbTaskData.tbReferIds[nReferIdx];
			if (nReferId) then
				local tbReferData	= self.tbReferDatas[nReferId];
				if (tbReferData.nParticular == pItem.nParticular) then
					me.SysMsg(Lang.task.str98[Lang.Idx]);
					return nil;
				end
			end

		end;
	end;

	return nil;
end;



-------------------------------------------------------------------------
-- 触发下个步骤时调用,NPC对话和使用道具的时候，见CalssBase.SetCurStep
function Task:Active(nTaskId, nReferId, nCurStep)
	local tbTask	= self:GetPlayerTask(me).tbTasks[nTaskId];
	if (not tbTask) then
		return nil;
	end;
	assert(tbTask.nReferId == nReferId);
	--assert(tbTask.nCurStep == nCurStep); -- 改为return nil; zounan
	if tbTask.nCurStep ~= nCurStep then
		return nil;
	end

	return tbTask:Active();
end;


-------------------------------------------------------------------------
-- 和withprocesstagnpc类型NPC交互时触发，不会弹Say界面，而是进度条之类的及时触发
function Task:OnExclusiveDialogNpc()
	local nTemplateId = him.dwTemplateId;
	local tbPlayerTasks	= self:GetPlayerTask(me).tbTasks;


	for _, tbTask in pairs(tbPlayerTasks) do
		if (tbTask:OnExclusiveDialogNpc(nTemplateId)) then
			return 1;
		end;
	end;

end

-- 触发图标类任务例如放烟火
function Task:OnExclusiveIconProcess(pPlayer)

	local tbPlayerTasks	= self:GetPlayerTask(pPlayer).tbTasks;

	for _, tbTask in pairs(tbPlayerTasks) do
		if (tbTask:OnExclusiveDialogNpc(self.ICON_PROCESS_ID)) then
			return 1;
		end;
	end;

end


-------------------------------------------------------------------------
-- 根据引用子任务Id获取奖励表
function Task:GetAwards(nReferId)
	local tbAwardRet = {};
	local tbRefSubData	= self.tbReferDatas[nReferId];
	if (tbRefSubData) then
		local tbAwardSrc = tbRefSubData.tbAwards;
		-- 1.固定奖励
		tbAwardRet.tbFix = {};
		for _, tbFix in ipairs(tbAwardSrc.tbFix) do
			if (tbFix.tbConditions) then
				if (Lib:DoTestFuncs(tbFix.tbConditions) == 1) then
					table.insert(tbAwardRet.tbFix, tbFix);
				end
			else
				table.insert(tbAwardRet.tbFix, tbFix);
			end
		end

		-- 2.随机奖励
		tbAwardRet.tbRand = {};
		for _, tbRand in ipairs(tbAwardSrc.tbRand) do
			if (tbRand.tbConditions) then
				if (Lib:DoTestFuncs(tbRand.tbConditions) == 1) then
					table.insert(tbAwardRet.tbRand, tbRand);
				end
			else
				table.insert(tbAwardRet.tbRand, tbRand);
			end
		end

		-- 3.可选奖励
			tbAwardRet.tbOpt = {};
		for _, tbOpt in ipairs(tbAwardSrc.tbOpt) do
			if (tbOpt.tbConditions) then
				if (Lib:DoTestFuncs(tbOpt.tbConditions) == 1) then
					table.insert(tbAwardRet.tbOpt, tbOpt);
				end
			else
				table.insert(tbAwardRet.tbOpt, tbOpt);
			end
		end

		return tbAwardRet;
	else
		return nil;
	end;
end;


-- 根据直接获取奖励表
function Task:GetAwardsForMe(nTaskId, bOutMsg)
	local tbAwardRet = {};
	local tbPlayerTask = self:GetPlayerTask(me);
	local tbTask = tbPlayerTask.tbTasks[nTaskId];
	if (not tbTask or not tbTask.tbReferData) then
		return nil;
	end
	local tbAwardSrc = tbTask.tbReferData.tbAwards;
	tbAwardRet.tbFix = self:GetTypeAward(tbAwardSrc.tbFix, bOutMsg);
	tbAwardRet.tbRand = self:GetTypeAward(tbAwardSrc.tbRand, bOutMsg);
	tbAwardRet.tbOpt = self:GetTypeAward(tbAwardSrc.tbOpt, bOutMsg);
	return tbAwardRet;
end;

function Task:GetTypeAward(tbSrc, bOutMsg)
	local tb = {};
	for _, tbAward in ipairs(tbSrc) do
		if (tbAward.tbConditions) then
			local bRet, szMsg = Lib:DoTestFuncs(tbAward.tbConditions);
			if (bRet == 1) then
				table.insert(tb, tbAward);
			elseif (szMsg and bOutMsg) then
				Dialog:SendBlackBoardMsg(me, szMsg);
			end
		else
			table.insert(tb, tbAward);
		end
	end
	return tb;
end
-------------------------------------------------------------------------
-- 取得当前玩家任务数据
function Task:GetPlayerTask(pPlayer)
	local tbPlayerData	= pPlayer.GetTempTable("Task");
	local tbPlayerTask	= tbPlayerData.tbTask;
	if (not tbPlayerTask) then
		tbPlayerTask	= {
			nCount	= 0,
			tbTasks	= {},
		};
		tbPlayerData.tbTask	= tbPlayerTask;
	end
	return tbPlayerTask;
end

function Task:HasTask(pPlayer, nTaskId, nStep)
	local tbPlayerTask	= self:GetPlayerTask(me);
	local tbTask = tbPlayerTask.tbTasks[nTaskId];
	if tbTask == nil then
		return 0;
	end
	if nStep ~= nil and tbTask.nCurStep ~= nStep then
		return 0;
	end
	return 1;
end

-------------------------------------------------------------------------
-- 取得特定任务完成的最后一个引用子任务ID
function Task:GetFinishedRefer(nTaskId)
	return me.pTask.GetTask(1000, nTaskId);
end


-------------------------------------------------------------------------
-- 取得特定任务完成的最后一个引用子任务序号
function Task:GetFinishedIdx(nTaskId)
	local nReferId	= self:GetFinishedRefer(nTaskId);
	if (nReferId == 0) then
		return 0;
	end;
	local tbReferData	= self.tbReferDatas[nReferId];
	if (tbReferData) then
		return tbReferData.nReferIdx;
	end
	local tbTaskData	= self.tbTaskDatas[nTaskId];
	return #tbTaskData.tbReferIds;
end


-------------------------------------------------------------------------
-- 或得一个任务的名字
function Task:GetTaskName(nTaskId)
	if (not self.tbTaskDatas[nTaskId]) then
		self:LoadTask(nTaskId);
	end;

	return self.tbTaskDatas[nTaskId].szName;
end


-------------------------------------------------------------------------
-- 获得一个任务的描述
function Task:GetTaskDesc(nTaskId)
	if (not self.tbTaskDatas[nTaskId]) then
		self:LoadTask(nTaskId);
	end;

	return self.tbTaskDatas[nTaskId].szDesc;
end


-------------------------------------------------------------------------
--获得一个引用子任务名
function Task:GetManSubName(nReferId)
	return self.tbReferDatas[nReferId].szName;
end


-------------------------------------------------------------------------
--获得一个引用子任务描述
function Task:GetManSubDesc(nReferId)
	return self.tbReferDatas[nReferId].szMainDesc;
end

-- 任务完成时的事件, 是否激活下一个任务
function Task:OnFinishTask(pPlayer, nTaskId)
	-- 获取TaskSetting的NextTask, 如果有nexttask，自动发起下个任务的接任务
	local nNextTask = self.tbTaskDatas[nTaskId].tbAttribute["NextTask"];
	if (nNextTask ~= nil) then
		TaskAct:AskAccept(nNextTask);
	end

	local tbPlayerTask	= self:GetPlayerTask(pPlayer);
	for _, tbTask in pairs(tbPlayerTask.tbTasks) do
		for _, tbCurTag in ipairs(tbTask.tbCurTags) do
			if (tbCurTag.OnFinishTask) then
				tbCurTag:OnFinishTask(pPlayer, nTaskId)
			end
		end;
	end
  	self:CheckCanAcceptBranchTask(pPlayer, nTaskId)
end

-- 副本完成时的事件
function Task:OnFinishMission(pPlayer, nMissionId)
	local tbPlayerTask	= self:GetPlayerTask(pPlayer);
	for _, tbTask in pairs(tbPlayerTask.tbTasks) do
		for _, tbCurTag in ipairs(tbTask.tbCurTags) do
			if (tbCurTag.OnFinishMission) then
				tbCurTag:OnFinishMission(pPlayer, nMissionId)
			end
		end;
	end
end
-------------------------------------------------------------------------
-- 选择题事件
function Task:OnQuizChoice(pPlayer, nRight)
	local tbPlayerTask	= self:GetPlayerTask(pPlayer);
	for _, tbTask in pairs(tbPlayerTask.tbTasks) do
		for _, tbCurTag in ipairs(tbTask.tbCurTags) do
			if (tbCurTag.OnQuizChoice) then
				tbCurTag:OnQuizChoice(pPlayer, nRight)
			end
		end;
	end
end
-------------------------------------------------------------------------
function Task:OnKillNpc(pPlayer, pNpc)
	local tbStudentList 	= {};
	local tbTeammateList 	= {};

	-- 队友和徒弟(组队的徒弟)计数
	local nTeamId = pPlayer.GetTeamId();
	local tbTeamMembers, nMemberCount	= KTeam.GetTeamMemberList(nTeamId);
	if (tbTeamMembers) then
		for i = 1, nMemberCount do
			if (pPlayer.dwId ~= tbTeamMembers[i]) then
				tbTeammateList[#tbTeammateList + 1] = tbTeamMembers[i];
			end
		end
	end


	self:OnKillNpcForCount(pPlayer, pNpc, tbStudentList, tbTeammateList);
	self:OnKillNpcForItem(pPlayer, pNpc, tbStudentList, tbTeammateList);
	self:OnKillBossForItem(pPlayer, pNpc, tbStudentList, tbTeammateList);
end


-- 杀怪计数
function Task:OnKillNpcForCount(pPlayer, pNpc, tbStudentList, tbTeammateList)
	-- 自己的和队友的
	for _, tbMyTask in pairs(Task:GetPlayerTask(pPlayer).tbTasks) do
		for _, nPlayerId in ipairs(tbTeammateList) do
			local pMember = KGameBase.GetPlayerById(nPlayerId);
			if pMember then
				if (Task:AtNearDistance(pPlayer, pMember) == 1) then
					for _, tbTask in pairs(Task:GetPlayerTask(pMember).tbTasks) do
						if (tbMyTask.nReferId == tbTask.nReferId and (not tbMyTask.tbReferData.nShareKillNpc or tbMyTask.tbReferData.nShareKillNpc == 0)) then
							tbTask:OnKillNpcForCount(pPlayer, pNpc);
						end
					end
				end
			end
		end

		tbMyTask:OnKillNpcForCount(pPlayer, pNpc);
	end

	for _, nPlayerId in ipairs(tbTeammateList) do
		local pMember = KGameBase.GetPlayerById(nPlayerId);
		if pMember then
			if (pMember.dwSceneTemplateId == pPlayer.dwSceneTemplateId) then
				for _, tbTask in pairs(Task:GetPlayerTask(pMember).tbTasks) do
					if (tbTask.tbReferData.nShareKillNpc == 1) then
						tbTask:OnKillNpcForCount(pPlayer, pNpc);
					end
				end
			end
		end
	end
end

-- 杀怪获物(自己需要的可以掉多个,别人的只能掉一个)
function Task:OnKillNpcForItem(pPlayer, pNpc, tbStudentList, tbTeammateList)
	-- 自己和队友的
	local nDropCount = 0;
	for _, tbMyTask in pairs(Task:GetPlayerTask(pPlayer).tbTasks) do
		if (tbMyTask:OnKillNpcForItem(pPlayer, pNpc) == 1) then
			nDropCount = nDropCount + 1;
		end
		for _, nPlayerId in ipairs(tbTeammateList) do
			local pMember = KGameBase.GetPlayerById(nPlayerId);
			if (nDropCount > 0) then -- 保证不会因为队友增多造成物品掉落增多
				break;
			end
			if pMember then
				if (Task:AtNearDistance(pPlayer, pMember) == 1) then
					local tbTask = Task:GetPlayerTask(pMember).tbTasks[tbMyTask.nTaskId];
					if (tbTask) then
						if (tbMyTask.nReferId == tbTask.nReferId and (not tbMyTask.tbReferData.nShareKillNpc or tbMyTask.tbReferData.nShareKillNpc == 0)) then
							if (tbTask:OnKillNpcForItem(pPlayer, pNpc) == 1) then
								nDropCount = nDropCount + 1;
							end
						end
					end
				end
			end
		end
	end

	for _, nPlayerId in ipairs(tbTeammateList) do
		local pMember = KGameBase.GetPlayerById(nPlayerId);
		if pMember then
			if (nDropCount > 0) then
				break;
			end

			for _, tbTask in pairs(Task:GetPlayerTask(pMember).tbTasks) do
				if (pMember.dwSceneTemplateId == pPlayer.dwSceneTemplateId) then
					if (tbTask.tbReferData.nShareKillNpc == 1) then
						if (tbTask:OnKillNpcForItem(pPlayer, pNpc) == 1) then
							nDropCount = nDropCount + 1;
							break;
						end
					end
				end
			end
		end
	end
end

function Task:OnKillBossForItem(pPlayer, pNpc, tbStudentList, tbTeammateList)
	-- 自己的和队友的
	for _, tbMyTask in pairs(Task:GetPlayerTask(pPlayer).tbTasks) do
		for _, nPlayerId in ipairs(tbTeammateList) do
			local pMember = KGameBase.GetPlayerById(nPlayerId);
			if pMember then
				if (Task:AtNearDistance(pPlayer, pMember) == 1) then
					for _, tbTask in pairs(Task:GetPlayerTask(pMember).tbTasks) do
						if (tbMyTask.nReferId == tbTask.nReferId and (not tbMyTask.tbReferData.nShareKillNpc or tbMyTask.tbReferData.nShareKillNpc == 0)) then
							tbTask:OnKillBossForItem(pPlayer, pNpc);
						end
					end
				end
			end
		end
		tbMyTask:OnKillBossForItem(pPlayer, pNpc);
	end

	for _, nPlayerId in ipairs(tbTeammateList) do
		local pMember = KGameBase.GetPlayerById(nPlayerId);
		if pMember then
			if (pMember.dwSceneTemplateId == pPlayer.dwSceneTemplateId) then
				for _, tbTask in pairs(Task:GetPlayerTask(pMember).tbTasks) do
					if (tbTask.tbReferData.nShareKillNpc == 1) then
						tbTask:OnKillBossForItem(pPlayer, pNpc);
					end
				end
			end
		end
	end
end

if (not Task.tbTrackTaskSet) then
	Task.tbTrackTaskSet = {};
end

if (not Task.tbTrackTaskSet) then
	Task.tbTrackTaskSet = {};
end

function Task:SendAward(nTaskId, nReferId, nSelIdx)
	KTask.SendAward(me, nTaskId, nReferId, nSelIdx);
end


-- C
function Task:OnRefresh(nTaskId, nReferId, nParam)
	local tbPlayerTask	= self:GetPlayerTask(me);
	if (nParam and nParam ~= 0) then
		local tbTask		= tbPlayerTask.tbTasks[nTaskId];
		if (not tbTask) then
			local tbReferData	= self.tbReferDatas[nReferId];
			local nSubTaskId	= tbReferData.nSubTaskId;

			if (not self.tbSubDatas[nSubTaskId]) then
				-- 任务链的任务特殊处理
				if self:GetTaskType(nSubTaskId) == "Task" then
					self:LoadSub(nSubTaskId);
				end;
			end

			tbTask	= Task:NewTask(nReferId);
			if (self.tbTaskDatas[nTaskId].tbAttribute["AutoTrack"]) then
				self:OnTrackTask(nTaskId);
			end
		end

		tbTask:LoadData(nParam);
	elseif (tbPlayerTask.tbTasks[nTaskId]) then
		tbPlayerTask.tbTasks[nTaskId]:CloseCurStep("finish");
		tbPlayerTask.tbTasks[nTaskId] = nil;
		tbPlayerTask.nCount	= tbPlayerTask.nCount - 1;
	end

	CoreEventNotify(UiNotify.emCOREEVENT_TASK_REFRESH, nTaskId, nReferId, nParam);
end

function Task:OnTrackTask(nTaskId)
	CoreEventNotify(UiNotify.emCOREEVENT_TASK_TRACK, nTaskId);
end

function Task:ShowInfoBoard(szInfo)
	local szMsg = Lib:ParseExpression(szInfo);
	szMsg = Task:ParseTag(szMsg);
	CoreEventNotify(UiNotify.emCOREEVENT_TASK_SHOWBOARD, szMsg)
end

function Task:GetPlayerMainTask(pPlayer)
	local tbMainTaskLogData = {};
	local tbPlayerTasks = self:GetPlayerTask(pPlayer);
	for _, tbTask in pairs(tbPlayerTask.tbTasks) do
		if (tbTask.tbTaskData.tbAttribute.TaskType == 1) then
			local szTaskName = self:GetTaskName(tbTask.nTaskId);
			local szSubTaskName = self:GetManSubName(tbTask.nReferId);
			table.insert(tbMainTaskLogData, {szTaskName, szSubTaskName});
		end
	end

	return tbMainTaskLogData;
end


function Task:SharePickItem(pPlayer, pItem)
	local tbTeamMembers, nMemberCount	= KTeam.GetTeamMemberList(pPlayer.GetTeamId());
	if (not nMemberCount) then
		return;
	end

	for i = 1, nMemberCount do
		if (pPlayer.nPlayerIndex ~= tbTeamMembers[i].nPlayerIndex) then
			--if (Task:AtNearDistance(pPlayer, tbTeamMembers[i]) == 1) then
			if (pPlayer.dwSceneTemplateId == tbTeamMembers[i].dwSceneTemplateId) then
				Task:GetShareItem(tbTeamMembers[i], {pItem.nGenre, pItem.nDetail, pItem.nParticular, pItem.nLevel});
			end
		end
	end
end

function Task:GetShareItem(pPlayer, tbItem)
	Setting:SetGlobalObj(pPlayer)
	if (TaskCond:CanAddCountItemIntoBag(tbItem, 1)) then
		for _, tbTask in pairs(Task:GetPlayerTask(pPlayer).tbTasks) do
			if (tbTask:IsItemToBeCollect(tbItem) == 1) then
				Task:AddItem(pPlayer, tbItem, tbTask.nTaskId);
				Setting:RestoreGlobalObj()
				return 1;
			end
		end
	end

	Setting:RestoreGlobalObj()
end

function Task:CanAcceptRepeatTask()
	local nAcceptCount = me.pTask.GetTask(self.TASK_GROUP_REPEAT, 1);
	if (nAcceptCount >= self.nRepeatTaskAcceptMaxTime) then
		return 0;
	end

	return 1;
end

function Task:GetTaskType(nTaskId)
	local nTaskId = tonumber(nTaskId);
	if (nTaskId) then
		for ni, tbTask in pairs(self.tbTaskTypes) do
			if nTaskId >= tbTask.nFirstId and nTaskId <= tbTask.nLastId then
				return tbTask.szTaskType;
			end
		end
	end

	return "Task";
end

function Task:GetTaskTypeName(nTaskId)
	local nTaskId = tonumber(nTaskId);
	if (nTaskId) then
		for ni, tbTask in pairs(self.tbTaskTypes) do
			if nTaskId >= tbTask.nFirstId and nTaskId <= tbTask.nLastId then
				return tbTask.szTaskName;
			end
		end
	end

	return Lang.task.str99[Lang.Idx];
end

--============= 任务目标自动寻路 ====================

-- 从指定语句当中获取
function Task:GetInfoFromSentence(szSource, szFormat)
	if (not szSource or not szFormat) then
		return;
	end
	local tbInfo = {};
	local s = 1;
	local e = 1;
	s, e = string.find(szSource, szFormat, 1);
	while (s and e and s ~= e) do
		local szSub = string.sub(szSource, s, e);
		s, e = string.find(szSource, szFormat, s + 1);
		table.insert(tbInfo, szSub);
	end
	return tbInfo;
end

-- 分析单条语句，返回改语句中的坐标信息
function Task:ParseSingleInfo(szDesc)
	if (not szDesc) then
		return;
	end
	local szFormat = "%<n?p?c?pos.-%>";				-- 模式匹配字符串，匹配"<" 和">" 之间的字符串
	local tbInfo = self:GetInfoFromSentence(szDesc, szFormat);
	if (not tbInfo or #tbInfo == 0) then
		return;
	end
	return tbInfo;
end

-- 获取一个任务当中某个步骤的的坐标信息
function Task:GetPosInfo(nTaskId, szTaskName, nCurStep)
	if (not szTaskName or not nCurStep or not nTaskId) then
		return;
	end
	if (not self.tbReferDatas or Lib:CountTB(self.tbReferDatas) <= 0) then
		return;
	end
	for _, tbInfo in pairs(self.tbReferDatas) do
		if (tbInfo.szName == szTaskName and tbInfo.tbDesc and
			tbInfo.tbStepsDesc and tbInfo.nTaskId == nTaskId and
			tbInfo.tbStepsDesc[nCurStep]) then
			local szCurStepInfo = tbInfo.tbStepsDesc[nCurStep];
			return self:ParseSingleInfo(szCurStepInfo);
		end
	end
end

-- 寻找关键字，例如传入参数是"<pos=红娘,5,1633,2941>"
-- 返回值就是"红娘"
function Task:FindKeyWord(szInfo)
	if (not szInfo) then
		return;
	end
	local s, e = string.find(szInfo, "=");
	if (not s or not e) then
		return;
	end
	local nBegin = e + 1;
	s, e = string.find(szInfo, ",", nBegin);
	if (not s or not e) then
		return;
	end
	local szKeyWord = string.sub(szInfo, nBegin, s - 1);
	if (not szKeyWord) then
		return;
	end

	return szKeyWord;
end

-- 在没有找到匹配的时候进行替换
-- szSource "与白秋琳对话"
-- szReplace "<npcpos=秋姨,X,X>"
-- 结果 "<npcpos=与白秋琳对话,X,X>"
function Task:ReplaceWhileNoMatch(szSource, szReplace)
	if (not szSource or not szReplace) then
		return;
	end
	local szKeyWord = self:FindKeyWord(szReplace);
	if (not szKeyWord) then
		return;
	end
	return string.gsub(szReplace, szKeyWord, szSource);
end

function Task:GetFinalDesc(szDesc, tbPosInfo)
	if (not tbPosInfo) then
		return szDesc;
	end

	for i, v in pairs(tbPosInfo) do
		tbPosInfo[i] = self:ReplaceName_Link(v);
	end
	szDesc = string.gsub(szDesc, "秋姨", "白秋琳");

	local nPosCount = #tbPosInfo;
	if (tbPosInfo and #tbPosInfo > 0) then
		for _, szPosInfo in pairs(tbPosInfo) do
			local szKeyWord = Task:FindKeyWord(szPosInfo);
			local s, e = string.find(szDesc, szKeyWord);
			if (s and e and s ~= e) then
				szDesc = string.gsub(szDesc, szKeyWord, szPosInfo);
			elseif (nPosCount == 1) then
				szDesc = Task:ReplaceWhileNoMatch(szDesc, szPosInfo) or szDesc;
			end
		end
	end
	return szDesc;
end

-- 统一把任务描述和任务目标中超链接部分的“秋姨”全部替换为“白秋琳”
function Task:ReplaceName_Link(szSource)
	if (not szSource) then
		return;
	end
	local szFormat = "%<n?p?c?pos=秋姨.-%>";
	local s, e = string.find(szSource, szFormat);
	if (s and e) then
		local szDst = string.gsub(szSource, "秋姨", "白秋琳", 1);
		return szDst;
	end
	return szSource;
end

--===================================================
-- 需要满足一定条件才能选择的可选奖励

-- 判断一个任务是否是可选奖励有特殊条件的任务
function Task:IsSpeOptAward(nTaskId, nReferId, nIndex)
	if (not nTaskId or not nReferId or nTaskId <= 0 or nReferId <= 0 or not nIndex) then
		return 0;
	end

	local tbSpeOptInfo = self.tbSpeOptInfo or {};
	for _, tbInfo in pairs(tbSpeOptInfo) do
		if (tbInfo.nTaskId == nTaskId and tbInfo.nSubId == nReferId and
			tbInfo.nIndex == nIndex) then
			return 1;
		end
	end

	return 0;
end

-- 获取这个可选奖励对应内容
function Task:GetSpeOptInfo(nTaskId, nReferId, nIndex)
	if (not nTaskId or not nReferId or not nIndex) then
		return;
	end

	local nFaction = me.nFaction;
	local nRouteId = me.nRouteId;
	local nSeries = me.nSeries;
	if (not nFaction or not nRouteId or nFaction <= 0 or nRouteId <= 0) then
		return;
	end

	local tbRetInfo = {};
	local bFind = 0;
	local tbSpeOptInfo = self.tbSpeOptInfo or {};
	for _, tbInfo in pairs(tbSpeOptInfo) do
		if (tbInfo.nTaskId == nTaskId and tbInfo.nSubId == nReferId) then
			if (tbInfo.nTaskId == nTaskId and tbInfo.nSubId == nReferId and tbInfo.nIndex == nIndex and
				((tbInfo.nFaction ~= -1 and tbInfo.nFaction == nFaction) or tbInfo.nRoute == -1) and
				((tbInfo.nRoute ~= -1 and tbInfo.nRoute == nRouteId) or tbInfo.nRoute == -1) and
				((tbInfo.nSeries ~= -1 and tbInfo.nSeries == nSeries) or tbInfo.nSeries == -1) and
				tbInfo.GetSex() == me.GetSex()) then

					tbRetInfo = tbInfo;
					bFind = 1;
					break;

			end
		end

	end

	if (1 == bFind) then
		return tbRetInfo;
	else
		return;
	end
end

-- 读取配置文件中门派装备的信息
function Task:LoadSpeOptInfo()
	self.tbSpeOptInfo = {};
	local szSpeOptInfoFile = "setting/task/speoptaward.txt";
	local tbInfoSetting = Lib:LoadTabFile(szSpeOptInfoFile);
	if (not tbInfoSetting) then
		return;
	end

	for nRow, tbRowData in pairs(tbInfoSetting) do
		local nTaskId = tonumber(tbRowData["TaskId"]) or 0;
		local nSubId = tonumber(tbRowData["SubId"]) or 0;
		local nFaction = tonumber(tbRowData["Faction"]) or -1;
		local nRoute = tonumber(tbRowData["nRoute"]) or -1;
		local nSex = tonumber(tbRowData["Sex"]) or 0;
		local nIndex = tonumber(tbRowData["Index"]) or 0;
		local nCost = tonumber(tbRowData["Cost"]) or 0;
		local nSeries = tonumber(tbRowData["nSeries"]) or -1;
		local szCostGDPL = tostring(tbRowData["szCostGDPL"]) or "";
		local szGDPL = tostring(tbRowData["szGDPL"]) or "";
		local tbCostGDPL = Lib:SplitStr(szCostGDPL);
		local tbGDPL = Lib:SplitStr(szGDPL);

		local tbTemp = {};
		tbTemp.nTaskId = nTaskId;
		tbTemp.nSubId = nSubId;
		tbTemp.nFaction = nFaction;
		tbTemp.nRoute = nRoute;
		tbTemp.nSex = nSex;
		tbTemp.nIndex = nIndex;
		tbTemp.nCost = nCost;
		tbTemp.nSeries = nSeries;
		tbTemp.tbGDPL = {};
		tbTemp.tbCostGDPL = {};
		tbTemp.tbGDPL[1], tbTemp.tbGDPL[2], tbTemp.tbGDPL[3], tbTemp.tbGDPL[4] =
			tonumber(tbGDPL[1]), tonumber(tbGDPL[2]), tonumber(tbGDPL[3]) ,tonumber(tbGDPL[4]);
		tbTemp.tbCostGDPL[1], tbTemp.tbCostGDPL[2], tbTemp.tbCostGDPL[3], tbTemp.tbCostGDPL[4] =
			tonumber(tbCostGDPL[1]), tonumber(tbCostGDPL[2]), tonumber(tbCostGDPL[3]) ,tonumber(tbCostGDPL[4]);

		table.insert(self.tbSpeOptInfo, tbTemp);
	end
end

Task:LoadSpeOptInfo();

Task.TrapTaskInfo = {};

function Task:LoadTrapTask()
	local tbFileData = Lib:LoadTabFile("/setting/task/traptask.txt")
	for _, tbRow in pairs(tbFileData) do
		if not self.TrapTaskInfo[tbRow.TrapClass] then
			self.TrapTaskInfo[tbRow.TrapClass] = {}
		end
		local info = self.TrapTaskInfo[tbRow.TrapClass];
		info[tonumber(tbRow.TrapTaskId)] = {
		TaskId = tonumber(tbRow.TaskId),
		TaskStep = tonumber(tbRow.TaskStep),
		InstanceId = tonumber(tbRow.InstanceId)
		};
	end
end

Task:LoadTrapTask();

function Task:CheckTrapTask(szTrapClass, pPlayer)
	--[[
	local TrapTaskInfo = self.TrapTaskInfo[szTrapClass];
	if (TrapTaskInfo == nil) then
		return;
	end
	assert(TrapTaskInfo);
	for _, info in pairs(TrapTaskInfo) do
		if info.TaskId == 0 and info.TaskStep == 0 then
			Instance:TryEnterInstance({me.dwId}, info.InstanceId);
		end

		if Task:HasTask(me, info.TaskId, info.TaskStep) == 1 then
			Instance:TryEnterInstance({me.dwId}, info.InstanceId);
			break;
		end
	end ]]
	-- 改成弹出面板，从面板上点击进入
	print("Task:CheckTrapTask " .. szTrapClass);
	-- pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowInstanceTaskPanel", szTrapClass});
	self:ShowInstanceTaskPanel(pPlayer.dwId, szTrapClass);
end

function Task:ShowInstanceTaskPanel(nPlayerId, szTrapClass)
	if (self.bDataLoaded == 1) then
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if (pPlayer) then
			local nRemainCount = Instance:GetRemainInstanceCount(pPlayer);
			local nInstanceId = pPlayer.pTask.GetTask(Instance.CURRENT_SWEEP_TASK_GID, Instance.CURRENT_SWEEP_INSTANCE_ID) or 0;
			local nStartTime = pPlayer.pTask.GetTask(Instance.CURRENT_SWEEP_TASK_GID, Instance.CURRENT_SWEEP_START_TIME) or 0;
			local nTotalCount = pPlayer.pTask.GetTask(Instance.CURRENT_SWEEP_TASK_GID, Instance.CURRENT_SWEEP_COUNT) or 0;
			local nFinishCount = pPlayer.pTask.GetTask(Instance.CURRENT_SWEEP_TASK_GID, Instance.CURRENT_SWEEP_FINISH_COUNT) or 0;
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowInstanceTaskPanel", szTrapClass, Player.tbDegree:GetMaxDegree(pPlayer, "Instance"), nRemainCount, nInstanceId, nTotalCount, nStartTime, nFinishCount});
			print("Task:ShowInstanceTaskPanel call client script");
		end
	elseif (self.bDataLoaded == 2) then
		-- 玩家下线，则直接退出
		print("Task:ShowInstanceTaskPanel player logout");
	else
		-- 玩家任务数据还没加载，需要等待加载后再弹出面板
		Timer:Register(1 * Env.GAME_FPS, Task.ShowInstanceTaskPanel, self, nPlayerId, szTrapClass);
	end
	return 0;
end

-- 注册离线事件
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, Task.OnLogout, Task)
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Task.OnLogin, Task)
--PlayerEvent:RegisterGlobal("OnLogout", Task.OnLogout, Task)
