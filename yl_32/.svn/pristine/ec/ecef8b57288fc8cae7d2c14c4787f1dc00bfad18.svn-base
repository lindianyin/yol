Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[49] =    --熔浆怪	
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 1,						-- 房间等级(1~5)
	nMapIndex		= 50,						-- 地图组的索引
	tbBeginPoint	= {1091, 1568, 1056256},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 9068, nLevel = {8129,8130,8131}, nSeries = -1},	
		[2] = {nTemplate = 9070, nLevel = {8056,8056,8056}, nSeries = -1},	-- 
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
		[2] = {nTime = 180, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str701[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str702[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "xiangzi"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 1,	
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 3, "xiangzi", "xiangzi9070"},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str703[Lang.Idx],3},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
			},
		},
		[4] = {nTime = 0, nNum = 4,
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str704[Lang.Idx]},
				{XoyoGame.ADD_NPC, 1, 4, 4, "guai", "rongjiang9068"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str705[Lang.Idx],4},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
			},
		},
		[5] = {nTime = 0, nNum = 4,
			tbPrelock = {4},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str706[Lang.Idx]},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str707[Lang.Idx],5},
				{XoyoGame.ADD_NPC, 1, 4, 5, "guai", "rongjiang9068"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[6] = {nTime = 0, nNum = 5,		
			tbPrelock = {5},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str708[Lang.Idx],6},
				{XoyoGame.ADD_NPC, 1, 5, 6, "guai", "rongjiang9068"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[7] = {nTime = 0, nNum = 5,		
			tbPrelock = {6},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str709[Lang.Idx],7},
				{XoyoGame.ADD_NPC, 1, 5, 7, "guai", "rongjiang9068"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 0, nNum = 6,		
			tbPrelock = {7},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str710[Lang.Idx],8},
				{XoyoGame.ADD_NPC, 1, 6, 8, "guai", "rongjiang9068"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DEL_NPC, "guai"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str711[Lang.Idx]},
			},
		},
		[9] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 9},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}