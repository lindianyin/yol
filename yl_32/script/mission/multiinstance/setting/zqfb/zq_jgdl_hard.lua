-- 40级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[319]=
{
	nTimeLimit = 600,  --本层时间20分钟
	nMapId = 319,
    tbBeginPos = {2038, 2200, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 1470, nLevel = -1, nSeries = -1},		-- 机关人（假）
		[2] = {nTemplate = 1471, nLevel = -1, nSeries = -1},		-- 机关人（真）
		[3] = {nTemplate = 1472, nLevel = -1, nSeries = -1},		-- 机关兽（假）
		[4] = {nTemplate = 1473, nLevel = -1, nSeries = -1},		-- 机关兽（真）
		[5] = {nTemplate = 1474, nLevel = -1, nSeries = -1},		-- 机关邪童（假）
		[6] = {nTemplate = 1475, nLevel = -1, nSeries = -1},		-- 机关邪童（真）
		[7] = {nTemplate = 1476, nLevel = -1, nSeries = -1},		-- 机关傀儡
		[8] = {nTemplate = 1477, nLevel = -1, nSeries = -1},		-- 火把（没火）
		[9] = {nTemplate = 1478, nLevel = -1, nSeries = -1},		-- 火把（有火）
		[10] = {nTemplate = 1479, nLevel = -1, nSeries = -1},		-- 一出来自杀
		[11] = {nTemplate = 1480, nLevel = -1, nSeries = -1},		-- 使机关兽失灵
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.TIME_INFO, Lang.mission.str952[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str953[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 1, 10, 2, "ren", "ren"},
			    {MultiInstance.ADD_NPC, 1, 10, 2, "ren1", "ren1"},
				{MultiInstance.ADD_NPC, 1, 10, 2, "ren2", "ren2"},
				{MultiInstance.ADD_NPC, 2, 1, 2, "ren3", "ren3"},
                {MultiInstance.TARGET_INFO, Lang.mission.str954[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 4, "zisha", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 2, 1, 3, "ren3", "ren3"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 4, "zisha1", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[26] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 2, 1, 26, "ren3", "ren3"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 4, "zisha2", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[4] = {nTime = 0, nNum = 3 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			   {MultiInstance.ADD_NPC, 11, 3, 5, "shiling", "shiling"},
               {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str956[Lang.Idx]},
			},
		},
		[5] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {4},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 5, "huoju1", "huoju1"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju11", "huoju1"},
				{MultiInstance.ADD_NPC, 10, 1, 9, "zisha11", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju1"},
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {4},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 6, "huoju2", "huoju2"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju22", "huoju2"},
				{MultiInstance.ADD_NPC, 10, 1, 9, "zisha22", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju2"},
			},
		},
		[7] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {4},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 7, "huoju3", "huoju3"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju33", "huoju3"},
				{MultiInstance.ADD_NPC, 10, 1, 9, "zisha33", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju3"},
			},
		},
		[8] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {4},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 8, "huoju4", "huoju4"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju44", "huoju4"},
				{MultiInstance.ADD_NPC, 10, 1, 9, "zisha44", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju4"},
			},
		},
		[9] = {nTime = 0, nNum = 4 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str957[Lang.Idx]},
			{MultiInstance.DEL_NPC, "ren"},
			{MultiInstance.DEL_NPC, "ren1"},
			{MultiInstance.DEL_NPC, "ren2"},
			{MultiInstance.DEL_NPC, "shiling"},
			},
		},
		[10] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {9},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 3, 10, 2, "shou", "shou"},
			    {MultiInstance.ADD_NPC, 3, 10, 2, "shou1", "shou1"},
				{MultiInstance.ADD_NPC, 3, 10, 2, "shou2", "shou2"},
				{MultiInstance.ADD_NPC, 4, 1, 10, "shou3", "shou3"},
                {MultiInstance.TARGET_INFO, Lang.mission.str958[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 12, "zisha", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[11] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {9},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 4, 1, 11, "shou3", "shou3"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 12, "zisha1", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[27] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {9},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 4, 1, 27, "shou3", "shou3"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 12, "zisha2", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[12] = {nTime = 0, nNum = 3 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {9},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			   {MultiInstance.ADD_NPC, 11, 3, 5, "shiling1", "shiling"},
               {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str956[Lang.Idx]},
			},
		},
		[13] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {12},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 13, "huoju5", "huoju5"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju55", "huoju5"},
				{MultiInstance.ADD_NPC, 10, 1, 16, "zisha11", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju5"},
			},
		},
		[14] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {12},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 14, "huoju6", "huoju6"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju66", "huoju6"},
				{MultiInstance.ADD_NPC, 10, 1, 16, "zisha22", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju6"},
			},
		},
		[15] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {12},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 15, "huoju7", "huoju7"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju77", "huoju7"},
				{MultiInstance.ADD_NPC, 10, 1, 16, "zisha33", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju7"},
			},
		},
		[16] = {nTime = 0, nNum = 3 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str961[Lang.Idx]},
			{MultiInstance.DEL_NPC, "shou"},
			{MultiInstance.DEL_NPC, "shou1"},
			{MultiInstance.DEL_NPC, "shou2"},
			{MultiInstance.DEL_NPC, "shiling1"},
			},
		},
		[17] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {16},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 5, 10, 2, "tong", "tong"},
			    {MultiInstance.ADD_NPC, 5, 10, 2, "tong1", "tong1"},
				{MultiInstance.ADD_NPC, 5, 10, 2, "tong2", "tong2"},
				{MultiInstance.ADD_NPC, 6, 1, 17, "tong3", "tong3"},
                {MultiInstance.TARGET_INFO, Lang.mission.str958[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 19, "zisha", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[18] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {16},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 6, 1, 18, "tong3", "tong3"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 19, "zisha1", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[28] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {16},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 6, 1, 28, "tong3", "tong3"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 10, 1, 19, "zisha2", "zisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str955[Lang.Idx]},
			},
		},
		[19] = {nTime = 0, nNum = 3 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {16},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			   {MultiInstance.ADD_NPC, 11, 3, 5, "shiling2", "shiling"},
               {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str960[Lang.Idx]},
			},
		},
		[20] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {19},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 20, "huoju8", "huoju8"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 9, 1,5, "huoju88", "huoju8"},
				{MultiInstance.ADD_NPC, 10, 1, 21, "zisha11", "zisha11"},
				{MultiInstance.DEL_NPC, "huoju8"},
			},
		},
		[21] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {19},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str962[Lang.Idx]},
			{MultiInstance.DEL_NPC, "tong"},
			{MultiInstance.DEL_NPC, "tong1"},
			{MultiInstance.DEL_NPC, "tong2"},
			{MultiInstance.DEL_NPC, "shiling2"},
			},
		},
		[22] = {nTime = 2, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {20},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "huoju11"},
			{MultiInstance.DEL_NPC, "huoju22"},
			{MultiInstance.DEL_NPC, "huoju33"},
			{MultiInstance.DEL_NPC, "huoju44"},
			{MultiInstance.DEL_NPC, "huoju55"},
			{MultiInstance.DEL_NPC, "huoju66"},
			{MultiInstance.DEL_NPC, "huoju77"},
			{MultiInstance.DEL_NPC, "huoju88"},
			},
		},
		[23] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {22},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 7, 1, 23, "boss", "boss"},
			},
			tbUnLockEvent =
			{
			},
		},
		[24] = {nTime = 1, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {23},
			tbStartEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str963[Lang.Idx]},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
	    },
		[25] = {nTime = 10, nNum = 0 ,		-- 总计时
			tbPrelock = {24},
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
