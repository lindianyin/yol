-- 完成任务指引, 监听任务变量
local tb	= Task:GetTarget("FinishNoviceGuide");
tb.szTargetName	= "FinishNoviceGuide";
tb.REFRESH_FRAME	= 18;	-- 一秒检测一次

-- 客户端新手指引id， 任务变量group， 任务变量id，  任务变量完成值
function tb:Init(nNoviceGuideId, nTaskGroupId, nTaskVarId, nEndTaskValue, szStaticDesc, szDynamicDesc)
	self.nNoviceGuideId		= nNoviceGuideId;
	self.nTaskGroupId		= nTaskGroupId;
	self.nTaskVarId			= nTaskVarId;
	self.nEndTaskValue		= nEndTaskValue;
	self.szStaticDesc		= szStaticDesc;
	self.szDynamicDesc		= szDynamicDesc;
end;


--目标开启
function tb:Start()
	self.bDone		= 0;
	self:Register();
end;

--目标保存
--根据任务变量组Id（nGroupId）以及组内变量起始Id（nStartTaskId），保存本目标的运行期数据
--返回实际存入的变量数量
function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone, 1);
	
	return 1;
end;

--目标载入
--根据任务变量组Id（nGroupId）以及组内变量起始Id（nStartTaskId），载入本目标的运行期数据
--返回实际载入的变量数量
function tb:Load(nGroupId, nStartTaskId)	
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	
	if (self.bDone ~= 1) then
		self:Register();
	end
	
	return 1;
end;

--返回目标是否达成
function tb:IsDone()
	return self.bDone == 1;
end;

--返回目标进行中的描述（客户端）
function tb:GetDesc()
	return self.szDynamicDesc;
end;


--返回目标的静态描述，与当前玩家进行的情况无关
function tb:GetStaticDesc()
	return self.szStaticDesc;
end;


function tb:Close(szReason)
	self:UnRegister();
end;
-- Timer
function tb:Register()
	self:UnRegister(); -- 确保没有残留timer
	if (MODULE_GAMESERVER and not self.nRegisterId) then
		self.nRegisterId	= Timer:Register(self.REFRESH_FRAME, self.OnTimer, self);
	end;
end;

function tb:UnRegister()
	if (MODULE_GAMESERVER and self.nRegisterId) then
		Timer:Close(self.nRegisterId);
		self.nRegisterId	= nil;
	end;
end;


function tb:OnTimer()
	if (self.me.pTask == nil) then
		--print("finishnoviceguide OnTimer: pTask is nil!! error")
		self:UnRegister()
		return;
	end
	
    local tmpEndVal = self.me.pTask.GetTask(self.nTaskGroupId, self.nTaskVarId);
	if (self.me.pTask.GetTask(self.nTaskGroupId, self.nTaskVarId) >= self.nEndTaskValue) then
		self.bDone = 1;
		local tbSaveTask	= self.tbSaveTask;
		if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
			self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
			KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
		end;
	
		self:UnRegister()	-- 本目标是一旦达成后不会失效的
	
		self.tbTask:OnFinishOneTag();
	end
end


