
#pragma once

#ifdef GAME_CLIENT
#include "onlinegameworld/kcharacterclient.h"

class KPlayer;

// -------------------------------------------------------------------------
// 玩家客户端扩展（对于自身客户端玩家，不包括客户端的其他玩家）
class KPlayerClient : public KCharacterClient
{
public:
	KPlayerClient(KPlayer& rcPlayer);;

	BOOL DoGoTo(INT nX, INT nY, INT nZ, INT nMoveMode);
	BOOL SwitchMoveMode(INT nRun);
	BOOL DoRequestOffline();
	BOOL DoJump();
	BOOL MoveCtrl(BOOL bWalk, BOOL bForward, BOOL bBackward, BOOL bLeft, BOOL bRight, INT nCamDirection); // TODO: 不应该放在逻辑层
	BOOL DoMoveCtrl(BOOL bMove, BOOL bWalk, INT nDirection);
	BOOL DoDialogDoodad(DWORD dwDoodadId);

	//请求服务端NPC会话
	BOOL DoApplyNpcTalk(KPlayer& rPlayer, DWORD dwNpcId);

	INT DynamicSceneFlag() const { return m_nDynamicSceneFlag; }
	void DynamicSceneFlag(INT val) { m_nDynamicSceneFlag = val; }
protected:
	KPlayer& m_rcPlayer;
	INT m_nDynamicSceneFlag; // 临时
private:
};

// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT

