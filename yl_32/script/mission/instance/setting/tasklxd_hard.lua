Include("script/mission/instance/instance_def.lua")
if not Instance.tbSetting then
	Instance.tbSetting = {}
end

local InstanceSetting = Instance.tbSetting;

--蟠桃树前
InstanceSetting[916]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 916;
    tbBeginPos = {860,858,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 315, nLevel = -1, nSeries = -1},		-- 守卫
	  [2] = {nTemplate = 316, nLevel = 26, nSeries = -1},		-- 鸟
	  [3] = {nTemplate = 317, nLevel = 26, nSeries = -1},		-- 鸟
	  [4] = {nTemplate = 318, nLevel = -1, nSeries = -1},		-- 结晶
	  [5] = {nTemplate = 319, nLevel = -1, nSeries = -1},		-- 鸟
	  [6] = {nTemplate = 320, nLevel = -1, nSeries = -1},		-- 鸟
	  [7] = {nTemplate = 321, nLevel = -1, nSeries = -1},		-- 鸟人
	  [8] = {nTemplate = 322, nLevel = 26, nSeries = -1},		-- 鸟人
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
		[2] = {nTime = 0, nNum = 15 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 7, 2, "niao", "niao"},
			{Instance.ADD_NPC, 2, 2, 2, "niao1", "niao1"},
			{Instance.ADD_NPC, 2, 1, 2, "niao11", "niao11"},
			{Instance.ADD_NPC, 2, 5, 2, "niao2", "niao2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str230[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 5, "niao3", "niao3"},
			{Instance.ADD_NPC, 6, 1, 5, "niao4", "niao4"},
			{Instance.NPC_BUBBLE, -1,"niao3",Lang.mission.str233[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "niao3"},
			{Instance.DEL_NPC, "niao4"},
			},
		},
		[6] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 6, "niaoren1", "niaoren1"},
			{Instance.ADD_NPC, 7, 1, 6, "niaoren11", "niaoren11"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "niaoren1"},
			{Instance.DEL_NPC, "niaoren11"},
			},
		},
		[7] = {nTime = 0, nNum = 10 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 8, 8, 7, "niaoren2", "niaoren2"},
			{Instance.ADD_NPC, 8, 1, 7, "niaoren22", "niaoren22"},
			{Instance.ADD_NPC, 8, 1, 7, "niaoren222", "niaoren222"},
		    {Instance.NPC_BUBBLE, -1,"niaoren22",Lang.mission.str234[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str235[Lang.Idx]},
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
		[9] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},
    }
}


--蟠桃树后
InstanceSetting[917]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 917;
    tbBeginPos = {926,1557,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 330, nLevel = 26, nSeries = -1},		-- 猴子（远攻）
	  [2] = {nTemplate = 331, nLevel = -1, nSeries = -1},		-- 悟空巨大
	  [3] = {nTemplate = 332, nLevel = -1, nSeries = -1},		-- 悟空正常
	  [4] = {nTemplate = 333, nLevel = -1, nSeries = -1},		-- 蟠桃园守卫
	  [5] = {nTemplate = 334, nLevel = 26, nSeries = -1},		-- 猕猴（近攻）
	  [6] = {nTemplate = 335, nLevel = -1, nSeries = -1},		-- 猕猴（剧情2）
	  [7] = {nTemplate = 336, nLevel = 1, nSeries = -1},		-- 桃（剧情2）
	  [8] = {nTemplate = 337, nLevel = -1, nSeries = -1},		-- 桃（站）
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
		[2] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 4 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 5, "houzi11", "houzi11"},
			{Instance.ADD_NPC, 5, 3, 5, "houzi1", "houzi1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str240[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 12 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "wukong2"},
			{Instance.DEL_NPC, "houzi"},
			{Instance.ADD_NPC, 5, 8, 6, "houzi2", "houzi2"},
			{Instance.ADD_NPC, 5, 4, 6, "houzi4", "houzi4"},
			{Instance.TARGET_INFO, -1, Lang.mission.str240[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 15 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 15, 9, "houzi5", "houzi5"},
			{Instance.TARGET_INFO, -1, Lang.mission.str240[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[11]:Close()"},
			},
		},
		[11] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},
    }
}

--花果山
InstanceSetting[918]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 918;
    tbBeginPos = {3372,1353,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
	  [1] = {nTemplate = 347, nLevel = -1, nSeries = -1},		-- 火焰(剧情)
	  [2] = {nTemplate = 348, nLevel = -1, nSeries = -1},		-- 大眼（剧情）
	  [3] = {nTemplate = 345, nLevel = 26, nSeries = -1},		-- 火焰
	  [4] = {nTemplate = 346, nLevel = 26, nSeries = -1},		-- 大眼
	  [5] = {nTemplate = 349, nLevel = -1, nSeries = -1},		-- 火焰(剧情)
	  [6] = {nTemplate = 350, nLevel = 26, nSeries = -1},		-- 法师(站立)
	  [7] = {nTemplate = 351, nLevel = 1, nSeries = -1},		-- 自杀
	  [8] = {nTemplate = 352, nLevel = 1, nSeries = -1},		-- 目标
	  [9] = {nTemplate = 353, nLevel = 26, nSeries = -1},		-- 祭司
	  [10] = {nTemplate = 354, nLevel = 26, nSeries = -1},		-- 法师
	  [11] = {nTemplate = 355, nLevel = -1, nSeries = -1},		-- 火焰(剧情2)
	  [12] = {nTemplate = 356, nLevel = -1, nSeries = -1},		-- 祭司(剧情2)
	  [13] = {nTemplate = 357, nLevel = -1, nSeries = -1},		-- 法师(剧情2)
	   [14] = {nTemplate = 358, nLevel = 26, nSeries = -1},		-- 祭司
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
		[2] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.NOVICE_GUIDE, -1, 15},
			{Instance.ADD_NPC, 3, 1, 4, "huoyan2", "huoyan"},
			{Instance.ADD_NPC, 3, 2, 4, "huoyan3", "huoyan1"},
			{Instance.ADD_NPC, 10, 4, 4, "fashi", "fashi"},
			{Instance.NPC_BUBBLE, -1,"huoyan2",Lang.mission.str246[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str240[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 4 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 3, 5, "dayan2", "dayan2"},
			{Instance.ADD_NPC, 4, 1, 5, "dayan3", "dayan3"},
			{Instance.NPC_BUBBLE, -1,"dayan3",Lang.mission.str247[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str240[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 14, 1, 8, "jisi1", "jisi"},
			{Instance.ADD_NPC, 3, 2, 8, "huoyan444", "huoyan2"},
			{Instance.ADD_NPC, 3, 1, 8, "huoyan4444", "huoyan22"},
			{Instance.ADD_NPC, 6, 4, 8, "fashi11", "fashi1"},
			--{Instance.NPC_BUBBLE, -1,"huoyan4444","小心他的冰冻！好猛的！", 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str240[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
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
		[10] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}

--巨人来袭
InstanceSetting[919]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 919;
    tbBeginPos = {4156,1196,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 360, nLevel = 1, nSeries = -1},		-- 巨人1
	  [2] = {nTemplate = 361, nLevel = 1, nSeries = -1},		-- 巨人2
	  [3] = {nTemplate = 362, nLevel = -1, nSeries = -1},		-- 巨石1
	  [4] = {nTemplate = 363, nLevel = -1, nSeries = -1},		-- 被撞目标
	  [5] = {nTemplate = 364, nLevel = -1, nSeries = -1},		-- 巨石2
	  [6] = {nTemplate = 365, nLevel = -1, nSeries = -1},		-- 巨石3
	  [7] = {nTemplate = 366, nLevel = -1, nSeries = -1},		-- 侏儒（剧情）
	  [8] = {nTemplate = 367, nLevel = 26, nSeries = -1},		-- 侏儒
	  [9] = {nTemplate = 368, nLevel = -1, nSeries = -1},		-- 侏儒（跑）
	  [10] = {nTemplate = 370, nLevel = -1, nSeries = -1},		-- 巨石3
	  [11] = {nTemplate = 371, nLevel = -1, nSeries = -1},		-- 石人侏儒
	  [12] = {nTemplate = 372, nLevel = 26, nSeries = -1},		-- 石人侏儒
	  [13] = {nTemplate = 373, nLevel = 1, nSeries = -1},		-- 透明目标
	  [14] = {nTemplate = 374, nLevel = 1, nSeries = -1},		-- 自杀
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
		[2] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 13 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 10, 6, 3, "jushi4", "jushi4"},
			{Instance.ADD_NPC, 8, 13, 3, "zhuru2", "zhuru2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str250[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 7, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 4, "shiren", "shiren"},
			{Instance.ADD_NPC, 13, 1, 6, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 14, 1, 6, "zisha", "zisha"},
			{Instance.NPC_BUBBLE, -1,"shiren",Lang.mission.str251[Lang.Idx], 3},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "shiren"},
			},
		},
		[5] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 12, 1, 5, "shiren1", "shiren1"},
			{Instance.NPC_BUBBLE, -1,"shiren1",Lang.mission.str252[Lang.Idx], 10},
			{Instance.NPC_BUBBLE, -1,"shiren1",Lang.mission.str253[Lang.Idx], 25},
			{Instance.TARGET_INFO, -1, Lang.mission.str254[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 16, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {13},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[16] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
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
		[17] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}

--小人国
InstanceSetting[920]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 920;
    tbBeginPos = {3081,3192,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 375, nLevel = 26, nSeries = -1},		-- 牛兵
		 [2] = {nTemplate = 376, nLevel = -1, nSeries = -1},		-- 墙（被攻击?
		 [3] = {nTemplate = 377, nLevel = -1, nSeries = -1},		-- 墙（攻击）?
		 [4] = {nTemplate = 378, nLevel = 26, nSeries = -1},		-- 牛兵
		 [5] = {nTemplate = 379, nLevel = 26, nSeries = -1},		-- 牛兵
		 [6] = {nTemplate = 380, nLevel = 26, nSeries = -1},		-- 牛兵
		 [7] = {nTemplate = 381, nLevel = 26, nSeries = -1},		-- 邪恶牛兵
		 [8] = {nTemplate = 382, nLevel = 26, nSeries = -1},		-- 邪恶牛兵
		 [9] = {nTemplate = 383, nLevel = -1, nSeries = -1},		-- 马车
		 [10] = {nTemplate = 384, nLevel = -1, nSeries = -1},		-- 士兵
		 [11] = {nTemplate = 385, nLevel = -1, nSeries = -1},		-- 邪恶牛兵
		 [12] = {nTemplate = 386, nLevel = -1, nSeries = -1},		-- 邪恶牛兵
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
		[2] = {nTime = 0, nNum = 5 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 2, "shibing1", "shibing1"},
			{Instance.ADD_NPC, 3, 1, 2, "shibing2", "shibing2"},
			{Instance.ADD_NPC, 1, 5, 2, "niubing", "niubing"},
			{Instance.ADD_NPC, 2, 5, 2, "qiang", "qiang"},
			{Instance.NPC_BUBBLE, -1,"shibing1",Lang.mission.str257[Lang.Idx], 5},
			{Instance.NPC_BUBBLE, -1,"shibing2",Lang.mission.str258[Lang.Idx], 25},
			{Instance.TARGET_INFO, -1, Lang.mission.str259[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 3, "niubing1", "niubing1"},
			{Instance.ADD_NPC, 5, 1, 3, "niubing2", "niubing2"},
			{Instance.ADD_NPC, 6, 1, 3, "niubing3", "niubing3"},
			{Instance.NPC_BUBBLE, -1,"niubing2",Lang.mission.str351[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str259[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 4, 1, 4, "niubing4", "niubing4"},
			{Instance.ADD_NPC, 5, 2, 14, "niubing55", "niubing5"},
			{Instance.ADD_NPC, 6, 2, 14, "niubing66", "niubing6"},
			{Instance.NPC_BUBBLE, -1,"niubing5",Lang.mission.str261[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str259[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 5 ,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 5, "xiee1", "xiee1"},
			{Instance.ADD_NPC, 7, 2, 5, "xiee2", "xiee2"},
			{Instance.ADD_NPC, 12, 2, 5, "xiee4", "xiee4"},
			{Instance.DEL_NPC, "shibing1"},
			{Instance.DEL_NPC, "shibing2"},
			{Instance.DEL_NPC, "qiang"},
			{Instance.TARGET_INFO, -1, Lang.mission.str260[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 12, 2, 11, "xiee4", "xiee4"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str260[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 11, 1, 12, "xiee", "xiee"},
			--{Instance.NPC_BUBBLE, -1,"xiee","哇哈哈~高手出场都是带特效的！", 1},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "xiee"},
			},
		},
		[13] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 7, 5, 13, "xiee3", "xiee3"},
			--{Instance.TARGET_INFO, -1, "Lang.mission.str260[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 4, 1, 4, "niubing4", "niubing4"},
			{Instance.ADD_NPC, 5, 1, 14, "niubing5", "niubing5"},
			{Instance.ADD_NPC, 6, 3, 14, "niubing6", "niubing6"},
			{Instance.TARGET_INFO, -1, Lang.mission.str259[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {13},
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
		[16] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}

--牛魔王洞2
InstanceSetting[921]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 921;
    tbBeginPos = {4282,4327,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 390, nLevel = -1, nSeries = -1},		-- 牛魔王（带电）
		 [2] = {nTemplate = 391, nLevel = 25, nSeries = -1},		-- 邪恶牛兵
		 [3] = {nTemplate = 392, nLevel = -1, nSeries = -1},		-- 牛兵（剧情2）
		 [4] = {nTemplate = 393, nLevel = 1, nSeries = -1},		-- 自杀
		 [5] = {nTemplate = 394, nLevel = 1, nSeries = -1},		-- 目标
		 [6] = {nTemplate = 395, nLevel = -1, nSeries = -1},		-- 邪恶牛兵
		 [7] = {nTemplate = 396, nLevel = -1, nSeries = -1},		-- 牛兵
		 [8] = {nTemplate = 397, nLevel = 25, nSeries = -1},		-- 邪恶牛兵
		 [9] = {nTemplate = 398, nLevel = -1, nSeries = -1},		-- 牛兵
		 [10] = {nTemplate = 399, nLevel = 25, nSeries = -1},		-- 牛魔王（不带电）
		 [11] = {nTemplate = 400, nLevel = -1, nSeries = -1},		-- 爆炸
		 [12] = {nTemplate = 401, nLevel = 25, nSeries = -1},		-- 邪恶牛兵
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
		[2] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{

			--{Instance.ADD_NPC, 3, 4, 2, "niubing", "niubing"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str260[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "niubing"},
			},
		},
		[3] = {nTime = 0, nNum = 4 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 2, 3, "xiee", "xiee"},
			{Instance.ADD_NPC, 2, 2, 3, "xiee1", "xiee1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str260[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[5] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 5 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 8, 1, 6, "xiee22", "xiee2"},
			{Instance.ADD_NPC, 8, 4, 6, "xiee33", "xiee3"},
			{Instance.TARGET_INFO, -1, Lang.mission.str260[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 4 ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 12, 4, 11, "xiee4", "xiee4"},
			{Instance.TARGET_INFO, -1, Lang.mission.str352[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 10, 1, 12, "mowang1", "mowang1"},
			{Instance.NPC_BUBBLE, -1,"mowang1",Lang.mission.str268[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str269[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[14]:Close()"},
			},
		},
		[14] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--深海小道
InstanceSetting[922]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 922;
    tbBeginPos = {3260,670,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 406, nLevel = -1, nSeries = -1},		-- 鱼人火
		 [2] = {nTemplate = 407, nLevel = 26, nSeries = -1},		-- 鱼人
		 [3] = {nTemplate = 408, nLevel = -1, nSeries = -1},		-- 鱼人冰
		 [4] = {nTemplate = 409, nLevel = -1, nSeries = -1},		-- 鱼人跑1
		 [5] = {nTemplate = 410, nLevel = -1, nSeries = -1},		-- 鱼人跑2
		 [6] = {nTemplate = 411, nLevel = 1, nSeries = -1},		-- 目标
		 [7] = {nTemplate = 412, nLevel = 1, nSeries = -1},		-- 自杀
		 [8] = {nTemplate = 413, nLevel = -1, nSeries = -1},		-- 鱼人傻站
		 [9] = {nTemplate = 414, nLevel = -1, nSeries = -1},		-- 鱼人火
		 [10] = {nTemplate = 415, nLevel = -1, nSeries = -1},		-- 鱼人治愈
		 [11] = {nTemplate = 416, nLevel = -1, nSeries = -1},		-- 宝物
		 [12] = {nTemplate = 417, nLevel = -1, nSeries = -1},		-- 宝物
		 [13] = {nTemplate = 418, nLevel = -1, nSeries = -1},		-- 鱼人打宝珠
		 [14] = {nTemplate = 419, nLevel = -1, nSeries = -1},		-- 鱼人恶霸
		 [15] = {nTemplate = 420, nLevel = 26, nSeries = -1},		-- 鱼人恶霸

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
		[2] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 17 ,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 16, 3, "yuguai1", "yuguai1"},
			{Instance.ADD_NPC, 2, 1, 3, "yuguai111", "yuguai1"},
			{Instance.NPC_BUBBLE, -1,"yuguai111",Lang.mission.str271[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str273[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0 ,nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 16 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{

			{Instance.ADD_NPC, 2, 8, 6, "yuguai33", "yuguai3"},
			{Instance.ADD_NPC, 2, 8, 6, "yuguai222", "yuguai2"},
			{Instance.NPC_BUBBLE, -1,"yuguai22","啊~~~~妈呀！", 1},
			{Instance.BLACK_MSG, -1, Lang.mission.str275[Lang.Idx]},
			{Instance.TARGET_INFO, -1, Lang.mission.str273[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 15 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 15, 9, "yuguai4", "yuguai4"},
			{Instance.TARGET_INFO, -1, Lang.mission.str273[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[11] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[12] = {nTime = 0, nNum = 4 ,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 15, 1, 12, "eba22", "eba22"},
			{Instance.ADD_NPC, 15, 3, 12, "eba2", "eba2"},
			{Instance.NPC_BUBBLE, -1,"eba22",Lang.mission.str281[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str282[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
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
		[16] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--复仇巨人
InstanceSetting[923]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 923;
    tbBeginPos = {2512,1387,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 425, nLevel = -1, nSeries = -1},		-- 邪恶魔人
		 [2] = {nTemplate = 426, nLevel = 26, nSeries = -1},		-- 邪恶魔人
		 [3] = {nTemplate = 427, nLevel = -1, nSeries = -1},		-- 透明
		 [4] = {nTemplate = 428, nLevel = -1, nSeries = -1},		-- 旋风
		 [5] = {nTemplate = 429, nLevel = -1, nSeries = -1},		-- 天兵站打
		 [6] = {nTemplate = 430, nLevel = -1, nSeries = -1},		-- 天兵冲打
		 [7] = {nTemplate = 431, nLevel = -1, nSeries = -1},		-- 魔兵站打
		 [8] = {nTemplate = 432, nLevel = -1, nSeries = -1},		-- 魔兵
		 [9] = {nTemplate = 433, nLevel = -1, nSeries = -1},		-- 天兵带头
		 [10] = {nTemplate = 434, nLevel = -1, nSeries = -1},		-- 天兵
		 [11] = {nTemplate = 435, nLevel = -1, nSeries = -1},		-- 天兵
		 [12] = {nTemplate = 436, nLevel = 26, nSeries = -1},		-- 魔兵
		 [13] = {nTemplate = 437, nLevel = -1, nSeries = -1},		-- 自杀
		 [14] = {nTemplate = 438, nLevel = -1, nSeries = -1},		-- 目标
		 [15] = {nTemplate = 439, nLevel = 26, nSeries = -1},		-- 弓箭手
		 [16] = {nTemplate = 440, nLevel = -1, nSeries = -1},		-- 天兵
		 [17] = {nTemplate = 441, nLevel = -1, nSeries = -1},		-- 邪恶魔人
		 [18] = {nTemplate = 442, nLevel = -1, nSeries = -1},		-- 魔石
		 [19] = {nTemplate = 443, nLevel = -1, nSeries = -1},		-- 魔兵（傻站）
		 [20] = {nTemplate = 444, nLevel = -1, nSeries = -1},		-- 魔兵（发技能?
		 [21] = {nTemplate = 445, nLevel = -1, nSeries = -1},		-- 火球
		 [22] = {nTemplate = 446, nLevel = -1, nSeries = -1},		-- 魔兵
		 [23] = {nTemplate = 447, nLevel = -1, nSeries = -1},		-- 魔兵弓箭手
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
		[2] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 5, 5, 2, "juqingtian", "juqingtian"},             --场景外的打斗
			--{Instance.ADD_NPC, 7, 5, 2, "juqingmo", "juqingmo"},                 --场景外的打斗
			--{Instance.ADD_NPC, 9, 1, 2, "tianbing", "tianbing"},
			--{Instance.ADD_NPC, 10, 2, 2, "tianbing1", "tianbing1"},
			--{Instance.ADD_NPC, 11, 2, 2, "tianbing2", "tianbing2"},
			--{Instance.ADD_NPC, 18, 1, 2, "moshi", "moshi"},
			{Instance.ADD_NPC, 17, 1, 13, "moren1", "moren1"},
			{Instance.ADD_NPC, 21, 1, 12, "huoqiu", "huoqiu"},
			--{Instance.NPC_BUBBLE, -1,"tianbing","快~！快阻止那个魔兵！", 2},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "huoqiu"},
			{Instance.DEL_NPC, "moren1"},
			},
		},
		[3] = {nTime = 5, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 2, 3, "tianbing3", "tianbing3"},
			{Instance.ADD_NPC, 8, 4, 7, "mobing", "mobing"},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 13, 3, 4, "zisha", "zisha"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "zisha"},
			},
		},
		[5] = {nTime = 5, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 12, 1, 7, "mobing11", "mobing11"},
			{Instance.ADD_NPC, 12, 5, 7, "mobing1", "mobing1"},
			{Instance.NPC_BUBBLE, -1,"mobing11",Lang.mission.str286[Lang.Idx], 4},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 5, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 15, 6, 7, "jian", "jian"},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 16 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.TARGET_INFO, -1, Lang.mission.str287[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 25, nNum = 1 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 13, 4, 8, "zisha1", "zisha1"},
			{Instance.ADD_NPC, 14, 1, 8, "mubiao1", "mubiao1"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "zisha1"},
			{Instance.DEL_NPC, "mubiao1"},
			},
		},
		[9] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			--{Instance.SHOW_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			--{Instance.ADD_NPC, 16, 2, 9, "tianbing4", "tianbing4"},
			--{Instance.ADD_NPC, 16, 4, 9, "tianbing5", "tianbing5"},
			--{Instance.ADD_NPC, 16, 1, 9, "tianbing55", "tianbing55"},
			--{Instance.ADD_NPC, 16, 2, 9, "tianbing6", "tianbing6"},
			--{Instance.ADD_NPC, 19, 1, 9, "mobing2", "mobing2"},
			--{Instance.NPC_BUBBLE, -1,"tianbing55","亲！别激动！放开那个石头吧！", 1},
			--{Instance.NPC_BUBBLE, -1,"mobing2","你们死期到了！", 3},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "mobing2"},
			},
		},
		[11] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 20, 1, 11, "mobing22", "mobing2"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "moshi"},
			},
		},
		[12] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {11},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 21, 3, 12, "huoqiu", "huoqiu"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "mobing22"},
			{Instance.DEL_NPC, "huoqiu"},
			},
		},
		[13] = {nTime = 0, nNum = 0 ,		-- 总计时
			tbPrelock = {12},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 17, 1, 13, "moren1", "moren1"},
			--{Instance.NPC_BUBBLE, -1,"moren1","啊~~", 1},
			--{Instance.NPC_BUBBLE, -1,"moren1","终于，我终于又重见天日啦！", 4},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.REMOVE_NOTICE_STATE, -1, Lang.mission.str237[Lang.Idx]},
			{Instance.DEL_NPC, "moren1"},
			{Instance.DEL_NPC, "tianbing5"},
			{Instance.DEL_NPC, "tianbing6"},
			{Instance.DEL_NPC, "tianbing55"},
			{Instance.DEL_NPC, "tianbing4"},
			},
		},
		[14] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {13},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 14, "moren11", "moren1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str292[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "mo"},
			{Instance.DEL_NPC, "mo1"},
			{Instance.DEL_NPC, "mo2"},
			{Instance.DEL_NPC, "mo3"},
			},
		},
		[15] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风
			tbPrelock = {13},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 15, "touming1", "touming1"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[16] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风
			tbPrelock = {13},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 16, "touming11", "touming1"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 16, "xuanfeng11", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo2", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo3", "mo"},
			},
		},
		[17] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 17, "touming2", "touming2"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 17, "xuanfeng2", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[18] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 18, "touming22", "touming2"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 18, "xuanfeng22", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[19] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 19, "touming111", "touming1"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 19, "xuanfeng111", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[20] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 20, "touming222", "touming2"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 20, "xuanfeng222", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[21] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 21, "touming1111", "touming1"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 21, "xuanfeng1111", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[22] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 22, "touming2222", "touming2"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 22, "xuanfeng2222", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[23] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 23, "touming11111", "touming1"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 23, "xuanfeng11111", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[24] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {13},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 24, "touming22222", "touming2"},
			},
			tbUnLockEvent =
			{
			--{Instance.ADD_NPC, 4, 2, 24, "xuanfeng22222", "xuanfeng"},
			{Instance.ADD_NPC, 22, 6, 15, "mo", "mo"},
			{Instance.ADD_NPC, 23, 3, 15, "mo1", "mo"},
			},
		},
		[25] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {14},
			tbStartEvent =
			{
            {Instance.SET_TASK, -1, 1022, 116, 1},
			{Instance.SET_TASK, -1, 1022, 117, 1},
			{Instance.SET_TASK, -1, 1022, 118, 1},
			{Instance.SET_TASK, -1, 1022, 119, 1},
			{Instance.SET_TASK, -1, 1022, 120, 1},
			{Instance.SET_TASK, -1, 1022, 121, 1},
			{Instance.SET_TASK, -1, 1022, 122, 1},
			{Instance.SET_TASK, -1, 1022, 123, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[26]:Close()"},
			},
		},
		[26] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}

--池塘
InstanceSetting[924]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 924;
    tbBeginPos = {1167,945,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 550, nLevel = -1, nSeries = -1},		-- 哮天犬
		 [2] = {nTemplate = 551, nLevel = -1, nSeries = -1},		-- 哮天母犬
		 [3] = {nTemplate = 552, nLevel = -1, nSeries = -1},		    -- 超猛钓鱼翁
		 [4] = {nTemplate = 553, nLevel = 26, nSeries = -1},		-- 超猛钓鱼翁
		 [5] = {nTemplate = 554, nLevel = 26, nSeries = -1},		-- 钓鱼翁
		 [6] = {nTemplate = 555, nLevel = -1, nSeries = -1},		-- 哮天犬（跑路）
		 [7] = {nTemplate = 556, nLevel = -1, nSeries = -1},		-- 哮天母犬（跑路）
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
		[2] = {nTime = 0, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 4, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {2},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 3, 1, 2, "yuwen1", "yuwen1"},
			{Instance.NPC_BUBBLE, -1,"yuwen1",Lang.mission.str294[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.NPC_BUBBLE, -1,"yuwen1",Lang.mission.str295[Lang.Idx], 1},
			},
		},
		[4] = {nTime = 0, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {3},
			tbStartEvent =
			{
            --{Instance.ADD_NPC, 5, 5, 4, "yuwen2", "yuwen2"},
			--{Instance.TARGET_INFO, -1, Lang.mission.str296[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 6 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {4},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 5, 6, 5, "yuwen2", "yuwen2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str296[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 8 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {5},
			tbStartEvent =
			{
            {Instance.ADD_NPC, 5, 8, 6, "yuwen2", "yuwen2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str296[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "yuwen1"},
            {Instance.ADD_NPC, 4, 1, 7, "yuwen11", "yuwen1"},
			{Instance.NPC_BUBBLE, -1,"yuwen11",Lang.mission.str297[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str298[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
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
		[10] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--天香崖
InstanceSetting[925]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 925;
    tbBeginPos = {1041,811,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 560, nLevel = 26, nSeries = -1},		-- 天香羚
		 [2] = {nTemplate = 561, nLevel = 26, nSeries = -1},		-- 巨型天香羚（旋转）
		 [3] = {nTemplate = 562, nLevel = 26, nSeries = -1},		-- 巨型天香羚（旋转）
		 [4] = {nTemplate = 563, nLevel = 26, nSeries = -1},		-- 巨型天香羚（旋转）
		 [5] = {nTemplate = 564, nLevel = -1, nSeries = -1},		-- 天香草
		 [6] = {nTemplate = 565, nLevel = -1, nSeries = -1},		-- 天香草（可采集）
		 [7] = {nTemplate = 566, nLevel = 26, nSeries = -1},		-- 巨型天香羚
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
		[2] = {nTime = 0, nNum = 10 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 10, 2, "yang", "yang"},
			{Instance.ADD_NPC, 5, 1, 3, "cao", "cao"},
			{Instance.ADD_NPC, 5, 5, 4, "cao1", "cao1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str300[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 8 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 3, "yang1", "yang1"},
			{Instance.ADD_NPC, 3, 1, 3, "yang2", "yang2"},
			{Instance.ADD_NPC, 4, 1, 3, "yang3", "yang3"},
			{Instance.ADD_NPC, 7, 4, 3, "yang4", "yang4"},
			{Instance.ADD_NPC, 7, 1, 3, "yang44", "yang44"},
			{Instance.NPC_BUBBLE, -1,"yang44",Lang.mission.str301[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str353[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {3},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 6, 1, 4, "cao2", "cao2"},
			--{Instance.DEL_NPC, "cao"},
			--{Instance.TARGET_INFO, -1, "打败所有巨型天香羚"},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[6]:Close()"},
			},
		},
		[6] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--神秘盆地1
InstanceSetting[926]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 926;
    tbBeginPos = {2692,1099,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 570, nLevel = -1, nSeries = -1},		-- 百年老妖（动作1）
		 [2] = {nTemplate = 571, nLevel = -1, nSeries = -1},		-- 百年老妖（动作2）
		 [3] = {nTemplate = 572, nLevel = -1, nSeries = -1},		-- 千年老妖（寻路）
		 [4] = {nTemplate = 573, nLevel = -1, nSeries = -1},		-- 千年老妖（傻站）
		 [5] = {nTemplate = 574, nLevel = 26, nSeries = -1},		-- 百年老妖
		 [6] = {nTemplate = 575, nLevel = 1, nSeries = -1},		-- 自杀
		 [7] = {nTemplate = 576, nLevel = 1, nSeries = -1},		-- 目标
		 [8] = {nTemplate = 577, nLevel = 26, nSeries = -1},		-- 精英百年老妖
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
		[2] = {nTime = 10, nNum = 1 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 2, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 6, 3, 2, "zisha", "zisha"},
			{Instance.ADD_NPC, 1, 1, 4, "jiazai", "jiazai"},          --加载用的
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "jiazai"},
			{Instance.DEL_NPC, "mubiao"},
			{Instance.DEL_NPC, "touming"},
			},
		},
		[3] = {nTime = 0, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {2},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 1, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 3, 4, "bai1", "bai1"},
			{Instance.ADD_NPC, 2, 3, 4, "bai11", "bai11"},
			--{Instance.ADD_NPC, 1, 3, 4, "bai2", "bai2"},
			--{Instance.ADD_NPC, 2, 3, 4, "bai22", "bai22"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 1, 3, 4, "bai2", "bai2"},
			{Instance.ADD_NPC, 2, 3, 4, "bai22", "bai22"},
			},
		},
		[5] = {nTime = 2, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 1, 3, 5, "bai3", "bai3"},
			{Instance.ADD_NPC, 2, 3, 5, "bai33", "bai33"},
			},
		},
		[6] = {nTime = 2, nNum = 0 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{

			},
		},
		[7] = {nTime = 0, nNum = 6 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "bai1"},
			{Instance.DEL_NPC, "bai11"},
			{Instance.ADD_NPC, 5, 3, 7, "bai111", "bai1"},
			{Instance.ADD_NPC, 5, 3, 7, "bai1111", "bai11"},
			{Instance.TARGET_INFO, -1, Lang.mission.str305[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 6 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "bai2"},
			{Instance.DEL_NPC, "bai22"},
			{Instance.ADD_NPC, 5, 3, 8, "bai222", "bai2"},
			{Instance.ADD_NPC, 5, 3, 8, "bai2222", "bai22"},
			{Instance.TARGET_INFO, -1, Lang.mission.str305[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 6 ,		-- 召唤龙卷风（用于BOSS重置血量后重新召唤）
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "bai3"},
			{Instance.DEL_NPC, "bai33"},
			{Instance.ADD_NPC, 8, 3, 9, "bai333", "bai3"},
			{Instance.ADD_NPC, 8, 3, 9, "bai3333", "bai33"},
			{Instance.TARGET_INFO, -1, Lang.mission.str305[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[11]:Close()"},
			},
		},
		[11] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--神秘盆地2
InstanceSetting[927]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 927;
    tbBeginPos = {2692,1099,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 580, nLevel = 26, nSeries = -1},		-- 百年老妖（巡逻）
		 [2] = {nTemplate = 581, nLevel = 33, nSeries = -1},		-- 百年老妖（被吸）
		 [3] = {nTemplate = 582, nLevel = -1, nSeries = -1},		-- 千年老妖（剧情）
		 [4] = {nTemplate = 583, nLevel = 26, nSeries = -1},		-- 千年老妖
		 [5] = {nTemplate = 584, nLevel = -1, nSeries = -1},		-- 宝塔（采集）
		 [6] = {nTemplate = 585, nLevel = -1, nSeries = -1},	    -- 宝塔（吸怪）
		 [7] = {nTemplate = 586, nLevel = 1, nSeries = -1},		    -- 千年老妖（被吸）
		 [8] = {nTemplate = 587, nLevel = 1, nSeries = -1},		    -- 目标
		 [9] = {nTemplate = 588, nLevel = -1, nSeries = -1},		-- 透明伤害
		 [10] = {nTemplate = 589, nLevel = -1, nSeries = -1},		-- 宝塔（剧情）
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
		[2] = {nTime = 0, nNum = 11 ,		--
			tbPrelock = {1},
			tbStartEvent =
			{
             {Instance.ADD_NPC, 1, 11, 2, "bai", "bai"},
			{Instance.TARGET_INFO, -1, Lang.mission.str306[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {2},
			tbStartEvent =
			{
             --{Instance.ADD_NPC, 5, 1, 3, "ta", "ta"},
			--{Instance.TARGET_INFO, -1, "打开宝塔"},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "ta"},
			},
		},
		[4] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {3},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 6, 1, 4, "ta1", "ta1"},
		--	{Instance.ADD_NPC, 2, 5, 4, "bai1", "bai1", 6, 4, "bai1_timer",0},
			--{Instance.ADD_NPC, 9, 1, 4, "shanghai", "ta"},           --透明伤害
			--{Instance.TARGET_INFO, -1, "收服30只妖怪"},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "shanghai"},
			},
		},
		[5] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {4},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 3, 1, 5, "qian", "qian"},       --透明伤害
			--{Instance.NPC_BUBBLE, -1,"qian","哼！还用这招！没那么容易！", 1},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "qian"},
			--{Instance.DEL_NPC, "ta1"},
			--{Instance.ADD_NPC, 10, 1, 3, "ta", "ta"},
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		--
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 6, "qian1", "qian1"},       --透明伤害
			{Instance.NPC_BUBBLE, -1,"qian1",Lang.mission.str310[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str312[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "ta"},
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {6},
			tbStartEvent =
			{
			--{Instance.ADD_NPC, 6, 1, 4, "ta1", "ta1"},
		   -- {Instance.ADD_NPC, 7, 1, 7, "qian1", "qian1"},
			--{Instance.NPC_BUBBLE, -1,"qian1","啊~~", 1},
			--{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "qian1"},
			},
		},
		[8] = {nTime = 3, nNum = 0 ,		-- 总计时
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
		[9] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--树精林
InstanceSetting[928]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 928;
    tbBeginPos = {819,1267,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 591, nLevel = 26, nSeries = -1},		-- 老树精
		 [2] = {nTemplate = 592, nLevel = 26, nSeries = -1},		-- 老树妖
		 [3] = {nTemplate = 593, nLevel = -1, nSeries = -1},		-- 孙悟空（剧情1）
		 [4] = {nTemplate = 594, nLevel = -1, nSeries = -1},		-- 孙悟空（剧情2）
		 [5] = {nTemplate = 595, nLevel = -1, nSeries = -1},		-- 喜羊羊（采集）
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
		[2] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 11 ,		--
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 9, 3, "shujing", "shujing"},
			{Instance.ADD_NPC, 1, 2, 3, "shujing11", "shujing11"},
			{Instance.NPC_BUBBLE, -1,"shujing11",Lang.mission.str317[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str318[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		--
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 4, "shujing1", "shujing1"},
			{Instance.NPC_BUBBLE, -1,"shujing1",Lang.mission.str319[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str320[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
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
		[8] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--海边
InstanceSetting[929]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 929;
    tbBeginPos = {3886,2321,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 600, nLevel = 26, nSeries = -1},		-- 螃蟹精
		 [2] = {nTemplate = 601, nLevel = 26, nSeries = -1},		-- 大螃蟹精
		 [3] = {nTemplate = 602, nLevel = -1, nSeries = -1},		-- 孙悟空（剧情1）
		 [4] = {nTemplate = 603, nLevel = -1, nSeries = -1},		-- 孙悟空（剧情2）
		 [5] = {nTemplate = 604, nLevel = -1, nSeries = -1},		-- 怪鱼
		 [6] = {nTemplate = 605, nLevel = -1, nSeries = -1},		-- 箱子（采集）
		 [7] = {nTemplate = 606, nLevel = -1, nSeries = -1},		-- 箱子（吸怪）
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
		[2] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 12 ,		--
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 3, "xiejing1", "xiejing1"},
			{Instance.ADD_NPC, 1, 11, 3, "xiejing", "xiejing"},
			{Instance.NPC_BUBBLE, -1,"xiejing1",Lang.mission.str326[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str327[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		--
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 4, "xiejing11", "xiejing11"},
			{Instance.NPC_BUBBLE, -1,"xiejing11",Lang.mission.str328[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str329[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
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
		[9] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
--小庙
InstanceSetting[930]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 930;
    tbBeginPos = {2449,4208,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 610, nLevel = 26, nSeries = -1},		-- 狐狸
		 [2] = {nTemplate = 611, nLevel = 26, nSeries = -1},		-- 狐狸精（毒雾）
		 [3] = {nTemplate = 612, nLevel = -1, nSeries = -1},		-- 孙悟空（剧情1）
		 [4] = {nTemplate = 613, nLevel = -1, nSeries = -1},		-- 孙悟空（剧情2）
		 [5] = {nTemplate = 614, nLevel = -1, nSeries = -1},		-- 狐狸
		 [6] = {nTemplate = 615, nLevel = -1, nSeries = -1},		-- 狐狸精（毒雾）
		 [7] = {nTemplate = 616, nLevel = -1, nSeries = -1},		-- 小庙
		 [8] = {nTemplate = 617, nLevel = -1, nSeries = -1},		-- 小庙
		 [9] = {nTemplate = 618, nLevel = -1, nSeries = -1},		-- 点燃
		 [10] = {nTemplate = 619, nLevel = -1, nSeries = -1},		-- 火焰
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
		[2] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 10 ,		--
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 3, "fang", "fang"},
			{Instance.ADD_NPC, 1, 9, 3, "huli", "huli"},
			{Instance.ADD_NPC, 1, 1, 3, "huli1", "huli1"},
			{Instance.NPC_BUBBLE, -1,"huli1",Lang.mission.str336[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str337[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {3},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 1 ,		--
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 5, "huli22", "huli22"},
			{Instance.NPC_BUBBLE, -1,"huli22",Lang.mission.str339[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str340[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {6},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {7},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
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
		[10] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}

--降雷台
InstanceSetting[931]=
{
	nTaskInstance = 0;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 931;
    tbBeginPos = {1267,952,1048576},
	tbLeavePos = {5,4000,2289,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
         [1] = {nTemplate = 621, nLevel = 26, nSeries = -1},		-- 鼓灵童
		 [2] = {nTemplate = 622, nLevel = 38, nSeries = -1},		-- 雷震子（傻站）
		 [3] = {nTemplate = 623, nLevel = -1, nSeries = -1},		-- 雷震子（打鼓）
		 [4] = {nTemplate = 624, nLevel = -1, nSeries = -1},		-- 透明被击
		 [5] = {nTemplate = 625, nLevel = -1, nSeries = -1},		-- 闪电
		 [6] = {nTemplate = 626, nLevel = -1, nSeries = -1},		-- 自杀
		 [7] = {nTemplate = 627, nLevel = -1, nSeries = -1},		-- 目标
		 [8] = {nTemplate = 628, nLevel = 26, nSeries = -1},		-- 鼓灵童boss
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
		[2] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 10, nNum = 1 ,		--
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 3, "zisha", "zisha"},
			{Instance.ADD_NPC, 7, 1, 3, "mubiao", "mubiao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str346[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "mubiao"},
			{Instance.DEL_NPC, "zisha"},
			},
		},
		[4] = {nTime = 0, nNum = 3 ,		--
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 3, 4, "lingtong", "lingtong"},
			{Instance.TARGET_INFO, -1, Lang.mission.str347[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {5},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 10, nNum = 1 ,		--
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 7, "zisha1", "zisha1"},
			{Instance.ADD_NPC, 7, 1, 7, "mubiao1", "mubiao1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str346[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "mubiao1"},
			{Instance.DEL_NPC, "zisha1"},
			},
		},
		[8] = {nTime = 0, nNum = 3 ,		--
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 3, 8, "lingtong1", "lingtong1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str347[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {9},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 10, nNum = 1 ,		--
			tbPrelock = {10},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 11, "zisha2", "zisha2"},
			{Instance.ADD_NPC, 7, 1, 11, "mubiao2", "mubiao2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str346[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "mubiao2"},
			{Instance.DEL_NPC, "zisha2"},
			},
		},
		[12] = {nTime = 0, nNum = 3 ,		--
			tbPrelock = {11},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 3, 12, "lingtong2", "lingtong2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str347[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[13] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[14] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {13},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 10, nNum = 1 ,		--
			tbPrelock = {14},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 15, "zisha3", "zisha3"},
			{Instance.ADD_NPC, 7, 1, 15, "mubiao3", "mubiao3"},
			{Instance.TARGET_INFO, -1, Lang.mission.str346[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "mubiao3"},
			{Instance.DEL_NPC, "zisha3"},
			},
		},
		[16] = {nTime = 0, nNum = 1 ,		--
			tbPrelock = {15},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 8, 1, 16, "boss", "boss"},
			{Instance.TARGET_INFO, -1, Lang.mission.str347[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[17] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {16},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[18] = {nTime = 0, nNum = 0 ,		--
			tbPrelock = {17},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[19] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {16},
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
		[20] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
             {Instance.REVIVE_INFO,  true, true, false},
			},
		},

    }
}
