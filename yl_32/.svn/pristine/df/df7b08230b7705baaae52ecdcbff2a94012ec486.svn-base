/* -------------------------------------------------------------------------
//	文件名		：	krelation.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	无附带数据的玩家关系（临时好友、黑名单等）
//
// -----------------------------------------------------------------------*/
#ifndef __KRELATION_H__
#define __KRELATION_H__

#include "playerrelation_i.h"

class KRelation : public IKRelation
{
public:
	KRelation(KEPLAYERRELATION_TYPE nType, IKRelationFactory *pFactory);
	virtual ~KRelation();
	BOOL Init(INT nMaster, INT nTarget);
public:
	virtual BOOL QueryRelation(INT nType, LPVOID *ppv);		// 查询子接口
	virtual INT	GetType();									// 查询关系类型
	virtual BOOL SaveInfo(CHAR *pBuf, INT nLen);			// 保存附加数据到指定缓冲区（用于同步好友信息）
	virtual BOOL LoadInfo(CHAR *pBuf, INT nLen);			// 加载数据
	virtual INT GetRole(BOOL bAsMaster);			// 获取主位/次位玩家（bAsMaster为true表示主位，false表示次位）
	virtual INT GetInfLen();								// 获取附加信息长度
	// 获取附加数据
	virtual LPVOID GetInfo(INT &nLen)
	{
		nLen = 0;
		return NULL;
	}
	virtual INT GetOtherPlayerId(INT nSelfId)
	{
		INT nPlayerId	= m_nMaster;
		if (nSelfId == m_nMaster)
			nPlayerId = m_nTarget;
		return nPlayerId;
	}
private:
	INT					m_nMaster;
	INT					m_nTarget;
	KEPLAYERRELATION_TYPE	m_nType;
	IKRelationFactory*		m_pFactory;
};
#endif //__KRELATION_H__
