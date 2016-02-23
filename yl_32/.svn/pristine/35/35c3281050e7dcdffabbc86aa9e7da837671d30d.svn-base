
local tb	= Task:GetTarget("EndEscort");
tb.szTargetName	= "EndEscort";

function tb:Init(nNpcTempId, nMapId, szOption, szMsg, szRepeatMsg,  szStaticDesc, szDynamicDesc, szBeforePop, szLaterPop)
	self.nNpcTempId	= nNpcTempId;
	self.szNpcName	= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId		= nMapId;
	self.szMapName	= Task:GetMapName(nMapId);
	self.szOption	= szOption;
	self.szMsg		= szMsg;
	if (szRepeatMsg ~= "") then
		self.szRepeatMsg	= szRepeatMsg;
	end;
	
	self.szStaticDesc	= szStaticDesc;
	self.szDynamicDesc	= szDynamicDesc;
	self.szBeforePop	= szBeforePop;
	self.szLaterPop		= szLaterPop;
end;

function tb:Start()
	self.bDone		= 0;
	self:Register();
end;

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);
	return 1;
end;

function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	if (not self:IsDone() or self.szRepeatMsg) then	-- 本目标是一旦达成后不会失效的
		self:Register();
	end;
	return 1;
end;

function tb:IsDone()
	return self.bDone == 1;
end;

function tb:GetDesc()
	return self.szDynamicDesc or "";
end;

function tb:GetStaticDesc()
	return self.szStaticDesc or "";
end;


function tb:Close(szReason)
	self:UnRegister();
end;


function tb:Register()
	self.tbTask:AddNpcMenu(self.nNpcTempId, self.nMapId, self.szOption, self.OnTalkNpc, self);
end;

function tb:UnRegister()
	self.tbTask:RemoveNpcMenu(self.nNpcTempId);
end;

function tb:OnTalkNpc()
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.GetMapId()) then
		TaskAct:Talk(string.fomat(Lang.task.target.str5[Lang.Idx],self.szNpcName,self.szMapName))
		return;
	end;
	if (self:IsDone()) then
		if (self.szRepeatMsg) then
			TaskAct:Talk(self.szRepeatMsg);
		end;
		return;
	end;
	
	-- 移除云雾效果
	local tbSaveTask	= self.tbSaveTask;
	if Escort.tbTaskId[self.tbTask.nTaskId] ~= nil then
		self.me.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:RemoveEscortClouds"});
		Escort:ResetPlayerSpeed(self.me.dwId);
		self.me.SetReceivedEscortTaskId(0);
		Escort.tbIsEscort[self.me.dwId] = 0;	--结束修仙
	end
	
	if self.szMsg == "" then
		self:OnTalkFinish() -- 没填对话内容，直接完成
	else
		TaskAct:Talk(self.szMsg, self.OnTalkFinish, self);
	end
end;

function tb:OnTalkFinish()
	self.bDone	= 1;
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	if (not self.szRepeatMsg) then
		self:UnRegister()	-- 本目标是一旦达成后不会失效的
	end;
	
	-- 完成处理
	if Escort.tbTaskId[self.tbTask.nTaskId] ~= nil then
		--奖励汇总
		Escort:EscortAwardCalc(self.me, Escort.tbTaskId[self.tbTask.nTaskId]);
		--弹出完成面板
		Escort:ShowFinishPanel(self.me);
		self.me.SetCurEscortTaskId(0);
	end
	
	self.tbTask:OnFinishOneTag();

	if (self.OnTalkFinishCallback) then
		self:OnTalkFinishCallback()
	end
end;

