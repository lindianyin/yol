
#pragma once

#include "Core.h"


class QIniFile
{
private:
	IIniFile*	m_pNativeFile;

public:
	QIniFile()
	{
		m_pNativeFile = NULL;
	}

	~QIniFile()
	{
		SAFE_RELEASE(m_pNativeFile);
	}

public:
	void	Create()
	{
		m_pNativeFile = g_CreateIniFile();
		_ASSERT(m_pNativeFile);
	}
	BOOL	Load(LPCSTR szFileName, BOOL bForWrite = FALSE)
	{
		SAFE_RELEASE(m_pNativeFile);

		m_pNativeFile = g_OpenIniFile(szFileName, FALSE, bForWrite);

		if (!m_pNativeFile)
			return FALSE;

		return TRUE;
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

	void	Clear()
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return;
		}

		m_pNativeFile->Clear();
	}

	int		GetSectionCount()
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return 0;
		}

		return m_pNativeFile->GetSectionCount();
	}
	BOOL	IsSectionExist(LPCSTR lpSection)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->IsSectionExist(lpSection);
	}
	BOOL	GetNextSection(LPCSTR pSection, LPSTR pNextSection)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetNextSection(pSection, pNextSection);
	}
	void	EraseSection(LPCSTR lpSection)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return;
		}

		m_pNativeFile->EraseSection(lpSection);
	}

	BOOL	GetNextKey(LPCSTR pSection, LPCSTR pKey, LPSTR pNextKey)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetNextKey(pSection, pKey, pNextKey);
	}
	void	EraseKey(LPCSTR lpSection, LPCSTR lpKey)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return;
		}

		m_pNativeFile->EraseKey(lpSection, lpKey);
	}

	BOOL	GetString(LPCSTR lpSection, LPCSTR lpKeyName, LPCSTR lpDefault,	LPSTR lpRString, DWORD dwSize)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetString(lpSection, lpKeyName, lpDefault, lpRString, dwSize);
	}

	BOOL	GetInteger(LPCSTR lpSection, LPCSTR lpKeyName, int nDefault, int* pnValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetInteger(lpSection, lpKeyName, nDefault, pnValue);
	}
	BOOL	GetInteger2(LPCSTR lpSection, LPCSTR lpKeyName, int* pnValue1, int* pnValue2)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetInteger2(lpSection, lpKeyName, pnValue1, pnValue2);
	}
	int		GetMultiInteger(LPCSTR lpSection, LPCSTR lpKeyName, int* pValues, int nCount)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return 0;
		}

		return m_pNativeFile->GetMultiInteger(lpSection, lpKeyName, pValues, nCount);
	}
	int		GetMultiLong(LPCSTR lpSection, LPCSTR lpKeyName, long* pValues, int nCount)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return 0;
		}

		return m_pNativeFile->GetMultiLong(lpSection, lpKeyName, pValues, nCount);
	}

	BOOL	GetFloat(LPCSTR lpSection, LPCSTR lpKeyName, float fDefault, float *pfValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetFloat(lpSection, lpKeyName, fDefault, pfValue);
	}
	BOOL	GetFloat2(LPCSTR lpSection, LPCSTR lpKeyName, float *pfValue1, float *pfValue2)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetFloat2(lpSection, lpKeyName, pfValue1, pfValue2);
	}
	int		GetMultiFloat(LPCSTR lpSection, LPCSTR lpKeyName, float* pValues, int nCount)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return 0;
		}

		return m_pNativeFile->GetMultiFloat(lpSection, lpKeyName, pValues, nCount);
	}

	BOOL	GetStruct(LPCSTR lpSection, LPCSTR lpKeyName, LPVOID lpStruct, DWORD dwSize)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetStruct(lpSection, lpKeyName, lpStruct, dwSize);
	}

	BOOL	GetBool(LPCSTR lpSection, LPCSTR lpKeyName, BOOL* pBool)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->GetBool(lpSection, lpKeyName, pBool);
	}

	BOOL	WriteString(LPCSTR lpSection, LPCSTR lpKeyName, LPCSTR lpString)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteString(lpSection, lpKeyName, lpString);
	}

	BOOL	WriteInteger(LPCSTR lpSection, LPCSTR lpKeyName, int nValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteInteger(lpSection, lpKeyName, nValue);
	}
	BOOL	WriteInteger2(LPCSTR lpSection, LPCSTR lpKeyName, int nValue1, int nValue2)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteInteger2(lpSection, lpKeyName, nValue1, nValue2);
	}
	BOOL	WriteMultiInteger(LPCSTR lpSection, LPCSTR lpKeyName, int* pValues, int nCount)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteMultiInteger(lpSection, lpKeyName, pValues, nCount);
	}

	BOOL	WriteFloat(LPCSTR lpSection, LPCSTR lpKeyName, float fValue)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteFloat(lpSection, lpKeyName, fValue);
	}
	BOOL	WriteFloat2(LPCSTR lpSection, LPCSTR lpKeyName, float fValue1, float fValue2)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteFloat2(lpSection, lpKeyName, fValue1, fValue2);
	}
	BOOL	WriteMultiFloat(LPCSTR lpSection, LPCSTR lpKeyName, float* pValues, int nCount)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteMultiFloat(lpSection, lpKeyName, pValues, nCount);
	}

	BOOL	WriteStruct(LPCSTR lpSection, LPCSTR lpKeyName, LPVOID lpStruct, DWORD dwSize)
	{
		if (!m_pNativeFile)
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		return m_pNativeFile->WriteStruct(lpSection, lpKeyName, lpStruct, dwSize);
	}
};

