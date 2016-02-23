/* -------------------------------------------------------------------------
//	文件名		：	relation.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	无附带数据的玩家关系（临时好友、黑名单等）
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "relation.h"
#include "unirelationfactory.h"

KRelation::KRelation(KEPLAYERRELATION_TYPE nType, IKRelationFactory *pFactory)
	: m_nType(nType), m_pFactory(pFactory)
{
	ASSERT(pFactory && nType >= 0 && nType < emKPLAYERRELATION_TYPE_COUNT);
	m_nMaster = m_nTarget = 0;
}

KRelation::~KRelation()
{
}

BOOL KRelation::Init(INT nMaster, INT nTarget)
{
	// KGC_CONFIRM_RET_FALSE(nMaster && nTarget);
	ASSERT(nMaster && nTarget);
	m_nMaster = nMaster;
	m_nTarget = nTarget;
	return TRUE;
}

// 查询子接口
BOOL KRelation::QueryRelation(INT nType, LPVOID *ppv)
{
	// KGC_CONFIRM_RET_FALSE (nType == m_nType);
	ASSERT(nType == m_nType);
	*ppv = this;
	return TRUE;
}

// 查询关系类型
INT	KRelation::GetType()
{
	return m_nType;
}

// 获取附加信息长度
INT KRelation::GetInfLen()
{
	return 0;
}

// 保存附加数据到指定缓冲区（用于同步好友信息）
BOOL KRelation::SaveInfo(CHAR *pBuf, INT nLen)
{
	// KGC_Error("");
	ASSERT(0);
	return FALSE;
}

// 加载数据
BOOL KRelation::LoadInfo(CHAR *pBuf, INT nLen)
{
	// KGC_Error("");
	ASSERT(0);
	return FALSE;
}

// 获取主位/次位玩家（bAsMaster为true表示主位，false表示次位）
INT KRelation::GetRole(BOOL bAsMaster)
{
	return bAsMaster ? m_nMaster : m_nTarget;
}
