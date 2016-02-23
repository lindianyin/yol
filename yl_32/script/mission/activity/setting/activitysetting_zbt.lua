-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[2] =
{
	nRequireLevel = 25,          -- 需求等级
	tbOpenTimes = {{{wday=1,hour=10, min=30, sec=0}, {wday=1,hour=11, min=0, sec=5}},{{wday=1,hour=20, min=30, sec=0}, {wday=1,hour=21, min=0, sec=5}}, {{wday=2,hour=10, min=30, sec=0}, {wday=2,hour=11, min=0, sec=5}},{{wday=2,hour=20, min=30, sec=0}, {wday=2,hour=21, min=0, sec=5}},{{wday=4,hour=10, min=30, sec=0}, {wday=4,hour=11, min=0, sec=5}},{{wday=4,hour=20, min=30, sec=0}, {wday=4,hour=21, min=0, sec=5}},{{wday=6,hour=10, min=30, sec=0}, {wday=6,hour=11, min=0, sec=5}},{{wday=6,hour=20, min=30, sec=0}, {wday=6,hour=21, min=0, sec=5}}},	--开放时间{StartTime, EndTime},开几次，填几组时间, 每组时间不能重叠
	ActivityName = "战宝堂",  -- 活动名字，输出信息用
	nStartStep = 1,			   				-- 开始步骤
	tbEnterPos = { nMapId = 330, tbPos = {{2337,3105,1048576},{1582,2407,1048576},{2322,1715,1048576}}},		-- 进入点
	nPrepareTime = 180,						-- 报名阶段时间
	nReadyStartTime = 0,					-- 准备场在活动开始后多久开启
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

--填写方式为相加的方式：Type_All + Ty pe_Self_Enable + Type_Team_Enable + Type_Not
ActivitySetting[2].Enemy1 = Team + Team_Enable + Kin + Kin_Enable + Not;  --非队友和非家族成员的人全都是敌人
--ActivitySetting[2].Enemy2 = 0;

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[2].STEP
STEP[1] =
{
	Time = 1,                                                --时间
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                             -- 小队优先的帮会模式
	MaxPlayer = 2000,                                          -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.ZHANBAOTANG_ZHUNBEICHANG,       -- 步骤类型
	Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {330,{2346,2347,1048576}},                          --玩家默认传送的位置
	NPC =
	{
	
	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时 准备场等待60秒
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str196[Lang.Idx]},	--准备场开启，发公告
			},
		},
    },
    NextSteps = {2,3,4,5,6,7,8,9},
}

STEP[2] =                                                  -- 战宝堂准备场
{
	Time = 1800,                                              -- 整个战宝堂时间 20分钟
	NeedTransport = 0,										-- 准备场手动传送
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.BASE,
	Pos = {330,{2337,3105,1048576},{1582,2407,1048576},{2322,1715,1048576},{2992,2384,1048576}},
	NPC =
	{
		-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
		[1] = {nTemplate = 2520, nLevel = -1, nSeries = -1},		--门1
		[2] = {nTemplate = 2521, nLevel = -1, nSeries = -1},		--门2
		[3] = {nTemplate = 2522, nLevel = -1, nSeries = -1},		--门3
		[4] = {nTemplate = 2523, nLevel = -1, nSeries = -1},		--门4

	},
  LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 准备场
			tbPrelock = {1},
			tbStartEvent =
			{

				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{0, 0, 0, 0, 0}}},
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str197[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str197[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str198[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str198[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 1, "men", "men1"},
			  {Activity.EVENT_TYPE.ADD_NPC, 2, 1, 1, "men", "men2"},
			  {Activity.EVENT_TYPE.ADD_NPC, 3, 1, 1, "men", "men3"},
			  {Activity.EVENT_TYPE.ADD_NPC, 4, 1, 1, "men", "men4"},
			},
			tbUnLockEvent =
			{
				
			},
		},
		[3] = {nTime = 1620, nNum = 0,		-- 战宝堂已经开始，时间倒数
			tbPrelock = {2},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str218[Lang.Idx]},
			  {Activity.EVENT_TYPE.REDUCE_ALL_DEGREE},  --扣1次次数
				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str219[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str219[Lang.Idx], 3}},
		    {Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}},
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}

STEP[3] =                                                        --战宝堂一层1
{
	Time = 1800,                                             --  时间5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG,
	BossTemplateId = 2530,																	--战宝堂战斗场中要加这个boss的templateid
	Pos = {331,{3136,881,1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2530, nLevel = -1, nSeries = -1},		-- 一层boss
 		[2] = {nTemplate = 2533, nLevel = -1, nSeries = -1},		-- 小怪
 		[3] = {nTemplate = 2524, nLevel = -1, nSeries = -1},		-- 门
	},
    LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{ 

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 报名时段
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},

				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str197[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str197[Lang.Idx], 2}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str198[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str198[Lang.Idx], 2}},
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1620, nNum = 10000,		-- 总计时，并且刷出小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.REDUCE_ALL_DEGREE},  --扣1次次数
				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str199[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str199[Lang.Idx], 3}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 3}, --时间显示
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, --时间显示

				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster1"},  --刷出小怪1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster2"},  --刷出小怪3
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster3"},  --刷出小怪5
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster4"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster5"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster6"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster7"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster8"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster9"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster10"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster11"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster12"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster13"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster14"},  --刷出小怪7
				

				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},
			},
			tbUnLockEvent =
			{

			},
		},
		 [4] = {nTime = 120, nNum = 0,		-- 时间倒计时，过2分钟，1层boss刷出
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 刷boss，时间填无限大
			tbPrelock = {4},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str200[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str200[Lang.Idx], 5}},

				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 5, "boss", "boss"},  --刷出1层boss
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str220[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str220[Lang.Idx]},
			},
			tbUnLockEvent =
			{
		  	{Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward()"}, --击杀boss发奖励
       	{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str221[Lang.Idx]},
			},
		},
		 [6] = {nTime = 0, nNum = 1,		-- boss被击杀后，刷出门！
			tbPrelock = {5},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str203[Lang.Idx], 6},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str203[Lang.Idx], 6}},

			  {Activity.EVENT_TYPE.ADD_NPC, 3, 1, 6, "men", "men"},  --刷出2层的门
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}

STEP[4] =                                                     --战宝堂一层2
{
	Time = 1800,                                             --  时间5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG,
	BossTemplateId = 2530,																	--战宝堂战斗场中要加这个boss的templateid
	Pos = {332,{3136,881,1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2530, nLevel = -1, nSeries = -1},		-- 一层boss
 		[2] = {nTemplate = 2533, nLevel = -1, nSeries = -1},		-- 小怪
 	  [3] = {nTemplate = 2525, nLevel = -1, nSeries = -1},		-- 门
	},
    LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 报名时段
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},

				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str197[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str197[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str198[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str198[Lang.Idx], 2}},
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1620, nNum = 10000,		-- 总计时，并且刷出小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.REDUCE_ALL_DEGREE},  --扣1次次数
				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str199[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str199[Lang.Idx], 3}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 3}, --时间显示
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, --时间显示

				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster1"},  --刷出小怪1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster2"},  --刷出小怪3
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster3"},  --刷出小怪5
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster4"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster5"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster6"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster7"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster8"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster9"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster10"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster11"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster12"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster13"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster14"},  --刷出小怪7

				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},
			},
			tbUnLockEvent =
			{
				--{Activity.EVENT_TYPE.DEL_ALL_NPC}, --DEL_NPC不能删除重生的npc对象
			},
		},
		 [4] = {nTime = 120, nNum = 0,		-- 时间倒计时，过2分钟，1层boss刷出
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 刷boss，时间填无限大
			tbPrelock = {4},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str200[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str200[Lang.Idx], 5}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 5, "boss", "boss"},  --刷出1层boss
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str222[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str222[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			  {Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward()"}, --击杀boss发奖励
       	{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str223[Lang.Idx]},
			},
		},
		 [6] = {nTime = 0, nNum = 1,		-- boss被击杀后，刷出门！
			tbPrelock = {5},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str203[Lang.Idx], 6},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str203[Lang.Idx], 6}},
				
			  {Activity.EVENT_TYPE.ADD_NPC, 3, 1, 6, "men", "men"},  --刷出2层的门
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}

STEP[5] =                                                     --战宝堂一层2
{
	Time = 1800,                                             --  时间5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG,
	BossTemplateId = 2530,																	--战宝堂战斗场中要加这个boss的templateid
	Pos = {333,{3136,881,1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2530, nLevel = -1, nSeries = -1},		-- 一层boss
 		[2] = {nTemplate = 2533, nLevel = -1, nSeries = -1},		-- 小怪
    [3] = {nTemplate = 2526, nLevel = -1, nSeries = -1},		-- 门
	},
    LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 报名时段
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},

				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str197[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str197[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str198[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str198[Lang.Idx], 2}},
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1620, nNum = 10000,		-- 总计时，并且刷出小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.REDUCE_ALL_DEGREE},  --扣1次次数
				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str199[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str199[Lang.Idx], 3}},

				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 3}, --时间显示
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, --时间显示

				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster1"},  --刷出小怪1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster2"},  --刷出小怪3
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster3"},  --刷出小怪5
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster4"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster5"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster6"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster7"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster8"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster9"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster10"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster11"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster12"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster13"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster14"},  --刷出小怪7

				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},
			},
			tbUnLockEvent =
			{
				
			},
		},
		 [4] = {nTime = 120, nNum = 0,		-- 时间倒计时，过2分钟，1层boss刷出
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 刷boss，时间填无限大
			tbPrelock = {4},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str200[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str200[Lang.Idx], 5}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 5, "boss", "boss"},  --刷出1层boss
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str201[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str201[Lang.Idx]},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward()"}, --击杀boss发奖励
       	{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str202[Lang.Idx]},
			},
		},
		 [6] = {nTime = 0, nNum = 1,		-- boss被击杀后，刷出门！
			tbPrelock = {5},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str203[Lang.Idx], 6},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str203[Lang.Idx], 6}},
				
			  {Activity.EVENT_TYPE.ADD_NPC, 3, 1, 6, "men", "men"},  --刷出2层的门
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}

STEP[6] =                                                     --战宝堂一层2
{
	Time = 1800,                                             --  时间5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG,
	BossTemplateId = 2530,																	--战宝堂战斗场中要加这个boss的templateid
	Pos = {334,{3136,881,1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2530, nLevel = -1, nSeries = -1},		-- 一层boss
 		[2] = {nTemplate = 2533, nLevel = -1, nSeries = -1},		-- 小怪
  	[3] = {nTemplate = 2527, nLevel = -1, nSeries = -1},		-- 门
	},
     LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 报名时段
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},

				{Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str197[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str197[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str198[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str198[Lang.Idx], 2}},
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1620, nNum = 10000,		-- 总计时，并且刷出小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.REDUCE_ALL_DEGREE},  --扣1次次数
				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str199[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str199[Lang.Idx], 3}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 3}, --时间显示
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, --时间显示

				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster1"},  --刷出小怪1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster2"},  --刷出小怪3
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster3"},  --刷出小怪5
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster4"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster5"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster6"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster7"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster8"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster9"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster10"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster11"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster12"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster13"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster14"},  --刷出小怪7

				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},
			},
			tbUnLockEvent =
			{
			
			},
		},
		 [4] = {nTime = 120, nNum = 0,		-- 时间倒计时，过2分钟，1层boss刷出
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 刷boss，时间填无限大
			tbPrelock = {4},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str200[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str200[Lang.Idx], 5}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 5, "boss", "boss"},  --刷出1层boss
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str204[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str204[Lang.Idx]},

			},
			tbUnLockEvent =
			{
		   	{Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward()"}, --击杀boss发奖励
       	{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str205[Lang.Idx]},
			},
		},
		 [6] = {nTime = 0, nNum = 1,		-- boss被击杀后，刷出门！
			tbPrelock = {5},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str203[Lang.Idx], 6},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str203[Lang.Idx], 6}},
				
			  {Activity.EVENT_TYPE.ADD_NPC, 3, 1, 6, "men", "men"},  --刷出2层的门
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}
STEP[7] =                                                     --战宝堂二层1
{
	Time = 1800,                                             --  时间5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG,
	BossTemplateId = 2531,																	--战宝堂战斗场中要加这个boss的templateid
	Pos = {335,{3136,881,1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2531, nLevel = -1, nSeries = -1},		-- 二层boss
 		[2] = {nTemplate = 2534, nLevel = -1, nSeries = -1},		-- 小怪
 		[3] = {nTemplate = 2528, nLevel = -1, nSeries = -1},		-- 门
	},
    LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 报名时段
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1620, nNum = 10000,		-- 总计时，并且刷出小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},


				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str206[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str206[Lang.Idx], 3}},

				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 3}, --时间显示
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, --时间显示
				
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster1"},  --刷出小怪1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster2"},  --刷出小怪3
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster3"},  --刷出小怪5
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster4"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster5"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster6"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster7"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster8"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster9"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster10"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster11"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster12"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster13"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster14"},  --刷出小怪7
			},
			tbUnLockEvent =
			{
				--{Activity.EVENT_TYPE.DEL_ALL_NPC}, --DEL_NPC不能删除重生的npc对象
			},
		},
		 [4] = {nTime = 420, nNum = 0,		-- 时间倒计时，过2分钟，1层boss刷出
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 刷boss，时间填无限大
			tbPrelock = {4},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str207[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str207[Lang.Idx], 5}},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 5, "boss", "boss"},  --刷出2层boss
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str208[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str208[Lang.Idx]},
				
			},
			tbUnLockEvent =
			{
			  {Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward()"}, --击杀boss发奖励
       	{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str209[Lang.Idx]},

			},
		},
		 [6] = {nTime = 0, nNum = 1,		-- boss被击杀后，刷出门！
			tbPrelock = {5},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str210[Lang.Idx], 6},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str210[Lang.Idx],6}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1,6, "men", "men"},  --刷出3层的门
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}
STEP[8] =                                                     --战宝堂二层2
{
	Time = 1800,                                             --  时间5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG,
	BossTemplateId = 2531,																	--战宝堂战斗场中要加这个boss的templateid
	Pos = {336,{3136,881,1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2531, nLevel = -1, nSeries = -1},		-- 二层boss
 		[2] = {nTemplate = 2534, nLevel = -1, nSeries = -1},		-- 小怪
 		[3] = {nTemplate = 2529, nLevel = -1, nSeries = -1},		-- 门
	},
    LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 报名时段
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1620, nNum = 10000,		-- 总计时，并且刷出小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},

				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str206[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str206[Lang.Idx], 3}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str180[Lang.Idx], 3}, --时间显示
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, --时间显示

				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster1"},  --刷出小怪1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster2"},  --刷出小怪3
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster3"},  --刷出小怪5
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster4"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster5"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster6"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster7"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster8"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster9"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster10"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster11"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster12"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster13"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster14"},  --刷出小怪7
			},
			tbUnLockEvent =
			{
				--{Activity.EVENT_TYPE.DEL_ALL_NPC}, --DEL_NPC不能删除重生的npc对象
			},
		},
		 [4] = {nTime = 420, nNum = 0,		-- 时间倒计时，过2分钟，1层boss刷出
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 刷boss，时间填无限大
			tbPrelock = {4},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str211[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str211[Lang.Idx], 5}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 5, "boss", "boss"},  --刷出2层boss
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str212[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str212[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			  {Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward()"}, --击杀boss发奖励
       	{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str213[Lang.Idx]},

			},
		},
		 [6] = {nTime = 0, nNum = 1,		-- boss被击杀后，刷出门！
			tbPrelock = {5},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str210[Lang.Idx], 6},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str210[Lang.Idx],6}},
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1,6, "men", "men"},  --刷出3层的门
			},
			tbUnLockEvent =
			{

			},
		},
    },
    NextSteps = {},
}

STEP[9] =                                                     --战宝堂三层
{
	Time = 1800,                                             --  时间5分钟
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG,
	BossTemplateId = 2532,																	--战宝堂战斗场中要加这个boss的templateid
	Pos = {337,{3136,881,1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2532, nLevel = -1, nSeries = -1},		-- 三层boss
 		[2] = {nTemplate = 2535, nLevel = -1, nSeries = -1},		-- 小怪
	},
    LOCK =
    {
    [1] = {nTime = 1, nNum = 0,		-- 空锁
			tbPrelock = {},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		[2] = {nTime = 179, nNum = 0,		-- 报名时段
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1620, nNum = 10000,		-- 总计时，并且刷出小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[2].Enemy1, 0, 0, 0, 0}}},

				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str214[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str214[Lang.Idx], 3}},
				
				{Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, 		--时间显示
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str180[Lang.Idx], 3}}, --时间显示

				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster1"},  --刷出小怪1
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster2"},  --刷出小怪3
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster3"},  --刷出小怪5
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster4"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster5"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster6"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster7"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster8"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster9"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster10"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster11"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster12"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster13"},  --刷出小怪7
				{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 3, "guaiwu", "monster14"},  --刷出小怪7
			},
			tbUnLockEvent =
			{
				--{Activity.EVENT_TYPE.DEL_ALL_NPC}, --DEL_NPC不能删除重生的npc对象
			},
		},
		 [4] = {nTime = 720, nNum = 0,		-- 时间倒计时，过2分钟，3层boss刷出
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 刷boss，时间填无限大
			tbPrelock = {4},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str215[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str215[Lang.Idx],5}},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1,5, "boss", "boss"},  --刷出3层boss
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str216[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str216[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			  {Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward()"}, --击杀boss发奖励
       	--{Activity.EVENT_TYPE.BLACK_MSG, -1, "战宝堂三层堂主[影魔]已被击杀！"},
			  {Activity.EVENT_TYPE.TARGET_INFO,-1,Lang.mission.str217[Lang.Idx], 5},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str217[Lang.Idx], 5}},
      },
		},
    },
    NextSteps = {},
}
