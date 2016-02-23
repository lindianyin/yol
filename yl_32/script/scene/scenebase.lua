
if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

Scene.tbSceneBase = {}
Scene.tbTrapBase = {}
local tbSceneBase	= Scene.tbSceneBase;

tbSceneBase.tbTraps		= nil;	-- Trap点类库

-- 根据参数，执行检查函数
function tbSceneBase:CallParam(tbSwitchExec, bIn)
	for _, fnCallBack in pairs(tbSwitchExec) do
		 Lib:CallBack({fnCallBack, Scene.tbSwitchs, bIn});
	end
end

-- 动态注册进入地图事件
function tbSceneBase:RegisterSceneEnterFun(szKey, fnExcute, ...)
	if not self.tbEnterSceneFun then
		self.tbEnterSceneFun = {};	-- 玩家进入地图事件函数表
	end
	self.tbEnterSceneFun[szKey] = {fnProcess = fnExcute, tbParam = arg};
end

-- 反注册
function tbSceneBase:UnregisterSceneEnterFun(szKey)
	if (not self.tbEnterSceneFun or not self.tbEnterSceneFun[szKey] )then
		return 0;
	end
	self.tbEnterSceneFun[szKey] = nil;
end

-- 执行进入地图事件
function tbSceneBase:ExcuteEnterFun()
	if not self.tbEnterSceneFun then
		return 0;
	end
	for _, tbExcute in pairs(self.tbEnterSceneFun) do
		tbExcute.fnProcess(unpack(tbExcute.tbParam));
	end
end

-- 动态注册离开地图事件
function tbSceneBase:RegisterSceneLeaveFun(szKey, fnExcute, ...)
	if not self.tbLeaveSceneFun then
		self.tbLeaveSceneFun = {};	-- 玩家进入地图事件函数表
	end
	self.tbLeaveSceneFun[szKey] = {fnProcess = fnExcute, tbParam = arg};
end

-- 反注册
function tbSceneBase:UnregisterSceneLeaveFun(szKey)
	if (not self.tbLeaveSceneFun or not self.tbLeaveSceneFun[szKey] )then
		return 0;
	end
	self.tbLeaveSceneFun[szKey] = nil;
end

-- 执行进入地图事件
function tbSceneBase:ExcuteLeaveFun()
	if not self.tbLeaveSceneFun then
		return 0;
	end
	for _, tbExcute in pairs(self.tbLeaveSceneFun) do
		tbExcute.fnProcess(unpack(tbExcute.tbParam));
	end
end

-- 定义玩家进入事件
function tbSceneBase:OnEnter()
end

-- 定义玩家离开事件
function tbSceneBase:OnLeave()
end

-- 获取当前场景的指定Trap点
function tbSceneBase:GetTrapClass(szClassName, bNotCreate)
	if (not self.tbTraps) then
		self.tbTraps	= {};
	end
	local tbTrap	= self.tbTraps[szClassName];
	-- 如果没有bNotCreate，当找不到指定模板时会自动建立新模板
	if (not tbTrap and bNotCreate ~= 1) then
		-- 新模板从基础模板派生
		tbTrap	= Lib:NewClass(Scene.tbTrapBase);
		tbTrap.szClass = szClassName;
		tbTrap.tbScene	= self;
		-- 加入到模板库里面
		self.tbTraps[szClassName]	= tbTrap;
	end
	return tbTrap;
end

-- 触发Trap点
function tbSceneBase:OnTrapPlayer(szClassName)
	if MODULE_GAMESERVER then
		local trapClass = self:GetTrapClass(szClassName)
		trapClass:OnServerPlayerTrap()
	end
end

local tbTrapBase	= Scene.tbTrapBase;

Scene.tbTrapNoEffect = {};

-- 判断一个trap点是否生效
function Scene:IsTrapEffect(szTrapClass)
	if not Scene.tbTrapNoEffect[szTrapClass] then
		return 1;
	end
	
	if Scene.tbTrapNoEffect[szTrapClass] ~= 1 then
		return 1;
	end
	
	return 0;
end

function Scene:SetTrapEffect(szTrapClass, nEffect)
	assert(nEffect == 1 or nEffect == 0);
	if nEffect == 1 then
		Scene.tbTrapNoEffect[szTrapClass] = 0;
	else
		Scene.tbTrapNoEffect[szTrapClass] = 1;
	end
end

-- 定义玩家Trap事件
function tbTrapBase:OnServerPlayerTrap()
	local tbTrapInfo = Scene.tbTrapInfo[self.szClass]
	print(tbTrapInfo, self.szClass)
	if not tbTrapInfo then
		return;
	end
	
	-- 自身对此trap点无效
	if me.IsTrapInvalid(tbTrapInfo.nTrapTemplateId) == 1 then
		return;
	end
	
	if tbTrapInfo.Type == "transfer" then
		-- 如果不是city类型，只能在当前地图传送
		local szSceneTypeName = KScene.GetSceneTypeName(tonumber(tbTrapInfo.Param1));
		if szSceneTypeName ~= "city" and Scene:IsTrapEffect(self.szClass) == 1 then
			me.TeleportTo(me.dwSceneId,
				tonumber(tbTrapInfo.Param2),
				tonumber(tbTrapInfo.Param3), 
				tonumber(tbTrapInfo.Param4) )
		elseif szSceneTypeName == "city" then
			me.TeleportTo(tonumber(tbTrapInfo.Param1),
				tonumber(tbTrapInfo.Param2),
				tonumber(tbTrapInfo.Param3), 
				tonumber(tbTrapInfo.Param4) )
		end
	elseif tbTrapInfo.Type == "TaskTransFb" then
		Task:CheckTrapTask(self.szClass, me);
	end
end

