/* -------------------------------------------------------------------------
//	文件名		：	kluaplayer.h
//	创建者		：	simon
//	创建时间	：	2009/11/21 18:01:30
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KLUAPLAYER_H__
#define __KLUAPLAYER_H__

#include "onlinegamebase/kscriptmanager.h"
#include "kluacharacter.h"
#include "onlinegameworld/kplayer.h"

// -------------------------------------------------------------------------

class KLuaPlayer : public KLuaCharacter
{
public:
	static KPlayer* GetHim(QLuaScript& sc, INT nIndex);

	explicit KLuaPlayer(KPlayer* pPlayer) : KLuaCharacter(pPlayer), m_pMe(pPlayer)
	{
	}
	virtual ~KLuaPlayer()
	{
	}

	KPlayer* GetHim()	{ return m_pMe; };

	BOOL	ClearTempTable();

	//脚本接口定义
	DECLARE_LUA_DRIVATE_CLASS(KLuaPlayer, KLuaCharacter);


	DWORD	getId(VOID) CONST;

	INT			LuaSetPosition(QLuaScript& sc); // 临时调试用
	INT			LuaSysMsg(QLuaScript& sc);

	// 组队相关通用
	INT		LuaInviteJoinTeam(QLuaScript& sc);
	INT		LuaLeaveTeam(QLuaScript& sc);

	//INT		LuaGetTeamGroupInfo(QLuaScript& sc);
	//INT		LuaGetTeamGroupIndex(QLuaScript& sc);
	INT		LuaGetTeamId(QLuaScript& sc);
	INT		LuaSetTeamId(QLuaScript& sc);
	INT		LuaGetSysTeamId(QLuaScript& sc);

	INT		LuaIsCaptain(QLuaScript& sc);
	INT		LuaGetKinId(QLuaScript& sc);
	INT		LuaIsKinMaster(QLuaScript& sc);
	INT		LuaGetKinMasterId(QLuaScript& sc);

	INT		LuaGetGameState(QLuaScript& sc);


#ifdef GAME_SERVER

	INT     LuaGetLastSaveTime(QLuaScript& sc);

	INT		LuaGetAccountCoin(QLuaScript &sc);
	INT		LuaGetLimitPlayTimeFlag(QLuaScript& sc);
	INT		LuaGetLimitOnlineSecond(QLuaScript& sc);
	INT		LuaGetLimitOfflineSecond(QLuaScript& sc);
	INT		LuaGetAwardFlag(QLuaScript& sc);
	INT		LuaSetAwardFlag(QLuaScript& sc);
	INT     LuaCancelProgressBar(QLuaScript& sc);
	INT     LuaStartProgressBar(QLuaScript& sc);
	INT	    LuaIsBZonePlayer(QLuaScript& sc);
	INT		LuaClientShowHeadText(QLuaScript& sc);  // 角色头顶冒字

#endif

#ifdef GAME_SERVER
	LPCSTR  getAccount() CONST;
	LPCSTR  getLoginIp() CONST;

	INT		LuaTeleportTo(QLuaScript& sc); // 传送
	INT		LuaBattleZoneTransfer(QLuaScript& sc); // 换线
	INT		LuaDoMultiTransferRequest(QLuaScript& sc);
	INT		LuaBattleZoneReturn(QLuaScript& sc);
	INT		LuaDoGetMultiAwardRequest(QLuaScript& sc);
	INT		LuaDoGetCrossSvExDataRequest(QLuaScript& sc);
	INT		LuaReturnToLastServer(QLuaScript& sc);
	INT		LuaKickOut(QLuaScript& sc);
	INT		LuaCallClientScript(QLuaScript& sc);
	INT		LuaReturnToLastCity(QLuaScript& sc);
	INT		LuaGetLastSaveCity(QLuaScript& sc);
	INT		LuaIsGm(QLuaScript& sc);

	INT		LuaSendDialog(QLuaScript& sc);
	INT		LuaSendTalk(QLuaScript& sc);
	INT		LuaSendBlackMsg(QLuaScript& sc);
	INT		LuaSendNoticeMsg(QLuaScript& sc);

	INT		LuaSyncDegree(QLuaScript& sc);

	INT	    LuaSetPositionForSaving(QLuaScript& sc);
	INT		LuaIsSwim(QLuaScript& sc);
	INT		LuaResetMeditationData(QLuaScript& sc);
	INT		LuaGetTakeMeditationOfflineAwardTime(QLuaScript& sc);
	INT		LuaSetTakeMeditationOfflineAwardTime(QLuaScript& sc);
	INT		LuaGetLastMeditationOnlineTime(QLuaScript& sc);
	INT		LuaGetMeditationOnlineTime(QLuaScript& sc);

	INT		LuaSetTrapInvalid(QLuaScript& sc);
	INT		LuaRemoveTrapInvalid(QLuaScript& sc);
	INT		LuaIsTrapInvalid(QLuaScript& sc);
	INT		LuaGetBladeInfo(QLuaScript& sc);
	INT 	LuaOnPayFinsh(QLuaScript& sc);

#endif
#ifdef GAME_CLIENT
	INT			LuaClientGoTo(QLuaScript& sc);
	INT			LuaSwitchMoveMode(QLuaScript& sc);
	INT			LuaRequestOffline(QLuaScript& sc);
	INT			LuaClientMoveCtrl(QLuaScript& sc);
	INT			LuaClientJump(QLuaScript& sc);
	INT			LuaDialogDoodad(QLuaScript& sc);
	INT			LuaApplySetVar(QLuaScript& sc);

	//请求与NPC会话
	INT			LuaApplyNpcTalk(QLuaScript& sc);

	INT			LuaCallServerScript(QLuaScript& sc);
	INT			LuaGetRepresentPosition(QLuaScript& sc);
#endif // GAME_CLIENT

protected:
	KPlayer* m_pMe;
};

// -------------------------------------------------------------------------

#endif /* __KLUAPLAYER_H__ */
