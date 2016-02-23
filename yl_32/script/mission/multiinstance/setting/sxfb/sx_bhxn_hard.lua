-- 20级，困难，多人副本配置文件

Include("script/mission/multiinstance/multi_def.lua")

if not MultiInstance.tbSetting then
	MultiInstance.tbSetting = {}
end

local tbMultiSetting = MultiInstance.tbSetting;

tbMultiSetting[522]=
{
	nTimeLimit = 605,  --本层时间20分钟
	nMapId = 522,
    tbBeginPos = {7137, 7404, 1048576},
	nGameType = MultiInstance.GAME_TYPE.LV_20_EASY,
	nMultiLevel = 20,
	nDifficulty = 1,

    -- 副本涉及的NPC种类
	NPC =
	{
	-- 		编号  	npc模板				等级(-1默认随机npcpro表)	5行(默认-1)
        [1] = {nTemplate = 1250, nLevel = 50, nSeries = -1},		-- 狂暴黑熊
		[2] = {nTemplate = 1251, nLevel = 50, nSeries = -1},		-- 牛兵守卫
		[3] = {nTemplate = 1252, nLevel = 50, nSeries = -1},		-- 牛魔王
		[4] = {nTemplate = 1253, nLevel = 50, nSeries = -1},		-- 凶残鸟人
		[5] = {nTemplate = 1254, nLevel = 50, nSeries = -1},		-- 大头领
		[6] = {nTemplate = 1255, nLevel = 50, nSeries = -1},		-- 厚血蛤蟆
		[7] = {nTemplate = 1256, nLevel = 50, nSeries = -1},		-- 封印将军
		[8] = {nTemplate = 1257, nLevel = 50, nSeries = -1},		-- 大教主
		[9] = {nTemplate = 1258, nLevel = -1, nSeries = -1},		-- 神仙姐姐
		[10] = {nTemplate = 1259, nLevel = -1, nSeries = -1},		-- 传送点1
		[11] = {nTemplate = 1260, nLevel = -1, nSeries = -1},		-- 群攻炮台传送点
		[12] = {nTemplate = 1261, nLevel = -1, nSeries = -1},		-- 减速炮台传送点
		[13] = {nTemplate = 1262, nLevel = -1, nSeries = -1},		-- 单攻炮台
		[14] = {nTemplate = 1263, nLevel = -1, nSeries = -1},		-- 群攻炮台
		[15] = {nTemplate = 1264, nLevel = -1, nSeries = -1},		-- 减速炮台
		[16] = {nTemplate = 1265, nLevel = -1, nSeries = -1},		-- 单攻魔法蛋1
		[17] = {nTemplate = 1266, nLevel = -1, nSeries = -1},		-- 群攻魔法蛋1
		[18] = {nTemplate = 1267, nLevel = -1, nSeries = -1},		-- 减速魔法蛋1
		[19] = {nTemplate = 1268, nLevel = -1, nSeries = -1},		-- 加攻buff
		[20] = {nTemplate = 1269, nLevel = -1, nSeries = -1},		-- 全部怪伤害buff
		[21] = {nTemplate = 1270, nLevel = -1, nSeries = -1},		-- 全部怪定身buff
		[22] = {nTemplate = 1271, nLevel = -1, nSeries = -1},		-- 猴子偷桃
		[23] = {nTemplate = 1272, nLevel = -1, nSeries = -1},		-- 单攻魔法蛋2
		[24] = {nTemplate = 1273, nLevel = -1, nSeries = -1},		-- 群攻魔法蛋2
		[25] = {nTemplate = 1274, nLevel = -1, nSeries = -1},		-- 减速魔法蛋2
		[26] = {nTemplate = 1275, nLevel = -1, nSeries = -1},		-- 单攻魔法蛋3
		[27] = {nTemplate = 1276, nLevel = -1, nSeries = -1},		-- 群攻魔法蛋3
		[28] = {nTemplate = 1277, nLevel = -1, nSeries = -1},		-- 减速魔法蛋3
		[29] = {nTemplate = 1278, nLevel = -1, nSeries = -1},		-- 自杀
		[30] = {nTemplate = 1279, nLevel = -1, nSeries = -1},		-- 被杀
		[31] = {nTemplate = 1280, nLevel = -1, nSeries = -1},		-- 帮玩家加隐形属性
		[32] = {nTemplate = 1281, nLevel = -1, nSeries = -1},		-- 起点传送
	},

    LOCK =
    {
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = 5, nNum = 0,		-- 总计时
			tbPrelock = {},
			tbStartEvent =
			{
			    {MultiInstance.ADD_NPC, 31, 1, 1, "yinxingBUFF", "yinxingBUFF"},          --帮玩家添加一个隐形BUFF
				{MultiInstance.TIME_INFO, Lang.mission.str886[Lang.Idx],1},
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str887[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			--{MultiInstance.DEL_NPC, "yinxingBUFF"},             --删除帮玩家加隐形BUFF的NPC
			},
		},
		[2] = {nTime = 900, nNum = 0 ,
			tbPrelock = {1},
			tbStartEvent =
			{
                {MultiInstance.TIME_INFO, Lang.mission.str888[Lang.Idx]},
				{MultiInstance.REVIVE_INFO, true, true, false},

				{MultiInstance.ADD_NPC, 11, 1, 2, "11", "paotaTransmit2"}, --回到起点（困难）

			},
			tbUnLockEvent =
			{
			},
		},
		--第1波 20只狂暴黑熊
		[3] = {nTime = 0, nNum = 20 ,
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 1, 1, 3, "1", "npcbornplace", 20,2,"1_timer",0},  --狂暴黑熊  --20
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str889[Lang.Idx]},
				{MultiInstance.TARGET_INFO, Lang.mission.str890[Lang.Idx]},

			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "111","11_timer"},                         --删掉童女
			},
		},
		--第2波开始计时器
		[4] = {nTime = 170, nNum = 1 ,
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		--第2波 时间到了，开始刷24只牛兵守卫和1只牛魔王
		[5] = {nTime = 0, nNum = 21 ,
			tbPrelock = {4},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 2, 1, 5, "2", "npcbornplace",20,2,"2_timer",0},  --牛兵守卫   --25
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str891[Lang.Idx]},
				{MultiInstance.TARGET_INFO,Lang.mission.str892[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "222","22_timer"},                              --删掉童女
			},
		},
		--精英刷出计时器
		[6] = {nTime = 25, nNum = 0 ,
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[7] = {nTime = 0, nNum = 100 ,
			tbPrelock = {6},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 3, 1, 5, "3", "jingying"}, 					 --牛魔王
			},
			tbUnLockEvent =
			{
			},
		},
		--第3波刷出计时器，继承第5步
		[8] = {nTime = 180, nNum = 1 ,
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},

		--第3波 时间到了，开始刷30只凶残鸟人和2只大头领
		[9] = {nTime = 0, nNum = 20 ,
			tbPrelock = {8},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 4, 1, 9, "4", "npcbornplace",20,2,"4_timer",0},  --凶残鸟人   --30
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str893[Lang.Idx]},
				{MultiInstance.TARGET_INFO, Lang.mission.str894[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "333","33_timer"},                              --删掉童女
			},
		},
		--精英第1次刷出计时器
		[10] = {nTime = 10, nNum = 0 ,
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[11] = {nTime = 0, nNum = 100 ,
			tbPrelock = {10},
			tbStartEvent =
			{
				--{MultiInstance.ADD_NPC, 5, 1, 9, "5", "npcbornplace",2,25,"5_timer",0}, 					 --每隔10s刷一只大头领
			},
			tbUnLockEvent =
			{
			},
		},
		--第4波刷出计时器，继承第9步
		[12] = {nTime = 180, nNum = 1 ,
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		--第4波 时间到了，开始刷32只厚血蛤蟆和3只封印将军
		[13] = {nTime = 0, nNum = 22 ,
			tbPrelock = {12},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 6, 1, 13, "6", "npcbornplace",20,3,"6_timer",0},  --龟     --32
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str895[Lang.Idx]},
				{MultiInstance.TARGET_INFO, Lang.mission.str896[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "444","44_timer"},                                 --删掉童女
			},
		},
		--精英刷出计时器
		[14] = {nTime = 15, nNum = 0 ,
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[15] = {nTime = 0, nNum = 100 ,
			tbPrelock = {14},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 7, 1,13, "7", "jingying",2,30,"7_timer",0}, 					 --封印将军
			},
			tbUnLockEvent =
			{
			},
		},
		--第5波刷出计时器
		[16] = {nTime = 200, nNum = 1 ,
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		--第5波 时间到了，开始刷大boss
		[17] = {nTime = 0, nNum = 1 ,
			tbPrelock = {16},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 8, 1, 17, "8", "npcbornplace"},  --大教主
				{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str897[Lang.Idx]},
				{MultiInstance.TARGET_INFO,Lang.mission.str898[Lang.Idx]},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			{MultiInstance.DEL_NPC, "555","55_timer"},                                           --删掉童女
			},
		},

		--【补】定时刷出 信春鸽 ，击杀信春鸽有几率触发增益buff
		[18] = {nTime = 15, nNum = 0 ,
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
		[19] = {nTime = 30, nNum = 10 ,
			tbPrelock = {16},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
			--{MultiInstance.ADD_NPC, 22, 1, 19, "22", "npcbornplace", 3, 30, "22_timer",0}, --定时刷出信春鸽
			},
		},
		--【补】时间到了，神仙姐姐没死 算胜利                1.计时锁，从头到尾都存在，解锁事件里设置bFinish=1
		[20] = {nTime = 900, nNum = 0 ,
			tbPrelock = {1},
			tbStartEvent =
			{

			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		},
		--【补】神仙姐姐在副本时间内死亡 算失败                2.神仙死亡锁，nNum设为1，解锁事件里设置bFailFinish=1
		[21] = {nTime = 0, nNum = 1 ,
			tbPrelock = {1},
			tbStartEvent =
			{
				{MultiInstance.ADD_NPC, 9, 1, 21, "9", "shenxianjiejie"}, --神仙姐姐
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFailFinish= 1"},
				{MultiInstance.DEL_NPC, "1"},
				{MultiInstance.DEL_NPC, "2"},
				{MultiInstance.DEL_NPC, "3"},
				{MultiInstance.DEL_NPC, "4"},
				{MultiInstance.DEL_NPC, "6"},
				{MultiInstance.DEL_NPC, "7"},
				{MultiInstance.DEL_NPC, "8"},

			},
		},
		--【补】副本时间内，杀死所有怪  算胜利               3.打死怪物计数锁，解锁时设置bFinish=1
		[22] = {nTime = 0, nNum = 100 ,
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
				{MultiInstance.DO_SCRIPT, "self.bFinish = 1"},
			},
		},
		[23] = {nTime = 10, nNum = 0 ,          --{第一波自杀怪}
			tbPrelock = {3},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 29, 1, 4, "zisha", "zisha"},
			},
			tbUnLockEvent =
			{
			},
		},
		[24] = {nTime = 10, nNum = 0 ,          --{第二波自杀怪}
			tbPrelock = {5},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 29, 1, 8, "zisha1", "zisha"},
			},
			tbUnLockEvent =
			{
			},
		},
		[25] = {nTime = 10, nNum = 0 ,          --{第三波自杀怪}
			tbPrelock = {9},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 29, 1, 12, "zisha2", "zisha"},
			},
			tbUnLockEvent =
			{
			},
		},
		[26] = {nTime = 10, nNum = 0 ,          --{第四波自杀怪}
			tbPrelock = {13},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 29, 1, 16, "zisha3", "zisha"},
			},
			tbUnLockEvent =
			{
			},
		},
		[27] = {nTime = 30, nNum = 0 ,          --{第一波怪童女}
			tbPrelock = {1},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 22, 1, 19, "111", "hou", 3, 35, "11_timer",0},            --童女
			},
		},
		[28] = {nTime = 30, nNum = 0 ,          --{第二波怪童女}
			tbPrelock = {4},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 22, 1, 19, "222", "hou", 3, 35, "22_timer",0},            --童女
			},
		},
		[29] = {nTime = 25, nNum = 0 ,          --{第三波怪童女}
			tbPrelock = {8},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 22, 1, 19, "333", "hou", 3, 35, "33_timer",0},            --童女
			},
		},
		[30] = {nTime = 40, nNum = 0 ,          --{第四波怪童女}
			tbPrelock = {12},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 22, 1, 19, "444", "hou", 3, 35, "44_timer",0},           --童女
			},
		},
		[31] = {nTime = 40, nNum = 0 ,          --{第五波怪童女}
			tbPrelock = {16},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			{MultiInstance.ADD_NPC, 22, 1, 19, "555", "hou", 2, 40, "55_timer",0},           --童女
			},
		},
		[32] = {nTime = 0, nNum = 1 ,          --{第五波怪童女}
			tbPrelock = {1},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 30, 3, 32, "beisha", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "beisha"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str899[Lang.Idx]},
			},
		},
		[33] = {nTime = 0, nNum = 1 ,          --{第五波怪童女}
			tbPrelock = {32},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 30, 3, 33, "beisha1", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "beisha1"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str900[Lang.Idx]},
			},
		},
		[34] = {nTime = 0, nNum = 1 ,          --{第五波怪童女}
			tbPrelock = {33},
			tbStartEvent =
			{
			{MultiInstance.ADD_NPC, 30, 3, 34, "beisha2", "beisha"},
			},
			tbUnLockEvent =
			{
			{MultiInstance.DEL_NPC, "beisha2"},
			{MultiInstance.SHOW_NOTICE_MSG, Lang.mission.str901[Lang.Idx]},
			},
		},
    }
}
