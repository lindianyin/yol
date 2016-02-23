-------------------------------------------------------
-- 文件名　：finishmission.lua
-- 文件描述：需要完成指定副本多少次
-- 创建者　：Chenpeilin / KK
-- 创建时间：2013-04-02 09:28:22
-------------------------------------------------------
local tb = Task:GetTarget("FinishMission")

tb.szTargetName = "FinishMission"

-- dialognpc, number, number, txt, txt
function tb:Init(nNpcTempId, nMissionId, nNeedCount, szStaticDesc, szDynamicDesc)
	self.nNpcTempId	= nNpcTempId;  -- 这里填dialognpc, 仅仅用来自动寻路寻去副本
	self.nMissionId = nMissionId
	self.nNeedCount = nNeedCount
	self.szStaticDesc = szStaticDesc or szDynamicDesc
	self.szDynamicDesc = szDynamicDesc
end

--目标开启
function tb:Start()
	self.bDone		= 0;
	self.nFinishCount = 0;  -- 任务完成计数，通过OnTaskFinish事件更新 by KK
	
	--self:Register();
end;

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone, 1);
	-- 保存目标人物完成次数
	self.me.pTask.SetTask(nGroupId, nStartTaskId + 1, self.nFinishCount, 1);
	return 1;
end;

function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	-- 读取目标人物完成次数
	self.nFinishCount = self.me.pTask.GetTask(nGroupId, nStartTaskId + 1);
	
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		-- self:Register();
	end;
	
	
	return 1;
end;

function tb:IsDone()
	return self.bDone == 1;
end;

function tb:GetDesc()
	local szMsg = "";
	if (not self:IsDone()) then
		szMsg	= string.format("%s：%d/%d", self.szDynamicDesc, self.nFinishCount, self.nNeedCount);
	else
		szMsg 	= self.szDynamicDesc .. Lang.task.target.str22[Lang.Idx];
	end;
	return szMsg;
end;

function tb:GetStaticDesc()
	return self.szStaticDesc;
end;

function tb:OnFinishMission(pPlayer, nMissionId)
	print("OnFinishMission 收到完成任务事件！！！！！")
	if (nMissionId == self.nMissionId) then
		self.nFinishCount = self.nFinishCount + 1
		
		if (self.nFinishCount >= self.nNeedCount) then
			self.bDone = 1
		end
		
		local tbSaveTask	= self.tbSaveTask;
		if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
			self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
			self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId + 1, self.nFinishCount, 1);
			KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
		end

		if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
			self.tbTask:OnFinishOneTag();
		end
	end
end

function tb:Close(szReason)
	
end;
