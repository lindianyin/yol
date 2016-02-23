#pragma once
#include "onlinegameworld/kmodulebase.h"
#include "gemprotocol.h"

class KPlayer;

class GemModule : public KModuleBase
{
public:
	GemModule();
	virtual ~GemModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

};

extern GemModule g_cGemModule;