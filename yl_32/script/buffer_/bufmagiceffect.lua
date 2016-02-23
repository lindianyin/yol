-- 文件名:bufmagiceffect.lua
-- 创建者:zhangjunjie
-- 创建时间:2012-05-09 17:15:30
-- buff 条件判断和定义
--	当前脚本中已经压入了me和him，me为buff的接受者，him为buff的释放者
--	me是必定存在的，him如果不存在则说明没有释放者

---------------------------------------- 魔法属性函数 -------------------------------------

BufferMagicEffect.nCriticalScale = 5;  -- 暴击放大系数
BufferMagicEffect.nDefScale = 6;     -- 防御缩小系数

-- 是否命中
function BufferMagicEffect:IsHit(caster, becast)

	local nDodge = becast.GetDodge();
	local nHit = caster.GetHitCoefficient();
	if nDodge == 0 then
	    nHit = 100;
	else
	    nHit = math.floor(nHit * 100 / nDodge);
	end

	if nHit < 70 then
		nHit = 70;
	end

	local nRand = KUnify.MathRandom(100);
	
	--print("BufferMagicEffect:IsHit", nHit, nRand)
	
	if nRand < nHit then
		return 1;
	end

	return 0;
end

-- 是否破防
function BufferMagicEffect:IsIgnoreDef(caster, becast)
	local nIgnoreDef = caster.GetIgnoreDef();

	local nRand = KUnify.MathRandom(1, 100);
	print("BufferMagicEffect:IsIgnoreDef", nRand, nIgnoreDef)
	if nRand < nIgnoreDef then
	  print("破防", caster.szName, becast.szName, nIgnoreDef)
		return 1;
	end

	return 0;
end

-- 是否暴击
function BufferMagicEffect:IsCritical(caster, becast)
	--免疫暴击
	if self:Get_IgnorCristal(becast) then
		return false;
	end

	local nDeCritical = becast.GetResist();
	local nCritical = caster.GetCriticalValue() / self.nCriticalScale;
	local nCriticalRate = 0
	if nDeCritical == 0 then
		nCriticalRate = 100
	else
	  nCriticalRate = math.floor(nCritical * 100 / nDeCritical);
  end

  self:DebugPrint("BufferMagicEffect:IsCritical", nCriticalRate, nCritical)
	if nCriticalRate > 30 then
		nCriticalRate = 30;
	end

	if nCriticalRate < 0 then
		nCriticalRate = 0;
	end

	local nRand = KUnify.MathRandom(100);
	if nRand < nCriticalRate then
		return 1;
	end

	return 0;
end

-- 暴击
function BufferMagicEffect:Cristal(caster, becast, nHarmValue)
	--暴击值
	local rate = self:Get_CriticalAttack(caster) - self:Get_CriticalDeAttack(becast);

	if (rate < 0) then
		rate = 0;
	end

	-- 默认200%暴击伤害
	nHarmValue = math.floor(nHarmValue * (200 + rate) / 100);

	return nHarmValue;
end

-- 普通抵挡计算，根据nHarmValue伤害量计算抵挡值(不消耗抵挡值)
function BufferMagicEffect:NormalWithstand(becast, nHarmValue)
	if (self:Get_WithstandNormalP(becast) == 0) then
		return 0;
	end

	if (self:Get_WithstandNormalV(becast) == 0) then
		return 0;
	end

	local nWithstandV1 = nHarmValue * self:Get_WithstandNormalP(becast);
	local nWithstandV2 = self:Get_WithstandNormalV(becast);

	local nWithstandV = 0;
	if nWithstandV2 == -1 or nWithstandV1 < nWithstandV2 then
		nWithstandV = nWithstandV1;
	else
		nWithstandV = nWithstandV2;
	end

	return nWithstandV;
end

-- 抵挡计算，根据nHarmValue伤害消耗抵挡值,返回剩余伤害量
function BufferMagicEffect:Withstand(caster, becast, nHarmValue)
	--普通抵挡伤害
	local nNormalWithstandValue = self:NormalWithstand(becast, nHarmValue);

	--普通抵挡
	if (0 < nNormalWithstandValue) then
		if (nNormalWithstandValue < nHarmValue) then
			nHarmValue = nHarmValue - nNormalWithstandValue;
			self:Add_WithstandNormalV(becast, -nNormalWithstandValue);
		else
			self:Add_WithstandNormalV(becast, -nHarmValue);
			return 0;
		end
	elseif (nNormalWithstandValue == -1) then
		-- 抵挡值-1为100%抵挡
		return 0;
	end

	return nHarmValue;
end

-- 反弹伤害，根据nHarmValue伤害消耗反弹给对方
function BufferMagicEffect:Rebound(caster, becast, nHarmValue)
	local nReboundValueByP = nHarmValue * self:Get_ReboundP(becast);
	local nReboundValueByV = self:Get_ReboundV(becast);
	local nReboundValue = nReboundValueByP + nReboundValueByV;

	if nReboundValue == 0 then
		return
	end
	self:Add_CurrentLife(becast, caster, -nReboundValue);
end

-- 伤害吸血吸蓝，根据nHarmValue伤害给对方+血+蓝
function BufferMagicEffect:TransDMG(caster, nHarmValue)
	local nTransToLifeV = nHarmValue * self:Get_TransToLifeP(caster);
	local nTransToManaV = nHarmValue * self:Get_TransToManaP(caster);

	self:Add_CurrentLife(caster, caster, nTransToLifeV);
	self:Add_CurrentMana(caster, nTransToManaV);
end

-- 放大伤害
function BufferMagicEffect:EnlargeDMG(caster, nHarmValue)
	return nHarmValue * (100 + caster.GetEnlargeAttackP()) / 100;
end

-- 减小伤害
function BufferMagicEffect:ReduceDMG(caster, nHarmValue)
	nHarmValue = nHarmValue * (100 - caster.GetReduceAttackP()) / 100;
	assert(nHarmValue > 0);
	return nHarmValue;
end

function BufferMagicEffect:damagelife_v(caster, becast, nAttackValue, nMagnification, nBuffTemplateId)
	--免疫伤害
	--[[
	if self:Get_IgnorDmg(becast) then
		self:DebugPrint("IgnoreDmg");
		self:ClientShowTextOnHead(caster, becast, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextImmune); -- 免疫2

		self:DebugPrint("显示“免疫”!");
		return 0;
	end
	--]]

	--命中
	if self:IsHit(caster, becast) == 0 then
		BufferCondition:Miss(caster, becast);
		BufferCondition:Dodge(becast);
		self:DebugPrint("Dodget");
		self:ClientShowTextOnHead(caster, becast, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextDodge); -- 闪避 1
		return 0;
	else
		BufferCondition:Hit(caster, becast);
	end

	--攻擊力
 	local nAttack = caster.GetAttack();
	local nDef = becast.GetDef() * self.nDefScale * 0.2;
	local nMaxLife = becast.GetMaxLife();

	local nAttackDeRate = 0;

	--[[
	
	local nDefSum = (nDef + nMaxLife);
	if (nDefSum == 0) then
	    print("被攻击角色的防御和生命值都等于0！", caster.szName, becast.szName)
	    nDefSum = 1
	end
	nAttackDeRate = nMaxLife / nDefSum;
	
	]]
	
	--换了个常见的除法公式试试看效果...
	local nDefSum = (nDef + nAttack);
	if (nDefSum == 0) then
	    print("被攻击角色的防御和生命值都等于0！", caster.szName, becast.szName)
	    nAttackDeRate = 1;
	else
		nAttackDeRate = nAttack / nDefSum;
		if nAttackDeRate < 0.1 then
			nAttackDeRate = 0.1;
		end
	end
	
	
	local nModifyDmgP = self:Get_ModifyDmgP(caster);
	nAttack = math.floor(nAttack * nModifyDmgP / 100);
	
	local nHarmValue = nAttack * nAttackDeRate * nMagnification / 1000 + nAttackValue;

	--是否暴击
	local bCriticalHit = 0;
	if self:IsCritical(caster, becast) == 1 then
		--nHarmValue = self:Cristal(caster, becast, nHarmValue);
		nHarmValue = nHarmValue * 2; -- 固定为2倍伤害

		--事件触发
		BufferCondition:Critical(caster, becast, nBuffTemplateId);
		self:DebugPrint("Critical");

		bCriticalHit = 1;   -- 标记成暴击，用于头顶文字输出 kk
	end

	--伤害减少
	nHarmValue = self:ReduceDMG(becast, nHarmValue);

	--[[
	--抵挡伤害
	nHarmValue = self:Withstand(caster, becast, nHarmValue);
	--]]
	
	--伤害反弹、转化
	self:Rebound(caster, becast, nHarmValue);
	self:TransDMG(caster, nHarmValue);

	--[[
	--伤害放大
	nHarmValue = self:EnlargeDMG(becast, nHarmValue);

	--未造成伤害
	if (nHarmValue == 0) then
		self:DebugPrint("All Attack Withstand");
		return 0;
	end

	--]]

	return nHarmValue, bCriticalHit;  -- 返回是否暴击
end

function BufferMagicEffect:pet_damagelife_v(becast, nPetAttack, nMagnification)
	local nDef = becast.GetDef() * self.nDefScale;
	local nMaxLife = becast.GetMaxLife();
	
	local nAttackDeRate = 0;
	
	local nDefSum = (nDef + nMaxLife);
	if (nDefSum == 0) then
	    print("被神仙攻击角色的防御和生命值都等于0！", becast.szName)
	    nDefSum = 1
	end
	nAttackDeRate = nMaxLife / nDefSum;

	local nHarmValue = nPetAttack * nAttackDeRate * nMagnification / 1000;

	return nHarmValue;
end

BufferMagicEffect.TestHurtValue = 0;  -- GM设置，特大型伤害

function BufferMagicEffect:SetTestHurtValue(nValue)
	BufferMagicEffect.TestHurtValue = nValue;
end

-- 神仙buff技能
function BufferMagicEffect:me_pet_addselflife(nValue, nBuffTemplateId, nSkillLevel)
   local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_pet_addselflife")
	 self:Add_CurrentLife(me, him, nLevelValue, 0);

	 KCharacter.OnRecoverLife(him.dwId, me.dwId,nLevelValue);

	 self:DebugPrint("me_pet_addselflife:",nLevelValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

function BufferMagicEffect:me_pet_addselfattack(nValue, nBuffTemplateId, nSkillLevel)

   print("......................",nValue, nBuffTemplateId, nSkillLevel);

	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_pet_addselfattack")
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ATTACK, nLevelValue);

	self:Add_Attack(him, nLevelValue);

	self:DebugPrint("me_pet_addselfattack"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
	print("me_pet_addselfattack"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster", nLevelValue);
end

function BufferMagicEffect:me_pet_subtargetlife(nValue, nBuffTemplateId, nSkillLevel)
	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_pet_subtargetlife")
	self:Add_CurrentLife(him, me, -nLevelValue, 0);

	print('...........................减血' .. nBuffTemplateId, nSkillLevel,nLevelValue)

	self:DebugPrint("me_pet_subtargetlife:",nLevelValue,"Target Name:",him.szName,"Caster Name:",me and me.szName or "No Caster")
end

function BufferMagicEffect:me_pet_subtargetdefence(nValue, nBuffTemplateId, nSkillLevel)
	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_pet_subtargetdefence")
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ARMOR, nLevelValue);

	self:Add_Def(him, -nLevelValue);

	print("me_pet_subtargetdefence"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

function BufferMagicEffect:me_pet_damagelife_v(nPercent, nBuffTemplateId, nSkillLevel)
	local caster = him;	-- 施法者
	local becast = me;	-- 被施法者

	--技能的計算信息(攻擊力係數、成長值、是否物理攻擊、需求等級)
	local nSkillId = becast.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	-- 根据等级，读取伤害值 获取技能等级,到表里找实际伤害
	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_pet_damagelife_v")
	-- local nCoefficientMin, nCoefficientMax = caster.pFightSkill.GetSkillCoefficient(nSkillId);
	local nSkillParam = KUnify.MathRandom(nCoefficientMin, nCoefficientMax);

	local pPet = KPet.GetFightPet(caster);
	assert(pPet);

	-- 得到宠物的攻击力
	local tbBaseAttr = pPet.GetBaseAttr();
	local nAttack = tbBaseAttr.nAttack;
	
	local nHarmValue = self:pet_damagelife_v(becast, nAttack, nSkillParam);
	self:Add_CurrentLife(caster, becast, -nHarmValue, 0, 1);

	self:DebugPrint("pet reduce life:", nHarmValue,"Target Name:",becast.szName,"Caster Name:",caster and caster.szName or "No Caster");
end

-- 伤害生命值
function BufferMagicEffect:me_damagelife_v(nValue, nBuffTemplateId, nSkillLevel)	
	-- 根据等级，读取伤害值 获取技能等级,到表里找实际伤害
	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_damagelife_v")
	if (nLevelValue ~= nil and nLevelValue > 0) then
		nValue = nLevelValue  -- 存在等级数值，则拿去, 否则兼容，用原来的
	end
	
	local caster = him;	-- 施法者
	local becast = me;	-- 被施法者
	
	-- 这里caster可能已经被回收了
	if caster.IsNpc() == 1 and caster.GetDeleteState() == 1 then
		return;
	end

	--事件触发
	BufferCondition:SkillDmgIn(becast, caster);
	BufferCondition:SkillDmgOut(caster, becast);

	CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_BEHIT, becast.dwId, 0, 0);

	--技能的計算信息(攻擊力係數、成長值、是否物理攻擊、需求等級)
	local nSkillId = becast.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	if nCoefficientMin == nil or nCoefficientMax == nil then
	    nCoefficientMin, nCoefficientMax = caster.pFightSkill.GetSkillCoefficient(nSkillId);
	end
	local nSkillParam = KUnify.MathRandom(nCoefficientMax-8, nCoefficientMax); -- 固定有0.8%的随机范围


	if caster.IsNpc() ~= 1 then
		nValue = nValue + self.TestHurtValue;
	end

	--计算伤害并减血
	local nHarmValue, bCriticalHit = self:damagelife_v(caster, becast, nValue, nSkillParam, nBuffTemplateId);
	nHarmValue = math.floor(nHarmValue);

	self:Add_CurrentLife(caster, becast, -nHarmValue, bCriticalHit);

	self:DebugPrint("reduce life:",nHarmValue,"Target Name:",becast.szName,"Caster Name:",caster and caster.szName or "No Caster");
end

-- 伤害生命百分比
function BufferMagicEffect:me_damagelife_p(nPercent, nBuffTemplateId, nSkillLevel)
	local caster = him;	-- 施法者
	local becast = me;	-- 被施法者

	--技能攻擊量
	nValue = math.floor(nPercent * becast.GetMaxLife() / 100);

	if nValue < 1 then
		nValue = 1;
	end

	self:Add_CurrentLife(caster, becast, -nValue);
end

-- 伤害真气值
function BufferMagicEffect:me_damagemana_v(nValue, nBuffTemplateId, nSkillLevel)
	self:Add_CurrentMana(me, -nValue);

	self:DebugPrint("reduce mana:", nValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 伤害真气百分比
function BufferMagicEffect:me_damagemana_p(nPercent, nBuffTemplateId, nSkillLevel)
	--傷害值
	local nHarmValue = self:Get_MaxMana(me) * nPercent / 100;

	self:Add_CurrentMana(me, -nHarmValue);

	self:DebugPrint("reduce mana:",nHarmValue,"%","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end


-- 击退
function BufferMagicEffect:me_knock(nLength)
	local mPosHim = him.GetPosition();
	local mPosMe = me.GetPosition();
	local nDirection = me.GetDirection(mPosHim.nX, mPosHim.nY);

	me.pBufferEffect.Knock(nLength, mPosMe.nX, mPosMe.nY, mPosMe.nZ, nDirection);

	--me.TeleportTo(me.dwSceneId, nEndX, nEndY, mPosMe.nZ);

	me.pFightSkill.CancelChannelSkill();

	self:DebugPrint("me_knock",nLength,"Target Name:", me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 击杀目标
function BufferMagicEffect:me_dodeath()
	me.KilledBy(him);
	self:DebugPrint("kill:",me.szName,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

function BufferMagicEffect:me_dorevive_immediately(nLifePercent, nManaPercent)
	if me.IsDead() ~= 1 then
		return
	end

	KCharacter.Revive(me, nLifePercent*10, nManaPercent*10);

	self:DebugPrint("revive:",me.szName,"Life:",nLifePercent,"%","Mana:",nManaPercent,"%","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

function BufferMagicEffect:OnAcceptRevive(caster, becast, nLifePercent, nManaPercent)
	-- TODO:复活的判断
	if becast.IsDead() ~= 1 then
		return
	end

	-- 玩家被其他玩家复活了，关闭自我复活面板
	becast.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:CloseDeathPanel"});

	self:me_dorevive(nLifePercent, nManaPercent, caster, becast);
end

-- 复活目标
function BufferMagicEffect:me_dorevive(nLifePercent, nManaPercent, caster, becast)
	if me.IsDead() ~= 1 then
		return;
	end

	if not caster then
		local text = string.format("%s要复活你，接受复活吗", him.szName);
		Dialog:_AskForSure(me.dwId, text, ({"BufferMagicEffect:OnAcceptRevive", him, me, nLifePercent, nManaPercent}));
		return;
	end

	KCharacter.Revive(becast, nLifePercent*10, nManaPercent*10);

	self:DebugPrint("revive:",becast.szName,"Life:",nLifePercent,"%","Mana:",nManaPercent,"%","Target Name:",me.szName,"Caster Name:",caster and caster.szName or "No Caster")
end

-- 清除对应id的buff
function BufferMagicEffect:me_clearbuff(nBuffId)
	self:DebugPrint("remove buff:",nBuffId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
	me.pBufferEffect.ClearBuff(nBuffId);
end

-- 清除debuff（debuff列填1的buff）
function BufferMagicEffect:me_cleardebuff(nType)
	me.pBufferEffect.ClearBuffByType(nType);
end

-- 清除对应id的skill的cd时间
function BufferMagicEffect:me_clearcd(nCateType,nCateId)
	--me.pFightSkill.ClearSkillCD(nCateType,nCateId);
	self:DebugPrint("remove skillcate interval:CateType:",nCateType,"CateId:",nCateId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 将目标拉到自己身前
function BufferMagicEffect:me_pulltarget()
	local nLength = 2; --拉到身前1米处
	local mPosHim = him.GetPosition();
	local nDirection = him.nFaceDirection;

	local nIgnoreControlRate = me.GetIgnoreDef();
  local nRandom = KUnify.MathRandom(100);
  if nRandom <= nIgnoreControlRate then
      self:ClientShowTextOnHead(him, me, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextIgnoreControl);
      return
  end

	me.pBufferEffect.PullTarget(nLength, mPosHim.nX, mPosHim.nY, mPosHim.nZ, nDirection);

	--me.TeleportTo(me.dwSceneId, nEndX, nEndY, mPosHim.nZ);

	me.pFightSkill.CancelChannelSkill();

	self:DebugPrint("pull:",me.szName,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 传送到目标地图的对应点，该点可动态生成，即可用于拉传送的技能
function BufferMagicEffect:me_setmappos(nMapId,nxPos,nyPos)
	me.TeleportTo(nMapId, nxPos, nyPos, 0);
	self:DebugPrint("go to map:",nMapId,"At Pos:",nxPos,",",nyPos,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 回城，回到之前保存过的安全区的对应位置
function BufferMagicEffect:me_tp()
	--no use
end

-- 增加对应id的buff的时间
function BufferMagicEffect:me_addbufftime(nCateType,nCateId,nFrame, nBuffTemplateId)
	me.pBufferEffect.ResetBufferTime(nCateType, nCateId, nFrame);

	self:DebugPrint("target buff cate:",nCateType,",",nCateId,"add time:",nFrame,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 重置id的buff的时间
function BufferMagicEffect:me_refreshbufftime(nCateType,nCateId)
	me.pBufferEffect.RefreshBufferTime(nCateType,nCateId);
	self:DebugPrint("refresh buff cate live time:",nCateType,",",nCateId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 删除掉该效果所在的buff自身
function BufferMagicEffect:me_kill_self()
	local becast = me;
	becast.pBufferEffect.ClearCurrentBuff();
	self:DebugPrint("clear the buff :", BufferCondition.nTriggerParam1, "Target Name:", becast.szName);
end

-- 给该buff的接受者添加对应id的buff
function BufferMagicEffect:me_add_bufftoreceiver(nBuffId, nBuffTemplateId, nSkillLevel)
  local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_add_bufftoreceiver")
  local nBuffPersist = 0
  if (nLevelValue ~= nil and nLevelValue > 0) then
		nBuffPersist = nLevelValue  -- 存在等级数值，则拿去, 否则兼容，用原来的
	end
	
	local nSkillID = me.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	self:DebugPrint("me_add_bufftoreceive:",nBuffId, "skillid:", nSkillID, "Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
	BufferApi.AddBufferToCharacter(him.dwId, nSkillID, me.dwId, nBuffId, 1, nLevelValue, nBuffPersist);
end

-- 给该buff的发送者添加对应id的buff
function BufferMagicEffect:me_add_bufftosender(nBuffId, nBuffTemplateId)
	local nSkillID = me.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	self:DebugPrint("me_add_bufftosender:",nBuffId,"Target Name:",him.szName,"Caster Name:",me and me.szName or "No Caster")
	BufferApi.AddBufferToCharacter(me.dwId, nSkillID, him.dwId, nBuffId, 1);
end

-- 给事件接受者添加对应的id的buff（只填在trigger里）
function BufferMagicEffect:me_add_bufftotriggerreceiver(nBuffId,nSkillID)
	self:DebugPrint("me_add_bufftotriggerreceive:",nBuffId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
	BufferApi.AddBufferToCharacter(him.dwId, nSkillID, me.dwId, nBuffId, 1);
end

-- 给事件发送者添加对应的id的buff（只填在trigger里）
function BufferMagicEffect:me_add_bufftotriggersender(nBuffId,nSkillID, nBuffTemplateId)
	--local nSkillID = him.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	self:DebugPrint("me_add_bufftotriggersender:",nBuffId,"SKillID:", nSkillID, "Target Name:",him.szName,"Caster Name:",me and me.szName or "No Caster")
	BufferApi.AddBufferToCharacter(me.dwId, nSkillID, him.dwId, nBuffId, 1);
end

function BufferMagicEffect:me_add_bufftoself(nBuffId, nBuffTemplateId)
	local nSkillID = me.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	self:DebugPrint("me_add_bufftoself:",nBuffId, "skillid:", nSkillID, "Target Name:",me.szName,"Caster Name:",me and me.szName or "No Caster")
	BufferApi.AddBufferToCharacter(me.dwId, nSkillID, me.dwId, nBuffId, 1);
end

function BufferMagicEffect:me_cast_skill(nSkillId)
	-- no use
end

-- him对me释放一个buff导致以me为中心释放groundbuffer
function BufferMagicEffect:me_add_groundbuffer(nSkillId,nBufferId)
	local mPos = me.GetPosition();
	BufferApi.AddGroundBuffer(nBufferId, him.dwId, nSkillId, me.dwSceneId,
		mPos.nX, mPos.nY, mPos.nZ, 1);
	self:DebugPrint("me_add_groundbuffer skillid:", nSkillId, "bufferid:", nBufferId, "caster:", me.szName);
end

-- me直接释放一个以me为中心的groundbuffer
function BufferMagicEffect:me_add_self_groundbuffer(nSkillId,nBufferId)
	local mPos = me.GetPosition();
	BufferApi.AddGroundBuffer(nBufferId, me.dwId, nSkillId, me.dwSceneId,
		mPos.nX, mPos.nY, mPos.nZ, 1);
	self:DebugPrint("me_add_self_groundbuffer skillid:", nSkillId, "bufferid:", nBufferId, "caster:", me.szName);
end

-- 给对应cate的技能添加对应id的buff
function BufferMagicEffect:me_add_bufftoskill(nSkillCateType, nSkillCateId, nBuffId, nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_BUFF);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = nBuffId;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_BUFF, nAppendValue);
	end

	him.pFightSkill.AddBuffToSkill(nSkillCateType, nSkillCateId, nAppendValue);

	self:DebugPrint("me_add_bufftoskill","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 从对应cate的技能删除对应id的buff
function BufferMagicEffect:me_del_bufffromskill(nSkillCateType, nSkillCateId, nBuffId, nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_BUFF);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = -nBuffId;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_BUFF, nAppendValue);
	end

	him.pFightSkill.AddBuffToSkill(nSkillCateType, nSkillCateId, nAppendValue);

	self:DebugPrint("me_del_bufffromskill","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加当前生命值
function BufferMagicEffect:me_addcurlife_v(nValue, nBuffTemplateId)
	self:Add_CurrentLife(him, me, nValue);

	KCharacter.OnRecoverLife(him.dwId, me.dwId, nValue);

	self:DebugPrint("add curlife:",nValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 主动技能附带buff ------------
function BufferMagicEffect:me_inactive_addselflife_v(nBuffTemplateId, nSkillLevel)
	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_inactive_addselflife_v")
	self:Add_CurrentLife(me, him, nLevelValue,0);

	KCharacter.OnRecoverLife(him.dwId, me.dwId,nLevelValue);

	print('...........................加血' .. nBuffTemplateId, nSkillLevel,nLevelValue)

	self:DebugPrint("inactive add self life:",nLevelValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

function BufferMagicEffect:me_inactive_subtargetlife_v(nBuffTemplateId, nSkillLevel)
	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_inactive_subtargetlife_v")
	self:Add_CurrentLife(him, me, -nLevelValue,0);

	print('...........................减血' .. nBuffTemplateId, nSkillLevel,nLevelValue)

	self:DebugPrint("inactive sub target life:",nLevelValue,"Target Name:",him.szName,"Caster Name:",me and me.szName or "No Caster")
end

function BufferMagicEffect:me_inactive_subtargetmana_v(nBuffTemplateId, nSkillLevel)
	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_inactive_subtargetmana_v")
	self:Add_CurrentMana(me, -nLevelValue);

	print('...........................扣蓝' .. nBuffTemplateId, nSkillLevel,nLevelValue)

	self:DebugPrint("inactive sub target mana:",nLevelValue,"Target Name:",him.szName,"Caster Name:",me and me.szName or "No Caster")
end

function BufferMagicEffect:me_inactive_addhit_v(nBuffTemplateId, nSkillLevel)

	local nLevelValue, nCoefficientMin, nCoefficientMax = BufferApi.GetBufferNumber(nBuffTemplateId, nSkillLevel, "me_inactive_addhit_v")
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HITCOEFFICIENT, nLevelValue);

	self:Add_CriticalValue(me, nLevelValue);

	print('...........................加暴击值' .. nBuffTemplateId, nSkillLevel,nLevelValue)

	self:DebugPrint("me_inactive_addhit_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end


-- 主动技能附带buff ------------


-- 增加当前生命百分比
function BufferMagicEffect:me_addcurlife_p(nPercent, nBuffTemplateId)
	--治疗量
	local nCureValue = math.floor(self:Get_MaxLife(me) * nPercent / 100);
	if nCureValue < 1 then
		nCureValue = 1;
	end

	self:Add_CurrentLife(him, me, nCureValue);

	KCharacter.OnRecoverLife(him.dwId, me.dwId, nCureValue);

	self:DebugPrint("add curlife:",nPercent,"%","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 增加当前真气值
function BufferMagicEffect:me_addcurmana_v(nValue, nBuffTemplateId)

	self:Add_CurrentMana(me, nValue);

	self:DebugPrint("add curmana:",nValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 增加当前真气百分比
function BufferMagicEffect:me_addcurmana_p(nPercent, nBuffTemplateId)
	--治疗量
	local nCureValue = self:Get_MaxMana(me) * nPercent / 100;

	self:Add_CurrentMana(me, nCureValue);

	self:DebugPrint("add curmana:",nPercent,"%","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 增加当前集中值
function BufferMagicEffect:me_addcurpower_v(nValue, nBuffTemplateId)
	self:Add_CurrentPower(me, nValue);

	self:DebugPrint("add curpower:",nValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 眩晕
function BufferMagicEffect:me_stun(nBuffTemplateId)
   if me.GetIsStun() > 0 then
   	    return
   end

	--local nAppendValue = 1;
	--me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_STUN, nAppendValue);

	--me.StopMove(1);
	--self:Add_IsStun(me, nAppendValue);
	me.SetIsStun(2);

	me.SetMoveState(Character.MOVE_STATE_ON_HALT);

	me.pFightSkill.CancelChannelSkill();

	-- 客户端通过人物属性判断，发送goto协议给服务器
	self:DebugPrint("me_stun"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

function BufferMagicEffect:me_invisibleToNpc()
    me.SetIsVisiontoNPC(1)
    print("me_invisibleToNpc", me.szName)
end

-- 定身
function BufferMagicEffect:me_fix()
	if me.GetIsFix() > 0  then
		return
	end

  local nIgnoreControlRate = me.GetIgnoreDef();
  local nRandom = KUnify.MathRandom(100);
  if nRandom <= nIgnoreControlRate then
      self:ClientShowTextOnHead(him, me, self.CHARACTER_HEAD_TEXT_TYPE.emHeadTextIgnoreControl);
      return
  end

	me.SetIsFix(2);
	if me.IsNpc() == 1 then -- npc马上停止移动
	    me.StopMove(1)
	end
	--me.pBufferEffect.Fix();

	me.SetMoveState(Character.MOVE_STATE_ON_FREEZE);
	me.pFightSkill.CancelChannelSkill();

  print("me_fix", me.szName);

	-- 客户端通过人物属性判断，发送goto协议给服务器
	self:DebugPrint("me_fix"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

function BufferMagicEffect:me_hide()
	me.SetSkillHidden(1);
	self:DebugPrint("me_hide");
end

-- 恐惧
function BufferMagicEffect:me_fear(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_FEAR);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = 1;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_FEAR, nAppendValue);
	end

	self:Add_IsFear(nAppendValue);

	me.SetMoveState(Character.MOVE_STATE_ON_HALT);
	me.pFightSkill.CancelChannelSkill();

	self:DebugPrint("me_fear"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 沉默
function BufferMagicEffect:me_silence(nBuffTemplateId)
	local nAppendValue = 1;
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SILENCE, nAppendValue);

	self:Add_IsSilence(me, nAppendValue);

	me.SetMoveState(Character.MOVE_STATE_ON_HALT);
	me.pFightSkill.CancelChannelSkill();

	self:DebugPrint("me_silence"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 疯狂
function BufferMagicEffect:me_crazy(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CRAZY);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = 1;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CRAZY, nAppendValue);
	end

	self:Add_IsCrazy(me, nAppendValue);
	me.pFightSkill.CancelChannelSkill();

	self:DebugPrint("me_crazy"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 减速
function BufferMagicEffect:me_slow_p(nPercent, nBuffTemplateId)
	-- 原来减的速度百分比
	local nOldPercent = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED_PERCENT);

	-- 不叠加，取最高的
	if (nPercent > 0 and nPercent <= nOldPercent) or (nPercent < 0 and nPercent >= nOldPercent) then
		return;
	end

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED_PERCENT, -nOldPercent);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED_PERCENT, nPercent);

	-- 原来加的速度，如果percent > 0则oldspeed < 0
	local nOldSpeed = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED, -nOldSpeed);
	self:Add_Speed(me, -nOldSpeed);

	local nSpeed = math.floor(Scene.BASE_RUN_SPEED * nPercent / 100);
	self:Add_Speed(me, -nSpeed);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED, -nSpeed);

	self:DebugPrint("me_slow_p %",nPercent,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 忽略目标护甲
function BufferMagicEffect:me_ignorearmor_v(nValue, nBuffTemplateId)
	local nAppendValue = -1 * nValue;

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ARMOR, nAppendValue);

	self:Add_Def(me, nAppendValue);

	self:DebugPrint("me_ignorarmor_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 闪避、扣血等文字显示, 双方显示
function BufferMagicEffect:ClientShowTextOnHead(caster, becast, nType, nNumber)
	if becast.IsPlayer() == 1 then
		 becast.ClientShowHeadText(becast.dwId, nType, nNumber);
	end
	if caster.IsPlayer() == 1 then
		if becast.IsNpc() == 1 then
			 caster.ClientShowHeadText(becast.dwId, nType, nNumber);
		else
			 caster.ClientShowHeadText(becast.dwId, nType, nNumber);
		end
	end
end

-- 更改造成的伤害百分比
function BufferMagicEffect:me_modifydmg_p(nPercent, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MODIFYDMG_P, nPercent);

	self:Add_ModifyDmgP(me, nPercent);

	self:DebugPrint("me_modifydmg_p", nPercent, "Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 更改药效
function BufferMagicEffect:me_add_medicineeffect_p(nPercent, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MEDICINE_EFFECT, nPercent);

	self:Add_MedicineEffect(me, nPercent);

	self:DebugPrint("me_add_medicineeffect_p", nPercent, "Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 更改造成的伤害值
function BufferMagicEffect:me_modifydmg_v(nValue)

	self:DebugPrint("me_modifydmg_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加最大生命值
function BufferMagicEffect:me_addmaxlife_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXLIFE, nValue);

	self:Add_MaxLife(me, nValue);

	self:DebugPrint("me_addmaxlife_v",nValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加最大生命百分比
function BufferMagicEffect:me_addmaxlife_p(nPercent, nBuffTemplateId)
	local nAppendValue = math.floor(self:Get_MaxLife(me) * nPercent / 100);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXLIFE, nAppendValue);

	self:Add_MaxLife(me, nAppendValue);

	self:DebugPrint("me_addmaxlife_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加最大真气值
function BufferMagicEffect:me_addmaxmana_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXMANA, nValue);

	self:Add_MaxMana(me, nValue);

	self:DebugPrint("me_addmaxmana_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加最大真气百分比
function BufferMagicEffect:me_addmaxmana_p(nPercent, nBuffTemplateId)
	local nAppendValue = math.floor(self:Get_MaxMana(me) * nPercent / 100);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXMANA, nAppendValue);

	self:Add_MaxMana(me, nAppendValue);

	self:DebugPrint("me_addmaxmana_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加最大集中值
function BufferMagicEffect:me_addmaxpower_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXPOWER, nValue);

	self:Add_MaxPower(me, nValue);

	self:DebugPrint("me_addmaxpower_v", nValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加最大集中值百分比
function BufferMagicEffect:me_addmaxpower_p(nPercent, nBuffTemplateId)

	local nAppendValue = math.floor(self:Get_MaxPower(me) * nPercent / 100);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAXPOWER, nAppendValue);

	self:Add_MaxPower(me, nAppendValue);

	self:DebugPrint("me_addmaxpower_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加命中率(通过命中系数调节)
function BufferMagicEffect:me_addhitrate_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HITCOEFFICIENT, nValue);

	self:Add_HitCoefficient(me, nValue);

	self:DebugPrint("me_addhitrate_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end


function BufferMagicEffect:me_addattack_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ATTACK, nValue);

	self:Add_Attack(me, nValue);

	self:DebugPrint("me_addattack_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

function BufferMagicEffect:me_addresist_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_RESIST, nValue);

	self:Add_Resist(me, nValue);

	self:DebugPrint("me_addresist_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加会心一击值
function BufferMagicEffect:me_addcriticalrate_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CRITICALVALUE, nValue);

	self:Add_CriticalValue(me, nValue);

	self:DebugPrint("me_addcriticalrate_v nAppendValue = ", nValue,", Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加百分比会心一击伤害
function BufferMagicEffect:me_addcriticaldmg_p(nPercent, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CRITICALATTACK, nPercent);

	self:Add_CriticalAttack(me, nPercent);

	self:DebugPrint("me_addcriticaldmg_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加移动速度
function BufferMagicEffect:me_addrunspeed_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SPEED_PERCENT, nValue);
	self:Add_Speed(me, nValue);

	self:DebugPrint("me_addrunspeed_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加防御值
function BufferMagicEffect:me_addarmor_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ARMOR, nValue);

	self:Add_Def(me, nValue);

	self:DebugPrint("me_addarmor_v", nValue ,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加闪避值
function BufferMagicEffect:me_adddodge_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_DODGEALL, nValue);

	self:Add_Dodge(me, nValue);

	self:DebugPrint("me_adddodge_v", nValue, "Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 对npc隐身
function BufferMagicEffect:me_hidetonpc(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HIDETO_NPC);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = 1;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HIDETO_NPC, nAppendValue);
	end

	self:Add_IsHidetoNPC(me, nAppendValue);

	self:DebugPrint("me_hidetonpc"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 对玩家隐身
function BufferMagicEffect:me_hidetoplayer(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HIDETO_PLAYER);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = 1;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_HIDETO_PLAYER, nAppendValue);
	end

	self:Add_IsHidetoPlayer(me, nAppendValue);

	self:DebugPrint("me_hidetoplayer"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 按百分比反弹受到的伤害
function BufferMagicEffect:me_rebound_dmg_p(nPercent)
	self:Add_ReboundP(me, nPercent);

	self:DebugPrint("me_rebound_dmg_p", nAppendValue,"Target Name:",him.szName,"Caster Name:",me and me.szName or "No Caster");
end

-- 按值反弹受到的伤害
function BufferMagicEffect:me_rebound_dmg_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_REBOUND_V, nValue);
	self:Add_ReboundV(me, nValue);

	self:DebugPrint("me_rebound_dmg_v",nValue,"Target Name:",him.szName,"Caster Name:",me and me.szName or "No Caster");
end

-- 按比例减少受到的伤害
function BufferMagicEffect:me_reducedmg(nPercent, nLifePercent, nBuffTemplateId)

	local nNewValue1 = nPercent;
	local nNewValue2 = 0
	if ( 0 <= nLifePercent ) then
		nNewValue2 = self:Get_MaxLife(me) * nLifePercent / 100;
	else
		nNewValue2 = -1;
	end

	self:Set_WithstandNormalP(me, nNewValue1);
	self:Set_WithstandNormalV(me, nNewValue2);

	self:DebugPrint("me_reducedmg"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 造成伤害按比例转化为生命回复
function BufferMagicEffect:me_transdmgtolife(nPercent, nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSTOLIFE_P);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = nPercent;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSTOLIFE_P, nAppendValue);
	end

	self:Add_TransToLifeP(me, nAppendValue);

	self:DebugPrint("me_transdmgtolife",nAppendValue,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 造成伤害按比例转化为真气回复
function BufferMagicEffect:me_transdmgtomana(nPercent, nBuffTemplateId)

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSTOMANA_P, nPercent);
	self:Add_TransToManaP(me, nPercent);

	self:DebugPrint("me_transdmgtomana"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

function BufferMagicEffect:me_ignorbuff(nBuffId, nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_ID);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = nBuffId;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_ID, nAppendValue);
	end

	me.pBufferEffect.IgnorBuff(nBuffId);

	self:DebugPrint("ignore buff:",nBuffId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end


function BufferMagicEffect:me_ignorbuffgroup(nGroupId, nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_GROUP);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = nGroupId;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_GROUP, nAppendValue);
	end

	me.pBufferEffect.IgnorBuffGroup(nAppendValue);

	self:DebugPrint("ignore buff group:",nGroupId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end


function BufferMagicEffect:me_ignorbuffcate(nCateType,nCateId, nBuffTemplateId)
	local nAppendValue1	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_CATETYPE);
	local nAppendValue2	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_CATEID);
	local bLogIn = ((nAppendValue1 ~= 0) and
					(nAppendValue2 ~= 0));

	if (not bLogIn) then
		nAppendValue1 = nCateType;
		nAppendValue2 = nCateId;

		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_CATETYPE, nAppendValue1);
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNOREBUFF_CATEID, nAppendValue2);
	end

	me.pBufferEffect.IgnorBuffCate(nAppendValue1,nAppendValue2);

	self:DebugPrint("ignore buff cate:",nCateType,",",nCateId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

function BufferMagicEffect:me_ignoredebuff(nType)
	me.SetIgnorDebuff(nType);

	self:DebugPrint("me_ignoredebuff", nType,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加治疗生命点数
function BufferMagicEffect:me_addtreatlife_v(nValue, nBuffTemplateId)

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_LIFE_BASE, nValue);

	self:Add_CuringLifeBase(me, nValue);

	self:DebugPrint("me_addtreatlife_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加治疗生命百分比
function BufferMagicEffect:me_addtreatlife_p(nPercent, nBuffTemplateId)

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_LIFE_PERCENT, nPercent);

	self:Add_CuringLifePercent(me, nPercent);

	self:DebugPrint("me_addtreatlife_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end


-- 增加治疗真气点数
function BufferMagicEffect:me_addtreatmana_v(nValue, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_MANA_BASE, nValue);

	self:Add_CuringManaBase(me, nValue);

	self:DebugPrint("me_addtreatmana_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加治疗真气百分比
function BufferMagicEffect:me_addtreatmana_p(nPercent, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CURING_MANA_PERCENT, nPercent);

	self:Add_CuringMana(me, nPercent);

	self:DebugPrint("me_addtreatmana_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 按值减少技能消耗
function BufferMagicEffect:me_reducecost_v(nValue,nCateType,nCateId, nBuffTemplateId)
	local nOldCateId = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILLCOST_CATEID);
	if nOldCateId ~= 0 then
		return;
	end

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILLCOST_CATEID, nCateId);

	me.pFightSkill.ReduceSkillCost(nCateType, nCateId, 1, nValue);

	self:DebugPrint("me_reducecost_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 按比例减少技能消耗
function BufferMagicEffect:me_reducecost_p(nPercent,nCateType,nCateId, nBuffTemplateId)
	local nOldCateId = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILLCOST_CATEID);
	if nOldCateId ~= 0 then
		return;
	end

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILLCOST_CATEID, nCateId);

	me.pFightSkill.ReduceSkillCost(nCateType, nCateId, 0, nPercent);

	self:DebugPrint("me_reducecost_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 免疫伤害
function BufferMagicEffect:me_ignoredmg()
	self:Add_IgnorDmg(me, 1);

	self:DebugPrint("me_ignoredmg"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 增加对应cate技能的攻击力百分比
function BufferMagicEffect:me_addmagnification(nCateType, nCateId, nValue)
	-- no use
end

-- 增加攻击力值
function BufferMagicEffect:me_addatkdmg_v(nAddMin, nAddMax, nBuffTemplateId)
	local nAppendValue11	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_PHYSICATTACK_MAX);
	local nAppendValue12	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_PHYSICATTACK_MIN);
	local nAppendValue21	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAGICATTACK_MAX);
	local nAppendValue22	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAGICATTACK_MIN);
	local bLogIn = ((nAppendValue11 ~= 0) and
					(nAppendValue12 ~= 0) and
					(nAppendValue21 ~= 0) and
					(nAppendValue22 ~= 0));

	if (not bLogIn) then
		nAppendValue11 = nAddMax;
		nAppendValue12 = nAddMin;
		nAppendValue21 = nAddMax;
		nAppendValue22 = nAddMin;

		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_PHYSICATTACK_MAX, nAppendValue11);
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_PHYSICATTACK_MIN, nAppendValue12);
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAGICATTACK_MAX, nAppendValue21);
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_MAGICATTACK_MIN, nAppendValue22);
	end

	self:modifydmg_v(nAppendValue11, nAppendValue12, nAppendValue21, nAppendValue22);

	self:DebugPrint("me_addatkdmg_v"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 免疫会心攻击
function BufferMagicEffect:me_ignorcristal(nBuffTemplateId)
	local nAppendValue	= me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNORE_CRISTAL);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = 1;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_IGNORE_CRISTAL, nAppendValue);
	end

	self:Add_IgnorCristal(me, nAppendValue);

	self:DebugPrint("me_ignorcristal"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

function BufferMagicEffect:GetParamIdx(szAttrName)
	return self.TransParamIdx[szAttrName];
end

-- 将参数nParam1按照nPercent附加到nParam2上
function BufferMagicEffect:GetParamValue(character, index)
	if(index == 6) then
		return self:Get_MaxLife(character);
	elseif(index == 7) then
		return self:Get_MaxMana(character);
	elseif(index == 10) then
		return self:Get_HitCoefficient(character);
	elseif(index == 11) then
		return self:Get_HitCoefficient(character);
	elseif(index == 12) then
		return self:Get_Dodge(character);
	elseif(index == 13) then
		return self:Get_Dodge(character);
	elseif(index == 14) then
		return self:Get_CriticalValue(character);
	elseif(index == 15) then
		return self:Get_CuringLife(character);  --治疗只有生命治疗
	elseif(index == 16) then
		local nLostLife = self:Get_MaxLife(character) - self:Get_CurrentLife(character);
		local nLostLifePercent = nLostLife / self:Get_MaxLife(character) * 100;
		return math.floor(nLostLifePercent);          --ldy

		--return self:Get_MaxLife(character) - self:Get_CurrentLife(character);
	elseif(index == 17) then
		return self:Get_Def(character);
	end
end

-- 将参数nParam1按照nPercent附加到nParam2上
function BufferMagicEffect:AddParamValue(character, index, nAppendValue)
	if(index == 6) then
		self:Add_MaxLife(character, him, nAppendValue);
	elseif(index == 7) then
		self:Add_MaxMana(character, nAppendValue);
	elseif(index == 8) then
		self:Add_Attack(character, nAppendValue);
	elseif(index == 9) then
		self:Add_Attack(character, nAppendValue);
	elseif(index == 10) then
		self:Add_HitCoefficient(character, nAppendValue);
	elseif(index == 11) then
		self:Add_HitCoefficient(character, nAppendValue);
	elseif(index == 12) then
		self:Add_Dodge(character, nAppendValue);
	elseif(index == 13) then
		self:Add_Dodge(character, nAppendValue);
	elseif(index == 14) then
		self:Add_CriticalValue(character, nAppendValue);
	elseif(index == 15) then
		self:Add_CuringLife(character, nAppendValue);	--治疗只有生命治疗
	elseif(index == 17) then
		self:Add_Def(character, nAppendValue);
	end
end

-- 将参数nParam1按照nPercent附加到nParam2上
function BufferMagicEffect:me_trans_param(nSrcParam,nDstParam,nPercent, nBuffTemplateId)
	--技能的計算信息(攻擊力係數、成長值、是否物理攻擊、需求等級)
	local nSkillID, fMagnification, nSkillGrow, nNotUseSkillLv = me.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	local bIsPhysic, nReqLevel = him.pFightSkill.GetSkillFightInfo(nSkillID);
	if nNotUseSkillLv == 1 then
		nReqLevel = 1;
	end
	bIsPhysic = (bIsPhysic == 1) and true or false;

	nPercent = nPercent + (him.GetLevel() - nReqLevel) * nSkillGrow;

	local nOldValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSPARAM_VALUE);
	self:AddParamValue(me, nDstParam, -nOldValue);

	local nValue = self:GetParamValue(me, nSrcParam) * nPercent / 100;
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSPARAM_TYPE, nDstParam);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_TRANSPARAM_VALUE, nValue);

	self:AddParamValue(me, nDstParam, nValue);

	me.pBufferEffect.SetTransParam(nSrcParam, nDstParam, nPercent);

	self:DebugPrint("me_trans_param"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 更改对应技能的引导类型，将原本的引导技能变成瞬发技能
function BufferMagicEffect:me_changechanneltype(nCateType,nCateId,nType, nBuffTemplateId)
	if nType == nil then
		nType = 0;
	end
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CHANGE_CHANNEL_CATETYPE, nCateType);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_CHANGE_CHANNEL_CATEID, nCateId);

	me.pFightSkill.ResetSkillChannelType(nCateType,nCateId, 0);
	self:DebugPrint("change the channel type of skill cate:",nCateType,",",nCateId,"Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster")
end

-- 缩短引导时间
function BufferMagicEffect:me_reducechanneltime(nCateType, nCateId, nFrame, nBuffTemplateId)
	local nAppendValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_CHANNELTIME);
	local bLogIn = nAppendValue ~= 0;

	if (not bLogIn) then
		nAppendValue = -nFrame;
		me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_SKILL_CHANNELTIME, nAppendValue);
	end

	me.pFightSkill.ResetSkillChannelTime(nCateType, nCateId, nAppendValue);

	self:DebugPrint("me_reducechanneltime"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 闪烁到目标背后
function BufferMagicEffect:me_twinkle()
	local nLength = -1; --跳到身后1米处
	local nDirection = me.nFaceDirection;
	local coordinate = me.GetFrontPosition(nLength, nDirection);
	local mPosHim = him.GetPosition();

	local nEndX, nEndY, nEndZ = him.pBufferEffect.JumpTo(mPosHim.nX, mPosHim.nY, mPosHim.nZ, coordinate.nX, coordinate.nY, coordinate.nZ);
	him.TeleportTo(him.dwSceneId, nEndX, nEndY, nEndZ);

	him.SetFaceDirection(me.nFaceDirection);

	self:DebugPrint("me_twinkle"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 跳到目标身前(类似闪烁),  by KK
function BufferMagicEffect:me_jumpto()
	-- me 是被buffer者, him是玩家
	local nLength = -1; --跳到对方身前1米处  ~注意是身前
	--local mPosHim = him.GetPosition();
	--local nDirection = me.GetDirection(mPosHim.nX, mPosHim.nY);--him.nFaceDirection;
	--local coordinate = me.GetFrontPosition(nLength, nDirection);

	--local nEndX, nEndY, nEndZ = me.pBufferEffect.JumpTo(mPosHim.nX, mPosHim.nY, mPosHim.nZ, coordinate.nX, coordinate.nY, coordinate.nZ);

	--him.TeleportTo(him.dwSceneId, nEndX, nEndY, nEndZ);
	local nX, nY, nZ = me.pBufferEffect.GetGroundBuffPosition()
	him.SetPosition(nX, nY, nZ)

	self:DebugPrint("me_jumpto"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 自己后翻滚  by KK
function BufferMagicEffect:me_backflip(nDis, nBuffTemplateId)
	-- him是玩家, me是敌人
	local nLength = -4; --自己向后翻滚距离

	local mPosMe = me.GetPosition();
	local nSkillDestX, nSkillDestY = me.pBufferEffect.GetDestPosition(nBuffTemplateId);
	local nDirection = KGameBase.GetDirection(mPosMe.nX, mPosMe.nY, nSkillDestX, nSkillDestY);
	local nDestX, nDestY, nDestZ = KGameBase.GetPosition(mPosMe.nX, mPosMe.nY, mPosMe.nZ, nDirection, nLength * 64);

	-- 临时注释瞬移 by KK
	--me.JumpTo(coordinate.nX, coordinate.nY, coordinate.nZ);
	-- 障碍检查
	local bIsObstacle, nLastOkX, nLastOkY = him.pFightSkill.CheckObstacle(
		mPosMe.nX, mPosMe.nY, mPosMe.nZ,
		nDestX, nDestY, nDestZ
	);

	if (bIsObstacle <= 0) then  -- 有障碍 false
		nDestX = nLastOkX;
		nDestY = nLastOkY;
	end

	me.SetPosition(nDestX, nDestY, nDestZ);
	--me.TeleportTo(him.dwSceneId, nDestX, nDestY, nDestZ);

	self:DebugPrint("me_backflip"," ","Target Name:",me.szName,"Caster Name:",me and me.szName or "No Caster");
end

-- 增加受到的伤害百分比
function BufferMagicEffect:me_addrecievedmg_p(nPercent, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_ENLARGE_ATTACK_P, nPercent);

	self:Add_EnlargeAttackP(me, nPercent);

	self:DebugPrint("me_addrecievedmg_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 减少受到的伤害百分比
function BufferMagicEffect:me_reducerecievedmg_p(nPercent, nBuffTemplateId)
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_REDUCE_ATTACK_P, nPercent);

	self:Add_ReduceAttackP(me, nPercent);

	self:DebugPrint("me_reducerecievedmg_p"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 同时出现hide及vision时，可见
function BufferMagicEffect:me_visiontonpc()
	me.SetIsVisiontoNPC(1);
	self:DebugPrint("me_visiontonpc"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 同时出现hide及vision时，可见
function BufferMagicEffect:me_visiontoplayer()
	me.SetIsVisiontoPlayer(1);
	self:DebugPrint("me_visiontoplayer"," ","Target Name:",me.szName,"Caster Name:",him and him.szName or "No Caster");
end

-- 吸血（一次属性）
function BufferMagicEffect:me_vampire(nValue,nPercent, nBuffTemplateId)
	local caster = him;	-- 施法者
	local becast = me;	-- 被施法者

	--技能的計算信息(攻擊力係數、成長值、是否物理攻擊、需求等級)
	local nSkillId = becast.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
	local nCoefficientMin, nCoefficientMax = caster.pFightSkill.GetSkillCoefficient(nSkillId);
	local nSkillParam = KUnify.MathRandom(nCoefficientMin, nCoefficientMax);

	if caster.IsNpc() ~= 1 then
		nValue = nValue + self.TestHurtValue;
	end

	--计算伤害并减血
	local nHarmValue, bCriticalHit = self:damagelife_v(caster, becast, nValue, nSkillParam, nBuffTemplateId);
	nHarmValue = math.floor(nHarmValue);
	self:Add_CurrentLife(caster, becast, -nHarmValue, bCriticalHit);

	--针对本次伤害加一次吸血
	local add_blood = nHarmValue * nPercent / 100;
	self:Add_CurrentLife(caster, caster, add_blood);

	self:DebugPrint("me_vampire"," ","Target Name:",becast.szName,"Caster Name:",caster and caster.szName or "No Caster");
end

function BufferMagicEffect:me_add_battleintegration_v(nValue)  --增加战场旗帜积分
	local nScore = me.GetGlobalBattleScore();
	nScore = nScore + nValue;
	assert(nScore > 0);
	me.SetGlobalBattleScore(nScore);
end

-- 死亡后对目标所在点释放一个范围buff
function BufferMagicEffect:me_explode(nBuffId, nSkillId)
	local mPos = me.GetPosition();
	BufferApi.AddGroundBuffer(nBuffId, him.dwId, nSkillId, me.dwSceneId, mPos.nX, mPos.nY, mPos.nZ, 1);
	self:DebugPrint("explode add buff:", nBuffId);
end

--------------------------------------------------------------------------------------------
function BufferMagicEffect:me_yongzhewudi(nValue, nBuffTemplateId) --勇者无敌，根据当前生命损失量计算闪避值
	local nOldValue = me.pBufferEffect.GetAppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_YONGZHEWUDI);
	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_YONGZHEWUDI, -nOldValue);
	self:Add_Dodge(me, -nOldValue);

	local nPercent = me.GetCurrentLife() / me.GetMaxLife() * 100;
	local nLostPercent = 100 - nPercent;
	local nDodgeValue = nLostPercent * nValue / 10; --填写的值是实际值的10倍

	me.pBufferEffect.AppendData(nBuffTemplateId, self.BuffDataType.DATATYPE_YONGZHEWUDI, nDodgeValue);
	self:Add_Dodge(me, nDodgeValue);

	--self:DebugPrint("me_yongzhewudi()");  --该属性不做服务端打印输出  不然一直刷
end

------------------------------------------ end ---------------------------------------------
