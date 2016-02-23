
local tb	= Task:GetTarget("KillBoss");
tb.szTargetName	= "KillBoss";

function tb:Init(nNpcTempId, nMapId, nNeedCount, szBeforePop, szLaterPop)
	self.nNpcTempId		= nNpcTempId;
	self.szNpcName		= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId			= nMapId;
	self.szMapName		= Task:GetMapName(nMapId);
	self.nNeedCount		= nNeedCount;
	self.szBeforePop	= szBeforePop;
	self.szLaterPop		= szLaterPop;
end;


function tb:Start()
	self.nCount		= 0;
end;


function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {
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
	return 1;
end;


function tb:IsDone()
	return self.nCount >= self.nNeedCount;
end;


function tb:GetDesc()
	local szMsg	= Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format("%s：%d/%d", self.szNpcName, self.nCount, self.nNeedCount);
	return szMsg;
end;


function tb:GetStaticDesc()
	local szMsg	=  Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format("%s%d个", self.szNpcName, self.nNeedCount);
	return szMsg;
end;


function tb:Close(szReason)

end;


function tb:OnKillNpc(pPlayer, pNpc)
	if (self:IsDone()) then
		return;
	end;
	
	if (self.nNpcTempId ~= pNpc.nTemplateId) then
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
