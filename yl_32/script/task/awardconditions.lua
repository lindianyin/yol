function TaskAwardCond:None()
	return 1;
end

--------------------------------------------------------------
-- ÄÐÐÔ
function TaskAwardCond:IsMale()
	if (me.GetSex() == 0) then
		return 1;
	end
end

-- Å®ÐÔ
function TaskAwardCond:IsFemale()
	if (me.GetSex() == 1) then
		return 1;
	end
end
--------------------------------------------------------------

-- ÎÞÉÏÐùÔ¯

function TaskAwardCond:JianshenWushang()
	if (me.GetPrimaryFaction() == Character.FACTION_WUSHANG and me.GetCurrentRoute() == Character.ROUTE_WUSHANG_JIANSHEN) then
		return 1;
	end
end

function TaskAwardCond:YuheWushang()
  	if (me.GetPrimaryFaction() == Character.FACTION_WUSHANG and me.GetCurrentRoute() == Character.ROUTE_WUSHANG_YUHE) then
		return 1;
	end
end


-- ÖïÐ°Ììê×

function TaskAwardCond:FanghuTianyao()
	if (me.GetPrimaryFaction() == Character.FACTION_TIANYAO and me.GetCurrentRoute() == Character.ROUTE_TIANYAO_FANGHU) then
		return 1;
	end
end

function TaskAwardCond:ShenshengTianyao()
  	if (me.GetPrimaryFaction() == Character.FACTION_TIANYAO and me.GetCurrentRoute() == Character.ROUTE_TIANYAO_SHENSHENG) then
		return 1;
	end
end


-- ³àÁ¶ÓÄÚ¤

function TaskAwardCond:WenguYouming()
	if (me.GetPrimaryFaction() == Character.FACTION_YOUMING and me.GetCurrentRoute() == Character.ROUTE_YOUMING_WENGU) then
		return 1;
	end
end

function TaskAwardCond:ChongjiYouming()
  	if (me.GetPrimaryFaction() == Character.FACTION_YOUMING and me.GetCurrentRoute() == Character.ROUTE_YOUMING_CHONGJI) then
		return 1;
	end
end

-- ÷ÈÓ°É­ÂÞ

function TaskAwardCond:ShoulieSenluo()
  	if (me.GetPrimaryFaction() == Character.FACTION_SENLUO and me.GetCurrentRoute() == Character.ROUTE_SENLUO_SHOULIE) then
		return 1;
	end
end

function TaskAwardCond:CishaSenluo()
	if (me.GetPrimaryFaction() == Character.FACTION_SENLUO and me.GetCurrentRoute() == Character.ROUTE_SENLUO_CISHA) then
		return 1;
	end
end