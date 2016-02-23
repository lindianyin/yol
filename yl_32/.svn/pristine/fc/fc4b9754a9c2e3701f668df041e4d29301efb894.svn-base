-- 40级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[553]=
{
	nTimeLimit = 1200,  --本层时间20分钟
	nMapId = 553,
    tbBeginPos = {2018, 2183, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 2130, nLevel = -1, nSeries = -1},		-- 小胖
		[2] = {nTemplate = 2131, nLevel = -1, nSeries = -1},		-- 一胖
		[3] = {nTemplate = 2132, nLevel = -1, nSeries = -1},		-- 小胖
		[4] = {nTemplate = 2133, nLevel = -1, nSeries = -1},		-- 二胖
		[5] = {nTemplate = 2134, nLevel = -1, nSeries = -1},		-- 小胖
		[6] = {nTemplate = 2135, nLevel = -1, nSeries = -1},		-- 三胖
		[7] = {nTemplate = 2136, nLevel = -1, nSeries = -1},		-- 小胖
		[8] = {nTemplate = 2137, nLevel = -1, nSeries = -1},		-- 四胖
	    [9] = {nTemplate = 2138, nLevel = -1, nSeries = -1},		-- 巨胖
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.TIME_INFO, Lang.mission.str868[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str869[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 16 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 1, 2, "yipang1", "yipang1", 5, 2, "yipang1_timer",0},
				{MultiInstance.ADD_NPC, 1, 1, 2, "yipang2", "yipang2", 5, 2, "yipang2_timer",0},
				{MultiInstance.ADD_NPC, 1, 1, 2, "yipang3", "yipang3", 5, 2, "yipang3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str870[Lang.Idx], 1}
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
				{MultiInstance.ADD_NPC, 3, 1, 5, "erpang1", "erpang1", 5, 2, "erpang1_timer",0},
				{MultiInstance.ADD_NPC, 3, 1, 5, "erpang2", "erpang2", 5, 2, "erpang2_timer",0},
				{MultiInstance.ADD_NPC, 3, 1, 5, "erpang3", "erpang3", 5, 2, "erpang3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str871[Lang.Idx], 1}			},
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
				{MultiInstance.ADD_NPC, 5, 1, 8, "sanpang1", "sanpang1", 5, 2, "sanpang1_timer",0},
				{MultiInstance.ADD_NPC, 5, 1, 8, "sanpang2", "sanpang2", 5, 2, "sanpang2_timer",0},
				{MultiInstance.ADD_NPC, 5, 1, 8, "sanpang3", "sanpang3", 5, 2, "sanpang3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str872[Lang.Idx], 1}			},
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
				{MultiInstance.ADD_NPC, 7, 1, 11, "sipang1", "sipang1", 5, 2, "sipang1_timer",0},
				{MultiInstance.ADD_NPC, 7, 1, 11, "sipang2", "sipang2", 5, 2, "sipang2_timer",0},
				{MultiInstance.ADD_NPC, 7, 1, 11, "sipang3", "sipang3", 5, 2, "sipang3_timer",0},
				{MultiInstance.TARGET_INFO, Lang.mission.str873[Lang.Idx], 1}			},
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
				{MultiInstance.TARGET_INFO, Lang.mission.str874[Lang.Idx], 1}			},
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
