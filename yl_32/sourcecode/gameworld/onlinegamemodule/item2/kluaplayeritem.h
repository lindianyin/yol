
#pragma once

class KPlayer;
class KPlayerItem;

class KLuaPlayerItem : public QLunaBase
{
public:

	static KPlayer* GetMe(QLuaScript&, INT nIndex);

	explicit KLuaPlayerItem(KPlayer* pPlayer);
	~KLuaPlayerItem()
	{
	}

	static INT RegisterToPlayer(QLuaScript& sc, QLunaBase* pLunaObj);
	KPlayer* GetMe()	{ return m_pMe; };

	//脚本接口定义
	DECLARE_LUA_CLASS(KLuaPlayerItem);

	INT LuaGetItem(QLuaScript& sc);
	INT LuaGetEquipByDetail(QLuaScript& sc);
	INT LuaGetItemCount(QLuaScript& sc);
	INT LuaHaveItem(QLuaScript& sc);
	INT LuaCountFreeBagCell(QLuaScript& sc);
	INT LuaCalcFreeItemCountInBags(QLuaScript& rcScript);
	INT LuaFindItem(QLuaScript& sc);
	INT LuaFindItemInBags(QLuaScript& sc);
	INT LuaFindItemId(QLuaScript& sc);
	INT LuaFindClassItem(QLuaScript& sc);
	INT LuaCanAddItemIntoBag(QLuaScript& sc);
	INT LuaGetItemAbsTimeout(QLuaScript& rcScript);
	INT LuaGetItemRelTimeout(QLuaScript& rcScript);
	INT LuaGetJbCoin(QLuaScript& sc);
	INT LuaGetMaxCarryMoney(QLuaScript& sc);
	INT LuaGetCashCoin(QLuaScript& sc);
	INT LuaGetBindMoney(QLuaScript& sc);
	INT LuaGetItemPos(QLuaScript& sc);
	INT LuaThrowAway(QLuaScript& sc);

#ifdef GAME_SERVER
	INT LuaDropRateItem(QLuaScript& sc);
	INT LuaOpenRepository(QLuaScript& sc);
	INT LuaSetExtRepState(QLuaScript& sc);
	INT LuaConsumeItemInBags(QLuaScript& rcScript);
	INT LuaSetItemTimeout(QLuaScript& rcScript);
	INT LuaDelItemTimeout(QLuaScript& rcScript);
	INT LuaCostMoney(QLuaScript& sc);
	INT LuaCostBindMoney(QLuaScript& sc);
	INT LuaIsMyItem(QLuaScript& sc);
	INT LuaCalcFreeItemCellCount(QLuaScript& sc);
	INT LuaThrowAllItem(QLuaScript& sc);
	INT LuaReApplyEquipRes(QLuaScript& sc);
	INT LuaFindAllItem(QLuaScript& sc);
	INT LuaItemLog(QLuaScript& sc);
	INT LuaAddBindCoin(QLuaScript& sc);
	INT LuaAutoEquip(QLuaScript& sc);
	INT LuaAutoUse(QLuaScript& sc);
#endif

private:
	KPlayer* CONST m_pMe;
	KPlayerItem* m_pPlayerItem;
};
