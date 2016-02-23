
local tb	= Task:GetTarget("ProtectNpc");
tb.szTargetName		= "ProtectNpc";
tb.REFRESH_FRAME 	= 16;
--tb.CHIEFSKILLID		= 100; -- 护送负责人的技能Id,目标关闭的时候关闭，开始的时候加载

--参数说明
--对话Npc模板Id, 刷对话Npc时间间隔,对话选项,对话内容,行走Npc模板Id,行走Npc等级,地图Id,玩家能离开的最远距离,目标点x坐标,目标点y坐标,目标半径,行走路线,完成前泡泡，完成后泡泡
function tb:Init(nDialogNpcTempId, szOption, szMsg, nMoveNpcTempId, nMapId, nAimMapId, nAimX, nAimY, nAimR, nViewScene, szBeforePop, strRszLaterPop, nAttartRate)
	self.nDialogNpcTempId	= nDialogNpcTempId;
	--self.nMapPosX			= nMapPosX;
	--self.nMapPosY			= nMapPosY;
	--self.nMapPosZ			= nMapPosZ;
	--self.nInterval			= nInterval;
	self.szDialogNpcName	= KGameBase.GetNameByTemplateId(nDialogNpcTempId);
	self.szOption			= szOption;
	self.szMsg				= szMsg;
	self.nMoveNpcTempId		= nMoveNpcTempId;
	self.szMoveNpcName		= KGameBase.GetNameByTemplateId(nMoveNpcTempId)
	--self.nMoveNpcLevel		= nMoveNpcLevel;
	self.nMapId				= nMapId;
	self.nAimMapId			= nAimMapId;
	self.szMapName			= Task:GetMapName(nMapId);
	--self.nMaxDistance		= nMaxDistance;
	--self.nLimitTime			= nLimitTime;
	self.nAimX				= nAimX;
	self.nAimY				= nAimY;
	self.nAimZ				= 0;
	self.nAimR				= nAimR;
	--self.tbTrack			= self:ParseTrack(szTrack);
	self.szBeforePop		= szBeforePop;
	self.szLaterPop			= szLaterPop;
	--self.nAttartRate 		= tonumber(nAttartRate) or 20;
	--self.nFightNpcAIType	= tonumber(nAIType)
	--self:IniTarget();
end;

-- 解析行走字符串
function tb:ParseTrack(szTrack)
	if (not szTrack or szTrack == "") then
		return;
	end
	local tbRet = {};
	local tbTrack = Lib:SplitStr(szTrack, "\n")
	for i=1, #tbTrack do
		if (tbTrack[i] and tbTrack[i] ~= "") then
			local tbPos = Lib:SplitStr(tbTrack[i]);
			table.insert(tbRet, tbPos);
		end
	end
	
	return tbRet;
end


function tb:IniTarget()
	if (MODULE_GAMESERVER) then
		if (not self.bStart) then
			local pDialogNpc = KGameBase.CallNpc(self.nDialogNpcTempId, self.nMapId, self.nMapPosX, self.nMapPosY, self.nMapPosZ);
			if (not pDialogNpc) then
				return;
			end
			self.nDialogNpcId = pDialogNpc.dwId;
			Timer:Register(Env.GAME_FPS * 60, self.OnCheckNpcExist, self);
			self.bStart = true;
		end
	end
end;

-- 用于防止意外造成Npc丢失
function tb:OnCheckNpcExist()
	if (Task:IsNpcExist(self.nDialogNpcId, self) == 1) then
		return;
	end
	
	if (self.nReviveDurationTimeId) then
		return;
	end
	
	self.nDialogNpcId = nil;
	self.nReviveDurationTimeId = nil;
	
	self:AddDialogNpc();
end

-- 开始的时候
function tb:Start()
	self.bDone = 0;
	--self._tbBase:IniTarget();
	self:RegisterTalk();
	self.bHaveProtecedNpc = 0;
end;

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);
	self.me.pTask.SetTask(nGroupId, nStartTaskId + 1, self.bHaveProtecedNpc);
	return 1;
end;

function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone	= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	self.bHaveProtecedNpc = self.me.pTask.GetTask(nGroupId, nStartTaskId + 1);
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		--self._tbBase:IniTarget(); 该为摆怪，不需要动态加载
		self:RegisterTalk();
	end;
	
	if (self.bHaveProtecedNpc == 1) then
		self:Reg_Npc_RunTimer();
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
	return Lang.task.target.str48[Lang.Idx]..self.szMoveNpcName;
end;


function tb:Close(szReason)
	self:UnReg_Npc_RunTimer();
	self:UnRegisterTalk();
	
	if (self.nMoveNpcId) then
		KGameBase.RemoveNpc(self.nMoveNpcId);
	end
end;


-- 注册和Npc对话
function tb:RegisterTalk()
	self.tbTask:AddNpcMenu(self.nDialogNpcTempId, self.nMapId, self.szOption, self.OnTalkNpc, self);
end;

function tb:UnRegisterTalk()
	self.tbTask:RemoveNpcMenu(self.nDialogNpcTempId);
end;

function tb:OnTalkNpc()
	if (not him) then
		return;
	end;
	
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.dwSceneTemplateId) then
		TaskAct:Talk(string.format(Lang.task.target.str49[Lang.Idx],self.szDialogNpcName,self.szMapName))
		return;
	end;
	local oldPlayer = me;
	me = self.me;
	TaskAct:Talk(self.szMsg, self.OnTalkFinish, self);
	me = oldPlayer;
	
	self.nMyDialogNpcId = him.dwId;
end;

-- 对话完后会删除当前对话Npc,在指定地方刷一个行走Npc,并让他行走，并注册计时器，指定时间后再刷一个对话Npc
function tb:OnTalkFinish()
	--[[if (self.nMyDialogNpcId ~= self._tbBase.nDialogNpcId) then
		return;
	end
	--]]
	-- 对话转战斗
	
	self:Dialog2Fight();	 
	
	--注册计时器到时间删Npc，每秒看玩家离Npc的距离，若距离远则删除 
	self:Reg_Npc_RunTimer();
	
	-- 注册指定时间后刷对话Npc
	--self._tbBase:RiseDialogNpc();
end;


function tb:Dialog2Fight()
	--local pFightNpc	= KGameBase.CallNpc(self.nMoveNpcTempId, self.nMapId, nCurPosX, nCurPosY, nCurPosZ);
	
	local tbSaveTask	= self.tbSaveTask;
	self.bHaveProtecedNpc = 1;
	self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId + 1, self.bHaveProtecedNpc, 1);
	
	me.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:StartProtoctedNpc",});
	--assert(pFightNpc); 
	--pFightNpc.SetLevel(self.nMoveNpcLevel);
	--pFightNpc.SetCurrentCamp(1) -- TODO camp常量定义
	
	
	
	--local szTitle = "由<color=yellow>"..self.me.szName.."<color>的队伍护送";
	--pFightNpc.SetTitle(szTitle); -- todo simon
	--Npc:RegPNpcOnDeath(pFightNpc, self.OnDeath, self);
	--self.nMoveNpcId = pFightNpc.dwId;
	
	--pFightNpc.SetAIType(self.nFightNpcAIType)
	
	--pFightNpc.AI_ClearPath(); -- todo simon
	--for _,Pos in ipairs(self.tbTrack) do
	--	if (Pos[1] and Pos[2]) then
	--		pFightNpc.AI_AddMovePos(tonumber(Pos[1])*32, tonumber(Pos[2])*32)
	--	end
	--end 
	
	--pFightNpc.AI_AddMovePos(tonumber(self.nAimX)*32, tonumber(self.nAimY)*32);-- 终点为目标
	--pFightNpc.SetNpcAI(9, self.nAttartRate, 1,-1, 25, 25, 25, 0, 0, 0, self.me.GetNpc().nIndex);	
end

function tb:RiseDialogNpc()
	assert(self._tbBase)
	assert(not self._tbBase._tbBase)
	if (MODULE_GAMESERVER) then
		self.nReviveDurationTimeId = Timer:Register(Env.GAME_FPS * self.nInterval, self.AddDialogNpc, self);
	end;
end

-- 添加一个对话Npc
function tb:AddDialogNpc()
	assert(self._tbBase);
	assert(not self._tbBase._tbBase);
	assert(not self.nDialogNpcId);
	local pDialogNpc = KGameBase.CallNpc(self.nDialogNpcTempId, self.nMapId, self.nMapPosX, self.nMapPosY, self.nMapPosZ);
	assert(pDialogNpc)
	
	self.nDialogNpcId = pDialogNpc.dwId;
	self.nReviveDurationTimeId = nil;
	return 0;
end


function tb:Reg_Npc_RunTimer()
	self.nRunElapseTime = 0;
	if (MODULE_GAMESERVER and not self.nRegisterRunTimerId) then
		self.nRegisterRunTimerId = Timer:Register(self.REFRESH_FRAME, self.OnRunTimer, self);
	end;
end;

function tb:UnReg_Npc_RunTimer()
	if (MODULE_GAMESERVER and self.nRegisterRunTimerId) then
		Timer:Close(self.nRegisterRunTimerId);
		self.nRegisterRunTimerId	= nil;
	end;
end;

-- 1.到指定时间删战斗Npc；2.每秒看玩家离Npc的距离，若距离远则删常 3.目标是否完成(和玩家走))
function tb:OnRunTimer()
	self.nRunElapseTime = self.nRunElapseTime + 1;

		local oldme = me;
		me = self.me;
		-- 判断到达目的地
		if (TaskCond:IsMeAtPos(me.dwId, self.nAimMapId, self.nAimX, self.nAimY, self.nAimZ, self.nAimR)) then
			me.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:EndProtoctedNpc",});
			self.bDone	=  1;
			self:ShareProtectNpc();
			local tbSaveTask	= self.tbSaveTask;
			if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
				self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
				if (tbSaveTask.nGroupId == Escort.ESCORT_GROUP_ID) then
					--nFinishedCount = nFinishedCount + 1;
				end 
				KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
			end;
			--KGameBase.RemoveNpc(self.nMoveNpcId);
			self.nMoveNpcId = nil;
			self:UnReg_Npc_RunTimer();
			self.nRunElapseTime = 0;
		end;
		me = oldme;
		
		if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
			--
			self:UnReg_Npc_RunTimer();
			self:UnRegisterTalk();
			self.tbTask:OnFinishOneTag();
			self.nRunElapseTime = 0;
			self.nRegisterRunTimerId	= nil;
			return 0
		end;
	--end;
	
	return self.REFRESH_FRAME;
end;


function tb:ShareProtectNpc()
-- 遍历所有队友所有任务的当前步骤的目标，若是和此目标相同则调用OnTeamMateKillNpc
	local tbTeamMembers, nMemberCount	= KTeam.GetTeamMemberList(self.me.GetTeamId());
	if (not tbTeamMembers) then --共享失败：没有组队
		return;
	end
	if (nMemberCount <= 0) then-- 共享失败：队伍没有成员
		return;
	end
	
	local nOldPlayerIndex = self.me.nPlayerIndex;
	for i = 1, nMemberCount do
		local pPlayer	= tbTeamMembers[i];
		if (pPlayer.nPlayerIndex ~= nOldPlayerIndex) then
			if (Task:AtNearDistance(pPlayer, self.me) == 1) then
				for _, tbTask in pairs(Task:GetPlayerTask(pPlayer).tbTasks) do
					
					for _, tbCurTag in pairs(tbTask.tbCurTags) do
						
						if (tbCurTag.szTargetName == self.szTargetName) then
							if (tbCurTag.nMoveNpcTempId == self.nMoveNpcTempId and
								(tbCurTag.nMapId == 0 or tbCurTag.nMapId == self.nMapId)) then
								tbCurTag:OnTeamMateProtectNpc();
							end
						end
					end
				end
			end
		end
	end
end;


function tb:OnTeamMateProtectNpc()
	self.bDone  = 1;
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		--
		self:UnReg_Npc_RunTimer();
		self:UnRegisterTalk();
		self.tbTask:OnFinishOneTag();
	end;
end;


function tb:OnDeath()
	if (self:IsDone()) then
		return;
	end
	self.nMoveNpcId = nil;
	self.me.SysMsg(self.szMoveNpcName..Lang.task.target.str50[Lang.Idx]);
end
