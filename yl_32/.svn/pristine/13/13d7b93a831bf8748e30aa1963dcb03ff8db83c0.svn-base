

#include "stdafx.h"
#include "onlinegamemodule/kscripthelper.h"
#include "onlinegamebase/kscriptmanager.h"

#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kluanpc.h"
#include "onlinegameworld/kluadoodad.h"

#include "trade/kluaplayerpurse.h"
#include "item2/kluaitem.h"
#include "item2/kluaplayeritem.h"

//#include "kluafaction.h"
#include "task/kluatask.h"
#include "ai/kluaai.h"
#include "onlinegamemodule/chat/kluaplayerchat.h"
#include "onlinegamemodule/mail/kluaplayermail.h"

#include "onlinegamemodule/ai/kluaailogic.h"
#include "onlinegamemodule/ai/kluaaistate.h"
#include "onlinegamemodule/ai/kluaaiaction.h"
#include "onlinegamemodule/luabuffereffect.h"
#include "onlinegamemodule/luacharacterskill.h"
#include "onlinegamemodule/luaplayerquiz.h"

#include "findpath/kluaplayerfindpath.h"
#include "pet/kluapet.h"
#include "pet/kluaride.h"
#include "arena/LuaPlayerArena.h"
#include "title/luaplayertitle.h"
#include "vip/luaplayervip.h"
#include "target/luaplayertarget.h"
#include "opense/luaplayeropense.h"
#include "awardlobby/luaplayerawardlobby.h"
#include "gem/luaplayergem.h"
#include "artifact/luaplayerartifact.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 设置脚本模块标记
#ifdef GAME_SERVER
	#define SCRIPT_MODEL_FLAG	"MODULE_GAMESERVER"
#elif GAME_CLIENT
	#define SCRIPT_MODEL_FLAG	"MODULE_GAMECLIENT"
#else
	#define SCRIPT_MODEL_FLAG	"UNKNOWMODULE"
#endif

#ifdef GAME_SERVER
extern BOOL g_RegisterGameserverEyes();
#endif

KScriptHelper g_cScriptHelper;

extern BOOL g_RegisterKLibScriptFun();
extern BOOL g_RegisterGameBaseScript();
extern BOOL g_RegisterItemScriptFun();
extern BOOL g_RegisterKFileScriptFun();

extern BOOL g_RegisterKCharacterScriptFun();

extern BOOL g_RegisterSceneScript();
extern BOOL g_RegisterTaskScript();
extern BOOL g_RegisterShopScript();

extern BOOL g_RegisterTimerMgrScriptFun();
extern BOOL g_RegisterKUnifyScriptFun();
extern BOOL g_RegisterChatScriptFun();

extern BOOL g_GCRegisterTeamScriptFuns();

extern BOOL g_RegisterKGlobalDataScriptFun();

extern BOOL g_RegisterMailScriptFuns();

extern BOOL g_RegisterBufferScriptFuns();

extern BOOL g_RegisterKinGcScriptFun();

extern BOOL g_RegisterPetScript();

extern BOOL g_RegisterAwardScript();

extern BOOL g_RegisterQuizScript();

extern BOOL g_RegisterPurseScript();

extern BOOL g_RegisterTowerScript();

extern BOOL g_RegisterStageScript();

extern BOOL g_RegisterInstanceScript();

extern BOOL g_RegisterFightSkillScript();

extern BOOL g_RegisterActivityScript();

extern BOOL g_RegisterRemindScript();

extern BOOL g_RegisterRankScript();

extern BOOL g_RegisterCrossSvScript();

extern BOOL g_RegisterEscortScript();

extern QLuaConstList g_LuaConstList[];
// -------------------------------------------------------------------------
BOOL KScriptHelper::InitScript()
{
	__InitScriptEnv();
	__RegisterScriptFuns();
	__ResisterLunaClass();
	__ResisterLunaExtFun();
	__RegisterLuaConst();

	__LoadScript();
	return TRUE;
}
BOOL KScriptHelper::Uninit()
{
	g_cScriptManager.Uninit();

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KScriptHelper::__InitScriptEnv()
{
	g_cScriptManager.Init();
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->PushNumber(1);
	cSafeScript->SetGlobalName(SCRIPT_MODEL_FLAG);

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KScriptHelper::__RegisterScriptFuns()
{
	g_RegisterKLibScriptFun();
	g_RegisterGameBaseScript();

	g_RegisterItemScriptFun();	
	g_RegisterKFileScriptFun();

	g_RegisterKCharacterScriptFun();
	g_RegisterKUnifyScriptFun();

	g_RegisterTimerMgrScriptFun();
	g_RegisterChatScriptFun();

	g_GCRegisterTeamScriptFuns();
	g_RegisterMailScriptFuns();
	g_RegisterBufferScriptFuns();
	g_RegisterKinGcScriptFun();
	g_RegisterPetScript();
	g_RegisterAwardScript();
	g_RegisterQuizScript();
	g_RegisterPurseScript();
	g_RegisterFightSkillScript();
	g_RegisterTowerScript();
	g_RegisterStageScript();
	g_RegisterInstanceScript();
	g_RegisterActivityScript();
	g_RegisterRemindScript();
	g_RegisterRankScript();
	g_RegisterCrossSvScript();
	g_RegisterEscortScript();

#ifdef GAME_SERVER
	g_RegisterSceneScript();
	g_RegisterTaskScript();
	g_RegisterShopScript();

	g_RegisterKGlobalDataScriptFun();
	g_RegisterGameserverEyes();

#elif GAME_CLIENT
	g_RegisterSceneScript();

#endif
	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KScriptHelper::__ResisterLunaClass()
{
	QLuaScript& rcScript	= g_cScriptManager.GetMainScript();

	KLuaCharacter::Register(rcScript);
	KLuaPlayer::Register(rcScript);
	KLuaNpc::Register(rcScript);
	KLuaDoodad::Register(rcScript);
	//KLuaCharacterBuffer::Register(rcScript);
	//KLuaCharacterSkill::Register(rcScript);
	KLuaItem::Register(rcScript);
	KLuaPlayerItem::Register(rcScript);

	KLuaPlayerPurse::Register(rcScript);

	//KLuaFaction::Register(rcScript);
	KLuaTask::Register(rcScript);
	KLuaPlayerChat::Register(rcScript);

	KAILogic::Register(rcScript);
	KAIState::Register(rcScript);
	KAIAction::Register(rcScript);
	KLuaAI::Register(rcScript);

	KLuaPlayerFindPath::Register(rcScript);
	KLuaPlayerMail::Register(rcScript);

	LuaCharacterBufferEffect::Register(rcScript);
	LuaCharacterSkill::Register(rcScript);
	KLuaPet::Register(rcScript);
	KLuaRide::Register(rcScript);

	LuaPlayerQuiz::Register(rcScript);
	LuaPlayerArena::Register(rcScript);
	LuaPlayerTitle::Register(rcScript);
	LuaPlayerTarget::Register(rcScript);
	LuaPlayerVip::Register(rcScript);
	LuaPlayerAwardLobby::Register(rcScript);
	LuaPlayerGem::Register(rcScript);
	LuaPlayerArtifact::Register(rcScript);
	LuaPlayerOpense::Register(rcScript);
	return TRUE;
}

BOOL KScriptHelper::__ResisterLunaExtFun()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	//KLuaCharacter::RegisterExtFun(cSafeScript->GetLuaState(), "pBufferList", KLuaCharacterBuffer::Register2Character);
	//KLuaCharacter::RegisterExtFun(cSafeScript->GetLuaState(), "pSkillList",  KLuaCharacterSkill::Register2Character);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pItem",   KLuaPlayerItem::RegisterToPlayer);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pPlayerPurse",  KLuaPlayerPurse::RegisterToPlayer);

	//KLuaCharacter::RegisterExtFun(cSafeScript->GetLuaState(), "pFaction",     KLuaFaction::Register2Character);
	KLuaCharacter::RegisterExtFun(cSafeScript->GetLuaState(), "pTask", KLuaTask::Register2Character);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pChat", KLuaPlayerChat::Register2Player);
	KLuaCharacter::RegisterExtFun(cSafeScript->GetLuaState(), "pAI", KLuaAI::Register2Character);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pFindPath", KLuaPlayerFindPath::RegisterToPlayer);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pMail", KLuaPlayerMail::RegisterToPlayer);
	KLuaCharacter::RegisterExtFun(cSafeScript->GetLuaState(), "pBufferEffect", LuaCharacterBufferEffect::Register2Character);
	KLuaCharacter::RegisterExtFun(cSafeScript->GetLuaState(), "pFightSkill", LuaCharacterSkill::Register2Character);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pQuiz", LuaPlayerQuiz::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pArena", LuaPlayerArena::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pTitle", LuaPlayerTitle::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pTarget", LuaPlayerTarget::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pVip", LuaPlayerVip::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pAwardLobby", LuaPlayerAwardLobby::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pGem", LuaPlayerGem::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pArtifact", LuaPlayerArtifact::Register2Player);
	KLuaPlayer::RegisterExtFun(cSafeScript->GetLuaState(), "pOpense", LuaPlayerOpense::Register2Player);
	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KScriptHelper::__LoadScript()
{
	g_cScriptManager.LoadScript("\\script\\preload.lua");
	g_cScriptManager.LoadDir("\\script\\preload\\", "\\script\\script.pak.txt");
	g_cScriptManager.LoadDir("\\script\\", NULL);
	
	return TRUE;
}

void KScriptHelper::__RegisterLuaConst()
{
	QLuaConstList* pConstList = g_LuaConstList;

	while (NULL != pConstList && NULL != pConstList->pszListName)
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->PushTable();

		QLuaConst* pConst = pConstList->pConstList;
		while (NULL != pConst && NULL != pConst->pszConstName)
		{
			cSafeScript->PushNumber(pConst->nValue);
			cSafeScript->SetTableField(pConst->pszConstName);

			pConst++;
		}

		cSafeScript->SetGlobalName(pConstList->pszListName);
		pConstList++;
	}
}

// -------------------------------------------------------------------------
