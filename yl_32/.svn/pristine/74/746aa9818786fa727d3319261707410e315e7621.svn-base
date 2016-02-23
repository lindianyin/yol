-----------------------------------------------------
--文件名		：	ktaskscript.lua
--创建者		：	zhengyuhua
--创建时间		：	2008-01-08
--功能描述		：	任务扩展脚本。
------------------------------------------------------
if MODULE_GC_SERVER then
	return
end

local TASK_AWARD_TIP = {};
TASK_AWARD_TIP.exp = function(tbAward)
	return Lang.task.str36[Lang.Idx] .."：<color=green>"..tostring(tbAward.varValue).."<color>";
end

TASK_AWARD_TIP.money = function(tbAward)
	return Lang.task.str37[Lang.Idx] .."：<color=green>"..tostring(tbAward.varValue).."<color>";
end

TASK_AWARD_TIP.bindmoney = function(tbAward)
	if (type(tbAward.varValue) == "number") then
		return Lang.task.str38[Lang.Idx] .."：<color=green>"..tostring(tbAward.varValue).."<color>";
	else
		return tbAward.szDesc;
	end
end

TASK_AWARD_TIP.activemoney = function(tbAward)
	return Lang.task.str39[Lang.Idx] .."：<color=green>"..tostring(tbAward.varValue).."<color>";
end

TASK_AWARD_TIP.gatherpoint = function(tbAward)
	return Lang.task.str40[Lang.Idx] .."：<color=green>"..tbAward.varValue.."<color>";
end

TASK_AWARD_TIP.linktask_repute = function(tbAward)
	return	Lang.task.str41[Lang.Idx] .."：<color=green>"..tbAward.varValue[3].."<color>";
end

TASK_AWARD_TIP.linktask_cancel = function(tbAward)
	return	"<color=green>".. Lang.task.str42[Lang.Idx]  .. "<color>";
end

TASK_AWARD_TIP.makepoint = function(tbAward)
	return Lang.task.str43[Lang.Idx] .. "：<color=green>"..tbAward.varValue.."<color>";
end

TASK_AWARD_TIP.title = function(tbAward)
	local tbTitle = KPlayer.GetTitleLevelAttr(tbAward.varValue[1], tbAward.varValue[2], tbAward.varValue[3]);
	local szDesc = tbTitle.szTitleName or "";
	return Lang.task.str44[Lang.Idx] .."：<color=green>"..szDesc.."<color>";
end

TASK_AWARD_TIP.repute = function(tbAward)
	local tbRepute = KPlayer.GetReputeInfo();
	local szReputeDesc = tbRepute[tbAward.varValue[1]][tbAward.varValue[2]].szName
	return Lang.task.str45[Lang.Idx] .."：<color=green>"..szReputeDesc.." "..tbAward.varValue[3]..Lang.task.str46[Lang.Idx] .."<color>";
end


-- 获取某奖励的tip
function KTask.GetAwardTip(tbAward)
	local fncTip = TASK_AWARD_TIP[tbAward.szType];
	if not fncTip then
		return tbAward.szDesc;
	end
	local szTip	= "";
	if type(fncTip) == "function" then
		szTip = fncTip(tbAward);
	end
	return szTip;
end
