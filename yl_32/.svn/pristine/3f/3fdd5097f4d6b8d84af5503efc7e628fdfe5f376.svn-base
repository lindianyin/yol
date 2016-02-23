Include("script/mission/level/xoyogame_def.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[16] =    --追风
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 3,						-- 房间等级(1~5)
	nMapIndex		= 7,						-- 地图组的索引
	tbBeginPoint	= {2535, 528, 1051136},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8019, nLevel = {8051,8051,8051}, nSeries = -1},		-- 葛英人（对话）
		[2] = {nTemplate = 8012, nLevel = {8034,8034,8034}, nSeries = -1},		-- 葛英人（战斗）
		[3] = {nTemplate = 8011, nLevel = {8031,8032,8033}, nSeries = -1},		-- 追风
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
				--{XoyoGame.ADD_NPC, 1, 1, 0, "butou_duihua", "1_yelang_3"},		-- 刷怪
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str788[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str789[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "butou_zhandou"},
			},
		},
		[3] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{
				
				{XoyoGame.ADD_NPC, 3, 1, 3, "boss", "zhuifeng8011"},		-- 刷怪
				{XoyoGame.NPC_BLOOD_PERCENT, 
					"boss",
					{25,XoyoGame.DO_SCRIPT, "self.tbLock[4]:UnLock()"},
				},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str790[Lang.Idx],3},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "butou_duihua"},
				{XoyoGame.DEL_NPC, "butou_zhandou"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str791[Lang.Idx]},
			},
		},
		[4] = {nTime = 4, nNum = 0,
			tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				
				{XoyoGame.ADD_NPC, 2, 1, 0, "butou_zhandou", "geyr8012"},		-- 刷怪
				
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