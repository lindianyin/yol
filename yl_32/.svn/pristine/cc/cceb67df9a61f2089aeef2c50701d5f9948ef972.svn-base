Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[14] =    --古墓冒险
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 2,						-- 房间等级(1~5)
	nMapIndex		= 15,						-- 地图组的索引
	tbBeginPoint	= {974, 3593, 1051970},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8037, nLevel ={8091,8092,8093}, nSeries = -1},		-- 怨魂
		[2] = {nTemplate = 8038, nLevel ={8094,8095,8096}, nSeries = -1},		-- 恶灵
		[3] = {nTemplate = 9093, nLevel ={8091,8092,8093}, nSeries = -1},		-- 怨魂
		[4] = {nTemplate = 9094, nLevel ={8091,8092,8093}, nSeries = -1},		-- 怨魂
		[5] = {nTemplate = 9095, nLevel ={8091,8092,8093}, nSeries = -1},		-- 怨魂
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
		[2] = {nTime = 210, nNum = 0,		-- 1阶段总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str652[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str653[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu1"},
				{XoyoGame.DEL_NPC, "guaiwu2"},
			},
		},
		[3] = {nTime = 0, nNum = 32,
			tbPrelock = {1},
			tbStartEvent = 
			{		
				{XoyoGame.ADD_NPC, 1, 7, 3, "guaiwu1", "yuanhun8091"},		-- 刷怪
				{XoyoGame.ADD_NPC, 3, 7, 3, "guaiwu1", "yuanhun9093"},		-- 刷怪
				{XoyoGame.ADD_NPC, 4, 7, 3, "guaiwu1", "yuanhun9094"},		-- 刷怪
				{XoyoGame.ADD_NPC, 5, 7, 3, "guaiwu1", "yuanhun9095"},		-- 刷怪
				{XoyoGame.ADD_NPC, 2, 4, 3, "guaiwu2", "eling8094"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str654[Lang.Idx],3},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str655[Lang.Idx]},
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