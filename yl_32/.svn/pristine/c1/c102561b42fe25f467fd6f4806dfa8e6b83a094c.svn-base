
local tbNpc = Npc:GetClass("fbchuansongzhen")

function tbNpc:OnDialog(pDialogNpc)
	-- 先查看是否领奖，没有领奖则弹出面板进行领奖，已经领奖则直接传出指定位置

	local nTemplateId = pDialogNpc.dwTemplateId;

	-- added here
	local tbPos = {
		[5] = {3,6410,1630,1048576},
		[11] = {3,1824,5397,1048576},
		[44] = {113, 2065, 400, 1048576},
		[45] = {115,301,742,1048576},
		[46] = {106,2052,3401,1050618},
		[56] = {111,2456,4440,1048576},
		[57] = {101,5276,7256,1056829},
		[58] = {110,534,1302,1051648},
		[59] = {109,3416,687,1056286},
		[61] = {118,3003,3065,1059834},
		[62] = {102,7278,5013,1064954},
		--[1020] = {500, 6595, 5900, 1048576},
		[5152] = {100, 3079, 7489, 1054822},
		[5163] = {100, 3079, 7489, 1054822},
	};

	if (tbPos[nTemplateId] and type(tbPos[nTemplateId]) == "table" and #tbPos[nTemplateId] == 4) then
		local pInstance = Instance:GetMyInstance(me.dwId);
		assert(pInstance);
		pInstance:OnTrap(me);
	end

end;
