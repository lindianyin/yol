
-- 药品，通用功能脚本

local tbMedicine = Item:GetClass("medicine");

--local PK_LIMIT_USE_MEDICINE	= 9;			-- 恶名值过高喝药无效

------------------------------------------------------------------------------------------

-- 判断是否可用
function tbMedicine:CheckUsable()
	return 1;
end

function tbMedicine:OnUse()					-- 喝药
	local tbBaseAttrib = it.GetBaseAttrib()
	for _, tb in ipairs(tbBaseAttrib) do
		it.ApplyMagicAttrib(tb.szName, tb.tbValue) -- TODO refact
	end
	return	1
end

function tbMedicine:GetTip(nState)			-- 获取Tip
	local szTip = "";
	szTip = szTip..self:Tip_Attrib();
	return szTip;
end

function tbMedicine:Tip_Attrib()			-- 获得Tip字符串：药品属性

	local szTip = "<color=white>";
	local tbAttrib = it.GetBaseAttrib();	-- 获得药品属性
	local tbDesc = {};

	for _, tbMA in ipairs(tbAttrib) do
		if tbMA.szName ~= "" then
			local szDesc = FightSkill:GetMagicDesc(tbMA.szName, tbMA.tbValue, nil, 1);
			if szDesc ~= "" then
				table.insert(tbDesc, szDesc);
			end
		end
	end

	return	szTip..table.concat(tbDesc, "\n").."<color>";

end
