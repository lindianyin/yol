/* -------------------------------------------------------------------------
//	文件名		：	KNSWorld.h
//	创建者		：	xiewen
//	创建时间	：	2009-10-23 10:11:39
//	功能描述	：	nine-sky world simulation
//
// -----------------------------------------------------------------------*/
#ifndef __KNSWORLD_H__
#define __KNSWORLD_H__
#include <map>
#include <queue>
#include "onlinegameworld/kobjgeneraleventcreator.h"
// -------------------------------------------------------------------------

#ifdef GAME_CLIENT
	struct KStartTimeEvaluation
	{
		INT nFirstEvaluationGameLoop;
		DWORD dwFirstStartTime;
		DWORD dwMinFirstStartTime;
		DOUBLE lfFirstStartTime;
		DOUBLE lfNetStartTime;
	};

	void InitStartTimeEvaluation(KStartTimeEvaluation* pStartTimeEvaluation);
	BOOL UpdateStartTimeEvaluation(KStartTimeEvaluation* pStartTimeEvaluation, INT nFrame);
	BOOL UpdateStartTimeEvaluation2(KStartTimeEvaluation* pStartTimeEvaluation, INT nFrame);
#endif // GAME_CLIENT

class KPlayer;
class KNpc;
class KDoodad;
class IKScene;

class KOrpgWorld
{
public:
	KOrpgWorld(void);
	~KOrpgWorld(void);

	BOOL	Init();
	BOOL	UnInit();
	BOOL	Activate();
#ifdef GAME_SERVER
	BOOL	IsGm(KPlayer* pPlayer); // 临时GM功能
	BOOL	LoadGmList();
#endif // GAME_SERVER

	BOOL	UnloadScene(INT nMapId, INT nMapCopyIndex = 0);
	BOOL	UnloadSceneDelay(DWORD dwId);

	KPlayer*	AddPlayer(LPCSTR szName, DWORD dwId = KD_BAD_ID);
	BOOL	RemovePlayer(DWORD dwId);
	KNpc*	AddNpc(DWORD dwTemplateId, DWORD dwId = KD_BAD_ID);
	BOOL	RemoveNpc(DWORD dwId, BOOL bRegenerate = FALSE);

	BOOL InitServerStartTime();

	BOOL OnGetServerStartTimeRespond(BYTE* pRecordData, size_t uRecordDataLen);

#ifdef GAME_CLIENT
	KPlayer* GetClientPlayer() const;
	VOID CheckApplySyncNeighbor();
	VOID AddApplySyncNeighbor(const DWORD& dwSceneObjId) { m_vApplySyncNeighbor.push(dwSceneObjId); }
	time_t GetServerTime() const { return m_dwServerTime + (timeGetTime() - m_dwStartTime) / 1000; }
#endif // GAME_CLIENT

public:
	KObjGeneralEventCreator m_cEventCreator;
	DWORD	m_dwLogicFrames;		// 游戏逻辑帧
	DWORD	m_dwServerStartTime;	// 开服时间
	BOOL m_bDebugSelf; // 允许输出场景等内部调试信息
	BOOL	m_nServerId; // 线Id
	INT		m_nIdleFrame;
	INT		m_nServerMode;


#ifdef GAME_CLIENT
	DWORD	m_dwClientSceneId;	// Client玩家所在场景Id
	DWORD	m_dwClientPlayerId;	// Client本机玩家Id

    KStartTimeEvaluation m_StartTimeEvaluation;
    DWORD m_dwStartTime;  // 起始时间,指本SubWorld开始运行的时间
    INT m_nStartLoop;
	INT	m_nGSGameLoop;
	DWORD m_dwServerTime; // 登录时服务器同步下来的时间

	INT m_nPing; // ping指令往返时间（测网速）
	std::queue<DWORD> m_vApplySyncNeighbor; // 申请同步列表
#endif // GAME_CLIENT

#ifdef GAME_SERVER
	BOOL	m_bDevMode; // 内部测试版（此标记从Coordinator同步过来）
#endif // GAME_SERVER

private:
#ifdef GAME_SERVER
	std::vector<DWORD> m_vDelayUnloadScene; // 待删除场景
#endif // GAME_SERVER
};

// -------------------------------------------------------------------------
extern	KOrpgWorld		g_cOrpgWorld;

#ifdef GAME_CLIENT
#define KD_CLIENT_PLAYER_ID g_cOrpgWorld.m_dwClientPlayerId //方便使用
#endif // GAME_CLIENT

#endif /* __KNSWORLD_H__ */
