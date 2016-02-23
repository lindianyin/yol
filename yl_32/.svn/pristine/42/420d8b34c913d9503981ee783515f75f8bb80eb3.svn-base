-- 50级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[303]=
{
	nTimeLimit = 600,  --本层时间20分钟
	nMapId = 303,
    tbBeginPos = {7466, 834, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 1420, nLevel = 25, nSeries = -1},		-- 被劫镖车
		[2] = {nTemplate = 1421, nLevel = 25, nSeries = -1},        --劫匪（戴帽）
		[3] = {nTemplate = 1422, nLevel = 25, nSeries = -1},        --劫匪（没戴帽）
		[4] = {nTemplate = 1423, nLevel = 25, nSeries = -1},        --劫匪(自由)
		[5] = {nTemplate = 1424, nLevel = 25, nSeries = -1},        --大劫匪
		[6] = {nTemplate = 1425, nLevel = 25, nSeries = -1},        --帐篷
		[7] = {nTemplate = 1426, nLevel = 25, nSeries = -1},        --箭塔
		[8] = {nTemplate = 1427, nLevel = -1, nSeries = -1},        --被踩自杀
		[9] = {nTemplate = 1428, nLevel = -1, nSeries = -1},        --出来自杀
		[10] = {nTemplate = 1429, nLevel = 25, nSeries = -1},        --劫匪头领
		[11] = {nTemplate = 1419, nLevel = -1, nSeries = -1},		-- 箭头
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
						    {MultiInstance.TIME_INFO, Lang.mission.str939[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str951[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 1, 2, "biaoche", "biaoche"},
				{MultiInstance.TARGET_INFO, Lang.mission.str940[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DO_SCRIPT, "self.bFailFinish= 1"},
			},
		},
		[22] = {nTime = 20, nNum = 0 ,		--让透明自杀出来晚点
			tbPrelock = {1},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 8, 1, 3, "zisha1", "zisha1"},
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 4, 8, 2, "ziyou", "ziyou"},                    --自由劫匪
			},
			tbUnLockEvent =
			{
			    {MultiInstance.ADD_NPC, 5, 1, 2, "dajiefei", "dajiefei", 50, 25,"dajiefei_timer",0},      --大劫匪
				{MultiInstance.ADD_NPC, 2, 1, 2, "jiefei1", "jiefei1"},
				{MultiInstance.ADD_NPC, 3, 2, 2, "jiefei11", "jiefei11"},
				{MultiInstance.ADD_NPC, 2, 1, 2, "jiefei2", "jiefei2"},
				{MultiInstance.ADD_NPC, 3, 2, 2, "jiefei22", "jiefei22"},
				--{MultiInstance.ADD_NPC, 11, 2, 2, "jiantou", "jiantou"},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str941[Lang.Idx]},
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {3},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 6, 1, 4, "zhangpeng1", "zhangpeng1"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.DEL_NPC, "jiefei1"},
			    {MultiInstance.DEL_NPC, "jiefei11"},
				{MultiInstance.ADD_NPC, 9, 1, 6, "chulai", "chulai"},
			},
		},
		[5] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {3},
			tbStartEvent =
			{

				{MultiInstance.ADD_NPC, 6, 1, 5, "zhangpeng2", "zhangpeng2"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.DEL_NPC,"jiefei2"},
			    {MultiInstance.DEL_NPC,"jiefei22"},
				{MultiInstance.ADD_NPC, 9, 1, 6, "chulai", "chulai"},
			},
		},
		[6] = {nTime = 0, nNum = 2 ,
			tbPrelock = {1},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str942[Lang.Idx]},
			{MultiInstance.CLEAR_NPC_BUFF, "biaoche", 3125},
			{MultiInstance.DEL_NPC,"dajiefei","dajiefei_timer"},
			{MultiInstance.ADD_NPC, 4, 6, 2, "ziyou1", "ziyou1", 2, 7, "ziyou2_timer",0},
			},
		},
		[23] = {nTime = 15, nNum = 0 ,		--让透明自杀出来晚点
			tbPrelock = {6},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 7, "zisha2", "zisha2"},
			},
		},
		[7] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {6},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{MultiInstance.ADD_NPC, 4, 6, 2, "ziyou2", "ziyou2", 3, 12, "ziyou2_timer",0},                    --自由劫匪
				{MultiInstance.ADD_NPC, 5, 2, 2, "dajiefie", "ziyou2", 2, 20,"dajiefei_timer",0},                            --大劫匪
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str943[Lang.Idx]},
			},
		},
		[8] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {7},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 7, 1, 8, "jianta1", "jianta1"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 9, 1, 10, "chulai", "chulai"},
			},
		},
		[9] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {7},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 7, 1, 9, "jianta2", "jianta2"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 9, 1, 10, "chulai", "chulai"},
				--
			},
		},
		[10] = {nTime = 0, nNum = 2 ,
			tbPrelock = {1},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG,Lang.mission.str942[Lang.Idx]},
			{MultiInstance.CLEAR_NPC_BUFF, "biaoche", 3125},
			{MultiInstance.ADD_NPC, 4, 8, 2, "ziyou22", "ziyou2", 2, 7, "ziyou2_timer",0},
			},
		},
		[24] = {nTime = 12, nNum = 0 ,		--让透明自杀出来晚点
			tbPrelock = {10},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 11, "zisha3", "zisha3"},
			},
		},
		[11] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {10},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{MultiInstance.ADD_NPC, 5, 3, 2, "dajiefie1", "dajiefei1", 2, 25,"dajiefei1_timer",0},                            --大劫匪
				{MultiInstance.ADD_NPC, 2, 2, 2, "jiefei3", "jiefei3"},
				{MultiInstance.ADD_NPC, 3, 1, 2, "jiefei33", "jiefei33"},
				{MultiInstance.ADD_NPC, 2, 2, 2, "jiefei4", "jiefei4"},
				{MultiInstance.ADD_NPC, 3, 1, 2, "jiefei44", "jiefei44"},
				{MultiInstance.ADD_NPC, 7, 1, 1, "jianta3", "jianta3"},
                {MultiInstance.ADD_NPC, 7, 1, 1, "jianta4", "jianta4"},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str941[Lang.Idx]},
			},
		},
		[12] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {11},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 6, 1, 12, "zhangpeng3", "zhangpeng3"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.DEL_NPC,"jiefei3"},
			    {MultiInstance.DEL_NPC,"jiefei33"},
				{MultiInstance.ADD_NPC, 9, 1, 14, "chulai", "chulai"},
			},
		},
		[13] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {11},
			tbStartEvent =
			{

				{MultiInstance.ADD_NPC, 6, 1, 13, "zhangpeng4", "zhangpeng4"},
			},
			tbUnLockEvent =
			{
			    {MultiInstance.DEL_NPC,"jiefei4"},
			    {MultiInstance.DEL_NPC,"jiefei44"},
				{MultiInstance.ADD_NPC, 9, 1, 14, "chulai", "chulai"},
			},
		},
		[14] = {nTime = 0, nNum = 2 ,
			tbPrelock = {1},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str944[Lang.Idx]},
			{MultiInstance.CLEAR_NPC_BUFF, "biaoche", 3125},
			},
		},
		[15] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {14},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 5, 2, 2, "ziyou3", "ziyou3"},
				{MultiInstance.ADD_NPC, 5, 2, 2, "ziyou4", "ziyou4"},
				{MultiInstance.ADD_NPC, 8, 1, 15, "zisha4", "zisha4"},
			},
			tbUnLockEvent =
			{
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str945[Lang.Idx]},
			},
		},
		[16] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {15},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 10, 1, 16, "boss", "boss"},
				{MultiInstance.TARGET_INFO, Lang.mission.str946[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str947[Lang.Idx]},
			},
		},
		[17] = {nTime = 3, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {16},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
		},
		[18] = {nTime = 10, nNum = 0 ,		-- 总计时
			tbPrelock = {17},
			tbStartEvent =
			{
				{MultiInstance.TARGET_INFO, Lang.mission.str857[Lang.Idx]},
				{MultiInstance.TIME_INFO, Lang.mission.str858[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		},
		[19] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 19, "zisha5", "zisha5"},                   --自由劫匪
			},
			tbUnLockEvent =
			{
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str948[Lang.Idx]},
			},
		},
		[20] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {6},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 20, "zisha6", "zisha6"},                   --自由劫匪
			},
			tbUnLockEvent =
			{
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str949[Lang.Idx]},
			},
		},
		[21] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {10},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 21, "zisha7", "zisha7"},                   --自由劫匪
			},
			tbUnLockEvent =
			{
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str950[Lang.Idx]},
			},
		}

    }
}
