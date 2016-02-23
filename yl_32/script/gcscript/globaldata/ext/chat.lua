--=======================================================================
-- 文件名　：chat.lua
-- 创建者　：ZZ
-- 修改列表：
-- 创建时间：2011-09-15 10:10:43
-- 功能描述：
--=======================================================================

if not MODULE_GAMECENTER then
	return
end

-- 重置线聊喇叭
function GlobalData:ResetWorldSpeaker()
	print("重置线聊喇叭，每日赠送10次")
	GlobalExecute({"Chat:ResetWorldSpeaker"})
end
