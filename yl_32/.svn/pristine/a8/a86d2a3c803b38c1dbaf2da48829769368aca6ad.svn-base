Include("script/mission/instance/instance_def.lua")
Include("script/pet/pet_island.lua")

if not Instance.tbSetting then
	Instance.tbSetting = {}
end

local InstanceSetting = Instance.tbSetting;


--宠物岛
InstanceSetting[233]=
{
	nTaskInstance = 1;
    nMinLevel = 1;
    nMaxLevel = 60;
	nMapId = 233;
	nNoTimeLimit = 1; -- 南天门无时间限制
    tbBeginPos = {2038,2060,1069056},
	OnInitCallback = "PetIsland:OnInstanceInit", -- 副本初始化后调用宠物岛脚本
	OnUnInitCallback = "PetIsland:OnInstanceUnInit",
	--OnLoginCallback = "",
	OnLogoutCallback = "PetIsland:OnLogout",

    -- 副本涉及的NPC种类
	NPC =
	{
		-- [1] = {nTemplate = 133, nLevel = 1, nSeries = -1},		-- 宠物1
		-- [2] = {nTemplate = 138, nLevel = 1, nSeries = -1},		-- 宠物2
		-- [3] = {nTemplate = 139, nLevel = 1, nSeries = -1},		-- 宠物3
	},

	LOCK =
	{  -- 具体副本实施 ： PetIsland:OnInstanceInit
       -- 1号锁不能不填，默认1号为起始锁
		[1] = {nTime = -1, nNum = 0,		-- 1秒后
			tbPrelock = {},
			tbStartEvent =
			{
			},
			tbUnLockEvent =
			{
			},
		},
	}
}