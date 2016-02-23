-- Pet脚本类

Include("/script/pet/define.lua");

local PARAM_FEED_HUNGRY = 1;		-- 宠物食物增长的饱食度
local PARAM_ADD_EXP = 1;			-- 宠物经验书增加的经验


function Pet:Feed(pPet, pItem)

	if pPet.nQuality > pItem.nQuality then
		me.SysMsg(Lang.pet.str18[Lang.Idx]);
		return;
	end
	-- kk 可使用高品质喂养
	-- elseif pPet.nQuality < pItem.nQuality then
	-- 	me.SysMsg("饲料的品质过高，请选用相应品质的饲料喂食宠物");
	-- 	return;
	-- end

	local nHungry = tonumber(pItem.GetExtParam(1));

	pItem.SetCount(pItem.nCount - 1, Item.emITEM_DATARECORD_REMOVE);

	pPet.AddHungry(nHungry);

	pPet.Sync();
end

function Pet:RerandPetCharacter(pPet, nCharacter)
	local nCostNum = 1;
	local tbCostGDPL = { 24, 6, 1 };

	if nCharacter == pPet.nCharacter then
		me.SysMsg(string.format(Lang.pet.str19[Lang.Idx], Pet.CHARACTER_TIPS[nCharacter]));
		return;
	end

	local tbFind = me.pItem.FindItem(Item.ROOM_PET, unpack(tbCostGDPL));
	if (self:GetTotalCount(tbFind) < nCostNum) then
		local szErrMsg = string.format(Lang.pet.str20[Lang.Idx], nCostNum);
		me.SysMsg(szErrMsg);
		return;
	end
	me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));

	if nCharacter == 0 then
		nCharacter = pPet.nCharacter;
		local tbCharacter = {};
		for i = Pet.emPET_CHARACTER_CONST, Pet.emPET_CHARACTER_SPIRT do
			if i ~= pPet.nCharacter then
				table.insert(tbCharacter, i);
			end
		end

		local nIdx = KUnify.MathRandom(#tbCharacter);
		nCharacter = tbCharacter[nIdx];
	end

	local bRet = pPet.Regenerate(
		pPet.nType,
		pPet.nQuality,
		nCharacter,
		pPet.nLevel,
		pPet.nRankLevel,
		pPet.dwRandSeed
	);

	pPet.Sync();
	me.SysMsg(Lang.pet.str21[Lang.Idx]);

	return bRet;
end

-- 培养
function Pet:Forster(pPet, nCostNum)
	local tbCostGDPL = { 24, 5, 1 };

	local tbFind = me.pItem.FindItemInBags(unpack(tbCostGDPL));
	local nNumHave = Item:GetItemAmount(tbFind);
	local nCostNum = 1;

	if (nNumHave < nCostNum) then
		local szErrMsg = string.format(Lang.pet.str22[Lang.Idx]);
		me.SendBlackMsg(szErrMsg);
		return;
	end

	local nExp = tbFind[1].GetExtParam(PARAM_ADD_EXP);
	local nTotalExpToNextLevel = pPet.GetTotalExp(me.GetLevel() + 1);
	local nTotalExpHave = pPet.GetTotalExp(0);
	if (nTotalExpHave + nExp) >= nTotalExpToNextLevel then
	    me.SendBlackMsg(Lang.pet.str23[Lang.Idx]);
	    return;
	end

	me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));

	nExp = nExp * nCostNum;
	local bRet = pPet.AddExp(nExp);

	pPet.Sync();

	return bRet;
end

function Pet:GetTotalCount(tbFind)
	local nCount = 0;

	for _, tbItem in ipairs(tbFind) do
		nCount = nCount + tbItem.pItem.nCount;
	end

	return nCount;
end

-- 添加宠物，带有宠物空间判定
function Pet:AddPet(pPlayer, nPetRepresentId, nCharacter)
	-- 随机性格 1-7
	if (nCharacter == nil) then
		nCharacter = KUnify.MathRandom(1,7)
	end

	local tbPets = KPet.GetPlayerPetList(pPlayer);
	local nPetCapacity = pPlayer.GetPetCapacity();
	if #tbPets == nPetCapacity then
		pPlayer.SendBlackMsg(Lang.pet.str24[Lang.Idx]);
		return 0;
	end
	assert(#tbPets < nPetCapacity);
	local pPet = KPet.AddPlayerPet(pPlayer, nPetRepresentId, nCharacter);
	--pPlayer.SysMsg("你获得了神仙: "..pPet.szName);

	return 1 -- success
end

function Pet:UpRank(pPet)

	--assert(pPet.nGrade == Pet.PET_MAX_GRADE); 无升品限制了

	-- TODO: 判断宠物品质，1品不能升阶，2品升满2阶，3品升满3阶，4品升满5阶，5品升满7阶，6品升满9

	local nQuality = pPet.nQuality;
	local nMaxRank = KAward.GetLevelNumOfStage("pet_uprank", pPet.nQuality);

	if pPet.nRankLevel >= nMaxRank then
		me.SendBlackMsg(string.format(Lang.pet.str25[Lang.Idx], nMaxRank));
		return;
	end

	local tbNumber = KAward.GetNumericalAwardData("pet_uprank", pPet.nQuality, pPet.nRankLevel + 1);

	local nCostNum = tbNumber.nCost;
	local tbCostGDPL = { 24, 2, 8 };	-- 升阶丸

	local tbFind = me.pItem.FindItemInBags(unpack(tbCostGDPL));
	local nHaveNum = Item:GetItemAmount(tbFind);

	if (nHaveNum < nCostNum) then
		local szErrMsg = string.format(Lang.pet.str26[Lang.Idx], nCostNum);
		me.SysMsg(szErrMsg);
		return;
	end
	me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));

	local bRet = pPet.Regenerate(
		pPet.nType,
		pPet.nQuality,
		pPet.nCharacter,
		pPet.nLevel,
		pPet.nRankLevel + 1,
		pPet.dwRandSeed
	);

	assert(bRet > 0);

	pPet.Sync();
	
	--统计点  ldy
	local szStr = string.format("nType=%d,nQuality=%d,nCharacter=%d,nLevel=%d,nRankLevel=%d",pPet.nType,pPet.nQuality,pPet.nCharacter,pPet.nLevel,pPet.nRankLevel + 1);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,60,szStr);
	DailyTasks:commitTaskValue(me,"",DailyTasks.TYPE_UPGREAD_PET,1);
	
	return bRet;
end

-- 检查传承条件是否符合并且扣除传承费用
function Pet:CheckCanTransExp(pSrcPet, pDstPet, nUseGold,nSkillBookNum,nUpRankBallNum)
	local nFreeCell = me.pItem.CountFreeBagCell();
	local nNeedCell = math.floor(nUpRankBallNum / 999) + 1 + math.floor(nSkillBookNum / 999) +1;
	if(nNeedCell > nFreeCell) then
		me.SendBlackMsg(Lang.pet.str27[Lang.Idx]);
		return 0;
	end
	if nUseGold == 1 then
		local nCurCoin = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
		local nNEED_COIN = 20;
		if (nCurCoin < nNEED_COIN) then
			me.SendBlackMsg(string.format(Lang.pet.str29[Lang.Idx], nNEED_COIN));
			return 0;
		end

		me.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNEED_COIN);
		me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNEED_COIN,49);
	else
		local nCurSilver = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_SILVER);
		local nNEED_SILVER = 1000;
		if (nCurSilver < nNEED_SILVER) then
			me.SendBlackMsg(string.format(Lang.pet.str30[Lang.Idx], nNEED_SILVER));
			return 0;
		end

		me.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, -nNEED_SILVER);
	end
	
	return 1;
end

function Pet:TransExp(pSrcPet, pDstPet, nUseGold)  -- 是否使用元宝传承
	local nExp = pSrcPet.GetTotalExp();
	if not nUseGold then
		nPercent = 90;
	else
		nPercent = 100;  -- 元宝传承
		-- 元宝
		local nCurCoin = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
		local nNEED_COIN = 50 -- 需求50个
		if (nCurCoin < nNEED_COIN) then
			me.SendBlackMsg(Lang.pet.str31[Lang.Idx])
			return
		end
	end

	nExp = nExp * nPercent / 100;

	-- 原来的宠物变成1级
	local bRet = pSrcPet.Regenerate(
		pSrcPet.nType,
		pSrcPet.nQuality,
		pSrcPet.nCharacter,
		1,
		pSrcPet.nRankLevel,
		pSrcPet.dwRandSeed
	);

	assert(bRet > 0);

	pDstPet.AddExp(nExp);

	me.SysMsg(Lang.pet.str32[Lang.Idx]);

	return 1;
end

function Pet:ResizePetBag()
	-- 扣元宝
	local nNeedCoin = 10;
	local nBindCoinNum = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_BIND_COIN)
	
	if nBindCoinNum >= nNeedCoin * 2 then  -- 优先消耗绑定元宝
		me.pPlayerPurse.AddMoney(Purse.EM_MONEY_BIND_COIN, -nNeedCoin * 2);
		local nCapacity = me.GetPetCapacity();
		assert(nCapacity < Pet.KD_MAX_PET_COUNT);
	
		me.SetPetCapacity(nCapacity + 1);
		return 1;
	end
	
	if me.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedCoin then
		me.SendBlackMsg(string.format(Lang.pet.str33[Lang.Idx], nNeedCoin));
		return -1;
	end
	me.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedCoin);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedCoin,47);

	local nCapacity = me.GetPetCapacity();
	assert(nCapacity < Pet.KD_MAX_PET_COUNT);

	me.SetPetCapacity(nCapacity + 1);
end

function Pet:c2s_UnlockEquipPos(pPlayer, nPetId, nPos)
	  -- 扣元宝
		local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
		local nNeedCoin = 10 -- 需求10个
		if (nCurCoin < nNeedCoin) then
			me.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", 10});
			return 0;
		end
    local pPet = KPet.GetPetById(nPetId);
    pPet.UnlockEquipPos(nPos);
end

function Pet:GiveBackItems(pPlayer, nUpRankBallNum, nSkillBookNum)
	if nUpRankBallNum > 0 then
    		KItem.AddPlayerItem(pPlayer, Pet.PET_UPRANK_GDPL[1], Pet.PET_UPRANK_GDPL[2], Pet.PET_UPRANK_GDPL[3], Pet.PET_UPRANK_GDPL[4], nUpRankBallNum);
   	end
	if nSkillBookNum > 0 then
		KItem.AddPlayerItem(pPlayer, Pet.PET_SKILLBOOK_GDPL[1], Pet.PET_SKILLBOOK_GDPL[2], Pet.PET_SKILLBOOK_GDPL[3], Pet.PET_SKILLBOOK_GDPL[4], nSkillBookNum);
	end
    return 1;
end

function Pet:c2s_UpgradePetSkill(pPlayer, nPetId, nSkillId)
    local pPet = KPet.GetPetById(nPetId);
		local nLevel = pPet.nLevel;
		local nQuality = pPet.nQuality;
		local nSkillLevel = pPet.GetSkillLevel(nSkillId);

		if nSkillLevel >= nLevel then
			me.SendBlackMsg(string.format(Lang.pet.str34[Lang.Idx]));
			return;
		end

		local tbNumber = KAward.GetNumericalAwardData("pet_upskill", nQuality, nSkillLevel + 1);

		local nCostNum = tbNumber.nCost;
		local tbCostGDPL = { 24, 8, 1 };	-- 技能书

		local tbFind = me.pItem.FindItemInBags(unpack(tbCostGDPL));
		local nHaveNum = Item:GetItemAmount(tbFind);

		if (nHaveNum < nCostNum) then
			local szErrMsg = string.format(Lang.pet.str35[Lang.Idx], nCostNum);
			me.SysMsg(szErrMsg);
			return;
		end
		me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));

		KPet.UpgradePetSkill(nPetId, nSkillId);
end
