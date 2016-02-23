/* -------------------------------------------------------------------------
//	文件名		：	kglobaldataprotocolprocess.cpp
//	创建者		：	liuchang
//	创建时间	：	2010-8-19 15:53:42
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kglobaldataprotocolprocess.h"
#include "gcmodule/globaldata/kglobaldataptc.h"
#include "kgcglobaldatamodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
KGlobalDataProtocolProcessor g_cGlobalDataAcceptor;
// -------------------------------------------------------------------------
BOOL KGlobalDataProtocolProcessor::OnConnectClose(INT nConnectId)
{
	return TRUE;
}

BOOL KGlobalDataProtocolProcessor::OnConnected(INT nConnectId)
{
	g_cGlobalDataModule.BatchSyncIntData(nConnectId);
	g_cGlobalDataModule.BatchSyncBinData(nConnectId);

	return TRUE;
}

BOOL KGlobalDataProtocolProcessor::ProcessData(INT nConnectId, LPBYTE pData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pData && uSize > 0);
	KPTC_HEADER_BASE *pMsg = (KPTC_HEADER_BASE *)pData;
	switch (pMsg->byProtocol)
	{
	case emKGLOBAL_DATA_PTC_S2G_SET_INT_VALUE:
		{
			ProcessSetIntData(nConnectId, pData, uSize);
		}
		break;
	case emKGLOBAL_DATA_PTC_S2G_ADD_INT_VALUE:
		{
			ProcessAddIntData(nConnectId, pData, uSize);
		}
		break;
	case emKGLOBAL_DATA_PTC_S2G_SET_BIN_VALUE:
		{
			ProcessSetBinData(nConnectId, pData, uSize);
		}
		break;
	case emKGLOBAL_DATA_PTC_S2G_DEL_BIN_VALUE:
		{
			ProcessDelBinData(nConnectId, pData, uSize);
		}
		break;
	default:
		QCONFIRM_RET_FALSE(FALSE);
	}
	return TRUE;
}

BOOL KGlobalDataProtocolProcessor::ProcessSetIntData(INT nConnectId, LPBYTE pData, UINT uSize)
{
	KGLOBALDATA_S2G_SET_INT_VALUE *pMsg = (KGLOBALDATA_S2G_SET_INT_VALUE *)pData;
	g_cGlobalDataModule.SetIntValue(HIWORD(pMsg->dwKey), LOWORD(pMsg->dwKey), pMsg->nValue, nConnectId);

	return TRUE;
}

BOOL KGlobalDataProtocolProcessor::ProcessAddIntData(INT nConnectId, LPBYTE pData, UINT uSize)
{
	KGLOBALDATA_S2G_ADD_INT_VALUE *pMsg = (KGLOBALDATA_S2G_ADD_INT_VALUE *)pData;
	g_cGlobalDataModule.AddIntValue(HIWORD(pMsg->dwKey), LOWORD(pMsg->dwKey), pMsg->nValue, nConnectId);

	return TRUE;
}

BOOL KGlobalDataProtocolProcessor::ProcessSetBinData(INT nConnectId, LPBYTE pData, UINT uSize)
{
	KGLOBALDATA_S2G_SET_BIN_VALUE *pMsg = (KGLOBALDATA_S2G_SET_BIN_VALUE *)pData;
	g_cGlobalDataModule.SetBinaryValue(HIWORD(pMsg->dwKey), LOWORD(pMsg->dwKey), pMsg->abValue, pMsg->wLength, nConnectId);

	return TRUE;
}

BOOL KGlobalDataProtocolProcessor::ProcessDelBinData(INT nConnectId, LPBYTE pData, UINT uSize)
{
	KGLOBALDATA_S2G_DEL_BIN_VALUE *pMsg = (KGLOBALDATA_S2G_DEL_BIN_VALUE *)pData;
	g_cGlobalDataModule.DeleteBinaryValue(HIWORD(pMsg->dwKey), LOWORD(pMsg->dwKey), nConnectId);

	return TRUE;
}
// -------------------------------------------------------------------------
