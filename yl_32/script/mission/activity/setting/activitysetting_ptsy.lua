-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[1002] =
{
	nRequireLevel = 20,            -- 需求等级
	nStartStep = 1,			   				-- 开始步骤
	tbEnterPos = { nMapId = 1002, tbPos = {{1137,2686,1048576},{2135,1992,1048576}}, nCopyIndex = 0},		-- 进入点
	tbOpenTimes = {{{hour=1, min=40, sec=0}, {hour=1, min=45, sec=10}}},	--开放时间{StartTime, EndTime},开几次，填几组时间, 每组时间不能重叠  30分钟
	nPrepareTime = 300,						-- 报名阶段时间
	nReadyStartTime = 2,					-- 准备场在活动开始后多久开启
	ActivityName = Lang.mission.str182[Lang.Idx],    -- 活动名字，输出信息用
	nBaseActivityIdx = 1002,
	STEP = {},
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

ActivitySetting[1002].Enemy1 = Self + Self_Enable + Not;

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[1002].STEP
STEP[1] =
{
	Time = 310,                                              --时间
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	NeedTransport = 0,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,                                         -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.KIN_PANTAOSHENGYAN,             -- 步骤类型
	Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {1002,{1726,2280,1048576}},
	NPC =
	{
		-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2650, nLevel = -1, nSeries = -1},		--蟠桃
	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 289 , nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str183[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str183[Lang.Idx]},
				
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str184[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str184[Lang.Idx], 2}},
			
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str185[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str185[Lang.Idx],2}},
			  {Activity.EVENT_TYPE.ADD_NPC, 1, 7, 2,"pantao1","pantao1", 10, 30,"timer1"},   --刷蟠桃
			  {Activity.EVENT_TYPE.ADD_NPC, 1, 7, 2,"pantao2","pantao2", 10, 30,"timer2"},   --刷蟠桃
			  {Activity.EVENT_TYPE.ADD_NPC, 1, 7, 2,"pantao3","pantao3", 10, 30,"timer3"},   --刷蟠桃
			  {Activity.EVENT_TYPE.ADD_NPC, 1, 7, 2,"pantao4","pantao4", 10, 30,"timer4"},   --刷蟠桃

			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.DEL_NPC,"pantao1"},   --蟠桃
				{Activity.EVENT_TYPE.DEL_NPC,"pantao2"},   --蟠桃
				{Activity.EVENT_TYPE.DEL_NPC,"pantao3"},   --蟠桃
				{Activity.EVENT_TYPE.DEL_NPC,"pantao4"},   --蟠桃
				{Activity.EVENT_TYPE.DEL_NPC,"pantao5"},   --蟠桃
			},
		},
       -- 1号锁不能不填，默认1号为起始锁
		[3] = {nTime = 10, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str186[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str186[Lang.Idx], 3}},
			
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str187[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str187[Lang.Idx],3}},
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}