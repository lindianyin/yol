

#include "stdafx.h"
#include "korpgmodule.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kplayer.h"
#include "item2/kplayeritem.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
//#include "onlinegamemodule/kcharacterbuffer.h"

#include "onlinegamemodule/ktaskctrl.h"
#include "onlinegamemodule/ai/kaictrl.h"
#include "onlinegamemodule/chat/kplayerchat.h"
#include "findpath/kplayerfindpath.h"
#include "onlinegamemodule/mail/kmailctrl.h"
#include "onlinegamemodule/kprogressbar.h"
#include "onlinegamemodule/characterbuffer.h"
#include "onlinegamemodule/playerfaction.h"
#include "onlinegamemodule/characterskill.h"
#include "onlinegamemodule/qplayerteam.h"
#include "pet/kpetctrl.h"
#include "pet/kridectrl.h"
#include "award/kawardctrl.h"
#include "tower/ktowerctrl.h"
#include "stage/kstagectrl.h"
#include "bladestate/kbladectrl.h"
#include "instance/kinstancectrl.h"
#include "activity/kactivityctrl.h"
#include "quiz/playerquiz.h"
#include "arena/playerarena.h"
#include "title/playertitle.h"
#include "meditation/kmeditationctrl.h"
#include "rank/krankctrl.h"
#include "target/playertarget.h"
#include "vip/playervip.h"
#include "awardlobby/playerawardlobby.h"
#include "gem/playergem.h"
#include "crosssv/kcrosssvctrl.h"
#include "artifact/playerartifact.h"
#include "escort/kescortctrl.h"
#include "bank/playerbank.h"
#include "opense/playeropense.h"
#include "onlinegamemodule/kmoduledef.h"
#include "onlinegameworld/kmoduleinterface.h"
#include "onlinegamebase/ktimermgr.h"
#include "onlinegamebase/ktimeline.h"
#include "../onlinegamemodule/trade/qshopmodule.h"
#include "../onlinegamemodule/trade/qplayershop.h"

#include "../nsworld/knpchelper.h"
#include "../nsworld/kcharacterhelper.h"
#include "ai/ThreatList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


VOID g_SetDoingFilter();

extern BOOL g_InitCharacterVarChange();
// -------------------------------------------------------------------------
KOrpgModule _gcOrgpModule;

template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_RPG>()
{
	return &_gcOrgpModule;
}
// -------------------------------------------------------------------------
KOrpgModule::KOrpgModule() : KModuleBase(emKMODULE_RPG)
{
}

BOOL KOrpgModule::PreInit()
{
	_RegisterCtrl();

	g_InitCharacterVarChange();

	g_SetDoingFilter();

	g_cNpcHelper.Init();
	g_cCharacterHelper.Init();

	return TRUE;
}

BOOL KOrpgModule::OnInit()
{
	g_cTimeline.Init();	// 原生C++的定时器，二者暂未整合
	return TRUE;
}

BOOL KOrpgModule::OnUnInit()
{
	g_cTimeline.Init();
	return TRUE;
}

BOOL KOrpgModule::Breath()
{
	g_cTimeline.Activate();
	return TRUE;
}

// -------------------------------------------------------------------------

void KOrpgModule::_RegisterCtrl()
{
	// 注册CharacterCtrl
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KTaskCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KAICtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KAIVM>();

	// 注册PlayerCtrl
	//KPlayer::TCTRL_FACTORY::RegisterCtrl<KPlayer, KPlayerPK>();
	//KPlayer::TCTRL_FACTORY::RegisterCtrl<KPlayer, KPlayerUiScript>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KPlayerItem>();

	//KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KPlayerRecipe>();	

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KPlayerPurse>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KPlayerChat>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KPlayerFindPath>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KMailCtrl>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KPlayerProgressBar>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, CharacterBuffer>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerFaction>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, CharacterSkill>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, QPlayerTeam>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KPetCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KRideCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KSimpThreatList>();
	
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KAwardCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerQuiz>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerArena>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KTowerCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KStageCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KInstanceCtrl>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerTitle>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KMeditationCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KActivityCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KRankCtrl>();

	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerTarget>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerVip>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerAwardLobby>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KBladeCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerGem>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KCrossSvCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerArtifact>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, KEscortCtrl>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerBank>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerShop>();
	KTCharacterCtrlFactory::RegisterCtrl<KCharacter, PlayerOpense>();
}

static IKModuleInterface* s_aModuleList[] = 
{
	GetModuleInterFace<emKMODULE_RPG>(), // 基础模块

	GetModuleInterFace<emMODULE_TEAM>(),
	GetModuleInterFace<emKMODULE_ITEM>(),

	GetModuleInterFace<emKMODULE_TASK>(),
	//GetModuleInterFace<emKMODULE_RECIPE>(),
	GetModuleInterFace<emKMODULE_PURSE>(),
	GetModuleInterFace<emKMODULE_CHAT_NEW>(),

	GetModuleInterFace<emKMODULE_FINDPATH>(),
	GetModuleInterFace<emKMODULE_AI>(),
	GetModuleInterFace<emKMODULE_MAIL>(),
	GetModuleInterFace<emMODULE_KIN>(),
	GetModuleInterFace<emMODULE_RELATION>(),

#ifdef GAME_SERVER
	GetModuleInterFace<emKMODULE_GLOBALDATA>(),

#endif
	GetModuleInterFace<emMODULE_BUFFER_>(),
	GetModuleInterFace<emMODULE_FIGHT_SKILL>(),
	GetModuleInterFace<emMODULE_FACTION>(),
	GetModuleInterFace<emMODULE_SHOP>(),
	//GetModuleInterFace<emMODULE_MYSTERYSHOP>(),
	GetModuleInterFace<emMODULE_PET>(),
	GetModuleInterFace<emMODULE_ESCORT>(),
	GetModuleInterFace<emMODULE_AWARD>(),
	GetModuleInterFace<emMODULE_QUIZ>(),

	GetModuleInterFace<emMODULE_ARENA>(),
	GetModuleInterFace<emMODULE_TOWER>(),
	GetModuleInterFace<emMODULE_STAGE>(),
	GetModuleInterFace<emMODULE_TITLE>(),
	GetModuleInterFace<emMODULE_INSTANCE>(),
	GetModuleInterFace<emMODULE_MEDITATION>(),
	GetModuleInterFace<emMODULE_REMIND>(),
	GetModuleInterFace<emMODULE_RANK>(),
	GetModuleInterFace<emMODULE_TARGET>(),
	GetModuleInterFace<emMODULE_ACTIVITY>(),
	GetModuleInterFace<emMODULE_VIP>(),
	GetModuleInterFace<emMODULE_AWARD_LOBBY>(),
	GetModuleInterFace<emMODULE_BLADE>(),
	GetModuleInterFace<emMODULE_GEM>(),
	GetModuleInterFace<emMODULE_ARTIFACT>(),
	GetModuleInterFace<emMODULE_BANK>(),
	GetModuleInterFace<emMODULE_SHOP>(),
	GetModuleInterFace<emMODULE_Opense>(),
	NULL	// 以NULL结尾 
};
// -------------------------------------------------------------------------
IKModuleInterface** g_GetModuleList()
{
	return s_aModuleList;
}

