-- 文件名　：
-- 创建者　：
-- 创建时间：
-- 描述：须弥幻境1期，虫王菠萝丁



-----------------虫王菠萝丁------------------
Include("\\script\\mission\\level\\room_base.lua");

XoyoGame.BaGuaZhen = Lib:NewClass(XoyoGame.BaseRoom);

local BaGuaZhen = XoyoGame.BaGuaZhen;

BaGuaZhen.tbIndex = {};

BaGuaZhen.nLock = 5;

function BaGuaZhen:OnInitRoom()
	self.tbBlood = {};
end

function BaGuaZhen:RecordBlood(szGroup)
	if not self.tbNpcGroup[szGroup] then
		return 0;
	end
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		local nLife = pNpc.GetCurrentLife();
		local nMaxLife = pNpc.GetMaxLife();
		if nLife and nLife > 0 then
			self.tbBlood[szGroup] = nLife / nMaxLife * 100;
			print(szGroup,self.tbBlood[szGroup]);
			break;
		end	
	end
end

function BaGuaZhen:SetNpcBlood(szGroup)
	if szGroup == "zhenyan1" then
		self:SetNpcLife(szGroup,self.tbBlood["zhenyan"] or 100);
	elseif szGroup == "zhenyan" then
		self:SetNpcLife(szGroup,self.tbBlood["zhenyan1"] or 100);
	end
end

function BaGuaZhen:SetNpcLife(szGroup,nPercent)
	for _,nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		local nCurLife = nPercent * pNpc.GetMaxLife() / 100;
		if nCurLife and nCurLife > 0 then
			pNpc.SetCurrentLife(nCurLife);
			print("percent",nPercent or 100);
			print("maxlife",pNpc.GetMaxLife() or 0);
			print("percent",nCurLife or 0);
		end
	end
end

function BaGuaZhen:GetRoomIndex()  --确定每次进入该房间会随机到的卦象
	local i,j,k;
	i = KUnify.MathRandom(1,8);
	self.tbIndex[1] = i + 3;
	repeat
		j = KUnify.MathRandom(1,8);
	until i ~= j
	self.tbIndex[2] = j + 3;
	repeat
		k = KUnify.MathRandom(1,8);
	until i ~= k and j ~= k
	self.tbIndex[3] = k + 3;
	
	self.tbIndex[4] = self.tbIndex[1] + 8;
	self.tbIndex[5] = self.tbIndex[2] + 8;
	self.tbIndex[6] = self.tbIndex[3] + 8;
	
	Lib:ShowTB(self.tbIndex);
end
