
local tbNpc = Npc:GetClass("xinshouzhiyin")

function tbNpc:OnDialog()
	local tbOpt =
	{
		{Lang.npc.str98[Lang.Idx],self.GetPaymentAward,self},
		{Lang.npc.str99[Lang.Idx],self.GetDailyAward,self},
		{Lang.npc.str100[Lang.Idx],self.GetFreshmanEquip,self},
		{Lang.npc.str101[Lang.Idx],self.GetTopLevelAward,self},
		{Lang.npc.str102[Lang.Idx],self.YouLongQandA,self},
		{Lang.npc.str103[Lang.Idx]},
	}
	Dialog:Say(Lang.npc.str104[Lang.Idx],tbOpt);
end

function tbNpc:GetPaymentAward()
	Dialog:Say(Lang.npc.str105[Lang.Idx]);
end

function tbNpc:GetDailyAward()
	local nCurrentDate = Lib:GetLocalDay();
	if not me.pTask.GetTask(2051,1) or nCurrentDate ~= me.pTask.GetTask(2051,1) then 
		me.pPlayerPurse.AddMoney(2, 100000);
		me.pTask.SetTask(2051,1,nCurrentDate);
	else
		Dialog:Say(Lang.npc.str106[Lang.Idx]);
	end
end

function tbNpc:GetFreshmanEquip()
	if me.pTask.GetTask(2053,1) == 1 then
		Dialog:Say(Lang.npc.str107[Lang.Idx]);
	else
		if me.pItem.CountFreeBagCell() >= 18 then
			self.GetFreshmanEquip2();
			me.pTask.SetTask(2053,1,1);
		else
			Dialog:Say(Lang.npc.str108[Lang.Idx]);
		end
	end
end


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

function tbNpc:GetFreshmanEquip2()
	local tbOpt = {}
	
	local nFactionId = me.GetPrimaryFaction();
	local tbWeaponTable = {};
	local tbDefenceTable = {};
	local j = 1;

	--获取武器	
	local tbWeaponData = Lib:LoadTabFile(tbNpc.tbEQUIP_WEAPON) 

	for key,tbRow in pairs(tbWeaponData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbWeaponTable[#tbWeaponTable + 1] = tbRow;
				end
			end
		end
	end
	
	-----------------------获取防具-------------------------
	--腰坠
	local tbAmulateData = Lib:LoadTabFile(tbNpc.tbEQUIP_AMULET) 

	for key,tbRow in pairs(tbAmulateData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--护甲
	local tbArmorData = Lib:LoadTabFile(tbNpc.tbEQUIP_ARMOR) 

	for key,tbRow in pairs(tbArmorData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--腰带
	local tbBeltData = Lib:LoadTabFile(tbNpc.tbEQUIP_BELT) 

	for key,tbRow in pairs(tbBeltData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--护腕
	local tbBracersData = Lib:LoadTabFile(tbNpc.tbEQUIP_BRACERS) 

	for key,tbRow in pairs(tbBracersData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--鞋子
	local tbFootData = Lib:LoadTabFile(tbNpc.tbEQUIP_FOOT) 

	for key,tbRow in pairs(tbFootData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--项链
	local tbNecklaceData = Lib:LoadTabFile(tbNpc.tbEQUIP_NECKLACE) 

	for key,tbRow in pairs(tbNecklaceData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--护符
	local tbPendantData = Lib:LoadTabFile(tbNpc.tbEQUIP_PENDANT) 

	for key,tbRow in pairs(tbPendantData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
					tbDefenceTable[#tbDefenceTable + 1] = tbRow;
				end
			end
		end
	end
	--戒指
	local tbRingData = Lib:LoadTabFile(tbNpc.tbEQUIP_RING) 

	for key,tbRow in pairs(tbRingData) do
		if tonumber(tbRow["ReqLevel"]) == 1 then
			if tonumber(tbRow["ReqProp1Type"]) == 3 and tonumber(tbRow["ReqProp1Value"]) == nFactionId then
				if tonumber(tbRow["Quality"]) == 1 then
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

function tbNpc:GetTopLevelAward()
	if me.GetLevel() < 60 then
		Dialog:Say(Lang.npc.str109[Lang.Idx]);
	else
		if me.pTask.GetTask(2053,2) == 1 then
			Dialog:Say(Lang.npc.str110[Lang.Idx]);
		else
			if me.pItem.CountFreeBagCell() >= 7 then
				self.GetTopLevelAward2();
				me.pTask.SetTask(2053,2,1);
			else
				Dialog:Say(Lang.npc.str108[Lang.Idx]);
			end
		end
	end
end

function tbNpc:GetTopLevelAward2()
	for i = 1 , 5 do
		if me.pItem.CountFreeBagCell() >= 1 then
			KItem.AddPlayerItem(me,18,3,1,5);
		else
			break;
		end
	end
	--KItem.AddPlayerItem(me,18,2,1,5);  --坐骑
	--KItem.AddPlayerItem(me,18,2,1,5);  --宠物
end

function tbNpc:YouLongQandA()
	local tbOpt =
	{
		{Lang.npc.str111[Lang.Idx]},
		{Lang.npc.str112[Lang.Idx]},
		{Lang.npc.str113[Lang.Idx]},
		{Lang.npc.str114[Lang.Idx]},
		{Lang.npc.str115[Lang.Idx]},
		{Lang.npc.str116[Lang.Idx]},
	}
	Dialog:Say(Lang.npc.str117[Lang.Idx],tbOpt);
end
