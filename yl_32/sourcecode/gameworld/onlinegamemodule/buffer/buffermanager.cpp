/* -------------------------------------------------------------------------
//	文件名		：	buffermanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "buffer/buffermanager.h"
#include "onlinegamemodule/bufferdef.h"
#include "./buffer/buftriggereffect.h"
#include "buffer/buftimereffect.h"
#include "buffer/bufsetting.h"
#include "buffer/buftriggereffect.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kcharactermgr.h"
#include "onlinegamemodule/characterbuffer.h"

BufferManager g_cBufferMgr;

BufferManager::BufferManager()
{

}

BufferManager::~BufferManager()
{
	UnInit();
}

BOOL BufferManager::Init()
{
	m_cBufferPool.Init(BUFFER_POOL_MAX_COUNT);
	m_cGroundBufferPool.Init(GROUND_BUFFER_POOL_MAX_COUNT);

	return TRUE;
}

BOOL BufferManager::UnInit()
{	
	m_cBufferPool.Uninit();
	m_cGroundBufferPool.Uninit();
	return TRUE;
}

INT BufferManager::AddBuffer(INT nTemplateId, INT nPersist /* = 0 */, DWORD dwId /* = KD_BAD_ID */)
{
	BOOL bOk = FALSE;
	INT nIndex = 0;
	Buffer* pBuf = NULL;

	BufferTemplate* pTemplate = g_cBufferSetting.GetBufferTemplate(nTemplateId);
	LOG_PROCESS_ERROR(pTemplate);

	nIndex = m_cBufferPool.Add(dwId);

	pBuf = m_cBufferPool.GetByIndex(nIndex);
	LOG_PROCESS_ERROR(pBuf);

	bOk = pBuf->Init(pTemplate, nPersist);
	LOG_PROCESS_ERROR(bOk && "Buffer Init Failed!");
	
EXIT0:
	return nIndex;
}

BOOL BufferManager::RemoveBuffer(DWORD dwId,BOOL bClearEffect)
{
	Buffer* pBuf = m_cBufferPool.GetById(dwId);
	QCONFIRM_RET_FALSE(pBuf);
	if (bClearEffect)
	{
		pBuf->OnRemoveBuff();	//消亡前进行逆运算
	}
	pBuf->Reset();
	BOOL bRet = m_cBufferPool.Remove(dwId);

	return bRet;
}

Buffer* BufferManager::GetBufferById(DWORD nID)
{
	return m_cBufferPool.GetById(nID);
}

Buffer* BufferManager::GetBufferByIndex(INT nIndex)
{
	return m_cBufferPool.GetByIndex(nIndex);
}

UINT BufferManager::GetIndexById(DWORD nID)
{
	UINT uIndex = m_cBufferPool.GetIndexById(nID);
	LOG_PROCESS_ERROR(uIndex);

	return uIndex;
EXIT0:
	return 0;
}

VOID BufferManager::Breath()
{
	for (INT i = 0; i <= m_cGroundBufferPool.GetMaxCount(); ++i)
	{
		m_pCurrentProcessingGroundBuff = m_cGroundBufferPool.GetByIndex(i);
		if (m_pCurrentProcessingGroundBuff)
		{
			m_pCurrentProcessingGroundBuff->Breath();
		}
	}
	m_pCurrentProcessingGroundBuff = NULL;
}

VOID BufferManager::RemoveGroundBufferBySceneId(INT nSceneId)
{
	for (INT i = 0; i <= m_cGroundBufferPool.GetMaxCount(); ++i)
	{
		GroundBuffer* pBuf = m_cGroundBufferPool.GetByIndex(i);
		if(pBuf && pBuf->GetSceneId() == nSceneId)
		{
			m_pCurrentProcessingGroundBuff = pBuf;
			pBuf->RemoveFromPool();
			m_pCurrentProcessingGroundBuff = NULL;
		}
	}
}

BOOL BufferManager::RemoveGroundBufferByCasterId(INT nCasterId, DWORD dwTemplateId /* = 0 */)
{
	for (INT i = 0; i <= m_cGroundBufferPool.GetMaxCount(); ++i)
	{
		GroundBuffer* pBuf = m_cGroundBufferPool.GetByIndex(i);
		if(pBuf && pBuf->IsActive())
		{
			KCharacter* pCaster = pBuf->GetBufCaster();
			if(pCaster && pCaster->GetId() == nCasterId)
			{
				if(!dwTemplateId || (dwTemplateId > 0 && pBuf->GetTemplateId() == dwTemplateId))
				{
					RemoveGroundBuffer(pBuf->GetID());
				}
			}
		}
	}
	return TRUE;
}

BOOL BufferManager::CheckCharacterBuffTrigger(DWORD dwCharacterID,
											  LPCSTR szEvent, 
											  KCharacter* pEventSender, 
											  KCharacter* pEventReceiver /* = NULL */)
{
	CharacterBuffer* characterBuffer = NULL;
	GroundBuffer* pGroundProcBuff = NULL;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(dwCharacterID);
	if(pCharacter == NULL)
		return FALSE;

	characterBuffer = pCharacter->GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(characterBuffer);

	if(pEventReceiver)
		characterBuffer->CheckBuffTrigger(szEvent, pEventReceiver, pEventSender);
	else
		characterBuffer->CheckBuffTrigger(szEvent, pCharacter, pEventSender);

	// 处理GroundBuffer的Trigger
	pGroundProcBuff = g_cBufferMgr.GetCurrentProcessingBuff();
	if(pGroundProcBuff)
	{
		vector<KCharacter*> tCharacterVec;
		tCharacterVec.push_back(pCharacter);
		pGroundProcBuff->CheckTriggerCondition(szEvent, tCharacterVec);
	}
	return TRUE;
EXIT0:
	return FALSE;
}

Buffer* BufferManager::ReplaceBuffer(KCharacter* pCharacter, Buffer* pBuf, Buffer* pOldBuf)
{
	Buffer* pNewBuf = NULL;
	CharacterBuffer* pCharacterBuffer = pCharacter->GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	// 高等级强制替换
	if (pBuf->GetBuffLevel() > pOldBuf->GetBuffLevel())
	{
		pCharacterBuffer->Broadcast_RemoveBuffer(pOldBuf);
		pNewBuf = pCharacterBuffer->OverWriteBuffer(pBuf->GetStackCategory(), pBuf->GetTemplateID());
		LOG_PROCESS_ERROR(pNewBuf);
		RemoveBuffer(pBuf->GetID(), FALSE);
		pCharacterBuffer->Broadcast_AddBuffer(pNewBuf, TRUE);
		goto EXIT0;
	}

	switch ((BufferStackDetail)pBuf->GetBuffCategory(2))
	{
	// 刷新buff时间
	case emUpdate:
		{
			pOldBuf->UpdateInterval(FALSE);
			pCharacterBuffer->Broadcast_StackBuffer(pOldBuf);
			pNewBuf = pOldBuf;
			RemoveBuffer(pBuf->GetID(), FALSE);
			break;
		}
	// 新来的强制替换
	case emReplace:
		{
			pNewBuf = pCharacterBuffer->OverWriteBuffer(pBuf->GetStackCategory(), pBuf->GetTemplateID());
			LOG_PROCESS_ERROR(pNewBuf);
			//pCharacterBuffer->Broadcast_RemoveBuffer(pBuf);
			RemoveBuffer(pBuf->GetID(), FALSE);
			pCharacterBuffer->Broadcast_AddBuffer(pNewBuf, TRUE);
			break;
		}
	// 强制不替换
	case emForbid:
		{
			pNewBuf = pOldBuf;
			RemoveBuffer(pBuf->GetID(), FALSE);
			break;
		}
	// 叠加剩余时间
	case emStackTime:
		{
			pOldBuf->UpdateInterval(TRUE);
			pNewBuf = pOldBuf;
			RemoveBuffer(pBuf->GetID(), FALSE);
			break;
		}
	default:
		{
			RemoveBuffer(pBuf->GetID(),FALSE);
			LOG_PROCESS_ERROR(!"判断叠加类型的逻辑结果错误");
			break;
		}
	}

EXIT0:
	return pNewBuf;
}

//判断叠加模式：先判断叠加类型，再判断Level（高覆盖低）
//相同等级再判断叠加细类，每次叠加Stack层数加1（不大于最大叠加层数）
BufferStackMode BufferManager::CheckBuffStack(KCharacter* pCharacter, Buffer* pBuf,CharacterBuffer* pCharacterBuf)
{
	//保证能找到的buff只有一个,如果没有就加上，下次再叠加的时候，就保证只有一个
	Buffer* pObjBuf = pCharacterBuf->GetBufferByStackCategory(pBuf->GetStackCategory());
	if (pObjBuf)
	{
		if (pObjBuf->GetBuffLevel() > pBuf->GetBuffLevel())
		{
			return emForbidStack;
		}
		else if (pObjBuf->GetBuffLevel() < pBuf->GetBuffLevel())
		{
			return emOverWrite;
		}
		else
		{
			switch ((BufferStackDetail)pBuf->GetBuffCategory(2))
			{
			case emUpdate:
			case emReplace:
				return emUpdateInterval;
			case emStackTime:
				return emStackInterval;
			case emForbid:
			default:
				return emForbidStack;
			}
		}
	}
	else
	{
		return emNoneStackBuff;	//没有可叠加的buff
	}
}

INT BufferManager::AddBufferToCharacter(DWORD dwCasterId, 
										INT nSkillId, 
										DWORD dwCharacterId,
										INT nBufferTemplateId,
										INT nBuffPersist /* = 0 */,
										BOOL bSync /* = TRUE */, 
										INT nDestX /* = 0 */, 
										INT nDestY /* = 0 */, 
										INT nDestZ /* = 0 */,
										INT nSkillLevel/* = 1 */)
{
	INT nRetCode = 0;
	INT nIndex = 0;
	KCharacter* pCaster = NULL;
	CharacterBuffer* characterBuffer = NULL;
	Buffer* buf = NULL;
	KCharacter* pCharacter = NULL;
	BufferStackMode emMode = emNoneStackBuff;
	Buffer* pObjBuf = NULL;

	const BufferTemplate* pTemplate = g_cBufferSetting.GetBufferTemplate(nBufferTemplateId);
	if(!pTemplate)
	{
		//printf("Can't find bufferId:%d, skillId:%d\n", nBufferTemplateId, nSkillId);
	}
	LOG_PROCESS_ERROR(pTemplate);

	// 给施法者自身加buff
	if (pTemplate->m_BuffPosition == BUFFER_POSITION_SELF)
	{
		dwCharacterId = dwCasterId;
	}

    pCharacter = g_cCharacterMgr.GetById(dwCharacterId);
	if (!(pCharacter))       
	{                       
		//QLogPrintf(LOG_DEBUG, "添加buffer(%d)时，绑定角色已不存在 at line %d in %s\n", nBufferTemplateId, __LINE__, __FUNCTION__);                  
		goto EXIT0;    
	}
	PROCESS_SUCCESS(pCharacter->m_bSystemShield);

	pCaster = g_cCharacterMgr.GetById(dwCasterId);
	if (!(pCaster))       
	{                       
		//QLogPrintf(LOG_DEBUG, "添加buffer(%d)时，发射角色已不存在 at line %d in %s\n", nBufferTemplateId, __LINE__, __FUNCTION__);                  
		goto EXIT0;    
	}
	PROCESS_SUCCESS(pCaster->m_bSystemShield);

	// 检测是否免疫固定类型的Debuff
	if(pCharacter->IsIgnoreDebuff(pTemplate->m_nBuffType))
	{
		nIndex = BUFFER_POOL_MAX_COUNT + 1;
		PROCESS_SUCCESS( "角色免疫Debuff" );
	}

	// 检测是否为范围Buff
	if ( pTemplate->m_fBuffRadius > 0)
	{
		INT nX, nY, nZ;
		INT nIdx = GetGroundBufferByCate(pTemplate->m_nStackCategory, dwCasterId);
		if(nIdx >= 0)
		{
			RemoveGroundBufferByIndex(nIdx);
		}
		pCaster->GetPosition(nX, nY, nZ);
		nIndex = AddGroundBuffer(nBufferTemplateId, dwCasterId, nSkillId, pCaster->GetSceneId(),
			nX, nY, nZ, bSync, nBuffPersist, nSkillLevel);

		GroundBuffer* pBuff = GetGroundBufferByIndex(nIndex);
		PROCESS_ERROR( pBuff );

		if (!pTemplate->m_bIsGroundBuffer)
		{
			pBuff->BindToCharacter(pCharacter);
		}

		PROCESS_SUCCESS( "添加范围Buff给玩家" );
	}

	//先暂时没有释放者，释放者由skill传入，如果前置条件未通过，就不会给目标加buff
	PROCESS_ERROR(g_cBufferSetting.CheckPreCondition(nBufferTemplateId, pCaster, pCharacter));

	characterBuffer = pCharacter->GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(characterBuffer);
	
	//免疫此buff
	PROCESS_SUCCESS( characterBuffer->CheckIgnore(pTemplate) );

	nIndex = AddBuffer(nBufferTemplateId, nBuffPersist);
	buf = GetBufferByIndex(nIndex);
	LOG_PROCESS_ERROR(buf);

	//保证能找到的buff只有一个,如果没有就加上，下次再叠加的时候，就保证只有一个
	pObjBuf = characterBuffer->GetBufferByStackCategory(buf->GetStackCategory());
	if (pObjBuf)
	{
		if(buf->GetMaxStackCount() > 1)
		{
			INT nCategory = buf->GetStackCategory();
			RemoveBuffer(buf->GetID(), FALSE);
			buf = characterBuffer->StackBuffer(nCategory);
		}
		else
		{
			buf = ReplaceBuffer(pCharacter, buf, pObjBuf);
			LOG_PROCESS_ERROR(buf);
			nIndex = GetIndexById(buf->GetID());
		}
	}
	else
	{
		characterBuffer->AddBuffer(buf, bSync, pCaster->GetId());
	}

	buf->BindSender(pCaster);	//绑定释放者
	buf->BindCharacter(pCharacter);	//将角色绑定到对应的buff上
	buf->BindFightSkill(nSkillId);
	buf->SetSkillLevel(nSkillLevel);

	buf->SetDestPosition(nDestX, nDestY, nDestZ);
	pCharacter->FireEvent(emKOBJEVENTTYPE_BUFF_BUFF_IN, pCharacter->GetId(), buf->GetTemplateID(), pCaster->GetId());

	{// 事件触发
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

		cSafeScript->CallTableFunction("BufferCondition", "BuffOut", 0, "ddddddd",
			pCaster->GetId(), pCharacter->GetId(), buf->GetTemplateID(), buf->GetCurrentStackCount(),
			buf->GetBuffCategory(1), buf->GetBuffCategory(2), buf->GetBuffCategory(3));
	}

EXIT1:
	return nIndex;
EXIT0:
	return 0;
}

BOOL BufferManager::RemoveBufferFromCharacter(DWORD dwCharacterID, DWORD nBufferIndex, BOOL bSync)
{
	CharacterBuffer* characterBuffer = NULL;
	Buffer* buf = NULL;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(dwCharacterID);
	INT nBuffTemplateId = 0;
	LOG_PROCESS_ERROR(pCharacter);

	buf = GetBufferByIndex(nBufferIndex);
	LOG_PROCESS_ERROR(buf);

	characterBuffer = pCharacter->GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(characterBuffer);

	characterBuffer->SetCurrentProcessingBuffer(buf);
	LOG_PROCESS_ERROR(characterBuffer->RemoveBuffer(buf, bSync));

	nBuffTemplateId = buf->GetTemplateID();

	LOG_PROCESS_ERROR(RemoveBuffer(buf->GetID(), TRUE));

	// 删除buff事件
	pCharacter->FireEvent(emKOBJEVENTTYPE_BUFF_BUFF_OUT, pCharacter->GetId(), nBuffTemplateId, 0);

	return TRUE;
EXIT0:
	return FALSE;
}


GroundBuffer* BufferManager::GetGroundBufferById(DWORD nID)
{
	return m_cGroundBufferPool.GetById(nID);
}


GroundBuffer* BufferManager::GetGroundBufferByIndex(INT nIndex)
{
	return m_cGroundBufferPool.GetByIndex(nIndex);
}

INT BufferManager::FindSamePassiveGroundBuffer(INT nTemplateId, DWORD dwCharacterId)
{
	for (INT nIdx = 0; nIdx <= m_cGroundBufferPool.GetMaxCount(); ++nIdx)
	{
		GroundBuffer* pBuf = m_cGroundBufferPool.GetByIndex(nIdx);
		if(pBuf && pBuf->IsActive() && pBuf->GetTemplateId() == nTemplateId && pBuf->GetBuffPersist() < 0)
		{
			KCharacter* pCaster = pBuf->GetBufCaster();
			if(pCaster && pCaster->GetId() == dwCharacterId)
			{
				return nIdx;
			}
		}
	}
	return -1;
}

INT BufferManager::GetGroundBufferByCate(INT nCate, DWORD dwCharacterId)
{
	for (INT nIdx = 0; nIdx <= m_cGroundBufferPool.GetMaxCount(); ++nIdx)
	{
		GroundBuffer* pBuf = m_cGroundBufferPool.GetByIndex(nIdx);
		if(pBuf && pBuf->IsActive() && pBuf->GetStackCategory() == nCate)
		{
			KCharacter* pCaster = pBuf->GetBufCaster();
			if(pCaster && pCaster->GetId() == dwCharacterId)
			{
				return nIdx;
			}
		}
	}
	return -1;
}

GroundBuffer* BufferManager::GetGroundBufferByTemplateId(INT nBuffTemplateId, DWORD dwCharacterId)
{
	for (INT nIdx = 0; nIdx <= m_cGroundBufferPool.GetMaxCount(); ++nIdx)
	{
		GroundBuffer* pBuf = m_cGroundBufferPool.GetByIndex(nIdx);
		if(pBuf && pBuf->IsActive() && pBuf->GetTemplateId() == nBuffTemplateId)
		{
			KCharacter* pCaster = pBuf->GetBufCaster();
			if(pCaster && pCaster->GetId() == dwCharacterId)
			{
				return pBuf;
			}
		}
	}
	return NULL;
}

INT BufferManager::AddGroundBuffer(INT nTemplateId, DWORD dwCharacterId, INT nSkillId, INT dwSceneId,INT nX,INT nY,INT nZ, BOOL bSync /* = TRUE */, INT nBuffPersist, INT nSkillLevel)
{
	BOOL bOk = FALSE;
	INT nIndex = 0;
	KCharacter* pCaster = NULL;
	GroundBuffer* pBuf = NULL;

	nIndex = FindSamePassiveGroundBuffer(nTemplateId, dwCharacterId);
	if(nIndex >= 0)
		goto EXIT0;

	nIndex = m_cGroundBufferPool.Add();
	pBuf = m_cGroundBufferPool.GetByIndex(nIndex);
	LOG_PROCESS_ERROR(pBuf);
	bOk = pBuf->Init(nTemplateId,dwSceneId,nX,nY,nZ);
	LOG_PROCESS_ERROR(bOk && "Ground Buffer Init Failed!");
	pBuf->SetActive(TRUE);	//将其激活
	pCaster = g_cCharacterMgr.GetById(dwCharacterId);
	if (pCaster)
	{
		pBuf->BindCaster(pCaster);
	}
	pBuf->BindFightSkill(nSkillId);

	if (nBuffPersist > 0)
	{
        pBuf->SetBuffPersist(nBuffPersist);
	}
	pBuf->SetSkillLevel(nSkillLevel);

	//同步到客户端
	if (bSync)
	{
	    LOG_PROCESS_ERROR(pBuf->BroadCastGroundBuffer());	
	}
EXIT0:
	return nIndex;
}

//TODO: 现在广播caster附近的人，应该改成ground buffer附近的人
BOOL BufferManager::RemoveGroundBuffer(DWORD dwId, BOOL bSync)
{
	GroundBuffer* pBuf = m_cGroundBufferPool.GetById(dwId);
	QCONFIRM_RET_FALSE(pBuf);
	
	if (bSync)
	{
		pBuf->BroadcastRemoveGroundBuffer();
	}

	m_pCurrentProcessingGroundBuff = pBuf;
	pBuf->UnInit();
	m_pCurrentProcessingGroundBuff = NULL;

	return m_cGroundBufferPool.Remove(dwId);
}

BOOL BufferManager::RemoveGroundBufferByIndex(INT nIndex)
{
	GroundBuffer* pBuf = m_cGroundBufferPool.GetByIndex(nIndex);
	QCONFIRM_RET_FALSE(pBuf);

	m_pCurrentProcessingGroundBuff = pBuf;
	pBuf->UnInit();
	m_pCurrentProcessingGroundBuff = NULL;

	return m_cGroundBufferPool.RemoveByIndex(nIndex);
}