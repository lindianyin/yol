#include "stdafx.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "remindmodule.h"
#include "remindprotocol.h"

RemindModule g_cRemindModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_REMIND>()
{
	return &g_cRemindModule;
}

RemindModule::RemindModule() : KModuleBase(emMODULE_REMIND)
{

}

RemindModule::~RemindModule()
{
}

BOOL RemindModule::OnInit()
{
	return TRUE;
}

BOOL RemindModule::OnUnInit()
{
	return TRUE;
}

BOOL RemindModule::Breath()
{
	return TRUE;
}

BOOL RemindModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(PTC_S2C_ADD_REMIND);
	KD_PTC_FIXED_SIZE_S2C(PTC_S2C_REMOVE_REMIND);
	return TRUE;
}

BOOL RemindModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	return TRUE;
}