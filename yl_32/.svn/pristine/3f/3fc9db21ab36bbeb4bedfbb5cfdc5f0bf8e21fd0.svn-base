#include "stdafx.h"
#include "titlemanager.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "playertitle.h"
#include "titledef.h"

TitleManager g_cTitleManager;
TitleManager::TitleManager()
{

}

TitleManager::~TitleManager()
{

}

BOOL TitleManager::Init()
{
	Watch(emKOBJEVENTTYPE_CHARACTER_VIP_LEVEL);
	Watch(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_1);
	Watch(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_2);
	Watch(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_3);
	return TRUE;
}

BOOL TitleManager::UnInit()
{
	CancelWatch(emKOBJEVENTTYPE_CHARACTER_VIP_LEVEL);
	CancelWatch(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_1);
	CancelWatch(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_2);
	CancelWatch(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_3);
	return TRUE;
}

BOOL TitleManager::Breath()
{
	return TRUE;
}

BOOL TitleManager::OnEvent(KEventCreator& rcSender, CONST KOBJEVENT& rcEvent)
{

	INT nId = rcEvent.anEventParam[0];
	INT nOld = rcEvent.anEventParam[1];
	INT nNew = rcEvent.anEventParam[2];
	PlayerTitle* pPlayerTitle = NULL;

	KPlayer* pPlayer = g_cPlayerMgr.GetById(nId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerTitle = pPlayer->GetCtrl<PlayerTitle>();
	LOG_PROCESS_ERROR(pPlayerTitle);
	switch (rcEvent.nEvent)
	{
	case emKOBJEVENTTYPE_CHARACTER_VIP_LEVEL:
		if(!nNew)
		{
			RemoveTitleToCharacter(nId, 2, nOld, 0);
		}
		else
		{
			AddTitleToCharacter(nId, 2, nNew);
		}
		
		break;

	case emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_1:
		pPlayerTitle->SetTitleCarray_1(nOld, nNew);
		break;

	case emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_2:
		pPlayerTitle->SetTitleCarray_2(nOld, nNew);
		break;

	case emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_3:
		pPlayerTitle->SetTitleCarray_3(nOld, nNew);
		break;
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL TitleManager::AddTitleToCharacter(DWORD dwId, INT nGetWay, INT nLevel, INT nType/* = 0*/, DWORD dwPassTime)
{
	PlayerTitle* pPlayerTitle = NULL;
	INT nRoleLevel =0;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerTitle = pPlayer->GetCtrl<PlayerTitle>();
	LOG_PROCESS_ERROR(pPlayerTitle);
	nRoleLevel = pPlayer->GetLevel();
	//by ldy称号等级限制
	if(7  == nGetWay)
	{
		LOG_PROCESS_ERROR(nRoleLevel >=15);
	}
	
	if(8  == nGetWay)
	{
		LOG_PROCESS_ERROR(nRoleLevel >=35);
	}
	
	if(15  == nGetWay)
	{
		LOG_PROCESS_ERROR(nRoleLevel >=35);
	}

	if(13  == nGetWay || 14  == nGetWay )
	{
		LOG_PROCESS_ERROR(nRoleLevel >=	32);
	}

	if(16  == nGetWay )
	{
		LOG_PROCESS_ERROR(nRoleLevel >= 30);
	}
	
	switch(nType)
	{
	case emTITLE_TYPE_RANK:
	case emTITLE_TYPE_ACTIVE:
		pPlayerTitle->AddTitleToCharacter1(nGetWay, nLevel, nType, dwPassTime);
		break;

	case emTITLE_TYPE_ACCUMULATIVE:
		pPlayerTitle->AddTitleToCharacter2(nGetWay, nLevel, nType, dwPassTime);
		break;
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL TitleManager::RemoveTitleToCharacter(DWORD dwId, INT nGetWay, INT nLevel, INT nType)
{

	PlayerTitle* pPlayerTitle = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerTitle = pPlayer->GetCtrl<PlayerTitle>();
	LOG_PROCESS_ERROR(pPlayerTitle);
	pPlayerTitle->RemoveTitleToCharacter(nGetWay, nLevel, nType);

	return TRUE;

EXIT0:
	return FALSE;
}