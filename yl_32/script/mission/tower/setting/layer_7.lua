-- 伐魔塔第7大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;

TowerSetting[61]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 8001, nLevel = -1, nSeries = -1},		-- 小鬼
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
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
				{Tower.TARGET_INFO, Lang.mission.str1237[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftshang"},     --魔兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "leftxia"},  		--魔兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightshang"},    --魔兵
				{Tower.ADD_NPC, 1, 2, 2, "1", "rightxia"},      --魔兵
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


TowerSetting[62]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8011, nLevel = -1, nSeries = -1},		-- 小鬼
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
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftshang"},     --白脸鬼
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftxia"},  		--白脸鬼
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightshang"},    --白脸鬼
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightxia"},      --白脸鬼
				
				{Tower.TARGET_INFO, Lang.mission.str1238[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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

TowerSetting[63]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8021, nLevel = -1, nSeries = -1},		-- 小鬼
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
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftshang"},     --黑脸鬼
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftxia"},  		--黑脸鬼
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightshang"},    --黑脸鬼
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightxia"},      --黑脸鬼
				
				{Tower.TARGET_INFO, Lang.mission.str1239[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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

TowerSetting[64]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8031, nLevel = -1, nSeries = -1},		-- 小鬼
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
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftshang"},     --鱼怪
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftxia"},  		--鱼怪
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightshang"},    --鱼怪
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightxia"},      --鱼怪
				
				{Tower.TARGET_INFO, Lang.mission.str1240[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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


TowerSetting[65]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8041, nLevel = -1, nSeries = -1},	-- 怪盗小李	
        [2] = {nTemplate = 8031, nLevel = -1, nSeries = -1},  -- 小鬼

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
		[2] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 1, 2, "2", "all"},     --小李boss	
				{Tower.TARGET_INFO, Lang.mission.str1241[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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


TowerSetting[66]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8051, nLevel = -1, nSeries = -1},		-- 河妖
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
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftshang"},     --河妖
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftxia"},  		--河妖
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightshang"},    --河妖
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightxia"},      --河妖
				
				{Tower.TARGET_INFO, Lang.mission.str1242[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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


TowerSetting[67]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8061, nLevel = -1, nSeries = -1},		-- 小鬼
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
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftshang"},     --恶霸
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftxia"},  		--恶霸
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightshang"},    --恶霸
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightxia"},      --恶霸
				
				{Tower.TARGET_INFO, Lang.mission.str1243[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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



TowerSetting[68]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8071, nLevel = -1, nSeries = -1},		-- 小鬼
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
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftshang"},     --狂暴者
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftxia"},  		--狂暴者
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightshang"},    --狂暴者
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightxia"},      --狂暴者
				
				{Tower.TARGET_INFO, Lang.mission.str1244[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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


TowerSetting[69]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8081, nLevel = -1, nSeries = -1},		-- 小鬼
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
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftshang"},     --强盗
				{Tower.ADD_NPC, 1, 2, 2, "2", "leftxia"},  		--强盗
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightshang"},    --强盗
				{Tower.ADD_NPC, 1, 2, 2, "2", "rightxia"},      --强盗
				
				{Tower.TARGET_INFO, Lang.mission.str1245[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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


TowerSetting[70]=
{
	nTimeLimit = 180,
	nMapId = 242,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8091, nLevel = -1, nSeries = -1},		-- 邪恶大土豪
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
		[2] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 1, 2, "2", "all"},     --邪恶大土豪	
				{Tower.TARGET_INFO, Lang.mission.str1246[Lang.Idx], 2},
				{Tower.TIME_INFO, Lang.mission.str990[Lang.Idx]},
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