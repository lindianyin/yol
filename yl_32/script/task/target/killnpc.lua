
local tb	= Task:GetTarget("KillNpc");
tb.szTargetName	= "KillNpc";	--本目标的名字，暂时还没用到

--目标的执行流程：
--	1、Init			系统启动LoadSub时生成此目标类的第一次派生，并针对每一个派生调用Init
--	2、Start/Load	需要激活一个目标时，系统生成此类的第二次派生，并调用Start（刚进入此步骤）或Load（玩家上线时载入旧目标）
--	3、Close		目标需要终止时，系统会调用Close
--	4、系统销毁此目标的第二次派生

--此外：
--	Save、GetDesc	在目标被激活期间（步骤2之后，步骤3之前），会根据情况不定期调用
--	GetStaticDesc	在目标被初始化之后（步骤1之后），会根据情况不定期调用

----==== 以下一段是所有目标都必须定义的几个函数 ====----

--目标初始化
--具体参数可在任务编辑器中自定义
function tb:Init(nNpcTempId, nMapId, nNeedCount, szBeforePop, szLaterPop)
	self.nNpcTempId		= nNpcTempId;
	self.szNpcName		= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId			= nMapId;
	self.szMapName		= Task:GetMapName(nMapId);
	self.nNeedCount		= nNeedCount;
	self.szBeforePop	= szBeforePop;
	self.szLaterPop		= szLaterPop;
end;

--目标开启
function tb:Start()
	self.nCount		= 0;
end;

--目标保存
--根据任务变量组Id（nGroupId）以及组内变量起始Id（nStartTaskId），保存本目标的运行期数据
--返回实际存入的变量数量
function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.nCount);
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
	self.nCount		= self.me.pTask.GetTask(nGroupId, nStartTaskId);

	return 1;
end;

--返回目标是否达成
function tb:IsDone()
	return self.nCount >= self.nNeedCount;
end;

--返回目标进行中的描述（客户端）
function tb:GetDesc()
	local szMsg	= Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format("%s：%d/%d", self.szNpcName, self.nCount, self.nNeedCount);
	return szMsg;
end;

--返回目标的静态描述，与当前玩家进行的情况无关
function tb:GetStaticDesc()
	local szMsg	=  Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName.. Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format("%s %d个", self.szNpcName, self.nNeedCount);
	return szMsg;
end;

--目标活动停止
--	szReason，停止的原因：
--		"logout"	玩家下线
--		"finish"	步骤完成
--		"giveup"	玩家放弃任务
--		"failed"	任务失败
--		"refresh"	客户端刷新
function tb:Close(szReason)
	local oldPlayer = me;
	me = self.me;
	me = oldPlayer;
end;

----==== 以下是本目标所特有的函数定义，如有雷同，纯属巧合 ====----
function tb:OnKillNpc(pPlayer, pNpc)
	if (self:IsDone()) then
		return;
	end;
	if (self.nNpcTempId ~= pNpc.dwTemplateId) then
		return;
	end;
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.dwSceneTemplateId) then
		return;
	end;
	self.nCount	= self.nCount + 1;
	
	
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.nCount, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		--
		self.tbTask:OnFinishOneTag();
	end;
	
end;
