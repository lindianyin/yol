-- by KK
-- quiz测验，类似answerthequestion回答问题，不同的是，这个更严格, 可以再其它模块（如活动）使用
-- 题目选项每次经过打乱，客户端没有保存答案，不能作弊
local tb = Task:GetTarget("DoQuiz")
tb.szTargetName = "DoQuiz"

--
function tb:Init(nNpcTempId, nMapId, szOption, nQuizId, nErrorLimit, szPreMsg, szEndMsg, szDesc)
	-- 读表属性
	self.nNpcTempId = nNpcTempId
	self.nMapId = nMapId
	self.szOption = szOption
	self.nQuizId = nQuizId
	self.nErrorLimit = nErrorLimit
	self.szEndMsg = szEndMsg
	self.szPreMsg = szPreMsg
	self.szDesc = szDesc
end

function tb:Start()
	self.bDone = 0;
	self:Register()
end
-------------------------------------------存盘
function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);
	return 1;
end

function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self:Register();
	end;
	return 1;
end

------------------------npc对话菜单
function tb:Register()
	self.tbTask:AddNpcMenu(self.nNpcTempId, self.nMapId, self.szOption, self.OnTalkNpc, self);
end
function tb:UnRegister()
	self.tbTask:RemoveNpcMenu(self.nNpcTempId);
end

---------------------------------描述
function tb:GetDesc()
	print("tb:GetDesc()", self.szDesc)
	return self.szDesc or "";
end;
function tb:GetStaticDesc()
  print("tb:GetStaticDesc()", self.szDesc)
	return self.szDesc or "";
end;
function tb:Close(szReason)
	self:UnRegister();
end;

function tb:OnFinishPreDialog()
    self:StartQuiz()
end

--------------------------------npc对话处理
function tb:OnTalkNpc()
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.dwSceneTemplateId) then
		TaskAct:Talk(string.format(Lang.task.target.str5[Lang.Idx],self.szNpcName,self.szMapName))
		return;
	end;
	print("tb:OnTalkNpc", self.bFinishedPreDialog, self.szPreMsg, self.bFinishedEndDialog,self.szEndMsg)
	TaskAct:Talk(self.szPreMsg, self.OnFinishPreDialog, self);
	--[[if (self:IsDone()) then
		if (self.bFinishedEndDialog == 0) then
			TaskAct:Talk(self.szEndMsg);
			self.bFinishedEndDialog = 1;
			return;
		end
	end;--]]
end

-- 弹出做题面板
function tb:StartQuiz()
	self.nErrorCount = 0  -- 本次做题错误次数统计
	self.me.pQuiz.CallQuiz(self.nQuizId) -- 做题
end

-- 是否完成
function tb:IsDone()
	return self.bDone == 1
end

function tb:OnShowEndDialog()
    TaskAct:Talk(self.szEndMsg, self.OnFinishEndDialog, self);
    return 0;
end

function tb:OnFinishEndDialog()
	self:DoRightChoiceHandle()
end

-- C++调过来的，选择题目后响应
function tb:OnQuizChoice(pPlayer, bRight)
	if (bRight == 1) then
		-- 题目做对, 设置任务完成
		self.bDone = 1
		Timer:Register(Env.GAME_FPS * 2, self.OnShowEndDialog, self) -- 3秒后显示结束对话
	else
		-- 题目做错
		--self.me.SysMsg("题目做错")
		self.nErrorCount = self.nErrorCount + 1  -- 错误次数统计+1
	end

	-- 错误次数已经达到最大错误上限
	if (self.nErrorCount >= self.nErrorLimit) then
		self.me.pQuiz.SendClientResult(0, 1)  -- 告诉客户端答题错误，并关闭面板！
		self.me.SysMsg(Lang.task.target.str23[Lang.Idx])
	end
end

-- 答对后的处理，TODO:延迟2秒后发送完成
function tb:DoRightChoiceHandle()
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	if (not self.szRepeatMsg) then
		self:UnRegister()	-- 本目标是一旦达成后不会失效的
	end;

	self.tbTask:OnFinishOneTag();
end
