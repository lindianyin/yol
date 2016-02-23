--- 进度条目标
local tb	= Task:GetTarget("WithProcessAddNpc");
tb.szTargetName	= "WithProcessAddNpc";
tb.REFRESH_FRAME	= 18;

function tb:Init(nNpcTempId, nMapId, nMapX, nMapY, nMapZ, nIntervalTime, nFightNpcId, nFightNpcLevel, szPos, nNeedCount, szDynamicDesc, szStaticDesc)
	self.nNpcTempId			= nNpcTempId;
	self.szNpcName			= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId				= nMapId;
	self.szMapName			= Task:GetMapName(nMapId);
	self.nMapX				= nMapX;
	self.nMapY				= nMapY;
	self.nMapZ				= nMapZ;
	self.nIntervalTime		= nIntervalTime * self.REFRESH_FRAME;
	
	self.szProcessInfo		= Lang.task.target.str33[Lang.Idx];
	self.szSucMsg			= Lang.task.target.str34[Lang.Idx];
	self.szFailedMsg		= Lang.task.target.str35[Lang.Idx];
	
	self.nFightNpcId		= nFightNpcId;
	self.nFightNpcLevel		= nFightNpcLevel;
	self.tbNpcPos			= self:ParsePos(szPos);
	self.nNeedCount			= nNeedCount;
	
	self.szDynamicDesc		= szDynamicDesc;
	self.szStaticDesc	  	= szStaticDesc;

	self:IniTarget();
end;

function tb:IniTarget()
	if (MODULE_GAMESERVER) then
		if (not self.bExist or self.bExist == 0) then
			local pProcessNpc = KGameBase.CallNpc(self.nNpcTempId, self.nMapId, self.nMapX, self.nMapY, self.nMapZ);
			if (not pProcessNpc) then
				return;
			end
			pProcessNpc.SetLevel(self.nFightNpcLevel);
			self.nProcessNpcId = pProcessNpc.dwId;
			Timer:Register(Env.GAME_FPS * 60, self.OnCheckNpcExist, self);
			self.bExist = 1; -- 只添加一次
		end
		
	end
end;

-- 用于防止意外造成Npc丢失
function tb:OnCheckNpcExist()
	if (Task:IsNpcExist(self.nProcessNpcId, self) == 1) then
		return;
	end
	
	if (self.nReviveDurationTimeId) then
		return;
	end
	
	print("TaskNpcMiss", self.nDialogNpcId, self.nReviveDurationTimeId);
	print(debug.traceback());
	
	self.nProcessNpcId = nil;
	
	if (MODULE_GAMESERVER) then
		self.nReviveDurationTimeId = Timer:Register(Env.GAME_FPS * self.nDeathDuration, self.AddProcessNpc, self);
	end;
end

-- 添加一个对话Npc
function tb:AddProcessNpc()
	assert(not self.nFightNpcId);
	
	-- 避免下面assert造成重复调用
	if (not self.nReviveDurationTimeId) then
		return 0;
	end

	self.nReviveDurationTimeId = nil;
	
	local pProcessNpc = KGameBase.CallNpc(self.nNpcTempId, self.nMapId, self.nMapX, self.nMapY, self.nMapZ);
	assert(pProcessNpc);
	
	pProcessNpc.SetLevel(self.nFightNpcLevel);
	self.nProcessNpcId = pProcessNpc.dwId;
	Task.tbToBeDelNpc[self.nProcessNpcId] = 1;
	return 0;
end

function tb:ParsePos(szPos)
	local tbRet = {};
	
	if (szPos and szPos ~= "") then
		local tbTrack = Lib:SplitStr(szPos, "\n")
		for i=1, #tbTrack do
			if (tbTrack[i] and tbTrack[i] ~= "") then
				tbRet[i] = {};
				local tbPos = Lib:SplitStr(tbTrack[i]);
				tbRet[i].nX = tonumber(tbPos[1]);
				tbRet[i].nY = tonumber(tbPos[2]);
			end
		end
	end
	
	return tbRet;
end

function tb:Start()
	self.nCount = 0;
	self:Register();
end;

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.nCount);
	
	return 1;
end;


function tb:Load(nGroupId, nStartTaskId)
		self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.nCount		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self:Register();
	end;
	
	return 1;
end;


function tb:IsDone()
	return self.nCount >= self.nNeedCount;
end;


function tb:GetDesc()
	local bHasTag = 0;
	local bTagStart, bTagEnd = string.find(self.szDynamicDesc, "%%d");
	if (bTagEnd) then
		bHasTag = bHasTag + 1;	
		bTagStart, bTagEnd = string.find(self.szDynamicDesc, "%%d", bTagEnd + 1);
		if (bTagEnd) then
			bHasTag = bHasTag + 1;
		end
	end
	
	if (bHasTag == 1) then
		return string.format(self.szDynamicDesc, self.nCount);
	elseif (bHasTag == 2) then
		return string.format(self.szDynamicDesc, self.nCount, self.nNeedCount);
	else
		return self.szDynamicDesc;
	end
end;


function tb:GetStaticDesc()
	return self.szStaticDesc;
end;




function tb:Close(szReason)
	self:UnRegister();
end;


function tb:Register()
	self.tbTask:AddExclusiveDialog(self.nNpcTempId, self.SelectOpenBox, self);
end;

function tb:UnRegister()
	self.tbTask:RemoveNpcExclusiveDialog(self.nNpcTempId);
end;


-- 玩家选择开
function tb:SelectOpenBox()
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.GetMapId()) then
		print(self.nMapId, self.me.GetMapId());
		self.me.SysMsg(string.format(Lang.task.target.str36[Lang.Idx],self.szNpcName,self.szMapName))
		return;
	end;

	if (self:IsDone()) then
		self.me.SysMsg(self.szFailedMsg)
		return;
	end;

	self.nCurTagIdx = him.dwId;

	Task:SetProgressTag(self, self.me);
	KTask.StartProgressTimer(self.me, self.nIntervalTime, self.szProcessInfo);
end;


function tb:OnProgressFull()
	self:AddFightNpc();
end;

function tb:AddFightNpc()	
	local tbNpcList = KGameBase.FindNpcWithTemplateId(self.nMapId, self.nFightNpcId);
	if (tbNpcList and #tbNpcList > self.nNeedCount * 2) then
		return; -- 最多存在2倍的
	end;
	
	for i = 1, #self.tbNpcPos do
		local pNpc = KGameBase.CallNpc(self.nFightNpcId, self.nMapId, self.tbNpcPos[i].nMapX, self.tbNpcPos[i].nMapY, self.tbNpcPos[i].nMapZ);
		if (not pNpc or pNpc.nIndex == 0) then
			print("[Task Error]:"..self.nNpcTempId..Lang.task.target.str68[Lang.Idx] );
		end;
		pNpc.SetLevel(self.nFightNpcLevel);
	end;
end;

function tb:OnKillNpc(pPlayer, pNpc)
	if (self:IsDone()) then
		return;
	end;
	if (self.nFightNpcId ~= pNpc.nTemplateId) then
		return;
	end;
	
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.GetMapId()) then
		return;
	end;
	
	self.nCount	= self.nCount + 1;
	
	
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.nCount, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		
		self.tbTask:OnFinishOneTag();
	end;
end;
