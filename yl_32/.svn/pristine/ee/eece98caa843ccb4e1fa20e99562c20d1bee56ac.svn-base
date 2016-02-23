
-- 装备，强化功能脚本

------------------------------------------------------------------------------------------
-- initialize

------------------------------------------------------------------------------------------
-- private

local function GetMaxEnhTimes(pEquip)
	return pEquip.nQuality * 12;
end

local function CheckEnhItem(pEquip)		-- 服务端检查装备的合法性
	if (not pEquip) or (pEquip.IsEquip() ~= 1) then
		return 0;
	end

	if ((pEquip.nDetail == Item.EQUIP_SUIT) or (pEquip.nDetail == Item.EQUIP_BOTTOM) or
		(pEquip.nDetail == Item.EQUIP_BACK))
	then
		return 0;			-- 外装不能强化
	end

	local nMaxEnhTimes = GetMaxEnhTimes(pEquip);
	if (pEquip.nEnhTimes >= nMaxEnhTimes) then
		if pEquip.nLevel >= 7 then
			me.SendBlackMsg(Lang.item.str118[Lang.Idx]);
		else
			me.SendBlackMsg(Lang.item.str119[Lang.Idx]);
		end
		
		return 0;			-- 已强化到最高级则不能强化
	end

	return 1;
end

------------------------------------------------------------------------------------------
-- public

function Item:Enhance(pEquip, nPrice)	-- 程序接口：服务端执行装备强化

	if CheckEnhItem(pEquip) ~= 1 then
		return -1;
	end

	local szPriceTips = Item:GetPriceTips(nPrice);
	if me.pPlayerPurse.GetMoney(Item.EM_MONEY_SILVER) < nPrice then
		me.SendBlackMsg(string.format(Lang.item.str120[Lang.Idx], szPriceTips))
		return -1;
	end
	me.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, -nPrice);

	local nRet = pEquip.Regenerate(
		pEquip.nGenre,
		pEquip.nDetail,
		pEquip.nParticular,
		pEquip.nLevel,
		pEquip.nEnhTimes + 1,			-- 强化次数加一
		pEquip.GetStoneInfo(),
		pEquip.GetAppendAttr(),
		0,
		pEquip.dwRandSeed,
		pEquip.dwRandSeedEx
	);

	--me.SysMsg("强化成功！");
	--统计点  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d,nEnhTimes=%d",pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel,pEquip.nEnhTimes + 1);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,53,szStr);
	
	--DailyTasks:commitTaskValue(me,table.concat({pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel},"#"),DailyTasks.TYPE_ENHANCE_ITEM,1);
	DailyTasks:commitTaskValue(me,"",DailyTasks.TYPE_ENHANCE_ITEM,1);
	return 1;
end

--一键强化
function Item:EnhanceEx(pEquip)
	if CheckEnhItem(pEquip) ~= 1 then
		return -1;
	end
	
	local nEnhTimes = 0;
	local nEnhLevel = pEquip.nEnhTimes;				-- 当前强化等级
	local nMaxEnhTimes = GetMaxEnhTimes(pEquip);
	local nSilver = me.pPlayerPurse.GetMoney(Item.EM_MONEY_SILVER);
	local nTotalCost = 0;
	
	for i = nEnhLevel + 1, nMaxEnhTimes do
		local nQuality = math.floor(i / 12) + 1;
		local tbNumAward = KAward.GetNumericalAwardData("equip_enhance", nQuality, i);
		assert(tbNumAward);
		local nCost = tbNumAward.nCost;
		if nSilver >= nCost then
			nEnhTimes = nEnhTimes + 1;
			nSilver = nSilver - nCost;
			nTotalCost = nTotalCost + nCost;
		else
			break;
		end
	end
	
	if nEnhTimes <= 0 then
		me.SendBlackMsg(Lang.item.str121[Lang.Idx]);
		return 0;
	end
	
	me.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, -nTotalCost);

	local nRet = pEquip.Regenerate(
		pEquip.nGenre,
		pEquip.nDetail,
		pEquip.nParticular,
		pEquip.nLevel,
		pEquip.nEnhTimes + nEnhTimes,
		pEquip.GetStoneInfo(),
		pEquip.GetAppendAttr(),
		0,
		pEquip.dwRandSeed,
		pEquip.dwRandSeedEx
	);
	
	--me.SysMsg("强化成功！");
	--统计点  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d,nEnhTimes=%d",pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel,pEquip.nEnhTimes + nEnhTimes);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,53,szStr);


	DailyTasks:commitTaskValue(me,table.concat({pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel},"#"),DailyTasks.TYPE_ENHANCE_ITEM,1);
	
	return 1;
end

function Item:CalcMaxEnhanceTimes(pEquip)	-- 计算一个可强化装备能强化的次数(最大强化等级)
	if (not pEquip) then
		return 0;
	end
	local nReqLevel = pEquip.nReqLevel;

	if (nReqLevel < 10) then
		return 0;
	elseif (nReqLevel <= 30) then		-- 10-30级装备可强化6次
		return 6;
	elseif (nReqLevel <= 50) then
		return 12;					-- 40-50级装备可强化12次
	else
		return 15;					-- 60级以上装备可强化15次
	end
end

------------------------------------------------------------------------------------------
