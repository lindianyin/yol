
#include "CorePrivate.h"
#include <set>
#include <fstream>
#include <algorithm>
#include "Misc/namefilter_i.h"
#include "Misc/objectpool.h"
#include "Core/QFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class QNameFilter : public INameFilter
{
public:
	typedef std::vector<WORD> VEC_WORD;

	// -------------------------------------------------------------------------
	// Function name   : InitFromFile
	// Description     : Read all legal-words from file
	// Return type     : BOOL 
	// Argument        : LPCSTR szFile
	// -------------------------------------------------------------------------
	virtual BOOL InitFromFile(LPCSTR szFile);

	// -------------------------------------------------------------------------
	// Function name   : IsNamePass
	// Description     : Check if name can pass
	// Return type     : BOOL 
	// Argument        : LPCSTR szName
	// -------------------------------------------------------------------------
	virtual BOOL IsNamePass(LPCSTR pszName);

	virtual void Release();
protected:
	VEC_WORD m_vecLegalWord;
};

static KDynReUseObjectPool<QNameFilter> gs_Pool;

BOOL QNameFilter::InitFromFile( LPCSTR szFile ) 
{
	QFile	FileChatFlt;

	if (FileChatFlt.Open(szFile) == FALSE)
		return FALSE;
	// clear
	m_vecLegalWord.resize(0);
	// use set, sort and avoid duplicate
	std::set<WORD> setTest;

	LPSTR pBuffer = NULL;
	INT nSize = FileChatFlt.Size();
	if (nSize)
		pBuffer = new CHAR[nSize + 1];
	if (pBuffer)
	{
		INT nFinalSize = FileChatFlt.Read(pBuffer, nSize);
		if (nFinalSize >= nSize)
			pBuffer[nSize] = 0;
		else
			memset(pBuffer + nFinalSize, 0, nSize - nFinalSize + 1);

		LPSTR pLineHeader = pBuffer;

		do
		{
			LPSTR pLineEnd = strchr(pLineHeader, 0x0a);
			INT nLineLen;
			if (pLineEnd)
			{
				*pLineEnd = 0;
				nLineLen = pLineEnd - pLineHeader;
			}
			else
			{
				nLineLen = nFinalSize - (pLineHeader - pBuffer);
			}

			if (pLineHeader[0] && nLineLen > 0)
			{
				if (pLineHeader[nLineLen - 1] == 0x0d)
					pLineHeader[--nLineLen] = 0;
				if (nLineLen)
				{
					WORD wLegal = 0;
					if (pLineHeader[1])
						wLegal = *(WORD* )pLineHeader;
					else
						wLegal = (WORD)(UCHAR)pLineHeader[0];
					setTest.insert(wLegal);
				}
			}

			pLineHeader = pLineEnd ? pLineEnd + 1: NULL;
		} while(pLineHeader);

		delete[] pBuffer;
	}
	m_vecLegalWord.reserve(setTest.size());
	m_vecLegalWord.assign(setTest.begin(), setTest.end());
	return TRUE;
}

BOOL QNameFilter::IsNamePass( LPCSTR pszName ) 
{
	while (*pszName)
	{
		if (*(pszName + 1))
		{
			// First check multi-byte, if multi-byte not pass, then check single-byte
			if (std::binary_search(m_vecLegalWord.begin(), m_vecLegalWord.end(), *(WORD* )pszName))
			{
				pszName += 2;
				continue;
			}
		}
		if (!std::binary_search(m_vecLegalWord.begin(), m_vecLegalWord.end(), *pszName))
			return FALSE;
		++pszName;
	}
	return TRUE;
}

void QNameFilter::Release() 
{
	gs_Pool.Free(this);
}

HRESULT CreateNameFilter(INameFilter** ppNameWordFilter)
{
	HRESULT rResult = S_FALSE;
	LOG_PROCESS_ERROR(ppNameWordFilter);
	*ppNameWordFilter = gs_Pool.New();
	LOG_PROCESS_ERROR(*ppNameWordFilter);
	rResult = S_OK;
EXIT0:
	return rResult;
}
