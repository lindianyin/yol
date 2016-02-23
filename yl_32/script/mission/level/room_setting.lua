Include("script/mission/level/setting/muji.lua");                  --lv1
Include("script/mission/level/setting/muren.lua");                 --lv1
Include("script/mission/level/setting/zhualaoshu.lua");            --lv1
Include("script/mission/level/setting/rongjiangguai.lua");         --lv1
Include("script/mission/level/setting/shanzeiguanai.lua");          --lv
Include("script/mission/level/setting/huianzhixue.lua");           --lv2
Include("script/mission/level/setting/dipi.lua");                  --lv2
Include("script/mission/level/setting/gumumaoxian.lua");           --lv2
Include("script/mission/level/setting/langwang.lua");              --lv2
Include("script/mission/level/setting/xiaogui.lua");               --lv2
Include("script/mission/level/setting/bagua.lua");                 --lv3
Include("script/mission/level/setting/chongwang.lua");             --lv3
Include("script/mission/level/setting/gumuhusong.lua");            --lv3
Include("script/mission/level/setting/zhuifeng.lua");              --lv3
Include("script/mission/level/setting/jiangwushi.lua");            --lv3
Include("script/mission/level/setting/jingling.lua");              --lv3
Include("script/mission/level/setting/shouhao.lua");               --lv4
Include("script/mission/level/setting/fengzhenjiangwang.lua");       --lv4
Include("script/mission/level/setting/wuou.lua");                  --lv4
Include("script/mission/level/setting/xiongyang.lua");             --lv4
Include("script/mission/level/setting/zengya.lua");                --lv4
Include("script/mission/level/setting/shirenhua.lua");             --lv4
Include("script/mission/level/setting/shuimoyan.lua");             --lv5
Include("script/mission/level/setting/jinbuhuan.lua");             --lv5
Include("script/mission/level/setting/luohu.lua");                 --lv5
Include("script/mission/level/setting/mufouyu.lua");               --lv5
Include("script/mission/level/setting/wenhuangmojiang.lua");       --lv5
Include("script/mission/level/setting/diaoyuweng.lua");            --lv6
Include("script/mission/level/setting/xiongxiezhishou.lua");       --lv6
Include("script/mission/level/setting/zhenshoudiaolong.lua");      --lv7

Include("script/mission/level/dttroom/dengtianta1_5.lua");
Include("script/mission/level/dttroom/dengtianta6_10.lua");
Include("script/mission/level/dttroom/dengtianta11_15.lua");
Include("script/mission/level/dttroom/dengtianta16_20.lua");
Include("script/mission/level/dttroom/dengtianta21_25.lua");
Include("script/mission/level/dttroom/dengtianta26_30.lua");
Include("script/mission/level/dttroom/dengtianta31_35.lua");
Include("script/mission/level/dttroom/dengtianta36_40.lua");
Include("script/mission/level/dttroom/dengtianta41_45.lua");
Include("script/mission/level/dttroom/dengtianta46_50.lua");
Include("script/mission/level/dttroom/dengtianta51_55.lua");
Include("script/mission/level/dttroom/dengtianta56_60.lua");
Include("script/mission/level/dttroom/dengtianta61_65.lua");
Include("script/mission/level/dttroom/dengtianta66_70.lua");
Include("script/mission/level/dttroom/dengtianta71_75.lua");
Include("script/mission/level/dttroom/dengtianta76_80.lua");
Include("script/mission/level/dttroom/dengtianta81_85.lua");
Include("script/mission/level/dttroom/dengtianta86_90.lua");
Include("script/mission/level/dttroom/dengtianta91_95.lua");
--Include("script/mission/level/dttroom/dengtianta96_100.lua");




local tbRoom = XoyoGame.RoomSetting.tbRoom;
local RoomSetting = XoyoGame.RoomSetting;

RoomSetting.RoomList = {};
local RoomList = RoomSetting.RoomList;
for _id, _info in pairs(tbRoom) do
	local nLevel = _info.nRoomLevel;
	assert(nLevel ~= nil);
	if RoomList[nLevel] == nil then
		RoomList[nLevel] = {};
	end
	table.insert(RoomList[nLevel], _id);
end

------------------------------------------------------------------------------------------------------------------------
-- Í¨¹Ø½±Àø

RoomSetting.tbMissionAward = {};
local tbMissionAward = RoomSetting.tbMissionAward;

function RoomSetting:LoadMissionAward()
	local tbFileData = Lib:LoadTabFile("/setting/award/missionaward.txt")
	for _, tbRow in pairs(tbFileData) do
		local nDifficulty = tonumber(tbRow.Difficulty);
		if not nDifficulty then
			nDifficulty = 1;
		end
		if not tbMissionAward[nDifficulty] then
			tbMissionAward[nDifficulty] = {};
		end
		local tbInfo = tbMissionAward[nDifficulty];
		tbInfo[tonumber(tbRow.Level)] = {
			ExpAward = tonumber(tbRow.ExpAward);
			CoinAward = tonumber(tbRow.CoinAward);
			ItemGerne = tonumber(tbRow.ItemGerne);
			ItemDetail = tonumber(tbRow.ItemDetail);
			ItemParticularType = tonumber(tbRow.ItemParticularType);
			ItemLevel = tonumber(tbRow.ItemLevel);
		};
	end
end

RoomSetting.tbDTTGameAward = {};
local tbDTTGameAward = RoomSetting.tbDTTGameAward;

function RoomSetting:LoadDTTGameAward()
	local tbFileData = Lib:LoadTabFile("/setting/award/dttgameaward.txt")
	for _, tbRow in pairs(tbFileData) do
		local nLevel = tonumber(tbRow.level);
		if (nLevel > 0) then
			if (not tbDTTGameAward[nLevel]) then
				tbDTTGameAward[nLevel] = {};
			end

			local nWave = tonumber(tbRow.wave);
			if (nWave > 0) then
				if (not tbDTTGameAward[nLevel][nWave]) then
					tbDTTGameAward[nLevel][nWave] = {};
				end

				local tbLevelWaveAward = tbDTTGameAward[nLevel][nWave];
				for i = 1, 5, 1 do
					tbLevelWaveAward["type"..i] = tonumber(tbRow["type"..i]);
					tbLevelWaveAward["value"..i] = tbRow["value"..i];
				end
			end
		end
	end
end

RoomSetting:LoadMissionAward();
RoomSetting:LoadDTTGameAward();