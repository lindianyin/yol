/* -------------------------------------------------------------------------
//	文件名		：	buffermanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "onlinegamemodule/buffermodule.h"
#include "bufferprotocol.h"
#include "buffer/buffermanager.h"
#include "bufsetting.h"

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_BUFFER_>()
{
	return GetYLBufferModule();
}

BufferModule::BufferModule() : KModuleBase(emMODULE_BUFFER_)
{

}

BufferModule::~BufferModule()
{

}

BOOL BufferModule::OnInit()
{
	KModuleBase::OnInit();

	g_cBufferSetting.Init();
	g_cBufferMgr.Init();

	return TRUE;
}

BOOL BufferModule::OnUnInit()
{
	KModuleBase::OnUnInit();

	g_cBufferMgr.UnInit();
	g_cBufferSetting.UnInit();

	return TRUE;
}

BOOL BufferModule::Breath()
{
	KModuleBase::Breath();

	g_cBufferMgr.Breath();

	return TRUE;
}

BOOL BufferModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	return TRUE;
}

BOOL BufferModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(BUFFER_ADDBUFFER);
	KD_PTC_FIXED_SIZE_S2C(BUFFER_REMOVEBUFFER);
	KD_PTC_FIXED_SIZE_S2C(BUFFER_STACKBUFFER);
	return TRUE;
}