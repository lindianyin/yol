-- 伐魔塔第12大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;


TowerSetting[111]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 247,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8501, nLevel = -1, nSeries = -1},		-- 蟾蜍
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
				{Tower.TARGET_INFO, Lang.mission.str1287[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --蟾蜍
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--蟾蜍
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --蟾蜍
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --蟾蜍
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


TowerSetting[112]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 8,
         tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8511, nLevel = -1, nSeries = -1},		-- 狂暴鱼人
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
				{Tower.TARGET_INFO, Lang.mission.str1288[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --狂暴鱼人
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--狂暴鱼人
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --狂暴鱼人
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --狂暴鱼人
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

TowerSetting[113]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8521, nLevel = -1, nSeries = -1},		-- 毒虫
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
				{Tower.TARGET_INFO, Lang.mission.str1289[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --毒虫
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--毒虫
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --毒虫
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --毒虫
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

TowerSetting[114]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8531, nLevel = -1, nSeries = -1},		-- 牛兵
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
				{Tower.TARGET_INFO, Lang.mission.str1290[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --牛兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--牛兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --牛兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --牛兵
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


TowerSetting[115]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8541, nLevel = -1, nSeries = -1},		-- [精英]鞭妖
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
				{Tower.TARGET_INFO, Lang.mission.str1291[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 1, 2, "1", "all"},     --[精英]鞭妖
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


TowerSetting[116]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 8,
   tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8551, nLevel = -1, nSeries = -1},		-- 空虚公子
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
				{Tower.TARGET_INFO, Lang.mission.str1292[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --空虚公子
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--空虚公子
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --空虚公子
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --空虚公子
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


TowerSetting[117]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 8,
     tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8561, nLevel = -1, nSeries = -1},		-- 天残脚
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
				{Tower.TARGET_INFO, Lang.mission.str1293[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --天残脚
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--天残脚
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --天残脚
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --天残脚
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



TowerSetting[118]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 8,
     tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8571, nLevel = -1, nSeries = -1},		--冰玉米
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
				{Tower.TARGET_INFO, Lang.mission.str1294[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --冰玉米
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--冰玉米
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --冰玉米
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --冰玉米
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


TowerSetting[119]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8581, nLevel = -1, nSeries = -1},		-- 火玉米
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
				{Tower.TARGET_INFO, Lang.mission.str1295[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --火玉米
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--火玉米
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --火玉米
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --火玉米
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


TowerSetting[120]=
{
	nTimeLimit = 180,
	nMapId = 247,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8591, nLevel = -1, nSeries = -1},		-- [BOSS]傀儡
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		--总计时
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
				{Tower.TARGET_INFO, Lang.mission.str1296[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 1, 2, "1", "all"},     --[BOSS]傀儡
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
				{Tower.DO_SCRIPT, "self.bAllFinish = 1"},
			},
		}
    }
}
