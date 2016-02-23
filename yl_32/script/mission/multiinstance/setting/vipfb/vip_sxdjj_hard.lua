-- 20级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

--苏醒的将军
tbMultiSetting[610]=
{
	nTimeLimit = 605,  --本层时间20分钟
	nMapId = 610,
    tbBeginPos = {2727, 400, 1048576},
	nGameType = MultiInstance.GAME_TYPE.LV_20_EASY,
	nMultiLevel = 20,
	nDifficulty = 1,

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
	  [1] = {nTemplate = 1200, nLevel = 25, nSeries = -1},		-- 大将军
	  [2] = {nTemplate = 1201, nLevel = -1, nSeries = -1},		-- 火焰
	  [3] = {nTemplate = 1202, nLevel = 1, nSeries = -1},		-- 自杀
	  [4] = {nTemplate = 1203, nLevel = -1, nSeries = -1},		-- 闪电
	  [5] = {nTemplate = 1204, nLevel = 25, nSeries = -1},		-- 被杀
	  [6] = {nTemplate = 1205, nLevel = 25, nSeries = -1},		-- 将军分身
	  [7] = {nTemplate = 1206, nLevel = 25, nSeries = -1},		-- 龙卷风
	  [8] = {nTemplate = 1207, nLevel = -1, nSeries = -1},		-- 全体加血
	  [9] = {nTemplate = 1208, nLevel = -1, nSeries = -1},		-- 大将军
	  [10] = {nTemplate = 1209, nLevel = -1, nSeries = -1},		-- 墙
	  [11] = {nTemplate = 1210, nLevel = -1, nSeries = -1},		-- 透明目标
	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.TIME_INFO, Lang.mission.str930[Lang.Idx],1},
				{MultiInstance.SET_TRAP_INVALID, 17, 1},
				{MultiInstance.SET_TRAP_INVALID, 16, 0},
				{MultiInstance.ADD_NPC, 10, 1, 1, "qiang1", "qiang1"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"qiang1","qiang1"},
			},
		},
		[2] = {nTime = 600, nNum = 0 ,		-- 时间倒计时，时间到了，系统会自动走退出流程
			tbPrelock = {1},
			tbStartEvent =
			{
                {MultiInstance.SET_TRAP_INVALID, 16, 1},
				{MultiInstance.ADD_NPC, 2, 2, 2, "huoyan", "huoyan"},
			    {MultiInstance.TIME_INFO, Lang.mission.str931[Lang.Idx],2},
			},
			tbUnLockEvent =
			{
			},
		},
	    [3] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {1},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 3, 3, 3, "zisha1", "zisha1"},
			   {MultiInstance.ADD_NPC, 11, 1, 3, "mubiao", "mubiao"},
			   {MultiInstance.BLACK_MSG, Lang.mission.str932[Lang.Idx]},
			   {MultiInstance.TARGET_INFO, Lang.mission.str933[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 2, 2, 3, "huoyan1", "huoyan1"},
			},
		},
		 [4] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {3},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 3, 3, 4, "zisha2", "zisha2"},
			},
			tbUnLockEvent =
			{
			   {MultiInstance.ADD_NPC, 2, 2, 4, "huoyan2", "huoyan2"},
			},
		},
		 [5] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {4},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 3, 3, 5, "zisha3", "zisha3"},
			},
			tbUnLockEvent =
			{
			   {MultiInstance.ADD_NPC, 2, 2, 5, "huoyan3", "huoyan3"},
			},
		},
		 [6] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {5},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 3, 3, 6, "zisha4", "zisha4"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.BLACK_MSG, Lang.mission.str934[Lang.Idx]},
			{MultiInstance.DEL_NPC,"mubiao"},
			},
		},
		 [7] = {nTime = 8, nNum = 0 ,		-- 火焰属性箱子开启
			tbPrelock = {6},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 4, 4, 7, "shandian", "shandian", 20, 2,"shandian_timer"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"shandian","shandian_timer"},
			{MultiInstance.ADD_NPC, 4, 5, 7, "shandian1", "shandian1"},
			},
		},
		[8] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {15},
			tbStartEvent =
			{
			{MultiInstance.SET_TRAP_INVALID, 17, 0},
			{MultiInstance.ADD_NPC, 8, 1, 8, "jiaxue", "jiaxue", 200, 30, "jiaxue_timer"},
			{MultiInstance.ADD_NPC, 10, 1, 1, "qiang2", "qiang2"},
			{MultiInstance.ADD_NPC, 1, 1, 8, "dajiangjun", "dajiangjun"},
			{MultiInstance.BLACK_MSG, Lang.mission.str935[Lang.Idx]},
			{MultiInstance.TARGET_INFO, Lang.mission.str933[Lang.Idx]},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DEL_NPC,"jiaxue","jiaxue_timer"},
				{MultiInstance.DEL_NPC,"fenshen1"},
				{MultiInstance.DEL_NPC,"fenshen"},

			},
		},
		 [9] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {7},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 5, 3, 9, "beisha", "beisha"},
			},
			tbUnLockEvent =
			{
			   --  {MultiInstance.BLACK_MSG, "大将军召唤出分身了！赶紧清理掉！"},
			   {MultiInstance.ADD_NPC, 6, 4, 9, "fenshen", "fenshen"},
			   	{MultiInstance.DEL_NPC,"beisha"},
			},
		},
		[10] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {9},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 5, 3, 10, "beisha1", "beisha"},
			},
			tbUnLockEvent =
			{
			  -- {MultiInstance.BLACK_MSG, "大将军召唤出分身了！赶紧清理掉！"},
			   {MultiInstance.ADD_NPC, 6, 4, 10, "fenshen1", "fenshen"},
			   	{MultiInstance.DEL_NPC,"beisha1"},
				{MultiInstance.DEL_NPC,"fenshen"},
			},
		},
		[11] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {10},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 5, 3, 11, "beisha2", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.BLACK_MSG, Lang.mission.str1191[Lang.Idx]},
			   {MultiInstance.ADD_NPC, 7, 3, 11, "feng", "feng"},
			   	{MultiInstance.DEL_NPC,"beisha2"},
			},
		},
		[12] = {nTime = 0, nNum = 1 ,		-- 火焰属性箱子开启
			tbPrelock = {11},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 5, 3, 12, "beisha3", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.BLACK_MSG, Lang.mission.str1191[Lang.Idx]},
			   {MultiInstance.ADD_NPC, 7, 3, 12, "feng1", "feng"},
			   	{MultiInstance.DEL_NPC,"beisha3"},
			},
		},
		[13] = {nTime = 10, nNum = 0 ,		-- 火焰属性箱子开启
			tbPrelock = {11},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"feng"},
			},
		},
		[14] = {nTime = 12, nNum = 0 ,		-- 火焰属性箱子开启
			tbPrelock = {12},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"feng1"},
			},
		},
		[15] = {nTime = 8, nNum = 0 ,		-- 火焰属性箱子开启
			tbPrelock = {7},
			tbStartEvent =
			{
			   {MultiInstance.ADD_NPC, 9, 1, 15, "dajiangjun1", "dajiangjun"},
			   {MultiInstance.NPC_BUBBLE, "dajiangjun1",Lang.mission.str937[Lang.Idx], 3},
			--{MultiInstance.TARGET_INFO, "关卡剧情中"},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DEL_NPC,"dajiangjun1"},
				{MultiInstance.DEL_NPC,"shandian1"},
			},
		},
		[16] = {nTime = 1, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {8},
			tbStartEvent =
			{
			{MultiInstance.SET_TRAP_INVALID, 16, 0},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
		},
		[17] = {nTime = 20, nNum = 0 ,		-- 通过后处理总计时
			tbPrelock = {16},
			tbStartEvent =
			{
				{MultiInstance.TARGET_INFO, Lang.mission.str857[Lang.Idx]},
				{MultiInstance.TIME_INFO, Lang.mission.str938[Lang.Idx], 4},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		}
    }
}
