#pragma once
#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "../../LibQrt/Include/Misc/objevent.h"
#include "config/kplayerbasedbdef.h"

class BankManager
{
public:
	BankManager();
	~BankManager();

	BOOL Init();
	BOOL UnInit();
	BOOL Breath();

	BOOL OnGetBankInvestRecordRespond(BYTE* pRecordData, size_t uRecordDataLen);

	//BOOL GetBankInvestRecord(std::vector<KBankInvestRecord>& vecBankInvestRecord);

	CONST std::vector<KBankInvestRecord>& GetBankInvestRecord(){ return m_vecBankInvestRecord;}

public: 
	std::vector<KBankInvestRecord> m_vecBankInvestRecord;
};

extern BankManager g_cBankManager;