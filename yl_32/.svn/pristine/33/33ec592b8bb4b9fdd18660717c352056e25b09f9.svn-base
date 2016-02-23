/* -------------------------------------------------------------------------
//	文件名		：	
//	创建者		：	simon
//	创建时间	：	2010/3/12 10:53:44
//	功能描述	：	小物件
//
// -----------------------------------------------------------------------*/

#pragma once

#include "Runtime/qstring.h"
#include "onlinegameworld/ksceneobject.h"
#include "onlinegameworld/kdoodadserver.h"
// -------------------------------------------------------------------------
class KLuaDoodad;
class KPlayer;


// -------------------------------------------------------------------------

struct KDOODAD_TEMPLATE
{
	DWORD dwTemplateId;
	QString strDisplayName;	// 显示名
	QString strClassName;		// 类名
	INT nVisibleMode;		// 可见模式
	DWORD dwRepresentId;
	DWORD dwActiveTime;		// 掉落存在时间

	INT nGenre;
	INT nDetail;
	INT nPariticular;
	INT nLevel;
	INT nAmountMin;
	INT nAmountMax;
};

enum KEDOODAD_STATE
{
	emKDOODADSTATE_NONE,
	emKDOODADSTATE_NORMAL,
	emKDOODADSTATE_OPING,		// 操作中
	emKDOODADSTATE_EMPTY,		// 取空了
	emKDOODADSTATE_DISABLE,		// 禁止状态

	emKDOODADSTATE_TOTAL
};

enum KEDOODAD_VISIBLE_MODE
{
	emDoodadVisibleNone = 0,
	emDoodadVisibleSelf = 1,
	emDoodadVisibleTeam = 2,
	emDoodadVisibleTong = 3,
	emDoodadVisibleCamp = 4,
	emDoodadVisibleAll  = 5,
	emDoodadVisibleAuto = 6, // 有队伍时队伍可见，无队伍有家族时家族可见，无队伍无家族时个人可见
};

enum DOODAD_KIND
{
	dkInvalid = 0,

	dkNormal,					// 普通的Doodad,有Tip,不能操作
	dkCorpse,					// 尸体
	dkQuest,					// 任务相关的Doodad
	dkNpcDrop,                  // 使用NPC掉落模式的doodad
	dkChair,					// 可以坐的Doodad
	dkDoor,                     // 门之类有动态障碍的Doodad

	dkTotal
};

enum KEDOODAD_COUNT_ENUM
{
	emKDOODADCOUNT_LAST_TIME = 160,	// 掉落物品保留时间（300）
};


class KDoodad : public KSceneObject
{
public:
	KDoodad(void);
	~KDoodad(void);

	BOOL Init(INT nIndex, KDOODAD_TEMPLATE* pstDoodadTemplate);
	BOOL Uninit();
	BOOL OnRemove();
	BOOL Activate();

	KLuaDoodad* GetScriptInterface() { return m_pLuna; }

	BOOL DoDialog(KPlayer* pDialogPlayer);
	VOID CancelDialog( KPlayer* pDialogPlayer );
	VOID SetOwnerId(DWORD dwOwnerId) { m_dwOwnerId = dwOwnerId; }
#ifdef GAME_SERVER
public:
	DWORD					m_dwTemplateId;					// 模板编号
	DOODAD_KIND				m_eKind;						// 类型

	INT						m_nVisibleMode;

	DWORD					m_dwNpcTemplateID;
	DWORD                   m_dwNpcID;                      // 尸体对应的NPC
	DWORD					m_dwRepresentID;				// Doodad的表现ID
	BOOL					m_bLooted;						// 尸体是否被拾取完的标记
	int                     m_nObstacleGroup;               // 控制动态障碍的分组

	//DWORD					m_dwReviveID;					// 重生群组的编号
	//int						m_nCurrentReliveIndex;			// 当前复活点的编号
	//KDoodadTemplate*		m_pTemplate;					// 指向模板的指针
	//DWORD					m_dwScriptID;					// 绑定脚本ID
	//KLootList*				m_pLootList;					// 拾取列表


	KDoodadServer m_cDoodadServer;

	DWORD m_dwActiveTimeOut;		// 掉落物品存在时间计数器
	//int m_dwRespawnCounter;	//重生计数器
	//当前操作玩家
	KPlayer* m_pCurrPlayer;
	DWORD		m_dwOwnerId;  // 主人Id（每个玩家单独处理掉落）
#endif

public:
	INT m_nGenre;
	INT m_nDetail;
	INT m_nParticular;
	INT m_nLevel;
	INT m_nAmount;

	INT nVisibleMode;

	std::string m_strClass;
	KEDOODAD_STATE m_eState;

private:
	std::map<DWORD, INT> m_mapVisiblePlayers;			// 此掉落物品可见列表
	BOOL IsMemberVisible(KPlayer* pPlayer, KPlayer* pMember);

public:
	BOOL InitVisibleList(std::vector<DWORD>& vecPlayers);
	BOOL IsPlayerVisible(KPlayer* pPlayer);

protected:
	INT m_nIndex; // DoodadIndex
	KLuaDoodad* m_pLuna; // Lua对象指针

	friend class KDoodadMgr;
	friend class KDoodadServer;
};

