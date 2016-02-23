AwardLobby.tbAwardLobby =  { 
					--[1]={1,2,3,4,5,6,7,8},
					--[2]={9,10,11,12,13,14,15,16},
					{1, 3, {{1,1}, {2,2}, {3,3}, {4,10}, {11,30}, {-1,-1}}},
					{4, 4, {{1,1}, {2,2}, {3,3}, {4,10}, {11,30}, {-1,-1}}},
					{5, 5, {{1,1}, {2,2}, {3,3}, {4,10}, {11,30}, {-1,-1}}},
					{7, 6, {{1,1}, {2,2}, {3,3}, {4,10}, {11,30}, {-1,-1}}},
					{2, 7, {{1,1}, {2,2}, {3,3}, {4,10}, {11,30}, {-1,-1}}},
					};
					
local tbAwardLobby = AwardLobby.tbAwardLobby;


function AwardLobby:c2s_SignIn(pPlayer, nDay)
	AwardLobby:SignIn(pPlayer, nDay);
end

function AwardLobby:SignIn(pPlayer, nDay)
	if pPlayer == nil then
		return;
	end
	if Vip:CanBuyCount(pPlayer, Vip.ID_SIGNED_IN, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.awardlobby.str1[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = 10;
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.awardlobby.str2[Lang.Idx], nNeedGold))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,114);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_SIGNED_IN, 1);
	
	pPlayer.pAwardLobby.DoSignIn(nDay);
end

function AwardLobby:DoFinishActivity(pPlayer, nId, nAwardId)
	if pPlayer == nil then
		return;
	end
	
	pPlayer.pAwardLobby.DoFinishActivity(nId, nAwardId);
end

function AwardLobby:ProcessLobbyRank()
	local tbAllPlayer = KGameBase.GetAllPlayers();
	for _, pPlayer in pairs(tbAllPlayer) do
		for i=1,#tbAwardLobby,1 do
			local nRank = KRank.GetMyRank(pPlayer.dwId, tbAwardLobby[i][1]);
			for j = 1, #tbAwardLobby[1][3],1 do
				if nRank >= tbAwardLobby[1][3][j][1] and nRank <= tbAwardLobby[1][3][j][2] then
					pPlayer.pAwardLobby.DoFinishRankActivity(tbAwardLobby[i][2], j);
				end
			end
		end
	end
end

Timer:Register(Env.GAME_FPS * 5, AwardLobby.ProcessLobbyRank, AwardLobby);


--银行投宝收益(测试)str: 2014-08-02 12:12:12
function AwardLobby:DoGetBankIncomeTest(pPlayer, nId, str)
	if pPlayer == nil then
		return;
	end
	
	pPlayer.pAwardLobby.DoGetBankIncomeTest(nId, str);
end
