#pragma once
#include "onlinegameworld/kmodulebase.h"
#include "artifactprotocol.h"

class KPlayer;

class ArtifactModule : public KModuleBase
{
public:
	ArtifactModule();
	virtual ~ArtifactModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

};

extern ArtifactModule g_cArtifactModule;