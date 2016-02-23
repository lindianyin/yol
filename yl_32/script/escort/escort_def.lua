
Escort.ESCORT_SCENE = 235;

Escort.emESCORT_STATE_NONE = 0;
Escort.emESCORT_STATE_NORMAL = 1;
Escort.emESCORT_STATE_BEHIT = 2;
Escort.emESCORT_STATE_COMPLETE_NORMAL = 3;
Escort.emESCORT_STATE_COMPLETE_BEHIT = 4;

Escort.STAND_POS = {
	{1467, 4255, 1057177},
	{1646, 4023, 1056209},
	{1815, 3863, 1055293},
	
	{1734, 4255, 1057177},
	{1916, 4023, 1056209},
	{2048, 3863, 1055293},
	
	{2039, 4255, 1057177},
	{2160, 4023, 1056209},
	{2268, 3863, 1055293},
	
	{2310, 4255, 1057177},
	{2421, 4023, 1056209},
	--[[
	{1815, 3863, 1055293},
	{2048, 3863, 1055293},
	{2268, 3863, 1055293},
	
	{1646, 4023, 1056209},
	{1916, 4023, 1056209},
	{2160, 4023, 1056209},
	{2421, 4023, 1056209},
	
	{1467, 4255, 1057177},
	{1734, 4255, 1057177},
	{2039, 4255, 1057177},
	{2310, 4255, 1057177},
	--]]
};

-- 刷新坐骑的概率
Escort.REFRESH_PROBABILTY =
{
	[1] = 100,			-- 白云
	[2] = 60,			-- 冰雪云
	[3] = 30,			-- 黄金云
	[4] = 10,			-- 佛光葫芦
	[5] = 5,			-- 大神莲座
};

Escort.BUY_ESCORT_TIMES_COST = 10;		-- 购买修仙次数花费
Escort.BUY_REFRESH_TIMES_COST = 10;		-- 购买刷新次数花费
Escort.BUY_ROB_TIMES_COST = 10;		-- 购买劫驾次数花费
Escort.BUY_HELP_TIMES_COST = 10;		-- 购买救驾次数花费