-- 40级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[552]=
{
	nTimeLimit = 1200,  --本层时间20分钟
	nMapId = 552,
    tbBeginPos = {2018, 2183, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 2120, nLevel = -1, nSeries = -1},		-- 鱼妖
		[2] = {nTemplate = 2121, nLevel = -1, nSeries = -1},		-- 大鱼妖
		[3] = {nTemplate = 2122, nLevel = -1, nSeries = -1},		-- 猴妖
		[4] = {nTemplate = 2123, nLevel = -1, nSeries = -1},		-- 大猴妖
		[5] = {nTemplate = 2124, nLevel = -1, nSeries = -1},		-- 树妖
		[6] = {nTemplate = 2125, nLevel = -1, nSeries = -1},		-- 大树妖
		[7] = {nTemplate = 2126, nLevel = -1, nSeries = -1},		-- 鸟妖
		[8] = {nTemplate = 2127, nLevel = -1, nSeries = -1},		-- 巨鸟妖
	    [9] = {nTemplate = 2128, nLevel = -1, nSeries = -1},		-- 龙妖
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.TIME_INFO, Lang.mission.str875[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str876[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 16 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 1, 2, "yuyao1", "yuyao1", 5, 2, "yuyao1_timer",0},
				{MultiInstance.ADD_NPC, 1, 1, 2, "yuyao2", "yuyao2", 5, 2, "yuyao2_timer",0},
				{MultiInstance.ADD_NPC, 1, 1, 2, "yuyao3", "yuyao3", 5, 2, "yuyao3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str877[Lang.Idx], 1}
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 12, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
        [4] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {3},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 2, 1, 2, "jingying", "jingying"},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 16 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {2},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 3, 1, 5, "houyao1", "houyao1", 5, 2, "houyao1_timer",0},
				{MultiInstance.ADD_NPC, 3, 1, 5, "houyao2", "houyao2", 5, 2, "houyao2_timer",0},
				{MultiInstance.ADD_NPC, 3, 1, 5, "houyao3", "houyao3", 5, 2, "houyao3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str878[Lang.Idx], 1}
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 12, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {2},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
        [7] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {6},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 4, 1, 5, "jingying", "jingying"},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 16 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {5},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 5, 1, 8, "shuyao1", "shuyao1", 5, 2, "shuyao1_timer",0},
				{MultiInstance.ADD_NPC, 5, 1, 8, "shuyao2", "shuyao2", 5, 2, "shuyao2_timer",0},
				{MultiInstance.ADD_NPC, 5, 1, 8, "shuyao3", "shuyao3", 5, 2, "shuyao3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str879[Lang.Idx], 1}
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 12, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {5},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
        [10] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {9},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 6, 1, 8, "jingying", "jingying"},
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 16 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {8},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 7, 1, 11, "niaoyao1", "niaoyao1", 5, 2, "shuyao1_timer",0},
				{MultiInstance.ADD_NPC, 7, 1, 11, "niaoyao2", "niaoyao2", 5, 2, "shuyao2_timer",0},
				{MultiInstance.ADD_NPC, 7, 1, 11, "niaoyao3", "niaoyao3", 5, 2, "shuyao3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str880[Lang.Idx], 1}
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 12, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {8},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
        [13] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {12},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 11, "jingying", "jingying"},
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {11},
			tbStartEvent =
			{
                {MultiInstance.ADD_NPC, 9, 1, 14, "boss", "boss"},
				{MultiInstance.TARGET_INFO, Lang.mission.str880[Lang.Idx], 1}
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 1, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {14},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
	    },
		[16] = {nTime = 10, nNum = 0 ,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
				{MultiInstance.TARGET_INFO, Lang.mission.str857[Lang.Idx]},
				{MultiInstance.TIME_INFO, Lang.mission.str858[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		}
    }
}
