Include("script/mission/level/xoyogame_def.lua");
Include("script/mission/level/xmhj1/wuou.lua");

local tbRoom = XoyoGame.RoomSetting.tbRoom;

tbRoom[26] =    --巫偶娃娃
{
	fnPlayerGroup 	= nil,						-- 玩家分群函数,不填则默认1支队伍1个群体
	fnDeath			= nil,						-- 房间死亡脚本; 不填则默认
	fnWinRule		= nil,						-- 胜利条件，竞赛类的房间需要重定义，其他一般不需要填
	nRoomLevel		= 4,						-- 房间等级(1~5)
	nMapIndex		= 17,						-- 地图组的索引
	tbBeginPoint	= {784, 3410, 1054720},	-- 起始点，格式根据fnPlayerGroup需求而定，默认是{nX,nY}
	DerivedRoom		= XoyoGame.WuOu,
	-- 房间涉及的NPC种类
	NPC = 
	{
-- 		编号  	npc模板		等级(-1默认)(强度表id列)	5行(默认-1)
-- E.g  [0] = {nTemplate, 			nLevel, 		nSeries }
		[1] = {nTemplate = 8042, nLevel = {8106,8107,8108}, nSeries = -1},		-- 稻草人
		[2] = {nTemplate = 8041, nLevel = {8103,8104,8105}, nSeries = -1},		-- 伦巴
		[3] = {nTemplate = 8051, nLevel = {8126,8126,8126}, nSeries = -1},		-- 毒罐
		[4] = {nTemplate = 9136, nLevel = {8103,8104,8105}, nSeries = -1},    --喷发人
		[5] = {nTemplate = 9137, nLevel = {8103,8104,8105}, nSeries = -1},     --封锁人
		[6] = {nTemplate = 9138, nLevel = {8103,8104,8105}, nSeries = -1},		-- 变大BOSS
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
				{XoyoGame.MOVIE_DIALOG, -1, Lang.mission.str736[Lang.Idx]},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, "<playername>: 此处不宜久留，我们还是先退出去吧<end>"},
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "boss"},
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
				{XoyoGame.DO_SCRIPT, "self.tbLock[28]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[29]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[30]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[31]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[32]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[33]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[34]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[35]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[36]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[37]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[38]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[39]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[40]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[41]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[42]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[43]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[44]:Close()"},
			},
		},
		[3] = {nTime = 0, nNum = 1,
			tbPrelock = {1},
			--tbPrelock = {},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 3, "boss", "wuou8103"},		-- 刷怪
				{XoyoGame.TARGET_INFO, -1, "打倒那个巨大的怪物",3},
				{XoyoGame.TIME_INFO, -1, Lang.mission.str601[Lang.Idx], 2},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.MOVIE_DIALOG, -1, "<playername>: 真是诡异的地方<end>"},
				{XoyoGame.DO_SCRIPT, "self.tbTeam[1].bIsWiner = 1"},		-- 完成任务设置标志
				{XoyoGame.DEL_NPC, "guaiwu"},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.CLOSE_INFO, -1},
				{XoyoGame.TARGET_INFO, -1, Lang.mission.str602[Lang.Idx]},
				{XoyoGame.DO_SCRIPT, "self.tbLock[2]:Close()"},
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
				{XoyoGame.DO_SCRIPT, "self.tbLock[28]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[29]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[30]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[31]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[32]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[33]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[34]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[35]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[36]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[37]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[38]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[39]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[40]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[41]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[42]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[43]:Close()"},
				{XoyoGame.DO_SCRIPT, "self.tbLock[44]:Close()"},
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
		[5] = {nTime = 30, nNum = 0,
			tbPrelock = {1},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str740[Lang.Idx]},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss]])"},
			},
		},
		[6] = {nTime = 10, nNum = 0,
			tbPrelock = {5},
			tbStartEvent = 
			{
				{XoyoGame.SYS_MSG, -1, Lang.mission.str741[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[7] = {nTime = 10, nNum = 0,
			tbPrelock = {6},
			tbStartEvent = 
			{
				{XoyoGame.SYS_MSG, -1, Lang.mission.str742[Lang.Idx]},
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[8] = {nTime = 10, nNum = 0,
			tbPrelock = {7},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[9] = {nTime = 30, nNum = 0,
			tbPrelock = {5},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 5, 14, 0, "fengsuo", "fengsuo9137"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "fengsuo"},
			},
		},
		[10] = {nTime = 10, nNum = 0,
			tbPrelock = {39},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str743[Lang.Idx]},
				{XoyoGame.ADD_NPC, 4, 3, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[11] = {nTime = 10, nNum = 0,
			tbPrelock = {10},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 3, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[12] = {nTime = 10, nNum = 0,
			tbPrelock = {11},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 3, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[13] = {nTime = 30, nNum = 0,
			tbPrelock = {12},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 3, "boss", "wuou8103"},
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str744[Lang.Idx]},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss]])"},
			},
		},
		[14] = {nTime = 10, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
				{XoyoGame.SYS_MSG, -1, Lang.mission.str745[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[15] = {nTime = 10, nNum = 0,
			tbPrelock = {14},
			tbStartEvent = 
			{
				{XoyoGame.SYS_MSG, -1, Lang.mission.str746[Lang.Idx]},
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[16] = {nTime = 10, nNum = 0,
			tbPrelock = {15},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[17] = {nTime = 30, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 5, 14, 0, "fengsuo", "fengsuo9137"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "fengsuo"},
			},
		},
		[18] = {nTime = 10, nNum = 0,
			tbPrelock = {40},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 4, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[19] = {nTime = 10, nNum = 0,
			tbPrelock = {18},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 4, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[20] = {nTime = 10, nNum = 0,
			tbPrelock = {19},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 4, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[21] = {nTime = 30, nNum = 0,
			tbPrelock = {20},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 2, 1, 3, "boss", "wuou8103"},
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str747[Lang.Idx]},
				{XoyoGame.DO_SCRIPT, "self:RecordBlood([[boss]])"},
			},
		},
		[22] = {nTime = 10, nNum = 0,
			tbPrelock = {21},
			tbStartEvent = 
			{
				{XoyoGame.SYS_MSG, -1, Lang.mission.str748[Lang.Idx]},
				{XoyoGame.DEL_NPC, "boss"},
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[23] = {nTime = 10, nNum = 0,
			tbPrelock = {22},
			tbStartEvent = 
			{
				{XoyoGame.SYS_MSG, -1, Lang.mission.str749[Lang.Idx]},
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[24] = {nTime = 10, nNum = 0,
			tbPrelock = {23},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 1, 6, 0, "guaiwu", "daocaoren8106"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[25] = {nTime = 30, nNum = 0,
			tbPrelock = {21},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 5, 14, 0, "fengsuo", "fengsuo9137"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "fengsuo"},
			},
		},
		[26] = {nTime = 10, nNum = 0,
			tbPrelock = {41},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[27] = {nTime = 10, nNum = 0,
			tbPrelock = {26},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[28] = {nTime = 10, nNum = 0,
			tbPrelock = {27},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[29] = {nTime = 100, nNum = 0,
			tbPrelock = {28},
			tbStartEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str750[Lang.Idx]},
				{XoyoGame.ADD_NPC, 6, 1, 3, "boss", "wuou9138"},
				{XoyoGame.DO_SCRIPT, "self:SetNpcBlood([[boss]])"},
			},
			tbUnLockEvent = 
			{
			},
		},
		[30] = {nTime = 10, nNum = 0,
			tbPrelock = {28},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
				{XoyoGame.BLACK_MSG, -1, Lang.mission.str751[Lang.Idx]},
			},
		},
		[31] = {nTime = 10, nNum = 0,
			tbPrelock = {30},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[32] = {nTime = 10, nNum = 0,
			tbPrelock = {31},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[33] = {nTime = 10, nNum = 0,
			tbPrelock = {32},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[34] = {nTime = 10, nNum = 0,
			tbPrelock = {33},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[35] = {nTime = 10, nNum = 0,
			tbPrelock = {34},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[36] = {nTime = 10, nNum = 0,
			tbPrelock = {35},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[37] = {nTime = 10, nNum = 0,
			tbPrelock = {36},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[38] = {nTime = 10, nNum = 0,
			tbPrelock = {37},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 4, 5, 0, "penfa", "penfa9136"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "penfa"},
			},
		},
		[39] = {nTime = 5, nNum = 0,
			tbPrelock = {5},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[40] = {nTime = 5, nNum = 0,
			tbPrelock = {13},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[41] = {nTime = 5, nNum = 0,
			tbPrelock = {21},
			tbStartEvent = 
			{
			},
			tbUnLockEvent = 
			{
			},
		},
		[42] = {nTime = 30, nNum = 0,
			tbPrelock = {28},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 5, 14, 0, "fengsuo", "fengsuo9137"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "fengsuo"},
			},
		},
		[43] = {nTime = 30, nNum = 0,
			tbPrelock = {42},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 5, 14, 0, "fengsuo", "fengsuo9137"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "fengsuo"},
			},
		},
		[44] = {nTime = 30, nNum = 0,
			tbPrelock = {43},
			tbStartEvent = 
			{
				{XoyoGame.ADD_NPC, 5, 14, 0, "fengsuo", "fengsuo9137"},
			},
			tbUnLockEvent = 
			{
				{XoyoGame.DEL_NPC, "fengsuo"},
			},
		},
	}
} 