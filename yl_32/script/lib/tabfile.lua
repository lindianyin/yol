
-- ====================== 文件信息 ======================

-- 剑侠世界读取表格文件函数脚本
-- Edited by peres
-- 2007/03/15 PM 08:44

-- 只有他和她两个人
-- 他们相爱
-- 她记得
-- 他的手抚摩在她的皮肤上的温情
-- 他的亲吻像鸟群在天空掠过
-- 他在她身体里面的暴戾和放纵
-- 他入睡时候的样子充满纯真
-- 她记得，清晨她醒过来的一刻，他在她的身边
-- 她睁着眼睛，看曙光透过窗帘一点一点地照射进来
-- 她的心里因为幸福而疼痛

-- ======================================================

local tbReadFile = {};

tbReadFile.START_ROW = 2;	-- 表格从第几行开始算起，默认是 2，前两分别是英文和中文表头
tbReadFile.m_szKey = "";	-- 文件的关键字


function tbReadFile:init(szFile)
	if (QFile.TabFile_Load(szFile, szFile) == 0) then
		print("Load table file error: "..szFile);
		return 0;
	end;
	self.m_szKey = szFile;
	return 1;
end;

function tbReadFile:GetRow()
	return QFile.TabFile_GetRowCount(self.m_szKey) - self.START_ROW;
end;

function tbReadFile:GetCell(szCol, nRow)
	local szCell = QFile.TabFile_GetCell(self.m_szKey, nRow + self.START_ROW, szCol);
		if szCell==nil then
			print("Get a empty cell: Col:"..szCol.." Row:"..nRow.." File:"..self.m_szKey);
			return "";
		else
			return szCell;
		end;
end;

function tbReadFile:GetCellInt(szCol, nRow)
	local szCell = QFile.TabFile_GetCell(self.m_szKey, nRow + self.START_ROW, szCol);
		if szCell==nil or szCell=="" then
			print("Get a empty cell: Col:"..szCol.." Row:"..nRow.." File:"..self.m_szKey);
			return -1;
		else
			return tonumber(szCell);
		end;
end;

-- 检索表格指定列内的所有行，返回指定数据的行数
-- 传入参数：data:szData 指定的数据
-- 传入参数：string:szCol 指定的列标题
-- 返回值：  int:row    符合条件的行
function tbReadFile:GetDateRow(szCol, szData)
	local strType = type(szData) -- 获取数据类型

	for i=1, self:GetRow() do
		if strType=="number" then
			if self:GetCellInt(szCol, i) == szData then
				return i;
			end;
		else
			if self:GetCell(szCol, i) == szData then
				return i;
			end;
		end;
	end;
	print ("[Error：] tbReadFile:GetDateRow Get row nil!");
	return 0;
end;

function tbReadFile:CountRate(szCol)
	local nRow = self:GetRow();
	local nRandom = 0;
	local nAdd = 0;
	local i=0;

	for i=1, nRow do
		nAdd = nAdd + self:GetCellInt(szCol, i);
	end;

	nRandom = KUnify.MathRandom(1, nAdd);

	nAdd = 0;

	for i=1, nRow do
		nAdd = nAdd + self:GetCellInt(szCol, i);
		if nAdd>=nRandom then
			return i;
		end;
	end;

	return 0;
end;

function tbReadFile:Release()
	QFile.TabFile_UnLoad(self.m_szKey);
end;

-- 属于 Lib 库
Lib.readTabFile = tbReadFile;
