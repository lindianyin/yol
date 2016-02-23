-- File       : box.lua
-- Creator    : simon
-- Date       : 2010-03-17 15:23:30
-- Description: 样本Doodad

if not MODULE_GAMESERVER then
	return
end

Include("script/doodad/doodad.lua")

local tbDoodad = Doodad:GetClass("box")

-- 虽然是OnDialog，但不限于对话的表现形式，可以是开箱动画等。
function tbDoodad:OnDialog()
	local tbOpt = 
	{
		{Lang.doodad.str1[Lang.Idx], self.OnOpen, self, it},
		{Lang.doodad.str2[Lang.Idx], self.TestDestroy, self, it},
		{Lang.doodad.str3[Lang.Idx]},
	}
	Dialog:Say(Lang.doodad.str4[Lang.Idx], tbOpt)
end

function tbDoodad:OnOpen(pOriginalDoodad)
	Dialog:Say(Lang.doodad.str5[Lang.Idx], tbOpt)
	me.KilledBy(him)
end

function tbDoodad:TestDestroy(pOriginalDoodad)
	KGameBase.RemoveDoodad(pOriginalDoodad.dwId) -- 消失
end
