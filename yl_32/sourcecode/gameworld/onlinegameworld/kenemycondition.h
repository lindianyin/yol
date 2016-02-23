#pragma once

#define ENEMY_CONDITION_TYPE_ALL		0
#define ENEMY_CONDITION_TYPE_SELF		1
#define ENEMY_CONDITION_TYPE_NPC		2
#define ENEMY_CONDITION_TYPE_TEAM		3
#define ENEMY_CONDITION_TYPE_KIN		4
#define ENEMY_CONDITION_TYPE_SERVER		5

#define ENEMY_CONDITION_TYPE_NOT		31

#define ENEMY_CONDITION_TYPE_NUM		6

#define ENEMY_CONDITION_ALL_FLAG			(1 << ENEMY_CONDITION_TYPE_ALL)
#define ENEMY_CONDITION_SELF_FLAG			(1 << ENEMY_CONDITION_TYPE_SELF)
#define ENEMY_CONDITION_NPC_FLAG			(1 << ENEMY_CONDITION_TYPE_NPC)
#define ENEMY_CONDITION_TEAM_FLAG			(1 << ENEMY_CONDITION_TYPE_TEAM)
#define ENEMY_CONDITION_KIN_FLAG			(1 << ENEMY_CONDITION_TYPE_KIN)
#define ENEMY_CONDITION_SERVER_FLAG			(1 << ENEMY_CONDITION_TYPE_SERVER)

#define ENEMY_CONDITION_ENABLE_FLAG_OFFSET		16

#define ENEMY_CONDITION_ALL_ENABLE_FLAG		(1 << (ENEMY_CONDITION_ENABLE_FLAG_OFFSET + ENEMY_CONDITION_TYPE_ALL))
#define ENEMY_CONDITION_SELF_ENABLE_FLAG	(1 << (ENEMY_CONDITION_ENABLE_FLAG_OFFSET + ENEMY_CONDITION_TYPE_SELF))
#define ENEMY_CONDITION_NPC_ENABLE_FLAG		(1 << (ENEMY_CONDITION_ENABLE_FLAG_OFFSET + ENEMY_CONDITION_TYPE_NPC))
#define ENEMY_CONDITION_TEAM_ENABLE_FLAG	(1 << (ENEMY_CONDITION_ENABLE_FLAG_OFFSET + ENEMY_CONDITION_TYPE_TEAM))
#define ENEMY_CONDITION_KIN_ENABLE_FLAG		(1 << (ENEMY_CONDITION_ENABLE_FLAG_OFFSET + ENEMY_CONDITION_TYPE_KIN))
#define ENEMY_CONDITION_SERVER_ENABLE_FLAG	(1 << (ENEMY_CONDITION_ENABLE_FLAG_OFFSET + ENEMY_CONDITION_TYPE_SERVER))

// 这一个比较特殊，单标识
#define ENEMY_CONDITION_NOT_FLAG			(1 << ENEMY_CONDITION_TYPE_NOT)
// 使用int来标识，最大不能超过32


#define ENEMY_CONDITION_NUM 5			// 角色身上带的判断条件个数


class KCharacter;


class QEnemyCondition
{
public:
	static QEnemyCondition &GetInstance()
	{
		static QEnemyCondition sInstance;
		return sInstance;
	}

protected:
	QEnemyCondition();
	~QEnemyCondition();

public:
	BOOL IsEnemy(KCharacter* pSrcCharacter, KCharacter* pDstCharacter);

protected:
	BOOL _CheckAll(KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition);
	BOOL _CheckSelf(KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition);
	BOOL _CheckNpc(KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition);
	BOOL _CheckTeam(KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition);
	BOOL _CheckKin(KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition);
	BOOL _CheckSameServer(KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition);

	typedef BOOL (QEnemyCondition::*_CheckFunPtr)(KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition);

protected:
	_CheckFunPtr m_AryCheckFun[ENEMY_CONDITION_TYPE_NUM];
};