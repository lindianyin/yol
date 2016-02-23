/* -------------------------------------------------------------------------
//	文件名		：	buffermanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "onlinegamemodule/characterbuffer.h"
#include "./buffer/buftriggereffect.h"
#include "buffer/buffermanager.h"
#include "bufferprotocolprocessor.h"

CharacterBuffer::CharacterBuffer(KCharacter& rcPlayer)
    : KCharacterCtrlBase(rcPlayer),m_cLunaObj(rcPlayer)
{

}

CharacterBuffer::~CharacterBuffer()
{

}

BOOL CharacterBuffer::Init()
{
	ObjectBuffer::Init();
	m_mapTransParam.clear();
	return TRUE;
}

BOOL CharacterBuffer::UnInit()
{
	ObjectBuffer::UnInit();
	m_arrBuffers.clear();
	return TRUE;
}

BOOL CharacterBuffer::OnLogin()
{
	return TRUE;
}

//如果玩家下线，需要将玩家身上对应的buff进行清理，否则会有内存泄漏
BOOL CharacterBuffer::OnLogout()
{
	for (BUFFER_ARRAY::iterator it_buf = m_arrBuffers.begin(); it_buf != m_arrBuffers.end(); ++it_buf)
	{
		if (*it_buf)
		{
			if(!(*it_buf)->IsSave())
			{
				g_cBufferMgr.RemoveBuffer((*it_buf)->GetID());
			}
		}
	}

	return TRUE;
}

BOOL CharacterBuffer::Active()
{
	QLunaBase* pOldMe = NULL;
	ObjectBuffer::Breath();

	return TRUE;
}

BOOL CharacterBuffer::Broadcast_AddBuffer(CONST Buffer* pBuf,BOOL bAddOnLogin /* = FALSE */, INT nCasterID/* = 0 */)
{
	if (pBuf->IsSync())
	{
	    BufferProtocolProcessor::Broadcast_AddCharacterBuffer(m_rcCharacter,pBuf,bAddOnLogin, nCasterID);
	}
	
	return TRUE;
}

BOOL CharacterBuffer::Broadcast_StackBuffer(CONST Buffer* pBuf)
{
	if (pBuf->IsSync())
	{
        BufferProtocolProcessor::Broadcast_StackCharacterBuffer(m_rcCharacter,pBuf);
	}

	return TRUE;
}

BOOL CharacterBuffer::Broadcast_RemoveBuffer(Buffer* pBuf)
{
	if (pBuf->IsSync())
	{
	    BufferProtocolProcessor::Broadcast_RemoveCharacterBuffer(m_rcCharacter, pBuf);
	}
	
	return TRUE;
}

BOOL CharacterBuffer::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	INT nTotalSize = 0;
	LOG_PROCESS_ERROR(pBuffer);

	for (BUFFER_ARRAY::iterator it = m_arrBuffers.begin(); it != m_arrBuffers.end(); ++it)
	{
		Buffer* pBuf = *it;
		QCONFIRM_RET_FALSE(pBuf);

		if(!pBuf->IsSave()) 
			continue;

		BufferSaveData* pBufferData = (BufferSaveData*)pBuffer;
		KCharacter* pCaster = pBuf->GetCasterCharacter();
		LOG_PROCESS_ERROR(pCaster);

		pBufferData->dwCasterId = pCaster->GetId();
		pBufferData->nBufferTemplateId = pBuf->GetTemplateID();
		pBufferData->nSkillId = pBuf->GetCastSkill();
		pBufferData->nRemainPersist = pBuf->GetCurrentBuffPersist();

		pBuffer += sizeof(BufferSaveData);
		uUsedSize += sizeof(BufferSaveData);
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL CharacterBuffer::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	if (uSize == 0)
		return TRUE;

	if (pData == NULL || uSize < sizeof(BufferSaveData))
	{
		QLogPrintf(LOG_ERR, "[CharacterBuffer::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(BufferSaveData)");
		return FALSE;
	}

	SIZE_T uLoadBuffSize = 0;
	while (uLoadBuffSize < uSize)
	{
		if (uLoadBuffSize + sizeof(BufferSaveData) > uSize)
		{
			QLogPrintf(LOG_ERR, "[CharacterBuffer::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}

		BufferSaveData* pBufData = (BufferSaveData*)pData;
		g_cBufferMgr.AddBufferToCharacter(pBufData->dwCasterId, pBufData->nSkillId, m_rcCharacter.GetId(), pBufData->nBufferTemplateId, pBufData->nRemainPersist);
		
		pData += sizeof(BufferSaveData);
		uLoadBuffSize += sizeof(BufferSaveData);
	}
	return TRUE;
}

BOOL CharacterBuffer::OnClientReady()
{
	return TRUE;
}

BOOL CharacterBuffer::SetTransParam(INT nSrcParam, INT nDstParam, DOUBLE dPercent)
{
	MAP_TRANSPARAM_DATA mapParam;
	MAP_TRANS_PARAM::iterator it = m_mapTransParam.find(nSrcParam);

	if(it != m_mapTransParam.end())
	{
		mapParam = it->second;
	}
	mapParam[nDstParam] = dPercent;

	m_mapTransParam[nSrcParam] = mapParam;

	return TRUE;
}

VOID CharacterBuffer::GetTransParam(INT nSrcParam, MAP_TRANSPARAM_DATA& mapParam)
{
	MAP_TRANS_PARAM::iterator it = m_mapTransParam.find(nSrcParam);

	if(it != m_mapTransParam.end())
	{
		mapParam = it->second;
	}
}

CONST LuaCharacterBufferEffect& CharacterBuffer::GetScriptInterface() CONST
{
	return m_cLunaObj;
}