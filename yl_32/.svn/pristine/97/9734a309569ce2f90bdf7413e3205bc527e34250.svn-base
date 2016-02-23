#ifndef __KTOWERCTRL_H__
#define __KTOWERCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "towerdef.h"

class KPlayer;

class KTowerCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_TOWER,	};


public:

	explicit KTowerCtrl(KCharacter& rcPlayer);
	virtual ~KTowerCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }
	virtual BOOL Active();

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

public:
	BOOL StartMopTower(INT nMopTime);

	BOOL StopMopTower(INT nCurLayer);

	BOOL OnFinishMop();

	BOOL OnSpeedUp();

	BOOL OnTakeAward();

private:
	VOID ResetMopInfo();

	BOOL SyncTowerInfo();

private:
	KPlayer& m_rcPlayer;	// ËùÊôÍæ¼Ò

public:
	KMopTowerInfo sMopInfo;

};

#endif