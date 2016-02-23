/* -------------------------------------------------------------------------
//	文件名		：	bufferprotocolprocessor.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "onlinegamemodule/buffermodule.h"
#include "bufferprotocolprocessor.h"
#include "bufferprotocol.h"
#include "onlinegameworld/kplayerprotocolprocess.h"
#include "onlinegameworld/kcharacter.h"
#include "buffer.h"

BOOL BufferProtocolProcessor::Broadcast_AddCharacterBuffer(KCharacter& rcHost,CONST Buffer* pBuf,BOOL bAddOnLogin, INT nCasterID)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetYLBufferModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	BUFFER_ADDBUFFER cAddBuffer;
	cAddBuffer.dwBufferID = pBuf->GetID();
	cAddBuffer.dwCasterID = nCasterID;
	cAddBuffer.nBuffTemplateId = pBuf->GetTemplateID();
	cAddBuffer.bGroundBuffer = FALSE;
	cAddBuffer.bStackChange = FALSE;
	cAddBuffer.bAddOnLogin = bAddOnLogin;
	cAddBuffer.nX = 0;
	cAddBuffer.nY = 0;
	cAddBuffer.nZ = 0;
	cAddBuffer.nBuffInterval = pBuf->GetCurrentBuffPersist();
	cAddBuffer.nStackCount = pBuf->GetStatckCount();
	cAddBuffer.nCharacterID = rcHost.GetId();

	return rcHost.m_cCharacterServer.BroadcastNearby(&cAddBuffer, sizeof(cAddBuffer), TRUE, pProtocolProcessor);
}


BOOL BufferProtocolProcessor::Broadcast_StackCharacterBuffer(KCharacter& rcHost,CONST Buffer* pBuf)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetYLBufferModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	BUFFER_STACKBUFFER cStackBuffer;
	cStackBuffer.dwCharacterId = rcHost.GetId();
	KCharacter* pCaster = pBuf->GetCasterCharacter();
	cStackBuffer.dwCasterID = 0;
	if (pCaster != NULL)
	{
		cStackBuffer.dwCasterID = pCaster->GetId();
	}
	cStackBuffer.dwBufferID = pBuf->GetID();
	cStackBuffer.nBuffTemplateId = pBuf->GetTemplateID();
	cStackBuffer.nBuffInterval = pBuf->GetCurrentBuffPersist();
	cStackBuffer.byStack = pBuf->GetStatckCount();
	
	return rcHost.m_cCharacterServer.BroadcastNearby(&cStackBuffer, sizeof(cStackBuffer), TRUE, pProtocolProcessor);
}


BOOL BufferProtocolProcessor::Broadcast_RemoveCharacterBuffer(KCharacter& rcHost, Buffer* pBuf)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetYLBufferModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	BUFFER_REMOVEBUFFER cRemoveBuffer;
	cRemoveBuffer.dwBufferID = pBuf->GetID();
	cRemoveBuffer.nBuffTemplateId = pBuf->GetTemplateID();
	cRemoveBuffer.bGroundBuffer = FALSE;
	cRemoveBuffer.dwCharacterId = rcHost.GetId();

	return rcHost.m_cCharacterServer.BroadcastNearby(&cRemoveBuffer, sizeof(cRemoveBuffer), TRUE, pProtocolProcessor);
}

// 对地buffer广播，放在groundbuffer里面
