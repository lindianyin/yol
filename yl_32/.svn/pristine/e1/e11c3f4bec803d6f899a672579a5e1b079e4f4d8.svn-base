-- 文件名　：
-- 创建者　：
-- 创建时间：
-- 描述：须弥幻境1期，虫王菠萝丁



-----------------虫王菠萝丁------------------
Include("\\script\\mission\\level\\room_base.lua");

XoyoGame.RoomBoLuoDing = Lib:NewClass(XoyoGame.BaseRoom);

local RoomBoLuoDing = XoyoGame.RoomBoLuoDing;


function RoomBoLuoDing:OnInitRoom()
	self.tbBlood = {};
end

function RoomBoLuoDing:RecordBlood(szGroup)
	if not self.tbNpcGroup[szGroup] then
		return 0;
	end
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		local nLife = pNpc.nCurLife;
		local nMaxLife = pNpc.nMaxLife;
		if nLife and nLife > 0 then
			self.tbBlood[szGroup] = (nLife / nMaxLife) * 100;
			break;
		end	
	end
end

function RoomBoLuoDing:SetNpcBlood(szGroup)
	if szGroup == "boss2" then
		self:SetNpcLife(szGroup,self.tbBlood["boss1"] or 100);
	elseif szGroup == "boss1" then
		self:SetNpcLife(szGroup,self.tbBlood["boss2"] or 100);
	end
end

function RoomBoLuoDing:CalDisBetweenPlayers(szGroup,nDis,nLockToClose,nLockToUnLock)
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		local tbThreatList = pNpc.GetThreatList();
		local nDistance;
		if not tbThreatList or #tbThreatList <= 1 then
			XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToClose].Close();
			return;
		else
			local tbPos = pNpc.GetPosition();
			for _, nId in pairs(tbThreatList) do
				pPlayer = KGameBase.GetPlayerById(nId);
				local tbPlayerPos = pPlayer.GetPosition();
			end
			for i = 1 , #tbPlayerPos do
				for j = i + 1 , #tbPlayerPos do
					nDistance = (tbPlayerPos[i].nX - tbPlayerPos[j].nX) ^ 2 + (tbPlayerPos[i].nY - tbPlayerPos[j].nY) ^ 2 + (tbPlayerPos[i].nZ - tbPlayerPos[j].nZ) ^ 2;
					if nDistance >= (nDis^2) then
						XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToClose].Close();
						return;
					end
				end
			end
			XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToClose].Close();
			XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToUnLock].UnLock();
			return;
		end
	end
end

function RoomBoLuoDing:CalDisBetweenPlayers2(szGroup,nDis,nLockToClose,nLockToUnLock)
	local tbPlayerPos = {};
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		local tbThreatList = pNpc.GetThreatList();
		local nDistance;
		if not tbThreatList or #tbThreatList <= 1 then
			XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToClose].Close();
			return;
		else
			local tbPos = pNpc.GetPosition();
			for _, nId in pairs(tbThreatList) do
				pPlayer = KGameBase.GetPlayerById(nId);
				tbPlayerPos[#tbPlayerPos + 1] = pPlayer.GetPosition();
			end
			for i = 1 , #tbPlayerPos do
				for j = i + 1 , #tbPlayerPos do
					nDistance = (tbPlayerPos[i].nX - tbPlayerPos[j].nX) ^ 2 + (tbPlayerPos[i].nY - tbPlayerPos[j].nY) ^ 2 + (tbPlayerPos[i].nZ - tbPlayerPos[j].nZ) ^ 2;
					if nDistance >= (nDis^2) then
						XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToClose].Close();
						XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToUnLock].UnLock();
						return;
					end
				end
			end
			XoyoGame.RoomSetting.tbRoom[5].tbLock[nLockToClose].Close();
			return;
		end
	end
end

function RoomBoLuoDing:CopyThreatList(szFromGroup,szToGroup)
	local pNpc;
	for _, nId in pairs(self.tbNpcGroup[szFromGroup]) do
		pNpc = KGameBase.GetNpcById(nId);
	end
	for _, nId in pairs(self.tbNpcGroup[szToGroup]) do
		pNpc.CopyThreatList(nId);
	end
end
	