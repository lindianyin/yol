Include("script/mission/instance/instance_def.lua")
if not Instance.tbSetting then
	Instance.tbSetting = {}
end

local InstanceSetting = Instance.tbSetting;

--东海海边
InstanceSetting[132]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 132;
    tbBeginPos = {3886,2321,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 630, nLevel = 40, nSeries = -1},		-- 螃蟹精
	  [2] = {nTemplate = 631, nLevel = 40, nSeries = -1},		-- 大螃蟹精
	  [3] = {nTemplate = 632, nLevel = -1, nSeries = -1},		-- 精卫(开打)
	  [4] = {nTemplate = 633, nLevel = -1, nSeries = -1},		-- 精卫（剧情）
	  [5] = {nTemplate = 634, nLevel = -1, nSeries = -1},		-- 自杀
	  [6] = {nTemplate = 635, nLevel = -1, nSeries = -1},		-- 透明目标
	  [7] = {nTemplate = 636, nLevel = -1, nSeries = -1},		-- 螃蟹（剧情）
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
		[2] = {nTime = 7, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 2, "jingwei", "jingwei"},
			{Instance.NPC_BUBBLE, -1,"jingwei",Lang.mission.str354[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"jingwei",Lang.mission.str361[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str313[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 19 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 3, "xiejing1", "xiejing1"},
			{Instance.ADD_NPC, 1, 18, 3, "xiejing", "xiejing"},
			{Instance.NPC_BUBBLE, -1,"xiejing1",Lang.mission.str355[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str356[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "jingwei"},
			},
		},
		[4] = {nTime = 7, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 4, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 4, 1, 4, "jingwei1", "jingwei1"},
			{Instance.ADD_NPC, 7, 1, 4, "xiejing11", "xiejing11"},
			{Instance.NPC_BUBBLE, -1,"xiejing11",Lang.mission.str357[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"jingwei1",Lang.mission.str358[Lang.Idx], 3},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "jingwei1"},
			{Instance.DEL_NPC, "xiejing11"},
			},
		},
		[5] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 2, 5, "xiejing2", "xiejing2"},
			{Instance.ADD_NPC, 2, 1, 5, "xiejing22", "xiejing22"},
			{Instance.NPC_BUBBLE, -1,"xiejing22",Lang.mission.str359[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str360[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 132, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[7]:Close()"},
			},
		},
		[7] = {nTime = 0, nNum = 1,		-- 总计时
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

--仙灵草地
InstanceSetting[133]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 133;
    tbBeginPos = {2449,4208,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 640, nLevel = 41, nSeries = -1},		-- 仙草灵
	  [2] = {nTemplate = 641, nLevel = 41, nSeries = -1},		-- 大仙草灵
	  [3] = {nTemplate = 642, nLevel = -1, nSeries = -1},		-- 魔王(剧情)
	  [4] = {nTemplate = 643, nLevel = -1, nSeries = -1},		-- 仙灵草（剧情）
	  [5] = {nTemplate = 644, nLevel = -1, nSeries = -1},		-- 自杀
	  [6] = {nTemplate = 645, nLevel = -1, nSeries = -1},		-- 透明目标

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
		[2] = {nTime = 0, nNum = 16 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 2, "mowang1", "mowang1"},
			{Instance.ADD_NPC, 1, 16, 2, "ling", "ling"},
			--{Instance.NPC_BUBBLE, -1,"ling","小妞！陪我们玩会吧！", 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str362[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "mowang1"},
			},
		},
		[3] = {nTime = 0, nNum = 3 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 3, 3, "ling1", "ling1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str363[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 10, nNum = 1 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 4, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 4, 1, 5, "cao", "cao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str363[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 5, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 5, "mowang", "mowang"},
			{Instance.NPC_BUBBLE, -1,"mowang",Lang.mission.str364[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "cao"},
			{Instance.DEL_NPC, "mowang"},
			},
		},
		[6] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 133, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[7]:Close()"},
			},
		},
		[7] = {nTime = 0, nNum = 1,		-- 总计时
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

--恶魔谷1
InstanceSetting[134]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 134;
    tbBeginPos = {3173,560,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 650, nLevel = 42, nSeries = -1},		-- 僵尸兵
	  [2] = {nTemplate = 651, nLevel = 42, nSeries = -1},		-- 精英僵尸兵
	  [3] = {nTemplate = 652, nLevel = -1, nSeries = -1},		-- 僵尸兵(剧情)
	  [4] = {nTemplate = 653, nLevel = -1, nSeries = -1},		-- 自杀
	  [5] = {nTemplate = 654, nLevel = -1, nSeries = -1},		-- 透明目标
	  [6] = {nTemplate = 655, nLevel = -1, nSeries = -1},		-- 仙灵草

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
		[2] = {nTime = 0, nNum = 25 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 2, "jiangshi11", "jiangshi11"},
			{Instance.ADD_NPC, 1, 7, 2, "jiangshi", "jiangshi"},
			{Instance.ADD_NPC, 1, 9, 2, "jiangshi1", "jiangshi1"},
			{Instance.ADD_NPC, 1, 8, 2, "jiangshi2", "jiangshi2"},
			{Instance.NPC_BUBBLE, -1,"jiangshi11",Lang.mission.str365[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str366[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 3, "jingying", "jingying"},
			{Instance.NPC_BUBBLE, -1,"jingying",Lang.mission.str367[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str368[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "mowang1"},
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 4, "cao", "cao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str369[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 134, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[6]:Close()"},
			},
		},
		[6] = {nTime = 0, nNum = 1,		-- 总计时
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

--恶魔谷2
InstanceSetting[135]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 135;
    tbBeginPos = {3173,560,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 660, nLevel = 42, nSeries = -1},		-- 僵尸兵
	  [2] = {nTemplate = 661, nLevel = -1, nSeries = -1},		-- 僵尸精英
	  [3] = {nTemplate = 662, nLevel = -1, nSeries = -1},		-- 陨石(剧情)
	  [4] = {nTemplate = 663, nLevel = 43, nSeries = -1},		-- 魔王

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
		[2] = {nTime = 0, nNum = 25 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 2, "jiangshi11", "jiangshi11"},
			{Instance.ADD_NPC, 1, 7, 2, "jiangshi", "jiangshi"},
			{Instance.ADD_NPC, 1, 9, 2, "jiangshi1", "jiangshi1"},
			{Instance.ADD_NPC, 1, 8, 2, "jiangshi2", "jiangshi2"},
			{Instance.NPC_BUBBLE, -1,"jiangshi11",Lang.mission.str370[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str371[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 3, "yunshi", "boss"},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 4, "boss", "boss"},
			{Instance.NPC_BUBBLE, -1,"boss",Lang.mission.str372[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str373[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{Instance.DEL_NPC, "mowang1"},
			},
		},
		[5] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 135, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[6]:Close()"},
			},
		},
		[6] = {nTime = 0, nNum = 1,		-- 总计时
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

--受污泉眼1
InstanceSetting[136]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 136;
    tbBeginPos = {2189,1482,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 670, nLevel = -1, nSeries = -1},		-- 变形水妖
	  [2] = {nTemplate = 671, nLevel = -1, nSeries = -1},		-- 大变形水妖
	  [3] = {nTemplate = 672, nLevel = 44, nSeries = -1},		-- 二郎神
	  [4] = {nTemplate = 673, nLevel = 44, nSeries = -1},		-- 哮天犬
	  [5] = {nTemplate = 674, nLevel = 44, nSeries = -1},		-- 孙悟空
	  [6] = {nTemplate = 675, nLevel = 44, nSeries = -1},		-- 大眼猴

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
		[2] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 2, "da", "da"},
			{Instance.ADD_NPC, 1, 6, 2, "xiao", "xiao"},
			{Instance.NPC_BUBBLE, -1,"da",Lang.mission.str374[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"da",Lang.mission.str375[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "da"},
			{Instance.DEL_NPC, "xiao"},
			},
		},
		[3] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 3, "els", "els"},
			{Instance.ADD_NPC, 4, 6, 3, "quan", "quan"},
			{Instance.NPC_BUBBLE, -1,"els",Lang.mission.str376[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str377[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 4, "da", "da"},
			{Instance.ADD_NPC, 1, 6, 4, "xiao", "xiao"},
			{Instance.NPC_BUBBLE, -1,"da",Lang.mission.str378[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "da"},
			{Instance.DEL_NPC, "xiao"},
			},
		},
		[5] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 5, "wukong", "wukong"},
			{Instance.ADD_NPC, 6, 6, 5, "hou", "hou"},
			{Instance.NPC_BUBBLE, -1,"wukong",Lang.mission.str379[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str380[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 136, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[7]:Close()"},
			},
		},
		[7] = {nTime = 0, nNum = 1,		-- 总计时
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

--受污泉眼2
InstanceSetting[137]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 137;
    tbBeginPos = {2247,920,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 680, nLevel = -1, nSeries = -1},		-- 法宝
	  [2] = {nTemplate = 681, nLevel = -1, nSeries = -1},		-- 吸怪
	  [3] = {nTemplate = 682, nLevel = -1, nSeries = -1},		-- 巨型水怪（被吸）
	  [4] = {nTemplate = 683, nLevel = -1, nSeries = -1},		-- 巨型水怪（变形）
	  [5] = {nTemplate = 684, nLevel = 45, nSeries = -1},		-- 怨灵
	  [6] = {nTemplate = 685, nLevel = 45, nSeries = -1},		-- 鬼王
	  [7] = {nTemplate = 686, nLevel = 45, nSeries = -1},		-- 阿婆
	  [8] = {nTemplate = 687, nLevel = -1, nSeries = -1},		-- 小变形怪
	  [9] = {nTemplate = 688, nLevel = 44, nSeries = -1},		-- 鬼魂
	  [10] = {nTemplate = 689, nLevel = 44, nSeries = -1},		-- 僵尸兵
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
			{Instance.ADD_NPC, 1, 1, 2, "fabao", "fabao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str381[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 7, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 2, "xiguai", "xiguai"},
			{Instance.ADD_NPC, 3, 1, 2, "da", "da"},
			{Instance.NPC_BUBBLE, -1,"da",Lang.mission.str382[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"da",Lang.mission.str383[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "xiguai"},
			{Instance.DEL_NPC, "da"},
			},
		},
		[4] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 2, "da1", "da1"},
			{Instance.ADD_NPC, 8, 6, 5, "xiao1", "xiao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "da1"},
			{Instance.DEL_NPC, "xiao1"},
			},
		},
		[5] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 5, "yuanling", "yuanling"},
			{Instance.ADD_NPC, 9, 6, 5, "gui", "gui"},
			{Instance.NPC_BUBBLE, -1,"yuanling",Lang.mission.str384[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str385[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 6, "da2", "da1"},
			{Instance.ADD_NPC, 8, 6, 5, "xiao2", "xiao"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "da2"},
			{Instance.DEL_NPC, "xiao2"},

			},
		},
		[7] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 7, "guiwang", "guiwang"},
			{Instance.ADD_NPC, 10, 6, 7, "shi", "shi"},
			{Instance.NPC_BUBBLE, -1,"guiwang",Lang.mission.str386[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str387[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[8] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 8, "da3", "da1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "da3"},
			},
		},
		[9] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 9, "apo", "apo"},
			{Instance.NPC_BUBBLE, -1,"apo","what..the....", 1},
			{Instance.NPC_BUBBLE, -1,"apo",Lang.mission.str388[Lang.Idx], 3},
			{Instance.TARGET_INFO, -1, Lang.mission.str389[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[10] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 137, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[11]:Close()"},
			},
		},
		[11] = {nTime = 0, nNum = 1,		-- 总计时
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

--炎热之地
InstanceSetting[138]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 138;
    tbBeginPos = {7223,4021,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 690, nLevel = -1, nSeries = -1},		-- 熔岩刀将（剧情）
	  [2] = {nTemplate = 691, nLevel = -1, nSeries = -1},		-- 徐福
	  [3] = {nTemplate = 692, nLevel = 46, nSeries = -1},		-- 熔岩魔兵(开打)
	  [4] = {nTemplate = 693, nLevel = -1, nSeries = -1},		-- 熔岩魔兵（傻站?
	  [5] = {nTemplate = 694, nLevel = -1, nSeries = -1},		-- 陨石
	  [6] = {nTemplate = 695, nLevel = 46, nSeries = -1},		-- 熔岩刀将

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
		[2] = {nTime = 5, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 2, "xufu", "xufu"},
			{Instance.ADD_NPC, 4, 6, 2, "rongyan1", "rongyan1"},
			{Instance.ADD_NPC, 4, 6, 2, "rongyan2", "rongyan2"},
			{Instance.ADD_NPC, 4, 6, 2, "rongyan3", "rongyan3"},
			{Instance.NPC_BUBBLE, -1,"xufu",Lang.mission.str390[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 6 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "rongyan1"},
			{Instance.ADD_NPC, 3, 6, 3, "rongyan11", "rongyan1"},
			{Instance.TARGET_INFO, -1, Lang.mission.str391[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 6 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "rongyan2"},
			{Instance.ADD_NPC, 3, 6, 4, "rongyan22", "rongyan2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str391[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 6 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.DEL_NPC, "rongyan3"},
			{Instance.ADD_NPC, 3, 6, 5, "rongyan33", "rongyan3"},
			{Instance.TARGET_INFO, -1, Lang.mission.str391[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 3, "yunshi1", "yunshi1"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 4, 1, 3, "juqing1", "juqing1"},
			},
		},
		[7] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 3, "yunshi2", "yunshi2"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 4, 1, 3, "juqing2", "juqing2"},
			},
		},
		[8] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 3, "yunshi3", "yunshi3"},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 1, 1, 3, "juqing3", "juqing3"},
			},
		},
		[9] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.NPC_BUBBLE, -1,"juqing3",Lang.mission.str392[Lang.Idx], 1},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "juqing1"},
			{Instance.DEL_NPC, "juqing2"},
			{Instance.DEL_NPC, "juqing3"},
			{Instance.DEL_NPC, "xufu"},
			},
		},
		[10] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {9},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 1, 10, "jingying", "jingying"},
			{Instance.NPC_BUBBLE, -1,"jingying",Lang.mission.str393[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str394[Lang.Idx]},
			},
			tbUnLockEvent =
			{

			},
		},
		[11] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {10},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 138, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[12]:Close()"},
			},
		},
		[12] = {nTime = 0, nNum = 1,		-- 总计时
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

--炎热之路
InstanceSetting[139]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 139;
    tbBeginPos = {6724,1315,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 700, nLevel = -1, nSeries = -1},		-- 徐福（跑）
	  [2] = {nTemplate = 701, nLevel = -1, nSeries = -1},		-- 熔岩魔兵（跑）
	  [3] = {nTemplate = 702, nLevel = -1, nSeries = -1},		-- 熔岩魔兵（跑?
	  [4] = {nTemplate = 703, nLevel = -1, nSeries = -1},		-- 箭头
	  [5] = {nTemplate = 704, nLevel = -1, nSeries = -1},		-- 加速
	  [6] = {nTemplate = 705, nLevel = -1, nSeries = -1},		-- 自杀
	  [7] = {nTemplate = 706, nLevel = -1, nSeries = -1},		-- 目标
	  [8] = {nTemplate = 707, nLevel = -1, nSeries = -1},		-- 陨石
	  [9] = {nTemplate = 708, nLevel = 47, nSeries = -1},		-- 熔岩将军
	  [10] = {nTemplate = 709, nLevel = 46, nSeries = -1},		-- 熔岩魔兵


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
		[2] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 2, "xufu", "xufu"},
			{Instance.ADD_NPC, 2, 1, 2, "rongyan1", "rongyan1"},
			{Instance.ADD_NPC, 3, 1, 2, "rongyan2", "rongyan2"},
			{Instance.ADD_NPC, 4, 6, 2, "jiantou", "jiantou"},
			{Instance.ADD_NPC, 4, 2, 2, "jiantou1", "jiantou1"},
			{Instance.NPC_BUBBLE, -1,"xufu",Lang.mission.str395[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"xufu",Lang.mission.str396[Lang.Idx], 10},
			{Instance.TARGET_INFO, -1, Lang.mission.str397[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.ADD_NPC, 5, 4, 2, "jiasu", "jiasu"},
			{Instance.ADD_NPC, 8, 1, 2, "yunshi", "yunshi", 50, 4,"yunshi_timer"},
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 3, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 6, 3, 3, "zisha", "zisha"},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 4, "mubiao2", "mubiao2"},
			{Instance.ADD_NPC, 6, 3, 4, "zisha2", "zisha2"},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 1, 6, "mubiao22", "mubiao2"},
			{Instance.ADD_NPC, 6, 3, 6, "zisha22", "zisha2"},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 7, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.NPC_BUBBLE, -1,"xufu",Lang.mission.str398[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"xufu",Lang.mission.str399[Lang.Idx], 4},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "rongyan1"},
			{Instance.DEL_NPC, "rongyan2"},
			{Instance.DEL_NPC, "xufu"},
			{Instance.DEL_NPC, "jiantou"},
			{Instance.DEL_NPC, "jiantou1"},
			{Instance.DEL_NPC, "jiasu"},
			{Instance.DEL_NPC, "yunshi","yunshi_timer"},
			},
		},
		[8] = {nTime = 0, nNum = 9 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 9, 1, 8, "boss", "boss"},
			{Instance.ADD_NPC, 10, 8, 8, "xiaobing", "xiaobing"},
			{Instance.NPC_BUBBLE, -1,"boss",Lang.mission.str400[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str401[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 1, nNum = 0,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 139, 1},
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

--酒仙泉
InstanceSetting[140]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 140;
    tbBeginPos = {2935,1553,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 710, nLevel = 50, nSeries = -1},		-- 喜酒蝇
	  [2] = {nTemplate = 711, nLevel = 50, nSeries = -1},		-- 喜酒蛙
	  [3] = {nTemplate = 712, nLevel = 50, nSeries = -1},		-- 喜酒巨蝇
	  [4] = {nTemplate = 713, nLevel = 50, nSeries = -1},		-- 喜酒巨蛙
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
		[2] = {nTime = 0, nNum = 11 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 10, 2, "chong", "chong"},
			{Instance.ADD_NPC, 1, 1, 2, "chong1", "chong1"},
			{Instance.NPC_BUBBLE, -1,"chong1",Lang.mission.str402[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str403[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 13 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 12, 3, "wa", "wa"},
			{Instance.ADD_NPC, 2, 1, 3, "wa1", "wa1"},
			{Instance.NPC_BUBBLE, -1,"wa1",Lang.mission.str404[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str403[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 13 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 5, 4, "wa2", "wa2"},
			{Instance.ADD_NPC, 1, 6, 4, "chong2", "chong2"},
			{Instance.ADD_NPC, 3, 1, 4, "jingying1", "jingying1"},
			{Instance.ADD_NPC, 4, 1, 4, "jingying2", "jingying2"},
			{Instance.TARGET_INFO, -1, Lang.mission.str403[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 140, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[6]:Close()"},
			},
		},
		[6] = {nTime = 0, nNum = 1,		-- 总计时
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

--神秘营地
InstanceSetting[141]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 141;
    tbBeginPos = {3443,1499,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 720, nLevel = 51, nSeries = -1},		-- 强盗
	  [2] = {nTemplate = 721, nLevel = 51, nSeries = -1},		-- 强盗
	  [3] = {nTemplate = 722, nLevel = 51, nSeries = -1},		-- 打强盗
	  [4] = {nTemplate = 723, nLevel = 51, nSeries = -1},		-- boss
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
		[2] = {nTime = 0, nNum = 20 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 2, "qiangdao1", "qiangdao1"},
			{Instance.ADD_NPC, 1, 7, 2, "qiangdao", "qiangdao"},
			{Instance.ADD_NPC, 2, 12, 2, "qiangdao2", "qiangdao2"},
			{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str406[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 13 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 6, 3, "qiangdao3", "qiangdao3", 2, 2, "qiangdao_timer", 0},
			{Instance.ADD_NPC, 3, 1, 3, "jingying", "jingying"},
			{Instance.NPC_BUBBLE, -1,"jingying",Lang.mission.str407[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str406[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 0, nNum = 7 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 6, 4, "jingying1", "jingying1"},
			{Instance.ADD_NPC, 3, 1, 4, "jingying", "jingying"},
			{Instance.NPC_BUBBLE, -1,"jingying",Lang.mission.str407[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str406[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[5] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 5, "boss", "boss"},
			{Instance.NPC_BUBBLE, -1,"boss",Lang.mission.str408[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str406[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 141, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[7]:Close()"},
			},
		},
		[7] = {nTime = 0, nNum = 1,		-- 总计时
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

--机关城1层
InstanceSetting[142]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 142;
    tbBeginPos = {2865,682,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 727, nLevel = 52, nSeries = -1},		-- 机关人
	  [2] = {nTemplate = 728, nLevel = 52, nSeries = -1},		-- 机关兽
	  [3] = {nTemplate = 729, nLevel = 52, nSeries = -1},		-- 超级机关兽
	  [4] = {nTemplate = 730, nLevel = -1, nSeries = -1},		-- 机关
	  [5] = {nTemplate = 731, nLevel = 52, nSeries = -1},		-- 火球1
	  [6] = {nTemplate = 732, nLevel = 52, nSeries = -1},		-- 火球2
	  [7] = {nTemplate = 733, nLevel = 52, nSeries = -1},		-- 火球3
	  [8] = {nTemplate = 734, nLevel = 52, nSeries = -1},		-- 火球4
	  [9] = {nTemplate = 735, nLevel = 52, nSeries = -1},		-- 火球5
	  [10] = {nTemplate = 736, nLevel = 52, nSeries = -1},		-- 火球6
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
		[2] = {nTime = 0, nNum = 18 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 7, 2, "jiguanren", "jiguanren"},
			{Instance.ADD_NPC, 1, 7, 2, "jiguanren1", "jiguanren1"},
			{Instance.ADD_NPC, 1, 4, 2, "jiguanren2", "jiguanren2"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 3, "jiguan1", "jiguan1"},
			{Instance.ADD_NPC, 5, 1, 2, "huoqiu1", "huoqiu1", 100, 5,"huoqiu1_timer"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "huoqiu1","huoqiu1_timer"},
			},
		},
		[4] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 4, "jiguan2", "jiguan2"},
			{Instance.ADD_NPC, 6, 1, 2, "huoqiu2", "huoqiu2", 100, 5,"huoqiu2_timer"},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "huoqiu2","huoqiu2_timer"},
			},
		},
		[5] = {nTime = 0, nNum = 23 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 11, 5, "jiguanren3", "jiguanren3"},
			{Instance.ADD_NPC, 1, 9, 5, "jiguanren4", "jiguanren4"},
			{Instance.ADD_NPC, 2, 3, 5, "jingying1", "jingying1"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 6, "jiguan3", "jiguan3"},
			{Instance.ADD_NPC, 7, 1, 2, "huoqiu3", "huoqiu3", 100, 4,"huoqiu3_timer"},
			{Instance.ADD_NPC, 8, 1, 2, "huoqiu4", "huoqiu4", 100, 4,"huoqiu4_timer"},
			{Instance.ADD_NPC, 9, 1, 2, "huoqiu5", "huoqiu5", 100, 4,"huoqiu5_timer"},
			{Instance.ADD_NPC, 10, 1, 2, "huoqiu6", "huoqiu6", 100, 4,"huoqiu6_timer"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "huoqiu3","huoqiu3_timer"},
			{Instance.DEL_NPC, "huoqiu4","huoqiu4_timer"},
			{Instance.DEL_NPC, "huoqiu5","huoqiu5_timer"},
			{Instance.DEL_NPC, "huoqiu6","huoqiu6_timer"},
			},
		},
		[7] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 142, 1},
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

--机关城2层
InstanceSetting[143]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 143;
    tbBeginPos = {2865,682,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 740, nLevel = 53, nSeries = -1},		-- 机关人
	  [2] = {nTemplate = 741, nLevel = 53, nSeries = -1},		-- 机关兽
	  [3] = {nTemplate = 742, nLevel = 53, nSeries = -1},		-- 机关邪童
	  [4] = {nTemplate = 743, nLevel = -1, nSeries = -1},		-- 机关
	  [5] = {nTemplate = 744, nLevel = 53, nSeries = -1},		-- 超级机关兽
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
		[2] = {nTime = 0, nNum = 23 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 3, 2, "jiguanren", "jiguanren"},
			{Instance.ADD_NPC, 1, 5, 2, "jiguanren1", "jiguanren1"},
			{Instance.ADD_NPC, 1, 4, 2, "jiguanren2", "jiguanren2"},
			{Instance.ADD_NPC, 2, 4, 2, "shou", "shou"},
			{Instance.ADD_NPC, 5, 1, 2, "jingying", "jingying"},
			{Instance.ADD_NPC, 2, 5, 2, "shou1", "shou1"},
			{Instance.ADD_NPC, 3, 1, 2, "boss", "boss"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 143, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[4]:Close()"},
			},
		},
		[4] = {nTime = 0, nNum = 1,		-- 总计时
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

--万魔城外
InstanceSetting[144]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 144;
    tbBeginPos = {4606,622,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 750, nLevel = 55, nSeries = -1},		-- 熔岩刀将
	  [2] = {nTemplate = 751, nLevel = 55, nSeries = -1},		-- 魔兵刀将
	  [3] = {nTemplate = 752, nLevel = 55, nSeries = -1},		-- 左魔将军
	  [4] = {nTemplate = 753, nLevel = 55, nSeries = -1},		-- 右魔将军
	  [5] = {nTemplate = 754, nLevel = 55, nSeries = -1},		-- 魔兵弓将
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
		[2] = {nTime = 0, nNum = 25 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 9, 2, "rongyang", "rongyang"},
			{Instance.ADD_NPC, 2, 8, 2, "dao", "dao"},
			{Instance.ADD_NPC, 5, 8, 2, "gong", "gong"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 2 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 3, "jingying1", "jingying1"},
			{Instance.ADD_NPC, 4, 1, 3, "jingying2", "jingying2"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 144, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[5]:Close()"},
			},
		},
		[5] = {nTime = 0, nNum = 1,		-- 总计时
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

--万魔城外2
InstanceSetting[145]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 145;
    tbBeginPos = {4260,4970,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 760, nLevel = 56, nSeries = -1},		-- 幽灵
	  [2] = {nTemplate = 761, nLevel = 56, nSeries = -1},		-- 恶灵
	  [3] = {nTemplate = 762, nLevel = -1, nSeries = -1},		-- 怨灵(剧情)
	  [4] = {nTemplate = 763, nLevel = -1, nSeries = -1},		-- 怨灵（剧情）
	  [5] = {nTemplate = 764, nLevel = 56, nSeries = -1},		-- 徐福
	  [6] = {nTemplate = 765, nLevel = -1, nSeries = -1},		-- 徐福（剧情）
	  [7] = {nTemplate = 766, nLevel = 56, nSeries = -1},		-- 怨灵
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
		[2] = {nTime = 0, nNum = 17 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 9, 2, "youling", "youling"},
			{Instance.ADD_NPC, 2, 8, 2, "eling", "eling"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 8, 3, "eling", "eling"},
			--{Instance.NPC_BUBBLE, -1,"qiangdao1",Lang.mission.str405[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 6, nNum = 0,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 3, 1, 4, "yuanling1", "yuanling1"},
			{Instance.ADD_NPC, 4, 1, 4, "yuanling2", "yuanling2"},
			{Instance.ADD_NPC, 6, 1, 4, "xufu1", "xufu1"},
			{Instance.NPC_BUBBLE, -1,"xufu1",Lang.mission.str410[Lang.Idx], 3},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "yuanling1"},
			{Instance.DEL_NPC, "yuanling2"},
			},
		},
		[5] = {nTime = 0, nNum = 2 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 7, 2, 5, "yuanling", "yuanling"},
			{Instance.NPC_BUBBLE, -1,"xufu1","上！", 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "xufu1"},
			},
		},
		[6] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 6, "xufu", "xufu"},
			{Instance.NPC_BUBBLE, -1,"xufu",Lang.mission.str411[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 145, 1},
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

--万魔城内
InstanceSetting[146]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 146;
    tbBeginPos = {1686,4092,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 770, nLevel = 57, nSeries = -1},		-- 僵尸
	  [2] = {nTemplate = 771, nLevel = 57, nSeries = -1},		-- 老妖
	  [3] = {nTemplate = 772, nLevel = 57, nSeries = -1},		-- 邪童
	  [4] = {nTemplate = 773, nLevel = -1, nSeries = -1},		-- 魔将（剧情）
	  [5] = {nTemplate = 774, nLevel = 57, nSeries = -1},		-- 魔将
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
		[2] = {nTime = 0, nNum = 32 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 12, 2, "jiangshi", "jiangshi"},
			{Instance.ADD_NPC, 2, 9, 2, "laoyao", "laoyao"},
			{Instance.ADD_NPC, 3, 11, 2, "xietong", "xietong"},
			{Instance.TARGET_INFO, -1, Lang.mission.str409[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 5, 1, 3, "boss", "boss"},
			{Instance.NPC_BUBBLE, -1,"boss",Lang.mission.str412[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str413[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[4] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 146, 1},
			},
			tbUnLockEvent =
			{
				{Instance.DO_SCRIPT, "self.bFinishTask = 1"},
				{Instance.TARGET_INFO, -1, Lang.mission.str236[Lang.Idx]},
				{Instance.SHOW_REWARD_PANEL, -1, 0},
			    {Instance.DO_SCRIPT, "self.tbLock[5]:Close()"},
			},
		},
		[5] = {nTime = 0, nNum = 1,		-- 总计时
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

--万魔城顶
InstanceSetting[147]=
{
	nTaskInstance = 1;
    nMinLevel = 6;
    nMaxLevel = 60;
	nMapId = 147;
    tbBeginPos = {2810,392,1048576},
	tbLeavePos = {6,6113,4003,1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
      [1] = {nTemplate = 780, nLevel = 59, nSeries = -1},		-- 大将军
	  [2] = {nTemplate = 781, nLevel = -1, nSeries = -1},		-- 大将军（剧情）
	  [3] = {nTemplate = 782, nLevel = -1, nSeries = -1},		-- 魔兵刀将（剧情）
	  [4] = {nTemplate = 783, nLevel = -1, nSeries = -1},		-- 刀将逃跑
	  [5] = {nTemplate = 784, nLevel = -1, nSeries = -1},		-- 灵魂
	  [6] = {nTemplate = 785, nLevel = 59, nSeries = -1},		-- 魔兵刀将
	  [7] = {nTemplate = 786, nLevel = -1, nSeries = -1},		-- 魔兵刀将（变身大将军）
	  [8] = {nTemplate = 787, nLevel = -1, nSeries = -1},		-- 透明吸人
	  [9] = {nTemplate = 788, nLevel = -1, nSeries = -1},		-- 闪电
	  [10] = {nTemplate = 789, nLevel = -1, nSeries = -1},		-- 出来闪电自杀魔兵
	  [11] = {nTemplate = 790, nLevel = -1, nSeries = -1},		-- 透明目标
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
		[2] = {nTime = 0, nNum = 26 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 6, 26, 2, "bing", "bing"},
			{Instance.TARGET_INFO, -1, Lang.mission.str414[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[3] = {nTime = 6, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 11, 1, 3, "mubiao", "mubiao"},
			{Instance.ADD_NPC, 3, 1, 3, "beixi11", "beixi11"},
			{Instance.ADD_NPC, 3, 7, 3, "beixi1", "beixi1"},
			{Instance.ADD_NPC, 3, 6, 3, "beixi2", "beixi2"},
			{Instance.ADD_NPC, 3, 6, 3, "beixi3", "beixi3"},
			{Instance.ADD_NPC, 7, 1, 3, "bianshen", "bianshen"},
			{Instance.NPC_BUBBLE, -1,"bianshen",Lang.mission.str415[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"beixi11",Lang.mission.str416[Lang.Idx], 4},
			--{Instance.NPC_BUBBLE, -1,"beixi11","快跑！！", 7},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "beixi2"},
			{Instance.DEL_NPC, "beixi3"},
			--{Instance.DEL_NPC, "bianshen"},
			},
		},
		[4] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {3},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 10, 6, 3, "beixi5", "beixi5"},
			{Instance.ADD_NPC, 10, 6, 3, "beixi6", "beixi6"},
			{Instance.ADD_NPC, 5, 12, 3, "linghun", "linghun"},
			{Instance.NPC_BUBBLE, -1,"beixi11",Lang.mission.str417[Lang.Idx], 2},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "beixi11"},
			{Instance.DEL_NPC, "beixi1"},
			},
		},
		[5] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {4},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 4, 1, 3, "beixi44", "beixi44"},
			{Instance.ADD_NPC, 4, 7, 3, "beixi4", "beixi4"},
			--{Instance.ADD_NPC, 7, 1, 3, "bianshen", "bianshen"},
			{Instance.NPC_BUBBLE, -1,"bianshen",Lang.mission.str418[Lang.Idx], 1},
			{Instance.NPC_BUBBLE, -1,"beixi44",Lang.mission.str419[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "beixi44"},
			{Instance.DEL_NPC, "beixi4"},
			},
		},
		[6] = {nTime = 2, nNum = 0 ,		-- 总计时
			tbPrelock = {5},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 8, 1, 3, "xiren", "xiren"},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "linghun"},
			{Instance.DEL_NPC, "xiren"},
			{Instance.DEL_NPC, "bianshen"},
			},
		},
		[7] = {nTime = 4, nNum = 0 ,		-- 总计时
			tbPrelock = {6},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 2, 1, 3, "dajiangjun", "dajiangjun"},
			{Instance.NPC_BUBBLE, -1,"dajiangjun",Lang.mission.str420[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str232[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{Instance.DEL_NPC, "dajiangjun"},
			},
		},
		[8] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {7},
			tbStartEvent =
			{
			{Instance.ADD_NPC, 1, 1, 8, "dajiangjun1", "dajiangjun"},
			{Instance.NPC_BUBBLE, -1,"dajiangjun1",Lang.mission.str421[Lang.Idx], 1},
			{Instance.TARGET_INFO, -1, Lang.mission.str422[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[9] = {nTime = 1, nNum = 0 ,		-- 总计时
			tbPrelock = {8},
			tbStartEvent =
			{
			{Instance.SET_TASK, -1, 1022, 147, 1},
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
