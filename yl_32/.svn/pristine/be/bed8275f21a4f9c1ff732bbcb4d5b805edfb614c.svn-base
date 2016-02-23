-- 文件名:bufconditioncommand.lua
-- 创建者:zhangjunjie
-- 创建时间:2012-05-11 15:51:23
-- buff 条件判断和定义
--	当前脚本中已经压入了me和him，me为buff的接受者，him为buff的释放者
--	me是必定存在的，him如果不存在则说明没有释放者

BufferCondition = BufferCondition or {};

BufferCondition.nResult = nil;
BufferCondition.nTriggerParam1 = nil;
BufferCondition.nTriggerParam2 = nil;
BufferCondition.nTriggerParam3 = nil;
BufferCondition.nTriggerParam4 = nil;
BufferCondition.nTriggerParam5 = nil;

function BufferCondition:PreTriggerEffect(arg1, arg2, arg3, arg4, arg5)
	self.nTriggerParam1 = arg1;
	self.nTriggerParam2 = arg2;
	self.nTriggerParam3 = arg3;
	self.nTriggerParam4 = arg4;
	self.nTriggerParam5 = arg5;
end

function BufferCondition:IsHaveBuff(nBuffId)
	return true;
end

function BufferCondition:IsPlayer()
	if me.IsPlayer() ~= 1 then
		return false;
	else
		return true;
	end
end

function BufferCondition:IsSameTeam()
	return true;
end

function BufferCondition:IsSex(nNum)
	return false;
end

function BufferCondition:IsWeekDay(nDayId)
	return true;
end

function BufferCondition:IsMap(nMapID)
	return false;
end

function BufferCondition:IsCamp(nCampId)
	return true;
end

function BufferCondition:IsFaction(nFactionId,nRouteId)
	return false;
end

function BufferCondition:IsLevel(nMinLv,nMaxLv)
	return true;
end

function BufferCondition:IsEqualPile(nBuffId, nPileNum)
	local nStack = me.pBufferEffect.GetCurrentBufferStack(nBuffId);
	return nStack == nPileNum;
end

function BufferCondition:GetResult()
	local nResult = BufferCondition.nResult;
	BufferCondition.nResult = nil;	--??
	if (nResult == true) then
	    return 1;
	else
		return 0;
	end
end

---buffer trigger 事件回调,triger定义在代码和脚本里各有一套,但是由代码fire，在脚本里回调
---buffer trigger 中是没有me和him的，需要从代码里fireevent里传入对应的me和him的playerid
function BufferCondition:RegisterTriggerEvent()	--注册对应的事件回调
	CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_BUFF_BUFF_IN,self.BuffIn,self);
	CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ATTRIBUTE_CHANGED, self.CharacterAttributeChange, self);
end

BufferCondition:RegisterTriggerEvent();


function BufferCondition:CharacterAttributeChange(nCharacterId, wVarGroup, wVarId)
end


-----------------------------------------------------一次检测的触发属性---------------------------------------------------

--buff触发的前置条件，可以进行组合
function BufferCondition:None()
	return true;
end

--打出buff时触发
function BufferCondition:BuffOut(nCasterId, nVictimId, nTemplateId, nStackCount, Cate1_Id, Cate2_Id, Cate3_Id)
	local pCaster = KGameBase.GetCharacterById(nCasterId);
	if not pCaster then
		return;
	end

	local pTarget = KGameBase.GetCharacterById(nVictimId);
	if not pTarget then
		return;
	end

	pCaster.CheckTrigger(string.format("BuffOut(%d)",nTemplateId), pCaster, pTarget);

	--打出某一category的buff时触发
	self:BuffCateOut(pCaster, pTarget, Cate1_Id, Cate2_Id, Cate3_Id);

	--被buff击中时触发
	self:BuffIn(pTarget, pCaster, nTemplateId, Cate1_Id, Cate2_Id, Cate3_Id);

	--buff层数判断
	self:BuffPile(pTarget, pCaster, nTemplateId, nStackCount);
end

--打出某一category的buff时触发
function BufferCondition:BuffCateOut(pCharacter, pTarget, Cate1_Id, Cate2_Id, Cate3_Id)
	pCharacter.CheckTrigger(string.format("BuffCateOut(%d,%d)", -1, -1), pCharacter, pTarget);
	pCharacter.CheckTrigger(string.format("BuffCateOut(%d,%d)", -1, Cate1_Id), pCharacter, pTarget);
	pCharacter.CheckTrigger(string.format("BuffCateOut(%d,%d)", -1, Cate2_Id), pCharacter, pTarget);
	pCharacter.CheckTrigger(string.format("BuffCateOut(%d,%d)", -1, Cate3_Id), pCharacter, pTarget);

	pCharacter.CheckTrigger(string.format("BuffCateOut(%d,%d)", 1, Cate1_Id), pCharacter, pTarget);
	pCharacter.CheckTrigger(string.format("BuffCateOut(%d,%d)", 2, Cate2_Id), pCharacter, pTarget);
	pCharacter.CheckTrigger(string.format("BuffCateOut(%d,%d)", 3, Cate3_Id), pCharacter, pTarget);
end

--打出伤害时触发
function BufferCondition:DmgOut(caster, becast)
	if not pCharacter then
		return false;
	end
	
	caster.CheckTrigger("DmgOut()", caster, becast);
end

--打出技能时触发
function BufferCondition:SkillDmgOut(caster, becast)
	if not caster then
		return false;
	end
	
	self:DebugPrint("[SkillDmgOut] name = ", caster.szName);
	caster.CheckTrigger("SkillDmgOut()", caster, becast);
end

--会心一击时会触发该效果
function BufferCondition:Critical(caster, becast, nBuffTemplateId)
	if not caster then
		return false;
	end
	
	becast.CheckTrigger("BeCritical()", caster);
	caster.CheckTrigger("Critical()", caster, becast);

	local nSkillID, fMagnification, nSkillGrow = becast.pBufferEffect.GetCurrProcBuffFightInfo(nBuffTemplateId);
end

--是否在目标的背后
function BufferCondition:IsBack()
	local isBack = me.IsBack(him.dwId);
	self:DebugPrint("is back:", 1);
	if isBack == 1 then
		return true;
	else
		return false;
	end
end

function BufferCondition:NoEnemyInDis(nRadius)
	local tbEnemy = me.GetEnemyInDis(nRadius);
	if not tbEnemy or #tbEnemy > 0 then
		return false;
	else
		return true;
	end
end

function BufferCondition:BloodLessThan(nPercent)
	local nLifePercent = me.GetCurrentLife() * 100 / me.GetMaxLife();
	if nLifePercent < nPercent then
		return true;
	else
	    return false;
	end
end

--发出技能时触发
function BufferCondition:SkillOut(nCharacterId, nTargetId, nSkillId)
	if not nCharacterId then
		return false;
	end
	
	local pCharacter = KGameBase.GetCharacterById(nCharacterId);
	if not pCharacter then
		return;
	end
	local pTarget = KGameBase.GetCharacterById(nTargetId);
	
	CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_BEGIN_CAST_SKILL, nCharacterId);
	
	pCharacter.CheckTrigger("SkillOut()", pCharacter);
	
	local tbCate = pCharacter.pFightSkill.GetSkillCate(nSkillId);
	if pTarget then
		pCharacter.CheckTrigger(string.format("SkillCateOut(%d,%d)", 1, tbCate.cate1), pCharacter, pTarget);
	else
		pCharacter.CheckTrigger(string.format("SkillCateOut(%d,%d)", 1, tbCate.cate1), pCharacter);
	end
end

--命中时触发
function BufferCondition:Hit(caster, becast)
	if not caster then
		return false;
	end
	
	caster.CheckTrigger("Hit()", caster, becast);
end

--未命中时触发该效果
function BufferCondition:Miss(caster, becast)
	if not caster then
		return false;
	end
	
	caster.CheckTrigger("Miss()", caster, becast);
end

--目标定身时触发
function BufferCondition:IsFix()
	return BufferMagicEffect:Get_IsFix(me)
end

--被buff击中时触发
function BufferCondition:BuffIn(pCharacter, pCaster, nTemplateId, Cate1_Id, Cate2_Id, Cate3_Id)
	self:DebugPrint(string.format("BuffIn(%d)",nTemplateId));
	--pCharacter.pBufferEffect.SetCurrentBuffer(nTemplateId);
	
	pCharacter.CheckTrigger(string.format("BuffIn(%d)",nTemplateId), pCaster, pCharacter);

	--被某一category的buff击中时触发
	self:BuffCateIn(pCharacter, pCaster, Cate1_Id, Cate2_Id, Cate3_Id);
end

--被某一category的buff击中时触发
function BufferCondition:BuffCateIn(pCharacter, pCaster, Cate1_Id, Cate2_Id, Cate3_Id)
	pCharacter.CheckTrigger(string.format("BuffCateIn(%d,%d)", -1, -1), pCaster, pCharacter);
	pCharacter.CheckTrigger(string.format("BuffCateIn(%d,%d)", -1, Cate1_Id), pCaster, pCharacter);
	pCharacter.CheckTrigger(string.format("BuffCateIn(%d,%d)", -1, Cate2_Id), pCaster, pCharacter);
	pCharacter.CheckTrigger(string.format("BuffCateIn(%d,%d)", -1, Cate3_Id), pCaster, pCharacter);

	pCharacter.CheckTrigger(string.format("BuffCateIn(%d,%d)", 1, Cate1_Id), pCaster, pCharacter);
	pCharacter.CheckTrigger(string.format("BuffCateIn(%d,%d)", 2, Cate2_Id), pCaster, pCharacter);
	pCharacter.CheckTrigger(string.format("BuffCateIn(%d,%d)", 3, Cate3_Id), pCaster, pCharacter);
end

--受到伤害时触发
function BufferCondition:DmgIn(pCharacter, pSender)
	-- 类似这种trigger因为是手动触发的，但是刚加上buff时的ProcessTrigger也会触发一次，所以第一次要忽略
	if not pCharacter then
		return false;
	end
	
	self:DebugPrint("[DmgIn] name = ", pCharacter.szName);
	pCharacter.CheckTrigger("DmgIn()", pSender, pCharacter);
end

--被伤害技能打中时触发，不一定造成伤害
function BufferCondition:SkillDmgIn(pCharacter, pSender)
	if not pCharacter then
		return false;
	end
	
	self:DebugPrint("[SkillDmgIn] name = ", pCharacter.szName);
	pCharacter.CheckTrigger("SkillDmgIn()", pSender, pCharacter);
end

--buff的拥有者死亡时触发
function BufferCondition:Death(pCharacter, pCaster)
	if not pCharacter then
		return false;
	end
	
	self:DebugPrint("[Death] name = ", pCharacter.szName);
	pCharacter.CheckTrigger("Death()", pCaster);
end

--buff的拥有者击杀其他单位时触发
function BufferCondition:Kill(killer, victim)
	killer.CheckTrigger(string.format("Kill(%d)", 0), killer, victim);

	if (victim.IsNpc() == 1) then
		killer.CheckTrigger(string.format("Kill(%d)", 1), killer, victim);
	else
		killer.CheckTrigger(string.format("Kill(%d)", 2), killer, victim);
	end
end

--buff层数判断，若对应buffid的buff层数达到指定值则触发该效果。
function BufferCondition:BuffPile(pCharacter, pCaster, nBuffId, nBuffPile)
	if not nBuffId then
		return false;
	end
	
	pCharacter.CheckTrigger(string.format("BuffPile(%d,%d)", nBuffId, nBuffPile), pCaster, pCharacter);
end

--移动时触发
function BufferCondition:Move(nCharacterId)
	local pCharacter = KGameBase.GetCharacterById(nCharacterId);
	if not pCharacter then
		return;
	end

	pCharacter.CheckTrigger("Move()");
end

--闪避时出发
function BufferCondition:Dodge(pCharacter)
	if not pCharacter then
		return false;
	end
	pCharacter.CheckTrigger("Dodge()", pCharacter);
end

--受到控制效果（眩晕、定身。沉默、恐惧）时触发
function BufferCondition:Be_controled()
	return (BufferMagicEffect:Get_IsStun(me) or
			BufferMagicEffect:Get_IsFix(me) or
			BufferMagicEffect:Add_IsFear(me) or
			BufferMagicEffect:Get_IsSilence(me) or
			BufferMagicEffect:Get_IsCrazy(me));
end

function BufferCondition:EnterRange()
	return false;
end

function BufferCondition:LeaveRange()
	return false;
end

--检查目标是否在规定的region内，为方形区域，region的单位为32*32
function BufferCondition:IsInRegion(xPosMin,yPosMin,xPosMax,yPosMax)
	return me.IsInRecRange(xPosMin, xPosMax, yPosMin, yPosMax);
end

--检查目标是否在规定的原型区域
function BufferCondition:IsInCircle(nRadius)
	return me.IsInCircle(BufferCondition.nTriggerParam1, BufferCondition.nTriggerParam2, BufferCondition.nTriggerParam3, nRadius);
end


----------------------------------------------------------------------------------------------------------------------------
