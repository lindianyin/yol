if (not MODULE_GAMESERVER and not MODULE_GAMECLIENT) then
	return;
end

-- 任务变量定义
local emKVARGROUP_NPC		= 5;

local newCounter = function()
	local i = -1;
	return function ()
		i = i + 1;
		return i;
	end
end
local NpcVarId = newCounter();

--定义变量的逻辑映射，若该变量不作逻辑映射则指定一个空串
Npc.Var = 
{
	[emKVARGROUP_NPC] = 
	{
		emKCHARACTER_TASK_REPRESENT_ID				= {NpcVarId(), "RepresentId"},				-- 表现ID
		--emKCHARACTER_TASK_RELATION                  = {NpcVarId(), "Relation"},               --  Npc对应玩家的关系，暂时只有npc需要这个属性
		emKCHARACTER_TASK_FIERCETITIMES             = {NpcVarId(), "FierceTimes"},              --NPC在一个战斗中可狂暴次数
	},
}

