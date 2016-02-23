
-- ¿Õ°×NpcÄ£°å

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

local tbAnimal	= Npc:GetClass("citynpc");

function tbAnimal:OnDialog()
    if him.dwTemplateId and Npc.tbNpcTemplate[him.dwTemplateId] then
		TaskAct:Talk(string.format("<npcpic=%d><npcname=%s>%s<end><playername=%s>%s", him.dwTemplateId, him.szName, Npc.tbNpcTemplate[him.dwTemplateId].dialogMsg, him.szName, "\\b"..Lang.npc.str1[Lang.Idx].."\\b#"));
	end
end

function tbAnimal:OnDeath()
end
