/* -------------------------------------------------------------------------
//	文件名		：	kluaplayeritem_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaPlayerItem)
	REGISTER_LUA_FUNC(GetItem, "获得角色指定位置的道具", "ddd", "nRoom:道具所在空间;nX:横坐标;nY:纵坐标;", "返回道具对象，失败返回nil")
	REGISTER_LUA_FUNC(GetItemCount, "获取指定空间中物品的数量", "d[?????]", "nRoom;nGenre;nDetail;nParticular;nBind;nTimeOut;", "返回总数量，空间不存在返回nil")
	REGISTER_LUA_FUNC(HaveItem, "看一个人是否拥有某件道具，并获得所在空间和坐标", "o", "pItem:目标道具的对象;", "没找到返回nil，否则返回一个Table：{ nRoom = 道具所在空间, nX = 横坐标, nY = 纵坐标 }")
	REGISTER_LUA_FUNC(CountFreeBagCell, "获取所有背包中空闲格子的数目", "", "", "空闲格子数")
	REGISTER_LUA_FUNC(CalcFreeItemCountInBags, "计算所有背包空间中还可放置某个物品的数量", "dddd", "nGenre;nDetail;nParticular;bBind;", "还可放置物品的数量")
	REGISTER_LUA_FUNC(FindItem, "在指定空间里查找指定类型的道具", "d[????]", "nRoom;nGenre/szClass;nDetail;nParticular;nLevel;", "返回找到的道具列表，nil表示失败（参数不正确），空Table表示没找到")
	REGISTER_LUA_FUNC(FindItemInBags, "在主背包和扩展背包里查找指定物品", "ddd[?]", "nGenre/szClass;nDetail;nParticular;nLevel;", "返回找到的道具列表，nil表示失败（参数不正确），空Table表示没找到")
	REGISTER_LUA_FUNC(FindItemId, "在指定空间里查找指定类型的道具", "d", "nItemId;", "返回nRoom,nX,nY,失败返回nil")
	REGISTER_LUA_FUNC(FindClassItem, "在指定空间里查找指定Class的道具", "ds", "nRoom;szClass;", "返回找到的道具列表，nil表示失败（参数不正确），空Table表示没找到")
	REGISTER_LUA_FUNC(CanAddItemIntoBag, "检查当前背包是否能够放得下指定的东西（会将各种复杂的叠加情况也计算在内）", "?", "道具列表，每一项的格式：{ nGenre, nDetail, nParticular, bBind, nCount [bTimeOut]}，项数不限;", "1放得下")
	REGISTER_LUA_FUNC(GetItemAbsTimeout, "获取物品绝对超时时间", "o", "物品对象;", "返回绝对超时时间表{年，月，日，时，分}或者nil")
	REGISTER_LUA_FUNC(GetItemRelTimeout, "获取物品相对超时时间", "o", "物品对象;", "返回相对超时时间表{天，时，分}或者nil")
	REGISTER_LUA_FUNC(GetJbCoin, "获取金币数", "", "", "")
	REGISTER_LUA_FUNC(GetMaxCarryMoney, "获取玩家携带银两上限", "", "", "")
	REGISTER_LUA_FUNC(GetCashCoin, "获取玩家金币替代物品的数量", "", "", "")
	REGISTER_LUA_FUNC(GetBindMoney, "获取玩家绑定银两", "", "", "")
	REGISTER_LUA_FUNC(GetItemPos, "根据已知道具对象返回它在自己身上的位置", "o", "pItem:要查找的道具对象;", "{ nRoom, nX, nY }，失败返回nil")
	REGISTER_LUA_FUNC(ThrowAway, "扔掉身上的指定物品", "d[dd]", "nRoom:要扔的物品所在空间;nX:要扔的物品所在X;nY:要扔的物品所在Y;", "1成功")
	REGISTER_LUA_FUNC(GetEquipByDetail, "根据D值获得角色身上装备", "d", "nDetail;", "返回道具对象，失败返回nil")

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(DropRateItem, "按照指定的DropRate掉落道具", "s[do]", "szDropRateFile:掉落率配置文件;nCount:掉落数量，默认为1;pNpc:掉落位置以哪个npc为中心，默认为当前玩家;", "无")
	REGISTER_LUA_FUNC(OpenRepository, "打开储物箱", "o", "pNpc:打开储物箱的NPC;", "1成功")
	REGISTER_LUA_FUNC(SetExtRepState, "设置扩展储物箱激活状态", "d", "nState;", "--")
	REGISTER_LUA_FUNC(ConsumeItemInBags, "消耗减少一定数量的指定类型的物品（个数为物品叠放数），物品必须在背包中", "d[????]", "nCount:请求数目;nGenre;nDetail;nParticular;nLevel;", "返回请求消费道具数目 - 实际消费数目")
	REGISTER_LUA_FUNC(SetItemTimeout, "设置物品超时时间", "??[d]", "物品对象或者物品索引;绝对时间（字符串）形如'YYYY-MM-DD HH:MM:SS'，或者分钟数（整数）;BOOL:1表示相对时间，0表示绝对时间，第二个参数为整数时有效，缺省为1;", "成功返回1，失败返回0")
	REGISTER_LUA_FUNC(DelItemTimeout, "取消物品超时时间", "?", "物品对象或者物品索引;", "成功返回1，失败返回0")
	REGISTER_LUA_FUNC(CostMoney, "扣除剑侠币", "dd", "扣除的数目;扣除途径;", "扣除成功与否")
	REGISTER_LUA_FUNC(CostBindMoney, "扣除绑定银两", "d[d]", "nCostCount 扣除的数目;nCostWay 扣除的途径;", "扣除成功与否")
	REGISTER_LUA_FUNC(IsMyItem, "制定物品的是否属于自己的", "d", "nItemIndex：物品索引;", "0：不是，1：是的")
	REGISTER_LUA_FUNC(CalcFreeItemCellCount, "获取当前玩家背包中空着的格子数", "", "", "背包中空着的格子数")
	REGISTER_LUA_FUNC(ReApplyEquipRes, "", "", "", "")
	REGISTER_LUA_FUNC(ThrowAllItem, "抛弃背包里的所有物品", "", "", "")
	REGISTER_LUA_FUNC(FindAllItem, "查找指定空间所有物品指定空间所有物品", "d", "nRoom 指定空间;", "{}")
	REGISTER_LUA_FUNC(ItemLog, "物品流向日志（发往Goddess）", "odd[sd]", "pItem: 物品;BOOL：是否获得物品;nType：行为类型，见KE_PLAYERLOG_TYPE@klogtype.h;szComment：注释;bForce：是否强制写日志,缺省为FALSE;", "")
	REGISTER_LUA_FUNC(AddBindCoin, "对绑定金币的操作", "d[d]", "nBindCoin;nWay;", "1：成功，0：失败")
	REGISTER_LUA_FUNC(AutoEquip, "自动装备道具", "?", "nItemIndex/pItem;", "成功返回1")
	REGISTER_LUA_FUNC(AutoUse, "自动使用", "?", "nItemIndex/pItem;", "成功返回1")
#endif // #ifdef GAME_SERVER
DEFINE_LUA_CLASS_END()
