#include "stdafx.h"

#include "kitemmgr.h"
#include "kdupeditemdeal.h"
#include "onlinegameworld/kplayermgr.h"

#include <map>
#include <string>
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#ifdef GAME_SERVER

// 系统安全警告Log
QLogFile	g_SysAlarmLogFile;

KDupedItemSetting g_dupedItemSetting;
///////////////////////////////////////////////////////////////////////////////
BOOL gInitSysAlarmDeal()
{
	g_dupedItemSetting.Init();

	g_SysAlarmLogFile.InitialLogFile(F_LOG_PATH_ROOT, "sysalarm.log");

	return TRUE;
}

namespace DupedDef
{
	QLogFile& GetDupedLogFile()
	{
		return g_SysAlarmLogFile;
	}

	LPVOID GetItemFromItemSet(INT nItemIdx)
	{
		if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
			return NULL;

		return ((LPVOID)&Item[nItemIdx]);
	}

	BOOL RemoveItemFromItemSet(INT nPlayerIdx, INT nItemIdx, BOOL bEraseInMap)
	{
		if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
			return FALSE;

		g_cItemMgr.Remove(nItemIdx, emITEM_DATARECORD_NORMAL, emITEM_CONSUMEMODE_DUPEDITEM, g_cPlayerMgr.GetByIndex(nPlayerIdx), bEraseInMap);
		return TRUE;
	}
	BOOL RemoveItemFromPlayer(INT nPlayerIdx, INT nItemIdx)
	{
		if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
			return FALSE;

		KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
		QCONFIRM_RET_FALSE(pPlayer);

		return pPlayer->GetCtrl<KPlayerItem>()->Remove(nItemIdx, emKLOSEITEM_DUP);
	}

	LPCSTR GetPlayerAccount(INT nPlayerIdx)
	{
		KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
		QCONFIRM_RET_FALSE(pPlayer);

		return pPlayer->m_cPlayerServer.GetAccount();
	}
	LPCSTR GetPlayerName(INT nPlayerIdx)
	{
		KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
		QCONFIRM_RET_FALSE(pPlayer);

		return pPlayer->GetName();
	}
	BOOL CheckPlayerTrading(INT nPlayerIdx)
	{
		return FALSE;
	}
	// ---------------------------------------------------------
	BOOL SendMsgForMark(INT nPlayerIndex, LPCSTR szItemName)
	{
		return FALSE;
	}

	BOOL SendMsgForDel(INT nPlayerIndex, LPCSTR szItemName)
	{
		return FALSE;
	}
};

#endif // #ifdef GAME_SERVER
