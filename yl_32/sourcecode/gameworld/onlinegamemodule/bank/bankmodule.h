#pragma once
#include "onlinegameworld/kmodulebase.h"
#include "bankprotocol.h"

class KPlayer;

class BankModule : public KModuleBase
{
public:
	BankModule();
	virtual ~BankModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

};

extern BankModule g_cBankModule;