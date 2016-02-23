
--Ìí¼Ó³ÆºÅ
function Title:AddTitleToCharacter(pPlayer, nGetWay, nLevel, nType, dwPassTime)
	if pPlayer == nil then
		return;
	end
	
	if not dwPassTime then
		dwPassTime = 0;
	end
	
	pPlayer.pTitle.AddTitleToCharacter(nGetWay, nLevel, nType, dwPassTime);
end

--É¾³ý³ÆºÅ
function Title:RemoveTitleToCharacter(pPlayer, nGetWay, nLevel, nType)
	if pPlayer == nil then
		return;
	end
	
	pPlayer.pTitle.RemoveTitleToCharacter(nGetWay, nLevel, nType);
end

--Çå¿Õ³ÆºÅ -test--
function Title:ClearTitleToCharacter(pPlayer)
	if pPlayer == nil then
		return;
	end
	pPlayer.SetTitleCarry1(0);
	pPlayer.SetTitleCarry2(0);
	pPlayer.SetTitleCarry3(0);
	pPlayer.pTitle.ClearTitle();
	pPlayer.pTitle.SnycTitle();
end

--ÕªÏÂ³ÆºÅ
function Title:OffTitle(pPlayer, nTitleId)
	local nTileCarry_1 = pPlayer.GetTitleCarry1();
	local nTileCarry_2 = pPlayer.GetTitleCarry2();
	local nTileCarry_3 = pPlayer.GetTitleCarry3();
	
	if nTileCarry_1 == nTitleId then
		pPlayer.SetTitleCarry1(0);
	elseif nTileCarry_2 == nTitleId then
		pPlayer.SetTitleCarry2(0);
	elseif nTileCarry_3 == nTitleId then
		pPlayer.SetTitleCarry3(0);
	end
end


--´øÉÏ³ÆºÅ
function Title:OnTitle(pPlayer, nTitleId)
	local nTileCarry_1 = pPlayer.GetTitleCarry1();
	local nTileCarry_2 = pPlayer.GetTitleCarry2();
	local nTileCarry_3 = pPlayer.GetTitleCarry3();
	
	if nTileCarry_1 == 0 then
		pPlayer.SetTitleCarry1(nTitleId);
	elseif nTileCarry_2 == 0 then
		pPlayer.SetTitleCarry2(nTitleId);
	elseif nTileCarry_3 == 0 then
		pPlayer.SetTitleCarry3(nTitleId);
	else
		pPlayer.SendBlackMsg(Lang.title.str1[Lang.Idx]);
	end
end

--¼ÆËãÕ½¶·Á¦
function Title:CalcFightScore()
end

function Title:c2s_OffTitle(pPlayer, nTitleId)
	Title:OffTitle(pPlayer, nTitleId);
end

function Title:c2s_OnTitle(pPlayer, nTitleId)
	Title:OnTitle(pPlayer, nTitleId);
end
--?gs Title:c2s_SyncTitle(me)
function Title:c2s_SyncTitle(pPlayer)
	if pPlayer == nil then
		return;
	end
	pPlayer.pTitle.SnycTitle();
end
