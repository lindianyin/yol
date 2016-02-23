
function Quiz:OnChoice(bRight)
	Task:OnQuizChoice(me, bRight)  -- me here
end

function Quiz:CallQuiz(pPlayer, nQuizId)
	return pPlayer.pQuiz.CallQuiz(nQuizId)
end


------------------每日活动-----------------------------------------------
Quiz.tbOpenTime = { {{hour=13, min=30, sec=0}, {hour=13, min=43, sec=0}}, {{hour=16, min=30, sec=0}, {hour=16, min=43, sec=0}} } --(5分钟准备时间+8分钟答题时间（30题）)
local tbQuizOpenTime = Quiz.tbOpenTime or {};

Quiz.ISSUE_READY_TIME = 300 		--300秒
Quiz.ISSUE_ANSWER_TIME = 12			--12秒
Quiz.ISSUE_WAIT_TIME = 4			--4秒

Quiz.ISSUE_INIT	= 0
Quiz.ISSUE_JOIN	= 1
--QUIZ.ISSUE_NOJOIN = 2

Quiz.ISSUE_INIT_SPIRIT_COIN = 5		--直接给答案初始元宝
Quiz.ISSUE_ADD_SPIRIT_COIN = 5		--直接给答案递增元宝

Quiz.RANK_ELEVEN_INTEGRAL = 3500	--11名需要的积分
Quiz.RANK_PER_LEVEL = 500			--排名积分区间

Quiz.REMIND_START_TIME = 300

function Quiz:JoinQuizIssue(pPlayer, nJoin)
	local curDayTime = KGameBase.GetTime();--Lib:GetLocalDayTime()
	local joinFirst = Player.tbDegree:GetDegree(pPlayer, "QuizJoinFirst")
	local curIssueId = Quiz:GetCurrentIssue()
	
	local nStartTime,nEndTime,nTimes = Quiz:GetOpenTime();
	
	if nStartTime <= curDayTime and curDayTime <= nEndTime then
		
		if nJoin == Quiz.ISSUE_INIT then
			if nTimes == 0 or joinFirst > 0 then
				if nStartTime + Quiz.ISSUE_READY_TIME > curDayTime then
					pPlayer.pQuiz.SendIssueStart(nStartTime + Quiz.ISSUE_READY_TIME - curDayTime, 
												Quiz.ISSUE_INIT_SPIRIT_COIN, 
												Player.tbDegree:GetDegree(pPlayer, "QuizDoubleFreeTimes"), 
												Player.tbDegree:GetMaxDegree(pPlayer, "QuizDoubleFreeTimes"))
				elseif nStartTime + Quiz.ISSUE_READY_TIME < curDayTime then			--相等的时候服务器会主动发。这里不做操作
					KQuiz.SetQuizIssueCurrentId(curIssueId)
					pPlayer.pQuiz.SendIssueQuestion(curIssueId * (Quiz.ISSUE_ANSWER_TIME + Quiz.ISSUE_WAIT_TIME) + nStartTime + Quiz.ISSUE_READY_TIME - curDayTime - Quiz.ISSUE_WAIT_TIME, 
													Quiz.ISSUE_INIT_SPIRIT_COIN, 
													Quiz.ISSUE_ADD_SPIRIT_COIN, 
													Player.tbDegree:GetDegree(pPlayer, "QuizDoubleFreeTimes"),
													Player.tbDegree:GetMaxDegree(pPlayer, "QuizDoubleFreeTimes"))
				end
			else
				pPlayer.SendBlackMsg(Lang.quiz.str1[Lang.Idx]);
			end
		else
			if nTimes == 0 then
				Player.tbDegree:ReduceDegree(pPlayer, "QuizJoinFirst", 1)
			elseif nTimes == 1 then
				Player.tbDegree:ReduceDegree(pPlayer, "QuizJoinSecond", 1)
			else
				pPlayer.SendBlackMsg(Lang.quiz.str2[Lang.Idx])
			end
		end
	else
		pPlayer.SendBlackMsg(Lang.quiz.str3[Lang.Idx])
	end	
	

	DailyTasks:commitTaskValue(pPlayer,"JoinQuizIssue",DailyTasks.TYPE_JOIN_ACTIVITY,1);
	
end

function Quiz:DoSumitIntegral(pPlayer, nCurrentIssue)

	local tbAnswerInfo = pPlayer.pQuiz.GetIssueAnswerInfo()
	if tbAnswerInfo == nil or tbAnswerInfo["bCorrect"] == 0 then
		return
	end
	local nIntegral = tbAnswerInfo["nIntegral"]
	local bCorrect = tbAnswerInfo["bCorrect"]
	local nMutilpe = (tbAnswerInfo["bDouble"] > 0 and 2) or 1
	local leftTime = 0
	local addIntegral = 0
	local curDayTime = tbAnswerInfo["tTime"];--Lib:GetLocalDayTime(tbAnswerInfo["tTime"])

	local nStartTime,nEndTime,nTimes = Quiz:GetOpenTime();
	if nStartTime < curDayTime and curDayTime < nEndTime then
		leftTime = nStartTime + Quiz.ISSUE_READY_TIME + nCurrentIssue*Quiz.ISSUE_ANSWER_TIME + (nCurrentIssue-1)*Quiz.ISSUE_WAIT_TIME - curDayTime
	end
	
	if leftTime > 4 then
		addIntegral = nMutilpe * 100
	elseif leftTime == 4 then
		addIntegral = nMutilpe * 90
	elseif leftTime == 3 then
		addIntegral = nMutilpe * 80
	elseif leftTime == 2 then
		addIntegral = nMutilpe * 70
	elseif leftTime > 0 then
		addIntegral = nMutilpe * 60
	else
		addIntegral = 0
		pPlayer.pQuiz.SetIssueCorrect(0)
	end
	pPlayer.pQuiz.SumitIntegral(addIntegral+nIntegral)
end

function Quiz:NotifyQuizIssueAll(currentIssue)
	local curDayTime = KGameBase.GetTime();--Lib:GetLocalDayTime()
	
	local nStartTime,nEndTime,nTimes = Quiz:GetOpenTime();
	
	--Quiz:RemindPlayer(0, nil);
	if nStartTime > curDayTime or curDayTime > nEndTime then
		KQuiz.SetQuizIssueCurrentId(0);
		return;
	end
	
	local curIssueInx = Quiz:GetCurrentIssue()--(currentIssue > 0 and currentIssue) or 1
	local tbPlayers = KGameBase.GetAllPlayers()
	if nStartTime == curDayTime then
		KQuiz.DoIssueStart();
		KChat.BroadcastNoticeMsg(Lang.quiz.str4[Lang.Idx]);
		--print("-------------答题活动开始-------------")
		for _, pPlayer in pairs(tbPlayers) do
			if Player.tbDegree:GetDegree(pPlayer, "QuizJoinFirst") ~= 0 or nStartTime == curDayTime then
				pPlayer.pQuiz.SendIssueStart(Quiz.ISSUE_READY_TIME, 
											Quiz.ISSUE_INIT_SPIRIT_COIN, 
											Player.tbDegree:GetDegree(pPlayer, "QuizDoubleFreeTimes"), 
											Player.tbDegree:GetMaxDegree(pPlayer, "QuizDoubleFreeTimes"))
			end
		end
	elseif nStartTime + Quiz.ISSUE_READY_TIME == curDayTime then
		KQuiz.SetQuizIssueCurrentId(1);
		--print("-------------发题-------------")
		for _, pPlayer in pairs(tbPlayers) do
			if Player.tbDegree:GetDegree(pPlayer, "QuizJoinFirst") ~= 0 or nStartTime + Quiz.ISSUE_READY_TIME == curDayTime then
				pPlayer.pQuiz.SendIssueQuestion(Quiz.ISSUE_ANSWER_TIME, 
												Quiz.ISSUE_INIT_SPIRIT_COIN, 
												Quiz.ISSUE_ADD_SPIRIT_COIN, 
												Player.tbDegree:GetDegree(pPlayer, "QuizDoubleFreeTimes"), 
												Player.tbDegree:GetMaxDegree(pPlayer, "QuizDoubleFreeTimes"))
			end
		end
	elseif nStartTime + Quiz.ISSUE_READY_TIME + curIssueInx * (Quiz.ISSUE_ANSWER_TIME + Quiz.ISSUE_WAIT_TIME) - Quiz.ISSUE_WAIT_TIME == curDayTime then
		--print("-------------发结果-------------")
		for _, pPlayer in pairs(tbPlayers) do
			Quiz:DoSumitIntegral(pPlayer, curIssueInx)
			if pPlayer.pQuiz.GetIsJoinIssue() == Quiz.ISSUE_JOIN then
				pPlayer.pQuiz.SendIssueResult(Quiz.ISSUE_WAIT_TIME)
			end
		end
		Timer:Register(Env.GAME_FPS, Quiz.NameListRequest, Quiz, 1);
	elseif nEndTime == curDayTime then
		 --print("-------------答题结束------------")
		 for _, pPlayer in pairs(tbPlayers) do
			if pPlayer.pQuiz.GetIsJoinIssue() ~= Quiz.ISSUE_INIT then
				Quiz:GiveAward(pPlayer, pPlayer.pQuiz.GetPlayerRank())
			end
		end
	elseif nStartTime + Quiz.ISSUE_READY_TIME + curIssueInx * (Quiz.ISSUE_ANSWER_TIME + Quiz.ISSUE_WAIT_TIME) == curDayTime then
		KQuiz.SetQuizIssueCurrentId(curIssueInx + 1);
		--print("-------------发题-2------------",currentIssue + 1)
		for _, pPlayer in pairs(tbPlayers) do
			if pPlayer.pQuiz.GetIsJoinIssue() == Quiz.ISSUE_JOIN then
				pPlayer.pQuiz.SendIssueQuestion(Quiz.ISSUE_ANSWER_TIME, 
												Quiz.ISSUE_INIT_SPIRIT_COIN, 
												Quiz.ISSUE_ADD_SPIRIT_COIN, 
												Player.tbDegree:GetDegree(pPlayer, "QuizDoubleFreeTimes"), 
												Player.tbDegree:GetMaxDegree(pPlayer, "QuizDoubleFreeTimes"))
			end
		end
	else
		return
	end
end

function Quiz:NameListRequest(nRequest)
	KQuiz.NameListRequest(nRequest);
	return 0;
end

function Quiz:RemindPlayer(isLogin, pPlayer)
	local tbPlayers = KGameBase.GetAllPlayers()
	local curDayTime = KGameBase.GetTime();--Lib:GetLocalDayTime()
	
	local nStartTime,nEndTime = Quiz:GetOpenTime();
	
	if nStartTime == curDayTime + Quiz.REMIND_START_TIME then
		for _, pPlayer in pairs(tbPlayers) do
			Remind:AddRemindToPlayer(pPlayer, Remind.ID_QUIZ_START, -1)
		end
	elseif nStartTime == curDayTime then
		for _, pPlayer in pairs(tbPlayers) do
			Remind:AddRemindToPlayer(pPlayer, Remind.ID_QUIZ_GOING, -1)
		end
	elseif nEndTime == curDayTime + Quiz.REMIND_START_TIME then
		for _, pPlayer in pairs(tbPlayers) do
			Remind:AddRemindToPlayer(pPlayer, Remind.ID_QUIZ_END, Quiz.REMIND_START_TIME)
		end
	end
	
	if isLogin == 1 then
		if  (nStartTime - Quiz.REMIND_START_TIME < curDayTime and curDayTime < nStartTime) then
			Remind:AddRemindToPlayer(pPlayer, Remind.ID_QUIZ_START, -1)
		elseif (nStartTime < curDayTime and curDayTime < nEndTime - Quiz.REMIND_START_TIME) then
			Remind:AddRemindToPlayer(pPlayer, Remind.ID_QUIZ_GOING, -1)
		elseif (nEndTime - Quiz.REMIND_START_TIME < curDayTime and curDayTime < nEndTime) then
			Remind:AddRemindToPlayer(pPlayer, Remind.ID_QUIZ_END, nEndTime - curDayTime)
		end
	end
end

function Quiz:GiveAward(pPlayer, nPlayerRank)
	--local nAwardId = 0
	local nElevenRank = Quiz.RANK_ELEVEN_INTEGRAL
	local tbAnswerInfo = pPlayer.pQuiz.GetIssueAnswerInfo()

	if nPlayerRank > 0 and nPlayerRank <= 10 then
		nAwardId = nPlayerRank
		KAward.AddPlayerAward(pPlayer, "QuizAward", nAwardId)
	--else
		--[[if tbAnswerInfo ~= nil then
			while nElevenRank > tbAnswerInfo["nIntegral"] do
				nAwardId = nAwardId + 1
				nElevenRank = nElevenRank - Quiz.RANK_PER_LEVEL
			end
		end --]]
	end
	--除了排行榜奖励  增加与积分相关奖励
	pPlayer.pPlayerPurse.AddMoney(2,math.floor(tbAnswerInfo["nIntegral"]*1.5))
	pPlayer.pPlayerPurse.AddMoney(3,math.floor(tbAnswerInfo["nIntegral"]*1))
	if nPlayerRank == 1 then
		KChat.BroadcastNoticeMsg(string.format(Lang.quiz.str5[Lang.Idx],pPlayer.szName));
	end
	
	if 0 < nPlayerRank and nPlayerRank <= 10 then
		Title:AddTitleToCharacter(pPlayer, 7, nPlayerRank, 0) -- 给称号奖励
	end

	return nAwardId
end

function Quiz:GetOpenTime()
	local nStartTime = 0;
	local nEndTime = 0;
	local nTimes = 0;
	for _, tbOpenTime in pairs(tbQuizOpenTime) do
		if Activity:IsOpenTime(tbOpenTime) == 1 then
			nStartTime = Lib:GetSecByTimeTable(tbOpenTime[1]);
			nEndTime = Lib:GetSecByTimeTable(tbOpenTime[2]);
			break;
		end
		
		nTimes = nTimes+1;
	end
	return nStartTime,nEndTime,nTimes
end

function Quiz:GetCurrentIssue()
	local curIssueId = 0
 	local curDayTime = KGameBase.GetTime();--Lib:GetLocalDayTime()
	
	local nStartTime,nEndTime,nTimes = Quiz:GetOpenTime();
	
	while curDayTime > curIssueId * (Quiz.ISSUE_ANSWER_TIME + Quiz.ISSUE_WAIT_TIME) + nStartTime + Quiz.ISSUE_READY_TIME and curDayTime < nEndTime do
		curIssueId = curIssueId + 1
	end
	return curIssueId
end

function Quiz:c2s_SetDoubleFreeReduce(pPlayer, bFree, nCoins)
	if bFree == 0 then
		pPlayer.pQuiz.AddVipDoubleCount(1)
	else
		Player.tbDegree:ReduceDegree(pPlayer, "QuizDoubleFreeTimes", 1)	
	end
	pPlayer.pQuiz.SetIssueDouble(1)
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -1 * nCoins);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -1 * nCoins,115);
end

function Quiz:c2s_DoClientSpirit(pPlayer, nIssueId, nAnswerChoice, nCoins)
	local leftTime 			= 0
	local curDayTime 		= KGameBase.GetTime();--Lib:GetLocalDayTime()
	local curIssueId 		= Quiz:GetCurrentIssue()
	local nStartTime,nEndTime,nTimes = Quiz:GetOpenTime();
	if nStartTime < curDayTime and curDayTime < nEndTime then
		leftTime = nStartTime + Quiz.ISSUE_READY_TIME + curIssueId*Quiz.ISSUE_ANSWER_TIME + (curIssueId-1)*Quiz.ISSUE_WAIT_TIME - curDayTime
	end
	
	if leftTime > 0 then
		pPlayer.pQuiz.DoClientSpirit(nIssueId, nAnswerChoice)
		pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -1 * nCoins);
	else
		pPlayer.SendBlackMsg(Lang.quiz.str6[Lang.Idx])
	end
end

function Quiz:c2s_EnterIssue(pPlayer)
	local curDayTime = KGameBase.GetTime();--Lib:GetLocalDayTime()
	local joinFirst = Player.tbDegree:GetDegree(pPlayer, "QuizJoinFirst")
	local nStartTime,nEndTime,nTimes = Quiz:GetOpenTime();
	if nStartTime <= curDayTime and curDayTime <= nEndTime then
		if joinFirst > 0 or nTimes == 0 then 
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:OpenUIIssuePanel"});
			
			--统计点  ldy
			local szStr = string.format("name=%s","QuizIssue");
			pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,68,szStr);
			
		else
			pPlayer.SendBlackMsg(Lang.quiz.str1[Lang.Idx])
		end
	else
		pPlayer.SendBlackMsg(Lang.quiz.str3[Lang.Idx])
	end
end

function Quiz:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	Quiz:RemindPlayer(1, pPlayer);
end

function Quiz:RemindAllPlayer()
	--print("..Quiz:RemindAllPlayer()");
	local tbPlayers = KGameBase.GetAllPlayers();
	for _,pPlayer in pairs(tbPlayers) do 
		Quiz:RemindPlayer(1, pPlayer);
	end
end



Timer:Register(5 * Env.GAME_FPS, Quiz.RemindAllPlayer, Quiz)

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Quiz.OnLogin, Quiz);
