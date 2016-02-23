local tbBase	= Task:GetTarget("WithProcessStatic");
local tb = Task:GetTarget("QTalkWithDragon1")
setmetatable(tb, {__index = tbBase}) -- 继承

-- 跟龙对话完毕后执行。。。！！
-- 继承自TalkWithNpc
-- function tb:OnTalkFinishCallback()
function tb:OnProgressFullCallbackBeforRefreshTask()  -- 进度条满后执行
	local tbTargetPos = {3, 2450, 4972, 1048576}
	-- local nSelfSceneId = me.GetPosition().dwSceneId;
	
	me.TeleportTo(tbTargetPos[1], tbTargetPos[2], tbTargetPos[3], tbTargetPos[4]);
	me.CallClientScript({"QClientScript.FirstLoginScript:DoLongFlyAnimation"});
end