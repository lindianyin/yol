-- Item脚本类

Include("/script/item/define.lua");

Item.ItemOb = {};
------------------------------------------------------------------------------------------
-- initialize

-- Item基础模板，详细的在default.lua中定义
if not Item.tbClassBase then
	Item.tbClassBase = {};
end

-- 防止重载脚本的时候模板库丢失
if not Item.tbClass then
	-- Item模板库
	Item.tbClass =
	{
		-- 默认模板，可以提供直接使用
		["default"]	= Item.tbClassBase,
		[""]		= Item.tbClassBase,
	};
end


Item.TASK_OWNER_TONGID = 1; -- General Info里的第一项记录绑定的帮会ID

--货币单位显示
Item.tbTipPriceUnit =
{
	[1] = Lang.item.str60[Lang.Idx],		--银两
	[2] = "",			--福缘
	[3] = Lang.item.str128[Lang.Idx],		-- 金币替代物品（魂石，月影之石，情花等）
	[4] = Lang.item.str129[Lang.Idx],		--积分
	[5] = "",			--贡献度
	[6] = "",			--联赛荣誉点数
	[7] = Lang.item.str130[Lang.Idx],			--绑定银两
	[8] = Lang.item.str131[Lang.Idx],		--机关耐久度货币类型
	[9] = Lang.item.str132[Lang.Idx],		-- 帮会建设资金
	[10]= "%s",					-- 数值货币
}
------------------------------------------------------------------------------------------
-- interface

-- 系统调用，默认的道具生成信息初始化（服务端&客户端）
function Item:InitGenInfo(szClassName)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return	tbClass:InitGenInfo();
end

-- 系统调用，检查是否允许使用道具（服务端&客户端）
function Item:CheckUsable(szClassName)
	local nMapId = -1;
	--if (MODULE_GAMESERVER) then
	--	nMapId = me.dwSceneId;
	--else
		nMapId = me.dwSceneTemplateId;
	--end
	print("check usable,MapId:",nMapId);
	local szForbidType = it.szForbidType or "NoLimit";
	print("szForbidType:",szForbidType);
  	local nCanUse = self:CheckUseableAtMap(nMapId, szForbidType);
	if nCanUse ~= 1 then
		me.SysMsg(Lang.item.str133[Lang.Idx]);
		return 0;
	end

	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return	tbClass:CheckUsable();
end

-- 当右键点击任何一个物品时，都会调用这里（仅限服务端）
function Item:OnUse(szClassName, nParam)
	local tbClass = self.tbClass[szClassName];
	local nOnUseFlag = 1;
	if (not tbClass) or (tbClass.OnUse == Item.tbClassBase.OnUse) then
		nOnUseFlag = 0; --原来没有OnUse
	end
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
--	local szGenIdName = string.format("%s,%s,%s", it.nGenre, it.nDetail, it.nParticular);
--	if EventManager.tbFun:CheckItemClassEventIsEffect(szClassName, 1) == 1 or EventManager.tbFun:CheckItemClassEventIsEffect(szGenIdName, 2) == 1 then
--		--发现有嵌入事件
--		local tbOpt = {};
--		local tbEventClass = EventManager:GetItemClass(szClassName);
--		for nEventId, tbPart in pairs(tbEventClass) do
--			for nPartId, tbEvent in pairs(tbPart) do
--				if EventManager.tbFun:CheckItemPartEventIsEffect(tbEvent) == 1 then
--					table.insert(tbOpt, {tbEvent.tbEventPart.szName, self.OnUseEventManagerEx, self, tbEvent, it.dwId, nParam})
--				end
--			end
--		end
--		local tbEventClass = EventManager:GetItemIdClass(szGenIdName);
--		for nEventId, tbPart in pairs(tbEventClass) do
--			for nPartId, tbEvent in pairs(tbPart) do
--				if EventManager.tbFun:CheckItemPartEventIsEffect(tbEvent) == 1 then
--					table.insert(tbOpt, {tbEvent.tbEventPart.szName, self.OnUseEventManagerEx, self, tbEvent, it.dwId, nParam})
--				end
--			end
--		end
--		if nOnUseFlag == 0 then
--			if #tbOpt == 1 then
--				local tbCallBack = {unpack(tbOpt[1], 2)};
--				Lib:CallBack(tbCallBack);
--				return 0;
--			end
--			table.insert(tbOpt, {"我再想想"});
--			Dialog:Say("你好，有什么可以帮到你呢？", tbOpt);
--			return 0;
--		else
--			table.insert(tbOpt, {"我想看看其他功能", self.OnUseEventManagerEx, self, tbClass, it.dwId, nParam});
--			table.insert(tbOpt, {"我再想想"});
--			Dialog:Say("你好，有什么可以帮到你呢？", tbOpt);
--		end
--		return 0;
--	end
	return	tbClass:OnUse(nParam);
end

function Item:CanResizeExtBag(pPlayer, nRoom, nWidth, nHeight)
	local nBagSize = pPlayer.GetExtBag();
	assert(nWidth > nBagSize);
	local nCellNum = nWidth - nBagSize;
	local nPrice = 5 * nCellNum;
	local nBindCoinNum = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_BIND_COIN)
	
	--if nBindCoinNum >= nPrice * 2 then  -- 优先消耗绑定元宝
	--	me.pPlayerPurse.AddMoney(Purse.EM_MONEY_BIND_COIN, -nPrice * 2);
	--	return 1;
	--end
	
	if me.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nPrice then
		me.SendBlackMsg(string.format(Lang.item.str134[Lang.Idx], nPrice));
		return 0;
	end

	me.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nPrice);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nPrice,40);--ldy:
	return 1;
end

--活动系统嵌入
function Item:OnUseEventManagerEx(tbClass, dwId, nParam)
	local pItem = KItem.GetObjById(dwId);
	if not pItem then
		return 0;
	end
	Setting:SetGlobalObj(nil, nil, pItem);
	if me.IsHaveItemInBags(pItem) ~= 1 then
		return 0;
	end
	if tbClass:OnUse(nParam) == 1 then
		if it.nCount <= 1 then
			me.DelItem(it, Player.emKLOSEITEM_USE);
		else
			it.SetCount(it.nCount - 1, Item.emITEM_DATARECORD_REMOVE);
		end
	end
	Setting:RestoreGlobalObj();
end


function Item:PickUpItem(nGenre, nDetail, nParticular, nLevel, nAmount)

	KItem.AddPlayerItem(me, nGenre, nDetail, nParticular, nLevel, nAmount);

	if me.IsBZonePlayer() == 1 then
		KCrossSv.RecordDropItems(me, nGenre, nDetail, nParticular, nLevel, nAmount);
	end

	return 1;
end

function Item:OnClientUse(szClassName)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return	tbClass:OnClientUse();
end

-- 当鼠标移动到任何一个物品图标上时，都触发这里获取Tip（客户端）
function Item:GetTip(szClassName, nState, szBindType)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	local nUnLockIntervalTimes = it.GetUnlockTime() or 0;
	local nNowTime = GetTime();
	local szTip = "";
	local nTipState = nState;
	local nShopId = me.GetShopId();
	if (nState == Item.TIPS_SHOP) and Shop:GetGoods(nShopId, it.nIndex) then
		nTipState = Item.TIPS_GOODS;
	end

	local tbEnhRandMASS, tbEnhEnhMASS, nEnhStarLevel, nEnhNameColor;
	if tbClass.CalcEnhanceAttrib then		-- TODO: zyh: 很龊的方法……暂时先这样做
		tbEnhRandMASS, tbEnhEnhMASS, nEnhStarLevel, nEnhNameColor = tbClass:CalcEnhanceAttrib(nTipState);
		local szTitel = tbClass:GetTitle(nTipState, nEnhNameColor);
		szTip = szTip..tbClass:GetTip(nTipState, tbEnhRandMASS, tbEnhEnhMASS);
		if szTip and szTip ~= "" then
			szTip = "\n\n"..szTip;
		end
		szTip = self:Tip_Prefix(nTipState, 0, szBindType)..szTip..self:Tip_Suffix(nState);
		if nUnLockIntervalTimes > 0 and it.GetLockIntervale() ~= 0 then
			if nUnLockIntervalTimes - nNowTime > 0 then
				szTip =szTip..Lang.item.str135[Lang.Idx]..os.date("%Y/%m/%d %H:%M:%S", nUnLockIntervalTimes).."<color>";
			else
				szTip =szTip..Lang.item.str136[Lang.Idx];
			end
		end
		return szTitel, szTip, it.szViewImage;
	else
		local szTitel = tbClass:GetTitle(nTipState); --物品名称
		local szTip = "";
		szTip = szTip..tbClass:GetTip(nTipState);
		if szTip and szTip ~= "" then
			szTip = "\n\n"..szTip;
		end
		szTip = self:Tip_Prefix(nTipState, 0, szBindType)..szTip..self:Tip_Suffix(nState);
		if nUnLockIntervalTimes > 0 and it.GetLockIntervale() ~= 0 then
			if nUnLockIntervalTimes - nNowTime > 0 then
				szTip =szTip..Lang.item.str135[Lang.Idx]..os.date("%Y/%m/%d %H:%M:%S", nUnLockIntervalTimes).."<color>";
			else
				szTip =szTip..Lang.item.str136[Lang.Idx];
			end
		end
		return szTitel, szTip, it.szViewImage;
	end
end

function Item:GetCompareTip(szClassName, nState, szBindType)
	local szTitle, szTip, szPic = self:GetTip(szClassName, nState, szBindType);
	local szCmpTitle = "";
	local szCmpTip = "";
	local szCmpPic = "";
	if it.IsEquip() == 1 then
		local nEquipPos = it.nEquipPos;
		if nEquipPos == -1 and it.nDetail == Item.EQUIP_ZHENYUAN then
			nEquipPos = Item.EQUIPPOS_ZHENYUAN_MAIN;	-- 真元的话，始终跟主真元格子的比
		end

		local pItem = nil;
		pItem = me.GetItem(Item.ROOM_EQUIP, nEquipPos, 0);

		if pItem and pItem.dwId ~= it.dwId then
			szCmpTitle, szCmpTip, szCmpPic = pItem.GetTip(Item.TIPS_NORMAL);
			szCmpTip = szCmpTip..Lang.item.str137[Lang.Idx];
		end
	end
	return szTitle, szTip, szPic, szCmpTitle, szCmpTip, szCmpPic;
end

-- 判断一个物品是否可以被拣起（服务端）
function Item:IsPickable(szClassName, nObjId)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return	tbClass:IsPickable(nObjId);
end

-- 当一个物品被拣起时会处理这里，同决定是否删除该物品（服务端）
function Item:PickUp(szClassName, nX, nY)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return	tbClass:PickUp(nX, nY);
end

-- 计算道具价值量相关信息，仅在道具生成时执行一次
-- 返回值：价值量，价值量星级，名字颜色，透明图层路径
function Item:CalcValueInfo(szClassName)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return tbClass:CalcValueInfo();
end

-----------------------------------------------------------------------------------------
-- public

-- 取得特定类名的Item模板
function Item:GetClass(szClassName, bNotCreate)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) and (bNotCreate ~= 1) then		-- 如果没有bNotCreate，当找不到指定模板类时会自动建立新模板类
		tbClass	= Lib:NewClass(self.tbClassBase);	-- 新模板从父模板类派生
		self.tbClass[szClassName] = tbClass;		-- 加入到模板库里面
	end
	return	tbClass;
end

-- 继承特定类名的Item模板
function Item:NewClass(szClassName, szParent)
	if (self.tbClass[szClassName]) then				-- 指定模板类必须还不存在
		print("[ITEM] 类"..tostring(szClassName).."已存在，请检查脚本！");
		return;
	end
	local tbParent = self.tbClass[szParent];
	if (not tbParent) then							-- 父模板类必须已经存在
		print("[ITEM] 父类"..tostring(szParent).."不存在，请检查脚本！");
		return;
	end
	local tbClass = Lib:NewClass(tbParent);			-- 从父模板类派生
	self.tbClass[szClassName] = tbClass;			-- 加入到模板库里面
	return	tbClass;
end

-- 转换装备位置为对应的切换装备位置
function Item:EqPos2EqExPos(nEquipPos)
	if (nEquipPos < 0) or (nEquipPos > self.EQUIPEXPOS_NUM) then
		return	-1;
	end
	return	nEquipPos;
end

-- 转换切换装备位置为对应的装备位置
function Item:EqExPos2EqPos(nEquipExPos)
	if (nEquipExPos < 0) or (nEquipPos > self.EQUIPEXPOS_NUM) then
		return	-1;
	end
	return	nEquipExPos;
end

function Item:IsTry(nEquipPos)
	if (nEquipPos >= Item.EQUIPPOS_HEAD and nEquipPos <= Item.EQUIPPOS_MASK) then
		return 1;
	else
		return 0;
	end
end

-- 把剑侠币数值格式化为字符串（显示“万”、“亿”等字）
function Item:FormatMoney(nMoney)
	local szMoney = "";
	if (not nMoney) or (nMoney < 0) then
		return	Lang.item.str138[Lang.Idx];								-- 价钱小于0，出错
	end
	if (nMoney >= 100000000) then
		szMoney = szMoney..tostring(math.floor(nMoney / 100000000))..Lang.item.str139[Lang.Idx];
		nMoney = nMoney % 100000000;
	end
	if (nMoney >= 10000) then
		szMoney = szMoney..tostring(math.floor(nMoney / 10000))..Lang.item.str140[Lang.Idx];
		nMoney = nMoney % 10000;
	end
	if (nMoney > 0) then
		szMoney = szMoney..tostring(nMoney);
	end
	if (szMoney == "") then
		szMoney = "0";
	end
	return	szMoney;
end

function Item:Tip_FixSeries()
	local szTip = Lang.item.str141[Lang.Idx]
	if Item.tbSeriesFix[it.nEquipPos] and it.nSeries > 0 then
		return szTip..Item.TIP_SERISE[Item.tbSeriesFix[it.nEquipPos][it.nSeries]]..Lang.item.str142[Lang.Idx];
	end
	return "";
end

function Item:FindFreeCellInRoom(nRoom, nWidth, nHeight)
	for nY = 0, nHeight - 1 do
		for nX = 0, nWidth - 1 do
			if me.GetItem(nRoom, nX, nY) == nil then
				return nX, nY;
			end
		end
	end
end
function Item:FormatMoney2Style(nMoney)

	if (not nMoney) or nMoney < 0 then
		return Lang.item.str138[Lang.Idx];
	end
	local k = 0;
  	local nFormatted = nMoney
  	while true do
  	  nFormatted, k = string.gsub(nFormatted, "^(-?%d+)(%d%d%d)", '%1,%2')
   	 if (k==0) then
    	  break
    	end
  	end
  return nFormatted;
end

function Item:Tip_Prefix(nState, nEnhStarLevel, szBindType)
	local szPreTip = "";
	if it.IsEquip() == 1 then
		szPreTip = szPreTip..self:Tip_StarLevel(nState, nEnhStarLevel);
		szPreTip = szPreTip..self:Tip_FightPower(nState);
		szPreTip = szPreTip..self:Tip_BindInfo(nState, szBindType);	-- 绑定状态
		szPreTip = szPreTip..self:Tip_Changeable(nState)..self:Tip_CanBreakUp(nState);  -- 可否兑换
		if it.IsZhenYuan() ~= 1 then
			szPreTip = szPreTip..self:Tip_GetRefineLevel();
		end
		szPreTip = szPreTip..self:Tip_FixSeries(nState);
	else
		if (it.IsPartnerEquip() == 1) then
			szPreTip = szPreTip..self:Tip_FightPower();
		end
		szPreTip = szPreTip..self:Tip_BindInfo(nState, szBindType);
		szPreTip = szPreTip..self:Tip_Changeable(nState);  -- 可否兑换
	end
	return szPreTip;
end

function Item:Tip_GetRefineLevel(nState)
	local szTip = " ";
	if it.nRefineLevel == 0 then
		return szTip..Lang.item.str143[Lang.Idx];
	elseif it.nRefineLevel > 0 then
		return szTip..string.format(Lang.item.str144[Lang.Idx],it.nRefineLevel)
		--return szTip.."炼化<color=gold>"..it.nRefineLevel.."<color>级"
	else
		return szTip..Lang.item.str145[Lang.Idx];
	end
end

function Item:Tip_Changeable(nState)
	local szTip = "\n";
	if (nState == Item.TIPS_PREVIEW) then
		return szTip;
	elseif (nState == Item.TIPS_BINDGOLD_SECTION) then
		return szTip..Lang.item.str146[Lang.Idx];
	end

	if Item:CalcChange({it}) > 0 then
		return szTip..Lang.item.str147[Lang.Idx];
	else
		return szTip..Lang.item.str146[Lang.Idx];
	end
end

function Item:Tip_Suffix(nState)
	local szTip = "";
	szTip = szTip..self:Tip_Intro();
	szTip = szTip..self:Tip_UseTime();
	szTip = szTip..self:Tip_Help();
	szTip = szTip..self:Tip_ReqLevel();
	szTip = szTip..self:Tip_Price(nState);
	szTip = szTip..self:Tip_Timeout();
	return	szTip;
end

function Item:Tip_StarLevel(nState, nEnhStarLevel)	-- 获得Tip字符串：装备价值量星级
	local tbSetting = Item.tbExternSetting:GetClass("value");
	--if (nState == Item.TIPS_PREVIEW) then
	--	return	"";			-- 属性预览状态时不显示价值量星级
	--end

	local szTip = "\n  ";
	local nStarLevel = it.nStarLevel;
	local szFillStar = "";
	local szEmptyStar = "";
	if tbSetting and tbSetting.m_tbStarLevelInfo and tbSetting.m_tbStarLevelInfo[nStarLevel] then
		szFillStar = string.format("<pic=%s>", tbSetting.m_tbStarLevelInfo[nStarLevel].nFillStar - 1);
		szEmptyStar = string.format("<pic=%s>", tbSetting.m_tbStarLevelInfo[nStarLevel].nEmptyStar - 1);
	else
		szFillStar = "★";
		szEmptyStar = "☆";
	end

	for i = 1, math.floor(nStarLevel / 2) do
		szTip = szTip..szFillStar;
		if i % 3 == 0 then
			szTip = szTip.." ";
		end
	end
	if (it.nStarLevel % 2 ~= 0) then
		szTip = szTip..szEmptyStar;
	end
	return	szTip;

end

function Item:Tip_BindInfo(nState, szBindType)
	local szTip = "";
	if it.IsEquip() == 1 then
		local nPos = KItem.EquipType2EquipPos(it.nDetail)
		if nPos < 0 and it.nDetail == Item.EQUIP_ZHENYUAN then
			nPos = me.GetZhenYuanPos();
		end
		szTip = Item.EQUIPPOS_NAME[nPos];
		if nPos == self.EQUIPPOS_WEAPON and self.WEAPON_NAME[it.nEquipCategory] then
			szTip = szTip.."（"..self.WEAPON_NAME[it.nEquipCategory].."） ";
		else
			szTip = szTip.." ";
		end
	end
	local nShopId = me.GetShopId();
	local nGoodsId = Shop:GetGoods(nShopId, it.nIndex);
	local nCurrencyType	= 0;
	if (nGoodsId) then		-- 只有买入时有不同的货币单位
		nCurrencyType	= me.nCurrencyType;
	end

	if (szBindType) then
		szTip = szTip..szBindType;
	elseif (nState == Item.TIPS_GOODS and nGoodsId and (KItem.IsGetBindType(nGoodsId) == 1 or nCurrencyType == 7)) then
		szTip	= szTip..Lang.item.str148[Lang.Idx];
	else
		local nBindType = it.nBindType;
		if (nState == Item.TIPS_BINDGOLD_SECTION) then
			szTip = szTip..Lang.item.str148[Lang.Idx];

		elseif (nState ~= Item.TIPS_PREVIEW) and (1 == it.IsBind()) then	-- 属性预览状态时只按照nBindType显示
			szTip = szTip..Lang.item.str149[Lang.Idx];
		elseif	(Item.BIND_NONE  == nBindType or Item.BIND_NONE_OWN == nBindType) then
			szTip = szTip..Lang.item.str150[Lang.Idx];
		elseif	(Item.BIND_GET   == nBindType) then
			szTip = szTip..Lang.item.str148[Lang.Idx];
		elseif	(Item.BIND_EQUIP == nBindType) then
			szTip = szTip..Lang.item.str151[Lang.Idx];
		elseif  (Item.BIND_OWN == nBindType) then
			szTip = szTip..Lang.item.str148[Lang.Idx];
		end
	end
	if szTip ~= "" then
		szTip = "\n"..szTip;
	end
	return	szTip;
end

function Item:Tip_FightPower(nState)	-- 获得Tip字符串：战斗力
	if (1 ~= Player.tbFightPower:IsFightPowerValid()) then
		return "";
	end

	local nFightPower, nPowerEncPreview = self:GetItemFightPower();
	local szTip = "";
	if (nFightPower) then
		nFightPower = math.floor(nFightPower * 100) / 100;
		szTip = string.format(Lang.item.str152[Lang.Idx], nFightPower);
		if it.szClass == "zhenyuan" then
			szTip = szTip..string.format(Lang.item.str153[Lang.Idx], Item.tbZhenYuan:GetRank(it));
		end

		if (nState == Item.TIPS_ENHANCE and nPowerEncPreview) then
			nPowerEncPreview = math.floor(nPowerEncPreview * 100) / 100;
			szTip = szTip .. string.format(" <color=blue>→ +%g<color>", nPowerEncPreview);
		end
		szTip = szTip .. "\n";
	end
	return szTip;
end

function Item:Tip_CanBreakUp(nState)
	local szTip = "";
	local nGTPCost, tbStuff, tbExp = Item:CalcBreakUpStuff(it);
	if (nGTPCost <= 0) or (#tbStuff <= 0) then
		szTip = szTip..Lang.item.str154[Lang.Idx];
	else
		szTip = szTip..Lang.item.str155[Lang.Idx]
	end
	if szTip ~= "" then
		szTip = ""..szTip;
	end
	return	szTip;
end

function Item:Tip_Intro()
	local szIntro = it.szIntro;
	if szIntro and szIntro ~= "" then
		return "\n\n"..szIntro;
	end
	return "";
end

function Item:Tip_Help()			-- 获得Tip字符串：帮助文字
	if it.szHelp and it.szHelp ~= ""then
		return	"\n\n"..it.szHelp;
	end
	return "";
end

function Item:Tip_UseTime()		-- 获得Tip字符串：使用时间
	local szTip = "";
	local nTime = GetCdTime(it.nCdType) / Env.GAME_FPS;

	if (nTime ~= 0) then

		local nHour = math.floor(nTime / 3600);
		nTime = nTime % 3600;
		local nMin  = math.floor(nTime / 60);
		nTime = nTime % 60;
		local nSec  = math.floor(nTime * 10) / 10;		-- 精确到小数点后一位

		local szTime = "";
		if (nHour ~= 0) then
			szTime = szTime..nHour..Lang.item.str156[Lang.Idx];
		end
		if (nMin ~= 0) then
			szTime = szTime..nMin..Lang.item.str157[Lang.Idx];
		end
		if (nSec ~= 0) then
			szTime = szTime..nSec..Lang.item.str158[Lang.Idx];
		end

		szTip = "\n\n"..szTip..Lang.item.str159[Lang.Idx]..szTime;

	end

	return	szTip;
end

function Item:Tip_ReqLevel()

	if (it.nReqLevel <= 0) then
		return	"";
	end

	local szTip = "";
	local nRed = 0;

	if (me.nLevel < it.nReqLevel) then
		nRed = 1;
	end

	if (nRed == 1) then
		szTip = szTip.."<color=red>";
	end

	szTip = "\n"..szTip..string.format(Lang.item.str160[Lang.Idx],it.nReqLevel);
	--szTip = "\n"..szTip.."使用等级需求："..it.nReqLevel.."级";

	if (nRed == 1) then
		szTip = szTip.."<color>";
	end

	return	szTip;

end

--临时增加使用, 2008.11.18后将被删除 dzh
function Item:SendDisableTrade(szMsg)
	--me.Msg("由于披风过于贵重，运送过程需十分小心，因此最近一批披风要等到最近一次维护后才能送到。");
end

function Item:Tip_Price(nState)

	local szTip = "";
 	local szUnit = "";

	if (nState == Item.TIPS_SHOP) then	-- NPC交易状态显示物品价格
		local nShopId = me.GetShopId();
		local pGoodsId = Shop:GetGoods(nShopId, it.nIndex);

		local nCurrencyType	= 1;	-- 卖出永远是剑侠币
		if pGoodsId then		-- 只有买入时有不同的货币单位
			nCurrencyType = me.nCurrencyType;
		end

		szUnit = self.tbTipPriceUnit[nCurrencyType] or "";
		szTip = szTip.."<color=yellow>";

		if pGoodsId then
			local tbGoods = me.GetShopBuyItemInfo(pGoodsId);
			local nPrice = tbGoods.nPrice;
			local nCamp = tbGoods.nReputeCamp;
			local nClass = tbGoods.nReputeClass;
			local nLevel = tbGoods.nReputeLevel;
			local nRequireHonor = tbGoods.nHonour;
			local nFightPowerRankNeed = tbGoods.nFightPowerRank;
			local nOfficialLevel = tbGoods.nOfficialLevel;
			local nEnergy = tbGoods.nEnergy;

			-- 实物货币单位
			if (nCurrencyType == 3) then
				local nItemCoinIndex = tbGoods.ItemCoinIndex;
				if (szUnit ~= "") then
					szUnit = string.format(szUnit, Shop:GetItemCoinUnit(nItemCoinIndex));
				end
			end

			-- 数值货币单位
			if (nCurrencyType == 10) then
				local nValueCoinIndex = tbGoods.ValueCoinIndex;
				if (szUnit ~= "") then
					szUnit = string.format(szUnit, Shop:GetValueCoinUnit(nValueCoinIndex));
				end
			end

			szTip = "\n\n"..szTip..Lang.item.str161[Lang.Idx]..self:FormatMoney(nPrice)..szUnit.."<color>";
			if nCurrencyType == 9 and nEnergy > 0 then
				szTip = szTip..string.format(Lang.item.str162[Lang.Idx], nEnergy);
			end
			-- 声望需求
			local bConditionLevel = false;
			if nLevel > 0 then
				bConditionLevel = true;
				local tbInfo = KPlayer.GetReputeInfo();
				if me.GetReputeLevel(nCamp, nClass) >= nLevel then
					szTip = szTip.."\n\n<color=white>"..Lang.item.str163[Lang.Idx];
				else
					szTip = szTip.."\n\n<color=red>"..Lang.item.str163[Lang.Idx];
				end
				if tbInfo then
					szTip = szTip..tbInfo[nCamp][nClass].szName..string.format(Lang.item.str164[Lang.Idx],tbInfo[nCamp][nClass][nLevel].szName,nLevel);
					--szTip = szTip..tbInfo[nCamp][nClass].szName.."声望达到"..tbInfo[nCamp][nClass][nLevel].szName.."["..nLevel.."级]";
				else
					szTip = szTip..Lang.item.str165[Lang.Idx];
				end
				szTip = szTip.."<color>";
			end

			--增加荣誉支持
			--local nRequireHonor = 0 --debug
			if nRequireHonor > 0 then
				if me.GetHonorLevel() >= nRequireHonor then
					szTip = szTip.."\n<color=white>";
				else
					szTip = szTip.."\n<color=red>";
				end
				if bConditionLevel == false then
					szTip = szTip..Lang.item.str166[Lang.Idx];
				end

				local strcondfmt = Lang.item.str167[Lang.Idx];
				local strcond = string.format(strcondfmt, nRequireHonor, nRequireHonor, nRequireHonor);
				szTip = szTip..strcond;

				szTip = szTip.."<color>";
			end
			if nOfficialLevel > 0 then
				local nLevel = me.GetOfficialLevel()
				local szColor = "white";
				if nOfficialLevel > nLevel then
					szColor = "red";
				end
				szTip = szTip..string.format(Lang.item.str168[Lang.Idx], szColor, nOfficialLevel);
			end

			if nFightPowerRankNeed > 0 then
				local nRank = Player.tbFightPower:GetFightPowerRank();
				local szColor = "white";
				if nRank <= 0 or nRank > nFightPowerRankNeed then
					szColor = "red";
				end
				szTip = szTip..string.format(Lang.item.str169[Lang.Idx], szColor, nFightPowerRankNeed);
			end
		else
			local nPrice = me.GetRecycleItemPrice(it)
			if nPrice then
				szTip = "\n"..szTip..Lang.item.str170[Lang.Idx]..nPrice..szUnit.."<color>";
			else
				nPrice = GetSalePrice(it);
				if it.IsForbitSell() == 1 then
					szTip = Lang.item.str171[Lang.Idx];
				else
          --卖道具需要判断道具的绑定属性
					if 1 == it.IsBind() then
					  szUnit = Lang.item.str130[Lang.Idx];
					else
					  szUnit = Lang.item.str60[Lang.Idx];
					end
					szTip = "\n"..szTip..Lang.item.str185[Lang.Idx]..self:FormatMoney(nPrice)..szUnit.."<color>";
					if it.nEnhTimes > 0 then -- 强化过的装备f售店
						szTip = szTip..Lang.item.str186[Lang.Idx];
					elseif it.IsPartnerEquip() == 1 and it.IsBind() == 1 then	-- 锁定的同伴装备不能卖店
						szTip = szTip..Lang.item.str186[Lang.Idx];
					end
				end
			end
		end

	elseif (nState == Item.TIPS_STALL) then			-- 摆摊状态显示摆摊信息

		local nPrice = me.GetStallPrice(it);		-- 先看是不是自己摆摊的东西

		if not nPrice then							-- 如果不是，看看是不是别人摆摊的东西
			local _, _, tbInfo = me.GetOtherStallInfo();
			if tbInfo then
				for i = 1, #tbInfo do
					if (tbInfo[i].uId == it.nIndex) and (tbInfo[i].nPrice >= 0) then
						nPrice = tbInfo[i].nPrice;
						break;
					end
				end
			end
		end

		if nPrice then								-- 如果都不是，就不显示摆摊信息
			szUnit = self.tbTipPriceUnit[1];---摆摊交易都是银两单位
			szTip = "\n"..szTip..Lang.item.str173[Lang.Idx]..self:FormatMoney2Style(nPrice)..szUnit.."("..self:FormatMoney(nPrice)..szUnit..")<color>";
			local nTotal = nPrice * it.nCount;
			szTip =  szTip .. Lang.item.str174[Lang.Idx]..self:FormatMoney2Style(nTotal)..szUnit.."("..self:FormatMoney(nTotal)..szUnit..")<color>";
		end

	elseif (nState == Item.TIPS_OFFER) then			-- 收购状态显示收购信息

		local nPrice, nCount = me.GetOfferReq(it);	-- 先看是不是自己收购的东西

		if (not nPrice) or (not nCount) then		-- 如果不是，再看是不是别人收购的东西
			local _, _, tbInfo = me.GetOtherOfferInfo();
			if tbInfo then
				for i = 1, #tbInfo do
					if (tbInfo[i].uId == it.nIndex) and (tbInfo[i].nPrice >= 0) then
						nPrice = tbInfo[i].nPrice;
						nCount = tbInfo[i].nNum;
						break;
					end
				end
			end
		end

		if nPrice and nCount then					-- 如果都不是，不显示收购信息
			szUnit = self.tbTipPriceUnit[1];---摆摊交易都是银两单位
			szTip = "\n"..szTip..string.format(Lang.item.str175[Lang.Idx],nCount)..self:FormatMoney2Style(nPrice)..szUnit.."("..self:FormatMoney(nPrice)..szUnit..") <color>";
			--szTip = "\n"..szTip.."<color=yellow>收购数量："..nCount.."个\n收购单价："..self:FormatMoney2Style(nPrice)..szUnit.."("..self:FormatMoney(nPrice)..szUnit..") <color>";
			local nTotal = nPrice * nCount;
			szTip =  szTip .. Lang.item.str176[Lang.Idx]..self:FormatMoney2Style(nTotal)..szUnit.."("..self:FormatMoney(nTotal)..szUnit..")<color>";
		end

	elseif (nState == Item.TIPS_NOBIND_SECTION or nState == Item.TIPS_BINDGOLD_SECTION) then
		local tbWareInfo = me.GetIbShopItemInfo(it.nIndex);
		if (tbWareInfo and tbWareInfo.nDiscount > 0) then
			local szTemp = "";
			if (tbWareInfo.nDiscountType == 0) then
				szTemp = tbWareInfo.nDiscount.."%";
			else
				if (tbWareInfo.nCurrencyType == 0) then
					szTemp = tbWareInfo.nDiscount .. IVER_g_szCoinName;
				elseif (tbWareInfo.nCurrencyType == 1) then
					szTemp = tbWareInfo.nDiscount .. Lang.item.str60[Lang.Idx];
				elseif (tbWareInfo.nCurrencyType == 2) then
					szTemp = tbWareInfo.nDiscount .. Lang.item.str177[Lang.Idx]..IVER_g_szCoinName;
				else
					assert(false);
				end
			end
			if (tbWareInfo.nDiscStart ~= tbWareInfo.nDiscEnd and GetTime() <= tbWareInfo.nDiscEnd) then
				local szStartDate = os.date(Lang.item.str187[Lang.Idx], tbWareInfo.nDiscStart);
				local szEndDate  = os.date(Lang.item.str187[Lang.Idx], tbWareInfo.nDiscEnd);
				szTip = szTip..string.format(Lang.item.str178[Lang.Idx],szTemp,szStartDate,szEndDate);
			end
		end
	end

	return	szTip;

end

function Item:Tip_Timeout()			-- 超时时间

	local szTip = "";
	local tbAbsTime = me.GetItemAbsTimeout(it);

	if tbAbsTime then
		local strTimeout = string.format(Lang.item.str179[Lang.Idx],
			tbAbsTime[1],
			tbAbsTime[2],
			tbAbsTime[3],
			tbAbsTime[4],
			tbAbsTime[5]);
		szTip = "\n\n"..szTip..strTimeout;
	else
		local tbRelTime = me.GetItemRelTimeout(it);
		if tbRelTime then
			local strTimeout = string.format(Lang.item.str180[Lang.Idx],
				tbRelTime[1],
				tbRelTime[2],
				tbRelTime[3]);
			szTip = "\n\n"..szTip..strTimeout;
		end
	end

	return	szTip;

end

--地图,物品限制,禁止在当前地图使用
function Item:CheckUseableAtMap(nMapId, szForbidType)   --ldy
	local szSceneType = "";
	local szForbidScene= "";
	local szProperScene= "";
	local tbForbidScene = {};
	local tbProperScene = {};
	local tbMapList = Lib:LoadTabFile("/scene/scenelist.txt");
	local tbUseLimitList = Lib:LoadTabFile("/setting/item/uselimit.txt");
	for _, tbRow in pairs(tbMapList) do
		if tonumber(tbRow.Id) == nMapId then
			szSceneType = tbRow.SceneType or "default";
			break;
		end
	end

	for _, tbRow in pairs(tbUseLimitList) do
		if tbRow.LimitName == szForbidType then
			szForbidScene = tbRow.ForbidScene;
			szProperScene = tbRow.ProperScene;
			break;
		end
	end

	if (szProperScene and szProperScene ~= "") then
		tbProperScene = Lib:SplitStr(szProperScene, ",");
		for i = 1, #tbProperScene do
			if tbProperScene[i] == szSceneType then
				return 1;
			end
		end
		return 0;
	elseif (szForbidScene and szForbidScene ~= "")then
		tbForbidScene = Lib:SplitStr(szForbidScene, ",");
		for i = 1, #tbForbidScene do
			if tbForbidScene[i] == szSceneType then
				return 0;
			end
		end
		return 1;
	end
	return 1;

	--local nCheck, szMsg, szMapClass, szItemClass = self:CheckForBidItemAtMap(nMapId, unpack(arg));
	--if not nCheck or nCheck == 0 then
	--	return nCheck, szMsg;
	--end
	--if nCheck == 2 then
	--	return 1;
	--end
	--if Map.tbMapItemState[szMapClass].tbForbiddenUse[szItemClass] ~= nil then
	--	return 0, Lang.item.str181[Lang.Idx];
	--end
    --
	--return 1;
end

--召唤类调用,是否允许自己被别人召唤出地图,[禁止被召唤出去]
--(为了清晰召出和召入分开2个函数，而不选择通过参数来进行判断的做法。)
function Item:IsCallOutAtMap(nMapId, ...)
	local szForbitType = "";
	if type(arg[1]) == "string" then
		szForbitType = arg[1]
	elseif #arg == 4 then
		szForbitType = KItem.GetOtherForbidType(unpack(arg));
	end

	if not szForbitType or szForbitType == "" then
		return 0, Lang.item.str181[Lang.Idx];
	end

	-- 要先判断该道具能不能在指定地图使用
	local nCanUse = KItem.CheckLimitUse(nMapId, szForbitType);
	if nCanUse == 0 then
		return 0, Lang.item.str181[Lang.Idx];
	end

	-- 再判断是否被禁止传出
	local szMapClass = GetMapType(nMapId);
	if Map.tbMapItemState[szMapClass] and Map.tbMapItemState[szMapClass].tbForbiddenCallOut[szItemClass] ~= nil then
		return 0, Lang.item.str181[Lang.Idx];
	end
	return 1;
end

--召唤类调用,是否允许召唤别人进入地图和 是 否允许使用物品传入地图
function Item:IsCallInAtMap(nMapId, ...)
	local szForbitType = "";
	if type(arg[1]) == "string" then
		szForbitType = arg[1]
	elseif #arg == 4 then
		szForbitType = KItem.GetOtherForbidType(unpack(arg));
	end

	if not szForbitType or szForbitType == "" then
		return 0, Lang.item.str181[Lang.Idx];
	end

	-- 要先判断该道具能不能在指定地图使用
	local nCanUse = KItem.CheckLimitUse(nMapId, szForbitType);
	if nCanUse == 0 then
		return 0, Lang.item.str181[Lang.Idx];
	end

	-- 再判断是否被禁止传入
	local szMapClass = GetMapType(nMapId);
	if Map.tbMapItemState[szMapClass] and Map.tbMapItemState[szMapClass].tbForbiddenCallIn[szForbitType] ~= nil then
		return 0, Lang.item.str181[Lang.Idx];
	end

	return 1;
end

Item.ForBidUseAtMap = {
	["mask"]= "MASK";
}

function Item:CheckForBidItemAtMap(nMapId, ...)
	local szItemClass = "";
	if #arg == 4 then
		szItemClass = KItem.GetOtherForbidType(unpack(arg)) or "";
	elseif #arg == 1 then
		if type(arg[1]) == "string" then
			szItemClass = arg[1] or "";
		elseif type(arg[1]) == "number" then
			local pItem = KItem.GetObjById(arg[1]);
			if pItem == nil then
				return 0, Lang.item.str181[Lang.Idx];
			end
			if self.ForBidUseAtMap[pItem.szClass] then
				szItemClass = self.ForBidUseAtMap[pItem.szClass];
			else
				szItemClass = pItem.GetForbidType() or "";
			end
		else
			return 0, Lang.item.str181[Lang.Idx];
		end
	else
		return 0, Lang.item.str181[Lang.Idx];
	end

	if (self:CheckDynamicForbiden(nMapId, szItemClass) == 1) then
		return 0, Lang.item.str181[Lang.Idx];
	end

	local szMapClass = GetMapType(nMapId) or "";

	if Map.tbMapItemState[szMapClass] == nil then
		return 2, "", szMapClass, szItemClass;
	end
	if Map.tbMapProperItem[szItemClass] then
		 if Map.tbMapProperItem[szItemClass] ~= szMapClass then
			--该物品为仅允许.
			local szInfo = Map.tbMapItemState[Map.tbMapProperItem[szItemClass]].szInfo;
			local szMsg = Lang.item.str181[Lang.Idx];
			if szInfo ~= "" then
				szMsg = string.format(Lang.item.str182[Lang.Idx],szInfo);
				--szMsg = "该道具仅允许在" .. szInfo .."使用";
			end
			return 0, szMsg;
		else
			return 1,"", szMapClass, szItemClass;
		end
	end

	if Map.tbMapItemState[szMapClass].tbForbiddenUse["ALL_ITEM"]  then
		return 0, Lang.item.str181[Lang.Idx];
	end

	return 1, "", szMapClass, szItemClass;
end


function Item:IsEquipRoom(nRoom)
	if nRoom == Item.ROOM_EQUIP or nRoom == Item.ROOM_EQUIPEX then
		return 1;
	end
	return 0;
end


-- UNDONE: Fanghao_Wu	临时代码，将药箱内药品数量*1.5，2008-9-1后删除！！！
function Item:OnLoaded(szClassName)
	local tbClass = self.tbClass[szClassName];
	if (szClassName == "xiang" and tbClass) then
		tbClass:OnLoaded();
	end
end

function Item:CheckCanChangable(pEquip)
	local tbSetting = Item:GetExternSetting("change", 1);
	local szGDPL = string.format("%d,%d,%d,%d", pEquip.nGenre, pEquip.nDetail, pEquip.nParticular, pEquip.nLevel);
	if not tbSetting.tbItemToChangeId or not tbSetting.tbItemToChangeId[szGDPL] then
		return;
	end
	local nId = tbSetting.tbItemToChangeId[szGDPL];
	if (not tbSetting.tbChange or
		not tbSetting.tbChange[nId])then
		return;
	end
	return tbSetting.tbChange[nId];
end

-- 将道具与帮会绑定
function Item:BindWithTong(pItem, nTongId)
	if pItem then
		pItem.SetGenInfo(Item.TASK_OWNER_TONGID, nTongId);
		pItem.Sync();
		return 1;
	end
end

-- 检查该道具是否可以被某帮会成员使用
function Item:IsBindItemUsable(pItem, nTongId)
	if pItem then
		local nOwnerTongId = KLib.Number2UInt(pItem.GetGenInfo(Item.TASK_OWNER_TONGID, 0));
		if nOwnerTongId == 0 or nOwnerTongId == nTongId then
			return 1;
		end
		local pTong = KTong.GetTong(nOwnerTongId);
		if pTong then
			me.Msg(string.format(Lang.item.str183[Lang.Idx],pTong.GetName()));
			--me.Msg("该道具已经和帮会["..pTong.GetName().."]绑定！");
		else
			me.Msg(Lang.item.str184[Lang.Idx]);
		end
		return 0;
	end
	return 0;
end


--检测动态注册的地图禁用，0：可用；1：不可用
function Item:CheckDynamicForbiden(nMapId, szClassName)
	local tbDynamicForbiden = Map.tbDynamicForbiden;
	if tbDynamicForbiden == nil or (tbDynamicForbiden[nMapId] == nil) then
		return 0;
	end
	if (tbDynamicForbiden[nMapId][szClassName] == nil) then
		return 0;
	end
	return 1;
end


--计算商品打折信息（对优惠券）
--返回值：打折商品数量，打折率
function Item:CalDiscount(szClassName, tbWareList)

	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return	tbClass:CalDiscount(tbWareList);
end

--消耗使用次数
function Item:DecreaseCouponTimes(szClassName, tbCouponWare)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return	tbClass:DecreaseCouponTimes(tbCouponWare);
end

function Item:CanCouponUse(szClassName, dwId)
	local tbClass = self.tbClass[szClassName];
	if (not tbClass) then
		tbClass = self.tbClass["default"];
	end
	return tbClass:CanCouponUse(dwId);
end

function Item:GetItemFightPower()
	if (1 == it.IsPartnerEquip()) then		-- 同伴装备
		return nil;	-- Edited by zhoupf 2010.09.26
		--return Player.tbFightPower:GetPartnerEquipPowerByItem(it);
	end
	if (1 ~= it.IsEquip()) then	-- 是不是装备
		return nil;
	end

	local nPos = it.nEquipPos;
	local nFightPower = nil;
	local nPowerEnhancePreview = nil;
	if ("zhenyuan" == it.szClass) then	-- 是装备的真元
		nFightPower = Item.tbZhenYuan:GetFightPower(it);
	elseif (10 > nPos) then
		nFightPower = it.CalcFightPower();
		if (it.nEnhTimes < 16) then
			local nDiff = it.CalcExtraFightPower(it.nEnhTimes + 1, 0) - it.CalcExtraFightPower(it.nEnhTimes, 0);
			nPowerEnhancePreview = nFightPower + nDiff;
		end
	elseif( Item.EQUIPPOS_MANTLE == nPos) then	-- 披风
		nFightPower = Player.tbFightPower:GetPiFengPower(me, it);
	elseif(Item.EQUIPPOS_SIGNET == nPos) then	-- 五行印
		nFightPower = Player.tbFightPower:Get5XingYinPower(me, it);
	elseif(Item.EQUIPPOS_BOOK == nPos) then		-- 秘籍
		nFightPower = Player.tbFightPower:GetMiJiPower(me, it);
	elseif(Item.EQUIPPOS_ZHEN == nPos) then		-- 阵法
		nFightPower = Player.tbFightPower:GetZhenFaPower(me, it);
	elseif(Item.EQUIPPOS_CHOP == nPos) then		-- 官印
		nFightPower = Player.tbFightPower:GetGuanYinPower(me, it);
	end
	return nFightPower, nPowerEnhancePreview;
end

function Item:GetExternSetting()
end

function Item:CalcStarLevelInfo(nVersion, nDetailType, nLevel, nValue)
	return 1, "white", "";
end

function Item:GetItemAmount(tbFind)
	local nCount = 0;
	for _, pItem in ipairs(tbFind) do
		nCount = nCount + pItem.nCount;
	end
	return nCount;
end

function Item:IfHasItem(pPlayer, nGenre, nDetail, nParticular, nLevel)
	if not nLevel then
		nLevel = 0;
	end

	local tbFind = pPlayer.pItem.FindItem(Item.ROOM_EQUIP, nGenre, nDetail, nParticular, nLevel);
	if #tbFind > 0 then
		return 1;
	end

	tbFind = pPlayer.pItem.FindItemInBags(nGenre, nDetail, nParticular, nLevel);
	if #tbFind > 0 then
		return 1;
	end

	return 0;
end

--获得ITEM的数量
function Item:GetItemCount(pPlayer, nGenre, nDetail, nParticular, nLevel)
	--local tbFind = pPlayer.pItem.FindItem(Item.ROOM_EQUIP, nGenre, nDetail, nParticular, nLevel);
	--if #tbFind > 0 then
	--	return 1;
	--end
	--tbFind = pPlayer.pItem.FindItemInBags(nGenre, nDetail, nParticular, nLevel);
	--if #tbFind > 0 then
	--	return #tbFind;
	--end
    local tbFind  = pPlayer.pItem.FindItemInBags(nGenre, nDetail, nParticular, nLevel);
	return Item:GetItemAmount(tbFind);
end





function Item:DelPlayerItem(pPlayer, nGenre, nDetail, nParticular, nLevel)
	-- 先删除装备着的
	local tbFind = pPlayer.pItem.FindItem(Item.ROOM_EQUIP, nGenre, nDetail, nParticular, nLevel);
	if #tbFind > 0 then
		local pItem = tbFind[1].pItem;
		KItem.DelPlayerItem(pPlayer, pItem);
	end

	-- 再删除背包里的
	tbFind = pPlayer.pItem.FindItemInBags(nGenre, nDetail, nParticular, nLevel);
	for _, pItem in ipairs(tbFind) do
		KItem.DelPlayerItem(pPlayer, pItem);
	end
end

function Item:DelPlayerItemByOnce(pPlayer, nGenre, nDetail, nParticular, nLevel,nCount)
	-- 先删除装备着的
    local bRet = 1;
    --local tbFind = pPlayer.pItem.FindItem(Item.ROOM_EQUIP, nGenre, nDetail, nParticular, nLevel);
	--if #tbFind > 0 then
	--	local pItem = tbFind[1].pItem;
	--	bRet = KItem.DelPlayerItem(pPlayer, pItem);
    --    return bRet;
	--end

	-- 再删除背包里的
    
	local tbFind = pPlayer.pItem.FindItemInBags(nGenre, nDetail, nParticular, nLevel)
    Lib:ShowTB(tbFind)
    print("..nCount=" .. nCount)
    local _nCount = nCount
	for _, pItem in ipairs(tbFind) do
        print("..pItem.nCount" .. pItem.nCount)
        if(_nCount <= 0 ) then 
            break;
        end
        if(_nCount > pItem.nCount) then 
            print("..1pItem.nCount" .. tostring(pItem.nCount))
            _nCount = _nCount - pItem.nCount
            KItem.DelPlayerItem(pPlayer, pItem, pItem.nCount)
        else 
            KItem.DelPlayerItem(pPlayer, pItem,_nCount)
            print("..2pItem.nCount" .. tostring(_nCount))
            break;
        end
	end
    return bRet;
end

function Item:GetPriceTips(nPrice)
	local szTips = ""..nPrice;
	local nLeftPrice;

	if nPrice >= 10000 then
		nLeftPrice = nPrice % 10000;
		nPrice = nPrice / 10000;
		if nLeftPrice ~= 0 then
			szTips = string.format("%d".. Lang.item.str140[Lang.Idx] .. "%d", nPrice, nLeftPrice);
		else
			szTips = string.format("%d" .. Lang.item.str140[Lang.Idx], nPrice);
		end
	end

	return szTips;
end

function Item:SellItems(tbItemIds)
	local nTotalPrice = 0;
	for _, pItem in ipairs(tbItemIds) do
		local nPrice = pItem.nPrice;
		nTotalPrice = nTotalPrice + nPrice;
		KItem.DelPlayerItem(me, pItem, pItem.nCount);
	end

	me.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, nTotalPrice);
end

function Item:GetEquipNumByLevel(pPlayer, nLevel)
	local pEquip;
	local nNum = 0;
	for i = Item.EQUIP_SWORD, Item.EQUIP_FOOT do
		pEquip = pPlayer.pItem.GetEquipByDetail(i);
		if pEquip and nLevel <= pEquip.nLevel then
			nNum = nNum + 1;
		end
	end

	return nNum;
end

function Item:CanTakeAward(pPlayer, tbAward)
	assert(tbAward);
	
	local nItemCellCount = Reward:GetNeedBagCell(tbAward);
	
	if pPlayer.pItem.CountFreeBagCell() < nItemCellCount then
		pPlayer.SendBlackMsg(string.format(Lang.item.str8[Lang.Idx], nItemCellCount));
		return 0;
	end
	
	return 1;
end

function Item:DoGetAwardByBox( pPlayer )
	
	local  it = self.ItemOb[pPlayer.dwId];

	self.ItemOb[pPlayer.dwId] = nil;
	
	local nNeedGold = tonumber(it.GetExtParam(2));
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.item.str9[Lang.Idx], szPriceTips))
		return 0;
	end
		
	local nListId = tonumber(it.GetExtParam(1)); --列表id
	local tbAwardData = KAward.GetAssetAwardData("BoxAward", nListId);
	if self:CanTakeAward(pPlayer, tbAwardData) ~= 1 then
		return;
	end

	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,125);

	KItem.DelPlayerItem(pPlayer, it);
	KAward.AddPlayerAward(pPlayer, "BoxAward", nListId);

end

--[35].Broadcast	= "1"
--[35].Desc	= "合成描述"
--[35].ExtParam1	= ""
--[35].ExtParam10	= ""
--[35].ExtParam2	= ""
--[35].ExtParam3	= ""
--[35].ExtParam4	= ""
--[35].ExtParam5	= ""
--[35].ExtParam6	= ""
--[35].ExtParam7	= ""
--[35].ExtParam8	= ""
--[35].ExtParam9	= ""
--[35].Icon	= ""
--[35].Id	= "35"
--[35].Intro	= "介绍"
--[35].Name	= "合成公式名字"
--[35].Rate	= ""
--[35].Target	= "18#5#2#34"
--[35].Type	= ""
--[35].View	= ""

--物品合成
function Item:c2s_composeItem(pPlayer, id, nCount)
        local tbItemCompose = Lib:LoadTabFile("/setting/item/itemcompose.txt");
        --Lib:ShowTB(tbItemCompose);
        local tbRow = nil;
        for k,v in pairs(tbItemCompose) do
                if(tonumber(v["Id"]) == id) then
                        tbRow = v;
                        break;
                end
        end
		if(not tbRow) then 
			return 0;
		end
		
		if(tbRow.Target == "") then 
			return 0;
		end
		local tbTarget = {} --合成目标
		
		if(tbRow["Target"] == "") then
			return 0;
		end
		
		tbTarget[#tbTarget+1] = Lib:SplitStr(tbRow["Target"],"#");
		
		local tbItems = {} --合成材料
		for i=1,10 do
			local szKey = "ExtParam" .. tostring(i);
			if(tbRow[szKey] ~= "") then 
				tbItems[#tbItems+1] = Lib:SplitStr(tbRow[szKey],"#");
			end
		end
		if(#tbItems == 0) then 
			return 0;
		end
		
		--判断背包格子至少要有一个
		--if pPlayer.pItem.CountFreeBagCell() < nCount then
        --    pPlayer.SendBlackMsg(string.format(Lang.item.str8[Lang.Idx], nCount));
        --    return 0;
        --end
        if pPlayer.pItem.CountFreeBagCell() < 1 then
            pPlayer.SendBlackMsg(string.format(Lang.item.str8[Lang.Idx], 1));
            return 0;
        end
        
        --判断物品是否足够
        for _,v in ipairs(tbItems) do
            local nItemCount = Item:GetItemCount(pPlayer, tonumber(v[1]), tonumber(v[2]), tonumber(v[3]), tonumber(v[4]));
            nItemCount = nItemCount * nCount;
            if(tonumber(v[5]) > nItemCount) then 
                pPlayer.SendBlackMsg("..Item is not enough!");
                return 0;
            end
		end
        
	    --Lib:ShowTB(tbItems);
        local nTargetCount = 0;
		local tbDel = {};
        for i=1,nCount do
            local randTable = {A = tonumber(tbRow["Rate"]),B= 1000 - tonumber(tbRow["Rate"])};
            local szKey = Lib:RandProb(randTable);
            --删除物品
            for _,v in ipairs(tbItems) do
				local szKey =  table.concat({ tonumber(v[1]), tonumber(v[2]), tonumber(v[3]), tonumber(v[4])},"#");
				print(".." .. szKey);
				tbDel[szKey] = (tbDel[szKey] or 0) + tonumber(v[5]);
                --local bRet = Item:DelPlayerItemByOnce(pPlayer, tonumber(v[1]), tonumber(v[2]), tonumber(v[3]), tonumber(v[4]),tonumber(v[5]));
                --if(bRet == 0) then 
                 --   return 0;
                --end
            end
            if(szKey == "A") then --成功
                --添加物品
                nTargetCount = nTargetCount + 1;
            else 
                --pPlayer.SendBlackMsg("..compose fail!!");
            end
        end
        --local szMsg = "..compose success";
		for k,v in pairs(tbDel) do 
			local tbKey = Lib:SplitStr(k,"#");
			Item:DelPlayerItemByOnce(pPlayer, tonumber(tbKey[1]),tonumber(tbKey[2]),tonumber(tbKey[3]),tonumber(tbKey[4]),v);
		end
        local szMsg = Lang.mission.str1221[Lang.Idx];
        ---[[
        if(nTargetCount > 0) then 
            KItem.AddPlayerItem(pPlayer, tonumber(tbTarget[1][1]), tonumber(tbTarget[1][2]), tonumber(tbTarget[1][3]), tonumber(tbTarget[1][4]),nTargetCount);
            if(tbRow["Intro"] ~="") then
                szMsg = string.format(tbRow["Intro"],pPlayer.szName);
                if(tonumber(tbRow["Broadcast"]) == 1) then 
                    KChat.BroadcastNoticeMsg(szMsg);  
                end
            end
        end
        --]]
        
        --[[
        for i=1,nTargetCount do
            KItem.AddPlayerItem(pPlayer, tonumber(tbTarget[1][1]), tonumber(tbTarget[1][2]), tonumber(tbTarget[1][3]), tonumber(tbTarget[1][4]));
            if(tbRow["Intro"] ~="") then
                szMsg = string.format(tbRow["Intro"],pPlayer.szName);
                if(tonumber(tbRow["Broadcast"]) == 1) then 
                    KChat.BroadcastNoticeMsg(szMsg);  
                end
            end
        end
        --]]
        
        
        if(nTargetCount == 0) then 
            --szMsg = "..compose failed!"
	    szMsg = Lang.mission.str1222[Lang.Idx];
        end
        
        
        pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowHeChengMessage", szMsg,(nTargetCount > 0) and 1 or 0});
        return 1;
end


--随机宝箱回调
function Item:RandomItemCallback(pPlayer,pItem)
	print("..Item:RandomItemCallback");
    --要改掉
    local tbRandomFile = Lib:LoadTabFile("/setting/item/magic/randomitem.txt");
    local me = pPlayer;
    local it = pItem;
    
    --local nKindId = tonumber(it.GetExtParam(1));
	local tbKindId = {};
	for i=1,8 do
		local nExtParam = tonumber(it.GetExtParam(i));
		if(nExtParam and nExtParam > 0) then 
			tbKindId[#tbKindId + 1] = nExtParam;
		end
	end
	Lib:ShowTB(tbKindId);
	local tbRandom = {};
	for _,v in ipairs(tbKindId) do 
		local tbRows = {};
	    for i=2,#tbRandomFile do
	    	local nKindId = tonumber(tbRandomFile[i].KindId);
	        if(v == nKindId) then 
	            tbRows[#tbRows + 1] = tbRandomFile[i];
	        end
	    end
	    tbRandom[v] = tbRows;
	end
	Lib:ShowTB(tbRandom);
	local nRandomCount = 0;
	
	for k,v in pairs(tbRandom) do 
		local _nCount = tonumber(v[1].RandomCount);
		nRandomCount = nRandomCount + _nCount;
	end
	
	if me.pItem.CountFreeBagCell() <  nRandomCount then
            me.SendBlackMsg(string.format(Lang.item.str8[Lang.Idx], nRandomCount));
            return;
    end
	
	local nMoneyType  = tonumber(it.GetExtParam(9));
	local nMoneyCount = tonumber(it.GetExtParam(10));

	print("..nMoneyType",nMoneyType,"nMoneyCount",nMoneyCount);
	

	if(nMoneyType and nMoneyType > 0) then
        local nCurrentMoney = me.pPlayerPurse.GetMoney(nMoneyType);
        if(nCurrentMoney < nMoneyCount) then 
            me.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", nMoneyCount});
            return;
        else
            me.pPlayerPurse.AddMoney(nMoneyType, -1 * nMoneyCount);
        end
    end
    
    --local tbRows = {};
    --for i=2,#tbRandomFile do 
    --    if(tonumber(tbRandomFile[i].KindId) == nKindId) then 
    --         tbRows[#tbRows + 1] = tbRandomFile[i];
    --    end
    --end
    
    --local nRandomCount = tonumber(tbRows[1].RandomCount);
    
    --[[
    --local nNeedBagCount = 0;
    --for i,v in ipairs(tbRows) do 
    --    if(tonumber(v.Genre)) then 
    --        nNeedBagCount = nNeedBagCount + 1;
    --    end
    --end
    --nNeedBagCount = nRandomCount * nNeedBagCount;
    --]]
    
    --if me.pItem.CountFreeBagCell() <  nRandomCount then
    --        me.SendBlackMsg(string.format(Lang.item.str8[Lang.Idx], nRandomCount));
    --       return;
    --end


    KItem.DelPlayerItem(me, it);
	
	for k,v in pairs(tbRandom) do 
		local tbRows = v;
		local nRandomCount = tonumber(tbRows[1].RandomCount);
	    for i = 1 ,nRandomCount do 
	        local tbRand = {};
	        for i,v in ipairs(tbRows) do 
	            tbRand[i] = tonumber(v.Probability);
	        end
	        local nIdx  = Lib:RandProb(tbRand);
	        local tbRow =  tbRows[nIdx];
	        
	        if(tonumber(tbRow.Exp)) then 
	            me.AddExp(tonumber(tbRow.Exp));
	        end
	        
	        if(tonumber(tbRow.Money)) then 
	            me.pPlayerPurse.AddMoney(2, tonumber(tbRow.Money));
	        end
	        
	        if(tonumber(tbRow.YLEnergy)) then 
	            me.pPlayerPurse.AddMoney(6, tonumber(tbRow.YLEnergy));
	        end
	        
	        if(tonumber(tbRow.Genre)) then 
	            local item = {tonumber(tbRow.Genre),tonumber(tbRow.DetailType),tonumber(tbRow.ParticularType),tonumber(tbRow.Level),tonumber(tbRow.Amount)};
	            --Lib:ShowTB(item)
	            KItem.AddPlayerItem(me, unpack(item));
	            if(tbRow.Announce ~= "") then 
	                local szMsg = string.format(tbRow.Announce,me.szName)
	                KChat.BroadcastNoticeMsg(szMsg);
	            end
	        end
	    end
	end
end



