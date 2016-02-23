/* -------------------------------------------------------------------------
//	文件名		：	kfilescript_table.hpp
//	创建者		：	JXLuaMapTableMaker
//	功能描述	：	本文件由VS2005 Macro自动生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"TabFile_Load", LuaTabFile_Load, "", "ss[d]", "filename;szKey;bWritable是否可写;", ""},
	{"TabFile_UnLoad", LuaTabFile_UnLoad, "", "s", "szKey;", ""},
	{"TabFile_GetRowCount", LuaTabFile_GetRowCount, "获取指定Table文件的高度（包括表头）", "s", "szKey;", "INT 文件的高度"},
	{"TabFile_GetCell", LuaTabFile_GetCell, "获取指定Table文件指定位置的值（行、列都是从1起始）", "sd?[s]", "strFileKey;nRow;nColumn / strColumnName;strDefaultValue;", "string 数据 （"" 表示获取失败 ）"},
	{"TabFile_SetCell", LuaTabFile_SetCell, "设置指定Table文件指定位置的值（行、列都是从1起始）", "sd?s", "strFileKey;nRow;nColumn / strColumnName;strValue;", "设置结果	1：成功		0：失败"},
	{"TabFile_Search", LuaTabFile_Search, "搜索某一列的指定值", "s?s[d]", "strFileKey;nColumn / strColumnName;strSearchWord;[nStartRow = 2];", "指定值所在行索引，-1失败"},
	{"TabFile_SaveFile", LuaTabFile_SaveFile, "保存Table文件", "s", "strFileKey;", "保存结果	1：成功		0：失败"},
	{"IniFile_Load", LuaIniFile_Load, "", "ss", "szFileName;szKey;", "0 or 1"},
	{"IniFile_UnLoad", LuaIniFile_UnLoad, "", "s", "szKey;", ""},
	{"IniFile_GetData", LuaIniFile_GetData, "获取指定Ini文件指定Section和Key的值", "sss", "szFileKey;szSection;szKey;", "string 数据 （"" 表示获取失败）"},
	{"IniFile_SetData", LuaIniFile_SetData, "设置值", "ssss", "szFileKey;szSection;szKey;szValue;", ""},
	{"IniFile_EraseSection", LuaIniFile_EraseSection, "删除Section", "ss", "szFileKey;szSection;", ""},
	{"IniFile_EraseKey", LuaIniFile_EraseKey, "删除Section下的Key", "sss", "szFileKey;szSection;szKey;", ""},
	{"File_CreateFile", LuaFile_CreateFile, "", "s", "szFileName;", ""},
	{"IniFile_SaveFile", LuaIniFile_SaveFile, "", "ss", "strSrcKeyName;strDesFileName;", "0 or 1"},
	{"LoadXmlFile", LuaLoadXmlFile, "载入Xml文件", "s", "szFileName;", "tbXmlData"},
	{"WriteFile", LuaWriteFile, "写入文件（不一定非得是文本文件）", "ss", "szFilePath: Txt文件路径;szContent: 文件内容（可以是非文本的字符串，即可以包含特殊字符'\0'等）;", "1、表示成功"},
	{"AppendFile", LuaAppendFile, "追加写入文件（不一定非得是文本文件）", "ss", "szFilePath: Txt文件路径;szContent: 文件内容（可以是非文本的字符串，即可以包含特殊字符'\0'等）;", "1、表示成功"},
	{"ReadTxtFile", LuaReadTxtFile, "读取Txt文件", "s", "pszTxtFilePath: Txt文件路径;", ""},
	{"IsFileExist", LuaIsFileExist, "文件是否存在", "s", "文件路径;", "1存在，0不存在"},
};
