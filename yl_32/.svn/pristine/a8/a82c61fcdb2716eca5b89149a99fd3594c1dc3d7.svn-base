Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[11] =    --狼
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 2,						-- 房间等级(1~5)
	nMapIndex		= 4,						-- 地图组的索引
	tbBeginPoint	= {3272, 920, 1052672},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8006, nLevel = {8016,8017,8018}, nSeries = -1},		-- 狼
		[2] = {nTemplate = 8007, nLevel = {8019,8020,8021}, nSeries = -1},		-- 狼王
		[3] = {nTemplate = 8017, nLevel = {8047,8048,8049}, nSeries = -1},		-- 狼守卫
		[4] = {nTemplate = 9089, nLevel = {8016,8017,8018}, nSeries = -1},		-- 狼
		[5] = {nTemplate = 9090, nLevel = {8016,8017,8018}, nSeries = -1},		-- 狼
		[6] = {nTemplate = 9091, nLevel = {8016,8017,8018}, nSeries = -1},		-- 狼
		[7] = {nTemplate = 9092, nLevel = {8016,8017,8018}, nSeries = -1},		-- 狼
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
		[2] = {nTime = 210, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str674[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str675[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "boss"},
			},
		},
		[3] = {nTime = 0, nNum = 25,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1,5, 3, "guaiwu", "lang8018"},		-- 刷怪
				{XoyoGame.ADD_NPC, 4,5, 3, "guaiwu", "lang9089"},		-- 刷怪
				{XoyoGame.ADD_NPC, 5,5, 3, "guaiwu", "lang9090"},		-- 刷怪
				{XoyoGame.ADD_NPC, 6,5, 3, "guaiwu", "lang9091"},		-- 刷怪
				{XoyoGame.ADD_NPC, 7,5, 3, "guaiwu", "lang9092"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str676[Lang.Idx],3},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str677[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
			},
		},
		[4] = {nTime = 0, nNum = 1,
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 4, "boss", "langwang8007"},		-- 刷怪
				{XoyoGame.NPC_BLOOD_PERCENT, 
					"boss",
					{75,XoyoGame.ADD_NPC, 3, 2, 0, "guaiwu", "langsw8017"},
					{50,XoyoGame.ADD_NPC, 3, 2, 0, "guaiwu", "langsw8017"},
					{25,XoyoGame.ADD_NPC, 3, 2, 0, "guaiwu", "langsw8017"},
				},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str678[Lang.Idx],4},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str677[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str679[Lang.Idx]},
			},
		},
		[5] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 5},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}