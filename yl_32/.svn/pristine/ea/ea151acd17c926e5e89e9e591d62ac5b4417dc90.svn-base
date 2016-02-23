
function Vip:AddDegree(pPlayer, szName, nDegree)
	if pPlayer == nil then
		return;
	end
	
	Player:AddMaxDegree(pPlayer, szName, nDegree)
	Player:AddDegree(pPlayer, szName, nDegree)
end

function Vip:DoBuyCount(pPlayer, nId, nNum)
	if pPlayer == nil then
		return 0;
	end
	
	if nNum == nil then
		nNum = 1;
	end
	
	return pPlayer.pVip.DoBuyCount(nId, nNum);
end

function Vip:CanBuyCount(pPlayer, nId, nNum)
	if pPlayer == nil then
		return 0;
	end
	
	if nNum == nil then
		nNum = 1;
	end
	
	return pPlayer.pVip.CanBuyCount(nId, nNum);
end

function Vip:IsOpen(pPlayer, nId, bSendMsg)

	if bSendMsg == nil then
		bSendMsg = 1;
	end
	
	if pPlayer == nil then
		return 0;
	end

	return pPlayer.pVip.IsOpen(pPlayer.dwId, nId, bSendMsg);
end

function Vip:GetRate(pPlayer, nId)
	if pPlayer == nil then
		return 0;
	end

	return pPlayer.pVip.GetRate(pPlayer.dwId, nId);
end

function Vip:RefreshSaveBuyCount()
	local tbPlayers = KGameBase.GetAllPlayers();
	for _, pPlayer in pairs(tbPlayers) do
		pPlayer.pVip.RefreshBuyCount(pPlayer.dwId);
	end
end