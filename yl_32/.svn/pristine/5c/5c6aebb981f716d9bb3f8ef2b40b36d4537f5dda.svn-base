-- Gamecenter事件

function GCEvent:OnCenterExecute(tbCall)
	--print("OnCenterExcute", unpack(tbCall));
	Lib:CallBack(tbCall);
end

-- Gamecenter初始化完毕
function GCEvent:OnStartUp()
	--TimeFrame:SaveStartServerTime();	--第一次启动服务器时,自动记录开服时间;
	--TimeFrame:Init();					--时间轴初始化;
	--Task.tbHelp:LoadDynamicNewsGC();	--加载动态消息
	--Player:SetMaxLevelGC();				--最大等级设置;
	--EventManager.EventManager:Init();	--活动系统初始化;
	-- 执行服务器启动函数
	--if self.tbStartFun then
	--	for i, tbStart in ipairs(self.tbStartFun) do
	--		tbStart.fun(unpack(tbStart.tbParam));
	--	end
	--end
end

-- Gamecent正常关闭时调用
function GCEvent:OnGCUnit()
	-- 执行服务器关闭函数
	print("GCEvent:OnGCUnit");
	if self.tbShutDownFun then
		for i, tbShutDown in ipairs(self.tbShutDownFun) do
			tbShutDown.fun(unpack(tbShutDown.tbParam));
		end
	end	
end

-- 注册服务器启动执行函数
function GCEvent:RegisterGCServerStartFunc(fnStartFun, ...)
	if not self.tbStartFun then
		self.tbStartFun = {}
	end
	table.insert(self.tbStartFun, {fun=fnStartFun, tbParam=arg});
end

-- 注册服务器正常关闭执行函数
function GCEvent:RegisterGCServerShutDownFunc(fnShutDownFun, ...)
	if not self.tbShutDownFun then
		self.tbShutDownFun = {}
	end
	table.insert(self.tbShutDownFun, {fun=fnShutDownFun, tbParam=arg});
end

function GCEvent:OnPlayerLogin(dwPlayerId, nServerId)

end

function GCEvent:OnPlayerLogout(dwPlayerId, nServerId)

end
