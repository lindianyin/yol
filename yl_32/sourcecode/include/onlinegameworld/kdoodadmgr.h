/* -------------------------------------------------------------------------
//	文件名		：	nsbase/kdoodadmgr.h
//	创建者		：	simon
//	创建时间	：	2010/3/11 11:17:02
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __NSBASE_KDOODADMGR_H__
#define __NSBASE_KDOODADMGR_H__

#include "onlinegameworld/kidobjpool.h"
#include "onlinegameworld/kdoodad.h"
// -------------------------------------------------------------------------

typedef std::map<DWORD, KDOODAD_TEMPLATE> KDoodadTemplateMap;
typedef KDoodadTemplateMap::iterator KDoodadTemplateMapIt;


class KDoodadMgr
{
public:
	KDoodadMgr(void);
	~KDoodadMgr(void);

public:
	BOOL Init();
	BOOL Uninit();
	BOOL Activate();
	KDoodad* GetById(DWORD dwId);
	KDoodad* GetByIndex(INT nIndex);
	CONST KDOODAD_TEMPLATE* GetTemplateInfo(DWORD dwTemplateId);
	CONST CHAR* GetNameByTemplateId(DWORD dwTemplateId);

public:
	BOOL AddToScene(KDoodad* pstDoodad, DWORD dwSceneId, INT nX, INT nY, INT nZ);
	BOOL RemoveFromScene(DWORD dwId);
	KDoodad* CreateDoodad(DWORD dwTemplateId, DWORD dwId = KD_BAD_ID);
	BOOL DestroyDoodad(DWORD dwId);
	BOOL NotifyRemoveFromScene(DWORD dwId);
	BOOL RemoveDoodad(DWORD dwId);
	BOOL RemoveDoodadNoScene(DWORD dwId);

	BOOL CheckAutoPick(KDoodad* pDoodad);

	INT GetDoodadCount() { return m_cObjPool.GetUsedCount(); }
protected:
	KDoodadTemplateMap m_mapDoodadTemplate;
	TKIdObjPool<KDoodad, KD_DOODAD_ID_MASK> m_cObjPool;

private:
	BOOL LoadDoodadTemplate();

};

extern KDoodadMgr g_cDoodadMgr;
// -------------------------------------------------------------------------

#endif /* __NSBASE_KDOODADMGR_H__ */
