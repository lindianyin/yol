#include "stdafx.h"
#include "titlemodule.h"
#include "titlemanager.h"
#include "titleprotocol.h"
#include "titlesetting.h"

TitleModule g_TitleModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_TITLE>()
{
	return &g_TitleModule;
}

TitleModule::TitleModule() : KModuleBase(emMODULE_TITLE)
{

}

TitleModule::~TitleModule()
{

}

BOOL TitleModule::Breath()
{
	return TRUE;
}

BOOL TitleModule::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_S2C(TITLE_S2C_SYNC_TITLE);
	KD_PTC_FIXED_SIZE_S2C(TITLE_S2C_ADD_TITLE);
	return TRUE;
}

BOOL TitleModule::OnInit()
{
	g_cTitleManager.Init();
	g_cTitleSetting.Init();
	return TRUE;
}

BOOL TitleModule::OnUnInit()
{
	g_cTitleManager.UnInit();
	g_cTitleSetting.UnInit();
	return TRUE;
}

BOOL TitleModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	return TRUE;
}
