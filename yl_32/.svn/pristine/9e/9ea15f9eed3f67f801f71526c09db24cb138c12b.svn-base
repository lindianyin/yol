Remind.ID_LIAN_GUA 				= 1;		--炼卦
Remind.ID_EMAIL 				= 2;		--邮件

Remind.ID_QUIZ_START 			= 14;		--答题开始前5分钟
Remind.ID_QUIZ_GOING 			= 15;		--答题进行中
Remind.ID_QUIZ_END 				= 16;		--答题结束前5分钟


Remind.ID_KINBATTLE_START		= 32;
Remind.ID_KINBATTLE_GOING		= 33;
Remind.ID_KINBATTLE_END			= 34;


function Remind:AddRemindToPlayer(pPlayer, nRemindId, nCountDown)
	--print("..Remind:AddRemindToPlayer(pPlayer, nRemindId, nCountDown)",pPlayer.dwId, nRemindId, nCountDown);
	if pPlayer == nil then
		return
	end 
	KRemind.AddRemindToPlayer(pPlayer, nRemindId, nCountDown);
end

function Remind:RemoveRemindToPlayer(pPlayer, nRemindId)
	print("..Remind:RemoveRemindToPlayer(pPlayer, nRemindId)",pPlayer.dwId, nRemindId);
	if pPlayer == nil then
		return
	end 
	KRemind.RemoveRemindToPlayer(pPlayer, nRemindId);
end

function Remind:RemindToAllPlayers(nRemindId, nCountDown)
	print("..Remind:RemindToAllPlayers(nRemindId, nCountDown)",nRemindId, nCountDown);
	local tbPlayers = KGameBase.GetAllPlayers();
	for _, pPlayer in pairs(tbPlayers) do
		KRemind.AddRemindToPlayer(pPlayer, nRemindId, nCountDown);
	end
end

--function Remind:c2s_AddRemindToPlayer(pPlayer, nRemindId, nCountDown, playerId)
--	print("..Remind:c2s_AddRemindToPlayer(pPlayer, nRemindId, nCountDown, playerId)",pPlayer.dwId, nRemindId, nCountDown, playerId);
--	if playerId ~= nil and playerId > 0 then
--		Remind:AddRemindToPlayer(KGameBase.GetPlayerById(nId), nRemindId, nCountDown);
--	else
--		Remind:AddRemindToPlayer(pPlayer, nRemindId, nCountDown);
--	end
--end

--function Remind:c2s_RemoveRemindToPlayer(pPlayer, nRemindId, playerId)
--	print("..Remind:c2s_RemoveRemindToPlayer(pPlayer, nRemindId, playerId)",pPlayer, nRemindId, playerId);
--	if playerId > 0 then
--		Remind:RemoveRemindToPlayer(KGameBase.GetPlayerById(playerId), nRemindId);
--	else
--		Remind:RemoveRemindToPlayer(pPlayer, nRemindId);
--	end
--end
