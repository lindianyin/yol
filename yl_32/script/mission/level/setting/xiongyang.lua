Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[24] =    --熊洋
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 4,						-- 房间等级(1~5)
	nMapIndex		= 16,						-- 地图组的索引
	tbBeginPoint	= {2717, 2245, 1055226},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8039, nLevel = {8097,8098,8099}, nSeries = -1},		-- 木人力士1
		[2] = {nTemplate = 8058, nLevel = {8129,8130,8131}, nSeries = -1},		-- 木人道士1
		[3] = {nTemplate = 8040, nLevel = {8100,8101,8102}, nSeries = -1},		-- 熊洋
		[4] = {nTemplate = 9127, nLevel = {8097,8098,8099}, nSeries = -1},		-- 木人力士2
		[5] = {nTemplate = 9128, nLevel = {8129,8130,8131}, nSeries = -1},		-- 木人道士2
		[6] = {nTemplate = 9129, nLevel = {8097,8098,8099}, nSeries = -1},		-- 木人力士3
		[7] = {nTemplate = 9130, nLevel = {8129,8130,8131}, nSeries = -1},		-- 木人道士3
		[8] = {nTemplate = 9131, nLevel = {8097,8098,8099}, nSeries = -1},		-- 木人力士4
		[9] = {nTemplate = 9132, nLevel = {8129,8130,8131}, nSeries = -1},		-- 木人道士4
		[10] = {nTemplate = 9133, nLevel = {8097,8098,8099}, nSeries = -1},		-- 爆炸人
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
		[2] = {nTime = 270, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str765[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str766[Lang.Idx]},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[10]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[11]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[13]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[14]:Close()"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "guaiwu"},
			},
		},
		[3] = {nTime = 0, nNum = 16,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 2, 3, "guaiwu", "muren8097"},		-- 木人力士
				{XoyoGame.ADD_NPC, 2, 2, 3, "guaiwu", "muren8126"},		-- 木人道士
				{XoyoGame.ADD_NPC, 4, 2, 3, "guaiwu", "muren9127"},		-- 木人力士
				{XoyoGame.ADD_NPC, 5, 2, 3, "guaiwu", "muren9128"},		-- 木人道士
				{XoyoGame.ADD_NPC, 6, 2, 3, "guaiwu", "muren9129"},		-- 木人力士
				{XoyoGame.ADD_NPC, 7, 2, 3, "guaiwu", "muren9130"},		-- 木人道士
				{XoyoGame.ADD_NPC, 8, 2, 3, "guaiwu", "muren9131"},		-- 木人力士
				{XoyoGame.ADD_NPC, 9, 2, 3, "guaiwu", "muren9132"},		-- 木人道士
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str767[Lang.Idx],3},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
			},
		},
		[4] = {nTime = 0, nNum = 1,
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 3, 1, 4, "boss", "xiongyang8100"},		-- 熊洋
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str768[Lang.Idx],4},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[10]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[11]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[13]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[14]:Close()"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str769[Lang.Idx]},
			},
		},
		[5] = {nTime = 15, nNum = 0,
			tbPrelock = {3},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[6] = {nTime = 30, nNum = 0,
			tbPrelock = {5},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str770[Lang.Idx]},
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
		[7] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 7},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 30, nNum = 0,
			tbPrelock = {6},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
		[9] = {nTime = 30, nNum = 0,
			tbPrelock = {8},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
		[10] = {nTime = 30, nNum = 0,
			tbPrelock = {9},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
		[11] = {nTime = 30, nNum = 0,
			tbPrelock = {10},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
		[12] = {nTime = 30, nNum = 0,
			tbPrelock = {11},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
		[13] = {nTime = 30, nNum = 0,
			tbPrelock = {12},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
		[14] = {nTime = 30, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 10, 2, 0, "guaiwu", "muren9133"},		-- 木人力士	
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}