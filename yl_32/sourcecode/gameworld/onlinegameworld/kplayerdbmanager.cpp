/* -------------------------------------------------------------------------
//	文件名		：	kplayerdbmanager.cpp
//	创建者		：	simon
//	创建时间	：	2009/12/24 22:14:59
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#ifdef GAME_SERVER

#include "stdafx.h"
#include "kplayerdbmanager.h"
#include "config/kplayerbasedbdef.h"
#include "config/kplayerctrldbdef.h"

#include "onlinegameworld/kplayer.h"
#include "ProtocolBasic.h"
#include "onlinegameworld/koverloadextension.h"
#include "kgamecenterprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------


KPlayerDbManager	g_cPlayerDbMgr;
// -------------------------------------------------------------------------

BOOL KPlayerDbManager::Init()
{
	return TRUE;
}

BOOL KPlayerDbManager::Uninit()
{
	return TRUE;
}

BOOL KPlayerDbManager::OnPlayerDbLoad(KPlayer& rPlayer, const KROLE_DATA& rRoleData, SIZE_T uSize)
{
	Q_Printl("[" << rPlayer.m_dwId << "][" << rPlayer.GetName() << "], Loading Role Data...");
	return LoadRole(rPlayer, rRoleData, uSize);
}

BOOL KPlayerDbManager::OnPlayerDbSave( KPlayer& rPlayer, BOOL bLeaving )
{
	Q_Printl("[" << rPlayer.m_dwId << "][" << rPlayer.GetName() << "], Saving Role Data...");

	g_cGameCenterProcess.OnPlayerSave(rPlayer);

	time_t tmNow = KSysService::Time(NULL);
	rPlayer.RawSetOnlineTime(rPlayer.GetOnlineTime() + tmNow - rPlayer.m_cPlayerServer.m_timeLastSave);
	rPlayer.m_cPlayerServer.m_timeLastSave = tmNow;
	if (bLeaving)
		rPlayer.RawSetLastLogoutTime(tmNow); //设置logout时间

	return TRUE;
}

BOOL KPlayerDbManager::LoadRole( KPlayer& rPlayer, const KROLE_DATA& rRoleData, SIZE_T uSize )
{
	BOOL bRet = FALSE;
	SIZE_T uContentSize = 0;
	LPBYTE pBlockContent = NULL;
	// CheckRoleData已经检查过大小和CRC了，不再重复检查

	DB_PLAYER_BASE_DATA* pBaseInfo = GET_DB_PLAYER_BASE_DATA(&rRoleData);
	QCONFIRM_RET_FALSE(pBaseInfo);
	const KDB_ROLE_BLOCK_HEADER* pBaseBlockHeader = GET_ROLE_BLOCK_HEADER(&rRoleData, emKDRBT_BASE_INFO);
	QCONFIRM_RET_FALSE(pBaseBlockHeader);

	// 载入基础信息
	//bRet = LoadBaseInfo(rPlayer, *pBaseInfo, pBaseBlockHeader->wDataLen); 
	//QCONFIRM_RET_FALSE(bRet);

	// 指向RoleBaseInfo后面的存盘块
	pBlockContent = LPBYTE(pBaseInfo) + pBaseBlockHeader->wDataLen;

	{
		// 载入扩展模块
		for (INT i = emKDRBT_EXTENSION_START; i <= rRoleData.wBlockCount; ++i)
		{
			const KDB_ROLE_BLOCK_HEADER& rBlockHeader = rRoleData.BlockList[i - 1];

			const KPLAYER_SAVE_DEF* pCtrlDef = 
				GetPlayerSaveCtrl(rPlayer, (KE_DB_ROLE_BLOCK_TYPE)rBlockHeader.byBlockType);
			// 先看看有没有PlayerCtrl管这块数据
			if (pCtrlDef)
			{
				KCharacterCtrlBase* pCtrl = rPlayer.GetCtrl(pCtrlDef->nCtrlId);
				QCONFIRM_RET_FALSE(pCtrl);

				//if (rBlockHeader.wDataLen > 0)
				//{
					bRet = pCtrl->OnDbLoad(pBlockContent, rBlockHeader.wDataLen);
					QCONFIRM_RET_FALSE(bRet);
				//}
			}
			if ( ! bRet)
				Q_Printl("Unhandled Db DataBlock[" << (INT)rBlockHeader.byBlockType << "] On Loading!");

			uContentSize += rBlockHeader.wDataLen;
			pBlockContent += rBlockHeader.wDataLen;

			QCONFIRM_RET_FALSE(rRoleData.dwDataLen - pBaseBlockHeader->wDataLen >=
				uContentSize + sizeof(KDB_ROLE_BLOCK_HEADER) * rRoleData.wBlockCount);
		}

		QCONFIRM_RET_FALSE(rRoleData.dwDataLen - pBaseBlockHeader->wDataLen ==
			uContentSize + sizeof(KDB_ROLE_BLOCK_HEADER) * rRoleData.wBlockCount);
	}

	if (rPlayer.GetCurrentLife() <= 0)
		rPlayer.SetCurrentLife(1);

	bRet = TRUE;
//EXIT0:
	return bRet;
}

BOOL KPlayerDbManager::SaveRole( KPlayer& rPlayer, KROLE_DATA& rRoleData, SIZE_T uSize, SIZE_T& ruUsedSize )
{
	BOOL bRet = FALSE;
	SIZE_T uBlockHeaderSize = sizeof(KDB_ROLE_BLOCK_HEADER) * (emKDRBT_TOTAL - 1);
	SIZE_T uBlockContentSize = 0;
	LPBYTE pBlockContent = NULL;
	ruUsedSize = 0;

	time_t tmNow = KSysService::Time(NULL);
	rPlayer.RawSetOnlineTime(rPlayer.GetOnlineTime() + tmNow - rPlayer.m_cPlayerServer.m_timeLastSave);
	rPlayer.m_cPlayerServer.m_timeLastSave = tmNow;

	rPlayer.RawSetLastLogoutTime(tmNow); // 跨场景一次，更新一次最后在线时间

	DB_PLAYER_BASE_DATA* pBaseInfo = (DB_PLAYER_BASE_DATA*)&rRoleData.BlockList[emKDRBT_TOTAL - 1];
	QCONFIRM_RET_FALSE(pBaseInfo);
	KDB_ROLE_BLOCK_HEADER* pBlockHeader = GET_ROLE_BLOCK_HEADER(&rRoleData, emKDRBT_BASE_INFO);
	QCONFIRM_RET_FALSE(pBlockHeader);

	// 存储基础信息
	uBlockContentSize = SaveBaseInfo(rPlayer, *pBaseInfo, uSize - sizeof(KROLE_DATA) - uBlockHeaderSize);
	QCONFIRM_RET_FALSE(uBlockContentSize > sizeof(DB_PLAYER_BASE_DATA));
	// 存储BaseInfo块大小
	pBlockHeader->byBlockType = emKDRBT_BASE_INFO;
	pBlockHeader->byFlag = 0;
	pBlockHeader->wDataLen = (WORD)uBlockContentSize;

	// 指向RoleBaseInfo后面的存盘块
	pBlockContent = LPBYTE(pBaseInfo) + uBlockContentSize;

	// 存储扩展模块
	for (INT i = emKDRBT_EXTENSION_START; i < emKDRBT_TOTAL; ++i)
	{
		SIZE_T uSizeUsed = 0;
		KDB_ROLE_BLOCK_HEADER& rBlockHeader = rRoleData.BlockList[i - 1];

		// 一定要先填好BlockHeader
		rBlockHeader.wDataLen = 0;
		rBlockHeader.byBlockType = i;
		rBlockHeader.byFlag = 0;

		const KPLAYER_SAVE_DEF* pCtrlDef = GetPlayerSaveCtrl(rPlayer, (KE_DB_ROLE_BLOCK_TYPE)i);
		// 先看看有没有PlayerCtrl管这块数据
		if (pCtrlDef)
		{
			KCharacterCtrlBase* pCtrl = rPlayer.GetCtrl(pCtrlDef->nCtrlId);
			QCONFIRM_CONTINUE(pCtrl);

			bRet = pCtrl->OnDbSave(pBlockContent,
				uSize - ( (SIZE_T)&rRoleData - (SIZE_T)pBlockContent ),
				uSizeUsed);
			QCONFIRM_CONTINUE(bRet); // TODO：保存失败要有额外处理
		}
		if ( ! bRet)
			Q_Printl("Unhandled Db DataBlock[" << i << "] On Saving!");

		rBlockHeader.wDataLen = (WORD)uSizeUsed;

		uBlockContentSize += uSizeUsed;
		pBlockContent += uSizeUsed;

		QCONFIRM_BREAK(uSize >= uBlockHeaderSize + uBlockContentSize + sizeof(KROLE_DATA));
	}

	rRoleData.dwDataLen = uBlockHeaderSize + uBlockContentSize;
	rRoleData.wVersion = emKROLE_DATA_VERSION_CURRENT;
	rRoleData.wBlockCount = emKDRBT_TOTAL - 1; // 减去emKDRBT_NONE
	rRoleData.wFirstTimeLogin = (WORD)FALSE;
	//rRoleData.dwCRC = Misc_CRC32(0, rRoleData.BlockList, rRoleData.dwDataLen);
	rRoleData.dwCRC = ngx_crc32_long((unsigned char*)rRoleData.BlockList, rRoleData.dwDataLen);

	ruUsedSize = rRoleData.dwDataLen + sizeof(KROLE_DATA);

	bRet = TRUE;
//EXIT0:
	return bRet;
}

const KPLAYER_SAVE_DEF* KPlayerDbManager::GetPlayerSaveCtrl( KPlayer& rPlayer, KE_DB_ROLE_BLOCK_TYPE eBlockType )
{
	const KPLAYER_SAVE_DEF* pCtrlDef = NULL;

	for (UINT i = 0; i < countof(gs_asPlayerSaveDef); ++i)
	{
		const KPLAYER_SAVE_DEF& rSaveDef = gs_asPlayerSaveDef[i];
		if (eBlockType == rSaveDef.eBlockType)
		{
			pCtrlDef = &rSaveDef;
			break;
		}
	}

	return pCtrlDef;
}

BOOL KPlayerDbManager::CheckRoleData( const KROLE_DATA* pRoleData, SIZE_T uDataSize )
{
	BOOL bRet = FALSE;
	SIZE_T uContentSize = 0;
	DWORD dwCRC = 0;
	LPBYTE pBlockContent = NULL;

	PROCESS_ERROR(pRoleData && pRoleData->dwDataLen > 0);
	PROCESS_ERROR(uDataSize == pRoleData->dwDataLen + sizeof(KROLE_DATA));

	if (pRoleData->wVersion != emKROLE_DATA_VERSION_CURRENT)
	{
		if (pRoleData->wVersion == emKROLE_DATA_VERSION_PREVIOUS)
		{
			QLogPrintf(LOG_INFO, "[KPlayerDbManager] Previous version of role data found.");
			// role data version conversion here
		}
		Q_Printl("[KPlayerDbManager] Incompatible role data. 角色数据版本[" << pRoleData->wVersion << "]与当前版本["
			<< emKROLE_DATA_VERSION_CURRENT << "]不兼容，需要清档!");
		return FALSE;
	}

	// 检查CRC校验码
	//dwCRC = Misc_CRC32(dwCRC, pRoleData->BlockList, pRoleData->dwDataLen);
	dwCRC = ngx_crc32_long((unsigned char*)pRoleData->BlockList, pRoleData->dwDataLen);
	PROCESS_ERROR(dwCRC == pRoleData->dwCRC);

	// 因为blocktype从1开始，所以emKDRBT_TOTAL - 1的块实际上指向BaseInfo数据块，跳过了BlockHeader
	pBlockContent = (LPBYTE)GET_DB_PLAYER_BASE_DATA(pRoleData);

	// 是否需要检查BaseInfo完整性？

	for (INT i = 1; i <= pRoleData->wBlockCount; ++i)
	{
		const KDB_ROLE_BLOCK_HEADER& rBlockHeader = pRoleData->BlockList[i - 1];

		uContentSize += rBlockHeader.wDataLen;
		pBlockContent += rBlockHeader.wDataLen;

		PROCESS_ERROR(pRoleData->dwDataLen >=
			uContentSize + sizeof(KDB_ROLE_BLOCK_HEADER) * pRoleData->wBlockCount);
	}

	PROCESS_ERROR(pRoleData->dwDataLen ==
		uContentSize + sizeof(KDB_ROLE_BLOCK_HEADER) * pRoleData->wBlockCount);

	bRet = TRUE;
EXIT0:
	return bRet;
}

SIZE_T KPlayerDbManager::SaveBaseInfo( KPlayer& rPlayer, DB_PLAYER_BASE_DATA& rBaseInfo, SIZE_T uSize )
{
	QCONFIRM_RET_FALSE(uSize > sizeof(DB_PLAYER_BASE_DATA));

	SIZE_T uUsedSize = sizeof(DB_PLAYER_BASE_DATA);
	IKScene* pScene = rPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene); // 找不到指定场景？

	//// 取得存档点
	//rBaseInfo.CurrentPos.nMapCopyIndex = pScene->GetId();
	//rPlayer.GetPosition(rBaseInfo.CurrentPos.nX, rBaseInfo.CurrentPos.nY, rBaseInfo.CurrentPos.nZ);
	//rBaseInfo.CurrentPos.byFaceDirection = (BYTE)rPlayer.m_nFaceDirection;
	//// 上次登录位置
	//rPlayer.m_cPlayerServer.GetLastEntryPosition(rBaseInfo.LastEntry);

	//if (IS_DYNAMIC_SCENE(pScene->GetId()))
	//	rBaseInfo.CurrentPos = rBaseInfo.LastEntry;

	//rBaseInfo.bySex = rPlayer.GetSex();
	//rBaseInfo.byLevel = rPlayer.GetLevel();
	ZeroStruct(rBaseInfo.abyReserved);

	time_t tmNow = KSysService::Time(NULL);
	//rBaseInfo.nTotalGameTime = rPlayer.GetOnlineTime();
	//rBaseInfo.nLastSaveTime = tmNow;
	//rBaseInfo.nLastLoginTime = rPlayer.m_cPlayerServer.m_timeLogin;
	rBaseInfo.dwLastLoginIp = rPlayer.m_cPlayerServer.m_dwLoginIp;
	//rBaseInfo.nCreateTime = rPlayer.m_cPlayerServer.m_dwCreateTime;
	rBaseInfo.dwCoin = rPlayer.m_cPlayerServer.GetAccountCoin();
	QStrCpyLen(rBaseInfo.szAccount, rPlayer.m_cPlayerServer.GetAccount(), sizeof(rBaseInfo.szAccount));
	QStrCpyLen(rBaseInfo.szRole, rPlayer.GetName(), sizeof(rBaseInfo.szRole));

	// save task var
	SIZE_T uTaskVarSize = rPlayer.m_cPlayerServer.SaveTaskVar(rBaseInfo.abyTaskVar, uSize - uUsedSize);
	rBaseInfo.uTaskVarSize = uTaskVarSize;
	if (uTaskVarSize > 0)
		uUsedSize += uTaskVarSize;

	return uUsedSize;
}

BOOL KPlayerDbManager::LoadBaseInfo2(KPlayer& rPlayer, const DB_PLAYER_BASE_DATA* pBaseInfo, SIZE_T uSize)
{
	BOOL bRet = FALSE;

	// 取得存档点
	INT nX = rPlayer.m_cPlayerServer.m_SavePosition.nX;
	INT nY = rPlayer.m_cPlayerServer.m_SavePosition.nY;
	INT nZ = rPlayer.m_cPlayerServer.m_SavePosition.nZ;

	IKScene* pScene = g_pSceneMgr->GetScene(rPlayer.m_cPlayerServer.m_SavePosition.dwMapID,
		rPlayer.m_cPlayerServer.m_SavePosition.nMapCopyIndex);
	QCONFIRM_RET_FALSE(pScene); // 找不到指定场景？拒绝登录

	rPlayer.m_nFaceDirection = rPlayer.m_cPlayerServer.m_SavePosition.nFaceDirection;
	rPlayer.m_nDirectionXY = rPlayer.m_nFaceDirection;
	bRet = pScene->AddPlayer(&rPlayer, nX, nY, nZ);
	QCONFIRM_RET_FALSE(bRet);

	// load task var, 新角色TaskVar为空
	if (pBaseInfo && pBaseInfo->uTaskVarSize > 0)
	{
		bRet = rPlayer.m_cPlayerServer.LoadTaskVar(pBaseInfo->abyTaskVar, pBaseInfo->uTaskVarSize);
		QCONFIRM_RET_FALSE(bRet);
	}

	return bRet;
}

#endif // GAME_SERVER
