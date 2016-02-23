-- 
-- 逍遥谷数据管理
-- 

-- NPC刷点表 
XoyoGame.NPC_POINT_PATH_LV1 	= "\\setting\\level\\npc_pos_lv1.txt";
XoyoGame.NPC_POINT_PATH_LV2 	= "\\setting\\level\\npc_pos_lv2.txt";
XoyoGame.NPC_POINT_PATH_LV3 	= "\\setting\\level\\npc_pos_lv3.txt";
XoyoGame.NPC_POINT_PATH_LV4 	= "\\setting\\level\\npc_pos_lv4.txt";
XoyoGame.NPC_POINT_PATH_LV5 	= "\\setting\\level\\npc_pos_lv5.txt";
-- NPC移动路线
XoyoGame.NPC_ROAD_PATH 			= "\\setting\\level\\npc_road.txt";
-- 题库
XoyoGame.GUESS_PATH				= "\\setting\\level\\xoyo_question.txt";
-- 房间权限 
XoyoGame.ROOM_WEIGHT			= "\\setting\\level\\room_weight.txt";


XoyoGame.tbNpcPoint = {};
XoyoGame.tbRoad = {};
XoyoGame.tbGuessQuestion = {};
XoyoGame.tbRoomWeight = {};
XoyoGame.tbRoomExp = {};
XoyoGame.tbRoomWeightDisable = {};

-- 加载NPC刷点表
function XoyoGame:LoadNpcPoint(szPath)
	local tbNumColName = { X = 1, Y = 1 };
	local tbFile = Lib:LoadTabFile(szPath, tbNumColName);
	if tbFile then
		for _, tbItem in pairs(tbFile) do
			local szClassName = tbItem.ClassName;
			if self.tbNpcPoint[szClassName] == nil then
				self.tbNpcPoint[szClassName] = {}
			end
			table.insert(self.tbNpcPoint[szClassName], {tbItem.X / 32, tbItem.Y / 32});
		end
	end
end

-- 加载NPC路线表
function XoyoGame:LoadNpcRoad(szPath)
	local tbNumColName = { X = 1, Y = 1 };
	local tbFile = Lib:LoadTabFile(szPath, tbNumColName);
	if tbFile then
		for _, tbItem in pairs(tbFile) do
			local szRoadName = tbItem.RoadName;
			if self.tbRoad[szRoadName] == nil then
				self.tbRoad[szRoadName] = {}
			end
			table.insert(self.tbRoad[szRoadName], {tbItem.X / 32 , tbItem.Y / 32 });
		end
	end
end

-- 加载题库
function XoyoGame:LoadGuessQuestion(szPath)	
	local tbsortpos = Lib:LoadTabFile(szPath);
	if tbsortpos then
		local nLineCount = #tbsortpos;
		for nLine=1, nLineCount do
			local tbQuestion = {}
			tbQuestion.szQuestion = tbsortpos[nLine].QUESTION;
			tbQuestion.szAnswer = tbsortpos[nLine].ANSWER;
			local nShowFlag = tonumber(tbsortpos[nLine].SHOWFLAG);
			-- 如果id为0表示此题是要屏蔽的
			if (nShowFlag and nShowFlag > 0) then
				tbQuestion.tbSelect = {}
				table.insert(tbQuestion.tbSelect, tbsortpos[nLine].SELECT1);
				table.insert(tbQuestion.tbSelect, tbsortpos[nLine].SELECT2);
				--table.insert(tbQuestion.tbSelect, tbsortpos[nLine].SELECT3);
				table.insert(self.tbGuessQuestion, tbQuestion);
			end
		end
	end
end

-- 加载房间权值表
function XoyoGame:LoadRoomWeight(szPath)
	local tbNumColName = {RoomId = 1, Level = 1, Teams = 1, Weight = 1, WinExp = 1, LostExp = 1, LostRepute = 1, WinRepute = 1, Prestige = 1};
	local tbFile = Lib:LoadTabFile(szPath, tbNumColName);
	if tbFile then
		for _, tbItem in pairs(tbFile) do
			local nLevel = tbItem.Level;
			local nRoomId = tbItem.RoomId;
			if self.tbRoomWeight[nLevel] == nil then
				self.tbRoomWeight[nLevel] = {};
			end
			if self.tbRoomWeight[nLevel][nRoomId] == nil then
				self.tbRoomWeight[nLevel][nRoomId] = {}
			end
			self.tbRoomWeight[nLevel][nRoomId].nWeight = tbItem.Weight;
			self.tbRoomWeight[nLevel][nRoomId].nTeams = tbItem.Teams;
			self.tbRoomExp[nRoomId] = {};
			self.tbRoomExp[nRoomId].nWinExp = tbItem.WinExp;
			self.tbRoomExp[nRoomId].nLostExp = tbItem.LostExp;	
			self.tbRoomExp[nRoomId].nWinRepute	= tbItem.WinRepute;		
			self.tbRoomExp[nRoomId].nLostRepute	= tbItem.LostRepute;		
			self.tbRoomExp[nRoomId].nPrestige = tbItem.Prestige;
		end
	end
end

function XoyoGame:GetRoomWeight()
	local tb = {};
	for nLevel, tbData1 in pairs(self.tbRoomWeight) do
		tb[nLevel] = {};
		for nRoomId, tbData2 in pairs(tbData1) do
			if not self.tbRoomWeightDisable[nRoomId] then
				tb[nLevel][nRoomId] = Lib:CopyTB1(tbData2);
			end
		end
	end
	return tb;
end

function XoyoGame:DisableRoom(nRoomId, nDisable)
	local nLevel = assert(XoyoGame.RoomSetting.tbRoom[nRoomId].nRoomLevel);
	assert(self.tbRoomWeight[nLevel][nRoomId]);
	
	if nDisable == 1 then
		self.tbRoomWeightDisable[nRoomId] = 1;
	else
		self.tbRoomWeightDisable[nRoomId] = nil;
	end
end

function XoyoGame:DisableLevel(nLevel, nDisable)
	for nRoomId, _ in pairs(self.tbRoomWeight[nLevel]) do
		self:DisableRoom(nRoomId, nDisable);
	end
end


XoyoGame:LoadNpcPoint(XoyoGame.NPC_POINT_PATH_LV1);
XoyoGame:LoadNpcPoint(XoyoGame.NPC_POINT_PATH_LV2);
XoyoGame:LoadNpcPoint(XoyoGame.NPC_POINT_PATH_LV3);
XoyoGame:LoadNpcPoint(XoyoGame.NPC_POINT_PATH_LV4);
XoyoGame:LoadNpcPoint(XoyoGame.NPC_POINT_PATH_LV5);
XoyoGame:LoadNpcRoad(XoyoGame.NPC_ROAD_PATH);
XoyoGame:LoadGuessQuestion(XoyoGame.GUESS_PATH);
XoyoGame:LoadRoomWeight(XoyoGame.ROOM_WEIGHT);

