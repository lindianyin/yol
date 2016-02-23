-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[1001] =
{
	nRequireLevel = 20,            -- 需求等级
	nStartStep = 1,			   				-- 开始步骤
	tbEnterPos = { nMapId = 3, tbPos={{2346, 2347, 1048576}}, nCopyIndex = 0},		-- 进入点
	tbOpenTimes = {{ {hour=19, min=0, sec=0}, {hour=19, min=20, sec=0} }},	--开放时间{StartTime, EndTime},开几次，填几组时间, 每组时间不能重叠  30分钟
	nPrepareTime = 1200,						-- 报名阶段时间
	nReadyStartTime = 2,					-- 准备场在活动开始后多久开启
	ActivityName = Lang.mission.str107[Lang.Idx],    -- 活动名字，输出信息用
	STEP = {},
	nBaseActivityIdx = 1001,
}

local All = Activity.Type_All;
local Self = Activity.Type_Self;
local Npc = Activity.Type_Npc;
local Team = Activity.Type_Team;
local Kin = Activity.Type_Kin;
local Server = Activity.Type_Server;
local All_Enable = Activity.Type_All_Enable;
local Self_Enable = Activity.Type_Self_Enable;
local Npc_Enable = Activity.Type_Npc_Enable;
local Team_Enable = Activity.Type_Team_Enable;
local Kin_Enable = Activity.Type_Kin_Enable;
local Server_Enable = Activity.Type_Server_Enable;
local Not = Activity.Type_Not;

ActivitySetting[1001].Enemy1 = Self + Self_Enable + Not;

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[1001].STEP
STEP[1] =
{
	Time = 1200,                                              --时间
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	NeedTransport = 0,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,                                          -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.BONFIRE_SAICHANG,                          -- 步骤类型
	--Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {3,{2346,2347,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)

	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		-- [1] = {nTime = 2, nNum = 0,		-- 总计时
			-- tbPrelock = {},
			-- tbStartEvent =
			-- {
			-- },
			-- tbUnLockEvent =
			-- {

			-- },
		-- },
		-- [2] = {nTime = 1798 , nNum = 0,		-- 总计时
			-- tbPrelock = {1},
			-- tbStartEvent =
			-- {
				-- {Activity.EVENT_TYPE.SHOW_NOTICE_MSG, "<color=green>家族篝火活动已开启,骚年们速度报名!"},	--准备场开启，发公告
			-- },
			-- tbUnLockEvent =
			-- {
				-- {Activity.EVENT_TYPE.BLACK_MSG, -1, "-- 战神无双自由竞技开启，各位勇士们加油！"},
			-- },
		-- },
    },
    NextSteps = {},
}