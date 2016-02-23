Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[43] =    --灰暗之血
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 2,						-- 房间等级(1~5)
	nMapIndex		= 44,						-- 地图组的索引
	tbBeginPoint	= {1850, 769, 1052154},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8066, nLevel = {8153,8154,8154}, nSeries = -1},		-- 灰暗之血
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
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str656[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 4,		-- 刷怪
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 4, 3, "guaiwu", "shuizhu8153"},		-- 刷怪
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str657[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str639[Lang.Idx], 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str659[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[4] = {nTime = 0, nNum = 5,		-- 刷怪
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 5, 4, "guaiwu", "shuizhu8153"},		-- 刷怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str639[Lang.Idx], 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str660[Lang.Idx],4},
			},
			tbUnLockEvent = 
			{
			},
		},
		[5] = {nTime = 0, nNum = 6,		-- 刷怪
			tbPrelock = {4},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 6, 5, "guaiwu", "shuizhu8153"},		-- 刷怪
				{XoyoGame.TIME_INFO, -1, "Lang.mission.str639[Lang.Idx]", 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str661[Lang.Idx],5},
			},
			tbUnLockEvent = 
			{
			    {XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str662[Lang.Idx]},
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