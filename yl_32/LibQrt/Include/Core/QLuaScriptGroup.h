
#pragma once

#include "QLuaScript.h"
#include <map>

#pragma warning(disable: 4251)

class CORE_API QScriptGroup
{
private:
	enum _QE_FILESTATE
	{
		emFILESTATE_NONE,
		emFILESTATE_LOADING,
		emFILESTATE_LOADED,
		emFILESTATE_ERROR,
	};

	typedef std::map<DWORD, _QE_FILESTATE> _MapFileState;

public:
	// 从脚本的全局变量中得到本类的指针
	static QScriptGroup* GetThis(QLuaScript& sc);

	//// 设置包伴侣文件，以后遍历读脚本的时候会自动使用这个伴侣遍历包内
	//static BOOL SetPackPartnerFile(LPCSTR pszPackPartnerFilePath);

private:
	// 不重复地引用其它脚本
	static int _LuaInclude(QLuaScript& sc);

	// 强行载入指定脚本，即使已经载入也会再载入一遍
	static int _LuaDoFile(QLuaScript& sc);

public:
	QScriptGroup(LPCSTR pszGroupPath = NULL);
	~QScriptGroup()	{ m_scScript.Exit(); }

	// 清除脚本缓存
	// bClearAll：TURE则将脚本环境完全清除（脚本内的全部变量都会清空），FALSE则只将所有文件置为未载入
	void Init(BOOL bClearAll, BOOL bDebug = FALSE);

	void Uninit()
	{
		m_scScript.Exit();
		m_scScript.ReleaseAllFreeMemory();
		m_mapFileStates.clear();
	}

	// 加载目录下全部脚本文件
	BOOL LoadAllScript()
	{ return LoadScriptInDirectory(m_szGroupPath); }

	// 加载指定目录下全部脚本文件
	BOOL LoadScriptInDirectory(LPCSTR pszScriptPath);

	// 加载指定脚本，用于不使用LoadAll方式载入脚本的地方
	BOOL LoadScript(LPCSTR pszFileName, BOOL bFource = FALSE);

	// 获取脚本指针
	QLuaScript* GetScript()
	{ return &m_scScript; }

	// 获取已加载脚本文件数
	DWORD GetFileCount() const
	{ return (DWORD)m_mapFileStates.size(); }

	// 设置是否可以加载包外文件（默认可以）
	BOOL SetCanUseUnpackFile(BOOL bCanUseUnpackFile)
	{ m_bCanUseUnpackFile = bCanUseUnpackFile; return TRUE; }

private:
	// 不重复加载指定脚本（传入目录参数为相对路径）
	BOOL _LoadScript(LPCSTR pszFileName);

	// 加载指定脚本，可能重复（传入目录参数为相对路径）
	BOOL _DoFile(LPCSTR pszFileName);

	// 加载某目录下含子目录的所有脚本（传入目录参数为绝对路径）
	BOOL _LoadScriptInDirectory(LPCSTR pszDirFullPath);

private:
	char			m_szGroupPath[MAX_PATH];
	QLuaScript		m_scScript;
	_MapFileState	m_mapFileStates;
	BOOL			m_bCanUseUnpackFile;
};


