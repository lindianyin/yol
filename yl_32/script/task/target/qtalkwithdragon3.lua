local tbBase	= Task:GetTarget("WithProcessStatic");
local tb = Task:GetTarget("QTalkWithDragon3")
setmetatable(tb, {__index = tbBase}) -- 继承

-- 跟龙对话完毕后执行。。。！！
-- 继承自TalkWithNpc
function tb:OnProgressFullCallback()
	-- 隐藏龙！ 隐藏马！骑上马！
	--Pet:GetUpRide(me) --by ldy 2015-07-10
	Pet:GetDownRide(me)
end