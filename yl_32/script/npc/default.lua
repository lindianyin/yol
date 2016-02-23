
-- Npc默认模板（也是基础模板）

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

-- 从Npc模板库中找到此模板，如不存在会自动建立新模板并返回
-- 提示：npc.lua 已经在 preload.lua 中前置，这里无需再Require
local tbDefault	= Npc:GetClass("default");

-- 定义对话事件
function tbDefault:OnDialog()
	local szMsg	= string.format(Lang.npc.str2[Lang.Idx], him.szName, me.szName);
--	local szMsg = "default class dialog";
	return Dialog:Say(szMsg, {});
end;

-- 定义死亡事件
function tbDefault:OnDeath(pNpcKiller)
	--local szMsg	= string.format("%s：我被 %s 杀了！", him.szName, pNpcKiller.szName);
	--Msg2SubWorld(szMsg);
end;

-- Client,触发聊天泡泡
function tbDefault:OnTriggerBubble()
	local nBubbleGroupTotleWeight = self:GetBubbleGroupTotleWeight();
	if (nBubbleGroupTotleWeight <= 0) then
		return;
	end
	
	local nGroupRandom = KUnify.MathRandom(nBubbleGroupTotleWeight);
	local nSum = 0;
	local tbSelectedBubble = nil;
	for _,item in pairs(Npc.BubbleProperty) do
		nSum = nSum + item.Weight;
		if (nSum >= nGroupRandom) then -- 选择了组
			if (not item.nIndex) then
				return;
			end
			
			tbSelectedBubble = self:GetSelectedBubble(item.nIndex);
			break;
		end
	end
	
	if (not tbSelectedBubble) then
		return;
	end
	
	--  判断所有条件是否满足，若满足则显示消息，并且执行客户端的回调
	if (tbSelectedBubble.tbConditions) then
		for _,cond in ipairs(tbSelectedBubble.tbConditions) do
			if (dostring(self:ReadBubbleConditionFaction(cond))() ~= 1) then
				return;
			end
		end
	end
		
	-- 执行到此处表明所有条件均满足
	local szMsg = tbSelectedBubble.szMsg;
	
	him.SetBubble(szMsg);
	
	-- 执行客户端回调
	if (tbSelectedBubble.tbCallBacks) then
		for _,callback in ipairs(tbSelectedBubble.tbCallBacks) do
			Lib:CallBack(callback);
		end
	end
end


-- 获得所有泡泡组的总权重见define.lua
function tbDefault:GetBubbleGroupTotleWeight()
	local nSum = 0;
	for _,item in pairs(Npc.BubbleProperty) do
		nSum = nSum + item.Weight;
	end
	
	return nSum;
end

function tbDefault:GetSelectedBubble(nIndex)
	local tbBubbleGroup = self.tbBubble[nIndex];
	if (not tbBubbleGroup) then
		return;
	end
	local nItemTotleWeight = self:GetTotleWeightInSingleGroup(nIndex);
	if (nItemTotleWeight <= 0) then
		return;
	end
	
	local nBubbleRandom = KUnify.MathRandom(nItemTotleWeight);
	local nSum = 0;
	for _, item in pairs(self.tbBubble[nIndex]) do
		nSum = nSum + item.nProbability;
		if (nSum >= nBubbleRandom) then
			return item;
		end
	end
end

function tbDefault:GetTotleWeightInSingleGroup(nIndex)
	local nSum = 0;
	if (not self.tbBubble[nIndex]) then
		print(nnIndex)
		assert(false);
	end
	for _, item in pairs(self.tbBubble[nIndex]) do
		nSum = nSum + item.nProbability;
	end
	
	return nSum
end

function tbDefault:ReadBubbleConditionFaction(tbFunction)
	local szFuncName	= "";
	local szFuncParam	= "";
	szFuncName = tbFunction[1];
	for i = 2, #tbFunction do
		szFuncParam	= table.concat(tbFunction[i], ",");	
	end

	return szFuncName.."("..szFuncParam..")";
end


--============ 定义属性 =============--

function tbDefault:no()
end

function tbDefault:cancel()
end

tbDefault.tbBubble = 
{
	--[[
	[Npc.BubbleProperty.Task.nIndex] 	= 
	{
		{nProbability = 1, szMsg = Task:GetTaskGossip(nTaskId), Conditions = {}, CallBacks = {}},
		{nProbability = 1, szMsg = Task:GetTaskGossip(nTaskId), Conditions = {}, CallBacks = {}},
	},
	]]--
	[Npc.BubbleProperty.Normal.nIndex] 	= 
	{
		{
			nProbability = 1, 
			szMsg = Lang.npc.str3[Lang.Idx],
		},
		{
			nProbability = 1, 
			szMsg = Lang.npc.str4[Lang.Idx],
		},
		--[[
		{
			nProbability = 1, 
			szMsg = "hi, if you are male you will maybe see this msg",
			tbConditions = 
			{
				{"BubbleCond:IsFaction","2"},
				{"BubbleCond:IsMale"},
			}, 
			tbCallBacks = 
			{
				{"Task:OnAskAward", 1, 1},
				{"Task:OnRefresh", 1, 1, 0},
			},
		},
		]]--
	},
}
