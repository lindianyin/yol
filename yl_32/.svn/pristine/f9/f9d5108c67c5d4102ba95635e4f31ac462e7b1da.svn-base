
#pragma once

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


