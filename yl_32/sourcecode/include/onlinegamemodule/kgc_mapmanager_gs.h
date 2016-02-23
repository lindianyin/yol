/* -------------------------------------------------------------------------
//	文件名		：	kgc_mapmanager_gs.h
//	创建者		：	luobaohang
//	创建时间	：	2007-5-18 23:45:23
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGC_MAPMANAGER_GS_H__
#define __KGC_MAPMANAGER_GS_H__
#ifdef KD_OP_GAME_SERVER

#include "gc_core/kgc_mapmanager.h"
#include "onlinegameworld/ikbishopsvc.h"
// -------------------------------------------------------------------------
class KGC_MapLogic_GS : public KGC_MapLogic
{
public:
};

class KGC_MapManager_GS : public KGC_MapManager
{
public:
	KGC_MapManager_GS(KGC_MapLogic_GS* pApplyLogic, KGC_MapLogic_GS* pResultLogic) : 
	  KGC_MapManager(pApplyLogic, pResultLogic)
	  {
		  g_pMapManager = this;
	  }
	BOOL ApplyMapAssign( INT nServerID );
	// 发送已加载地图信息到Bishop及Gamecenter
	BOOL SendLoadedMap();
	BOOL RegisterMap(INT nMapID, INT nMapTemplateID, BOOL bRegister2Bishop);
	// 申请一个动态地图Id创建动态副本地图, bOccupy是否永久地图Id
	BOOL ApplyCreateNewDynMap(KE_DYN_MAP_TYPE eType, DWORD dwMapCopy, DWORD dwParam, BOOL bOccupy);
protected:
	virtual BOOL ProcessProtocol(CHAR* pData, INT nSize, INT nConnectId);
private:
	virtual BOOL ProcMapAssign(CHAR *pData, INT nSize, INT nConnectId);
	virtual BOOL ProcDynMapCreate(CHAR *pData, INT nSize, INT nConnectId);
	// 处理动态地图Id申请结果协议
	BOOL ProcNewDynMapId(CHAR* pData, INT nSize, INT nConnectId);

	virtual BOOL Init() { m_cBishopUser.Init(this); return TRUE; }
	////////////////////////////////Bishop Consumer接口//////////////////////////////////////////
	class KBishopUser : public IKBishopSvcUser
	{
	public:
		BOOL Init(KGC_MapManager_GS* pMan);

		STDPROC_(BOOL)	ProcessData(LPCBYTE pData, UINT uSize);
	protected:
		KGC_MapManager_GS* m_pMapMan;
	};
	KBishopUser m_cBishopUser;
};
extern KGC_MapManager_GS g_MapManager;
// -------------------------------------------------------------------------
#endif // #ifdef KD_OP_GAME_SERVER
#endif /* __KGC_MAPMANAGER_GS_H__ */
