if not (MODULE_GAMECENTER) then
	return
end

function Escort:DoInviteFriend_GC(nInviteId, szPlayerName, nFriendId)
	GlobalExecute({"Escort:DoInviteFriend_GS", nInviteId, szPlayerName, nFriendId});
end

function Escort:OnInviteFriend_GC(nInviteId, szFriendName, nFriendId)
	GlobalExecute({"Escort:OnInviteFriend_GS", nInviteId, szFriendName, nFriendId});
end

function Escort:OnInviteFriendFailed_GC(szPlayerName, nFriendId)
	GlobalExecute({"Escort:OnInviteFriendFailed_GS", szPlayerName, nFriendId});
end

function Escort:OnInviteFriendSuccess_GC(nFriendId)
	GlobalExecute({"Escort:OnInviteFriendSuccess_GS", nFriendId});
end

function Escort:SendRescueMsg_GC(nRescuePlayerId, szResName)
	GlobalExecute({"Escort:SendRescueMsg_GS", nRescuePlayerId, szResName});
end

function Escort:OnFinishEscort_GC(nProtecterId, nEscortLevel)
	GlobalExecute({"Escort:OnFinishEscort_GS", nProtecterId, nEscortLevel});
end