Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[9] =    --地痞流氓
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 2,						-- 房间等级(1~5)
	nMapIndex		= 6,						-- 地图组的索引
	tbBeginPoint	= {1911, 1280, 1049600},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8004, nLevel = {8010,8011,8012}, nSeries = -1},		-- 流氓
		[2] = {nTemplate = 8005, nLevel = {8013,8014,8015}, nSeries = -1},		-- 大姐大
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str628[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str629[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "guaiwu1"},
				{XoyoGame.DEL_NPC, "boss"},
			},
		},
		[3] = {nTime = 0, nNum = 4,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 4, 3, "guaiwu", "dipi8004"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str630[Lang.Idx],3},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				--该处可制定路线，类似动画，流氓逃走
			},
		},
		[4] = {nTime = 8, nNum = 0,
			tbPrelock = {3},
			tbStartEvent = 
			{		
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str631[Lang.Idx]},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str632[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
			},
		},
		[5] = {nTime = 0, nNum = 5,
			tbPrelock = {4},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 5, "boss", "junjie8005"},		-- 王
				{XoyoGame.ADD_NPC, 1, 4, 5, "guaiwu1", "dipi8004_2"},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str633[Lang.Idx]},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str634[Lang.Idx],5},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "guaiwu1"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1,Lang.mission.str635[Lang.Idx]},
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