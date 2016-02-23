---------------------------------- 策划调用的基本操作(装备相关) ------------------------------

function BufferMagicEffect:modifydmg_p(nValue0)
	self:Add_ModifyDmgP(me, nValue0);
end

function BufferMagicEffect:add_medicineeffect_p(nValue0)
	self:Add_MedicineEffect(me, nValue0);
end

function BufferMagicEffect:addweapondmg_v(nValue)
	self:Add_Attack(me, nValue);
end

function BufferMagicEffect:modifydmg_v(nValue)
	self:Add_Attack(me, nValue);
end

function BufferMagicEffect:addmaxlife_v(nValue0)
	self:Add_MaxLife(me, nValue0);
end

function BufferMagicEffect:addcurlife_v(nValue)
	local nCurrentLife = me.GetCurrentLife();
	nCurrentLife = nCurrentLife + nValue;
	if nCurrentLife > me.GetMaxLife() then
		nCurrentLife = me.GetMaxLife();
	end
	me.SetCurrentLife(nCurrentLife);
end

--[[
function BufferMagicEffect:addmaxlife_p()
end
--]]

function BufferMagicEffect:addmaxmana_v(nValue0)
	self:Add_MaxMana(me, nValue0);
end

function BufferMagicEffect:addcurmana_v(nValue)
	local nCurrentMana = me.GetCurrentMana();
	nCurrentMana = nCurrentMana + nValue;
	if nCurrentMana > me.GetMaxMana() then
		nCurrentMana = me.GetMaxMana();
	end
	me.SetCurrentMana(nCurrentMana);
end

--[[
function BufferMagicEffect:addmaxmana_p()
end
--]]

function BufferMagicEffect:addmaxpower_v(nValue0)
	self:Add_MaxPower(me, nValue0);
end

--[[
function BufferMagicEffect:addmaxpower_p()
end
--]]

function BufferMagicEffect:addhitrate_v(nValue0)
	self:Add_HitCoefficient(me, nValue0);
end

function BufferMagicEffect:addcriticalrate_v(nValue0)
	self:Add_CriticalValue(me, nValue0);
end

function BufferMagicEffect:addresist_v(nValue0)
	self:Add_Resist(me, nValue0);
end

function BufferMagicEffect:addcriticaldmg_p(nValue0)
	self:Add_CriticalAttack(me, nValue0);
end

function BufferMagicEffect:addrunspeed_v(nValue0)
	self:Add_Speed(me, nValue0);
end

function BufferMagicEffect:addrunspeed_p(nPercent)
	local nSpeed = math.floor(20 * nPercent / 100);
	self:Add_Speed(me, nSpeed);
end

function BufferMagicEffect:adddef_v(nValue0)
	self:Add_Def(me, nValue0);
end

function BufferMagicEffect:adddodge_v(nValue)
	self:Add_Dodge(me, nValue);
end

function BufferMagicEffect:rebound_dmg_p(nValue0)
	self:Add_ReboundP(me, nValue0);
end

function BufferMagicEffect:rebound_dmg_v(nValue0)
	self:Add_ReboundV(me, nValue0);
end

function BufferMagicEffect:addcurmana_v(nValue0)
	self:Add_CurrentMana(me, nValue0);
end

--[[
function BufferMagicEffect:reducedmg()
end

function BufferMagicEffect:transdmgtolife()
end

function BufferMagicEffect:transdmgtomana()
end
--]]

function BufferMagicEffect:addtreatlife_v(nValue0)
	self:Add_CuringLifeBase(me, nValue0);
end

--[[
function BufferMagicEffect:addtreatlife_p()
end
--]]

function BufferMagicEffect:addtreatmana_v(nValue0)
	self:Add_CuringManaBase(me, nValue0);
end

--[[
function BufferMagicEffect:addtreatmana_p()
end
--]]

--[[
function BufferMagicEffect:add_buffer()
end
--]]

-------------------------------------------------------------------------------------------