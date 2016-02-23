
---- 给予界面基类

------------------------------------------------------------------------------------------
-- initialize

Gift._szTitle	= "";						-- 标题文字
Gift._szContent	= "";						-- 内容信息

-- 创建新的给予界面对象
function Gift:New()
	return	Lib:NewClass(self);
end

------------------------------------------------------------------------------------------
-- common method

function Gift:SetTitle(szTitle)
	self._szTitle = szTitle;
end

function Gift:SetContent(szContent)
	self._szContent = szContent;
end

function Gift:GetCells()			-- 获得容器格子
	local tbCells = {};
	for j = 0, Item.ROOM_GIFT_HEIGHT - 1 do
		tbCells[i] = {};
		for i = 0, Item.ROOM_GIFT_WIDTH - 1 do
			tbCells[i][j] = me.GetGiftItem(i, j);
		end
	end
	return	tbCells;
end

function Gift:First()				-- 获得容器中第一个道具
	return	self:Find(0, 0);
end

function Gift:Next()				-- 获得容器下一个道具
	local nX = self._nX + 1;
	local nY = self._nY;
	if (nX >= Item.ROOM_GIFT_WIDTH) then
		nX = 0;
		nY = nY + 1;
	end
	if (nY >= Item.ROOM_GIFT_HEIGHT) then
		return	nil;
	end
	return	self:Find(nX, nY);
end

function Gift:LastX()				-- 最后一次找到道具的X坐标
	return	self._nX;
end

function Gift:LastY()				-- 最后一次找到道具的Y坐标
	return	self._nY;
end

------------------------------------------------------------------------------------------
-- client callback interface

function Gift:OnSwitch(pPickItem, pDropItem, nX, nY)	-- 当向容器里放入东西
	return	1;
end

function Gift:OnUpdate()			-- 更新界面文字（在打开界面和发生变化时调用）
	
end

------------------------------------------------------------------------------------------
-- server callback interface

function Gift:OnOK()				-- 确定

end

function Gift:OnCancel()			-- 取消

end

------------------------------------------------------------------------------------------
-- private method

function Gift:Find(nX, nY)			-- 在容器中查找一个道具

	local pItem = nil;

	for j = nY, Item.ROOM_GIFT_HEIGHT - 1 do
		if (j > nY) then
			nX = 0;
		end
		for i = nX, Item.ROOM_GIFT_WIDTH - 1 do
			pItem = me.GetGiftItem(i, j);
			if (pItem) then
				self._nX = i;
				self._nY = j;
				break;
			end
		end
		if (pItem) then
			break;
		end
	end

	if (not pItem) then
		return	nil;				-- 什么都没找到
	end

	return pItem;

end
