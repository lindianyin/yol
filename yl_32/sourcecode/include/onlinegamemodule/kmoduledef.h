
#pragma once

// 修改此枚举需要更新协议版本号kprotocol.h
// 各模块实现IKModuleInterface* GetModuleInterFace<>()接口，并在kmodulelist.cpp中加入模块列表
// -------------------------------------------------------------------------
enum KE_MODULE_ID
{
	emKMODULE_NONE			= 0,
	emKMODULE_RPG			= 1,	// 基础服务模块，包括Timer等基础功能

	emMODULE_TEAM			= 3,
	emKMODULE_ITEM			= 4,

	//emKMODULE_FACTION,			//职业系统
	emKMODULE_TASK			= 6,	//任务系统
	emKMODULE_PURSE			= 7,	//钱袋
	emKMODULE_CHAT_NEW		= 8,	// 聊天
	emKMODULE_GLOBALDATA	= 9,	// 全局变量
	emKMODULE_MAP			= 10,
	emKMODULE_AI			= 11,	//AI
	emKMODULE_MAIL			= 12,	//邮件
	emKMODULE_FINDPATH		= 13,   //寻路
	emMODULE_BUFFER_		= 14,   // 游龙Buffer
	emMODULE_FIGHT_SKILL	= 15,   // 游龙战斗技能
	emMODULE_FACTION		= 16,   // 游龙门派系统
	emMODULE_SHOP			= 17,   // 商店
	emMODULE_MYSTERYSHOP	= 18,   // 神秘商店////////////////////////////记得去掉
	emMODULE_KIN			= 19,	// 家族
	emMODULE_RELATION		= 20,   // 好友关系
	emMODULE_PET			= 21,	// 宠物
	emMODULE_AUTOMATIC		= 22,	// 自动任务只占位
	emMODULE_ESCORT			= 23,	// 修仙
	emMODULE_AUCTION		= 24,	// 拍卖行
	emMODULE_AWARD			= 25,	// 奖励

	emMODULE_QUIZ			= 26,   // 问答题
	
	emMODULE_ARENA			= 28,  	// PVE竞技场

	emMODULE_TOWER			= 29,	// 伐魔塔
	emMODULE_STAGE			= 30,	// 关卡

	emMODULE_TITLE			= 31,	//称号
	
	emMODULE_INSTANCE		= 32,	// 副本
	
	emMODULE_MEDITATION		= 33,	//打坐

	emMODULE_REMIND			= 34,	//提醒

	emMODULE_RANK			= 35,	// 排行榜

	emMODULE_TARGET			= 36,	//目标

	emMODULE_ACTIVITY		= 37,	// 活动

	emMODULE_VIP			= 38,	//vip

	emMODULE_AWARD_LOBBY	= 39,	//奖励大厅

	emMODULE_BLADE			= 40,	// 变身

	emMODULE_GEM			= 41,	//宝石

	emMODULE_ARTIFACT		= 42,	//神器

	emMODULE_BANK			= 43,	//钱庄

	emMODULE_Opense         = 44,	//开服活动
	
	// 最大为127, 扩展模块
	emKGAMEEXT_RESERVED		 = 126,	//GameExtension保留
	emKMAX_ID = 127,
};

class IKModuleInterface;
extern IKModuleInterface** g_GetModuleList();

