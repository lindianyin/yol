/* -------------------------------------------------------------------------
//	文件名		：	klibscript_table.hpp
//	创建者		：	JXLuaMapTableMaker
//	功能描述	：	本文件由VS2005 Macro自动生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"FormatTime2String", LuaFormatTime2String, "根据给定的秒数，转换成制定格式的字符串返回", "sd", "szParam：格式;tTime：秒数;", "str：格式化后的时间表示"},
	{"GetRootPath", LuaGetRootPath, "", "", "", "返回根目录"},
	{"SplitStr", LuaSplitStr, "以一个分隔符切分字符串", "ss", "要切分的字符串;切分符（取参数第一个字符）;", "切分的子字符串数组，失败返回nil"},

	{"Number2Int", LuaNumber2Int, "把数字转换成INT（取整的同时转为有符号数字）", "d", "dNumber:要取整的数;", "整数"},
	{"Number2UInt", LuaNumber2UInt, "把数字转换成UINT（取整的同时转为无符号数字）", "d", "dNumber:被转换的数;", "转换后的数"},
	{"FileName2Id", LuaFileName2Id, "返回指定文件的哈希ID", "s", "pszFile：文件名;", "DWORD 哈希ID, 失败返回nil"},
	{"String2Id", LuaString2Id, "返回指定字符串的哈希ID", "s", "str: 字符串;", ""},
	{"GetValByStr", LuaGetValByStr, "按照字符串取得Table成员变量，支持多级“.”和“:”。", "s", "字符串;", "value"},
	{"SaveValue2Buffer", LuaSaveValue2Buffer, "将一个值转换成可以储存、传送的Buffer", "{}", "", "Buffer"},
	{"LoadBuffer2Value", LuaLoadBuffer2Value, "将一个Buffer换成值", "s", "Buffer;", "value"},
	{"GetBitTB", LuaGetBitTB, "将一个数值装换成一个表示每个二进制位的Table", "d", "Number;", "table"},	
	{"LoadTabFile", LuaLoadTabFile, "加载table文件", "s", "文件名;", "table对象"},
	{"LoadIniFile", LuaLoadIniFile, "加载ini文件", "s", "文件名;", "ini对象"},
	{"FindStr", LuaFindStr, "在字符串中搜索子串，会跳过不安全的双字节汉字", "ss[d]", "szStr: 原字符串;szSub: 要查找的子串;nStart: 搜索起始位置（下标从1开始，不是0）;", "nil表示未找到；否则返回找到的子串起始位置（下标从1开始，不是0）"},
	{"GetScriptMemSize", LuaGetScriptMemSize, "得到脚本已分配内存大小", "", "", "nSize"},
	{"GetScriptAllocTimes", LuaGetScriptAllocTimes, "得到脚本已分配内存次数", "", "", "nTimes"},
	{"GetScriptBlockMaxSize", LuaGetScriptBlockMaxSize, "得到脚本已分配的内存块的最大大小", "", "", "nSize"},
	{"GetScriptAllocMemSize", LuaGetScriptAllocMemSize, "得到脚本全部已分配的内存块大小", "", "", "nSize"},
	{"GetScriptFreeMemSize", LuaGetScriptFreeMemSize, "得到脚本未使用的缓冲内存块大小", "", "", "nSize"},
	{"ReleaseScriptFreeMemory", LuaReleaseScriptFreeMemory, "释放脚本不用的缓冲内存", "", "", "1、成功；0、失败"},
	{"ClrScr", LuaClrScr, "清空控制台信息", "", "", ""},
	{"HashDword", LuaHashDword, "获得参数对应的32位Hash值，若连续用上一个值获取，相当于得到一连串伪随机数", "d", "32位参数(随机种子);", ""},
	{"GetMD5String", LuaGetMD5String, "用MD5加密一个字符串", "s", "一个要加密的字符串;", ""},
	//{"Base64Encode", LuaBase64Encode, "base64编码", "s", "一个要编码的字符串;", ""},
	//{"Base64Decode", LuaBase64Decode, "base64解码", "s", "一个要解码的字符串;", ""},
};
