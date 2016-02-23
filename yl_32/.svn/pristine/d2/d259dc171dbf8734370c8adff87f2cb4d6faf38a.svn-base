
#ifndef kaward_h__
#define kaward_h__


#include "kawardsetting.h"
#include "kawardctrl.h"

class KAward
{
public:
	DWORD m_dwId;

public:
	KAward(void);
	~KAward(void);

	BOOL Init(KPlayer* pPlayer, LPCTSTR szModuleName, INT nLevel, INT nTimes = 1, INT nAwardPercent = 100);

public:

	DWORD GetId() { return m_dwId; }
	VOID SetId(DWORD dwId) { m_dwId = dwId; }

	INT GetIndex() { return m_nIndex; }
	VOID SetIndex(INT nIdx) { m_nIndex = nIdx; }

	INT GetRankIdx() { return m_nRankIdx; }
	VOID SetRankIdx(INT nRankIdx) { m_nRankIdx = nRankIdx; }

	INT GetFightSpirit() { return m_nFightingSpirit; }
	INT GetEnergy() { return m_nEnergy; }
	INT GetSilver() { return m_nSilver; }
	INT GetExp() { return m_nExp; }
	INT GetPrestige() { return m_nPrestige; }

	INT GetItemNum() { return m_nItemNum; }

	BOOL IsSync() { return m_bSync; }

	LPCSTR GetModuleName() { return m_szModuleName; }

	INT GetLevel() { return m_nLevel; }

	CONST KAwardItem* GetAwardItem(INT nItemIdx) { return &(m_arrItems[nItemIdx]); }

private:
	INT m_nIndex;

	INT m_nRankIdx;

	CHAR m_szModuleName[MAX_NAME_LEN];
	INT m_nLevel;

	BOOL m_bAutoGive;

	BOOL m_bSync;
	BOOL m_bSave;
	time_t m_tOutdateTime;		// 奖励过期时间，永不过期为-1

	INT m_nFightingSpirit;
	INT m_nEnergy;
	INT m_nSilver;
	INT m_nExp;
	INT m_nPrestige;

	INT m_nItemNum;
	KAwardItem m_arrItems[ASSET_AWARD_MAX_ITEM_NUM];

public:
	BOOL m_bGiveExp;		// 是否给经验

protected:

	friend class KAwardCtrl;
};


#endif