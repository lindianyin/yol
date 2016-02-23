Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[45] =    --镇守雕龙
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 7,						-- 房间等级(1~5)
	nMapIndex		= 46,						-- 地图组的索引
	tbBeginPoint	= {7584, 2549, 1045724},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8069, nLevel = {8155,8155,8155}, nSeries = -1},		-- 镇守雕龙
		[2] = {nTemplate = 8070, nLevel = {8125,8125,8125}, nSeries = -1},      --火
		[3] = {nTemplate = 8071, nLevel = {8125,8125,8125}, nSeries = -1},      --云雾
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
		[2] = {nTime = 360, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str778[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 1,		-- 刷BOSS
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str779[Lang.Idx]},
				{XoyoGame.ADD_NPC, 1, 1, 3, "boss", "long8155"},		-- 刷怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str639[Lang.Idx], 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str780[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str781[Lang.Idx]},
			    {XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "huo"},
				{XoyoGame.DEL_NPC, "yunwu"},
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
				{XoyoGame.DO_SCRIPT, "self.tbLock[23]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[24]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[25]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[26]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[27]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[28]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[29]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[30]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[31]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[32]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[33]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[34]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[35]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[36]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[37]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
			},
		},
		[4] = {nTime = 25, nNum = 0,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 15, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[5] = {nTime = 5, nNum = 0,
			tbPrelock = {4},
			tbStartEvent = 
			{	
			},
			tbUnLockEvent = 
			{
			},
		},
		[6] = {nTime = 25, nNum = 0,
			tbPrelock = {5},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 17, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[7] = {nTime = 5, nNum = 0,
			tbPrelock = {6},
			tbStartEvent = 
			{				
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 25, nNum = 0,
			tbPrelock = {7},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 19, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[9] = {nTime = 5, nNum = 0,
			tbPrelock = {8},
			tbStartEvent = 
			{	
			},
			tbUnLockEvent = 
			{
			},
		},
		[10] = {nTime = 25, nNum = 0,
			tbPrelock = {9},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 21, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[11] = {nTime = 5, nNum = 0,
			tbPrelock = {10},
			tbStartEvent = 
			{				
			},
			tbUnLockEvent = 
			{
			},
		},
		[12] = {nTime = 25, nNum = 0,
			tbPrelock = {11},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 23, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[13] = {nTime = 5, nNum = 0,
			tbPrelock = {12},
			tbStartEvent = 
			{	
			},
			tbUnLockEvent = 
			{
			},
		},
		[14] = {nTime = 25, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 25, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[15] = {nTime = 5, nNum = 0,
			tbPrelock = {14},
			tbStartEvent = 
			{				
			},
			tbUnLockEvent = 
			{
			},
		},
		[16] = {nTime = 25, nNum = 0,
			tbPrelock = {15},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 27, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[17] = {nTime = 5, nNum = 0,
			tbPrelock = {16},
			tbStartEvent = 
			{	
			},
			tbUnLockEvent = 
			{
			},
		},
		[18] = {nTime = 25, nNum = 0,
			tbPrelock = {17},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 29, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[19] = {nTime = 5, nNum = 0,
			tbPrelock = {18},
			tbStartEvent = 
			{				
			},
			tbUnLockEvent = 
			{
			},
		},
		[20] = {nTime = 25, nNum = 0,
			tbPrelock = {19},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 30, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[21] = {nTime = 5, nNum = 0,
			tbPrelock = {20},
			tbStartEvent = 
			{	
			},
			tbUnLockEvent = 
			{
			},
		},
		[22] = {nTime = 25, nNum = 0,
			tbPrelock = {21},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 31, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[23] = {nTime = 5, nNum = 0,
			tbPrelock = {22},
			tbStartEvent = 
			{				
			},
			tbUnLockEvent = 
			{
			},
		},
		[24] = {nTime = 25, nNum = 0,
			tbPrelock = {23},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 32, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[25] = {nTime = 5, nNum = 0,
			tbPrelock = {24},
			tbStartEvent = 
			{	
			},
			tbUnLockEvent = 
			{
			},
		},
		[26] = {nTime = 25, nNum = 0,
			tbPrelock = {25},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 33, 0, "huo", "huo8070"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "huo"},
			},
		},
		[27] = {nTime = 30, nNum = 0,
			tbPrelock = {1},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[28] = {nTime = 30, nNum = 0,
			tbPrelock = {27},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[29] = {nTime = 30, nNum = 0,
			tbPrelock = {28},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[30] = {nTime = 30, nNum = 0,
			tbPrelock = {29},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[31] = {nTime = 30, nNum = 0,
			tbPrelock = {30},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[32] = {nTime = 30, nNum = 0,
			tbPrelock = {31},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[33] = {nTime = 30, nNum = 0,
			tbPrelock = {32},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[34] = {nTime = 30, nNum = 0,
			tbPrelock = {33},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[35] = {nTime = 30, nNum = 0,
			tbPrelock = {34},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[36] = {nTime = 30, nNum = 0,
			tbPrelock = {35},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[37] = {nTime = 30, nNum = 0,
			tbPrelock = {36},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 3, 1, 0, "yunwu", "yunwu8071"},		
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "yunwu"},
			},
		},
		[38] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 38},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx],38},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}