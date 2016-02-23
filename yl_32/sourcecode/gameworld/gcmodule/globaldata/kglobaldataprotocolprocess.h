/* -------------------------------------------------------------------------
//	文件名		：	kglobaldataprotocolprocess.h
//	创建者		：	liuchang
//	创建时间	：	2010-8-19 15:41:29
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGLOBALDATAPROTOCOLPROCESS_H__
#define __KGLOBALDATAPROTOCOLPROCESS_H__

#include "serverbase/kgcmoduleinterface.h"

// -------------------------------------------------------------------------
// 处理GS协议
class KGlobalDataProtocolProcessor : public IKProcessServer
{
public:
	// IKProcessServer
	virtual BOOL OnConnected(INT nConnectId);
	virtual BOOL OnConnectClose(INT nConnectId);
	virtual BOOL ProcessData(INT nConnectId, LPBYTE pData, UINT uSize);

private:
	BOOL ProcessSetIntData(INT nConnectId, LPBYTE pData, UINT uSize);
	BOOL ProcessAddIntData(INT nConnectId, LPBYTE pData, UINT uSize);
	BOOL ProcessSetBinData(INT nConnectId, LPBYTE pData, UINT uSize);
	BOOL ProcessDelBinData(INT nConnectId, LPBYTE pData, UINT uSize);
};
extern KGlobalDataProtocolProcessor g_cGlobalDataAcceptor;
// -------------------------------------------------------------------------

#endif /* __KGLOBALDATAPROTOCOLPROCESS_H__ */
