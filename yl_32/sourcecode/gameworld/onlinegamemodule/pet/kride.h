/* -------------------------------------------------------------------------
//	文件名		：	kride.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-3-15
//	功能描述	：	坐骑
//
// -----------------------------------------------------------------------*/

#ifndef kride_h__
#define kride_h__


#include "kpetsetting.h"

// -------------------------------------------------------------------------

class KLuaRide;

// -------------------------------------------------------------------------

class KRide
{
public:
	DWORD m_dwId;
	DWORD m_dwOwnerId;

	struct QRIDE_ADD_ATTR
	{
        INT nLife;           // 生命
		INT nDefense;        // 防御
		INT nAttack;         // 攻击
		INT nSpeed;          // 速度
		INT nIgnoreHarmRate; // 免疫伤害几率
		INT nIgnoreDefRate;  // 破防几率
	};

	KLuaRide* GetScriptInterface() { return m_pLuna; }

public:
	KRide(void);
	~KRide(void);

	BOOL Init(KITEM_INDEX sIndex, INT nHungry, INT nMagicRate);

	BOOL Regenerate(INT nGenre, INT nDetail, INT nParticular, INT nLevel);

	VOID SetIndex(INT nIndex) { m_nIndex = nIndex; }

public:

	INT GetGenre() { return m_nGenre; }

	INT GetDetail() { return m_nDetail; }

	INT GetParticular() { return m_nParticular; }

	INT GetLevel() { return m_nLevel; }

	INT GetQuality() { return m_nQuality; }

	INT GetRankId() { return m_nRankId; }

	INT GetHungry() { return m_nHungry; }

	INT GetRepresentId() { return m_nRepresentId; }

	BYTE GetState() { return m_byState; }

	DWORD GetId() { return m_dwId; }

	DWORD GetOwnerId() { return m_dwOwnerId; }

	INT GetMagicRate() { return m_nMagicRate; }

	BOOL SetMagicRate(INT nRate);

	VOID SetRideFightScore(INT nScore) { m_nFightScore = nScore; }

	INT GetRideFightScore() { return m_nFightScore; }

	INT GetNextLevelFightScore();

	BOOL CalcRideFightScore();

private:
	VOID SetState(BYTE byState) { m_byState = byState; }

	BOOL Gen_Ride(KITEM_INDEX sIndex);

	BOOL CopyRideData(CONST KRide* pRide);

	BOOL ApplyRideAddAttr();

	BOOL RemoveRideAddAttr();

private:
	INT m_nGenre;
	INT m_nDetail;
	INT m_nParticular;
	INT m_nLevel;

	INT m_nQuality;
	INT m_nRankId;
	INT m_nHungry;     
	INT m_nRepresentId;

	INT m_nMagicRate;	// 当前食用幻化果的数量

	BYTE m_byState;

	INT m_nFightScore;	// 坐骑战斗力

	QRIDE_ADD_ATTR m_cCurrAddAttr; // 当前坐骑属性加成

protected:
	INT m_nIndex; // RideIndex

	KLuaRide* m_pLuna; // Lua对象指针

	friend class KRideCtrl;
	friend class KPetModule;
};


#endif