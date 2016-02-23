-- 40级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[551]=
{
	nTimeLimit = 1200,  --本层时间20分钟
	nMapId = 551,
    tbBeginPos = {6993, 1290, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 2110, nLevel = -1, nSeries = -1},		-- 小鬼
		[2] = {nTemplate = 2111, nLevel = -1, nSeries = -1},		-- 凶鬼
		[3] = {nTemplate = 2112, nLevel = -1, nSeries = -1},		-- 弓兵
		[4] = {nTemplate = 2113, nLevel = -1, nSeries = -1},		-- 尸鬼弓将
		[5] = {nTemplate = 2114, nLevel = -1, nSeries = -1},		-- 尸鬼兵
		[6] = {nTemplate = 2115, nLevel = -1, nSeries = -1},		-- 尸鬼刀兵
		[7] = {nTemplate = 2116, nLevel = -1, nSeries = -1},		-- 尸鬼王
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.TIME_INFO, Lang.mission.str862[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str863[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 29 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 9, 2, "xiaogui", "xiaogui"},
				{MultiInstance.ADD_NPC, 2, 1, 2, "xiaogui1", "xiaogui1"},
				{MultiInstance.ADD_NPC, 3, 8, 2, "gongbing", "gongbing"},
				{MultiInstance.ADD_NPC, 4, 1, 2, "gongbing1", "gongbing1"},
				{MultiInstance.ADD_NPC, 5, 8, 2, "daobing", "daobing"},
				{MultiInstance.ADD_NPC, 6, 1, 2, "daobing1", "daobing1"},
				{MultiInstance.ADD_NPC, 7, 1, 2, "boss", "boss"},
                {MultiInstance.TARGET_INFO, Lang.mission.str864[Lang.Idx], 1},
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
				{MultiInstance.TIME_INFO, Lang.mission.str858[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		}
    }
}
