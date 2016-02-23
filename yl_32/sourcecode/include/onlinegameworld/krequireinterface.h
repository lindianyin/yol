
#pragma once

#define KD_SERVER_OVERLOAD_CFG	"Overload"

#ifdef GAME_SERVER
	#define	KD_DEFAULT_MAX_PLAYER	500 // 默认player上限
	#define	KD_DEFAULT_MAX_NPC		1000 // 默认npc上限
	#define KD_DEFAULT_MAX_DOODAD	600 // 默认doodad上限

	#define KD_DEFAULT_MAX_WAIT_PLAYER 1000 // 默认等待连接的玩家上限
	#define KD_DEFAULT_VIP_RESERVATION 50 // VIP额外预留可登录人数
	#define KD_DEFAULT_START_SEVER_TIME "2014-08-01 12:00:00"
#else
// 客户端默认值
	#define	KD_DEFAULT_MAX_PLAYER	400
	#define	KD_DEFAULT_MAX_NPC		500
	#define KD_DEFAULT_MAX_DOODAD	300
#endif // GAME_SERVER

// 环境配置参数
struct KENV_CONFIG
{
	INT nMaxPlayer; // 最大单服玩家数
	INT nMaxNpc; // 最大单服承载Npc数
	INT nMaxDoodad; // 最大单服承载Doodad数
	INT nMaxItem; // 总道具数量
	DWORD dwStartSeverTime;//服务器开启时间

#ifdef GAME_SERVER
	INT nMaxWaitPlayer; // 等待连接的玩家上限
	INT nMaxVipPlayer; // vip玩家上限
	INT nCharacterArrayValueMaxIndexDw; // 角色四字节常用变量最大个数
	INT nCharacterArrayValueMaxIndexW; // 角色两字节常用变量最大个数
	INT nCharacterArrayValueMaxIndexBy; // 角色一字节常用变量最大个数
#endif // GAME_SERVER
};
class IKModuleInterface;
// -------------------------------------------------------------------------

class IKNsBaseRequire
{
public:
	// 获取环境配置参数
	virtual const KENV_CONFIG* GetEnvConfig() PURE;
	// 获取模块列表，以NULL结尾
	virtual IKModuleInterface** GetModuleList() PURE;
};

extern IKNsBaseRequire* g_piNsBaseRequire;
// -------------------------------------------------------------------------

