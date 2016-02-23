#include "stdafx.h"
#include "kenemycondition.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kplayer.h"

#define REGISTER_ENEMY_CONDITION_TYPE(slotNum, funcPtr) \
	m_AryCheckFun[slotNum] = &QEnemyCondition::funcPtr

QEnemyCondition::QEnemyCondition()
{
	::memset(m_AryCheckFun, 0, sizeof(m_AryCheckFun));

	REGISTER_ENEMY_CONDITION_TYPE(ENEMY_CONDITION_TYPE_ALL, _CheckAll);
	REGISTER_ENEMY_CONDITION_TYPE(ENEMY_CONDITION_TYPE_SELF, _CheckSelf);
	REGISTER_ENEMY_CONDITION_TYPE(ENEMY_CONDITION_TYPE_NPC, _CheckNpc);
	REGISTER_ENEMY_CONDITION_TYPE(ENEMY_CONDITION_TYPE_TEAM, _CheckTeam);
	REGISTER_ENEMY_CONDITION_TYPE(ENEMY_CONDITION_TYPE_KIN, _CheckKin);
	REGISTER_ENEMY_CONDITION_TYPE(ENEMY_CONDITION_TYPE_SERVER, _CheckSameServer);
}

QEnemyCondition::~QEnemyCondition()
{

}

BOOL QEnemyCondition::IsEnemy( KCharacter* pSrcCharacter, KCharacter* pDstCharacter )
{
	if (pSrcCharacter->GetEnableEnemyCondition() == FALSE)
		return FALSE;

	BOOL bResult = FALSE;

	INT nAryCondition[ENEMY_CONDITION_NUM] = {
		pSrcCharacter->GetEnemyCondition1(),
		pSrcCharacter->GetEnemyCondition2(),
		pSrcCharacter->GetEnemyCondition3(),
		pSrcCharacter->GetEnemyCondition4(),
		pSrcCharacter->GetEnemyCondition5()
	};

	for (INT i = 0; i < ENEMY_CONDITION_NUM; ++i)
	{
		if (nAryCondition[i] == 0)
			continue;

		BOOL nAryConditionTypeEnabled[ENEMY_CONDITION_TYPE_NUM] = {
			nAryCondition[i] & ENEMY_CONDITION_ALL_ENABLE_FLAG,
			nAryCondition[i] & ENEMY_CONDITION_SELF_ENABLE_FLAG,
			nAryCondition[i] & ENEMY_CONDITION_NPC_ENABLE_FLAG,
			nAryCondition[i] & ENEMY_CONDITION_TEAM_ENABLE_FLAG,
			nAryCondition[i] & ENEMY_CONDITION_KIN_ENABLE_FLAG,
			nAryCondition[i] & ENEMY_CONDITION_SERVER_ENABLE_FLAG
		};

		for (INT j = 0; j < ENEMY_CONDITION_TYPE_NUM; ++j)
		{
			if (m_AryCheckFun[j] && nAryConditionTypeEnabled[j])
			{
				bResult = (this->*m_AryCheckFun[j])(pSrcCharacter, pDstCharacter, nAryCondition[i]);

				// or 运算，有一个为true，则为true
				if (bResult == TRUE)
					break;
			}
		}

		if (nAryCondition[i] & ENEMY_CONDITION_NOT_FLAG)
			bResult = bResult ? FALSE : TRUE;

		// or 运算，有一个为true，则最后总结果为true
		if (bResult)
			break;
	}

	return bResult;
}

BOOL QEnemyCondition::_CheckAll( KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition )
{
	return nCondition & ENEMY_CONDITION_ALL_FLAG ? TRUE : FALSE;
}

BOOL QEnemyCondition::_CheckSelf( KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition )
{
	if (pSrcCharacter == NULL || pDstCharacter == NULL)
		return FALSE;

	BOOL bResult = (pSrcCharacter->m_dwId == pDstCharacter->m_dwId) ? TRUE : FALSE;

	if ((nCondition & ENEMY_CONDITION_SELF_FLAG) == FALSE)
		bResult = bResult ? FALSE : TRUE;

	return bResult;
}

BOOL QEnemyCondition::_CheckNpc( KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition )
{
	if (pSrcCharacter == NULL || pDstCharacter == NULL)
		return FALSE;

	BOOL bResult = IS_NPC(pDstCharacter->m_dwId);

	if ((nCondition & ENEMY_CONDITION_NPC_FLAG) == FALSE)
		bResult = bResult ? FALSE : TRUE;

	return bResult;
}

BOOL QEnemyCondition::_CheckTeam( KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition )
{
	if (pSrcCharacter == NULL || pDstCharacter == NULL)
		return FALSE;

	KPlayer *pSrcPlayer = NULL;
	KPlayer *pDstPlayer = NULL;
	if (IS_PLAYER(pSrcCharacter->m_dwId) && IS_PLAYER(pDstCharacter->m_dwId))
	{
		pSrcPlayer = static_cast<KPlayer *>(pSrcCharacter);
		pDstPlayer = static_cast<KPlayer *>(pDstCharacter);
	}

	BOOL bResult = FALSE;
	if (pSrcPlayer && pDstPlayer && pSrcPlayer->GetTeamId() != 0 && pDstPlayer->GetTeamId() != 0)
		bResult = pSrcPlayer->GetTeamId() == pDstPlayer->GetTeamId();

	if ((nCondition & ENEMY_CONDITION_TEAM_FLAG) == FALSE)
		bResult = bResult ? FALSE : TRUE;

	return bResult;
}

BOOL QEnemyCondition::_CheckKin( KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition )
{
	if (pSrcCharacter == NULL || pDstCharacter == NULL)
		return FALSE;

	KPlayer *pSrcPlayer = NULL;
	KPlayer *pDstPlayer = NULL;
	if (IS_PLAYER(pSrcCharacter->m_dwId) && IS_PLAYER(pDstCharacter->m_dwId))
	{
		pSrcPlayer = static_cast<KPlayer *>(pSrcCharacter);
		pDstPlayer = static_cast<KPlayer *>(pDstCharacter);
	}

	BOOL bResult = FALSE;
	if (pSrcPlayer && pDstPlayer && pSrcPlayer->GetKinId() != 0 && pDstPlayer->GetKinId() != 0)
		bResult = pSrcPlayer->GetKinId() == pDstPlayer->GetKinId();

	if ((nCondition & ENEMY_CONDITION_KIN_FLAG) == FALSE)
		bResult = bResult ? FALSE : TRUE;

	return bResult;
}

BOOL QEnemyCondition::_CheckSameServer( KCharacter* pSrcCharacter, KCharacter* pDstCharacter, INT nCondition )
{
	if (pSrcCharacter == NULL || pDstCharacter == NULL)
		return FALSE;

	// todo not implement
	return FALSE;
	// return nCondition & ENEMY_CONDITION_SERVER_FLAG ? TRUE : FALSE;
}
