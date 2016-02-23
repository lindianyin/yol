-- 伐魔塔第四大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;


TowerSetting[31]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 830, nLevel = -1, nSeries = -1},		-- 木人
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
				{Tower.TARGET_INFO, Lang.mission.str1023[Lang.Idx], 2}, --层数编号
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


TowerSetting[32]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 831, nLevel = -1, nSeries = -1},		-- 公鸡
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
				{Tower.TARGET_INFO, Lang.mission.str1024[Lang.Idx], 2},
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

TowerSetting[33]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 832, nLevel = -1, nSeries = -1},		-- 狼
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
				{Tower.TARGET_INFO, Lang.mission.str1025[Lang.Idx], 2},
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

TowerSetting[34]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 833, nLevel = -1, nSeries = -1},		-- 野狐狸
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
				{Tower.TARGET_INFO, Lang.mission.str1026[Lang.Idx], 2},
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


TowerSetting[35]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 834, nLevel = -1, nSeries = -1},		-- [精英]曾雅
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
				{Tower.TARGET_INFO, Lang.mission.str1027[Lang.Idx], 2},
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


TowerSetting[36]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 835, nLevel = -1, nSeries = -1},		-- 野狼
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
				{Tower.TARGET_INFO, Lang.mission.str1028[Lang.Idx], 2},
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


TowerSetting[37]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 836, nLevel = -1, nSeries = -1},		-- 树妖
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
				{Tower.TARGET_INFO, Lang.mission.str1029[Lang.Idx], 2},
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



TowerSetting[38]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 837, nLevel = -1, nSeries = -1},		-- 天香羚
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
				{Tower.TARGET_INFO, Lang.mission.str1030[Lang.Idx], 2},
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


TowerSetting[39]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 838, nLevel = -1, nSeries = -1},		-- 遗迹魔塔
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
				{Tower.TARGET_INFO, Lang.mission.str1031[Lang.Idx], 2},
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


TowerSetting[40]=
{
	nTimeLimit = 180,
	nMapId = 239,
    tbBeginPos = {284, 378, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 839, nLevel = -1, nSeries = -1},		-- [BOSS]钓鱼翁
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
				{Tower.TARGET_INFO, Lang.mission.str1032[Lang.Idx], 2},
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
