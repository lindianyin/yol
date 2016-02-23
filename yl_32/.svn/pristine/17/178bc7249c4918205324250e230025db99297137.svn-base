
Task.szTaskFilePath	= "setting/task_publish"
Task.szTaskLevelRangeInfo = "setting/task/task_levelrange_info.txt"
Task.szGenieAwardTypeInfo = "setting/task/genieawardtype.txt";


if (not Task.tbSubDatas) then
	Task.tbSubDatas		= {};
	Task.tbTaskDatas	= {};  -- 所有读取的xml任务任务
	Task.tbReferDatas	= {};
	Task.tbLevelRangeInfo = {};
	Task.tbToBeDelNpc  	= {};
	Task.tbTaskTypes	= {};
	Task.tbGenieAwardType = {};
end;

local tbGenieAwardFile =  Lib:LoadTabFile(Task.szGenieAwardTypeInfo, {["Index"] = 1});
for _, Row in ipairs(tbGenieAwardFile) do
	Task.tbGenieAwardType[Row.Index] = Row.szAwardType;
end

function Task:LoadLevelRangeInfo()
	local tbNumColName = {["level_range_min"] = 1, ["level_range_max"] = 1};
	self.tbLevelRangeInfo = Lib:LoadTabFile(self.szTaskLevelRangeInfo, tbNumColName);
end
	
-- 读取全部参数
function Task:ReadParams(tbXml)
	--Lib:ShowTB(tbXml,"")
	local tbParams	= {};
	for _, tb in ipairs(tbXml.children) do
		local szType	= tb.name;--<number>
		local szValue;
		for _, tb in ipairs(tb.children) do--这里不会有多个childern吧
			if (tb.name == "Value") then
				if (szType == "number" or szType == "faction" or szType == "bool"
					or szType == "itemid" or szType == "taskitem" or szType == "award") then
					szValue	= tb.value;
				elseif (szType == "dialognpc" or szType == "fightnpc") then
					szValue	= tostring(tb.value);    -- 这里可能会有特殊的参数来衡定npc所在位置场景~ kk
					local tbNpcInfo = Lib:SplitStr(szValue)  -- 逗号隔开，取第一个值即为npc Id
					szValue = tbNpcInfo[1]  -- index start from 1
				elseif (szType == "text") then
					local nTextId	= tonumber(tb.attrib.id, 16);
					szValue	= self.tbTextDatas[nTextId];
				elseif (szType == "taskid" or szType == "referid") then
					szValue = tonumber(tb.value, 16);
				else
					szValue	= Lib:StrVal2Str(tb.value);
				end;
			elseif (tb.name == "Script") then
				szValue	= "loadstring("..Lib:StrVal2Str(tb.value)..")()";
			elseif (tb.name == "Call") then
				szValue	= self:ReadFunction(tb);
			end;
		end;
		tbParams[#tbParams + 1]	= szValue;
	end;
	return tbParams;
end;

-- 读取一个函数及其参数
function Task:ReadFunction(tbXml)
	local szFuncName	= "";
	local szFuncParam	= "";
	for _, tb in ipairs(tbXml.children) do
		if (tb.name == "Function") then
			szFuncName	= tb.value;
		elseif (tb.name == "Parameter") then
			szFuncParam	= table.concat(self:ReadParams(tb), ",");
		end;
	end;
	return szFuncName.."("..szFuncParam..")";
end;

-- 读取一组函数
function Task:ReadFunctions(tbXml)
	if (not tbXml) then
		return nil;
	end;
	local tbFunctions	= {};
	for _, tb in ipairs(tbXml.children) do
		if (tb.name == "Grid") then
			tbFunctions[#tbFunctions+1]	= "return "..self:ReadFunction(tb);
		end;
	end;
	return tbFunctions;
end;

function Task:LoadTask2(nTaskId)
	local tbXml	= QFile.LoadXmlFile(string.format("%s/%016X.xml", self.szTaskFilePath, nTaskId));
	if (not tbXml) then
		return;
	end;
	
	local tbTaskData	= {};
	tbTaskData.nId		= nTaskId	--tonumber(tbXml.attrib.id)
	tbTaskData.szName	= "["..tbXml.attrib.name.."]";

	local tbReferIds	= {};
	tbTaskData.tbReferIds	= tbReferIds;
	local nReferId		= nTaskId; -- 引用子任务Id
	local nReferIdx		= #tbReferIds + 1;	-- 引用子任务索引
	local tbReferData	= {};
	assert(not self.tbReferDatas[nReferId]);
	self.tbReferDatas[nReferId]	= tbReferData;
	tbReferIds[nReferIdx]		= nReferId;
	tbReferData.nReferId		= nReferId;
	tbReferData.nReferIdx		= nReferIdx;
	tbReferData.nTaskId			= nTaskId;
	tbReferData.nSubTaskId		= nTaskId;
	tbReferData.szName			= tbXml.attrib.name;
	
	for _, tb in ipairs(tbXml.children) do
		if (tb.name == "Attribute") then
			-- Xml:	Task\Attribute
			local tbAttribute	= {};
			tbTaskData.tbAttribute	= tbAttribute;
			for _, tb in ipairs(tb.children) do
				print("..",tb.name);
				tbAttribute[tb.name]	= Lib:Str2Val(tb.value);
			end;
			
			tbReferData.szMainDesc = tbAttribute["TaskDesc"]
			tbReferData.tbStepsDesc = {}
			
			tbReferData.nAcceptNpcId	= tbAttribute["StartNPC"]
			if (tbAttribute["StartItem"]) then
				tbReferData.nParticular = tbAttribute["StartItem"]
				if (tbReferData.nAcceptNpcId > 0 and tbReferData.nParticular) then -- 不能同时物品和Npc都可接
					print("[Task:Warning] 物品和Npc可同时触发:"..tbReferData.szName);
				end
			end
			tbReferData.bCanGiveUp	= tbAttribute["CanGiveUp"]
			tbReferData.nReplyNpcId	= tbAttribute["EndNPC"]
			tbReferData.szReplyDesc	= tbAttribute["EndDesc"]
			tbReferData.nBagSpaceCount = tbAttribute["Bags"]
			tbReferData.nLevel = tbAttribute["Level"]
			tbTaskData.nLevel = tbAttribute["Level"]
					
			local szDefaultInfo = "";
			if (tbReferData.nAcceptNpcId > 0) then
				local szNpcName = KGameBase.GetNameByTemplateId(tbReferData.nAcceptNpcId)
				if (not szNpcName) then
					print("LoadTask", tbTaskData.szName, "not found AcceptNpc", tbReferData.nAcceptNpcId)
				end
						
				if (szNpcName) then
					szDefaultInfo = string.format(Lang.task.str47[Lang.Idx],szNpcName)
				end
			elseif (tbReferData.nParticular and tbReferData.nParticular > 0) then
				local szItemName = KItem.GetNameById(20, 1, tbReferData.nParticular, 1)
				if (szItemName) then
					szDefaultInfo = string.format(Lang.task.str48[Lang.Idx],szItemName)
				end
			end

			tbReferData.szIntrDesc = tbAttribute["StartDesc"]
			tbReferData.nDegree	= tbAttribute["Difficulty"]
			tbReferData.nShareKillNpc = tbAttribute["ShareKillNpc"]
		elseif (tb.name == "Condition") then
			tbReferData.tbVisable	= Task:ReadFunctions(tb);	-- 读取可见条件
			tbReferData.tbAccept	= Task:ReadFunctions(tb); -- 读取可接条件
		elseif (tb.name == "Award") then
			tbReferData.tbAwards	= {
				tbFix	= Lib:Str2Val(tb.children[1].value) or {},
				tbOpt	= Lib:Str2Val(tb.children[2].value) or {},
				tbRand	= Lib:Str2Val(tb.children[3].value) or {},
			};
			
			if (tbReferData.tbAwards.tbFix) then
				for _, tbFix in ipairs(tbReferData.tbAwards.tbFix) do
					if (tbFix.szType == "genieaward") then
						tbFix.szType = self:GetAwardTypeFromIndex(tbFix.varValue[1]);
						if (tbFix.szType == "bindmoney" or tbFix.szType == "activemoney") then
							tbFix.varValue = tbFix.varValue[2];
						else
							tbFix.varValue = {unpack(tbFix.varValue, 2)}
						end
					end
				end
			end
			
			if (tbReferData.tbAwards.tbOpt) then
				for _, tbOpt in ipairs(tbReferData.tbAwards.tbOpt) do
					if (tbOpt.szType == "genieaward") then
						tbOpt.szType = self:GetAwardTypeFromIndex(tbOpt.varValue[1]);
						if (tbOpt.szType == "bindmoney" or tbOpt.szType == "activemoney") then
							tbOpt.varValue = tbOpt.varValue[2];
						else
							tbOpt.varValue = {unpack(tbOpt.varValue, 2)}
						end
					end
				end
			end
					
			if (tbReferData.tbAwards.tbRand) then
				for _, tbRand in ipairs(tbReferData.tbAwards.tbRand) do
					if (tbRand.szType == "genieaward") then
						tbRand.szType = self:GetAwardTypeFromIndex(tbRand.varValue[1]);
						if (tbRand.szType == "bindmoney" or tbRand.szType == "activemoney") then
							tbRand.varValue = tbRand.varValue[2];
						else
							tbRand.varValue = {unpack(tbRand.varValue, 2)}
						end
					end
				end
			end
					
			if (tbReferData.tbAwards.tbFix) then
				for _, tbFix in ipairs(tbReferData.tbAwards.tbFix) do	
					if (tbFix.szCondition1 and tbFix.szCondition2 and tbFix.szCondition3) then
						local tbCondition = {};
						table.insert(tbCondition, "return "..tbFix.szCondition1.."()");
						table.insert(tbCondition, "return "..tbFix.szCondition2.."()");
						table.insert(tbCondition, "return "..tbFix.szCondition3.."()");
						tbFix.tbConditions = tbCondition;		
					end
				end
			end
					
			if (tbReferData.tbAwards.tbOpt) then
				for _, tbOpt in ipairs(tbReferData.tbAwards.tbOpt) do
					if (tbOpt.szCondition1 and tbOpt.szCondition2 and tbOpt.szCondition3) then
						local tbCondition = {};
						table.insert(tbCondition, "return "..tbOpt.szCondition1.."()");
						table.insert(tbCondition, "return "..tbOpt.szCondition2.."()");
						table.insert(tbCondition, "return "..tbOpt.szCondition3.."()");
						tbOpt.tbConditions = tbCondition;
					end
				end
			end
					
			if (tbReferData.tbAwards.tbRand) then
				for _, tbRand in ipairs(tbReferData.tbAwards.tbRand) do
					if (tbRand.szCondition1 and tbRand.szCondition2 and tbRand.szCondition3) then
						local tbCondition = {};
						table.insert(tbCondition, "return "..tbRand.szCondition1.."()");
						table.insert(tbCondition, "return "..tbRand.szCondition2.."()");
						table.insert(tbCondition, "return "..tbRand.szCondition3.."()");
						tbRand.tbConditions = tbCondition;
					end
				end
			end
		else
			print(".." .. tb.name);
			--assert(false);
		end 
	end
	self.tbTaskDatas[nTaskId]	= tbTaskData;
	self:LoadSub2(nTaskId, tbXml)
	return tbTaskData;
end;

function Task:LoadSub2(nSubTaskId, tbXml)
	local tbSubData		= {};
	tbSubData.nId		= nSubTaskId	--tonumber(tbXml.attrib.id, 16)
	tbSubData.szName	= "["..tbXml.attrib.name.."]";
	--tbSubData.szDesc	= Lib:StrTrim(tbXml.attrib.describe, "\n");
	
	tbSubData.tbSteps	= {};
	tbSubData.tbExecute = {};
	tbSubData.tbStartExecute = {};
	tbSubData.tbFailedExecute = {};
	tbSubData.tbFinishExecute = {};
	for _, tb in ipairs(tbXml.children) do
		if (tb.name == "Attribute") then
			-- Xml:	Sub\Attribute
			local tbAttribute	= {};
			tbSubData.tbAttribute	= tbAttribute;
			local tbDialog	= {};
			tbAttribute.tbDialog = tbDialog
			tbAttribute.tbDialog["Pop"] = { szMsg = "" }
			tbAttribute.tbDialog["Procedure"] = { szMsg = "" }
			tbAttribute.tbDialog["Error"] = { szMsg = "" }
			tbAttribute.tbDialog["Prize"] = { szMsg = "" }
			tbAttribute.tbDialog["End"] = { szMsg = "" }
			
			for _, tb in ipairs(tb.children) do
				if (tb.name == "StartDialog") then
					-- Xml:	Sub\Attribute\Dialog
					local szDialog = Lib:Str2Val(tb.value);
					tbDialog["Start"] = self:ParseDialog(szDialog)
				elseif (tb.name == "TaskDesc") then
					tbSubData.szDesc = tb.value
				end
			end;
		elseif (tb.name == "Step") then
			-- Xml:	Procedures
			local tbStep	= {};
			table.insert(tbSubData.tbSteps, tbStep);
			for _, tb in ipairs(tb.children) do
				if (tb.name == "Event") then
					-- Xml:	Procedures\Step\Event\
					local tbEvent	= {};
					tbStep.tbEvent	= tbEvent;
					tbEvent.nType	= tonumber(tb.attrib.type);
					if (tbEvent.nType == 1) then	-- NPC
						tbEvent.nValue	= tonumber(tb.value)
					else	-- Item
						tbEvent.nValue	= tonumber(tb.value);
					end
				elseif (tb.name == "Desc") then
					local tbStepsDesc = self.tbReferDatas[nSubTaskId].tbStepsDesc
					tbStepsDesc[#tbStepsDesc + 1] = Lib:Str2Val(tb.value)
				elseif (tb.name == "Target") then
					-- Xml:	Procedures\Step\Target
					local tbTargets		= {};
					tbStep.tbTargets	= tbTargets;

						local nNewWorldFlag = 0; -- 传送目标只能第一个有效
						for _, tb in ipairs(tb.children) do
							if (tb.name == "Grid") then
								-- Xml:	Procedures\Step\Target\Grid
								local szTargetName;
								local tbParams;
								local bAvailably = 1;
								for _, tb in ipairs(tb.children) do
									if (tb.name == "Function") then
										szTargetName	= tb.value;
										if (szTargetName == "Send2NewWorld") then
											if (nNewWorldFlag == 0) then
												nNewWorldFlag = 1;
											else
												print("此步骤已经有传送目标，其它传送目标被乎略！")
												bAvailably = 0;
												break;
											end
										end
									elseif (tb.name == "Parameter") then
										tbParams	= self:ReadParams(tb);
										for i = 1, #tbParams do
											tbParams[i]	= Lib:Str2Val(tbParams[i]);
										end;
									end;
								end;
								if (bAvailably ~= 0) then
									local tbTagLib	= self.tbTargetLib[szTargetName];
									assert(tbTagLib, "Target["..szTargetName.."] not found!!!");
									local tbTarget	= Lib:NewClass(tbTagLib);--根据函数名new目标
									tbTarget:Init(unpack(tbParams));--从子任务文件把目标数据读入
									tbTargets[#tbTargets+1]	= tbTarget;
								end;
							end;
						end;

				elseif (tb.name == "Judge") then
					-- Xml:	Procedures\Step\Judge\Condition
					tbStep.tbJudge	= Task:ReadFunctions(tb.children[1]);
				elseif (tb.name == "Execute") then
					-- Xml:	Procedures\Step\Execute
					tbStep.tbExecute	= Task:ReadFunctions(tb);
					-- 对一些行为进行特殊处理，他们应该属于子任务而不属于步骤
					local tbNewExecute = {};
					tbStep.szAwardDesc = "";
					for _,szExecute in pairs(tbStep.tbExecute) do
						local szExecuteDesc = "";
						if (type(szExecute) ~= "string") then
							assert(false);
						end
						
						
						if (string.find(szExecute, "TaskAct:AddExp")) then
							local nExp = loadstring(string.gsub(szExecute,"return TaskAct:AddExp(.+)", "return tonumber(%1)"))()
							szExecuteDesc = Lang.task.str49[Lang.Idx].."："..tostring(nExp).."\n";
						elseif (string.find(szExecute, "TaskAct:AddItems")) then
							local tbItem, nCount = loadstring(string.gsub(szExecute, "return TaskAct:AddItems%((.+)%)", "return %1"))()
							szExecuteDesc = Lang.task.str50[Lang.Idx].."："..(KItem.GetNameById(tbItem[1], tbItem[2], tbItem[3], tbItem[4]).." X "..tostring(nCount) or "");	
						elseif (string.find(szExecute, "TaskAct:AddItem")) then
							local tbItem = loadstring(string.gsub(szExecute, "return TaskAct:AddItem(.+)", "return %1"))()
							szExecuteDesc = Lang.task.str51[Lang.Idx].."："..(KItem.GetNameById(tbItem[1], tbItem[2], tbItem[3], tbItem[4]) or "");			
						elseif(string.find(szExecute, "TaskAct:AddMoney")) then
							local nMoney = loadstring(string.gsub(szExecute,"return TaskAct:AddMoney(.+)", "return tonumber(%1)"))()
							szExecuteDesc = Lang.task.str52[Lang.Idx].."："..tostring(nMoney).."\n";
						elseif(string.find(szExecute, "TaskAct:GiveActiveMoney")) then
							local nMoney = loadstring(string.gsub(szExecute,"return TaskAct:GiveActiveMoney%((.+)%)", "return %1"))()
							szExecuteDesc = Lang.task.str53[Lang.Idx].."："..tostring(nMoney).."\n";
						end
						
						if (szExecuteDesc and szExecuteDesc ~= "") then
							tbStep.szAwardDesc = tbStep.szAwardDesc.."  "..szExecuteDesc;
						end
						
						if (string.find(szExecute,"TaskAct:AskAccept") or 
							string.find(szExecute,"TaskAct:DoAccept")) then  -- 放到下一个任务起始执行
							tbSubData.tbExecute[#tbSubData.tbExecute+1] = szExecute;
						elseif (string.find(szExecute, "TaskAct:SetTaskValueOnStart") or 
								string.find(szExecute, "TaskAct:DelItemOnAccept") or 
								string.find(szExecute, "TaskAct:SetTaskValueBitOnStart") or
								string.find(szExecute, "TaskAct:AddTaskValueOnStart")) then
							tbSubData.tbStartExecute[#tbSubData.tbStartExecute + 1] = szExecute;
						elseif (string.find(szExecute, "TaskAct:SetTaskValueOnFailed") or 
								string.find(szExecute, "TaskAct:DelItemOnFailed") or 
								string.find(szExecute, "TaskAct:DelTitleOnFailed")
								) then
							tbSubData.tbFailedExecute[#tbSubData.tbFailedExecute + 1] = szExecute;
						elseif (string.find(szExecute, "TaskAct:SetTaskValueOnFinish")) then
							tbSubData.tbFinishExecute[#tbSubData.tbFinishExecute + 1] = szExecute;
						else
							tbNewExecute[#tbNewExecute+1] = szExecute; 
						end
					end
					
					tbStep.tbExecute = tbNewExecute;
				end;
			end;
		end;
	end;
	
	self.tbSubDatas[nSubTaskId]	= tbSubData;
	return tbSubData;
end;

function Task:LoadTask(nTaskId)
	local tbXml	= QFile.LoadXmlFile(string.format("%s/task/%016X.xml", self.szTaskFilePath, nTaskId));
	if (not tbXml) then
		return;
	end;
	
	local tbTaskData	= {};
	tbTaskData.nId		= nTaskId	--tonumber(tbXml.attrib.id)
	tbTaskData.szName	= "["..tbXml.attrib.name.."]";
	tbTaskData.szDesc	= Lib:StrTrim(tbXml.attrib.describe, "\n");
	
	for _, tb in ipairs(tbXml.children) do
		if (tb.name == "Attribute") then
			-- Xml:	Task\Attribute
			local tbAttribute	= {};
			tbTaskData.tbAttribute	= tbAttribute;
			for _, tb in ipairs(tb.children) do
				tbAttribute[tb.name]	= Lib:Str2Val(tb.value);
			end;
		elseif (tb.name == "Managed") then
			-- Xml:	Task\Managed
			local tbReferIds	= {};
			tbTaskData.tbReferIds	= tbReferIds;
			for _, tb in ipairs(tb.children) do
				if (tb.name == "Sub") then
					-- Xml:	Task\Managed\Sub
					local nReferId		= tonumber(tb.attrib.id, 16); -- 引用子任务Id
					local nReferIdx		= #tbReferIds + 1;	-- 引用子任务索引
					local tbReferData	= {};
					assert(not self.tbReferDatas[nReferId]);
					self.tbReferDatas[nReferId]	= tbReferData;
					tbReferIds[nReferIdx]		= nReferId;
					tbReferData.nReferId		= nReferId;
					tbReferData.nReferIdx		= nReferIdx;
					tbReferData.nTaskId			= nTaskId;
					tbReferData.nSubTaskId		= tonumber(tb.attrib.refer, 16);
					tbReferData.szName			= tb.attrib.name;
					
					local tbDesc				= self:ParseRefSubDesc(tb.attrib.describe);
					if tbDesc then
						tbReferData.szMainDesc		= tbDesc.szMainDesc;
						tbReferData.tbStepsDesc		= tbDesc.tbStepsDesc;
					end
					
					-- Xml:	Task\Managed\Sub\Attribute
					local tbAttrib	= tb.children[1];
					tbReferData.tbVisable	= Task:ReadFunctions(tbAttrib.children[2]);	-- 读取可见条件
					tbReferData.tbAccept	= Task:ReadFunctions(tbAttrib.children[3]); -- 读取可接条件
					-- Xml:	Task\Managed\Sub\Attribute\Parameter
					local tbParams	= self:ReadParams(tbAttrib.children[1]); -- 读取参数
					tbReferData.nAcceptNpcId	= Lib:Str2Val(tbParams[1]);
					
					if (tbParams[2] ~= "") then
						--local tbAcceptItemId	= Lib:Str2Val(tbParams[2]);
						--if (tbAcceptItemId[1] == 6 and tbAcceptItemId[2] == 1) then
						--	tbReferData.nParticular	= tbAcceptItemId[3];
						--end;
						local tbAcceptItemId = Lib:Str2Val(tbParams[2]);
						tbReferData.nParticular = tbAcceptItemId[3];
						if (tbReferData.nAcceptNpcId > 0 and tbReferData.nParticular) then -- 不能同时物品和Npc都可接
							print("[Task:Warning] 物品和Npc可同时触发:"..tbReferData.szName);
						end
					end;
					if (tbParams[3] == "") then
						tbParams[3] = "{}";
					end
					if (tbParams[4] == "") then
						tbParams[4] = "{}";
					end
					if (tbParams[5] == "") then
						tbParams[5] = "{}";
					end
					tbReferData.tbAwards	= {
						tbFix	= Lib:Str2Val(tbParams[3]),
						tbOpt	= Lib:Str2Val(tbParams[4]),
						tbRand	= Lib:Str2Val(tbParams[5]),
					};
					
					if (tbReferData.tbAwards.tbFix) then
						for _, tbFix in ipairs(tbReferData.tbAwards.tbFix) do
							if (tbFix.szType == "genieaward") then
								tbFix.szType = self:GetAwardTypeFromIndex(tbFix.varValue[1]);
								if (tbFix.szType == "bindmoney" or tbFix.szType == "activemoney") then
									tbFix.varValue = tbFix.varValue[2];
								else
									tbFix.varValue = {unpack(tbFix.varValue, 2)}
								end
							end
						end
					end
					
					if (tbReferData.tbAwards.tbOpt) then
						for _, tbOpt in ipairs(tbReferData.tbAwards.tbOpt) do
							if (tbOpt.szType == "genieaward") then
								tbOpt.szType = self:GetAwardTypeFromIndex(tbOpt.varValue[1]);
								if (tbOpt.szType == "bindmoney" or tbOpt.szType == "activemoney") then
									tbOpt.varValue = tbOpt.varValue[2];
								else
									tbOpt.varValue = {unpack(tbOpt.varValue, 2)}
								end
							end
						end
					end
					
					if (tbReferData.tbAwards.tbRand) then
						for _, tbRand in ipairs(tbReferData.tbAwards.tbRand) do
							if (tbRand.szType == "genieaward") then
								tbRand.szType = self:GetAwardTypeFromIndex(tbRand.varValue[1]);
								if (tbRand.szType == "bindmoney" or tbRand.szType == "activemoney") then
									tbRand.varValue = tbRand.varValue[2];
								else
									tbRand.varValue = {unpack(tbRand.varValue, 2)}
								end
							end
						end
					end
					
					if (tbReferData.tbAwards.tbFix) then
						for _, tbFix in ipairs(tbReferData.tbAwards.tbFix) do	
							if (tbFix.szCondition1 and tbFix.szCondition2 and tbFix.szCondition3) then
								local tbCondition = {};
								table.insert(tbCondition, "return "..tbFix.szCondition1.."()");
								table.insert(tbCondition, "return "..tbFix.szCondition2.."()");
								table.insert(tbCondition, "return "..tbFix.szCondition3.."()");
								tbFix.tbConditions = tbCondition;		
							end
						end
					end
					
					if (tbReferData.tbAwards.tbOpt) then
						for _, tbOpt in ipairs(tbReferData.tbAwards.tbOpt) do
							if (tbOpt.szCondition1 and tbOpt.szCondition2 and tbOpt.szCondition3) then
								local tbCondition = {};
								table.insert(tbCondition, "return "..tbOpt.szCondition1.."()");
								table.insert(tbCondition, "return "..tbOpt.szCondition2.."()");
								table.insert(tbCondition, "return "..tbOpt.szCondition3.."()");
								tbOpt.tbConditions = tbCondition;
							end
						end
					end
					
					if (tbReferData.tbAwards.tbRand) then
						for _, tbRand in ipairs(tbReferData.tbAwards.tbRand) do
							if (tbRand.szCondition1 and tbRand.szCondition2 and tbRand.szCondition3) then
								local tbCondition = {};
								table.insert(tbCondition, "return "..tbRand.szCondition1.."()");
								table.insert(tbCondition, "return "..tbRand.szCondition2.."()");
								table.insert(tbCondition, "return "..tbRand.szCondition3.."()");
								tbRand.tbConditions = tbCondition;
							end
						end
					end
					
					--tbReferData.bCanShare	= Lib:Str2Val(tbParams[6]);
					tbReferData.bCanGiveUp	= Lib:Str2Val(tbParams[6]);
					--[[tbReferData.tbVisable	= {
						nLevelMin	= Lib:Str2Val(tbParams[7]),
						nLevelMax	= Lib:Str2Val(tbParams[8]),
						nFaction	= Lib:Str2Val(tbParams[9]),
					};
					]]--
					
					tbReferData.szGossip = Lib:Str2Val(tbParams[10]);
					tbReferData.nReplyNpcId	= Lib:Str2Val(tbParams[11] or 0);
					tbReferData.szReplyDesc	= Lib:Str2Val(tbParams[12] or "\"\"");
					tbReferData.nBagSpaceCount = Lib:Str2Val(tbParams[13] or 0);
					tbReferData.nLevel = Lib:Str2Val(tbParams[14] or 1);
					local szDefaultInfo = "";
					if (tbReferData.nAcceptNpcId > 0) then
						local szNpcName = KGameBase.GetNameByTemplateId(tbReferData.nAcceptNpcId)
						if (not szNpcName) then
							print("LoadTask", tbTaskData.szName, "not found AcceptNpc", tbReferData.nAcceptNpcId)
						end
						
						if (szNpcName) then
							szDefaultInfo = string.format(Lang.task.str54[Lang.Idx],szNpcName);
						end
					elseif (tbReferData.nParticular and tbReferData.nParticular > 0) then
						local szItemName = KItem.GetNameById(20, 1, tbReferData.nParticular, 1);
						if (szItemName) then
							szDefaultInfo = string.format(Lang.task.str55[Lang.Idx],szItemName);
						end
					end
					
					
					tbReferData.szIntrDesc = Lib:Str2Val(tbParams[15] or "\""..szDefaultInfo.."\"");
					tbReferData.nDegree	= Lib:Str2Val(tbParams[16] or 1);
					tbReferData.nShareKillNpc = Lib:Str2Val(tbParams[17] or 0);
				end;
			end;
		end;
	end;
	self.tbTaskDatas[nTaskId]	= tbTaskData;
	return tbTaskData;
end;

function Task:LoadSub(nSubTaskId)
	local tbXml	= QFile.LoadXmlFile(string.format("%s/sub/%016X.xml", self.szTaskFilePath, nSubTaskId));
	if (not tbXml) then
		return nil;
	end;
	
	local tbSubData		= {};
	tbSubData.nId		= nSubTaskId	--tonumber(tbXml.attrib.id, 16)
	tbSubData.szName	= "["..tbXml.attrib.name.."]";
	tbSubData.szDesc	= Lib:StrTrim(tbXml.attrib.describe, "\n");
	
	tbSubData.tbSteps	= {};
	tbSubData.tbExecute = {};
	tbSubData.tbStartExecute = {};
	tbSubData.tbFailedExecute = {};
	tbSubData.tbFinishExecute = {};
	for _, tb in ipairs(tbXml.children) do
		if (tb.name == "Attribute") then
			-- Xml:	Sub\Attribute
			local tbAttribute	= {};
			tbSubData.tbAttribute	= tbAttribute;
			for _, tb in ipairs(tb.children) do
				if (tb.name == "Dialog") then
					-- Xml:	Sub\Attribute\Dialog
					local tbDialog	= {};
					tbAttribute.tbDialog	= tbDialog;
					for _, tb in ipairs(tb.children) do
						tbDialog[tb.name]	= self:ParseDialog(tb.value);
						if (not tbDialog[tb.name]) then
							print(tb.name,tbSubData.szName);
							assert(false);
						end
					end;
				end;
			end;
		elseif (tb.name == "Step") then
			-- Xml:	Sub\Step
			local tbStep	= {};
			table.insert(tbSubData.tbSteps, tbStep);
			for _, tb in ipairs(tb.children[1].children) do
				if (tb.name == "Event") then
					-- Xml:	Sub\Step\Process\Event\
					local tbEvent	= {};
					tbStep.tbEvent	= tbEvent;
					tbEvent.nType	= tonumber(tb.attrib.type);
					if (tbEvent.nType == 1) then	-- NPC
						tbEvent.nValue	= tonumber(tb.value)
					else	-- Item
						tbEvent.nValue	= tonumber(tb.value);
					end;
				elseif (tb.name == "Target") then
					-- Xml:	Sub\Step\Process\Target\Target
					local tbTargets		= {};
					tbStep.tbTargets	= tbTargets;
					if (tb.children[1]) then
						local nNewWorldFlag = 0; -- 传送目标只能第一个有效
						for _, tb in ipairs(tb.children[1].children) do
							if (tb.name == "Grid") then
								-- Xml:	Sub\Step\Process\Target\Target\Grid
								local szTargetName;
								local tbParams;
								local bAvailably = 1;
								for _, tb in ipairs(tb.children) do
									if (tb.name == "Function") then
										szTargetName	= tb.value;
										if (szTargetName == "Send2NewWorld") then
											if (nNewWorldFlag == 0) then
												nNewWorldFlag = 1;
											else
												print("此步骤已经有传送目标，其它传送目标被乎略！")
												bAvailably = 0;
												break;
											end
										end
									elseif (tb.name == "Parameter") then
										tbParams	= self:ReadParams(tb);
										for i = 1, #tbParams do
											tbParams[i]	= Lib:Str2Val(tbParams[i]);
										end;
									end;
								end;
								if (bAvailably ~= 0) then
									local tbTagLib	= self.tbTargetLib[szTargetName];
									assert(tbTagLib, "Target["..szTargetName.."] not found!!!");
									local tbTarget	= Lib:NewClass(tbTagLib);--根据函数名new目标
									tbTarget:Init(unpack(tbParams));--从子任务文件把目标数据读入
									tbTargets[#tbTargets+1]	= tbTarget;
								end;
							end;
						end;
					end;
				elseif (tb.name == "Judge") then
					-- Xml:	Sub\Step\Process\Judge\Condition
					tbStep.tbJudge	= Task:ReadFunctions(tb.children[1]);
				elseif (tb.name == "Execute") then
					-- Xml:	Sub\Step\Process\Execute\Action
					tbStep.tbExecute	= Task:ReadFunctions(tb.children[1]);
					-- 对一些行为进行特殊处理，他们应该属于子任务而不属于步骤
					local tbNewExecute = {};
					tbStep.szAwardDesc = "";
					for _,szExecute in pairs(tbStep.tbExecute) do
						local szExecuteDesc = "";
						if (type(szExecute) ~= "string") then
							assert(false);
						end
						
						
						if (string.find(szExecute, "TaskAct:AddExp")) then
							local nExp = loadstring(string.gsub(szExecute,"return TaskAct:AddExp(.+)", "return tonumber(%1)"))()
							szExecuteDesc = Lang.task.str49[Lang.Idx] .."："..tostring(nExp).."\n";
						elseif (string.find(szExecute, "TaskAct:AddItems")) then
							local tbItem, nCount = loadstring(string.gsub(szExecute, "return TaskAct:AddItems%((.+)%)", "return %1"))()
							szExecuteDesc = Lang.task.str50[Lang.Idx].."："..(KItem.GetNameById(tbItem[1], tbItem[2], tbItem[3], tbItem[4]).." X "..tostring(nCount) or "");	
						elseif (string.find(szExecute, "TaskAct:AddItem")) then
							local tbItem = loadstring(string.gsub(szExecute, "return TaskAct:AddItem(.+)", "return %1"))()
							szExecuteDesc = Lang.task.str51[Lang.Idx].."："..(KItem.GetNameById(tbItem[1], tbItem[2], tbItem[3], tbItem[4]) or "");			
						elseif(string.find(szExecute, "TaskAct:AddMoney")) then
							local nMoney = loadstring(string.gsub(szExecute,"return TaskAct:AddMoney(.+)", "return tonumber(%1)"))()
							szExecuteDesc = Lang.task.str52[Lang.Idx].."："..tostring(nMoney).."\n";
						elseif(string.find(szExecute, "TaskAct:GiveActiveMoney")) then
							local nMoney = loadstring(string.gsub(szExecute,"return TaskAct:GiveActiveMoney%((.+)%)", "return %1"))()
							szExecuteDesc = Lang.task.str53[Lang.Idx].."："..tostring(nMoney).."\n";
						end
						
						if (szExecuteDesc and szExecuteDesc ~= "") then
							tbStep.szAwardDesc = tbStep.szAwardDesc.."  "..szExecuteDesc;
						end
						
						if (string.find(szExecute,"TaskAct:AskAccept") or 
							string.find(szExecute,"TaskAct:DoAccept")) then   -- 放到下一个任务起始执行
							tbSubData.tbExecute[#tbSubData.tbExecute+1] = szExecute;
						elseif (string.find(szExecute, "TaskAct:SetTaskValueOnStart") or 
								string.find(szExecute, "TaskAct:DelItemOnAccept") or 
								string.find(szExecute, "TaskAct:SetTaskValueBitOnStart") or
								string.find(szExecute, "TaskAct:AddTaskValueOnStart")) then
							tbSubData.tbStartExecute[#tbSubData.tbStartExecute + 1] = szExecute;
						elseif (string.find(szExecute, "TaskAct:SetTaskValueOnFailed") or 
								string.find(szExecute, "TaskAct:DelItemOnFailed") or 
								string.find(szExecute, "TaskAct:DelTitleOnFailed")
								) then
							tbSubData.tbFailedExecute[#tbSubData.tbFailedExecute + 1] = szExecute;
						elseif (string.find(szExecute, "TaskAct:SetTaskValueOnFinish")) then
							tbSubData.tbFinishExecute[#tbSubData.tbFinishExecute + 1] = szExecute;
						else
							tbNewExecute[#tbNewExecute+1] = szExecute; 
						end
					end
					
					tbStep.tbExecute = tbNewExecute;
				end;
			end;
		end;
	end;
	
	self.tbSubDatas[nSubTaskId]	= tbSubData;
	return tbSubData;
end;

function Task:LoadText()
	local tbXml	= QFile.LoadXmlFile(string.format("%s/textlist.xml", self.szTaskFilePath));
	if (not tbXml) then
		return nil;
	end;
	
	local tbTextDatas	= {};
	self.tbTextDatas	= tbTextDatas;
	for _, tb in ipairs(tbXml.children) do
		if (tb.name == "String") then
			-- Xml:	Text\String
			local nTextId	= tonumber(tb.attrib.id, 16);
			tbTextDatas[nTextId]	= tb.value;
		end;
	end;
	return tbTextDatas;
end;

function Task:ParseRefSubDesc(strSrc)
	local tbResult = {};
	local nCurIdx = 1;
	
	-- 找第一个<step>标记
	local nFirstStepStartIdx, nFirstStepEndIdx = string.find(strSrc, "<stepdesc>", nCurIdx);
	-- 若没有<step>标记
	if (not nFirstStepStartIdx) then --没有步骤描述
		tbResult.szMainDesc = strSrc;
		return tbResult;
	end;
	tbResult.szMainDesc = string.sub(strSrc, 1, nFirstStepStartIdx-1);
	nCurIdx = nFirstStepStartIdx -1; -- 定位到第一个<step>之前
	tbResult.tbStepsDesc = {}
	while true do
		local nCurStepStartIdx, nCurStepEndIdx = string.find(strSrc, "<stepdesc>", nCurIdx);
		local nNextStepStartIdx, nNextStepEndIdx = string.find(strSrc, "<stepdesc>", nCurStepEndIdx);
		assert(nCurStepStartIdx);
		if (not nNextStepStartIdx) then -- 没有下一个<step>标记
			tbResult.tbStepsDesc[#tbResult.tbStepsDesc + 1] = string.sub(strSrc, nCurStepEndIdx+1);
			return tbResult;
		end;
		tbResult.tbStepsDesc[#tbResult.tbStepsDesc + 1] = string.sub(strSrc, nCurStepEndIdx+1, nNextStepStartIdx-1);
		nCurIdx = nCurStepEndIdx + 1;
	end;
end;

function Task:ParseDialog(szAllMsg)
	local tbAllStepMsg = Lib:SplitStr(szAllMsg, "<stepend>");
	local szStepMsg = "";
	local tbRetStepMsg = {};
	
	tbRetStepMsg.szMsg = nil; -- 若没分步骤则会填充此字段
	tbRetStepMsg.tbSetpMsg = {}; -- 若分了步骤则此表被填充
	
	if (#tbAllStepMsg == 1) then -- 没有分步骤，即所有步骤都使用同样的对话。
		tbRetStepMsg.szMsg = tbAllStepMsg[1];
	else --分有步骤，得到当前步骤对话信息
		for i = 1, #tbAllStepMsg do
			local _, nStepStart	= string.find(tbAllStepMsg[i], "<step=");
			local nStepEnd = string.find(tbAllStepMsg[i], ">", nStepStart);
			local nStep = -1;
			if (nStepStart and nStepEnd) then
				nStep = tonumber(string.sub(tbAllStepMsg[i], nStepStart+1, nStepEnd-1));
				assert(nStep > 0);
				tbRetStepMsg.tbSetpMsg[nStep] = Lib:StrTrim(string.sub(tbAllStepMsg[i], nStepEnd+1), "\n");
			end
		end
	end
	
	return tbRetStepMsg;
end;

function Task:LoadTaskTypeFile()
	
	local tbFile = Lib:LoadTabFile("setting/task/task_def.txt");
	if not tbFile then
		return;
	end	
	for i=2, #tbFile do
		local tbTemp = {
		 nFirstId	= tonumber(tbFile[i].TASK_ID_FIRST),
		 nLastId	= tonumber(tbFile[i].TASK_ID_LAST),
		 szTaskType = (tbFile[i].TASK_TYPE),
		 szTaskName = (tbFile[i].TASK_NAME),
		}
		table.insert(self.tbTaskTypes, tbTemp);
	end
end


function Task:GetAwardTypeFromIndex(nIndex)
	assert(self.tbGenieAwardType[nIndex]);
	return self.tbGenieAwardType[nIndex];
end
