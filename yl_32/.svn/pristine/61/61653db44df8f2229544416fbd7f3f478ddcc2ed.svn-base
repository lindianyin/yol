/* -------------------------------------------------------------------------
//	文件名		：	kkeysharemem.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/8/24 11:31:02
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "ksharememorypool.h"
#include "kkeysharemem.h"
#ifdef __unix
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#endif // __unix


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KKeyShareMem::KKeyShareMem(void)
{
	m_pvBuffer = NULL;
}

KKeyShareMem* KKeyShareMem::Inst()
{
	static KKeyShareMem s_Inst;
	return &s_Inst;
}

LPVOID KKeyShareMem::GetMem( BYTE byKey )
{
	if (!m_pvBuffer)
		m_pvBuffer = ShareMemGet();
	QCONFIRM_RET_NULL(m_pvBuffer);
	KSHARE_MEM_PTR ptr = ((KSHARE_MEM_PTR* )m_pvBuffer)[byKey];
	QCONFIRM_RET_NULL( ! KD_SHARE_MEM_PTR_IS_NULL(ptr) );
	return KShareMemProvider::Inst()->SharePtr2Pointer(ptr);
}

LPVOID KKeyShareMem::CreateMem( BYTE byKey, UINT uSize )
{
	if (!m_pvBuffer)
		m_pvBuffer = ShareMemGet();
	QCONFIRM_RET_NULL(m_pvBuffer);
	KSHARE_MEM_PTR& ptr = ((KSHARE_MEM_PTR* )m_pvBuffer)[byKey];
	QCONFIRM_RET_NULL( KD_SHARE_MEM_PTR_IS_NULL(ptr) );
	ptr = KShareMemPool::Inst()->Alloc(KShareMemProvider::Inst()->GetPageTypeBySize(uSize));
	QCONFIRM_RET_NULL( ! KD_SHARE_MEM_PTR_IS_NULL(ptr) );
	return KShareMemProvider::Inst()->SharePtr2Pointer(ptr);
}

BOOL KKeyShareMem::Init()
{
	Q_Printl("Init KeyShareMem...");

	LPVOID pvBuffer = ShareMemAlloc();
	QCONFIRM_RET_FALSE(pvBuffer);
	ZeroMemory(pvBuffer, sizeof(KKeyShareData));
	return TRUE;
}

const static LPCSTR cszMapFileName = "/_Gc_Key_Share_Mem";

LPVOID KKeyShareMem::ShareMemAlloc()
{
	LPVOID pvBuffer = NULL;
	DWORD dwLastErr = ERROR_SUCCESS;
	// Alloc Size sizeof(KKeyShareData)
#ifdef WIN32
	HANDLE hMem = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(KKeyShareData), cszMapFileName);
	dwLastErr = GetLastError();
	if ( ! hMem || dwLastErr == ERROR_ALREADY_EXISTS)
		Q_Error("CreateFileMapping Failed : 0x" << std::hex << dwLastErr);
	pvBuffer = MapViewOfFile(hMem, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#else
	// 因为GcGateway主进程为单例，加上无法保证shm_unlink，不用O_EXCL
	int fd = shm_open(cszMapFileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		Q_Error("shm_open Failed : " << errno);
		if (errno == EEXIST)
			Q_Error("shared memory " << cszMapFileName << " already exists.");
	}
	else
	{
		ftruncate(fd, getpagesize());
		void* pMapping = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (pMapping == (void*)-1)
		{
			int nErr = errno;
			Q_Error("mmap Failed: " << nErr << " " << strerror(nErr));
		}
		else
		{
			pvBuffer = pMapping;
		}
	}
#endif
	QCONFIRM_RET_NULL(pvBuffer);
	return pvBuffer;
}

LPVOID KKeyShareMem::ShareMemGet()
{
	LPVOID pvBuffer = NULL;
	// Alloc Size sizeof(KKeyShareData)
#ifdef WIN32
	HANDLE hMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cszMapFileName);
	QCONFIRM_RET_FALSE(hMem != NULL);
	pvBuffer = MapViewOfFile(hMem, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#else
	int fd = shm_open(cszMapFileName, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		int nErr = errno;
		Q_Error("shm_open Failed : " << nErr << " " << strerror(nErr));
	}
	else
	{
		void* pMapping = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (pMapping == (void*)-1)
		{
			int nErr = errno;
			Q_Error("mmap Failed: " << nErr << " " << strerror(nErr));
		}
		else
		{
			pvBuffer = pMapping;
		}
	}

#endif
	QCONFIRM_RET_NULL(pvBuffer);
	return pvBuffer;
}


// -------------------------------------------------------------------------


