/* -------------------------------------------------------------------------
//	文件名		：	KNpcMgr.h
//	创建者		：	simon
//	创建时间	：	2009/11/21 12:10:56
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KNPCMGR_H__
#define __KNPCMGR_H__

#include "onlinegameworld/kidobjpool.h"
#include "onlinegameworld/knpc.h"

// -------------------------------------------------------------------------

class KNpcMgr
{
public:
	BOOL	Init();
	BOOL	Uninit();
	INT		Add(DWORD dwTemplateId, DWORD dwId = KD_BAD_ID);
	BOOL	Remove(DWORD dwId, BOOL bRegenerate = FALSE);
	KNpc*	GetById(DWORD dwId);
	KNpc*	GetByIndex(INT nIndex);
	const KNPC_TEMPLATE* GetTemplateInfo(DWORD dwTemplateID);

	INT GetNpcCount() { return m_cObjPool.GetUsedCount(); }
protected:
	typedef std::map<DWORD, KNPC_TEMPLATE> KNpcTemplates;
	TKIdObjPool<KNpc, KD_NPC_ID_MASK> m_cObjPool;
	KNpcTemplates	m_mapNpcTemplate;
private:
	BOOL	ReadNpcTemplate();
};

extern KNpcMgr g_cNpcMgr;
// -------------------------------------------------------------------------

#endif /* __KNPCMGR_H__ */
