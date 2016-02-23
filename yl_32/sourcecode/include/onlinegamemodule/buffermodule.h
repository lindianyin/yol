/* -------------------------------------------------------------------------
//	文件名		：	buffermanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUFFER_MODULE_H__
#define __BUFFER_MODULE_H__

#include "onlinegameworld/kgcmodulebase.h"

class BufferModule : public KModuleBase
{
public:
	BufferModule();
	virtual ~BufferModule();

public:	
	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();
};

inline BufferModule* GetYLBufferModule()
{
	static BufferModule s_buffModule;
	return &s_buffModule;
}

#endif