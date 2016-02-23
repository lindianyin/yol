Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[30] =    --木否愚
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 5,						-- 房间等级(1~5)
	nMapIndex		= 18,						-- 地图组的索引
	tbBeginPoint	= {1384, 1553, 1049041},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	tbClearBuff		= {934,936},
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8043, nLevel = {8109,8110,8111}, nSeries = -1},		-- 木否愚
		[2] = {nTemplate = 8044, nLevel = {8112,8112,8112}, nSeries = -1},		-- 葫芦
		[3] = {nTemplate = 8045, nLevel = {8113,8113,8113}, nSeries = -1},		-- 炉子
		[4] = {nTemplate = 8052, nLevel = {8127,8127,8127}, nSeries = -1},      --药葫芦
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str684[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str685[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "hulu"},
				{XoyoGame.DEL_NPC, "luzi"},
				{XoyoGame.DEL_NPC, "yao"},
			},
		},
		[3] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 1, 3, "boss", "mufouyu5109"},		-- 刷怪
				{XoyoGame.ADD_NPC, 4, 4, 0, "yao", "yaotong8127"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str686[Lang.Idx],3},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "hulu"},
				{XoyoGame.DEL_NPC, "luzi"},
				{XoyoGame.DEL_NPC, "yao"},
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
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
				{XoyoGame.DO_SCRIPT, "self.tbLock[19]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[20]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[21]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[22]:Close()"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str687[Lang.Idx]},
			},
		},
		[4] = {nTime = 15, nNum = 0,
			tbPrelock = {1},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[5] = {nTime = 15, nNum = 0,
			tbPrelock = {4},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str688[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[6] = {nTime = 15, nNum = 0,
			tbPrelock = {5},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[7] = {nTime = 15, nNum = 0,
			tbPrelock = {6},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str689[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[8] = {nTime = 15, nNum = 0,
			tbPrelock = {7},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[9] = {nTime = 15, nNum = 0,
			tbPrelock = {8},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[10] = {nTime = 15, nNum = 0,
			tbPrelock = {9},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[11] = {nTime = 15, nNum = 0,
			tbPrelock = {10},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[12] = {nTime = 15, nNum = 0,
			tbPrelock = {11},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[13] = {nTime = 15, nNum = 0,
			tbPrelock = {12},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[14] = {nTime = 15, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[15] = {nTime = 15, nNum = 0,
			tbPrelock = {14},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[16] = {nTime = 15, nNum = 0,
			tbPrelock = {15},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[17] = {nTime = 15, nNum = 0,
			tbPrelock = {16},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[18] = {nTime = 15, nNum = 0,
			tbPrelock = {17},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[19] = {nTime = 15, nNum = 0,
			tbPrelock = {18},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[20] = {nTime = 15, nNum = 0,
			tbPrelock = {19},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[21] = {nTime = 15, nNum = 0,
			tbPrelock = {20},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[22] = {nTime = 15, nNum = 0,
			tbPrelock = {21},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 0, "hulu", "honghulu8112"},		-- 刷怪
			},
		},
		[23] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 23},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}