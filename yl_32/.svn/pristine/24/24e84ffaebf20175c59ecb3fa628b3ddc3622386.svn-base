

if not MODULE_GAMECLIENT then
	return
end

function ProgressBar:OnStartProgressBar(nInterval, szDesc)
	--tbProgressInfo: {nTime = 100, szDesc = "´ò¿ªÖÐ"}
	local tbProgressInfo = { nTime = nInterval, szDesc = szDesc }
	UiManager:OpenWindow(Ui.UI_PROGRESSBAR, tbProgressInfo)
end

function ProgressBar:OnCloseProgressBar(bFinished)
	Ui(Ui.UI_PROGRESSBAR):OnServerClose()
end

function ProgressBar:CancelProgressBar()
	me.CallServerScript{"ProgressBar:c2s_CancelProgressBar"}
end

function ProgressBar:OnUninit()
	Ui(Ui.UI_PROGRESSBAR):OnServerClose()
end


if ProgressBar.nLostConnectEventId then
	CallCenter:UnRegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_GAMEWORLD_LOST_CONNECT, ProgressBar.nLostConnectEventId)
end
if ProgressBar.nShutdownEventId then
	CallCenter:UnRegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_GAMEWORLD_SHUTDOWN, ProgressBar.nShutdownEventId)
end
ProgressBar.nLostConnectEventId =
	CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_GAMEWORLD_LOST_CONNECT, ProgressBar.OnUninit, ProgressBar)
ProgressBar.nShutdownEventId =
	CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_GAMEWORLD_SHUTDOWN, ProgressBar.OnUninit, ProgressBar)

