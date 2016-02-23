-- 对C导出的Item对象进行封装
if MODULE_GC_SERVER then
	return
end

local self;		-- 提供以下函数用的UpValue

-------------------------------------------------------------------------------
-- for both server & client

function _KLuaItem.CanUse(pPlayer)
	return KItem.CanPlayerUseItem(pPlayer, self);
end

-- 获取活动定制的字符串信息
function _KLuaItem.GetEventCustomString()
	local nType = self.nCustomType;
	if nType == KItem.CUSTOM_TYPE_EVENT then
		return self.szCustomString;
	end
	return nil;
end

-------------------------------------------------------------------------------
-- for server

-- 从指定角色身上删除自己
function _KLuaItem.Delete(pPlayer, nWay)
	return	KItem.DelPlayerItem(pPlayer, self, 1, (nWay or 100));
end

function _KLuaItem.GetForbidType()
	return KItem.GetOtherForbidType(self.nGenre, self.nDetail, self.nParticular, self.nLevel);
end

function _KLuaItem.Equal(g,d,p,l)
	g = g or 0;
	
	if d and p and l then
		if self.nGenre == g and self.nDetail == d and self.nParticular == p and self.nLevel == l then
			return 1;
		else
			return 0;
		end
	end
	
	if d and p then
		if self.nGenre == g and self.nDetail == d and self.nParticular == p then
			return 1;
		else
			return 0;
		end
	end
	
	if d then
		if self.nGenre == g and self.nDetail == d then
			return 1;
		else
			return 0;
		end
	end
	
	if self.nGenre == g then
		return 1;
	else
		return 0;
	end
end

function _KLuaItem.SzGDPL()
	return string.format("%d,%d,%d,%d", self.nGenre, self.nDetail, self.nParticular, self.nLevel);
end

function _KLuaItem.TbGDPL()
	return {self.nGenre, self.nDetail, self.nParticular, self.nLevel};
end

-------------------------------------------------------------------------------
-- for client

-- 获得自己的Tip信息
function _KLuaItem.GetTip(nState, szBindType)
	local pIt = it;
	it = self;
	local szTitle, szTip, szView = Item:GetTip(self.szClass, nState, szBindType);
	it = pIt;
	return	szTitle, szTip, szView;
end

-- 获得自己的对比Tip信息（装备有效,非装备道具与GetTip无异）
function _KLuaItem.GetCompareTip(nState, szBindType)
	local pIt = it;
	it = self;
	local szTitle, szTip, szView, szCmpTitle, szCmpTip, szCmpView = Item:GetCompareTip(self.szClass, nState, szBindType);
	it = pIt;
	return	szTitle, szTip, szView, szCmpTitle, szCmpTip, szCmpView;
end

-- 获得自己的性别需取
function _KLuaItem.GetSex()
	local tbReq = self.GetReqAttrib();
	for i, tbTmp in ipairs(tbReq) do
		if tbTmp then
			if tbTmp.nReq == 8 then
				return tbTmp.nValue;
			end		
		end
	end
	return nil;
end

-- 计算除去原始的装备战斗力
function _KLuaItem.CalcExtraFightPower(nEnhance, nRefine)
	if self.IsEquip() == 0 then
		return 0;
	end
	local nXiShu = Item.tbEnhanceOfEquipPos[self.nEquipPos] or 1;	
	local nEnhancePower, nRefinePower;
	if nEnhance then
		nEnhancePower = Item.tbEnhanceFightPower[nEnhance];
	else
		nEnhancePower = Item.tbEnhanceFightPower[self.nEnhTimes];
	end
	if nRefine then
		nRefinePower = Item.tbRefineFightPower[nRefine];
	else
		nRefinePower = Item.tbRefineFightPower[self.nRefineLevel];
	end
	return nEnhancePower * nXiShu + nRefinePower;
end

-- 计算装备战斗力
function _KLuaItem.CalcFightPower(nEnhance, nRefine)
	return self.nFightPower + self.CalcExtraFightPower();
end
