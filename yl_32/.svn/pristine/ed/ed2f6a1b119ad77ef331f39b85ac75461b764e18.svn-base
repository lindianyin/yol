
#pragma once

#include "kcharacter.h"
#include "knpcserver.h"
#include "knpcclient.h"
#include "config/knpcdef.h"

enum NPC_KIND
{
	NPC_KIND_FIGHT  = 0,         // 战斗NPC
	NPC_KIND_DIALOG = 1,         // 对话NPC
	NPC_KIND_NORMAL = 2,         // 非对话非战斗NPC
	NPC_KIND_NOT_SHOW_NAME = 3,  // 不显示名字的NPC
	NPC_KIND_PROGRESS = 4,		//读条npc
	NPC_KIND_PLAYER   = 5,      // 玩家形象的npc，用于竞技场等
	NPC_KIND_GUIDE    = 6,      // 纯用于指引攻击方位的npc，在角色到达后需控制死亡
	NPC_KIND_TRANSFER = 7,      // 传送npc
};

// -------------------------------------------------------------------------
class KLuaNpc;
class KLuaCharacter;

// -------------------------------------------------------------------------

struct KNPC_TEMPLATE
{
	DWORD dwTemplateId;
	INT nRepresentId;
	QString strClassName;	// 英文或拼音，做脚本Npc类名
	QString strTitle;	// 头衔
	QString strDisplayName; // 显示名
	QString strAIClassName; // AI类
	QString strAiParam1; // Ai参数
	BYTE byKind; // Npc类型

	INT nTouchRange;     // 交互范围
	INT nSelectable;     // 是否能选中
	INT nDropListId1;     // 掉落表ID
	INT nDropTimes1;      // 掉落次数

	DWORD						dwSkillIDList[MAX_NPC_AI_SKILL];		//AI技能ID
	DWORD						dwSkillLevelList[MAX_NPC_AI_SKILL];		//AI技能等级
	int							nSkillCastInterval[MAX_NPC_AI_SKILL];	//AI技能间隔
	int							nAISkillType[MAX_NPC_AI_SKILL];			//AI技能类型
	int							nSkillRate[MAX_NPC_AI_SKILL];			//AI技能释放的概率
	int							nSkillAniFrame[MAX_NPC_AI_SKILL];       //AI技能动作帧数
	int							nSkillRestFrame[MAX_NPC_AI_SKILL];      //AI技能停止帧数

	int							nIgnoreDebuff[MAX_NPC_IGNORE_DEBUFF];	// 免疫buff列表

	int							nThreatTime;				            //仇恨消退间隔
	int							nOverThreatPercent;
	int                         nPursuitRange;                          //追击距离
	int							nThreatLinkId;							// 仇恨分组Id
};

class KNpc : public KCharacter
{
public:
	KD_CHARACTER_TASKFUN(RepresentId,					emKVARGROUP_NPC,	emKCHARACTER_TASK_REPRESENT_ID)
	//KD_CHARACTER_TASKFUN(Relation,                      emKVARGROUP_NPC,    emKCHARACTER_TASK_RELATION)
	KD_CHARACTER_TASKFUN(FierceTimes,                   emKVARGROUP_NPC,    emKCHARACTER_TASK_FIERCETITIMES)
	//KD_CHARACTER_TASKFUN(AIType,						emKVARGROUP_NPC,	emKCHARACTER_TASK_AI_TYPE)
	KD_CHARACTER_TASKFUN(LifeReplenish,					emKVARGROUP_NPC,	emCHARACTER_TASK_LIFEREPLENISH)
    KD_CHARACTER_TASKFUN(ManaReplenish,					emKVARGROUP_NPC,	emCHARACTER_TASK_MANAREPLENISH)

public:
	KNpc();
	virtual ~KNpc();

	BOOL Init(INT nIndex, CONST KNPC_TEMPLATE& rsTemplate);
	BOOL Uninit();
	BOOL OnRemove();
	BOOL Activate();

	DWORD GetTemplateId() const { return m_dwTemplateId; }
	QString GetTitle() const { return m_strTitle; }
	QString GetClassName() const { return m_strClassName; }

	BYTE GetKind() CONST {return m_byKind;}
	INT GetDropListID() CONST { return m_nDropListId;}

	BOOL IsIgnoreDebuff(INT nBuffType);

	// 获得脚本接口
	virtual KLuaCharacter* GetScriptInterface() { return (KLuaCharacter*)m_pLuna;}

	BOOL GoTo(INT nX, INT nY, INT nZ, BOOL bSyncSelf = FALSE);
	BOOL StopMove(BOOL bSyncSelf /* = FALSE */);
	BOOL Jump(BOOL bSyncSelf = FALSE);
	BOOL KnockTo(INT nX, INT nY, INT nZ, INT nDirection);
	BOOL Pull(INT nX, INT nY, INT nZ, INT nDirection);
	BOOL SetDisappearFrame(INT nDelayFrame);
	BOOL SetStrName(CHAR *m_setName){ m_strName = m_setName; return TRUE;}
protected:
	BOOL	ChangeRegion(KRegion* pDestRegion);

public:
	int				m_nNpcTeamID;					//Npc小队ID
	int				m_nNpcTeamIndex;				//NPC小队中的编号，没有队伍的npc为-1
	int				m_nSkillCommomCD;						//Npc技能的公共CD,防止Npc一次释放多个技能
	int				m_nSkillCastFrame[MAX_NPC_AI_SKILL];	//NPC每个技能的最后释放时间

	int             m_nSkillSelectIndex;            // 技能选择器的结果
	const KNPC_TEMPLATE*	m_pTemplate;			// Npc模板

	BOOL			m_bDeleted;		// 是否已经准备回收

#ifdef GAME_SERVER
	friend class KNpcServer;
	KNpcServer m_cNpcServer;
#else
	friend class KNpcClient;
	KNpcClient m_cNpcClient;
#endif

protected:
	DWORD m_dwTemplateId; // 表现模板Id
	INT m_nIndex; // NpcIndex
	KLuaNpc* m_pLuna; // 脚本指针
	QString m_strTitle;
	QString m_strClassName;

	INT m_nSelectable;     // 是否能选中
	INT m_nDropListId;     // 掉落表ID
	INT m_nDropTimes;      // 掉落次数
	BYTE m_byKind;               // NPC类型

private:
	// add by zhaoyu
	INT m_nDisappearFrame; // NPC延迟删除

private:
	friend class KNpcMgr;
};

// -------------------------------------------------------------------------

