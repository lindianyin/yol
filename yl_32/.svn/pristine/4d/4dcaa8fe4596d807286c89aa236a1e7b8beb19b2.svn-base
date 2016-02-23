Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[44] =    --凶邪之手
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 6,						-- 房间等级(1~5)
	nMapIndex		= 45,						-- 地图组的索引
	tbBeginPoint	= {583, 1606, 1053690},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8075, nLevel = {8138,8139,8140}, nSeries = -1},		-- 腐败之血
		[2] = {nTemplate = 8072, nLevel = {8141,8142,8143}, nSeries = -1},		-- 凶邪之手
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
		[2] = {nTime = 330, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str760[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str761[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[10]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[11]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
		},
		[3] = {nTime = 0, nNum = 1,		-- 刷BOSS
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 3, "boss", "xiongxiezhishou8141"},		-- 刷BOSS
				{XoyoGame.TIME_INFO, -1, Lang.mission.str762[Lang.Idx], 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str763[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			    {XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "guaiwu"},
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
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str764[Lang.Idx]},
			},
		},
		[4] = {nTime = 20, nNum = 0,		-- 刷小怪
			tbPrelock = {1},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[5] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {4},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[6] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {5},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[7] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {6},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {7},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[9] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {8},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[10] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {9},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[11] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {10},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[12] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {11},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[13] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {12},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[14] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {13},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[15] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {14},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[16] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {15},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[17] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {16},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[18] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {17},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[19] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {18},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[20] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {19},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[21] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {20},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[22] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {21},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[23] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {22},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[24] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {23},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[25] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {24},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[26] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {25},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[27] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {26},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[28] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {27},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[29] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {28},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[30] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {29},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[31] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {30},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[32] = {nTime = 10, nNum = 0,		-- 刷小怪
			tbPrelock = {31},
			tbStartEvent = 
			{
			    {XoyoGame.ADD_NPC, 1, 2, 0, "guaiwu", "fubaizhixue8138"},		-- 刷小怪
			},
			tbUnLockEvent = 
			{
			},
		},
		[33] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 33},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}