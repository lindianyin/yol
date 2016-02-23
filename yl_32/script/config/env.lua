-- 游戏世界基础常量定义（注意保持与程序的一致）

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER or MODULE_GC_SERVER or MODULE_SKILLSIMULATOR or MODULE_GAMECENTER) then
	return
end

Env.GAME_FPS			= 16;		-- 游戏世界每秒帧数

-- 性别定义
Env.SEX_MALE			= 0;		-- 男性
Env.SEX_FEMALE			= 1;		-- 女性

-- 性别描述字符串
Env.SEX_NAME =
{
	[Env.SEX_MALE]		= "男性",
	[Env.SEX_FEMALE]	= "女性",
};

-- 世界新闻类型
Env.NEWSMSG_NORMAL 		= 0;    -- 普通
Env.NEWSMSG_COUNT		= 1;	-- 延时播放
Env.NEWSMSG_TIMEEND		= 2;	-- 定时停止

Env.WEIWANG_BATTLE		= 1;
Env.WEIWANG_MENPAIJINGJI= 2;
Env.WEIWANG_DATI		= 3;
Env.WEIWANG_BAIHUTANG 	= 4;
Env.WEIWANG_TREASURE	= 5;
Env.WEIWANG_BAOWANTONG	= 6;
Env.WEIWANG_GUOZI		= 7;
Env.WEIWANG_BOSS		= 8;