
local tb	= Task:GetTarget("CastSkillAtAreaWithDesc");
tb.szTargetName	= "CastSkillAtAreaWithDesc";

function tb:Init(tbItemId, nSkillId, nMapId, nPosX, nPosY, nPosZ, nR, szDynamicDesc, szStaticDesc, szExtraPos)
	if (tbItemId[1] ~= 20) then
		--print("[Task Error]"..self.szTargetName.."  没有使用任务道具！")
	end
	self.tbItemId		= tbItemId;
	self.szItemName		= KItem.GetNameById(unpack(tbItemId));
	self.nSkillId		= nSkillId;
	self.nParticular 	= tbItemId[3];
	self.tbPos			= self:ParsePos(szExtraPos);
	table.insert(self.tbPos, {nMapId, nPosX, nPosY, nPosZ, nR});
	self.szDynamicDesc  = szDynamicDesc;
	self.szStaticDesc	= szStaticDesc;
end;


function tb:ParsePos(szPosSet)
	local tbRet = {};
	local tbPos = Lib:SplitStr(szPosSet, "\n")
	for i=1, #tbPos do
		if (tbPos[i] and tbPos[i] ~= "") then
			-- 坐标的格式：{nMapId, nPosX, nPosY, nR}
			local tbTemp = loadstring(string.gsub(tbPos[i],"(.+),(.+),(.+),(.+)", "return {tonumber(%1),tonumber(%2),tonumber(%3),tonumber(%4)}"))()
			table.insert(tbRet, tbTemp);
		end
	end
	return tbRet;
end;


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
--根据任务变量组Id（nGroupId）以及组内变量起始Id（nStartTaskId），保存本目标的运行期数据
--返回实际存入的变量数量
function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);
	return 1;
end;

--目标载入
--根据任务变量组Id（nGroupId）以及组内变量起始Id（nStartTaskId），载入本目标的运行期数据
--返回实际载入的变量数量
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
	if (not self.szDynamicDesc) then
		return self:GetStaticDesc();
	else
		return self.szDynamicDesc;
	end
end;


--返回目标的静态描述，与当前玩家进行的情况无关
function tb:GetStaticDesc()
	if (not self.szStaticDesc) then
		return Lang.task.target.str15[Lang.Idx]..self.szItemName;
	else
		return self.szStaticDesc;
	end
end;


--目标活动停止
--	szReason，停止的原因：
--		"logout"	玩家下线
--		"finish"	步骤完成
--		"giveup"	玩家放弃任务
--		"failed"	任务失败
--		"refresh"	客户端刷新
function tb:Close(szReason)
	self:UnRegister();
	if (MODULE_GAMESERVER) then	-- 服务端看情况删除物品，完成的话在完成瞬间删
		if (szReason == "giveup" or szReason == "failed") then
			Task:DelItem(self.me, self.tbItemId, 1);
		end;
	end;
end;

----==== 以下是本目标所特有的函数定义，如有雷同，纯属巧合 ====----

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
	for _, Pos in pairs(self.tbPos) do
		if (TaskCond:IsAtPos(unpack(Pos))) then
			bIsAtPos = 1;
			break;
		end
	end
	
	if (bIsAtPos ~= 1) then
		Dialog:SendInfoBoardMsg(me, Lang.task.target.str14[Lang.Idx]);
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
self.me.CastSkill(self.nSkillId, 1, -1, self.me.GetNpc().nIndex);
	
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
