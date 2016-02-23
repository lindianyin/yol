/* -------------------------------------------------------------------------
//	文件名		：	ksharememoryprovider.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/8/5 10:23:06
//	功能描述	：	跨进程共享内存池
//
·跨进程共享内存池的设计目的是：在共享内存池分配中，一个进程分配的共享内存块数据能直接以类似指针方式传给另一个进程，
然后在另一个进程可以直接释放该共享内存块指针，这样大大提高了数据传递效率及减少拷贝开销，但会造成单进程内共享内存的
分配和释放不均衡，所以用跨进程的内存池管理所有进程的内存分配和回收，因为所有进程的共享内存分配和释放一定是均衡的。

·内存回收是一个Page大小为单位回收，比如Page大小是1000，那么对长度是10的Block，一次就回收分配100个，
长度为50的，一次就回收分配20个。

·内存块先在进程内的内存池回收分配，回收分配都在在BlockList里，一个BlockList总内存最大大小就等于一个Page大小。
如果进程内存池对某种Block的分配速度大于回收速度，那么它就总要向跨进程内存池以一个BlockList为单位申请内存，反之，
就总是以一个BlockList为单位释放内存。

·进程内存池向跨进程共享内存池申请BlockList时，会先把一个空的BlockList（如果有）归还到跨进程共享内存池，
然后直接获得一个满的BlockList。释放时则相反，直接把满的BlockList归还，获得一个空的BlockList。
BlockList的传递都用共享内存指针（KSHARE_MEM_LONG_PTR）的方式传递。

// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include <vector>
#include <new>
#include "ksharememoryprovider.h"
#ifdef __unix
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#endif // __unix

#define KD_PLACEMENT_NEW new
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

#define KD_MUTEX_NAME "Mutex_KShareMemProvider"
const static CHAR kszMapFilePrefix[] = "/Gc_Key_ShareMemProvider_";

// 开辟共享内存，使用一个字节类型作键值
static LPVOID _ShareMemoryAlloc(BYTE byKey, UINT uSize)
{
	LPVOID pvBuffer = NULL;
	DWORD dwLastErr = ERROR_SUCCESS;
	// Gen Key
	CHAR szMapFileName[MAX_PATH];
	snprintf(szMapFileName, sizeof(szMapFileName), "%s%1d%1d", kszMapFilePrefix, byKey >> 4, byKey & 0xf);
#ifdef WIN32
	// CreateFileMaping
	HANDLE hMem = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, uSize, szMapFileName);
	dwLastErr = GetLastError();
	if ( ! hMem || dwLastErr == ERROR_ALREADY_EXISTS)
		Q_Error("CreateFileMapping Failed : 0x" << std::hex << dwLastErr);
	pvBuffer = MapViewOfFile(hMem, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#else
	// 因为GcGateway主进程为单例，加上无法保证shm_unlink，不用O_EXCL
	int fd = shm_open(szMapFileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		int nErr = errno;
		Q_Error("shm_open Failed : " << nErr << " " << strerror(nErr));
		if (errno == EEXIST)
			Q_Error("shared memory " << szMapFileName << " already exists.");
	}
	else
	{
		ftruncate(fd, uSize);
		void* pMapping = mmap(NULL, uSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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

// 获取共享内存
static LPVOID _ShareMemoryGet(BYTE byKey)
{
	LPVOID pvBuffer = NULL;
	// Gen Key
	CHAR szMapFileName[MAX_PATH];
	snprintf(szMapFileName, sizeof(szMapFileName), "%s%1d%1d", kszMapFilePrefix, byKey >> 4, byKey & 0xf);
#ifdef WIN32
	// OpenFileMaping
	HANDLE hMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szMapFileName);
	DWORD dwLastErr = GetLastError();
	if ( ! hMem)
	{
		Q_Error("OpenFileMapping [" << szMapFileName << "] Failed : 0x" << std::hex << dwLastErr);
		return NULL;
	}
	pvBuffer = MapViewOfFile(hMem, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#else
	int fd = shm_open(szMapFileName, O_RDWR, S_IRUSR | S_IWUSR); // get a named memobj
	if (fd == -1)
	{
		int nErr = errno;
		Q_Error("shm_open Failed : " << nErr << " " << strerror(nErr));
	}
	else
	{
		struct stat sFileStat;
		fstat(fd, &sFileStat);
		void* pMapping = mmap(NULL, sFileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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

KLongPtrAlloctor::KLongPtrAlloctor( DWORD dwBasePageSize, DWORD dwIncPageSize, INT nMaxIncPage ) :
	m_dwBasePageSize(dwBasePageSize), m_nMaxIncPage(nMaxIncPage), m_dwIncPageSize(dwIncPageSize)
{
	ASSERT(nMaxIncPage <= emKINC_PAGE_MAX_COUNT);
	m_dwBaseUsePos = 1;	// 从1开始，偏移量为0表示空指针
	m_nCurMaxIncPage = 0;
	ZeroStruct(m_adwIncUsedPos);
	ZeroStruct(m_byPageIsAlloc);
}

KSHARE_MEM_LONG_PTR KLongPtrAlloctor::Alloc( UINT uSize )
{
	KSHARE_MEM_LONG_PTR ptr = { 0, uSize };
	// 分配大小不能大于一个IncSize
	QCONFIRM_RET(uSize <= m_dwIncPageSize, ptr);
	// 如果Base块剩余大小足够
	if (m_dwBaseUsePos <= m_dwBasePageSize - uSize)
	{
		ptr.dwOffsetBegin = m_dwBaseUsePos;
		m_dwBaseUsePos += uSize;
	}
	else
	{
		INT i;
		for (i = 0; i < m_nCurMaxIncPage; ++i)
		{
			// 如果该Inc块剩余大小足够
			if (m_adwIncUsedPos[i] <= m_dwIncPageSize - uSize)
				break;
		}
		if (i == m_nCurMaxIncPage)
		{
			// 已达最大IncPage
			if (m_nCurMaxIncPage >= m_nMaxIncPage)
			{
				ptr.dwOffsetBegin = 0;
				ptr.dwSize = 0;
				return ptr;
			}
			++m_nCurMaxIncPage;
		}
		ptr.dwOffsetBegin = m_dwBasePageSize + i * m_dwIncPageSize + m_adwIncUsedPos[i];
		m_adwIncUsedPos[i] += uSize;
	}
	// 长指针
	BYTE byKey = KShareMemProvider::emKLONG_PTR_PAGE_KEY_BEGIN;
	UINT uPageSize = m_dwBasePageSize;
	if (ptr.dwOffsetBegin > m_dwBasePageSize)
	{
		byKey += (BYTE)((ptr.dwOffsetBegin - m_dwBasePageSize) / m_dwIncPageSize + 1);
		uPageSize = m_dwIncPageSize;
	}
	if (!m_byPageIsAlloc[byKey - KShareMemProvider::emKLONG_PTR_PAGE_KEY_BEGIN])
	{
		_ShareMemoryAlloc(byKey, uPageSize);
		m_byPageIsAlloc[byKey - KShareMemProvider::emKLONG_PTR_PAGE_KEY_BEGIN] = TRUE;
	}
	return ptr;
}

// -------------------------------------------------------------------------
KShareMemProvider::KShareMemProvider(void)
{
	ZeroStruct(m_apvShareMemoryPointer);
	m_pShareMemData = NULL;
#ifdef WIN32
	m_hMutex = INVALID_HANDLE_VALUE;
#else
	m_pMutex = NULL;
#endif
}

KShareMemProvider::~KShareMemProvider(void)
{
#ifdef WIN32
	if (m_hMutex != INVALID_HANDLE_VALUE)
		CloseHandle(m_hMutex);
#else
	if (m_pMutex)
		pthread_mutex_destroy(m_pMutex);
#endif
}

KShareMemProvider* KShareMemProvider::Inst()
{
	static KShareMemProvider _sInst;
	return &_sInst;
}

BOOL KShareMemProvider::Init()
{
	Q_Printl("Init ShareMemProvider for GcGateway...");

	m_pShareMemData = (KSHARE_MEMORY_DATA *)_ShareMemoryAlloc(emKSAHRE_MEM_KEY, sizeof(KSHARE_MEMORY_DATA));
	QCONFIRM_RET_FALSE(m_pShareMemData);
	ZeroStruct(m_pShareMemData->aBlockLists);
	ZeroStruct(m_pShareMemData->aFreeLongPtr);
	m_pShareMemData->nPageIdGentor = 0;
	KD_PLACEMENT_NEW(&m_pShareMemData->cLongPtrAlloctor) KLongPtrAlloctor(emKLONG_PTR_BASE_PAGE_SIZE,
		emKLONG_PTR_INC_PAGE_SIZE, emKMAX_INC_PAGE);
#ifdef WIN32
	m_hMutex = CreateMutex(NULL, FALSE, KD_MUTEX_NAME);
	QCONFIRM_RET_FALSE(m_hMutex);
#else
	m_pMutex = &m_pShareMemData->cMutex;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(m_pMutex, &attr);
#endif
	return TRUE;
}

BOOL KShareMemProvider::LoaderInit()
{
	Q_Printl("Init SharedMemProvider for Loader(GC)...");
	m_pShareMemData = (KSHARE_MEMORY_DATA *)_ShareMemoryGet(emKSAHRE_MEM_KEY);
	QCONFIRM_RET_FALSE(m_pShareMemData);
#ifdef WIN32
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, KD_MUTEX_NAME);
	QCONFIRM_RET_FALSE(m_hMutex);
#else
	m_pMutex = &m_pShareMemData->cMutex;
#endif
	return TRUE;
}

BOOL KShareMemProvider::BuyBlockList( BYTE byPageType, KSHARE_MEM_LONG_PTR& lptr )
{
	CONST UINT uLPtrSize = sizeof(KSHARE_MEM_PTR) * (KD_SHARE_MEM_PAGE_SIZE / s_auShareMemBlock[byPageType]);
	Lock();
	if (m_pShareMemData->aBlockLists[byPageType].nCursor > 0)
	{
		// 回收lptr
		if (lptr.dwOffsetBegin > 0)
		{
			if (lptr.dwSize == uLPtrSize)
			{
				INT nCursor = m_pShareMemData->aFreeLongPtr[byPageType].nCursor++;
				m_pShareMemData->aFreeLongPtr[byPageType].aLstBuf[nCursor] = lptr;
			}
			else
			{
				Q_Error("Invalid Size");
			}
		}
		lptr = m_pShareMemData->aBlockLists[byPageType].aLstBuf[m_pShareMemData->aBlockLists[byPageType].nCursor--];
		Unlock();
	}	
	else // 没分配到，新申请一个
	{
		if (lptr.dwOffsetBegin == 0 || lptr.dwSize == 0)
		{
			if (m_pShareMemData->aFreeLongPtr[byPageType].nCursor > 0)
			{
				INT nCursor = --m_pShareMemData->aFreeLongPtr[byPageType].nCursor;
				lptr = m_pShareMemData->aFreeLongPtr[byPageType].aLstBuf[nCursor];
			}
			else // 没有空闲长指针，重新申请
			{
				lptr = m_pShareMemData->cLongPtrAlloctor.Alloc(uLPtrSize);
			}
		}

		WORD wPageId = AllocPage(byPageType);
		// 及时解锁
		Unlock();

		KSHARE_MEM_PTR* pBlockList = (KSHARE_MEM_PTR* )SharePtr2Pointer(lptr);
		QCONFIRM_RET_FALSE(pBlockList);
		for (UINT i = 0; i < KD_SHARE_MEM_PAGE_SIZE / s_auShareMemBlock[byPageType]; ++i)
		{
			pBlockList[i].wPageId = wPageId;
			pBlockList[i].wBlockId = i;
		}
	}
	return TRUE;
}

BOOL KShareMemProvider::FreeBlockList( BYTE byPageType, KSHARE_MEM_LONG_PTR& lptr )
{
	const UINT uLPtrSize = sizeof(KSHARE_MEM_PTR) * (KD_SHARE_MEM_PAGE_SIZE / s_auShareMemBlock[byPageType]);
	// 空闲长指针是否已满
	QCONFIRM_RET_FALSE(m_pShareMemData->aBlockLists[byPageType].nCursor < emKMAX_LST_COUNT);
	// 检验参数合法（长度符合或者为空）
	QCONFIRM_RET_FALSE(lptr.dwSize == uLPtrSize || lptr.dwOffsetBegin == 0);
	Lock();
	if (lptr.dwOffsetBegin != 0)	// 如果不为空，回收
		m_pShareMemData->aBlockLists[byPageType].aLstBuf[m_pShareMemData->aBlockLists[byPageType].nCursor++] = lptr;
	// lptr重新重新指向一个空内存块链
	if (m_pShareMemData->aFreeLongPtr[byPageType].nCursor > 0)
	{
		INT nCursor = --m_pShareMemData->aFreeLongPtr[byPageType].nCursor;
		lptr = m_pShareMemData->aFreeLongPtr[byPageType].aLstBuf[nCursor];
	}
	else // 没有空闲长指针，重新申请
	{
		lptr = m_pShareMemData->cLongPtrAlloctor.Alloc(uLPtrSize);
	}
	Unlock();
	return TRUE;
}

WORD KShareMemProvider::AllocPage( BYTE byPageType )
{
	// 检查是否已满
	QCONFIRM_RET_FALSE((m_pShareMemData->nPageIdGentor) >> 2 <= emKMAX_PAGE_KEY);
	INT nPageIndex = m_pShareMemData->nPageIdGentor++;
	// 每个Key对应4个Page
	BYTE byKey = BYTE(nPageIndex >> 2);
	if (!m_apvShareMemoryPointer[byKey])
		m_apvShareMemoryPointer[byKey] = _ShareMemoryAlloc(byKey, KD_SHARE_MEM_PAGE_SIZE * 4);
	return (byPageType << 10) | nPageIndex;
}

LPVOID KShareMemProvider::SharePtr2Pointer( KSHARE_MEM_PTR ptr )
{
	if (KD_SHARE_MEM_PTR_IS_NULL(ptr))
		return NULL;
	INT nPageIndex = KD_PAGE_INDEX_FROM_ID(ptr.wPageId);
	// 取高位作为Key
	BYTE byKey = BYTE(nPageIndex >> 2);
	INT nPage = nPageIndex & 0x3;	// 低2位为第几个Page
	// 若当前进程还未有该指针，获取
	if (!m_apvShareMemoryPointer[byKey])
		m_apvShareMemoryPointer[byKey] = _ShareMemoryGet(byKey);
	return (LPBYTE)m_apvShareMemoryPointer[byKey] + nPage * KD_SHARE_MEM_PAGE_SIZE
		+ ptr.wBlockId * (s_auShareMemBlock[KD_PAGE_TYPE_FROM_ID(ptr.wPageId)]);
}

LPVOID KShareMemProvider::SharePtr2Pointer( KSHARE_MEM_LONG_PTR lptr )
{
	QCONFIRM_RET_NULL(lptr.dwOffsetBegin > 0 && lptr.dwSize > 0);
	// 长指针
	BYTE byKey = emKLONG_PTR_PAGE_KEY_BEGIN;
	if (lptr.dwOffsetBegin > emKLONG_PTR_BASE_PAGE_SIZE)
		byKey += (BYTE)((lptr.dwOffsetBegin - emKLONG_PTR_BASE_PAGE_SIZE) / emKLONG_PTR_INC_PAGE_SIZE + 1);
	if (m_apvShareMemoryPointer[byKey])
		return m_apvShareMemoryPointer[byKey];
	return m_apvShareMemoryPointer[byKey] = _ShareMemoryGet(byKey);
}

KSHARE_MEM_LONG_PTR KShareMemProvider::FewAlloc( UINT uSize )
{
	Lock();
	KSHARE_MEM_LONG_PTR lptr = m_pShareMemData->cLongPtrAlloctor.Alloc(uSize);
	Unlock();
	return lptr;
}

BYTE KShareMemProvider::GetPageTypeBySize( UINT uSize )
{
	// TODO:改哈希
	CONST static INT nReserveSize = countof(s_auShareMemBlock) - 1;
	/*二分查找*/
	INT nLeft = 0;					//左端点
	INT nRight = nReserveSize;		//右端点
	INT nMid = nReserveSize >> 1;	//中点
	do 
	{
		if (uSize < (size_t)s_auShareMemBlock[nMid])
			nRight = nMid;
		else
			nLeft = nMid;
		nMid = (nLeft + nRight) >> 1;

	} while(nLeft < nMid);
	if (uSize <= (size_t)s_auShareMemBlock[nLeft])
		return (BYTE)nLeft;
	if (uSize > (size_t)s_auShareMemBlock[nRight])
	{
		Q_Error(uSize << " " << nRight);
		return 0;
	}
	return (BYTE)nRight;
}

UINT KShareMemProvider::GetPageBlockSize( BYTE byPageType )
{
	QCONFIRM_RET_NULL(byPageType < countof(s_auShareMemBlock));
	return s_auShareMemBlock[byPageType];
}

VOID KShareMemProvider::Lock()
{
#ifdef WIN32
	WaitForSingleObject(m_hMutex, INFINITE);
#else
	pthread_mutex_lock(m_pMutex);
#endif
}

VOID KShareMemProvider::Unlock()
{
#ifdef WIN32
	ReleaseMutex(m_hMutex);
#else
	pthread_mutex_unlock(m_pMutex);
#endif
}
// -------------------------------------------------------------------------
