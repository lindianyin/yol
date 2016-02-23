--- 客户端事件

-- 所有客户端资源加载完之后执行的回调
function ClientEvent:OnStart()
	if self.tbStartFun then
		for i, tbStart in ipairs(self.tbStartFun) do
			tbStart.fun(unpack(tbStart.tbParam));
		end
	end
end

-- 注册客户端启动函数
function ClientEvent:RegisterClientStartFunc(fnStartFun, ...)
	assert(fnStartFun);
	if not self.tbStartFun then
		self.tbStartFun = {};
	end
	table.insert(self.tbStartFun, {fun=fnStartFun, tbParam=arg});
end

function ClientEvent:OnServerCall(tbCall)
	Dbg:Output("ClientEvent", "OnServerCall", unpack(tbCall));
	Lib:CallBack(tbCall);
end

function ClientEvent:OnMiniMapClick(nMapX, nMapY)
	Dbg:Output("ClientEvent", "OnMiniMapClick", me.nMapId, nMapX, nMapY);
	local szMsg	= string.format("?gm ds me.NewWorld(me.nMapId, %d, %d)", nMapX, nMapY);
	SendChannelMsg("GM", szMsg);
end

function ClientEvent:OnConsoleCommand(szCommand)
	local nReceiveId = 0;
	me.pChat.SendMsg(Chat.emKCHATCHANNEL_GM, nReceiveId, szCommand);
end
