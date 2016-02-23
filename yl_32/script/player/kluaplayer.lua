
local self;		-- 提供以下函数用的UpValue

if MODULE_GAMESERVER then

-- tbItemInfo =
--{
--		nSeries or Env.SERIES_NONE,		五行，默认无
--		nEnhTimes or 0,					强化次数，默认0
--		nLucky or 0,					幸运
--		tbGenInfo,						
--		tbRandomInfo, 				装备随机品质
--		nVersion or 0,					
--		uRandSeed or 0,					随机种子
--		bForceBind,						强制绑定默认0
--		bTimeOut,						是否会超时
-- 		bMsg,							是否消息通知
--}	

function _KLuaPlayer.AddItemEx(nGenre, nDetail, nParticular, nLevel, nWay)
	
	nGenre 		= tonumber(nGenre) or 0;
	nDetail 	= tonumber(nDetail) or 0;
	nParticular = tonumber(nParticular) or 0;
	nLevel 		= tonumber(nLevel) or 0;
	
	return	KItem.AddPlayerItem(
		self,
		nGenre,
		nDetail,
		nParticular,
		nLevel,
		1
	);
end

end

