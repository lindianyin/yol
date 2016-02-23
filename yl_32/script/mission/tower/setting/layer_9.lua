-- 伐魔塔第9大层配置文件

Include("script/mission/tower/tower_def.lua")
if not Tower.tbSetting then
	Tower.tbSetting = {}
end

local TowerSetting = Tower.tbSetting;


TowerSetting[81]=
{
	nTimeLimit = 180,  --本层时间5分钟
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8201, nLevel = -1, nSeries = -1},		-- 黑老鸦
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
				{Tower.TARGET_INFO, Lang.mission.str1257[Lang.Idx], 2}, --层数编号
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --黑老鸦
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


TowerSetting[82]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8211, nLevel = -1, nSeries = -1},		-- 鸟人
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
				{Tower.TARGET_INFO, Lang.mission.str1258[Lang.Idx], 2},
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

TowerSetting[83]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8221, nLevel = -1, nSeries = -1},		-- 猕猴
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
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --猕猴
				{Tower.TARGET_INFO, Lang.mission.str1259[Lang.Idx], 2},
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

TowerSetting[84]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8231, nLevel = -1, nSeries = -1},		-- 江湖人士
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
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --江湖人士
				{Tower.TARGET_INFO, Lang.mission.str1260[Lang.Idx], 2},
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


TowerSetting[85]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8241, nLevel = -1, nSeries = -1},		-- 大巨人
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
				{Tower.ADD_NPC, 1, 1, 2, "1", "monster"}, --大巨人
				{Tower.TARGET_INFO, Lang.mission.str1261[Lang.Idx], 2},
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


TowerSetting[86]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},

    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8251, nLevel = -1, nSeries = -1},		-- 毒箭兵
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
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --毒箭兵
				{Tower.TARGET_INFO, Lang.mission.str1262[Lang.Idx], 2},
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


TowerSetting[87]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8261, nLevel = -1, nSeries = -1},		-- 熔岩怪
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
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --熔岩怪
				{Tower.TARGET_INFO, Lang.mission.str1263[Lang.Idx], 2},
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



TowerSetting[88]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8271, nLevel = -1, nSeries = -1},		-- 鬼魂
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
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --鬼魂
				{Tower.TARGET_INFO, Lang.mission.str1264[Lang.Idx], 2},
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


TowerSetting[89]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8281, nLevel = -1, nSeries = -1},		-- 食人花
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
				{Tower.ADD_NPC, 1, 8, 2, "1", "monster"}, --食人花
				{Tower.TARGET_INFO, Lang.mission.str1265[Lang.Idx], 2},
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


TowerSetting[90]=
{
	nTimeLimit = 180,
	nMapId = 244,
    tbBeginPos = {1831, 312, 1048576},


    -- 副本涉及的NPC种类
	NPC =
	{
-- 		编号  	npc模板				等级(-1默认)	5行(默认-1)
        [1] = {nTemplate = 8291, nLevel = -1, nSeries = -1},		-- 右封印将军
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
				{Tower.ADD_NPC, 1, 1, 2, "1", "monster"}, --右封印将军
				{Tower.TARGET_INFO, Lang.mission.str1266[Lang.Idx], 2},
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
