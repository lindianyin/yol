Include("script/mission/level/xoyogame_def.lua");
Include("script/mission/level/xmhj1/shirenhua.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[41] =    --食人花
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 4,						-- 房间等级(1~5)
	nMapIndex		= 42,						-- 地图组的索引
	tbBeginPoint	= {6283, 1337, 1068328},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	DerivedRoom		= XoyoGame.ShiRenHua,
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8064, nLevel = {8132,8133,8134}, nSeries = -1},		-- 吐毒花
		[2] = {nTemplate = 8065, nLevel = {8135,8136,8137}, nSeries = -1},		-- 巨型食人花
		[3] = {nTemplate = 9134, nLevel = {9051,9052,9053}, nSeries = -1},		-- 巨型食人花2
		[4] = {nTemplate = 9135, nLevel = {9054,9055,9056}, nSeries = -1},		-- 巨型食人花3
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
		[2] = {nTime = 270, nNum = 0,		-- 总计时
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.REVIVE_INFO, false, false, true, 97, 1897,2918, 1048576},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str716[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DEL_NPC, "boss1"},
				{XoyoGame.DEL_NPC, "boss2"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[6]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[8]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[10]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[11]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 17,		-- 刷小花
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 17, 3, "guaiwu", "tudu8132"},		-- 刷怪
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str717[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str639[Lang.Idx], 2},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str718[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[4] = {nTime = 0, nNum = 3,		-- 刷大花
			tbPrelock = {3},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 4, "boss", "juxingshirenhua8135"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str719[Lang.Idx]},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str639[Lang.Idx], 2},
				{XoyoGame.NPC_BLOOD_PERCENT,
					"boss",
					{50,XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss]])"},
					{50,XoyoGame.DO_SCRIPT, "self.tbLock[6]:UnLock()"},
				},
			},
			tbUnLockEvent = 
			{
			    {XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str720[Lang.Idx]},
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
		[6] = {nTime = 1, nNum = 0,
			tbPrelock = {4},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[7] = {nTime = 270, nNum = 0,
			tbPrelock = {6},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.ADD_NPC, 3, 1, 4, "boss1", "juxingshirenhua9134"},
				{XoyoGame.NPC_BLOOD_PERCENT,
					"boss1",
					{50,XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss1]])"},
					{50,XoyoGame.DO_SCRIPT, "self.tbLock[8]:UnLock()"},
				},	
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 1, nNum = 0,
			tbPrelock = {7},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[9] = {nTime = 270, nNum = 0,
			tbPrelock = {8},
			tbStartEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbLock[7]:Close()"},
				{XoyoGame.DEL_NPC, "boss1"},
				{XoyoGame.ADD_NPC, 4, 1, 4, "boss2", "juxingshirenhua9135"},
				{XoyoGame.NPC_BLOOD_PERCENT,
					"boss2",
					{50,XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss2]])"},
					{50,XoyoGame.DO_SCRIPT, "self.tbLock[10]:UnLock()"},
				},	
			},
			tbUnLockEvent = 
			{
			},
		},
		[10] = {nTime = 1, nNum = 0,
			tbPrelock = {9},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[11] = {nTime = 3, nNum = 0,
			tbPrelock = {10},
			tbStartEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbLock[9]:Close()"},
				{XoyoGame.DEL_NPC, "boss2"},	
			},
			tbUnLockEvent = 
			{
			},
		},
		[12] = {nTime = 5, nNum = 0,
			tbPrelock = {11},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 4, "boss", "juxingshirenhua8135"},
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[13] = {nTime = 5, nNum = 0,
			tbPrelock = {12},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 3, 1, 4, "boss1", "juxingshirenhua9134"},
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss1]])"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[14] = {nTime = 5, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 1, 4, "boss2", "juxingshirenhua9135"},
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss2]])"},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
}