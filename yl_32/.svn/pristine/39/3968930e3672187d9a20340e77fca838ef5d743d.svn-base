Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.BattleFieldZhunbeichang1 = Lib:NewClass(Activity.ActivityStep);  -- 红方准备场
Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.BATTLE_FIELD_ZHUNBEICHANG_1] = Activity.BattleFieldZhunbeichang1 -- 注册到活动工厂
local BattleFieldZhunbeichang1 = Activity.BattleFieldZhunbeichang1

-- 红方准备场玩家分配
function BattleFieldZhunbeichang1:OnEnterActivityScene(nPlayerId)
	--local nCamp = Activity.START_CAMP + self.nGroupNum - 1;
	--local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	-- 设置阵营
	--pPlayer.SetCurrentCamp(nCamp);
	print(string.format("[OnJoinPlayer]1开始战场准备场玩家阵营分配， Group:%d", self.nGroupNum));
	-- 会设置阵营了
	self:AddPlayerToGroup(nPlayerId, 1)  -- group 1,2  TODO:BUG:加到group的时候，会重新自动分配Camp
end

Activity.BattleFieldZhunbeichang2 = Lib:NewClass(Activity.ActivityStep);  -- 蓝方准备场
Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.BATTLE_FIELD_ZHUNBEICHANG_2] = Activity.BattleFieldZhunbeichang2 -- 注册到活动工厂
local BattleFieldZhunbeichang2 = Activity.BattleFieldZhunbeichang2

-- 蓝方准备场玩家分配
function BattleFieldZhunbeichang2:OnEnterActivityScene(nPlayerId)
	-- 会设置阵营了
	print(string.format("[OnJoinPlayer]2开始战场准备场玩家阵营分配， Group:%d", self.nGroupNum));
	self:AddPlayerToGroup(nPlayerId, 2)  -- group 1,2  TODO:BUG:加到group的时候，会重新自动分配Camp
end
