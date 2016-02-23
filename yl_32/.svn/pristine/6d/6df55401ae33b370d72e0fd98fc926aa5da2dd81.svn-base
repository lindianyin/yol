
#pragma once

#pragma warning(disable:4200)

#include "fseye/kipcstream.h"
#include "fseye/fseye_protocol.h"
// -------------------------------------------------------------------------

#define KD_KEYES_INIT_BUFF_LEN 1024

class KEyes
{
public:
	KEyes();
	virtual ~KEyes();

	virtual INT Init(int nKey);
	virtual INT Uninit();
	virtual INT Active();
	virtual INT OnProtocal(BYTE byProtocal, BYTE* pData, size_t uDataLen);

	//int SendWarnningInfo(const char* pszWarningInfo);
	INT SendCustomInfo(const CHAR* pszCustomInfo);
	INT SendPing();
	VOID PostGmResult(INT nSession, BYTE byAction, BYTE byResult, BYTE byAsker, LPCSTR pszResult);

protected:
	KIpcStream m_Stream;
	int m_bInited;
	char* m_pszBuffer;
	size_t m_uBufferLen;

};

// -------------------------------------------------------------------------

