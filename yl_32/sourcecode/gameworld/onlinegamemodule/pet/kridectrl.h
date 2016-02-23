#ifndef __KRIDECTRL_H__
#define __KRIDECTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "config/kplayerbasedbdef.h"

class KPlayer;
class KRide;

class KRideCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_RIDE	};

private:

	struct RideSaveData
	{
		BYTE byGenre;
		BYTE byDetail;
		WORD wParticular;
		BYTE byLevel;
		BYTE byHungry;
		BYTE byState;
		WORD wMagicRate;
		WORD wRepresentId;
	};


public:

	explicit KRideCtrl(KCharacter& rcPlayer);
	virtual ~KRideCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL Active();
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);
	virtual BOOL OnLogout();

private:
	BOOL ModifyRideBaseAttrib(KRide* pRide, BOOL bIsEndProcess);
	BOOL ModifyRideAttrib(KRide* pRide, BOOL bIsEndProcess);

public:
	KRide* GetCurrentRide() { return m_pCurrentRide; }

	BOOL Add(KRide* pRide, BOOL bSync = FALSE);

	BOOL SyncRideInfo(KRide* pRide);

	BOOL ApplyRideAttrib(KRide* pRide);

	BOOL RemoveRideAttrib(KRide* pRide);

	BOOL SaveRideData(RideSaveData* pBuffer, KRide* pRide);

	BOOL MakeRide(KRide* pRide, RideSaveData* pSaveData);

	BOOL TakeOffRide(KRide* pRide);

	BOOL RemoveOnly();

	BOOL ChangeRideState(KRide* pRide, BYTE byState);

	BOOL SyncRideState(KRide* pRide);

	BOOL BroadcastRideLevel(KRide* pRide);

	BOOL BroadcastRideState(KRide* pRide);

	BOOL AddHungry(KRide* pRide, INT nHungry);

	BOOL SaveRideDelayData(KDelayRideData* pData);

	BOOL ChangeRideRepresentId(DWORD dwRideId,INT nRepresentId);

private:
	KPlayer& m_rcPlayer;	// 所属玩家

	KRide* m_pCurrentRide;		// 玩家当前坐骑
};


#endif