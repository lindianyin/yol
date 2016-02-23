
-- 宠物岛宠物
local tbNpc = Npc:GetClass("petisland_static")

-- 宠物打静态宠物之星对话
function tbNpc:OnDialog()
	-- 开始战斗
	--PetIsland:Start
	Dialog:Say("Oh Baby");
end

-- 宠物岛战斗宠物执行
function tbNpc:OnDeath(pKiller, pNpc)
	PetIsland:OnDeath(pKiller, pNpc)
end