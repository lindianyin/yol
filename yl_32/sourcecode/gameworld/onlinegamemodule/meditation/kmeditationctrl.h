#ifndef __KMEDITATIONCTRL_H__
#define __KMEDITATIONCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "meditationdef.h"

class KPlayer;

class KMeditationCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_MEDITATION,	};


public:

	explicit KMeditationCtrl(KCharacter& rcPlayer);
	virtual ~KMeditationCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }
	virtual BOOL Active();
	virtual BOOL OnLogout();
	virtual BOOL OnLogin();
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

public:
	INT  GetMeditationOnlineTime();
	INT	 GetLastMeditationOnlineTime();
	VOID ResetMeditationInfo();
	VOID SetTakeOfflineAwardTime(DWORD nTime);
	DWORD GetTakeOfflineAwardTime();
	
private:
	KPlayer& m_rcPlayer;	// 所属玩家

public:
	KMeditationInfo sMeditationInfo;
	INT m_nStandStateTotalTime;
	BOOL m_IsOpenOnlineMeditation;	//在线打坐面板是否已经打开
};

#endif