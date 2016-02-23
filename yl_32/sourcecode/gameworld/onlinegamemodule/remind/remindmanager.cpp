#include "stdafx.h"
#include "remindmanager.h"
#include "remindprotocol.h"
#include "remindmodule.h"
#include "onlinegamebase/ksysservice.h"

RemindManager g_cRemindManger;

RemindManager::RemindManager()
{

}

RemindManager::~RemindManager()
{

}

BOOL RemindManager::AddRemindToPlayer(INT nPlayerIndex, INT nRemindId, INT nCountDown)
{
	//QLogPrintf(LOG_INFO,"..RemindManager::AddRemindToPlayer(INT nPlayerIndex=%d, INT nRemindId=%d, INT nCountDown=%d)",
	//	nPlayerIndex,nRemindId,nCountDown);
	PTC_S2C_ADD_REMIND ptcAddRemind;
	ptcAddRemind.byProtocol = (BYTE)emS2C_ADD_REMIND;
	ptcAddRemind.nRemindId = nRemindId;
	ptcAddRemind.nCountDown = nCountDown;
	ptcAddRemind.nStartTime = (UINT)KSysService::GameTime(NULL);

	return g_cRemindModule.GetProtocolProcessor()->SendData(nPlayerIndex, ptcAddRemind);
}

BOOL RemindManager::RemoveRemindToPlayer(INT nPlayerIndex, INT nRemindId)
{
	QLogPrintf(LOG_INFO,"..RemindManager::RemoveRemindToPlayer(INT nPlayerIndex=%d, INT nRemindId=%d)",
		nPlayerIndex,nRemindId);
	PTC_S2C_REMOVE_REMIND ptcRemoveRemind;
	ptcRemoveRemind.byProtocol = (BYTE)emS2C_REMOVE_REMIND;
	ptcRemoveRemind.nRemindId = nRemindId;

	return g_cRemindModule.GetProtocolProcessor()->SendData(nPlayerIndex, ptcRemoveRemind);

}