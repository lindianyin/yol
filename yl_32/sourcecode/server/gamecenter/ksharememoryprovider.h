/* -------------------------------------------------------------------------
//	文件名		：	ksharememoryprovider.h
//	创建者		：	luobaohang
//	创建时间	：	2010/8/5 10:22:52
//	功能描述	：	管理底层共享内存页申请释放
//
// 按一个Page大小为960K，最大Page数1024(实际取1~1000，1001~1023给LongPtr使用)算，
	该系统最大支持接近一G的共享内存分配，且一次最大分配大小不能超过最大块大小（64k）
// -----------------------------------------------------------------------*/
#ifndef __KSHAREMEMORYPROVIDER_H__
#define __KSHAREMEMORYPROVIDER_H__

#ifndef WIN32
#include <pthread.h>
#endif

#define KD_SHARE_MEM_PAGE_SIZE (5 * 3 * 1024 * 64) // = 960k，一个Page大小，最好保证能整除各种块长度
#define KD_PAGE_TYPE_FROM_ID(PageId) (PageId >> 10) // 高6位是PageType，这样PageType最大值是64
#define KD_PAGE_INDEX_FROM_ID(PageId) (PageId & 0x3ff) // 低10位是PageIndex，这样最大Page数是1024
#define KD_SHARE_MEM_PTR_IS_NULL(ptr) (ptr.wPageId == 0)

// #define KD_SHARE_MEM_PAGE_SIZE (5 * 3 * 1024 * 64) , 以下块大小的定义最好保证一个Page长度能整除各种块长度
// s_auShareMemBlock[]元素个数不能超过64（因PageType只用了6个位来记）
static const unsigned s_auShareMemBlock[] = 
{
	0,			// undefined page
	3 * 8,
	4 * 8,
	5 * 8,
	6 * 8,
	8 * 8,
	10 * 8,
	12 * 8,
	16 * 8,
	20 * 8,
	32 * 8,
	64 * 8,
	96 * 8,
	1024,
	2 * 1024,
	3 * 1024,
	4 * 1024,
	5 * 1024,
	6 * 1024,
	8 * 1024,
	16 * 1024,
	32 * 1024,
	64 * 1024
};

//static LPVOID s_apvShareMemPage[countof(s_auShareMemBlock) * 256] = { 0 };

// 一个共享内存块指针的跨进程数据表示，用于定长内存块
struct KSHARE_MEM_PTR
{
	WORD wPageId;	// 共享内存页Id
	WORD wBlockId;	// 块Id
};

// 一个共享内存块长指针表示，主要用于指向空闲内存块链表的指针
// 用于存放链表数据的内存是从不定长内存申请的，所以要用长指针表示
struct KSHARE_MEM_LONG_PTR
{
	DWORD dwOffsetBegin;	// 起始偏移量
	DWORD dwSize;		// 大小
};
// -------------------------------------------------------------------------

// 不定长内存分配器（仅KShareMemProvider）
class KLongPtrAlloctor
{
	friend class KShareMemProvider;
	KLongPtrAlloctor(DWORD dwBasePageSize, DWORD dwIncPageSize, INT nMaxIncPage);
	KSHARE_MEM_LONG_PTR Alloc(UINT uSize);
	enum
	{
		emKMIN_DROP = 1024,	// 剩余内存小于多少时放弃该页
		emKINC_PAGE_MAX_COUNT = 16
	};
private:
	// BasePage、IncPage大小
	DWORD m_dwBasePageSize, m_dwIncPageSize;
	// 基础页的使用量
	DWORD m_dwBaseUsePos;
	// 最大IncPage，目前使用第几个IncPage
	INT m_nMaxIncPage, m_nCurMaxIncPage;
	// 每个IncPage的已使用量
	DWORD m_adwIncUsedPos[emKINC_PAGE_MAX_COUNT];
	BYTE m_byPageIsAlloc[emKINC_PAGE_MAX_COUNT];
};

class KShareMemProvider
{
	enum
	{
		// 每个PageType的最大List数量（即最多会用掉多少个Page）
		emKMAX_LST_COUNT = 1024,
		// 一个不定长分配Base Page长度
		emKLONG_PTR_BASE_PAGE_SIZE = 10 * 1024 * 1024,
		// 一个不定长分配Inc Page长度
		emKLONG_PTR_INC_PAGE_SIZE = 4 * 1024 * 1024,
		// 最大PageKey
		emKMAX_PAGE_KEY = 248,
		// KSHARE_MEMORY_DATA的Key
		emKSAHRE_MEM_KEY = 249,
		// 不定长内存Page的Key起始
		emKLONG_PTR_PAGE_KEY_BEGIN = 250,
		emKMAX_INC_PAGE = 255 - emKLONG_PTR_PAGE_KEY_BEGIN
	};
	struct KLONG_PTR_LIST
	{
		INT nCursor;
		// 空闲内存块链表指针，每个链表记录的总内存长度是一个Page大小，进程申请某种内存块大小的共享内存时，
		// 总是整个链表一起申请，即一次申请的总长度是一个Page
		KSHARE_MEM_LONG_PTR aLstBuf[emKMAX_LST_COUNT];
	};
	// 置于共享内存的类成员数据，使得结构内定义的数据能在各子进程共享，注意里面不可有指针数据
	struct KSHARE_MEMORY_DATA
	{
		// 记录了一个PageType的所有指向空闲内存块链表的长指针
		KLONG_PTR_LIST aBlockLists[countof(s_auShareMemBlock)];
		// 空闲的长指针
		KLONG_PTR_LIST aFreeLongPtr[countof(s_auShareMemBlock)];
		// PageId Gentor
		INT nPageIdGentor;
		// 不定长内存分配器
		KLongPtrAlloctor cLongPtrAlloctor;
#ifndef WIN32
		pthread_mutex_t cMutex;
#endif
	};
public:
	KShareMemProvider(void);
	~KShareMemProvider(void);
	static KShareMemProvider* Inst();
	// 初始化，由父进程(GcGateWay)调用
	BOOL Init();
	// 初始化，由Loader调用
	BOOL LoaderInit();
	// 申请一个内存块链，赋值给lptr，同时回收lptr原指向的内存（若非空）
	BOOL BuyBlockList(BYTE byPageType, KSHARE_MEM_LONG_PTR& lptr);
	// 归还一个内存块链，同时lptr重新重新指向一个空内存块链
	BOOL FreeBlockList(BYTE byPageType, KSHARE_MEM_LONG_PTR& lptr);
	// 转为普通指针
	LPVOID SharePtr2Pointer(KSHARE_MEM_PTR ptr);
	LPVOID SharePtr2Pointer(KSHARE_MEM_LONG_PTR lptr);
	// 根据内存大小获取内存块页类型Id
	BYTE GetPageTypeBySize(UINT uSize);
	// 获取指定类型内存页的每个Block大小
	UINT GetPageBlockSize(BYTE byPageType);
	// 申请一个不定长内存，用此函数申请的内存不能释放
	KSHARE_MEM_LONG_PTR FewAlloc(UINT uSize);
protected:
	// 开辟一个共享内存页
	WORD AllocPage(BYTE byPageType);
	VOID Lock();
	VOID Unlock();
private:
	friend class KLongPtrAlloctor;
	KSHARE_MEMORY_DATA* m_pShareMemData;
	// 指向共享内存的指针
	LPVOID m_apvShareMemoryPointer[256];
#ifdef WIN32
	HANDLE m_hMutex;
#else
	pthread_mutex_t* m_pMutex;
#endif
};

// -------------------------------------------------------------------------

#endif /* __KSHAREMEMORYPROVIDER_H__ */
