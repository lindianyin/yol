#include "stdafx.h"
#include "bankmanager.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "playerbank.h"
#include "bankdef.h"
#include "serverbase/RecordDef.h"
#include "../record/RecordRelayClient.h"

BankManager g_cBankManager;
BankManager::BankManager()
{

}

BankManager::~BankManager()
{

}

BOOL BankManager::Init()
{
	RecordRelayClient::DoGetServerRecordRequest(emRecordBankInvest);
	return TRUE;
}

BOOL BankManager::UnInit()
{
	return TRUE;
}

BOOL BankManager::Breath()
{
	return TRUE;
}

BOOL BankManager::OnGetBankInvestRecordRespond(BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL		bResult = FALSE;
	BYTE*		pBuffer = pRecordData;
	size_t		uPakSize = uRecordDataLen;
	std::vector<KPlayer*>			vPlayers;

	m_vecBankInvestRecord.clear();

	while(uPakSize >= sizeof(KBankInvestRecord))
	{
		KBankInvestRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KBankInvestRecord));
		m_vecBankInvestRecord.push_back(sRecord);
		pBuffer += sizeof(KBankInvestRecord);
		uPakSize -= sizeof(KBankInvestRecord);
	}

	g_cPlayerMgr.GetAllPlayers(vPlayers);
	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		PlayerBank *pPlayerBank = pPlayer->GetCtrl<PlayerBank>();
		pPlayerBank->SyncAllInvestInfo();
	}

	LOG_PROCESS_ERROR(uPakSize == 0);

	bResult = TRUE;
EXIT0:
	return bResult;
}
