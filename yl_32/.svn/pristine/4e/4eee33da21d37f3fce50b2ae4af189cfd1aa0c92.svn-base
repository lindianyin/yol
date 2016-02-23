
#include "CorePrivate.h"
#include "xmldocument.h"
#include "Core/QLuaScript.h"
#include "Core/QFile.h"

#ifdef unix
#include <iconv.h>
#include <sys/param.h>
#endif // unix


// -------------------------------------------------------------------------

static INT s_Utf16_Ansi(LPCWSTR pwszIn, UINT uInBytes, LPSTR pszOut, UINT uOutLen)
{
	INT nResult = 0;
	INT nOutLen		= -1;

	LOG_PROCESS_ERROR(pwszIn);
	LOG_PROCESS_ERROR(uInBytes > 0);

	LOG_PROCESS_ERROR(pszOut);
	LOG_PROCESS_ERROR(uOutLen > 0);

#ifdef unix
	{
#if defined(__FreeBSD__)
		LPCSTR pInBuf	= (LPCSTR)pwszIn;
#else
		LPSTR pInBuf	= (LPSTR)pwszIn;
#endif
		size_t nInSize	= uInBytes;
		LPSTR pOutBuf	= pszOut;
		size_t nOutSize	= uOutLen;

		// Linux x86下utf-16默认是utf-16le，FreeBSD x86下utf-16默认是utf-16be
		// utf-16与utf-16le、utf-16be的区别是后者不接受BOM标志，因为Endianness已经确定
		// http://unicode.org/faq/utf_bom.html#bom10
		//ldy
		//iconv_t cd = iconv_open("gbk", "utf-16le");
		iconv_t cd = iconv_open("utf-8", "utf-16le");
		LOG_PROCESS_ERROR_RET(cd != (iconv_t)-1, errno);

		INT nRet	= iconv(cd, &pInBuf, &nInSize, &pOutBuf, &nOutSize);
		iconv_close(cd);

		LOG_PROCESS_ERROR_RET(nRet != -1, errno);
		nOutLen = uOutLen - nOutSize;
	}
#else	// #ifdef unix
	{
		nOutLen = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pwszIn, uInBytes / 2, pszOut, uOutLen, NULL, NULL);
		LOG_PROCESS_ERROR(nOutLen > 0);
	}
#endif	// #ifdef unix

EXIT0:
	return nOutLen;
}

static BOOL s_PushElem2Script(CONST TiXmlElement* pcElement, QLuaScript& sc)
{
	BOOL bResult	= FALSE;
	INT nChildrenCount	= 0;
	LPCTSTR pszValue	= NULL;

	LOG_PROCESS_ERROR(pcElement);

	sc.PushTable();

	// .name
	pszValue	= pcElement->Value();
	LOG_PROCESS_ERROR(pszValue);
	LOG_PROCESS_ERROR(pszValue[0]);
	sc.PushString(pszValue);
	sc.SetTableField("name");

	// .value
	pszValue	= pcElement->GetText();
	if (pszValue)
		sc.PushString(pszValue);
	else
		sc.PushString("");
	sc.SetTableField("value");

	// .attrib
	sc.PushTable();
	for (CONST TiXmlAttribute* pcAttribute = pcElement->FirstAttribute();
		pcAttribute;
		pcAttribute = pcAttribute->Next())
	{
		pszValue	= pcAttribute->Value();
		LOG_PROCESS_ERROR(pszValue);
		sc.PushString(pszValue);

		pszValue	= pcAttribute->Name();
		LOG_PROCESS_ERROR(pszValue);
		LOG_PROCESS_ERROR(pszValue[0]);
		sc.SetTableField(pszValue);
	}
	sc.SetTableField("attrib");

	// .children
	sc.PushTable();
	for (CONST TiXmlElement* pcChildElem = pcElement->FirstChildElement();
		pcChildElem;
		pcChildElem = pcChildElem->NextSiblingElement())
	{
		LOG_PROCESS_ERROR(s_PushElem2Script(pcChildElem, sc));
		sc.SetTableIndex(++nChildrenCount);
	}
	sc.SetTableField("children");

	bResult	= TRUE;

EXIT0:

	return bResult;
}

BOOL KXMLDocument::Load(LPCTSTR pszXMLFileName)
{
	BOOL bResult		= FALSE;
	IFile* pIFile		= NULL;
	LPSTR pszBufFile	= NULL;
	LPSTR pszBufConv	= NULL;
	LPCTSTR pszBufRead	= NULL;
	INT nSize			= 0;
	
	LOG_PROCESS_ERROR(pszXMLFileName);
	LOG_PROCESS_ERROR(pszXMLFileName[0]);

	pIFile	= g_OpenFile(pszXMLFileName);
	PROCESS_ERROR(pIFile);

	nSize		= pIFile->Size();
	pszBufFile	= new CHAR[nSize + 1];
	pIFile->Read(pszBufFile, nSize);
	pszBufFile[nSize]	= '\0';

	if (nSize >= 2 && pszBufFile[0] == (CHAR)0xff && pszBufFile[1] == (CHAR)0xfe)	// unicode
	{
		pszBufConv	= new CHAR[nSize];
		nSize	= s_Utf16_Ansi((LPCWSTR)(pszBufFile + 2), nSize - 2, pszBufConv, nSize - 1);
		LOG_PROCESS_ERROR(nSize >= 0);
		pszBufConv[nSize]	= '\0';

		pszBufRead	= pszBufConv;
	}
	else
	{
		pszBufRead	= pszBufFile;
	}
	QLogPrintf(LOG_INFO,"%s",pszXMLFileName);
	LOG_PROCESS_ERROR(m_xmlDoc.LoadBuffer(pszBufRead, nSize, TIXML_ENCODING_LEGACY));

	bResult	= TRUE;

EXIT0:
	SAFE_DELETE_ARRAY(pszBufFile);
	SAFE_DELETE_ARRAY(pszBufConv);
	if (pIFile)
		pIFile->Release();

	return bResult;
}
BOOL KXMLDocument::LoadFromBuffer(LPCTSTR szBuffer, INT nLength)
{
	return m_xmlDoc.LoadBuffer(szBuffer, nLength);
}

BOOL KXMLDocument::Push2Script(QLuaScript& sc)
{
	BOOL bResult	= FALSE;
	INT nTopIndex	= sc.GetTopIndex();

	CONST TiXmlElement* pcElement	= m_xmlDoc.FirstChildElement();
	LOG_PROCESS_ERROR(pcElement);

	bResult	= s_PushElem2Script(pcElement, sc);

EXIT0:
	if (bResult)
	{
		_ASSERT(sc.GetTopIndex() == nTopIndex + 1);
	}
	else
	{
		sc.SetTopIndex(nTopIndex);
	}


	return bResult;
}
BOOL KXMLDocument::SaveToBuffer(LPSTR szBuffer, INT nLength)
{
	TIXML_OSTREAM outStream;
	outStream<<m_xmlDoc;
	QStrCpyLen(szBuffer, outStream.c_str(), nLength);
	return TRUE;
}

IXmlDocument* gCreateXmlDocument(INT nCodePage, LPCSTR pszCodePage)
{
	
	KXMLDocument* pDoc = new KXMLDocument;
	return pDoc;
}
