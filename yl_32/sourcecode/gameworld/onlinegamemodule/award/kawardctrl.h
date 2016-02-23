#ifndef __KAWARDCTRL_H__
#define __KAWARDCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "awarddef.h"

class KPlayer;
class KAward;

class KAwardCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_AWARD,	};


public:

	explicit KAwardCtrl(KCharacter& rcPlayer);
	virtual ~KAwardCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL Active() { return TRUE; }
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);
	virtual BOOL OnLogout();
	virtual BOOL OnLogin() { return TRUE; }

public:
	BOOL SyncAwardInfo(KAward* pAward);

	BOOL DoTakeAwardRespond(DWORD dwId, BOOL bSucceed);

	BOOL Add(KAward* pAward, BOOL bSync = TRUE);

	BOOL TakeAward(DWORD dwId,const CHAR *szModuleName);

	BOOL DelAward(DWORD dwId = 0);

	// 得到指定模块的奖励id列表
	BOOL GetAwardListByModuleName(std::vector<DWORD>& vecAwardIds, LPCHAR szModuleName);

	BOOL GetPlayerAwards(std::vector<DWORD>& arrAwards, LPCTSTR szModuleName = NULL);

	BOOL TakeAllAward(LPCSTR szModuleName = NULL);

private:
	BOOL SaveAwardData(CONST KAward* pAward, KAwardSaveData* pSaveData);
	BOOL LoadAwardData(KAward* pAward, KAwardSaveData* pSaveData);

private:
	KPlayer& m_rcPlayer;	// 所属玩家

	std::vector<KAward*> m_arrAwards;

};

#endif