
#include "stdafx.h"
#include "kcdtime.h"
#include "kitemmgr.h"
#include "onlinegameworld/korpgworld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

VOID KCDTime::Init(VOID)
{
	KItemSetting::KCdTypeMap::iterator i = g_cItemMgr.m_cSetting.m_mapCdType.begin();
	for (; i != g_cItemMgr.m_cSetting.m_mapCdType.end(); ++i)
		m_mapNextUse[i->first] = 0UL;
}

VOID KCDTime::Clear(VOID)
{
	KItemSetting::KCdTypeMap::iterator i = g_cItemMgr.m_cSetting.m_mapCdType.begin();
	for (; i != g_cItemMgr.m_cSetting.m_mapCdType.end(); ++i)
	{
		INT nID = i->first;
		if (!m_mapNextUse[nID])
			continue;
		m_mapNextUse[nID] = 0UL;
#ifndef GAME_SERVER
		g_CoreEventNotify(emCOREEVENT_SYNC_CDTYPE, "d", nID);
#endif
	}
}

DWORD KCDTime::GetCDTime(INT nID) CONST
{
	if (!g_cItemMgr.m_cSetting.m_mapCdType.count(nID))
		return	0UL;
	return	g_cItemMgr.m_cSetting.m_mapCdType[nID];
}

DWORD KCDTime::GetNextUseFrame(INT nID) CONST
{
	KLastFrameMap::const_iterator it = m_mapNextUse.find(nID);
	if (it == m_mapNextUse.end())
		return	0UL;
	return	it->second;
}

INT KCDTime::GetFramePass(INT nID) CONST
{
	DWORD dwNextFrame = GetNextUseFrame(nID);
	if (!dwNextFrame)
		return	-1;

	return (INT)(g_cOrpgWorld.m_dwLogicFrames + GetCDTime(nID) - dwNextFrame);
}

BOOL KCDTime::IsTimeUp(INT nID) CONST
{
	DWORD dwNextFrame = GetNextUseFrame(nID);
	if (!dwNextFrame)
		return	TRUE;

	DWORD dwNowFrame = g_cOrpgWorld.m_dwLogicFrames;
	INT nFrameDelta	= (INT)(dwNextFrame - dwNowFrame);
	if (nFrameDelta > 0)
		return	FALSE;

	return	TRUE;
}

BOOL KCDTime::CheckID(INT nID) CONST
{
	return	m_mapNextUse.count(nID) ? TRUE : FALSE;
}

BOOL KCDTime::Start(INT nID)
{
	if (!CheckID(nID))
		return	FALSE;

	m_mapNextUse[nID] = g_cOrpgWorld.m_dwLogicFrames + GetCDTime(nID);
#ifndef GAME_SERVER
	g_CoreEventNotify(emCOREEVENT_SYNC_CDTYPE, "d", nID);
#endif
	return	TRUE;
}

BOOL KCDTime::Stop(INT nID)
{
	if (!CheckID(nID))
		return	FALSE;

	m_mapNextUse[nID] = 0UL;
#ifndef GAME_SERVER
	g_CoreEventNotify(emCOREEVENT_SYNC_CDTYPE, "d", nID);
#endif
	return	TRUE;
}

// -------------------------------------------------------------------------
