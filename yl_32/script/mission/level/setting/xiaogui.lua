Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[12] =    --小鬼
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 2,						-- 房间等级(1~5)
	nMapIndex		= 13,						-- 地图组的索引
	tbBeginPoint	= {1269, 3889, 1048842},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8028, nLevel = {8064,8065,8066}, nSeries = -1},		-- 小鬼
		[2] = {nTemplate = 9077, nLevel = {8056,8056,8056}, nSeries = -1},		-- 引魂灯1灭
		[3] = {nTemplate = 9078, nLevel = {8056,8056,8056}, nSeries = -1},		-- 引魂灯1亮
		[4] = {nTemplate = 9079, nLevel = {8056,8056,8056}, nSeries = -1},		-- 引魂灯2灭
		[5] = {nTemplate = 9080, nLevel = {8056,8056,8056}, nSeries = -1},		-- 引魂灯2亮
		[6] = {nTemplate = 9081, nLevel = {9042,9043,9043}, nSeries = -1},		-- 小鬼统领
		[7] = {nTemplate = 9096, nLevel = {8064,8065,8066}, nSeries = -1},		-- 小鬼
		[8] = {nTemplate = 9097, nLevel = {8064,8065,8066}, nSeries = -1},		-- 小鬼
		[9] = {nTemplate = 9098, nLevel = {8064,8065,8066}, nSeries = -1},		-- 小鬼
		[10] = {nTemplate = 9099, nLevel = {8064,8065,8066}, nSeries = -1},		-- 小鬼
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str752[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str753[Lang.Idx]},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "deng1"},
				{XoyoGame.DEL_NPC, "deng2"},
				{XoyoGame.DEL_NPC, "deng3"},
				{XoyoGame.DEL_NPC, "deng4"},
				{XoyoGame.DEL_NPC, "boss"},
			},
		},
		[3] = {nTime = 0, nNum = 26,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 5, 3, "guaiwu", "xiaogui8064"},		-- 刷怪
				{XoyoGame.ADD_NPC, 7, 4, 3, "guaiwu", "xiaogui9096"},		-- 刷怪
				{XoyoGame.ADD_NPC, 8, 5, 3, "guaiwu", "xiaogui9097"},		-- 刷怪
				{XoyoGame.ADD_NPC, 9, 6, 3, "guaiwu", "xiaogui9098"},		-- 刷怪
				{XoyoGame.ADD_NPC, 10, 6, 3, "guaiwu", "xiaogui9099"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str754[Lang.Idx],3},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},
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
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str755[Lang.Idx]},
				{XoyoGame.ADD_NPC, 2, 1, 4, "deng1", "hundeng9077"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str756[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.ADD_NPC, 3, 1, 0, "deng3", "hundeng9078"},
			},
		},
		[5] = {nTime = 0, nNum = 1,
			tbPrelock = {4},
			tbStartEvent = 
			{	
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str757[Lang.Idx]},
				{XoyoGame.ADD_NPC, 6, 1, 5, "boss", "tongling9081"},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str758[Lang.Idx],5},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str759[Lang.Idx]},
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