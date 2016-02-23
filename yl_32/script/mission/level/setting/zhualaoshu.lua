Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[8] =    --抓田鼠
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 1,						-- 房间等级(1~5)
	nMapIndex		= 12,						-- 地图组的索引
	tbBeginPoint	= {2620, 880, 1051136},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8023, nLevel = {8055,8055,8055}, nSeries = -1},		-- 二多
		[2] = {nTemplate = 8024, nLevel = {8056,8056,8056}, nSeries = -1},		-- 田鼠
		[3] = {nTemplate = 9071, nLevel = {9038,9038,9038}, nSeries = -1}, 
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str782[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str783[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "erduo"},
				{XoyoGame.DEL_NPC, "boss"},
			},
		},
		[3] = {nTime = 0, nNum = 30,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 36, 3, "guaiwu", "laoshu5056"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str784[Lang.Idx],3},
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str785[Lang.Idx]},
				{XoyoGame.ADD_NPC, 3, 1, 4, "boss", "yishuke9038"},		-- 刷怪
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str786[Lang.Idx],4},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
			},
		},
		[5] ={nTime = 1, nNum = 0,
			tbPrelock = {4},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str787[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "erduo"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
			},
		},
		[6] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 6},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}