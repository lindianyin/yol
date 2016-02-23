Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

--密室逃脱
tbMultiSetting[521]=
{
	nTimeLimit = 605,  --本关时间5分钟
	nMapId = 521,
    tbBeginPos = {1095, 909, 1048576},
	nGameType = MultiInstance.GAME_TYPE.LV_20_EASY,
	nMultiLevel = 20,
	nDifficulty = 1,

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	       [1] = {nTemplate = 1084, nLevel = -1, nSeries = -1},		-- 水晶1~2
		   [2] = {nTemplate = 1085, nLevel = -1, nSeries = -1},		-- 水晶2~3
		   [3] = {nTemplate = 1052, nLevel = -1, nSeries = -1},		-- 水晶3~4
		   [4] = {nTemplate = 1086, nLevel = -1, nSeries = -1},		-- 水晶boss
		   [5] = {nTemplate = 1054, nLevel = 1, nSeries = -1},		-- 火焰木箱
		   [6] = {nTemplate = 1055, nLevel = 1, nSeries = -1},		-- 速度木箱
		   [7] = {nTemplate = 1056, nLevel = 1, nSeries = -1},		-- 恢复木箱
		   [8] = {nTemplate = 1057, nLevel = -1, nSeries = -1},		-- 火焰buff
		   [9] = {nTemplate = 1058, nLevel = -1, nSeries = -1},		-- 速度uff
		   [10] = {nTemplate = 1059, nLevel = -1, nSeries = -1},		-- 恢复buff
		   [11] = {nTemplate = 1060, nLevel = 30, nSeries = -1},		-- 魔兵
		   [12] = {nTemplate = 1061, nLevel = 30, nSeries = -1},		-- 魔兵弓箭手
		   [13] = {nTemplate = 1062, nLevel = 30, nSeries = -1},		-- 魔将
		   [14] = {nTemplate = 1063, nLevel = 30, nSeries = -1},		-- 魔将
		   [15] = {nTemplate = 1064, nLevel = 30, nSeries = -1},		-- 怨灵
		   [16] = {nTemplate = 1065, nLevel = -1, nSeries = -1},		-- 圣光
		   [17] = {nTemplate = 1066, nLevel = 30, nSeries = -1},		-- 妖王
		    [18] = {nTemplate = 1067, nLevel = -1, nSeries = -1},		-- 一出来就自杀
		   [19] = {nTemplate = 1068, nLevel = 25, nSeries = -1},		-- 缸
		   [20] = {nTemplate = 1069, nLevel = 1, nSeries = -1},		-- 人踩自杀
		   [21] = {nTemplate = 1070, nLevel = 30, nSeries = -1},		-- 鬼魂
		   [22] = {nTemplate = 1071, nLevel = 30, nSeries = -1},		-- 鬼魂
		   [23] = {nTemplate = 1072, nLevel = 30, nSeries = -1},		-- 鬼魂
		   [24] = {nTemplate = 1073, nLevel = 30, nSeries = -1},		-- 鬼魂
		    [25] = {nTemplate = 1074, nLevel = 30, nSeries = -1},		-- 凶猛鬼魂
			[26] = {nTemplate = 1075, nLevel = 30, nSeries = -1},		-- 召唤鬼魂
		   [27] = {nTemplate = 1076, nLevel = 30, nSeries = -1},		-- 召唤鬼魂
		   [28] = {nTemplate = 1077, nLevel = 30, nSeries = -1},		-- 召唤鬼魂
		   [29] = {nTemplate = 1078, nLevel = 30, nSeries = -1},		-- 召唤鬼魂
		   [30] = {nTemplate = 1079, nLevel = -1, nSeries = -1},		-- 透明被杀
		    [31] = {nTemplate = 1080, nLevel = -1, nSeries = -1},		-- 帮妖王加血
			[32] = {nTemplate = 1081, nLevel = -1, nSeries = -1},		-- 妖王追人大技
			[33] = {nTemplate = 1082, nLevel = -1, nSeries = -1},		-- 箭头
			[34] = {nTemplate = 1083, nLevel = 30, nSeries = -1},		-- 凶鬼
	},
    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
				{MultiInstance.REVIVE_INFO, true, true, false},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1217[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 900, nNum = 0 ,		-- 时间倒计时，时间到了，系统会自动走退出流程
			tbPrelock = {1},
			tbStartEvent =
			{
                {MultiInstance.TIME_INFO, Lang.mission.str1140[Lang.Idx]},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1141[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
	    [3] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 1 ,        -- 无用了
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 1 ,        -- 无用了
			tbPrelock = {9},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 30 ,		-- 第一节点：消灭所有魔兵
			tbPrelock = {1},
			tbStartEvent =
			{
              {MultiInstance.ADD_NPC, 11, 18, 11, "mobing", "mobing"},
			   {MultiInstance.ADD_NPC, 12, 10, 11, "gongjian", "gongjian"},
			   {MultiInstance.ADD_NPC, 13, 1, 11, "mobing1", "mobing1"},
			   {MultiInstance.ADD_NPC, 14, 1, 11, "mobing2", "mobing2"},
				{MultiInstance.TARGET_INFO, Lang.mission.str1142[Lang.Idx]},
			},
			tbUnLockEvent =
			{
		     	{MultiInstance.ADD_NPC, 2, 1, 11, "shuijing2", "shuijing2"},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1143[Lang.Idx]},
			},
		},
		[12] = {nTime = 0, nNum = 1 ,		-- 第二水晶开启
			tbPrelock = {11},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 20, 1, 12, "zisha2", "zisha2"},
			   {MultiInstance.TARGET_INFO, Lang.mission.str1144[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			   {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1145[Lang.Idx]},
			},
		},
		[13] = {nTime = 0, nNum = 1 ,		-- 第二节点：消灭怨灵
			tbPrelock = {12},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 15, 1, 13, "yuanling", "yuanling"},
			   {MultiInstance.ADD_NPC, 16, 1, 13, "shengguang", "shengguang", 100, 10,"shengguang_timer"},
				{MultiInstance.TARGET_INFO, Lang.mission.str1146[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			   {MultiInstance.ADD_NPC, 4, 1, 2, "shuijing4", "shuijing4"},
			   {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1143[Lang.Idx]},
			   {MultiInstance.DEL_NPC,"shengguang","shengguang_timer"},
			},
		},
		[14] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {13},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {14},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[16] = {nTime = 0, nNum = 1 ,		-- 无用了
			tbPrelock = {14},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[17] = {nTime = 0, nNum = 2 ,		-- 无用了
			tbPrelock = {14},
		    tbStartEvent =
			{
			},
			tbUnLockEvent =
		   {
			},
		},
		[18] = {nTime = 0, nNum = 1 ,		-- 第四水晶开启
			tbPrelock = {13},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 20, 1, 18, "zisha4", "zisha4"},
			   {MultiInstance.TARGET_INFO, Lang.mission.str1147[Lang.Idx]},
			    {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1148[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			   {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1149[Lang.Idx]},
			},
		},
		[19] = {nTime = 0, nNum = 1 ,		-- 第四节点：消灭BOSS妖王
			tbPrelock = {18},
			tbStartEvent =
			{
               {MultiInstance.ADD_NPC, 17, 1, 19, "yaowang", "yaowang"},
			   {MultiInstance.NPC_BUBBLE, "yaowang",Lang.mission.str1150[Lang.Idx], 2},
				{MultiInstance.TARGET_INFO, Lang.mission.str1151[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_ALL_NPC},              --删除所有NPC
			 {MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str1218[Lang.Idx]},
			},
		},
		[20] = {nTime = 0, nNum = 1 ,		-- 辅助妖王BOSS（被杀）
			tbPrelock = {18},
			tbStartEvent =
			{
              -- {MultiInstance.ADD_NPC, 30, 3, 20, "beisha", "beisha"},
			},
			tbUnLockEvent =
			{
			 --  {MultiInstance.DEL_NPC,"beisha"},
			},
		},
		[21] = {nTime = 30, nNum = 0 ,		-- 第一次召唤一堆小鬼
			tbPrelock = {20},
			tbStartEvent =
			{
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan1", "zhaohuan1"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan2", "zhaohuan2"},
				--{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan3", "zhaohuan3"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan4", "zhaohuan4"},
			  -- {MultiInstance.SHOW_NOTICE_MSG, "妖王召唤出小鬼了！赶快清掉！"},
			},
			tbUnLockEvent =
			{
			},
		},
		[22] = {nTime = 0, nNum = 1 ,		-- 辅助妖王（被杀）
			tbPrelock = {20},
			tbStartEvent =
			{
              -- {MultiInstance.ADD_NPC, 30, 3, 22, "beisha1", "beisha"},
			},
			tbUnLockEvent =
			{
			 --  {MultiInstance.DEL_NPC,"beisha1"},
			--   {MultiInstance.DEL_NPC,"zhaohuan1"},
			--   {MultiInstance.DEL_NPC,"zhaohuan2"},
			--   {MultiInstance.DEL_NPC,"zhaohuan3"},
			--   {MultiInstance.DEL_NPC,"zhaohuan4"},
			},
		},
		[23] = {nTime = 30, nNum = 0 ,		-- 第二次召唤一堆小鬼
			tbPrelock = {22},
			tbStartEvent =
			{
				--{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan11", "zhaohuan1"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan22", "zhaohuan2"},
			--	{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan33", "zhaohuan3"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan44", "zhaohuan4"},
			 --  {MultiInstance.SHOW_NOTICE_MSG, "妖王召唤出小鬼了！赶快清掉！"},
			},
			tbUnLockEvent =
			{
			},
		},
		[24] = {nTime = 0, nNum = 1 ,		-- 辅助妖王
			tbPrelock = {22},
			tbStartEvent =
			{
             --  {MultiInstance.ADD_NPC, 30, 3, 24, "beisha2", "beisha"},
			},
			tbUnLockEvent =
			{
			 --  {MultiInstance.DEL_NPC,"beisha2"},
			 --  {MultiInstance.DEL_NPC,"zhaohuan11"},
			--   {MultiInstance.DEL_NPC,"zhaohuan22"},
			--   {MultiInstance.DEL_NPC,"zhaohuan33"},
			 --  {MultiInstance.DEL_NPC,"zhaohuan44"},
			},
		},
		[25] = {nTime = 30, nNum = 0 ,		-- 召唤加血小鬼
			tbPrelock = {24},
			tbStartEvent =
			{
			--	{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan111", "zhaohuan1"},
			 --   {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan222", "zhaohuan2"},
			--	{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan333", "zhaohuan3"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan444", "zhaohuan4"},
			 -- {MultiInstance.ADD_NPC, 31, 1, 25, "jiaxue", "jiaxue", 200, 7, "jiaxue_timer"},
			 -- {MultiInstance.SHOW_NOTICE_MSG, "妖王暴走了！"},
			},
			tbUnLockEvent =
			{
			},
		},
		[26] = {nTime = 0, nNum = 1 ,		-- 辅助妖王
			tbPrelock = {24},
			tbStartEvent =
			{
              -- {MultiInstance.ADD_NPC, 30, 3, 26, "beisha3", "beisha"},
			},
			tbUnLockEvent =
			{
			  -- {MultiInstance.DEL_NPC,"beisha3"},
			  -- {MultiInstance.DEL_NPC,"zhaohuan111"},
			 --  {MultiInstance.DEL_NPC,"zhaohuan222"},
			  -- {MultiInstance.DEL_NPC,"zhaohuan333"},
			  -- {MultiInstance.DEL_NPC,"zhaohuan444"},
			},
		},
		[27] = {nTime = 30, nNum = 0 ,		-- 召唤加血小鬼
			tbPrelock = {26},
			tbStartEvent =
			{
				--{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan1111", "zhaohuan1"},
			   -- {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan2222", "zhaohuan2"},
				--{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan3333", "zhaohuan3"},
			   -- {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan4444", "zhaohuan4"},
			},
			tbUnLockEvent =
			{
			},
		},
		[28] = {nTime = 0, nNum = 1 ,		-- 辅助妖王
			tbPrelock = {26},
			tbStartEvent =
			{
              -- {MultiInstance.ADD_NPC, 30, 3, 28, "beisha4", "beisha"},
			},
			tbUnLockEvent =
			{
			  -- {MultiInstance.DEL_NPC,"beisha4"},
			  -- {MultiInstance.DEL_NPC,"zhaohuan1111"},
			  -- {MultiInstance.DEL_NPC,"zhaohuan2222"},
			  -- {MultiInstance.DEL_NPC,"zhaohuan3333"},
			  -- {MultiInstance.DEL_NPC,"zhaohuan4444"},
			},
		},
		[29] = {nTime = 30, nNum = 0 ,		-- 召唤加血小鬼
			tbPrelock = {28},
			tbStartEvent =
			{
				--{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan11111", "zhaohuan1"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan22222", "zhaohuan2"},
				--{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan33333", "zhaohuan3"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan44444", "zhaohuan4"},
			},
			tbUnLockEvent =
			{
			},
		},
		[30] = {nTime = 0, nNum = 1 ,		-- 辅助妖王
			tbPrelock = {28},
			tbStartEvent =
			{
              -- {MultiInstance.ADD_NPC, 30, 3, 30, "beisha5", "beisha"},
			},
			tbUnLockEvent =
			{
			 --  {MultiInstance.DEL_NPC,"beisha5"},
			 --  {MultiInstance.DEL_NPC,"zhaohuan11111"},
			 --  {MultiInstance.DEL_NPC,"zhaohuan22222"},
			 --  {MultiInstance.DEL_NPC,"zhaohuan33333"},
			 --  {MultiInstance.DEL_NPC,"zhaohuan44444"},
			},
		},
		[31] = {nTime = 30, nNum = 0 ,		-- 召唤加血小鬼
			tbPrelock = {30},
			tbStartEvent =
			{
			--	{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan111111", "zhaohuan1"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan222222", "zhaohuan2"},
			--	{MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan333333", "zhaohuan3"},
			  --  {MultiInstance.ADD_NPC, 34, 2, 21, "zhaohuan444444", "zhaohuan4"},
			},
			tbUnLockEvent =
			{
			},
		},
		[32] = {nTime = 1, nNum = 0 ,		-- 删除所有BUFF
			tbPrelock = {19},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bCanTakeAward = 1"},
			},
		},
		[33] = {nTime = 5, nNum = 0 ,		-- 通过后处理总计时
			tbPrelock = {32},
			tbStartEvent =
			{
				{MultiInstance.TARGET_INFO, Lang.mission.str857[Lang.Idx]},
				{MultiInstance.TIME_INFO, Lang.mission.str1152[Lang.Idx], 4},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		}
    }
}
