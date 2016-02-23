-- 服务端事件
Include("/script/chat/chatchanneldef.lua")

function ServerEvent:OnGlobalExecute(tbCall)
	self:DbgOut("OnGlobalExcute", unpack(tbCall));
	Lib:CallBack(tbCall);
end

function ServerEvent:OnConsoleCommand(szCommand)
	-- 控制台上执行指令不知道谁是me，设置GM.MyName便于使用包含me的指令
	if GM.MyName then
		me =  KGameBase.GetPlayerByName(GM.MyName)
	elseif GM.MyId then
		me =  KGameBase.GetPlayerById(GM.MyId)
	end
	-- 安全起见，gs控制台调?gc脚本要填gm名
	KChat.SendMsg(Chat.emKCHATCHANNEL_GM, 0, 0, GM.MyName or "", szCommand, 1)
	me = nil
end

-- 服务器启动完毕后需要执行的操作
function ServerEvent:OnServerReady()
	Lib:CallBack({"XoyoGame:CreatGame"});
end
