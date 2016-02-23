
local tbNpc = Npc:GetClass("pkchangjieyinren")

--武器
tbNpc.tbEQUIP_WEAPON = "/setting/item/equip/weapon.txt";

--防具
tbNpc.tbEQUIP_AMULET = "/setting/item/equip/amulet.txt";
tbNpc.tbEQUIP_ARMOR = "/setting/item/equip/armor.txt";
tbNpc.tbEQUIP_BELT = "/setting/item/equip/belt.txt";
tbNpc.tbEQUIP_BRACERS = "/setting/item/equip/bracers.txt";
tbNpc.tbEQUIP_FOOT = "/setting/item/equip/foot.txt";
tbNpc.tbEQUIP_NECKLACE = "/setting/item/equip/necklace.txt";
tbNpc.tbEQUIP_PENDANT = "/setting/item/equip/pendant.txt";
tbNpc.tbEQUIP_RING = "/setting/item/equip/ring.txt";

--外装
tbNpc.tbEQUIP_HAT = "/setting/item/equip/hat.txt";
tbNpc.tbEQUIP_SHOULDER = "/setting/item/equip/shoulder.txt";
tbNpc.tbEQUIP_BACK = "/setting/item/equip/back.txt";
tbNpc.tbEQUIP_BOTTOM = "/setting/item/equip/bottom.txt";

function tbNpc:OnDialog()
	local tbOpt = 
	{
		--{"领取适合装备",self.GetEquip,self},
		{Lang.npc.str83[Lang.Idx],self.LeaveMap,self},
		{Lang.npc.str84[Lang.Idx]},
	}
	return Dialog:Say(Lang.npc.str85[Lang.Idx],tbOpt);
end;

function tbNpc:LeaveMap()
	--me.SetLevel(me.pTask.GetTask(2052,1) or me.GetLevel());
	me.TeleportTo(100,1904,7121,1048576);
	me.SetCurrentCamp(1);
end

function tbNpc:GetEquip()
	local dwLevel = me.GetLevel();
	local nQualityMax = 4;
	local tbOpt = {}
	
	if dwLevel <= 19 then
		nQualityMax = 2;
	elseif dwLevel <= 59 then
		nQualityMax = 3;
	end
	local nFactionId = me.GetPrimaryFaction();
	local tbWeaponTable = {};
	local tbDefenceTable = {};
	local j = 1;
	
	local dwMinLevel = dwLevel - 10
	if dwMinLevel <= 0 then
		dwMinLevel = math.max(1,dwMinLevel + 2);
	end
	
	local nNeedLevel = ((dwMinLevel == 1) and dwMinLevel or (math.floor(dwLevel / 10) * 10));

	--获取武器	
	local tbWeaponData = Lib:LoadTabFile(tbNpc.tbEQUIP_WEAPON) 

	for key,tbRow in pairs(tbWeaponData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbWeaponTable[#tbWeaponTable + 1] = tbRow;
				end
			end
		end
	end
	
	-----------------------获取防具-------------------------
	--腰坠
	local tbAmulateData = Lib:LoadTabFile(tbNpc.tbEQUIP_AMULET) 

	for key,tbRow in pairs(tbAmulateData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--护甲
	local tbArmorData = Lib:LoadTabFile(tbNpc.tbEQUIP_ARMOR) 

	for key,tbRow in pairs(tbArmorData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--腰带
	local tbBeltData = Lib:LoadTabFile(tbNpc.tbEQUIP_BELT) 

	for key,tbRow in pairs(tbBeltData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--护腕
	local tbBracersData = Lib:LoadTabFile(tbNpc.tbEQUIP_BRACERS) 

	for key,tbRow in pairs(tbBracersData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--鞋子
	local tbFootData = Lib:LoadTabFile(tbNpc.tbEQUIP_FOOT) 

	for key,tbRow in pairs(tbFootData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--项链
	local tbNecklaceData = Lib:LoadTabFile(tbNpc.tbEQUIP_NECKLACE) 

	for key,tbRow in pairs(tbNecklaceData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--护符
	local tbPendantData = Lib:LoadTabFile(tbNpc.tbEQUIP_PENDANT) 

	for key,tbRow in pairs(tbPendantData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--戒指
	local tbRingData = Lib:LoadTabFile(tbNpc.tbEQUIP_RING) 

	for key,tbRow in pairs(tbRingData) do
		if tonumber(tbRow["ReqLevel"]) == nNeedLevel then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == nQualityMax then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--------------------获取外装------------------------
	--不能直接获取外装
	----------------------------------------------------
	
	for key,tbRow in pairs(tbWeaponTable) do
		if me.pItem.CountFreeBagCell() >= 1 then
			KItem.AddPlayerItem(me, tonumber(tbRow.Genre), tonumber(tbRow.DetailType), tonumber(tbRow.ParticularType));
		else
			break;
		end
	end
	for key,tbRow in pairs(tbDefenceTable) do
		if me.pItem.CountFreeBagCell() >= 1 then
			KItem.AddPlayerItem(me, tonumber(tbRow.Genre), tonumber(tbRow.DetailType), tonumber(tbRow.ParticularType));
		else
			break;
		end
	end
end
