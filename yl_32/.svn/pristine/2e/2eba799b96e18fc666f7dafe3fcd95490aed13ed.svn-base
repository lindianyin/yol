

-- 药物技能
function FightSkill:ApplyMedicineSkill(nMeidicineType, nSkillLevel)
	local nValue = KFightSkill.GetMeidicineSkillNumber(nMeidicineType, nSkillLevel)

	if (nValue == nil) then
		return
	end

	if (nMeidicineType == 1) then -- 红药
		local nNewLife = math.min(me.GetMaxLife(), me.GetCurrentLife() + nValue) -- 取消，不能超过
		me.SetCurrentLife(nNewLife)
	elseif (nMeidicineType == 2) then
		local nNewMana = math.min(me.GetMaxMana(), me.GetCurrentMana() + nValue) -- 取消，不能超过
		me.SetCurrentMana(nNewMana)
	end
end

-- 被动技能
--function FightSkill:SetPassiveSkill(nPassiveType, nSkillLevel, bAddOrRemove, bSync)
	--local nValue = KFightSkill.GetPassiveSkillNumber(nPassiveType, nSkillLevel)
--end


-- 获取某个技能的对应的抗性技能类型~~~ 通过slot判断
function FightSkill:GetSkillResistType(pPlayer, nSkillTemplateId)
	local nSlot = KFightSkill.GetSkillSlot(nSkillTemplateId)
	local nRoute = KFightSkill.GetSkillRoute(nSkillTemplateId)  -- 路线 1近2远

	local nResistType =0
	if (nSlot == 1) then  -- 单体技能，抗性1
		if (nRoute == 1) then
			nResistType = 1  -- 近单抗性1
		elseif (nRoute == 2) then
			nResistType = 2  -- 远单抗性2
		end
	elseif (nSlot == 2) then
		nResistType = 3      ----群体抗性2
	elseif (nSlot == 3) then
		if (nRoute == 1) then
			nResistType = 4  -- 近控抗性 4
		elseif (nRoute == 2) then
			nResistType = 5  -- 远控抗性 5
		end
	elseif (nSlot == 4) then
		nResistType = 4      -- 必杀抗性6
	
	end

	return nResistType
end