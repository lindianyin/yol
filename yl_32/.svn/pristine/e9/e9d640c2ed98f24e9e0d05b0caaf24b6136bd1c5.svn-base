------------------------------------------  魔法属性函数逆运算  ----------------------------------

-- 伤害生命值
function BufferMagicEffect:de_me_damagelife_v()
	--self:DebugPrint("");
end

-- 伤害生命百分比
function BufferMagicEffect:de_me_damagelife_p()
	-- self:DebugPrint("");
end

-- 伤害真气值
function BufferMagicEffect:de_me_damagemana_v()
	-- self:DebugPrint("");
end

-- 伤害真气百分比
function BufferMagicEffect:de_me_damagemana_p()
	-- self:DebugPrint("");
end

-- 击退
function BufferMagicEffect:de_me_knock()
	-- self:DebugPrint("");
end

-- 击杀目标
function BufferMagicEffect:de_me_dodeath()
	-- self:DebugPrint("");
end

-- 复活目标
function BufferMagicEffect:de_me_dorevive()
	-- self:DebugPrint("");
end

-- 清除对应id的buff
function BufferMagicEffect:de_me_clearbuff()
	-- self:DebugPrint("");
end

-- 清除debuff（debuff列填1的buff）
function BufferMagicEffect:de_me_cleardebuff()
	-- self:DebugPrint("");
end

-- 清除对应id的skill的cd时间
function BufferMagicEffect:de_me_clearcd()
	-- self:DebugPrint("");
end

-- 将目标拉到自己身前
function BufferMagicEffect:de_me_pulltarget()
	-- self:DebugPrint("");
end

-- 传送到目标地图的对应点，该点可动态生成，即可用于拉传送的技能
function BufferMagicEffect:de_me_setmappos()
	-- self:DebugPrint("");
end

-- 回城，回到之前保存过的安全区的对应位置
function BufferMagicEffect:de_me_tp()
	-- self:DebugPrint("");
end

-- 增加对应id的buff的时间
function BufferMagicEffect:de_me_addbufftime()
	-- self:DebugPrint("");
end

-- 重置id的buff的时间
function BufferMagicEffect:de_me_refreshbufftime()
	-- self:DebugPrint("");
end

-- 删除掉该效果所在的buff自身
function BufferMagicEffect:de_me_kill_self()
	-- self:DebugPrint("");
end


function BufferMagicEffect:de_me_cast_skill()
	-- self:DebugPrint("");
end

function BufferMagicEffect:de_me_hide()
	self:DebugPrint("de_me_hide");
	me.SetSkillHidden(0);
end

-- 给对应cate的技能添加对应id的buff
function BufferMagicEffect:de_me_add_bufftoskill(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_BUFF);
	him.pFightSkill.AddBuffToSkill(nSkillCateType, nSkillCateId, -nAppendValue);

	self:DebugPrint("de_me_add_bufftoskill");
end

-- 从对应cate的技能删除对应id的buff
function BufferMagicEffect:de_me_del_bufffromskill(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_BUFF);
	him.pFightSkill.AddBuffToSkill(nSkillCateType, nSkillCateId, -nAppendValue);

	self:DebugPrint("de_me_del_bufffromskill");
end

-- 增加当前生命值
function BufferMagicEffect:de_me_addcurlife_v()
	-- self:DebugPrint("");
end

-- 增加当前生命百分比
function BufferMagicEffect:de_me_addcurlife_p()
	-- self:DebugPrint("");
end

function BufferMagicEffect:de_me_state_add_buff()

end

-- 增加当前真气值
function BufferMagicEffect:de_me_addcurmana_v()
	-- self:DebugPrint("");
end

function BufferMagicEffect:de_me_add_groundbuffer()
	self:DebugPrint("de_me_add_groundbuffer");
end

-- 增加当前真气百分比
function BufferMagicEffect:de_me_addcurmana_p()
	-- self:DebugPrint("");
end

-- 增加当前集中值
function BufferMagicEffect:de_me_addcurpower_v()
	-- self:DebugPrint("");
end

-- 眩晕
function BufferMagicEffect:de_me_stun(nBuffTemplateId)
	--local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_STUN);
	--self:Add_IsStun(me, -nAppendValue);
	me.SetIsStun(0);
	if me.GetMoveState() == Character.MOVE_STATE_ON_HALT then
		me.SetMoveState(Character.MOVE_STATE_ON_STAND);
	end

	self:DebugPrint("de_me_stun");
end

function BufferMagicEffect:de_me_pet_damagelife_v(nBuffTemplateId)

end

function BufferMagicEffect:de_me_pet_addselflife(nBuffTemplateId)

end

function BufferMagicEffect:de_me_pet_subtargetlife(nValue, nBuffTemplateId, nSkillLevel)

end

function BufferMagicEffect:de_me_pet_addselfattack(nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ATTACK);

	self:Add_Attack(him, -nAppendValue);

	self:DebugPrint("de_me_pet_addselfattack", nAppendValue);
	print("de_me_pet_addselfattack", nAppendValue);
end

function BufferMagicEffect:de_me_pet_subtargetdefence(nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ARMOR);

	self:Add_Def(him, nAppendValue);

	self:DebugPrint("de_me_pet_subtargetdefence", nAppendValue);
end

function BufferMagicEffect:de_me_invisibleToNpc()
    me.SetIsVisiontoNPC(0)
    print("de_me_invisibleToNpc", me.szName)
end

-- 定身
function BufferMagicEffect:de_me_fix()
	me.SetIsFix(0);
	if me.GetMoveState() == Character.MOVE_STATE_ON_FREEZE then
		me.SetMoveState(Character.MOVE_STATE_ON_STAND);
	end

  print("de_me_fix", me.szName);
	self:DebugPrint("de_me_fix");
end

-- 恐惧
function BufferMagicEffect:de_me_fear(nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_FEAR);
	self:Add_IsFear(me, -nAppendValue);

	if me.GetMoveState() == Character.MOVE_STATE_ON_HALT then
		me.SetMoveState(Character.MOVE_STATE_ON_STAND);
	end

	self:DebugPrint("de_me_fear");
end

-- 沉默
function BufferMagicEffect:de_me_silence(nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SILENCE);
	self:Add_IsSilence(me, -nAppendValue);

	if me.GetMoveState() == Character.MOVE_STATE_ON_HALT then
		me.SetMoveState(Character.MOVE_STATE_ON_STAND);
	end

	self:DebugPrint("de_me_silence");
end

-- 疯狂
function BufferMagicEffect:de_me_crazy(nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CRAZY);
	self:Add_IsCrazy(me, -nAppendValue);

	self:DebugPrint("de_me_crazy");
end

function BufferMagicEffect:de_me_ignoredmg()
	me.SetIgnorDmg(0);
	self:DebugPrint("de_me_ignoredmg");
end

-- 减速
function BufferMagicEffect:de_me_slow_p(nBuffTemplateId)
	local nSpeed = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED, -nSpeed);
	self:Add_Speed(me, -nSpeed);

	local nSpeedPercent = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED_PERCENT);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED_PERCENT, -nSpeedPercent);
	self:DebugPrint("de_me_slow_p");
end

-- 忽略目标护甲
function BufferMagicEffect:de_me_ignorearmor_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ARMOR);
	self:Add_Def(me, -nAppendValue);

	self:DebugPrint("de_me_ignorarmor_v");
end

-- 更改造成的伤害百分比
function BufferMagicEffect:de_me_modifydmg_p(nBuffTemplateId)
	local nModifyDmgP = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MODIFYDMG_P);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MODIFYDMG_P, -nModifyDmgP);
	self:Add_ModifyDmgP(me, -nModifyDmgP);

	self:DebugPrint("de_me_modifydmg_p");
end

function BufferMagicEffect:de_me_add_medicineeffect_p(nBuffTemplateId)
	local nMedicineEffect = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MEDICINE_EFFECT);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MEDICINE_EFFECT, -nMedicineEffect);
	self:Add_ModifyDmgP(me, -nMedicineEffect);

	self:DebugPrint("de_me_add_medicineeffect_p");
end

-- 更改造成的伤害值
function BufferMagicEffect:de_me_modifydmg_v()
	self:DebugPrint("de_me_modifydmg_v");
end

-- 增加最大生命值
function BufferMagicEffect:de_me_addmaxlife_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXLIFE);
	self:Add_MaxLife(me, -nAppendValue);

	self:DebugPrint("de_me_addmaxlife_v");
end

-- 增加最大生命百分比
function BufferMagicEffect:de_me_addmaxlife_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXLIFE);
	self:Add_MaxLife(me, -nAppendValue);

	self:DebugPrint("de_me_addmaxlife_p");
end

-- 增加最大真气值
function BufferMagicEffect:de_me_addmaxmana_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXMANA);
	self:Add_MaxMana(me, -nAppendValue);

	self:DebugPrint("de_me_addmaxmana_v");
end

-- 增加最大真气百分比
function BufferMagicEffect:de_me_addmaxmana_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXMANA);
	self:Add_MaxMana(me, -nAppendValue);

	self:DebugPrint("de_me_addmaxmana_p");
end

-- 增加最大集中值
function BufferMagicEffect:de_me_addmaxpower_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXPOWER);
	self:Add_MaxPower(me, -nAppendValue);

	self:DebugPrint("de_me_addmaxpower_v");
end

-- 增加最大集中值百分比
function BufferMagicEffect:de_me_addmaxpower_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXPOWER);
	self:Add_MaxPower(me, -nAppendValue);

	self:DebugPrint("de_me_addmaxpower_p");
end

-- 增加命中率值
function BufferMagicEffect:de_me_addhitrate_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HITCOEFFICIENT);
	self:Add_HitCoefficient(me, -nAppendValue);

	self:DebugPrint("de_me_addhitrate_v");
end

function BufferMagicEffect:de_me_addattack_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ATTACK);
	self:Add_Attack(me, -nAppendValue);

	self:DebugPrint("de_me_addattack_v");
end

function BufferMagicEffect:de_me_addresist_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_RESIST);
	self:Add_Resist(me, -nAppendValue);

	self:DebugPrint("de_me_addattack_v");
end

-- 增加会心一击值
function BufferMagicEffect:de_me_addcriticalrate_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CRITICALVALUE);
	self:Add_CriticalValue(me, -nAppendValue);

	self:DebugPrint("de_me_addcriticalrate_v nAppendValue = ", nAppendValue);
end

function BufferMagicEffect:de_me_back_add_buff()
end

-- 增加会心一击伤害百分比
function BufferMagicEffect:de_me_addcriticaldmg_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CRITICALATTACK);
	self:Add_CriticalAttack(me, -nAppendValue);

	self:DebugPrint("de_me_addcriticaldmg_p");
end

-- 增加移动速度
function BufferMagicEffect:de_me_addrunspeed_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED_PERCENT);
	self:Add_Speed(me, -nAppendValue);

	self:DebugPrint("de_me_addrunspeed_v");
end

-- 增加护甲值
function BufferMagicEffect:de_me_addarmor_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ARMOR);
	self:Add_Def(me, -nAppendValue);

	self:DebugPrint("de_me_addarmor_v");
end

-- 增加闪避值
function BufferMagicEffect:de_me_adddodge_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_DODGEALL);
	self:Add_Dodge(me, -nAppendValue);

	self:DebugPrint("de_me_adddodge_v");
end

-- 对npc隐身
function BufferMagicEffect:de_me_hidetonpc(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HIDETO_NPC);
	self:Add_IsHidetoNPC(me, -nAppendValue);

	self:DebugPrint("de_me_hidetonpc");
end

-- 对玩家隐身
function BufferMagicEffect:de_me_hidetoplayer(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HIDETO_PLAYER);
	self:Add_IsHidetoPlayer(me, -nAppendValue);

	self:DebugPrint("de_me_hidetoplayer");
end

-- 按百分比反弹受到的伤害
function BufferMagicEffect:de_me_rebound_dmg_p()
	local nPercent = me.GetReboundP();
	self:Add_ReboundP(me, -nPercent);

	self:DebugPrint("de_me_rebound_dmg_p");
end

-- 按值反弹受到的伤害
function BufferMagicEffect:de_me_rebound_dmg_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_REBOUND_V);
	self:Add_ReboundV(me, -nAppendValue);

	self:DebugPrint("de_me_rebound_dmg_v");
end

-- 按比例减少受到的伤害
function BufferMagicEffect:de_me_reducedmg()
	self:Set_WithstandNormalP(me, 0);
	self:Set_WithstandNormalV(me, 0);

	self:DebugPrint("de_me_reducedmg");
end

-- 造成伤害按比例转化为生命回复
function BufferMagicEffect:de_me_transdmgtolife(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSTOLIFE_P);
	self:Add_TransToLifeP(me, -nAppendValue);

	self:DebugPrint("de_me_transdmgtolife");
end

-- 造成伤害按比例转化为真气回复
function BufferMagicEffect:de_me_transdmgtomana(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSTOMANA_P);
	self:Add_TransToManaP(me, -nAppendValue);

	self:DebugPrint("de_me_transdmgtomana");
end

function BufferMagicEffect:de_me_ignorbuff(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_ID);
	me.pBufferEffect.IgnorBuff(-nBuffId);

	self:DebugPrint("de_me_ignorbuff");
end


function BufferMagicEffect:de_me_ignorbuffgroup(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_GROUP);
	me.pBufferEffect.IgnorBuffGroup(-nAppendValue);

	self:DebugPrint("de_me_ignorbuffgroup");
end


function BufferMagicEffect:de_me_ignorbuffcate(nBuffTemplateId)
	local nAppendValue1	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_CATETYPE);
	local nAppendValue2	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_CATEID);
	me.pBufferEffect.IgnorBuffCate(-nAppendValue1,nAppendValue2);

	self:DebugPrint("de_me_ignorbuffcate");
end

function BufferMagicEffect:de_me_ignoredebuff()
	me.SetIgnorDebuff(0);
	self:DebugPrint("de_me_ignoredebuff");
end

-- 增加治疗生命点数
function BufferMagicEffect:de_me_addtreatlife_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_LIFE);
	self:Add_CuringLifeBase(me, -nAppendValue);

	self:DebugPrint("de_me_addtreatlife_v");
end

-- 增加治疗生命百分比
function BufferMagicEffect:de_me_addtreatlife_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_LIFE_PERCENT);
	self:Add_CuringLifePercent(me, -nAppendValue);

	self:DebugPrint("de_me_addtreatlife_p");
end

-- 增加治疗真气点数
function BufferMagicEffect:de_me_addtreatmana_v(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_MANA_BASE);
	self:Add_CuringManaBase(me, -nAppendValue);

	self:DebugPrint("de_me_addtreatmana_v");
end

-- 增加治疗真气百分比
function BufferMagicEffect:de_me_addtreatmana_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_MANA_PERCENT);
	self:Add_CuringManaPercent(me, -nAppendValue);

	self:DebugPrint("de_me_addtreatmana_p");
end

-- 按值减少技能消耗
function BufferMagicEffect:de_me_reducecost_v(nBuffTemplateId)
	local nCateId	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILLCOST_CATEID);
	me.pFightSkill.ResetSkillCost(1, nCateId);

	self:DebugPrint("de_me_reducecost_v");
end

-- 按比例减少技能消耗
function BufferMagicEffect:de_me_reducecost_p(nBuffTemplateId)
	local nCateId	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILLCOST_CATEID);
	me.pFightSkill.ResetSkillCost(1, nCateId);

	self:DebugPrint("de_me_reducecost_p");
end

-- 免疫伤害
function BufferMagicEffect:de_me_ignordmg(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNORE_DMG);
	self:Add_IgnorDmg(me, -nAppendValue);

	self:DebugPrint("de_me_ignordmg");
end

-- 增加对应cate技能的攻击力百分比
function BufferMagicEffect:de_me_addmagnification()
	-- self:DebugPrint("");
end

-- 增加攻击力值
function BufferMagicEffect:de_me_addatkdmg_v(nBuffTemplateId)
	local nAppendValue11	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_PHYSICATTACK_MAX);
	local nAppendValue12	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_PHYSICATTACK_MIN);
	local nAppendValue21	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAGICATTACK_MAX);
	local nAppendValue22	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAGICATTACK_MIN);

	self:modifydmg_v(-nAppendValue11, -nAppendValue12, -nAppendValue21, -nAppendValue22);

	self:DebugPrint("de_me_addatkdmg_v");
end

-- 免疫会心攻击
function BufferMagicEffect:de_me_ignorcristal(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNORE_CRISTAL);
	self:Add_IgnorCristal(me, -nAppendValue);

	self:DebugPrint("de_me_ignorcristal");
end

-- 将参数nParam1按照nPercent附加到nParam2上
function BufferMagicEffect:de_me_trans_param(nBuffTemplateId)
	local nAppendValue1	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSPARAM_TYPE);
	local nAppendValue2	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSPARAM_VALUE);
	self:AddParamValue(me, nAppendValue1, -nAppendValue2);

	self:DebugPrint("de_me_trans_param");
end

-- 更改对应技能的引导类型，将原本的引导技能变成瞬发技能
function BufferMagicEffect:de_me_changechanneltype(nBuffTemplateId)
	local nCateType	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CHANGE_CHANNEL_CATETYPE);
	local nCateId	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CHANGE_CHANNEL_CATEID);

	me.pFightSkill.ResetSkillChannelType(nCateType, nCateId, -1);
	self:DebugPrint("de_me_changechanneltype");
end

function BufferMagicEffect:de_me_add_bufftoself()
end

-- 缩短引导时间
function BufferMagicEffect:de_me_reducechanneltime(nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_CHANNELTIME);
	me.pFightSkill.ResetSkillChannelTime(nCateType, nCateId, -nAppendValue);

	self:DebugPrint("de_me_reducechanneltime");
end

-- 闪烁到目标背后
function BufferMagicEffect:de_me_twinkle()
	self:DebugPrint("de_me_twinkle");
end


-- 跳到目标身前(类似闪烁),  by KK
function BufferMagicEffect:de_me_jumpto()
	self:DebugPrint("de_me_jumpto");
end

-- 自己后翻滚  by KK
function BufferMagicEffect:de_me_backflip()
	self:DebugPrint("de_me_backflip");
end

-- 增加受到的伤害百分比
function BufferMagicEffect:de_me_addrecievedmg_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ENLARGE_ATTACK_P);
	self:Add_EnlargeAttackP(me, -nAppendValue);

	self:DebugPrint("de_me_addrecievedmg_p");
end

function BufferMagicEffect:de_me_reducerecievedmg_p(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_REDUCE_ATTACK_P);
	self:Add_ReduceAttackP(me, -nAppendValue);

	self:DebugPrint("de_me_reducerecievedmg_p");
end

-- 同时出现hide及vision时，可见
function BufferMagicEffect:de_me_visiontonpc()
	me.SetIsVisiontoNPC(0);
	self:DebugPrint("de_me_visiontonpc");
end

-- 同时出现hide及vision时，可见
function BufferMagicEffect:de_me_visiontoplayer()
	me.SetIsVisiontoPlayer(0);
	self:DebugPrint("me_visiontoplayer");
end

-- 吸血（一次属性）
function BufferMagicEffect:de_me_vampire()
	self:DebugPrint("de_me_vampire");
end

-- 为目标承受伤害(可叠加)
function BufferMagicEffect:de_me_divertdmgtoself()
	me.SetWithstandBuffPlayer(0);
	me.SetWithstandBuffPercent(0);
	me.SetWithstandBuffLifePercent(0);

	self:DebugPrint("de_me_divertdmgtoself"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

function BufferMagicEffect:de_me_add_self_groundbuffer()

end

function BufferMagicEffect:de_me_add_bufftoreceiver()
end

function BufferMagicEffect:de_me_add_bufftosender()
end

function BufferMagicEffect:de_me_add_bufftotriggerreceiver()
end

function BufferMagicEffect:de_me_add_bufftotriggersender()
end

function BufferMagicEffect:de_me_explode()
end

function BufferMagicEffect:de_me_yongzhewudi(nBuffTemplateId) --勇者无敌，根据当前生命损失量计算闪避值
	local nValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_YONGZHEWUDI);
	self:Add_Dodge(me, -nValue);
	--self:DebugPrint("de_me_yongzhewudi()");  --不做显示，刷的太多
end

function BufferMagicEffect:de_me_add_battleintegration_v()
end





-- 主动技能附带buff ------------
function BufferMagicEffect:de_me_inactive_addselflife_v(nBuffTemplateId, nSkillLevel)

end

function BufferMagicEffect:de_me_inactive_subtargetlife_v(nBuffTemplateId, nSkillLevel)

end

function BufferMagicEffect:de_me_inactive_subtargetmana_v(nBuffTemplateId, nSkillLevel)

end

function BufferMagicEffect:de_me_inactive_addhit_v(nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HITCOEFFICIENT);

	self:Add_CriticalValue(me, -nAppendValue);

	self:DebugPrint("de_me_inactive_addhit_v");
end
-- 主动技能附带buff ------------


--------------------------------------------------------------------------------
