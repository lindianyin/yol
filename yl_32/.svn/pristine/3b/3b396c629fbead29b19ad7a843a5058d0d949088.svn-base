
-- 随机道具

-------------------------------------------------------------------
--File: 	chuansongfu.lua
--Author: 	
--Date: 	
--Describe:	传送符，通用脚本


------------------------------------------------------------------------------------------
-- initialize

local tbDeliveryItem = Item:GetClass("chuansongfu");

------------------------------------------------------------------------------------------
-- public
tbDeliveryItem.tbSceneList = 
{
	[1] =  --城镇
	{
		{Lang.item.str11[Lang.Idx],{1,2122,1939,1068108}},
		{Lang.item.str12[Lang.Idx],{100,4448,4392,1050854}},
		{Lang.item.str13[Lang.Idx],{111,3534,2284,1056010}},
		{Lang.item.str14[Lang.Idx],{101,5786,1797,1051130}},
		{Lang.item.str15[Lang.Idx],{102,3443,1731,1064893}},
	},
	[2] =  --野外
	{
		{Lang.item.str16[Lang.Idx],{106,1952,1857,1051648}},
		{Lang.item.str17[Lang.Idx],{113,2135,2083,1049159}},
		{Lang.item.str18[Lang.Idx],{115,3285,1346,1053184}},
		{Lang.item.str19[Lang.Idx],{110,2023,2590,1053184}},
		{Lang.item.str20[Lang.Idx],{109,2129,1117,1054720}},
		{Lang.item.str21[Lang.Idx],{118,2334,1128,1057797}},
	},
}

function tbDeliveryItem:OnUse()
	local tbOpt = {
		{Lang.item.str22[Lang.Idx],self.GetTeleType,self,1},
		{Lang.item.str23[Lang.Idx],self.GetTeleType,self,2},
		};
	Dialog:Say(Lang.item.str24[Lang.Idx],tbOpt);
end

function tbDeliveryItem:GetTeleType(Index)
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
	table.insert(tbOpt, Lang.item.str25[Lang.Idx]);
	Dialog:Say(Lang.item.str26[Lang.Idx] .. tbOpt); 
end

function tbDeliveryItem:GetDelivery(tbSceneInfo)
	me.TeleportTo(unpack(tbSceneInfo));
end