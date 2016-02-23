
local tb	= Task:GetTarget("SayNpc");
tb.szTargetName	= "SayNpc";

function tb:Init(nNpcTempId, nMapId, szOption, szMsg, szBeforePop, szLaterPop)
	self.nNpcTempId	= nNpcTempId;
	self.szNpcName	= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId		= nMapId;
	self.szMapName	= Task:GetMapName(nMapId);
	self.szOption	= szOption;
	self.tbSayContent = self:ParseSayContent(szMsg);
	
	self.szBeforePop	= szBeforePop;
	self.szLaterPop		= szLaterPop;
end;

function tb:ParseSayContent(szAllMsg)
	local tbMsg	= Lib:SplitStr(szAllMsg or "", "<end>");
	table.remove(tbMsg, #tbMsg); -- 最后一项会为""
	return tbMsg;
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
	
	if (self.bDone ~= 1) then
		self:Register();
	end
	
	return 1;
end;

function tb:IsDone()
	return self.bDone == 1;
end;

function tb:GetDesc()
	return self:GetStaticDesc();
end;

function tb:GetStaticDesc()
	local szMsg	= Lang.task.target.str25[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format(Lang.task.target.str26[Lang.Idx], self.szNpcName);
	return szMsg;
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
		TaskAct:Talk(string.format(Lang.task.target.str36[Lang.Idx],self.szNpcName,self.szMapName))
		return;
	end;
	if (self:IsDone()) then
		return;
	end;
	
	self:StartSay();
end;


function tb:StartSay()
	self:ShowMessage(1);
end;

function tb:ShowMessage(nIdx)
	local szMsg = self.tbSayContent[nIdx];
	szMsg = Lib:ParseExpression(szMsg);
	szMsg = Task:ParseTag(szMsg);
	if (nIdx < #self.tbSayContent) then -- 若还有
		Dialog:Say(szMsg,
			{
				{Lang.task.target.str51[Lang.Idx], tb.OnSelect, self, nIdx},
			});
	else
		Dialog:Say(szMsg,
			{
				{Lang.task.target.str52[Lang.Idx], tb.OnSelect, self, nIdx},
			});
	end
end;

		

function tb:OnSelect(nIdx)
	if (nIdx < #self.tbSayContent) then
		self:ShowMessage(nIdx + 1);
		return;
	end
	
	self.bDone	= 1;
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	
	
	if (not self.szRepeatMsg) then
		self:UnRegister()	-- 本目标是一旦达成后不会失效的
	end;

	self.tbTask:OnFinishOneTag();
	
	return;
end;



