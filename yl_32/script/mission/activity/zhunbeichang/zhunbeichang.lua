Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.ZHUNBEICHANG = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.ZHUNBEICHANG] = Activity.ZHUNBEICHANG -- 注册到活动工厂

local ZHUNBEICHANG = Activity.ZHUNBEICHNAG