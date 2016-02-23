Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[46] =    --瘟癀魔将
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 5,						-- 房间等级(1~5)
	nMapIndex		= 47,						-- 地图组的索引
	tbBeginPoint	= {3326, 1805, 1054704},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8073, nLevel = {8150,8151,8152}, nSeries = -1},		-- 瘟癀魔将
		[2] = {nTemplate = 8074, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟魔兵
		[3] = {nTemplate = 9113, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟魔兵
		[4] = {nTemplate = 9114, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟魔兵
		[5] = {nTemplate = 9115, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟魔兵
		[6] = {nTemplate = 9116, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟魔兵
		[7] = {nTemplate = 9117, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟魔兵
		[8] = {nTemplate = 9118, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟魔兵
		[9] = {nTemplate = 9119, nLevel = {8147,8148,8149}, nSeries = -1},		-- 散瘟亲卫
	},
	-- 锁结构
	LOCK = 
	{
		-- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 15, nNum = 0,
			tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str600[Lang.Idx], 1},
				{XoyoGame.TARGET_INFO, -1, ""},
			},
			tbUnLockEvent = 
			{
			},
		},
		[2] = {nTime = 300, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str730[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "guaiwu1"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[10]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[11]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[13]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[14]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[15]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[16]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[17]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[18]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 25,		-- 刷小怪
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 4, 3, "guaiwu", "mobing8147"},		-- 刷怪
				{XoyoGame.ADD_NPC, 3, 3, 3, "guaiwu", "mobing9113"},		-- 刷怪
				{XoyoGame.ADD_NPC, 4, 4, 3, "guaiwu", "mobing9114"},		-- 刷怪
				{XoyoGame.ADD_NPC, 5, 4, 3, "guaiwu", "mobing9115"},		-- 刷怪
				{XoyoGame.ADD_NPC, 6, 3, 3, "guaiwu", "mobing9116"},		-- 刷怪
				{XoyoGame.ADD_NPC, 7, 3, 3, "guaiwu", "mobing9117"},		-- 刷怪
				{XoyoGame.ADD_NPC, 8, 4, 3, "guaiwu", "mobing9118"},		-- 刷怪
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str731[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str639[Lang.Idx], 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str732[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[4] = {nTime = 0, nNum = 1,		-- 刷BOSS
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 1, 4, "boss", "mojiang8150"},		-- 刷怪
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str733[Lang.Idx]},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str734[Lang.Idx],4},
			},
			tbUnLockEvent = 
			{
			    {XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "guaiwu1"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[10]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[11]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[13]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[14]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[15]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[16]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[17]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[18]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str735[Lang.Idx]},
			},
		},
		[5] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 5},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx], 5},
			},
			tbUnLockEvent = 
			{
			},
		},
		[6] = {nTime = 15, nNum = 0,
			tbPrelock = {3},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[7] = {nTime = 30, nNum = 0,
			tbPrelock = {6},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 30, nNum = 0,
			tbPrelock = {7},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[9] = {nTime = 30, nNum = 0,
			tbPrelock = {8},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[10] = {nTime = 30, nNum = 0,
			tbPrelock = {9},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[11] = {nTime = 30, nNum = 0,
			tbPrelock = {10},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[12] = {nTime = 30, nNum = 0,
			tbPrelock = {11},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[13] = {nTime = 30, nNum = 0,
			tbPrelock = {12},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[14] = {nTime = 30, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[15] = {nTime = 30, nNum = 0,
			tbPrelock = {14},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[16] = {nTime = 30, nNum = 0,
			tbPrelock = {15},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[17] = {nTime = 30, nNum = 0,
			tbPrelock = {16},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[18] = {nTime = 30, nNum = 0,
			tbPrelock = {17},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 9, 2, 0, "guaiwu1", "qinwei9119"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[19] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 19},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}