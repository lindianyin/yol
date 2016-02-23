-- 伐魔塔第10大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;


TowerSetting[91]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8301, nLevel = -1, nSeries = -1},		-- 木人
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
				{Tower.TARGET_INFO, Lang.mission.str1267[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --木人
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


TowerSetting[92]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8311, nLevel = -1, nSeries = -1},		-- 公鸡
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --鸟人
				{Tower.TARGET_INFO, Lang.mission.str1268[Lang.Idx], 2},
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

TowerSetting[93]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8321, nLevel = -1, nSeries = -1},		-- 狼
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --狼
				{Tower.TARGET_INFO, Lang.mission.str1269[Lang.Idx], 2},
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

TowerSetting[94]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8331, nLevel = -1, nSeries = -1},		-- 野狐狸
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --野狐狸
				{Tower.TARGET_INFO, Lang.mission.str1270[Lang.Idx], 2},
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


TowerSetting[95]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8341, nLevel = -1, nSeries = -1},		-- [精英]曾雅
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 1, 2, "1", "monster"}, --[精英]曾雅
				{Tower.TARGET_INFO, Lang.mission.str1271[Lang.Idx], 2},
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


TowerSetting[96]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8351, nLevel = -1, nSeries = -1},		-- 野狼
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --野狼
				{Tower.TARGET_INFO, Lang.mission.str1272[Lang.Idx], 2},
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


TowerSetting[97]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8361, nLevel = -1, nSeries = -1},		-- 树妖
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --树妖
				{Tower.TARGET_INFO, Lang.mission.str1273[Lang.Idx], 2},
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



TowerSetting[98]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8371, nLevel = -1, nSeries = -1},		-- 天香羚
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --天香羚
				{Tower.TARGET_INFO, Lang.mission.str1274[Lang.Idx], 2},
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


TowerSetting[99]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8381, nLevel = -1, nSeries = -1},		-- 遗迹魔塔
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 8 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --遗迹魔塔
				{Tower.TARGET_INFO, Lang.mission.str1275[Lang.Idx], 2},
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


TowerSetting[100]=
{
	nTimeLimit = 180,
	nMapId = 245,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8391, nLevel = -1, nSeries = -1},		-- [BOSS]钓鱼翁
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 3, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {Tower.TIME_INFO, Lang.mission.str989[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			},
		},
		[2] = {nTime = 0, nNum = 1 ,		-- 总计时
			tbPrelock = {1},
			tbStartEvent =
			{
				{Tower.ADD_NPC, 1, 1, 2, "1", "monster"}, --[BOSS]钓鱼翁
				{Tower.TARGET_INFO, Lang.mission.str1276[Lang.Idx], 2},
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
