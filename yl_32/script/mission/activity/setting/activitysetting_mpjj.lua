-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[10] =
{
	nRequireLevel = 25,            -- 需求等级
	--RequireFaction = 1,         -- 需求职业
	nStartStep = 1,			   				-- 开始步骤
	tbEnterPos = { nMapId = 338, tbPos = {{2359,5431,1048576},{2716,5774,1048576},{2698,2641,1048576},{2359,2304,1048576},{5600,2338,1048576},{6014,2537,1048576},{5529,5395,1048576},{5942,5701,1048576}}},		-- 进入点
	tbOpenTimes = {{{ wday=3,hour=14, min=30, sec=0}, {wday=3,hour=15, min=0, sec=5}},
					{{ wday=3,hour=19, min=30, sec=0}, {wday=3,hour=20, min=0, sec=5}},
					{{ wday=5,hour=14, min=30, sec=0}, {wday=5,hour=15, min=00, sec=5}},
					{{ wday=5,hour=20, min=00, sec=0}, {wday=5,hour=21, min=0, sec=5}},
					{{ wday=7,hour=14, min=30, sec=0}, {wday=7,hour=15, min=0, sec=5}},
					{{ wday=7,hour=19, min=30, sec=0}, {wday=7,hour=20, min=0, sec=5}}},		--开放时间{StartTime, EndTime},开几次，填几组时间, 每组时间不能重叠  10分钟
	--时间间隔20分
	nPrepareTime = 180,						-- 报名阶段时间
	nReadyStartTime = 2,					-- 准备场在活动开始后多久开启
	ActivityName = Lang.mission.str137[Lang.Idx],    -- 活动名字，输出信息用
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

ActivitySetting[10].Enemy1 = Self + Self_Enable + Not;

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[10].STEP
STEP[1] =                                                  --报名阶段
{
	Time = 180,                                              --时间
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	NeedTransport = 0,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,                                          -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.ZHUNBEICHANG, 
	nPrepareTime = 180,						-- 报名阶段时间                         -- 步骤类型
	Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {338,{2359,5431,1048576},{2716,5774,1048576},{2698,2641,1048576},{2359,2304,1048576},{5600,2338,1048576},{6014,2537,1048576},{5529,5395,1048576},{5942,5701,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap1
 		[2] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap2
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
		[2] = {nTime = 179 , nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str138[Lang.Idx]},	--准备场开启，发公告
			  {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str139[Lang.Idx], 2},
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str139[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str140[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str140[Lang.Idx], 2}},
		
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 2, "trap1", "trap1"},  --刷出trap1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 1, 2, "trap2", "trap2"},  --刷出trap2
				{Activity.EVENT_TYPE.SET_TRAP_EFFECT, "bwc_trap1", 1},  --增加trap1
				{Activity.EVENT_TYPE.SET_TRAP_EFFECT, "bwc_trap2", 1},  --增加trap2
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str141[Lang.Idx]},
				{Activity.EVENT_TYPE.DEL_NPC, "trap1"}, --删除trap1
				{Activity.EVENT_TYPE.DEL_NPC, "trap2"}, --删除trap2
			},
		},
    },
    NextSteps = {2},
}

STEP[2] =                                                        --战神无双混合赛
{
	Time = 340,                                                    --时间
	NeedTransport = 0,
	AssignType = Activity.ASSIGN_TYPE.SPECIFY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_YUXIANSAI,
	Pos = {338,{2359,5431,1048576},{2716,5774,1048576},{2698,2641,1048576},{2359,2304,1048576},{5600,2338,1048576},{6014,2537,1048576},{5529,5395,1048576},{5942,5701,1048576}},
	NeedTransport = 0,
	NPC =
	{
							-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2536, nLevel = -1, nSeries = -1},		--自杀npc
 		[2] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap1
 		[3] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap2

	},
    LOCK =
    {
		[1] = {nTime = 300, nNum = 0,		-- 战神无双混合赛
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
			  {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str142[Lang.Idx], 1},
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str142[Lang.Idx]}, 1},

			  {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str143[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str143[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:ShowStartEffect()"},
				
				{Activity.EVENT_TYPE.ADD_NPC, 2, 1, 1, "trap1", "trap1"},  --刷出trap1
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 1, "trap2", "trap2"},  --刷出trap2

				
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str144[Lang.Idx]},
			},
		},
		[2] = {nTime = 40, nNum = 0,		-- 混合赛结束，休息30秒。
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str145[Lang.Idx], 2},
			 	{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str145[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str146[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str146[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:ShowEndEffect()"},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.DEL_NPC, "trap1"}, --删除trap1
				{Activity.EVENT_TYPE.DEL_NPC, "trap2"}, --删除trap2
			},
		},

    },
    AssignIndexs = {{1,16},{2,15},{3,14},{4,13},{5,12},{6,11},{7,10},{8,9}},   --16强分组
    NextSteps = {3,4,5,6,7,8,9,10,18},
    ClearanceStep = 18,
}

STEP[3] =                                                        --16进8擂台1
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{711,645,1048576},{663,1079,1048576},{1081,1095,1048576},{1098,638,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str147[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str148[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str148[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str149[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str149[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str150[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 1)"},  --设置比赛进程，就是当前是16强比赛
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str152[Lang.Idx]},
			},
		},
    },
    NextSteps = {11},
    ClearanceStep = 18,
}

STEP[4] =                                                        --16进8 擂台2
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{667,3888,1048576},{636,4316,1048576},{1050,4329,1048576},{1064,3864,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str153[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str154[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str154[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str155[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str155[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str156[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 5)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str157[Lang.Idx]},
			},
		},
    },
    NextSteps = {12},
    ClearanceStep = 18,
}

STEP[5] =                                                        --16进8 擂台3
{
	Time = 140,                                                   --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                                  -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{420,7207,1048576},{512,7719,1048576},{1052,7717,1048576},{956,7169,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str147[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str154[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str154[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str155[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str155[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str156[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 8)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str157[Lang.Idx]},
			},
		},
    },
    NextSteps = {14},
    ClearanceStep = 18,
}

STEP[6] =                                                        --16进8擂台4
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{3898,7095,1048576},{3877,7539,1048576},{4364,7551,1048576},{4374,7089,1048576}},
	NPC =
	{
			-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str147[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str154[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str154[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str155[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str155[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str156[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 4)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str157[Lang.Idx]},
			},
		},
    },
    NextSteps = {13},
    ClearanceStep = 18,
}

STEP[7] =                                                         --16进8擂台5
{
	Time = 140,                                                     --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                                   -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{7142,7089,1048576},{7128,7585,1048576},{7617,7594,1048576},{7633,7093,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str147[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str154[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str154[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str155[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str155[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str156[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 3)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str157[Lang.Idx]},
			},
		},
    },
    NextSteps = {13},
    ClearanceStep = 18,
}

STEP[8] =                                                        --16进8擂台6
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{7593,4339,1048576},{7147,4338,1048576},{7142,3851,1048576},{7580,3823,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str147[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str154[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str154[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str155[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str155[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str156[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 7)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str157[Lang.Idx]},
			},
		},
    },
    NextSteps = {14},
    ClearanceStep = 18,
}

STEP[9] =                                                        --16进8 擂台7
{
	Time = 140,                                                    --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{7571,1079,1048576},{7137,1101,1048576},{7129,625,1048576},{7562,609,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str147[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str154[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str154[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str155[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str155[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str156[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 6)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str157[Lang.Idx]},
			},
		},
    },
    NextSteps = {12},
    ClearanceStep = 18,
}

STEP[10] =                                                        --16进8擂台8
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{4344,1076,1048576},{3861,1087,1048576},{3848,633,1048576},{4331,616,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str147[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str154[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str154[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str155[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str155[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str156[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1,Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1,Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(16, 2)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str157[Lang.Idx]},
			},
		},
    },
    NextSteps = {11},
    ClearanceStep = 18,
}

STEP[11] =                                                        --8进4擂台1
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{4344,1076,1048576},{3861,1087,1048576},{3848,633,1048576},{4331,616,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str158[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str159[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str159[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str160[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str160[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str161[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(8, 1)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str162[Lang.Idx]},
			},
		},
    },
    NextSteps = {15},
    ClearanceStep = 18,
}

STEP[12] =                                                        --8进4擂台2
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{7571,1079,1048576},{7137,1101,1048576},{7129,625,1048576},{7562,609,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str158[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str159[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str159[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str160[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str160[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str161[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1,Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(8, 3)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str162[Lang.Idx]},
			},
		},
    },
    NextSteps = {16},
    ClearanceStep = 18,
}

STEP[13] =                                                        --8进4擂台1
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{7593,4339,1048576},{7147,4338,1048576},{7142,3851,1048576},{7580,3823,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str158[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str159[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str159[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str160[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str160[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str161[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(8, 2)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str162[Lang.Idx]},
			},
		},
    },
    NextSteps = {15},
    ClearanceStep = 18,
}

STEP[14] =                                                        --8进4擂台4
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{7142,7089,1048576},{7128,7585,1048576},{7617,7594,1048576},{7633,7093,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str158[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str159[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str159[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str160[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str166[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str161[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(8, 4)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str162[Lang.Idx]},
			},
		},
    },
    NextSteps = {16},
    ClearanceStep = 18,
}

STEP[15] =                                                        --4进2擂台1
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{4344,1076,1048576},{3861,1087,1048576},{3848,633,1048576},{4331,616,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str163[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str164[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str164[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str165[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str166[Lang.Idx], 1}},
		},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str167[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1,Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(4, 1)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str168[Lang.Idx]},
			},
		},
    },
    NextSteps = {17},
    ClearanceStep = 18,
}

STEP[16] =                                                        --4进2擂台1
{
	Time = 140,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{7571,1079,1048576},{7137,1101,1048576},{7129,625,1048576},{7562,609,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str163[Lang.Idx]},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str164[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str164[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str165[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str166[Lang.Idx], 1}},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str167[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1,Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(4, 2)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str168[Lang.Idx]},
			},
		},
    },
    NextSteps = {17},
    ClearanceStep = 18,
}

STEP[17] =                                                        --4进2擂台1
{
	Time = 140,                                              --时间
	NeedTransport = 1,
  AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 比赛结束不用传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2,
	Type = Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG,
	Pos = {338,{3978,4097,1048576},{3887,4449,1048576},{4222,4574,1048576},{4399,4390,1048576},{4309,4116,1048576}},
	NPC =
	{
	 		[1] = {nTemplate = 2537, nLevel = -1, nSeries = -1},		--旗子
	 		[3] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap1
 		  [4] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap2
	},
    LOCK =
    {
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str169[Lang.Idx]},
		  	{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str170[Lang.Idx],1},
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str171[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str171[Lang.Idx], 1}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str172[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str172[Lang.Idx], 1}},
						 
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 1, "trap1", "trap1"},  --刷出trap1
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 1, "trap2", "trap2"},  --刷出trap2	
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str169[Lang.Idx]},
			},
		},
		[2] = {nTime = 137, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str151[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str151[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[10].Enemy1, 0, 0, 0, 0}},
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SetGameProgress(2, 1)"},
				{Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK,2 ,0},
			},
			tbUnLockEvent =
			{
				--{Activity.EVENT_TYPE.JOIN_STEP ,10, 18, 0, 0}, --进入到18的步骤

				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str173[Lang.Idx],1},		 
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 2, "qizi", "qizi"},  --刷出旗子
				{Activity.EVENT_TYPE.SET_TRAP_EFFECT, "bwc_trap1", 0},  --删除trap1
				{Activity.EVENT_TYPE.SET_TRAP_EFFECT, "bwc_trap2", 0},  --删除trap2
			  {Activity.EVENT_TYPE.DEL_NPC, "trap1"}, --删除trap1
			  {Activity.EVENT_TYPE.DEL_NPC, "trap2"}, --删除trap2
				
			},
		},
    },
    NextSteps = {18},
    ClearanceStep = 18,
}

STEP[18] =                                                        --淘汰赛休息的锁
{
	Time = 680,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 比赛结束传到下一个步骤
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.BASE,
	--Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {338,{4659,3144,1048576},{4959,3381,1048576},{5160,3626,1048576},{4348,2379,1048576},{4007,2432,1048576},{3604,3162,1048576},{3425,3011,1048576}},
	NPC =
	{
							-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2536, nLevel = -1, nSeries = -1},		--自杀npc
  	[3] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap1
 		[4] = {nTemplate = 2538, nLevel = -1, nSeries = -1},		--trap2

	},
    LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		--缓冲一下。
			tbPrelock = {},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 139, nNum = 0,		-- 16进8
			tbPrelock = {1},
			tbStartEvent =
			{
			    {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str174[Lang.Idx], 2},
			   	{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str174[Lang.Idx]}, 2},
			    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str175[Lang.Idx], 2},
				  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str175[Lang.Idx], 2}},
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 140, nNum = 0,		-- 8进4
			tbPrelock = {2},
			tbStartEvent =
			{
			    {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str174[Lang.Idx], 3},
			   	{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str174[Lang.Idx]}, 3},
			    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str176[Lang.Idx], 3},
				  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str176[Lang.Idx], 3}},
			},
			tbUnLockEvent =
			{

			},
		},
		[4] = {nTime = 140, nNum = 0,		-- 4进2
			tbPrelock = {3},
			tbStartEvent =
			{
			    {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str174[Lang.Idx], 4},
			   	{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str174[Lang.Idx]}, 4},
			    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str177[Lang.Idx], 4},
				  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str177[Lang.Idx], 4}},
			},
			tbUnLockEvent =
			{

			},
		},		
		[5] = {nTime = 140, nNum = 0,		-- 4进2
			tbPrelock = {4},
			tbStartEvent =
			{
			    {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str174[Lang.Idx], 5},
			   	{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str174[Lang.Idx]}, 5},
			    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str178[Lang.Idx], 5},
				  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str178[Lang.Idx], 5}},
			},
			tbUnLockEvent =
			{

			},
		},			
    [6] = {nTime = 27, nNum = 0,		--缓冲一下。
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 530, nNum = 0,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
					{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str179[Lang.Idx],1},
				  {Activity.EVENT_TYPE.ADD_NPC, 1, 20, 7, "baoxiang", "baoxiang", 14, 30, "baoxiang_timer1"},  --定时刷出战魂包
			},
			tbUnLockEvent =
			{

			},
		},
		[8] = {nTime = 120, nNum = 0,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			    {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 8},
			   	{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx]}, 8},
			    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str181[Lang.Idx],8},
			   	{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str181[Lang.Idx], 8}},
				 
			},
			tbUnLockEvent =
			{
				  {Activity.EVENT_TYPE.DEL_NPC, "qizi"},
				  {Activity.EVENT_TYPE.DEL_NPC, "baoxiang"},

			},
		},
		[9] = {nTime = 419, nNum = 0,		-- add trap点的锁
			tbPrelock = {1},
			tbStartEvent =
			{
				  {Activity.EVENT_TYPE.ADD_NPC, 3, 1, 9, "trap1", "trap1"},  --刷出trap1
				  {Activity.EVENT_TYPE.ADD_NPC, 4, 1, 9, "trap2", "trap2"},  --刷出trap2
			},
			tbUnLockEvent =
			{
			    {Activity.EVENT_TYPE.DEL_NPC, "trap1"}, --删除trap1
			    {Activity.EVENT_TYPE.DEL_NPC, "trap2"}, --删除trap2
			},
		},
		
    },
    NextSteps = {},
}
