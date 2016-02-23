
--Describe: 客户端调用服务端脚本接口，不要在客户端暴露此文件
-------------------------------------------------------------------
if not MODULE_GAMESERVER then
	return
end

-- 表内每个c2sCallName都对应gs脚本函数名，以c2s_开头。
-- c2s_函数对客户端参数做校验，不要在接口函数里处理逻辑
-- 只允许在此处统一注册c2s接口
-- !!!服务端被调函数传参插入当前玩家pPlayer
local tbAllowedC2SCall = 
{
	["Directory.DirectoryMgr:c2s_ClientApplyGSTime"] = 1,

	["Dialog:c2s_ClearCallBacks"] = 1,
	["Dialog:c2s_OnSelect"] = 1,
	["Dialog:c2s_OnEnterPage"] = 1,
	["Dialog:c2s_OnAskStringOk"] = 1,
	["Dialog:c2s_OnAskNumberOk"] = 1,
	
	["Task:c2s_AutoAcceptStoryTask"] = 1, 
	
	["Reward:c2s_TakeAllAwards"] = 1,

	["AutoPath:c2s_AutoTalkNpc"] = 1,
	["AutoPath:c2s_AutoOpenDoodad"] = 1,
	
	["Doodad:c2s_Gather"] = 1,
	
	["Scene:c2s_LeaveMission"] = 1,
	["Scene:c2s_GoToInstance"] = 1,
	
	-- 通用进度条
	["ProgressBar:c2s_CancelProgressBar"] = 1,
	
	["Player:c2s_CommitSurveyResult"] = 1,
	["XoyoGame:c2s_ApplyJoinGame"] = 1,
	["Instance:c2s_TryEnterInstance"] = 1,
	["Instance:c2s_StartGame"] = 1,
	["Player:c2s_BroadcastCreateTeamMsg"] = 1,
	["Player:c2s_BroadcastCreateOverteamMsg"] = 1,
	["Player:c2s_BroadcastEscortHelpMsg"] = 1,
	
	-- 神仙
	  ["Pet:c2s_UnlockEquipPos"] = 1,
	  ["Pet:c2s_UpgradePetSkill"] = 1,
	
	-- 关卡副本
	["PVEMatch:c2s_EnterInstanceWaitQueue"] = 1,
	["PVEMatch:c2s_AcceptEnterInstance"] = 1,
	["PVEMatch:c2s_EnterRoomWaitQueue"] = 1,
	
	["PVEMatch:c2s_EnterStageRequest"] = 1,
	["PVEMatch:c2s_DoEnterStage"] = 1,
	
	["PVEMatch:c2s_EnterInstanceRequest"] = 1,
	["PVEMatch:c2s_DoEnterInstance"] = 1,
	
	["PVEMatch:c2s_LeaveWaitQueue"] = 1,
	["PVEMatch:c2s_RejectEnterInstance"] = 1,
	["PVEMatch:c2s_GetSuitableInstanceId"] = 1,
	["Instance:c2s_GetAward"] = 1,
	["Stage:c2s_GetAward"] = 1,
	["MultiInstance:c2s_GetAward"] = 1,
	["Instance:c2s_StartSweepInstance"] = 1,
	["Instance:c2s_StopSweepInstance"] = 1,
	["Instance:c2s_FinishSweepInstance"] = 1,
	["Instance:c2s_BuyCount"] = 1,
	["Instance:c2s_SpeedUpInstance"] = 1,
	["Instance:c2s_UnlockGod"] = 1,
	
	["Relation:c2s_AddRelation"] = 1,
	["Relation:c2s_DelRelation"] = 1,
	["Relation:c2s_SyncRelation"] = 1,
	["Kin:c2s_Impeach"] = 1,
	["Kin:c2s_SetApplyerState"] = 1,
	["XoyoGame:c2s_TryEnterDTTGame"] = 1,
	["XoyoGame:c2s_TryEndDTTGame"] = 1,
	["Kin:c2s_CreateTong"] = 1,
	["Kin:c2s_UpgradeKin"] = 1,
	["Kin:c2s_RandomSelectKinCamp"] = 1,
	["Kin:c2s_SelectKinCamp"] = 1,
	["Kin:c2s_BroadcastRecruitMsg"] = 1,
	["Activity:c2s_EnterKinActivity"] = 1,
	
	["EightTrigram:c2s_TryUpdateLevel"] = 1,
	["EightTrigram:c2s_TryAutoUpdateLevel"] = 1,
	["EightTrigram:c2s_TrySpeedUp"] = 1,
	
	["Activity:c2s_JoinZbt"] = 1,  -- 战宝堂  k
	["Activity:c2s_JoinBattle"] = 1,  -- 跨服战场  k
	["Activity:c2s_JoinBoss"] = 1,  -- BOSS  k
	["Activity:c2s_Mpjj"] = 1,  -- 战神无双(门派竞技)积分  k
	["Activity:c2s_EnterBonfireActivity"] = 1,  -- 帮派篝火  k
	["Activity:c2s_ApplyJoinKinBattle"] = 1,
	["Activity:c2s_ApplyEnterKinBattle"] = 1,
	["Swim:c2s_JoinSwim"] = 1,	-- 泡澡活动
	["Activity:c2s_JoinMHZD"] = 1,	--蛮荒之地
	["Activity:c2s_SetPlayerFightMode"] = 1, --设置玩家战斗模式
	["Activity:c2s_EnterKinPTSY"] = 1, --参加蟠桃盛宴活动
	["Activity:c2s_SyncMHZD"] = 1, --同步蛮荒之地boss数量
	
	["PetIsland:c2s_EnterPetIsland"] = 1,  -- 请求进入宠物岛 k
	["PetIsland:c2s_RefreshPets"] = 1,  -- 请求刷新宠物岛的宠物 k
	["PetIsland:c2s_StartFight"] = 1,  -- 请求跟宠物开战 k
	["PetIsland:c2s_ChoosePet"] = 1,  -- 选择宠物 k
	["PetIsland:c2s_UpRefreshRate"] = 1,  -- 提升刷新成功率 k
	["PetIsland:c2s_RefreshNature"] = 1, -- 重新随机性格
	["PetIsland:c2s_GetCard"] = 1,       -- 抽取神仙卡
	
	["Task:c2s_DoAcceptTask"] = 1,
	--竞技场相关
	["Arena:c2s_DoEnterArena"] = 1, 
	["Arena:c2s_LeaveArena"] = 1, 
	["Arena:c2s_LoadArenaPlayers"] = 1,
	["Arena:c2s_LoadFrontArenaPlayers"] = 1,
	["Arena:c2s_TakeComboAward"] = 1,
	["Arena:c2s_GetChallengeTimes"] = 1,
	["Arena:c2s_ClearCDTime"] = 1,
	
	--伐魔塔相关
	["Tower:c2s_DoChallenge"] = 1,
	["Tower:c2s_StartGame"] = 1, 
	["Tower:c2s_DoLeaveGame"] = 1,
	["Tower:c2s_BuyMopTimes"] = 1, 
	["Tower:c2s_BuyChallengeTimes"] = 1, 
	["Tower:c2s_DoStartNextLayer"] = 1,
	
	-- 关卡相关
	["Stage:c2s_StartGame"] = 1,
	["Stage:c2s_StopMop"] = 1,
	["Stage:c2s_FinishMopNow"] = 1,
	["Stage:c2s_BuyMopTimes"] = 1,
	["Stage:c2s_BuyRushTimes"] = 1,
	["Stage:c2s_DoEnterGameSingle"] = 1,
	
	-- 多人副本
	["MultiInstance:c2s_StartGame"] = 1,
	["MultiInstance:c2s_DoEnterGameSingle"] = 1,
	["MultiInstance:c2s_BuyRushTimes"] = 1,
	
	-- 放烟火
	["Player:c2s_TaskIconProcess"] = 1,

	["Dialog:c2s_AskForSureCallBack"] = 1,
	["Player.tbDegree:c2s_SyncDegree"] = 1,
	["Player.tbDegree:c2s_ReduceDegree"] = 1, -- 调试用
	
	--换元宝
	["Player.tbPurse:c2s_AddMoney"] 	= 1,
	["Player.tbPurse:c2s_AddYLEnergy"] = 1,
	["Player.tbPurse:c2s_AddMenoyYLEnergy_Free"] = 1,
	["Player.tbPurse:c2s_AddMenoyYLEnergy_Coin"] = 1,
	
	-- 修仙
	["Escort:c2s_EnterEscortMap"] = 1,
	["Escort:c2s_RefreshEscort"] = 1,
	["Escort:c2s_DoInviteFriend"] = 1,
	["Escort:c2s_LeaveEscort"] = 1,
	["Escort:c2s_SpeedUpEscort"] = 1,
	["Escort:c2s_OnRescuePlayer"] = 1,
	["Escort:c2s_BuyEscortTimes"] = 1,
	["Escort:c2s_BuyRefreshTimes"] = 1,
	["Escort:c2s_BuyRobTimes"] = 1,
	["Escort:c2s_OnInviteFriend"] = 1,
	["Escort:c2s_BuyHelpTimes"] = 1,
	
	--答题
	["Quiz:c2s_SetDoubleFreeReduce"] = 1,
	["Quiz:c2s_DoClientSpirit"] = 1,
	["Quiz:c2s_EnterIssue"] = 1,
	
	--称号
	["Title:c2s_OffTitle"] = 1,
	["Title:c2s_OnTitle"] = 1,
	["Title:c2s_SyncTitle"] = 1,
	
	--打坐
	["Meditation:c2s_TakeOfflineAward"] = 1,
	
	--提醒
	["Remind:c2s_AddRemindToPlayer"] = 1,
	["Remind:c2s_RemoveRemindToPlayer"] = 1,
	
	-- 聊天
	["Chat:c2s_DisablePlayerChat"] = 1,
	
	--每日任务
	["DailyTasks:c2s_GetAward"] = 1,
	
	--奖励大厅
	["AwardLobby:c2s_SignIn"] = 1,
	
	["Scene:c2s_CheckRemoveBladeState"] = 1,
	
	--通用物品合成
	["Item:c2s_composeItem"] = 1,


	--每日任务
	["DailyTasks:c2s_accept"] = 1,
	["DailyTasks:c2s_getreward"] = 1,
	["DailyTasks:c2s_directFinish"] = 1,
	["DailyTasks:c2s_getStarAward"] = 1,
	["DailyTasks:c2s_reflashAllTask"] = 1,
	["DailyTasks:c2s_handin_item"] = 1,
	["DailyTasks:c2s_giveup"] = 1,

	--领取在线奖励
	["OnlineReward:c2s_getOnlineReward"] = 1,
}

function c2s:OnClientCall(pCallingPlayer, tbCall)
	self:DbgOut("OnClientCall", pCallingPlayer, unpack(tbCall))
	local szFuncRequested = tbCall[1]
	-- 第一个参数必须是c2s表里的一个函数名字
	local varFunc = tbAllowedC2SCall[szFuncRequested]
	if not varFunc then
		print("ERROR: c2s '"..szFuncRequested.."' not in tbAllowedC2SCall")
		assert(false);
	end
	if varFunc ~= 1 then
		print("ERROR: c2s '"..szFuncRequested.."' is disabled")
		assert(false);
	end
	
	if string.find(szFuncRequested, "c2s_") == nil then
		print("ERROR: "..szFuncRequested.." no 'c2s_' prefix")
		assert(false)
	end
	
	--Lib:CallBack(tbCall)
	-- 服务端被调函数传参插入当前玩家pPlayer
	Lib:CallBack{szFuncRequested, pCallingPlayer, unpack(tbCall, 2)}
end
