-- 60级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[611]=
{
	nTimeLimit = 600,  --本层时间20分钟
	nMapId = 611,
    tbBeginPos = {3483, 1379, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 2400, nLevel = -1, nSeries = -1},		-- 冰玉米（剧情）
		[2] = {nTemplate = 2401, nLevel = -1, nSeries = -1},		-- 火玉米（剧情）
		[3] = {nTemplate = 2402, nLevel = -1, nSeries = -1},		-- 冰玉米
		[4] = {nTemplate = 2403, nLevel = -1, nSeries = -1},		-- 火玉米
		[5] = {nTemplate = 2404, nLevel = -1, nSeries = -1},		-- 开场被打
		[6] = {nTemplate = 2405, nLevel = -1, nSeries = -1},		-- 一出来自杀
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.TIME_INFO, Lang.mission.str1179[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1180[Lang.Idx]},
				{MultiInstance.TARGET_INFO,Lang.mission.str1181[Lang.Idx], 1},

			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 10, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 1, 2, "bing", "bing"},
				{MultiInstance.ADD_NPC, 5, 1, 2, "beida", "beida"},
				{MultiInstance.NPC_BUBBLE, "bing",Lang.mission.str1170[Lang.Idx], 1},
				{MultiInstance.NPC_BUBBLE, "bing",Lang.mission.str1170[Lang.Idx], 5},
				{MultiInstance.ADD_NPC, 2, 1, 2, "huo", "huo"},
				{MultiInstance.NPC_BUBBLE, "huo",Lang.mission.str1171[Lang.Idx], 1},
				{MultiInstance.NPC_BUBBLE, "huo",Lang.mission.str1171[Lang.Idx], 5},
				{MultiInstance.TARGET_INFO,Lang.mission.str1182[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
                {MultiInstance.DEL_NPC, "bing"},
				 {MultiInstance.DEL_NPC, "beida"},
				{MultiInstance.DEL_NPC, "huo"},
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {2},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 3, 1, 3, "bing1", "bing"}, --
				{MultiInstance.TARGET_INFO, Lang.mission.str1179[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 5, 1, 2, "beida", "beida"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1180[Lang.Idx]},
			{MultiInstance.NPC_BUBBLE, "huo1",Lang.mission.str1181[Lang.Idx], 1},
			{MultiInstance.ADD_NPC, 6, 1, 5, "zisha1", "zisha"},
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {2},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 4, 1, 4, "huo1", "huo"}, --
				{MultiInstance.TARGET_INFO, Lang.mission.str1182[Idx], 1},
			},
			tbUnLockEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 2, "beida1", "beida"},
			   {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1180[Lang.Idx]},
			   {MultiInstance.NPC_BUBBLE, "bing1",Lang.mission.str1177[Lang.Idx], 1},
			   {MultiInstance.ADD_NPC, 6, 1, 5, "zisha2", "zisha"},
			},
		},
		[5] = {nTime = 0, nNum = 2 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
		},
		[6] = {nTime = 10, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
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
