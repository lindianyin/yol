#ifndef __KSTAGECTRL_H__
#define __KSTAGECTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "stagedef.h"

class KPlayer;

class KStageCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_STAGE,	};


public:

	explicit KStageCtrl(KCharacter& rcPlayer);
	virtual ~KStageCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }
	virtual BOOL Active();

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

public:
	INT GetStageStar(INT nDifficulty);

	BOOL SetStageStar(INT nDifficulty, INT nStarLevel);

	DWORD GetRushStartTime() { return sRushInfo.dwStartTime; }

	VOID SetRushStartTime(DWORD dwTime) { sRushInfo.dwStartTime = dwTime; }

	INT GetRushLayer() { return sRushInfo.nRushLayer; }

	VOID SetRushLayer(INT nLayer) { sRushInfo.nRushLayer = nLayer; }

	INT GetMopLeftTime() { return sMopInfo.nLeftTime; }

	INT GetMopState() { return sMopInfo.nState; }

	INT GetMopStage() { return sMopInfo.nMopStage; }

	VOID SetMopStage(INT nStage) { sMopInfo.nMopStage = nStage; }

	INT GetRushDifficulty() { return sRushInfo.nStageDifficulty; }

	VOID SetRushDifficulty(INT nDifficulty) { sRushInfo.nStageDifficulty = nDifficulty; }

	INT GetRushState() { return sRushInfo.nState; }

	VOID SetRushState(INT nState) { sRushInfo.nState = nState; }

private:

	BOOL SyncStageInfo();

	VOID ResetMopInfo();

	VOID ResetRushInfo();

public:
	BOOL StartMopStage(INT nMopTime);

	BOOL StopMopStage(INT bFinish);

	BOOL OnTakeAward();

	BOOL OnFinishMop();

	BOOL OnSpeedUp();

private:
	KPlayer& m_rcPlayer;	// ËùÊôÍæ¼Ò

public:
	KStageRushInfo sRushInfo;
	
	KMopStageInfo sMopInfo;
};

#endif