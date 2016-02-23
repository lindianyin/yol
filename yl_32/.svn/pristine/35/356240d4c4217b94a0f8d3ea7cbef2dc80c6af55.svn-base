
// TODO：内存管理还可改进（每次改变均要重新分配，且ref为1且长度不变时也重分配！），有些字符串指针参数没检查等。

#include "CorePrivate.h"
#include "Core/QMemory.h"
#include "Runtime/qstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace{

class EmptyBuffer : public IMemBlock
{
	CHAR m_Buf[1];
	INT m_nRef;
public:
	EmptyBuffer() { m_Buf[0] = 0; m_nRef = 1; }
	virtual void* GetData() { return m_Buf; }
	virtual unsigned GetSize() { return 1; }
	virtual unsigned GetReserveSize() { return 0; }
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) { return -1; }
	virtual ULONG STDMETHODCALLTYPE AddRef() { ++m_nRef; return 0; }
	virtual ULONG STDMETHODCALLTYPE Release() { --m_nRef; return 0; }
};

EmptyBuffer _gEmptyBuffer;
}

IMemBlock* QString::s_piEmptyBuffer = &_gEmptyBuffer;
// -------------------------------------------------------------------------
QString::QString(LPCSTR ptr)
{
	UINT uLen  = strlen(ptr) + 1;
	m_piBuffer = QCreateMemBlock(uLen);
	memcpy(_QS_BUFFER, ptr, uLen);
}

QString::QString(LPCSTR ptr, INT nLen)
{
	m_piBuffer = QCreateMemBlock(nLen + 1);
	memcpy(_QS_BUFFER, ptr, nLen + 1);
}

QString::QString(const QString& str)
{
	m_piBuffer = str.m_piBuffer;
	m_piBuffer->AddRef();
}

QString::QString(CHAR ch, INT nRepeat)
{
	m_piBuffer = QCreateMemBlock(nRepeat + 1);
	memset(_QS_BUFFER, ch, nRepeat);
	_QS_BUFFER[nRepeat] = '\0';	
}
///////////////////////////////////////////////////////
QString& QString::operator = (LPCSTR ptr)
{
	UINT nLen = strlen(ptr);
	m_piBuffer->Release();
	m_piBuffer = QCreateMemBlock(nLen + 1);
	memcpy(_QS_BUFFER, ptr, nLen + 1);
	return *this;
}

QString& QString::operator = (const QString& str)
{
	if (this != &str)
	{
		m_piBuffer->Release();
		m_piBuffer = str.m_piBuffer;
		m_piBuffer->AddRef();
	}
	return *this;
}

QString& QString::operator += (CHAR c)
{
	// 缓存原Buffer
	IMemBlock* piBuffer = m_piBuffer;
	// 重分配空间
	m_piBuffer = QCreateMemBlock(piBuffer->GetSize() + 1);
	// 复制原字符串
	memcpy(_QS_BUFFER, piBuffer->GetData(), piBuffer->GetSize());
	// 加上c
	_QS_BUFFER[piBuffer->GetSize() - 1] = c;
	_QS_BUFFER[piBuffer->GetSize()] = '\0';
	// 释放原buffer
	piBuffer->Release();
	return *this;
}

QString& QString::operator += (LPCSTR ptr)
{
	UINT uStrLen = strlen(ptr);
	// 缓存原Buffer
	IMemBlock* piBuffer = m_piBuffer;
	// 重分配空间
	m_piBuffer = QCreateMemBlock(piBuffer->GetSize() + uStrLen);
	// 复制原字符串
	memcpy(_QS_BUFFER, piBuffer->GetData(), piBuffer->GetSize());
	// 加上str
	memcpy(_QS_BUFFER + piBuffer->GetSize() - 1, ptr, uStrLen + 1);	
	// 释放原buffer
	piBuffer->Release();
	return *this;
}

QString& QString::operator += (const QString& str)
{
	// 缓存原Buffer
	IMemBlock* piBuffer = m_piBuffer;
	// 重分配空间
	m_piBuffer = QCreateMemBlock(piBuffer->GetSize() + str.GetLength());
	// 复制原字符串
	memcpy(_QS_BUFFER, piBuffer->GetData(), piBuffer->GetSize());
	// 加上str
	memcpy(_QS_BUFFER + str.GetLength() - 1, str.operator LPCSTR(), str.GetLength() + 1);
	// 释放原buffer
	piBuffer->Release();
	return *this;
}

QString operator + (const QString& str1, const QString& str2)
{
	// 创建buffer
	IMemBlock* piBuffer = QCreateMemBlock(str1.GetLength() + str2.GetLength() + 1);
	// 复制数据
	memcpy(piBuffer->GetData(), str1.operator LPCSTR(), str1.GetLength());
	memcpy((LPSTR)piBuffer->GetData() + str1.GetLength(), str2.operator LPCSTR(), str2.GetLength() + 1);
	return QString(piBuffer);
}

QString operator + (const QString& str1, LPCSTR str2)
{
	INT nLen2 = strlen(str2);
	// 创建buffer
	IMemBlock* piBuffer = QCreateMemBlock(str1.GetLength() + nLen2 + 1);
	// 复制数据
	memcpy(piBuffer->GetData(), str1.operator LPCSTR(), str1.GetLength());
	memcpy((LPSTR)piBuffer->GetData() + str1.GetLength(), str2, nLen2 + 1);
	return QString(piBuffer);
}

QString operator + (LPCSTR str1, const QString& str2)
{
	INT nLen1 = strlen(str1);
	// 创建buffer
	IMemBlock* piBuffer = QCreateMemBlock(str2.GetLength() + nLen1 + 1);
	// 复制数据
	memcpy(piBuffer->GetData(), str1, nLen1);
	memcpy((LPSTR)piBuffer->GetData() + nLen1, str2.operator LPCSTR(), str2.GetLength() + 1);
	return QString(piBuffer);
}

QString operator + (const QString& str, CHAR c)
{
	// 创建buffer
	IMemBlock* piBuffer = QCreateMemBlock(str.GetLength() + 2);
	// 复制数据
	memcpy(piBuffer->GetData(), str.operator LPCSTR(), str.GetLength());
	*(((LPSTR)piBuffer->GetData()) + str.GetLength()) = c;
	*(((LPSTR)piBuffer->GetData()) + str.GetLength() + 1) = '\0';
	return QString(piBuffer);
}

QString operator + (CHAR c, const QString& str)
{
	// 创建buffer
	IMemBlock* piBuffer = QCreateMemBlock(str.GetLength() + 2);
	// 复制数据
	*((LPSTR)piBuffer->GetData()) = c;
	memcpy((LPSTR)piBuffer->GetData() + 1, str.operator LPCSTR(), str.GetLength() + 1);
	return QString(piBuffer);
}

INT QString::Compare(LPCSTR ptr) const
{
	return strcmp(_QS_BUFFER, ptr);
}

INT QString::CompareNoCase(LPCSTR ptr) const
{
	for (INT i = 0; i < _QS_LEN + 1; i++)
	{
		INT b = *(_QS_BUFFER + i) - *(ptr + i);
		switch(b)
		{
		case 0:
			continue;
			break;
		case 'a'-'A':
			if (*(_QS_BUFFER + i) >= 'a' && *(_QS_BUFFER + i) <= 'z')
				continue;
			else
				return 1;
			break;
		case 'A'-'a':
			if (*(_QS_BUFFER + i) >= 'A' && *(_QS_BUFFER + i) <= 'Z')
				continue;
			else
				return -1;
		default:
			return b > 0 ? 1 : -1;
			break;
		}	
	}
	return 0;
}

BOOL QString::LeftEqual(LPCSTR str, INT num) const
{
	INT i;
	for (i = 0; i < num; i++)
	{
		if (_QS_BUFFER[i] != str[i])
			return FALSE;
	}
	return TRUE;
}

VOID QString::MakeUpper()
{
	INT i;
	for (i = 0; i < _QS_LEN; i++)
	{
		if (_QS_BUFFER[i] >= 'a' && _QS_BUFFER[i] <= 'z')
			break;
	}

	if (i == _QS_LEN)
		return;	// No Write No Copy

	CopyOnWrite();
	_QS_BUFFER[i++] -= 'a' - 'A';
	for (; i < _QS_LEN; i++)
	{
		if (_QS_BUFFER[i] >= 'a' && _QS_BUFFER[i] <= 'z')
			_QS_BUFFER[i] -= 'a' - 'A';
	}
}

VOID QString::MakeLower()
{
	INT i;
	for (i = 0; i < _QS_LEN; i++)
	{
		if (_QS_BUFFER[i] >= 'A' && _QS_BUFFER[i] <= 'Z')
			break;
	}

	if (i == _QS_LEN)
		return;	// No Write No Copy

	CopyOnWrite();
	_QS_BUFFER[i] += 'a' - 'A';
	for (; i < _QS_LEN; i++)
	{
		if (_QS_BUFFER[i] >= 'A' && _QS_BUFFER[i] <= 'Z')
			_QS_BUFFER[i] += 'a' - 'A';
	}
}

static unsigned _format_len(LPCSTR format,va_list argList, BOOL *bUF)
{
	unsigned len = 0;
	*bUF = FALSE;
	for (LPCSTR lpsz = format; *lpsz != '\0'; ++lpsz) {
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(++lpsz) == '%') {
			++len;
			continue;
		}

		INT itenLen = 0;
		INT width = 0;
		for (; *lpsz != '\0'; ++lpsz)
		{
			if (*lpsz == '#')
				len += 2;   // for '0x'
			else if (*lpsz == '*')
				width = va_arg(argList, INT);
			else if (*lpsz != '-' && *lpsz != '+' && *lpsz != '0' && *lpsz != ' ') break;
		}

		if (width == 0)
		{
			width = atoi(lpsz);
			for (; *lpsz != '\0' && (*lpsz>='0' && *lpsz<='9'); ++lpsz)	;
		}

		INT width2 = 0;
		if (*lpsz == '.')
		{
			++lpsz;

			if (*lpsz == '*')
			{
				width2 = va_arg(argList, INT);
				++lpsz;
			}
			else
			{
				width2 = atoi(lpsz);
				for (; *lpsz != '\0' && (*lpsz>='0' && *lpsz<='9'); ++lpsz)	;
			}
		}

		// 去掉长度修饰
		switch (*lpsz) {
		case 'h':
		case 'l':
		case 'F':
		case 'N':
		case 'L':
			++lpsz;
			break;
		}

		switch (*lpsz) {
		case 'c':
		case 'C':
			itenLen=2;
			// 在32位下char入栈提升为32位，在64位下入栈或存入寄存器升格为64位 [3/11/2011 simon]
			va_arg(argList, INT);
			break;

		case 's':
		case 'S':
			{
				LPCSTR  nextstr = va_arg(argList, LPCSTR );
				if (nextstr == 0) itenLen = 6;  // "(null)"
				else itenLen = (nextstr[0]=='\0')?1:strlen(nextstr);
			}
			break;
		}

		if (itenLen != 0) {
			if (width2 != 0) {
				if (width2<itenLen) itenLen=width2;
			}
			if (width>itenLen) itenLen=width;
		}
		else {
			switch (*lpsz)
			{
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				va_arg(argList, INT);
				itenLen = (32>width+width2)?32:width+width2;
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				itenLen = (128>width+width2)?128:width+width2;
				break;
			case 'f':
				*bUF = TRUE;
				va_arg(argList, double);
				itenLen = 128; // width isn't truncated
				// 312 == strlen("-1+(309 zeroes).")
				// 309 zeroes == max precision of a double
				itenLen = MAX(itenLen, 312+width2);
				break;
			case 'p':
				va_arg(argList, VOID*);
				itenLen = (32>width+width2)?32:width+width2;
				break;

				// no output
			case 'n':
				va_arg(argList, INT*);
				break;

			}
		}

		len+=itenLen;
	}
	return len;
}

VOID QString::Format(LPCSTR lpszFormat, ...)
{
	va_list va;
	UINT uMaxLen;
	BOOL bUF;
	va_start(va, lpszFormat);
	uMaxLen = _format_len(lpszFormat, va, &bUF);
	IMemBlock* piBuffer = QCreateMemBlock(uMaxLen);
	if (piBuffer)
	{
		UINT uLen = 0;
#ifndef WIN32
		uLen = vsprintf((LPSTR)piBuffer->GetData(), lpszFormat, va);
#else
		if (bUF)
			uLen = vsprintf((LPSTR)piBuffer->GetData(), lpszFormat, va);
		else
			uLen = wvsprintf((LPSTR)piBuffer->GetData(), lpszFormat, va); // #include <winuser.h>
#endif
		m_piBuffer->Release();
		m_piBuffer = QCreateMemBlock(uLen);
		memcpy(_QS_BUFFER, piBuffer->GetData(), uLen + 1);
		piBuffer->Release();
	}
	va_end(va);
}

VOID QString::TrimRight()
{
	INT nLen = _QS_LEN;
	while (nLen > 0)
	{
		if (!isspace(_QS_BUFFER[nLen - 1]))
			break;
		else
			nLen--;
	}
	IMemBlock* piBuffer = m_piBuffer;
	if (nLen <= 0)
	{
		m_piBuffer = s_piEmptyBuffer;
	}
	else
	{
		m_piBuffer = QCreateMemBlock(nLen + 1);
		memcpy(_QS_BUFFER, piBuffer->GetData(), nLen);
		_QS_BUFFER[nLen] = '\0';
	}
	piBuffer->Release();
}

VOID QString::TrimLeft()
{
	INT i;
	for (i = 0; _QS_LEN > i; i++)
	{
		if (!isspace(_QS_BUFFER[i]))
			break;
	}
	INT nLen = _QS_LEN - i;
	IMemBlock* piBuffer = m_piBuffer;
	if (nLen <= 0)
	{
		m_piBuffer = s_piEmptyBuffer;
	}
	else
	{
		m_piBuffer = QCreateMemBlock(nLen + 1);
		memcpy(_QS_BUFFER, (LPSTR)piBuffer->GetData() + i, nLen + 1);
	}
	piBuffer->Release();
}

INT	QString::Find(CHAR c, INT nStart) const
{
	if (nStart >= _QS_LEN)
		return -1;
	for (INT i = nStart; i < _QS_LEN; i++)
	{
		if (_QS_BUFFER[i] == c)
			return i;
	}
	return -1;
}

INT	QString::ReverseFind(CHAR c, INT nStart) const
{
	INT i;
	if (nStart < 0)
	{
		i = _QS_LEN - 1;
	}
	else 
	{
		i = nStart > _QS_LEN ? _QS_LEN : nStart;
	}
	for (; i >= 0; i--)
	{
		if (_QS_BUFFER[i] == c)
			return i;
	}
	return -1;
}

INT	QString::Find(LPCSTR  ptr, INT nStart) const
{
	if (nStart < 0)
		nStart = 0;
	else if(nStart > _QS_LEN)
		return -1;
	LPCSTR szFound = strstr(_QS_BUFFER + nStart, ptr);
	if (!szFound)
		return -1;
	return szFound - _QS_BUFFER;
}

INT	QString::ReverseFind(LPCSTR  ptr, INT nStart) const
{
	INT len = strlen(ptr);
	if (len > _QS_LEN)
		return -1;	
	if (nStart < 0)
		nStart = _QS_LEN - len;
	else 
	{
		if(nStart > _QS_LEN - len)
			nStart = _QS_LEN - len;
	}
	LPCSTR tpr = _QS_BUFFER + nStart;
ULoop:
	INT j = 0;
	for (j = 0; j <= len - 4; j += 4)
	{
		if (*(DWORD *)(ptr + j) != *(DWORD *)(tpr + j)) 
		{
			if (--tpr < _QS_BUFFER)
				return -1;
			goto ULoop;
		};
	}
	for (; j < len; j++)
	{
		if (ptr[j] != tpr[j]) 
		{
			if (--tpr < _QS_BUFFER)
				return -1;
			goto ULoop;
		}
	}
	return tpr - _QS_BUFFER;
}

VOID QString::CopyOnWrite()
{
	// 没这函数。。 if (m_piBuffer->GetRef() > 1)
	{
		IMemBlock* piBuffer = m_piBuffer;
		m_piBuffer = QCreateMemBlock(piBuffer->GetSize());
		memcpy(m_piBuffer->GetData(), piBuffer->GetData(), piBuffer->GetSize());
		piBuffer ->Release();
	}
}

// -------------------------------------------------------------------------
