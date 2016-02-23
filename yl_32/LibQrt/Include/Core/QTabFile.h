
#pragma once

#include "Core.h"


class QTabFile
{
private:
	ITabFile*	m_pNativeFile;

public:
	QTabFile()
	{
		m_pNativeFile = NULL;
	}

	~QTabFile()
	{
		SAFE_RELEASE(m_pNativeFile);
	}

public:
	BOOL	Load(LPCSTR szFileName)
	{
		SAFE_RELEASE(m_pNativeFile);

		m_pNativeFile = g_OpenTabFile(szFileName);

		if (!m_pNativeFile)
			return FALSE;

		return TRUE;
	}

	BOOL	Save(LPCSTR szFileName)
	{
		//ÎÞ·¨±£´æ
		_ASSERT(FALSE);
		return FALSE;
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
		if (!m_pNativeFile || !szSearchWord ||
			nSearchRow < 1 || nSearchRow > m_pNativeFile->GetHeight() ||
			nStartColumn < 1 || nStartColumn > m_pNativeFile->GetWidth())
		{
			_ASSERT(FALSE);
			return -1;
		}

		for (int i = nStartColumn; i <= m_pNativeFile->GetWidth(); i++)
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

	BOOL	GetInteger(int nRow, LPCSTR szColumn, int nDefault, int* pnValue, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetInteger(nRow, szColumn, nDefault, pnValue, bColumnLab) == 1);
	}
	BOOL	GetInteger(int nRow, int nColumn, int nDefault, int* pnValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetInteger(nRow, nColumn, nDefault, pnValue) == 1);
	}
	BOOL	GetInteger(LPCSTR szRow, LPCSTR szColumn, int nDefault, int* pnValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetInteger(szRow, szColumn, nDefault, pnValue) == 1);
	}

	BOOL	GetFloat(int nRow, LPCSTR szColumn, float fDefault, float* pfValue, BOOL bColumnLab = TRUE)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetFloat(nRow, szColumn, fDefault, pfValue, bColumnLab) == 1);
	}
	BOOL	GetFloat(int nRow, int nColumn, float fDefault, float* pfValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetFloat(nRow, nColumn, fDefault, pfValue) == 1);
	}
	BOOL	GetFloat(LPCSTR szRow, LPCSTR szColumn, float fDefault, float* pfValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return (m_pNativeFile->GetFloat(szRow, szColumn, fDefault, pfValue) == 1);
	}

	void	Clear()
	{
		if (!m_pNativeFile)
			return;

		m_pNativeFile->Clear();
	}
};

