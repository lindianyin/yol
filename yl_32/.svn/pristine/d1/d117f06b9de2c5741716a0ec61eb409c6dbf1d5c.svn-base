-- 40级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[556]=
{
	nTimeLimit = 1200,  --本层时间20分钟
	nMapId = 556,
    tbBeginPos = {6552, 1321, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 2160, nLevel = -1, nSeries = -1},		-- 狂暴鱼人
		[2] = {nTemplate = 2161, nLevel = -1, nSeries = -1},		-- 狂暴刀将
		[3] = {nTemplate = 2162, nLevel = -1, nSeries = -1},		-- 狂暴老妖
		[4] = {nTemplate = 2163, nLevel = -1, nSeries = -1},		-- 狂暴巨鸟
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
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str881[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 4 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 1, 2, "1", "1"},
				{MultiInstance.ADD_NPC, 2, 1, 2, "2", "2"},
				{MultiInstance.ADD_NPC, 3, 1, 2, "3", "3"},
				{MultiInstance.ADD_NPC, 4, 1, 2, "4", "4"},
				{MultiInstance.TARGET_INFO, Lang.mission.str882[Lang.Idx], 1}
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
