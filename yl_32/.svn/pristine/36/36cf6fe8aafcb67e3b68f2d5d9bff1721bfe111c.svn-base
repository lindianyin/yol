/* -------------------------------------------------------------------------
//	文件名		：	kbadwordfilter.h
//	创建者		：	simon
//	创建时间	：	2011/4/11 14:48:45
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KBADWORDFILTER_H__
#define __KBADWORDFILTER_H__

// -------------------------------------------------------------------------

#include <string>
typedef std::string	CString;
#include "Regexp.h"
#include "Core/QFile.h"
#include "onlinegamebase/ikbadwordfilter.h"


class KBadwordFilter : public IKBadwordFilter
{
public:
	INT Init()
	{
		ZeroStruct(m_aPlainTextBucket);
		return TRUE;
	}
	INT Uninit()
	{
		for (UINT i = 0; i < countof(m_aPlainTextBucket); ++i)
		{
			SAFE_DELETE(m_aPlainTextBucket[i]);
		}
		m_cRegexArray.clear();
		return TRUE;
	}
	INT LoadFilterFile(CONST CHAR* pFileName)
	{
		QFile cFile;
		if (!cFile.Open(pFileName))
			return FALSE;

		LPSTR pBuffer = (LPSTR)cFile.GetBuffer();
		QCONFIRM_RET_FALSE(pBuffer);
		size_t uSize = cFile.Size();
		LPSTR pLineHeader = pBuffer;

		for (size_t i = 0; i < uSize; ++i)
		{
			if (pBuffer[i] == '\r' || pBuffer[i] == '\n')
			{
				pBuffer[i] = '\0';

				// parse line
				_ParseLine(pLineHeader);

				pLineHeader = pBuffer + i + 1;
			}
		}
		if (pLineHeader < pBuffer + uSize)
			_ParseLine(pLineHeader);

		return TRUE;
	}
	INT AddPlainText(CONST CHAR* pString)
	{
		//assert(pString);
		//assert(*pString);
		// 用关键字的前两个字节做索引加快检测速度（1000个关键词时处理速度可超过300MB/s）
		WORD wHeader = *(WORD*)pString;

		KEXPR_ARRAY* pExpArray = m_aPlainTextBucket[wHeader];
		if ( ! pExpArray)
		{
			pExpArray = new KEXPR_ARRAY;
			m_aPlainTextBucket[wHeader] = pExpArray;
		}

		//将敏感词按从长到短顺序加入到词库
		KEXPR_ARRAY::iterator iWhere;
		for (iWhere = pExpArray->begin(); iWhere < pExpArray->end(); ++iWhere)
		{
			if (iWhere->size() < strlen(pString))
			{
				break;
			}
		}
		pExpArray->insert(iWhere, std::string(pString));

		return TRUE;
	}
	INT HasBadword(CONST CHAR* pszText)
	{
		// check plain
		//assert(pszText);
		//assert(*pszText);
		return FALSE;  //by ldy

		
		CONST CHAR* pszPos = pszText;
		while (*pszPos)
		{
			BYTE byHeader = *(BYTE*)pszPos;
			if (m_aPlainTextBucket[byHeader])
				return TRUE; // 单字节关键词

			WORD wHeader = *(WORD*)pszPos;
			KEXPR_ARRAY* pExpArray = m_aPlainTextBucket[wHeader];
			if (pExpArray)
			{
				for (KEXPR_ARRAY::iterator it = pExpArray->begin(); it != pExpArray->end(); ++it)
				{
					if (strncmp(pszPos, it->c_str(), it->size()) == 0)
						return TRUE;
				}
			}
			// 临时判断中文 [5/4/2011 simon]
			if ((BYTE)pszPos[0] > 0x80 && pszPos[1])
				++++pszPos;
			else
				++pszPos;
		}

		// check regexp
		for (KREGEX_ARRAY::iterator it = m_cRegexArray.begin(); it != m_cRegexArray.end(); ++it)
		{
			if (it->Match(pszText))
				return TRUE;
		}
		

		return FALSE;
	}
	INT KillBadword(CONST CHAR* pszText, CHAR* pszFilterText, INT nFilterTextLen)
	{
		// check plain
		//assert(pszText);
		//assert(*pszText);
		memset(pszFilterText, 0, nFilterTextLen);
		QStrCpyLen(pszFilterText, pszText, nFilterTextLen);

		CHAR* pszPos = pszFilterText;
		while (*pszPos)
		{
			BOOL bMoveNext = TRUE;
			BYTE byHeader = *(BYTE*)pszPos;
			if (m_aPlainTextBucket[byHeader])//单字节关键字
			{
				memset((void *)pszPos, '*', 1);
				++pszPos;
				continue;
			}

			WORD wHeader = *(WORD*)pszPos;
			KEXPR_ARRAY* pExpArray = m_aPlainTextBucket[wHeader];
			if (pExpArray)
			{
				for (KEXPR_ARRAY::iterator it = pExpArray->begin(); it != pExpArray->end(); ++it)
				{
					if (strncmp(pszPos, it->c_str(), it->size()) == 0)
					{
						memset((void*)pszPos, '*', it->size());
						pszPos += it->size();
						
						bMoveNext = FALSE;
						break;
					}
				}
			}

			// 临时判断中文 [5/4/2011 simon]
			if (bMoveNext)
			{
				if ((BYTE)pszPos[0] > 0x80 && pszPos[1])
					++++pszPos;
				else
					++pszPos;
			}
		}
		// check regexp
		for (KREGEX_ARRAY::iterator it = m_cRegexArray.begin(); it != m_cRegexArray.end(); ++it)
		{
			CHAR* pszTemp = pszFilterText;
			while (strlen(pszTemp))
			{
				if (it->Match(pszTemp))
				{
					memset((void *)(pszTemp + it->SubStart(0)), '*', it->SubLength(0));
					pszTemp += it->SubStart(0) + it->SubLength(0);
				}
				else
				{
					++pszTemp;
				}
			}
		}

		return !HasBadword(pszText);
	}
	INT AddRegularExpression(CONST CHAR* pString, BOOL bIgnoreCase = FALSE)
	{
		//assert(pString);
		Regexp cRegex(pString, bIgnoreCase);
		if ( ! cRegex.CompiledOK())
			return FALSE;

		m_cRegexArray.push_back(cRegex);
		return TRUE;
	}
	INT Dump()
	{
		INT nCount = 0;
		INT nNonEmptyBucket = 0;

		for (UINT i = 0; i < countof(m_aPlainTextBucket); ++i)
		{
			if (m_aPlainTextBucket[i])
			{
				nCount += m_aPlainTextBucket[i]->size();
				//printf("%1s +%d %s\n", &i, m_aPlainTextBucket[i]->size(), m_aPlainTextBucket[i]->at(0).c_str());
				//printf("%1s +%d   ", &i, m_aPlainTextBucket[i]->size());
				nNonEmptyBucket ++;
			}
		}
		printf("plain count: %d in %d buckets\n", nCount, nNonEmptyBucket);
		printf("regex count: %d\n", (int32_t)m_cRegexArray.size());
		return TRUE;
	}
	INT Release()
	{
		Uninit();
		delete (this);

		return TRUE;
	}
private:
	INT _ParseLine(CHAR* pLine)
	{
		QCONFIRM_RET_FALSE(pLine);

		// 过滤行首空格
		while (*pLine == ' ' || *pLine == '\t')
		{
			pLine++;
		}
		if (*pLine == '#' || *pLine == '\0')
			return FALSE;

		INT nResult = FALSE;
		CHAR chLeadingChar = *pLine;

		if (chLeadingChar == '@')
			nResult = AddRegularExpression(&pLine[1], TRUE);
		else if (chLeadingChar == '+')
			nResult = AddRegularExpression(&pLine[1]);
		else
			nResult = AddPlainText(pLine);
		if (! nResult)
			;//printf("addexp failed! %s", pLine);
		return nResult;
	}
private:

	typedef std::vector<std::string> KEXPR_ARRAY;
	KEXPR_ARRAY* m_aPlainTextBucket[USHRT_MAX + 1];

	typedef std::vector<Regexp> KREGEX_ARRAY;
	KREGEX_ARRAY m_cRegexArray;
};

extern KBadwordFilter g_cBadwordFilter;
// -------------------------------------------------------------------------

#endif /* __KBADWORDFILTER_H__ */
