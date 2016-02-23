-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[21] =
{
	nRequireLevel = 25,                -- 需求等级
	tbOpenTimes = {{{ wday=1,hour=14, min=30, sec=0}, {wday=1,hour=15, min=00, sec=5}},{{ wday=1,hour=19, min=30, sec=0}, {wday=1,hour=20, min=0, sec=5}},{{ wday=2,hour=14, min=30, sec=0}, {wday=2,hour=15, min=0, sec=5}},{{ wday=2,hour=19, min=30, sec=0}, {wday=2,hour=20, min=0, sec=5}},{{ wday=4,hour=14, min=30, sec=0}, {wday=4,hour=15, min=0, sec=5}},{{ wday=4,hour=19, min=30, sec=0}, {wday=4,hour=20, min=0, sec=5}},{{ wday=6,hour=14, min=30, sec=0}, {wday=6,hour=15, min=0, sec=5}},{{ wday=6,hour=19, min=30, sec=0}, {wday=6,hour=20, min=0, sec=5}}},	--开放时间{StartTime, EndTime},9分钟，开几次，填几组时间, 每组时间不能重叠
	ActivityName = Lang.mission.str88[Lang.Idx],        -- 活动名字，输出信息用
	nStartStep = 1,			   	          -- 开始步骤，记录进入位置
	tbEnterPos = { nMapId = 345, tbPos = {{1726,2280,1048576}}},		-- 准备场进入点
	nPrepareTime = 180,						-- 报名阶段时间
	nReadyStartTime = 2,					-- 准备场在活动开始后多久开启
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
--类型 ：全部，			自身，			npc，			队友，			同家族成员，	同服务器玩家
--类型1：All,			Self,			Npc,			Team,			Kin,			Server
--类型2：All_Enable,	Self_Enable,	Npc_Enable,		Team_Enable,	Kin_Enable,		Server_Enable

--类型3：Not

--type_enable填写时则该类判定生效，否则不对此项内容做判定
--type_enable填写了的情况下，不填写对应type，含义为，该项不为敌人
--all包含self，如果填写了all一定要把self去掉
--type_not的含义为，除了填写的类型外的单位为敌人

--填写方式为相加的方式：Type_All + Type_Self_Enable + Type_Team_Enable + Type_Not
ActivitySetting[21].Enemy1 = Team + Team_Enable + Not;  --非队友的人全都是敌人
--ActivitySetting[2].Enemy2 = 0;

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[21].STEP


STEP[1] =
{
	Time = 210,                                              --时间   3.5分钟
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 100,                                          -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.ZHUNBEICHANG,                   -- 步骤类型
	--Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {345,{1155,2074,1048576},{1083,2657,1048576},{1452,2931,1048576},{1946,2686,1048576},{2123,2336,1048576},{1638,1716,1048576},{1500,2352,1048576}},                         --玩家默认传送的位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)

	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时 准备场等待2秒
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},

		[2] = {nTime = 179, nNum = 0,
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str89[Lang.Idx]},	--准备场开启，发公告
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str90[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str91[Lang.Idx]}, 2},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str92[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str93[Lang.Idx], 2}},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 30, nNum = 0,
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str94[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str95[Lang.Idx]}, 3},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str96[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str97[Lang.Idx], 3}},

			},
			tbUnLockEvent =
			{
			},
		},
    },
    NextSteps = {2},
}


STEP[2] =                                                        --世界boss准备场
{
	Time = 1560,                                              	  --时间  26分钟
	NeedTransport = 0,									                          -- 准备场手动传送
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,          --随机分配
	GroupType = Activity.GROUP_TYPE.FIGHT_SCORE_GROUP,			      -- 战力分组
	GroupNum = 2,
	MaxPlayer = 100,
	Type = Activity.STEP_TYPE.BATTLE_FIELD,
	Pos = {349,{4501,1107,1048576},{4497,7368,1048576}},
	NPC =
	{
		-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2540, nLevel = -1, nSeries = -1},		--神族基地
		[2] = {nTemplate = 2541, nLevel = -1, nSeries = -1},		--魔族基地
		[3] = {nTemplate = 2546, nLevel = -1, nSeries = -1},		--神族炮塔中1
 		[4] = {nTemplate = 2547, nLevel = -1, nSeries = -1},		--神族炮塔中2
 		[5] = {nTemplate = 2548, nLevel = -1, nSeries = -1},		--神族炮塔左1
 		[6] = {nTemplate = 2549, nLevel = -1, nSeries = -1},		--神族炮塔左2
 		[7] = {nTemplate = 2550, nLevel = -1, nSeries = -1},		--神族炮塔右1
 		[8] = {nTemplate = 2551, nLevel = -1, nSeries = -1},		--神族炮塔右2
 		[9] = {nTemplate = 2552, nLevel = -1, nSeries = -1},		--魔族炮塔中1
 		[10] = {nTemplate = 2553, nLevel = -1, nSeries = -1},		--魔族炮塔中2
 		[11] = {nTemplate = 2554, nLevel = -1, nSeries = -1},		--魔族炮塔左1
 		[12] = {nTemplate = 2555, nLevel = -1, nSeries = -1},		--魔族炮塔左2
 		[13] = {nTemplate = 2556, nLevel = -1, nSeries = -1},		--魔族炮塔右1
 		[14] = {nTemplate = 2557, nLevel = -1, nSeries = -1},		--魔族炮塔右2
 		[15] = {nTemplate = 2571, nLevel = -1, nSeries = -1},		--神族npc 神将
 		[16] = {nTemplate = 2576, nLevel = -1, nSeries = -1},		--神族npc 花仙子
 		[17] = {nTemplate = 2589, nLevel = -1, nSeries = -1},		--魔族npc 魔将
 		[18] = {nTemplate = 2594, nLevel = -1, nSeries = -1},		--魔族npc 毒箭手

	},
  LOCK =
    {
	-- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 1秒缓冲
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 29, nNum = 0,		-- 总计时 准备场等待29秒
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str98[Lang.Idx],1},	--准备场开启，发公告
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str99[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str99[Lang.Idx]}, 2},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str100[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str101[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 2, "jidi", "jidi"},  								  --神族基地
				{Activity.EVENT_TYPE.ADD_NPC, 2, 1, 2, "jidi", "jidi"},  								  --魔族基地
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 2, "middletower1", "middletower1"},   --神族炮塔中1
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 2, "middletower2", "middletower2"},   --神族炮塔中2
				{Activity.EVENT_TYPE.ADD_NPC, 5, 1, 2, "shangtower1", "shangtower1"},  		  --神族炮塔左1
				{Activity.EVENT_TYPE.ADD_NPC, 6, 1, 2, "shangtower2", "shangtower2"},  		  --神族炮塔左2
				{Activity.EVENT_TYPE.ADD_NPC, 6, 1, 2, "shangtower3", "shangtower3"},  		  --神族炮塔左3
				{Activity.EVENT_TYPE.ADD_NPC, 6, 1, 2, "shangtower4", "shangtower4"},  		  --神族炮塔左4
				{Activity.EVENT_TYPE.ADD_NPC, 7, 1, 2, "xiatower1", "xiatower1"},     --神族炮塔右1
				{Activity.EVENT_TYPE.ADD_NPC, 8, 1, 2, "xiatower2", "xiatower2"},     --神族炮塔右2
				{Activity.EVENT_TYPE.ADD_NPC, 8, 1, 2, "xiatower3", "xiatower3"},     --神族炮塔右3
				{Activity.EVENT_TYPE.ADD_NPC, 8, 1, 2, "xiatower4", "xiatower4"},     --神族炮塔右4
				
				{Activity.EVENT_TYPE.ADD_NPC, 9, 1, 2, "middletower1", "middletower1"},   --魔族炮塔中1
				{Activity.EVENT_TYPE.ADD_NPC, 10, 1, 2, "middletower2", "middletower2"},  --魔族炮塔中2
				{Activity.EVENT_TYPE.ADD_NPC, 11, 1, 2, "shangtower1", "shangtower1"},  		--魔族炮塔左1
				{Activity.EVENT_TYPE.ADD_NPC, 12, 1, 2, "shangtower2", "shangtower2"},  		--魔族炮塔左2
				{Activity.EVENT_TYPE.ADD_NPC, 12, 1, 2, "shangtower3", "shangtower3"},  		--魔族炮塔左3
				{Activity.EVENT_TYPE.ADD_NPC, 12, 1, 2, "shangtower4", "shangtower4"},  		--魔族炮塔左4
				{Activity.EVENT_TYPE.ADD_NPC, 13, 1, 2, "xiatower1", "xiatower1"},    --魔族炮塔右1
				{Activity.EVENT_TYPE.ADD_NPC, 14, 1, 2, "xiatower2", "xiatower2"},    --魔族炮塔右2
				{Activity.EVENT_TYPE.ADD_NPC, 14, 1, 2, "xiatower3", "xiatower3"},    --魔族炮塔右3
				{Activity.EVENT_TYPE.ADD_NPC, 14, 1, 2, "xiatower4", "xiatower4"},    --魔族炮塔右4


				{Activity.EVENT_TYPE.REVIVE_INFO, 1 ,false, true, true, 0,{4048,970,1048576},{4289,1139,1048576},{4594,1123,1048576},{4851,897,1048576},{4989,990,1048576}}, --神族复活点
		    {Activity.EVENT_TYPE.REVIVE_INFO, 2 ,false, true, true, 0,{3929,7641,1048576},{4136,7497,1048576},{4416,7450,1048576},{4695,7455,1048576},{4842,7588,1048576}}, --魔族复活点
			},
			tbUnLockEvent =
			{

			},
		},
  [3] = {nTime = 15, nNum = 0,		-- 15秒缓冲
			tbPrelock = {1},
			tbStartEvent =
			{
        {Activity.EVENT_TYPE.ADD_NPC, 15, 3, 3, "shangnpc1", "shangnpc1",},  						--神族 左1神将  5只
        {Activity.EVENT_TYPE.ADD_NPC, 16, 1, 3, "shangnpc1", "shangnpc1",},  						--神族 左1花仙子 1只
        {Activity.EVENT_TYPE.ADD_NPC, 15, 3, 3, "shangnpc2", "shangnpc2",},  						--神族 左2神将  5只
        {Activity.EVENT_TYPE.ADD_NPC, 16, 1, 3, "shangnpc2", "shangnpc2",},  						--神族 左2花仙子 1只
        {Activity.EVENT_TYPE.ADD_NPC, 15, 3, 3, "shangnpc3", "shangnpc3",},      			  --神族 左3神将  5只
        {Activity.EVENT_TYPE.ADD_NPC, 16, 1, 3, "shangnpc3", "shangnpc3",},  						--神族 左3花仙子 1只

				{Activity.EVENT_TYPE.ADD_NPC, 15, 3, 3, "middlenpc1", "middlenpc1",},  			  	--神族 中1神将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 16, 1, 3, "middlenpc1", "middlenpc1",},  			  	--神族 中1花仙子 1只

				{Activity.EVENT_TYPE.ADD_NPC, 15, 3, 3, "xianpc1", "xianpc1",},  			  	--神族 右1神将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 16, 1, 3, "xianpc1", "xianpc1",},  			  	--神族 右1花仙子 1只
				{Activity.EVENT_TYPE.ADD_NPC, 15, 3, 3, "xianpc2", "xianpc2",},  			  	--神族 右2神将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 16, 1, 3, "xianpc2", "xianpc2",},  			  	--神族 右2花仙子 1只
				{Activity.EVENT_TYPE.ADD_NPC, 15, 3, 3, "xianpc3", "xianpc3",},  			  	--神族 右3神将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 16, 1, 3, "xianpc3", "xianpc3",},  			  	--神族 右3花仙子 1只
				
				
			  {Activity.EVENT_TYPE.ADD_NPC, 17, 3, 3, "shangnpc1", "shangnpc1",},  						--魔族 左1魔将  5只
        {Activity.EVENT_TYPE.ADD_NPC, 18, 1, 3, "shangnpc1", "shangnpc1",},  						--魔族 左1毒箭手 1只
			  {Activity.EVENT_TYPE.ADD_NPC, 17, 3, 3, "shangnpc2", "shangnpc2",},  						--魔族 左2魔将  5只
        {Activity.EVENT_TYPE.ADD_NPC, 18, 1, 3, "shangnpc2", "shangnpc2",},  						--魔族 左2毒箭手 1只
        {Activity.EVENT_TYPE.ADD_NPC, 17, 3, 3, "shangnpc3", "shangnpc3",},        			--魔族 左3魔将  5只
        {Activity.EVENT_TYPE.ADD_NPC, 18, 1, 3, "shangnpc3", "shangnpc3",},  						--魔族 左3毒箭手 1只

				{Activity.EVENT_TYPE.ADD_NPC, 17, 3, 3, "middlenpc1", "middlenpc1",},  			  	--魔族 中1魔将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 18, 1, 3, "middlenpc1", "middlenpc1",},  			  	--魔族 中1毒箭手 1只

				{Activity.EVENT_TYPE.ADD_NPC, 17, 3, 3, "xianpc1", "xianpc1",},  			  	--魔族 右1魔将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 18, 1, 3, "xianpc1", "xianpc1",},  			  	--魔族 右1毒箭手 1只
				{Activity.EVENT_TYPE.ADD_NPC, 17, 3, 3, "xianpc2", "xianpc2",},  			  	--魔族 右2魔将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 18, 1, 3, "xianpc2", "xianpc2",},  			  	--魔族 右2毒箭手 1只
				{Activity.EVENT_TYPE.ADD_NPC, 17, 3, 3, "xianpc3", "xianpc3",},  			  	--魔族 右3魔将  5只
				{Activity.EVENT_TYPE.ADD_NPC, 18, 1, 3, "xianpc3", "xianpc3",},  			  	--魔族 右3毒箭手 1只
			},
			tbUnLockEvent =
			{

			},
		},
	[4] = {nTime = 1530, nNum = 0,		--神魔战场开始   26分钟
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str102[Lang.Idx],1},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str103[Lang.Idx], 4},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str103[Lang.Idx]},4},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str104[Lang.Idx], 4},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str104[Lang.Idx], 4}},
			},
			tbUnLockEvent =
			{

			},
		},

    },
    NextSteps = {3},
}

STEP[3] =
{
	Time = 30,                                              	    --时间  0.5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,          --随机分配
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,
	MaxPlayer = 100,
	NeedTransport = 0;
	Type = Activity.STEP_TYPE.ZHUNBEICHANG,                   -- 步骤类型
	Pos = {349,{6650,1688,1048576}},
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime =30, nNum = 0,		-- 总计时 准备场等待2秒
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str106[Lang.Idx], 1},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str106[Lang.Idx]}, 1},
			},
			tbUnLockEvent =
			{

			},
		},

    },
    NextSteps = {},
}
