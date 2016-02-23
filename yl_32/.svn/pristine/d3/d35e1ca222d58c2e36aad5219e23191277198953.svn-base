
#pragma once

#include <io.h>
#include <direct.h>
#include <string>
#include <vector>

template<class KToolT>
struct _KTraverse_DirDefaultTool
{
	bool DealForSubDir(KToolT& tool, LPCSTR szRootPath, LPCSTR szSubDir){return true;}
};

template <class KToolT, class KDirToolT = _KTraverse_DirDefaultTool<KToolT> >
class KTraverseInDir
{
public:
	KTraverseInDir(bool bReocrdFiles) : 
		m_nFileCount(0), m_bReocrdFiles(bReocrdFiles)
	{
	}
	~KTraverseInDir()
	{
	}

private:
	INT m_nFileCount;
	bool m_bReocrdFiles;
	typedef std::vector<std::string> FILERECORD;
	FILERECORD m_arFiles;
	//typedef KDirToolT<KToolT>	_KDirTool;
	KDirToolT	m_dirTool;
public:
	// -------------------------------------------------------------------------
/*	BOOL FilterFileName(LPCSTR szFileName, LPCSTR szKeyName)
	{
		std::string strName = szFileName;
		INT nPos = strName.find(szKeyName, 0);
		if (nPos >= 0)
			return TRUE;

		return FALSE;
	}
*/
	INT GetFileCount()
	{
		return m_arFiles.size();
	}
	LPCSTR GetFileName(INT nIndex)
	{
		if (nIndex < 0 || nIndex >= GetFileCount())
			return "";

		return m_arFiles[nIndex].c_str();
	}

	VOID TraverseFile(LPCSTR lpszRootDir, LPCSTR lpszSubDir, KToolT& tool)
	{
		INT				nFlag;
		CHAR			szRealDir[MAX_PATH];

		if (lpszSubDir && strlen(lpszSubDir))
			sprintf(szRealDir, "%s\\%s", lpszRootDir, lpszSubDir);
		else
			sprintf(szRealDir, "%s", lpszRootDir);

		if(chdir(szRealDir))
			return;
		_finddata_t FindData;
		INT dir = _findfirst("*.*", &FindData);
		while(dir != -1) {
			if(strcmp(FindData.name, ".") == 0 || strcmp(FindData.name, "..") == 0)	{
				if(_findnext(dir, &FindData)) break;
				continue;
			}
			if(FindData.attrib == _A_SUBDIR)
			{
				TraverseFile(szRealDir, FindData.name, tool);
				
				m_dirTool.DealForSubDir(tool, szRealDir, FindData.name);
			}
			else
			{	
				nFlag = 0;
				for (INT i = 0; i < (INT)strlen(FindData.name);  i++)
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
					TraverseFile(szRealDir, FindData.name, tool);
					m_dirTool.DealForSubDir(tool, szRealDir, FindData.name);
				}
				else
				{
					// CHAR szExt[50];
					if (strlen(FindData.name) >= 4) 
					{
						//strcpy(szExt, FindData.name + strlen(FindData.name) - 4);
						//_strupr(szExt);
						if (tool.IsEqualFile(FindData.name))
						{
							m_nFileCount ++;
							//printf("SearchFile Count: %d\r\n", m_nFileCount);
							std::string strFullName = szRealDir;
							if ('\\' != strFullName[strFullName.length() - 2])
								strFullName += "\\";
							strFullName += FindData.name;
							tool.DealForFile(strFullName.c_str(), szRealDir);

							if (m_bReocrdFiles)
							{
								m_arFiles.push_back(strFullName);
							}
						}
					}
				}
			}
			if(_findnext(dir, &FindData)) break;
		} 
		_findclose(dir);
		chdir(lpszRootDir);
	}

	VOID TraverseDir(LPCSTR lpszRootDir, LPCSTR lpszSubDir, KToolT& tool)
	{
		INT				nFlag;
		CHAR			szRealDir[MAX_PATH];

		if (lpszSubDir && strlen(lpszSubDir))
			sprintf(szRealDir, "%s\\%s", lpszRootDir, lpszSubDir);
		else
			sprintf(szRealDir, "%s", lpszRootDir);

		if(chdir(szRealDir))
			return;
		_finddata_t FindData;
		INT dir = _findfirst("*.*", &FindData);
		while(dir != -1) 
		{
			if(strcmp(FindData.name, ".") == 0 || strcmp(FindData.name, "..") == 0)	
			{
				if(_findnext(dir, &FindData)) break;
				continue;
			}
			if(FindData.attrib == _A_SUBDIR)
			{
				m_nFileCount ++;
				tool.DealForDir(FindData.name, 
							FindData.time_create, FindData.time_write);
				if (m_bReocrdFiles)
				{
					m_arFiles.push_back(FindData.name);
				}
			}
			else
			{	
				nFlag = 0;
				for (INT i = 0; i < (INT)strlen(FindData.name);  i++)
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
					m_nFileCount ++;
					tool.DealForDir(FindData.name, 
								FindData.time_create, FindData.time_write);
					if (m_bReocrdFiles)
					{
						m_arFiles.push_back(FindData.name);
					}
				}
			}
			if(_findnext(dir, &FindData)) break;
		} 
		_findclose(dir);
		chdir(lpszRootDir);
	}
};

// -------------------------------------------------------------------------

