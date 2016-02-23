
#pragma once


// -------------------------------------------------------------------------
class QString
{
#define _QS_LEN	((INT)m_piBuffer->GetSize() - 1)
#define _QS_SIZE (m_piBuffer->GetSize())
#define _QS_BUFFER ((LPSTR)m_piBuffer->GetData())
public:
	~QString() { SAFE_RELEASE(m_piBuffer); }
	QString() : m_piBuffer(s_piEmptyBuffer) { m_piBuffer->AddRef(); }
	QString(const QString &str);
	QString(LPCSTR ptr);
	QString(LPCSTR ptr, INT nLen); // 截取ptr到ptr + nLen一段
	QString(CHAR ch, INT nRepeat  = 1);
	///////////////
	UINT GetLength() const;
	// 暂保留GetAt，其实作为纯正字符串应用GetAt和SetAt都不该有。该函数不检查越界！
	CHAR GetAt(INT index) const;
	// VOID SetAt(INT index, CHAR ch);
	// 是否空串""
	BOOL IsEmpty() const;
	VOID Clear() { SAFE_RELEASE(m_piBuffer); m_piBuffer = s_piEmptyBuffer; }
	INT Compare(LPCSTR str) const;
	// 忽略大小写比较
	INT CompareNoCase(LPCSTR str) const;
	// 比较左边n个字符是否相等
	BOOL LeftEqual(LPCSTR str, INT num) const;
	VOID MakeUpper();
	VOID MakeLower();
	QString Mid(INT nFirst) const;
	QString Mid(INT nFirst, INT nCount) const;
	QString Left(INT nCount) const;
	QString Right(INT nCount) const;
	VOID	Format(LPCSTR lpszFormat, ...);
	VOID	TrimLeft();
	VOID	TrimRight();
	VOID	TrimSpace();
	//从nStart起查找
	INT		Find(CHAR c, INT nStart = 0) const;
	INT		Find(LPCSTR  ptr, INT nStart = 0) const;
	//-1表示从最后一个字符反向查找
	INT		ReverseFind(CHAR c, INT nStart = -1) const;
	INT		ReverseFind(LPCSTR ptr, INT nStart = -1) const;
	LPCSTR	CStr() const { return _QS_BUFFER; }
	//获取缓冲区（危险操作）
	//CHAR*	DangerGetBuffer();
	///////////////
	operator LPCSTR() const { return _QS_BUFFER; }
	//operator LPSTR() { return _QS_BUFFER; }
	CHAR operator [](INT index) const { return _QS_BUFFER[index]; }
	QString& operator = (LPCSTR ptr);
	QString& operator = (const QString &str);
	QString& operator += (CHAR c);
	QString& operator += (LPCSTR ptr);
	QString& operator += (const QString &str);
	friend QString operator + (const QString &str1, const QString &str2);
	friend QString operator + (const QString &str1, LPCSTR str2);
	friend QString operator + (LPCSTR str1, const QString &str2);
	friend QString operator + (const QString &str, CHAR c);
	friend QString operator + (CHAR c, const QString &str2);
	friend bool operator == (const QString &str1, const QString &str2);
	friend bool operator == (LPCSTR str1, const QString &str2);
	friend bool operator == (const QString &str1, LPCSTR str2);
	friend bool operator != (const QString &str1, const QString &str2);
	friend bool operator != (LPCSTR str1, const QString &str2);
	friend bool operator != (const QString &str1, LPCSTR str2);
	friend bool operator > (const QString &str1, const QString &str2);
	friend bool operator > (LPCSTR str1, const QString &str2);
	friend bool operator > (const QString &str1, LPCSTR str2);
	friend bool operator < (const QString &str1, const QString &str2);
	friend bool operator < (LPCSTR str1, const QString &str2);
	friend bool operator < (const QString &str1, LPCSTR str2);
	friend bool operator >= (const QString &str1, const QString &str2);
	friend bool operator >= (LPCSTR str1, const QString &str2);
	friend bool operator >= (const QString &str1, LPCSTR str2);
	friend bool operator <= (const QString &str1, const QString &str2);
	friend bool operator <= (LPCSTR str1, const QString &str2);
	friend bool operator <= (const QString &str1, LPCSTR str2);
private:
	// 私有构造，直接赋值buffer（不增加引用计数）
	QString(IMemBlock* piBuffer) : m_piBuffer(piBuffer) { }
	VOID CopyOnWrite();
protected:
	IMemBlock* m_piBuffer;
	static IMemBlock* s_piEmptyBuffer;
};

//////////inline function///////////////////////////////////////////////
inline UINT QString::GetLength() const
{
	return _QS_LEN;
}
inline CHAR QString::GetAt(INT index) const
{
	return _QS_BUFFER[index];
}/*
inline VOID KQString::SetAt(INT index, const CHAR ch)
{
	CopyOnWrite();
	_QS_BUFFER[index] = ch;
}*/
inline BOOL QString::IsEmpty() const
{
	return _QS_BUFFER[0] == 0 ? TRUE : FALSE;
}
inline QString QString::Mid(INT nFirst) const
{
	if (nFirst >= _QS_LEN) return QString();
	return QString(_QS_BUFFER + nFirst, _QS_LEN - nFirst);
}
inline QString QString::Mid(INT nFirst, INT nCount) const
{
	if (nFirst >= _QS_LEN) return QString();
	return QString(_QS_BUFFER + nFirst, MIN(nCount, _QS_LEN - nFirst));
}
inline QString QString::Left(INT nCount) const
{
	return QString(_QS_BUFFER, nCount);
}
inline QString QString::Right(INT nCount) const
{
	if (nCount > _QS_LEN) return QString();
	return QString(_QS_BUFFER + _QS_LEN - nCount, nCount);
}
inline bool operator == (const QString &str1, const QString &str2)
{
	return (str2.Compare(str1) == 0);
}
inline bool operator == (LPCSTR str1, const QString &str2)
{
	return (str2.Compare(str1) == 0);
}
inline bool operator == (const QString &str1, LPCSTR str2)
{
	return (str1.Compare(str2) == 0);
}
inline bool operator != (const QString &str1, const QString &str2)
{
	return (str2.Compare(str1) != 0);
}
inline bool operator != (LPCSTR str1, const QString &str2)
{
	return (str2.Compare(str1) != 0);
}
inline bool operator != (const QString &str1, LPCSTR str2)
{
	return (str1.Compare(str2) != 0);
}
inline bool operator > (const QString &str1, const QString &str2)
{
	return (str2.Compare(str1) < 0);
}
inline bool operator > (LPCSTR str1, const QString &str2)
{
	return (str2.Compare(str1) < 0);
}
inline bool operator > (const QString &str1, LPCSTR str2)
{
	return (str1.Compare(str2) > 0);
}
inline bool operator < (const QString &str1, const QString &str2)
{
	return (str2.Compare(str1) > 0);
}
inline bool operator < (LPCSTR str1, const QString &str2)
{
	return (str2.Compare(str1) > 0);
}
inline bool operator < (const QString &str1, LPCSTR str2)
{
	return (str1.Compare(str2) < 0);
}
inline bool operator >= (const QString &str1, const QString &str2)
{
	return (str2.Compare(str1) <= 0);
}
inline bool operator >= (LPCSTR str1, const QString &str2)
{
	return (str2.Compare(str1) <= 0);
}
inline bool operator >= (const QString &str1, LPCSTR str2)
{
	return (str1.Compare(str2) >= 0);
}
inline bool operator <= (const QString &str1, const QString &str2)
{
	return (str2.Compare(str1) >= 0);
}
inline bool operator <= (LPCSTR str1, const QString &str2)
{
	return (str2.Compare(str1) >= 0);
}
inline bool operator <= (const QString &str1, LPCSTR str2)
{
	return (str1.Compare(str2) <= 0);
}
inline VOID QString::TrimSpace()
{
	TrimLeft();
	TrimRight();
}


