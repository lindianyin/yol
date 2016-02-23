
local tbNpc = Npc:GetClass("chefu")

tbNpc.tbSceneList = 
{
	[1] =  --≥«’Ú
	{
		{Lang.npc.str39[Lang.Idx],{1,2122,1939,1068108}},
		{Lang.npc.str40[Lang.Idx],{100,4448,4392,1050854}},
		{Lang.npc.str41[Lang.Idx],{111,3534,2284,1056010}},
		{Lang.npc.str42[Lang.Idx],{101,5786,1797,1051130}},
		{Lang.npc.str43[Lang.Idx],{102,3443,1731,1064893}},
	},
	[2] =  --“∞Õ‚
	{
		{Lang.npc.str44[Lang.Idx],{106,1952,1857,1051648}},
		{Lang.npc.str45[Lang.Idx],{113,2135,2083,1049159}},
		{Lang.npc.str46[Lang.Idx],{115,3285,1346,1053184}},
		{Lang.npc.str47[Lang.Idx],{110,2023,2590,1053184}},
		{Lang.npc.str48[Lang.Idx],{109,2129,1117,1054720}},
		{Lang.npc.str49[Lang.Idx],{118,2334,1128,1057797}},
	},
}

function tbNpc:OnDialog()
	local tbOpt = {
		{Lang.npc.str50[Lang.Idx],self.GetTeleType,self,1},
		{Lang.npc.str51[Lang.Idx],self.GetTeleType,self,2},
		{Lang.npc.str52[Lang.Idx]},
		};
	if me.GetLevel() < 10 then	
		return Dialog:Say(Lang.npc.str53[Lang.Idx]);
	end
	Dialog:Say(Lang.npc.str54[Lang.Idx],tbOpt);
end

function tbNpc:GetTeleType(Index)
	local tbOpt = {};
	local tbSceneInfo = {};
	local tbSceneData = self.tbSceneList[Index];
	local dwSceneId = me.GetPosition().dwSceneId;
	for i = 1 ,#tbSceneData do
		if dwSceneId == tbSceneData[i][2][1] then
			table.remove(tbSceneData,i);
		end
	end
	for i = 1 ,#tbSceneData do
		table.insert(tbOpt, {tbSceneData[i][1], self.GetDelivery, self, tbSceneData[i][2]});
	end
	table.insert(tbOpt, Lang.npc.str52[Lang.Idx]);
	Dialog:Say(Lang.npc.str55[Lang.Idx], tbOpt); 
end

function tbNpc:GetDelivery(tbSceneInfo)
	me.TeleportTo(unpack(tbSceneInfo));
end
