
if not MODULE_GAMECLIENT then
	return
end

Include("/script/npc/define.lua");

if (not Npc.NpcMgr) then
	Npc.NpcMgr = {};
end

local NpcMgr = Npc.NpcMgr;

Npc.tbDispatch = 
{
	[Npc.PNC_NOTIFY.RELATIONCHANGED]       = "OnServerNpcRelationChanged",
}

local tbDispatch = Npc.tbDispatch;

--收到服务端通知，并进行派发 
function NpcMgr:OnServerNotify(...)
	local nMsgid = arg[1];	
	local szFunctionName = tbDispatch[nMsgid];
	Lib:CallBack({NpcMgr[szFunctionName], NpcMgr, unpack(arg, 2)});
end

--修改npc关系
function NpcMgr:OnServerNpcRelationChanged(nNpcId)
	CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_NPCRELATION_CHANGED, nNpcId);
end


--将NPC设置为死亡状态
function NpcMgr:SetNpcDeath(dwNpcId)
	local pNpc = KGameBase.GetNpcById(dwNpcId)
	if pNpc then
		pNpc.SetDeath()
	end
end

	