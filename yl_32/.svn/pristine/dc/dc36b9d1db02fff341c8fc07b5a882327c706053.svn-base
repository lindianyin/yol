
-- 随机道具

-------------------------------------------------------------------
--File: 	randomitem.lua
--Author: 	
--Date: 	
--Describe:	获取随机物品，通用脚本
--第一个扩展参数代表随机表中的第几组
--其他扩展参数暂时不做处理

------------------------------------------------------------------------------------------
-- initialize

local tbRandomItem = Item:GetClass("randomitem");

------------------------------------------------------------------------------------------
-- public

local tbRandomFile = Lib:LoadTabFile("/setting/item/magic/randomitem.txt") or {};

--[[
[1]:
[1].Amount	= "数量（只有道具有用）"
[1].Announce	= "世界公告"
[1].Bind	= "绑定"
[1].Desc	= "类描述"
[1].DetailType	= ""
[1].EnhTimes	= "强化"
[1].Exp	= "经验"
[1].Genre	= ""
[1].Id	= "组内ID"
[1].KinOrTongMsg	= "家族或帮会公告"
[1].KindId	= "Kind类ID"
[1].Level	= ""
[1].Money	= "银两"
[1].Name	= "开出物品描述"
[1].ParticularType	= ""
[1].Probability	= "概率（上限为10000，总概率小于10000时有几率不随出道具，如果大于10000则大于10000的部分随不到）"
[1].RandomCount	= "随机次数"
[1].TimeLimit	= "有效期"
[1].YLEnergy	= "游龙真气"
[2]:
[2].Amount	= ""
[2].Announce	= ""
[2].Bind	= ""
[2].Desc	= ""
[2].DetailType	= "1"
[2].EnhTimes	= ""
[2].Exp	= "100"
[2].Genre	= "1"
[2].Id	= "1"
[2].KinOrTongMsg	= ""
[2].KindId	= "1"
[2].Level	= "1"
[2].Money	= ""
[2].Name	= "100经验"
[2].ParticularType	= "1"
[2].Probability	= "2000"
[2].RandomCount	= "3"
[2].TimeLimit	= ""
[2].YLEnergy	= ""
[3]:
[3].Amount	= ""
[3].Announce	= ""
[3].Bind	= ""
[3].Desc	= ""
[3].DetailType	= ""
[3].EnhTimes	= ""
[3].Exp	= "200"
[3].Genre	= ""
[3].Id	= "2"
[3].KinOrTongMsg	= ""
[3].KindId	= "1"
[3].Level	= ""
[3].Money	= ""
[3].Name	= "200经验"
[3].ParticularType	= ""
[3].Probability	= "2000"
[3].RandomCount	= "3"
[3].TimeLimit	= ""
[3].YLEnergy	= ""
[4]:
[4].Amount	= ""
[4].Announce	= ""
[4].Bind	= ""
[4].Desc	= ""
[4].DetailType	= ""
[4].EnhTimes	= ""
[4].Exp	= "300"
[4].Genre	= ""
[4].Id	= "3"
[4].KinOrTongMsg	= ""
[4].KindId	= "1"
[4].Level	= ""
[4].Money	= ""
[4].Name	= "300经验"
[4].ParticularType	= ""
[4].Probability	= "2000"
[4].RandomCount	= "3"
[4].TimeLimit	= ""
[4].YLEnergy	= ""

--]]

function tbRandomItem:CheckUsable()
	return	1;						-- 可用
end




function tbRandomItem:OnUse()
	print("..tbRandomItem:OnUse()");
    --[[
	local nNeedBag = 0;
	local nRandomId = tonumber(it.GetExtParam(1));
	for _,tbRow in pairs(tbRandomFile) do  --概率-1的特殊处理
		if tonumber(tbRow.KindId) == nRandomId then
			if tonumber(tbRow.Probability) == -1 then
				if (tonumber(tbRow.Genre) and tonumber(tbRow.Genre) ~= 0) then
					nNeedBag = nNeedBag + 1;
				end
			end
		end
	end
	
	for _,tbRow in pairs(tbRandomFile) do  --
		if tonumber(tbRow.KindId) == nRandomId then
			if tonumber(tbRow.Probability) ~= -1 then
				if (tonumber(tbRow.Genre) and tonumber(tbRow.Genre) ~= 0) then
					nNeedBag = nNeedBag + 1;
					return;
				end
			end
		end
	end
	if me.pItem.CountFreeBagCell() < nNeedBag then
		me.SysMsg(Lang.item.str55[Lang.Idx]);
		return 0;
	end
    --]]
    
    --Lib:ShowTB(tbRandomFile);

    --local nKindId = tonumber(it.GetExtParam(1));
	
	local nMoneyType  = tonumber(it.GetExtParam(9));
	local nMoneyCount = tonumber(it.GetExtParam(10));
	
    
    --local tbRows = {};
    --for i=2,#tbRandomFile do 
    --    if(tonumber(tbRandomFile[i].KindId) == nKindId) then 
    --        tbRows[#tbRows + 1] = tbRandomFile[i];
    --    end
    --end
    
    if(nMoneyType and nMoneyType > 0) then
         local szMoneyUnit = "";
         if(nMoneyType == 1 ) then 
            szMoneyUnit = Lang.item.str59[Lang.Idx];
         elseif(nMoneyType == 2) then
            szMoneyUnit = Lang.item.str60[Lang.Idx];
         elseif(nMoneyType == 3) then
            szMoneyUnit = Lang.item.str61[Lang.Idx];
         elseif(nMoneyType == 4) then
            szMoneyUnit = Lang.item.str62[Lang.Idx];
         end
         local szMsg = string.format(Lang.mission.str1220[Lang.Idx],nMoneyCount,szMoneyUnit);
         Dialog:_AskForSure(me.dwId, szMsg, ({"Item:RandomItemCallback", me,it}), "GetRandomItem", 0);
	else
		Item:RandomItemCallback(me,it);
    end
	--tbRandomItem:SureOnUse();
end

--废弃
function tbRandomItem:SureOnUse()
	local nRandomId = tonumber(it.GetExtParam(1));
	local nRate = KUnify.MathRandom(1,10000);
	local nId;
	local nLineCount = 0;
	
	for _,tbRow in pairs(tbRandomFile) do  --概率-1的特殊处理
		nLineCount = nLineCount + 1;
		if tonumber(tbRow.KindId) == nRandomId then
			if tonumber(tbRow.Probability) == -1 then
				nId = tbRow.Id;
				tbRandomItem:GetItem(nLineCount);
			end
		end
	end
	
	nLineCount = 0;
	for _,tbRow in pairs(tbRandomFile) do  --各个概率中随机出一种
		nLineCount = nLineCount + 1;
		if tonumber(tbRow.KindId) == nRandomId then
			if (tonumber(tbRow.Probability) and tonumber(tbRow.Probability) ~= -1) then
				nRate = nRate - tonumber(tbRow.Probability);
				if nRate <= 0 then	--如果没有到达，即意味着有一定概率不会随出道具
					nId = tonumber(tbRow.Id);
					tbRandomItem:GetItem(nLineCount);
					return;
				end
			end
		end	
	end
end

--废弃
function tbRandomItem:GetItem(nLineCount)
	local nKindId = tonumber(tbRandomFile[nLineCount].KindId);
	local szDesc = tbRandomFile[nLineCount].Desc;
	local nId = tonumber(tbRandomFile[nLineCount].Id);
	local szName = tbRandomFile[nLineCount].Name;
	local nExp = tonumber(tbRandomFile[nLineCount].Exp) or 0;
	local nMoney = tonumber(tbRandomFile[nLineCount].Money) or 0;
	local nYLEnergy = tonumber(tbRandomFile[nLineCount].YLEnergy) or 0;
	local nGenre = tonumber(tbRandomFile[nLineCount].Genre) or 0;
	local nDetailType = tonumber(tbRandomFile[nLineCount].DetailType) or 0;
	local nParticularType = tonumber(tbRandomFile[nLineCount].ParticularType) or 0;
	local nLevel = tonumber(tbRandomFile[nLineCount].Level) or 0;
	local nEnhTimes = tonumber(tbRandomFile[nLineCount].EnhTimes) or 0;
	local nAmount = tonumber(tbRandomFile[nLineCount].Amount) or 1;
	local nBindType = tonumber(tbRandomFile[nLineCount].Bind) or 0;
	local nTimeLimit = tonumber(tbRandomFile[nLineCount].TimeLimit) or 0;
	local nAnnounce = tonumber(tbRandomFile[nLineCount].Announce) or 0;
	local nKinOrTongMsg = tonumber(tbRandomFile[nLineCount].KinOrTongMsg) or 0;
	
	--获取经验
	if (nExp and nExp ~= 0 and nAnnounce and nAnnounce ~= 0) then
		me.AddExp(nExp);
		local Msg = string.format(Lang.item.str65[Lang.Idx], me.szName, szName, nExp);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
	elseif (nExp and nExp ~= 0 and nKinOrTongMsg and nKinOrTongMsg ~= 0) then
		me.AddExp(nExp);
		local Msg = string.format(Lang.item.str65[Lang.Idx], me.szName, szName, nExp);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
	elseif (nExp and nExp ~= 0) then
		me.AddExp(nExp);
	end
	
	--获取银两
	if (nMoney and nMoney ~= 0 and nAnnounce and nAnnounce ~= 0) then
		me.pPlayerPurse.AddMoney(2, nMoney);
		local Msg = string.format(Lang.item.str66[Lang.Idx], me.szName, szName, nMoney);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
	elseif (nMoney and nMoney ~= 0 and nKinOrTongMsg and nKinOrTongMsg ~= 0) then
		me.pPlayerPurse.AddMoney(2, nMoney);
		local Msg = string.format(Lang.item.str66[Lang.Idx], me.szName, szName, nMoney);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
	elseif (nMoney and nMoney ~= 0) then
		me.pPlayerPurse.AddMoney(2, nMoney);
	end
	
	--获取游龙真气
	if (nYLEnergy and nYLEnergy ~= 0 and nAnnounce and nAnnounce ~= 0) then
		me.pPlayerPurse.AddMoney(6, nYLEnergy);
		local Msg = string.format(Lang.item.str67[Lang.Idx], me.szName, szName, nYLEnergy);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
	elseif (nYLEnergy and nYLEnergy ~= 0 and nKinOrTongMsg and nKinOrTongMsg ~= 0) then
		me.pPlayerPurse.AddMoney(6, nYLEnergy);
		local Msg = string.format(Lang.item.str67[Lang.Idx], me.szName, szName, nYLEnergy);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
	elseif (nYLEnergy and nYLEnergy ~= 0) then
		me.pPlayerPurse.AddMoney(6, nYLEnergy);
	end
	
	--获取道具
	if nAmount ~= 0 then  --Amount专门用于添加道具
		local pItem = KItem.AddPlayerItem(me, nGenre, nDetailType, nParticularType, nLevel, nAmount);
		local szItemName = pItem.szName;
		if nBindType and nBindType == 1 then
			pItem.Bind(1);
		end
		if pItem and nAnnounce ~= 0 then
			local Msg = string.format(Lang.item.str68[Lang.Idx], me.szName, szName, nAmount, szItemName);
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
		elseif pItem and nKinOrTongMsg ~= 0 then
			local Msg = string.format(Lang.item.str68[Lang.Idx], me.szName, szName, nAmount, szItemName);
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "Msg"});
		end
	end
	KItem.DelPlayerItem(me, it);	
	--道具限时
	--道具强化次数
	--世界公告
	--家族公告等

end



