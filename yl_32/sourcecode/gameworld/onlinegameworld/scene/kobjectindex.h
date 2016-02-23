////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KObjectIndex.h
//  Version     : 1.0
//  Creator     : zhaochunfeng, simon
//  Comment     : ID生成,以及ID-->Object的映射
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _KOBJECTINDEX_H_
#define _KOBJECTINDEX_H_

#include <utility>

template <class T>
class KObjectIndex
{
public:
    KObjectIndex()
    {
		m_dwLastId = 1;
		m_itCurrent = m_ObjectIndex.begin();
		m_bAlreadyNext = FALSE;
    }

    void SetPrefix(DWORD dwPrefix)
    { 
        m_dwLastId = (DWORD)(dwPrefix << (sizeof(m_dwLastId) * CHAR_BIT - ID_PREFIX_BIT_NUM)) + 1;
    }

    BOOL Register(T* pObject, DWORD dwId)
    {
        std::pair<typename KOBJECT_INDEX::iterator, bool> InsRet;

        QCONFIRM_RET_FALSE(pObject);
        QCONFIRM_RET_FALSE(pObject->m_dwId == KD_BAD_ID);
        QCONFIRM_RET_FALSE(m_dwLastId != KD_BAD_ID);

        if (dwId == KD_BAD_ID)
        {
            dwId = m_dwLastId;
        }

		InsRet = m_ObjectIndex.insert(std::make_pair(dwId, pObject));
        if (InsRet.second)
        {
            pObject->m_dwId = dwId;
            m_dwLastId++;
        }

        return InsRet.second;
    }

    void Unregister(T* pObject)
    {
		ASSERT(pObject);
		if (m_itCurrent != m_ObjectIndex.end() && m_itCurrent->first == pObject->m_dwId)
		{
			++m_itCurrent;
			m_bAlreadyNext = TRUE;
		}
		m_ObjectIndex.erase(pObject->m_dwId);

        pObject->m_dwId = KD_BAD_ID;
    }

    T* GetObj(DWORD dwId)
    {
        typename KOBJECT_INDEX::iterator it = m_ObjectIndex.find(dwId);
        if (it != m_ObjectIndex.end())
        {
            return it->second;
        }
        return NULL;
    }

	T* FindFirst()
	{
		T* pElement = NULL;
		m_itCurrent = m_ObjectIndex.begin();
		m_bAlreadyNext = FALSE;

		if (m_itCurrent != m_ObjectIndex.end())
		{
			pElement = m_itCurrent->second;
		}
		return pElement;
	}

	T* FindNext()
	{
		T* pElement = NULL;

		if (m_itCurrent == m_ObjectIndex.end())
			return pElement;

		if ( ! m_bAlreadyNext)
			++m_itCurrent;
		m_bAlreadyNext = FALSE;

		if (m_itCurrent != m_ObjectIndex.end())
		{
			pElement = m_itCurrent->second;
		}
		return pElement;
	}

    size_t GetObjCount()
    {
        return m_ObjectIndex.size();
    }

	template <class TFunc>
	void Traverse(TFunc& Func)
    {
        BOOL bContinue = false;
        typename KOBJECT_INDEX::iterator it = m_ObjectIndex.begin();
        while (it != m_ObjectIndex.end())
        {
            typename KOBJECT_INDEX::iterator Current = it++;

            bContinue = Func(Current->first, Current->second);
            if (!bContinue)
                break;
        }
    }

private:
    typedef std::map<DWORD, T*> KOBJECT_INDEX;
    KOBJECT_INDEX   m_ObjectIndex;
    DWORD           m_dwLastId;
	typename KOBJECT_INDEX::iterator	m_itCurrent;
	BOOL			m_bAlreadyNext; // 删除m_itCurrent后的特殊处理
};

#endif
