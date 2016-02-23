Include("script/mission/instance/instance_def.lua")
if not Instance.tbSetting then
	Instance.tbSetting = {}
end

local InstanceSetting = Instance.tbSetting;

--小镇河道
InstanceSetting[104]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 104;
    tbBeginPos = {1760,892,1048576},
	tbLeavePos = {4,6295,7080,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 75, nLevel = -1, nSeries = -1},		-- 猪八戒（剧情1）
	  [2] = {nTemplate = 76, nLevel = 6, nSeries = -1},		   -- 河妖
	  [3] = {nTemplate = 77, nLevel = 6, nSeries = -1},		   -- 河妖
	  [4] = {nTemplate = 78, nLevel = -1, nSeries = -1},		   -- 隐形
	  [5] = {nTemplate = 83, nLevel = -1, nSeries = -1},		   -- 河妖（剧情1）
	  [6] = {nTemplate = 79, nLevel = -1, nSeries = -1},		   -- 河妖（剧情2）
	  [7] = {nTemplate = 81, nLevel = -1, nSeries = -1},		-- 大公鸡（剧情2）
	  [8] = {nTemplate = 82, nLevel = -1, nSeries = -1},		-- 小鸡3
	  [9] = {nTemplate = 80, nLevel = -1, nSeries = -1},		-- 猪八戒（剧情2）
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 1, 1, 2, "guaiwu8", "xiaoji1"},
			--{Instance.ADD_NPC, 7, 1, 2, "guaiwu8", "xiaoji2"},
			--{Instance.ADD_NPC, 8, 1, 2, "guaiwu8", "xiaoji3"},
			{Instance.ADD_NPC, 1, 1, 2, "bajie", "bajie"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 6, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{

			{Instance.ADD_NPC, 5, 1, 4, "heyao10", "heyao10"},
			{Instance.NPC_BUBBLE, -1,"heyao10",Lang.mission.str427[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"bajie",Lang.mission.str428[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"bajie"},
			{Instance.DEL_NPC,"heyao10"},
			},
		},
		[5] = {nTime = 0, nNum = 9 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 8, 5, "guaiwu3", "heyao"},
			{Instance.ADD_NPC, 2, 1, 5, "guaiwu4", "heyao1"},
			{Instance.NPC_BUBBLE, -1,"guaiwu4",Lang.mission.str429[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str430[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 5, nNum = 0  ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 9, 1, 6, "bajie1", "bajie1"},
			{Instance.ADD_NPC, 7, 1, 6, "dagongji", "dagongji"},
			{Instance.ADD_NPC, 6, 1, 6, "heyao9", "heyao9"},
			{Instance.ADD_NPC, 4, 1, 6, "yinxing", "yinxing"},
			{Instance.NPC_BUBBLE, -1,"bajie1",Lang.mission.str431[Lang.Idx], 1},
			--{Instance.NPC_BUBBLE, -1,"heyao9","help~!", 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"bajie1"},
			{Instance.DEL_NPC,"dagongji"},
			{Instance.DEL_NPC,"heyao9"},
			{Instance.DEL_NPC,"yinxing"},
			},
		},
		[7] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 6  ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 5, 8, "guaiwu3", "heyao2"},
			{Instance.ADD_NPC, 3, 1, 8, "guaiwu4", "heyao8"},
			{Instance.NPC_BUBBLE, -1,"guaiwu4",Lang.mission.str432[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str433[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		--[9] = {nTime = 0, nNum = 11  ,		-- 先去掉
		--	tbPrelock = {8},
		--	tbStartEvent =
		--	{
		--	{Instance.ADD_NPC, 3, 6, 9, "guaiwu3", "heyao2"},
		--	{Instance.ADD_NPC, 2, 5, 9, "guaiwu3", "heyao"},
		--	{Instance.TARGET_INFO, -1, "消灭所有河妖"},
		--	},
		--	tbUnLockEvent =
		--	{

		--	},
	--	},
		[9] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[10]:Close()"},
			},
		},
		[10] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--夜护城河
InstanceSetting[105]=
{
	nTaskInstance = 1;
    nMinLevel = 7;
    nMaxLevel = 60;
	nMapId = 105;
    tbBeginPos = {1990,492,1048576},
	tbLeavePos = {4,6295,7080,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 85, nLevel = 6, nSeries = -1},		   -- 河妖
	  [2] = {nTemplate = 86, nLevel = 6, nSeries = -1},		   -- 河怪
	  [3] = {nTemplate = 87, nLevel = 7, nSeries = -1},		   -- 大河怪
	  [4] = {nTemplate = 88, nLevel = -1, nSeries = -1},	   -- 剧情
	  [5] = {nTemplate = 89, nLevel = -1, nSeries = -1},	   -- 透明目标

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 10 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 10, 2, "guaiwu3", "heguai"},
			{Instance.TARGET_INFO, -1, Lang.mission.str434[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[3] = {nTime = 8, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.ADD_NPC, 4, 1, 3, "guaiwu1", "bianshen1"},
			{Instance.ADD_NPC, 5, 1, 3, "yinxing", "yinxing"},
			{Instance.NPC_BUBBLE, -1,"guaiwu1",Lang.mission.str435[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"guaiwu1",Lang.mission.str436[Lang.Idx], 3},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			 {Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.DEL_NPC,"yinxing"},
			},
		},
		[4] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 5, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 5, 5, "guaiwu2", "bianshen"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"guaiwu1"},
			{Instance.DEL_NPC,"guaiwu2"},
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
		   {Instance.ADD_NPC, 3, 1, 6, "guaiwu1", "daheguai"},
			{Instance.NPC_BUBBLE, -1,"guaiwu1",Lang.mission.str437[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str438[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[7] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[8]:Close()"},
			},
		},
		[8] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--城镇要道
InstanceSetting[106]=
{
	nTaskInstance = 1;
    nMinLevel = 8;
    nMaxLevel = 60;
	nMapId = 106;
    tbBeginPos = {611,1927, 1048376},
	tbLeavePos = {4,2404,4821,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
		[1] = {nTemplate = 95, nLevel = 8, nSeries = -1},		-- 强盗
		[2] = {nTemplate = 96, nLevel = -1, nSeries = -1},		-- 小白脸
		[3] = {nTemplate = 98, nLevel = -1, nSeries = -1},		-- 强盗（剧情）
		[4] = {nTemplate = 99, nLevel = -1, nSeries = -1},		-- 信号弹
		[5] = {nTemplate = 100, nLevel = -1, nSeries = -1},		-- 透明(爆炸)
		[6] = {nTemplate = 101, nLevel = 8, nSeries = -1},		-- 强盗
		[7] = {nTemplate = 102, nLevel = 8, nSeries = -1},		-- 强盗
		[8] = {nTemplate = 103, nLevel = 8, nSeries = -1},		-- 强盗
		[9] = {nTemplate = 104, nLevel = 8, nSeries = -1},		-- 强盗(带头)
		[10] = {nTemplate = 105, nLevel = -1, nSeries = -1},		-- 隐形目标
		[11] = {nTemplate = 106, nLevel = 1, nSeries = -1},		-- 炸弹
		[12] = {nTemplate = 97, nLevel = 8, nSeries = -1},		-- 怪盗小李

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    	{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.NOVICE_GUIDE, -1, 15},
			{Instance.ADD_NPC, 1, 3, 2, "guaiwu5", "qiangdao"},
			{Instance.ADD_NPC, 1, 1, 2, "guaiwu10", "qiangdao10"},
			{Instance.ADD_NPC, 12, 3, 2, "guaiwu1", "qiangdao1"},
			{Instance.NPC_BUBBLE, -1,"guaiwu10",Lang.mission.str439[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str440[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 8, nNum = 0 ,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.ADD_NPC, 3, 1, 3, "guaiwu1", "qiangdao5"},
			{Instance.ADD_NPC, 10, 1, 3, "yinxing", "yinxing"},
			{Instance.NPC_BUBBLE, -1,"guaiwu1",Lang.mission.str441[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[4] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
                {Instance.DEL_NPC,"yinxing"},
			},
		},
		[5] = {nTime = 6, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 5, "guaiwu2", "xinhaodan"},
			{Instance.NPC_BUBBLE, -1,"guaiwu1",Lang.mission.str442[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
			{Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
		    {Instance.DEL_NPC,"guaiwu1"},
			{Instance.DEL_NPC,"guaiwu2"},

			},
		},
		[6] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 2, 6, "guaiwu1", "qiangdao6"},
			{Instance.ADD_NPC, 7, 2, 6, "guaiwu3", "qiangdao7"},
			{Instance.ADD_NPC, 8, 2, 6, "guaiwu6", "qiangdao8"},
			{Instance.ADD_NPC, 9, 1, 6, "guaiwu3", "qiangdao9"},
			{Instance.TARGET_INFO, -1, Lang.mission.str440[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[7] = {nTime = 10, nNum = 0 ,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 6, 3, 7, "guaiwu1", "qiangdao6"},
			--{Instance.ADD_NPC, 7, 3, 7, "guaiwu3", "qiangdao7"},
			--{Instance.ADD_NPC, 8, 3, 7, "guaiwu6", "qiangdao8"},
			--{Instance.ADD_NPC, 9, 1, 7, "guaiwu3", "qiangdao9"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str440[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[8] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
			--{Instance.NPC_BUBBLE, -1,"guaiwu8","让开！！", 1},
			--{Instance.NPC_BUBBLE, -1,"guaiwu8","葵~花~神~功！", 3},
			},
			tbUnLockEvent =
			{

			},
		},
		[9] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 5, 1, 6, "guaiwu7", "juqing2"},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 5, 1, 6, "guaiwu7", "juqing3"},
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 5, 3, 6, "guaiwu7", "juqing4"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 5, 3, 6, "guaiwu7", "juqing4"},
			},
		},
		[12] = {nTime = 5, nNum = 0 ,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			--{Instance.NPC_BUBBLE, -1,"guaiwu8","遇到坏人要说不！知道吗？", 1},
			},
			tbUnLockEvent =
			{

			},
		},
		[13] = {nTime = 0, nNum = 4 ,		-- 后面加上去的步骤（实际是插入到前面的步骤的）
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.NOVICE_GUIDE, -1, 15},
			{Instance.ADD_NPC, 12, 4, 13, "guaiwu4", "qiangdao2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str440[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 5 ,		-- 后面加上去的步骤（实际是插入到前面的步骤的）
			tbPrelock = {13},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 4, 14, "guaiwu1", "qiangdao3"},
			{Instance.ADD_NPC, 12, 1, 14, "guaiwu4", "qiangdao4"},
			{Instance.TARGET_INFO, -1, Lang.mission.str440[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 1, nNum = 0 ,		-- 后面加上去的步骤（实际是插入到前面的步骤的）
			tbPrelock = {6},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 10, 1, 3, "yinxing", "yinxing"},
           -- {Instance.ADD_NPC, 1, 4, 15, "guaiwu5", "qiangdao11"},
			--{Instance.ADD_NPC, 12, 2, 15, "guaiwu4", "qiangdao111"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str440[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 2, 1, 2, "guaiwu8", "luren"},
			},
		},
		[16] = {nTime = 3, nNum = 0 ,		--
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[17] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[18]:Close()"},
			},
		},
		[18] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--城门口
InstanceSetting[107]=
{
	nTaskInstance = 1;
    nMinLevel = 9;
    nMaxLevel = 60;
	nMapId = 107;
    tbBeginPos = {2458,974, 1048576},
	tbLeavePos = {4,2404,4821,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
		[1] = {nTemplate = 113, nLevel = -1, nSeries = -1},		-- 大当家（剧情）
		[2] = {nTemplate = 114, nLevel = -1, nSeries = -1},		-- 二当家（剧情）
		[3] = {nTemplate = 115, nLevel = -1, nSeries = -1},		-- 三当家（剧情）
		[4] = {nTemplate = 116, nLevel = 8, nSeries = -1},		-- 强盗
		[5] = {nTemplate = 117, nLevel = 8, nSeries = -1},		-- 强盗
		[6] = {nTemplate = 118, nLevel = -1, nSeries = -1},		-- 小白脸（剧情?
		[7] = {nTemplate = 119, nLevel = 8, nSeries = -1},		-- 强盗
		[8] = {nTemplate = 120, nLevel = -1, nSeries = -1},		-- 自杀目标
		[10] = {nTemplate = 110, nLevel = 9, nSeries = -1},		-- 大当家（攻击）
		[11] = {nTemplate = 111, nLevel = 8, nSeries = -1},		-- 二当家（攻击）
		[12] = {nTemplate = 112, nLevel = 8, nSeries = -1},		-- 三当家（攻击）
		[13] = {nTemplate = 108, nLevel = -1, nSeries = -1},		-- 大罗守卫
		[14] = {nTemplate = 109, nLevel = -1, nSeries = -1},		-- 小罗守卫

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			 {Instance.ADD_NPC, 1, 1, 11, "guaiwu1", "dadangjia1"},    --提前加载
			  {Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
	    [2] = {nTime = 0, nNum = 6 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 3, 2, "kaitou1", "kaitou"},
			{Instance.ADD_NPC, 5, 3, 2, "kaitou2", "kaitou"},
			},
			tbUnLockEvent =
			{
             {Instance.DEL_NPC,"guaiwu1"},
			},
		},
		[3] = {nTime = 6, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[4] = {nTime = 11, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[5] = {nTime = 17, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[6] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 10, "qiangdao1", "qiangdao1"},
			{Instance.ADD_NPC, 5, 1, 10, "qiangdao2", "qiangdao2"},
			{Instance.ADD_NPC, 7, 1, 10, "qiangdao3", "qiangdao3"},
			{Instance.NPC_BUBBLE, -1,"qiangdao2",Lang.mission.str443[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{

			},
		},
		[7] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 10, "guaiwu1", "qiangdao1"},
			{Instance.ADD_NPC, 5, 1, 10, "guaiwu1", "qiangdao2"},
			{Instance.ADD_NPC, 7, 1, 10, "guaiwu1", "qiangdao3"},
			},
			tbUnLockEvent =
			{

			},
		},
		[8] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 10, "guaiwu1", "qiangdao1"},
			{Instance.ADD_NPC, 5, 1, 10, "guaiwu1", "qiangdao2"},
			{Instance.ADD_NPC, 7, 1, 10, "guaiwu1", "qiangdao3"},
			},
			tbUnLockEvent =
			{

			},
		},
		[9] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 10, "guaiwu1", "qiangdao1"},
			{Instance.ADD_NPC, 5, 1, 10, "guaiwu1", "qiangdao2"},
			{Instance.ADD_NPC, 7, 1, 10, "guaiwu1", "qiangdao3"},
			},
			tbUnLockEvent =
			{

			},
		},
		[10] = {nTime = 0, nNum = 14 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.TARGET_INFO, -1, Lang.mission.str440[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 11, nNum = 0  ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.ADD_NPC, 1, 1, 11, "guaiwu1", "dadangjia1"},
			{Instance.ADD_NPC, 2, 1, 11, "guaiwu2", "erdangjia1"},
			{Instance.ADD_NPC, 3, 1, 11, "guaiwu3", "sandangjia1"},
			{Instance.ADD_NPC, 8, 1, 11, "mubiao", "mubiao"},
			{Instance.NPC_BUBBLE, -1,"guaiwu1",Lang.mission.str444[Lang.Idx], 2},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[12] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[13] = {nTime = 8, nNum = 1 ,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.DEL_NPC,"daluo"},
			{Instance.DEL_NPC,"xiaoluo"},
			{Instance.ADD_NPC, 6, 1, 13, "guaiwu6", "xiaobailian"},
			{Instance.NPC_BUBBLE, -1,"guaiwu6",Lang.mission.str445[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"guaiwu1",Lang.mission.str449[Lang.Idx], 4},
			},
			tbUnLockEvent =
			{
            {Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			},
		},
		[14] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {13},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[15] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			{Instance.DEL_NPC,"guaiwu1"},
			{Instance.DEL_NPC,"guaiwu2"},
			{Instance.DEL_NPC,"guaiwu3"},
			{Instance.ADD_NPC, 10, 1, 15, "guaiwu4", "dadangjia1"},
			{Instance.ADD_NPC, 11, 1, 15, "guaiwu5", "erdangjia1"},
			{Instance.ADD_NPC, 12, 1, 15, "guaiwu6", "sandangjia1"},
			{Instance.NPC_BUBBLE, -1,"guaiwu4",Lang.mission.str446[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str447[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[16] = {nTime = 0, nNum = 2 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 13, 1, 16, "daluo", "daluo"},
			{Instance.ADD_NPC, 14, 1, 16, "xiaoluo", "xiaoluo"},
			{Instance.ADD_NPC, 4, 1, 10, "qiangdao11", "qiangdao11"},
			{Instance.ADD_NPC, 7, 1, 10, "qiangdao22", "qiangdao22"},
			{Instance.NPC_BUBBLE, -1,"daluo",Lang.mission.str448[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{

			},
		},
		[17] = {nTime = 4, nNum = 0 ,		-- 天兵
			tbPrelock = {15},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[18] = {nTime = 0, nNum = 1 ,		-- 天兵
			tbPrelock = {17},
			tbStartEvent =
			{
              {Instance.UNLOCK_GOD, -1, 18, 2},               --解锁第二大神珠
			},
			tbUnLockEvent =
			{
			},
		},
		[19] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {18},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[20]:Close()"},
			},
		},
		[20] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--年兽山洞
InstanceSetting[108]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 108;
    tbBeginPos = {991,1189,1048576},
	tbLeavePos = {4,2404,4821,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 125, nLevel = -1, nSeries = -1},		-- 年兽
		[2] = {nTemplate = 126, nLevel = 1, nSeries = -1},		-- 透明
		[3] = {nTemplate = 127, nLevel = -1, nSeries = -1},		-- 年兽分身前(中立)
		[4] = {nTemplate = 128, nLevel = -1, nSeries = -1},		-- 年兽分身
		[5] = {nTemplate = 129, nLevel = -1, nSeries = -1},		-- 年兽分身
		[6] = {nTemplate = 130, nLevel = -1, nSeries = -1},		-- 年兽分身
		[7] = {nTemplate = 131, nLevel = -1, nSeries = -1},		-- 年兽分身
		[8] = {nTemplate = 124, nLevel = 10, nSeries = -1},		-- 年兽开打
		[9] = {nTemplate = 123, nLevel = 10, nSeries = -1},		-- 年兽开打（主体）
		[10] = {nTemplate = 122, nLevel = -1, nSeries = -1},		-- 年兽（跑步）
		[11] = {nTemplate = 121, nLevel = 1, nSeries = -1},		-- 透明
		[12] = {nTemplate = 120, nLevel = 1, nSeries = -1},		-- 透明目标

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 2, "guaiwu1", "nianshou"},
			{Instance.ADD_NPC, 12, 1, 2, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 2, 5, 2, "guaiwu2", "touming"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"guaiwu2"},
			{Instance.DEL_NPC,"mubiao"},
			},
		},
	    [3] = {nTime = 7, nNum = 0,		-- 后面插进来一步
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.DEL_NPC,"guaiwu1"},
			{Instance.NOVICE_GUIDE, -1, 16},
			{Instance.ADD_NPC, 3, 1, 3, "guaiwu3", "nianshou8"},
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str450[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str451[Lang.Idx], 6},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 4, "guaiwu4", "nianshou2"},
			{Instance.ADD_NPC, 5, 1, 4, "guaiwu5", "nianshou3"},
			{Instance.ADD_NPC, 6, 1, 4, "guaiwu6", "nianshou4"},
			{Instance.ADD_NPC, 7, 1, 4, "guaiwu7", "nianshou5"},
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str452[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 4,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.DEL_NPC,"guaiwu4"},
			{Instance.DEL_NPC,"guaiwu5"},
			{Instance.DEL_NPC,"guaiwu6"},
			{Instance.DEL_NPC,"guaiwu7"},
			{Instance.ADD_NPC, 8, 4, 5, "guaiwu8", "nianshou6"},
			{Instance.TARGET_INFO, -1, Lang.mission.str453[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 2, nNum = 0,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 6, "guaiwu4", "nianshou2"},
			{Instance.ADD_NPC, 5, 1, 6, "guaiwu5", "nianshou3"},
			{Instance.ADD_NPC, 6, 1, 6, "guaiwu6", "nianshou4"},
			{Instance.ADD_NPC, 7, 1, 6, "guaiwu7", "nianshou5"},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 2, nNum = 0,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 7, "guaiwu44", "nianshou2"},
			{Instance.ADD_NPC, 5, 1, 7, "guaiwu55", "nianshou3"},
			{Instance.ADD_NPC, 6, 1, 7, "guaiwu66", "nianshou4"},
			{Instance.ADD_NPC, 7, 1, 7, "guaiwu77", "nianshou5"},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 4,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.DEL_NPC,"guaiwu4"},
			{Instance.DEL_NPC,"guaiwu5"},
			{Instance.DEL_NPC,"guaiwu6"},
			{Instance.DEL_NPC,"guaiwu7"},
			{Instance.ADD_NPC, 8, 4, 11, "guaiwu8", "nianshou6"},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 4,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.DEL_NPC,"guaiwu44"},
			{Instance.DEL_NPC,"guaiwu55"},
			{Instance.DEL_NPC,"guaiwu66"},
			{Instance.DEL_NPC,"guaiwu77"},
			{Instance.ADD_NPC, 8, 4, 11, "guaiwu9", "nianshou6"},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 0,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			--{Instance.DEL_NPC,"guaiwu4"},
			--{Instance.DEL_NPC,"guaiwu5"},
			--{Instance.DEL_NPC,"guaiwu6"},
			--{Instance.DEL_NPC,"guaiwu7"},
			--{Instance.ADD_NPC, 8, 4, 11, "guaiwu8", "nianshou6"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str453[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str454[Lang.Idx], 3},
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str455[Lang.Idx], 10},
			{Instance.TARGET_INFO, -1, Lang.mission.str453[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"guaiwu3"},
			},
		},
		[13] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 10, 1, 20, "nianshou1", "nianshou1"},
			{Instance.ADD_NPC, 11, 1, 13, "guaiwu2", "touming1"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 20, "nianshou10", "nianshou10"},
			},
		},
		[14] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 14, "guaiwu2", "touming2"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 20, "nianshou10", "nianshou11"},
			},
		},
		[15] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 15, "guaiwu2", "touming3"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 20, "nianshou10", "nianshou12"},
			},
		},
		[16] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 16, "guaiwu2", "touming4"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 20, "nianshou10", "nianshou13"},
			},
		},
		[17] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 17, "guaiwu2", "touming5"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 20, "nianshou10", "nianshou14"},
			},
		},
		[18] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 18, "guaiwu2", "touming6"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 20, "nianshou10", "nianshou15"},
			},
		},
		[19] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 19, "guaiwu2", "touming7"},
			{Instance.NPC_BUBBLE, -1,"nianshou1",Lang.mission.str456[Lang.Idx], 6},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 20, "nianshou10", "nianshou16"},
			},
		},
		[20] = {nTime = 0, nNum = 7,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str457[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str458[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str453[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[21] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "nianshou1"},
			{Instance.ADD_NPC, 10, 1, 28, "nianshou11", "nianshou1"},
			{Instance.ADD_NPC, 11, 1, 21, "guaiwu2", "touming1"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 28, "nianshou10", "nianshou10"},
			},
		},
		[22] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 22, "guaiwu2", "touming2"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 28, "nianshou10", "nianshou11"},
			},
		},
		[23] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 23, "guaiwu2", "touming3"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 28, "nianshou10", "nianshou12"},
			},
		},
		[24] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 24, "guaiwu2", "touming4"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 28, "nianshou10", "nianshou13"},
			},
		},
		[25] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 25, "guaiwu2", "touming5"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 28, "nianshou10", "nianshou14"},
			},
		},
		[26] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 26, "guaiwu2", "touming6"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 28, "nianshou10", "nianshou15"},
			},
		},
		[27] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 27, "guaiwu2", "touming7"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 8, 1, 28, "nianshou10", "nianshou16"},
			},
		},
		[28] = {nTime = 0, nNum = 7,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			{Instance.NPC_BUBBLE, -1,"nianshou11",Lang.mission.str459[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str453[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[29] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {28},
			tbStartEvent =
			{
			{Instance.DEL_NPC,"nianshou11"},
			{Instance.ADD_NPC, 9, 1, 29, "guaiwu3", "nianshou1"},
			{Instance.NPC_BUBBLE, -1,"guaiwu3",Lang.mission.str460[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str461[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[30] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {29},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[31]:Close()"},
			},
		},
		[31] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--副本山路
InstanceSetting[109]=
{
	nTaskInstance = 1;
    nMinLevel = 11;
    nMaxLevel = 60;
	nMapId = 109;
    tbBeginPos = {2223,672,1048576},
	tbLeavePos = {4,2404,4821,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
       [1] = {nTemplate = 200, nLevel = 11, nSeries = -1},		-- 熊精
	   [2] = {nTemplate = 201, nLevel = -1, nSeries = -1},		-- 熊精（剧情）
	   [3] = {nTemplate = 202, nLevel = -1, nSeries = -1},		-- 女孩
	   [4] = {nTemplate = 203, nLevel = -1, nSeries = -1},		-- 狂暴熊精（闪电剧情）
	   [5] = {nTemplate = 204, nLevel = 11, nSeries = -1},		-- 狂暴熊精
	   [6] = {nTemplate = 205, nLevel = -1, nSeries = -1},		-- 打雷（剧情?
	   [7] = {nTemplate = 206, nLevel = -1, nSeries = -1},		-- 熊精（剧情）?
	   [8] = {nTemplate = 207, nLevel = -1, nSeries = -1},		-- 透明目标
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 11,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 11, 2, "guaiwu1", "xiongyao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str462[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 11, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			--{Instance.ADD_NPC, 8, 1, 3, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 2, 1, 3, "guaiwu2", "xiongyao1"},
			{Instance.ADD_NPC, 2, 1, 3, "guaiwu3", "xiongyao2"},
			{Instance.ADD_NPC, 2, 1, 3, "guaiwu4", "xiongyao3"},
			{Instance.ADD_NPC, 3, 1, 3, "guaiwu5", "jingti"},
			{Instance.NPC_BUBBLE, -1,"guaiwu2",Lang.mission.str463[Lang.Idx], 1},
			--{Instance.NPC_BUBBLE, -1,"guaiwu3","对啊！好想摸一下啊！", 4},
			{Instance.NPC_BUBBLE, -1,"guaiwu4",Lang.mission.str464[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"mubiao"},
			 {Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			},
		},
		[4] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"guaiwu4"},
			{Instance.ADD_NPC, 7, 1, 4, "guaiwu9", "xiongyao3"},
			},
		},
		[5] = {nTime = 0, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 6, 3, 4, "guaiwu8", "dalei"},
			},
		},
		[6] = {nTime = 6, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"guaiwu2"},
			{Instance.DEL_NPC,"guaiwu3"},
			{Instance.DEL_NPC,"guaiwu4"},
			{Instance.DEL_NPC,"guaiwu9"},
			{Instance.ADD_NPC, 4, 1, 6, "guaiwu11", "xiongyao1"},
			{Instance.ADD_NPC, 4, 1, 6, "guaiwu10", "xiongyao2"},
			{Instance.ADD_NPC, 4, 1, 6, "guaiwu10", "xiongyao3"},
			{Instance.NPC_BUBBLE, -1,"guaiwu11",Lang.mission.str465[Lang.Idx], 1},
			},
		},
		[7] = {nTime = 0, nNum = 10,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			--{Instance.DEL_NPC,"guaiwu2"},
			--{Instance.DEL_NPC,"guaiwu3"},
			{Instance.DEL_NPC,"guaiwu10"},
			{Instance.DEL_NPC,"guaiwu11"},
			{Instance.DEL_NPC,"guaiwu5"},
			{Instance.DEL_NPC,"guaiwu8"},
			{Instance.ADD_NPC, 5, 9, 7, "guaiwu6", "xiongyao4"},
			{Instance.ADD_NPC, 5, 1, 7, "guaiwu7", "xiongyao5"},
			--{Instance.NPC_BUBBLE, -1,"guaiwu7","纳命来~！", 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str466[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[9]:Close()"},
			},
		},
		[9] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--田地
InstanceSetting[110]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 110;
    tbBeginPos = {1086,2908,1048576},
	tbLeavePos = {4,2404,4821,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
       [1] = {nTemplate = 211, nLevel = 12, nSeries = -1},		-- 蟾蜍
	   [2] = {nTemplate = 212, nLevel = -1, nSeries = -1},		-- 大毒虫（剧情1）
	   [3] = {nTemplate = 213, nLevel = -1, nSeries = -1},		-- 蟾蜍带毒（剧情2）
	   [4] = {nTemplate = 214, nLevel = -1, nSeries = -1},		-- 大毒虫（剧情2）
	   [5] = {nTemplate = 215, nLevel = -1, nSeries = -1},		-- 药草
	   [6] = {nTemplate = 216, nLevel = -1, nSeries = -1},		-- 大药草
	   [7] = {nTemplate = 217, nLevel = 12, nSeries = -1},		-- 毒虫
	   [8] = {nTemplate = 218, nLevel = 12, nSeries = -1},		-- 蜘蛛
	   [9] = {nTemplate = 209, nLevel = -1, nSeries = -1},		-- 毒虫（剧情1）
	   [10] = {nTemplate = 210, nLevel = -1, nSeries = -1},		-- 蜘蛛（剧情1）
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
	        {Instance.ADD_NPC, 3, 1, 3, "chanchu", "chanchu"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"chanchu"},
			},
		},
		[3] = {nTime = 0, nNum = 3,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 3, 3, "yaocao1", "yaocao1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str467[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 5, 4, "feichong1", "feichong1"},
			{Instance.ADD_NPC, 8, 3, 4, "zhizhu", "zhizhu"},
			{Instance.TARGET_INFO, -1, Lang.mission.str468[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 3,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 3, 5, "yaocao2", "yaocao2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str467[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 2, 1, 6, "feichong3", "feichong3"},
			--{Instance.ADD_NPC, 9, 3, 6, "feichong4", "feichong4"},
			--{Instance.ADD_NPC, 10, 2, 6, "zhizhu2", "zhizhu2"},
			--{Instance.NPC_BUBBLE, -1,"feichong3","今天一定要捉一个人给我扎！", 1},
			--{Instance.NPC_BUBBLE, -1,"zhizhu2","是！大王！", 4},
			--{Instance.NPC_BUBBLE, -1,"feichong4","是！大王！", 4},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 5, 7, "feichong2", "feichong2"},
			{Instance.ADD_NPC, 8, 3, 7, "zhizhu1", "zhizhu1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str468[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 8, "yaocao", "yaocao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str467[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.ADD_NPC, 4, 1, 9, "feichong", "feichong"},
			{Instance.NPC_BUBBLE, -1,"feichong",Lang.mission.str469[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"feichong",Lang.mission.str470[Lang.Idx], 3},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 9, "chanchu", "chanchu"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"chanchu"},
			 {Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			},
		},
		[11] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC,"feichong"},
			{Instance.NPC_BUBBLE, -1,"chanchu",Lang.mission.str471[Lang.Idx], 1},
			},
		},
		[12] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 12, "chanchu1", "chanchu"},
			{Instance.NPC_BUBBLE, -1,"chanchu1",Lang.mission.str472[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str473[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			--{Instance.DEL_NPC,"feichong4"},
			--{Instance.DEL_NPC,"zhizhu2"},
			--{Instance.DEL_NPC,"feichong3"},
			--{Instance.ADD_NPC, 7, 3, 13, "feichong5", "feichong4"},
			--{Instance.ADD_NPC, 8, 2, 13, "zhizhu3", "zhizhu2"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str468[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 4,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 8, 4, 14, "zhizhu3", "zhizhu3"},
			{Instance.TARGET_INFO, -1, Lang.mission.str468[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[16]:Close()"},
			},
		},
		[16] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--吊桥
InstanceSetting[111]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 111;
    tbBeginPos = {6376,1882,1048576},
	tbLeavePos = {4,2404,4821,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
     [1] = {nTemplate = 221, nLevel = -1, nSeries = -1},		-- 巨石
	 [2] = {nTemplate = 222, nLevel = -1, nSeries = -1},		-- 巨石
	 [3] = {nTemplate = 223, nLevel = -1, nSeries = -1},		-- 狂暴熊精（剧情）
	 [4] = {nTemplate = 224, nLevel = 14, nSeries = -1},		-- 狂暴熊精
	 [5] = {nTemplate = 225, nLevel = -1, nSeries = -1},		-- 西域飞龙（剧情）
	 [6] = {nTemplate = 226, nLevel = 14, nSeries = -1},		-- 西域飞龙
	 [7] = {nTemplate = 227, nLevel = -1, nSeries = -1},		-- 狂暴熊精（剧情）
	 [8] = {nTemplate = 228, nLevel = -1, nSeries = -1},		-- 狂暴熊精（剧情）
	 [9] = {nTemplate = 229, nLevel = -1, nSeries = -1},		-- 狂暴熊精（剧情）
	 [10] = {nTemplate = 230, nLevel = 1, nSeries = -1},		-- 透明（自杀）
	 [11] = {nTemplate = 231, nLevel = -1, nSeries = -1},		-- 狂暴熊精（剧情）
	 [12] = {nTemplate = 220, nLevel = -1, nSeries = -1},		-- 巨石
	 [13] = {nTemplate = 232, nLevel = 1, nSeries = -1},		-- 透明目标（自杀?

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 2, "xiong1", "xiong1"},
			{Instance.ADD_NPC, 12, 2, 2, "jushi12", "jushi12" },
			--{Instance.NPC_BUBBLE, -1,"xiong1","我会告诉你桥对面有好东西吗~？", 1},
			{Instance.NPC_BUBBLE, -1,"xiong1",Lang.mission.str474[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 6, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "xiong1" },
			{Instance.ADD_NPC, 1, 1, 3, "jushi3", "jushi3" },
			{Instance.ADD_NPC, 2, 1, 3, "jushi4", "jushi4" },
			{Instance.ADD_NPC, 13, 1, 2, "mubiao", "mubiao" },
			},
			tbUnLockEvent =
			{
            {Instance.DEL_NPC, "jushi3"},
			{Instance.DEL_NPC, "jushi4"},
			},
		},
		[4] = {nTime = 0, nNum = 3,		-- 总计时
			tbPrelock = {17},
			tbStartEvent =
			{

			{Instance.ADD_NPC, 4, 2, 4, "xiong", "xiong"},
			{Instance.ADD_NPC, 4, 1, 4, "xiong11", "xiong11"},
			{Instance.NPC_BUBBLE, -1,"xiong11",Lang.mission.str475[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str476[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 10, nNum = 0,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 3, 5, "jushi5", "jushi5"},
			{Instance.ADD_NPC, 13, 1, 5, "mubiao1", "mubiao1" },
			{Instance.TARGET_INFO, -1, Lang.mission.str477[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "jushi5"},
			},
		},
		[6] = {nTime = 12, nNum = 0,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 3, 6, "jushi6", "jushi6"},
			{Instance.ADD_NPC, 13, 1, 6, "mubiao2", "mubiao2" },
			{Instance.TARGET_INFO, -1, Lang.mission.str477[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "jushi6"},
			},
		},
		[7] = {nTime = 0, nNum = 5,		-- 总计时
			tbPrelock = {16},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 4, 7, "xiong2", "xiong2"},
			{Instance.ADD_NPC, 4, 1, 7, "xiong22", "xiong22"},
			{Instance.NPC_BUBBLE, -1,"xiong22",Lang.mission.str478[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str476[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "jushi5"},
			},
		},
		[8] = {nTime = 0, nNum = 0,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 0,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 9, 1, 9, "xiong3", "xiong3"},
			--{Instance.ADD_NPC, 2, 1, 9, "jushi8", "jushi8"},
			--{Instance.ADD_NPC, 1, 1, 9, "jushi9", "jushi9"},
			--{Instance.NPC_BUBBLE, -1,"xiong3","快跟上！快要拿到好东西了！", 2},
			--{Instance.NPC_BUBBLE, -1,"xiong3","好东西？哈哈~骗你的！", 8},
			--{Instance.TARGET_INFO, -1, "躲开巨石"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "xiong3"},
			},
		},
		[10] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 2, 1, 10, "jushi10", "jushi10"},
			--{Instance.ADD_NPC, 1, 1, 10, "jushi11", "jushi11"},
			},
		},
		[11] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 4, 11, "xiong3", "xiong4"},
			{Instance.ADD_NPC, 8, 3, 11, "xiong3", "xiong5"},
			{Instance.ADD_NPC, 8, 1, 11, "xiong7", "xiong7"},
			{Instance.NPC_BUBBLE, -1,"xiong7",Lang.mission.str479[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str476[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 0, nNum = 0,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 7, 4, 12, "xiong3", "xiong4"},
			--{Instance.ADD_NPC, 8, 3, 12, "xiong3", "xiong5"},
			--{Instance.ADD_NPC, 8, 1, 12, "xiong7", "xiong7"},
			--{Instance.NPC_BUBBLE, -1,"xiong7","让他看一下桥底的风景~！", 1},
			--{Instance.TARGET_INFO, -1, Lang.mission.str476[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 13, nNum = 0,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.ADD_NPC, 5, 1, 13, "feilong", "feilong"},
			{Instance.NPC_BUBBLE, -1,"feilong",Lang.mission.str480[Lang.Idx], 6},
			{Instance.NPC_BUBBLE, -1,"feilong",Lang.mission.str481[Lang.Idx], 9},
			},
			tbUnLockEvent =
			{
			 {Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			},
		},
		[14] = {nTime = 3, nNum = 1,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 10, 2, 14, "touming1", "touming1" },
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming1"},
			},
		},
		[15] = {nTime = 0, nNum = 2,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 15, "xiong6", "xiong6"},
			{Instance.ADD_NPC, 4, 1, 15, "xiong66", "xiong66"},
			{Instance.NPC_BUBBLE, -1,"xiong66",Lang.mission.str482[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str476[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[16] = {nTime = 5, nNum = 1,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 10, 2, 16, "touming2", "touming2" },
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming2"},
			},
		},
		[17] = {nTime = 6, nNum = 1,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 10, 2, 17, "touming", "touming" },
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming"},
			},
		},
		[18] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {13},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "feilong"},
			{Instance.ADD_NPC, 6, 1, 18, "feilong1", "feilong1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str483[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[19] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 19, "xiong8", "xiong8"},
			{Instance.ADD_NPC, 13, 1, 19, "mubiao3", "mubiao3" },
			{Instance.NPC_BUBBLE, -1,"xiong8",Lang.mission.str484[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str476[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "xiong8"},
			},
		},
		[20] = {nTime = 3, nNum = 0 ,		-- 天兵
			tbPrelock = {18},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[21] = {nTime = 0, nNum = 1 ,		-- 天兵
			tbPrelock = {20},
			tbStartEvent =
			{
              {Instance.UNLOCK_GOD, -1, 21, 3},               --解锁第三大神珠
			},
			tbUnLockEvent =
			{
			},
		},
		[22] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {21},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[23]:Close()"},
			},
		},
		[23] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}
--牛魔王洞穴
InstanceSetting[112]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 112;
    tbBeginPos = {4260,4443,1048576},
	tbLeavePos = {4,6960,779,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 235, nLevel = 14, nSeries = -1},		-- 牛兵守卫
	    [2] = {nTemplate = 236, nLevel = -1, nSeries = -1},		-- 牛兵守卫（剧情列队）
		[3] = {nTemplate = 237, nLevel = -1, nSeries = -1},		-- 牛兵队长（剧情带头）
	    [4] = {nTemplate = 238, nLevel = 14, nSeries = -1},		-- 牛兵守卫
		[5] = {nTemplate = 239, nLevel = 14, nSeries = -1},		-- 牛兵守卫
		[6] = {nTemplate = 240, nLevel = 1, nSeries = -1},		-- 透明自杀
		[7] = {nTemplate = 241, nLevel = -1, nSeries = -1},		-- 牛魔王(剧情)
		[8] = {nTemplate = 242, nLevel = 15, nSeries = -1},		-- 牛魔王
		[9] = {nTemplate = 234, nLevel = -1, nSeries = -1},		-- 牛兵守卫(剧情1)
		[10] = {nTemplate = 233, nLevel = 14, nSeries = -1},    -- 牛兵团长
		[11] = {nTemplate = 243, nLevel = -1, nSeries = -1},	-- 牛魔王(剧情3)
		[12] = {nTemplate = 244, nLevel = 1, nSeries = -1},		-- 透明自杀
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 4, 2, "shouwei", "shouwei"},
			{Instance.ADD_NPC, 1, 4, 2, "shouwei1", "shouwei1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str485[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 4, "shouwei3", "shouwei3"},
			--{Instance.NPC_BUBBLE, -1, "shouwei3", "都吵吵个鸟，是我牛逼的！",1},
			},
		},
		[4] = {nTime = 8, nNum = 0,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{

		--	{Instance.ADD_NPC, 2, 1, 4, "shouwei10", "shouwei7"},
			--{Instance.ADD_NPC, 2, 1, 4, "shouwei11", "shouwei8"},
			--{Instance.NPC_BUBBLE, -1, "shouwei10", "都是你妈逼的！",1},
			--{Instance.NPC_BUBBLE, -1, "shouwei11", "是你妈逼的！",3},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 6,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "shouwei2"},
			{Instance.DEL_NPC, "shouwei10"},
			{Instance.DEL_NPC, "shouwei11"},
			{Instance.DEL_NPC, "shouwei3"},
			{Instance.ADD_NPC, 5, 5, 5, "shouwei4", "shouwei2"},
			{Instance.ADD_NPC, 4, 1, 5, "shouwei5", "shouwei5"},
			{Instance.TARGET_INFO, -1, Lang.mission.str486[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 10, nNum = 1,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 2, 6, "touming", "touming"},
			{Instance.ADD_NPC, 12, 1, 6, "touming2", "touming2"},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 12, nNum = 0,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.DEL_NPC, "shouwei88"},
			{Instance.ADD_NPC, 7, 1, 7, "niu", "niu"},
			{Instance.ADD_NPC, 9, 1, 7, "shouwei77", "shouwei77"},
			{Instance.ADD_NPC, 9, 1, 7, "shouwei88", "shouwei88"},
			{Instance.ADD_NPC, 9, 1, 7, "shouwei99", "shouwei99"},
			{Instance.ADD_NPC, 9, 1, 7, "shouwei66", "shouwei66"},
			{Instance.NPC_BUBBLE, -1,"shouwei66", Lang.mission.str487[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"niu", Lang.mission.str488[Lang.Idx], 4},
			{Instance.NPC_BUBBLE, -1,"shouwei66", Lang.mission.str489[Lang.Idx], 9},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			 {Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			},
		},
		[8] = {nTime = 0, nNum = 4,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "shouwei66"},
			{Instance.DEL_NPC, "shouwei77"},
			{Instance.DEL_NPC, "shouwei88"},
			{Instance.DEL_NPC, "shouwei99"},
			{Instance.DEL_NPC, "niu"},
			{Instance.ADD_NPC, 10, 1, 8, "shouwei6", "shouwei66"},
			{Instance.ADD_NPC, 10, 1, 8, "shouwei7", "shouwei77"},
			{Instance.ADD_NPC, 10, 1, 8, "shouwei8", "shouwei88"},
			{Instance.ADD_NPC, 10, 1, 8, "shouwei9", "shouwei99"},
			{Instance.NPC_BUBBLE, -1,"shouwei6", Lang.mission.str490[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str491[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 2, 9, "touming1", "touming1"},
			{Instance.ADD_NPC, 12, 1, 9, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 11, 1, 10, "niu2", "niu1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str491[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "mubiao"},
			--{Instance.ADD_NPC, 11, 1, 10, "niu2", "niu1"},
			{Instance.NPC_BUBBLE, -1,"niu2", Lang.mission.str492[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str491[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "niu2"},
			{Instance.ADD_NPC, 8, 1, 11, "niu1", "niu1"},
			{Instance.NPC_BUBBLE, -1,"niu1", Lang.mission.str493[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str494[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[13]:Close()"},
			},
		},
		[13] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}
--决斗山崖
InstanceSetting[113]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 113;
    tbBeginPos = {3276,190,1048576},
	tbLeavePos = {4,6960,779,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
       [1] = {nTemplate = 250, nLevel = -1, nSeries = -1},		-- 江湖人士（剧情）
	   [2] = {nTemplate = 251, nLevel = 15, nSeries = -1},		-- 江湖人士
	   [3] = {nTemplate = 252, nLevel = 1, nSeries = -1},		-- 透明目标
	   [4] = {nTemplate = 253, nLevel = 1, nSeries = -1},		-- 透明自杀
	   [5] = {nTemplate = 254, nLevel = -1, nSeries = -1},		-- 剑
	   [6] = {nTemplate = 255, nLevel = -1, nSeries = -1},		-- 透明击退剑
	   [7] = {nTemplate = 256, nLevel = -1, nSeries = -1},		-- 透明击侠客
	   [8] = {nTemplate = 257, nLevel = -1, nSeries = -1},		-- 空虚公子（剧情）
	   [9] = {nTemplate = 258, nLevel = 16, nSeries = -1},		-- 空虚公子
	   [10] = {nTemplate = 259, nLevel = -1, nSeries = -1},		-- 老太婆
	   [11] = {nTemplate = 260, nLevel = -1, nSeries = -1},		-- 老太婆
	   [12] = {nTemplate = 261, nLevel = -1, nSeries = -1},		-- 天残脚（剧情）
	   [13] = {nTemplate = 262, nLevel = 16, nSeries = -1},		-- 天残脚
	   [14] = {nTemplate = 263, nLevel = -1, nSeries = -1},		-- 空虚公子（剧情4）
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 16,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 16, 2, "xiake","xiake"},
			{Instance.TARGET_INFO, -1, Lang.mission.str495[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		--[3] = {nTime = 5, nNum = 0,		-- 总计时
			--tbPrelock = {2},
			--tbStartEvent =
			--{
			--{Instance.ADD_NPC, 1, 1, 3, "xiake1","xiake1"},
			--{Instance.ADD_NPC, 1, 1, 3, "xiake11","xiake11"},
			--{Instance.NPC_BUBBLE, -1,"xiake1", "来啦！来啦！", 1},
			--{Instance.NPC_BUBBLE, -1,"xiake1", "哇！", 3},
			--{Instance.NPC_BUBBLE, -1,"xiake11", "牛！", 3},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			--},
			--tbUnLockEvent =
			--{
			--},
		--},
		--[4] = {nTime = 2, nNum = 0,		-- 总计时
		--	tbPrelock = {2},
		--	tbStartEvent =
		--	{
		--	{Instance.ADD_NPC, 6, 1, 4, "touming2","touming2"},
		---	},
		--	tbUnLockEvent =
		--	{
		--	},
		--},
		--[5] = {nTime = 5, nNum = 0,		-- 总计时
		--	tbPrelock = {4},
		--	tbStartEvent =
		--	{
		--	{Instance.ADD_NPC, 5, 1, 5, "jian","jian"},
		--	{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
		--	},
		--	tbUnLockEvent =
		--	{
		--	{Instance.DEL_NPC, "jian"},
		--	{Instance.DEL_NPC, "touming2"},
		--	},
		--},
		--[6] = {nTime = 0, nNum = 6,		-- 总计时
		--	tbPrelock = {5},
		--	tbStartEvent =
		--	{
		--	{Instance.DEL_NPC, "xiake1"},
		--	{Instance.DEL_NPC, "xiake11"},
		--	{Instance.ADD_NPC, 2, 6, 6, "xiake2","xiake2"},
		--	{Instance.TARGET_INFO, -1, "打败江湖人士"},
		--	},
		--	tbUnLockEvent =
		--	{
		--	},
		--},
		[3] = {nTime = 3, nNum = 1,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 3, "touming","touming"},
			{Instance.ADD_NPC, 4, 4, 3, "touming1","touming1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming"},
			{Instance.DEL_NPC, "touming1"},
			},
		},
		[4] = {nTime = 10, nNum = 0,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.ADD_NPC, 1, 3, 4, "xiake3","xiake3"},
			{Instance.ADD_NPC, 1, 1, 4, "xiake33","xiake33"},
			{Instance.ADD_NPC, 1, 1, 4, "xiake333","xiake333"},
			{Instance.ADD_NPC, 1, 1, 4, "xiake3333","xiake3333"},
			{Instance.NPC_BUBBLE, -1,"xiake33", Lang.mission.str496[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"xiake333", Lang.mission.str497[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 7, nNum = 0,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 6, "touming3","touming3"},
			{Instance.NPC_BUBBLE, -1,"xiake3333", Lang.mission.str498[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming3"},
			{Instance.DEL_NPC, "xiake3333"},
			{Instance.DEL_NPC, "xiake3"},
			{Instance.DEL_NPC, "xiake33"},
			{Instance.DEL_NPC, "xiake333"},
			},
		},
		[7] = {nTime = 10, nNum = 1,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 7, "touming4","touming4"},
			{Instance.ADD_NPC, 4, 4, 7, "touming5","touming5"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming4"},
			{Instance.DEL_NPC, "touming5"},
			},
		},
		[8] = {nTime = 12, nNum = 0,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 8, 1, 8, "kongxu","kongxu"},
			{Instance.ADD_NPC, 10, 4, 8, "laopopo","laopopo"},
			{Instance.ADD_NPC, 12, 1, 8, "tiancan","tiancan"},
			{Instance.NPC_BUBBLE, -1,"tiancan", Lang.mission.str499[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"tiancan", Lang.mission.str500[Lang.Idx], 6},
			{Instance.NPC_BUBBLE, -1,"kongxu", Lang.mission.str501[Lang.Idx], 9},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "jian"},
			},
		},
		[9] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 5, 5, 9, "jian", "jian"},
			},
		},
		[10] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "kongxu"},
			{Instance.DEL_NPC, "tiancan"},
			{Instance.DEL_NPC, "laopopo"},
			--{Instance.ADD_NPC, 12, 1, 10, "tiancan1","tiancan1"},
			--{Instance.NPC_BUBBLE, -1,"tiancan1", "就喜欢把高手踩到脚下！哈哈哈~", 1},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			 {Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.DEL_NPC, "tiancan1"},
			{Instance.ADD_NPC, 13, 1, 11, "tiancan2","tiancan1"},
			{Instance.NPC_BUBBLE, -1,"tiancan2", Lang.mission.str502[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str503[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 10, nNum = 1,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 12, "touming4","touming4"},
			{Instance.ADD_NPC, 4, 4, 12, "touming5","touming5"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming4"},
			{Instance.DEL_NPC, "touming5"},
			},
		},
		[13] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 14, 1, 13, "kongxu1","kongxu1"},
			{Instance.ADD_NPC, 10, 3, 8, "laopopo1","laopopo1"},
			{Instance.ADD_NPC, 10, 1, 8, "laopopo11","laopopo11"},
			{Instance.NPC_BUBBLE, -1,"kongxu1", Lang.mission.str504[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "laopopo1"},
			{Instance.DEL_NPC, "laopopo11"},
			},
		},
		[14] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {13},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 14, "laopopo22","laopopo22"},
			{Instance.ADD_NPC, 11, 3, 14, "laopopo2","laopopo2"},
			{Instance.NPC_BUBBLE, -1,"laopopo22", Lang.mission.str505[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"kongxu1", "。。。。。", 3},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "laopopo2"},
			{Instance.DEL_NPC, "laopopo22"},
			},
		},
		[15] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			--{Instance.DEL_NPC, "laopopo1"},
			--{Instance.DEL_NPC, "laopopo11"},
			{Instance.DEL_NPC, "kongxu1"},
			{Instance.ADD_NPC, 9, 1, 15, "kongxu2","kongxu2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str506[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[16] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[17]:Close()"},
			},
		},
		[17] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--火焰山
InstanceSetting[114]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 114;
    tbBeginPos = {2316,1128,1048576},
	tbLeavePos = {4,6960,779,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
       [1] = {nTemplate = 270, nLevel = -1, nSeries = -1},		-- 战斗种子
	   [2] = {nTemplate = 271, nLevel = -1, nSeries = -1},		-- 战斗树人
	   [3] = {nTemplate = 272, nLevel = 16, nSeries = -1},		-- 熔岩怪1
	   [4] = {nTemplate = 273, nLevel = 16, nSeries = -1},		-- 熔岩怪2
	   [5] = {nTemplate = 274, nLevel = 16, nSeries = -1},		-- 熔岩怪3
	   [6] = {nTemplate = 275, nLevel = -1, nSeries = -1},		-- 冰冻树人
	   [7] = {nTemplate = 276, nLevel = 17, nSeries = -1},		-- 超级熔岩怪1
	   [8] = {nTemplate = 277, nLevel = 17, nSeries = -1},		-- 超级熔岩怪2
	   [9] = {nTemplate = 278, nLevel = 17, nSeries = -1},		-- 超级熔岩怪3
	   [10] = {nTemplate = 279, nLevel = 1, nSeries = -1},		-- 透明自杀
	   [11] = {nTemplate = 280, nLevel = -1, nSeries = -1},		-- 红孩儿（剧情）
	   [12] = {nTemplate = 281, nLevel = 17, nSeries = -1},		-- 红孩儿
	   [13] = {nTemplate = 282, nLevel = -1, nSeries = -1},		-- 透明目标
	   [14] = {nTemplate = 283, nLevel = 17, nSeries = -1},		-- 超级熔岩怪（和红孩儿一齐出现）
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 14, 1, 2, "yunshi","yunshi", 10 , 20, "yunshi_timer"},
			{Instance.ADD_NPC, 1, 1, 2, "zhongzi1","zhongzi1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str510[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 2, 1, 2, "shuren1","shuren1"},
			},
		},
		[3] = {nTime = 0, nNum = 4,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 3, "rongyan11","rongyan11"},
			{Instance.ADD_NPC, 3, 3, 3, "rongyan1","rongyan1"},
			{Instance.NPC_BUBBLE, -1,"rongyan11", Lang.mission.str508[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str509[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 4, "zhongzi2","zhongzi2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str510[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 2, 1, 4, "shuren2","shuren2"},
			},
		},
		[5] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 5, "zhongzi3","zhongzi3"},
			{Instance.TARGET_INFO, -1, Lang.mission.str510[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 2, 1, 5, "shuren3","shuren3"},
			},
		},
		[6] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 3, 8, "rongyan2","rongyan2"},
			{Instance.ADD_NPC, 4, 1, 8, "rongyan22","rongyan22"},
			{Instance.ADD_NPC, 5, 4, 8, "rongyan3","rongyan3"},
			{Instance.NPC_BUBBLE, -1,"rongyan22", Lang.mission.str511[Lang.Idx], 1},
			 {Instance.DO_SCRIPT, "self.tbLock[7]:Close()"},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 3, 8, "rongyan2","rongyan2"},
			{Instance.ADD_NPC, 4, 1, 8, "rongyan22","rongyan22"},
			{Instance.ADD_NPC, 5, 4, 8, "rongyan3","rongyan3"},
			{Instance.NPC_BUBBLE, -1,"rongyan22", Lang.mission.str512[Lang.Idx], 1},
			 {Instance.DO_SCRIPT, "self.tbLock[6]:Close()"},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.TARGET_INFO, -1, Lang.mission.str509[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 12,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 3, 9, "rongyan1","rongyan1"},
			{Instance.ADD_NPC, 3, 1, 9, "rongyan11","rongyan11"},
			{Instance.ADD_NPC, 4, 3, 9, "rongyan2","rongyan2"},
			{Instance.ADD_NPC, 4, 1, 9, "rongyan22","rongyan22"},
			{Instance.ADD_NPC, 5, 4, 9, "rongyan3","rongyan3"},
			{Instance.NPC_BUBBLE, -1,"rongyan22", Lang.mission.str513[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str509[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "shuren1"},
			{Instance.DEL_NPC, "shuren2"},
			{Instance.DEL_NPC, "shuren3"},
			},
		},
		[10] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 10, "zhongzi4","zhongzi4"},
			{Instance.TARGET_INFO, -1, Lang.mission.str510[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 6, 1, 10, "shuren4","shuren4"},
			{Instance.ADD_NPC, 10, 1, 13, "touming1","touming1"},
			},
		},
		[11] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 11, "zhongzi5","zhongzi5"},
			{Instance.TARGET_INFO, -1, Lang.mission.str510[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 2, 1, 11, "shuren5","shuren5"},
			{Instance.ADD_NPC, 10, 1, 13, "touming2","touming2"},
			},
		},
		[12] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 12, "zhongzi6","zhongzi6"},
			{Instance.TARGET_INFO, -1, Lang.mission.str510[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 2, 1, 12, "shuren6","shuren6"},
			{Instance.ADD_NPC, 10, 1, 13, "touming3","touming3"},
			},
		},
		[13] = {nTime = 15, nNum = 3,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.TARGET_INFO, -1, Lang.mission.str514[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 12,		-- 总计时
			tbPrelock = {13},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 3, 3, 14, "chaoji1","chaoji1"},
			{Instance.ADD_NPC, 7, 1, 14, "chaoji11","chaoji11"},
			{Instance.ADD_NPC, 4, 3, 14, "chaoji2","chaoji2"},
			{Instance.ADD_NPC, 8, 1, 14, "chaoji22","chaoji22"},
			{Instance.ADD_NPC, 5, 3, 14, "chaoji3","chaoji3"},
			{Instance.ADD_NPC, 9, 1, 14, "chaoji33","chaoji33"},
			{Instance.TARGET_INFO, -1, Lang.mission.str518[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "shuren4"},
			{Instance.DEL_NPC, "shuren5"},
			{Instance.DEL_NPC, "shuren6"},
			{Instance.DEL_NPC, "zhongzi4"},
			{Instance.DEL_NPC, "zhongzi5"},
			{Instance.DEL_NPC, "zhongzi6"},
			},
		},
		[15] = {nTime = 10, nNum = 1,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 10, 4, 15, "touming","touming"},
			{Instance.ADD_NPC, 13, 1, 15, "touming4","touming4"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming"},
			{Instance.DEL_NPC, "touming4"},
			},
		},
		[16] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 11, 1, 16, "hong","hong"},
			{Instance.NPC_BUBBLE, -1,"hong", Lang.mission.str515[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[17] = {nTime = 0, nNum = 8,		-- 总计时
			tbPrelock = {16},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "hong"},
            {Instance.ADD_NPC, 12, 1, 17, "hong1","hong"},
			{Instance.ADD_NPC, 14, 7, 17, "chaoji5","chaoji5"},
			{Instance.NPC_BUBBLE, -1,"hong1", Lang.mission.str516[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str517[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[18] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {17},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[19]:Close()"},
			},
		},
		[19] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}
--大闹天宫
InstanceSetting[115]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 115;
    tbBeginPos = {2019,1691,1048576},
	tbLeavePos = {4,6960,779,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
       [1] = {nTemplate = 290, nLevel = -1, nSeries = -1},		-- 天兵
	   [2] = {nTemplate = 291, nLevel = 19, nSeries = -1},		-- 天兵
	   [3] = {nTemplate = 292, nLevel = 19, nSeries = -1},		-- 孙悟空
	   [4] = {nTemplate = 293, nLevel = -1, nSeries = -1},		-- 孙悟空
	   [5] = {nTemplate = 294, nLevel = -1, nSeries = -1},		-- 紫霞仙子
	   [6] = {nTemplate = 295, nLevel = -1, nSeries = -1},		-- 紫霞仙子
	   [7] = {nTemplate = 296, nLevel = 19, nSeries = -1},		-- 巨灵神
	   [8] = {nTemplate = 297, nLevel = -1, nSeries = -1},		-- 巨灵神
	   [9] = {nTemplate = 298, nLevel = 19, nSeries = -1},		-- 哪咤
	   [10] = {nTemplate = 299, nLevel = -1, nSeries = -1},		-- 哪咤
	   [11] = {nTemplate = 300, nLevel = 19, nSeries = -1},		-- 二郎神
	   [12] = {nTemplate = 301, nLevel = -1, nSeries = -1},		-- 二郎神
	   [13] = {nTemplate = 302, nLevel = -1, nSeries = -1},		-- 哮天犬（停用）
	   [14] = {nTemplate = 303, nLevel = -1, nSeries = -1},		-- 哮天犬（停用）
	   [15] = {nTemplate = 304, nLevel = 1, nSeries = -1},		-- 透明自杀
	   [16] = {nTemplate = 305, nLevel = 1, nSeries = -1},		-- 透明目标
	   [17] = {nTemplate = 306, nLevel = -1, nSeries = -1},		-- 火焰陷阱
	   [18] = {nTemplate = 307, nLevel = 1, nSeries = -1},		-- 透明被杀
	   [19] = {nTemplate = 308, nLevel = -1, nSeries = -1},		-- 哮天犬
	   [20] = {nTemplate = 309, nLevel = -1, nSeries = -1},		-- 小哮天犬
	   [21] = {nTemplate = 310, nLevel = -1, nSeries = -1},		-- 火球1
	   [22] = {nTemplate = 311, nLevel = -1, nSeries = -1},		-- 火球2
	   [23] = {nTemplate = 312, nLevel = -1, nSeries = -1},		-- 火柱

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 8, 1, 2, "juling", "juling"},
				{Instance.NPC_BUBBLE, -1, "juling", Lang.mission.str519[Lang.Idx],1},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "juling"},
			},
		},
		[3] = {nTime = 0, nNum = 15,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 7, 1, 3, "juling1", "juling1"},
				{Instance.ADD_NPC, 2, 7, 3, "tianbing", "tianbing", 2, 9 ,"tianbing_timer",0},
				{Instance.TARGET_INFO, -1, Lang.mission.str520[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 22,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 9, 1, 4, "nazha", "nazha"},
				{Instance.ADD_NPC, 2, 7, 4, "tianbing", "tianbing", 3, 9 ,"tianbing_timer",0},
			    {Instance.NPC_BUBBLE, -1, "nazha", Lang.mission.str521[Lang.Idx],2},
				{Instance.TARGET_INFO, -1, Lang.mission.str522[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 12, 1, 5, "erlang1", "erlang"},
			    {Instance.NPC_BUBBLE, -1, "erlang1", Lang.mission.str523[Lang.Idx],1},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "erlang1"},
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		-- 天兵
			tbPrelock = {5},
			tbStartEvent =
			{
              {Instance.UNLOCK_GOD, -1, 6, 4},               --解锁第四大神珠
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 4, nNum = 0,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 11, 1, 8, "erlang", "erlang"},
			    {Instance.NPC_BUBBLE, -1, "erlang", Lang.mission.str524[Lang.Idx],1},
				{Instance.TARGET_INFO, -1, Lang.mission.str525[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
				--{Instance.ADD_NPC, 2, 7, 7, "tianbing", "tianbing", 5, 6 ,"tianbing_timer",0},
				{Instance.ADD_NPC, 1, 12, 9, "tianbing1", "tianbing1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str526[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "tianbing1"},
			},
		},
		[9] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[10]:Close()"},
			},
		},
		[10] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}

--大闹天宫                          --已被打入冷宫
InstanceSetting[1000]=
{
	nTaskInstance = 1;
    nMinLevel = 10;
    nMaxLevel = 60;
	nMapId = 1000;
    tbBeginPos = {3018,589,1048576},
	tbLeavePos = {4,6960,779,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
       [1] = {nTemplate = 290, nLevel = -1, nSeries = -1},		-- 天兵
	   [2] = {nTemplate = 291, nLevel = 19, nSeries = -1},		-- 天兵
	   [3] = {nTemplate = 292, nLevel = 19, nSeries = -1},		-- 孙悟空
	   [4] = {nTemplate = 293, nLevel = -1, nSeries = -1},		-- 孙悟空
	   [5] = {nTemplate = 294, nLevel = -1, nSeries = -1},		-- 紫霞仙子
	   [6] = {nTemplate = 295, nLevel = -1, nSeries = -1},		-- 紫霞仙子
	   [7] = {nTemplate = 296, nLevel = 19, nSeries = -1},		-- 巨灵神
	   [8] = {nTemplate = 297, nLevel = -1, nSeries = -1},		-- 巨灵神
	   [9] = {nTemplate = 298, nLevel = 19, nSeries = -1},		-- 哪咤
	   [10] = {nTemplate = 299, nLevel = -1, nSeries = -1},		-- 哪咤
	   [11] = {nTemplate = 300, nLevel = 19, nSeries = -1},		-- 二郎神
	   [12] = {nTemplate = 301, nLevel = -1, nSeries = -1},		-- 二郎神
	   [13] = {nTemplate = 302, nLevel = -1, nSeries = -1},		-- 哮天犬（停用）
	   [14] = {nTemplate = 303, nLevel = -1, nSeries = -1},		-- 哮天犬（停用）
	   [15] = {nTemplate = 304, nLevel = 1, nSeries = -1},		-- 透明自杀
	   [16] = {nTemplate = 305, nLevel = 1, nSeries = -1},		-- 透明目标
	   [17] = {nTemplate = 306, nLevel = -1, nSeries = -1},		-- 火焰陷阱
	   [18] = {nTemplate = 307, nLevel = 1, nSeries = -1},		-- 透明被杀
	   [19] = {nTemplate = 308, nLevel = -1, nSeries = -1},		-- 哮天犬
	   [20] = {nTemplate = 309, nLevel = -1, nSeries = -1},		-- 小哮天犬
	   [21] = {nTemplate = 310, nLevel = -1, nSeries = -1},		-- 火球1
	   [22] = {nTemplate = 311, nLevel = -1, nSeries = -1},		-- 火球2
	   [23] = {nTemplate = 312, nLevel = -1, nSeries = -1},		-- 火柱

	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			{Instance.TIME_INFO, -1, Lang.mission.str229[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			    --{Instance.ADD_NPC, 4, 1, 2, "wukong", "wukong"},
				--{Instance.ADD_NPC, 6, 1, 2, "zhixia", "zhixia"},
				--{Instance.NPC_BUBBLE, -1, "wukong", "我蟠桃忘拿了！你先走！",1},
				--{Instance.NPC_BUBBLE, -1, "wukong", "顺便帮我清理一下那些麻烦人！",4},
				--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 5, nNum = 1,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 15, 3, 3, "touming", "touming"},
				{Instance.ADD_NPC, 16, 1, 3, "mubiao", "mubiao"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming"},
			{Instance.DEL_NPC, "mubiao"},
			{Instance.DEL_NPC, "zhixia"},
			{Instance.DEL_NPC, "wukong"},
			--{Instance.ADD_NPC, 5, 1, 3, "zhixia1", "zhixia"},
			},
		},
		[4] = {nTime = 5, nNum = 1,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			   {Instance.ADD_NPC, 15, 3, 4, "touming1", "touming1"},
			   {Instance.ADD_NPC, 16, 1, 4, "mubiao", "mubiao"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming1"},
			{Instance.DEL_NPC, "mubiao"},
			},
		},
		[5] = {nTime = 6, nNum = 0,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 8, 1, 5, "juling", "juling"},
				{Instance.NPC_BUBBLE, -1, "juling", Lang.mission.str527[Lang.Idx],2},
				--{Instance.ADD_NPC, 2, 3, 5, "tianbing", "tianbing"},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "zhixia1"},
			},
		},
		[7] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			    {Instance.DEL_NPC, "juling"},
			    {Instance.ADD_NPC, 7, 1, 7, "juling1", "juling"},
				{Instance.NPC_BUBBLE, -1, "juling1", Lang.mission.str528[Lang.Idx],1},
				{Instance.TARGET_INFO, -1, Lang.mission.str529[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "tianbing1"},
			{Instance.DEL_NPC, "tianbing"},
			{Instance.DO_SCRIPT, "self.tbLock[10]:Close()"},
			},
		},
		[8] = {nTime =5 , nNum = 0,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 8, nNum = 0,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 6, 9, "tianbing", "tianbing"},
			},
			tbUnLockEvent =
			{

			},
		},
		[10] = {nTime = 10, nNum = 0,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 6, 10, "tianbing1", "tianbing1"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 1, 6, 9, "tianbing", "tianbing"},
			},
		},
		[11] = {nTime = 2, nNum = 0,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 1, 6, 10, "tianbing1", "tianbing1"},
			},
		},
		[12] = {nTime = 5, nNum = 1,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 15, 3, 12, "touming2", "touming2"},
				{Instance.ADD_NPC, 16, 1, 12, "mubiao1", "mubiao1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming2"},
			{Instance.DEL_NPC, "mubiao1"},
			--{Instance.ADD_NPC, 5, 1, 12, "zhixia2", "zhixia1"},
			},
		},
		[13] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "zhixia2"},
			},
		},
		[14] = {nTime = 6, nNum = 0,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 21, 1, 14, "huoqiu1", "huoqiu1"},
				{Instance.ADD_NPC, 22, 1, 14, "huoqiu2", "huoqiu2"},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {22},
			tbStartEvent =
			{
			    {Instance.DEL_NPC, "nazha"},
			    {Instance.ADD_NPC, 9, 1, 15, "nazha1", "nazha1"},
				--{Instance.NPC_BUBBLE, -1, "nazha1", "看我怎么把你戳成马蜂窝吧！",1},
				{Instance.TARGET_INFO, -1, Lang.mission.str530[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "tianbing2"},
			{Instance.DEL_NPC, "xianjing","xianjing_timer"},
			},
		},
		[16] = {nTime = 2, nNum = 0,		-- 总计时
			tbPrelock = {15},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[17] = {nTime = 10, nNum = 0,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 17, 3, 17, "xianjing", "xianjing", 100, 10, "xianjing_timer"},
			},
		},
		[18] = {nTime = 10, nNum = 1,		-- 总计时
			tbPrelock = {16},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 15, 3, 18, "touming3", "touming3"},
				{Instance.ADD_NPC, 16, 1, 18, "mubiao2", "mubiao2"},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "touming3"},
			{Instance.DEL_NPC, "mubiao2"},
			--{Instance.ADD_NPC, 5, 1, 18, "zhixia3", "zhixia2"},
			},
		},
		[19] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {18},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "zhixia3"},
			},
		},
		[20] = {nTime = 6, nNum = 0,		-- 总计时
			tbPrelock = {18},
			tbStartEvent =
			{
			    {Instance.ADD_NPC, 12, 1, 20, "erlang", "erlang"},
				{Instance.NPC_BUBBLE, -1, "erlang", Lang.mission.str531[Lang.Idx],3},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[21] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {20},
			tbStartEvent =
			{
			    {Instance.DEL_NPC, "erlang"},
				{Instance.DEL_NPC, "quan"},
			    {Instance.ADD_NPC, 11, 1, 21, "erlang1", "erlang"},
				--{Instance.ADD_NPC, 13, 1, 21, "quan1", "quan"},
				{Instance.NPC_BUBBLE, -1, "erlang1", Lang.mission.str532[Lang.Idx],1},
				{Instance.TARGET_INFO, -1, Lang.mission.str533[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			  {Instance.DEL_NPC, "quan1"},
			  {Instance.DEL_NPC, "quan2"},
			},
		},
		[22] = {nTime = 3, nNum = 0,		-- 插到哪咤那里
			tbPrelock = {14},
			tbStartEvent =
			{
			    {Instance.DEL_NPC, "huoqiu1"},
                {Instance.DEL_NPC, "huoqiu2"},
			    {Instance.ADD_NPC, 10, 1, 14, "nazha", "nazha"},
				{Instance.NPC_BUBBLE, -1, "nazha", Lang.mission.str534[Lang.Idx],1},
				{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[23] = {nTime = 0, nNum = 1,		-- 插到哪咤那里
			tbPrelock = {20},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 18, 3, 23, "beisha", "beisha"},
			},
			tbUnLockEvent =
			{
            {Instance.DEL_NPC, "beisha"},
			{Instance.ADD_NPC, 19, 1, 21, "quan1", "quan1"},
			{Instance.ADD_NPC, 20, 5, 21, "quan2", "quan2"},
			},
		},
		[24] = {nTime = 0, nNum = 1,		-- 插到哪咤那里
			tbPrelock = {23},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 18, 3, 23, "beisha1", "beisha"},
			},
			tbUnLockEvent =
			{

			},
		},
		[25] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {21},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[26]:Close()"},
			},
		},
		[26] = {nTime = 0, nNum = 1,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO, true, true, false},
			},
		},

    }
}
