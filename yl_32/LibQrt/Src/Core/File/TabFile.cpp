
#include "CorePrivate.h"
#include "File.h"
#include "TabFile.h"
#include "TabFileWritable.h"
#include "QStrBase.h"
#include "QLog.h"


ITabFile*
g_OpenTabFile(const char* FileName, int ForceUnpakFile/* = false*/, int ForWrite/* = false*/)
{
	ITabFile*	pTab = NULL;
	IFile*		pFile = g_OpenFile(FileName, ForceUnpakFile, ForWrite);
	int			nResult = false;
	QLogPrintf(LOG_INFO, "..g_OpenTabFile=%s", FileName);
	if (pFile)
	{
		if (!ForWrite)
		{
			if ((pTab = QTabFileImpl::New()) != NULL)
				nResult = ((QTabFileImpl*)pTab)->LoadData(pFile);
		}
		else if ((pTab = QTabFileWritable::New()) != NULL)
		{
			nResult = ((QTabFileWritable*)pTab)->LoadData(pFile);
		}
	}
	if (!nResult)
		SAFE_RELEASE(pTab);
	SAFE_RELEASE(pFile);
	return pTab;
}

bool g_OpenFile(ITabFile** ppTabFile, const char* FileName, int ForceUnpakFile /* = false */, int ForWrite /* = false */)
{
	
	*ppTabFile = g_OpenTabFile(FileName, ForceUnpakFile, ForWrite);
	return (*ppTabFile != NULL);
}

ITabFile* g_CreateTabFile()
{
	return QTabFileWritable::New();
}

#define _IS_NEWLINE(c) ((c) == 0x0d || (c) == 0x0a)

static inline BOOL s_GoNextLine(LPBYTE& pbyBuf, INT& nOffset, INT nSize)
{
	BOOL bResult	= FALSE;
	PROCESS_ERROR(nOffset < nSize);

	// 先找到换行
	while (!_IS_NEWLINE(*pbyBuf))
	{
		pbyBuf ++;
		nOffset ++;
		LOG_PROCESS_ERROR(nOffset < nSize);	// 请在文件尾部添加换行
	}

	// 跳过换行符
	pbyBuf ++;
	nOffset ++;

	// 双字节换行符
	if (nOffset < nSize && pbyBuf[-1] == 0x0d && pbyBuf[0] == 0x0a)
	{
		pbyBuf ++;
		nOffset ++;
	}

	bResult	= TRUE;

EXIT0:
	return bResult;
}

//---------------------------------------------------------------------------
// 函数:	KTabFileImpl
// 功能:	购造函数
//---------------------------------------------------------------------------
QTabFileImpl::QTabFileImpl()
{
	m_Width		= 0;
	m_Height	= 0;
	m_pMemory	= NULL;
	m_uMemorySize = 0;
	m_pOffsetTable = NULL;
    m_bErrorLogEnable = true;
}

//---------------------------------------------------------------------------
// 函数:	~KTabFileImpl
// 功能:	析造函数
//---------------------------------------------------------------------------
QTabFileImpl::~QTabFileImpl()
{
	Clear();
}

//构造一个空的KTabFileImpl对象
QTabFileImpl* QTabFileImpl::New()
{
    QTabFileImpl* pTabFile = new QTabFileImpl();
    if (pTabFile)
    {
        pTabFile->SetErrorLog(true);
    }
	return pTabFile;
}

// 功能:	加载一个Tab文件
int	QTabFileImpl::LoadData(IFile* pFile)
{
	assert(pFile);
	Clear();

	int				nResult = false;
	unsigned int	dwSize = pFile->Size();
	if (dwSize == 0)
		return true;

	m_pMemory = (unsigned char *)malloc(dwSize);
	if (m_pMemory)
	{
		m_uMemorySize = dwSize;
		if (pFile->Read(m_pMemory, dwSize) == dwSize)
		{
			if (CreateTabOffset())
				nResult = true;
		}
	}
	if (!nResult)
		Clear();
	return nResult;
}

int	QTabFileImpl::GetWidth()
{
	return m_Width;
}

int QTabFileImpl::GetHeight()
{
	return m_Height;
}

//---------------------------------------------------------------------------
// 函数:	CreateTabOffset
// 功能:	建立制作表符分隔文件的偏移表
//---------------------------------------------------------------------------
int QTabFileImpl::CreateTabOffset()
{
	int		nWidth, nHeight, nOffset, nSize;
	unsigned char	*Buffer;

	nWidth	= 1;
	nHeight	= 0;
	nOffset = 0;

	Buffer	= m_pMemory;
	nSize	= m_uMemorySize;
	
	if (!Buffer || !nSize)
		return true;

	// 读第一行决定有多少列
	while (nOffset < nSize && !_IS_NEWLINE(*Buffer))
	{
		if (*Buffer == 0x09)
			nWidth++;

		Buffer++;
		nOffset++;
	}

	while(s_GoNextLine(Buffer, nOffset, nSize))
		nHeight++;

	m_Width		= nWidth;
	m_Height	= nHeight;

	m_pOffsetTable = (TABOFFSET*)malloc(m_Width * m_Height * sizeof(TABOFFSET));
	if (m_pOffsetTable == NULL)
		return false;

	memset(m_pOffsetTable, 0, m_Width * m_Height * sizeof(TABOFFSET));
	TABOFFSET* TabBuffer = m_pOffsetTable;
	Buffer = m_pMemory;

	nOffset = 0;
	int nLength;
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			TabBuffer->dwOffset = nOffset;
			nLength = 0;
			while(nOffset < nSize && *Buffer != 0x09 && !_IS_NEWLINE(*Buffer))
			{
				Buffer++;
				nOffset++;
				nLength++;
			}
			TabBuffer->dwLength = nLength;

			if (nOffset >= nSize)
				break;

			if (_IS_NEWLINE(*Buffer))	//	本行已经结束了，虽然可能没到nWidth
			{
				TabBuffer	+= nWidth - j;
				break;
			}

			Buffer++;	// 0x09跳过
			nOffset++;

			TabBuffer++;
		}
		if (nOffset >= nSize)
			break;

		s_GoNextLine(Buffer, nOffset, nSize);
	}
	return true;
}

//---------------------------------------------------------------------------
// 函数:	Str2Column
// 功能:	取得某行某列字符串的值
// 参数:	szColumn
// 返回:	第几列
//---------------------------------------------------------------------------
int QTabFileImpl::Str2Col(const char* szColumn)
{
	char	szTemp[4];
	strncpy(szTemp, szColumn, 3);
	szTemp[2] = 0;
	QStrLower(szTemp);

	int		nIndex;
	if (szTemp[0])
	{
		if (szTemp[1] == 0)
			nIndex = (szTemp[0] - 'a');
		else
			nIndex = ((szTemp[0] - 'a' + 1) * 26 + szTemp[1] - 'a') + 1;
	}
	else
	{
		nIndex = -1;
	}
	return nIndex;
}

//---------------------------------------------------------------------------
// 函数:	GetString
// 功能:	取得某行某列字符串的值
// 参数:	nRow			行
//			nColomn			列
//			lpDefault		缺省值
//			lpRString		返回值
//			dwSize			返回字符串的最大长度
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetString(int nRow, const char* szColumn, const char* lpDefault,
						char* lpRString, unsigned int dwSize, int bColumnLab)
{
	int nColumn;
	int	nRet = 0;

	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);

	nRet = GetValue(nRow - 1, nColumn - 1, lpRString, dwSize);
	if (1 != nRet)
		QStrCpyLen(lpRString, lpDefault, dwSize);

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetString(%d, %s) failed !\n", nRow, szColumn);
    }
	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetString
// 功能:	取得某行某列字符串的值
// 参数:	szRow			行	（关键字）
//			szColomn		列	（关键字）
//			lpDefault		缺省值
//			lpRString		返回值
//			dwSize			返回字符串的最大长度
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetString(const char* szRow, const char* szColumn,
					const char* lpDefault, char* lpRString, unsigned int dwSize)
{
	int nRow, nColumn;
	int nRet = 0;

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	nRet = GetValue(nRow - 1, nColumn - 1, lpRString, dwSize);
	if (1 != nRet)
		QStrCpyLen(lpRString, lpDefault, dwSize);

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetString(%s, %s) failed !\n", szRow, szColumn);
    }
	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetString
// 功能:	取得某行某列字符串的值
// 参数:	nRow			行		从1开始
//			nColomn			列		从1开始
//			lpDefault		缺省值
//			lpRString		返回值
//			dwSize			返回字符串的最大长度
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetString(int nRow, int nColumn, const char* lpDefault,
						char* lpRString, unsigned int dwSize)
{
	int nRet = 0;

	nRet = GetValue(nRow - 1, nColumn - 1,  lpRString, dwSize);

	if (1 != nRet)
		QStrCpyLen(lpRString, lpDefault, dwSize);

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetString(%d, %d) failed !\n", nRow, nColumn);
		
    }

	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetInteger
// 功能:	取得某行某列字符串的值
// 参数:	nRow			行
//			szColomn		列
//			nDefault		缺省值
//			pnValue			返回值
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetInteger(int nRow, const char* szColumn, int nDefault,
						int *pnValue, int bColumnLab)
{
	char	Buffer[32];
	int		nColumn;
	int		nRet = 0;

	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	nRet = GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer));

	if (1 == nRet)
	{
		*pnValue = atoi(Buffer);
	}
	else
	{
		*pnValue = nDefault;
	}

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetInteger(%d, %s) failed !\n", nRow, szColumn);
		//assert(FALSE);
		
    }

	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetInteger
// 功能:	取得某行某列字符串的值
// 参数:	szRow			行
//			szColomn		列
//			nDefault		缺省值
//			pnValue			返回值
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetInteger(const char* szRow, const char* szColumn,
						int nDefault, int *pnValue)
{
	int		nRow, nColumn;
	char	Buffer[32];
	int		nRet = 0;

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);

	nRet = GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer));
	if (1 == nRet)
	{
		*pnValue = atoi(Buffer);
	}
	else
	{
		*pnValue = nDefault;
	}

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetInteger(%s, %s) failed !\n", szRow, szColumn);
		//assert(FALSE);
    }

	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetInteger
// 功能:	取得某行某列字符串的值
// 参数:	nRow			行		从1开始
//			nColomn			列		从1开始
//			nDefault		缺省值
//			pnValue			返回值
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int QTabFileImpl::GetInteger(int nRow, int nColumn, int nDefault, int *pnValue)
{
	char	Buffer[32];
	int		nRet = 0;

	nRet = GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer));
	if (1 == nRet)
	{
		*pnValue = atoi(Buffer);
	}
	else
	{
		*pnValue = nDefault;
	}

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetInteger(%d, %d) failed !\n", nRow, nColumn);
		//assert(FALSE);
    }

	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetFloat
// 功能:	取得某行某列字符串的值
// 参数:	nRow			行
//			szColomn		列
//			nDefault		缺省值
//			pnValue			返回值
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetFloat(int nRow, const char* szColumn, float fDefault,
						float *pfValue, int bColumnLab)
{
	char	Buffer[32];
	int		nColumn;
	int		nRet = 0;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	nRet = GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer));
	if (1 == nRet)
	{
		*pfValue = (float)atof(Buffer);
	}
	else
	{
		*pfValue = fDefault;
	}

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetFloat(%d, %s) failed !\n", nRow, szColumn);
    }

	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetFloat
// 功能:	取得某行某列字符串的值
// 参数:	szRow			行
//			szColomn		列
//			nDefault		缺省值
//			pnValue			返回值
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetFloat(const char* szRow, const char* szColumn,
						float fDefault, float *pfValue)
{
	int		nRow, nColumn;
	char	Buffer[32];
	int		nRet = 0;

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	nRet = GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer));
	if (1 == nRet)
	{
		*pfValue = (float)atof(Buffer);
	}
	else
	{
		*pfValue = fDefault;
	}

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetFloat(%s, %s) failed !\n", szRow, szColumn);
    }

	return nRet;
}

//---------------------------------------------------------------------------
// 函数:	GetFloat
// 功能:	取得某行某列字符串的值
// 参数:	nRow			行		从1开始
//			nColomn			列		从1开始
//			nDefault		缺省值
//			pnValue			返回值
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int QTabFileImpl::GetFloat(int nRow, int nColumn, float fDefault, float *pfValue)
{
	char	Buffer[32];
	int nRet = 0;
	
	nRet = GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer));
	if (1 == nRet)
	{
		*pfValue = (float)atof(Buffer);
	}
	else
	{
		*pfValue = fDefault;
	}

    if (nRet == 0 && m_bErrorLogEnable)
    {
        QLogPrintf(LOG_ERR, "GetFloat(%d, %d) failed !\n", nRow, nColumn);
    }

	return nRet;
}
//---------------------------------------------------------------------------
// 函数:	GetValue
// 功能:	取得某行某列字符串的值
// 参数:	nRow			行
//			nColomn			列
//			lpRString		返回值
//			dwSize			返回字符串的最大长度
// 返回:	1：成功	0：表格不对	-1：未填，使用默认值
//---------------------------------------------------------------------------
int	QTabFileImpl::GetValue(int nRow, int nColumn, char* lpRString, unsigned int dwSize)
{
	if (nRow >= m_Height || nColumn >= m_Width || nRow < 0 || nColumn < 0)
		return 0;

	CONST TABOFFSET& rsTempOffset	= m_pOffsetTable[nRow * m_Width + nColumn];
	LPCSTR pchBuffer				= (LPCSTR)&m_pMemory[rsTempOffset.dwOffset];

	if (rsTempOffset.dwLength == 0)
	{
		lpRString[0] = 0;
		return -1;
	}

	if (dwSize > rsTempOffset.dwLength)
	{
		memcpy(lpRString, pchBuffer, rsTempOffset.dwLength);
		lpRString[rsTempOffset.dwLength] = 0;
	}
	else
	{
		memcpy(lpRString, pchBuffer, dwSize);
		lpRString[dwSize - 1] = 0;
	}

	return 1;
}
//---------------------------------------------------------------------------
// 函数:	Clear
// 功能:	清除TAB文件的内容
// 参数:	void
// 返回:	void
//---------------------------------------------------------------------------
void QTabFileImpl::Clear()
{
	m_Width = 0;
	m_Height = 0;
	m_uMemorySize = 0;
	SAFE_FREE(m_pMemory);
	SAFE_FREE(m_pOffsetTable);
}

//---------------------------------------------------------------------------
// 函数:	FindRow
// 功能:	查找行关键字
// 参数:	szRow（行关键字）
// 返回:	int
//---------------------------------------------------------------------------
int QTabFileImpl::FindRow(const char* szRow)
{
	CONST TABOFFSET* psTempOffset	= m_pOffsetTable;
	INT nStrLen	= QStrLen(szRow);

	for (int i = 0; i < m_Height; i++, psTempOffset+=m_Width)
	{
		if (nStrLen != psTempOffset->dwLength)
			continue;

		// strncmp比memcmp快，原因未知
		if (strncmp((LPCSTR)m_pMemory + psTempOffset->dwOffset, szRow, nStrLen))
			continue;


		return i + 1;//改动此处为加一 by Romandou,即返回以1为起点的标号
	}
	return -1;
}

//---------------------------------------------------------------------------
// 函数:	FindColumn
// 功能:	查找列关键字
// 参数:	szColumn（行关键字）
// 返回:	int
//---------------------------------------------------------------------------
int QTabFileImpl::FindColumn(const char* szColumn)
{
	CONST TABOFFSET* psTempOffset	= m_pOffsetTable;
	INT nStrLen	= strlen(szColumn);

	for (int i = 0; i < m_Width; i++, psTempOffset++)
	{
		if (nStrLen != psTempOffset->dwLength)
			continue;

		// strncmp比memcmp快，原因未知
		if (strncmp((LPCSTR)m_pMemory + psTempOffset->dwOffset, szColumn, nStrLen))
			continue;

		return i + 1;//改动此处为加一 by Romandou,即返回以1为起点的标号
	}
	return -1;
}

//---------------------------------------------------------------------------
// 函数:	Col2Str
// 功能:	把整数转成字符串
// 参数:	szColumn
// 返回:	第几列
//---------------------------------------------------------------------------
void QTabFileImpl::Col2Str(int nCol, char* szColumn)
{
	if (nCol < 26)
	{
		szColumn[0] = 'A' + nCol;
		szColumn[1]	= 0;
	}
	else
	{
		szColumn[0] = 'A' + (nCol / 26 - 1);
		szColumn[1] = 'A' + nCol % 26;
		szColumn[2] = 0;
	}
}

void QTabFileImpl::Release()
{
	Clear();
	delete this;
}

void QTabFileImpl::SetErrorLog(BOOL bEnable)
{
    m_bErrorLogEnable = bEnable;
}
