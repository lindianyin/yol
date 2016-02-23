
-- Item默认模板（普通道具，通用功能脚本）

local tbDefault = Item:GetClass("default");

function tbDefault:InitGenInfo()
	return	{};						-- 啥都不做
end

function tbDefault:CheckUsable()
	return	1;						-- 可用
end

-- nParam 为客户端传来的参数，无参则为0
function tbDefault:OnUse(nParam)			-- 什么都不做，也不会删除物品
	return	0;
end

-- 客户端回调，返回值会作为参数传送到服务器内
function tbDefault:OnClientUse()
	return 0;
end

function tbDefault:GetTitle()
	return	it.szName.."\n";
end

function tbDefault:GetTip(nState)			-- 获取普通道具Tip
	return	"";
end

function tbDefault:CalcStarLevelInfo(nLevel, nValue)
	return 1, "white", "";
end

function tbDefault:IsPickable(nObjId)		-- 总可以拣起
	return	1;
end

function tbDefault:PickUp(nX, nY)	-- 拣起后不删除
	return	1;
end

-- 计算道具价值量相关信息，仅在道具生成时执行一次
-- 返回值：价值量，价值量星级，名字颜色，透明图层路径
function tbDefault:CalcValueInfo()
	return	it.nOrgValue, 1, "white", "";			-- 使用原始价值量
end

function tbDefault:GetChangeable(pItem)
	if pItem.nMakeCost > 0 and pItem.IsBind() ~= 1 then
		return 1;
	else
		return 0;
	end
end

--计算商品打折信息（对优惠券）
--返回值：打折商品数量，打折率
function tbDefault:CalDiscount(tbWareList)
	return 0, {};
end

--消耗使用次数
function tbDefault:DecreaseCouponTimes(tbCouponWare)
	return 0;
end

function tbDefault:CanCouponUse(dwId)
	return 0, "物品类型不匹配";
end
