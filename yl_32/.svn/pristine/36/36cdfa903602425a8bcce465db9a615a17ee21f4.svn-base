Include("script/mission/level/xoyogame_def.lua");
Include("script/mission/level/xmhj1/baguazhen.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[18] =    --八卦阵
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 3,						-- 房间等级(1~5)
	nMapIndex		= 14,						-- 地图组的索引
	tbBeginPoint	= {1367,6442,1053690},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	DerivedRoom		= XoyoGame.BaGuaZhen,
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8029, nLevel = {8067,8068,8069}, nSeries = -1},		-- 阵眼
		[2] = {nTemplate = 8059, nLevel = {8067,8068,8069}, nSeries = -1},		-- 阵眼（不可攻击）
		[3] = {nTemplate = 8030, nLevel = {8070,8071,8072}, nSeries = -1},		-- 乾卦怪
		[4] = {nTemplate = 8031, nLevel = {8073,8074,8075}, nSeries = -1},		-- 坤卦怪
		[5] = {nTemplate = 8032, nLevel = {8076,8077,8078}, nSeries = -1},		-- 坎卦怪
		[6] = {nTemplate = 8033, nLevel = {8079,8080,8081}, nSeries = -1},		-- 离卦怪
		[7] = {nTemplate = 8034, nLevel = {8082,8083,8084}, nSeries = -1},		-- 震卦怪
		[8] = {nTemplate = 8035, nLevel = {8085,8086,8087}, nSeries = -1},		-- 艮卦怪
		[9] = {nTemplate = 8036, nLevel = {8088,8089,8090}, nSeries = -1},		-- 巽卦怪
		[10] = {nTemplate = 9072, nLevel = {9039,9040,9041}, nSeries = -1},		-- 兑位石
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str604[Lang.Idx]},
				{XoyoGame.DO_SCRIPT, "self:GetRoomIndex()"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str605[Lang.Idx]},
				{XoyoGame.DO_SCRIPT, "for i=3 ,22 do self.tbLock[i]:Close() end"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "zhenyan"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[12]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[13]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[14]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[15]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[16]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[17]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[18]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[19]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[20]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[21]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[22]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[23]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[24]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[25]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[26]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[27]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
				{XoyoGame.NPC_BLOOD_PERCENT,
					"zhenyan",
					{75,XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan]])"},
					
					{75,XoyoGame.ADD_NPC, 2, 1, 0, "zhenyan1", "baguazhu8068"},
					{75,XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan1]])"},
					{75,XoyoGame.DO_SCRIPT, "self.tbLock[self.tbIndex[1]]:UnLock()"},
					--{75,XoyoGame.DO_SCRIPT, "self.tbLock[3]:Close()"},
					--{75,XoyoGame.DEL_NPC, "zhenyan"},
					{50,XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan]])"},
					--{50,XoyoGame.DEL_NPC, "zhenyan"},
					{50,XoyoGame.ADD_NPC, 2, 1, 0, "zhenyan1", "baguazhu8068"},
					{50,XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan1]])"},
					{50,XoyoGame.DO_SCRIPT, "self.tbLock[XoyoGame.BaGuaZhen.tbIndex[2]]:UnLock()"},
					--{50,XoyoGame.DO_SCRIPT, "self.tbLock[4]:Close()"},
					{25,XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan]])"},
					--{25,XoyoGame.DEL_NPC, "zhenyan"},
					{25,XoyoGame.ADD_NPC, 2, 1, 0, "zhenyan1", "baguazhu8068"},
					{25,XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan1]])"},
					{25,XoyoGame.DO_SCRIPT, "self.tbLock[XoyoGame.BaGuaZhen.tbIndex[3]]:UnLock()"},
					--{25,XoyoGame.DO_SCRIPT, "self.tbLock[5]:Close()"},
				},
				
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
				--{XoyoGame.CHANGE_FIGHT, -1, 1, Player.emKPK_STATE_PRACTISE},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
				
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
				{XoyoGame.DO_SCRIPT, "for i=4,22 do self.tbLock[i]:Close() end"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str607[Lang.Idx]},
			},
		},
		[4] = {nTime = 0, nNum = 0,   --乾卦
			tbPrelock = {},
			--tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[5] = {nTime = 0, nNum = 0,   --坤卦
			tbPrelock = {},
			--tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[6] = {nTime = 0, nNum = 0,   --坎卦
			tbPrelock = {},
			--tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[7] = {nTime = 0, nNum = 0,   --离卦
			tbPrelock = {},
			--tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 0, nNum = 0,   --震卦
			tbPrelock = {},
			--tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[9] = {nTime = 0, nNum = 0,   --艮卦
			tbPrelock = {},
			--tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[10] = {nTime = 0, nNum = 0,   --巽卦
			tbPrelock = {},
			--tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[11] = {nTime = 0, nNum = 0,   --兑卦
			tbPrelock = {},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[12] = {nTime = 0, nNum = 8,   --乾卦
			tbPrelock = {20},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str608[Lang.Idx],12},
				{XoyoGame.ADD_NPC, 3, 8, 12, "guaiwu", "qianbing8070"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "zhenyan1"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[13] = {nTime = 0, nNum = 8,    --坤卦
			tbPrelock = {21},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str609[Lang.Idx],13},
				{XoyoGame.ADD_NPC, 4, 8, 13, "guaiwu", "kunbing8073"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[14] = {nTime = 0, nNum = 8,    --坎卦
			tbPrelock = {22},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str610[Lang.Idx],14},
				{XoyoGame.ADD_NPC, 5, 8, 14, "guaiwu", "bingpo8076"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[15] = {nTime = 0, nNum = 8,    --离卦
			tbPrelock = {23},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str611[Lang.Idx],15},
				{XoyoGame.ADD_NPC, 6, 8, 15, "guaiwu", "huohun8079"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[16] = {nTime = 0, nNum = 8,    --震卦
			tbPrelock = {24},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str612[Lang.Idx],16},
				{XoyoGame.ADD_NPC, 7, 8, 16, "guaiwu", "leigui8082"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[17] = {nTime = 0, nNum = 8,    --艮卦
			tbPrelock = {25},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str613[Lang.Idx],17},
				{XoyoGame.ADD_NPC, 8, 8, 17, "guaiwu", "shiren8085"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[18] = {nTime = 0, nNum = 8,    --巽卦
			tbPrelock = {26},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str614[Lang.Idx],18},
				{XoyoGame.ADD_NPC, 9, 8, 18, "guaiwu", "fenglang8088"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[19] = {nTime = 0, nNum = 4,    --兑卦
			tbPrelock = {27},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str615[Lang.Idx],19},
				{XoyoGame.ADD_NPC, 10, 4, 19, "guaiwu", "zhenshi9072"},		-- 小怪
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[zhenyan1]])"},
				{XoyoGame.DEL_NPC, "zhenyan1"},
				{XoyoGame.ADD_NPC, 1, 1, 3, "zhenyan", "baguazhu8067"},		-- 阵眼
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[zhenyan]])"},
			},
		},
		[20] = {nTime = 5, nNum = 0,   --乾卦
			tbPrelock = {4},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[21] = {nTime = 5, nNum = 0,   --坤卦
			tbPrelock = {5},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[22] = {nTime = 5, nNum = 0,   --坎卦
			tbPrelock = {6},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[23] = {nTime = 5, nNum = 0,   --离卦
			tbPrelock = {7},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[24] = {nTime = 5, nNum = 0,   --震卦
			tbPrelock = {8},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[25] = {nTime = 5, nNum = 0,   --艮卦
			tbPrelock = {9},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[26] = {nTime = 5, nNum = 0,   --巽卦
			tbPrelock = {10},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[27] = {nTime = 5, nNum = 0,   --兑卦
			tbPrelock = {11},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.DEL_NPC, "zhenyan"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[28] = {nTime = 15, nNum = 0,
			tbPrelock = {2},
			tbStartEvent = 
			{
				{XoyoGame.TIME_INFO, -1, Lang.mission.str616[Lang.Idx], 28},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str603[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
			},
		},
		[29] = {nTime = 1, nNum = 0,
			tbPrelock = {12},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[30] = {nTime = 1, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[31] = {nTime = 1, nNum = 0,
			tbPrelock = {14},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[32] = {nTime = 1, nNum = 0,
			tbPrelock = {15},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[33] = {nTime = 1, nNum = 0,
			tbPrelock = {16},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[34] = {nTime = 1, nNum = 0,
			tbPrelock = {17},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[35] = {nTime = 1, nNum = 0,
			tbPrelock = {18},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
		[36] = {nTime = 1, nNum = 0,
			tbPrelock = {19},
			tbStartEvent = 
			{
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str617[Lang.Idx],3},
			},
			tbUnLockEvent = 
			{
			},
		},
	}
} 