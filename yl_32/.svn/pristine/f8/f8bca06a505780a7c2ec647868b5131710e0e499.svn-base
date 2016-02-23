#include "CorePrivate.h"

#ifdef SCRIPT_BINDK

#include "QLuaScriptSet.h"
#include "QLuaScript.h"

#include "Runtime/Debug.h"

#include<sys/stat.h>
#ifndef WIN32
	#include <unistd.h>
	#include <sys/types.h>
	#include <dirent.h>
#else 
	#include <io.h>
	#include <direct.h>
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


QLuaScriptSet::QLuaScriptSet(int nSetSize)
{
	m_nSetSize = nSetSize;
	m_ScriptSet = new SCRIPT_INFO[nSetSize];
	m_dwCurrentScriptNum = 0;
	m_PreLoadCallBack = NULL;
	m_bVirtualLoadMode = FALSE;
}

QLuaScriptSet::~QLuaScriptSet()
{
	delete[] m_ScriptSet;
}

//由文件ID查找一个文件名
LPCSTR QLuaScriptSet::FindFile(DWORD dwScriptId)
{
	ITOR_SCRIPTNODE it = m_NodeSet.find(dwScriptId);
	if (it != m_NodeSet.end() && it->second < m_dwCurrentScriptNum)
	{
		return m_ScriptSet[it->second].m_szFileName;
	}
	return NULL;
}

//加载一个脚本文件
SCRIPT_SET_RESULT QLuaScriptSet::LoadScript(LPCSTR cszScriptPath, BOOL bReloadExist /* = FALSE */)
{
	if (!cszScriptPath || !cszScriptPath[0])
		return SSR_LOAD_ERROR;

	char szRelativePath[MAX_PATH] = "";
	char szFullPath[MAX_PATH] = "";

	g_GetRelativePath(szRelativePath, cszScriptPath);
	g_GetFullPath(szFullPath, cszScriptPath);

	DWORD dwID = GenID(szRelativePath);
	ITOR_SCRIPTNODE pNode = m_NodeSet.find(dwID);
	//已在set中
	if (pNode != m_NodeSet.end())
	{
		if (bReloadExist)
			return _ReloadScript(pNode->second);
		else
			return SSR_OK;
	}
	if (m_dwCurrentScriptNum < (DWORD)m_nSetSize)
	{
		if (m_bVirtualLoadMode)
		{
			QStrCpy(m_ScriptSet[m_dwCurrentScriptNum].m_szFileName, szRelativePath);
			m_ScriptSet[m_dwCurrentScriptNum].m_bLoaded = FALSE;
		}
		else
		{
			m_ScriptSet[m_dwCurrentScriptNum].m_Script.Init();
			//注册游戏运行函数库
			if (m_PreLoadCallBack)
				m_PreLoadCallBack(&m_ScriptSet[m_dwCurrentScriptNum].m_Script, m_ScriptSet[m_dwCurrentScriptNum].m_szFileName);
			//记录文件更新时间
#ifdef _CHECK_FILE_TIME
			struct stat bufStat;
			if (stat(szFullPath, &bufStat) == 0)
			{
				m_ScriptSet[m_dwCurrentScriptNum].m_LastModifyTime = bufStat.st_mtime;
			}
			else
			{
				m_ScriptSet[m_dwCurrentScriptNum].m_LastModifyTime = 0;
			}
#endif
			if (m_ScriptSet[m_dwCurrentScriptNum].m_Script.DoFile(szRelativePath))
			{
				m_ScriptSet[m_dwCurrentScriptNum].m_bLoaded = TRUE;
			}
			else
			{
				m_ScriptSet[m_dwCurrentScriptNum].m_Script.Exit();
				g_DebugLog("Cann't load file %s, please check it!\n", szRelativePath);
				printf("\nCann't load file %s, please check it!\n", szRelativePath);
				return SSR_LOAD_ERROR;
			}
		}
	}
	else
	{
		_ASSERT(FALSE);
		g_DebugLog("The number of script file is overflow!\n");
		printf("The number of script file is overflow!\n");
		return SSR_OVERFLOW;
	}	
	printf("\b\b\b\b\b\b%.6d", m_dwCurrentScriptNum);
	m_NodeSet[dwID] = m_dwCurrentScriptNum++;
	return SSR_OK;
}

//加载某目录下含子目录的所有脚本（传入目录参数为绝对路径）
SCRIPT_SET_RESULT QLuaScriptSet::_LoadScriptInDirectory(LPCSTR cszDirFullPath, BOOL bReloadExist /* = FALSE */)
{
	int nFlag;
	char szOldPath[MAX_PATH];
	char buf[MAX_PATH];

	strcpy(buf, cszDirFullPath);

	int nPathLen = strlen(buf);
	getcwd(szOldPath, sizeof(szOldPath));

#ifdef WIN32
	if(chdir(buf))
		return SSR_OK;

	buf[nPathLen++] = '\\';
	buf[nPathLen] = '\0';
	_finddata_t FindData;
	long dir = _findfirst("*.*", &FindData);
	while (dir != -1) 
	{
		if (QStrCmp(FindData.name, ".") || QStrCmp(FindData.name, ".."))
		{
			if(_findnext(dir, &FindData))
				break;

			continue;
		}
		if(FindData.attrib & _A_SUBDIR)
		{
			if (_LoadScriptInDirectory(strcat(buf, FindData.name)) == SSR_OVERFLOW)
			{
				chdir(szOldPath);
				return SSR_OVERFLOW;
			}
			buf[nPathLen] = '\0';
		}
		else
		{	
			nFlag = 0;
			for (int i = 0; i < (int)strlen(FindData.name);  i++)
			{
				if (FindData.name[i] == '.')
					break;
				if (FindData.name[i] == '\\')
				{
					nFlag = 1;
					break;
				}
			}
			if (nFlag == 1)
			{
				if (_LoadScriptInDirectory(strcat(buf, FindData.name)) == SSR_OVERFLOW)
				{
					chdir(szOldPath);
					return SSR_OVERFLOW;
				}				
				buf[nPathLen] = '\0';
			}
			else
			{
				char szExt[50];
				if (strlen(FindData.name) >= 4)
				{
					strcpy(szExt, FindData.name + strlen(FindData.name) - 4);
					QStrUpper(szExt);
					if (QStrCmp(szExt, ".LUA"))
					{
						if (LoadScript(strcat(buf, FindData.name)) == SSR_OVERFLOW)
						{
							chdir(szOldPath);
							return SSR_OVERFLOW;
						}
						buf[nPathLen] = '\0';
					}
				}
			}
		}
		if(_findnext(dir, &FindData))
			break;
	} 
	_findclose(dir);
	chdir(szOldPath);

#else

	DIR *dp;
	int i;
	struct dirent *ep;

	if(chdir(buf))
		return SSR_OK;

	 buf[nPathLen++] = '/';
	 buf[nPathLen] = '\0';
     dp = opendir(".");
     if(dp) 
	 {
          while(ep = readdir(dp)) 
		  {
			  if(QStrCmp(ep->d_name, ".") || QStrCmp(ep->d_name, ".."))
				  continue;

			  if(ep->d_type ==4) 
			  {
				  if (_LoadScriptInDirectory(strcat(buf, ep->d_name)) == SSR_OVERFLOW)
				  {
					  chdir(szOldPath);
					  return SSR_OVERFLOW;
				  }
				  buf[nPathLen] = '\0';
			  }
			  else
			  {
				  nFlag = 0;
				  for (i = 0; i < (int)strlen(ep->d_name);  i++)
				  {
					  if (ep->d_name[i] == '.')
						  break;
					  if (ep->d_name[i] == '/')
					  {
						  nFlag = 1;
						  break;
					  }
				  }
				  if (nFlag == 1)
				  {
					  if (_LoadScriptInDirectory(strcat(buf, ep->d_name)) == SSR_OVERFLOW)
					  {
						  chdir(szOldPath);
						  return SSR_OVERFLOW;
					  }
					  buf[nPathLen] = '\0';
				  }
				  else
				  {
					  char szExt[50];
					  if (strlen(ep->d_name) >= 4)
					  {
						  strcpy(szExt, ep->d_name + strlen(ep->d_name) - 4);
						  QStrUpper(szExt);
						  if (QStrCmp(szExt, ".LUA"))
						  {
							  if (LoadScript(strcat(buf, ep->d_name)) == SSR_OVERFLOW)
							  {
								  chdir(szOldPath);
								  return SSR_OVERFLOW;
							  }
							  buf[nPathLen] = '\0';
						  }
					  }
				  }
			  }
		  }
          closedir(dp);
     }
	chdir(szOldPath);
#endif

	return SSR_OK;
}

//加载某目录下含子目录的所有脚本（传入目录参数为相对路径）
SCRIPT_SET_RESULT QLuaScriptSet::LoadScriptInDirectory(LPCSTR cszDirPath, BOOL bReloadExist /* = FALSE */)
{
	char szFullPath[MAX_PATH];

	g_GetFullPath(szFullPath, cszDirPath);

	for (char* ptr = szFullPath; *ptr != '\0'; ptr++)
	{
#ifdef WIN32
		if(*ptr == '/')
			*ptr = '\\';
#else
		if(*ptr == '\\')
			*ptr = '/';
#endif
	}

	int nLen = strlen(szFullPath);
	if (szFullPath[nLen - 1] == '\\' || szFullPath[nLen - 1] == '/')
		szFullPath[nLen - 1] = '\0';

	return _LoadScriptInDirectory(szFullPath);
}

//重新加载脚本文件，如之前未加载(包括预加载)，相当于LoadScript
SCRIPT_SET_RESULT QLuaScriptSet::ReloadScript(LPCSTR cszScriptPath)
{
	if (!cszScriptPath || !cszScriptPath[0])
		return SSR_LOAD_ERROR;

	char szRelativePath[MAX_PATH] = "";
	g_GetHalfPath(szRelativePath, cszScriptPath);

	DWORD dwID = GenID(szRelativePath);
	ITOR_SCRIPTNODE pNode = m_NodeSet.find(dwID);
	if (pNode == m_NodeSet.end())
	{
		//找不到文件，加载
		return LoadScript(szRelativePath);
	}

	return _ReloadScript(pNode->second);
}

SCRIPT_SET_RESULT QLuaScriptSet::_ReloadScript(UINT index)
{
	if (index >= m_dwCurrentScriptNum)
	{
		_ASSERT(FALSE);
		return SSR_OVERFLOW;
	}
	//文件处于预加载状态，返回
	if (m_ScriptSet[index].m_bLoaded == FALSE)
		return SSR_OK;
	if (m_bVirtualLoadMode)
	{
		m_ScriptSet[index].m_bLoaded = FALSE;
		return SSR_OK;
	}
	char buf[MAX_PATH];
#ifdef _CHECK_FILE_TIME
	struct stat bufStat;
	if (stat(GetFullPath(buf, m_ScriptSet[index].m_szFileName), &bufStat) == 0)
	{
		if (m_ScriptSet[index].m_LastModifyTime == bufStat.st_mtime)
			return SSR_OK;
		else
			m_ScriptSet[index].m_LastModifyTime = bufStat.st_mtime;
	}
#endif
	strcpy(buf, m_ScriptSet[index].m_szFileName);
	m_ScriptSet[index].m_Script.Exit();
	m_ScriptSet[index].m_Script.Init();
	if (m_PreLoadCallBack)
		m_PreLoadCallBack(&m_ScriptSet[index].m_Script, m_ScriptSet[index].m_szFileName);
	printf("Reloading...: %s\n", buf);
	if (!m_ScriptSet[index].m_Script.DoFile(buf))
	{
		m_ScriptSet[index].m_bLoaded = FALSE;
		return SSR_LOAD_ERROR;
	}
	return SSR_OK;	
}

//重新加载所有脚本
int  QLuaScriptSet::ReloadAllScript(BOOL bForce)
{
	UINT i, nReloaded = 0;
	printf("Reloading Files.....\n");
	for (i = 0; i < m_dwCurrentScriptNum; i++)
	{
		if (m_ScriptSet[i].m_bLoaded == FALSE)
			continue;
		if (m_bVirtualLoadMode)
		{
			m_ScriptSet[i].m_bLoaded = FALSE;
		}
		else
		{
			char szFileName[MAX_PATH];
#ifdef _CHECK_FILE_TIME
			struct stat bufStat;
			if (stat(GetFullPath(szFileName, m_ScriptSet[i].m_szFileName), &bufStat) == 0)
			{
				if (!bForce)
				{
					if (m_ScriptSet[i].m_LastModifyTime == bufStat.st_mtime)
							continue;
					printf("Reloading File: %s\n", m_ScriptSet[i].m_szFileName);
				}
				m_ScriptSet[i].m_LastModifyTime = bufStat.st_mtime;
			}
			else
				continue;
#endif
			strcpy(szFileName, m_ScriptSet[i].m_szFileName);
			m_ScriptSet[i].m_Script.Exit();
			m_ScriptSet[i].m_Script.Init();
			if (m_PreLoadCallBack)
				m_PreLoadCallBack(&m_ScriptSet[i].m_Script, m_ScriptSet[i].m_szFileName);
			if (!m_ScriptSet[i].m_Script.DoFile(szFileName))
			{
				m_ScriptSet[i].m_bLoaded = FALSE;
				printf("Cann't load %s, please check this file!\n", szFileName);
			}
			else
			{
				nReloaded ++;
				printf("\b\b\b\b\b\b%.6d", nReloaded);
			}
		}
	}
	return nReloaded;
}
//由文件ID查找并获取一个脚本，文件ID可由g_FileName2Id在外部构造(本不应如此但已成现实)
QLuaScript* QLuaScriptSet::GetScript(DWORD dwScriptId)
{
	ITOR_SCRIPTNODE it = m_NodeSet.find(dwScriptId);
	if (it != m_NodeSet.end())
	{
		if (it->second < m_dwCurrentScriptNum)
		{
			if (!m_ScriptSet[it->second].m_bLoaded)
			{
				char buf[MAX_PATH];
				strcpy(buf, m_ScriptSet[it->second].m_szFileName);
				m_ScriptSet[it->second].m_Script.Exit();
				m_ScriptSet[it->second].m_Script.Init();
				if (m_PreLoadCallBack)
					m_PreLoadCallBack(&m_ScriptSet[it->second].m_Script, m_ScriptSet[it->second].m_szFileName);
				QStrCpy(m_ScriptSet[it->second].m_szFileName, buf);
#ifdef _CHECK_FILE_TIME
				struct stat bufStat;
				if (stat(GetFullPath(buf, m_ScriptSet[it->second].m_szFileName), &bufStat) == 0)
				{
					m_ScriptSet[it->second].m_LastModifyTime = bufStat.st_mtime;
				}
				else
				{
					m_ScriptSet[it->second].m_LastModifyTime = 0;
				}
#endif
				if(m_ScriptSet[it->second].m_Script.DoFile(m_ScriptSet[it->second].m_szFileName))
				{
					m_ScriptSet[it->second].m_bLoaded = TRUE;
					return &m_ScriptSet[it->second].m_Script;
				}
			}
			else
			{
				return &m_ScriptSet[it->second].m_Script;
			}
		}
		else
		{
			_ASSERT(!"Error on KJxScriptSet::GetScript");
		}
	}
	return NULL;
}

//清除所有脚本缓存
void QLuaScriptSet::ClearAll()
{
	m_NodeSet.clear();
	m_dwCurrentScriptNum = 0;
}

#endif	// SCRIPT_BINDK
