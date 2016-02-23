/* -------------------------------------------------------------------------
//	文件名		：	objectbuffer.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegamemodule/objectbuffer.h"
#include "./buffer/buftriggereffect.h"
#include "bufferprotocolprocessor.h"
#include "buffer/buffermanager.h"
#include "bufsetting.h"
#include <algorithm>

BOOL ObjectBuffer::Init()
{
	m_pCurrentProcessingBuffer = NULL;
	return TRUE;
}

BOOL ObjectBuffer::UnInit()
{
	return TRUE;
}

VOID ObjectBuffer::Breath()
{
	if (m_arrBuffers.size() <= 0)
	{
		return;
	}

	//删除过期Buff
	BUFFER_ARRAY swap_buffer;
	BOOL buffInValid = TRUE;
	for (size_t i = 0; i < m_arrBuffers.size(); ++i)
	{
		m_pCurrentProcessingBuffer = m_arrBuffers[i];		
		buffInValid = CheckBuffLifeTime(m_arrBuffers[i]);
		if (m_arrBuffers[i] &&
			m_arrBuffers[i]->GetReceiveCharacter() &&
			!buffInValid)
		{
			swap_buffer.push_back(m_arrBuffers[i]);
		}
	}
	swap_buffer.swap(m_arrBuffers);

	//Update剩余Buff
	const BUFFER_ARRAY mem_buffer = m_arrBuffers;//Buffer表副本，以防Buff表状态改变
	if(mem_buffer.size() <= 0)
	{
		m_pCurrentProcessingBuffer = NULL;
	}
	else
	{
		for (size_t i = 0; i < mem_buffer.size(); ++i)
		{
			m_pCurrentProcessingBuffer = mem_buffer[i];
			mem_buffer[i]->Breath();
		}
	}
	

	/*
	BUFFER_ARRAY::iterator it_buf;
	for (it_buf = m_arrBuffers.begin(); it_buf != m_arrBuffers.end(); ++it_buf)
	{
		m_pCurrentProcessingBuffer = (*it_buf);

		if (*it_buf && (*it_buf)->GetReceiveCharacter() != NULL)
		{
			if(CheckBuffLifeTime(*it_buf))
			{
				it_buf = m_arrBuffers.erase(it_buf);
				if (it_buf == m_arrBuffers.end())
				{
					break;
				}
			}
			else
			{
				(*it_buf)->Breath();
			}
		}
		else
		{
			it_buf = m_arrBuffers.erase(it_buf);
			if (it_buf == m_arrBuffers.end())
			{
				break;
			}
		}
	}*/
}

//如果在目标身上到结束期了，或者是死亡不保留并且角色死亡，就删除这个buff
BOOL ObjectBuffer::CheckBuffLifeTime(Buffer* buf)
{
	BOOL bResult = FALSE;
	BOOL bSync = FALSE;
	if(!buf->CheckSelfCondition(bSync))	
	{
//		如果是正常消亡，则不用同步到client
// 		if (buf->IsSync())
// 		{
// 			Broadcast_RemoveBuffer(buf->GetID(),buf->GetTemplateID());
// 		}
// 		
		if(bSync)
		{
			Broadcast_RemoveBuffer(buf);
		}

		g_cBufferMgr.RemoveBuffer(buf->GetID());
		bResult = TRUE;
	}
	else if(!buf->CheckSceneBuffEffect())		// 检查buff生效场景
	{
		Broadcast_RemoveBuffer(buf);
		g_cBufferMgr.RemoveBuffer(buf->GetID());
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL ObjectBuffer::CheckBuffTrigger(LPCSTR szEvent, KCharacter* pEventReceive, KCharacter* pEventSender)
{
	if (m_arrBuffers.size() <= 0)
	{
		return FALSE;
	}

	const BUFFER_ARRAY mem_buffer = m_arrBuffers;//Buffer表副本，以防Buff表状态改变
	BUFFER_ARRAY::const_iterator it_buf = mem_buffer.begin();
	for ( ; it_buf != mem_buffer.end(); ++it_buf)
	{
		if (*it_buf)
		{
			(*it_buf)->CheckTriggerCondition(szEvent, pEventReceive, pEventSender);
		}
	}
	return TRUE;
}


BOOL ObjectBuffer::HasBuffer(Buffer* buf)
{
	BUFFER_ARRAY::iterator it_buf;

	PROCESS_ERROR(buf);

	for (it_buf = m_arrBuffers.begin(); it_buf != m_arrBuffers.end(); ++it_buf)
	{
		if ((*it_buf) == buf)
			return TRUE;
	}
EXIT0:
	return FALSE;
}

Buffer* ObjectBuffer::GetBuffer(INT nBufID)
{
	BUFFER_ARRAY::iterator it_buf;
	for (it_buf = m_arrBuffers.begin(); it_buf != m_arrBuffers.end(); ++it_buf)
	{
		if ((*it_buf)->GetID() == nBufID)
			return (*it_buf);
	}

	return NULL;
}

Buffer* ObjectBuffer::GetBufferByTemplateId(INT nBufTemplateId)
{
	BUFFER_ARRAY::iterator it_buf;
	for (it_buf = m_arrBuffers.begin(); it_buf != m_arrBuffers.end(); ++it_buf)
	{
		if ((*it_buf)->GetTemplateID() == nBufTemplateId)
			return (*it_buf);
	}

	return NULL;
}


Buffer* ObjectBuffer::GetBufferByStackCategory(INT nCategory)
{
	BUFFER_ARRAY::iterator it_buf;
	for (it_buf = m_arrBuffers.begin(); it_buf != m_arrBuffers.end(); ++it_buf)
	{
		if ((*it_buf)->GetStackCategory() == nCategory)
			return (*it_buf);
	}
	return NULL;
}

Buffer* ObjectBuffer::OverWriteBuffer(INT nCategory,INT nTemplateId)
{
	Buffer* pBuf = GetBufferByStackCategory(nCategory);
	if (pBuf)
	{
		BufferTemplate* pTemplate = g_cBufferSetting.GetBufferTemplate(nTemplateId);
		LOG_PROCESS_ERROR(pTemplate);

		LOG_PROCESS_ERROR(pBuf->OnRemoveBuff());	// 消亡前进行逆运算
		INT nBufPersist = pBuf->GetBuffPersist();   // 先保存要重置的buff持续时间
		LOG_PROCESS_ERROR(pBuf->Reset());	//重置buff
		LOG_PROCESS_ERROR(pBuf->Init(pTemplate, nBufPersist));

		//Broadcast_StackNewBuffer(pBuf);
		return pBuf;
	}

EXIT0:
	return NULL;
}

Buffer* ObjectBuffer::StackBuffer(INT nCategory)
{
	Buffer* pBuf = NULL;
	pBuf = GetBufferByStackCategory(nCategory);
	LOG_PROCESS_ERROR(pBuf);

	pBuf->UpdateBufferStack();
	Broadcast_StackBuffer(pBuf);

EXIT0:
	return pBuf;
}

BOOL ObjectBuffer::AddBuffer(Buffer* buf, BOOL bSync, INT nCasterID/* = 0 */)
{
	LOG_PROCESS_ERROR(buf);
	m_arrBuffers.push_back(buf);
	if (buf->IsSync() && bSync)	//如果需要同步，则同步
	{
		Broadcast_AddBuffer(buf, TRUE, nCasterID);
	}
	return TRUE;
EXIT0:
	return FALSE;
}

BOOL ObjectBuffer::RemoveBuffer(Buffer* buf, BOOL bSync)
{
	QCONFIRM_RET_FALSE(buf);

	if(!HasBuffer(buf))
		return TRUE;

	for (BUFFER_ARRAY::iterator it_buf = m_arrBuffers.begin(); it_buf != m_arrBuffers.end(); ++it_buf)
	{
		if ((*it_buf) == buf)
		{
			m_arrBuffers.erase(it_buf);
			break;
		}
	}
	
	if (buf->IsSync() && bSync)
	{
		Broadcast_RemoveBuffer(buf);
	}

	return TRUE;
}

BOOL ObjectBuffer::CheckIgnore(const BufferTemplate* pBufferConfig)
{
	LOG_PROCESS_ERROR(pBufferConfig);

	for(size_t i = 0; i < m_arrIgnoreBuffGroup.size(); ++i)
	{
		PROCESS_SUCCESS(pBufferConfig->m_eBuffType == m_arrIgnoreBuffGroup[i]);
	}

	for(size_t i = 0; i < m_arrIgnoreBuffCate.size(); ++i)
	{
		switch(m_arrIgnoreBuffCate[i].CateType)
		{
		case 1:
			{
				PROCESS_SUCCESS(pBufferConfig->m_nStackCategory == m_arrIgnoreBuffCate[i].CateId);
			}
			break;
		case 2:
			{
				PROCESS_SUCCESS(pBufferConfig->m_nStackCategory1 == m_arrIgnoreBuffCate[i].CateId);
			}
			break;
		case 3:
			{
				PROCESS_SUCCESS(pBufferConfig->m_nStackCategory2 == m_arrIgnoreBuffCate[i].CateId);
			}
			break;
		}
	}

	for(size_t i = 0; i < m_arrIgnoreBuffID.size(); ++i)
	{
		PROCESS_SUCCESS(pBufferConfig->m_nBuffTemplateID == m_arrIgnoreBuffID[i]);
	}

EXIT0:
	return FALSE;
EXIT1:
	return TRUE;
}

VOID ObjectBuffer::AddIgnoreBuffID(INT buff_id)
{
	if (0 < buff_id)
	{
		if( std::find(m_arrIgnoreBuffID.begin(), m_arrIgnoreBuffID.end(), buff_id) == m_arrIgnoreBuffID.end() )
		{
			m_arrIgnoreBuffID.push_back(buff_id);
		}
	}
	else
	{
		buff_id = -buff_id;
		std::vector<INT>::iterator iter =
			std::find(m_arrIgnoreBuffID.begin(), m_arrIgnoreBuffID.end(), buff_id);

		if(iter != m_arrIgnoreBuffID.end())
		{
			m_arrIgnoreBuffID.erase(iter);
		}
	}
}

VOID ObjectBuffer::AddIgnoreBuffGroup(INT buff_group)
{
	if (0 < buff_group)
	{
		if( std::find(m_arrIgnoreBuffGroup.begin(), m_arrIgnoreBuffGroup.end(), buff_group) ==
			m_arrIgnoreBuffGroup.end() )
		{
			m_arrIgnoreBuffGroup.push_back(buff_group);
		}
	}
	else
	{
		buff_group = -buff_group;
		std::vector<INT>::iterator iter =
			std::find(m_arrIgnoreBuffGroup.begin(), m_arrIgnoreBuffGroup.end(), buff_group);

		if(iter != m_arrIgnoreBuffGroup.end())
		{
			m_arrIgnoreBuffGroup.erase(iter);
		}
	}
}

VOID ObjectBuffer::AddIgnoreBuffCate(INT CateType, INT CateId)
{
	CateInfo value = {CateType, CateId};

	if (0 < CateType)
	{
		if( std::find(m_arrIgnoreBuffCate.begin(), m_arrIgnoreBuffCate.end(), value) ==
			m_arrIgnoreBuffCate.end() )
		{
			m_arrIgnoreBuffCate.push_back(value);
		}
	}
	else
	{
		value.CateType = -CateType;
		std::vector<CateInfo>::iterator iter =
			std::find(m_arrIgnoreBuffCate.begin(), m_arrIgnoreBuffCate.end(), value);

		if(iter != m_arrIgnoreBuffCate.end())
		{
			m_arrIgnoreBuffCate.erase(iter);
		}
	}
}

