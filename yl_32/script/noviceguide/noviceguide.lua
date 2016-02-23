-- by KK, 新手指引事件，完成特定事情后，给新手指引调用的脚本，用于控制任务变量, 
GuideEvent.TASK_GROUP_VAR = 9999  -- 控制新手指引的组id

-- 通用，告诉客户端刷新新手指引
function GuideEvent:__RefreshClient()
	
end

-- 申请加入家族~~~ 按申请后
function GuideEvent:ApplyJoinKin()
	local nValue = me.pTask.GetTask(self.TASK_GROUP_VAR, 6)
	if (nValue == 0) then
		me.pTask.SetTask(self.TASK_GROUP_VAR, 6, 1, 1); -- group, varkey,   value,   sync
	end
	
	self:__RefreshClient();
end

-- 第一次升级符文
function GuideEvent:UpgradeRuneSkill(nSlot, nRune)
	-- 第一次升级第一槽第0符文，设置新手指引完成
	if (nSlot == 1 and nRune == 0) then
		local nValue = me.pTask.GetTask(self.TASK_GROUP_VAR, 4)
		if (nValue == 0) then
			me.pTask.SetTask(self.TASK_GROUP_VAR, 4, 1, 1); -- group, varkey,   value,   sync
		end
	end
	
	self:__RefreshClient();
end