/* -------------------------------------------------------------------------
//	文件名		：	kitemscript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"GetItemObj", LuaGetItemObj, "根据道具索引，得到道具对象", "d", "nIdx:道具索引;", "返回道具对象，失败返回nil"},
	{"GetItemCount", LuaGetItemCount, "获得道具总数", "", "", "道具总数"},
	{"GetNameById", LuaGetNameById, "根据道具ID，得到道具名称", "dddd", "nGenre:主类型;nDetailType:详细类别;nParticular:具体类别;nLevel:级别;", "成功返回Item名称，失败返回空"},
	{"GetMagicValue", LuaGetMagicValue, "获得指定魔法属性的原始价值量", "sd", "szAttrib 魔法属性名;nLevel 魔法属性等级;", "价值量，出错看作0"},
	{"CanPlayerUseItem", LuaCanPlayerUseItem, "检查指定角色是否可以使用/装备指定道具", "oo", "pPlayer:指定角色;pItem:指定道具;", "1可以使用"},
	{"CheckLimitUse", LuaCheckLimitUse, "检查指定角色在当前地图是否受限使用（CanPlayerUseItem是检查装备的）", "ds[d]", "nMapId:地图ID;szForbitType:禁用类型;bChat:是否发送失败信息给角色;", "1可以使用"},
	{"EquipType2EquipPos", LuaEquipType2EquipPos, "从装备类型获得装备位置", "d", "nType:装备类型;", "成功返回装备位置，失败返回-1"},
	{"GetItemBaseProp", LuaGetItemBaseProp, "获得道具基础信息", "dddd", "nGenre;nDetail;nParticular;nLevel;", "返回基础信息表，失败返回nil"},
	{"GetEquipBaseProp", LuaGetEquipBaseProp, "获得装备基础属性信息", "dddd", "nGenre;nDetail;nParticular;nLevel;", "返回装备基础属性信息表，出错返回nil"},
	{"GetOtherBaseProp", LuaGetOtherBaseProp, "获得非装备基础属性信息", "ddd[d]", "nGenre;nDetail;nParticular;nLevel;", "返回非装备基础属性信息表，出错返回nil"},
	{"GetOtherForbidType", LuaGetOtherForbidType, "获得非装备道具禁止类型", "dddd", "nGenre;nDetail;nParticular;nLevel;", "返回非装备道具禁止类型，出错返回nil"},
	{"IsItemBindByBindType", LuaIsItemBindByBindType, "根据绑定类型判断道具新添加到玩家身上时是否是绑定的", "d", "nBindType:绑定类型;", "1绑定"},
	{"GetObjById", LuaGetObjById, "根据道具ID获得道具对象", "d", "dwId;", "pItem，失败返回nil"},
	{"IsEquipExist", LuaIsEquipExist, "根据GDPL判断一件装备是否存在", "dddd", "G;D;P;L;", ""},

#ifdef GAME_SERVER
	{"AddPlayerItem", LuaAddPlayerItem, "给指定角色增加一个新道具", "?ddd[ddd??uudddd]", "nPlayerIndex/pPlayer:角色索引或者对象;nGenre;nDetail;nParticular;nLevel:道具等级，默认为0;nCount:道具数量，默认为1;nEnhTimes:道具已强化次数，默认为0;tbStoneInfo:道具生成参数，默认为nil;tbRandomInfo:道具生成随机参数，默认为nil;uRandSeed:随机种子，默认为自动分配(0);uRandSeedEx:第二随机种子，默认为自动分配(0);nBind:是否强制绑定，默认不强制绑定(0);bIsTimeOut:是否会超时,默认叠加(FALSE);bMsg2Player:是否提示获得;nWay: 获取物品的途径, 默认100;", "成功返回物品对象，失败返回nil"},
	{"GetStackInfo", LuaGetStackInfo, "判断物品是否可叠加", "?ddd[dd???uudddd]", "nPlayerIndex/pPlayer:角色索引或者对象;nGenre;nDetail;nParticular;nLevel:道具等级，默认为0;nEnhTimes:道具已强化次数，默认为0;tbGenInfo:道具生成参数，默认为nil;tbStoneInfo:道具生成参数，默认为nil;tbRandomInfo:道具生成随机参数，默认为nil;uRandSeed:随机种子，默认为自动分配(0);uRandSeedEx:第二随机种子，默认为自动分配(0);nBind:是否强制绑定，默认不强制绑定(0);bIsTimeOut:是否会超时,默认叠加(FALSE);bMsg2Player:是否提示获得;nWay: 获取物品的途径, 默认100;", "成功返回插入成功的值，失败返回0"},
	{"SetFailedFrameInterval", LuaSetFailedFrameInterval, "设置使用道具失败判断的帧间隔", "d", "nNewInterval, 新的帧间隔;", "无"},
	{"ArrangeBag", LuaArrangeBag, "整理背包", "?", "player;", "无"},
	{"SplitItem", LuaSplitItem, "拆分道具", "?dd", "player;itemid;拆分数量;", "无"},
	{"SetFailedTimeLimit", LuaSetFailedTimeLimit, "设置使用道具连续失败次数限制", "d", "nNweLimit, 新的限制次数;", "无"},
	{"AddItemInPos", LuaAddItemInPos, "在某个地点掉落一件道具", "dddddd[d??uo]", "nMapId;nX;nY;nGenre;nDetail;nParticular;nEnhTimes:道具已强化次数，默认为0;tbGenInfo:道具生成参数，默认为nil;tbRandomInfo:道具生成随机参数，默认为nil;uRandSeed:随机种子，默认为自动分配(0);pBelong:归属人;", "成功返回物品对象，失败返回nil"},
	{"DelPlayerItem", LuaDelPlayerItem, "将指定角色身上的某物品删除", "??[dd]", "nPlayerIndex/pPlayer:角色索引或对象;nItemIndex/pItem:道具索引或对象;消耗数量;nWay:失去途径;", "1成功"},
#endif // #ifdef GAME_SERVER

#ifdef GAME_CLIENT
	{"IsGetBindType", LuaIsGetBindType, "根据goods的类型返回对应物品的是否为绑定类型", "d", "Goods的Index;", "1:获取绑定，0：不绑定"},
	{"GetPlayerGoldSuiteAttrib", LuaGetPlayerGoldSuiteAttrib, "根据套装ID获得黄金装备激活魔法属性", "od", "pPlayer:玩家对象;nSuiteId:套装ID;", "返回套装的名称、总数、各件名称、激活属性等信息表"},
	{"GetPlayerGreenSuiteAttrib", LuaGetPlayerGreenSuiteAttrib, "根据套装ID获得绿色装备激活魔法属性", "od", "pPlayer:玩家对象;nSuiteId:套装ID;", "返回套装的名称、总数、各件名称、激活属性等信息表"},
	{"GetRandAttribInfo", LuaGetRandAttribInfo, "根据随机魔法属性名字/ID和级别获得相关信息", "?d", "szAttrib/nAttrib:魔法属性名/魔法属性ID;nLevel:魔法属性级别;", "随机魔法属性信息表，出错返回nil。"},
#endif // #ifdef GAME_CLIENT
};
