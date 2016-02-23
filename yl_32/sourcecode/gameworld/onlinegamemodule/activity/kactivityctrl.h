#ifndef __KACTIVITYCTRL_H__
#define __KACTIVITYCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "activitydef.h"
#include "config/kplayerbasedbdef.h"

class KPlayer;

class KActivityCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_ACTIVITY,	};


public:

	explicit KActivityCtrl(KCharacter& rcPlayer);
	virtual ~KActivityCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	virtual BOOL OnLogin();

private:
	BOOL ResetActivityInfo();

private:
	KPlayer& m_rcPlayer;	// ËùÊôÍæ¼Ò

public:
	KActivityInfo sActivityInfo;

public:

	BOOL SyncKinBattleRecord();

	BOOL SyncKinBattleWinnerId();

	BOOL SyncLastWinKinInfo();

	BOOL SyncKinBattleJoinNum(INT nJoinNum, INT nHasJoined);

	BOOL SyncActivityTime(INT nActivityIdx, INT nBaseActivityIdx, DWORD dwOpenTime, DWORD dwCloseTime);

};

#endif