
#ifndef __KSWITCHITEM_H__
#define __KSWITCHITEM_H__

#include "onlinegamemodule/item2/itemdef.h"

class KPlayerItem;

// 集中处理物品更换的一些事件
class KSwitchNotify
{
public:
	explicit KSwitchNotify(KPlayerItem& rcPlayerItem) : m_rcPlayerItem(rcPlayerItem) {}
	BOOL SwitchBegin();
	BOOL SwitchEnd();
	BOOL OnPickNotify(CONST KITEM_POS& rsPos, INT nPickList);
	BOOL OnDropNotify(CONST KITEM_POS& rsPos, INT nPickList);
private:
	KPlayerItem& m_rcPlayerItem;
};

// -------------------------------------------------------------------------
// 该类主要的对外方法Switch()负责交换两个位置的物品，注意不区分源和目的，仅交换，因而叠加的情况应该放到更外围去处理；
// 所有的判断都应该在CheckProc里进行，SwitchProc没有失败，目前还没有对应的异常处理；
// Switch机制保证回调函数的参数一定是正确无冲突的的，Drop、Pick两边不会是同一个位置，没必要不要做多余的判断。

class KSwitchItem
{

private:

	typedef BOOL (KSwitchItem::*CheckProc)(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);	// 切换检查回调类型
	typedef VOID (KSwitchItem::*PickProc)(CONST KITEM_POS& rsPos, INT nPickList);			// 拿起处理回调类型
	typedef VOID (KSwitchItem::*DropProc)(CONST KITEM_POS& rsPos, INT nDropList);			// 放下处理回调类型

	struct KPROCPAIR
	{
		CheckProc	pfnCheck;
		PickProc	pfnPick;
		DropProc	pfnDrop;
	};

public:

	static VOID	InitProc(VOID);		// 注册两个可以相互交换的空间

public:

	explicit KSwitchItem(KPlayerItem& rcPlayerItem) : m_rcPlayerItem(rcPlayerItem), m_SwitchNotify(rcPlayerItem) {}

	BOOL Switch(CONST KITEM_POS& rsLeft, CONST KITEM_POS& rsRight);

	// CheckProc Callbacks
	BOOL CheckBase		(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);
	BOOL CheckEquip		(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);
	BOOL CheckEquipEx2	(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);
	BOOL CheckRepository(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);
	BOOL CheckExtBagBar	(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);
	BOOL CheckExtBag	(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);
	BOOL CheckExtRep	(CONST KITEM_POS& rsPos, INT nPick, INT nDrop);

	// PickProc Callbacks
	VOID PickBase		(CONST KITEM_POS& rsPos, INT nPickList);
	VOID PickEquip		(CONST KITEM_POS& rsPos, INT nPickList);
	VOID PickExtBagBar	(CONST KITEM_POS& rsPos, INT nPickList);
	
	// DropProc Callbacks
	VOID DropBase		(CONST KITEM_POS& rsPos, INT nDropList);
	VOID DropEquip		(CONST KITEM_POS& rsPos, INT nDropList);
	VOID DropEquipEx	(CONST KITEM_POS& rsPos, INT nDropList);
	VOID DropExtBagBar	(CONST KITEM_POS& rsPos, INT nDropList);
	
private:

	static KPROCPAIR	ms_asProc[emROOM_NUM];
	static BOOL			ms_aabSwitchProc[emROOM_NUM][emROOM_NUM];

public:

	KSwitchNotify		m_SwitchNotify;

private:

	KPlayerItem&		m_rcPlayerItem;

};

// -------------------------------------------------------------------------

#endif /* __KSWITCHITEM_H__ */
