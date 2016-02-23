Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[1] =    --木人
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 1,						-- 房间等级(1~5)
	nMapIndex		= 3,						-- 地图组的索引
	tbBeginPoint	= {420, 1935, 1050618},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8003, nLevel = {8007,8008,8009}, nSeries = -1},		-- 木人
		[2] = {nTemplate = 9083, nLevel = {8007,8008,8009}, nSeries = -1},      --muren9083
		[3] = {nTemplate = 9084, nLevel = {8007,8008,8009}, nSeries = -1},     --muren9084
		[4] = {nTemplate = 9085, nLevel = {8064,8065,8066}, nSeries = -1},
		[5] = {nTemplate = 9086, nLevel = {8064,8065,8066}, nSeries = -1},
		[6] = {nTemplate = 9087, nLevel = {8064,8065,8066}, nSeries = -1},
		[7] = {nTemplate = 9088, nLevel = {8064,8065,8066}, nSeries = -1},
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str697[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str698[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu"},
				
			},
		},
		[3] = {nTime = 0, nNum = 28,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1,4, 3, "guaiwu", "muren8003"},		-- 刷怪
				{XoyoGame.ADD_NPC, 2,4, 3, "guaiwu", "muren9083"},		-- 刷怪
				{XoyoGame.ADD_NPC, 3,4, 3, "guaiwu", "muren9084"},		-- 刷怪
				{XoyoGame.ADD_NPC, 4,4, 3, "guaiwu", "muren9085"},		-- 刷怪
				{XoyoGame.ADD_NPC, 5,4, 3, "guaiwu", "muren9086"},		-- 刷怪
				{XoyoGame.ADD_NPC, 6,4, 3, "guaiwu", "muren9087"},		-- 刷怪
				{XoyoGame.ADD_NPC, 7,4, 3, "guaiwu", "muren9088"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str699[Lang.Idx],3},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str700[Lang.Idx]},
			},
		},
		[4] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 4},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}