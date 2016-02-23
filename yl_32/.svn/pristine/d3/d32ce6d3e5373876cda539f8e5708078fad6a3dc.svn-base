#include "stdafx.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "kmeditationmodule.h"
#include "kmeditationctrl.h"
#include "kmeditationprotocol.h"
#include "Runtime/TypeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMeditationModule g_cMeditationModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_MEDITATION>()
{
	return &g_cMeditationModule;
}

KMeditationModule::KMeditationModule() : KModuleBase(emMODULE_MEDITATION)
{}

BOOL KMeditationModule::OnInit()
{
	return TRUE;
}

BOOL KMeditationModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(QC2S_ENTER_MEDITATION);

	return TRUE;
}

BOOL KMeditationModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_enter_meditation:
		// by ldy
		EnterMeditation(pbData, uDataLen, pPlayer);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL KMeditationModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cMeditationModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KMeditationModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cMeditationModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL KMeditationModule::EnterMeditation( LPCBYTE pData, INT nDataLen, KPlayer* rcPlayer )
	
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	if(rcPlayer->GetIsFightMap() > 0)
	{
		cSafeScript->CallTableFunction("Meditation","ErrorMsg",0,"os",rcPlayer->GetScriptInterface(),LANG_002);
	}
	else if(rcPlayer->IsSwim())
	{
		cSafeScript->CallTableFunction("Meditation","ErrorMsg",0,"os",rcPlayer->GetScriptInterface(),LANG_003);
	}
	else if(rcPlayer->GetMeditationState())
	{
		cSafeScript->CallTableFunction("Meditation","ErrorMsg",0,"os",rcPlayer->GetScriptInterface(),LANG_004);
	}
	else if(rcPlayer->GetMoveState() != cmsOnStand)
	{
		cSafeScript->CallTableFunction("Meditation","ErrorMsg",0,"os",rcPlayer->GetScriptInterface(),LANG_005);
	}
	else
	{
		rcPlayer->SetMoveState(cmsOnSit);
		rcPlayer->SetMeditationState(TRUE);
		//重置脚本中的累计奖励
		cSafeScript->CallTableFunction("Meditation", "ResetOnlineTotalExp", 0, "o", rcPlayer->GetScriptInterface());
		//打开在线打坐面板
		cSafeScript->CallTableFunction("Meditation", "ShowMeditationOnlinePanel", 0, "o", rcPlayer->GetScriptInterface());
	}

EXIT0:
	return TRUE;
}
