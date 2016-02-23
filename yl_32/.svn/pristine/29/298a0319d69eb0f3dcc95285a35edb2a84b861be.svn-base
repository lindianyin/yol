-- TaskAct为行为，在编辑器的任务完成处填写

TaskAct.tbTalkForbitHead	= {
-- string.byte返回字符的整数形式，比如"a"返回的就是97
	[string.byte("\r")]	= 1, 
	[string.byte("\n")]	= 1, 
	[string.byte(" ")]	= 1,
};

-- 立刻接收任务 ！
function TaskAct:DoAccept(nReferId)
	local tbReferData = Task.tbReferDatas[nReferId];
	if (not tbReferData) then
		print("[TaskAct:DoAccept]玩家"..me.szName.."准备接的任务："..nReferId.."不存在！");
		print(debug.traceback())
		return;
	end
	local nTaskId = tbReferData.nTaskId;
	
	local tbSubData	= Task.tbSubDatas[tbReferData.nSubTaskId];
	local szMsg = "";
	if (tbSubData.tbAttribute.tbDialog.Start.szMsg) then -- 未分步骤
			szMsg = tbSubData.tbAttribute.tbDialog.Start.szMsg;
	else
			szMsg = tbSubData.tbAttribute.tbDialog.Start.tbSetpMsg[1];
	end
	
	Task:GetPlayerTask(me).tbAskAccept	= { -- 防止客户端作弊(没接任务也发送接任务请求)
		nTaskId			= nTaskId,
		nReferId		= nReferId,
		nAskDate		= KGameBase.GetTime(),
		nSharedPlayerId	= 0;   -- TODO: Forced 0
	};
	--Task:OnAccept(nTaskId, nReferId, 1);
	TaskAct:TalkInDark(szMsg,Task.OnAccept, Task, nTaskId, nReferId, 1);
end

-- 询问接收任务
function TaskAct:AskAccept(nReferId)
	-- 获得这个引用任务所关联的任务
	local tbReferData = Task.tbReferDatas[nReferId];
	if (not tbReferData) then
		print("玩家"..me.szName.."准备接的任务："..nReferId.."不存在！");
		print(debug.traceback())
		return;
	end
	
	local nTaskId = tbReferData.nTaskId;
	
	local tbSubData	= Task.tbSubDatas[tbReferData.nSubTaskId];
	local szMsg = "";
	if (tbSubData.tbAttribute.tbDialog.Start.szMsg) then -- 未分步骤
			szMsg = tbSubData.tbAttribute.tbDialog.Start.szMsg;
	else
			szMsg = tbSubData.tbAttribute.tbDialog.Start.tbSetpMsg[1];
	end
	print("玩家"..me.szName.."准备接的任务ID："..nTaskId .. "referId" .. nReferId);
	TaskAct:TalkInDark(szMsg,Task.AskAccept, Task, nTaskId, nReferId);
end

-- 增加经验
function TaskAct:AddExp(nExp, nTaskId)
  local nExpPercent = nExp;
	if nExpPercent > 1 then
	  nExpPercent = 1
	end
	local tbTaskData = Task.tbTaskDatas[nTaskId]
	local tbAward = KAward.GetAssetAwardData("levelexp", tbTaskData.nLevel + 1); -- 得到角色在任务等级时的升级经验
	local nExp = tbAward.nExpAwardId * nExpPercent
	me.AddExp(nExp);
end;

-- 增加金钱
function TaskAct:AddMoney(nMoney, nTaskId)
	--me.AddBindMoney2(nMoney,"task",Player.emKBINDMONEY_ADD_TASK_ACT);
	
	Task:TskProduceLog(nTaskId, Task.TSKPRO_LOG_TYPE_BINDMONEY, nMoney);
end;

function TaskAct:GiveActiveMoney(nMoney, szExtParam1, szExtParam2, szExtParam3, nTaskId)
	local nTaskId = tonumber(nTaskId);
	local tbPlayerTask	= Task:GetPlayerTask(me);
	local tbTask		= tbPlayerTask.tbTasks[nTaskId];
	if (tbTask) then
		--me.Earn(nMoney, Player.emKEARN_TASK_ACT);
		KStatLog.ModifyAdd("jxb", "[产出]"..Task:GetTaskTypeName(nTaskId), "总量", nMoney);
	end
	
	Task:TskProduceLog(nTaskId, Task.TSKPRO_LOG_TYPE_MONEY, nMoney);
end

function TaskAct:AddItem(tbItem)
	Task:AddItem(me, tbItem);
end

function TaskAct:AddItems(tbItem, nCount)
	if (nCount < 1) then
		return;
	end
	
	for i = 1, nCount do
			local pItem = Task:AddItem(me, tbItem, nTaskId);
			--self:WriteItemLog(pItem, me, nTaskId);
			if Task:IsAutoUse(pItem) == 1 then
				me.pItem.AutoUse(pItem);
			end
		end
end


-- 对话行为
--玩家和Npc对话
--<playername>npc你好<end>
--<npc=2>玩家你好<end>
--KGameBase.GetNameByTemplateId(nDiglogNpcTempId);
function TaskAct:Talk(AllMsg, ...)
	local szAllMsg = "";
	local nCallBackInfo = 0;
	if (type(AllMsg) == "table") then
		nCallBackInfo , szAllMsg = Lib:CallBack(AllMsg);
	elseif(type(AllMsg) == "string") then
		szAllMsg = AllMsg;
	end
	
	szAllMsg = szAllMsg or "";

	BlackSky:SimpleTalk(me, szAllMsg, ...)
end

-- 对话完不会结束黑屏
function TaskAct:TalkInDark(szAllMsg, ...)
	BlackSky:SendDarkCommand(me, {TaskAct.NormalTalk, TaskAct, me, szAllMsg, unpack(arg)})
end

function TaskAct:NormalTalk(pPlayer, szAllMsg, ...)
	local tbMsg	= Lib:SplitStr(szAllMsg or "", "<end>"); -- 为以<end>为分割点把szAllMsg分割为n段对话
	for i, szMsg in ipairs(tbMsg) do
		--print("分割的第"..i.."个为："..szMsg)
		while (self.tbTalkForbitHead[string.byte(szMsg)]) do -- 去除非法首字符
			szMsg	= string.sub(szMsg, 2);
		end;
		local nStart	= string.find(szMsg, ":");
		if (not nStart) then
			nStart	= string.find(szMsg, "：");
		end;
		if (nStart) then
			local szName	= string.sub(szMsg, 1, nStart-1);
			if (szName == "<playername>") then -- 若是玩家
				if (pPlayer.GetSex() == 1) then
					--szMsg	= ""..szMsg;
				else
					--szMsg	= ""..szMsg;
				end;
				szMsg = string.gsub(szMsg, "<playername>", "<playername="..pPlayer.szName..">", 1);
			elseif (szName == "<Playername>") then
			    szMsg = string.gsub(szMsg, "<Playername>", "<playername="..pPlayer.szName..">", 1);
			elseif (szName == "<PlayerName>") then
			    szMsg = string.gsub(szMsg, "<PlayerName>", "<playername="..pPlayer.szName..">", 1);
			else -- 若是NPC
				local _, nNpcIdStart	= string.find(szMsg, "<npc=");
				local nNpcIdEnd			= string.find(szMsg, ">");
				local nNpcTempId		= -1;
				if (nNpcIdStart and nNpcIdEnd) then
					nNpcTempId 		= tonumber(string.sub(szMsg, nNpcIdStart+1, nNpcIdEnd-1));
				end
				if (nNpcTempId and nNpcTempId >0) then
					--local szHeadPic = GetNpcHeadPic(nNpcTempId);
					--if (szHeadPic) then
					--	szHeadPic = "<Pic:"..szHeadPic..">";
					--end
					local szHeadPic = "<npcpic="..nNpcTempId..">"
					-- 根据模板获取和设置Npc肖像
					szMsg	= szHeadPic..szMsg;
				end
			end;
		end;
		-- 找到<Npc=xx>的位置
		local nCurIdx = 1;

		while true do  -- 根据npc=id 获取npc名字
			local nNpcTagStart, nNpcIdStart	= string.find(szMsg, "<npc=", string.find(szMsg, "<npc="));  -- 这里，忽略掉第一个<npc=, 从第二个开始! by KK
			local nNpcTagEnd, nNpcIdEnd			= string.find(szMsg, ">", nNpcIdStart);
			local nNpcTempId = -1;
			if (not nNpcIdStart or not nNpcIdEnd) then
				break;
			end
			local nNpcTempId 		= tonumber(string.sub(szMsg, nNpcIdStart+1, nNpcIdEnd-1));
			
			if (nNpcTempId and nNpcTempId > 0) then
				local szNpcName = KGameBase.GetNameByTemplateId(nNpcTempId);
				local nTagIndex = string.find(szNpcName,"_")
				if (nTagIndex) then
					szNpcName = string.sub(szNpcName, 1, nTagIndex-1);
				end
				szMsg = Lib:ReplaceStrFormIndex(szMsg, nNpcTagStart, nNpcTagEnd, "<npcname="..szNpcName..">");
			end
			nCurIdx = nNpcTagStart + 1; --不能是nNpcIdEnd + 1,因为字符串被替换了 
		end
		--print("tbMsg[i]",tbMsg[i])
		tbMsg[i]	= Lib:ReplaceStr(szMsg, "<playername>", pPlayer.szName);  -- 颜色设置在客户端
		tbMsg[i]	= Lib:ReplaceStr(tbMsg[i], "<Playername>", pPlayer.szName);
		tbMsg[i]	= Lib:ReplaceStr(tbMsg[i],"<PlayerName>", pPlayer.szName);
	end;

	if (tbMsg[#tbMsg] == "") then
		tbMsg[#tbMsg]	= nil;
	end;
	
	for i = 1, #tbMsg do
		tbMsg[i] = Lib:ParseExpression(tbMsg[i]);
	end
	
	me = pPlayer;
	if (tbMsg[1]) then
		Dialog:Talk(pPlayer, tbMsg, unpack(arg));
	elseif (arg[1]) then
		arg[1](unpack(arg,2));
	end;
end;

-- 增加Obj
function TaskAct:AddObj(nNpcTempId, szNewName)
	local tbPos	= me.GetPosition();
	local him	= KGameBase.CallNpc(nNpcTempId, tbPos.dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ);
	--if (szNewName) then
	--	him.szName	= szNewName;
	--end
	
	return him;
end;

function TaskAct:TeleportTo(dwSceneId, nPosX, nPosY, nPosZ)
	-- 传送玩家
	me.TeleportTo(dwSceneId, nPosX, nPosY, nPosZ);
end;

-- TODO: liuchang 不安全，以后战斗状态转换应该写在地图OnEnter里面。
function TaskAct:TeleportToWithState(dwSceneId, nPosX, nPosY, nPosZ, nFightState)
	me.TeleportTo(dwSceneId, nPosX, nPosY, nPosZ);
	me.SetFightState(tonumber(nFightState));
end;

-- 释放技能
function TaskAct:DoSkill(nSkillId)
	--CastSkill(nSkillId);
end;

-- 设置任务变量
function TaskAct:SetTaskValue(nGroupId, nTaskId, nValue)
	me.pTask.SetTask(nGroupId, nTaskId, nValue, 1);
end

function TaskAct:AddATaskValue(nGroupId, nTaskId, nValue)
	local nOldValue = me.pTask.GetTask(nGroupId, nTaskId);
	me.pTask.SetTask(nGroupId, nTaskId, (nOldValue + nValue));
end

function TaskAct:AddTaskValue(nGroupId, nTaskId, nValue)
	local nOldValue = me.pTask.GetTask(nGroupId, nTaskId);
	me.pTask.SetTask(nGroupId, nTaskId, (nOldValue + nValue));
end

function TaskAct:SetPlayerLife(nLifeValue)
	me.SetCurrentLife(nLifeValue);
end

function TaskAct:DelItem(tbTaskItemId, nDelCount)
	local tbItemId	= {20,1,tbTaskItemId[1],1,0,0};
	
	if (nDelCount <= 0) then
		nDelCount = Task:GetItemCount(me, tbItemId);
	end
	
	Task:DelItem(me, tbItemId, nDelCount);
end

function TaskAct:AddCustomizeItem(nGenre, nDetailType, nParticularType, nCount)
	if (nCount < 1) then
		return;
	end
	
	for nCount = 1, nCount do
		Task:AddItem(me, {nGenre, nDetailType, nParticularType});
	end
end

function TaskAct:AddRecip(nRecipId)
	if (nRecipId <= 0) then
		return;
	end
	
	--LifeSkill:AddRecipe(me, nRecipId)
end

function TaskAct:AddLifeSkill(nSkillId, nSkillLevel)
	if (nSkillId <= 0 or nSkillLevel <= 0) then
		return;
	end
	
	--LifeSkill:AddLifeSkill(me, nSkillId, nSkillLevel);
end

function TaskAct:StepOverEvent(szAllMsg)
	if (MODULE_GAMESERVER) then
		-- todo simon
		--me.CallClientScript({"Ui:ServerCall", "UI_TASKTIPS", "Begin", szAllMsg})
		me.SendBlackMsg(szAllMsg);
	end
end


function TaskAct:StepOverWithTalk(szMsg1, szMsg2)
	TaskAct:Talk(szMsg1, self.StepOverEvent, self, szMsg2);
end

function TaskAct:SetTaskValueOnStart(nTaskGroup, nTaskId, nTaskValue)
	me.pTask.SetTask(nTaskGroup, nTaskId, nTaskValue, 1);
end

function TaskAct:AddTaskValueOnStart(nGroupId, nTaskId, nValue)
	local nOldValue = me.pTask.GetTask(nGroupId, nTaskId);
	me.pTask.SetTask(nGroupId, nTaskId, (nOldValue + nValue));
end

function TaskAct:SetTaskValueOnFailed(nTaskGroup, nTaskId, nTaskValue)
	me.pTask.SetTask(nTaskGroup, nTaskId, nTaskValue, 1);
end

function TaskAct:SetTaskValueOnFinish(nTaskGroup, nTaskId, nTaskValue)
  print("SetTaskValueOnFinish", nTaskGroup, nTaskId)
	me.pTask.SetTask(nTaskGroup, nTaskId, nTaskValue, 1);
end

function TaskAct:DelItemOnFailed(tbTaskItemId, nDelCount)
	local tbItemId	= {20,1,tbTaskItemId[1],1,0,0};
	
	if (nDelCount <= 0) then
		nDelCount = Task:GetItemCount(me, tbItemId);
	end
	
	Task:DelItem(me, tbItemId, nDelCount);
end

function TaskAct:DelTitleOnFailed(nTitleGenre, nTitleDetailType, nTitleLevel)
	assert(nTitleGenre > 0);
	assert(nTitleDetailType > 0);
	assert(nTitleLevel > 0);

-- todo simon	
	--me.RemoveTitle(nTitleGenre, nTitleDetailType, nTitleLevel);
end


function TaskAct:LearnFightSkill(nSkillId, nSkillLevel)
	--if (me.GetSkillLevel(nSkillId) > nSkillLevel) then
	--	return;
	--end
	
	--me.AddFightSkill(nSkillId, nSkillLevel);
end

function TaskAct:DelItemOnAccept(tbTaskItemId, nDelCount, szExtParam1, szExtParam2, szExtParam3)
	local tbItemId	= {20,1,tbTaskItemId[1],1,0,0};
	
	if (nDelCount <= 0) then
		nDelCount = Task:GetItemCount(me, tbItemId);
	end
	
	Task:DelItem(me, tbItemId, nDelCount);
end

function TaskAct:SetTaskValueBitOnStart(nTaskGroup, nTaskId, nBitNum, bBitValue, szExtParam1, szExtParam2, szExtParam3)
	local nTaskValue = me.pTask.GetTask(nTaskGroup, nTaskId);
	assert(nBitNum >= 1 and nBitNum <= 16);
	local nBitValue = 0;
	if (bBitValue) then
		nBitValue = 1;
	end
	nTaskValue = KLib.SetBit(nTaskValue, nBitNum, nBitValue);
	me.pTask.SetTask(nTaskGroup, nTaskId, nTaskValue);
end
