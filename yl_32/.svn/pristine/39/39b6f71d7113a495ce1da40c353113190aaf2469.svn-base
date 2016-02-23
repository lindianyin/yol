
#pragma once

#include "Runtime/qstring.h"
#include "config/kplayerdef.h"
#include "onlinegameworld/kcharacter.h"

#include "onlinegameworld/kplayerserver.h"
#include "onlinegameworld/kplayerclient.h"

struct IMemBlock;
class KLuaPlayer;
class KDoodad;
class KLuaCharacter;

// 玩家的游戏状态
enum PLAYER_GAME_STATE
{
	emPLAYERSTATE_BEGIN,			// 初始状态，不接收登录以外的协议 0

	gsWaitForConnect,			// 正在等待客户端的连接 1
	gsWaitForPermit,		    // 登陆时正在等待GC的确认 2
	gsWaitForRoleData,		    // 登录时正在等待Goddess的回包 3
	gsWaitForLoginLoading,      // 登录(或跨服)流程中等待客户端载入 4
	gsWaitForNewMapLoading,     // 切换地图过程中等待客户端载入 5

	gsPlaying,				    // 正在游戏中 6

	gsSearchMap,			    // 正在搜索地图,不一定需要跨服 7
	gsWaitForTransmissionSave,  // 保存数据准备跨服 8
	gsWaitForTransmissionGuid,	// 跨服时正在等待目标服务器返回GUID  9 

	gsWaitForBZoneTransferSave, // 战区跨服保存 10 

	gsDeleting,				    // 客户端断线引起的延时登出 11

};

// -------------------------------------------------------------------------
class KPlayer : public KCharacter
{
public:
	KD_CHARACTER_TASKFUN_U(OnlineTime,			emKVARGROUP_PLAYER,	emKPLAYER_TASK_ONLINE_TIME)
	KD_CHARACTER_TASKFUN_U(LastUpdateMysteryShopTime,	emKVARGROUP_PLAYER,	emKPLAYER_LAST_UPDATE_MYSTERY_SHOP_TIME)
public:
	INT GetPlayerIndex() const { return m_nIndex; }
	DWORD GetPlayerId() const { return m_dwId; }
	// Player Game State
	PLAYER_GAME_STATE GetGameState() const { return m_eGameState; }
	VOID SetGameState(PLAYER_GAME_STATE eState) { QLogPrintf(LOG_INFO, "SetGameState %d %d\n", m_dwId, eState); m_eGameState = eState; }

	// 获得脚本接口
	virtual KLuaCharacter* GetScriptInterface() {return (KLuaCharacter*)m_pLuna;}
public:
	KPlayer();
	virtual ~KPlayer();

#ifdef GAME_SERVER
	KPlayerServer m_cPlayerServer;
	friend class KPlayerServer;

	KDoodad* m_pDoodad;

	virtual BOOL LogicGoTo(INT nX, INT nY, INT nZ); // 临时跟随优化
#endif
#ifdef GAME_CLIENT
	KPlayerClient m_cPlayerClient;
	friend class KPlayerClient;
	BOOL ClientGoTo(INT nX, INT nY, INT nZ);
	BOOL ClientJump();
#endif

	BOOL Activate();
	BOOL OnRemove();
	BOOL GoTo(INT nX, INT nY, INT nZ, BOOL bSyncSelf = FALSE);
	BOOL StopMove(BOOL bSyncSelf /* = FALSE */);
	BOOL Jump(BOOL bSyncSelf = FALSE);
	BOOL JumpTo(INT nX, INT nY, INT nZ);
	BOOL KnockTo(INT nX, INT nY, INT nZ, INT nDirection);
	BOOL Pull(INT nX, INT nY, INT nZ, INT nDirection);
	BOOL OnTrap(LPCSTR pszTrapScript);
	BOOL OnLeaveTrap(LPCSTR pszTrapScript);
	virtual	BOOL	OnReachTarget();

	BOOL ForwardPosition(int nFrame);

protected:
	BOOL Init(INT nPlayerIndex, const QString& strName);
	BOOL Uninit();

	BOOL	ChangeRegion(KRegion* pDestRegion);
protected:
	PLAYER_GAME_STATE	m_eGameState;

	INT					m_nIndex; // PlayerIndex
	KLuaPlayer*			m_pLuna; // 脚本指针

public:
	struct KMOVE_CTRL 
	{
		BOOL bDisable;      // 禁止玩家作主动控制
		BOOL bMove;         // 是否在移动
		BOOL bWalk;         // 是否在Walk(否则是Run,或者Swim)
		int  nDirection;   // 移动的方向
		int  nMoveCount;    // 客户端移动操作计数器
		BOOL bUpdate;

		INT	nDestX;
		INT	nDestY;
		INT	nDestZ;
		INT	nMoveState;
	};
	KMOVE_CTRL m_sMoveCtrl;
	void MoveCtrl();

	friend class KPlayerMgr;

	DWORD m_dwTeamId;
	DWORD m_dwSystemTeamId;
	DWORD m_dwKinId;
	DWORD m_dwKinMasterId;
	BYTE m_byIsKinMaster;

	INT				m_nTimer; // 跨服等闲置状态计时器
public:
	//场景对象距离过远时 走近再出场景对象对话框，这里记录走路停止后 需要延迟对话的场景对象ID
	DWORD GetDelayTalkObjId()						{ return m_dwDelayTalkObjId; }
	DWORD GetSetDelayTalkObjIdFrame()				{ return m_dwSetDelayTalkObjIdFrame; }
	VOID  SetDelayTalkObjId(DWORD dwDelayTalkNpcId);

	VOID SetPlayerTeamId(BOOL bSystem, DWORD dwTeamID)
	{
		if (bSystem)
		{
			m_dwSystemTeamId = dwTeamID;
		}
		m_dwTeamId = dwTeamID;

		SetTeamId(dwTeamID);
	}
	DWORD GetTeamId()
	{
		if (m_dwSystemTeamId != KD_BAD_ID)
			return m_dwSystemTeamId;
		return m_dwTeamId;
	}

	DWORD GetSysTeamId()
	{
		return m_dwSystemTeamId;
	}

	DWORD GetKinId()
	{
		return m_dwKinId;
	}

	BOOL IsKinMaster()
	{
		return (m_dwKinMasterId == GetId());
	}

	DWORD GetKinMasterId() { return m_dwKinMasterId; }

	//原来KPlayerUiScript::ApplyTalk()NPC会话接口移到这里
	BOOL ApplyNpcTalk(DWORD dwNpcId);

	//打开doodad
	BOOL ApplyOpenDoodad(DWORD dwDoodadId);

	//检测并设置延迟会话信息，返回TRUE表示有设置，返回FALSE表示未设置
	//nDistance2Limit表示会话距离的平方，防止内部做开方操作
	BOOL CheckAndSetDelayOpen(KSceneObject* pScenceObj, INT32 nDistanceLimit = KD_MIN_SCENCE_OBJ_TALK_DISTANCE);

	//走路停止时的回调
	BOOL OnPlayerMoveStop();

	//目标点改变回调，主要是玩家鼠标左击触发
	BOOL OnTargetChange();

	//是否可以移动
	virtual BOOL	CanMove();
private:
	//移动停止时，会话的场景对象ID
	DWORD m_dwDelayTalkObjId;
	//设置m_dwDelayTalkObjId的逻辑帧
	DWORD m_dwSetDelayTalkObjIdFrame;

private:
	std::map<INT, INT> m_mapTrapInvalid;	// 无效的trap点id集合

public:
	VOID SetTrapInvalid(INT nTemplateId);
	VOID RemoveTrapInvalid(INT nTemplateId);
	BOOL IsTrapInvalid(INT nTemplateId);

private:
	std::map<DWORD, INT> m_mapDoodadIds;		// 掉落的道具id集合

public:
	VOID AddDropDoodadId(DWORD dwDoodadId);

	VOID RemoveDropDoodadId(DWORD dwDoodadId);

	VOID PickUpAllDoodad();

};

