---------------------------------------- Buff用来修改玩家基本属性的函数 -------------------------------------

BufferMagicEffect.IsRawSet = 0;

function BufferMagicEffect:SetAttrRawSet(bRawSet)
	assert(bRawSet == 0 or bRawSet == 1);
	BufferMagicEffect.IsRawSet = bRawSet;
end

-- 最大生命值
function BufferMagicEffect:Add_MaxLife(character, nValue)
	local nNewValue = character.GetMaxLife() + nValue;

	--assert(nNewValue >= 90);
	
	if self.IsRawSet == 1 then
		character.RawSetMaxLife(nNewValue);
	else
		character.SetMaxLife(nNewValue);
		local isInFightMap = character.GetIsFightMap()
		if isInFightMap <= 0 then -- 在主城血量与最大血量保持一致
			 character.SetCurrentLife(nNewValue);
		end
	end
end

function BufferMagicEffect:TransParamValue(character, ParamIdx, nValue)
	local tbParam = character.pBufferEffect.GetTransParam(ParamIdx);

	for nParamIdx, nPercent in pairs(tbParam) do
		local nValueEx = nValue * nPercent / 100;
		if nValueEx < 0 then
			nValueEx = math.ceil(nValueEx);
		end
		self:AddParamValue(character, nParamIdx, nValueEx);
	end
end

function BufferMagicEffect:Add_MaxLifeEx(character, nValue)
	local ParamIdx = self:GetParamIdx("MaxLife");
	self:TransParamValue(character, ParamIdx, nValue);
end

function BufferMagicEffect:Get_MaxLife(character)
	local nValue = character.GetMaxLife();

	if (nValue < 90) then
		nValue = 90;
	end

	return nValue;
end

-- 最大内力值
function BufferMagicEffect:Add_MaxMana(character, nValue)
	local nNewValue = character.GetMaxMana() + nValue;

	--assert(nNewValue >= 90);

	if self.IsRawSet == 1 then
		character.RawSetMaxMana(nNewValue);
	else
		character.SetMaxMana(nNewValue);
		if (nNewValue < character.GetCurrentMana()) then
			character.SetCurrentMana(nNewValue);
		end
	end
end

function BufferMagicEffect:Get_MaxMana(character)
	local nValue = character.GetMaxMana();

	if (nValue < 90) then
		nValue = 90;
	end

	return nValue;
end

-- 最大力量值
function BufferMagicEffect:Add_MaxPower(character, nValue)
	local nNewValue = character.GetMaxPower() + nValue;

	assert(nNewValue >= 100);

	if self.IsRawSet == 1 then
		character.RawSetMaxPower(nNewValue);
	else
		character.SetMaxPower(nNewValue);
		if (nNewValue < character.GetCurrentPower()) then
			character.SetCurrentPower(nNewValue);
		end
	end
end

function BufferMagicEffect:Get_MaxPower(character)
	local nValue = character.GetMaxPower();

	if (nValue < 100) then
		nValue = 100;
	end

	return nValue;
end

-- 当前生命值
function BufferMagicEffect:Add_CurrentLife(caster, becast, nValue, bCriticalHit, bGodHarm)
	--print("..BufferMagicEffect:Add_CurrentLife",nValue);
	if nValue == 0 then
		return;
	end

	if (nValue < 0) then
	  local ignoreDmgRate = becast.GetIgnorDmg();
	  local nRate = KUnify.MathRandom(1, 100);
	  if (nRate <= ignoreDmgRate) then  -- 免疫对方伤害
	      self:ClientShowTextOnHead(caster, becast, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextImmune); -- 免疫2
	      return;
	  end
	  
		KCharacter.ReduceLife(caster.dwId, becast.dwId, -nValue);
		if bCriticalHit == 0 then
			-- 普通扣血
			if caster.IsPlayer() == 1 then -- 对方扣血，显示黄血
			  if (bGodHarm == 1) then
				    caster.ClientShowHeadText(becast.dwId, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextEnemyBloodByGod, nValue);
				else
				    caster.ClientShowHeadText(becast.dwId, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextEnemyBlood, nValue);
				end
			end
			if becast.IsPlayer() == 1 then -- 自己扣血，显示红血
			  if (bGodHarm == 1) then
				    becast.ClientShowHeadText(becast.dwId, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextBloodByGod, nValue);
				else
				    becast.ClientShowHeadText(becast.dwId, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextBlood, nValue);
				end
			end
		else
			-- 暴击扣血
			if caster.IsPlayer() == 1 then -- 对方扣血，显示黄血
				caster.ClientShowHeadText(becast.dwId, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextEnemyCriticalHit, nValue);
			end
			if becast.IsPlayer() == 1 then -- 自己扣血，显示红血
				becast.ClientShowHeadText(becast.dwId, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextCriticalHit, nValue);
			end
		end

		--事件触发
		BufferCondition:DmgIn(becast, caster);
		BufferCondition:DmgOut(caster, becast);

		if (becast.IsDead() == 1) then
			BufferCondition:Death(becast, caster);
			BufferCondition:Kill(caster, becast);
		end

		-- TODO 计算伤害威胁
		if becast.dwId ~= caster.dwId then
			local nThreat = -nValue
			becast.ModifyThreat(caster.dwId, nThreat)
		end

		if (becast.IsNpc() == 1 and becast.GetCurrentLife() > 0) then
			-- 触发其他事件
			CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_SELFHURT, becast.dwId, 0, 0);
		end
	else
		KCharacter.AddLife(caster.dwId, becast.dwId, nValue);
		--self:ClientShowTextOnHead(caster, becast, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextAddBlood, nValue);  -- 加血，type0
	end
end

function BufferMagicEffect:Get_CurrentLife(character)
	local nValue = character.GetCurrentLife();

	if (nValue < 0) then
		nValue = 0;
		self:DebugPrint("wrong CurrentLife < 0");
	end

	return nValue;
end

-- 当前内力值
function BufferMagicEffect:Add_CurrentMana(character, nValue)

	local nNewValue = character.GetCurrentMana() + nValue;

	if (nNewValue < 0) then
		nNewValue = 0;
	elseif (character.GetMaxMana() < nNewValue) then
		nNewValue = character.GetMaxMana();
	end

	character.SetCurrentMana(nNewValue);
end

function BufferMagicEffect:Get_CurrentMana(character)
	local nValue = character.GetCurrentMana();

	if (nValue < 0) then
		nValue = 0;
		self:DebugPrint("wrong CurrentMana < 0");
	end

	return nValue;
end

-- 当前力量值
function BufferMagicEffect:Add_CurrentPower(character, nValue)

	local nNewValue = character.GetCurrentPower() + nValue;

	if (nNewValue < 0) then
		nNewValue = 0;
	elseif (character.GetMaxPower() < nNewValue) then
		nNewValue = character.GetMaxPower();
	end

	character.SetCurrentPower(nNewValue);
end

function BufferMagicEffect:Get_CurrentPower(character)
	local nValue = character.GetCurrentPower();

	if (nValue < 0) then
		nValue = 0;
		self:DebugPrint("wrong CurrentPower < 0");
	end

	return nValue;
end

-- 物理攻击力
function BufferMagicEffect:Add_Attack(character, nValue)
	local nNewValue = character.GetAttack() + nValue;
	if self.IsRawSet == 1 then
		character.RawSetAttack(nNewValue);
	else
		character.SetAttack(nNewValue);
	end
end

function BufferMagicEffect:Get_Attack(character)
	return character.GetAttack();
end

-- 命中系数
function BufferMagicEffect:Add_HitCoefficient(character, nValue)
	local nNewValue = character.GetHitCoefficient() + nValue;

	if self.IsRawSet == 1 then
		character.RawSetHitCoefficient(nNewValue);
	else
		character.SetHitCoefficient(nNewValue);
	end
end

function BufferMagicEffect:Get_HitCoefficient(character)
	local nValue = character.GetHitCoefficient();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 伤害加成
function BufferMagicEffect:Add_ModifyDmgP(character, nValue)
	local nNewValue = character.GetModifyDmgP() + nValue;

	assert(nNewValue >= 0);

	if self.IsRawSet == 1 then
		character.RawSetModifyDmgP(nNewValue);
	else
		character.SetModifyDmgP(nNewValue);
	end
end

function BufferMagicEffect:Get_ModifyDmgP(character)
	local nValue = character.GetModifyDmgP();

	assert(nValue >= 0);

	return nValue;
end

-- 药效
function BufferMagicEffect:Add_MedicineEffect(character, nValue)
	local nNewValue = character.GetMedicineEffect() + nValue;

	assert(nNewValue >= -100);

	character.SetMedicineEffect(nNewValue);
end

function BufferMagicEffect:Get_MedicineEffect(character)
	local nValue = character.GetMedicineEffect();

	return nValue;
end

-- 自身命中率(包括额外命中率)
function BufferMagicEffect:Get_HitRate(character)
	local nValue = self:Get_HitCoefficient(character);

	return nValue / (character.GetLevel() * 2000) + 0.9;
end

-- 闪避值
function BufferMagicEffect:Add_Dodge(character, nValue)
	local nNewValue = character.GetDodge() + nValue;

	if self.IsRawSet == 1 then
		character.RawSetDodge(nNewValue);
	else
		character.SetDodge(nNewValue);
	end
end

function BufferMagicEffect:Get_Dodge(character)
	local nValue = character.GetDodge();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 物理闪避率
function BufferMagicEffect:Get_DodgeRate(character)
	local nValue = self:Get_Dodge(character);

	return nValue / (character.GetLevel() * 2000);
end

-- 防御
function BufferMagicEffect:Add_Def(character, nValue)
	local nNewValue = character.GetDef() + nValue;

	if self.IsRawSet == 1 then
		character.RawSetDef(nNewValue);
	else
		character.SetDef(nNewValue);
	end
end

function BufferMagicEffect:Get_Def(character)
	local nValue = character.GetDef();

	if (nValue < 0) then
		nValue = 0;
	end
	return nValue;
end

-- 移动速度
function BufferMagicEffect:Add_Speed(character, nValue)
	local nNewValue = character.GetSpeed() + nValue;

	if character.IsPlayer() == 1 then
		assert(nNewValue >= 2);
	end

    if nNewValue > 0 then
		if self.IsRawSet == 1 then
			character.RawSetSpeed(nNewValue);
		else
		  if character.IsPlayer() == 1 then
			    character.SetSpeedDelaySet(nNewValue);
			else
			    character.SetSpeed(nNewValue);
		  end
		end
	end
end

function BufferMagicEffect:Get_Speed(character)
	local nValue = character.GetSpeed();

	if (nValue < 5) then
		nValue = 5;
	end

	return nValue;
end

-- 会心值
function BufferMagicEffect:Add_CriticalValue(character, nValue)
	local nNewValue = character.GetCriticalValue() + nValue;

	if self.IsRawSet == 1 then
		character.RawSetCriticalValue(nNewValue);
	else
		character.SetCriticalValue(nNewValue);
	end
end

function BufferMagicEffect:Get_CriticalValue(character)
	local nValue = character.GetCriticalValue();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 招架
function BufferMagicEffect:Add_Resist(character, nValue)
	local nNewValue = character.GetResist() + nValue;

	if self.IsRawSet == 1 then
		character.RawSetResist(nNewValue);
	else
		character.SetResist(nNewValue);
	end
end

function BufferMagicEffect:Get_Resist(character)
	local nValue = character.GetResist();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 会心率（物理）
function BufferMagicEffect:Get_CriticalRate(character)
	local nValue = self:Get_CriticalValue(character);

	local rate = nValue  / (5 * character.GetLevel() * character.GetLevel() + 1000);

	return rate;
end

-- 会心伤害
function BufferMagicEffect:Add_CriticalAttack(character, nValue)
	local nNewValue = character.GetCriticalAttack() + nValue;

	if self.IsRawSet == 1 then
		character.RawSetCriticalAttack(nNewValue);
	else
		character.SetCriticalAttack(nNewValue);
	end
end

function BufferMagicEffect:Get_CriticalAttack(character)
	local nValue = character.GetCriticalAttack();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 抵御会心伤害
function BufferMagicEffect:Add_CriticalDeAttack(character, nValue)
	local nNewValue = character.GetCriticalDeAttack() + nValue;

	character.SetCriticalDeAttack(nNewValue);
end

function BufferMagicEffect:Get_CriticalDeAttack(character)
	local nValue = character.GetCriticalDeAttack();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 治疗生命值
function BufferMagicEffect:Add_CuringLifePercent(character, nValue)
	local nNewValue = character.GetCuringLife() + nValue;

	character.SetCuringLife(nNewValue);
end

function BufferMagicEffect:Add_CuringLifeBase(character, nValue)
	local nNewValue = character.GetCuringLifeBase() + nValue;

	character.SetCuringLifeBase(nNewValue);
end

function BufferMagicEffect:Get_CuringLifePercent(character)
	local nValue = character.GetCuringLife();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

function BufferMagicEffect:Get_CuringLifeBase(character)
	local nValue = character.GetCuringLifeBase();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 治疗内力值
function BufferMagicEffect:Add_CuringManaPercent(character, nValue)
	local nNewValue = character.GetCuringMana() + nValue;
	character.SetCuringMana(nNewValue);
end

function BufferMagicEffect:Add_CuringManaBase(character, nValue)
	local nNewValue = character.GetCuringManaBase() + nValue;
	character.SetCuringManaBase(nNewValue);
end

function BufferMagicEffect:Get_CuringManaPercent(character)
	local nValue = character.GetCuringMana();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

function BufferMagicEffect:Get_CuringManaBase(character)
	local nValue = character.GetCuringManaBase();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 普通抵挡伤害百分比[0~100]
function BufferMagicEffect:Set_WithstandNormalP(character, nValue)
	character.SetWithstandNormalP(nValue);
end

function BufferMagicEffect:Get_WithstandNormalP(character)
	local nValue = character.GetWithstandNormalP() / 100;

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 普通抵挡伤害值
function BufferMagicEffect:Add_WithstandNormalV(character, nValue)
	local nNewValue = character.GetWithstandNormalV() + nValue;
	character.SetWithstandNormalV(nNewValue);
end

function BufferMagicEffect:Set_WithstandNormalV(character, nValue)
	character.SetWithstandNormalV(nValue);
end

function BufferMagicEffect:Get_WithstandNormalV(character)
	local nValue = character.GetWithstandNormalV();

	if (nValue < 0) then
		nValue = -1;
	end

	return nValue;
end

-- 反弹伤害百分比[0~100]
function BufferMagicEffect:Add_ReboundP(character, nValue)
	local nNewValue = character.GetReboundP() + nValue;

	character.SetReboundP(nNewValue);
end

function BufferMagicEffect:Get_ReboundP(character)
	local nValue = character.GetReboundP() / 100;

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 反弹伤害值
function BufferMagicEffect:Add_ReboundV(character, nValue)
	local nNewValue = character.GetReboundV() + nValue;
	character.SetReboundV(nNewValue);
end

function BufferMagicEffect:Get_ReboundV(character)
	local nValue = character.GetReboundV();

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 伤害转生命[0~100]
function BufferMagicEffect:Add_TransToLifeP(character, nValue)
	local nNewValue = character.GetTransToLifeP() + nValue;
	character.SetTransToLifeP(nNewValue);
end

function BufferMagicEffect:Get_TransToLifeP(character)
	local nValue = character.GetTransToLifeP() / 100;

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 伤害转魔法[0~100]
function BufferMagicEffect:Add_TransToManaP(character, nValue)
	local nNewValue = character.GetTransToManaP() + nValue;
	character.SetTransToManaP(nNewValue);
end

function BufferMagicEffect:Get_TransToManaP(character)
	local nValue = character.GetTransToManaP() / 100;

	if (nValue < 0) then
		nValue = 0;
	end

	return nValue;
end

-- 職業係數(仇恨用)
function BufferMagicEffect:Set_OccupationCoefficient(character, nValue)
	character.SetOccupationCoefficient(nValue);
end

function BufferMagicEffect:Get_OccupationCoefficient(character)
	return character.GetOccupationCoefficient();
end

-- 眩晕
function BufferMagicEffect:Add_IsStun(character, nValue)
	local nNewValue = character.GetIsStun() + nValue;
	character.SetIsStun(nNewValue);
end

function BufferMagicEffect:Get_IsStun(character)
	return 0 < character.GetIsStun();
end

function BufferMagicEffect:Get_IsFix(character)
	return 0 < character.GetIsFix();
end

-- 恐惧
function BufferMagicEffect:Add_IsFear(character, nValue)
	local nNewValue = character.GetIsFear() + nValue;
	character.SetIsFear(nNewValue);
end

function BufferMagicEffect:Get_IsFear(character)
	return 0 < character.GetIsFear();
end

-- 沉默
function BufferMagicEffect:Add_IsSilence(character, nValue)
	local nNewValue = character.GetIsSilence() + nValue;
	character.SetIsSilence(nNewValue);
end

function BufferMagicEffect:Get_IsSilence(character)
	return 0 < character.GetIsSilence();
end

-- 疯狂
function BufferMagicEffect:Add_IsCrazy(character, nValue)
	local nNewValue = character.GetIsCrazy() + nValue;
	character.SetIsCrazy(nNewValue);
end

function BufferMagicEffect:Get_IsCrazy(character)
	return 0 < character.GetIsCrazy();
end

-- 对NPC隐藏
function BufferMagicEffect:Add_IsHidetoNPC(character, nValue)
	local nNewValue = character.GetIsHidetoNPC() + nValue;
	character.SetIsHidetoNPC(nNewValue);
end

function BufferMagicEffect:Get_IsHidetoNPC(character)
	return 0 < character.GetIsHidetoNPC();
end

-- 对PLAYER隐藏
function BufferMagicEffect:Add_IsHidetoPlayer(character, nValue)
	local nNewValue = character.GetIsHidetoPlayer() + nValue;
	character.SetIsHidetoPlayer(nNewValue);
end

function BufferMagicEffect:Get_IsHidetoPlayer(character)
	return 0 < character.GetIsHidetoPlayer();
end

-- 免疫伤害
function BufferMagicEffect:Add_IgnorDmg(character, nValue)
	local nNewValue = character.GetIgnorDmg() + nValue;
	character.SetIgnorDmg(nNewValue);
end

function BufferMagicEffect:Get_IgnorDmg(character)
	return 0 < character.GetIgnorDmg();
end

function BufferMagicEffect:Add_EnlargeAttackP(character, nValue)
	local nNewValue = character.GetEnlargeAttackP() + nValue;

	character.SetEnlargeAttackP(nNewValue);
end

function BufferMagicEffect:Add_ReduceAttackP(character, nValue)
	local nNewValue = character.GetReduceAttackP() + nValue;

	character.SetReduceAttackP(nNewValue);
end

-- 免疫会心一击
function BufferMagicEffect:Add_IgnorCristal(character, nValue)
	local nNewValue = character.GetIgnorCristal() + nValue;
	character.SetIgnorCristal(nNewValue);
end

function BufferMagicEffect:Get_IgnorCristal(character)
	return 0 < character.GetIgnorCristal();
end

-- 免疫所有debuff
function BufferMagicEffect:Add_IgnorDebuff(character, nValue)
	local nNewValue = character.GetIgnorDebuff() + nValue;
	character.SetIgnorDebuff(nNewValue);
end

function BufferMagicEffect:Get_IgnorDebuff(character)
	return 0 < character.GetIgnorDebuff();
end

----------------------------------------------------------------------------------------
