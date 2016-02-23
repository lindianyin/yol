
/************************************************************************/
#ifndef _KSCENEOBJECT_H_
#define	_KSCENEOBJECT_H_

#include "Misc/utilities.h"
#include "scene_def.h"
#include "kbaseobject.h"
#include "ksceneobjnode.h"
#include "kmath.h"
#include "Runtime/qstring.h"


struct KCell;
class KRegion;
class IKScene;
class KScene;

//重生信息
struct KREGENERATION_INFO
{
	KREGENERATION_INFO()
	{
		::memset(this, 0, sizeof(KREGENERATION_INFO));
	}
	//DWORD		dwTemplateId;	// Npc模板Id
	INT			nX;				// 逻辑坐标x
	INT			nY;				// y
	INT			nZ;				// z	
	INT			nFaceDirection; // 面部朝向
	INT			nGenerationTime;// 重生时间,单位秒
	INT			nNormalAIType;	//Normal AI type
	INT			nRelation;		//Relation

	DWORD		dwExValue;		// 重生附加信息
	CHAR		szExInfo[MAX_NAME_LEN];	// 重生附加信息
};

enum KE_MOVE_STATE
{
	cmsInvalid,

	// 受控状态
	cmsOnStand,					// 站立状态
	cmsOnWalk,					// 走路状态
	cmsOnRun,					// 跑步状态
	cmsOnJump,                  // 跳跃状态
	cmsOnSwimJump,              // 水中跳跃
	cmsOnSwim,                  // 水中游泳
	cmsOnFloat,                 // 水中悬浮
	cmsOnSit,					// 坐状态

	// 不受控状态
	cmsOnChannelSkill,          // 发引导技能状态（可走路打断）
	cmsOnSkill,					// 发技能状态, 此状态下不能再多触发一次技能

	cmsOnKnockedDown,			// 被击倒状态
	cmsOnKnockedBack,			// 被击退状态
	cmsOnKnockedOff,			// 被击飞状态 
	cmsOnHalt,                  // 定身(眩晕、笑穴等等)
	cmsOnFreeze,                // 不能动(固定动作)
	cmsOnEntrap,                // 不能移动
	cmsOnAutoFly,               // 自动移动,比如坐车等

	cmsOnDeath,					// 死亡状态
	cmsOnDash,					// 冲刺状态
	cmsOnPull,                  // 被抓状态，被别人抓住自己拉到对方身边
	cmsOnRepulsed,              // 滑步状态，一个会减速的状态

	cmsOnRise,                  // 爬起状态
	cmsOnSkid,                  // 停滑状态

	cmsTotal,
};

class KSceneObject : public KBaseObject
{
public:
	KSceneObject();
	virtual ~KSceneObject();
	
	virtual	BOOL	Init();
	virtual	BOOL	Uninit();
	virtual	BOOL	Activate();
	BOOL	CheckGameLoop();

	virtual	BOOL	GoTo(INT x, INT y, INT z, BOOL bSyncSelf = FALSE);
	virtual	BOOL	Jump(BOOL bSyncSelf = FALSE);
	BOOL Jump(BOOL bStandJump, INT nJumpDirection, BOOL bSyncSelf);
	virtual	BOOL	JumpTo(INT nX, INT nY, INT nZ); // Test
	virtual BOOL	KnockTo(INT nX, INT nY, INT nZ, INT nDirection);
	virtual BOOL	Pull(INT nX, INT nY, INT nZ, INT nDirection);
	virtual	INT GetMoveSpeed() const { return m_nMoveSpeed; }
	virtual	BOOL	SetMoveSpeed(INT nMoveSpeed) { m_nMoveSpeed = nMoveSpeed; return TRUE; }
	virtual	BOOL	StopMoving();
	virtual	BOOL	ProcessMove();

	BOOL CheckObstacleDest(INT nStartX, INT nStartY, INT nStartZ, INT nEndX, INT nEndY, INT nEndZ, INT nTouchRange, INT& nLastOkX, INT& nLastOkY);

	inline INT GetVelocityXY() const { return m_nVelocityXY / VELOCITY_ZOOM_COEFFICIENT; } // 单位 点每帧
	inline INT GetVelocityZ() const { return ZPOINT_TO_XYPOINT(m_nVelocityZ); } // 单位 点每帧

	virtual	BOOL	OnRemove()
	{
		Q_Printl(std::hex << "Sobj[0x" << m_dwId << "] OnRemove.");
		Uninit();
		return FALSE;
	} 
	// Trap点
	virtual	BOOL	OnTrap(LPCSTR pszScript)
	{
		//if (IS_PLAYER(GetId()))
		//	QLogPrintf(LOG_DEBUG, "[0x%x][%s] OnEnterTrap [%s]", m_dwId, GetName(), pszScript);
		return FALSE;
	}
	virtual	BOOL	OnLeaveTrap(LPCSTR pszScript)
	{
		//if (IS_PLAYER(GetId()))
		//	QLogPrintf(LOG_DEBUG, "[0x%x][%s] OnLeaveTrap [%s]", m_dwId, GetName(), pszScript);
		return FALSE;
	}
	// 移动状态变化
	virtual	BOOL	OnMoveStateChanged(KE_MOVE_STATE eMoveState)
	{
		QLogPrintf(LOG_DEBUG, "[0x%x][%s] MoveStateChanged -> %d", m_dwId, GetName(), eMoveState);
		return FALSE;
	}
	virtual	BOOL	OnReachTarget()
	{
		Q_Printl("[ReachTarget] [" << m_nX << "," << m_nY << "," << m_nZ << "].");
		return FALSE;
	}

	 // 是否在目标附近(单位cm 默认1Region)
	virtual	BOOL	IsNearby(KSceneObject* pTarget, INT nDistance = REGION_GRID_WIDTH * LOGICAL_CELL_CM_LENGTH);
	virtual BOOL	IsNearbyIn2D(KSceneObject* pTarget, INT nDistance);
	// 判断是否处于同步范围内
	BOOL IsWithinSyncRange(KSceneObject* pTarget);
	BOOL IsVisible() { return m_bVisible; }
	virtual VOID SetVisible(BOOL bVisible) { m_bVisible = bVisible; }
	inline INT GetFaceDirection() const { return m_nFaceDirection; }
	// 获得目的地坐标
	inline BOOL	GetDestination(INT& rnX, INT& rnY, INT& rnZ) { rnX = m_nDestX; rnY = m_nDestY; rnZ = m_nDestZ; return TRUE; }
	// 获得逻辑位置
	inline BOOL	GetPosition(INT& rnX, INT& rnY, INT& rnZ) 
	{
		rnX = m_nX; rnY = m_nY; rnZ = 1048576;//m_nZ; //TODO: 强制设成0高度
		return TRUE; 
	}
	// 设置逻辑位置(注意：因为这一层不处理同步，瞬移应该调用Player和Npc的TeleportTo接口而不是这个)
	BOOL	SetPosition(INT x, INT y, INT z);

	BOOL    IsSwim() { return m_IsSwim; }

	DWORD GetSceneId();
	inline IKScene*	GetScene() { return (IKScene*)m_pScene; }

	inline KE_MOVE_STATE GetMoveState() const { return m_eMoveState; }
	virtual void SetMoveState(KE_MOVE_STATE eMoveState)
	{
		QCONFIRM_RET(eMoveState > cmsInvalid && eMoveState < cmsTotal, );
		//printf("movestate %d,%d\n", eMoveState, m_eMoveState);
		if (eMoveState != m_eMoveState)
		{
			OnMoveStateChanged(eMoveState);
			m_eMoveState = eMoveState;
		}
	}
	INT MoveMode() const { return m_nMoveMode; }
	void SetMoveMode(INT val) { m_nMoveMode = val; }
	// 普通GetName返回字符串指针，避免直接用KQString导致的格式化输出，void指针转换等问题
	inline LPCSTR GetName() const { return m_strName; }
	const QString& GetStrName() const { return m_strName; }
	inline LPCSTR GetNickName() const { return m_strNickName; }
	BOOL SetNickName(LPCSTR pszNickName);

public:
	INT			m_nTouchRange;      // 受击范围，触及范围，物体半径
	INT			m_nDestDistance;
	INT			m_nClimbAbility; // 爬升能力

	// 当前Region中的格子坐标，Region左下角为原点
	INT			m_nXCell;			// Region中的格子X坐标		单位：格子
	INT			m_nYCell;			// Region中的格子Y坐标		单位：格子

	INT			m_nMass;			// 质量						单位：g
	
	INT			m_nWidth;			// 宽度，X轴上的				单位：点
	INT			m_nLength;			// 长度，Y轴上的				单位：点
	INT			m_nHeight;			// 厚度，Z轴上的				单位：点
	
	INT			m_nPitchDirection;  // 俯仰角,单位(360/256)度

	BOOL		m_bTryPathState;            // 是否正在绕路过程之中
	INT			m_nTryPathSide;             // 正在往左还是往右绕路
	INT			m_nTryPathStartFrame;	// 绕路起始帧
	//INT			m_nTryPathDestX;            // 绕路开始的时候的移动目标点X
	//INT			m_nTryPathDestY;            // 绕路开始的时候的移动目标点Y
	QString	m_strName;
	QString	m_strNickName; // 别名
	BOOL		m_bVisible;	// 是否可见（对所有人不可见，友方也看不到）
	INT			m_nDestX;
	INT			m_nDestY;
	INT			m_nDestZ;
	KScene*		m_pScene;			// 所属的地图

	INT m_nMoveMode; // 目前只有走0跑1

	BOOL       m_IsSwim; // 是否游泳中
public:
	INT				m_nVelocityXY;  // 水平速度
	INT				m_nDirectionXY; // 水平速度方向
	INT				m_nFaceDirection;	// 物体正面的朝向 单位: (360/256)度
	INT				m_nVelocityZ;   // 垂直速度
	INT				m_nGravity; // 重力参数
	INT				m_nCurrentGravity;		// 当前重力
	INT				m_nJumpSpeed;               // 跳跃的垂直初速度
	INT				m_nMoveSpeed;               // 水平运动速度

	KE_MOVE_STATE		m_eMoveState;	// 当前移动状态
	DWORD	m_dwBroadcastFrame;

	INT				m_nStopedFaceDirection;	// 运动停止时物体正面的朝向 单位: (360/256)度

	INT			m_nX;
	INT			m_nY;
	INT			m_nZ;

	KCell*						m_pCell;			// 所属的Cell
	KRegion*					m_pRegion;			// 所属的Region
	KRegion*                    m_pLastRegion;      // 上次所属的Region
	SceneObjNode		m_cRegionNode;	// 用于维护KRegion中的场景对象链表
	

	std::vector<INT>		m_vRegionEnterFrameTable;
	std::vector<INT>		m_vRegionLeaveFrameTable;
	DWORD					m_dwActiveFrame;		// 游戏循环数，主要用来判断重复计算
	LPCSTR m_pszLastTrap;

public:
	//设置重生信息
	BOOL				SetRegenerationInfo(const KREGENERATION_INFO& info);
	//获取重生信息
	KREGENERATION_INFO* GetRegenerationInfo(){ return m_pRegenerationInfo; }
private:
	//重生信息
	KREGENERATION_INFO*		m_pRegenerationInfo;//用指针管理，没重生信息的对象不需要空间

protected:
	BOOL	TryMove(int nStepLength, int nDirection, int* pnObstacleDirection);
	BOOL	CellMoveStupid(int nStepLength);
	BOOL	ProcessHorizontalMoveStupid();
	BOOL	ProcessVerticalMove();
	BOOL	ProcessDropSpeed();
	BOOL	GetWaterline();

	BOOL	CellMoveSmart(int nStepLength);
	void	ProcessHorizontalMoveSmart();
	void	ProcessHorizontalMove();

	BOOL	ForwardPosition(INT nFrame);
	virtual BOOL	ChangeRegion(KRegion* pDestRegion);
	void	SetCell(KCell* pCell);

	void	ResetRegionFrames(INT nRadius);
	void	ResetRegionEnterAndLeaveFrame(KRegion* pCenterRegion);
	BOOL	UpdateRegionFrames(KRegion* pFromRegion, KRegion* pToRegion);

protected:
	// 行走结果
	enum KMOVE_RESULT
	{
		mrInvalid,
		mrSucceed,      // 正常移动了
		mrOutOfMap,     // 到地图边缘了,无效的区域
		mrObstacle,     // 有障碍,无法前进
		mrDeleteSelf,   // 在客户端,进入同步范围之外,把自己删除了
		mrReachTarget,  // 已经到达目标
		mrTotal
	};
public:
	friend class KScene;
	friend class KRegion;
	// 特殊处理，需要直接操作内部数据
	friend class KPlayerBaseProcess;
};

inline BOOL g_InRange(KSceneObject* pObjA, KSceneObject* pObjB, int nRange)
{
	BOOL    bResult     = false;
	BOOL    bRetCode    = false;
	int     nXa         = 0;
	int     nYa         = 0;
	int     nZa         = 0;
	int     nXb         = 0;
	int     nYb         = 0;
	int     nZb         = 0;

	assert(pObjA);
	assert(pObjB);

	PROCESS_ERROR(pObjA->m_pScene);
	PROCESS_ERROR(pObjB->m_pScene);

	PROCESS_ERROR(pObjA->m_pScene == pObjB->m_pScene);

	bRetCode = pObjA->GetPosition(nXa, nYa, nZa);
	PROCESS_ERROR(bRetCode);

	bRetCode = pObjB->GetPosition(nXb, nYb, nZb);
	PROCESS_ERROR(bRetCode);

	bRetCode = g_InRange(nXa, nYa, ZPOINT_TO_XYPOINT(nZa), nXb, nYb, ZPOINT_TO_XYPOINT(nZb), nRange);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

inline IN_RANGE_RESULT g_InRange(KSceneObject* pObjA, KSceneObject* pObjB, int nMinRange, int nMaxRange)
{
	IN_RANGE_RESULT nResult     = irrInvalid;
	BOOL            bRetCode    = false;
	int             nXa         = 0;
	int             nYa         = 0;
	int             nZa         = 0;
	int             nXb         = 0;
	int             nYb         = 0;
	int             nZb         = 0;

	assert(pObjA);
	assert(pObjB);

	PROCESS_ERROR(pObjA->m_pScene);
	PROCESS_ERROR(pObjB->m_pScene);

	PROCESS_ERROR(pObjA->m_pScene == pObjB->m_pScene);

	bRetCode = pObjA->GetPosition(nXa, nYa, nZa);
	PROCESS_ERROR(bRetCode);

	bRetCode = pObjB->GetPosition(nXb, nYb, nZb);
	PROCESS_ERROR(bRetCode);

	nResult = g_InRange(nXa, nYa, ZPOINT_TO_XYPOINT(nZa), nXb, nYb, ZPOINT_TO_XYPOINT(nZb), nMinRange, nMaxRange);

EXIT0:
	return nResult;
}

#endif	// _KSCENEOBJECT_H_
