-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[15] =
{
	nRequireLevel = 25,          -- 需求等级
	tbOpenTimes = {{{wday=3,hour=10, min=30, sec=0}, {wday=3,hour=11, min=0, sec=5}},
					{{wday=3,hour=20, min=30, sec=0}, {wday=3,hour=21, min=0, sec=5}},
					{{wday=5,hour=10, min=30, sec=0}, {wday=5,hour=11, min=0, sec=5}},
					{{wday=5,hour=22, min=30, sec=0}, {wday=5,hour=23, min=0, sec=5}},
					{{wday=7,hour=10, min=30, sec=0}, {wday=7,hour=11, min=0, sec=5}},
					{{wday=7,hour=20, min=30, sec=0}, {wday=7,hour=21, min=0, sec=5}}},	--开放时间{StartTime, EndTime},开几次，填几组时间, 每组时间不能重叠
	ActivityName = Lang.mission.str108[Lang.Idx],  -- 活动名字，输出信息用
	nStartStep = 1,			   	-- 开始步骤，记录进入位置
	tbWorldBossRandSteps = {},    -- 世界boss活动随机的步骤
	nPrepareTime = 1799,						-- 报名阶段时间
	nReadyStartTime = 0,					-- 准备场在活动开始后多久开启
	tbEnterPos = { nMapId = 290, tbPos = {{1564,2804,1048576},{1909,1635,1048576},{2918,1224,1048576},{2687,2060,1048576}}},		-- 进入点
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
ActivitySetting[15].Enemy1 = Team + Team_Enable + Not;  --非队友的人全都是敌人
--ActivitySetting[2].Enemy2 = 0;

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[15].STEP
STEP[1] =
{
	Time = 1,                                              --时间
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,                                          -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.ZHUNBEICHANG,                   -- 步骤类型
	--Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {290,{2346,2347,1048576}},                         --玩家默认传送的位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
		[1] = {nTemplate = 2500, nLevel = -1, nSeries = -1},		--去东门
		[2] = {nTemplate = 2501, nLevel = -1, nSeries = -1},		--去南门
		[3] = {nTemplate = 2502, nLevel = -1, nSeries = -1},		--去西门
		[4] = {nTemplate = 2503, nLevel = -1, nSeries = -1},		--去北门

	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 1, "enterdoor", "east"},
				{Activity.EVENT_TYPE.ADD_NPC, 2, 1, 1, "enterdoor", "south"},
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 1, "enterdoor", "west"},
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 1, "enterdoor", "north"},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str109[Lang.Idx]},	--准备场开启，发公告
			},
		},
    },
    NextSteps = {2,3,4,5,6},
}

STEP[2] =                                                        --世界boss准备场
{
	Time = 1800,                                             --时间
	NeedTransport = 1,										-- 准备场手动传送
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.BASE,
	Pos = {290,{1564,2804,1048576},{1909,1635,1048576},{2918,1224,1048576},{2687,2060,1048576}},
	NPC =
	{

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
		 [2] = {nTime = 59 , nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{				
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{0, 0, 0, 0, 0}}},  --ALL, SELF, NPC, TEAM, KIN 分别对应的填写
		    
		    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str110[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1,Lang.mission.str111[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str112[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str112[Lang.Idx], 2}},
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 1740, nNum = 0,		-- 格斗准备场
			tbPrelock = {2},
			tbStartEvent =
			{
		    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str113[Lang.Idx],3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1,Lang.mission.str114[Lang.Idx], 3}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str115[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str115[Lang.Idx]}, 3},
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.REMOVE_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str116[Lang.Idx], 3}}
			},
		},
		[4] = {nTime = 1600 , nNum = 1,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str117[Lang.Idx]},
			},
		},
		[5] = {nTime = 1740, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{

			},
		},

    },
    NextSteps = {},
}

STEP[3] =
{
	Time = 1800,                                             --  时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.KUAFUBOSS_SAICHANG,
	WorldBossTemplateId = 2507,		--世界boss战斗场中要加这个boss的templateid
	Pos = {291,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2507, nLevel = -1, nSeries = -1},		-- 圆月狂刀
 		[2] = {nTemplate = 2506, nLevel = -1, nSeries = -1},		-- 守卫
 		[3] = {nTemplate = 2504, nLevel = -1, nSeries = -1},		--去准备场

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
		 [2] = {nTime = 59 , nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{				
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{0, 0, 0, 0, 0}}},  --ALL, SELF, NPC, TEAM, KIN 分别对应的填写
		    
		    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str119[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1,Lang.mission.str118[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str112[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str112[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 2, "men", "men"}, --去准备场
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 0, nNum = 1,		--boss锁
			tbPrelock = {2},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str121[Lang.Idx]},	--准备场开启，发公告
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[15].Enemy1, 0, 0, 0, 0}}}, --设阵营
		    {Activity.EVENT_TYPE.TARGET_INFO,-1,Lang.mission.str122[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str122[Lang.Idx], 3}},
 				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 3, "boss", "boss"},  --刷出圆月狂刀
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward() "}, --击杀boss发奖励
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str123[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str124[Lang.Idx]},
			},
		},
		[4] = {nTime = 1740, nNum = 100000,		-- 左路小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str115[Lang.Idx], 4},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str115[Lang.Idx]}, 4},
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 8, 4, "guaiwu", "leftroad"},  --刷出守卫
			},
			tbUnLockEvent =
			{

			},
		},
		[5] = {nTime = 1740, nNum = 0,		--复活锁
			tbPrelock = {2},
			tbStartEvent =
			{
			  {Activity.EVENT_TYPE.REVIVE_INFO, -1 ,false, true, true, 0,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},
			},
			tbUnLockEvent =
			{

			},
		},
		[6] = {nTime = 60, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 6, "guaiwu", "bossleft"},   --刷出守卫

			},
		},
		[7] = {nTime = 160, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 7, "guaiwu", "bossright"},  --刷出守卫
			},
		},
		[8] = {nTime = 260, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 8, "guaiwu", "bossback"},   --刷出守卫
			},
		},
    },
    NextSteps = {},
}


STEP[4] =
{
	Time = 1800,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,         -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.KUAFUBOSS_SAICHANG,
	WorldBossTemplateId = 2508,								 --世界boss战斗场中要加这个boss的templateid
	Pos = {292,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},            --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2508, nLevel = -1, nSeries = -1},		-- 鲁大深
 		[2] = {nTemplate = 2506, nLevel = -1, nSeries = -1},		-- 守卫
 		[3] = {nTemplate = 2504, nLevel = -1, nSeries = -1},		--去准备场
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
		 [2] = {nTime = 59 , nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{				
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{0, 0, 0, 0, 0}}},  --ALL, SELF, NPC, TEAM, KIN 分别对应的填写
		    
		    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str110[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1,Lang.mission.str110[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str112[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str112[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 2, "men", "men"}, --去准备场
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 0, nNum = 1,		--boss锁
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[15].Enemy1, 0, 0, 0, 0}}}, --设阵营
				{Activity.EVENT_TYPE.TARGET_INFO,-1,Lang.mission.str125[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str125[Lang.Idx], 3}},
 				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 3, "boss", "boss"},  --刷出鲁大深
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward() "}, --击杀boss发奖励
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str126[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str127[Lang.Idx]},
			},
		},
		[4] = {nTime = 1740, nNum = 10000,		-- 左路小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str115[Lang.Idx], 4},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str115[Lang.Idx]}, 4},
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 8, 4, "guaiwu", "leftroad"},  --刷出守卫
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.REMOVE_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str128[Lang.Idx], 4}},
			},
		},
		[5] = {nTime = 1740, nNum = 0,		-- 复活锁
			tbPrelock = {2},
			tbStartEvent =
			{
			 	{Activity.EVENT_TYPE.REVIVE_INFO, -1 ,false, true, true, 0,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},
			},
			tbUnLockEvent =
			{

			},
		},
		[6] = {nTime = 60, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 6, "guaiwu", "bossleft"},   --刷出守卫

			},
		},
		[7] = {nTime = 160, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 7, "guaiwu", "bossright"},  --刷出守卫
			},
		},
		[8] = {nTime = 260, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 8, "guaiwu", "bossback"},   --刷出守卫
			},
		},
    },
    NextSteps = {},
}

STEP[5] =
{
	Time = 1800,                                               --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED, 						 -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                              -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.KUAFUBOSS_SAICHANG,
	WorldBossTemplateId = 2509,																 --世界boss战斗场中要加这个boss的templateid
	Pos = {293,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2509, nLevel = -1, nSeries = -1},		-- 魔僧
 		[2] = {nTemplate = 2506, nLevel = -1, nSeries = -1},		-- 守卫
 		[3] = {nTemplate = 2504, nLevel = -1, nSeries = -1},		--去准备场
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
		 [2] = {nTime = 59 , nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{				
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{0, 0, 0, 0, 0}}},  --ALL, SELF, NPC, TEAM, KIN 分别对应的填写
		    
		    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str118[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1,Lang.mission.str118[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str120[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str120[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 2, "men", "men"}, --去准备场
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 0, nNum = 1,		--boss锁
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[15].Enemy1, 0, 0, 0, 0}}}, --设阵营
				{Activity.EVENT_TYPE.TARGET_INFO,-1,Lang.mission.str129[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str129[Lang.Idx], 3}},
 				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 3, "boss", "boss"},  --刷出魔僧
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward() "}, --击杀boss发奖励
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str130[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str131[Lang.Idx]},
			},
		},
		[4] = {nTime = 1740, nNum = 10000,		-- 左路小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str115[Lang.Idx], 4},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str115[Lang.Idx]}, 4},
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 8, 4, "guaiwu", "leftroad"},  --刷出守卫
			},
			tbUnLockEvent =
			{

			},
		},
		[5] = {nTime = 1740, nNum = 0,		-- 复活锁
			tbPrelock = {2},
			tbStartEvent =
			{
			 	{Activity.EVENT_TYPE.REVIVE_INFO, -1 ,false, true, true, 0,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},
			},
			tbUnLockEvent =
			{

			},
		},
		[6] = {nTime = 60, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 6, "guaiwu", "bossleft"},   --刷出守卫

			},
		},
		[7] = {nTime = 160, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 7, "guaiwu", "bossright"},  --刷出守卫
			},
		},
		[8] = {nTime = 260, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 8, "guaiwu", "bossback"},   --刷出守卫
			},
		},
		},
    NextSteps = {},
}


STEP[6] =
{
	Time = 1800,                                              --时间
	NeedTransport = 1,
	AssignType = Activity.ASSIGN_TYPE.DONOT_NEED,            -- 小队优先的帮会模式
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,
	Type = Activity.STEP_TYPE.KUAFUBOSS_SAICHANG,
	WorldBossTemplateId = 2510,								 --世界boss战斗场中要加这个boss的templateid
	Pos = {294,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},  --战斗地图角色所在位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
 		[1] = {nTemplate = 2510, nLevel = -1, nSeries = -1},		-- 武痴
 		[2] = {nTemplate = 2506, nLevel = -1, nSeries = -1},		-- 守卫
 		[3] = {nTemplate = 2504, nLevel = -1, nSeries = -1},		--去准备场
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
		 [2] = {nTime = 59 , nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{				
			  {Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{0, 0, 0, 0, 0}}},  --ALL, SELF, NPC, TEAM, KIN 分别对应的填写
		    
		    {Activity.EVENT_TYPE.TARGET_INFO,-1, Lang.mission.str119[Lang.Idx],2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1,Lang.mission.str119[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str112[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str112[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 2, "men", "men"}, --去准备场
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 0, nNum = 1,		--boss锁
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.SET_RELATION, {{ActivitySetting[15].Enemy1, 0, 0, 0, 0}}}, --设阵营
				{Activity.EVENT_TYPE.TARGET_INFO,-1,Lang.mission.str132[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str132[Lang.Idx], 3}},
 				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 3, "boss", "boss"},  --刷出武痴
			},
			tbUnLockEvent =
			{
				{Activity.EVENT_TYPE.DO_SCRIPT, "self:SendAward() "}, --击杀boss发奖励
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str133[Lang.Idx]},
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str134[Lang.Idx]},
			},
		},
		[4] = {nTime = 1740, nNum = 10000,		-- 左路小怪
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str115[Lang.Idx], 4},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str115[Lang.Idx]}, 4},
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 8, 4, "guaiwu", "leftroad"},  --刷出守卫
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 1740, nNum = 0,		-- 复活锁
			tbPrelock = {2},
			tbStartEvent =
			{
			 	{Activity.EVENT_TYPE.REVIVE_INFO, -1 ,false, true, true, 0,{2865, 1120, 1048576},{2861, 776, 1048576},{3152, 961, 1048576}},
			},
			tbUnLockEvent =
			{

			},
		},
		[6] = {nTime = 60, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 6, "guaiwu", "bossleft"},   --刷出守卫

			},
		},
		[7] = {nTime = 160, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 7, "guaiwu", "bossright"},  --刷出守卫
			},
		},
		[8] = {nTime = 260, nNum = 0,		--时间计时 刷 小怪
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			 	{Activity.EVENT_TYPE.ADD_NPC, 2, 3, 8, "guaiwu", "bossback"},   --刷出守卫
			},
		},
    },
    NextSteps = {},

}
