

-- 宠物蛋
local tbPetCard = Item:GetClass("pet_card");

function tbPetCard:OnUse()

	local tbPets = KPet.GetPlayerPetList(me);
	local nPetCapacity = me.GetPetCapacity();
	
	if #tbPets == nPetCapacity then
		me.SendBlackMsg(Lang.mission.str1302[Lang.Idx]);
		return;
	end
	
	assert(#tbPets < nPetCapacity);
	-- 物品附加参数注释: 1 宠物编号 2宠物品质 3阵法中位置
	local nType = tonumber(it.GetExtParam(1))--KUnify.MathRandom(it.GetExtParam(1), it.GetExtParam(2));
	local nQuality = tonumber(it.GetExtParam(2))--Lib:RandProb(tbRate); -- 品质
	assert(nQuality);
	local nCharacter = tonumber(it.GetExtParam(3))--KUnify.MathRandom(it.GetExtParam(3), it.GetExtParam(4));
	if(nCharacter > 7 or nCharacter < 0) then
		nCharacter = KUnify.MathRandom(1,7)
	end
	-- local tbRate = {};
	-- for i = it.GetExtParam(5), it.GetExtParam(6) do
	-- 	tbRate[i] = Pet.QUALITY_RATE[i];
	-- end
	-- Lib:ShowTB(tbRate);

	--local nQuality = KUnify.MathRandom(it.GetExtParam(5), it.GetExtParam(6));
	
	KItem.DelPlayerItem(me, it);
	print("add pet:", nType, nQuality, nCharacter);
	local pPet = KPet.AddPlayerPet(me, nType, nCharacter);
	
	me.SysMsg(Lang.mission.str1303[Lang.Idx] .. pPet.szName);
	
	if nQuality >= 2 then
		local szMsg = string.format(Lang.mission.str1304[Lang.Idx],Pet.QUALITY_TIPS[nQuality], pPet.szName);
		me.SendBlackMsg(szMsg);
	end
end

-- 坐骑卡
local tbRideCard = Item:GetClass("ride_card");

function tbRideCard:OnUse()
	local pRide = KPet.GetRide(me);
	--local tbRideColor =
	--{
	--	[1] = "<color=white>",
	--	[2] = "<color=green>",
	--	[3] = "<color=pink>",
	--	[4] = "<color=blue>",
	--	[5] = "<color=purple>",
	--	[6] = "<color=gold>",
	--}
	
	local nGenre = it.nGenre;
	local nDetail = it.nDetail;
	local nParticular = it.nParticular;
	local nLevel = it.nLevel;
	local nHungry = it.GetExtParam(1);
	local nMagicRate = it.GetExtParam(2);
	
	if nHungry == 0 then
		nHungry = Pet.RIDE_MAX_HUNGRY;
	elseif nHungry < 0 then
		nHungry = 0;
	end
	
	KItem.DelPlayerItem(me, it);
	
	-- 先卸下当前坐骑
	if pRide then
		local bRet = Pet:TakeOffRide(me, pRide);
		if bRet == 0 then
			return;
		end
	end
	
	local pRide = KPet.AddPlayerRide(me, nGenre, nDetail, nParticular, nLevel, nHungry, nMagicRate);
	me.SysMsg(Lang.mission.str1305[Lang.Idx]..pRide.szName);
	--local szMsg = string.format("您装备了坐骑[%s%s<color=white>]", tbRideColor[nLevel], pRide.szName);
	--me.SendBlackMsg(szMsg);
end

local tbTaskPetCard = Item:GetClass("task_pet_card");

function tbTaskPetCard:OnUse()
	local tbPets = KPet.GetPlayerPetList(me);
	local nPetCapacity = me.GetPetCapacity();
	
	if #tbPets == nPetCapacity then
		me.SendBlackMsg(Lang.mission.str1306[Lang.Idx]);
		return;
	end
	
	assert(#tbPets < nPetCapacity);
	
	local nType = KUnify.MathRandom(it.GetExtParam(1), it.GetExtParam(2));
	local nCharacter = KUnify.MathRandom(it.GetExtParam(3), it.GetExtParam(4));
	local tbRate = {};
	for i = it.GetExtParam(5), it.GetExtParam(6) do
		tbRate[i] = Pet.QUALITY_RATE[i];
	end
	local nQuality = Lib:RandProb(tbRate);
	assert(nQuality);
	--local nQuality = KUnify.MathRandom(it.GetExtParam(5), it.GetExtParam(6));
	
	KItem.DelPlayerItem(me, it);
	print("add pet:", nType, nQuality, nCharacter);
	local pPet = KPet.AddPlayerPet(me, nType, nCharacter);
	me.SysMsg(Lang.mission.str1307[Lang.Idx] .. pPet.szName);

	local szMsg = string.format(Lang.mission.str1308[Lang.Idx],Pet.QUALITY_TIPS[nQuality], pPet.szName);
	me.SendBlackMsg(szMsg);
end

local tbTaskRideCard = Item:GetClass("task_ride_card");

function tbTaskRideCard:OnUse()
	local pRide = KPet.GetRide(me);
	
	local nGenre = it.GetExtParam(1);
	local nDetail = it.GetExtParam(2);
	local nParticular = it.GetExtParam(3);
	local nLevel = it.GetExtParam(4);
	
	KItem.DelPlayerItem(me, it);
	
	-- 先卸下当前坐骑
	if pRide then
		local bRet = Pet:TakeOffRide(me, pRide);
		if bRet == 0 then
			return;
		end
	end
	
	local pRide = KPet.AddPlayerRide(me, nGenre, nDetail, nParticular, nLevel);
	me.SysMsg(Lang.mission.str1309[Lang.Idx] .. pRide.szName);
end