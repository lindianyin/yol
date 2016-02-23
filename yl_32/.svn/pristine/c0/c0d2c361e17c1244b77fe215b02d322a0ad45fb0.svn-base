
#pragma once

#include "Core.h"
#include <string>


class QTabFileCtrl
{
private:
	ITabFile*	m_pNativeFile;
	char		m_szNativeFileName[MAX_PATH];

public:
	QTabFileCtrl()
	{
		m_pNativeFile = NULL;
		m_szNativeFileName[0] = '\0';
	}

	~QTabFileCtrl()
	{
		SAFE_RELEASE(m_pNativeFile);
		m_szNativeFileName[0] = '\0';
	}

public:
	BOOL	Init(LPCSTR szFileName, std::string aryszColName[], int nColCount)
	{
		if (!Load(szFileName))
			return FALSE;

		for (int i = 0; i < nColCount; i++)
		{
			if (FindColumn(aryszColName[i].c_str()) <= 0)
				InsertNewCol(aryszColName[i].c_str());
		}

		return Save();
	}

	BOOL	Load(LPCSTR szFileName)
	{
		SAFE_RELEASE(m_pNativeFile);

		m_pNativeFile = g_OpenTabFile(szFileName, false, true);

		if (!m_pNativeFile)
			return FALSE;

		QStrCpyLen(m_szNativeFileName, szFileName, sizeof(m_szNativeFileName));

		return TRUE;
	}

	BOOL	Save()
	{
		return Save(m_szNativeFileName);
	}
	BOOL	Save(LPCSTR szFileName)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->Save(szFileName);
	}

	int		SearchStringInColumn(LPCSTR szSearchWord, LPCSTR szSearchColumn, int nStartRow = 1)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return -1;
		}

		return SearchStringInColumn(szSearchWord, m_pNativeFile->FindColumn(szSearchColumn), nStartRow);
	}

	int		SearchStringInColumn(LPCSTR szSearchWord, int nSearchColumn, int nStartRow = 1)
	{
		if (!m_pNativeFile || !szSearchWord ||
			nSearchColumn < 1 || nSearchColumn > m_pNativeFile->GetWidth() ||
			nStartRow < 1 || nStartRow > m_pNativeFile->GetHeight())
		{
			_ASSERT(FALSE);
			return -1;
		}

		for (int i = nStartRow; i <= m_pNativeFile->GetHeight(); i++)
		{
			char szTemp[128];

			if (m_pNativeFile->GetString(i, nSearchColumn, "", szTemp, sizeof(szTemp)) == 1 &&
				strcmp(szTemp, szSearchWord) == 0)
				return i;
		}

		return -1;
	}

	int		SearchStringInRow(LPCSTR szSearchWord, LPCSTR szSearchRow, int nStartColumn = 1)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return -1;
		}

		return SearchStringInRow(szSearchWord, m_pNativeFile->FindRow(szSearchRow), nStartColumn);
	}

	int		SearchStringInRow(LPCSTR szSearchWord, int nSearchRow, int nStartColumn = 1)
	{
		if (!m_pNativeFile || !szSearchWord)
		{
			_ASSERT(FALSE);
			return -1;
		}
		if (nSearchRow < 1 || nSearchRow > m_pNativeFile->GetHeight() ||
			nStartColumn < 1 || nStartColumn > m_pNativeFile->GetWidth())
		{
			return -1;
		}

		for (int i = nStartColumn; i <= m_pNativeFile->GetHeight(); i++)
		{
			char szTemp[128];

			if (m_pNativeFile->GetString(nSearchRow, i, "", szTemp, sizeof(szTemp)) == 1 &&
				strcmp(szTemp, szSearchWord) == 0)
				return i;
		}

		return -1;
	}

	int		FindRow(LPCSTR szRow, int nStartRow = 1)
	{
		return SearchStringInColumn(szRow, 1, nStartRow);
	}

	int		FindColumn(LPCSTR szColumn, int nStartColumn = 1)
	{
		return SearchStringInRow(szColumn, 1, nStartColumn);
	}

	LPCSTR	GetRowName(int nRow)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return NULL;
		}

		return m_pNativeFile->GetRowName(nRow);
	}
	LPCSTR	GetColName(int nCol)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return NULL;
		}

		return m_pNativeFile->GetColName(nCol);
	}

	int		InsertNewCol(LPCSTR szNewCol)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return -1;
		}

		return m_pNativeFile->InsertNewCol(szNewCol);
	}
	BOOL	InsertAfter(int nRow)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->InsertAfter(nRow);
	}
	BOOL	InsertAfter(LPCSTR szRow, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->InsertAfter(szRow, bColumnLab);
	}
	BOOL	InsertBefore(int nRow)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->InsertBefore(nRow);
	}
	BOOL	InsertBefore(LPCSTR szRow, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->InsertBefore(szRow, bColumnLab);
	}

	int		GetWidth()
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return 0;
		}

		return m_pNativeFile->GetWidth();
	}
	int		GetHeight()
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return 0;
		}

		return m_pNativeFile->GetHeight();
	}

	BOOL	GetString(int nRow, LPCSTR szColumn, LPCSTR lpDefault, LPSTR lpRString, DWORD dwSize, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetString(nRow, szColumn, lpDefault, lpRString, dwSize, bColumnLab) == 1);
	}
	BOOL	GetString(int nRow, int nColumn, LPCSTR lpDefault, LPSTR lpRString, DWORD dwSize)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetString(nRow, nColumn, lpDefault, lpRString, dwSize) == 1);
	}
	BOOL	GetString(LPCSTR szRow, LPCSTR szColumn, LPCSTR lpDefault, LPSTR lpRString, DWORD dwSize)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetString(szRow, szColumn, lpDefault, lpRString, dwSize) == 1);
	}

	BOOL	GetInteger(int nRow, LPCSTR szColumn, int nDefault, int *pnValue, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetInteger(nRow, szColumn, nDefault, pnValue, bColumnLab) == 1);
	}
	BOOL	GetInteger(int nRow, int nColumn, int nDefault, int *pnValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetInteger(nRow, nColumn, nDefault, pnValue) == 1);
	}
	BOOL	GetInteger(LPCSTR szRow, LPCSTR szColumn, int nDefault, int *pnValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetInteger(szRow, szColumn, nDefault, pnValue) ==  1);
	}

	BOOL	GetFloat(int nRow, LPCSTR szColumn, float fDefault, float *pfValue, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetFloat(nRow, szColumn, fDefault, pfValue, bColumnLab) == 1);
	}
	BOOL	GetFloat(int nRow, int nColumn, float fDefault, float *pfValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetFloat(nRow, nColumn, fDefault, pfValue) == 1);
	}
	BOOL	GetFloat(LPCSTR szRow, LPCSTR szColumn, float fDefault, float *pfValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetFloat(szRow, szColumn, fDefault, pfValue) == 1);
	}

	BOOL	WriteString(int nRow, LPCSTR szColumn, LPCSTR lpString,  BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteString(nRow, szColumn, lpString, bColumnLab);
	}
	BOOL	WriteString(int nRow, int nColumn,  LPCSTR lpString,  DWORD dwSize = 0)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteString(nRow, nColumn, lpString, dwSize);
	}
	BOOL	WriteString(LPCSTR szRow, LPCSTR szColumn,  LPCSTR lpString)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteString(szRow, szColumn, lpString);
	}

	BOOL	WriteInteger(int nRow, LPCSTR szColumn, int nValue, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteInteger(nRow, szColumn, nValue, bColumnLab);
	}
	BOOL	WriteInteger(int nRow, int nColumn, int nValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteInteger(nRow, nColumn, nValue);
	}
	BOOL	WriteInteger(LPCSTR szRow, LPCSTR szColumn, int nValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteInteger(szRow, szColumn, nValue);
	}

	BOOL	WriteFloat(int nRow, LPCSTR szColumn, float fValue, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteFloat(nRow, szColumn, fValue, bColumnLab);
	}
	BOOL	WriteFloat(int nRow, int nColumn,float fValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteFloat(nRow, nColumn, fValue);
	}
	BOOL	WriteFloat(LPCSTR szRow, LPCSTR szColumn,  float fValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteFloat(szRow, szColumn, fValue);
	}

	BOOL	Remove(int nRow)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->Remove(nRow);
	}
	BOOL	Remove(LPCSTR szRow, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return Remove(szRow, bColumnLab);
	}

	void	Clear()
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return;
		}

		m_pNativeFile->Clear();
	}
};


