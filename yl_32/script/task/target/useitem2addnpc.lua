
local tb	= Task:GetTarget("UseItem2AddNpc");
tb.szTargetName	= "UseItem2AddNpc";

function tb:Init(tbItemId, nNpcTempId, nMapId, nPosX, nPosY, nPosZ, nR, szPos, nDelTime, szDynamicDesc, szStaticDesc)
	if (tbItemId[1] ~= 20) then
		print("[Task Error]"..self.szTargetName..Lang.task.target.str66[Lang.Idx] )
	end
	
	self.tbItemId		= tbItemId;
	self.szItemName		= KItem.GetNameById(unpack(tbItemId));
	self.nParticular 	= tbItemId[3];
	self.nMapId			= nMapId;
	self.nNpcTempId		= nNpcTempId;
	self.tbPos			= {nMapId, nPosX, nPosY, nPosZ, nR};
	self.tbNpcPos		= self:ParsePos(szPos);
	self.tbNpc 			= {};
	
	self.DEL_TIME		= nDelTime * 18; -- 秒
	
	self.szDynamicDesc  = szDynamicDesc;
	self.szStaticDesc	= szStaticDesc;
end;

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

--目标开启
function tb:Start()
	self.bDone		= 0;
	self:Register();
	if (MODULE_GAMESERVER) then	-- 服务端看情况添加物品
		if (Task:GetItemCount(self.me, self.tbItemId) <= 0) then
			Task:AddItem(self.me, self.tbItemId);
		end
	end
end;

--目标保存
function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);
	return 1;
end;

--目标载入
function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self:Register();
		if (MODULE_GAMESERVER) then	-- 服务端看情况添加物品
			if (Task:GetItemCount(self.me, self.tbItemId) <= 0) then
				Task:AddItem(self.me, self.tbItemId);
			end
		end
	end;
	return 1;
end;

--返回目标是否达成
function tb:IsDone()
	return self.bDone == 1;
end;

--返回目标进行中的描述（客户端）
function tb:GetDesc()
	return self:GetStaticDesc();
end;

--返回目标的静态描述，与当前玩家进行的情况无关
function tb:GetStaticDesc()
	return Lang.mission.str1205[Lang.Idx] .. self.szItemName;
end;

function tb:Close(szReason)
	self:UnRegister();
	if (MODULE_GAMESERVER) then	-- 服务端看情况删除物品，完成的话在完成瞬间删
		if (szReason == "giveup" or szReason == "failed") then
			Task:DelItem(self.me, self.tbItemId, 1);
		end;
	end;
end;

-------------------------------------------------------------------------------------------

function tb:Register()
	self.tbTask:AddItemUse(self.nParticular, self.OnTaskItem, self)
end;

function tb:UnRegister()
	self.tbTask:RemoveItemUse(self.nParticular);
end;

function tb:OnTaskItem()
	if (self:IsDone()) then
		return;
	end;
	local oldme = me;
	me = self.me;
	local bIsAtPos = 0;

	if (TaskCond:IsAtPos(unpack(self.tbPos))) then
		bIsAtPos = 1;
	end
	
	if (bIsAtPos ~= 1) then
		Dialog:SendInfoBoardMsg(me, Lang.task.target.str67[Lang.Idx]);
		me = oldme;
		return;
	end
	
	-- 开始进度条计时
	local tbEvent = {
		Player.ProcessBreakEvent.emEVENT_MOVE,
		Player.ProcessBreakEvent.emEVENT_ATTACK,
		Player.ProcessBreakEvent.emEVENT_SIT,
		Player.ProcessBreakEvent.emEVENT_USEITEM,
		Player.ProcessBreakEvent.emEVENT_ARRANGEITEM,
		Player.ProcessBreakEvent.emEVENT_DROPITEM,
		Player.ProcessBreakEvent.emEVENT_SENDMAIL,		
		Player.ProcessBreakEvent.emEVENT_TRADE,
		Player.ProcessBreakEvent.emEVENT_CHANGEFIGHTSTATE,
		Player.ProcessBreakEvent.emEVENT_CLIENTCOMMAND,
		Player.ProcessBreakEvent.emEVENT_ATTACKED,
		Player.ProcessBreakEvent.emEVENT_LOGOUT,
		Player.ProcessBreakEvent.emEVENT_DEATH,
	}
	
	GeneralProcess:StartProcess("", 180, {self.OnProgressFull, self}, nil, tbEvent)
	me = oldme;
end;

function tb:OnProgressFull()
	self:AddNpcInPos();
	Timer:Register(self.DEL_TIME, self.OnTimer, self);
	
	-- 删物品
	Task:DelItem(self.me, self.tbItemId, 1);
	
	self.bDone = 1;
	
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

-- 添加NPC
function tb:AddNpcInPos()
	for i = 1, #self.tbNpcPos do
		local pNpc = KGameBase.CallNpc(self.nNpcTempId, self.nMapId, self.tbNpcPos[i].nX, self.tbNpcPos[i].nY, self.tbNpcPos[i].nZ);
		if (not pNpc or pNpc.nIndex == 0) then
			print("[Task Error]:"..self.nNpcTempId..Lang.task.target.str68[Lang.Idx]);
		else
			self.tbNpc[#self.tbNpc + 1] = pNpc.dwId;
		end;
	end;
end;

-- 删除NPC
function tb:OnTimer()
	for i = 1, #self.tbNpc do
		KGameBase.RemoveNpc(self.tbNpc[i]);
	end;
	return 0;
end;
