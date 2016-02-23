-- 活动相关配置，由策划填写
Include("script/mission/activity/activitydef.lua")

local ActivitySetting = Activity.ActivitySetting

ActivitySetting[26] =
{
	nRequireLevel = 20,                -- 需求等级 11分钟
	tbOpenTimes = {{{wday=2,hour=21, min=30, sec=0}, {wday=2,hour=22, min=0, sec=5}},{{wday=4,hour=21, min=30, sec=0}, {wday=4,hour=22, min=0, sec=5}}, {{wday=6,hour=21, min=30, sec=0}, {wday=6,hour=22, min=0, sec=5}}},	--开放时间{StartTime, EndTime},开几次，填几组时间, 每组时间不能重叠
	ActivityName = Lang.mission.str188[Lang.Idx],        -- 活动名字，输出信息用
	nStartStep = 1,			   	          -- 开始步骤，记录进入位置
	tbEnterPos = { nMapId = 341, tbPos = {{3912,1112,1048576},{4087,1111,1048576},{4273,1164,1048576},{4329,936,1048576},{4148,920,1048576},{3938,993,1048576},{3949,772,1048576},{4138,702,1048576},{4401,677,1048576},{4533,408,1048576},{4271,331,1048576},{3776,452,1048576}}},		-- 准备场进入点
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
ActivitySetting[26].Enemy1 = Kin + Kin_Enable + Not;  --非本家族的人全都是敌人

-- 步骤定义分开写，不然层次太多很容易乱
local STEP = ActivitySetting[26].STEP

STEP[1] =
{
	Time = 480,                                              --时间  
	AssignType = Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED,
	GroupType = Activity.GROUP_TYPE.DONNOT_GROUP,
	GroupNum = 2,                                            -- 小队优先的帮会模式
	MaxPlayer = 2000,                                          -- 最多能参加的玩家数
	NeedTransport = 0,
	Type = Activity.STEP_TYPE.TONGFIGHT_BATTLEFILED,                   -- 步骤类型
	Pos = {341,{3912,1112,1048576},{4087,1111,1048576},{4273,1164,1048576},{4329,936,1048576},{4148,920,1048576},{3938,993,1048576},{3949,772,1048576},{4138,702,1048576},{4401,677,1048576},{4533,408,1048576},{4271,331,1048576},{3776,452,1048576}},                         --玩家默认传送的位置
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
		[1] = {nTemplate = 2600, nLevel = -1, nSeries = -1},		-- 金牌
		[2] = {nTemplate = 2601, nLevel = -1, nSeries = -1},		-- 隐形npc
		[3] = {nTemplate = 2604, nLevel = -1, nSeries = -1},		-- 屏障
		[4] = {nTemplate = 2605, nLevel = -1, nSeries = -1},		-- 路标
		[5] = {nTemplate = 2606, nLevel = -1, nSeries = -1},		-- 自杀掉落npc
	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时 准备场等待2秒
			tbPrelock = {},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str189[Lang.Idx]},	--准备场开启，发公告
			},
			tbUnLockEvent =
			{

			},
		},

		[2] = {nTime = 179, nNum = 0,		
			tbPrelock = {1},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.REVIVE_INFO, {-1, false, false, true, 0, {6038,625,1048576}, {2620, 990, 1048576}, {6211, 714, 1048576}, {2537, 1205, 1048576}, {6298, 879, 1048576}, {2250, 1310, 1048576}}},
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str190[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str190[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str191[Lang.Idx], 2},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str191[Lang.Idx], 2}},
				
				{Activity.EVENT_TYPE.ADD_NPC, 3, 1, 2, "pingzhang", "pingzhang"},  --刷出屏障npc特效
							
			},
			tbUnLockEvent =
			{
				
			},
		},
		
		[3] = {nTime = 290, nNum = 0,		
			tbPrelock = {2},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SET_RELATION, {ActivitySetting[26].Enemy1, 0, 0, 0, 0}},
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, Lang.mission.str192[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, Lang.mission.str192[Lang.Idx], 3}},
				
				{Activity.EVENT_TYPE.TARGET_INFO, -1, Lang.mission.str191[Lang.Idx], 3},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TARGET_INFO, {-1, Lang.mission.str191[Lang.Idx], 3}},
				
				{Activity.EVENT_TYPE.SET_TRAP_EFFECT, "D_HD_SJtrap01", 0},  --删屏障特效
				{Activity.EVENT_TYPE.DEL_NPC,"pingzhang"},   --删屏障npc
				
				{Activity.EVENT_TYPE.ADD_NPC, 2, 1, 3, "yinxing", "yinxing"},  --刷出隐形npc(攻方 守方 buff)
				
				{Activity.EVENT_TYPE.ADD_NPC, 1, 1, 3, "flag", "flag"},  --刷出金牌
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 3, "lubiao", "lubiao1"},  --路标1
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 3, "lubiao", "lubiao2"},  --路标2
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 3, "lubiao", "lubiao3"},  --路标3
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 3, "lubiao", "lubiao4"},  --路标4
				{Activity.EVENT_TYPE.ADD_NPC, 4, 1, 3, "lubiao", "lubiao5"},  --路标5
	
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str193[Lang.Idx]},	
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str193[Lang.Idx]},	--战场开启，发公告
				
			},
			tbUnLockEvent =
			{
				
			},
		},
		[4] = {nTime = 60, nNum = 0,		-- 总计时 准备场等待2秒
			tbPrelock = {2},
			tbStartEvent =
			{
				
			},
			tbUnLockEvent =
			{

			},
		},
		 [5] = {nTime = 0, nNum = 1,		-- 总计时 准备场等待2秒
			tbPrelock = {4},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.ADD_NPC, 5, 5, 5, "zhanhun1", "zhanhun1", 4, 60, "zhanhun1_timer"},  --定时刷出战魂包
				{Activity.EVENT_TYPE.ADD_NPC, 5, 5, 5, "zhanhun2", "zhanhun2", 4, 60, "zhanhun2_timer"},  --定时刷出战魂包
				{Activity.EVENT_TYPE.ADD_NPC, 5, 5, 5, "zhanhun3", "zhanhun3", 4, 60, "zhanhun3_timer"},  --定时刷出战魂包
				{Activity.EVENT_TYPE.ADD_NPC, 5, 5, 5, "zhanhun4", "zhanhun4", 4, 60, "zhanhun4_timer"},  --定时刷出战魂包
				{Activity.EVENT_TYPE.ADD_NPC, 5, 5, 5, "zhanhun5", "zhanhun5", 4, 60, "zhanhun5_timer"},  --定时刷出战魂包
				{Activity.EVENT_TYPE.ADD_NPC, 5, 5, 5, "zhanhun6", "zhanhun6", 4, 60, "zhanhun6_timer"},  --定时刷出战魂包
				
			},
			tbUnLockEvent =
			{

			},
		},
		
		[6] = {nTime = 10, nNum = 0,		
			tbPrelock = {3},
			tbStartEvent =
			{
				{Activity.EVENT_TYPE.SET_RELATION, {0, 0, 0, 0, 0}},  --重置战斗关系
				
				{Activity.EVENT_TYPE.TIME_INFO, -1, "<color=green>剩余时间:<color=red>%s", 6},
				{Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT, Activity.EVENT_TYPE.TIME_INFO, {-1, "<color=green>剩余时间:<color=red>%s", 6}},
				
				{Activity.EVENT_TYPE.BLACK_MSG, -1, Lang.mission.str194[Lang.Idx]},	--战场开启，发公告
				{Activity.EVENT_TYPE.SHOW_NOTICE_MSG, Lang.mission.str194[Lang.Idx]},
			},
			tbUnLockEvent =
			{
				
			},
		},
    },
    NextSteps = {},
}