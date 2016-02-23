-- 伐魔塔第11大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;


TowerSetting[101]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 246,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8401, nLevel = -1, nSeries = -1},		-- 百年老妖
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1277[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --百年老妖
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--百年老妖
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --百年老妖
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --百年老妖
},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}


TowerSetting[102]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 8,
         tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8411, nLevel = -1, nSeries = -1},		-- 鼓灵童
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1278[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --鼓灵童
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--鼓灵童
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --鼓灵童
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --鼓灵童
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}

TowerSetting[103]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8421, nLevel = -1, nSeries = -1},		-- 螃蟹精
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1279[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --螃蟹精
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--螃蟹精
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --螃蟹精
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --螃蟹精
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}

TowerSetting[104]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8431, nLevel = -1, nSeries = -1},		-- 僵尸兵
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1280[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --僵尸兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--僵尸兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --僵尸兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --僵尸兵
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}


TowerSetting[105]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8441, nLevel = -1, nSeries = -1},		-- [精英]瘟癀魔将
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1281[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 1, 2, "1", "all"},     --[精英]瘟癀魔将
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}


TowerSetting[106]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 8,
   tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8451, nLevel = -1, nSeries = -1},		-- 大眼猴
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1282[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --大眼猴
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--大眼猴
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --大眼猴
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --大眼猴
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}


TowerSetting[107]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 8,
     tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8461, nLevel = -1, nSeries = -1},		-- 怨灵
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1283[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --怨灵
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--怨灵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --怨灵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --怨灵
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}



TowerSetting[108]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 8,
     tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8471, nLevel = -1, nSeries = -1},		-- 变种鳄龟
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1284[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --变种鳄龟
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--变种鳄龟
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --变种鳄龟
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --变种鳄龟
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}


TowerSetting[109]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8481, nLevel = -1, nSeries = -1},		-- 激素熊精
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1285[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --激素熊精
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--激素熊精
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --激素熊精
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --激素熊精
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}


TowerSetting[110]=
{
	nTimeLimit = 180,
	nMapId = 246,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8491, nLevel = -1, nSeries = -1},		-- [BOSS]大教主
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx],1},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1 ,		-- 锁通过的条件（时间倒计时，杀怪个数）
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1286[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 1, 2, "1", "all"},     --[BOSS]大教主
			},
			tbUnLockEvent =
			{
			},
		},

		[3] = {nTime = 3, nNum = 0 ,		-- 总计时
			tbPrelock = {2},
			tbStartEvent =
			{
				{Tower.TARGET_INFO, Lang.mission.str991[Lang.Idx]},
				{Tower.TIME_INFO, Lang.mission.str992[Lang.Idx], 3},
			},
			tbUnLockEvent =
			{
				{Tower.DO_SCRIPT, "self.bFinishLayer = 1"},
			},
		}
    }
}
