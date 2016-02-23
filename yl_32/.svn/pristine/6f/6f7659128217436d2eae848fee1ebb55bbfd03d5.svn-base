-------------------------------------------------------------------
--File: dialog.lua
--Author: luobaohang
--Date: 2010/1/5 14:46:27
--Describe: 对话功能脚本
-------------------------------------------------------------------
Dialog.tbCmd = {}

function Dialog:c2s_ClearCallBacks(pPlayer)
	self:ClearCallBacks()
end

function Dialog:ClearCallBacks()
	local tbPlayerDialog	= self:_GetMyDialog()
	if (tbPlayerDialog and tbPlayerDialog.tbCallBacks) then
		tbPlayerDialog.tbCallBacks = {};
		--这里一起把HimId也清掉
		tbPlayerDialog.dwHimId = nil
	end
	if MODULE_GAMESERVER then
		--self:_SyncCloseSay();
	end
end

function Dialog:ClearFollowDialog(pPlayer)
	local tbPlayerDialog	= self:_GetPlayerDialog(pPlayer);
	if tbPlayerDialog and tbPlayerDialog.nFollow then
		tbPlayerDialog.tbCallBacks = {};
		return 1
	end
end

function Dialog:SimpleSay(szMsg, ...)
	local tbPlayerDialog	= self:_GetMyDialog();
	local tbCallBacks		= {};
	tbPlayerDialog.tbCallBacks	= tbCallBacks;
	tbPlayerDialog.nFollow = nil;
	local tbSayOptions = self:_GenerateOption(tbCallBacks, ...)

	if not tbSayOptions then
		return
	end


	me.CallClientScript({"YoulongApp.UI.Controller.UISimplePanelController:ShowDialog", szMsg, tbSayOptions.tbMenu});

		--[[
	if MODULE_GAMECLIENT then
		tbPlayerDialog.bClientFlag = 1;
		tbPlayerDialog.bOpen = 1; --用于标识这个对话框到了定时器事件时是否需要关闭
		-- TODO 等core fireevent 事件好了这个必须改成事件机制
		local tbConversationParam = {};
		tbConversationParam.szMsg = szMsg;
		tbConversationParam.tbImg = tbSayOptions.tbImg;
		tbConversationParam.szExInfo = tbSayOptions.szExInfo;
		tbConversationParam.tbMenu = tbSayOptions.tbMenu;
		tbConversationParam.bCloseByFocus = tbSayOptions.bCloseByFocus

		UiManager:OpenWindow("UI_GMCMD_PANEL", tbConversationParam);
		if self.nRegisterShowId then
			Timer:Close(self.nRegisterShowId)
			self.nRegisterShowId = nil
		end
	else
		me.CallClientScript({"Dialog:_ShowSay", szMsg, tbSayOptions, 0, 1});
	end
	--]]
end

-- 需要选项的一段文字对话
function Dialog:Say(szMsg, ...)
	local tbPlayerDialog	= self:_GetMyDialog();
	local tbCallBacks		= {};
	tbPlayerDialog.tbCallBacks	= tbCallBacks;
	tbPlayerDialog.nFollow = nil;
	local tbSayOptions = self:_GenerateOption(tbCallBacks, ...)

	if not tbSayOptions then
		return
	end
	if MODULE_GAMECLIENT then
		-- 纯客户端对话
		Dialog:_ShowSay(szMsg, tbSayOptions, 1);
	else
		--这个HimId的清除放在清除回调函数ClearCallBacks() 一起
		if him then
			tbPlayerDialog.dwHimId = him.dwId
		end
		--me.CallClientScript({"Dialog:_ShowSay", szMsg, tbSayOptions, 0});
		local szContent = string.format("<Msg>%s</Msg>", szMsg)
		
		if #tbSayOptions.tbMenu <= 3 then   -- 只有一个选择情况下直接回调
		  self:c2s_OnSelect(me, 1)
		else
			for _, szOpt in pairs(tbSayOptions.tbMenu) do
			  print(szOpt)
				szContent = string.format("%s<Opt>%s</Opt>", szContent, szOpt)
			end
			me.SendDialog(szContent)
		end
	end

	return 1  -- have talk
end;

function Dialog:SendInfoBoardMsg(pPlayer, szMsg)
	-- 临时使用系统消息
	pPlayer.SysMsg(szMsg)
end

function Dialog:GetHimId()
	local tbPlayerDialog	= self:_GetMyDialog()
	if tbPlayerDialog.dwHimId then
		return tbPlayerDialog.dwHimId
	end
end

-- 需要选项的一段文字对话
function Dialog:Say2(szMsg, pPlayer, ...)
	local tbPlayerDialog	= self:_GetPlayerDialog(pPlayer);
	local tbCallBacks		= {};
	tbPlayerDialog.tbCallBacks	= tbCallBacks;
	tbPlayerDialog.nFollow = 1;
	local tbSayOptions = self:_GenerateOption(tbCallBacks, ...)

	if not tbSayOptions then
		return
	end
	if MODULE_GAMECLIENT then
		-- 纯客户端对话
		Dialog:_ShowSay(szMsg, tbSayOptions, 1);
	else
		pPlayer.CallClientScript({"Dialog:_DelayShowSay", szMsg, tbSayOptions, 0});
	end
end;

-- Npc头顶冒泡
function Dialog:NpcBubble(nNpcId, szMsg, delay)
	if delay and delay > 0 then
		Timer:Register(delay, self.NpcBubble, self, nNpcId, szMsg);
		return
	end
	me.CallClientScript({"QClientScript.UI:ShowNpcBubble", nNpcId, szMsg});
	--print("npc bubble", szMsg);
	return 0;
end

-- Npc头顶冒泡给npc附近所有玩家看
function Dialog:NpcBubbleToNearBy(nNpcId, szMsg, delay)
	if delay and delay > 0 then
		Timer:Register(delay, self.NpcBubbleToNearBy, self, nNpcId, szMsg);
		return
	end

	local pNpc = KGameBase.GetNpcById(nNpcId)
	if pNpc then
		pNpc.CallClientScriptOfNearByPlayer({"QClientScript.UI:ShowNpcBubble", nNpcId, szMsg});
		--print("npc bubble", szMsg);
	end
	return 0;
end

-- Npc头顶冒泡给npc附近所有玩家看
function Dialog:StartScaleAnimationToNearBy(nNpcId, targetScale, delay)
	if delay and delay > 0 then
		Timer:Register(delay, self.StartScaleAnimationToNearBy, self, nNpcId, targetScale);
		return
	end
	print("Dialog:StartScaleAnimationToNearBy(nNpcId, targetScale, delay)", nNpcId, targetScale, delay)
	local pNpc = KGameBase.GetNpcById(nNpcId)
	pNpc.CallClientScriptOfNearByPlayer({"YoulongApp.Represent.QRepresentClientScript:StartNpcScaleAnimation", nNpcId, targetScale});
	return 0;
end

-- 人物头顶冒泡
function Dialog:PlayerBubble(szMsg, delay)
	if delay and delay > 0 then
		Timer:Register(delay, self.PlayerBubble, self, szMsg);
		return
	end
	me.CallClientScript({"QClientScript.UI:ShowPlayerBubble", me.dwId, szMsg});
	print("player bubble", szMsg);
	return 0;
end

-- 没有选项的多段文字对话
function Dialog:Talk(pPlayer, tbMsg, ...)
--	if (arg[1]) then	-- 允许没有回调
--		self:_GetMyDialog().tbCallBacks	= {arg} -- arg本身就是一个表，说明对于Talk来说就只有tbCallBacks[1]
--	end;
--	if MODULE_GAMECLIENT then
--		-- 纯客户端对话
--		Dialog:_ShowTalk(tbMsg, 1);
--	else
--		print("talk", unpack(tbMsg))
--		--me.CallClientScript{"Dialog:_ShowTalk", tbMsg};
--	end
	if (arg[1]) then	-- 允许没有回调
		self:_GetMyDialog().tbCallBacks	= {arg} -- arg本身就是一个表，说明对于Talk来说就只有tbCallBacks[1]

		self:_GetMyDialog().tbGRoleArgs = self:_GetMyDialog().tbGRoleArgs or {};
		if (pPlayer) then
			self:_GetMyDialog().tbGRoleArgs.playerId = pPlayer.dwId;
		else
			self:_GetMyDialog().tbGRoleArgs.playerId = 0;
		end;
		if (him) then
			self:_GetMyDialog().tbGRoleArgs.npcId = him.dwId;
		else
			self:_GetMyDialog().tbGRoleArgs.npcId = 0;
		end;

	end;
	pPlayer.SendTalk(table.concat(tbMsg, "<end>"));
end;

function Dialog:SendBlackBoardMsg(pPlayer, szMsg)
	pPlayer.SendBlackMsg(szMsg);
end

-- 请求客户端输入字符串
function Dialog:AskString(szTitle, nMax, ...)
	local tbPlayerDialog	= self:_GetMyDialog();
	tbPlayerDialog.tbStringCallBack	= arg;

	if MODULE_GAMECLIENT then
		-- 纯客户端对话
		Dialog:_ShowAskString(szTitle, nMax, 1);
	else
		me.CallClientScript("Dialog:_ShowAskString", szTitle, nMax);
	end
end;

-- 请求客户端输入数字
function Dialog:AskNumber(szTitle, nMax, szCallBack)
	me.CallClientScript({"YoulongApp.UI.GUIPanelManager:ShowInputNumberWindow", szTitle, nMax, szCallBack});
end

function Dialog:AskString(szTitle, nMin, nMax, szCallBack)
	me.CallClientScript({"YoulongApp.UI.GUIPanelManager:ShowCreateKinPanel", nMin, nMax, szCallBack});
end

---- 本服公告
--function Dialog:Msg2SubWorld(szMsg)
--	KDialog.Msg2SubWorld(szMsg);
--end
--
---- 全服公告
--function Dialog:Msg2Global(szMsg)
--	GlobalExecute{"KDialog.Msg2SubWorld", szMsg};
--end

------------------------------------------------------------------------
-- 生成对话框所需要的选项
function Dialog:_GenerateOption(tbCallBacks, ...)
	if not tbCallBacks then
		return;
	end
	local tbOpt;
	-- 选项参数分三大类方式
	if (not arg[1]) then	-- 1、无选项，默认空
		tbOpt	= {}
	elseif (type(arg[1][1]) == "table") then	-- 2、一组选项作为一个参数
		tbOpt	= arg[1];
	else	-- 3、每一个参数一个选项，一共N个参数
		tbOpt	= arg;
	end;

	local tbOptions	= {};
	for i, v in ipairs(tbOpt) do
		if (type(v) == "table") then
			tbOptions[i]	= v[1];
			if (v[2]) then
				tbCallBacks[i]	= {unpack(v,2)};
			end;
		else
			tbOptions[i]	= v;
		end;
	end;
	if (not tbOptions[1]) then
		tbOptions[1]	= "结束对话";
	end;

	local tbSayOptions = {};
	tbSayOptions.tbMenu = tbOptions;
	tbSayOptions.tbImg = arg[2];
	tbSayOptions.szExInfo = arg[3];
	tbSayOptions.bCloseByFocus = arg[4]

	return tbSayOptions;
end
-- 取得当前玩家对话相关的临时Table
function Dialog:_GetMyDialog()
	local tbPlayerData = {};
	if MODULE_GAMECLIENT then
		tbPlayerData		= me.GetTempTable("Dialog");
	end

	if MODULE_GAMESERVER then
		tbPlayerData		= me.GetTempTable("Dialog");
	end
	local tbPlayerDialog	= tbPlayerData.tbDialog;
	if (not tbPlayerDialog) then
		tbPlayerDialog	= {
			tbCallBacks	= {},
		};
		tbPlayerData.tbDialog	= tbPlayerDialog;
	end;
	return tbPlayerDialog;
end;

if MODULE_GAMESERVER then
-- 取得指定玩家对话相关的临时Table 服务端专用）
function Dialog:_GetPlayerDialog(pPlayer)
	if not pPlayer then
		return
	end
	local tbPlayerData = {};
	tbPlayerData = pPlayer.GetTempTable("Dialog");

	local tbPlayerDialog	= tbPlayerData.tbDialog;
	if (not tbPlayerDialog) then
		tbPlayerDialog	= {
			tbCallBacks	= {},
		};
		tbPlayerData.tbDialog	= tbPlayerDialog;
	end;
	return tbPlayerDialog;
end;
end

-- 系统调用的选项被选中
function Dialog:c2s_OnSelect(pPlayer, nSelect)
	self:_OnSelect(nSelect)
end
function Dialog:_OnSelect(nSelect)
	local tbPlayerDialog	= self:_GetMyDialog();
	if tbPlayerDialog.nFollow then
		return
	end
	if MODULE_GAMECLIENT and tbPlayerDialog.bClientFlag ~= 1 then
		me.CallServerScript({"Dialog:c2s_OnSelect", nSelect});
	else
		if MODULE_GAMESERVER then
			--self:_SyncSelect(nSelect);
			--self:_SyncCloseSay();
		end
		local tbCallBack		= tbPlayerDialog.tbCallBacks[nSelect];
		tbPlayerDialog.tbCallBacks	= {};
		if (tbCallBack) then
			if (tbPlayerDialog.dwHimId) then
				him = KGameBase.GetNpcById(tbPlayerDialog.dwHimId);
			end
			Lib:CallBack(tbCallBack);
		end;
	end
end;

function Dialog:c2s_OnEnterPage(pPlayer, nPageIndex)
	self:_OnEnterPage(nPageIndex)
end
function Dialog:_OnEnterPage(nPageIndex)
	local tbPlayerDialog	= self:_GetMyDialog();
	if MODULE_GAMECLIENT and tbPlayerDialog.bClientFlag ~= 1 then
		me.CallServerScript({"Dialog:c2s_OnEnterPage", nPageIndex});
	elseif MODULE_GAMESERVER then
		--self:_SyncEnterPage(nPageIndex)
	end
end

function Dialog:_SyncCloseSay()
	--只有队长才可将跟随队友显示的关掉
	if me.IsCaptain() == 1  then
		local nTeamId = me.GetTeamId()
		if nTeamId > 0 then
			local tbFollowMember = KTeam.GetFollowLeaderMemberList(nTeamId)
			if not tbFollowMember then
				return
			end
			for _,nPlayerId in pairs(tbFollowMember) do
				if nPlayerId ~= me.dwId then
					local pPlayer = KGameBase.GetPlayerById(nPlayerId)
					if self:ClearFollowDialog(pPlayer) then
						pPlayer.CallClientScript({"Dialog:_DelayCloseSay"})
					end
				end
			end
		end
	end
end
function Dialog:_SyncSelect(nSelectIndex)
	--只有队长才可将跟随队友显示的关掉
	if me.IsCaptain() == 1  then
		local nTeamId = me.GetTeamId()
		if nTeamId > 0 then
			local tbFollowMember = KTeam.GetFollowLeaderMemberList(nTeamId)
			if not tbFollowMember then
				return
			end
			for _,nPlayerId in pairs(tbFollowMember) do
				if nPlayerId ~= me.dwId then
					local pPlayer = KGameBase.GetPlayerById(nPlayerId)
					if self:ClearFollowDialog(pPlayer) then
						pPlayer.CallClientScript({"Dialog:_ShowSelect", nSelectIndex})
					end
				end
			end
		end
	end
end
function Dialog:_SyncEnterPage(nPageIndex)
	--只有队长才可将跟随队友显示的关掉
	if me.IsCaptain() == 1  then
		local nTeamId = me.GetTeamId()
		if nTeamId > 0 then
			local tbFollowMember = KTeam.GetFollowLeaderMemberList(nTeamId)
			if not tbFollowMember then
				return
			end
			for _,nPlayerId in pairs(tbFollowMember) do
				if nPlayerId ~= me.dwId then
					local pPlayer = KGameBase.GetPlayerById(nPlayerId)
					if self:ClearFollowDialog(pPlayer) then
						pPlayer.CallClientScript({"Dialog:_EnterPage", nPageIndex})
					end
				end
			end
		end
	end
end
-- 点击确定
function Dialog:c2s_OnAskStringOk(pPlayer, szValue)
	self:_OnAskStringOk(szValue)
end
function Dialog:_OnAskStringOk(szValue)
	local tbPlayerDialog	= self:_GetMyDialog();
	if MODULE_GAMECLIENT and tbPlayerDialog.bClientFlag_AskString ~= 1 then
		me.CallServerScript{"Dialog:c2s_OnAskStringOk", szValue}
	else
		local tbCallback = tbPlayerDialog.tbStringCallBack
		tbPlayerDialog.tbStringCallBack	= nil
		self:_OnAskCallback(tbCallback, szValue)
	end
end

-- 点击确定
function Dialog:c2s_OnAskNumberOk(pPlayer, nValue)
	self:_OnAskNumberOk(nValue)
end
function Dialog:_OnAskNumberOk(nValue)
	if not nValue then
		return
	end
	nValue	= math.floor(tonumber(nValue));
	if (nValue < 0 or nValue > 20*10000*10000) then	-- 暂不允许负数和20亿以上
		--ServerEvent:WriteLog(Dbg.LOG_ERROR, "DlgCmd-InputNum Error!", me.szName, me.szAccount, nValue);
		return;
	end
	local tbPlayerDialog	= self:_GetMyDialog();
	if MODULE_GAMECLIENT and tbPlayerDialog.bClientFlag_AskString ~= 1 then
		me.CallServerScript{"Dialog:c2s_OnAskNumberOk", nValue}
	else
		local tbCallback = tbPlayerDialog.tbNumberCallBack
		tbPlayerDialog.tbNumberCallBack	= nil
		self:_OnAskCallback(tbCallback, nValue)
	end
end

function Dialog:_OnAskCallback(tbCallBack, tParam)
	if (not tbCallBack) then
		return;
	end;
	table.insert(tbCallBack, tParam);
	Lib:CallBack(tbCallBack);
end;

Dialog.tbAskForSure = {};

-- szKeyStr : 是否出现“下次跳过按钮”
function Dialog:_AskForSure(characterId, text, tbCallBack, szKeyStr, isMask)
	if szKeyStr == nil then
		szKeyStr = ""
	end
	local pPlayer = KGameBase.GetPlayerById(characterId);
	if not pPlayer then
		return;
	end
	if not isMask then
		isMask = 0;
	end
	self.tbAskForSure[characterId] = tbCallBack;
	pPlayer.CallClientScript({"YoulongApp.UI.GUIPanelManager:CommonShowConfirmWindow", text, szKeyStr, isMask});
end

function Dialog:c2s_AskForSureCallBack(pPlayer)
	local callback = self.tbAskForSure[pPlayer.dwId];
	--assert(callback);
	if(callback) then
		self.tbAskForSure[pPlayer.dwId] = nil;
		Lib:CallBack(callback);
	end
end

-- 客户端
if MODULE_GAMECLIENT then

function Dialog:_ShowSay(szMsg, tbOption, bClientFlag, bSimple)
	local tbPlayerDialog	= self:_GetMyDialog();
	tbPlayerDialog.bClientFlag = bClientFlag or 0;
	tbPlayerDialog.bOpen = 1; --用于标识这个对话框到了定时器事件时是否需要关闭
	-- TODO 等core fireevent 事件好了这个必须改成事件机制
	local tbConversationParam = {};
	tbConversationParam.szMsg = szMsg;
	tbConversationParam.tbImg = tbOption.tbImg;
	tbConversationParam.szExInfo = tbOption.szExInfo;
	tbConversationParam.tbMenu = tbOption.tbMenu;
	tbConversationParam.bCloseByFocus = tbOption.bCloseByFocus

	if bSimple == 1 then
		UiManager:OpenWindow("UI_GMCMD_PANEL", tbConversationParam);
	else
		UiManager:OpenWindow("UI_CONVERSATIONPANEL", tbConversationParam);
	end
	if self.nRegisterShowId then
		Timer:Close(self.nRegisterShowId)
		self.nRegisterShowId = nil
	end
end

function Dialog:_DelayShowSay(szMsg, tbOption, bClientFlag)
	if self.nRegisterCloseId then
		if not self.nRegisterShowId then
			self.nRegisterShowId = Timer:Register(6, self._ShowSay, self, szMsg, tbOption, bClientFlag)
		end
	else
		self:_ShowSay(szMsg, tbOption, bClientFlag)
	end
end
function Dialog:_DelayCloseSay()
	if not self.nRegisterCloseId then
		local tbPlayerDialog = self:_GetMyDialog();
		tbPlayerDialog.bOpen = 0;
		self.nRegisterCloseId = Timer:Register(6, self._OnTimerClose, self)
	end
end
function Dialog:_OnTimerClose()
	local tbPlayerDialog = self:_GetMyDialog();
	if tbPlayerDialog.bOpen == 0 then
		UiManager:CloseWindow("UI_CONVERSATIONPANEL");
		self:ClearCallBacks()
	end
	if self.nRegisterCloseId then
		Timer:Close(self.nRegisterCloseId)
		self.nRegisterCloseId = nil
	end
end
function Dialog:_ShowSelect(nSelectIndex)
	local tbWnd = Ui(Ui.UI_CONVERSATIONPANEL);
	tbWnd:ShowSelect(nSelectIndex);
end
function Dialog:_EnterPage(nPageIndex)
	local tbWnd = Ui(Ui.UI_CONVERSATIONPANEL);
	tbWnd:EnterToPage(nPageIndex);
end
function Dialog:_ShowTalk(tbMsg, bClientFlag)
	local tbPlayerDialog	= self:_GetMyDialog()
	tbPlayerDialog.bClientFlag = bClientFlag or 0;
	-- 调界面显示选项
end

function Dialog:_ShowAskString(szTitle, nMax, bClientFlag)
	local tbPlayerDialog	= self:_GetMyDialog()
	tbPlayerDialog.bClientFlag_AskString = bClientFlag or 0;
	-- 调界面显示选项
end

function Dialog:_ShowAskNumber(szTitle, nMax, bClientFlag)
	local tbPlayerDialog	= self:_GetMyDialog()
	tbPlayerDialog.bClientFlag_AskNumber = bClientFlag or 0;
	-- 调界面显示选项
end

end -- if MODULE_GAMECLIENT then
