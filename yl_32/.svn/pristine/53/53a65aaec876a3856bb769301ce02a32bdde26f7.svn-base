local tbNpc = Npc:GetClass("npcshop")

tbNpc.tbMsg = {
	[1] = Lang.npc.str80[Lang.Idx],
	[2] = Lang.npc.str81[Lang.Idx],	
	[3] = Lang.npc.str82[Lang.Idx],
}

function tbNpc:OnDialog()
	-- local tbOpt =
	-- {
		-- {"杂货商店",self.OpenNpcShop,self,2},
		-- {"银两商店",self.OpenNpcShop,self,3},
		-- {"家族商店",self.OpenNpcShop,self,4},
		-- {"炫酷道具",self.OpenNpcShop,self,5},
	-- };
	--Dialog:Say("这里有奇珍异宝，欢迎来瞧瞧!",tbOpt);
	--if him.dwTemplateId and tbNpc.tbMsg[him.dwTemplateId] then
		--TaskAct:Talk(string.format("<npcpic=%d>%s<end><playername=%s>%s", him.dwTemplateId, tbNpc.tbMsg[him.dwTemplateId], him.szName, " "));
	--end
	
	if him.dwTemplateId and Npc.tbNpcTemplate[him.dwTemplateId] then
		TaskAct:Talk(string.format("<npcpic=%d>%s<end><playername=%s>%s", him.dwTemplateId, Npc.tbNpcTemplate[him.dwTemplateId].dialogMsg, him.szName, " "));
	end
end

function tbNpc:OpenNpcShop(shopId)
	me.CallClientScript({"QClientScript.NpcShop:OpenNpcShopPanel", shopId});
end
