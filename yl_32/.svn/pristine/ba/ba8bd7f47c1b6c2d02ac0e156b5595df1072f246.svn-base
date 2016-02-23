

local tb = Task:GetTarget("GuardNpc");
tb.szTargetName		= "GuardNpc";


function tb:Init(nMapId, nMapPosX, nMapPosY, nMapPosZ, nDialogNpcTempId, szOption, 
	szMsg, nFightNpcTempId, nFightNpcLevel, nFightNpcHP, nGuardDuration, 
	nDeathDuration, szStaticDesc, szDynamicDesc, szFailedMsg, szBeforePop, szLaterPop)
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
	self.nFightNpcHP		= nFightNpcHP;
	self.nFightNpcLevel		= nFightNpcLevel;
	self.nGuardDuration		= nGuardDuration;
	self.nDeathDuration		= nDeathDuration;
	self.szStaticDesc		= szStaticDesc;
	self.szDynamicDesc		= szDynamicDesc;
	self.szFailedMsg		= szFailedMsg;
	self.szBeforePop		= szBeforePop;
	self.szLaterPop			= szLaterPop;
	
	self:IniTarget();
end;


-- 初始化目标在读取任务数据的时候执行
function tb:IniTarget()
	if (MODULE_GAMESERVER) then
		if (not self.bExist or self.bExist == 0) then
			-- 添加对话Npc
			local pDialogNpc = KGameBase.CallNpc(self.nDialogNpcTempId, self.nMapId, self.nMapPosX, self.nMapPosY, self.nMapPosZ);
			if (not pDialogNpc) then
				return;
			end
			self.nDialogNpcId = pDialogNpc.dwId;
			Timer:Register(Env.GAME_FPS * 6, self.OnCheckNpcExist, self);
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
	
	self.nDialogNpcId = nil;
	self.nFightNpcId  = nil;
	self.nReviveDurationTimeId = nil;
	self:AddDialogNpc();
end


-- 玩家进入步骤
function tb:Start()
	self.bDone	= 0;
	self:RegisterTalk();
end;


-- 保存玩家任务数据
function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);	-- 是否已经完成
	self.me.pTask.SetTask(nGroupId, nStartTaskId + 1, 0);			-- 过去的时间
	
	return 2;
end;


--目标载入
--根据任务变量组Id（nGroupId）以及组内变量起始Id（nStartTaskId），载入本目标的运行期数据
--返回实际载入的变量数量
function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	if (MODULE_GAMECLIENT) then
		local nCostTime			= self.me.pTask.GetTask(nGroupId, nStartTaskId + 1);
		self.nClientRestTime	= self.nGuardDuration - nCostTime;
	else
		self.me.pTask.SetTask(nGroupId, nStartTaskId + 1, 0, 1);
	end
	self.bDone	= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self:RegisterTalk();
	end;
	
	return 2;
end;


--返回目标是否达成
function tb:IsDone()
	assert(self._tbBase._tbBase);
	return self.bDone == 1;
end;


--返回目标进行中的描述（客户端）
function tb:GetDesc()
	assert(self._tbBase._tbBase);
	local szDesc = string.format(self.szDynamicDesc, self.nGuardDuration);
	if (MODULE_GAMECLIENT) then
		if (self.nClientRestTime and (not self:IsDone() and self.nClientRestTime ~= self.nGuardDuration)) then
			szDesc = szDesc.."："..self.nClientRestTime;
		end
	end
	
	return szDesc;
end;


--返回目标的静态描述，与当前玩家进行的情况无关
function tb:GetStaticDesc()
	assert(self._tbBase._tbBase);
	return self.szStaticDesc or "";
end;


function tb:Close(szReason)
	assert(self._tbBase._tbBase);
	self:UnRegisterTimer();
	self:UnRegisterTalk(); -- 注销NPC对话
	if (MODULE_GAMESERVER) then
		if (self.nRegisterLogoutId) then
			self:OnLogout();
		end
	end
	self:UnRegisterLogout();
end;


function tb:RegisterTalk()
	assert(self._tbBase._tbBase);
	self.tbTask:AddNpcMenu(self.nDialogNpcTempId, self.nMapId, self.szOption, self.OnTalkNpc, self);
end;

function tb:UnRegisterTalk()
	assert(self._tbBase._tbBase);
	self.tbTask:RemoveNpcMenu(self.nDialogNpcTempId);
end;

function tb:OnTalkNpc()
	if (self:IsDone()) then
		return;
	end
	assert(self._tbBase._tbBase);
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.GetMapId()) then
		return;
	end;
	TaskAct:Talk(self.szMsg, self.OnTalkFinish, self);
end;

function tb:OnTalkFinish()
	assert(self._tbBase._tbBase)
	assert (MODULE_GAMESERVER);
	if (self._tbBase.nDialogNpcId) then
		self:Dialog2Fight();
	end
end;

function tb:Dialog2Fight()
	assert(MODULE_GAMESERVER);
	assert(self._tbBase.nDialogNpcId);
	local pDialogNpc = KGameBase.GetNpcById(self._tbBase.nDialogNpcId);
	assert(pDialogNpc);

	local nCurMapId, nCurPosX, nCurPosY, nCurPosZ = pDialogNpc.GetWorldPos();
	KGameBase.RemoveNpc(self._tbBase.nDialogNpcId);
	self._tbBase.nDialogNpcId = nil;
	
	self:AddFightNpc(nCurMapId, nCurPosX, nCurPosY, nCurPosZ);
end

function tb:AddFightNpc(nMapId, nMapX, nMapY, nMapZ)
	local pFightNpc	= KGameBase.CallNpc(self.nFightNpcTempId, nMapId, nMapX, nMapY, nMapZ);
	assert(pFightNpc);
	pFightNpc.SetLevel(self.nFightNpcLevel); -- todo simon
	local szTitle = string.format(Lang.task.target.str32[Lang.Idx],self.me.szName);
	pFightNpc.SetTitle(szTitle);
	pFightNpc.SetCurCamp(0);
	
	if (self.nFightNpcHP > pFightNpc.nMaxLife) then
		pFightNpc.SetMaxLife(self.nFightNpcHP);
		pFightNpc.RestoreLife();
	end
	
	self._tbBase.nFightNpcId = pFightNpc.dwId;
	Npc:RegPNpcOnDeath(pFightNpc, self.OnDeath, self);
	
	self:RegisterLogout(); -- 接任务的下线则删除此Npc
	self:RegisterTimer();
end

function tb:RegisterLogout()
	assert(self._tbBase._tbBase);
	if (MODULE_GAMESERVER) then
		if (not self.nRegisterLogoutId) then
			self.nRegisterLogoutId = true;
		end;
	end;
end

function tb:UnRegisterLogout()
	assert(self._tbBase._tbBase);
	if (MODULE_GAMESERVER) then
		if (self.nRegisterLogoutId) then
			self.nRegisterLogoutId	= false;
		end;
	end;
end

function tb:OnLogout()
	assert(self._tbBase._tbBase);
	--assert(self._tbBase.nFightNpcId);
	--改成保护 zounan
	if not self._tbBase.nFightNpcId then
		return;
	end
	
	local pFightNpc = KGameBase.GetNpcById(self._tbBase.nFightNpcId);
	assert(pFightNpc);
	KGameBase.RemoveNpc(self._tbBase.nFightNpcId);
	self._tbBase.nFightNpcId = nil;
	self._tbBase:RiseDialogNpc();	
end



function tb:RegisterTimer()
	assert(self._tbBase._tbBase)	--没有经过两次派生，脚本书写错误
	if (MODULE_GAMESERVER and not self.nRegisterGuardTimerId) then
		self.nElapse = 0;
		self.nRegisterGuardTimerId	= Timer:Register(Env.GAME_FPS, self.OnGuardTimer, self);
	end;
end;

function tb:UnRegisterTimer()
	assert(self._tbBase._tbBase)	--没有经过两次派生，脚本书写错误
	if (MODULE_GAMESERVER and self.nRegisterGuardTimerId) then
		Timer:Close(self.nRegisterGuardTimerId);
		self.nRegisterGuardTimerId	= nil;
	end;
end;


-- 同一时间只能有一个人守卫这个Npc
function tb:OnGuardTimer()
	assert(MODULE_GAMESERVER);
	assert(self._tbBase._tbBase);
	assert(not self:IsDone());

	if (not self.nElapse) then
		self.nElapse = 0;
	end
	
	local pFightNpc = KGameBase.GetNpcById(self._tbBase.nFightNpcId);
	assert(pFightNpc);
	self.nElapse = self.nElapse + 1;
	local tbSaveTask	= self.tbSaveTask;
	if (tbSaveTask) then
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId + 1, self.nElapse, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end
	
	self:ShareTimer(self.nElapse);
	
	if (self.nElapse <  self.nGuardDuration) then
		return;
	end
	
	KGameBase.RemoveNpc(self._tbBase.nFightNpcId);
	self._tbBase.nFightNpcId = nil;
	
	self:ShareGuardNpc();
	self.bDone	=  1;
	
	if (tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	
	self:UnRegisterTimer();
	self:UnRegisterTalk();
	self:UnRegisterLogout()
	self.tbTask:OnFinishOneTag();
		
	self._tbBase:RiseDialogNpc(); -- 指定时间刷对话Npc
	return 0;
end;

function tb:ShareGuardNpc()
	assert(self._tbBase._tbBase);
	-- 遍历所有队友所有任务的当前步骤的目标，若是和此目标相同则调用OnTeamMateKillNpc
	local tbTeamMembers, nMemberCount	= KTeam.GetTeamMemberList(self.me.GetTeamId());
	if (not tbTeamMembers) then --共享失败：没有组队
		return;
	end
	if (nMemberCount <= 0) then-- 共享失败：队伍没有成员
		return;
	end
	
	local plOld	= self.me;
	local nOldPlayerIndex = self.me.nPlayerIndex;
	for i = 1, nMemberCount do
		me	= tbTeamMembers[i];
		if (me.nPlayerIndex ~= nOldPlayerIndex) then
			if (Task:AtNearDistance(me, self.me) == 1) then
				for _, tbTask in pairs(Task:GetPlayerTask(me).tbTasks) do
					for _, tbCurTag in pairs(tbTask.tbCurTags) do
						if (tbCurTag.szTargetName == self.szTargetName and (not tbCurTag:IsDone())) then
							if (tbCurTag.nFightNpcTempId == self.nFightNpcTempId and
								(tbCurTag.nMapId == 0 or tbCurTag.nMapId == self.nMapId)) then
								tbCurTag:OnTeamMateGuardNpc();
							end
						end
					end
				end
			end
		end
	end
	self.me	= plOld;
end;

function tb:OnTeamMateGuardNpc()
	assert(self._tbBase._tbBase);
	if (self:IsDone()) then
		return;
	end
	self.bDone  = 1;
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		
		self:UnRegisterTalk();
		self:UnRegisterLogout();
		self.tbTask:OnFinishOneTag();
	end;
end;


function tb:RiseDialogNpc()
	assert(self._tbBase)
	assert(self._tbBase._tbBase == nil)
	if (MODULE_GAMESERVER) then
		self.nReviveDurationTimeId = Timer:Register(Env.GAME_FPS * self.nDeathDuration, self.AddDialogNpc, self);
	end;
end;

function tb:AddDialogNpc()
	assert(not self.nDialogNpcId);
	assert(not self.nFightNpcId);
	assert(not self._tbBase._tbBase);
	assert(self._tbBase);
	
	local pDialogNpc = KGameBase.CallNpc(self.nDialogNpcTempId, self.nMapId, self.nMapPosX, self.nMapPosY, self.nMapPosZ);
	assert(pDialogNpc);
	
	self.nDialogNpcId = pDialogNpc.dwId;
	self.nReviveDurationTimeId = nil;
	return 0;
end


function tb:OnDeath()
	assert(self._tbBase._tbBase)
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER) then
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId + 1, 0, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end
	self:ShareTimer(0);
	self.me.SysMsg(self.szFailedMsg);
	self._tbBase.nFightNpcId = nil;
	self:UnRegisterTimer();
	self._tbBase:RiseDialogNpc();
end;


function tb:ShareTimer(nElapse)
	-- 遍历所有队友所有任务的当前步骤的目标，若是和此目标相同则调用OnTeamMateKillNpc
	local tbTeamMembers, nMemberCount	= KTeam.GetTeamMemberList(self.me.GetTeamId());
	if (not tbTeamMembers) then --共享失败：没有组队
		return;
	end
	if (nMemberCount <= 0) then-- 共享失败：队伍没有成员
		return;
	end
	
	local plOld	= self.me;
	local nOldPlayerIndex = self.me.nPlayerIndex;
	for i = 1, nMemberCount do
		me	= tbTeamMembers[i];
		if (me.nPlayerIndex ~= nOldPlayerIndex) then
			for _, tbTask in pairs(Task:GetPlayerTask(me).tbTasks) do
				if (tbTask.nReferId == self.tbTask.nReferId) then
					for _, tbCurTag in pairs(tbTask.tbCurTags) do
						if (tbCurTag.szTargetName == self.szTargetName and (not tbCurTag:IsDone())) then
							if (tbCurTag.nFightNpcTempId == self.nFightNpcTempId and (tbCurTag.nMapId == 0 or tbCurTag.nMapId == self.nMapId)) then
								if (Task:AtNearDistance(me, self.me) == 1) then
									tbCurTag:OnUpDateTeamMateTimer(nElapse);
								else
									tbCurTag:OnUpDateTeamMateTimer(0);
								end
							end
						end
					end
				end
			end
			
		end
	end
	
	self.me	= plOld;
end

function tb:OnUpDateTeamMateTimer(nElapse)	
	if (self:IsDone()) then
		return;
	end
	
	self.nElapse = nElapse;
	
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId + 1, self.nElapse, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
end
