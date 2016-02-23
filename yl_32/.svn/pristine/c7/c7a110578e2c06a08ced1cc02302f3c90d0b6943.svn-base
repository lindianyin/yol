-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[32] =
{
	nRequireLevel = 20,                -- 需求等级
	tbOpenTimes = {{ {hour=0, min=0, sec=0}, {hour=23, min=50, sec=0} }},	--开放时间{StartTime, EndTime},开几次，填几组时间, 每组时间不能重叠
	ActivityName = Lang.mission.str1136[Lang.Idx],        -- 活动名字，输出信息用
	nStartStep = 1,			   	          -- 开始步骤，记录进入位置
	tbEnterPos = { nMapId = 355, tbPos = {{2016,1750,1048576},{2656,2407,1048576},{3889,424,1048576},{4993,1819,1048576},{7546,3715,1048576},{7694,4444,1048576},{5711,5607,1048576},{5845,6284,1048576}}},		-- 进入点
	nPrepareTime = 86390,						-- 报名阶段时间
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
ActivitySetting[32].Enemy1 = Team + Team_Enable + Not;  --非队友的人全都是敌人
--ActivitySetting[2].Enemy2 = 0;

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[32].STEP


STEP[1] =
{
	Time = 86390,                                              --时间
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,                                          -- 最多能参加的玩家数
	Type = Activity.STEP_TYPE.MANHUANGZHIDI,                   -- 步骤类型
	--Enemy = {0,0,0,0,0},                                      -- 填了这个就不能再分组，以玩家关系判断是否敌人
	Pos = {355,{2016,1750,1048576},{2656,2407,1048576},{3889,424,1048576},{4993,1819,1048576},{7546,3715,1048576},{7694,4444,1048576},{5711,5607,1048576},{5845,6284,1048576}},                         --玩家默认传送的位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)

		[1] = {nTemplate = 2803, nLevel = -1, nSeries = -1},		--桃源树妖
		[2] = {nTemplate = 2807, nLevel = -1, nSeries = -1},		--boss1
		[3] = {nTemplate = 2808, nLevel = -1, nSeries = -1},		--boss2
		[4] = {nTemplate = 2809, nLevel = -1, nSeries = -1},		--boss3
		[5] = {nTemplate = 2810, nLevel = -1, nSeries = -1},		--boss4
		[6] = {nTemplate = 2811, nLevel = -1, nSeries = -1},		--boss5
		[7] = {nTemplate = 2812, nLevel = -1, nSeries = -1},		--boss6
		[8] = {nTemplate = 2813, nLevel = -1, nSeries = -1},		--boss7
		
		[9] = {nTemplate = 2820, nLevel = -1, nSeries = -1},		--乌黑大鹏
		[10] = {nTemplate = 2821, nLevel = -1, nSeries = -1},		--凶猛野狼
		[11] = {nTemplate = 2822, nLevel = -1, nSeries = -1},		--胡须男
		[12] = {nTemplate = 2823, nLevel = -1, nSeries = -1},		--无魂毒虫
		[13] = {nTemplate = 2824, nLevel = -1, nSeries = -1},		--西山羚
		[14] = {nTemplate = 2825, nLevel = -1, nSeries = -1},		--橘黄蟹
		[15] = {nTemplate = 2826, nLevel = -1, nSeries = -1},		--大臂怪
		[16] = {nTemplate = 2827, nLevel = -1, nSeries = -1},		--怨灵
		
		[17] = {nTemplate = 2814, nLevel = -1, nSeries = -1},		--驿站

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

		[2] = {nTime = 86389, nNum = 0,
			tbPrelock = {1},
			tbStartEvent =
			{

				{Activity.EVENT_TYPE.TARGET_INFO,-1, string.format(Lang.mission.str136[Lang.Idx],60), 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, string.format(Lang.mission.str136[Lang.Idx],60), 2}},

				{Activity.EVENT_TYPE.ADD_NPC, 17, 1, 2,"chuangsong","chuangsong"}, --驿站
	
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos1"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos2"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos3"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos4"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos5"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos6"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos7"},
				{Activity.EVENT_TYPE.ADD_NPC, 13, 4, 2,"guaiwu1","pos8"},
				{Activity.EVENT_TYPE.ADD_NPC, 13, 4, 2,"guaiwu1","pos9"},
				{Activity.EVENT_TYPE.ADD_NPC, 13, 4, 2,"guaiwu1","pos10"},
				{Activity.EVENT_TYPE.ADD_NPC, 13, 4, 2,"guaiwu1","pos11"},
				{Activity.EVENT_TYPE.ADD_NPC, 13, 4, 2,"guaiwu1","pos12"},
				{Activity.EVENT_TYPE.ADD_NPC, 13, 4, 2,"guaiwu1","pos13"},
				{Activity.EVENT_TYPE.ADD_NPC, 14, 4, 2,"guaiwu1","pos14"},
				{Activity.EVENT_TYPE.ADD_NPC, 14, 4, 2,"guaiwu1","pos15"},
				{Activity.EVENT_TYPE.ADD_NPC, 14, 4, 2,"guaiwu1","pos16"},
				{Activity.EVENT_TYPE.ADD_NPC, 14, 4, 2,"guaiwu1","pos17"},
				{Activity.EVENT_TYPE.ADD_NPC, 14, 4, 2,"guaiwu1","pos18"},
				{Activity.EVENT_TYPE.ADD_NPC, 14, 4, 2,"guaiwu1","pos19"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos20"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos21"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos22"},
				{Activity.EVENT_TYPE.ADD_NPC, 1, 4, 2,"guaiwu1","pos23"},



				{Activity.EVENT_TYPE.ADD_NPC_ON_TIME,100000, 7, 1, 2,"boss","boss"},  --12点刷出boss2
				{Activity.EVENT_TYPE.ADD_NPC_ON_TIME,180000, 7, 1, 2,"boss","boss"}, --20点刷出boss6

			},
			tbUnLockEvent =
			{
 
			},
		},
    },
    NextSteps = {},
}
