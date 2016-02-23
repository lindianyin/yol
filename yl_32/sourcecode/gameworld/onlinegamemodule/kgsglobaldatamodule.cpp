
#ifdef GAME_SERVER

#include "stdafx.h"
#include "kgsglobaldatamodule.h"
#include "gcmodule/globaldata/kglobaldataptc.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kobjeventsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KGS_GlobalDataModule g_cGlobalDataModule;

template<>
IKModuleInterface *GetModuleInterFace<emKMODULE_GLOBALDATA>()
{
	return &g_cGlobalDataModule;
}

BOOL KGS_GlobalDataModule::SetIntValue(DWORD dwKey, INT nValue, BOOL bRequest /* = TRUE */)
{
	if (bRequest)
	{
		KGLOBALDATA_S2G_SET_INT_VALUE cSendData;
		cSendData.dwKey = dwKey;
		cSendData.nValue = nValue;

		IKGcSvcClient *pSender = GetGcSvcProvider();
		QCONFIRM_RET_FALSE(pSender);
		pSender->SendData(&cSendData, sizeof(KGLOBALDATA_S2G_SET_INT_VALUE));
	}
	else
	{
		m_mapIntData[dwKey] = nValue;
	}

	return TRUE;
}


BOOL KGS_GlobalDataModule::AddIntValue(DWORD dwKey, INT nValue, BOOL bRequest /* = TRUE */)
{
	if (bRequest)
	{
		KGLOBALDATA_S2G_ADD_INT_VALUE cSendData;
		cSendData.dwKey = dwKey;
		cSendData.nValue = nValue;

		IKGcSvcClient *pSender = GetGcSvcProvider();
		QCONFIRM_RET_FALSE(pSender);
		pSender->SendData(&cSendData, sizeof(KGLOBALDATA_S2G_ADD_INT_VALUE));
	}
	else
	{
		m_mapIntData[dwKey] += nValue;
	}


	return TRUE;
}

BOOL KGS_GlobalDataModule::GetIntValue(DWORD dwKey, INT &rnValue)
{
	// 	QCONFIRM_RET_FALSE(m_mapIntData.find(dwKey) != m_mapIntData.end());
	// 	rnValue = m_mapIntData[dwKey];

	if (m_mapIntData.find(dwKey) == m_mapIntData.end())
		rnValue = 0;

	rnValue = m_mapIntData[dwKey];

	return TRUE;
}

BOOL KGS_GlobalDataModule::SetBinaryValue(DWORD dwKey, BYTE abValue[], WORD wLength, BOOL bRequest /* = TRUE */)
{
	QCONFIRM_RET_FALSE(wLength > 0 && abValue);
	QCONFIRM_RET_FALSE(wLength < KD_BIN_DATA_MAX_LEN);

	if (bRequest)
	{
		static BYTE bySyncPackBuffer[10240];
		memset(bySyncPackBuffer, 0, sizeof(bySyncPackBuffer));
		KGLOBALDATA_S2G_SET_BIN_VALUE& cSendData = *(KGLOBALDATA_S2G_SET_BIN_VALUE* )bySyncPackBuffer;
		cSendData = KGLOBALDATA_S2G_SET_BIN_VALUE();
		cSendData.dwKey = dwKey;
		memcpy(cSendData.abValue, abValue, wLength);
		cSendData.wLength = wLength;

		IKGcSvcClient *pSender = GetGcSvcProvider();
		QCONFIRM_RET_FALSE(pSender);
		pSender->SendData(&cSendData, sizeof(KGLOBALDATA_S2G_SET_BIN_VALUE) + wLength);
	}
	else
	{
		m_mapBinaryData[dwKey] = std::vector<BYTE>(abValue, abValue + wLength);
	}

	return TRUE;
}

BOOL KGS_GlobalDataModule::DeleteBinaryValue(DWORD dwKey, BOOL bRequest /* = TRUE */)
{
	if (bRequest)
	{
		KGLOBALDATA_S2G_DEL_BIN_VALUE cSendData;
		cSendData.dwKey = dwKey;

		IKGcSvcClient *pSender = GetGcSvcProvider();
		QCONFIRM_RET_FALSE(pSender);
		pSender->SendData(&cSendData, sizeof(KGLOBALDATA_S2G_DEL_BIN_VALUE));
	}
	else
	{
		m_mapBinaryData.erase(dwKey);
	}

	return TRUE;
}

BOOL KGS_GlobalDataModule::GetBinaryValue(DWORD dwKey, BYTE abValue[], WORD &rwLength)
{
	QCONFIRM_RET_FALSE(abValue);
	QCONFIRM_RET_FALSE(rwLength > m_mapBinaryData[dwKey].size());
	QCONFIRM_RET_FALSE(m_mapBinaryData.find(dwKey) != m_mapBinaryData.end());

	rwLength = m_mapBinaryData[dwKey].size();
	QCONFIRM_RET_FALSE(rwLength > 0);
	memcpy(abValue, &m_mapBinaryData[dwKey][0], rwLength);
	
	return TRUE;
}

BOOL KGS_GlobalDataModule::ProcessGcProtocol(LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData && uDataLen > 0);
	KPTC_HEADER_BASE *pMsg = (KPTC_HEADER_BASE *)pbData;
	switch (pMsg->byProtocol)
	{
	case emKGLOBAL_DATA_PTC_G2S_INT_VALUE:
		{
			ProcessSetIntValue(pbData, uDataLen);
		}
		break;
	case emKGLOBAL_DATA_PTC_G2S_BIN_VALUE:
		{
			ProcessSetBinaryValue(pbData, uDataLen);
		}
		break;
	case emKGLOBAL_DATA_PTC_G2S_DEL_BIN_VALUE:
		{
			ProcessDeleteBinaryValue(pbData, uDataLen);
		}
		break;
	case emKGLOBAL_DATA_PTC_G2S_BATCH_INT_VALUE:
		{
			ProcessBatchIntValue(pbData, uDataLen);
		}
		break;
	case emKGLOBAL_DATA_PTC_G2S_BATCH_BIN_VALUE:
		{
			ProcessBatchBinValue(pbData, uDataLen);
		}
		break;
	default:
		QCONFIRM_RET_FALSE(FALSE);
	}
	return TRUE;
}

BOOL KGS_GlobalDataModule::ProcessSetIntValue(LPCBYTE pbData, UINT uDataLen)
{
	KGLOBALDATA_G2S_INT_VALUE *pMsg = (KGLOBALDATA_G2S_INT_VALUE *)pbData;
	return SetIntValue(MAKELONG(pMsg->wKey,pMsg->wGroupId), pMsg->nValue, FALSE);
}


BOOL KGS_GlobalDataModule::ProcessSetBinaryValue(LPCBYTE pbData, UINT uDataLen)
{
	KGLOBALDATA_G2S_BIN_VALUE *pMsg = (KGLOBALDATA_G2S_BIN_VALUE *)pbData;
	return SetBinaryValue(MAKELONG(pMsg->wKey,pMsg->wGroupId), pMsg->abValue, pMsg->wLength, FALSE);
}

BOOL KGS_GlobalDataModule::ProcessDeleteBinaryValue(LPCBYTE pbData, UINT uDataLen)
{
	KGLOBALDATA_G2S_DEL_BIN_VALUE *pMsg = (KGLOBALDATA_G2S_DEL_BIN_VALUE *)pbData;
	return DeleteBinaryValue(MAKELONG(pMsg->wKey,pMsg->wGroupId), FALSE);
}


static BOOL s_bFinishSyncIntValue = FALSE;
static BOOL s_bFinishSyncBinValue = FALSE;
BOOL KGS_GlobalDataModule::ProcessBatchIntValue(LPCBYTE pbData, UINT uDataLen)
{
	KGLOBALDATA_G2S_BATCH_INT_VALUE *pMsg = (KGLOBALDATA_G2S_BATCH_INT_VALUE *)pbData;
	for (INT i = 0; i < pMsg->wItemCount; ++i)
	{
		QCONFIRM_RET_FALSE(SetIntValue(pMsg->acIntItem[i].dwKey, pMsg->acIntItem[i].nValue, FALSE));
	}

	if (s_bFinishSyncBinValue && !s_bFinishSyncIntValue)
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("CallCenter", "OnEvent", 0, "u", emKOBJEVENTTYPE_GLOBALDATA_DATA_SYNC_FINISH);
	}

	s_bFinishSyncIntValue = TRUE;

	return TRUE;
}

BOOL KGS_GlobalDataModule::ProcessBatchBinValue(LPCBYTE pbData, UINT uDataLen)
{
	KGLOBALDATA_G2S_BATCH_BIN_VALUE *pMsg = (KGLOBALDATA_G2S_BATCH_BIN_VALUE *)pbData;
	BYTE *pData = (BYTE *)pMsg->acBinItem;
	while (pMsg->wItemCount > 0)
	{
		BIN_ITEM *pItem = (BIN_ITEM *)pData;
		QCONFIRM_RET_FALSE(SetBinaryValue(pItem->dwKey, pItem->abValue, pItem->wLength, FALSE));
		pData = pItem->abValue + pItem->wLength;
		--pMsg->wItemCount;
	}

	
	if (s_bFinishSyncIntValue && !s_bFinishSyncBinValue)
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("CallCenter", "OnEvent", 0, "u", emKOBJEVENTTYPE_GLOBALDATA_DATA_SYNC_FINISH);
	}

	s_bFinishSyncBinValue = TRUE;

	return TRUE;
}

// -------------------------------------------------------------------------
#endif
