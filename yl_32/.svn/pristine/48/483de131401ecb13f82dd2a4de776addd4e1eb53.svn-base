-- 60级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[603]=
{
	nTimeLimit = 600,  --本层时间20分钟
	nMapId = 603,
    tbBeginPos = {2693,1232, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 2415, nLevel = -1, nSeries = -1},		-- 白骨精（剧情）
		[2] = {nTemplate = 2416, nLevel = -1, nSeries = -1},		-- 白骨精
		[3] = {nTemplate = 2417, nLevel = -1, nSeries = -1},		-- 骨弓
		[4] = {nTemplate = 2418, nLevel = -1, nSeries = -1},		-- 骨剑
		[5] = {nTemplate = 2419, nLevel = -1, nSeries = -1},		-- 透明被杀
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.TIME_INFO, Lang.mission.str1162[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1163[Lang.Idx]},
				{MultiInstance.TARGET_INFO, Lang.mission.str1164[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 4, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 1, 2, "boss", "boss"}, --
				{MultiInstance.NPC_BUBBLE, "boss",Lang.mission.str1165[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
                {MultiInstance.DEL_NPC, "boss"},
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {2},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 2, 1, 3, "boss1", "boss"}, --
				{MultiInstance.TARGET_INFO, Lang.mission.str1166[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{

			},
		},
		[4] = {nTime = 0, nNum = 1 ,            --1
			tbPrelock = {1},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 5, 1, 4, "beisha", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian", "jian"},
			},
		},
		[5] = {nTime = 20, nNum = 0 ,
			tbPrelock = {4},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 1 ,            --2
			tbPrelock = {5},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 6, "beisha1", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong1", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian1", "jian"},
			},
		},
		[7] = {nTime = 20, nNum = 0 ,
			tbPrelock = {6},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 1 ,            --3
			tbPrelock = {7},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 8, "beisha2", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong2", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian2", "jian"},
			},
		},
		[9] = {nTime = 20, nNum = 0 ,
			tbPrelock = {8},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 1 ,             --4
			tbPrelock = {9},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 10, "beisha3", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong3", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian3", "jian"},
			},
		},
		[11] = {nTime = 20, nNum = 0 ,
			tbPrelock = {10},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 0, nNum = 1 ,              --5
			tbPrelock = {11},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 12, "beisha4", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong4", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian4", "jian"},
			},
		},
		[13] = {nTime = 20, nNum = 0 ,
			tbPrelock = {12},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 1 ,              --6
			tbPrelock = {13},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 14, "beisha5", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong5", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian5", "jian"},
			},
		},
		[15] = {nTime = 20, nNum = 0 ,
			tbPrelock = {14},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[16] = {nTime = 0, nNum = 1 ,              --7
			tbPrelock = {15},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 16, "beisha6", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong6", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian6", "jian"},
			},
		},
		[17] = {nTime = 20, nNum = 0 ,
			tbPrelock = {16},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[18] = {nTime = 0, nNum = 1 ,              --8
			tbPrelock = {17},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 18, "beisha7", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong7", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian7", "jian"},
			},
		},
		[19] = {nTime = 20, nNum = 0 ,
			tbPrelock = {18},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[20] = {nTime = 0, nNum = 1 ,              --9
			tbPrelock = {19},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 20, "beisha8", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong8", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian8", "jian"},
			},
		},
		[21] = {nTime = 20, nNum = 0 ,
			tbPrelock = {20},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[22] = {nTime = 0, nNum = 1 ,              --10
			tbPrelock = {21},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 22, "beisha9", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong9", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian9", "jian"},
			},
		},
		[23] = {nTime = 20, nNum = 0 ,
			tbPrelock = {22},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[24] = {nTime = 0, nNum = 1 ,              --11
			tbPrelock = {23},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 24, "beisha10", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong10", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian10", "jian"},
			},
		},
		[25] = {nTime = 20, nNum = 0 ,
			tbPrelock = {24},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[26] = {nTime = 0, nNum = 1 ,              --12
			tbPrelock = {25},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 26, "beisha11", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong11", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian11", "jian"},
			},
		},
		[27] = {nTime = 20, nNum = 0 ,
			tbPrelock = {26},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[28] = {nTime = 0, nNum = 1 ,              --13
			tbPrelock = {27},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 28, "beisha12", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong12", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian12", "jian"},
			},
		},
		[29] = {nTime = 20, nNum = 0 ,
			tbPrelock = {28},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[30] = {nTime = 0, nNum = 1 ,              --14
			tbPrelock = {29},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 5, 1, 29, "beisha13", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 3, 3, 4, "gong13", "gong"},
			{MultiInstance.ADD_NPC, 4, 4, 4, "jian13", "jian"},
			},
		},
		[31] = {nTime = 1, nNum = 0 ,
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
		},
		[32] = {nTime = 10, nNum = 0 ,		-- 总计时
			tbPrelock = {31},
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
