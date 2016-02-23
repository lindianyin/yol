
AI.g_AIParam = {
}

local tbFileData = Lib:LoadTabFile("/setting/ai/ai_type.txt") or {}

for _, tbRow in pairs(tbFileData) do
	AI.g_AIParam[tonumber(tbRow.AIType)] = tbRow
end
