Include("script/mission/level/xoyogame_def.lua");
Include("script/mission/level/xmhj1/zengya.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[22] =    --曾雅
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 4,						-- 房间等级(1~5)
	nMapIndex		= 8,						-- 地图组的索引
	tbBeginPoint	= {2728, 1745, 1056762},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	DerivedRoom		= XoyoGame.ZengYa,
	tbClearBuff		= {6101, 6105, 6099},               --需要清除的BUFF
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8013, nLevel = {8035,8036,8037}, nSeries = -1},		-- 曾雅 现实zya8013
		[2] = {nTemplate = 9139, nLevel = {8035,8036,8037}, nSeries = -1},		-- 曾雅 梦境zya9139
		[3] = {nTemplate = 9140, nLevel = {8056,8056,8056}, nSeries = -1},		-- 迴梦葫芦hulu9140
		[4] = {nTemplate = 9141, nLevel = {8056,8056,8056}, nSeries = -1},		-- 望乡井jing9141
	},
	-- 锁结构
	LOCK = 
	{
		-- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 15, nNum = 0,
			tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str600[Lang.Idx], 1},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, ""},
			},
			tbUnLockEvent = 
			{
			},
		},
		[2] = {nTime = 270, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str771[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str772[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "boss1"},
                {XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"}, 
                {XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"}, 
                {XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[10]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[11]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[13]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[14]:Close()"},
                {XoyoGame.DO_SCRIPT, "self.tbLock[15]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str773[Lang.Idx]},
				{XoyoGame.ADD_NPC, 1, 1, 16, "boss", "zya8013"},		-- 刷怪
				{XoyoGame.ADD_NPC, 3, 1, 3, "rumeng", "hulu9140"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str774[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss]])"},
			},
		},
		[4] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 4},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
		[5] = {nTime = 0, nNum = 1,
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str775[Lang.Idx]},
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 2, 1, 12, "boss1", "zya9139"},		-- 刷怪
				{XoyoGame.ADD_NPC, 4, 1, 5, "chumeng", "jing9141"},		-- 刷怪
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss1]])"},
			},
		},
		[6] = {nTime = 0, nNum = 1,
			tbPrelock = {5},
			tbStartEvent = 
			{
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 1, 1, 14, "boss", "zya8013"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
				{XoyoGame.ADD_NPC, 3, 1, 6, "rumeng", "hulu9140"},		-- 刷怪
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss]])"},
			},
		},
		[7] = {nTime = 0, nNum = 1,
			tbPrelock = {6},
			tbStartEvent = 
			{
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 2, 1, 12, "boss1", "zya9139"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss1]])"},
				{XoyoGame.ADD_NPC, 4, 1, 7, "chumeng", "jing9141"},		-- 刷怪
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss1]])"},
			},
		},
		[8] = {nTime = 0, nNum = 1,
			tbPrelock = {7},
			tbStartEvent = 
			{
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 1, 1, 14, "boss", "zya8013"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
				{XoyoGame.ADD_NPC, 3, 1, 8, "rumeng", "hulu9140"},		-- 刷怪
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss]])"},
			},
		},
		[9] = {nTime = 0, nNum = 1,
			tbPrelock = {8},
			tbStartEvent = 
			{
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 2, 1, 12, "boss1", "zya9139"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss1]])"},
				{XoyoGame.ADD_NPC, 4, 1, 9, "chumeng", "jing9141"},		-- 刷怪
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss1]])"},
			},
		},
		[10] = {nTime = 0, nNum = 1,
			tbPrelock = {9},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str776[Lang.Idx]},
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 1, 1, 10, "boss", "zya8013"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[11] = {nTime = 0, nNum = 1,
			tbPrelock = {10},
			tbStartEvent = 
			{
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 2, 1, 11, "boss1", "zya9139"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss1]])"},
			},
			tbUnLockEvent = 
			{      
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志              
                {XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},                                            
                {XoyoGame.DEL_NPC, "boss"}, 
                {XoyoGame.DEL_NPC, "boss1"},
                {XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"},                                                                 
                {XoyoGame.CLOSE_INFO, -1},                                                                 
                {XoyoGame.CLEAR_BUFF, -1},                                                                 
                {XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},                                                    
                {XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str777[Lang.Idx]},  
			},
		},
		[12] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[13] = {nTime = 0, nNum = 1,
			tbPrelock = {12},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"}, 
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 1, 1, 13, "boss", "zya8013"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志              
                {XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},                                            
                {XoyoGame.DEL_NPC, "boss"}, 
                {XoyoGame.DEL_NPC, "boss1"},
                {XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"},                                                                 
                {XoyoGame.CLOSE_INFO, -1},                                                                 
                {XoyoGame.CLEAR_BUFF, -1},                                                                 
                {XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},                                                    
                {XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str777[Lang.Idx]},  
			},
		},
		[14] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[15] = {nTime = 0, nNum = 1,
			tbPrelock = {14},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"}, 
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 2, 1, 15, "boss1", "zya9139"},		-- 刷怪
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志              
                {XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},                                            
                {XoyoGame.DEL_NPC, "boss"}, 
                {XoyoGame.DEL_NPC, "boss1"},
                {XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"},                                                               
                {XoyoGame.CLOSE_INFO, -1},                                                                 
                {XoyoGame.CLEAR_BUFF, -1},                                                                 
                {XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},                                                    
                {XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str777[Lang.Idx]},  
			},
		},
		[16] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[17] = {nTime = 0, nNum = 1,
			tbPrelock = {16},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"}, 
				{XoyoGame.CLEAR_BUFF, -1},
				{XoyoGame.ADD_NPC, 2, 1, 17, "boss1", "zya9139"},		-- 刷怪
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志              
                {XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},                                            
                {XoyoGame.DEL_NPC, "boss"}, 
                {XoyoGame.DEL_NPC, "boss1"},
                {XoyoGame.DEL_NPC, "rumeng"},
                {XoyoGame.DEL_NPC, "chumeng"},                                                               
                {XoyoGame.CLOSE_INFO, -1},                                                                 
                {XoyoGame.CLEAR_BUFF, -1},                                                                 
                {XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},                                                    
                {XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str777[Lang.Idx]},  
			},
		},
		[18] = {nTime = 1, nNum = 0,
			tbPrelock = {3},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "boss"},
			},
		},
		[19] = {nTime = 1, nNum = 0,
			tbPrelock = {5},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "boss1"},
			},
		},
		[20] = {nTime = 1, nNum = 0,
			tbPrelock = {6},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "boss"},
			},
		},
		[21] = {nTime = 1, nNum = 0,
			tbPrelock = {7},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "boss1"},
			},
		},
		[22] = {nTime = 1, nNum = 0,
			tbPrelock = {8},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "boss"},
			},
		},
		[23] = {nTime = 1, nNum = 0,
			tbPrelock = {9},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "boss1"},
			},
		},
	}
}