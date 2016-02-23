-- 为了方便调试而建立一些函数
-- 重要注意事项：
--	1、常规、通用的函数不应该写在这里，这里的所有函数原则上不允许在游戏世界内调用。
--		原因：此文件可能会根据调试需要经常修改，所有的函数都有被破坏的可能。
--	2、本文件内可以按照全局的方式定义函数（实际上函数都在GM这个table内），使用时也可以当作全局使用
--		原因：为了方便，本文件采用了特殊方式模拟成了全局环境
function GM:DoCommand(szCmd)
	DoScript("/script/lib/debug/gm.lua");	-- 每次都重载这个脚本
	print("GmCmd["..tostring(me and me.szName).."]:", szCmd);
	
	local fnCmd, szMsg	= loadstring(szCmd, "[GmCmd]");
	if (not fnCmd) then
		error("Do GmCmd failed:"..szMsg);
	else
		setfenv(fnCmd, GM);
		return fnCmd();
	end
end

if (MODULE_GC_SERVER) then
--补偿接口start详细参数，考看\script\event\compensate\compensate_gm.lua
function GM:AddOnLine(szGate, szAccount, szName, nSDate, nEDate, szScript)
	return SpecialEvent.CompensateGM:AddOnLine(szGate, szAccount, szName, nSDate, nEDate, szScript);
end

function GM:AddOnNpc(szGate, szAccount, szName, nSDate, nEDate, tbAward)
	return SpecialEvent.CompensateGM:AddOnNpc(szGate, szAccount, szName, nSDate, nEDate, tbAward);
end

function GM:DelOnLine(szGate, szAccount, szName, nLogId, nGcManul, szResult)
	return SpecialEvent.CompensateGM:DelOnLine(szGate, szAccount, szName, nLogId, nGcManul, szResult);
end

function GM:DelOnNpc(szGate, szAccount, szName, nLogId, nGcManul, szResult)
	return SpecialEvent.CompensateGM:DelOnNpc(szGate, szAccount, szName, nLogId, nGcManul, szResult);
end

function GM:ClearDateOut()
	return SpecialEvent.CompensateGM:ClearDateOut();
end
--补偿接口end

-- 将玩家从桃源天牢释放
function GM:SetFree(szPlayerName)	
	GlobalExecute{"Player:SetFree", szPlayerName};
end
-- 将玩家抓入桃源天牢
function GM:Arrest(szPlayerName, nJailTerm)
	GlobalExecute{"Player:Arrest", szPlayerName, nJailTerm};
end
end

-- 模拟全局环境
setmetatable(GM, {__index=_G});
setfenv(1, GM);

function GetRobot(nFromId, nToId)
	local tbAllPlayer	= KPlayer.GetAllPlayer();
	local tbRobot		= {};
	local nCount	= 0;
	nToId	= nToId or nFromId;
	for _, pPlayer in pairs(tbAllPlayer) do
		local szName	= pPlayer.szAccount;
		if (string.sub(szName, 1, 5) == "robot") then
			local nRobotId	= tonumber(string.sub(szName, 6));
			if (nRobotId and nRobotId >= nFromId and nRobotId <= nToId) then
				nCount	= nCount + 1;
				tbRobot[nRobotId]	= pPlayer;
			end
		end
	end
	
	return tbRobot, nCount;
end

function CallRobot(nFromId, nToId, nRange)
	local nMapId, nMapX, nMapY = me.GetWorldPos();
	local tbRobot, nCount	= GetRobot(nFromId, nToId);
	nRange	= nRange or 0;
	nMapX	= nMapX - nRange - 1;
	nMapY	= nMapY - nRange - 1;
	for _, pPlayer in pairs(tbRobot) do
		pPlayer.NewWorld(nMapId, nMapX + KUnify.MathRandom(nRange * 2 + 1), nMapY + KUnify.MathRandom(nRange * 2 + 1));
	end
	me.SysMsg(nCount.." robot(s) called!");
end

