
#ifndef __KITEMTIMEOUTCTRL_H__
#define __KITEMTIMEOUTCTRL_H__

#include "kitem.h"
#include <set>
#include <functional>
#include <map>

class KPlayerItem;
class KItemTimeoutCtrl
{
	struct KimeoutLess : public std::binary_function<INT, INT, bool>
	{
		bool operator()(CONST INT& rcIdx1, CONST INT& rcIdx2) CONST;
	};
	typedef std::set<INT, KimeoutLess>	 SET_TIMEOUT;
public:
	KItemTimeoutCtrl();
	~KItemTimeoutCtrl();
	VOID Init(KPlayerItem* pItemList);
	VOID UnInit();
	BOOL AddTimeout(INT nItemIdx);
	VOID DelTimeout(INT nItemIdx);
	VOID SetTimeout(INT nItemIdx, CONST KITEM_TIMEOUT* pTimeout);
	VOID CheckTimeout(time_t tNow, INT nInterval);
private:
	VOID CheckRelTime(SET_TIMEOUT* pSet, INT nInterval);
#ifdef GAME_SERVER
	VOID CheckAbsTime(SET_TIMEOUT* pSet, time_t tNow);
	VOID RemoveItem(INT nItemIdx);
#endif
private:
	KPlayerItem*	m_pItemList;
	SET_TIMEOUT		m_aTimeout[emKITEM_TIMEOUTTYPE_COUNT];

	BOOL			m_bNotify;
};

#endif //__KITEMTIMEOUTCTRL_H__
