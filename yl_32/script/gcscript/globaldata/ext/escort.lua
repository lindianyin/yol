
if not MODULE_GAMECENTER then
	return
end

-- 重置所有在线玩家的修仙任务
function GlobalData:ResetEscortTasks()
	GlobalExecute({"Escort:ResetEscortTasks"});
end