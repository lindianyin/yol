
#pragma once

#ifdef GAME_SERVER
#include "Runtime/qstring.h"
#include "onlinegameworld/kcharacterserver.h"
#include "onlinegameworld/ksceneobject.h"
#include "ProtocolBasic.h"


class KPlayer;
struct KCHARACTER_VAR_SYNC_OTHER;
struct KCHARACTER_VAR_SYNC_SELF;
class KPlayerProtocolProcessor;

#include <vector>
#define MAX_POSITION_RECORD	16

struct KDB_ROLE_SCENE;
struct KRoleBaseInfo;
struct KDelayRoleData;
struct KDelayPetData;

enum KEKICK_PLAYER_REASON
{
	emKICK_PLAYER_REASON_SCRIPT, // 脚本调用的（脚本内必须指出踢号原因）
	emKICK_PLAYER_REASON_GM, // GM系统
	emKICK_PLAYER_REASON_NETWORK, // 客户端网络故障或Coordinator要求等
	emKICK_PLAYER_REASON_PING_CHECK, // ping超时
	emKICK_PLAYER_REASON_ANTI_HACK, // 非法协议/客户端、外挂等
	emKICK_PLAYER_REASON_BUG_CHECK, // 内部故障，需要踢号
	emKICK_PLAYER_REASON_ALL
};

struct KROLE_POSITION
{
	DWORD	dwMapID;
	int	    nMapCopyIndex;
	int		nX;
	int		nY;
	int		nZ;
	int     nFaceDirection;
};

// -------------------------------------------------------------------------
// 玩家服务端扩展
// Login（客户端Attach）时，玩家对象激活，进行初始化工作，Logout时，玩家对象释放，Init和Release由玩家对象自己控制，外部不得调用
class KPlayerServer : public KCharacterServer
{
public:
	KPlayerServer(KPlayer& rcPlayer);
	LPCSTR GetAccount() { return m_strAccount; }
	// KQString GetRoleName() { return m_strRoleName; }
	// 单独同步一个变量
	BOOL SyncVarSelf(WORD wVarGroup, WORD wVarId);
	BOOL SyncKnockToSelf(INT nDestX, INT nDestY, INT nDestZ);
	BOOL SyncPullToSelf(INT nDestX, INT nDestY, INT nDestZ);

	// 玩家登入此服务器
	BOOL LoginServer2(IMemBlock* piRoleData);

	// 玩家从此服务器登出
	BOOL LogoutServer();
	BOOL TransferToBattleZone(INT nX, INT nY, INT nZ );
	BOOL DoMultiTransferRequest();
	// Synchronization
	BOOL SyncChangeRegion();
	BOOL SyncToOldNeighborPlayers(std::vector<KPlayer*>& vPlayers);
	BOOL SyncToNeighborPlayers(std::vector<KPlayer*>& vPlayers);
	BOOL FillSyncVar(KCHARACTER_VAR_SYNC_OTHER& rsVarSync, SIZE_T uInLen, SIZE_T& ruOutLen);
	// implementation of sync
	BOOL CheckMoveAdjust(DWORD dwClientFrame, const KMOVE_CRITICAL_PARAM& crParam);
	BOOL ReversePosition(DWORD dwFrame);
	BOOL RecordPosition();
	VOID ClearRecordPosition();

	BOOL TeleportTo2(INT nMapId, INT nMapCopyIndex, INT nX, INT nY, INT nZ); // 瞬移至
	BOOL PrepareTeleportTo(INT nMapId, INT nMapCopyIndex, INT nX, INT nY, INT nZ); // 瞬移至

	// 从外部数据（角色数据）导入到角色变量容器
	BOOL LoadTaskVar(LPCBYTE pData, SIZE_T uDataLen);
	// 把角色变量保存到指定缓冲区，用于存档（返回写入数据大小，0表示错误）
	SIZE_T SaveTaskVar(LPBYTE pBuffer, SIZE_T uBufSize);

	BOOL KickOut(KEKICK_PLAYER_REASON eReason);

	// 返回最近去过的普通场景（通常是从副本传送回城市）
	BOOL SaveLastEntry();
	BOOL ReturnToLastCity();
	BOOL DoMessageNotify(INT nModule, INT nMsgId, const void* pAdditional = 0, UINT uDataLen = 0);

	BOOL SyncSelfVar(); // 同步自己基本数据及变量数据
	BOOL SyncSelfScene(); // 同步自身场景
	BOOL FillSyncVar(KCHARACTER_VAR_SYNC_SELF& rsVarSync, SIZE_T uInLen, SIZE_T& ruOutLen);
	virtual BOOL DoSyncVarSelf(WORD wVarGroup, WORD wVarId);
public:
	VOID SetAccountCoin(INT nCoin) { m_AccountData.dwCoin = nCoin; }
	DWORD GetAccountCoin(){ return m_AccountData.dwCoin; }
	DWORD GetLimitPlayTimeFlag(){ return m_AccountData.dwLimitPlayTimeFlag; }
	DWORD GetLimitOnlineSecond(){ return m_AccountData.dwLimitOnlineSecond; }
	DWORD GetLimitOfflineSecond(){ return m_AccountData.dwLimitOfflineSecond; }
	INT	  GetAwardLimitFlag() { return m_nAwardLimitFlag; }
	VOID  SetAwardLimitFlag(INT nFlag) { m_nAwardLimitFlag = nFlag; }
	BOOL LoadBaseInfo(KRoleBaseInfo* pBaseInfo, SIZE_T uSize );
	BOOL SaveBaseInfo(KRoleBaseInfo* pBaseInfo);

	BOOL SaveDelayRoleData(KDelayRoleData* pDelayData);

	BOOL SavePosition();
	BOOL SetPositionForSaving(DWORD dwMapId, INT nX, INT nY, INT nZ); // 设置保存位置

	BOOL OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId);

	DWORD m_dwVirtualFrame;
	time_t m_timeLastPing;
	time_t m_timeLastSave;
	time_t m_timeLogin;
	DWORD m_dwLoginIp;
	INT m_nTargetBZoneId;
	DWORD m_dwCreateTime; // 角色创建时间

	// 保存角色数据,或者在Load时,先要把玩家的位置信息保存到这个结构里面,因为过程中
	// 需要访问的Scene,Cell等指针可能为空
	KROLE_POSITION		m_SavePosition;
	KROLE_POSITION      m_cDelayedTeleport;
	KROLE_POSITION*		m_pDelayedTeleport;
	KROLE_POSITION      m_LastEntry;
	QString				m_strAccount;
	GUID				m_Guid;
	time_t              m_nBanishTime; // 将被遣返回城的时间
	BOOL				m_bOnThisGs;	// 是否在本gs加载地图

private:
	BOOL SaveBaseDelayData(KDelayRoleData* pDelayData);

	BOOL SaveDelayPetData(KDelayRoleData* pDelayData);

	BOOL SaveDelayRideData(KDelayRoleData* pDelayData);

	BOOL SaveDelaySkillData(KDelayRoleData* pDelayData);

	BOOL SaveEquipDelayData(KDelayRoleData* pDelayData);

	INT GetDelayPetsNum(KDelayRoleData* pData);

	BOOL SyncDelayData(KPlayer* pPlayer, KDelayRoleData* pData);

	BOOL LoginFindSql(DWORD playerID); 

protected:
	KPlayer& m_rcPlayer;
	BOOL m_bSetSavePosition; // 如果为true，代表在外部设置了保存位置，优先使用这个

	struct KPOSITION_RECORD
	{
		DWORD				dwFrame;     // 这个变量只临时做调试的,检查是否出错:)
		KE_MOVE_STATE		eMoveState;
		INT                     nX;
		INT                     nY;
		INT                     nZ;
		INT                     nDestX;
		INT                     nDestY;
		INT                     nDestZ;
		INT                     nVelocityXY;
		INT                     nDirectionXY;
		INT                     nVelocityZ;
		INT                     nConvergenceSpeed;
		INT                     nCurrentGravity;
		INT                     nJumpCount;	
		INT                     nFaceDirection;
		INT                     nDivingCount;
		BOOL                    bTryPathState;
		INT                     nTryPathSide;
		BOOL                    bSlip;
		INT                     nCurrentTrack;
		INT                     nFromNode;
		INT                     nTargetCity;
		INT                     nMoveFrameCount;
		BOOL					bEffect;		// 是否还有效
	};
	// 重复定义 待清理
	struct  ACCOUNT_DATA
	{
		INT nGatewayId;
		INT nChargeFlag;
		tagExtPointInfo	ExtPointInfo;			    // 可用的附加点
		DWORD           dwEndTimeOfFee;             // 收费总截止时间
		DWORD           dwCoin;
		DWORD           dwLimitPlayTimeFlag;        // 防沉迷标志位
		DWORD           dwLimitOnlineSecond;        // 防沉迷累计在线时间
		DWORD           dwLimitOfflineSecond;       // 防沉迷累计离线时间
	};

	ACCOUNT_DATA		m_AccountData;
	INT					m_nAwardLimitFlag;
	KPOSITION_RECORD	m_PositionRecord[MAX_POSITION_RECORD];
	DWORD	m_dwRecordCount;

private:
	// load完基础属性后需要保存的属性，在各个模块的ctrl load完以后再设置上去
	struct KSAVE_DATA
	{
		INT nCurrentLife;
		INT nCurrentMana;
		INT nCurrentPower;
	};

	VOID SavePlayerBaseValue(KSAVE_DATA& sSaveData, KPlayer* pPlayer);

	VOID LoadPlayerBaseValue(KSAVE_DATA* pSaveData, KPlayer& rcPlayer);
};
// -------------------------------------------------------------------------
#endif // #ifdef GAME_SERVER

