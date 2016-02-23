-- 40级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[554]=
{
	nTimeLimit = 1200,  --本层时间20分钟
	nMapId = 554,
    tbBeginPos = {5581, 1086, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 2140, nLevel = -1, nSeries = -1},		-- 天残弟子
		[2] = {nTemplate = 2141, nLevel = -1, nSeries = -1},		-- 天残四长老
		[3] = {nTemplate = 2142, nLevel = -1, nSeries = -1},		-- 天残弟子
		[4] = {nTemplate = 2143, nLevel = -1, nSeries = -1},		-- 天残三长老
		[5] = {nTemplate = 2144, nLevel = -1, nSeries = -1},		-- 天残弟子
		[6] = {nTemplate = 2145, nLevel = -1, nSeries = -1},		-- 天残二长老
		[7] = {nTemplate = 2146, nLevel = -1, nSeries = -1},		-- 天残弟子
		[8] = {nTemplate = 2147, nLevel = -1, nSeries = -1},		-- 天残一长老
	    [9] = {nTemplate = 2148, nLevel = -1, nSeries = -1},		-- 天残脚
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
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str884[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 33 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 7, 2, "yi", "yi"},
				{MultiInstance.ADD_NPC, 2, 1, 2, "jingying1", "jingying"},
				{MultiInstance.ADD_NPC, 3, 7, 2, "er", "er"},
				{MultiInstance.ADD_NPC, 4, 1, 2, "jingying2", "jingying"},
				{MultiInstance.ADD_NPC, 5, 7, 2, "san", "san"},
				{MultiInstance.ADD_NPC, 6, 1, 2, "jingying3", "jingying"},
				{MultiInstance.ADD_NPC, 7, 7, 2, "si", "si"},
				{MultiInstance.ADD_NPC, 8, 1, 2, "jingying4", "jingying"},
				{MultiInstance.ADD_NPC, 9, 1, 2, "boss", "boss"},
				{MultiInstance.TARGET_INFO, Lang.mission.str885[Lang.Idx], 1}
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 1, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
	    },
		[4] = {nTime = 10, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
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
