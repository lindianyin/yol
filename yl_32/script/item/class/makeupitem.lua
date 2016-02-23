
-- 随机道具

-------------------------------------------------------------------
--File: 	makeupitem.lua
--Author:
--Date:
--Describe:	制造类道具，将可以点击的道具赋以此classname其他材料为空即可
--第一个扩展参数代表制造表中的第几组
--其他扩展参数暂时不做处理

------------------------------------------------------------------------------------------
-- initialize

local tbMakeUpItem = Item:GetClass("makeupitem");

tbMakeUpItem.tbMaterialsList = {};
tbMakeUpItem.tbCurrencyList = {};
tbMakeUpItem.tbOutPutList = {};

function tbMakeUpItem:PreloadFile()
	local FileData = Lib:LoadTabFile("/setting/item/magic/makeup.txt") or {};
	for _,tbRow in pairs(FileData) do  
		if tonumber(tbRow.MaterialGenre) and tonumber(tbRow.MaterialGenre) ~= 0 then  --获取材料列表
			local nIdx = #tbMakeUpItem.tbMaterialsList + 1;
			tbMakeUpItem.tbMaterialsList[nIdx] = {};
			tbMakeUpItem.tbMaterialsList[nIdx].nId = tonumber(tbRow.KindId) or 0;
			tbMakeUpItem.tbMaterialsList[nIdx].MaterialName = tbRow.MaterialName or "";
			tbMakeUpItem.tbMaterialsList[nIdx].MaterialGenre = tonumber(tbRow.MaterialGenre) or 0;
			tbMakeUpItem.tbMaterialsList[nIdx].MaterialDetailType = tonumber(tbRow.MaterialDetailType) or 0;
			tbMakeUpItem.tbMaterialsList[nIdx].MaterialParticularType = tonumber(tbRow.MaterialParticularType) or 0;
			tbMakeUpItem.tbMaterialsList[nIdx].MaterialLevel = tonumber(tbRow.MaterialLevel) or 0;
			tbMakeUpItem.tbMaterialsList[nIdx].MaterialQuality = tonumber(tbRow.MaterialQuality) or 1;
			tbMakeUpItem.tbMaterialsList[nIdx].MaterialNum = tonumber(tbRow.MaterialNum) or 1; 
		end
	end
	
	for _,tbRow in pairs(FileData) do	
		if tonumber(tbRow.CurrencyType) and tonumber(tbRow.CurrencyType) ~= 0 then  --获取货币列表
			local nMIdx = #tbMakeUpItem.tbCurrencyList + 1;
			tbMakeUpItem.tbCurrencyList[nMIdx] = {};
			tbMakeUpItem.tbCurrencyList[nMIdx].nId = tonumber(tbRow.KindId) or 0;
			tbMakeUpItem.tbCurrencyList[nMIdx].CurrencyType = tonumber(tbRow.CurrencyType) or 0;
			tbMakeUpItem.tbCurrencyList[nMIdx].CurrencyValue = tonumber(tbRow.CurrencyValue) or 0;
		end
	end
	
	for _,tbRow in pairs(FileData) do  --获取产出列表
		if tonumber(tbRow.OutPutGenre) and tonumber(tbRow.OutPutGenre) ~= 0 then
			local nIdx2 = #tbMakeUpItem.tbOutPutList + 1;
			tbMakeUpItem.tbOutPutList[nIdx2] = {};
			tbMakeUpItem.tbOutPutList[nIdx2].nId = tonumber(tbRow.KindId) or 0;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutName = tbRow.OutPutName or "";
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutGenre = tonumber(tbRow.OutPutGenre) or 0;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutDetailType = tonumber(tbRow.OutPutDetailType) or 0;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutParticularType = tonumber(tbRow.OutPutParticularType) or 0;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutLevel = tonumber(tbRow.OutPutLevel) or 0;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutQuality = tonumber(tbRow.OutPutQuality) or 1;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutNum = tonumber(tbRow.OutPutNum) or 1;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutFactionType = tonumber(tbRow.OutPutFactionType) or 0;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutSexType = tonumber(tbRow.OutPutSexType) or -1;
			tbMakeUpItem.tbOutPutList[nIdx2].OutPutRate = tonumber(tbRow.OutPutRate) or 0;
			tbMakeUpItem.tbOutPutList[nIdx2].BindType = tonumber(tbRow.BindType) or 0;
		end
	end
end

tbMakeUpItem:PreloadFile();
------------------------------------------------------------------------------------------
-- public
function tbMakeUpItem:OnUse()
	--Lib:ShowTB(tbMakeUpFile);
	local tbMaterialList = {};
	local tbCurrencyList = {};
	local tbOutPutList = {};
	local nMakeUpId = tonumber(it.GetExtParam(1));
	local nFactionType = tonumber(it.GetExtParam(2)) or 0;
	local nSexType = tonumber(it.GetExtParam(3)) or 0;
	local nSelfSex = me.GetSex();
	local nFactionId = me.GetPrimaryFaction();
	--print("nMakeUpId",nMakeUpId);
	for _,tbRow in pairs(tbMakeUpItem.tbMaterialsList) do  --获取对应id的材料列表
		if tonumber(tbRow.nId) == nMakeUpId then
			local nIdx = #tbMaterialList + 1;
			tbMaterialList[nIdx] = {};
			tbMaterialList[nIdx].MaterialName = tbRow.MaterialName or "";
			tbMaterialList[nIdx].MaterialGenre = tonumber(tbRow.MaterialGenre) or 0;
			tbMaterialList[nIdx].MaterialDetailType = tonumber(tbRow.MaterialDetailType) or 0;
			tbMaterialList[nIdx].MaterialParticularType = tonumber(tbRow.MaterialParticularType) or 0;
			tbMaterialList[nIdx].MaterialLevel = tonumber(tbRow.MaterialLevel) or 0;
			tbMaterialList[nIdx].MaterialQuality = tonumber(tbRow.MaterialQuality) or 1;
			tbMaterialList[nIdx].MaterialNum = tonumber(tbRow.MaterialNum) or 1;
		end
	end
	
	for _,tbRow in pairs(tbMakeUpItem.tbCurrencyList) do  --获取对应id的货币列表
		if tonumber(tbRow.nId) == nMakeUpId then
			local nMIdx = #tbCurrencyList + 1;
			tbCurrencyList[nMIdx] = {};
			tbCurrencyList[nMIdx].CurrencyType = tonumber(tbRow.CurrencyType) or 0;
			tbCurrencyList[nMIdx].CurrencyValue = tonumber(tbRow.CurrencyValue) or 0;
		end
	end

	for _,tbRow in pairs(tbMakeUpItem.tbOutPutList) do  --获取对应id的产出列表
		if tonumber(tbRow.nId) == nMakeUpId then
			local nFaction = tonumber(tbRow.OutPutFactionType) or 0;
			local nSexId = tonumber(tbRow.OutPutSexType) or -1;
			if  nFaction == nFactionId or nFactionType == 0 or nFaction == 0 then
				if nSexId == nSelfSex or nSexType == 0 or nSexId == -1 then
					local nIdx2 = #tbOutPutList + 1;
					tbOutPutList[nIdx2] = {};
					tbOutPutList[nIdx2].OutPutName = tbRow.OutPutName or "";
					tbOutPutList[nIdx2].OutPutGenre = tonumber(tbRow.OutPutGenre) or 0;
					tbOutPutList[nIdx2].OutPutDetailType = tonumber(tbRow.OutPutDetailType) or 0;
					tbOutPutList[nIdx2].OutPutParticularType = tonumber(tbRow.OutPutParticularType) or 0;
					tbOutPutList[nIdx2].OutPutLevel = tonumber(tbRow.OutPutLevel) or 0;
					tbOutPutList[nIdx2].OutPutQuality = tonumber(tbRow.OutPutQuality) or 1;
					tbOutPutList[nIdx2].OutPutNum = tonumber(tbRow.OutPutNum) or 1;
					tbOutPutList[nIdx2].OutPutRate = tonumber(tbRow.OutPutRate) or 0;
					tbOutPutList[nIdx2].BindType = tonumber(tbRow.BindType) or 0;
				end
			end
		end
	end
	
	local nIsMaterialEnough,szDialogMsg = tbMakeUpItem:CheckMaterial(tbMaterialList);
	if nIsMaterialEnough == 0 then
		me.SendBlackMsg(szDialogMsg);
		return 0;
	end

	if me.pItem.CountFreeBagCell() < 1 then
		me.SendBlackMsg(Lang.item.str55[Lang.Idx]);
		return 0;
	end

	local szName,nG,nD,nP,nL,nQuality,nNum,nBind = tbMakeUpItem:GetOutPutItem(tbOutPutList);
	if not nG or nG == 0 then
		Dialog:Say(Lang.item.str56[Lang.Idx]);
		return 0;
	end
	
	local nIfNeedInherit ,nInheritEnhInfo, tbInheritStoneInfo, nExtraMoney = tbMakeUpItem:GetInheritAttr(tbMaterialList,nQuality);
	
	local nIsCurrenyEnough,szMoneyMsg = tbMakeUpItem:CheckMoney(tbCurrencyList, nExtraMoney);
	if nIsCurrenyEnough == 0 then
		me.SendBlackMsg(szMoneyMsg);
		return 0;
	end

	--print("get here!!!")

	for _,tbRow in pairs (tbMaterialList) do  --扣除材料
		local nMGenre = tonumber(tbRow.MaterialGenre);
		local nMDetail = tonumber(tbRow.MaterialDetailType);
		local nMParticular = tonumber(tbRow.MaterialParticularType);
		local nMLevel = tonumber(tbRow.MaterialLevel);

		if nMGenre and nMGenre ~= 0 then
			if nMLevel and nMLevel ~= 0 then
				me.pItem.ConsumeItemInBags(tonumber(tbRow.MaterialNum), nMGenre, nMDetail, nMParticular, nMLevel);
			else
				me.pItem.ConsumeItemInBags(tonumber(tbRow.MaterialNum), nMGenre, nMDetail, nMParticular);
			end
		end
	end
	for i= 1, #tbCurrencyList do --扣除货币
		local nCurType = tonumber(tbCurrencyList[i].CurrencyType);
		local nCurValue = tonumber(tbCurrencyList[i].CurrencyValue);
		if nCurValue and nCurValue ~= 0 then
			me.pPlayerPurse.AddMoney(nCurType, -nCurValue);
		end
	end
	
	local pItem;
	for i = 1 , nNum do  --添加道具
		if nL and nL ~= 0 then
			pItem = KItem.AddPlayerItem(me, nG, nD, nP, nL);
		else
			pItem = KItem.AddPlayerItem(me, nG, nD, nP);
		end
		if nBind and nBind ~= 0 then
			pItem.Bind(1);
		end
	end
	if nIfNeedInherit == 1 then
		local nRet = pItem.Regenerate(
		pItem.nGenre,
		pItem.nDetail,
		pItem.nParticular,
		0,
		nInheritEnhInfo,			-- 继承强化次数
		tbInheritStoneInfo,			-- 继承宝石信息
		0,
		0,
		pItem.dwRandSeed,
		pItem.dwRandSeedEx
	)
	end
	
	local szSucMsg = string.format(Lang.item.str57[Lang.Idx],nNum,szName);
	me.SendBlackMsg(szSucMsg);
end

function tbMakeUpItem:CheckMaterial(tbMaterial)
	local szMsg = "";
	local tbMaterialItem;
	for _,tbRow in pairs(tbMaterial) do
		if tbRow.MaterialGenre ~= 0 then
			local nGenre = tonumber(tbRow.MaterialGenre);
			local nDetail = tonumber(tbRow.MaterialDetailType);
			local nParticular = tonumber(tbRow.MaterialParticularType);
			local nLevel = tonumber(tbRow.MaterialLevel);

			if nGenre and nGenre ~= 0 then
				if nLevel and nLevel ~= 0 then
					tbMaterialItem = me.pItem.FindItemInBags(nGenre, nDetail, nParticular, nLevel);
				else
					tbMaterialItem = me.pItem.FindItemInBags(nGenre, nDetail, nParticular);
				end
				local nCount = Item:GetItemAmount(tbMaterialItem);
				if nCount < tonumber(tbRow.MaterialNum) then
					local nNum = tonumber(tbRow.MaterialNum) - nCount;
					szMsg = string.format(Lang.item.str58[Lang.Idx], nNum, tbRow.MaterialName);
					return 0, szMsg;
				end
			end
		end
	end
	return 1;
end

function tbMakeUpItem:GetOutPutItem(tbOutPutList)
	local nRate = 0;
	for _,tbRow in pairs(tbOutPutList) do
		nRate = nRate + (tonumber(tbRow.OutPutRate) or 0);
	end
	print("nRate",nRate);
	local nGetRate = KUnify.MathRandom(1, nRate);
	for _,tbRow in pairs(tbOutPutList) do
		nGetRate = nGetRate - tonumber(tbRow.OutPutRate);
		if nGetRate <= 0 then
			local nG = tonumber(tbRow.OutPutGenre);
			local nD = tonumber(tbRow.OutPutDetailType);
			local nP = tonumber(tbRow.OutPutParticularType);
			local nL = tonumber(tbRow.OutPutLevel);
			local nQuality = tonumber(tbRow.OutPutQuality);
			local nNum = tonumber(tbRow.OutPutNum);
			local nBind = tonumber(tbRow.BindType);
			print("OutPutInfo:",tbRow.OutPutName, nG, nD, nP, nL, nQuality, nNum, nBind);
			return tbRow.OutPutName, nG, nD, nP, nL, nQuality, nNum, nBind;
		end
	end
	return 0;
end

function tbMakeUpItem:CheckMoney(tbCurrencyList, nExtraMoney)
	local szMsg = "";
	local tbCurrencyType = 
	{
		[1] = Lang.item.str59[Lang.Idx];
		[2] = Lang.item.str60[Lang.Idx];
		[3] = Lang.item.str61[Lang.Idx];
		[4] = Lang.item.str62[Lang.Idx];
		[5] = Lang.item.str63[Lang.Idx]; --暂不使用
	}
	local tbMoney = {0,0,0,0,0,0,0};
	for _,tbRow in pairs(tbCurrencyList) do
		local nCurrencyType = tonumber(tbRow.CurrencyType) or 0;
		local nCurrencyValue = tonumber(tbRow.CurrencyValue) or 0;
		if nCurrencyType and nCurrencyType ~= 0 then
			tbMoney[nCurrencyType] = tbMoney[nCurrencyType] + nCurrencyValue;
		end
	end
	
	tbMoney[2] = tbMoney[2] + nExtraMoney;
	
	for i= 1, #tbMoney do
		if tbMoney[i] and tbMoney[i] ~= 0 then
			if me.pPlayerPurse.GetMoney(i) < tbMoney[i] then
				local nLackMoney = tbMoney[i] - me.pPlayerPurse.GetMoney(i);
				local szFailMsg = string.format(Lang.item.str64[Lang.Idx],nLackMoney,tbCurrencyType[i]);
				return 0 , szFailMsg;
			end
		end
	end
	return 1;
end

function tbMakeUpItem:GetInheritAttr(tbMaterialList,nOutPutQuality)
	local tbGetEquip;
	for _,tbRow in pairs(tbMaterialList) do
		local nGenre = tonumber(tbRow.MaterialGenre) or 0;
		local nDetail = tonumber(tbRow.MaterialDetailType) or 0;
		local nParticular = tonumber(tbRow.MaterialParticularType) or 0;
		local nLevel = tonumber(tbRow.MaterialLevel) or 0;
		if (nGenre == 1 and (nDetail >= 1 and nDetail <= 9)) then
			if nLevel and nLevel ~= 0 then
				tbGetEquip = me.pItem.FindItemInBags(nGenre, nDetail, nParticular, nLevel);
				break;
			else
				tbGetEquip = me.pItem.FindItemInBags(nGenre, nDetail, nParticular);
				break;
			end
		end
	end
	local pGetEquip = tbGetEquip[1];
	if pGetEquip then
		local nEnhTimes = pGetEquip.nEnhTimes or 0;
		local tbStoneInfo = pGetEquip.GetStoneInfo() or {};
		local nMaterialEnhCost = 0;
		local nOutPutEnhCost = 0;
		if pGetEquip.nQuality < nOutPutQuality then
			for i= 1 ,nEnhTimes do
				nMaterialEnhCost = nMaterialEnhCost + Item:CalcEnhanceCost(i, pGetEquip.nQuality);
				nOutPutEnhCost = nOutPutEnhCost + Item:CalcEnhanceCost(i, nOutPutQuality);
			end
		end
		local nExtraCost = nOutPutEnhCost - nMaterialEnhCost;
		local nIfNeedInherit = 1;
		return nIfNeedInherit, nEnhTimes, tbStoneInfo, nExtraCost;
	end
	return 0;
end