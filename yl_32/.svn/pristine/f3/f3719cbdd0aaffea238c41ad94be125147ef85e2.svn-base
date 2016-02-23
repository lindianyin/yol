-- 伐魔塔第一大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;

TowerSetting[1]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 800, nLevel = -1, nSeries = -1},		-- 小鬼
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
				{Tower.TARGET_INFO, Lang.mission.str993[Lang.Idx], 2}, --层数编号
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


TowerSetting[2]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 801, nLevel = -1, nSeries = -1},		-- 小鬼
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
				
				{Tower.TARGET_INFO, Lang.mission.str994[Lang.Idx], 2},
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

TowerSetting[3]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 802, nLevel = -1, nSeries = -1},		-- 小鬼
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
				
				{Tower.TARGET_INFO, Lang.mission.str995[Lang.Idx], 2},
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

TowerSetting[4]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 803, nLevel = -1, nSeries = -1},		-- 小鬼
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
				
				{Tower.TARGET_INFO, Lang.mission.str996[Lang.Idx], 2},
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


TowerSetting[5]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 804, nLevel = -1, nSeries = -1},	-- 怪盗小李	
        [2] = {nTemplate = 803, nLevel = -1, nSeries = -1},  -- 小鬼

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
				{Tower.TARGET_INFO, Lang.mission.str997[Lang.Idx], 2},
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


TowerSetting[6]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 805, nLevel = -1, nSeries = -1},		-- 河妖
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
				
				{Tower.TARGET_INFO, Lang.mission.str998[Lang.Idx], 2},
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


TowerSetting[7]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 806, nLevel = -1, nSeries = -1},		-- 小鬼
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
				
				{Tower.TARGET_INFO, Lang.mission.str999[Lang.Idx], 2},
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



TowerSetting[8]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 807, nLevel = -1, nSeries = -1},		-- 小鬼
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
				
				{Tower.TARGET_INFO, Lang.mission.str1000[Lang.Idx], 2},
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


TowerSetting[9]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 8,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 809, nLevel = -1, nSeries = -1},		-- 小鬼
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
				
				{Tower.TARGET_INFO, Lang.mission.str1001[Lang.Idx], 2},
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


TowerSetting[10]=
{
	nTimeLimit = 180,
	nMapId = 236,
	nEnemyNum = 1,
    tbBeginPos = {1078, 1016, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 808, nLevel = -1, nSeries = -1},		-- 邪恶大土豪
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
				{Tower.TARGET_INFO, Lang.mission.str1002[Lang.Idx], 2},
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