#include "stdafx.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/characterbuffer.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "qplayershop.h"
#include "kbuysellmgr.h"
#include "item2/kitemmgr.h"
#include "record/RecordRelayClient.h"
#include "qshopmodule.h"

PlayerShop::PlayerShop(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerShop::~PlayerShop()
{

}

BOOL PlayerShop::Init()
{
	m_vecSaveData.clear();
	return TRUE;
}

BOOL PlayerShop::UnInit()
{
	m_vecSaveData.clear();
	//SAFE_DELETE(m_pLunaObj);
	return TRUE;
}

BOOL PlayerShop::OnLogin()
{
	SyncAllPurchaseRecord();
	return TRUE;
}

BOOL PlayerShop::OnLogout()
{
	return TRUE;
}

BOOL PlayerShop::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	LPCBYTE pBuffer = pData;
	SIZE_T uUsedSize = uSize;

	if (uSize == 0)
		return TRUE;

	if (pData == NULL)
	{
		QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(BufferSaveData)");
		return FALSE;
	}

	while(uUsedSize > 0)
	{
		ShopSaveData sSavedata;
		memcpy(&sSavedata, pBuffer, sizeof(ShopSaveData));
		pBuffer += sizeof(ShopSaveData);
		uUsedSize -= sizeof(ShopSaveData);

		m_vecSaveData.push_back(sSavedata);
	}

	SyncMyPurchaseRecord();
EXIT0:
	return TRUE;
}

BOOL PlayerShop::OnDbSave(LPBYTE pData, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	LPBYTE pBuffer = pData;
	LOG_PROCESS_ERROR(pBuffer);

	for (std::vector<ShopSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{

		memcpy(pBuffer, &(*it), sizeof(ShopSaveData));
		pBuffer += sizeof(ShopSaveData);
		uUsedSize += sizeof(ShopSaveData);
	}


EXIT0:
	return TRUE;
}

BOOL PlayerShop::SyncMyPurchaseRecord()
{
	IMemBlock* piPackage					= NULL;
	KSHOP_S2C_MY_PURCHASE_SYNC* ptcInfo		= NULL;
	INT nIdx								= m_vecSaveData.size();
	BOOL bOk								= FALSE;
	piPackage = QCreateMemBlock(sizeof(KSHOP_S2C_MY_PURCHASE_SYNC)+sizeof(KSHOP_PURCHASE_RECORD) * nIdx);
	ptcInfo = (KSHOP_S2C_MY_PURCHASE_SYNC*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcInfo);
	
	ptcInfo->byProtocol					= (BYTE)emSHOP_PTC_S2C_SYNC_MY_PURCHASE;
	ptcInfo->nCount						= nIdx;

	nIdx = 0;
	for (std::vector<ShopSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		ptcInfo->arrInfo[nIdx].nGoodsId = it->nGoodsId;
		ptcInfo->arrInfo[nIdx].nCategoryId = it->nCategoryId;
		ptcInfo->arrInfo[nIdx].nShopId= it->nShopId;
		ptcInfo->arrInfo[nIdx].nCount = it->nCount;
		ptcInfo->arrInfo[nIdx].dwLastTime = it->dwLastTime;
		nIdx++;

	}

	bOk = g_cShopModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcInfo, sizeof(KSHOP_S2C_MY_PURCHASE_SYNC)+sizeof(KSHOP_PURCHASE_RECORD) * nIdx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}
BOOL PlayerShop::SyncAllPurchaseRecord()
{
	QLogPrintf(LOG_INFO,"..PlayerShop::SyncAllPurchaseRecord()..");
	IMemBlock* piPackage					= NULL;
	KSHOP_S2C_MY_PURCHASE_SYNC* ptcInfo		= NULL;
	std::vector<KBuyAllPurchaseRecord> vecRecord = g_cBuySellMgr.GetAllPurchaseRecord();
	INT nIdx								= vecRecord.size();
	BOOL bOk								= FALSE;

	piPackage = QCreateMemBlock(sizeof(KSHOP_S2C_MY_PURCHASE_SYNC)+sizeof(KSHOP_PURCHASE_RECORD) * nIdx);
	ptcInfo = (KSHOP_S2C_MY_PURCHASE_SYNC*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcInfo);
	
	ptcInfo->byProtocol					= (BYTE)emSHOP_PTC_S2C_SYNC_ALL_PURCHASE;
	ptcInfo->nCount						= nIdx;

	nIdx = 0;
	for (std::vector<KBuyAllPurchaseRecord>::iterator it = vecRecord.begin(); it != vecRecord.end(); ++it)
	{
		ptcInfo->arrInfo[nIdx].nGoodsId = it->nGoodsId;
		ptcInfo->arrInfo[nIdx].nCategoryId = it->nCategoryId;
		ptcInfo->arrInfo[nIdx].nShopId= it->nShopId;
		ptcInfo->arrInfo[nIdx].nCount = it->nCount;
		ptcInfo->arrInfo[nIdx].dwLastTime = it->dwLastTime;
		ptcInfo->arrInfo[nIdx].nTotal = it->nTotal;//
		ptcInfo->arrInfo[nIdx].nMaxCount= it->nMaxCount;//
		QLogPrintf(LOG_INFO,"nCount=%d,nTotal=%d,nMaxCount=%d",ptcInfo->arrInfo[nIdx].nCount,ptcInfo->arrInfo[nIdx].nTotal,ptcInfo->arrInfo[nIdx].nMaxCount);
		nIdx++;
	}
	
	bOk = g_cShopModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcInfo, sizeof(KSHOP_S2C_MY_PURCHASE_SYNC)+sizeof(KSHOP_PURCHASE_RECORD) * nIdx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

BOOL PlayerShop::DoBuyGoods(INT nShopId, INT nCategoryId, INT nGoodsId, INT nCount)
{
	QLogPrintf(LOG_INFO,"PlayerShop::DoBuyGoods(INT nShopId=%d, INT nCategoryId=%d, INT nGoodsId=%d, INT nCount=%d)"
		,nShopId,nCategoryId,nGoodsId,nCount);
	
	BOOL result = FALSE;
	DWORD dwStartTime;
	DWORD dwNow = KSysService::GameTime(NULL);
	CONST KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	KGoods *pGoods = g_cBuySellMgr.GetGoodsByIndex(nShopId, nCategoryId, nGoodsId);
	ShopSaveData* pSaveData = NULL;
	KBuyAllPurchaseRecord *pRecord = NULL;
	S2R_BUY_ALL_PURCHASE_REQUEST sRequest;

	if (pGoods == NULL || pConfig == NULL)
		goto EXIT0;

	if (pGoods->m_nPurchaseLimit > 0 && nCount > pGoods->m_nPurchaseLimit)
		goto EXIT0;

	if (pGoods->m_nLimitType == 0)
	{
		result = IndividualPurchase(pGoods, nShopId, nCategoryId, nGoodsId, nCount);
		goto EXIT0;
	}
	else if (pGoods->m_nLimitType == 1)
	{
		pRecord = g_cBuySellMgr.GetAllPurchaseRecord(nShopId, nCategoryId, nGoodsId);
		if (pRecord == NULL || pGoods->m_nPurchaseLimit <= 0 || pRecord->nCount + nCount <= pGoods->m_maxCount)
		{
			//发协议到cr验证是否有货
			sRequest.dwRoleId = m_rcPlayer.GetId();
			sRequest.nShopId = nShopId;
			sRequest.nCategoryId = nCategoryId;
			sRequest.nGoodsId = nGoodsId;
			sRequest.nCount = nCount;
			sRequest.dwLimitTime = pGoods->m_nLimitTime;
			sRequest.dwStartTime = pGoods->m_timeSaleStart;
			sRequest.nMaxCount = pGoods->m_maxCount;
			RecordRelayClient::DoBuyAllPurchaseRequest(sRequest);
		}
		goto EXIT0;
	}

	result = TRUE;
EXIT0:
	return result;
}

BOOL PlayerShop::IndividualPurchase(KGoods *pGoods, INT nShopId, INT nCategoryId, INT nGoodsId, INT nCount)
{
	DWORD dwStartTime;
	DWORD dwNow = KSysService::GameTime(NULL);
	CONST KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	ShopSaveData* pSaveData = NULL;

	for (std::vector<ShopSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nShopId == nShopId && it->nCategoryId == nCategoryId && it->nGoodsId == nGoodsId)
		{
			pSaveData = &(*it);
			break;
		}
	}

	if (pGoods->m_nPurchaseLimit > 0)
	{
		if (pGoods->m_timeSaleStart > 0)
			dwStartTime = pGoods->m_timeSaleStart;
		else
			dwStartTime = pConfig->dwStartSeverTime;

		if (pSaveData != NULL)
		{
			if (pGoods->m_nLimitTime <= 0 ||
				(pSaveData->dwLastTime - dwStartTime) / pGoods->m_nLimitTime == (dwNow - dwStartTime) / pGoods->m_nLimitTime)
			{
				if (pSaveData->nCount + nCount > pGoods->m_nPurchaseLimit)
				{
					goto EXIT0;
				}

				pSaveData->nCount += nCount;
				pSaveData->dwLastTime = dwNow;
			}
			else
			{
				pSaveData->nCount = nCount;
				pSaveData->dwLastTime = dwNow;
			}
		}
		else
		{
			ShopSaveData sSaveData;
			sSaveData.nCount = nCount;
			sSaveData.nShopId = nShopId;
			sSaveData.nCategoryId = nCategoryId;
			sSaveData.nGoodsId = nGoodsId;
			sSaveData.dwLastTime = dwNow;
			m_vecSaveData.push_back(sSaveData);
		}

	}
	SyncMyPurchaseRecord();
	return TRUE;
EXIT0:
	return FALSE;
}


//LuaPlayerTitle* PlayerTitle::GetScriptInterface() CONST
//{
//	return m_pLunaObj;
//}