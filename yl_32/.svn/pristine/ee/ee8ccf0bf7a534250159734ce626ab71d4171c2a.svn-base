Include("script/mission/multiinstance/multi_base.lua")

MultiInstance.Lv20_Easy = Lib:NewClass(MultiInstance.BaseGame);

MultiInstance.GAME_FACTORY[MultiInstance.GAME_TYPE.LV_20_EASY] = MultiInstance.Lv20_Easy -- 注册到活动工厂

local Lv20_Easy = MultiInstance.Lv20_Easy;

function Lv20_Easy:OnKillNpc(pKiller, pNpc)
	if not self.nTotalKilledNpc then
		self.nTotalKilledNpc = 0;
	end
	self.nTotalKilledNpc = self.nTotalKilledNpc + 1;
end

function Lv20_Easy:CalcRushStar()
	local nStar = 1;
	local nLastTime = KGameBase.GetTime() - self.dwStartTime;
	
	-- 副本失败就是1星
	if self.bFinish ~= 1 then
		return 1;
	end
	
	local pSister = self:GetNpcByGroup("shenxianjiejie");
	assert(pSister);
	
	local nLifePercent = math.floor(pSister.GetCurrentLife() * 100 / pSister.GetMaxLife());
	
	if nLifePercent == 100 and self.nTotalKilledNpc == 111 then
		nStar = 5;
	elseif nLifePercent >= 50 and self.nTotalKilledNpc == 111 then
		nStar = 4;
	elseif nLifePercent >= 30 then
		nStar = 3;
	elseif nLifePercent < 30 then
		nStar = 2;
	end
		
	return nStar;
end