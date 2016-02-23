-- 此文件部分功能属于gmcmd_s.lua，写在这里仅为演示纯client对话框调服务器GM指令的方法


if not MODULE_GAMECLIENT then
	return
end

GM.tbGM_CMD_HELPER = {}
local tbGmCmdHelper = GM.tbGM_CMD_HELPER

-- 基本指令表（静态表）
function tbGmCmdHelper:ShowBasicCommands()
	local tbOpt =
	{
		{Lang.misc.str1[Lang.Idx], self.CmdWithArg_Select, self},
		{" "},

		{Lang.misc.str2[Lang.Idx], KGameBase.Console},
		{Lang.misc.str3[Lang.Idx], KGRLInterface.LockCamera, 0},
		{Lang.misc.str4[Lang.Idx], KGRLInterface.LockCamera, 1},
	}
	Dialog:SimpleSay(Lang.misc.str5[Lang.Idx], tbOpt)
end

function tbGmCmdHelper:NavBack()
	self:ShowBasicCommands()
end

local function SendZoneGmCommand(szCommand)
	me.pChat.SendMsg(Chat.emKCHATCHANNEL_GM, 0, "?gc "..szCommand);
end

local function SendGmCommand(szCommand)
	me.pChat.SendMsg(Chat.emKCHATCHANNEL_GM, 0, "?gs "..szCommand);
end


function tbGmCmdHelper:CmdWithArg_Select()
	me.SysMsg(Lang.misc.str6[Lang.Idx])
	local tbOpt =
	{
		{Lang.misc.str7[Lang.Idx], self.CmdWithArg_ZoneBroadcast, self},
		{Lang.misc.str8[Lang.Idx], self.CmdWithArg_ZoneSysMsg, self},
		--{"[区] 统计在线人数", self.CmdWithArg_ZoneCountPlayer, self},
		{Lang.misc.str9[Lang.Idx], self.CmdWithArg_CallNpc, self},
	}
	Dialog:SimpleSay(Lang.misc.str10[Lang.Idx], tbOpt)
end

function tbGmCmdHelper:CmdWithArg_ZoneBroadcast()
	local szMsg = Edt_GetText(Ui.UI_CHATPANEL, "EdtChatInputInner")
	if not szMsg then
		me.SysMsg(Lang.misc.str11[Lang.Idx])
		return
	end
	me.pChat.SendMsg(Chat.emKCHATCHANNEL_GLOBAL, 0, szMsg);
	Edt_SetText(Ui.UI_CHATPANEL, "EdtChatInputInner", "")
end

function tbGmCmdHelper:CmdWithArg_ZoneSysMsg()
	local szMsg = Edt_GetText(Ui.UI_CHATPANEL, "EdtChatInputInner")
	if not szMsg then
		me.SysMsg(Lang.misc.str11[Lang.Idx])
		return
	end
	SendZoneGmCommand([[
		GlobalExecute{"KChat.BroadcastSysMsg", "]]..szMsg..[["}
		]])
	Edt_SetText(Ui.UI_CHATPANEL, "EdtChatInputInner", "")
end

function tbGmCmdHelper:CmdWithArg_CallNpc()
	local szMsg = Edt_GetText(Ui.UI_CHATPANEL, "EdtChatInputInner")
	if not szMsg then
		me.SysMsg(Lang.misc.str11[Lang.Idx])
		return
	end
	SendGmCommand([[
		local tbPos = me.GetPosition()
		KGameBase.CallNpc(]]..szMsg..[[, tbPos.dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ, 140)
		]])
	Edt_SetText(Ui.UI_CHATPANEL, "EdtChatInputInner", "")
end
