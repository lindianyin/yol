-- 文件名　：purseinit.lua
-- 创建者　：wdb
-- 创建时间：2010-10-19 10:52:04
-- 功能描述：玩家钱袋 client, gs

local PURSE_CONFIG_FILE		= "/setting/currency/currency.txt"

function Purse:LoadCfg()
	local tbList = {};
	local tbData = Lib:LoadTabFile(PURSE_CONFIG_FILE)
	for i, money in pairs(tbData) do
		local tbOne = {};
		tbOne["Id"] = tonumber(money.Id) or -1
		tbOne["Name"] = money.Name
		tbOne["Desc"] = money.Desc
		tbOne["IconId"] = tonumber(money.IconId)
		tbList[tbOne["Id"]] = tbOne
	end
	self.tbPurseCfg = tbList
	return 1;
end

function Purse:GetPurseImageId(nPurseId)
	return self.tbPurseCfg[nPurseId]["IconId"]
end

function Purse:GetPurseDesc(nPurseId)
	return self.tbPurseCfg[nPurseId]["Desc"]
end

function Purse:GetPurseName(nPurseId)
	return self.tbPurseCfg[nPurseId]["Name"]
end

function Purse:GetPlayerMaxCarryCurrency()
	return Purse.EM_PLAYER_MAXCARRYCURRENCY;
end

Purse:LoadCfg()
