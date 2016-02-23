/* -------------------------------------------------------------------------
//	文件名		：	KLuaCharacter.h
//	创建者		：	simon
//	创建时间	：	2009/11/21 18:01:30
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KLUACHARACTER_H__
#define __KLUACHARACTER_H__

#include "onlinegamebase/kscriptmanager.h"
#include "Core/luna.h"

// -------------------------------------------------------------------------

class KCharacter;


#define MAX_PLAYER_LEVEL (60)
class KLuaCharacter : public QLunaBase
{
public:
	static KCharacter* GetHim(QLuaScript& sc, INT nIndex);

	explicit KLuaCharacter(KCharacter* pCharacter) : m_pMe(pCharacter)
	{
	}
	virtual ~KLuaCharacter()
	{
	}

	KCharacter* GetHim()	{ return m_pMe; };

	BOOL	ClearTempTable();

	//脚本接口定义
	DECLARE_LUA_CLASS(KLuaCharacter);


	DWORD		getId(VOID) CONST;
	DWORD		getSceneId(VOID) CONST;
	DWORD		getSceneTemplateId(VOID) CONST;
	INT			getFaceDirection(VOID) CONST;
	LPCSTR		getName() CONST;
	DWORD		getKinId() CONST;
	INT			getVisible(VOID) CONST;

	INT			LuaIsDead(QLuaScript& sc);
	INT			LuaIsNpc(QLuaScript& sc);
	INT			LuaIsPlayer(QLuaScript& sc);
	INT			LuaIsBZonePlayer(QLuaScript& sc);
	INT			LuaGetVar(QLuaScript& sc);
	INT			LuaSetVar(QLuaScript& sc);
	INT			LuaRawSetVar(QLuaScript& sc);
	INT			LuaGetPosition(QLuaScript& sc);
	INT			LuaGetWorldPos(QLuaScript& sc);
	INT			LuaGetDestination(QLuaScript& sc);
	INT			LuaJump(QLuaScript& sc);
	INT			LuaGoTo(QLuaScript& sc);
	INT			LuaStopMove(QLuaScript& sc);

	INT			LuaAddExp(QLuaScript& sc);
	INT			LuaChangeFaceDirection(QLuaScript& sc);

	INT			LuaGetCharacterTempTable(QLuaScript& sc);
	INT			LuaIsNearBy(QLuaScript& sc);
	INT			LuaIsNearByIn2D(QLuaScript& sc);

	INT			LuaIsInDynamicScene(QLuaScript& sc);
	INT			LuaIsOnFierce(QLuaScript& sc);

	INT			LuaKilledBy(QLuaScript& sc);

	INT			LuaIsInRecRange(QLuaScript &sc);
	INT			LuaIsInCircle(QLuaScript &sc);

	INT			LuaIsBack(QLuaScript &sc);
	INT			LuaGetEnemyInDis(QLuaScript &sc);
	INT			LuaCopyAITarget(QLuaScript &sc);

	INT			LuaSetTarget(QLuaScript& sc);
	INT			LuaGetTarget(QLuaScript& sc);
	INT			LuaCalcFightScore(QLuaScript &sc);
#ifdef GAME_SERVER
	INT			LuaSetVisible(QLuaScript& sc);
	INT			LuaSetSkillHidden(QLuaScript& sc);
	INT			LuaSetMoveState(QLuaScript& sc);
	INT			LuaGetMoveState(QLuaScript& sc);
	INT			LuaSetFaceDirection(QLuaScript& sc);
	INT			LuaGetNearByNpcInSector(QLuaScript& sc);
	INT			LuaGetNearByPlayerInSector(QLuaScript& sc);
	INT			LuaGetFrontPosition(QLuaScript& sc);
	INT			LuaGetDirection(QLuaScript& sc);
	INT			LuaCheckTrigger(QLuaScript& sc);
	INT			LuaAddSkillTest(QLuaScript& sc);

	INT			LuaModifyThreat(QLuaScript& sc);
#endif // GAME_SERVER

protected:
	KCharacter* m_pMe;

};
// -------------------------------------------------------------------------

#endif /* __KLUACHARACTER_H__ */
