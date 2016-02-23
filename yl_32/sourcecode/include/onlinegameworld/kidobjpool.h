/* -------------------------------------------------------------------------
//	文件名		：	kidobjpool.h
//	创建者		：	simon
//	创建时间	：	2009/11/21 15:57:01
//	功能描述	：	Id对象池
//
// -----------------------------------------------------------------------*/
#ifndef __KIDOBJPOOL_H__
#define __KIDOBJPOOL_H__

#include <map>
#include "Misc/qidpool.h"
#include "onlinegameworld/kbaseobject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// -------------------------------------------------------------------------

// T必须从KBaseObject继承或有m_dwId public成员
template<class T, DWORD dwIdMask = 0>
class TKIdObjPool
{
public:
	TKIdObjPool(void);
	virtual ~TKIdObjPool(void);

public:
	virtual BOOL	Init(INT nMaxCount);
	virtual BOOL	Uninit();

	// 添加对象，失败返回0
	virtual INT		Add(DWORD dwId = KD_BAD_ID);
	virtual BOOL	Remove(DWORD dwId);
	virtual BOOL	RemoveByIndex(INT nIndex);

	// 获取对象指针，失败返回NULL
	virtual T* GetByIndex(INT nIndex);
	virtual T* GetById(DWORD dwId);

	// 通过Id获取对象的Index(无效ID的Index为0)
	INT GetIndexById(DWORD dwId);

	// 获取允许的对象上限
	INT GetMaxCount()
	{ return m_cIdPool.GetMaxNum(); }

	// 获取当前使用数量
	INT GetUsedCount()
	{ return m_cIdPool.GetUsedCount(); }

	INT NextUsedIndex(INT nLastIdx)
	{ return m_cIdPool.GetNextUsedId(nLastIdx); }

	VOID	SetIdMask(DWORD dwIM)
	{ m_dwIdMask = dwIM; }

protected:
	T*						m_pObjects;
	QIdPool					m_cIdPool;			// 对象index生成器
	std::map<DWORD, INT>	m_mapId2Index;		// ID到Index映射
	DWORD					m_dwIdMask;
	DWORD					m_dwIdGenerator;	// ID生成
private:
};

template<class T, DWORD dwIdMask>
TKIdObjPool<T, dwIdMask>::TKIdObjPool()
{
	m_pObjects = NULL;
	m_dwIdMask = dwIdMask;
	m_dwIdGenerator = 0;
}

template<class T, DWORD dwIdMask>
TKIdObjPool<T, dwIdMask>::~TKIdObjPool(void)
{
}

template<class T, DWORD dwIdMask>
T* TKIdObjPool<T, dwIdMask>::GetByIndex( INT nIndex )
{
	if (nIndex <= 0 || nIndex > m_cIdPool.GetMaxNum())
		return NULL;

	return &m_pObjects[nIndex];
}

template<class T, DWORD dwIdMask>
T* TKIdObjPool<T, dwIdMask>::GetById( DWORD dwId )
{
	std::map<DWORD, INT>::const_iterator it = m_mapId2Index.find(dwId);
	if (it == m_mapId2Index.end())
		return NULL;

	INT nIndex = it->second;
	QCONFIRM_RET_NULL(nIndex > 0 && nIndex <= m_cIdPool.GetMaxNum());

	return &m_pObjects[nIndex];
}

template<class T, DWORD dwIdMask>
INT TKIdObjPool<T, dwIdMask>::GetIndexById(DWORD dwId)
{
	std::map<DWORD, INT>::const_iterator it = m_mapId2Index.find(dwId);
	if (it == m_mapId2Index.end())
	{
		return 0;
	}
	else
	{
		return m_mapId2Index[dwId];
	}
}

template<class T, DWORD dwIdMask>
BOOL TKIdObjPool<T, dwIdMask>::Remove( DWORD dwId )
{
	std::map<DWORD, INT>::const_iterator it = m_mapId2Index.find(dwId);
	if (it != m_mapId2Index.end() && it->second > 0 && it->second <= m_cIdPool.GetMaxNum())
	{
		m_cIdPool.Delete(it->second);
	}
	m_mapId2Index.erase(dwId);
	return TRUE;
}


template<class T, DWORD dwIdMask>
BOOL TKIdObjPool<T, dwIdMask>::RemoveByIndex( INT nIndex )
{
	// 越界
	if (nIndex <= 0 || nIndex > m_cIdPool.GetMaxNum())
		return FALSE;
	//KBaseObject* pObject = static_cast<KBaseObject*>(&m_pObjects[nIndex]);
	T* pObject = &m_pObjects[nIndex];
	m_mapId2Index.erase(pObject->m_dwId);
	return m_cIdPool.Delete(nIndex);
}

template<class T, DWORD dwIdMask>
INT TKIdObjPool<T, dwIdMask>::Add(DWORD dwId /* = KD_BAD_ID */)
{
	// 满了
	if (m_cIdPool.GetUsedCount() >= m_cIdPool.GetMaxNum())
	{
		Q_Error("Too many Objects!");
		return 0;
	}

	INT nIndex = m_cIdPool.New();
	if (nIndex <= 0)
	{
		Q_Error("Can't allocate index from IdPool!");
		return 0;
	}

	T* pObj = &m_pObjects[nIndex];

	if (pObj)
	{
		if (KD_BAD_ID == dwId)
		{
			dwId = m_dwIdGenerator++;
			dwId |= m_dwIdMask;
		}

		pObj->m_dwId = dwId;
		//static_cast<KBaseObject*>(pObj)->m_dwId = dwId;

		// 保存ID->Index映射
		m_mapId2Index[dwId] = nIndex;
	}

	return nIndex;
}

template<class T, DWORD dwIdMask>
BOOL TKIdObjPool<T, dwIdMask>::Init( INT nMaxCount )
{
	// 初始化对象数组
	m_pObjects = new T[nMaxCount + 1];
	QCONFIRM_RET_FALSE(m_pObjects);

	// 初始化最大对象数
	m_cIdPool.Init(nMaxCount);

	m_dwIdGenerator = 1;

	return TRUE;
}

template<class T, DWORD dwIdMask>
BOOL TKIdObjPool<T, dwIdMask>::Uninit()
{
	SAFE_DELETE_ARRAY(m_pObjects);
	m_cIdPool.Uninit();
	m_mapId2Index.clear();

	return TRUE;
}

// -------------------------------------------------------------------------

#endif /* __KIDOBJPOOL_H__ */
