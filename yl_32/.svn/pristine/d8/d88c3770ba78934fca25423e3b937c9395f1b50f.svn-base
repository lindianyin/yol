
#ifndef __KLUAITEM_H__
#define __KLUAITEM_H__

// -------------------------------------------------------------------------

class KItem;

class KLuaItem : public QLunaBase
{

public:

	static KItem* GetIt(QLuaScript& sc, INT nIndex);

	explicit KLuaItem(KItem* pItem) : m_pIt(pItem)
	{

	}

	~KLuaItem()
	{

	}

	KItem*	GetIt(VOID) { return m_pIt; };

	BOOL	ClearTempTable(VOID);

	// 脚本接口定义
	DECLARE_LUA_CLASS(KLuaItem);

	LPCSTR	getName(VOID) CONST;
	LPCSTR	getOrgName(VOID) CONST;
	LPCSTR	getSuffix(VOID) CONST;
	INT		getGenre(VOID) CONST;
	INT		getDetail(VOID) CONST;
	INT		getParticular(VOID) CONST;
	INT		getLevel(VOID) CONST;
	INT		getFightPower(VOID) CONST;

	INT		getCount(VOID) CONST;
	INT		getMaxCount(VOID) CONST;
	INT		getPrice(VOID) CONST;
	INT		getBindType(VOID) CONST;
	INT		getEnhTimes(VOID) CONST;
	INT		getSlotSize(VOID) CONST;
	INT		getStrengthen(VOID) CONST;
	INT		getEquipPos(VOID) CONST;
	INT		getValue(VOID) CONST;
	INT		getStarLevel(VOID) CONST;
	INT		getNameColor(VOID) CONST;
	INT		getReqLevel(VOID) CONST;
	INT		getQuality(VOID) CONST;
	INT		getCdType(VOID) CONST;
	INT		getCustomType(VOID) CONST;
	INT		getEquipCategory(VOID) CONST;

	DWORD	getId(VOID) CONST;
	DWORD	getRandSeed(VOID) CONST;
	DWORD	getRandSeedEx(VOID) CONST;
	LPCSTR	getClass(VOID) CONST;
	LPCSTR	getCustomString(VOID) CONST;
	LPCSTR	getForbidType(VOID) CONST;
	VOID	setEnhTimes(INT);
#ifdef GAME_SERVER
	DWORD	getGenTime(VOID) CONST;
	INT		getBuyPrice(VOID) CONST;
	VOID		setBuyPrice(INT);
	LPCSTR	getGUID(VOID) CONST;
#else
	INT		getIndex(VOID) CONST;
	LPCSTR	getIntro(VOID) CONST;
	LPCSTR	getHelp(VOID) CONST;
	LPCSTR	getIconImage(VOID) CONST;
	LPCSTR	getViewImage(VOID) CONST;
	LPCSTR	getTransparencyIcon(VOID) CONST;
	LPCSTR  getMaskLayerIcon(VOID) CONST;
#endif

	INT		LuaIsEquip(QLuaScript& rcScript);
	INT		LuaIsStone(QLuaScript& rcScript);
	INT		LuaIsInvalid(QLuaScript& rcScript);
	INT		LuaIsDuped(QLuaScript& rcScript);
	INT		LuaIsTemp(QLuaScript& rcScript);
	INT		LuaIsLock(QLuaScript& rcScript);
	INT		LuaIsBind(QLuaScript& rcScript);
	INT		LuaCanBeShortcut(QLuaScript& rcScript);
	INT		LuaGetTempTable(QLuaScript& rcScript);
	INT		LuaGetStoneInfo(QLuaScript& rcScript);
	INT		LuaGetAppendAttr(QLuaScript& sc);
	INT		LuaGetSaveAppendAttr(QLuaScript& sc);
	INT		LuaGetBaseAttrib(QLuaScript& rcScript);
	INT		LuaGetReqAttrib(QLuaScript& rcScript);
	INT		LuaGetExtParam(QLuaScript& rcScript);
	INT		LuaGetUpgradeItems(QLuaScript& sc);
	INT		LuaIsStackable(QLuaScript& rcScript);
	INT		LuaIsDestoriable(QLuaScript& rcScript);
	INT		LuaIsForbitThrow(QLuaScript& rcScript);
	INT		LuaIsForbitSell(QLuaScript& rcScript);
	INT		LuaGetOwner(QLuaScript& rcScript);
	INT		LuaRemove(QLuaScript& rcScript);
	INT		LuaClearSaveAppendAttr(QLuaScript& sc);
	INT		LuaSetTimeOut(QLuaScript& rcScript);
	INT		LuaSetCount(QLuaScript& rcScript);
	INT		LuaSetGenerateMoneyType(QLuaScript& sc);
	INT		LuaGetSellMoneyType(QLuaScript& rcScript);
	INT		LuaHasStone(QLuaScript& rcScript);
	INT		LuaGetEquipScore(QLuaScript& rcScript);
	INT		LuaGetResourceId(QLuaScript& rcScript);
	INT		LuaGetChangeColorScheme(QLuaScript& rcScript);
	INT		LuaGetTimeOut(QLuaScript& rcScript);
	INT		LuaBind(QLuaScript& rcScript);
	INT		LuaGetBagPosLimit(QLuaScript& rcScript);
	INT		LuaGetNextReqLevel(QLuaScript& sc);
	INT		LuaGetStoneType(QLuaScript& sc);
	INT		LuaGetLockIntervale(QLuaScript& rcScript);
	INT		LuaStoneClean(QLuaScript& rcScript);
	INT		LuaStonePunch(QLuaScript& sc);
	INT		LuaGetGDPLStr(QLuaScript& rcScript);
#ifdef GAME_SERVER
	INT		LuaIsIbItem(QLuaScript& rcScript);
	INT		LuaRegenerate(QLuaScript& rcScript);
	INT		LuaSync(QLuaScript& rcScript);
	INT		LuaChangeColorScheme(QLuaScript& rcScript);
	INT		LuaSetCustom(QLuaScript& rcScript);
	INT		LuaSetExtParam(QLuaScript& rcScript);
	INT		LuaRemoveMagicAttrib(QLuaScript &sc);
	INT		LuaApplyMagicAttrib(QLuaScript &sc);
#else
#endif

private:

	KItem* m_pIt;

};

// -------------------------------------------------------------------------

#endif /* __KLUAITEM_H__ */
