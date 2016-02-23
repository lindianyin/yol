Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[15] =    --虫王菠萝丁
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 3,						-- 房间等级(1~5)
	nMapIndex		= 11,						-- 地图组的索引
	tbBeginPoint	= {3715, 577, 1048576},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	DerivedRoom		= XoyoGame.RoomBoLuoDing,
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8008, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
		[2] = {nTemplate = 8010, nLevel = {8025,8026,8027}, nSeries = -1},		-- 虫王（虫）
		[3] = {nTemplate = 8009, nLevel = {8028,8029,8030}, nSeries = -1},		-- 虫王（人）
		[4] = {nTemplate = 9106, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
		[5] = {nTemplate = 9107, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
		[6] = {nTemplate = 9108, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
		[7] = {nTemplate = 9109, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
		[8] = {nTemplate = 9110, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
		[9] = {nTemplate = 9111, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
		[10] = {nTemplate = 9112, nLevel = {8022,8023,8024}, nSeries = -1},		-- 小虫
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str618[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "boss1"},
				{XoyoGame.DEL_NPC, "boss2"},
			},
		},
		[3] = {nTime = 0, nNum = 32,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 4, 3, "guaiwu", "chong8008"},		-- 刷怪
				{XoyoGame.ADD_NPC, 4, 4, 3, "guaiwu", "chong9106"},		-- 刷怪
				{XoyoGame.ADD_NPC, 5, 4, 3, "guaiwu", "chong9107"},		-- 刷怪
				{XoyoGame.ADD_NPC, 6, 4, 3, "guaiwu", "chong9108"},		-- 刷怪
				{XoyoGame.ADD_NPC, 7, 4, 3, "guaiwu", "chong9109"},		-- 刷怪
				{XoyoGame.ADD_NPC, 8, 4, 3, "guaiwu", "chong9110"},		-- 刷怪
				{XoyoGame.ADD_NPC, 9, 4, 3, "guaiwu", "chong9111"},		-- 刷怪
				{XoyoGame.ADD_NPC, 10, 4, 3, "guaiwu", "chong9112"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str619[Lang.Idx],3},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str620[Lang.Idx]},
			},
		},
		[4] = {nTime = 0, nNum = 2,
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 4, "boss1", "cwcx8010"},		-- 刷怪
				{XoyoGame.ADD_NPC, 3, 1, 4, "boss2", "cwrx8009"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str621[Lang.Idx],4},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "boss1"},
				{XoyoGame.DEL_NPC, "boss2"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str622[Lang.Idx]},
			},
		},
		[5] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str623[Lang.Idx], 5},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}