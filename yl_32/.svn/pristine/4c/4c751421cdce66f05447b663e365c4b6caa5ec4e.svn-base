-- Â–“£¬º
local tbXoyolu = Item:GetClass("xoyolu");
function tbXoyolu:InitGenInfo()
--[[
	local tbTime = os.date("*t", GetTime());
	local month = tbTime.month;
	local year = tbTime.year;
	if month == 12 then
		month = 1;
		year = year + 1;
	else
		month = month + 1;
	end
	
	it.SetTimeOut(0, Lib:GetDate2Time(year*10000 + month*100 + 1) - 1);
--]]
	return {};
end

function tbXoyolu:GetTip()
	return XoyoGame.XoyoChallenge:GetXoyoluTips(me);
end

-- Â–“£π»ø®∆¨
local tbXoyoCard = Item:GetClass("xoyo_card");
function tbXoyoCard:InitGenInfo()
--	it.SetTimeOut(0, GetTime() + 2*3600);
	return {};
end

function tbXoyoCard:OnUse()
	local nRes, szMsg = XoyoGame.XoyoChallenge:UseCard(me, it);
	if nRes == 0 then
		me.SysMsg(szMsg);
	end
end

-- √ÿ±¶
local tbXoyoTreasure = Item:GetClass("xoyo_treasure");
function tbXoyoTreasure:InitGenInfo()
	it.SetTimeOut(0, GetTime() + 2*3600);
	return {};
end

--DoScript("\\script\\mission\\xoyogame\\item\\xoyo_challenge.lua")