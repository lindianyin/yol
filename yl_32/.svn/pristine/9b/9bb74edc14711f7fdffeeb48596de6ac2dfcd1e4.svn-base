-- 杀掉指定Npc此任务完成,但是这个指定的Npc要通过对话才能Call出来
-- 一个队伍中任何接这个目标的人均可共享
-- Npc被杀后指定时间变为对话Npc
-- 注意：两个Npc都是不能复生的。

local tb = Task:GetTarget("DialogNpc2FightNpc");
tb.szTargetName		= "DialogNpc2FightNpc";


function tb:Init(nMapId, nMapPosX, nMapPosY, nMapPosZ, nDialogNpcTempId, szOption, szMsg, nFightNpcTempId, nFightNpcLevel, nDeathDuration,  szStaticDesc, szDynamicDesc, szBeforePop, szLaterPop)
	self.nMapId				= nMapId;
	self.szMapName			= Task:GetMapName(nMapId);
	self.nMapPosX			= nMapPosX;
	self.nMapPosY			= nMapPosY;
	self.nMapPosZ			= nMapPosZ;
	self.nDialogNpcTempId	= nDialogNpcTempId;
	self.szDialogNpcName	= KGameBase.GetNameByTemplateId(nDialogNpcTempId);
	self.szOption			= szOption;
	self.szMsg				= szMsg;
	self.nFightNpcTempId	= nFightNpcTempId;
	self.szFightNpcName		= KGameBase.GetNameByTemplateId(nFightNpcTempId);
	self.nFightNpcLevel		= nFightNpcLevel;
	self.nDeathDuration		= nDeathDuration;
	self.szStaticDesc		= szStaticDesc;
	self.szDynamicDesc		= szDynamicDesc;
	self.szBeforePop		= szBeforePop;
	self.szLaterPop			= szLaterPop;
	self:IniTarget();
end;


function tb:IniTarget()
	if (MODULE_GAMESERVER) then
		if (not self.bExist or self.bExist == 0) then
			local pDialogNpc = KGameBase.CallNpc(self.nDialogNpcTempId, self.nMapId, self.nMapPosX, self.nMapPosY, self.nMapPosZ);
			if (not pDialogNpc) then
				return;
			end
			self.nDialogNpcId = pDialogNpc.dwId;
			Timer:Register(Env.GAME_FPS * 60, self.OnCheckNpcExist, self);
			self.bExist = 1; -- 只添加一次
		end
		
	end
end;

-- 用于防止意外造成Npc丢失
function tb:OnCheckNpcExist()
	if (Task:IsNpcExist(self.nDialogNpcId, self) == 1) then
		return;
	end
	
	if (Task:IsNpcExist(self.nFightNpcId, self) == 1) then
		return;
	end
	
	if (self.nReviveDurationTimeId) then
		return;
	end
	
	print("TaskNpcMiss", self.nDialogNpcId, self.nFightNpcId, self.nReviveDurationTimeId);
	print(debug.traceback());
	
	self.nDialogNpcId = nil;
	self.nFightNpcId  = nil;
	
	if (MODULE_GAMESERVER) then
		self.nReviveDurationTimeId = Timer:Register(Env.GAME_FPS * self.nDeathDuration, self.AddDialogNpc, self);
	end;
end

--目标开启
function tb:Start()
	self.bDone	= 0;
	self:RegisterTalk();
end;


--目标保存
--根据任务变量组Id（nGroupId）以及组内变量起始Id（nStartTaskId），保存本目标的运行期数据
--返回实际存入的变量数量
function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);
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
	self.bDone	= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self:RegisterTalk();
	end;
	
	return 1;
end;


--返回目标是否达成
function tb:IsDone()
	return self.bDone == 1;
end;

--返回目标进行中的描述（客户端）
function tb:GetDesc()
	return self.szDynamicDesc or "";
end;


--返回目标的静态描述，与当前玩家进行的情况无关
function tb:GetStaticDesc()
	return self.szStaticDesc or "";
end;

function tb:Close(szReason)
	self:UnRegisterTalk(); -- 注销NPC对话
end;


function tb:RegisterTalk()
	self.tbTask:AddNpcMenu(self.nDialogNpcTempId, self.nMapId, self.szOption, self.OnTalkNpc, self);
end;


function tb:UnRegisterTalk()
	self.tbTask:RemoveNpcMenu(self.nDialogNpcTempId);
end;


function tb:OnTalkNpc()
	if (not him) then
		return;
	end;
	
	if (self.nDialogNpcId ~= him.dwId) then
		return;
	end
	
	local oldPlayer = me;
	me = self.me;
	TaskAct:Talk(self.szMsg, self.OnTalkFinish, self);
	me = oldPlayer;
end;


function tb:OnTalkFinish()
	assert(self._tbBase._tbBase)
	assert (MODULE_GAMESERVER);
	
	self:Dialog2Fight();
end;

function tb:Dialog2Fight()
	if (not self._tbBase.nDialogNpcId) then
		-- 被别人先领一步
		return;
	end
	
	-- 删除对话Npc
	local pDialogNpc = KGameBase.GetNpcById(self._tbBase.nDialogNpcId);
	assert(pDialogNpc);
	
	local nCurMapId, nCurPosX, nCurPosY, nCurPosZ = pDialogNpc.GetWorldPos();
	Task.tbToBeDelNpc[self._tbBase.nDialogNpcId] = 0;
	KGameBase.RemoveNpc(self._tbBase.nDialogNpcId);
	self._tbBase.nDialogNpcId = nil;

		
	--然后在指定位置（玩家位置）添加一个战斗Npc
	local pFightNpc	= KGameBase.CallNpc(self.nFightNpcTempId, nCurMapId, nCurPosX, nCurPosY, nCurPosZ);
	pFightNpc.SetLevel(self.nFightNpcLevel); -- todo simon
	self._tbBase.nFightNpcId = pFightNpc.dwId;
	Task.tbToBeDelNpc[self._tbBase.nFightNpcId] = 1;
	assert(pFightNpc);
	Npc:RegPNpcOnDeath(pFightNpc, self.OnFightDeath, self._tbBase);
end


-- 静态注册,当Npc死亡后会开始计时，到30秒后New新的Npc然后注销
function tb:RiseDialogNpc()
	assert(self._tbBase)
	assert(self._tbBase._tbBase == nil)
	if (MODULE_GAMESERVER) then
		self.nReviveDurationTimeId = Timer:Register(Env.GAME_FPS * self.nDeathDuration, self.AddDialogNpc, self);
		Timer.tbAttach[self.nReviveDurationTimeId] = 1;
	end;
end;



-- 添加一个对话Npc
function tb:AddDialogNpc()
	assert(not self.nDialogNpcId);
	assert(not self.nFightNpcId);
	assert(not self._tbBase._tbBase);
	assert(self._tbBase);
	-- 避免下面assert造成重复调用
	if (not self.nReviveDurationTimeId) then
		return 0;
	end
	Timer.tbAttach[self.nReviveDurationTimeId] = nil;
	self.nReviveDurationTimeId = nil;
	
	local pDialogNpc = KGameBase.CallNpc(self.nDialogNpcTempId, self.nMapId, self.nMapPosX, self.nMapPosY, self.nMapPosZ);
	assert(pDialogNpc);
	
	self.nDialogNpcId = pDialogNpc.dwId;
	Task.tbToBeDelNpc[self.nDialogNpcId] = 1;
	return 0;
end


-- 战斗Npc死亡，一段时间后会生出对话Npc
function tb:OnFightDeath()
	assert(self._tbBase);
	assert(not self._tbBase._tbBase);
	
	if (self.nFightNpcId and him.dwId == self.nFightNpcId) then
		self.nFightNpcId = nil;
		self:RiseDialogNpc();
	else
		assert(false);
	end
end;


function tb:OnKillNpc(pPlayer, pNpc)
	if (self:IsDone()) then
		return;
	end;
	
	if (pNpc.dwId ~= self._tbBase.nFightNpcId) then
		return;
	end
	
	self.bDone	=  1;
	Task.tbToBeDelNpc[self._tbBase.nFightNpcId] = nil;
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self:UnRegisterTalk();
		self.tbTask:OnFinishOneTag();
	end;
end
