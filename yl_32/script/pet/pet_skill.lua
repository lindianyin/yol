
Include("/script/pet/define.lua");

--[[
Pet.tbSkill = {};
Pet.tbSkillToLv = {};
Pet.tbSkillQuality = {};
Pet.tbSkillName = {};

local function PreloadSkillTemplate()
	local tbFileData = Lib:LoadTabFile("\\setting\\pet\\petskill.txt")
	for _, tb in pairs(tbFileData) do
		local nSkillId = tonumber(tb.SkillId1);
		if not Pet.tbSkill[nSkillId] then
			Pet.tbSkill[nSkillId] = {};
			local tbSkillList = Pet.tbSkill[nSkillId];
			for i = 1, 30 do
				local nId = tonumber(tb["SkillId"..i])
				tbSkillList[i] = nId;
				Pet.tbSkillToLv[nId] = {nBaseSkill = nSkillId, nLevel = i};
			end
		end
		
		local nQuality = tonumber(tb.SkillQuality);
		Pet.tbSkillQuality[nSkillId] = nQuality;
		
		Pet.tbSkillName[nSkillId] = tb.SkillName;
	end
end

PreloadSkillTemplate();

-- 得到下一个等级的技能Id
local function GetNextSkillId(nSkillId)
	local tbInfo = Pet.tbSkillToLv[nSkillId];
	assert(tbInfo);
	local nBaseSkill = tbInfo.nBaseSkill;
	local nLevel = tbInfo.nLevel;
	assert(nLevel > 0 and nLevel < 30);
	return Pet.tbSkill[nBaseSkill][nLevel + 1];
end

local function GetSkillId(nSkillId, nSkillLevel)
	assert(nSkillLevel > 0 and nSkillLevel < 30);
	local tbInfo = Pet.tbSkillToLv[nSkillId];
	assert(tbInfo);
	local nBaseSkill = tbInfo.nBaseSkill;
	return Pet.tbSkill[nBaseSkill][nSkillLevel];
end

local function GetSkillLevel(nSkillId)
	local tbInfo = Pet.tbSkillToLv[nSkillId];
	assert(tbInfo);
	return tbInfo.nLevel;
end

local function GetBaseSkillId(nSkillId)
	local tbInfo = Pet.tbSkillToLv[nSkillId];
	assert(tbInfo);
	return tbInfo.nBaseSkill;
end

function Pet:IfHasSkill(pPet, nSkillId)
	nSkillId = GetBaseSkillId(nSkillId);
	local tbPetSkill = pPet.GetSkillList();
	
	for _, nId in ipairs(tbPetSkill) do
		nId = GetBaseSkillId(nId);
		if nSkillId == nId then
			return 1;
		end
	end
	
	return 0;
end

function Pet:RandSkillId(pPet)
	local tbSkill = {};
	local nQuality = Lib:RandProb(self.SKILL_QUALITY);
	assert(nQuality);
	
	-- 从随机技能列表中随机出一个宠物身上没有的技能ID
	local tbPetSkill = pPet.GetSkillList();
	
	for nSkillId, tb in pairs(self.tbSkill) do
		local nFind = 0;
		for _, nId in ipairs(tbPetSkill) do
			nId = GetBaseSkillId(nId);
			if nSkillId == nId then
				nFind = 1;
				break;
			end
		end
		
		if nFind == 0 and Pet.tbSkillQuality[nSkillId] == nQuality then
			table.insert(tbSkill, nSkillId);
		end
	end
	
	assert(#tbSkill > 0);
	
	local nIdx = KUnify.MathRandom(#tbSkill);
	local nRandSkillId = tbSkill[nIdx];
	
	local szSkillMsg = string.format("您的宠物[%s%s<color=white>]成功领悟技能[%s%s<color=white>]", Pet.SKILL_COLOR[pPet.nQuality], pPet.szName, Pet.SKILL_COLOR[nQuality], self.tbSkillName[nRandSkillId]);
	me.SendBlackMsg(szSkillMsg);
	
	return nRandSkillId;
end

local PARAM_SKILL_ID = 1;		-- 宠物技能书记录的技能ID
local PARAM_SKILL_LEVEL = 2;

function Pet:LearnSkill(pPet, pItem)
	local nSkillId = pItem.GetExtParam(PARAM_SKILL_ID);
	local nSkillLevel = pItem.GetExtParam(PARAM_SKILL_LEVEL);
	if nSkillLevel == 0 then
		nSkillLevel = 1;
	end
	
	if self:IfHasSkill(pPet, nSkillId) == 1 then
		me.SysMsg("这只宠物已经有这个技能了，无需再学习");
		return;
	end
	
	-- 根据基础id和等级得到要学习的技能id
	nSkillId = GetSkillId(nSkillId, nSkillLevel);
	
	local nBaseSkillId = GetBaseSkillId(nSkillId);
	
	local nFind = 0;
	for nId, tb in pairs(self.tbSkill) do
		if nBaseSkillId == nId then
			nFind = 1;
			break;
		end
	end
	assert(nFind > 0);
	
	KItem.DelPlayerItem(me, pItem);
	
	local bRet = pPet.LearnSkill(nSkillId);
	assert(bRet > 0);
	
	self:DelayCalcFightScore(pPet);
	
	return pPet.Sync();
end

function Pet:UpgradeSkill(pPet, nPos)
	local nSkillId = pPet.GetSkillId(nPos);
	assert(nSkillId > 0);
	
	local nSkillLevel = GetSkillLevel(nSkillId);
	if nSkillLevel >= 30 then
		me.SysMsg("你的技能已经升到最高级");
		return;
	end
	
	local nReqLevel = KFightSkill.GetReqLevel(nSkillId);
	if me.GetLevel() < nReqLevel then
		me.SysMsg(string.format("升级这个技能需要%d级，你目前的等级不足", nReqLevel));
		return;
	end
	
	local nNeedYLE = KFightSkill.GetYlEnery(nSkillId);
	if me.pPlayerPurse.GetMoney(Purse.EM_MONEY_YLENERGY) < nNeedYLE then
		me.SysMsg(string.format("升级这个技能需要游龙真气%d点，你的游龙真气不足", nNeedYLE));
		return;
	end
	
	nSkillId = GetNextSkillId(nSkillId);

	me.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, -nNeedYLE);
	
	local bRet = pPet.UpgradeSkill(nPos, nSkillId);
	assert(bRet > 0);
	
	me.SysMsg("升级成功！");
	
	self:DelayCalcFightScore(pPet);
	
	return pPet.Sync();
end

function Pet:RecordSkill(pPet, nPos)
	local tbCostGDPL = { 24, 7, 1 };	-- 拓印书
	
	local tbFind = me.pItem.FindItem(Item.ROOM_PET, unpack(tbCostGDPL));
	if (#tbFind < 1) then
		local szErrMsg = string.format("请随身携带1本宠物技能拓印书");
		me.SysMsg(szErrMsg);
		return;
	end
	
	local nSkillId = pPet.GetSkillId(nPos);
	assert(nSkillId > 0);
	local nSkillLevel = GetSkillLevel(nSkillId);
	
	nSkillId = GetBaseSkillId(nSkillId);
	
	local tbSkillBook = self.SKILL_BOOK_GDP[nSkillId];
	assert(tbSkillBook);
	
	me.pItem.ConsumeItemInBags(1, unpack(tbCostGDPL));
	local pItem = KItem.AddPlayerItem(me, unpack(tbSkillBook));
	
	-- 需要拓印时考虑等级就取消下面这句话的注释
	--pItem.SetExtParam(PARAM_SKILL_LEVEL, nSkillLevel);
	
	pPet.RemoveSkill(nPos);
	
	return pPet.Sync();
end

function Pet:DelayCalcFightScore(pPet)
	Timer:Register(6, self.CalcFightScore, self, pPet);
end

function Pet:CalcFightScore(pPet)
	local nScore = pPet.GetBaseAttrScore();
	local tbPetSkill = pPet.GetSkillList();
	
	-- 宠物技能每级加3点战斗力
	for _, nId in ipairs(tbPetSkill) do
		local nSkillLevel = GetSkillLevel(nId);
		nScore = nScore + 3 * nSkillLevel;
	end
	
	pPet.SetFightScore(nScore);
	
	pPet.Sync();
	
	return 0;
end

--]]