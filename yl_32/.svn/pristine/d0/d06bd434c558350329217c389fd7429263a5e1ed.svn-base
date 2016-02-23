-- 黑屏模式，希望不会给任务系统带来黑色的天空...

-- S
-- 黑屏模式下的回掉函数
function BlackSky:GetDarkData()
	local tbPlayerData		= me.GetTempTable("Task");
	local tbPlayerDarkData	= tbPlayerData.tbDarkData;
	if (not tbPlayerDarkData) then
		tbPlayerDarkData	= {};
		tbPlayerData.tbDarkData	= tbPlayerDarkData;
	end;
	
	return tbPlayerDarkData;
end


-- S
-- 通知客户端，让其黑屏，并把回调函数存入临时Table
function BlackSky:SendDarkCommand(pPlayer, tbParam)
	if (not tbParam) then
		assert(false);
		return;	
	end
	
	if (pPlayer.dwId ~= me.dwId) then
		Dbg:WriteLog("Task", "Player ID Error In Dark[2]!", pPlayer.dwId, me.dwId);
	end
	
	local tbPlayerDarkData	= self:GetDarkData();
	local nInDark = tbPlayerDarkData.nInDark or 0;
	if (tbPlayerDarkData.tbParam) then
		assert(false or "上一个黑屏的还没处理完，不应该同时无顺序的几个黑屏");
		return;
	end
	tbPlayerDarkData.tbParam = tbParam;
	tbPlayerDarkData.tbGRoleArgs = {player = pPlayer};
	tbPlayerDarkData.nDarkPlayerId = pPlayer.dwId;
	KTask.SetBlackSky(pPlayer, 1);	-- 通知客户端开启黑屏模式
	
	-- 若此时为黑屏模式则不用等协议了，直接执行。
	-- 协议到的时候因为tbPlayerDarkData.tbParam为空所以不会执行任何操作
	if (nInDark == 1) then
		self:WhenEvnChange(pPlayer, 1);
		-- self:WhenEvnChange(1);
	end
end

-- S
-- 孤立的Talk对话回调,它封装了关闭的过程，没有回调，若有回调要主动关闭
function BlackSky:SimpleTalk(pPlayer, szMsg, ...)
	self:SendDarkCommand(pPlayer, {TaskAct.NormalTalk, TaskAct, pPlayer, szMsg, self.GiveMeBright, self, pPlayer, unpack(arg)});
end

-- S
-- 孤立的Say对话回调，它封装了关闭过程，没有回掉，若有回掉要主动关闭
function BlackSky:SimpleSay(pPlayer, szMsg, ...)
	if (arg.n <= 0) then
		local tbOpt = { Lang.task.str3[Lang.Idx], self.GiveMeBright, self, pPlayer};
		self:SendDarkCommand(pPlayer, {Dialog.NormalSay, Dialog, szMsg, tbOpt});
	elseif (type(arg[1][1]) == "table") then
		local tbOpt = {};
		for _, item in pairs(arg[1]) do
			if (type(item) == "table") then
				local szDesc = item[1];
				local tbTemp = {};
				for i = 2, #item do
					tbTemp[i-1] = item[i];
				end
				if (type(szDesc) == "string") then
					table.insert(tbOpt, #tbOpt+1, {szDesc, self.GiveMeBright, self, pPlayer, unpack(tbTemp)});	
				end
			end
		end
		self:SendDarkCommand(pPlayer, {Dialog.NormalSay, Dialog, szMsg, tbOpt}); 
	else	-- 3、每一个参数一个选项，一共N个参数
		local tbOpt = {};
		for _, item in pairs(arg) do
			if (type(item) == "table") then
				local szDesc = table.remove (item, 1);
				if (type(szDesc) == "string") then
					table.insert(tbOpt, #tbOpt+1, {szDesc, self.GiveMeBright, self, pPlayer, unpack(item)});
				end
			end
			if (not tbOpt[1]) then
				tbOpt[1] = {Lang.task.str3[Lang.Idx], self.GiveMeBright, self, pPlayer};
			end
			self:SendDarkCommand(pPlayer, {Dialog.NormalSay, Dialog, szMsg, unpack(tbOpt)});
		end 
	end;
end

-- S
-- 服务端收到黑屏完成的操作，执行回调函数,并清空它。或者收到黑屏结束
function BlackSky:WhenEvnChange(nDark)
	local tbPlayerDarkData = self:GetDarkData();
	
	if (not tbPlayerDarkData or not tbPlayerDarkData.tbGRoleArgs or not tbPlayerDarkData.tbGRoleArgs.player) then
		return;
	end
	
	if (tbPlayerDarkData.tbGRoleArgs.player.dwId ~= me.dwId) then
		Dbg:WriteLog("Task", "Player ID Error In Dark[2]!", pPlayer.dwId, me.dwId);
	end
	
	if (nDark == 1) then
		tbPlayerDarkData.nInDark = 1;
		if (not tbPlayerDarkData or not tbPlayerDarkData.tbParam) then
			return;
		end
		
		if(tbPlayerDarkData.nDarkPlayerId ~= tbPlayerDarkData.tbGRoleArgs.player.dwId) then
			Dbg:WriteLog("Task", "Player ID Error In Dark!", tbPlayerDarkData.nDarkPlayerId, tbPlayerDarkData.tbGRoleArgs.player.dwId);
		end
		tbPlayerDarkData.nDarkPlayerId = nil;
		
		local tbDarkData = tbPlayerDarkData.tbParam;
		local oldme = me;
		me = tbPlayerDarkData.tbGRoleArgs.player;
		if (tbDarkData) then
			Lib:CallBack(tbDarkData);
		end
		
		tbPlayerDarkData.tbParam = nil;
		me = oldme;
	elseif (nDark == 0) then
		tbPlayerDarkData.nInDark = 0;	
	end
end


-- S
-- 服务端主动结束黑屏
function BlackSky:GiveMeBright(pPlayer, ...)
	KTask.SetBlackSky(pPlayer, 0);
	local oldme = me;
	me = pPlayer;
	if (arg.n > 0) then
		Lib:CallBack(arg);
	end
	me = oldme;
end


-- C
-- 客户端全黑并且向服务端发送指定协议
function BlackSky:BeginDark()
	Ui(Ui.UI_GUTMODEL):GutBegin(self, self.CompleteDardCallBack);
end

-- C
-- 客户端接到停止黑屏的协议
function BlackSky:EndDark()
	Ui(Ui.UI_GUTMODEL):GutEnd(self, self.InBright);
end


-- C
-- 当客户端全黑的时候的回调函数
function BlackSky:CompleteDardCallBack()
	KTask.SendEnvCallBack(me, 1);
end


-- C
-- 当客户端黑屏结束的时候回掉函数
function BlackSky:InBright()
	KTask.SendEnvCallBack(me, 0);
end

-- 客户端加上黑暗光环
function BlackSky:OnCoverBegin()
	CoreEventNotify(UiNotify.emCOREEVENT_COVER_BEGIN);
end;

-- 客户端去除黑暗光环
function BlackSky:OnCoverEnd()
	CoreEventNotify(UiNotify.emCOREEVENT_COVER_END);
end;
