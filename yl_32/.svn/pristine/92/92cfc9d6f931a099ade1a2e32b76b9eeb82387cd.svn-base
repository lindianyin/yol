-- 20级，普通难度，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

--惊险陡坡
tbMultiSetting[520]=
   {
	nTimeLimit = 605,  --本层时间20分钟
	nMapId = 520,
    tbBeginPos = {5100,888, 1048576},
	nGameType = MultiInstance.GAME_TYPE.LV_20_EASY,
	nMultiLevel = 20,
	nDifficulty = 1,

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
	       [1] = {nTemplate = 1000, nLevel = -1, nSeries = -1},		-- 标识
		   [2] = {nTemplate = 1001, nLevel = 40, nSeries = -1},		-- 石头中1
		   [3] = {nTemplate = 1002, nLevel = 40, nSeries = -1},		-- 石头中2
		   [4] = {nTemplate = 1003, nLevel = 40, nSeries = -1},		-- 石头左1
		   [5] = {nTemplate = 1004, nLevel = 40, nSeries = -1},		-- 石头左2
		   [6] = {nTemplate = 1005, nLevel = 40, nSeries = -1},		-- 石头右1
		   [7] = {nTemplate = 1006, nLevel = 40, nSeries = -1},		-- 石头右2
		   [8] = {nTemplate = 1007, nLevel = 40, nSeries = -1},		-- 自杀
		   [9] = {nTemplate = 1008, nLevel = 40, nSeries = -1},		-- 定时炸弹
		   [10] = {nTemplate = 1009, nLevel = 40, nSeries = -1},		-- 石龟
		   [11] = {nTemplate = 1010, nLevel = -1, nSeries = -1},		-- 墙
		   [12] = {nTemplate = 1011, nLevel = 40, nSeries = -1},		-- 假
		   [13] = {nTemplate = 1012, nLevel = 40, nSeries = -1},		-- 真
		   [14] = {nTemplate = 1013, nLevel = -1, nSeries = -1},		-- 定身
		   [15] = {nTemplate = 1014, nLevel = 40, nSeries = -1},		-- 超级巨石（山坡）
		   [16] = {nTemplate = 1015, nLevel = 40, nSeries = -1},		-- 超级巨石（山坡）
		   [17] = {nTemplate = 1016, nLevel = 40, nSeries = -1},		-- 真东西
		   [18] = {nTemplate = 1019, nLevel = -1, nSeries = -1},		-- 保护罩
		   [19] = {nTemplate = 1020, nLevel = -1, nSeries = -1},		-- 水晶
		   [20] = {nTemplate = 1021, nLevel = -1, nSeries = -1},		-- 巨石怪（剧情）
		   [21] = {nTemplate = 1022, nLevel = 40, nSeries = -1},		-- 巨石怪（攻击）
		   [22] = {nTemplate = 1023, nLevel = 40, nSeries = -1},		-- 石头支援BOSS
		   [23] = {nTemplate = 1024, nLevel = 40, nSeries = -1},		-- 石头支援BOSS
		   [24] = {nTemplate = 1025, nLevel = 40, nSeries = -1},		-- 石头支援BOSS
		   [25] = {nTemplate = 1026, nLevel = -1, nSeries = -1},		-- 透明被杀
		   [26] = {nTemplate = 1027, nLevel = 40, nSeries = -1},		-- 石龟
		   [27] = {nTemplate = 1028, nLevel = 40, nSeries = -1},		-- 水晶（困难）
		   [28] = {nTemplate = 1029, nLevel = 1, nSeries = -1},		-- 透明目标
		   [29] = {nTemplate = 1017, nLevel = 40, nSeries = -1},		-- 石头新增左
		   [30] = {nTemplate = 1018, nLevel = 40, nSeries = -1},		-- 石头新增右
		   [31] = {nTemplate = 1030, nLevel = 40, nSeries = -1},		-- 防御石柱
	},

   LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1217[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SET_TRAP_INVALID, 13, 1},
				{MultiInstance.SET_TRAP_INVALID, 14, 1},
				{MultiInstance.SET_TRAP_INVALID, 15, 1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 900, nNum = 0 ,		-- 本小关总时间
			tbPrelock = {1},
			tbStartEvent =
			{
                {MultiInstance.TIME_INFO, Lang.mission.str917[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 进入通道
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 2, 3, "biaoshi", "biaoshi"},
				{MultiInstance.ADD_NPC, 8, 5, 3, "zisha2", "zisha2"},
				{MultiInstance.ADD_NPC, 28, 1, 3, "mubiao", "mubiao"},
				{MultiInstance.TARGET_INFO, Lang.mission.str918[Lang.Idx]},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str919[Lang.Idx]},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DEL_NPC,"biaoshi"},
				{MultiInstance.DEL_NPC,"mubiao"},
				--{MultiInstance.ADD_NPC, 3, 1, 5, "shitou_m22", "shitou_m22"},
			   --{MultiInstance.ADD_NPC, 7, 1, 4, "shitou_r22", "shitou_r22"},
				{MultiInstance.ADD_NPC, 2, 1, 4, "shitou_m1", "shitou_m1", 100, 30, "shitou_timer1",0},                  --石头

			},
		},
		[4] = {nTime = 0, nNum = 30 ,		-- 第一小节：采集"冏"
			tbPrelock = {3},
			tbStartEvent =
			{

			    {MultiInstance.ADD_NPC, 9, 4, 4, "gui5", "gui5", 7, 5, "gui5_timer",0},
				{MultiInstance.ADD_NPC, 16, 2, 4, "gui6", "gui6"},
				{MultiInstance.TARGET_INFO, Lang.mission.str918[Lang.Idx]},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str919[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"shitou_m1", "shitou_timer1"},
			{MultiInstance.DEL_NPC,"shitou_m2", "shitou_timer2"},
			{MultiInstance.DEL_NPC,"shitou_l1", "shitou_timer3"},
			{MultiInstance.DEL_NPC,"shitou_l2", "shitou_timer4"},
			{MultiInstance.DEL_NPC,"shitou_r1", "shitou_timer5"},
			{MultiInstance.DEL_NPC,"shitou_r2", "shitou_timer6"},
			{MultiInstance.DEL_NPC,"shitou_m", "shitou_timer7"},
			{MultiInstance.DEL_NPC,"shitou_l", "shitou_timer8"},
			{MultiInstance.DEL_NPC,"shitou_r", "shitou_timer9"},
			{MultiInstance.DEL_NPC,"shitou_r11", "shitou_r11"},
			{MultiInstance.DEL_NPC,"shitou_l11", "shitou_l11"},
			{MultiInstance.DEL_NPC,"shitou_r111", "shitou_r111"},
			{MultiInstance.DEL_NPC,"zhen1"},
			{MultiInstance.DEL_NPC,"zhen2"},
			{MultiInstance.DEL_NPC,"zhen3"},
			{MultiInstance.DEL_NPC,"ding1"},
			{MultiInstance.DEL_NPC,"ding2"},
			{MultiInstance.DEL_NPC,"ding3"},
			{MultiInstance.DEL_NPC,"ding4"},
			{MultiInstance.DEL_NPC,"ding5"},
			{MultiInstance.DEL_NPC,"qiang"},
				{MultiInstance.SET_TRAP_INVALID, 11, 1},

			},
		},
		[5] = {nTime = 15, nNum = 0 ,		-- 石头（为了让石头不同时间滚下）
			tbPrelock = {3},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 4, 1, 4, "shitou_l1", "shitou_l1", 100, 20, "shitou_timer3",0},
			},
		},
		[6] = {nTime = 35, nNum = 1 ,		-- "囧"触发定身
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 30, 1, 4, "shitou_r111", "shitou_r11", 100, 50, "shitou_r111",0},
			},
		},
		[7] = {nTime = 0, nNum = 1 ,		-- "囧"触发定身
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 1 ,		-- "囧"触发定身
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 1 ,		-- "囧"触发定身
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 1 ,		-- "囧"触发定身
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 1 ,		-- 第二小节“红龟勿扰”
			tbPrelock = {4},
			tbStartEvent =
			{
				--{MultiInstance.ADD_NPC, 10, 18, 11, "gui", "gui"},
				{MultiInstance.ADD_NPC, 15, 1, 11, "juxing", "juxing"},
				--{MultiInstance.ADD_NPC, 10, 8, 11, "gui1", "gui1"},
				{MultiInstance.TARGET_INFO, Lang.mission.str920[Lang.Idx]},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str921[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			 {MultiInstance.DEL_NPC, "gui2"},
			{MultiInstance.DEL_NPC, "gui22"},
			{MultiInstance.DEL_NPC, "zhadan",},
			{MultiInstance.DEL_NPC,"qiang1"},
			{MultiInstance.SET_TRAP_INVALID, 12, 1},
			},
		},
		[12] = {nTime = 0, nNum = 18 ,		-- 第二小节“红龟莫打”
			tbPrelock = {11},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 0, nNum = 1 ,		-- 开启屏障
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
            -- trap失效
			},
		},
		[14] = {nTime = 0, nNum = 1 ,		-- 无用锁
			tbPrelock = {13},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[15] = {nTime = 0, nNum = 1 ,		-- 第三小节：斜坡石头开始滚
			tbPrelock = {17},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[16] = {nTime = 1, nNum = 0 ,		-- 无用锁
			tbPrelock = {11},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			},
		},
		[17] = {nTime = 0, nNum = 1 ,		-- 打掉这个开始滚石头
			tbPrelock = {16},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[18] = {nTime = 0, nNum = 12 ,		-- 第四节：巨石来啦！出现很多龟
			tbPrelock = {17},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[19] = {nTime = 0, nNum = 1 ,		-- 无用锁
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[20] = {nTime = 0, nNum = 0 ,		-- 无用锁
			tbPrelock = {19},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[21] = {nTime = 2, nNum = 0 ,		-- 无用锁
			tbPrelock = {20},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"baohu1"},
			{MultiInstance.DEL_NPC,"baohu2"},
			{MultiInstance.DEL_NPC,"baohu3"},
			{MultiInstance.DEL_NPC,"baohu4"},
			},
		},
		[22] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {11},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 27, 1, 22, "shuijing", "shuijing"},
			{MultiInstance.ADD_NPC, 8, 3, 22, "zisha1", "zisha1"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str922[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[23] = {nTime = 5, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {22},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 20, 1, 23, "jushiguai", "jushiguai"},
			{MultiInstance.NPC_BUBBLE, "jushiguai",Lang.mission.str923[Lang.Idx], 2},
			{MultiInstance.TARGET_INFO, Lang.mission.str924[Lang.Idx]},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str922[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"jushiguai"},
			},
		},
		[24] = {nTime = 5, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {22},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 26, 8, 24, "gui1", "gui1"},
			},
		},
		[25] = {nTime = 0, nNum = 1 ,		-- 第四节BOSS：巨石怪
			tbPrelock = {23},
			tbStartEvent =
			{
                {MultiInstance.ADD_NPC, 21, 1, 25, "jushiguai1", "jushiguai"},                     --BOSS出现
				{MultiInstance.TARGET_INFO, Lang.mission.str925[Lang.Idx]},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str926[Lang.Idx]},

			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_ALL_NPC},
			},
		},
		[26] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {23},
			tbStartEvent =
			{
                {MultiInstance.ADD_NPC, 25, 3, 26, "beisha", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"beisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str927[Lang.Idx]},
			},
		},
		[27] = {nTime = 0, nNum = 3 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {26},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 31, 3, 27, "fangyu", "fangyu"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1094},
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1093},
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1106},
			},
		},
		[28] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {26},
			tbStartEvent =
			{
                {MultiInstance.ADD_NPC, 25, 3, 28, "beisha1", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"beisha1"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str927[Lang.Idx]},
			},
		},
		[29] = {nTime = 0, nNum = 3 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {28},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 31, 3, 29, "fangyu", "fangyu"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1094},
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1093},
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1106},
			},
		},
		[30] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {28},
			tbStartEvent =
			{
				{MultiInstance.DEL_NPC,"gui2","gui_timer"},
                {MultiInstance.ADD_NPC, 25, 3, 30, "beisha2", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC,"beisha2"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str928[Lang.Idx]},
			},
		},
		[31] = {nTime = 0, nNum = 3 ,		-- 召唤防御石头
			tbPrelock = {30},
			tbStartEvent =
			{
				 {MultiInstance.ADD_NPC, 31, 3, 31, "fangyu", "fangyu"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1094},
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1093},
			{MultiInstance.CLEAR_NPC_BUFF, "jushiguai1", 1106},
			},
		},
		[32] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {30},
			tbStartEvent =
			{
                {MultiInstance.ADD_NPC, 25, 3, 32, "beisha3", "beisha"},
			},
			tbUnLockEvent =
			{
		        {MultiInstance.DEL_NPC,"beisha3"},
		        {MultiInstance.DEL_NPC,"shitou","shitou_timer"},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str928[Lang.Idx]},
			},
		},
		[33] = {nTime = 10, nNum = 0 ,		-- 补加到前面步骤（滚石头）
			tbPrelock = {5},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			 {MultiInstance.ADD_NPC, 3, 1, 5, "shitou_m", "shitou_m", 100, 30, "shitou_timer7",0},

			},
		},
		[34] = {nTime = 0, nNum = 1 ,		-- 无用锁
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{              -- trap失效
			},
		},
		[35] = {nTime = 1, nNum = 0 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {25},
			tbStartEvent =
			{
			{MultiInstance.SET_TRAP_INVALID, 11, 0},      -- trap点在关卡结束前一定要恢复生效
			{MultiInstance.SET_TRAP_INVALID, 12, 0},
			{MultiInstance.SET_TRAP_INVALID, 13, 0},
			{MultiInstance.SET_TRAP_INVALID, 14, 0},
			{MultiInstance.SET_TRAP_INVALID, 15, 0},

			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
		},
		[36] = {nTime = 5, nNum = 0 ,		-- 通过后处理总计时
			tbPrelock = {35},
			tbStartEvent =
			{
				{MultiInstance.TARGET_INFO, Lang.mission.str857[Lang.Idx]},
				{MultiInstance.TIME_INFO, Lang.mission.str929[Lang.Idx], 4},
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		}
    }
}

