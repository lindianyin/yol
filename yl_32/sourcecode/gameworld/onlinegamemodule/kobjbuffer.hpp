/* -------------------------------------------------------------------------
//	文件名		：	kobjbuffer.hpp
//	创建者		：	liuchang
//	创建时间	：	2009-10-20 17:33:13
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KOBJBUFFER_HPP__
#define __KOBJBUFFER_HPP__

// -------------------------------------------------------------------------

#include "kmagicadjust.h"
#include "kmagicapply.h"
#include "kobjbuffercheck.h"
#include "kbuffermanager.h"
#include "kbuffer.h"

// -------------------------------------------------------------------------
// KObjBuffer
template<typename TObj>
TKObjBufferMap<TObj>::TKObjBufferMap(TObj& rcTObj)
:m_rcHost(rcTObj)
{}

// -------------------------------------------------------------------------
template<typename TObj>
TKObjBufferMap<TObj>::~TKObjBufferMap()
{
	Release();
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL TKObjBufferMap<TObj>::Release()
{
	for (KOBJSKILLMAP::iterator it = m_mapBuffer.begin(); it != m_mapBuffer.end(); ++it)
	{
		it->second->Release();
		SAFE_DELETE(it->second);
	}

	m_mapBuffer.clear();

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL TKObjBufferMap<TObj>::Active()
{
	for (KOBJSKILLMAP::iterator it = m_mapBuffer.begin(); it != m_mapBuffer.end(); ++it)
	{
		it->second->Active();
	}

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL TKObjBufferMap<TObj>::RoundElapsing()
{
	for (KOBJSKILLMAP::iterator it = m_mapBuffer.begin(); it != m_mapBuffer.end();)
	{
		KObjBuffer<TObj>* pObjBuffer = it->second;
		if (!pObjBuffer)
		{
			_ASSERT(FALSE);
			++it;
			continue;
		}

		pObjBuffer->RoundElapsing();
		if (pObjBuffer->GetTime())
		{
			++it;
			continue;
		}
		
		pObjBuffer->Release();
		SAFE_DELETE(pObjBuffer);
		m_mapBuffer.erase(it++);
	}

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj> template<typename TBufferOrigin>
BOOL TKObjBufferMap<TObj>::AddBuffer(INT nBufferId, CONST TBufferOrigin& cOriginInfo)
{
	BOOL bResult = FALSE;

	CONST KBuffer* pBuffer = NULL;
	KObjBuffer<TObj>* pObjBuffer = NULL;

	pBuffer = KBufferManager::GetInstance().GetBuffer(nBufferId);
	KGLOG_PROCESS_ERROR(pBuffer);

	KGLOG_PROCESS_ERROR(TKBufferConditionCheck<TObj>::Check(m_rcHost, *pBuffer));
	RemoveBuffer(pBuffer->GetExclusiveType(), pBuffer->GetExclusivePriority()); // 移除同类型的Buffer

	pObjBuffer = new KObjBuffer<TObj>(m_rcHost, *pBuffer, cOriginInfo);
	KGLOG_PROCESS_ERROR(pObjBuffer->Init());

	KGLOG_PROCESS_ERROR(m_mapBuffer.insert(KOBJSKILLMAP::value_type(nBufferId,pObjBuffer)).second); // 插入进Buffer列表

	bResult = TRUE;

Exit0:
	if (!bResult && pObjBuffer)
		SAFE_DELETE(pObjBuffer);

	return bResult;
}


// -------------------------------------------------------------------------
template<typename TObj>
BOOL TKObjBufferMap<TObj>::RemoveBuffer(INT nBufferId)
{
	KOBJSKILLMAP::iterator it = m_mapBuffer.find(nBufferId);
	if (it == m_mapBuffer.end())
	{
		return FALSE;
	}

	KObjBuffer<TObj>* pObjBuffer = it->second;
	if (!pObjBuffer)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	pObjBuffer->Release();
	SAFE_DELETE(pObjBuffer);
	m_mapBuffer.erase(it);

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL TKObjBufferMap<TObj>::RemoveBuffer(KE_BUFFER_EXCLUSIVETYPE eExclusiveType, INT nExclusivePriority)
{
	// TODO:liuchang 效率待优化，RemoveBuffer还会find一次
	for (KOBJSKILLMAP::iterator it = m_mapBuffer.begin(); it != m_mapBuffer.end(); ++it)
	{
		KObjBuffer<TObj>* pObjBuffer = it->second;
		if (!pObjBuffer)
		{
			_ASSERT(FALSE);
			continue;
		}
		CONST KBuffer& rcBuffer = pObjBuffer->GetBufferData();
		if (rcBuffer.GetExclusivePriority() == eExclusiveType)
		{
			if (nExclusivePriority > rcBuffer.GetExclusivePriority())
			{
				RemoveBuffer(rcBuffer.GetId());
				return TRUE;
			}
			
			break;
		}
	}

	return FALSE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL TKObjBufferMap<TObj>::RemoveBuffer(KE_BUFFER_LOGICTYPE eLogicType)
{
	for (KOBJSKILLMAP::iterator it = m_mapBuffer.begin(); it != m_mapBuffer.end();)
	{
		KObjBuffer<TObj>* pObjBuffer = it->second;
		if (!pObjBuffer)
		{
			_ASSERT(FALSE);
			continue;
		}
		CONST KBuffer& rcBuffer = pObjBuffer->GetBufferData();
		if (rcBuffer.GetLogicType() & eLogicType)
		{
			RemoveBuffer(rcBuffer.GetId());
			m_mapBuffer.erase(it++);
			continue;
		}

		++it;
	}

	return TRUE;
}

// -------------------------------------------------------------------------
// KObjBuffer
template<typename TObj> template<typename TBufferOrigin>
KObjBuffer<TObj>::KObjBuffer(TObj& rcHost, CONST KBuffer& rcBufferData, CONST TBufferOrigin& rcBufferOrigin)
:m_rcHost(rcHost), m_rcBufferData(rcBufferData),
m_rcBufferOrigin(*(new TBufferOrigin(rcBufferOrigin)))
{

}

// -------------------------------------------------------------------------
template<typename TObj>
KObjBuffer<TObj>::~KObjBuffer()
{
	delete &m_rcBufferOrigin;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::Init()
{
	m_mapITMagic.clear();
	m_mapOTMagic.clear();
	m_mapDOOMMagic.clear();

	// 初始化Buffer数据
	m_nAccumulateCount = 1;
	m_nTime = m_rcBufferData.GetTotalTime();
	m_nTriggerCount = 0;

	m_mapITMagic = m_rcBufferData.GetITMagic();
	m_mapOTMagic = m_rcBufferData.GetOTMagic();
	m_mapDOOMMagic = m_rcBufferData.GetDoomMagic();

	__AdjustMagic();
	__ITTrigger();

	// NOTE:liuchang 
	// Buffer目前只会监听 *宿主(m_rcHost)* 的事件，若要监听其它Obj
	// 的事件也是其它事件触发所有者转发的事件，比如天气变化事件，是由
	// Weather转发给战场中的Obj，然后Obj再FireEvent使得Buffer触发。

	CONST EVENTLIST& rcEventList =  m_rcBufferData.GetTriggerEventList();
	for (EVENTLIST::const_iterator it = rcEventList.begin(); it != rcEventList.end(); ++it)
	{
		Watch(&m_rcHost, it->nEvent);
	}

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::Release()
{
	__ITUnTrigger();
	__DOOMTrigger();

	KObjEventWatch::Release();

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::Active()
{
	// TODO:liuchang 判断Buffer时间是否到了，如果到了则删除
	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::RoundElapsing()
{
	if (m_rcBufferData.GetTimeType() != emKBUFFER_TIMETYPE_ROUND)
		return TRUE;

	if (m_nTime > 0)
		--m_nTime;

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::OnEvent(KEventCreator& rcSender, CONST KOBJEVENT& rcEvent)
{	
	TObj* pSender = dynamic_cast<TObj*>(&rcSender);
	if (!pSender)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	if (!TKBufferEventCheck<TObj>::Check(*pSender, rcEvent, m_rcBufferData))
		return FALSE;

	return __OTTrigger();
}

// -------------------------------------------------------------------------
template<typename TObj>
CONST KBuffer& KObjBuffer<TObj>::GetBufferData()
{
	return m_rcBufferData;
}

// -------------------------------------------------------------------------
template<typename TObj>
INT KObjBuffer<TObj>::GetTime()
{
	return m_nTime;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::__AdjustMagic()
{
	for (MAGICMAP::iterator it = m_mapITMagic.begin(); it != m_mapITMagic.end(); ++it)
	{
		KMagicAdjust<TObj>::Adjust(m_rcHost, it->second, m_rcBufferOrigin);
	}

	for (MAGICMAP::iterator it = m_mapOTMagic.begin(); it != m_mapOTMagic.end(); ++it)
	{
		KMagicAdjust<TObj>::Adjust(m_rcHost, it->second, m_rcBufferOrigin);
	}

	for (MAGICMAP::iterator it = m_mapDOOMMagic.begin(); it != m_mapDOOMMagic.end(); ++it)
	{
		KMagicAdjust<TObj>::Adjust(m_rcHost, it->second, m_rcBufferOrigin);
	}

	return TRUE;
}


// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::__OTTrigger()
{
	for (MAGICMAP::iterator it = m_mapOTMagic.begin(); it != m_mapOTMagic.end(); ++it)
	{
		KMagicApply<TObj>::Apply(m_rcHost, it->second, m_rcBufferOrigin);
	}

	--m_nTriggerCount;

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::__ITTrigger()
{
	for (MAGICMAP::const_iterator it = m_mapITMagic.begin(); it != m_mapITMagic.end(); ++it)
	{
		KMagicApply<TObj>::Apply(m_rcHost, it->second, m_rcBufferOrigin);
	}

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::__ITUnTrigger()
{
	for (MAGICMAP::iterator it = m_mapITMagic.begin(); it != m_mapITMagic.end(); ++it)
	{
		KMagicApply<TObj>::Apply(m_rcHost, it->second, m_rcBufferOrigin, TRUE);
	}

	return TRUE;
}

// -------------------------------------------------------------------------
template<typename TObj>
BOOL KObjBuffer<TObj>::__DOOMTrigger()
{
	for (MAGICMAP::iterator it = m_mapDOOMMagic.begin(); it != m_mapDOOMMagic.end(); ++it)
	{
		KMagicApply<TObj>::Apply(m_rcHost, it->second, m_rcBufferOrigin);
	}

	return TRUE;
}

// -------------------------------------------------------------------------

#endif /* __KOBJBUFFER_HPP__ */
