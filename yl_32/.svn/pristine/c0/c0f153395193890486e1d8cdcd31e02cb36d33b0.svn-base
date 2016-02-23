Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[17] =    --古墓护送
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 3,						-- 房间等级(1~5)
	nMapIndex		= 21,						-- 地图组的索引
	tbBeginPoint	= {2958, 1796, 1048576},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8025, nLevel = {8057,8057,8057}, nSeries = -1},		-- 小栗
		[2] = {nTemplate = 8026, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
		[3] = {nTemplate = 8027, nLevel = {8061,8062,8063}, nSeries = -1},		-- 铁尸
		[4] = {nTemplate = 9073, nLevel = {8056,8056,8056}, nSeries = -1},      --off火把
		[5] = {nTemplate = 9074, nLevel = {8056,8056,8056}, nSeries = -1},      --on火把
		[6] = {nTemplate = 9075, nLevel = {8056,8056,8056}, nSeries = -1},      --off火把
		[7] = {nTemplate = 9076, nLevel = {8056,8056,8056}, nSeries = -1},      --on火把
		[8] = {nTemplate = 9120, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
		[9] = {nTemplate = 9121, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
		[10] = {nTemplate = 9122, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
		[11] = {nTemplate = 9123, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
		[12] = {nTemplate = 9124, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
		[13] = {nTemplate = 9125, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
		[14] = {nTemplate = 9126, nLevel = {8058,8059,8060}, nSeries = -1},		-- 幽魂
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
		[2] = {nTime = 240, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str644[Lang.Idx]},
				{XoyoGame.DEL_NPC, "xiaoli"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 2,
			tbPrelock = {1},
			tbStartEvent = 
			{				
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str645[Lang.Idx]},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str646[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{		
			},
		},
		[4] = {nTime = 0, nNum = 1,		-- 护送npc
			tbPrelock = {6,7},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 1, 0, "xiaoli", "xiaoli8057"},		-- 刷怪
				{XoyoGame.ADD_NPC, 3, 1, 4, "boss", "tieshi8061"},		-- 刷怪
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str647[Lang.Idx]},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str648[Lang.Idx],4},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str649[Lang.Idx]},
				
			},
		},
		[5] = {nTime = 0, nNum = 32,
			tbPrelock = {4},
			tbStartEvent = 
			{				
				{XoyoGame.ADD_NPC, 2, 4, 5, "guaiwu", "youhun8058"},		-- 幽魂
				{XoyoGame.ADD_NPC, 8, 4, 5, "guaiwu", "youhun9120"},		-- 幽魂
				{XoyoGame.ADD_NPC, 9, 4, 5, "guaiwu", "youhun9121"},		-- 幽魂
				{XoyoGame.ADD_NPC, 10, 4, 5, "guaiwu", "youhun9122"},		-- 幽魂
				{XoyoGame.ADD_NPC, 11, 4, 5, "guaiwu", "youhun9123"},		-- 幽魂
				{XoyoGame.ADD_NPC, 12, 4, 5, "guaiwu", "youhun9124"},		-- 幽魂
				{XoyoGame.ADD_NPC, 13, 4, 5, "guaiwu", "youhun9125"},		-- 幽魂
				{XoyoGame.ADD_NPC, 14, 4, 5, "guaiwu", "youhun9126"},		-- 幽魂
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str650[Lang.Idx],5},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "xiaoli"},
				{XoyoGame.DEL_NPC, "huoba1off1"},
				{XoyoGame.DEL_NPC, "huoba1on1"},
				{XoyoGame.DEL_NPC, "huoba2off1"},
				{XoyoGame.DEL_NPC, "huoba2on1"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
			},
		},
		[6] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{				
				{XoyoGame.ADD_NPC, 4, 1, 6, "huoba1off1", "huoba1off"},	
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{		
				{XoyoGame.ADD_NPC, 5, 1, 0, "huoba1on1", "huoba1on"},	
			},
		},
		[7] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{				
				{XoyoGame.ADD_NPC, 6, 1, 7, "huoba2off1", "huoba2off"},	
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{	
				{XoyoGame.ADD_NPC, 7, 1, 0, "huoba2on1", "huoba2on"},	
			},
		},
		[8] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str651[Lang.Idx], 8},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}