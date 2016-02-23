

-- 文件载入的时候可以在Task.tbTargetLib中载入这个目标
local tb	= Task:GetTarget("AddObj");
tb.szTargetName	= "AddObj";

function tb:Init(tbItemId, nNpcTempId, szNewName, nMapId, nPosX, nPosY, nPosZ, nR, szPosName, szMsg)
	if (tbItemId[1] ~= 20) then
		print("[Task Error]"..self.szTargetName.. Lang.task.target.str1[Lang.Idx])
	end
	self.tbItemId	= tbItemId;
	self.szItemName	= KItem.GetNameById(unpack(tbItemId));
	self.nParticular= tbItemId[3];
	self.nNpcTempId	= nNpcTempId;
	assert(nNpcTempId > 0);
	self.szNpcName	= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.szNewName	= szNewName;
	self.nMapId		= nMapId;
	self.nPosX		= nPosX;
	self.nPosY		= nPosY;
	self.nPosZ		= nPosZ;
	self.nR			= nR;
	self.szPosName	= szPosName;
	if (self.szMsg ~= "") then
		self.szMsg	= szMsg;
	end;
end;

function tb:Start()
	self.bDone		= 0;
	self:Register();
	if (MODULE_GAMESERVER) then	-- 服务端看情况添加物品
		if (Task:GetItemCount(self.me, self.tbItemId) <= 0) then
			Task:AddItem(self.me, self.tbItemId);
		end
	end
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

function tb:IsDone()
	return self.bDone == 1;
end;

function tb:GetDesc()
	return self:GetStaticDesc();
end;

function tb:GetStaticDesc()
	return string.format(Lang.task.target.str2[Lang.Idx], self.szNpcName, self.szPosName);
end;

function tb:Close(szReason)
	self:UnRegister();
	if (MODULE_GAMESERVER) then	-- 服务端看情况删除物品，完成的话在完成瞬间删
		if (szReason == "giveup" or szReason == "failed") then
			Task:DelItem(self.me, self.tbItemId, 1);
		end;
	end;
end;


function tb:Register()
	self.tbTask:AddItemUse(self.nParticular, self.OnItem, self)
end;

function tb:UnRegister()
	self.tbTask:RemoveItemUse(self.nParticular);
end;

function tb:OnItem()
	if (self:IsDone()) then
		return nil;
	end;
	
	local oldme = me;
	me = self.me;
	local bAtPos, szMsg	= TaskCond:IsAtPos(self.nMapId, self.nPosX, self.nPosY, self.nPosZ, self.nR);
	if (not bAtPos) then
		self.me.SysMsg(szMsg);
		me = oldme;
		return 1;
	end;
	
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
	local oldme = me;
	me = self.me;
	local pNpc = TaskAct:AddObj(self.nNpcTempId, self.szNewName);
	me = oldme;
	
	Timer:Register(2 * 60 *Env.GAME_FPS, self.DelNpc, self, pNpc.dwId);
	Task:DelItem(self.me, self.tbItemId);
	
	self.bDone	= 1;
	
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	self:UnRegister()	-- 本目标是一旦达成后不会失效的
	if (self.szMsg) then
		local oldPlayer = me;
		me = self.me;
		me.SysMsg(self.szMsg);
		me = oldPlayer;
	end;
	
	self.tbTask:OnFinishOneTag();
	 
	return 0;
end


function tb:DelNpc(nNpcId)
	KGameBase.RemoveNpc(nNpcId);
	return 0;
end
