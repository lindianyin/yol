--脚本定义的常用基础函数
--范围：主要是数学、字符串、table、文件等方面的操作，与游戏世界无关
--zzf
Lang.lib.str1 = {"天","","","","",""}
Lang.lib.str2 = {"小时","","","","",""}
Lang.lib.str3 = {"分钟","","","","",""}
Lang.lib.str4 = {"秒","","","","",""}
Lang.lib.str5 = {"一","","","","",""}
Lang.lib.str6 = {"二","","","","",""}
Lang.lib.str7 = {"三","","","","",""}
Lang.lib.str8 = {"四","","","","",""}
Lang.lib.str9 = {"五","","","","",""}
Lang.lib.str10 = {"六","","","","",""}
Lang.lib.str11 = {"七","","","","",""}
Lang.lib.str12 = {"八","","","","",""}
Lang.lib.str13 = {"九","","","","",""}
Lang.lib.str14 = {"十","","","","",""}
Lang.lib.str15 = {"百","","","","",""}
Lang.lib.str16 = {"千","","","","",""}
Lang.lib.str17 = {"万","","","","",""}
Lang.lib.str18 = {"亿","","","","",""}
Lang.lib.str19 = {"零","","","","",""}
Lang.lib.str20 = {"负","","","","",""}
Lang.lib.str21 = {"个半小时","","","","",""}
Lang.lib.str22 = {"%d分%d秒","","","","",""}
Lang.lib.str23 = {"%.1f小时","","","","",""}
Lang.lib.str24 = {"%.1f天","","","","",""}
Lang.Idx = 1
Lib.tbTypeOrder = {
	["nil"] = 1, ["number"] = 2, ["string"] = 3, ["userdata"] = 4, ["function"] = 5, ["table"] = 6,
};
Lib.TYPE_COUNT = 6;	-- 类型数量
Lib.tbAwardType	= {"Exp", "Repute", "Money", "Item"};
Lib._tbCommonMetatable	= {
	__index	= function (tb, key)
		return rawget(tb, "_tbBase")[key];
	end;
};

Lib.TB_TIME_DESC	= {
	{Lang.lib.str1[Lang.Idx], 3600 * 24};
	{Lang.lib.str2[Lang.Idx], 3600};
	{Lang.lib.str3[Lang.Idx], 60};
	{Lang.lib.str4[Lang.Idx], 1};
};

Lib.TIME_AREATIME = 3600 * 8;

--对table进行一层的复制（不遍历下层）
--如果是连续的table，完全可以使用{unpack(tb)}，而不必使用此函数
function Lib:CopyTB1(tb)
	local tbCopy	= {};
	for k, v in pairs(tb) do
		tbCopy[k]	= v;
	end;
	return tbCopy;
end;

Lib.MAX_COPY_LAY = 7;
function Lib:DeepCopyForTable(tbSrc, nMaxLay)
	nMaxLay = nMaxLay or self.MAX_COPY_LAY;
	if (nMaxLay <= 0) then
		error("Error: DeepCopy拷贝的层数操作最大层，检查是否有循环引用");
		return;
	end
	
	local tbRet = {};
	for k, v in pairs(tbSrc) do
		if (type(v) == "table") then
			tbRet[k] = self:DeepCopyForTable(v, nMaxLay-1);
		else
			tbRet[k] = v;
		end
	end
	
	return tbRet;
end

-- 默认拷贝7层
function Lib:CopyTBN(tb, n)
	n = n or Lib.MAX_COPY_LAY;
	if (n == 0) then
		return;
	end 
	
	local tbRet = {};
	
	for k, v in pairs(tb) do
		if (type(v) == "table") then
			tbRet[k] = self:CopyTBN(v, n-1);
		else
			tbRet[k] = v;
		end
	end
	return tbRet;
end


function Lib:TypeId(szType)
	if self.tbTypeOrder[szType] then
		return self.tbTypeOrder[szType];
	end;
	self.TYPE_COUNT = self.TYPE_COUNT + 1;
	self.tbTypeOrder[szType] = self.TYPE_COUNT;
	return self.TYPE_COUNT;
end;

function Lib:ShowTB1(tbVar, szBlank)
	if (not szBlank) then
		szBlank = "";
	end;
	for k, v in pairs(tbVar) do
		print(szBlank.."["..self:Val2Str(k).."]	= "..tostring(v));
	end;
end;

function Lib:ShowTBN(tbVar, nLevel, szBlank)
	if (not szBlank) then
		szBlank = "";
	end;
	for k, v in pairs(tbVar) do
		if (type(v) == "table" and nLevel > 1) then
			print(szBlank.."["..self:Val2Str(k).."]	:");
			self:ShowTBN(v, nLevel - 1, szBlank .. "  ");
		else
			print(szBlank.."["..self:Val2Str(k).."]	= "..tostring(v));
		end
	end;
end;

function Lib:ShowTB(tbVar, szBlank)
	if (not szBlank) then
		szBlank = "";
	end;
	local tbType = {};
	for k, v in pairs(tbVar) do
		local nType = self:TypeId(type(v));
		if (not tbType[nType]) then
			tbType[nType] = {n = 0, name = type(v)};
		end;
		local tbTmp = tbType[nType];
		tbTmp.n = tbTmp.n + 1;
		tbTmp[tbTmp.n] = k;
	end;
	for i = 1, self.TYPE_COUNT do
		if tbType[i] then
			local tbTmp = tbType[i];
			local szType = tbTmp.name;
			--print(">"..szType..":")
			table.sort(tbTmp);
			for i = 1, tbTmp.n do
				local key = tbTmp[i];
				local value = tbVar[key];
				local str;
				if (type(key) == "number") then
					str = szBlank.."["..key.."]";
				else
					str = szBlank.."."..key;
				end;
				if (szType == "nil") then
					print(str.."\t= nil");
				elseif (szType == "number") then
					print(str.."\t= "..tbVar[key]);
				elseif (szType == "string") then
					print(str..'\t= "'..tbVar[key]..'"');
				elseif (szType == "function") then
					print(str.."()");
				elseif (szType == "table") then
					if (tbVar[key] == tbVar) then
						print(str.."\t= {...}(self)");
					else
						print(str..":");
						self:ShowTB(tbVar[key], str);
					end;
				elseif (szType == "userdata") then
					print(str.."*");
				else
					print(str.."\t= "..tostring(tbVar[key]));
				end;
			end;
		end;
	end;
end;

function Lib:CountTB(tbVar)
	local nCount = 0;
	for _, _ in pairs(tbVar) do
		nCount	= nCount + 1;
	end;
	return nCount;
end;

-- 合并2个表，用于下标默认的表
function Lib:MergeTable(tableA, tableB)
	for _, item in ipairs(tableB) do
		tableA[#tableA + 1] = item;
	end
	
	return tableA;
end;

--合并两个NewClass类
function Lib:MergeFunTable(tableA, tableB)
	for param, item in pairs(tableB) do
		if param ~= "_tbBase" then
			tableA[param] = item;
		end
	end
	if tableB._tbBase ~= nil then
		for param, item in pairs(tableB._tbBase) do
			tableA[param] = item;
		end
	end
	return tableA;
end;

function Lib:StrVal2Str(szVal)
	szVal	= string.gsub(szVal, "\\", "\\\\");
	szVal	= string.gsub(szVal, '"', '\\"');
	szVal	= string.gsub(szVal, "\n", "\\n");
	szVal	= string.gsub(szVal, "\r", "\\r");
	--szVal	= string.format("%q", szVal);
	return '"'..szVal..'"';
end;

-- 去除指定字符串首尾指定字符
function Lib:StrTrim(szDes, szTrimChar)
	if (not szTrimChar) then
		szTrimChar = " ";
	end
	
	if (string.len(szTrimChar) ~= 1) then
		return szDes;
	end
	
	local szRet, nCount = string.gsub(szDes, "("..szTrimChar.."*)([^"..szTrimChar.."]*.*[^"..szTrimChar.."])("..szTrimChar.."*)", "%2");
	if (nCount == 0) then
		return "";
	end
	
	return szRet;
end


function Lib:Val2Str(var, szBlank)
	local szType	= type(var);
	if (szType == "nil") then
		return "nil";
	elseif (szType == "number") then
		return tostring(var);
	elseif (szType == "string") then
		return self:StrVal2Str(var);
	elseif (szType == "function") then
		local szCode	= string.dump(var);
		local arByte	= {string.byte(szCode, i, #szCode)};
		szCode	= "";
		for i = 1, #arByte do
			szCode	= szCode..'\\'..arByte[i];
		end;
		return 'loadstring("' .. szCode .. '")';
	elseif (szType == "table") then
		if not szBlank then
			szBlank	= "";
		end;
		local szTbBlank	= szBlank .. "  ";
		local szCode	= "";
		for k, v in pairs(var) do
			local szPair	= szTbBlank.."[" .. self:Val2Str(k) .. "]	= " .. self:Val2Str(v, szTbBlank) .. ",\n";
			szCode	= szCode .. szPair;
		end;
		if (szCode == "") then
			return "{}";
		else
			return "\n"..szBlank.."{\n"..szCode..szBlank.."}";
		end;
	else	--if (szType == "userdata") then
		return '"' .. tostring(var) .. '"';
	end;
end;

function Lib:Str2Val(szVal)
	print("..szVal",szVal);
	return assert(loadstring("return "..szVal))();
end;

function Lib:DoTestFuncs(tbFunctions)
	for _, szCode in ipairs(tbFunctions) do
		local bOK, str	= loadstring(szCode)();
		if (not bOK) then
			return nil, str;
		end;
	end;
	return 1;
end;

function Lib:DoExecFuncs(tbFunctions, nExtParam)
	for _, szCode in ipairs(tbFunctions) do
		if (nExtParam) then
			local nEnd = string.find(szCode, ")", string.len(szCode));
			if (nEnd) then
				szCode = string.sub(szCode, 1, nEnd-1)..","..nExtParam..string.sub(szCode, nEnd);
			end
		end
		
		loadstring(szCode)();
	end;
	
	return 1;
end;

function Lib:NewClass(tbBase, ...)
	local tbNew	= { _tbBase = tbBase };							-- 基类
	setmetatable(tbNew, self._tbCommonMetatable);
	local tbRoot = tbNew;
	local tbInit = {};
	repeat										-- 寻找最基基类
		tbRoot = rawget(tbRoot, "_tbBase");
		local fnInit = rawget(tbRoot, "init");
		if (type(fnInit) == "function") then
			table.insert(tbInit, fnInit);		-- 放入构造函数栈
		end
	until (not rawget(tbRoot, "_tbBase"));
	for i = #tbInit, 1, -1 do
		local fnInit = tbInit[i];
		if fnInit then
			fnInit(tbNew, unpack(arg));			-- 从底向上调用构造函数
		end
	end
	return tbNew;
end

function Lib:DelayInherit(tbBase, tbDerive)
	assert(not tbDerive._tbBase and not getmetatable(tbDerive));
	tbDerive._tbBase = tbBase;
	setmetatable(tbDerive, self._tbCommonMetatable);
end

function Lib:ConcatStr(tbStrElem, szSep)
	if (not szSep) then
		szSep = ",";
	end
	return table.concat(tbStrElem, szSep);
end

function Lib:SplitStr(szStrConcat, szSep)
	if (not szSep) then
		szSep = ",";
	end;
	local tbStrElem = {};
	local nSepLen = #szSep;
	local nStart = 1;
	local nAt = string.find(szStrConcat, szSep);
	while nAt do
		tbStrElem[#tbStrElem+1] = string.sub(szStrConcat, nStart, nAt - 1);
		nStart = nAt + nSepLen;
		nAt = string.find(szStrConcat, szSep, nStart);
	end
	tbStrElem[#tbStrElem+1] = string.sub(szStrConcat, nStart);
	return tbStrElem;
end

function Lib:ReplaceStr(szStr, szFrom, szTo)
	local nStart, nEnd	= string.find(szStr, szFrom);
	local nLen			= #szTo;
	while (nStart) do
		szStr			= string.sub(szStr, 1, nStart - 1) .. szTo .. string.sub(szStr, nEnd + 1);
		nStart, nEnd	= string.find(szStr, szFrom, nStart + nLen);
	end;
	return szStr;
end;

-- 汉字安全的字符串替换（不支持“magic”）
function Lib:ReplaceStrS(szStr, szFrom, szTo)
	local nStart	= KLib.FindStr(szStr, szFrom);
	local nFromLen	= #szFrom;
	local nToLen	= #szTo;
	while (nStart) do
		szStr	= string.sub(szStr, 1, nStart - 1) .. szTo .. string.sub(szStr, nStart + nFromLen);
		nStart	= KLib.FindStr(szStr, szFrom, nStart + nToLen);
	end;
	return szStr;
end;

-- 把字符串中从nBegin到nEnd的子串替换为szTo
function Lib:ReplaceStrFormIndex(str, nBegin, nEnd, szTo)
	local nLen = #str;
	if (nLen < nEnd) then
		return str;
	end
	str = string.sub(str, 1, nBegin-1)..szTo..string.sub(str, nEnd+1)
	return str;
end;

function Lib:IsTrue(var)
	return (var ~= nil and var ~= 0 and var ~= false and var ~= "false" and var ~= "");
end;

-- 按照统一的格式回调函数
function Lib:CallBack(tbCallBack)
	local varFunc	= tbCallBack[1];
	local szType	= type(varFunc);
	
	local function InnerCall()
		if (szType == "function") then	-- 直接指定了函数及参数
			return	tbCallBack[1](unpack(tbCallBack, 2));
		elseif (szType == "string") then	-- 按照字符串的方式指定了函数
			local fnFunc, tbSelf	= KLib.GetValByStr(varFunc);
			if (fnFunc) then
				if (tbSelf) then
					return fnFunc(tbSelf, unpack(tbCallBack, 2));
				else
					return fnFunc(unpack(tbCallBack, 2));
				end;
			else
				return false, "Wrong name string : "..varFunc;
			end;
		end;
	end
	
	local tbRet	= {xpcall(InnerCall, Lib.ShowStack)};
	
	return unpack(tbRet);
end;

function Lib.ShowStack(s)
	print(debug.traceback(s,2));
	return s;
end

-- 检查一个Table是否另一个Table的派生Table
function Lib:IsDerived(tbThis, tbBase)
	if (not tbThis) or (not tbBase) then
		return	0;
	end;
	repeat
		local pBase = rawget(tbThis, "_tbBase");
		if (pBase == tbBase) then
			return	1;
		end
		tbThis = pBase;
	until (not tbThis);
	return	0;
end

-- If串格式："你好，<if me.nSex=0>库哥<else>美女<endif>，再见";
-- 结果：{Fun(),{},{}}
function Lib:ParseIF(strSrc)
	if (not strSrc) then
		return;
	end
	
	local tbResult = {};
	local nCurIdx = 1;
	while true do
		-- if
		local nIfStartIdx, _ = string.find(strSrc, "<if ", nCurIdx);
		local nIfEndIdx, _ = string.find(strSrc, ">", nIfStartIdx);
		-- else
		local nElseStartIdx, _ = string.find(strSrc,"<else>",nIfEndIdx);
		-- endif
		local nEndIfStartIdx, _ = string.find(strSrc, "<endif>", nIfEndIdx);
		
		-- nEndIfStartIdx为空表明已经遍历完成，从nCurIdx到末尾的字符串插入到tbResult;
		if (not nIfStartIdx or not nEndIfStartIdx) then
			self:MergeTable(tbResult, self:ParseEqual(string.sub(strSrc, nCurIdx)));
			return tbResult;
		end
		-- 标识错误情况
		if (nIfEndIdx >= nEndIfStartIdx or nIfEndIdx >= nEndIfStartIdx) then
			self:MergeTable(tbResult, self:ParseEqual(string.sub(strSrc, nCurIdx)));
			return tbResult;
		end
		
		local bElseTagAvailably = (nElseStartIdx and (nElseStartIdx < nEndIfStartIdx)); -- Else标签是否有效
		-- 插入这一轮
		self:MergeTable(tbResult, self:ParseEqual(string.sub(strSrc, nCurIdx, nIfStartIdx-1)));--插入If之前的字符串
		if (bElseTagAvailably) then -- else有效
			-- if elseif endif 的表
		tbResult[#tbResult + 1] = {
			function() 
				return loadstring("return "..string.sub(strSrc, nIfStartIdx+4, nIfEndIdx-1))();
			end,
			}
			table.insert(tbResult[#tbResult], self:ParseEqual(string.sub(strSrc, nIfEndIdx+1, nElseStartIdx-1)));
			table.insert(tbResult[#tbResult], self:ParseEqual(string.sub(strSrc, nElseStartIdx+6, nEndIfStartIdx-1)));
		else	-- else无效
		 	tbResult[#tbResult + 1] = {
		 	function()
		 		return loadstring("return "..string.sub(strSrc, nIfStartIdx+4, nIfEndIdx-1))();
		 	end,
		 	}
		 	table.insert(tbResult[#tbResult], self:ParseEqual(string.sub(strSrc, nIfEndIdx+1, nEndIfStartIdx-1)));
		end
		
		-- 已经遍历完成，这个时候字符串最后为<endif>
		if (string.len(strSrc) <= nEndIfStartIdx) then
			return tbResult;
		end
		
		nCurIdx = nEndIfStartIdx + 7;
	end
end;

function Lib:ParseEqual(strSrc)
	if (not strSrc) then
		return;
	end
	
	local tbResult = {};
	local nCurIdx = 1;
	
	while true do
		-- <=
		local nEqualStartIdx, _ = string.find(strSrc, "<=", nCurIdx);
		local nEqualEndIdx, _ = string.find(strSrc, ">", nEqualStartIdx);
		
		-- nEqualEndIdx为空表明已经遍历完成，从nCurIdx到末尾的字符串插入到tbResult;
		if (not nEqualStartIdx or not nEqualEndIdx) then
			tbResult[#tbResult + 1] = string.sub(strSrc, nCurIdx);
			return tbResult;
		end
		
		-- 插入这一轮
		tbResult[#tbResult + 1] = string.sub(strSrc, nCurIdx, nEqualStartIdx-1);
		
	 	tbResult[#tbResult + 1] = 
	 	function()
	 		return loadstring("return "..string.sub(strSrc, nEqualStartIdx+2, nEqualEndIdx-1))();
	 	end;
	 	
	 	
	 	-- 已经遍历完成，这个时候字符串最后为<endif>
		if (string.len(strSrc) <= nEqualEndIdx) then
			return tbResult;
		end
		
	 	nCurIdx = nEqualEndIdx + 1;
	end;
end;


function Lib:LinkExpressionTable(tbMsgs)
	if (not tbMsgs) then
		return;
	end
	local strFinal = "";
	for i = 1, #tbMsgs do
		if (type(tbMsgs[i]) == "function") then
				strFinal = strFinal..tbMsgs[i]();
		elseif (type(tbMsgs[i]) == "table") then
			if (#tbMsgs[i] == 3) then
				strFinal = strFinal..self:IIF(tbMsgs[i][1], self:LinkExpressionTable(tbMsgs[i][2]), self:LinkExpressionTable(tbMsgs[i][3]));
			elseif (#tbMsgs[i] == 2) then
				strFinal = strFinal..self:IIF(tbMsgs[i][1], self:LinkExpressionTable(tbMsgs[i][2]), "");
			else
				assert(false);
			end
		else
			strFinal = strFinal..tbMsgs[i];
		end
	end
	
	return strFinal;
end;

function Lib:IIF(JugeFunction, szLeft, szRight)
	return (JugeFunction() and szLeft) or szRight;
end;


function Lib:ParseExpression(strExpression)
	return self:LinkExpressionTable(self:ParseIF(strExpression));
end


-- 随机选取一个含有权重的两维数组
function Lib:CountRateTable(tbRate, nPos)
	local nRow = #tbRate;
	local nRandom = 0;
	local nAdd = 0;
	local i=0;
	
	for i=1, nRow do
		nAdd = nAdd + tbRate[i][nPos];
	end;
	
	nRandom = KUnify.MathRandom(1, nAdd);
	
	nAdd = 0;
	
	for i=1, nRow do
		nAdd = nAdd + tbRate[i][nPos];
		if nAdd>=nRandom then
			return i;
		end;
	end;
	
	self:_Debug("[Lib Error]CountRateTable: can't get number!");
	return 0;
end;

-- 功能:	把秒数转换为 nHour小时，nMinute分钟, nSecond秒
-- 参数:	nSecondTime秒
-- 返回值:	nHour小时，nMinute分钟, nSecond秒
function Lib:TransferSecond2NormalTime(nSecondTime)
	local nHour, nMinute, nSecond = 0, 0, 0;
	
	if (nSecondTime >= 3600) then
		nHour = math.floor(nSecondTime / 3600);
		nSecondTime = math.floor(nSecondTime % 3600)
	end
	
	if (nSecondTime >= 60) then
		nMinute = math.floor(nSecondTime / 60);
		nSecondTime = math.floor(nSecondTime % 60)
	end
	nSecond	= math.floor(nSecondTime);
	return nHour, nMinute, nSecond;
end

-- 功能:	把一个长度不超过4位的阿拉伯数字整数转化成为中文数字
-- 参数:	nDigit, (0 <= nDigit) and (nDigit < 10000)
-- 返回值:	中文数字
function Lib:Transfer4LenDigit2CnNum(nDigit)
	local tbCnNum  = {
		[1] 	= Lang.lib.str5[Lang.Idx],
		[2]	 	= Lang.lib.str6[Lang.Idx],
		[3]		= Lang.lib.str7[Lang.Idx],
		[4]		= Lang.lib.str8[Lang.Idx],
		[5] 	= Lang.lib.str9[Lang.Idx],
		[6]		= Lang.lib.str10[Lang.Idx],
		[7] 	= Lang.lib.str11[Lang.Idx],
		[8]		= Lang.lib.str12[Lang.Idx],
		[9] 	= Lang.lib.str13[Lang.Idx],
	};
	local tb4LenCnNum = {
		[1]		= "",
		[2]		= Lang.lib.str14[Lang.Idx],
		[3]		= Lang.lib.str15[Lang.Idx],
		[4]		= Lang.lib.str16[Lang.Idx],
	};
	
	local nDigitTmp	= nDigit;			-- 临时变量
	local nModel	= 0;				-- nDigit中每一位数字的值
	local nPreNum	= 0;				-- nDigit低一位数字的值
	local bOneEver	= false;			-- 做标记,当前是否出现过不为0的值
	local szCnNum	= "";				-- 保存中文数字的变量
	local szNumTmp	= "";				-- 临时变量 

	if (nDigit == 0) then
		return;
	end
	
	if (nDigit >= 10 and nDigit < 20) then
		if (nDigit == 10) then
			szCnNum = tb4LenCnNum[2];
		else
			szCnNum = tb4LenCnNum[2]..tbCnNum[math.floor(nDigit % 10)];
		end
		return szCnNum;
	end

	for i = 1, #tb4LenCnNum do
		szNumTmp	= "";
		nModel		= math.floor(nDigitTmp % 10);	-- 取得nDigit当前位上的值
		if (nModel ~= 0) then
			szNumTmp = szNumTmp..tbCnNum[nModel]..tb4LenCnNum[i];
			if (nPreNum == 0 and bOneEver) then
				szNumTmp = szNumTmp.."零";
			end
			bOneEver = true;
		end
		szCnNum	= szNumTmp..szCnNum;
		
		nPreNum	= nModel;
		nDigitTmp	= math.floor(nDigitTmp / 10);
		if (nDigitTmp == 0) then
			break;
		end
	end
	
	return szCnNum;
end

-- 功能:	把一个阿拉伯数字nDigit转化成为中文数字
-- 参数:	nDigit,nDigit是整数,并且(1万亿 > nDigit) and (nDigit > -1万亿)
-- 返回值:	中文数字
function Lib:TransferDigit2CnNum(nDigit)
	local tbModelUnit = {
		[1]	= "";
		[2]	= Lang.lib.str17[Lang.Idx];
		[3] = Lang.lib.str18[Lang.Idx]; 	
	};
	
	local nDigitTmp = nDigit;	-- 临时变量,
	local n4LenNum	= 0;		-- 每一次对nDigit取4位操作,n4LenNum表示取出来的数的值
	local nPreNum	= 0;		-- 记录前一次进行取4位操作的n4LenNum的值
	local szCnNum	= "";		-- 就是所要求的结果
	local szNumTmp	= "";		-- 临时变量,每取四位的操作中得到的中文数字
	
	if (nDigit == 0) then
		szCnNum = Lang.lib.str19[Lang.Idx];
		return szCnNum;
	end

	if (nDigit < 0) then
		nDigitTmp = math.floor(nDigit * (-1));
		szCnNum	  = Lang.lib.str20[Lang.Idx];
	end
	
	-- 分别从个,万,亿三段考虑,因为nDigit的值小于1万亿,所以每一段都不超过4位
	for i = 1, #tbModelUnit do
		szNumTmp	= "";
		n4LenNum	= math.floor(nDigitTmp % 10000);
		if (n4LenNum ~= 0) then
			szNumTmp = self:Transfer4LenDigit2CnNum(n4LenNum);					-- 得到该四位的中文表达式
			szNumTmp = szNumTmp..tbModelUnit[i];								-- 加上单位
			if ((nPreNum > 0 and nPreNum < 1000) or								-- 两个数字之间有0,所以要加"零"
				(math.floor(n4LenNum % 10) == 0 and i > 1)) then
				szNumTmp	= szNumTmp..Lang.lib.str19[Lang.Idx];
			end	
		end
		szCnNum	= szNumTmp..szCnNum;
		
		nPreNum	= n4LenNum;
		nDigitTmp = math.floor(nDigitTmp / 10000);
		if (nDigitTmp == 0) then
			break;
		end
	end
	
	return szCnNum;
end

-- 功能:	把阿拉伯数字表示的小时转换成中文的小时
-- 参数:	nHour,小时,(1万亿 > nHour) and (nHour > -1万亿)
-- 返回值:	szXiaoshi小时
function Lib:GetCnTime(nHour)
	local szXiaoshi	= "";
	local szShichen	= "";
	local nDigit	= math.floor(nHour);

	if (nHour - nDigit == 0.5 and nDigit > 0) then
		szXiaoshi	= self:TransferDigit2CnNum(nDigit)..Lang.lib.str21[Lang.Idx];
	elseif (nHour - nDigit == 0.5) then
		szXiaoshi	= Lang.lib.str21[Lang.Idx];
	else
		szXiaoshi	= self:TransferDigit2CnNum(nDigit)..Lang.lib.str21[Lang.Idx];
	end

	return szXiaoshi;
end

-- 将秒数转为时间描述字符串，短描述
function Lib:TimeDesc(nSec)
	if (nSec < 3600) then	-- 小于1小时
		return string.format(Lang.lib.str22[Lang.Idx], nSec / 60, math.mod(nSec, 60));
	elseif (nSec < 3600 * 24) then	-- 小于1天
		return string.format(Lang.lib.str23[Lang.Idx], nSec / 3600);
	else
		return string.format(Lang.lib.str24[Lang.Idx], nSec / (3600 * 24));
	end
end

-- 将秒数转为时间描述字符串，短描述
function Lib:TimeDescEn(nSec)
	if (nSec < 3600) then	-- 小于1小时
		return string.format("%02d:%02d", nSec / 60, math.mod(nSec, 60));
	elseif (nSec < 3600 * 24) then	-- 小于1天
		return string.format("%.1fHour", nSec / 3600);
	else
		return string.format("%.1fDay", nSec / (3600 * 24));
	end
end

-- 将秒数转为时间描述字符串，精确值
function Lib:TimeFullDesc(nSec)
	local szMsg	= "";
	local nLastLevel	= #self.TB_TIME_DESC;
	for nLevel, tbTimeDesc in ipairs(self.TB_TIME_DESC) do
		local nUnit	= tbTimeDesc[2];
		if (nSec >= nUnit or (nUnit == 1 and szMsg == "")) then
			if (nLevel > nLastLevel + 1) then
				szMsg	= szMsg .. Lang.lib.str19[Lang.Idx];
			end
			szMsg	= szMsg .. math.floor(nSec / nUnit) .. tbTimeDesc[1];
			nSec	= math.mod(nSec, nUnit);
			nLastLevel	= nLevel;
		end
	end
	return szMsg;
end

-- 将游戏桢数转换为时间描述字符串
function Lib:FrameTimeDesc(nFrame)
	local nSec	= math.floor(nFrame / Env.GAME_FPS);
	return self:TimeDesc(nSec);
end

-- 将TabFile一次性载入，并返回一个数据table
--	数据以第一行为依据形成列名，返回值形如：
--	{
--		[nRow]	= {[szCol] = szValue, [szCol] = szValue, ...},
--		[nRow]	= {[szCol] = szValue, [szCol] = szValue, ...},
--		...
--	}
function Lib:LoadTabFile(szFileName, tbNumColName)
	local tbData	= KLib.LoadTabFile(szFileName);
	if (not tbData) then	-- 未能读取到
		return;
	end
	tbNumColName	= tbNumColName or {};
	local tbColName	= tbData[1];
	tbData[1]	= nil;
	local tbRet	= {};
	for nRow, tbDataRow in pairs(tbData) do
		local tbRow	= {}
		tbRet[nRow - 1]	= tbRow;
		for nCol, szName in pairs(tbColName) do
			if (tbNumColName[szName]) then
				tbRow[szName]	= tonumber(tbDataRow[nCol]) or 0;
			else
				tbRow[szName]	= tbDataRow[nCol];
			end
		end;
	end;
	
	
	return tbRet;
end

-- 将IniFile一次性载入，并返回一个数据table
--	以[Section]为第一级table下标，Key为第二级table下标，形如：
--	{
--		[szSection]	= {[szKey] = szValue, [szKey] = szValue, ...},
--		[szSection]	= {[szKey] = szValue, [szKey] = szValue, ...},
--		...
--	}
function Lib:LoadIniFile(szFileName)
	return KLib.LoadIniFile(szFileName);
end

-- 随机打乱一个连续的Table
function Lib:SmashTable(tb)
	local nLen	= #tb;
	for n, value in pairs(tb) do
		local nRand = KUnify.MathRandom(nLen);
		tb[n]		= tb[nRand];
		tb[nRand]	= value;
	end
end;

-- 获得一个32位数中指定位段(0~31)所表示的整数
function Lib:LoadBits(nInt32, nBegin, nEnd)
	if (nBegin > nEnd) then
		local _ = nBegin;
		nBegin = nEnd;
		nEnd   = _;
	end
	if (nBegin < 0) or (nEnd >= 32) then
		return 0;
	end
	nInt32 = nInt32 % (2 ^ (nEnd + 1));
	nInt32 = nInt32 / (2 ^ nBegin);
	return math.floor(nInt32);
end

-- 设置一个32位数中的指定位段(0~31)为指定整数
function Lib:SetBits(nInt32, nBits, nBegin, nEnd)
	if (nBegin > nEnd) then
		local _ = nBegin;
		nBegin = nEnd;
		nEnd   = _;
	end
	nBits = nBits % (2 ^ (nEnd - nBegin + 1));
	nBits = nBits * (2 ^ nBegin);
	nInt32 = nInt32 % (2 ^ nBegin) + nInt32 - nInt32 % (2 ^ (nEnd + 1));
	nInt32 = nInt32 + nBits;
	return nInt32;
end

--32位数按位与
function Lib:BitsAnd(nValue1, nValue2)
	if not (nValue1>=0 and nValue2>=0) then
		return nil;
	end;
	local nBitScan = 2^31;
	local nResult = 0;
	for i=1,32 do
		local nBitScan2 = nBitScan * 2;
		local nBit1 = math.floor((nValue1 % nBitScan2) / nBitScan);
		local nBit2 = math.floor((nValue2 % nBitScan2) / nBitScan);
		if (nBit1~=0) and (nBit2~=0) then
			nResult = nResult + nBitScan;
		end;
		nBitScan = nBitScan / 2;
	end;
	return nResult;
end;

--32位数按位或
function Lib:BitsOr(nValue1, nValue2)
	if not (nValue1>=0 and nValue2>=0) then
		return nil;
	end;
	local nBitScan = 2^31;
	local nResult = 0;
	for i=1,32 do
		local nBitScan2 = nBitScan * 2;
		local nBit1 = math.floor((nValue1 % nBitScan2) / nBitScan);
		local nBit2 = math.floor((nValue2 % nBitScan2) / nBitScan);
		if (nBit1~=0) or (nBit2~=0) then
			nResult = nResult + nBitScan;
		end;
		nBitScan = nBitScan / 2;
	end;
	return nResult;
end;

--32位数按位或
function Lib:BitsXOr(nValue1, nValue2)
	if not (nValue1>=0 and nValue2>=0) then
		return nil;
	end;
	local nBitScan = 2^31;
	local nResult = 0;
	for i=1,32 do
		local nBitScan2 = nBitScan * 2;
		local nBit1 = math.floor((nValue1 % nBitScan2) / nBitScan);
		local nBit2 = math.floor((nValue2 % nBitScan2) / nBitScan);
		if nBit1~=nBit2 then
			nResult = nResult + nBitScan;
		end;
		nBitScan = nBitScan / 2;
	end;
	return nResult;
end;

--32位数按位非
function Lib:BitsNot(nValue)
	if not (nValue>=0) then
		return nil;
	end;
	local nBitScan = 2^31;
	local nResult = 0;
	for i=1,32 do
		local nBitScan2 = nBitScan * 2;
		local nBit = math.floor((nValue % nBitScan2) / nBitScan);
		if nBit==0 then
			nResult = nResult + nBitScan;
		end;
		nBitScan = nBitScan / 2;
	end;
	return nResult;
end;


 
-- 功能:	把字符串扩展为长度为nLen,左对齐, 其他地方用空格补齐
-- 参数:	szStr	需要被扩展的字符串
-- 参数:	nLen	被扩展成的长度
function Lib:StrFillL(szStr, nLen, szFilledChar)
	szStr				= tostring(szStr);
	szFilledChar		= szFilledChar or " ";
	local nRestLen		= nLen - string.len(szStr);								-- 剩余长度
	local nNeedCharNum	= math.floor(nRestLen / string.len(szFilledChar));	-- 需要的填充字符的数量
	
	szStr = szStr..string.rep(szFilledChar, nNeedCharNum);					-- 补齐
	return szStr;
end

-- 功能:	把字符串扩展为长度为nLen,右对齐, 其他地方用空格补齐
-- 参数:	szStr	需要被扩展的字符串
-- 参数:	nLen	被扩展成的长度
function Lib:StrFillR(szStr, nLen, szFilledChar)
	szStr				= tostring(szStr);
	szFilledChar		= szFilledChar or " ";
	local nRestLen		= nLen - string.len(szStr);								-- 剩余长度
	local nNeedCharNum	= math.floor(nRestLen / string.len(szFilledChar));	-- 需要的填充字符的数量
	
	szStr = string.rep(szFilledChar, nNeedCharNum).. szStr;					-- 补齐
	return szStr;
end

-- 功能:	把字符串扩展为长度为nLen,居中对齐, 其他地方以空格补齐
-- 参数:	szStr	需要被扩展的字符串
-- 参数:	nLen	被扩展成的长度
function Lib:StrFillC(szStr, nLen, szFilledChar)
	szStr				= tostring(szStr);
	szFilledChar		= szFilledChar or " ";
	local nRestLen		= nLen - string.len(szStr);								-- 剩余长度
	local nNeedCharNum	= math.floor(nRestLen / string.len(szFilledChar));	-- 需要的填充字符的数量
	local nLeftCharNum	= math.floor(nNeedCharNum / 2);							-- 左边需要的填充字符的数量
	local nRightCharNum	= nNeedCharNum - nLeftCharNum;							-- 右边需要的填充字符的数量

	szStr = string.rep(szFilledChar, nLeftCharNum)
			..szStr..string.rep(szFilledChar, nRightCharNum);				-- 补齐
	return szStr;
end

-- 游戏逻辑帧数换算为毫秒
function Lib:FrameNum2Ms(nFrame)
	if (nFrame < 0) then
		return 4294967295;			-- 返回无穷大 TODO: 写成0xffffffff在release版会变成0，原因不详，疑是lua的bug
	end
	return math.floor(nFrame * 1000 / Env.GAME_FPS);
end

-- 根据秒数（UTC，GetTime()返回）计算当地时间今天已经过的秒数
function Lib:GetLocalDayTime(nUtcSec)
	local nLocalSec	= (nUtcSec or KGameBase.GetTime()) + self.TIME_AREATIME;
	return math.mod(nLocalSec, 3600 * 24);
end

-- 根据秒数（UTC，GetTime()返回）计算当地小时数
--	1970年1月1日0时 返回0
--	1970年1月1日1时 返回1
--	……依此类推
function Lib:GetLocalHour(nUtcSec)
	local nLocalSec	= (nUtcSec or KGameBase.GetTime()) + self.TIME_AREATIME;
	return math.floor(nLocalSec / 3600);
end

-- 根据秒数（UTC，GetTime()返回）计算当地天数
--	1970年1月1日 返回0
--	1970年1月2日 返回1
--	1970年1月3日 返回2
--	……依此类推
function Lib:GetLocalDay(nUtcSec)
	local nLocalSec	= (nUtcSec or KGameBase.GetTime()) + self.TIME_AREATIME;
	return math.floor(nLocalSec / (3600 * 24));
end


-- 根据秒数（UTC，GetTime()返回）计算当地周数
--	1970年1月1日 星期四 返回0
--	1970年1月4日 星期日 返回0
--	1970年1月5日 星期一 返回1
--	……依此类推
function Lib:GetLocalWeek(nUtcSec)
	local nLocalDay	= self:GetLocalDay(nUtcSec);
	return math.floor((nLocalDay + 3) / 7);
end

-- 根据秒数（UTC，GetTime()返回）计算当地月数
--	1970年1月 返回0
--	1970年2月 返回1
--	1970年3月 返回2
--	……依此类推
function Lib:GetLocalMonth(nUtcSec)
	local tbTime 	= os.date("*t", nUtcSec or GetTime());
	return (tbTime.year - 1970) * 12 + tbTime.month - 1;
end

--返回固定日期的秒数
--nDate格式如(2008-6-25 00:00:00):20080625 ; 2008062500; 200806250000 ; 20080625000000
function Lib:GetDate2Time(nDate)
	local nDate = tonumber(nDate);
	if nDate == nil then
		return
	end
	local nSecd = 0;
	local nMin 	= 0;
	local nHour	= 0;
	local nDay 	= 0;
	local nMon 	= 0;
	local nYear = 0;
	if string.len(nDate) == 8 then
		 nDay = math.mod(nDate, 100);
		 nMon = math.mod(math.floor(nDate/100), 100);
		 nYear = math.mod(math.floor(nDate/10000),10000);
	elseif string.len(nDate) == 10 then
		 nHour = math.mod(nDate, 100);
		 nDay = math.mod(math.floor(nDate/100), 100);
		 nMon = math.mod(math.floor(nDate/10000),100);
		 nYear = math.mod(math.floor(nDate/1000000),10000);			 
	elseif string.len(nDate) == 12 then 
		 nMin = math.mod(nDate, 100);
		 nHour= math.mod(math.floor(nDate/100), 100);
		 nDay = math.mod(math.floor(nDate/10000),100);
		 nMon = math.mod(math.floor(nDate/1000000),100);
		 nYear = math.mod(math.floor(nDate/100000000),10000);
	elseif string.len(nDate) == 14 then
		 nSecd = math.mod(nDate, 100);
		 nMin = math.mod(math.floor(nDate/100), 100);
		 nHour= math.mod(math.floor(nDate/10000), 100);
		 nDay = math.mod(math.floor(nDate/1000000),100);
		 nMon = math.mod(math.floor(nDate/100000000),100);
		 nYear = math.mod(math.floor(nDate/10000000000),10000);
	else
		return 0;		
	end
	local tbData = {year=nYear, month=nMon, day=nDay, hour=nHour, min=nMin, sec=nSecd};
	local nSec = Lib:GetSecFromNowData(tbData)
	return nSec;
end

function Lib:GetSecFromNowData(tbData)
	local nSecTime = os.time(tbData);
	return nSecTime;
end

-- 得到类似{year = 1998, month = 9, day = 16, yday = 259, wday = 4, hour = 23, min = 48, sec = 10, isdst = false}的结构
function Lib:GetTimeTable(tSecTime)
	local tbTime = os.date("*t", tSecTime);
	return tbTime;
end

-- 根据一个不完整的timetable得到时间sec
function Lib:GetSecByTimeTable(tbTime)
	local nSecd = 0;
	local nMin 	= 0;
	local nHour	= 0;
	local nDay 	= 0;
	local nMon 	= 0;
	local nYear = 0;
	
	local tNow = KGameBase.GetTime();
	local tbNow = Lib:GetTimeTable(tNow);
	
	if not tbTime.year then
		nYear = tbNow.year;
	else
		nYear = tbTime.year;
	end
	
	if not tbTime.month then
		nMon = tbNow.month;
	else
		nMon = tbTime.month;
	end
	
	if not tbTime.day then
		nDay = tbNow.day;
	else
		nDay = tbTime.day;
	end
	
	if not tbTime.hour then
		nHour = tbNow.hour;
	else
		nHour = tbTime.hour;
	end
	
	if not tbTime.min then
		nMin = tbNow.min;
	else
		nMin = tbTime.min;
	end
	
	if not tbTime.sec then
		nSecd = tbNow.sec;
	else
		nSecd = tbTime.sec;
	end
	
	local tbData = {year=nYear, month=nMon, day=nDay, hour=nHour, min=nMin, sec=nSecd};
	
	local nSec = Lib:GetSecFromNowData(tbData);
	
	return nSec;
end

-- 把字符串中的全局转为半角，有几个特殊转换
function Lib:Convert2Semiangle(szSrcStr)
	local nStrLen = string.len(szSrcStr);
	if (nStrLen < 2) then
		return szSrcStr;
	end

	local szDesStr	= "";
	local nIter 	= 1;
	while(nIter <= nStrLen) do
		if (string.byte(szSrcStr, nIter) == 163 and (nStrLen >= (nIter + 1))) then
			local nChar = string.byte(string.sub(szSrcStr, nIter + 1, nIter + 1)) - 128;
			if (nChar > 0) then
				szDesStr = szDesStr..string.char(nChar);
			end
			nIter = nIter + 2;
		elseif (string.byte(szSrcStr, nIter) == 161  and (nStrLen >= (nIter + 1)) and string.byte(szSrcStr, nIter + 1) == 161) then
			szDesStr = szDesStr.." ";
			nIter = nIter + 2;
		elseif (string.byte(szSrcStr, nIter) == 161  and (nStrLen >= (nIter + 1)) and string.byte(szSrcStr, nIter + 1) == 182) then
			szDesStr = szDesStr.."<";
			nIter = nIter + 2;
		elseif (string.byte(szSrcStr, nIter) == 161  and (nStrLen >= (nIter + 1)) and string.byte(szSrcStr, nIter + 1) == 176) then
			szDesStr = szDesStr.."\"";
			nIter = nIter + 2;
		elseif (string.byte(szSrcStr, nIter) == 161  and (nStrLen >= (nIter + 1)) and string.byte(szSrcStr, nIter + 1) == 177) then
			szDesStr = szDesStr.."\"";
			nIter = nIter + 2;
		elseif (string.byte(szSrcStr, nIter) == 161  and (nStrLen >= (nIter + 1)) and string.byte(szSrcStr, nIter + 1) == 183) then
			szDesStr = szDesStr..">";
			nIter = nIter + 2;
		elseif (string.byte(szSrcStr, nIter) == 161  and (nStrLen >= (nIter + 1)) and string.byte(szSrcStr, nIter + 1) == 163) then
			szDesStr = szDesStr..".";
			nIter = nIter + 2;
		elseif (string.byte(szSrcStr, nIter) > 128) then
			szDesStr = szDesStr..string.sub(szSrcStr, nIter, nIter+1);
			nIter = nIter + 2;
		else
			szDesStr = szDesStr..string.sub(szSrcStr, nIter, nIter);
			nIter = nIter + 1;
		end
	end
	
	return szDesStr;
end

--因Excel表格编辑导致字符串前后带""，把带有""的字符串的""号去掉
function Lib:ClearStrQuote(szParam)
	if szParam == nil then
		szParam = "";
	end
	if string.len(szParam) > 1 then
		local nSit = string.find(szParam, "\"");
		if nSit ~= nil and nSit == 1 then
			local szFlag = string.sub(szParam, 2, string.len(szParam));
			local szLast = string.sub(szParam, string.len(szParam), string.len(szParam));
			szParam = szFlag;
			if szLast == "\"" then
				szParam = string.sub(szParam, 1, string.len(szParam)-1);
			end
		end
	end
	return szParam;
end

--把整形的IP地址转成字符串表示(xxx.xxx.xxx.xxx)
function Lib:IntIpToStrIp(nIp)
	--local nIp = tonumber(nIp);
	if nIp == nil then
		return "";
	end
	local tbIp = {};
	tbIp[1] = self:LoadBits(nIp, 0,  7);
	tbIp[2] = self:LoadBits(nIp, 8, 15);
	tbIp[3] = self:LoadBits(nIp, 16, 23);
	tbIp[4] = self:LoadBits(nIp, 24, 31);
	local szIp = string.format("%d.%d.%d.%d", tbIp[1], tbIp[2], tbIp[3], tbIp[4]);
	return szIp;
end


-- 将传进来的table的数据随机打乱
function Lib:ShuffleInPlace(tb)
	if #tb <= 1 then
		return;
	end
	
	local i = #tb;
	repeat
		local n = KUnify.MathRandom(1, i);
		local temp = tb[i];
		tb[i] = tb[n];
		tb[n] = temp;
		i = i - 1;
	until i == 0
end

-- 按照权重随机出table中里的一个key，val是权重
-- 如 {A=10, B=5, C=7}，随机出A的几率是10/(10+5+7),B的几率是5/(10+5+7),C的几率是7/(10+5+7),ABC可以是任意类型数据
function Lib:RandProb(tbProb)
	local nSum = 0;
	for _, nVal in pairs(tbProb) do
		assert(type(nVal) == "number");
		nSum = nSum + nVal;
	end
	local nRandVal = KUnify.MathRandom(1, nSum);
	nSum = 0;
	
	for Key, nVal in pairs(tbProb) do
		nSum = nSum + nVal;
		if (nSum >= nRandVal) then
			return Key;
		end 
	end
end

-- 从数组中随机选取nNum个元素
function Lib:RandArray(tbArr, nNum)
	assert(#tbArr >= nNum);
	local tbArrSelected = {};
	
	for nIdx, _ in ipairs(tbArr) do
		tbArrSelected[nIdx] = 1;
	end
	
	local tbTempArr = {};
	local tbRandArr = {};
	
	for i = 1, nNum do
		tbTempArr = {};
		for nIdx, nSelect in ipairs(tbArrSelected) do
			if nSelect == 1 then
				table.insert(tbTempArr, nIdx);
			end
		end
		
		local nRandIdx = tbTempArr[KUnify.MathRandom(1, #tbTempArr)];
		table.insert(tbRandArr, tbArr[nRandIdx]);
		tbArrSelected[nRandIdx] = 0;
	end
	
	return tbRandArr;
end

Include("/script/lib/tabfile.lua");

