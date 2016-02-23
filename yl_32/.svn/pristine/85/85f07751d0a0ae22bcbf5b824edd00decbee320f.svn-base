-- Player的常量定义
if not (MODULE_GAMECLIENT or MODULE_GAMESERVER or MODULE_GC_SERVER) then
	return
end

Player.nMaxLevel = 60
Player.nMaxLevelLimit = 60

-- CHANGE_SERVER_RESULT
Player.emCHANGE_SERVER_ALLOWED = 0
Player.emCHANGE_SERVER_FULL = 1
Player.emCHANGE_SERVER_NOT_FOUND = 2
Player.emCHANGE_SCENE_NOT_FOUND = 3
Player.emCHANGE_BUG_CHECK = 4
--Player.emCHANGE_SERVER_ALL

--防沉迷标志
Player.ACCOUNT_LIMITTIME_FLAG_NOINFO_NOTLIMIT	= 0	--无实名信息，不属于防沉迷管辖范围
Player.ACCOUNT_LIMITTIME_FLAG_NOINFO_LIMIT		= 1	--无实名信息，属于防沉迷管辖范围
Player.ACCOUNT_LIMITTIME_FLAG_INFO_NOTLIMIT		= 2	--有实名信息，不属于防沉迷管辖范围
Player.ACCOUNT_LIMITTIME_FLAG_INFO_LIMIT		= 3	--有实名信息，属于防沉迷管辖范围

Player.UNLIMIT_NEW_ACCOUNT_EX	= 5		--用户既有身份证又有联系方式，不纳入防沉迷
Player.LIMIT_NEW_ACCOUNT_EX	= 6			--用户既有身份证又有联系方式，纳入防沉迷
Player.UNLIMIT_NO_ID_CARD_INFO	= 7		--用户有联系方式，没有身份证，不纳入防沉迷
Player.UNLIMIT_NO_CONTACT_INFO	= 8		--用户有身份证，没有联系方式，不纳入防沉迷
Player.UNLIMIT_NO_ALL_INFO	= 9			--用户既没有身份证，也没有联系方式，不纳入防沉迷
Player.LIMIT_NO_ID_CARD_INFO = 10		--用户有联系方式，没有身份证，纳入防沉迷
Player.LIMIT_NO_CONTACT_INFO = 11		--用户有身份证，没有联系方式，纳入防沉迷
Player.LIMIT_NO_ALL_INFO = 12			--用户既没有联系方式，也没有身份证，纳入防沉迷

Player.tbLimitFlag = {
	[Player.ACCOUNT_LIMITTIME_FLAG_NOINFO_NOTLIMIT] = 0,
	[Player.ACCOUNT_LIMITTIME_FLAG_NOINFO_LIMIT] = 1,
	[Player.ACCOUNT_LIMITTIME_FLAG_INFO_NOTLIMIT] = 0,
	[Player.ACCOUNT_LIMITTIME_FLAG_INFO_LIMIT] = 1,
	
	[Player.UNLIMIT_NEW_ACCOUNT_EX] = 0,
	[Player.LIMIT_NEW_ACCOUNT_EX] = 1,
	[Player.UNLIMIT_NO_ID_CARD_INFO] = 0,
	[Player.UNLIMIT_NO_CONTACT_INFO] = 0,
	[Player.UNLIMIT_NO_ALL_INFO] = 0,
	[Player.LIMIT_NO_ID_CARD_INFO] = 1,
	[Player.LIMIT_NO_CONTACT_INFO] = 1,
	[Player.LIMIT_NO_ALL_INFO] = 1,
}

--累积下线时间清状态
Player.UnLimitOffLineTime = 5 * 60 * 60 --5小时，单位：秒

Player.HalfLimitOnlineTime = 3 * 60 * 60 --3小时后收益变为减半
Player.WholeLimitOnlineTime = 3 * 60 * 60 --5小时后收益完全为0

--间隔时间提示
Player.NoLimitInterval = 1 * 60 * 60  --奖励正常时，1小时提示一次
Player.HalfLimitInterval = 30 * 60  --奖励减半时，30分钟提示一次
Player.WholeLimitInterval = 15 * 60  --奖励为0时，15分钟提示一次

--防沉迷奖励收益标志
Player.AwardFlagNoLimit = 0
Player.AwardFlagHalf = 1
Player.AwardFlagZero = 2

Player.tbAwardLimitMsg = {
	[Player.AwardFlagHalf] = "",
	[Player.AwardFlagZero] = "",
}

Player.LIMIT_ONEHOUR_MESSAGE 	= 1
Player.LIMIT_TWOHOUR_MESSAGE 	= 2
Player.LIMIT_THREEHOUR_MESSAGE 	= 3
Player.LIMIT_FOURHOUR_MESSAGE 	= 4

Player.emKLOSEITEM_SELL = 1
Player.emKLOSEITEM_TRADE = 2
Player.emKLOSEITEM_RECYCLE = 3
Player.emKLOSEITEM_BUYRECYCLE = 4
Player.emKLOSEITEM_USE = 5
Player.emKLOSEITEM_THROW = 6
Player.emKLOSEITEM_MAIL = 7		
Player.emKLOSEITEM_TIMEOUT = 8
Player.emKLOSEITEM_STALL_SELL = 9
Player.emKLOSEITEM_STALL_BUY = 10
Player.emKLOSEITEM_STALL_BUY_STACK = 11
Player.emKLOSEITEM_PICK = 12		-- 拾起删除
Player.emKLOSEITEM_BUYFAIL = 13
Player.emKLOSEITEM_STACK = 14
Player.emKLOSEITEM_SET_STACK = 15
Player.emKLOSEITEM_CONSUME = 16
Player.emKLOSEITEM_REGENE = 17		-- Regenerate
Player.emKLOSEITEM_FAVOR = 18		-- 亲密度道具
Player.emKLOSEITEM_DUP = 19
Player.emKLOSEITEM_BYGDP = 20		-- Remove by g,d,p
Player.emKLOSEITEM_FEATURE = 21
Player.emKLOSEITEM_MANTLE = 22
Player.emKLOSEITEM_AUCTION = 23
Player.emKLOSEITEM_MYSTERY_SHOP_SALE = 24
Player.emKLOSEITEM_RETURN_TO_KIN_REPERTORY = 25

Player.emKLOSEITEM_SCRIPT = 100
Player.emKLOSEITEM_SCRIPT_COMPOSE = 101		--玄晶合成
Player.emKLOSEITEM_SCRIPT_ENHANCE = 102		--强化时扣除的玄晶
Player.emKLOSEITEM_SCRIPT_EVENTUSE = 103	-- 活动扣除
Player.emKLOSEITEM_SCRIPT_TASKUSE = 104		-- 任务扣除
Player.emKLOSEITEM_SCRIPT_BREAKUP = 106		-- 玄晶、装备拆解
Player.emKLOSEITEM_SCRIPT_REPAIR = 108			-- 修理品消耗
Player.emKLOSEITEM_SCRIPT_STONE_MIX = 117		-- 宝石合成
Player.emKLOSEITEM_SCRIPT_STONE_MOUNT = 118			-- 宝石镶嵌

Player.emMOP_STATE_NONE 	= 0	-- 未扫荡
Player.emMOP_STATE_DOING 	= 1	-- 扫荡中
Player.emMOP_STATE_DONE 	= 2	-- 扫荡完成


Player.tbLimitMessage = {
	[Player.LIMIT_ONEHOUR_MESSAGE] = Lang.player.str1[Lang.Idx],
	[Player.LIMIT_TWOHOUR_MESSAGE] = Lang.player.str2[Lang.Idx],
	[Player.LIMIT_THREEHOUR_MESSAGE] = Lang.player.str3[Lang.Idx],
	[Player.LIMIT_FOURHOUR_MESSAGE] = Lang.player.str4[Lang.Idx],
}

--新手调查问卷，在这4个级别让玩家做问卷
Player.tbFreshManSurveyLevel = {18, 35, 45, 82}

Player.tbFreshManSurveyAward = {
	[18] = {1061, 128},			-- 服装箱、藏宝图
	[35] = {66, 67},			-- 修为，雪玉（够一个铭文）
	[45] = {40005, 68, 146},	-- 化冥道具，宠物洗点道具，职业洗点道具
	[82] = {138,},
}

function Player:GetPlayerMaxLevel()
	if not Player.nMaxLevel then
		Player.nMaxLevel = 1
	end
	return Player.nMaxLevel
end

function Player:GetFreshManSurveyLevel2VarTable()
	if (not self.tbFreshManLevel2Var) then
		local tbVar = Character.CustomVar[Character.emKCUSTOM_VARGROUP_FRESHMAN]
		self.tbFreshManLevel2Var = {
				[Player.tbFreshManSurveyLevel[1]] = tbVar.emFRESHMAN_SUREVY_LEVEL_FIRST,
				[Player.tbFreshManSurveyLevel[2]] = tbVar.emFRESHMAN_SUREVY_LEVEL_SECOND,
				[Player.tbFreshManSurveyLevel[3]] = tbVar.emFRESHMAN_SUREVY_LEVEL_THIRD,
				[Player.tbFreshManSurveyLevel[4]] = tbVar.emFRESHMAN_SUREVY_LEVEL_FOURTH,
			}
	end
	return self.tbFreshManLevel2Var
end
