
#include "CorePrivate.h"
#include "AloneFile.h"
#include "QStrBase.h"


#define CONTENT_BUFFER_NOT_SUPPORT	(m_nContentBufferSize = (unsigned long)(-1))
#define	IS_CONTENT_BUFFER_SUPPORT	(m_nContentBufferSize != (unsigned long)(-1))

//---------------------------------------------------------------------------
// 功能:	购造函数
//---------------------------------------------------------------------------
QAloneFile::QAloneFile(FILE* pFile)
{
	m_hFile	= pFile;
	m_pContentBuffer = NULL;
	m_nContentBufferSize = 0;
}

//---------------------------------------------------------------------------
// 功能:	析造函数
//---------------------------------------------------------------------------
QAloneFile::~QAloneFile()
{
	Close();
}

//---------------------------------------------------------------------------
// 功能:	打开一个文件，准备读取写
// 参数:	FileName	文件名
// 返回:	成功返回TRUE，失败返回FALSE。
//---------------------------------------------------------------------------
QAloneFile*	QAloneFile::Open(const char* FileName, int WriteSupport /*= false*/)
{
	FILE* pFile = NULL;
	char PathName[MAX_PATH];
	g_GetFullPath(PathName, FileName);

	#ifdef unix
	{
		char *ptr = PathName;
        while(*ptr)
		{
			if (*ptr == '\\')
				*ptr = '/';
			ptr++;
        }
	}
	#endif	// #ifdef unix

	const char*	pMode = "rb";
	if (WriteSupport)
	{
		if (g_IsFileExist(PathName))
			pMode = "r+b";
		else
			pMode = "a+b";
	}

	pFile = fopen(PathName, pMode);
	#ifdef unix
	{
		if (pFile == NULL)
		{
			QStrLower(PathName);
			pFile = fopen(PathName, pMode);
		}
	}
	#endif	// #ifdef unix

	if (pFile)
	{
		QAloneFile* pAloneFile = new QAloneFile(pFile);
		if (pAloneFile && WriteSupport)
			pAloneFile->m_nContentBufferSize = -1;
		return pAloneFile;
	}
	else
	{
		return NULL;
	}
}
//---------------------------------------------------------------------------
// 功能:	创建一个文件，准备写入。
// 参数:	FileName	文件名
// 返回:	成功返回TRUE，失败返回FALSE。
//---------------------------------------------------------------------------
QAloneFile*	QAloneFile::Create(const char*  FileName)
{
	FILE* pFile = NULL;

	char FullPathName[MAX_PATH] = "";
	g_GetFullPath(FullPathName, FileName);
#ifdef unix
	{
		char *ptr = FullPathName;
		while(*ptr)
		{
			if (*ptr == '\\')
				*ptr = '/';
			ptr++;
		}
	}
#endif	// #ifdef unix
	char PathName[MAX_PATH] = "";
	g_ExtractFilePath(PathName, FullPathName);
	g_CreatePath(PathName);
	pFile = fopen(FullPathName, "wb+");
#ifdef unix
	{
		if (pFile == NULL)
		{
			QStrLower(FullPathName);
			pFile = fopen(FullPathName, "wb+");
		}
	}
#endif	// #ifdef unix

	return pFile ? new QAloneFile(pFile) : NULL;
}

//---------------------------------------------------------------------------
// 功能:	关闭打开的文件
//---------------------------------------------------------------------------
void QAloneFile::Close()
{
	m_nContentBufferSize = 0;
	SAFE_FREE(m_pContentBuffer);
	if (m_hFile)
	{
		fclose(m_hFile);
		m_hFile	= NULL;
	}
}

//---------------------------------------------------------------------------
// 功能:	读取文件数据
// 参数:	Buffer	读取数据存放的内存区域
//			ReadBytes	读取数据的字节数
// 返回:	成功返回读取的字节数，失败返回0。
//---------------------------------------------------------------------------
unsigned long QAloneFile::Read(void* Buffer, unsigned long ReadBytes)
{
	long BytesReaded;
	if (m_hFile)
		BytesReaded = (unsigned long)fread(Buffer, 1, ReadBytes, m_hFile);
	else
		BytesReaded = 0;
	return BytesReaded;
}

//---------------------------------------------------------------------------
// 功能:	写入文件数据
// 参数:	Buffer		写入数据存放的内存区域
//			WriteBytes	写入数据的字节数
// 返回:	成功返回写入的字节数，失败返回0。
//---------------------------------------------------------------------------
unsigned long QAloneFile::Write(const void* Buffer, unsigned long WriteBytes)
{
	unsigned long BytesWrited;
	if (m_hFile)
		BytesWrited = (unsigned long)fwrite(Buffer, 1, WriteBytes, m_hFile);
	else
		BytesWrited = 0;

	return BytesWrited;
}

// 获得文件内容Buffer
void* QAloneFile::GetBuffer()
{
	if (m_pContentBuffer == NULL && m_hFile && IS_CONTENT_BUFFER_SUPPORT)
	{
		m_nContentBufferSize = Size();
		if (m_nContentBufferSize)
		{
			m_pContentBuffer = malloc(m_nContentBufferSize);
			if (m_pContentBuffer)
			{
				Seek(0, SEEK_SET);
				if (Read(m_pContentBuffer, m_nContentBufferSize) != m_nContentBufferSize)
				{
					free(m_pContentBuffer);
					m_pContentBuffer = NULL;
				}
			}
		}
	}
	return m_pContentBuffer;
}

//---------------------------------------------------------------------------
// 功能:	移动文件指针位置
// 参数:	Offset		Number of bytes from origin. 
//			Origin		Initial position: SEEK_CUR SEEK_END SEEK_SET
// 返回:	成功返回指针位置，失败返回-1。
//---------------------------------------------------------------------------
long QAloneFile::Seek(long Offset, int Origin)
{
	if (m_hFile)
	{
		fseek(m_hFile, Offset, Origin);
		Offset = ftell(m_hFile);
	}
	else
	{
		Offset = -1;
	}

	return Offset;
}

//---------------------------------------------------------------------------
// 功能:	取得文件指针位置
// 返回:	成功返回指针位置，失败返回-1。
//---------------------------------------------------------------------------
long QAloneFile::Tell()
{
	long Offset;
	if (m_hFile)
		Offset = ftell(m_hFile);
	else
		Offset = -1;
	return Offset;
}

//---------------------------------------------------------------------------
// 功能:	取得文件长度
// 返回:	成功返回文件长度，失败返回0。
//---------------------------------------------------------------------------
unsigned long QAloneFile::Size()
{
	unsigned long Size;
	if (m_pContentBuffer)
	{
		Size = m_nContentBufferSize;
	}
	else if (m_hFile)
	{
		long Offset = ftell(m_hFile);
		fseek(m_hFile, 0, SEEK_END);
		Size = ftell(m_hFile);
		fseek(m_hFile, Offset, SEEK_SET);
//		if (Size == (unsigned long)(-1))
//			Size = 0;
	}
	else
	{
		Size = 0;
	}
	return Size;
}

// 判断打开的是否是包中的文件
int	QAloneFile::IsFileInPak()
{
	return false;
}

void QAloneFile::Release()
{
	Close();
	delete this;
}

//---------------------------------------------------------------------------
// 功能:	判断是否分块压缩文件

// 返回:	非0值为分块压缩文件
//---------------------------------------------------------------------------
int	QAloneFile::IsPackedByFragment()
{
	return 0;
}

//获取文件分块的数目
int	QAloneFile::GetFragmentCount()
{
	return 0;
}

//获取分块的大小
unsigned int QAloneFile::GetFragmentSize(int nFragmentIndex)
{
	return 0;
}

//读取一个文件分块
unsigned long	QAloneFile::ReadFragment(int nFragmentIndex, void*& pBuffer)
{
	return 0;
}
