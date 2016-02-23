
#pragma once

#include "onlinegameworld/kprotocol.h"
#include "onlinegameworld/kplayerprotocolprocess.h"
#include "config/kcharacterdef.h"
#include "config/gamedef.h"
#include "config/kplayerbasedbdef.h"

extern KPlayerProtocolProcessor* g_pBaseSender;

#pragma pack(push, 1)
// -------------------------------------------------------------------------
enum KE_PLAYERBASE_C2S_PROTOCOL
{
	emKPLBASE_PTC_C2S_NONE = 0,
	emKPLBASE_PTC_C2S_LOGIN,
	emKPLBASE_PTC_C2S_SYNCEND,	// 客户端加载完资源后通知server准备就绪

	emKPLBASE_PTC_C2S_PING,
	emKPLBASE_PTC_C2S_JUMP,
	emKPLBASE_PTC_C2S_MOVE,
	emKPLBASE_PTC_C2S_MOVE_CTRL,
	emKPLBASE_PTC_C2S_DIALOG_DOODAD, // 与Doodad交互
	emKPLBASE_PTC_C2S_REQUEST_OFFLINE,

	emKPLBASE_PTC_C2S_CALL_SERVER_SCRIPT,//客户端调用服务端脚本
	emKPLBASE_PTC_C2S_APPLYNPCTALK,		//与NPC会话
	emKPLBASE_PTC_C2S_APPLY_SYNC_NEIGHBOR,		// 申请同步附近对象
	emKPLBASE_PTC_C2S_APPLY_SET_VAR,		// 请求设置角色变量SetVar
	
	emKPLBASE_PTC_C2S_APPLY_REVIVE,		//请求复活
	emPLAYERBASE_C2S_DIALOG_SELECT, // c#客户端选择对话框选项
	emPLAYERBASE_C2S_PICKUP_DOODAD,	// 拾取物品

	emPLAYERBASE_C2S_ACTION_AROUND,		// 请求向周围广播动作

	emPLAYERBASE_C2S_GET_DELAYDATA,			// 获取离线数据
	
	
	emPLAYERBASE_C2S_PROCESS_DAILY_TASK,	//处理日常任务

	emKPLBASE_PTC_C2S_END,
	emKPLBASE_PTC_C2S_MAX = KD_MAX_BASE_PROTOCOL_HEADER, // 最大66
};

enum KE_PLAYERBASE_S2C_PROTOCOL
{
	emPLAYERBASE_S2C_NONE = 0,
	emPLAYERBASE_S2C_PLAYERDATA,

	emPLAYERBASE_S2C_PING,					// 回ping
	emKPBP_S2C_FRAME_SIGNAL,		// 帧同步信号
	emPLAYERBASE_S2C_CHANGE_SERVER,			// 跨服通知

	emKPBP_S2C_SYNC_SELF,		// 场景同步 自己位置状态
	emKPBP_S2C_SYNC_LEAVE, // 场景同步 sobj离开场景
	emKPBP_S2C_SYNC_PLAYER, // 场景同步 新玩家
	emKPBP_S2C_SYNC_NPC, // 场景同步 新Npc
	emKPBP_S2C_SYNC_DOODAD, // 场景同步 新doodad

	emKPBP_S2C_SYNC_MOVE, // 场景同步 sobj移动
	emKPBP_S2C_SYNC_JUMP, // 场景同步 sobj跳跃
	emKPBP_S2C_SYNC_JUMP_TO, // 场景同步 sobj跳跃至 (test)
	emKPBP_S2C_SYNC_ACTION,	// 场景同步 sobj做动作
	emKPBP_S2C_SYNC_FACE_DIRECTION, // 场景同步 非运动时的朝向改变
	emKPBP_S2C_SYNC_ADJUST_MOVE, // 场景同步 校正自己移动状态
	emKPBP_S2C_SYNC_TELEPORT, // 场景同步 sobj场景内瞬移

	emKPBP_S2C_SYNC_VAR_SELF,	//同步变量改变到自身
	emKPBP_S2C_SYNC_VAR_AROUND,	//同步变量改变到周围玩家
	
	emKPBP_S2C_RESPONSE_OFFLINE,
	emKPLBASE_PTC_S2C_CALL_CLIENT_SCRIPT,
	emKPBP_S2C_SYNC_VISIBLE, // 隐身状态
	emKPBP_S2C_SYNC_NPC_BORN, //通知NPC出生事件
	emPLAYERBASE_S2C_SHOW_DIALOG, // 对c#客户端发送对话框
	emPLAYERBASE_S2C_FACE_TALK, // 发送Dialog:Talk
	emPLAYERBASE_S2C_MESSAGE_NOTIFY, // 模板化通用消息
	emPLAYERBASE_S2C_NPC_TALK,	// 发送NPC对白
	emPLAYERBASE_S2C_BLACK_MSG, // 黑条字幕
	emPLAYERBASE_S2C_NOTICE_MSG,	// 公告
	emPLAYERBASE_S2C_SYNC_HEAD_TEXT,	// 头顶显示伤害数字
	emPLAYERBASE_S2C_SYNC_DEGREE,		// 同步次数

	emKPBP_S2C_SYNC_DELAYROLEDATA,		// 发送离线玩家数据

	emPLAYERBASE_S2C_SYNC_DAILY_TASK_LIST, //发送日常任务数据


	

	emKPLBASE_PTC_S2C_MAX = KD_MAX_BASE_PROTOCOL_HEADER, // 最大66
};

struct KMAP_VAR_VALUE
{
	BYTE byVarGroup;
	BYTE byVarId;
	DWORD dwVarValue;
};

struct KCHARACTER_VAR_SYNC_SELF
{
	BYTE aByVarSet[countof(csByteVarSyncSelf)];
	WORD aWVarSet[countof(csWordVarSyncSelf)];
	DWORD aDwVarSet[countof(csDWordVarSyncSelf)];
	WORD wMapVarNum;
	KMAP_VAR_VALUE aMapVarSet[1];
};
struct KCHARACTER_VAR_SYNC_OTHER
{
	BYTE aByVarSet[countof(csByteVarSyncOther)];
	WORD aWVarSet[countof(csWordVarSyncOther)];
	DWORD aDwVarSet[countof(csDWordVarSyncOther)];
	WORD wMapVarNum;
	KMAP_VAR_VALUE aMapVarSet[1];
};

/*___________________________ client -> server __________________________________*/

// 玩家跳跃
struct KC2S_PING : KPTC_HEADER<emKPLBASE_PTC_C2S_PING>
{
	DWORD dwClientTime;
};

// 玩家跳跃
struct KC2S_JUMP : KPTC_HEADER<emKPLBASE_PTC_C2S_JUMP>
{
	DWORD dwJumpCount;
	INT	nMoveState;

	INT	nX;
	INT	nY;
	INT	nZ;

	DWORD dwFrame;
};

// 玩家移动
struct KC2S_MOVE : KPTC_HEADER<emKPLBASE_PTC_C2S_MOVE>
{
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nDestX;
	INT	nDestY;
	INT	nDestZ;
	INT	nMoveMode;
	INT	nMoveState;

	DWORD dwFrame;
	INT nMoveCount;
	INT nMapId;
	INT nSpeed;
};

struct KC2S_MOVE_CTRL : KPTC_HEADER<emKPLBASE_PTC_C2S_MOVE_CTRL>
{
	INT nWalk;
	INT nMove;
	INT nDirection;
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nMoveState;

	DWORD dwFrame;
	INT nMoveCount;
};

struct KC2S_DIALOG_DOODAD : KPTC_HEADER<emKPLBASE_PTC_C2S_DIALOG_DOODAD>
{
	DWORD dwDoodadId;
};

// 玩家下线
struct KC2S_REQUESTOFFLINE : KPTC_HEADER<emKPLBASE_PTC_C2S_REQUEST_OFFLINE>
{
};

struct KC2SSYNC_END  : KPTC_HEADER<emKPLBASE_PTC_C2S_SYNCEND>
{
	DWORD dwHwId[4];
};

struct KC2S_LOGIN : KPTC_HEADER<emKPLBASE_PTC_C2S_LOGIN>
{
	DWORD	dwRoleId;
	GUID	guid;
	BYTE	nHidden;				// 是否隐身，1：隐身，0：不隐身
	BYTE	byGameServerVersion;	// 客户端记录的GameServer版本
	DWORD   dwTinyTerminal;         // 是否是微端,  1:是微端 0:不是微端
};

struct KC2S_APPLYNPCTALK : KPTC_HEADER<emKPLBASE_PTC_C2S_APPLYNPCTALK>
{
	DWORD dwNpcId;
};

// 申请同步附近对象，消除部分不同步问题
struct KC2S_APPLY_SYNC_NEIGHBOR : KPTC_HEADER<emKPLBASE_PTC_C2S_APPLY_SYNC_NEIGHBOR>
{
	DWORD dwId;
};

struct KC2S_APPLY_SET_VAR : KPTC_HEADER<emKPLBASE_PTC_C2S_APPLY_SET_VAR>
{
	WORD wVarGroupId;
	WORD wVarId;
	DWORD dwValue;
};

struct KC2S_APPLY_REVIVE : KPTC_HEADER<emKPLBASE_PTC_C2S_APPLY_REVIVE>
{
	INT32 nChoice;
};


struct KC2S_SCRIPT_REMOTECALL : KLENPTC_HEADER<emKPLBASE_PTC_C2S_CALL_SERVER_SCRIPT>
{
	BYTE	arBuffer[];
};

struct KC2S_DIALOG_SELECT : KPTC_HEADER<emPLAYERBASE_C2S_DIALOG_SELECT>
{
	BYTE bySelect; // 选项
};

struct KC2S_PICKUP_DOODAD : KPTC_HEADER<emPLAYERBASE_C2S_PICKUP_DOODAD>
{
	DWORD dwId;
};

struct KC2S_GET_DELAYDATA : KPTC_HEADER<emPLAYERBASE_C2S_GET_DELAYDATA>
{
	DWORD dwSrcId;
	DWORD dwRoleId;
	CHAR szRoleName[MAX_NAME_LEN];
};

struct KC2S_ACTION_AROUND : KPTC_HEADER<emPLAYERBASE_C2S_ACTION_AROUND>
{
	BYTE byActionId;
	INT pendantId;
};


//日常任务action
enum KE_PLAYERBASE_C2S_DAILDY_TASK_ACTION
{
	KE_PLAYERBASE_C2S_DAILDY_TASK_NONE,
	KE_PLAYERBASE_C2S_DAILDY_TASK_GET_TASK_LIST,//获取日常任务列表

	
	KE_PLAYERBASE_C2S_DAILDY_TASK_END,
};


//处理每日任务
struct KC2S_PROCESS_DAILY_TASK : KPTC_HEADER<emPLAYERBASE_C2S_PROCESS_DAILY_TASK>
{
	BYTE byActionId;
};


//日常任务奖励
struct DTASK_REWARD
{
    DWORD itemType;
	DWORD itemCount;
};
//日常任务详细信息
struct DTASK_DETAIL
{
	BYTE byTaskLevel;//任务等级
	BYTE byTaskStatus;//任务状态 1.待接受  2.已接受  4. 已完成可领取奖励 8.已经领取
	BYTE byTaskTimes;//任务完成次数
	BYTE byTaskId;//任务对应的基础任务编号
};

enum emDTASK_STATUS
{
	DTASK_PRE_ACCEPT = 1,//1.待接受 
	DTASK_ACCEPT = 2,//2.已接受
	DTASK_CAN_GETREWARD = 4,//4. 已完成可领取奖励
	DTASK_FINISH = 8,//8.已经领取
};



//日常任务条目
struct KS2C_DTASK_ITEM
{
	DTASK_DETAIL dtaskDetail;
	DTASK_REWARD reward[2];//任务对应的奖励
	char szDesc[50];//日常任务描述	
};	


//日常任务结构
struct KS2C_SYNC_DAILY_TASK_LIST  : KPTC_HEADER<emPLAYERBASE_S2C_SYNC_DAILY_TASK_LIST>
{
	KS2C_DTASK_ITEM dwDailyTaskList[4];
	DWORD dwLastDtaskTimes;//剩余任务次数
	DWORD dwLastDtaskLing;//当前任务令
	DWORD dwLastTimeRefleshTime;//上一次刷新时间
	DWORD dwNowJingpo;//当前精魄	
};

//日常任务随机表结构
struct DTASK_RAND
{
	int nLevel;
	int nRate;
};





/*___________________________ server -> client __________________________________*/
// 玩家连上gs时同步玩家数据
struct KS2C_PLAYERDATA  : KLENPTC_HEADER<emPLAYERBASE_S2C_PLAYERDATA>
{
	DWORD dwPlayerId;
	CHAR szPlayerName[MAX_NAME_LEN];
	//// scene
	//DWORD dwSceneId;
	//INT	nX;
	//INT	nY;
	//INT	nZ;
	//INT	nVelocityXY;
	DWORD dwLogicFrames;
	INT nServerId;
	DWORD dwServerTime; // 服务器时间
	DWORD dwKinId;
	INT nTeamId;
	DWORD dwStartServerTime;

	// 下面的包含变长部分，要放到最后
	union
	{
		CHAR buf[1];
		KCHARACTER_VAR_SYNC_SELF sVarSync;
	};
};

// 同步自己的位置
struct KS2C_SYNC_SELF  : KPTC_HEADER<emKPBP_S2C_SYNC_SELF>
{
	DWORD dwSceneId;
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nDirectionXY;
	DWORD dwLogicFrames;
	INT	nMapCopyIndex;
};

struct KS2C_SYNC_DEGREE : KPTC_HEADER<emPLAYERBASE_S2C_SYNC_DEGREE>
{
	CHAR szModuleName[MAX_NAME_LEN];
	INT nCurDegree;
	INT nMaxDegree;
};

// 回复ping包，dwClientTime原封不动送回去
struct KS2C_PING  : KPTC_HEADER<emPLAYERBASE_S2C_PING>
{
	DWORD dwClientTime;
};

struct KS2C_FRAME_SIGNAL  : KPTC_HEADER<emKPBP_S2C_FRAME_SIGNAL>
{
	DWORD dwLogicFrames;
};

struct KS2C_CHANGE_SERVER  : KPTC_HEADER<emPLAYERBASE_S2C_CHANGE_SERVER>
{
	GUID guid;
	DWORD dwPlayerId;
	UINT uServerIp;
	WORD	wPort;
	INT nCode; // 临时：跨线结果
};

// 同步玩家移动
struct KS2C_SYNC_MOVE  : KPTC_HEADER<emKPBP_S2C_SYNC_MOVE>
{
	DWORD dwId;
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nDestX;
	INT	nDestY;
	INT	nDestZ;
	INT	nMoveState;
	INT	nMoveMode;
};
// 同步玩家跳跃
struct KS2C_SYNC_JUMP  : KPTC_HEADER<emKPBP_S2C_SYNC_JUMP>
{
	DWORD dwId;
	INT	nJumpCount;
};
// 同步非运动时的朝向变更
struct KS2C_SYNC_FACE_DIRECTION  : KPTC_HEADER<emKPBP_S2C_SYNC_FACE_DIRECTION>
{
	DWORD dwId;
	INT	nFaceDirection;
};
// 同步玩家跳跃
struct KS2C_SYNC_JUMP_TO  : KPTC_HEADER<emKPBP_S2C_SYNC_JUMP_TO>
{
	DWORD dwId;
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nDestX;
	INT	nDestY;
	INT	nDestZ;
};

// 同步新玩家
struct KS2C_SYNC_PLAYER  : KLENPTC_HEADER<emKPBP_S2C_SYNC_PLAYER>
{
	DWORD dwPlayerId;
	CHAR	szName[MAX_NAME_LEN];
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nDestX;
	INT	nDestY;
	INT	nDestZ;
	INT	nMoveState;
	INT	nVelocityXY;
	INT	nDirectionXY;

	DWORD dwKinId;
	INT nTeamId;

	// 下面的包含变长部分，要放到最后
	union
	{
		CHAR buf[1];
		KCHARACTER_VAR_SYNC_OTHER sVarSync;
	};
};

struct KS2C_SYNC_NPC  : KLENPTC_HEADER<emKPBP_S2C_SYNC_NPC>
{
	DWORD dwId;
	DWORD dwTemplateId;
	CHAR szName[MAX_NAME_LEN];
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nDestX;
	INT	nDestY;
	INT	nDestZ;
	INT	nMoveState;
	INT	nVelocityXY;
	INT	nDirectionXY;

	// 下面的包含变长部分，要放到最后
	union
	{
		CHAR buf[1];
		KCHARACTER_VAR_SYNC_OTHER sVarSync;
	};
};

struct KS2C_SYNC_DOODAD  : KPTC_HEADER<emKPBP_S2C_SYNC_DOODAD>
{
	DWORD dwId;
	DWORD dwTemplateId;
	INT	nX;
	INT	nY;
	INT	nZ;
	INT nGenre;
	INT nDetail;
	INT nParticular;
	INT nLevel;
	INT nAmount;
	BYTE byAutoPickUp;
	/*
	INT	nDestX;
	INT	nDestY;
	INT	nDestZ;
	INT	nMoveState;
	INT	nVelocityXY;
	INT	nDirectionXY;
	INT	nDoodadState;
	*/
};

struct KS2C_SYNC_LEAVE  : KPTC_HEADER<emKPBP_S2C_SYNC_LEAVE>
{
	DWORD dwId;
};

// 这个协议只适用同场景瞬移，跨场景是发送删除和添加协议
struct KS2C_SYNC_TELEPORT  : KPTC_HEADER<emKPBP_S2C_SYNC_TELEPORT>
{
	DWORD	dwId;
	INT	nX;
	INT	nY;
	INT	nZ;
};

struct KS2C_SYNC_ADJUST_MOVE  : KPTC_HEADER<emKPBP_S2C_SYNC_ADJUST_MOVE>
{
	INT	nX;
	INT	nY;
	INT	nZ;
	INT	nMoveState;
	DWORD	dwVirtualFrame;
	INT	nVelocityXY;
	INT	nDirectionXY;
	INT	nDestX;
	INT	nDestY;
	INT	nDestZ;
	INT	nMoveCount;
};

struct KS2C_SCRIPT_REMOTECALL : KLENPTC_HEADER<emKPLBASE_PTC_S2C_CALL_CLIENT_SCRIPT>
{
	BYTE	arBuffer[];
};

struct KS2C_SHOW_DIALOG : KLENPTC_HEADER<emPLAYERBASE_S2C_SHOW_DIALOG>
{
	BYTE	arBuffer[];
};

struct KS2C_FACE_TALK : KLENPTC_HEADER<emPLAYERBASE_S2C_FACE_TALK>
{
	BYTE	arBuffer[];
};

struct KS2C_BLACK_MSG : KLENPTC_HEADER<emPLAYERBASE_S2C_BLACK_MSG>
{
	BYTE arBuffer[];
};

struct KS2C_NOTICE_MSG : KLENPTC_HEADER<emPLAYERBASE_S2C_NOTICE_MSG>
{
	BYTE arBuffer[];
};

struct KS2C_SYNC_VAR_SELF : KPTC_HEADER<emKPBP_S2C_SYNC_VAR_SELF>
{
	BYTE byVarGroup;
	BYTE byVarId;
	DWORD dwVarValue;
};

struct KS2C_SYNC_VAR_AROUND : KPTC_HEADER<emKPBP_S2C_SYNC_VAR_AROUND>
{
	DWORD dwId;
	BYTE byVarGroup;
	BYTE byVarId;
	DWORD dwVarValue;
};

//应答客户端的下线请求
struct KS2C_RESPONSE_OFFLINE : KPTC_HEADER<emKPBP_S2C_RESPONSE_OFFLINE> 
{
};

struct KS2C_SYNC_VISIBLE : KPTC_HEADER<emKPBP_S2C_SYNC_VISIBLE>
{
	DWORD dwId;
	BYTE byVisible;
};

struct S2C_MESSAGE_NOTIFY :KLENPTC_HEADER<emPLAYERBASE_S2C_MESSAGE_NOTIFY> 
{
	BYTE    byModule;
	BYTE	byMsgId;
	BYTE    byData[];
};

struct KS2C_SYNC_HEAD_TEXT : KPTC_HEADER<emPLAYERBASE_S2C_SYNC_HEAD_TEXT>
{
	DWORD dwCharacterId;	// 头顶冒字角色id
	INT nType;				// 冒文字类型
	INT nDamage;			// 伤害值
};

struct KS2C_SYNC_ACTION_AROUND : KPTC_HEADER<emKPBP_S2C_SYNC_ACTION>
{
	DWORD dwCharacterId;
	BYTE byActionId;
	INT nPendantId;
};

struct KS2C_SYNC_DELAYROLEDATA : KLENPTC_HEADER<emKPBP_S2C_SYNC_DELAYROLEDATA>
{
	KDelayBaseData sBaseData;

	KDelayEquipData arrEquips[MAX_DELAY_EQUIP_COUNT];

	KDelayRideData sRideData;

	BYTE byPetCount;
	KDelayPetData arrPets[];
};

// -------------------------------------------------------------------------
#pragma pack(pop)
