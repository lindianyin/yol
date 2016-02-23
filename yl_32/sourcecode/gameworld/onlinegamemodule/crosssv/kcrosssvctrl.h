#ifndef __KCROSSSVCTRL_H__
#define __KCROSSSVCTRL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "crosssvdef.h"
#include "onlinegamemodule/item2/itemdef.h"

class KPlayer;

class KCrossSvCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_CROSS_SV,	};


public:

	explicit KCrossSvCtrl(KCharacter& rcPlayer);
	virtual ~KCrossSvCtrl();

	virtual BOOL Init();
	virtual BOOL UnInit() { return TRUE; }
	virtual BOOL Active();

	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

private:
	KPlayer& m_rcPlayer;	// 所属玩家

private:
	std::map<KITEM_INDEX, INT> m_mapDropItems;		// 跨服时掉落的道具记录

private:
	BOOL GetDropItemsDataSize(SIZE_T& uUsedSize);

	BOOL SaveDropItems(LPBYTE pBuffer, SIZE_T& uUsedSize);

	BOOL LoadDropItems(LPBYTE pBuffer, SIZE_T uDataLen);

public:
	VOID ClearDropItems();

	VOID RecordDropItems(INT nGenre, INT nDetail, INT nParticular, INT nLevel, INT nAmount);

	BOOL GetCrossSvDataSize(SIZE_T& uUsedSize);

	BOOL SaveCrossSvDatas(LPBYTE pBuffer, SIZE_T& uUsedSize);

	BOOL LoadCrossSvDatas(LPBYTE pBuffer, SIZE_T uDataSize);
};

#endif