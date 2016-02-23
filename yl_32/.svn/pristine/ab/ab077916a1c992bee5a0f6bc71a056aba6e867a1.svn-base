/* -------------------------------------------------------------------------
//	文件名		：	kdatarelation_t.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	附带数据（如亲密度）的玩家关系
//
// -----------------------------------------------------------------------*/
#ifndef	__KDATARELATION_T_H__
#define __KDATARELATION_T_H__

#include "playerrelation_i.h"
#include "playerrelation_datadef.h"

template <typename KData_T>
class KDataRelation_T : public IKRelation
{
public:
	static KData_T GetDefaultValue()
	{
		return KData_T();
	}
	static KData_T Load(LPVOID pData, INT nLen)
	{
		ASSERT(nLen == KSIZE_T<KData_T>::size);
		return *(KData_T*)pData;
	}
	KDataRelation_T(INT nType, IKRelationFactory *pFactory) : m_pFactory(pFactory), m_nType(nType), m_pData(NULL)
	{
		ASSERT(KSIZE_T<KData_T>::size == 0 || KSIZE_T<KData_T>::size == sizeof(KData_T));
		ASSERT(pFactory);
		m_nMaster = m_nTarget = 0;
	}
	virtual ~KDataRelation_T()
	{
	}
	VOID Init(INT nMaster, INT nTarget, KData_T *pData)
	{
		m_nMaster = nMaster;
		m_nTarget = nTarget;
		m_pData = pData;
	}
public:
	// 查询子接口
	virtual BOOL QueryRelation(INT nType, LPVOID *ppv)
	{
		if (nType != m_nType)
			return FALSE;
		*ppv = this;
		return TRUE;
	}
	// 查询关系类型
	virtual INT GetType()
	{
		return m_nType;
	}
	// 获取附加数据
	virtual LPVOID GetInfo(INT &nLen)
	{
		nLen = KSIZE_T<KData_T>::size;
		if (KSIZE_T<KData_T>::size == 0)
			return NULL;
		else
			return m_pData;
	}
	// 获取附加信息长度
	virtual INT GetInfLen()
	{
		return KSIZE_T<KData_T>::size;
	}
	// 保存附加数据到指定缓冲区（用于同步好友信息）
	virtual BOOL SaveInfo(CHAR *pBuf, INT nLen)
	{
		// KGC_CONFIRM_RET_FALSE(pBuf && nLen > 0);
		if (KSIZE_T<KData_T>::size == 0 || nLen != KSIZE_T<KData_T>::size)
		{
			// KGC_Error("");
			return FALSE;
		}
		memcpy(pBuf, m_pData, KSIZE_T<KData_T>::size);
		return TRUE;
	}
	virtual BOOL LoadInfo(CHAR *pBuf, INT nLen)
	{
		if (KSIZE_T<KData_T>::size == 0 || nLen != KSIZE_T<KData_T>::size)
		{
			// KGC_Error("");
			return FALSE;
		}
		memcpy(m_pData, pBuf, KSIZE_T<KData_T>::size);
		return TRUE;
	}
	// 获取主位/次位玩家（bAsMaster为TRUE表示主位，FALSE表示次位）
	virtual INT GetRole(BOOL bAsMaster)
	{
		return bAsMaster ? m_nMaster : m_nTarget;
	}
	virtual INT GetOtherPlayerId(INT nSelfId)
	{
		INT nPlayerId = m_nMaster;
		if (nSelfId == m_nMaster)
			nPlayerId = m_nTarget;
		return nPlayerId;
	}
private:
	KData_T*			m_pData;
	IKRelationFactory*	m_pFactory;
	INT					m_nMaster;
	INT					m_nTarget;
	INT					m_nType;
};

template <class KBaseT = IKRelationFactory>
class KRelationFactoryBaseT : public KBaseT
	{
public:
	KRelationFactoryBaseT() : m_pHelper(NULL), m_pSender(NULL)
	{}
	virtual ~KRelationFactoryBaseT()
	{}
	BOOL Init(IKRelationHelper *pHelper)
	{
		assert(pHelper);
		// assert(pSender);

		m_pHelper = pHelper;
		// m_pSender = pSender;
		return TRUE;
	}
protected:
	IKRelationHelper*	m_pHelper;
	IKRelationSender*	m_pSender;
};
#endif //__KDATARELATION_T_H__
