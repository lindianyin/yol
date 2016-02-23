/* -------------------------------------------------------------------------
//	文件名		：	kgc_transfer.h
//	创建者		：	luobaohang
//	创建时间	：	2007-5-22 19:19:25
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGC_TRANSFER_H__
#define __KGC_TRANSFER_H__
#include "gc_lib/kgc_submodulebase.h"
// -------------------------------------------------------------------------
class KGC_Transfer_GS : public KGC_SubModuleBase
{
public:
	KGC_Transfer_GS() : KGC_SubModuleBase(PF_TRANSFER, "TRANSFER") {}
	
protected:
	virtual BOOL ProcessProtocol(CHAR *pData, INT nSize, INT nConnectID);
	BOOL ProcTransferRole(CHAR *pData, INT nSize, INT nSrcConnectID);
	BOOL ProcTransRolePermit(CHAR *pData, INT nSize, INT nSrcConnectID);
private:
	BOOL ProcTransData(CHAR *pData, INT nSize, INT nSrcConnectID);
};
extern KGC_Transfer_GS g_Transfer;
// -------------------------------------------------------------------------

#endif /* __KGC_TRANSFER_H__ */
