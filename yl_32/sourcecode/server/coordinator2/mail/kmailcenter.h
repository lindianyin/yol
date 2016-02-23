/* -------------------------------------------------------------------------
//	文件名		：	kmailcenter.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/17 17:39:02
//	功能描述	：	邮件中心
//
// -----------------------------------------------------------------------*/
#ifndef __KMAILCENTER_H__
#define __KMAILCENTER_H__

// -------------------------------------------------------------------------
#ifdef GAME_CENTER
#include "Misc/objectpool.h"

struct KItemData_Common;

class KMailCenter
{
public:
	KMailCenter();
	~KMailCenter();

public:
	static KMailCenter *Instance() {return &ms_inst;}

public:
	BOOL Init();
	BOOL UnInit();
	BOOL ProcessData(INT nConnectId, LPBYTE pData, UINT uSize);
	BOOL OnPlayerLogin(DWORD dwPlayerId);
	BOOL OnPlayerLogout(DWORD dwPlayerId);
	BOOL OnShutDown();
	BOOL Breath();

public:
	// 投递系统邮件
	BOOL PostSysMail(DWORD dwReceiverId, LPCSTR pszCaption, LPCSTR pszContent, 
					 KItemData_Common* pItemData, INT nItemCount, INT nIconId);
	
private:
	static KMailCenter				ms_inst;
	BOOL							m_bStartSearch;			// 是否开始遍历邮箱
	DWORD							m_dwLastPlayerId;		// 上次搜索的玩家Id，用于遍历邮箱邮件
	INT								m_nSearchTime;			// 上一次遍历完后的时间
	INT								m_nSearchMailBoxCount;	// 每帧遍历的邮箱个数
	INT								m_nSearchTimeSpace;		// 两轮遍历的间隔时间
};

#endif /* GAME_CENTER */
// -------------------------------------------------------------------------

#endif /* __KMAILCENTER_H__ */
