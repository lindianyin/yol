-- 宠物岛, 玩家随机出的宠物缓存
-- PetIsland.IslandPetsCache = {}
-- PetIsland.tbConfigFile = {} -- 读取的配置文件
PetIsland.nPetIslandMapId = 233	-- 宠物岛地图编号
-- PetIsland.tbPlayerLeavePos = {}  -- 玩家进入前位置
PetIsland.tbPlayers = {}  -- 玩家信息保存
PetIsland.tbPlayerNpcs = {}  -- 宠物dwId逻辑id保存, 对应tbPlayerPetDefs里的宠物集
PetIsland.tbPlayerPetDefs = {}  -- 玩家随机出是哪几只宠物的templateid : {npc编号，宠物id，品质}
PetIsland.tbPlayerPetQuality = {} -- 保存玩家选择的是哪个品质的宠物
PetIsland.PETCARD_ITEMID_BEGIN   = 10         -- 神仙卡道具的起始ID(Particular)
PetIsland.PETCARD_ITEMID_END     = 177        -- 神仙卡道具的结束ID(Particular)
PetIsland.PETCARD_NUM_ONEQUALITY = 28         -- 单个品质的神仙卡数量

-- 宠物的NPC范围
PetIsland.nPetMinNpcId = 500  -- 宠物最小索引
PetIsland.nPetMaxNpcId = 158

PetIsland.tbPetCardsRefreshed = {} -- 刷出来的神仙卡
PetIsland.tbPetCardsGet = {}       -- 抽取到的神仙卡

PetIsland.tbPetDefs = {}   -- 所有的静态宠物和战斗宠物映射, 读表进去  :  [品质] =>{需求等级=1, 宠物定义={静态npc，战斗npc，宠物id，品质}}
PetIsland.tbGDPL_JuLingQiu = { 24, 3, 1 }; -- 聚灵球
PetIsland.tbPetNumOfNextLevel =            -- 幸运值对应的高品质神仙数量，即达到某个幸运值后刷出的10个神仙位中有几个可能会刷出高品神仙
{
   [0]  = 1,           -- 在幸运值达到一半前，只可能出现一个高品神仙
   [50] = 2,
   [60] = 3,
   [65] = 4,
   [70] = 5,
   [75] = 6,
   [80] = 7,
   [85] = 8,
   [90] = 9,
   [95] = 10
}

PetIsland.tbLastRefreshTime = {} 
PetIsland.tbGetPetCardCost = {}      -- 不同品质抽牌消耗元宝
PetIsland.tbRefreshPetCardCost = {}  -- 不同品质翻牌消耗元宝

-- 获取宠物岛刷新成功率默认值, C++获取
function PetIsland:GetDefaultRefreshRates()
	return {100, 70, 50, 30, 20 ,5}
end

-- 宠物岛初始化, 从C++调用, 读表
function PetIsland:OnInit()
	local tbFile = Lib:LoadTabFile("setting/pet/pet_island.txt");

	if not tbFile then
		return;
	end

	-- self.tbConfigFile = tbFile -- 配置文件保存起来

	for i=2, #tbFile do
		local nQuality = tonumber(tbFile[i].Quality)
		local tbPetDefItem = {}
		if (self.tbPetDefs[nQuality] == nil) then
			self.tbPetDefs[nQuality] = tbPetDefItem  -- 初始化品质表
			tbPetDefItem.RequireLevel = tonumber(tbFile[i].RequireLevel) -- 等级需求
			tbPetDefItem.nMaxLuckyValue = tonumber(tbFile[i].MaxLuckyValue) -- 此阶段的最大幸运值
			tbPetDefItem.nLuckyValueAdd = tonumber(tbFile[i].LuckyValueAdd) -- 此阶段刷新一次增加的幸运值
			tbPetDefItem.UpRateCostItem = tonumber(tbFile[i].UpRateCostItem) -- 提升刷新率聚灵丘消耗
			tbPetDefItem.PetDefs = {} -- 初始化宠物容器
		else
			tbPetDefItem = self.tbPetDefs[nQuality]  -- 这个品质已经有初始化了，直接用
		end

		-- 宠物读取
		local tbPets = Lib:SplitStr(tbFile[i].Pets, ",")
		for index, szPetId in pairs(tbPets) do
		  szPetId = string.gsub(szPetId, '"', '')
			local nPetType = tonumber(szPetId)
			--local nPetNpcId = KPet.GetPetNpcIdByType(nPetType) -- 通过Pet表获取对应Npc
			tbPetDefItem.PetDefs[index] = nPetType;
			-- 宠物读取 
			--[[table.insert(tbPetDefItem.PetDefs, {
		 		nPetNpcId, -- 静态宠物
		 		nPetType, -- 宠物模板ID
			})--]]
		end

    local tbGetPetCostStr = Lib:SplitStr(tbFile[i].GetPetCardCost, ",")
    local tbGetPetCost = {}
    for index, szPetCost in pairs(tbGetPetCostStr) do
      szPetCost = string.gsub(szPetCost, '"', '')
    	tbGetPetCost[index] = tonumber(szPetCost)
    end
    
    self.tbGetPetCardCost[nQuality] = tbGetPetCost
    self.tbRefreshPetCardCost[nQuality] = tonumber(tbFile[i].RefreshPetCardCost);
	end
	
	-- 读取神仙对应的道具id
	--[[tbFile = Lib:LoadTabFile("setting/pet/petitems.txt");

	if not tbFile then
		return;
	end
	
	self.tbPetItemsMap = {}
  for i=2, #tbFile do
		local nPetID = tonumber(tbFile[i].PetID)
		local tbPets = self.tbPetItemsMap[nPetId]
		if tbPets == nil then
		    tbPets = {}
		    self.tbPetItemsMap[nPetId] = tbPets
		end
		
		local nPetNature = tonumber(tbFile[i].PetNature)
		tbFile[i].PetItem = string.gsub(tbFile[i].PetItem, '"', '')
    local tbPetItemIdStr = Lib:SplitStr(tbFile[i].PetItem, ",")
		local tbPetItemId = {}
		tbPetItemId.nGenre = tonumber(tbPetItemIdStr[1])
		tbPetItemId.nDetail = tonumber(tbPetItemIdStr[2])
		tbPetItemId.nParticular = tonumber(tbPetItemIdStr[3])
		tbPetItemId.nLevel = tonumber(tbPetItemIdStr[4])
		
		tbPets[nPetNature] = tbPetItemId
	end	--]]
	
	-- 读取神仙卡牌合成道具表
	tbFile = Lib:LoadTabFile("setting/pet/petcard_compose.txt");

	if not tbFile then
		return;
	end
	
	-- 读取神仙卡牌合成表
	self.tbPetCardComposeMap = {}
  for i=2, #tbFile do
    local tbComposeItem = {}
  
  	local tbItem1 = {}
  	tbFile[i].Item1ID = string.gsub(tbFile[i].Item1ID, '"', '')
  	local tbPetItemIdStr = Lib:SplitStr(tbFile[i].Item1ID, ",")
		tbItem1.nItem1ID  = {}
  	tbItem1.nItem1ID.nGenre = tonumber(tbPetItemIdStr[1])
  	tbItem1.nItem1ID.nDetail = tonumber(tbPetItemIdStr[2])
  	tbItem1.nItem1ID.nParticular = tonumber(tbPetItemIdStr[3])
  	tbItem1.nItem1Rate = tonumber(tbFile[i].Item1Rate)
  	tbItem1.nItem1MaxNum = tonumber(tbFile[i].Item1MaxNum)
  	tbComposeItem[1] = tbItem1
  	
  	local tbItem2 = {}
  	tbFile[i].Item2ID = string.gsub(tbFile[i].Item2ID, '"', '')
  	tbPetItemIdStr = Lib:SplitStr(tbFile[i].Item2ID, ",")
  	tbItem2.nItem2ID = {}
  	tbItem2.nItem2ID.nGenre = tonumber(tbPetItemIdStr[1])
  	tbItem2.nItem2ID.nDetail = tonumber(tbPetItemIdStr[2])
  	tbItem2.nItem2ID.nParticular = tonumber(tbPetItemIdStr[3])
  	tbItem2.nItem2Rate = tonumber(tbFile[i].Item2Rate)
  	tbItem2.nItem2MaxNum = tonumber(tbFile[i].Item2MaxNum)
  	tbComposeItem[2] = tbItem2
  	
  	local tbItem3 = {}
  	tbFile[i].Item3ID = string.gsub(tbFile[i].Item3ID, '"', '')
  	tbPetItemIdStr = Lib:SplitStr(tbFile[i].Item3ID, ",")
  	tbItem3.nItem3ID = {}
  	tbItem3.nItem3ID.nGenre = tonumber(tbPetItemIdStr[1])
  	tbItem3.nItem3ID.nDetail = tonumber(tbPetItemIdStr[2])
  	tbItem3.nItem3ID.nParticular = tonumber(tbPetItemIdStr[3])
  	tbItem3.nItem3Rate = tonumber(tbFile[i].Item3Rate)
  	tbItem3.nItem3MaxNum = tonumber(tbFile[i].Item3MaxNum)
  	tbComposeItem[3] = tbItem3
  	
  	local nQuality = tonumber(tbFile[i].Quality)
  	self.tbPetCardComposeMap[nQuality] = tbComposeItem
	end	
end
-- ====================================================

function PetIsland:ComposePetCard(pPlayer, tbPetIds)
	 local nItem1Num = 0
	 local nItem2Num = 0
	 local tbComposeItem = {}
	 for _, nPetId in pairs(tbPetIds) do	    
	    if (nPetId > 0) then
		    local pPet = KPet.GetPetById(nPetId);
				if pPet == nil then
				    return 
				end
				
				--KPet.FreePet(nPetId)
				
		    local nQuality = pPet.nQuality
		     
		    tbComposeItem = self.tbPetCardComposeMap[nQuality]
		    local tbItem1 = tbComposeItem[1]
		    nItem1Num = nItem1Num + tbItem1.nItem1Rate
		     
		    local tbItem2 = tbComposeItem[2]
		    nItem2Num = nItem2Num + tbItem2.nItem2Rate    
	    end
	 end
	 
	 nItem1Num = math.floor(nItem1Num / 1000)
	 nItem2Num = math.floor(nItem2Num / 1000)
	 
	 -- 检查背包空间
	 local nNeedCellNum = 0;
	 if nItem1Num > 0 then
	     nNeedCellNum = nNeedCellNum + 1
	 end
	 if nItem2Num > 0 then
	     nNeedCellNum = nNeedCellNum + 1
	 end
	 if pPlayer.pItem.CountFreeBagCell() < nNeedCellNum then
			pPlayer.SendBlackMsg(Lang.pet.str36[Lang.Idx]);
			return
	 end
	 
	 if nItem1Num > 0 then
	    KItem.AddPlayerItem(pPlayer, tbComposeItem[1].nItem1ID.nGenre, tbComposeItem[1].nItem1ID.nDetail, tbComposeItem[1].nItem1ID.nParticular, 0, nItem1Num) 
	 end
	 if nItem2Num > 0 then
	    KItem.AddPlayerItem(pPlayer, tbComposeItem[2].nItem2ID.nGenre, tbComposeItem[2].nItem2ID.nDetail, tbComposeItem[2].nItem2ID.nParticular, 0, nItem2Num) 
	 end
	 
	 local nComposeCount = pPlayer.pTask.GetTask(5002, 1)
	 pPlayer.pTask.SetTask(5001, 2, nComposeCount + 1);

   -- 释放合成的神仙	 
 	 for _, nPetId in pairs(tbPetIds) do	    
    if (nPetId > 0) then
	    local pPet = KPet.GetPetById(nPetId);
			if pPet == nil then
			    return 
			end
			
			KPet.FreePet(nPetId)
		end
   end
   
   pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:PlayerComposeItemAnim", nItem1Num, nItem2Num});
end

-- 进入宠物岛, 创建NPC
function PetIsland:OnInstanceInit(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	if pPlayer.IsNpc() == 1 then
		return
	end

	-- 如果不是玩家进入到宠物岛，不作为
	local nMapId = pPlayer.dwSceneTemplateId;
	-- KScene.GetSceneType(nMapId) ~= 1 or
	if nMapId ~= self.nPetIslandMapId then--self.tbSetting[nMapId] == nil then
		return;
	end

	-- 记录玩家
	self.tbPlayers[pPlayer.dwId] = {
		dwSceneId = pPlayer.dwSceneId,  -- 场景记录
		bWin = 0,  -- 是否已战胜宠物
		bFighting = 0, -- 是否正在跟宠物打架
		nKillCount = 0  -- 杀死宠物多少次，需要4次
	}

	-- 玩家本人进入宠物岛，开始刷新npc
	self:RefreshLastPet(pPlayer); -- 检查有没上次刷出的神仙
end

-- 宠物岛反初始化
function PetIsland:OnInstanceUnInit(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	if (pPlayer ~= nil) and (pPlayer.IsNpc() == 1) then
		return
	end

	-- 如果不是玩家进入到宠物岛，不作为
	local nMapId = pPlayer.dwSceneTemplateId;
	-- KScene.GetSceneType(nMapId) ~= 1 or
	if nMapId ~= self.nPetIslandMapId then--self.tbSetting[nMapId] == nil then
		return;
	end

	self.tbPlayers[pPlayer.dwId] = nil;
	self.tbPlayerPetDefs[pPlayer.dwId] = nil
	self.tbPlayerNpcs[pPlayer.dwId] = nil
	self.tbPlayerPetQuality[pPlayer.dwId] = nil
end

-- 进入宠物岛
function PetIsland:EnterPetIsland(pPlayer)
	--pPlayer.TeleportTo(self.nPetIslandMapId, 2042,260,1048576)
	Instance:TryEnterInstance({pPlayer.dwId}, self.nPetIslandMapId);
	self:__ClientShowIslandPanel(pPlayer) -- 宠物岛UI面板
end

-- 提升刷成宠物成功率
function PetIsland:UpRefreshRate(pPlayer, nQuality)
	local nNowRate = KPet.GetPetIslandRefreshRate(pPlayer, nQuality)
	if nNowRate < 100 then  -- 概率大于等于100，不理
		-- TODO: 扣钱
		local tbCostGDPL = self.tbGDPL_JuLingQiu -- 聚灵球
		local nCostNum = tonumber(self.tbPetDefs[nQuality].UpRateCostItem);  -- 需要消耗多少个聚灵球
		local tbFind = me.pItem.FindItem(Item.ROOM_PET, unpack(tbCostGDPL));
		if (Pet:GetTotalCount(tbFind) < nCostNum) then
			local szErrMsg = string.format(Lang.pet.str37[Lang.Idx] , nCostNum);
			me.SysMsg(szErrMsg);
			return;
		end
		me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));

		KPet.SetPetIslandRefreshRate(pPlayer, nQuality, nNowRate+1)
	else
		pPlayer.SysMsg(Lang.pet.str38[Lang.Idx])
	end
end

function PetIsland:RefreshLastPet(pPlayer)
	local tbLastPetInfo = KPet.GetPetIslandLastInfo(pPlayer);
	if tbLastPetInfo.nLastIndex ~= 0  then
		local nQuality = tbLastPetInfo.nLastQuality;
		local nNature = tbLastPetInfo.nLastNature;
		
		-- 获取场景的位置信息~ 取任意一个npc的PosFile  # = 4
		print("PetIsland:RefreshLastPet", pPlayer.dwSceneTemplateId, self.nPetMinNpcId, Scene.tbNpcCfg[pPlayer.dwSceneTemplateId][self.nPetMinNpcId])
		local tbPosList = Scene.tbNpcCfg[pPlayer.dwSceneTemplateId][self.nPetMinNpcId].tbPosList["nantianmen"];
	
		-- 刷新静态宠物
		local dwSceneId = pPlayer.dwSceneId;
		local tbPetDefs = self.tbPetDefs[nQuality].PetDefs  -- 获取指定品质4个可用宠物
		
		local tbPetDef2 = tbPetDefs[tbLastPetInfo.nLastIndex]-- 随机取1个
		tbPetDef2[4] = nNature
	
		self.tbPlayerPetDefs[pPlayer.dwId] = tbPetDef2  -- 保存玩家的随出宠物
		self.tbPlayerPetQuality[pPlayer.dwId] = nQuality -- 玩家选择的宠物品质
	
		-- 开始创建宠物
		local tbPos = tbPosList[1]--[nIdx]  -- 获取位置, 宠物定位
		local nNpcTemplateId = tbPetDef2[1]  -- 宠物NPC
		local nPetTemplateId = tbPetDef2[2] -- 宠物模板id
		local nPetNature = tbPetDef2[4] -- 宠物性格
		self:__AddPet(pPlayer, nNpcTemplateId, nQuality, nPetNature, tbPos)  -- 循环添加宠物
		
		-- 传递刷出的宠物的信息
		if tbLastPetInfo.nLastFightState == 0 then
	      self:__ClientPetInfo(pPlayer, nPetTemplateId, nQuality, nPetNature)
	    else 
	    	self:StartFight(pPlayer);
	    end
    end
end

-- 刷新宠物 (静态宠物保存起来，战斗宠物拿保存的战斗模板ID出来创建)
function PetIsland:c2s_RefreshPets(pPlayer, Decuple)
	-- 有一定成功概率
 	--[[local tbRefreshData = KAward.GetNumericalAwardData("PetIsland", 1, nQuality); --KPet.GetPetIslandRefreshRate(pPlayer, nQuality);
 	local nHavePrestige = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_PRESTIGE);
	if nHavePrestige < tbRefreshData.nCost then
		-- 声望不足
		pPlayer.SendBlackMsg("声望不足");
		return
	end
  
  -- 扣除声望
  pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_PRESTIGE, -tbRefreshData.nCost);--]]

	local nNowTime = KGameBase.GetTime();	
	if self.tbLastRefreshTime[pPlayer.dwId] ~= nil then
	    local nLastTime = self.tbLastRefreshTime[pPlayer.dwId];
	    if nNowTime - nLastTime < 1 then   -- 如果小于1秒内连续发送，可能是外挂
	        pPlayer.SendBlackMsg(Lang.pet.str39[Lang.Idx] );
	        return;
	    end
	else
	    self.tbLastRefreshTime[pPlayer.dwId] = nNowTime;
	end
	
  local tbPetIslandInfo = KPet.GetPetIslandLastInfo(pPlayer);    
  local nQuality = tbPetIslandInfo.nCurrentQuality;
  
  local nTimes = Player.tbDegree:GetDegree(pPlayer, "RefreshPetCardTimes")
  if nTimes <= 0 then -- 次数不足时消耗元宝
     local nCost = self.tbRefreshPetCardCost[nQuality]
     if Decuple == 1 then
         nCost = nCost * 10;
     end
     local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN)
     if nCurCoin < nCost then
         pPlayer.SendBlackMsg(Lang.pet.str40[Lang.Idx] )
         return
     end
     
     pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -nCost);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nCost,103);
  else
     Player.tbDegree:ReduceDegree(pPlayer, "RefreshPetCardTimes")
  end
  
  local nRefreshCount = pPlayer.pTask.GetTask(5001, 1)
	pPlayer.pTask.SetTask(5001, 1, nRefreshCount + 1);
	 
  local nCurrentLuckyValue = tbPetIslandInfo.nCurrentLuckyValue;
  
  if nQuality < 6 then
    local nAddValue = self.tbPetDefs[nQuality].nLuckyValueAdd;
    if Decuple == 1 then  -- 10倍幸运值
        nAddValue = nAddValue * 10
    end
    
	  nCurrentLuckyValue = nCurrentLuckyValue + nAddValue;
	  if nCurrentLuckyValue > self.tbPetDefs[nQuality].nMaxLuckyValue then
	      nCurrentLuckyValue = 0
	      nQuality = nQuality + 1
	  end
  end
  
  local nKeyLuckyValue = 0
  local nLuckyProgress = (tbPetIslandInfo.nCurrentLuckyValue / self.tbPetDefs[nQuality].nMaxLuckyValue) * 100;
  for lv, num in pairs(self.tbPetNumOfNextLevel) do
      if nLuckyProgress > lv then
          nKeyLuckyValue = lv;
      end
  end
  
  local tbRefreshedPets = {}
  local tbPetCardPoses = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
  
  -- 小概率爆高2品神仙
  local isHit = KUnify.MathRandom(1, 100) --生成随机数
  if (nRefreshCount == 2 or isHit == 1) and nQuality < 5 then
  --if (nRefreshCount == 2 or isHit > 50) and nQuality < 5 then
      local tbPetDef2 = {}
      
      local tbPetDefs = self.tbPetDefs[nQuality + 2].PetDefs  -- 获取指定品质4个可用宠物
	    local nRand = KUnify.MathRandom(#tbPetDefs) --生成随机数
			tbPetDef2[1] = tbPetDefs[nRand]-- 随机取1个
			tbPetDef2[2] = KUnify.MathRandom(1,7)  -- 随机性格, 加入
			
			local index = KUnify.MathRandom(#tbPetCardPoses)
			tbRefreshedPets[tbPetCardPoses[index] * 2 - 1] = tbPetDef2[1]
			tbRefreshedPets[tbPetCardPoses[index] * 2] = tbPetDef2[2]
			
			self.higherQualityPet = tbPetDef2[1]
			
			table.remove(tbPetCardPoses, index)
  end

  -- 先刷新高品质神仙
  local nNextLevelPetNum = self.tbPetNumOfNextLevel[nKeyLuckyValue]

  for i = 1, nNextLevelPetNum do    
    local nRand = KUnify.MathRandom(self.tbPetDefs[nQuality].nMaxLuckyValue)
    if nRand < nCurrentLuckyValue then
      local tbPetDef2 = {}
      
	    local tbPetDefs = self.tbPetDefs[nQuality + 1].PetDefs  -- 获取指定品质4个可用宠物
	    nRand = KUnify.MathRandom(#tbPetDefs) --生成随机数
			tbPetDef2[1] = tbPetDefs[nRand]-- 随机取1个
			tbPetDef2[2] = KUnify.MathRandom(1,7)  -- 随机性格, 加入
			
			local index = KUnify.MathRandom(#tbPetCardPoses)
			tbRefreshedPets[tbPetCardPoses[index] * 2 - 1] = tbPetDef2[1]
			tbRefreshedPets[tbPetCardPoses[index] * 2] = tbPetDef2[2]
			
			table.remove(tbPetCardPoses, index)
		end
  end
  for _, key in pairs(tbPetCardPoses) do
    local tbPetDefs = self.tbPetDefs[nQuality].PetDefs  -- 获取指定品质4个可用宠物
    local nRand = KUnify.MathRandom(#tbPetDefs) --生成随机数
		local tbPetDef2 = {}
		tbPetDef2[1] = tbPetDefs[nRand]-- 随机取1个
		tbPetDef2[2] = KUnify.MathRandom(1,7)  -- 随机性格, 加入
		--tbPetDef2[3] = nQuality
		
		tbRefreshedPets[key * 2 - 1] = tbPetDef2[1]
		tbRefreshedPets[key * 2]     = tbPetDef2[2]
  end
  
  self.tbPetCardsRefreshed[pPlayer.dwId] = tbRefreshedPets;
  
  -- 将刷出的卡牌同步给客户端
  KPet.SendRefreshPetInfo(pPlayer, tbRefreshedPets);
	KPet.RecordPetIslandCurrentInfo(pPlayer, nCurrentLuckyValue, nQuality, 0, 0)
	-- 重置抽牌次数
	Player.tbDegree:SetDegree(pPlayer, "GetPetCardTimes", 3, 1);
	
	--[[self:__RemoveAllPets(pPlayer.dwId)  -- 移除当前岛上所有宠物

	-- 设置战斗状态
	self.tbPlayers[pPlayer.dwId].bFighting = 0
	self.tbPlayers[pPlayer.dwId].bWin = 0
	self.tbPlayers[pPlayer.dwId].nKillCount = 0

	-- 获取场景的位置信息~ 取任意一个npc的PosFile  # = 4
	local tbPosList = Scene.tbNpcCfg[pPlayer.dwSceneTemplateId][self.nPetMinNpcId].tbPosList["nantianmen"];

	-- 刷新静态宠物
	local dwSceneId = pPlayer.dwSceneId;
	local tbPetDefs = self.tbPetDefs[nQuality].PetDefs  -- 获取指定品质4个可用宠物

	local nRand = KUnify.MathRandom(#tbPetDefs) --生成随机数
	local tbPetDef2 = tbPetDefs[nRand]-- 随机取1个
	tbPetDef2[4] = KUnify.MathRandom(1,7)  -- 随机性格, 加入
	tbPetDef2[5] = nQuality

	self.tbPlayerPetDefs[pPlayer.dwId] = tbPetDef2  -- 保存玩家的随出宠物
	self.tbPlayerPetQuality[pPlayer.dwId] = nQuality -- 玩家选择的宠物品质

	-- 开始创建宠物
	local tbPos = tbPosList[1]--[nIdx]  -- 获取位置, 宠物定位
	local nNpcTemplateId = tbPetDef2[1]  -- 宠物NPC
	local nPetTemplateId = tbPetDef2[2] -- 宠物模板id
	local nPetNature = tbPetDef2[4] -- 宠物性格
	
	self:__AddPet(pPlayer, nNpcTemplateId, nQuality, nPetNature, tbPos)  -- 循环添加宠物

	-- 传递刷出的宠物的信息
	self:__ClientPetInfo(pPlayer, nPetTemplateId, nQuality, nPetNature)

    KPet.RecordPetIslandCurrentInfo(pPlayer, nRand, nQuality, nPetNature, 0); -- 保存此次信息--]]
end
-- 抽卡
function PetIsland:c2s_GetCard(pPlayer)
   print("..PetIsland:c2s_GetCard",pPlayer.dwId);
   local nTimes = Player.tbDegree:GetDegree(pPlayer, "GetPetCardTimes")
   local tbPetIslandInfo = KPet.GetPetIslandLastInfo(pPlayer);    
   local nQuality = tbPetIslandInfo.nCurrentQuality;
   local tbCards = self.tbPetCardsRefreshed[pPlayer.dwId]
   
   local tbPets = KPet.GetPlayerPetList(pPlayer);
	 local nPetCapacity = pPlayer.GetPetCapacity();
	 if #tbPets == nPetCapacity then
		 pPlayer.SendBlackMsg(Lang.pet.str41[Lang.Idx] );
		 return 0;
	 end
	
   if nTimes <= 0 then
     local tbCost = self.tbGetPetCardCost[nQuality]
     local nCost = tbCost[8 - #tbCards / 2]
       
     local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN)
     if nCurCoin < nCost then
         pPlayer.SendBlackMsg(Lang.pet.str42[Lang.Idx] )
         return
     end
     
     pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -nCost);
	 pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nCost,102);
   else
     Player.tbDegree:ReduceDegree(pPlayer, "GetPetCardTimes", 1)  
   end

   local nRandIndex = KUnify.MathRandom(#tbCards / 2)
   local nRefreshCount = pPlayer.pTask.GetTask(5001, 1)
   if nRefreshCount == 3 and (#tbCards / 2) > 2 then
       while (tbCards[nRandIndex * 2 - 1] == self.higherQualityPet) do
           nRandIndex = KUnify.MathRandom(#tbCards / 2)
       end
   end
   Lib:ShowTB(tbCards);
   print("..nRandIndex <= (#tbCards / 2) ",(nRandIndex <= (#tbCards / 2) ),nRandIndex,#tbCards,tbCards[nRandIndex * 2 - 1],self.higherQualityPet)
   if nRandIndex <= (#tbCards / 2) then
      self.tbPetCardsGet[#self.tbPetCardsGet] = tbCards[nRandIndex * 2 - 1]
      if(tbCards[nRandIndex * 2 - 1] == self.higherQualityPet) then
      		local tbFile = Lib:LoadTabFile("setting/pet/pet.txt");
      		local szMsg = Lang.mission.str1223[Lang.Idx];
      		local szQuality = "";
      		local szName = "";
      		for i=2,#tbFile do 
				if(tonumber(tbFile[i].PetId) == tbCards[nRandIndex * 2 - 1]) then 
					szQuality = Pet.QUALITY_TIPS[tonumber(tbFile[i].Quality)];
					szName = tbFile[i].Name;
					break;
				end
      		end
      		local szFormatMsg = string.format(szMsg,pPlayer.szName,szQuality,szName);
			KChat.BroadcastNoticeMsg(szFormatMsg);
			pPlayer.SysMsg(szFormatMsg);
      end
      self.tbPetCardsGet[#self.tbPetCardsGet + 1] = tbCards[nRandIndex * 2]
      table.remove(tbCards, nRandIndex * 2)
      table.remove(tbCards, nRandIndex * 2 - 1)
      print("PetIsland:c2s_GetCar2", #tbCards, #tbCards / 2, self.tbPetCardsGet[#self.tbPetCardsGet - 1], self.tbPetCardsGet[#self.tbPetCardsGet])
      
      Pet:AddPet(pPlayer, self.tbPetCardsGet[#self.tbPetCardsGet - 1], self.tbPetCardsGet[#self.tbPetCardsGet])
   end
   
   --DailyTasks:commitTaskValue(pPlayer,table.concat({self.tbPetCardsGet[#self.tbPetCardsGet - 1],self.tbPetCardsGet[#self.tbPetCardsGet]},"#"),DailyTasks.TYPE_TAKE_OUT_PET,1);
	DailyTasks:commitTaskValue(pPlayer,"",DailyTasks.TYPE_TAKE_OUT_PET,1);
   pPlayer.CallClientScript({"QClientScript.PetIsland:GetOnePetCard", nRandIndex});
end

-- 开始跟宠物战斗， 刷出战斗宠物
function PetIsland:StartFight(pPlayer)
	if self.tbPlayers[pPlayer.dwId].bFighting == 1 then -- 正在战斗
		return
	end

	-- 当前npc，阵营变换成3，可战斗
	if self.tbPlayerPetDefs[pPlayer.dwId] ~= nil then -- 刷新过
		if self.tbPlayerNpcs[pPlayer.dwId] ~= nil then -- 刷新出过npc
			for _, nId in pairs(self.tbPlayerNpcs[pPlayer.dwId]) do
				local pNpc = KGameBase.GetNpcById(nId);
				if pNpc and pNpc.IsDead() ~= 1 then
					--KGameBase.RemoveNpc(pNpc.dwId);
					pNpc.SetCurrentCamp(3)
				end
			end

			-- 标记玩家正在战斗
			self.tbPlayers[pPlayer.dwId].bWin = 0
			self.tbPlayers[pPlayer.dwId].bFighting = 1
			pPlayer.SendBlackMsg(Lang.pet.str43[Lang.Idx] )

			-- 标记玩家正在战斗
			self.tbPlayers[pPlayer.dwId].bWin = 0
			self.tbPlayers[pPlayer.dwId].bFighting = 1
		end
	else
		pPlayer.SysMsg(Lang.pet.str44[Lang.Idx] )
		return
	end

    KPet.RecordPetIslandCurrentInfo(pPlayer, -1, -1, -1, 1); -- 更新战斗状态信息, -1表示保留原来的值
end
-- 宠物岛神仙信息面板 s2c
function PetIsland:__ClientPetInfo(pPlayer, nNpcTemplateId, nQuality, nPetNature)
	pPlayer.CallClientScript({"QClientScript.PetIsland:ShowPetInfo", nNpcTemplateId, nQuality, nPetNature});
end
-- 宠物岛ui面板 s2c
function PetIsland:__ClientShowIslandPanel(pPlayer)
	pPlayer.CallClientScript({"QClientScript.PetIsland:OpenIslandPanel"});
end

-- 客户端显示宠物选择界面 s2c
function PetIsland:__ClientShowChoosePetPanel(pPlayer, nPetTemplateId, nNature)
	-- 传送4个宠物id
	pPlayer.CallClientScript({"QClientScript.PetIsland:OpenChoosePetPanel", nPetTemplateId, nNature});
end

-- 客户端npc性格偏色
function PetIsland:__ClientSetPetNpcPartialColor(pPlayer, pNpc, nNature)
	pPlayer.CallClientScript({"QClientScript.PetIsland:SetPetNpcPartialColor", pNpc.dwId, nNature});
end

-- 传入逻辑场景，场景模板，npc模板id
function PetIsland:__AddPet(pPlayer, nNpcTemplateId, nQuality, nNature, tbPos)
	local dwPlayerId = pPlayer.dwId
	local dwSceneId = pPlayer.dwSceneId
	local dwSceneTemplateId = pPlayer.dwSceneTemplateId

	-- npc的场景模板，位置信息
	local tbNpcT = Scene.tbNpcCfg[dwSceneTemplateId][self.nPetMinNpcId];
	local tbNpc = {}
	--tbNpc.tbPosList = tbNpcT.tbPosList
	tbNpc.nTemplateId = nNpcTemplateId; --tbNpcT.nTemplateId
	tbNpc.nRegenerationTime = tbNpcT.nRegenerationTime;
	tbNpc.nNormalAI = tbNpcT.nNormalAI;
	tbNpc.nCount = 1; -- 一个

	local tbRetNpcs = Scene:_CallOneNpc(tbNpc, dwSceneId, dwSceneTemplateId, tbPos)

	-- 宠物添加成功后保存引用
	for _, pNpc in ipairs(tbRetNpcs) do
	    -- KCharacter.ResetNpcIntensity(pNpc, nPropId, nLevel);
	    -- self:AddNpcInLock(pNpc, nLock);
		-- self:AddNpcInGroup(pNpc);
		-- table.insert(tbNpcId, pNpc.dwId);
		-- local tbTmp = pNpc.GetTempTable("Instance")
		-- tbTmp.tbInstance = self;
		if self.tbPlayerNpcs[pPlayer.dwId] == nil then
			self.tbPlayerNpcs[pPlayer.dwId] = {}
		end
		table.insert(self.tbPlayerNpcs[pPlayer.dwId], pNpc.dwId);
		self:__ClientSetPetNpcPartialColor(pPlayer, pNpc, nNature)
	end
end

-- 移除指定玩家宠物岛的所有宠物
function PetIsland:__RemoveAllPets(dwPlayerId)
	-- 删除原来的宠物
	if self.tbPlayerNpcs[dwPlayerId] ~= nil then
		for _, nId in pairs(self.tbPlayerNpcs[dwPlayerId]) do
			local pNpc = KGameBase.GetNpcById(nId);
			if pNpc and pNpc.IsDead() ~= 1 then
				KGameBase.RemoveNpc(pNpc.dwId);
			end
		end
	end
end

-- 宠物被杀,  me是Dead, him是killer
function PetIsland:OnDeath(pKiller, pDead)
	if pKiller.IsPlayer() then  -- 玩家杀宠物了
		if self.tbPlayers[pKiller.dwId] ~= nil then
			-- 判断玩家是否在战斗, 是的话，作处理
			if self.tbPlayers[pKiller.dwId].bFighting == 1 then
				-- 杀宠统计
				self.tbPlayers[pKiller.dwId].nKillCount = self.tbPlayers[pKiller.dwId].nKillCount + 1
				if self.tbPlayers[pKiller.dwId].nKillCount >= 1 then -- 所有宠物杀光，

					-- 标记战胜
					self.tbPlayers[pKiller.dwId].bWin = 1
					self.tbPlayers[pKiller.dwId].bFighting = 0
					self.tbPlayers[pKiller.dwId].nKillCount = 0  -- 归零

					-- 客户端出选择宠物界面, 并传递宠物id
					local tbPetDef2 = self.tbPlayerPetDefs[pKiller.dwId]
					self:__ClientShowChoosePetPanel(pKiller, tbPetDef2[2], tbPetDef2[4])  -- 宠物ID， 性格
					
					local nQuality = tbPetDef2[5]  
					if nQuality ==  3 then -- 从蓝品开始公告
					   KChat.BroadcastNoticeMsg("<"..pKiller.szName..Lang.pet.str45[Lang.Idx]..pDead.szName..">")
					elseif nQuality == 4 then
					   KChat.BroadcastNoticeMsg("<"..pKiller.szName..Lang.pet.str46[Lang.Idx]..pDead.szName..">")
					elseif nQuality == 5 then
					  KChat.BroadcastNoticeMsg("<"..pKiller.szName..Lang.pet.str47[Lang.Idx]..pDead.szName..">")
					elseif nQuality == 6 then
					  KChat.BroadcastNoticeMsg("<"..pKiller.szName..Lang.pet.str48[Lang.Idx]..pDead.szName..">")
					end
				end

			end
		end
	end

	if pDead.IsPlayer() then -- 玩家被宠物杀了
		if self.tbPlayers[pDead.dwId] ~= nil then
			self.tbPlayers[pDead.dwId].bWin = 0
			self.tbPlayers[pDead.dwId].bFighting = 0
			self.tbPlayers[pDead.dwId].nKillCount = 0  -- 归零
			pDead.SendBlackMsg(Lang.pet.str49[Lang.Idx])
			

			self.__RemoveAllPets(pDead.dwId) -- 删掉所有npc
		end
	end
end

-- 玩家登出时推出副本
function PetIsland:OnLogout(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	if (pPlayer ~= nil) and (pPlayer.IsNpc() == 1) then
		return
	end

	-- 如果不是玩家进入到宠物岛，不作为
	local nMapId = pPlayer.dwSceneTemplateId;
	-- KScene.GetSceneType(nMapId) ~= 1 or
	if nMapId ~= self.nPetIslandMapId then--self.tbSetting[nMapId] == nil then
		return;
	end

	Instance:LeaveMyInstance(pPlayer)
end

-- ==================================================== C2S
-- 请求进入宠物岛, 副本233
function PetIsland:c2s_EnterPetIsland(pPlayer)
	Instance:LeaveMyInstance(pPlayer)  -- 先确保没有副本
	self:EnterPetIsland(pPlayer)
end

-- 刷新npc, 副本2号锁
--[[function PetIsland:c2s_RefreshPets(pPlayer, nQuality)
	-- 是否满足等级，刷新该品质宠物
	local nRequireLevel = tonumber(self.tbPetDefs[nQuality].RequireLevel)
	if pPlayer.GetLevel() < nRequireLevel then
		pPlayer.SendBlackMsg(string.format("需要达到%d级才能召唤该品质的大神", nRequireLevel))
		return
	end

	self:RefreshPets(pPlayer, nQuality);
end--]]

-- 开始跟宠物战斗
function PetIsland:c2s_StartFight(pPlayer)
	return self:StartFight(pPlayer)
end

-- 提升刷新宠物成功率
function PetIsland:c2s_UpRefreshRate(pPlayer, nQuality)
	return self:UpRefreshRate(pPlayer,nQuality)
end

function PetIsland:c2s_RefreshNature(pPlayer)
	-- 扣元宝
	local nCurCoin = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
	local nNeedCoin = 10 -- 需求10个
	if (nCurCoin < nNeedCoin) then
		me.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", 10});
		return 0;
	end
	
	me.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -10);
	local tbPetDef2 = self.tbPlayerPetDefs[pPlayer.dwId]
	tbPetDef2[4] = KUnify.MathRandom(1,7)  -- 随机性格, 加入
	
	-- 刷新客户端
	self:__ClientPetInfo(pPlayer, tbPetDef2[2], self.tbPlayerPetQuality[pPlayer.dwId], tbPetDef2[4])
end

-- 战胜宠物后，选择宠物(nIndex : 1-4)
function PetIsland:c2s_ChoosePet(pPlayer)
	if (self.tbPlayers[pPlayer.dwId].bWin == 1) then
		-- AddPlayerPet
		local tbPetDef2 = self.tbPlayerPetDefs[pPlayer.dwId]
		local nPetTempId = tbPetDef2[2]
		local nNature = tbPetDef2[4]
		-- 判断是否有足够空间放宠物
		local bOk = Pet:AddPet(pPlayer, nPetTempId, nNature)

		if bOk == 1 then
		    --Instance:LeaveMyInstance(pPlayer)
		else
		    -- 宠物创建失败
		    --pPlayer.SendBlackMsg("无法领取宠物！请检查宠物格子是否已满")
		    -- 添加宠物蛋
		    local tbPetItemGDPL = tbPetDef2[3]
		    KItem.AddPlayerItem(pPlayer, tbPetItemGDPL[1], tbPetItemGDPL[2], tbPetItemGDPL[3], tbPetItemGDPL[4], 1)
		    -- TODO: 宠物蛋

		    --pPlayer.SysMsg("领取到宠物蛋...")
		end
		self:__ClientShowIslandPanel(pPlayer)  -- 重新出现刷新面板
		KPet.RecordPetIslandCurrentInfo(pPlayer, 0, 0, 0, 0); -- 清空上次召唤信息
	end
end
