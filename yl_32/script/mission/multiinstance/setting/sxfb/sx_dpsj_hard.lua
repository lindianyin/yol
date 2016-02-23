-- 20级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[523]=
{
	nTimeLimit = 605,  --本层时间20分钟
	nMapId = 523,
    tbBeginPos = {2545, 1420, 1048576},
	nGameType = MultiInstance.GAME_TYPE.LV_20_EASY,
	nMultiLevel = 50,
	nDifficulty = 1,

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 1440, nLevel = 20, nSeries = -1},		-- 魔——》人
		[2] = {nTemplate = 1441, nLevel = 20, nSeries = -1},		-- 魔——》神
		[3] = {nTemplate = 1442, nLevel = 20, nSeries = -1},		-- 人——》魔
		[4] = {nTemplate = 1443, nLevel = 20, nSeries = -1},		-- 人——》神
		[5] = {nTemplate = 1444, nLevel = 20, nSeries = -1},		-- 神——》人
		[6] = {nTemplate = 1445, nLevel = 20, nSeries = -1},		-- 神——》魔
		[7] = {nTemplate = 1446, nLevel = 20, nSeries = -1},		-- 利爪鬼
		[8] = {nTemplate = 1447, nLevel = 20, nSeries = -1},		-- 透明自杀
		[9] = {nTemplate = 1448, nLevel = 20, nSeries = -1},		-- 鞭妖（剧情）
		[10] = {nTemplate = 1449, nLevel = 20, nSeries = -1},		-- 鞭妖（开打）
		[11] = {nTemplate = 1450, nLevel = 20, nSeries = -1},		-- 魔物（剧情?
		[12] = {nTemplate = 1451, nLevel = 20, nSeries = -1},		-- 流星锤
		[13] = {nTemplate = 1452, nLevel = 20, nSeries = -1},		-- 弓鬼
		[14] = {nTemplate = 1453, nLevel = 20, nSeries = -1},		-- 炮塔
		[15] = {nTemplate = 1454, nLevel = 20, nSeries = -1},		-- 刀鬼
		[16] = {nTemplate = 1455, nLevel = 20, nSeries = -1},		-- 炮塔（冰）
		[17] = {nTemplate = 1456, nLevel = 20, nSeries = -1},		-- 巨斧魔
		[18] = {nTemplate = 1457, nLevel = 20, nSeries = -1},		-- 魔兵

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				--{MultiInstance.SHOW_NOTICE_MSG, "注意！怪物5秒后到达此地！"},
				{MultiInstance.REVIVE_INFO, true, true, false},
			},
			tbUnLockEvent =
			{
			--{MultiInstance.DEL_NPC, "yinxingBUFF"},             --删除帮玩家加隐形BUFF的NPC
			},
		},
		[2] = {nTime = 900, nNum = 0 ,
			tbPrelock = {1},
			tbStartEvent =
			{
                {MultiInstance.TIME_INFO, Lang.mission.str902[Lang.Idx]},
				{MultiInstance.TARGET_INFO, Lang.mission.str903[Lang.Idx]},



			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 1 ,
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 3, 3, "zisha", "zisha"},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str904[Lang.Idx]},

			},
			tbUnLockEvent =
			{
			    {MultiInstance.DEL_NPC,"zisha"},
			},
		},
		[4] = {nTime = 0, nNum = 12 ,
			tbPrelock = {3},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 7, 6, 4, "gui", "gui", 2, 4, "gui_timer", 0},
				{MultiInstance.TARGET_INFO, Lang.mission.str903[Lang.Idx]},
                {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str906[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 1 ,
			tbPrelock = {4},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 2, 5, "zisha1", "zisha1"},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str907[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			   {MultiInstance.DEL_NPC, "zisha1"},
			},
		},
		[6] = {nTime = 0, nNum = 12 ,
			tbPrelock = {5},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 7, 6, 6, "gui1", "gui1", 2, 5, "gui1_timer", 0},
                {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str908[Lang.Idx]},
				{MultiInstance.TARGET_INFO, Lang.mission.str903[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			   {MultiInstance.ADD_NPC, 8, 1, 3, "zisha2", "zisha2"},
			},
		},
		[7] = {nTime = 7, nNum = 0 ,
			tbPrelock = {6},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 2, 1, 2, "2", "chuansong1"},
				{MultiInstance.ADD_NPC, 9, 1, 4, "boss", "boss"},
				{MultiInstance.NPC_BUBBLE, "boss",Lang.mission.str909[Lang.Idx], 2},
				{MultiInstance.ADD_NPC, 11, 2, 4, "mowu", "mowu", 7, 2,"mowu_timer"},

			},
			tbUnLockEvent =
			{
			    {MultiInstance.NPC_BUBBLE, "boss",Lang.mission.str910[Lang.Idx], 1},
			    {MultiInstance.DEL_NPC, "mowu","mowu_timer"}
			},
		},
		[8] = {nTime = 0, nNum = 1 ,
			tbPrelock = {7},
			tbStartEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str911[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 21 ,
			tbPrelock = {6},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 15, 8, 9, "dao", "dao"},
				{MultiInstance.ADD_NPC, 15, 6, 9, "gui1", "gui1"},
				{MultiInstance.ADD_NPC, 12, 1, 9, "chui1", "chui1"},
				{MultiInstance.ADD_NPC, 14, 1, 9, "paota", "paota"},
				{MultiInstance.ADD_NPC, 13, 5, 9, "gong", "gong"},
				{MultiInstance.TARGET_INFO, Lang.mission.str903[Lang.Idx]},

			},
			tbUnLockEvent =
			{
				{MultiInstance.ADD_NPC, 3, 1, 2, "3", "chuansong4"},
			},
		},
		[10] = {nTime = 0, nNum = 1 ,
			tbPrelock = {9},
			tbStartEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str913[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 26 ,
			tbPrelock = {9},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 16, 1, 11, "paota1", "paota1"},
				{MultiInstance.ADD_NPC, 16, 1, 11, "paota2", "paota2"},
				{MultiInstance.ADD_NPC, 18, 10, 11, "mobing", "mobing"},
				{MultiInstance.ADD_NPC, 18, 7, 11, "mobing1", "mobing1"},
				{MultiInstance.ADD_NPC, 18, 6, 11, "mobing2", "mobing2"},
				{MultiInstance.ADD_NPC, 17, 1, 11, "futou", "futou"},
				{MultiInstance.TARGET_INFO, Lang.mission.str903[Lang.Idx]},
			},
			tbUnLockEvent =
			{
				{MultiInstance.ADD_NPC, 5, 1, 2, "5", "chuansong6"},
				{MultiInstance.DEL_NPC, "1"},
				{MultiInstance.DEL_NPC, "boss"},
			},
		},
		[12] = {nTime = 0, nNum = 1 ,
			tbPrelock = {11},
			tbStartEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str915[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 0, nNum = 1 ,
			tbPrelock = {11},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 10, 1, 13, "boss1", "boss1"},
				{MultiInstance.TARGET_INFO, Lang.mission.str916[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 2, nNum = 0 ,          --{第五波怪童女}
			tbPrelock = {13},
			tbStartEvent =
			{
			{MultiInstance.TARGET_INFO, Lang.mission.str236[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		},
    }
}
