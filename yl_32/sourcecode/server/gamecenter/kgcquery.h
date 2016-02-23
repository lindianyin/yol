/* -------------------------------------------------------------------------
//	文件名		：	kgcquery.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/1 16:43:30
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGCQUERY_H__
#define __KGCQUERY_H__
#include "serverbase/kgcmoduleinterface.h"
#include "kgcserver.h"

#include <map>
// -------------------------------------------------------------------------
class KGcQuery : public IKGcQuery
{
public:
	KGcQuery(BYTE byModuleId);
	// 查询接口
	virtual BOOL QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void **ppvObject);
private:
	KGcServer m_cGcServer;
	BYTE m_byModuleId;	// 属于该Query接口的模块ID
};

// -------------------------------------------------------------------------

#endif /* __KGCQUERY_H__ */
