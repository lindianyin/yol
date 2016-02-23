
local tb	= Task:GetTarget("KillNpcCount");
tb.szTargetName	= "KillNpcCount";

function tb:Init(nNpcTempId, nMapId, nNeedCount, nTaskValueId, szBeforePop, szLaterPop)
	self.nNpcTempId		= nNpcTempId;
	self.szNpcName		= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId			= nMapId;
	self.szMapName		= Task:GetMapName(nMapId);
	self.nNeedCount		= nNeedCount;
	self.nTaskValueId	= nTaskValueId;
	self.szBeforePop	= szBeforePop;
	self.szLaterPop		= szLaterPop;
end;

function tb:Start()
	self.nCount		= 0;
end;

function tb:Save(nGroupId, nStartTaskId)
	return 0	-- 随时保存的，此时不需要再保存
end;

function tb:Load(nGroupId, nStartTaskId)
	self.nCount	= self.me.pTask.GetTask(nGroupId, self.nTaskValueId);
	return 0	-- 有特定的保存位置，不进行统一读取
end;

function tb:IsDone()
	return self.nCount >= self.nNeedCount;
end;

function tb:GetDesc()
	local szMsg	= Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format("%s：%d", self.szNpcName, self.nCount);
	if (self.nNeedCount ~= 0) then
		szMsg	= szMsg.."/"..self.nNeedCount;
	end;
	return szMsg;
end;

function tb:GetStaticDesc()
	local szMsg	= Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format("%s", self.szNpcName);
	if (self.nNeedCount ~= 0) then
		szMsg	= szMsg..string.format(Lang.task.target.str40[Lang.Idx], self.nNeedCount);
	end;
	return szMsg;
end;


function tb:Close(szReason)

end;


-- pPlayer 为杀死NPC的玩家
function tb:OnKillNpc(pPlayer, pNpc)
	if (self.nNpcTempId ~= pNpc.nTemplateId) then
		return;
	end;
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.GetMapId()) then
		return;
	end;
	self.nCount	= self.nCount + 1;
	if (MODULE_GAMESERVER) then	-- 自行同步到客户端，要求客户端刷新
		local tbTask	= self.tbTask;
		self.me.pTask.SetTask(tbTask.nSaveGroup, self.nTaskValueId, self.nCount, 1);
		KTask.SendRefresh(self.me, tbTask.nTaskId, tbTask.nReferId, tbTask.nSaveGroup);
	end;
	
	if (self:IsDone()) then
		self.tbTask:OnFinishOneTag();
	end
end;
