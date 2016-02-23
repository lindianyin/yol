-- 一旦指定任务变量改变为指定值触发此目标达成

local tb	= Task:GetTarget("RequireTaskValue");
tb.szTargetName	= "RequireTaskValue";
tb.REFRESH_FRAME	= 18;	-- 一秒检测一次

function tb:Init(nTaskGroupId, nTaskId, nStartTaskValue, nNeedTaskValue, szStaticDesc, szDynamicDesc)
	self.nTaskGroupId 			= nTaskGroupId;
	self.nTaskId						= nTaskId;
	self.nStartTaskValue		= nStartTaskValue;
	self.nNeedTaskValue			= nNeedTaskValue;
	self.szStaticDesc				= szStaticDesc;
	self.szDynamicDesc			= szDynamicDesc;
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
	if (self:IsDone()) then
		return 1;
	end;
	
	local nTaskValue = self.me.pTask.GetTask(self.nTaskGroupId, self.nTaskId);
	if (nTaskValue == self.nNeedTaskValue) then
		self:OnTaskValueChange();
		return 1;
	end;
		
	self:Register();
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

function tb:Register()
	assert(self._tbBase._tbBase)	--没有经过两次派生，脚本书写错误
	local oldPlayer = me;
	me = self.me;
	if (MODULE_GAMESERVER and not self.nRegisterId) then
		if (self.nTaskGroupId == 1022 and self.nTaskId == 119) then
			local nOldTask = self.me.pTask.GetTask(self.nTaskGroupId, self.nTaskId);
			if (nOldTask == 1) then
				print("Task Value Set Error");
				print(debug.traceback());
			end
		end
		
		self.me.pTask.SetTask(self.nTaskGroupId, self.nTaskId, self.nStartTaskValue, 1);
		self.nRegisterId	= Timer:Register(self.REFRESH_FRAME, self.OnTaskValueChange, self);
	end;
	me = oldPlayer;
end;

function tb:UnRegister()
	assert(self._tbBase._tbBase)	--没有经过两次派生，脚本书写错误
	local oldPlayer = me;
	me = self.me;
	if (MODULE_GAMESERVER and self.nRegisterId) then
		Timer:Close(self.nRegisterId);
		self.nRegisterId	= nil;
	end;
	me = oldPlayer;
end;


function tb:OnTaskValueChange()
	local nTaskValue = self.me.pTask.GetTask(self.nTaskGroupId, self.nTaskId);
	if (nTaskValue == self.nNeedTaskValue) then
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

