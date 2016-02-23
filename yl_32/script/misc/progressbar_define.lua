
if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

ProgressBar.tbRegisteredEvent = {}

ProgressBar.EVENT_MOVE = 0 -- 移动
ProgressBar.EVENT_LOGOUT = 1  -- 离开游戏
ProgressBar.EVENT_COMBAT_START = 2  -- 进入战斗
ProgressBar.EVENT_PLOT_PLAY_START = 3  -- 播放剧情
ProgressBar.EVENT_NEW_PROGRESS = 4  -- 开启了新进度条
ProgressBar.EVENT_BE_HIT = 5  -- 被打
ProgressBar.EVENT_DEATH = 6 -- 死亡
ProgressBar.EVENT_CAST_SKILL = 7 -- 释放技能
--ProgressBar.EVENT_ITEM_BUY = 5  -- 买道具
--ProgressBar.EVENT_ITEM_SELL = 6  -- 卖道具
--ProgressBar.EVENT_ITEM_REMOVE = 7  -- 丢弃道具
--ProgressBar.EVENT_ITEM_CHANGEEQUIP = 8  -- 更换装备
--ProgressBar.EVENT_ITEM_ARRANGEITEM = 9  -- 移动背包道具
--ProgressBar.EVENT_ITEM_USE = 10  -- 使用道具
ProgressBar.EVENT_CLIENT_CANCEL = 11 -- 客户端ESC键
ProgressBar.EVENT_ITEM_OPS = 12 -- 道具操作
ProgressBar.EVENT_CAPTAIN_CHANGE = 13 -- 移交队长
ProgressBar.EVENT_LEAVESCENE = 14	--传送

-- 以下事件始终打断读条
ProgressBar.tbAlwaysBreak =
{
	[ProgressBar.EVENT_MOVE] = 1,
	--[ProgressBar.EVENT_BE_HIT] = 1,
	[ProgressBar.EVENT_DEATH] = 1,
	[ProgressBar.EVENT_CAST_SKILL] = 1,
	[ProgressBar.EVENT_COMBAT_START] = 1,
	[ProgressBar.EVENT_CLIENT_CANCEL] = 1,
	[ProgressBar.EVENT_LOGOUT] = 1,
	[ProgressBar.EVENT_NEW_PROGRESS] = 1,
	[ProgressBar.EVENT_ITEM_OPS] = 1,
	[ProgressBar.EVENT_LEAVESCENE] = 1,
}
