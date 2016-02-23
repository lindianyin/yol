#include "stdafx.h"
#include "krankctrl.h"
#include "onlinegameworld/kplayer.h"
#include "krankprotocol.h"
#include "krankmodule.h"
#include "rank/RankRelayClient.h"
#include "krankmanager.h"
#include "onlinegamebase/ksysservice.h"

KRankCtrl::KRankCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KRankCtrl::~KRankCtrl()
{

}

BOOL KRankCtrl::Init()
{
	InitLastTakeAwardTime();

	return TRUE;
}

BOOL KRankCtrl::OnLogin()
{
	return TRUE;
}

BOOL KRankCtrl::Active()
{
	return TRUE;
}

INT KRankCtrl::GetRoleRankNum()
{
	INT nNum = 0;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetRoleRankNum();

	for(INT i = 0; i < nRankNum; ++i)
	{
		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrRoleRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			continue;
//		}
		++nNum;
	}

	return nNum;
}

//同步"个人排行"的"战斗力榜"
BOOL KRankCtrl::SyncRoleRanks()
{
	INT nIdx = 0;
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_ROLE_RANK* pSync = NULL;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetRoleRankNum();
	INT nSyncSize = GetRoleRankNum();
	IMemBlock* piMemSync = NULL;
	KS2C_SYNC_END* pSyncEnd = NULL;

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_ROLE_RANK) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);
	pSync = (KS2C_SYNC_ROLE_RANK*)piMem->GetData();

	//构建同步结束数据包
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_ROLE_FIGHTSCORE;
	pSyncEnd->byProtocol = s2c_sync_end;
	pSync->byProtocol = s2c_sync_role_rank;


	//自己的排行
	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_ROLE_FIGHTSCORE, m_rcPlayer.GetId());
	for(INT i = 0; i < nRankNum; ++i)
	{
		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrRoleRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrRoleRanks[i].szRoleName, _NAME_LEN) == 0)
//			{
//				pSync->nMeRank = -1;
//			}
//			continue;
//		}

		//根据名字判断获得自己的排名
		if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrRoleRanks[i].szRoleName, _NAME_LEN) == 0)
		{
			pSync->nMeRank = nIdx;
		}

		//过滤不符合条件的条目
		if(g_cRankManager.m_arrRoleRanks[i].byLevel == 0 
			|| g_cRankManager.m_arrRoleRanks[i].szRoleName[0] == '\0')
		{
			continue;
		}
		nIdx++;
		pSync->nRoleRankNum = 1;
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrRoleRanks[i]), sizeof(KRoleBaseInfo));
		//发送排行榜里面的条目
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	//发送结束标志
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

INT KRankCtrl::GetPetLvRankNum()
{
	INT nNum = 0;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetPetLvRankNum();

	for(INT i = 0; i < nRankNum; ++i)
	{
		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrPetLvRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			continue;
//		}
		++nNum;
	}
	
	return nNum;
}

BOOL KRankCtrl::SyncPetLvRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_PET_LV_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetPetLvRankNum();
	INT nSyncSize = GetPetLvRankNum();
	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	time_t 			tNow 		= KSysService::Time(NULL);
	INT				nIdx	 	= 0;
	
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_PET_LEVEL;
	pSyncEnd->byProtocol = s2c_sync_end;
		
	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_PET_LV_RANK) + sizeof(KPetRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_PET_LV_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_pet_lv_rank;
	pSync->nMeRank = -1;
	pSync->nPetRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		if(g_cRankManager.m_arrPetLvRanks[i].byLevel <= 0)
		{
			continue;
		}

		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrPetLvRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrPetLvRanks[i].szOwnerName, _NAME_LEN) == 0)
//			{
//				pSync->nMeRank = -1;
//			}
//
//			continue;
//		}

		if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrPetLvRanks[i].szOwnerName, _NAME_LEN) == 0)
		{
			pSync->nMeRank = nIdx;
		}
		nIdx ++;
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrPetLvRanks[i]), sizeof(KPetRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

INT KRankCtrl::GetPetFSRankNum()
{
	INT nNum = 0;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetPetFSRankNum();

//	for(INT i = 0; i < nRankNum; ++i)
//	{
//		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrPetFSRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			continue;
//		}
//		++nNum;
//	}

	return nNum;
}

BOOL KRankCtrl::SyncPetFSRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_PET_FS_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetPetFSRankNum();
	INT nSyncSize = GetPetFSRankNum();
	time_t tNow = KSysService::Time(NULL);
	INT nIdx = 0;
	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;

	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_PET_FIGHTSCORE;
	pSyncEnd->byProtocol = s2c_sync_end;
	
	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_PET_FS_RANK) + sizeof(KPetRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_PET_FS_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);


	pSync->byProtocol = s2c_sync_pet_fightscore_rank;

	pSync->nMeRank = -1;
	pSync->nPetRankNum = 1;
	
	
	for(INT i = 0; i < nRankNum; ++i)
	{
		if(g_cRankManager.m_arrPetFSRanks[i].nFightScore <= 0)
		{
			continue;
		}

		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrPetFSRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrPetFSRanks[i].szOwnerName, _NAME_LEN) == 0)
//			{
//				pSync->nMeRank = -1;
//			}
//
//			continue;
//		}

		if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrPetFSRanks[i].szOwnerName, _NAME_LEN) == 0)
		{
			pSync->nMeRank = nIdx;
		}
		nIdx++;
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrPetFSRanks[i]), sizeof(KPetRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

INT KRankCtrl::GetRideRankNum()
{
	INT nNum = 0;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetRideRankNum();

	for(INT i = 0; i < nRankNum; ++i)
	{
		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrRideRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			continue;
//		}
		++nNum;
	}

	return nNum;
}

BOOL KRankCtrl::SyncRideRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_RIDE_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetRideRankNum();
	INT nSyncSize = GetRideRankNum();
	time_t tNow = KSysService::Time(NULL);
	INT nIdx = 0;
	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_RIDE_FIGHTSCORE;
	pSyncEnd->byProtocol = s2c_sync_end;

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_RIDE_RANK) + sizeof(KRideRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_RIDE_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_ride_rank;

	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_RIDE_FIGHTSCORE, m_rcPlayer.GetId());
	pSync->nRideRankNum = 1;

	for(INT i = 0; i < nRankNum; ++i)
	{
		// 7天没登陆不上榜单
//		if(tNow - g_cRankManager.m_arrRideRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
//		{
//			if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrRideRanks[i].szOwnerName, _NAME_LEN) == 0)
//			{
//				pSync->nMeRank = -1;
//			}
//			continue;
//		}

		if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrRideRanks[i].szOwnerName, _NAME_LEN) == 0)
		{
			pSync->nMeRank = nIdx;
		}
		nIdx ++;
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrRideRanks[i]), sizeof(KRideRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncMpjjRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_MPJJ_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetMpjjRankNum();

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_MPJJ;
	pSyncEnd->byProtocol = s2c_sync_end;
	
	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_MPJJ_RANK) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_MPJJ_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_mpjj_rank;
	pSync->nRoleRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrMpjjRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncLgRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_LG_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetLGRankNum();

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_LG;
	pSyncEnd->byProtocol = s2c_sync_end;

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_LG_RANK) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_LG_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_lg_rank;

	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_LG, m_rcPlayer.GetId());
	pSync->nRoleRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		if(g_cRankManager.m_arrLGRanks[i].dwExValue / 10  <= 0)
		{
			continue;
		}
		Q_Printl(".. g_cRankManager.m_arrLGRanks[i].dwExValue = " << g_cRankManager.m_arrLGRanks[i].dwExValue);
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrLGRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncSmzcRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_SMZC_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetSmzcRankNum();

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_SMZC;
	pSyncEnd->byProtocol = s2c_sync_end;


	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_SMZC_RANK) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_SMZC_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_smzc_rank;

	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_SMZC, m_rcPlayer.GetId());
	pSync->nRoleRankNum = 1;

	for(INT i = 0; i < nRankNum; ++i)
	{
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrSmzcRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncTongFSRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_TONG_FS_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetTongFsRankNum();

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_TONG_FIGHTSCORE;
	pSyncEnd->byProtocol = s2c_sync_end;


	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_TONG_FS_RANK) + sizeof(KTongRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_TONG_FS_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_tong_fs_rank;

	pSync->nMeRank = g_cRankManager.GetTongRank(emRANK_TONG_FIGHTSCORE, m_rcPlayer.GetKinId());
	pSync->nTongRankNum = 1;

	for (INT i=0;i < nRankNum;++i)
	{
		if(g_cRankManager.m_arrTongSrRanks[i].szTongName[0] == '\0')
		{
			continue;
		}
		
		if(g_cRankManager.m_arrTongSrRanks[i].nMemberNum == 0)
		{
			continue;
		}
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrTongFSRanks[i]), sizeof(KTongRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncTongSrRanks()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_TONG_SR_RANK* pSync = NULL;
	INT nRankNum = g_cRankManager.GetTongSrRankNum();

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_TONG_SALARY;
	pSyncEnd->byProtocol = s2c_sync_end;

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_TONG_SR_RANK) + sizeof(KTongRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_TONG_SR_RANK*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_tong_sr_rank;

	pSync->nMeRank = g_cRankManager.GetTongRank(emRANK_TONG_SALARY, m_rcPlayer.GetKinId());
	pSync->nTongRankNum = 1;

	for (INT i=0;i < nRankNum;++i)
	{
		if(g_cRankManager.m_arrTongSrRanks[i].szTongName[0] == '\0')
		{
			continue;
		}
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrTongSrRanks[i]), sizeof(KTongRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

VOID KRankCtrl::InitLastTakeAwardTime()
{
	for(INT nRankId = emRANK_ROLE_FIGHTSCORE; nRankId <= emRANK_SMZC; ++nRankId)
	{
		m_mapLastTakeAwardTime[nRankId] = 0;
	}
}

BOOL KRankCtrl::SetLastTakeAwardTime(INT nRankId, DWORD dwTime)
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(nRankId >= emRANK_ROLE_FIGHTSCORE && nRankId <= emRANK_SMZC);

	m_mapLastTakeAwardTime[nRankId] = dwTime;

	bResult = TRUE;
EXIT0:
	return bResult;
}

DWORD KRankCtrl::GetLastTakeAwardTime(INT nRankId)
{
	DWORD dwTime = 0;
	std::map<INT, DWORD>::iterator it;
	LOG_PROCESS_ERROR(nRankId >= emRANK_ROLE_FIGHTSCORE && nRankId <= emRANK_SMZC);

	it = m_mapLastTakeAwardTime.find(nRankId);
	LOG_PROCESS_ERROR(it != m_mapLastTakeAwardTime.end());
	
	dwTime = it->second;
EXIT0:
	return dwTime;
}

BOOL KRankCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	BOOL bResult = FALSE;
	std::map<INT, DWORD>::iterator it;
	for(INT nRankId = emRANK_ROLE_FIGHTSCORE; nRankId <= emRANK_SMZC; ++nRankId)
	{
		it = m_mapLastTakeAwardTime.find(nRankId);
		LOG_PROCESS_ERROR(it != m_mapLastTakeAwardTime.end());

		memcpy(pBuffer, &(it->second), sizeof(DWORD));

		pBuffer += sizeof(DWORD);
		uUsedSize += sizeof(DWORD);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;
	INT nRankId = emRANK_ROLE_FIGHTSCORE;
	LPCBYTE pBuffer = pData;
	
	while(uSize > 0)
	{
		DWORD dwTakeTime = *((DWORD*)pBuffer);
		m_mapLastTakeAwardTime[nRankId] = dwTakeTime;
		uSize -= sizeof(DWORD);
		pBuffer += sizeof(DWORD);
		++nRankId;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankCtrl::SyncLastUpdateTime(INT nRankId)
{
	KS2C_SYNC_LASTUPDATETIME sSync;

	sSync.nRankId = nRankId;
	sSync.dwLastUpdateTime = g_cRankManager.GetLastUpdateTime(nRankId);

	return g_cRankModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

//同步"全民冲级"活动
BOOL KRankCtrl::SyncRankRoleLvOfOpenServer()
{
	INT nIdx = 0;
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_PLAYER_LEVEL_OPEN_SERVICE* pSync = NULL;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetPlayerLevelNum();

	if (nRankNum > emRANK_MAX_COUNT_OPEN_SERVICE)
	{
		nRankNum = emRANK_MAX_COUNT_OPEN_SERVICE;
	}


	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_ROLE_LEVEL_OPEN_SERVER;
	pSyncEnd->byProtocol = s2c_sync_end;

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_PLAYER_LEVEL_OPEN_SERVICE) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_PLAYER_LEVEL_OPEN_SERVICE*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_player_level_open_server;
	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_ROLE_LEVEL_OPEN_SERVER, m_rcPlayer.GetId());
	pSync->nRoleRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		if (g_cRankManager.m_arrRoleLevelRanks[i].byLevel < 45)
		{
			continue;
		}

		if(g_cRankManager.m_arrRoleLevelRanks[i].szRoleName[0] == '\0' 
			||g_cRankManager.m_arrRoleLevelRanks[i].byLevel == 0)
		{
			continue;
		}
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrRoleLevelRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncRankRideLV()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_RIDE_LV_OS* pSync = NULL;
	INT nRankNum = g_cRankManager.GetRideLVRanksSaveNum();
	if (nRankNum > emRANK_MAX_COUNT_OPEN_SERVICE)
	{
		nRankNum = emRANK_MAX_COUNT_OPEN_SERVICE;
	}
	
	//INT nSyncSize = GetRideRankNum();
	time_t tNow = KSysService::Time(NULL);
	INT nIdx = 0;

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId = emRANK_RIDE_LV;
	pSyncEnd->byProtocol = s2c_sync_end;

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_RIDE_LV_OS) + sizeof(KRideRankInfo));
	LOG_PROCESS_ERROR(piMem);


	pSync = (KS2C_SYNC_RIDE_LV_OS*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_ride_lv_open_server;

	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_RIDE_LV, m_rcPlayer.GetId());
	pSync->nRideRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		if (g_cRankManager.m_arrRideLvRanks[i].byLevel <= 3)//4阶以内不上榜
		{
			continue;
		}
		if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrRideLvRanks[i].szOwnerName, _NAME_LEN) == 0)
		{
			pSync->nMeRank = nIdx;
		}
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrRideLvRanks[i]), sizeof(KRideRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncRankArean()
{
	INT nIdx = 0;
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_AREAN_OS* pSync = NULL;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetAreanRanksSaveNum();

	if (nRankNum > emRANK_MAX_COUNT_OPEN_SERVICE)
	{
		nRankNum = emRANK_MAX_COUNT_OPEN_SERVICE;
	}

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId 		= emRANK_AREAN_OS;
	pSyncEnd->byProtocol	= s2c_sync_end;

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_AREAN_OS) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_AREAN_OS*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_arean_open_server;
	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_AREAN_OS, m_rcPlayer.GetId());
	pSync->nRoleRankNum = 1;

	for(INT i = 0; i < nRankNum; ++i)
	{

		if (g_cRankManager.m_arrAreanSaveRanks[i].byLevel < 45)//45级内不上榜
		{
			continue;
		}
		if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrAreanSaveRanks[i].szRoleName, _NAME_LEN) == 0)
		{
			pSync->nMeRank = nIdx;
		}
		if(g_cRankManager.m_arrAreanSaveRanks[i].szRoleName[0] == '\0')
		{
			continue;
		}
		
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrAreanSaveRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncPetLV()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_PET_LV_OS* pSync = NULL;
	INT nRankNum = g_cRankManager.GetPetLVSaveNum();

	if (nRankNum > emRANK_MAX_COUNT_OPEN_SERVICE)
	{
		nRankNum = emRANK_MAX_COUNT_OPEN_SERVICE;
	}

	time_t tNow = KSysService::Time(NULL);
	INT nIdx = 0;

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId 		= emRANK_PET_LV_OS;
	pSyncEnd->byProtocol	= s2c_sync_end;


	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_PET_LV_OS) + sizeof(KPetRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_PET_LV_OS*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_pet_lv_open_server;

	pSync->nMeRank = -1;
	pSync->nPetRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		if(g_cRankManager.m_arrPetLVSaveRanks[i].nFightScore <= 8000)
		{
			continue;
		}
		if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrPetLVSaveRanks[i].szOwnerName, _NAME_LEN) == 0)
		{
			pSync->nMeRank = nIdx;
		}
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrPetLVSaveRanks[i]), sizeof(KPetRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncLGRank_OS()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_LG_OS* pSync = NULL;
	INT nRankNum = g_cRankManager.GetLGSaveNum();


	if (nRankNum > emRANK_MAX_COUNT_OPEN_SERVICE)
	{
		nRankNum = emRANK_MAX_COUNT_OPEN_SERVICE;
	}
	INT nIdx = 0;

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId 		= emRANK_LG_OS;
	pSyncEnd->byProtocol	= s2c_sync_end;

	
	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_LG_OS) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_LG_OS*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_lg_open_server;

	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_LG_OS, m_rcPlayer.GetId());
	pSync->nRoleRankNum = nRankNum;
	pSync->nRoleRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		if(g_cRankManager.m_arrLGSaveRanks[i].dwExValue/10 < 8)
		{
			continue;
		}

		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrLGSaveRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;	
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}


//斩妖伐魔
BOOL KRankCtrl::SyncRankTower()
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_TOWER_OPEN_SERVICE* pSync = NULL;
	INT nRankNum = g_cRankManager.GetTowerkNum();
	
	if (nRankNum > emRANK_MAX_COUNT_OPEN_SERVICE)
	{
		nRankNum = emRANK_MAX_COUNT_OPEN_SERVICE;
	}

	INT nIdx = 0;

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId 		= emRANK_TOWER;
	pSyncEnd->byProtocol	= s2c_sync_end;	
	
	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_TOWER_OPEN_SERVICE) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_TOWER_OPEN_SERVICE*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_sync_tower_open_server;

	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_TOWER, m_rcPlayer.GetId());
	pSync->nRoleRankNum = nRankNum;
	pSync->nRoleRankNum = 1;

	for(INT i = 0; i < nRankNum; ++i)
	{
		if(g_cRankManager.m_arrTowerRanks[i].dwExValue < 30)
		{
			continue;
		}

		if(g_cRankManager.m_arrTowerRanks[i].szRoleName[0] == '\0')
		{
			continue;
		}
		
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrTowerRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;	
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}

BOOL KRankCtrl::SyncRoleFightScore_OS()
{
	INT nIdx = 0;
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_ROLE_FIGHTSCORE_OS* pSync = NULL;
	time_t tNow = KSysService::Time(NULL);
	INT nRankNum = g_cRankManager.GetRoleRankNum();

	if (nRankNum > emRANK_MAX_COUNT_OPEN_SERVICE)
	{
		nRankNum = emRANK_MAX_COUNT_OPEN_SERVICE;
	}

	INT nSyncSize = GetRoleRankNum();

	IMemBlock* 		piMemSync 	= NULL;
	KS2C_SYNC_END* 	pSyncEnd 	= NULL;
	piMemSync = QCreateMemBlock(sizeof(KS2C_SYNC_END));
	LOG_PROCESS_ERROR(piMemSync);
	pSyncEnd = (KS2C_SYNC_END*)piMemSync->GetData();
	pSyncEnd->nRankId 		= emRANK_ROLE_FIGSHTSCORE_OS;
	pSyncEnd->byProtocol	= s2c_sync_end;	


	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_ROLE_FIGHTSCORE_OS) + sizeof(KRoleRankInfo));
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_ROLE_FIGHTSCORE_OS*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = s2c_role_fightscore_open_server;
	pSync->nMeRank = g_cRankManager.GetPlayerRank(emRANK_ROLE_FIGSHTSCORE_OS, m_rcPlayer.GetId());
	pSync->nRoleRankNum = 1;
	for(INT i = 0; i < nRankNum; ++i)
	{
		// 7天没登陆不上榜单
		if(tNow - g_cRankManager.m_arrRoleRanks[i].dwLastModifyTime >= RANK_OUTDATE_TIME)
		{
			if(strncmp(m_rcPlayer.GetName(), g_cRankManager.m_arrRoleRanks[i].szRoleName, _NAME_LEN) == 0)
			{
				pSync->nMeRank = -1;
			}
			continue;
		}
		
		if (g_cRankManager.m_arrRoleFightSaveRanks[i].nFightScore < 30000)
		{
			break;
		}
		
		memcpy(&(pSync->arrRanks[0]), &(g_cRankManager.m_arrRoleFightSaveRanks[i]), sizeof(KRoleRankInfo));
		bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = g_cRankModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, piMemSync->GetSize());
	LOG_PROCESS_ERROR(bResult);
	bResult = TRUE;	
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemSync);
	return bResult;
}
