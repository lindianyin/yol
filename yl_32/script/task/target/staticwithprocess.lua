
-- 静态进度条目标
local tb	= Task:GetTarget("WithProcessStatic");
tb.szTargetName	= "WithProcessStatic";


function tb:Init(nNpcTempId, nMapId, nIntervalTime, szProcessInfo, szSucMsg, szFailedMsg,  tbItem, nNeedCount, szDynamicDesc, szStaticDesc, szBeforePop, szLaterPop)
	self.nNpcTempId			= nNpcTempId;
	self.szNpcName			= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId				= nMapId;
	self.szMapName			= Task:GetMapName(nMapId);
	self.nIntervalTime 		= tonumber(nIntervalTime) * Env.GAME_FPS;
	self.szProcessInfo		= szProcessInfo or Lang.task.target.str33[Lang.Idx];
	self.szSucMsg			= szSucMsg or Lang.task.target.str35[Lang.Idx]
	self.szFailedMsg		= szFailedMsg or  Lang.task.target.str34[Lang.Idx]
--	self.ItemList			= self:ParseItem(tbItem);
	self.nNeedCount			= nNeedCount;
	self.szDynamicDesc		= szDynamicDesc;
	self.szStaticDesc	  	= szStaticDesc;
	self.szBeforePop		= szBeforePop;
	self.szLaterPop			= szLaterPop;
end;

function tb:ParseItem(szItemSet)
	local tbRet = {};
	local tbItem = Lib:SplitStr(szItemSet, "\n")
	for i=1, #tbItem do
		if (tbItem[i] and tbItem[i] ~= "") then
			-- 每行的物品格式：{物品名, nGenre, nDetail, nParticular, nLevel, nSeries, nItemNum}
			local tbTemp = loadstring(string.gsub(tbItem[i],"{.+,(.+),(.+),(.+),(.+),(.+),(.+)}", "return {tonumber(%1),tonumber(%2),tonumber(%3),tonumber(%4),tonumber(%5),tonumber(%6)}"))()
			for i = 1, tbTemp[6] do
				table.insert(tbRet, {tbTemp[1],tbTemp[2],tbTemp[3],tbTemp[4],tbTemp[5]});
			end
		end
	end
	
	return tbRet;
end;


function tb:Start()
	self.nCount = 0;
	self:Register();
end;

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.nCount);
	
	return 1;
end;


function tb:Load(nGroupId, nStartTaskId)
		self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.nCount		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	if (not self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self:Register();
	end;
	
	return 1;
end;


function tb:IsDone()
	return self.nCount >= self.nNeedCount;
end;


function tb:GetDesc()
	-- TODO: liuchang 字符串检查
	return string.format(self.szDynamicDesc,self.nCount,self.nNeedCount);
end;


function tb:GetStaticDesc()
	return self.szStaticDesc;
end;




function tb:Close(szReason)
	self:UnRegister();
end;


function tb:Register()
	self.tbTask:AddExclusiveDialog(self.nNpcTempId, self.SelectOpenBox, self);
end;

function tb:UnRegister()
	self.tbTask:RemoveNpcExclusiveDialog(self.nNpcTempId);
end;


-- 玩家选择打开箱子[S]
function tb:SelectOpenBox()
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.dwSceneTemplateId) then
		self.me.SysMsg(string.format(Lang.task.target.str36[Lang.Idx],self.szNpcName,self.szMapName))
		return;
	end;
	
	if (self:IsDone()) then
		self.me.SysMsg(self.szFailedMsg)
		return;
	end;
	
	--Task:SetProgressTag(self, self.me);
	--KTask.StartProgressTimer(self.me, self.nIntervalTime, self.szProcessInfo);
	
	ProgressBar:StartProcessOnPlayer(self.me, self.szProcessInfo, self.nIntervalTime, {self.OnProgressFull, self})
end;


-- 客户端进度条完成回掉，通知服务端给奖励
function tb:OnProgressFull()
--	local nTotleCount = #self.ItemList;
	
--	if (nTotleCount > 0 and TaskCond:CanAddItemsIntoBag(self.ItemList) ~= 1) then
--		self.me.SysMsg("包裹已满，无法装载新的物品！")
--		return;
--	end
	if (self.OnProgressFullCallbackBeforRefreshTask) then
		self:OnProgressFullCallbackBeforRefreshTask()
	end
	
	self.me.SysMsg(self.szSucMsg);
	self.nCount	= self.nCount + 1;
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.nCount, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
--	for _, tbItem in ipairs(self.ItemList) do
--		Task:AddItem(self.me, tbItem);
--	end
	
	if (self:IsDone()) then
		
		self:UnRegister()	-- 本目标是一旦达成后不会失效的
		self.tbTask:OnFinishOneTag();
	end

	if (self.OnProgressFullCallback) then
		self:OnProgressFullCallback()
	end
end;
