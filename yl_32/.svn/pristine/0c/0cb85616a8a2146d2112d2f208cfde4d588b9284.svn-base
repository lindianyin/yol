#include "stdafx.h"
#include "playervip.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/characterbuffer.h"
#include "vipsetting.h"
#include "vipmodule.h"
#include "vipprotocol.h"
#include "award/kawardmanager.h"
#include "award/awarddef.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
//#include "../gameworld/nsworld/kcharacterhelper.h"
#include "Runtime/TypeDef.h"


PlayerVip::PlayerVip(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerVip::~PlayerVip()
{

}

BOOL PlayerVip::Init()
{
	m_pLunaObj = new LuaPlayerVip(m_rcCharacter);
	//memset(m_SaveData.arrViplevel, 0, MAX_VIP_LEVEL * sizeof(INT));
	m_SaveData.mapVipCount.clear();
	m_SaveData.nExp = 0;
	ZeroMemory(m_SaveData.szMsg,sizeof(m_SaveData.szMsg));
	ResetFreeState();

	return TRUE;
}

BOOL PlayerVip::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	//memset(m_SaveData.arrViplevel, 0, MAX_VIP_LEVEL * sizeof(INT));
	m_SaveData.mapVipCount.clear();
	m_SaveData.nExp = 0;
	return TRUE;
}

BOOL PlayerVip::OnLogin()
{
	return TRUE;
}

BOOL PlayerVip::OnLogout()
{
	return TRUE;
}

//BOOL PlayerVip::Active()
//{
//	// 每秒检测一次
//	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
//		return TRUE;
//	return TRUE;
//}


BOOL PlayerVip::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	LPCBYTE pBuffer = pData;
	SIZE_T uUsedSize = uSize;

	tm *pNow = NULL;
	INT nYear = 0;
	INT nMonth = 0;
	INT nDay = 0;
	tm *pLogout = NULL;

	m_SaveData.mapVipCount.clear();
	TIME_T dwLogoutTime = 0;
	//TIME_T tNow = 0;
	TIME_T dwNow = 0;

	if (uSize == 0)
		return TRUE;

	if (pData == NULL)
	{
		QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(BufferSaveData)");
		return FALSE;
	}

	LOG_PROCESS_ERROR(uSize >= sizeof(INT));// + MAX_VIP_LEVEL * sizeof(INT));

	memcpy(&m_SaveData.nExp, pBuffer, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize -= sizeof(INT);


	memcpy(&m_SaveData.sFree, pBuffer, sizeof(VipFreeExp));
	pBuffer += sizeof(VipFreeExp);
	uUsedSize -= sizeof(VipFreeExp);

	memcpy(m_SaveData.szMsg, pBuffer, sizeof(m_SaveData.szMsg));
	pBuffer += sizeof(m_SaveData.szMsg);
	uUsedSize -= sizeof(m_SaveData.szMsg);

	

	/*memcpy(m_SaveData.arrViplevel, pBuffer, MAX_VIP_LEVEL * sizeof(INT));
	pBuffer += MAX_VIP_LEVEL * sizeof(INT);
	uUsedSize -= MAX_VIP_LEVEL * sizeof(INT);*/

	dwLogoutTime = m_rcPlayer.GetLastLoginTime();
	dwNow = KSysService::GameTime(NULL);
	pNow = ::localtime(&dwNow);
	nYear = pNow->tm_year;
	nMonth = pNow->tm_mon;
	nDay = pNow->tm_mday;
	pLogout = ::localtime(&dwLogoutTime);

	//上次下线时间判断
	if (nYear == pLogout->tm_year && nMonth == pLogout->tm_mon && nDay == pLogout->tm_mday)
	{
		while(uUsedSize > 0)
		{
			VipBuyCount sVipBuyCount;
			memcpy(&sVipBuyCount, pBuffer, sizeof(VipBuyCount));
			pBuffer += sizeof(VipBuyCount);
			uUsedSize -= sizeof(VipBuyCount);
			m_SaveData.mapVipCount[sVipBuyCount.nId] = sVipBuyCount;
		}
	}
	else
	{
		DoAddDegree();
	}

	if(m_SaveData.sFree.dwFreeTime > 0)
	{
		SyncFreeExpTime();
	}

EXIT0:
	return TRUE;
}

BOOL PlayerVip::OnDbSave(LPBYTE pData, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	LPBYTE pBuffer = pData;
	LOG_PROCESS_ERROR(pBuffer);

	memcpy(pBuffer, &m_SaveData.nExp, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	memcpy(pBuffer, &m_SaveData.sFree, sizeof(VipFreeExp));
	pBuffer += sizeof(VipFreeExp);
	uUsedSize += sizeof(VipFreeExp);

	/*memcpy(pBuffer, m_SaveData.arrViplevel, MAX_VIP_LEVEL * sizeof(INT));
	pBuffer += sizeof(INT) * MAX_VIP_LEVEL;
	uUsedSize += sizeof(INT) * MAX_VIP_LEVEL;*/

	memcpy(pBuffer, m_SaveData.szMsg, sizeof(m_SaveData.szMsg));
	pBuffer += sizeof(m_SaveData.szMsg);
	uUsedSize += sizeof(m_SaveData.szMsg);
	

	

	for (std::map<INT, VipBuyCount>::iterator it = m_SaveData.mapVipCount.begin(); it != m_SaveData.mapVipCount.end(); ++it)
	{
		memcpy(pBuffer, &(it->second), sizeof(VipBuyCount));

		pBuffer += sizeof(VipBuyCount);
		uUsedSize += sizeof(VipBuyCount);
	}

EXIT0:
	return TRUE;
}

BOOL PlayerVip::IsOpen(INT nId, BOOL bSendMsg)
{
	INT nVipLevel = m_rcPlayer.GetVipLevel();
	CHAR szMsg[128];
	VipAttributeInfo* pAttribute = g_cVipSetting.GetVipAttribute(nId, nVipLevel);
	LOG_PROCESS_ERROR(pAttribute);

	if (pAttribute->nIsOpen == 0)
	{
		if (bSendMsg == 0)
		{
			return FALSE;
		}

		while(++nVipLevel <= MAX_VIP_LEVEL)
		{
			pAttribute = g_cVipSetting.GetVipAttribute(nId, nVipLevel);
			LOG_PROCESS_ERROR(pAttribute);
			if (pAttribute->nIsOpen == 1)
			{
				sprintf(szMsg, LANG_008, nVipLevel);
				g_cPlayerBaseProcess.DoSyncBlackMsg(&m_rcPlayer, szMsg);
				return FALSE;
			}
		}
	}

	return TRUE;

EXIT0:
	return FALSE;
}

INT PlayerVip::GetTotalBuyCount(INT nId)
{
	INT nVipLevel = m_rcPlayer.GetVipLevel();
	VipAttributeInfo* pAttribute = g_cVipSetting.GetVipAttribute(nId, nVipLevel);
	LOG_PROCESS_ERROR(pAttribute);

	return pAttribute->nNum;

EXIT0:
	return 0;
}

INT PlayerVip::GetNowBuyCount(INT nId)
{
	return m_SaveData.mapVipCount[nId].nCount;
}

INT PlayerVip::GetRate(INT nId)
{
	INT nVipLevel = m_rcPlayer.GetVipLevel();
	VipAttributeInfo* pAttribute = g_cVipSetting.GetVipAttribute(nId, nVipLevel);
	LOG_PROCESS_ERROR(pAttribute);

	//return MAX(1, pAttribute->nRate);
	return pAttribute->nRate;
EXIT0:
	return 0;
}


BOOL PlayerVip::SyncBuyCount()
{
	IMemBlock* piPackage					= NULL;
	VIP_S2C_BUY_COUNT* ptcBuyCount			= NULL;
	INT nInx								= m_SaveData.mapVipCount.size();
	BOOL bOk								= FALSE;
	piPackage = QCreateMemBlock(sizeof(VIP_S2C_BUY_COUNT)+sizeof(VIP_BUY_COUNT) * nInx);
	ptcBuyCount = (VIP_S2C_BUY_COUNT*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcBuyCount);

	ptcBuyCount->byProtocol					= (BYTE)emVIP_PTC_S2C_SYNC_COUNT;
	ptcBuyCount->nCount						= nInx;

	nInx = 0;
	for (std::map<INT, VipBuyCount>::iterator it = m_SaveData.mapVipCount.begin(); it != m_SaveData.mapVipCount.end(); ++it)
	{
		ptcBuyCount->buyCount[nInx].nId = it->second.nId;
		ptcBuyCount->buyCount[nInx].nCount = it->second.nCount;
		nInx++;

	}

	bOk =  g_VipModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcBuyCount, sizeof(VIP_S2C_BUY_COUNT)+sizeof(VIP_BUY_COUNT) * nInx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

//BOOL PlayerVip::SyncRecevie()
//{
//	VIP_S2C_RECEVIE ptcRecevie;
//	ptcRecevie.byProtocol = (BYTE)emVIP_PTC_S2C_SYNC_RECEVIE;
//	memcpy(ptcRecevie.nRecevie, m_SaveData.arrViplevel, MAX_VIP_LEVEL  * sizeof(INT));
//
//	return g_VipModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), ptcRecevie);
//}

// 获得到目标vip等级所需的元宝数量
INT PlayerVip::GetOverviewCoins(INT nTargetVipLevel)
{
	INT nTotalCoin = 0;
	VipOverview *pOverview = NULL;

	pOverview = g_cVipSetting.GetVipAttribute(nTargetVipLevel);
	LOG_PROCESS_ERROR(pOverview);
	nTotalCoin = pOverview->nNeedCoins;

EXIT0:
	return nTotalCoin;
}

BOOL PlayerVip::AddExp(INT nCoins)
{
	INT nVipLevel = 0;
	VipOverview *pOverview = NULL;

	m_SaveData.nExp += nCoins;

	for (INT i = 0; i < MAX_VIP_LEVEL; ++i)
	{
		pOverview = g_cVipSetting.GetVipAttribute(i + 1);
		LOG_PROCESS_ERROR(pOverview);

		if (m_SaveData.nExp < pOverview->nNeedCoins)
			break;

		nVipLevel = i + 1;
	}

	m_rcCharacter.SetVipLevel(nVipLevel);

EXIT0:
	return TRUE;
};

INT PlayerVip::GetExp()
{
	return m_SaveData.nExp;
}

BOOL PlayerVip::SyncExp()
{
	//AddExp(0);
	VIP_S2C_EXP ptcExp;
	ptcExp.byProtocol = (BYTE)emVIP_PTC_S2C_SYNC_EXP;
	ptcExp.nExp = m_SaveData.nExp;

	return g_VipModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), ptcExp);
}

BOOL PlayerVip::DoSync()
{
	//SyncRecevie();
	SyncExp();
	return TRUE;
}

BOOL PlayerVip::DoRecevie(INT nVipLevel)
{
	QLogPrintf(LOG_INFO,"..PlayerVip::DoRecevie nVipLevel=%d",nVipLevel);
	BOOL result = FALSE;
	KAward* pAward = NULL;
	VipOverview* pOverview = NULL;
	std::map<INT, VipGift*> mapVipGifts;
	LOG_PROCESS_ERROR (1 <= nVipLevel && nVipLevel <= MAX_VIP_LEVEL);
	//LOG_PROCESS_ERROR (m_SaveData.arrViplevel[nVipLevel-1] == 0);
	LOG_PROCESS_ERROR (m_rcPlayer.GetVipLevel() >= nVipLevel);

	pOverview = g_cVipSetting.GetVipAttribute(nVipLevel);
	LOG_PROCESS_ERROR(pOverview);
	pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer,"VipAward", pOverview->nAwardId);
	LOG_PROCESS_ERROR(pAward);

	/*m_SaveData.arrViplevel[nVipLevel-1] = 1;
	SyncRecevie();
	return TRUE;*/
EXIT0:
	return FALSE;
}

BOOL PlayerVip::DoBuyCount(INT nId, INT nNum)
{
	//调用脚本系统初始化
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nVipLevel = m_rcPlayer.GetVipLevel();
	VipAttributeInfo* pAttribute = g_cVipSetting.GetVipAttribute(nId, nVipLevel);
	LPCSTR szModuleName = g_cVipSetting.GetModuleName(nId);
	LOG_PROCESS_ERROR(pAttribute);

	//次数
	if (pAttribute->nType != 3)
		return FALSE;

	if (pAttribute->nNum < nNum + m_SaveData.mapVipCount[nId].nCount)
		return FALSE;

	cSafeScript->CallTableFunction("Vip" , "AddDegree", 1, "osd", m_rcPlayer.GetScriptInterface(), szModuleName, nNum);
	m_SaveData.mapVipCount[nId].nCount += nNum;
	m_SaveData.mapVipCount[nId].nId = nId;
	SyncBuyCount();

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerVip::CanBuyCount(INT nId, INT nNum)
{
	//调用脚本系统初始化
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nVipLevel = m_rcPlayer.GetVipLevel();
	VipAttributeInfo* pAttribute = g_cVipSetting.GetVipAttribute(nId, nVipLevel);
	LPCSTR szModuleName = g_cVipSetting.GetModuleName(nId);
	LOG_PROCESS_ERROR(pAttribute);

	//次数
	if (pAttribute->nType != 3)
		return FALSE;

	if (pAttribute->nNum < nNum + m_SaveData.mapVipCount[nId].nCount)
		return FALSE;

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerVip::RefreshSaveData()
{
	for (std::map<INT, VipBuyCount>::iterator it = m_SaveData.mapVipCount.begin(); it != m_SaveData.mapVipCount.end(); ++it)
	{
		it->second.nCount = 0;
	}
	DoAddDegree();
	return TRUE;
}

BOOL PlayerVip::DoAddDegree()
{
	VipAttributeInfo* pAttribute = NULL;
	INT nVipLevel = m_rcPlayer.GetVipLevel();
	LPCSTR szModuleName = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	int nId = 1;
	do 
	{
		pAttribute = g_cVipSetting.GetVipAttribute(nId, nVipLevel);
		szModuleName = g_cVipSetting.GetModuleName(nId);
		nId++;
		if (pAttribute == NULL) return TRUE;

		if (pAttribute->nType == 0 && strcmp(szModuleName, "") != 0)
		{
			cSafeScript->CallTableFunction("Vip" , "AddDegree", 1, "osd", m_rcPlayer.GetScriptInterface(), szModuleName, pAttribute->nNum);
		}
	} while (pAttribute != NULL);

	return TRUE;
}

BOOL PlayerVip::DoAddDegree(INT nOldVipLevel, INT nNewVipLevel)
{
	VipAttributeInfo* pOldAttribute = NULL;
	VipAttributeInfo* pNewAttribute = NULL;
	LPCSTR szModuleName = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	int nId = 1;
	do 
	{
		pOldAttribute = g_cVipSetting.GetVipAttribute(nId, nOldVipLevel);
		pNewAttribute = g_cVipSetting.GetVipAttribute(nId, nNewVipLevel);
		szModuleName = g_cVipSetting.GetModuleName(nId);
		nId++;
		if (pNewAttribute == NULL) return TRUE;

		if (pNewAttribute->nType == 0 && strcmp(szModuleName, "") != 0)
		{
			cSafeScript->CallTableFunction("Vip" , "AddDegree", 1, "osd", m_rcPlayer.GetScriptInterface(), szModuleName, pNewAttribute->nNum - pOldAttribute->nNum);
		}
	} while (pNewAttribute != NULL);

	return TRUE;
}


LuaPlayerVip* PlayerVip::GetScriptInterface() CONST
{
	return m_pLunaObj;
}

BOOL PlayerVip::Active()
{
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
	{
		goto EXIT0;
	}

	PROCESS_ERROR(m_SaveData.sFree.dwFreeTime > 0);

	--m_SaveData.sFree.dwFreeTime;

	// 体验时间结束
	if(m_SaveData.sFree.dwFreeTime <= 0)
	{
		// 设置回以前的vip等级
		m_rcPlayer.SetVipLevel(m_SaveData.sFree.byRecVipLevel);
	}

EXIT0:
	return TRUE;
}

BOOL PlayerVip::IsFreeExpState(INT nNewVipLevel, INT& nOldVipLevel)
{
	INT nTotalExp = 0;
	INT nVipLevel = m_rcPlayer.GetVipLevel();
	INT nVipNeedCoin = 0;
	BOOL bResult = FALSE;

	nOldVipLevel = m_SaveData.sFree.byRecVipLevel;

	if(nNewVipLevel < VIP_FREE_EXP_LEVEL)
	{
		nVipLevel = nNewVipLevel;
	}

	nVipNeedCoin = GetOverviewCoins(nVipLevel);

	nTotalExp = m_SaveData.nExp;

	// 有体验时间而且元宝数量不足升vip所需元宝数量，那么就是体验状态
	if(m_SaveData.sFree.dwFreeTime > 0 && nTotalExp < nVipNeedCoin)
	{
		bResult = TRUE;
	}
	
EXIT0:
	return bResult;

}

// 设置体验状态
VOID PlayerVip::SetFreeVipState(DWORD dwFreeTime /* = 0 */)
{
	INT nCurVipLevel = m_rcPlayer.GetVipLevel();
	// 已经开通了就不用体验了
	if(nCurVipLevel >= VIP_FREE_EXP_LEVEL)
	{
		return;
	}

	if(!dwFreeTime)
	{
		m_SaveData.sFree.dwFreeTime = VIP_FREE_EXP_TIME;
	}
	else
	{
		m_SaveData.sFree.dwFreeTime = dwFreeTime;
	}
	
	m_SaveData.sFree.byRecVipLevel = nCurVipLevel;

	m_rcPlayer.SetVipLevel(VIP_FREE_EXP_LEVEL);

	SyncFreeExpTime();
}

BOOL PlayerVip::SyncFreeExpTime()
{
	VIP_S2C_SYNC_EXP_TIME sSync;
	sSync.dwExpTime = m_SaveData.sFree.dwFreeTime;

	return g_VipModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL PlayerVip::GetOrSetMsg(INT nType,CHAR *szMsg)
{
	if(0 == nType) //get
	{
		VIP_S2C_SYNC_MSG sSync;
		memcpy(sSync.szMsg,m_SaveData.szMsg,sizeof(m_SaveData.szMsg));
		g_VipModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	}
	else if(1 == nType)//set
	{
		VIP_S2C_SYNC_MSG sSync;
		memcpy(m_SaveData.szMsg,szMsg,sizeof(m_SaveData.szMsg));
		memcpy(sSync.szMsg,m_SaveData.szMsg,sizeof(m_SaveData.szMsg));
		g_VipModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	}
	return TRUE;
}


VOID PlayerVip::ResetFreeState()
{
	ZeroMemory(&m_SaveData.sFree, sizeof(VipFreeExp));
}

VOID PlayerVip::SetRecFreeVipLevel(INT nVipLevel)
{
	m_SaveData.sFree.byRecVipLevel = nVipLevel;
}

VOID PlayerVip::DoGiveFirstRechargeReward()
{
	KAward *pAward = NULL;
	pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer,"LevelUpGift", 1181);
	LOG_PROCESS_ERROR(pAward);
EXIT0:
	return;
}