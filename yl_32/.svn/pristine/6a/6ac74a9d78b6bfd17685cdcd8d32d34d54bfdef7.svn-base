#ifndef __KMEDITATIONMODULE_H__
#define __KMEDITATIONMODULE_H__

#include "onlinegameworld/kmodulebase.h"

class KMeditationModule : public KModuleBase
{
public:
	KMeditationModule();
	~KMeditationModule() {}

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();
	
	BOOL EnterMeditation(LPCBYTE pData, INT nDataLen, KPlayer* rcPlayer);

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:

};

extern KMeditationModule g_cMeditationModule;

#endif
