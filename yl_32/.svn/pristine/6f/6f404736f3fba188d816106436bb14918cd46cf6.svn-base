-- 伐魔塔第五大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;


TowerSetting[41]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 240,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 840, nLevel = -1, nSeries = -1},		-- 百年老妖
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
				{Tower.TARGET_INFO, Lang.mission.str1033[Lang.Idx], 2}, --层数编号
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


TowerSetting[42]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 8,
         tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 841, nLevel = -1, nSeries = -1},		-- 鼓灵童
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
				{Tower.TARGET_INFO, Lang.mission.str1034[Lang.Idx], 2}, --层数编号
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

TowerSetting[43]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 842, nLevel = -1, nSeries = -1},		-- 螃蟹精
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
				{Tower.TARGET_INFO, Lang.mission.str1035[Lang.Idx], 2}, --层数编号
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

TowerSetting[44]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 843, nLevel = -1, nSeries = -1},		-- 僵尸兵
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
				{Tower.TARGET_INFO, Lang.mission.str1036[Lang.Idx], 2}, --层数编号
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


TowerSetting[45]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 844, nLevel = -1, nSeries = -1},		-- [精英]瘟癀魔将
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
				{Tower.TARGET_INFO, Lang.mission.str1037[Lang.Idx], 2}, --层数编号
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


TowerSetting[46]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 8,
   tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 845, nLevel = -1, nSeries = -1},		-- 大眼猴
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
				{Tower.TARGET_INFO, Lang.mission.str1038[Lang.Idx], 2}, --层数编号
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


TowerSetting[47]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 8,
     tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 846, nLevel = -1, nSeries = -1},		-- 怨灵
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
				{Tower.TARGET_INFO, Lang.mission.str1039[Lang.Idx], 2}, --层数编号
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



TowerSetting[48]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 8,
     tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 847, nLevel = -1, nSeries = -1},		-- 变种鳄龟
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
				{Tower.TARGET_INFO, Lang.mission.str1040[Lang.Idx], 2}, --层数编号
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


TowerSetting[49]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 848, nLevel = -1, nSeries = -1},		-- 激素熊精
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
				{Tower.TARGET_INFO, Lang.mission.str1041[Lang.Idx], 2}, --层数编号
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


TowerSetting[50]=
{
	nTimeLimit = 180,
	nMapId = 240,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 849, nLevel = -1, nSeries = -1},		-- [BOSS]大教主
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
				{Tower.TARGET_INFO, Lang.mission.str1042[Lang.Idx], 2}, --层数编号
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
