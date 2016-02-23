/* -------------------------------------------------------------------------
//	文件名		：	qsellmgr.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-10-30 17:13:00
//	功能描述	：  神秘商店卖出物品设置
// -------------------------------------------------------------------------*/

#ifndef __QSELL_MGR_H__
#define __QSELL_MGR_H__
#include "item2/kmanager_i.h"
#include "qsellsetting.h"
#include <map>
#include <string>

extern BOOL g_bGlobalServer;

class KItemRoom;

class QSellMgr : public IKManager
{
private:
	std::map<INT, QMysteryGoods>	m_mapGoodsInShop;
	QSellSetting	m_cSetting;
public:
	QSellMgr();
	~QSellMgr();

	BOOL	Init();
	BOOL	Release();

	INT		GetWidth() CONST { return m_cSetting.GetWidth(); }
	INT		GetHeight() CONST { return m_cSetting.GetHeight(); }

	BOOL	GetBindType(INT nIndex);

#ifdef GAME_SERVER
	BOOL    ProtocolProcess(LPCBYTE pData, INT nDataLen, KPlayer& rcPlayer);
#endif

#ifdef GAME_SERVER
	BOOL	GenrateGoods(INT nPlayerIdx);
	
#endif
};

extern QSellMgr		g_cSellMgr;

#endif