-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

--[[
local ActivitySetting = Activity.ActivitySetting

ActivitySetting[1] = 
{
	--nRequireLevel = 0,       -- 需求等级
	--RequireFaction = -1,     -- 需求职业
	StartTime = 0,             -- 开始时间
    EndTime = 240000,          -- 结束时间
    ActivityName = "", -- 活动名字，输出信息用
	STEP = {},
}

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[1].STEP
STEP[1] =
{
	Time = 300,                                              --时间
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,   
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 400,                                          -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.ZHUNBEICHANG,                   -- 步骤类型
	Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {801,4567,4353,1048576},
	NPC =
	{},
    LOCK = 
    {},
    NextSteps = {},
}
--]]