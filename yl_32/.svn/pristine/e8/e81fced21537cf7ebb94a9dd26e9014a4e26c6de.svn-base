#ifndef _H_PROGRESS_BAR_H
#define  _H_PROGRESS_BAR_H

#include "onlinegameworld/kplayer.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kcharacterctrlbase.h"

class KPlayerProgressBar : public KCharacterCtrlBase
{
public:
	enum
	{
		emKCtrlId = emKCHARACTER_CTRL_PLAYER_PROGRESS_BAR,
	};
	
	KPlayerProgressBar(KCharacter& rcCharacter): KCharacterCtrlBase(rcCharacter){}
	virtual ~KPlayerProgressBar(){}

	BOOL StartProgressBar(INT nIntervalFrame);
	BOOL CancelProgressBar();

	/*Ctrl需要的接口*/
	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL Active();

private:
	INT OnTimer();

private:
	DWORD m_dwFinishFrame;
};

#endif
