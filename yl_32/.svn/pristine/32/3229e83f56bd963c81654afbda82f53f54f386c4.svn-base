/* -------------------------------------------------------------------------
//	文件名		：	kmempipesys.h
//	创建者		：	luobaohang
//	创建时间	：	2010/4/29 16:39:57
//	功能描述	：	共享内存模拟实现管道通信
//	
// -----------------------------------------------------------------------*/
#ifndef __KMEMPIPESYS_H__
#define __KMEMPIPESYS_H__

#include "Misc/objectpool.h"
#include "ksharememorybuffer.h"

// 合包器
class KShareMemPacker
{
public:
	KShareMemPacker();
	// 打入包中，如包已满返回FALSE
	BOOL Pack(LPVOID pvData, UINT uDataSize);
	// 打入包中，如包已满返回FALSE
	BOOL Pack(BYTE byConnectId, BYTE byInterfaceId, LPVOID pvData, UINT uDataSize);
	// 打上结束标记，返回缓冲区内存指针，并重置（重新申请缓冲区）
	KSHARE_MEM_PTR Flush(); 
	// 是否有数据
	BOOL IsEmpty();
protected:
	// 初始化，最小缓冲区大小
	BOOL Init(INT nMinBufferSize);
protected:
	KSHARE_MEM_PTR m_ptrBuffer; // 缓冲区共享内存指针
	BYTE m_byBufferPageType; // 记录缓冲区的PageType
	INT m_nBufSize;	// 记录缓冲区大小
	LPBYTE m_pbBuffer; // 缓冲区指针
	INT m_nCurWritePos;	// 当前写入位置
	BOOL m_bInited;
};

// 二级合包器，即把合包器合的每个包共享内存指针再合到一个包内
class KShareMemPakPak
{
public:
	KShareMemPakPak();
	// 打入包中，如包已满返回FALSE
	BOOL Pack(KSHARE_MEM_PTR ptr);
	// 打上结束标记，返回缓冲区内存指针，并重置（重新申请缓冲区）
	KSHARE_MEM_PTR Flush();
	BOOL IsFull();
protected:
	// 初始化，最大合包数
	BOOL Init(INT nMaxPakCount);
protected:
	KSHARE_MEM_PTR m_ptrBuffer; // 缓冲区共享内存指针
	BYTE m_byBufferPageType; // 记录缓冲区的PageType
	INT m_nMaxPakCount;	// 记录最大合包数
	LPBYTE m_pbBuffer; // 缓冲区指针
	INT m_nCurPakCount;	// 当前已合包数
	BOOL m_bInited;
};

struct IKPacketProcess
{
	virtual BOOL ProcessData(LPVOID pvData, UINT uSize) = 0;
};

// 使用这个将一个类成员函数转化为IKPacketProcess
template<class Processor, class ProcessFun>
struct KTPacketProcess : public IKPacketProcess
{
	KTPacketProcess(Processor pr, ProcessFun fn) : m_pr(pr), m_fn(fn) { };
	BOOL ProcessData(LPVOID pvData, UINT uSize)
	{
		return (m_pr->*m_fn)(pvData, uSize);
	}
	Processor m_pr;
	ProcessFun m_fn;
};

template<class Processor, class ProcessFun>
KTPacketProcess<Processor, ProcessFun> PipeProcessFun(Processor pr, ProcessFun fn)
{
	return KTPacketProcess<Processor, ProcessFun>(pr, fn);
}

// -------------------------------------------------------------------------
// TODO:Linux
typedef DWORD ATOM_T;
class KMemPipeSys
{
public:
	enum
	{
		emKMAX_PACKET_SIZE = 64 * 1024,
		emKMAX_MIN_PACKING_BUF_SIZE = emKMAX_PACKET_SIZE,	// 合包缓冲的最小长度
		emKMAX_PACKING_COUNT = 100,	// 最大合并包数量
	};
	struct KSHARE_MEM_DATA
	{
		INT nLoaderCount;
		KSHARE_MEM_PTR aMemPtr[1];	// 可读数据指针
	};
	KMemPipeSys();
	~KMemPipeSys();
	static KMemPipeSys* Inst();
	// Init，Called by GcGateway，传入的为总Loader数量（包括非本机器的）
	BOOL GwInit(INT nTotalLoaderCount);
	// Loader Init，Called by Loader
	BOOL LoaderInit();
	// 处理Pipe数据包，成功返回1，无数据返回0，错误返回-1, GcGateway则nLoaderId填0
	INT Process(INT nLoaderId, IKPacketProcess* piProcess);
	// 写入Loader Pipe数据，GcGateway则nLoaderId填0
	BOOL Write(INT nLoaderId, LPVOID pvData, UINT uSize);
	BOOL WriteInterfaceData(INT nLoaderId, BYTE byConnectId, BYTE byInterfaceId, LPVOID pvData, UINT uSize);
	// 执行写入
	BOOL Flush();
private:
	// 读取Pipe数据包，成功返回1，无数据返回0，错误返回-1
	INT Read( INT nPos, KSHARE_MEM_PTR& ptr );
	ATOM_T _AtomRead(LPVOID pvData);
	VOID _AtomSet(LPVOID pvData, ATOM_T tValue);
protected:
	KSHARE_MEM_DATA* m_pShareMemData;
	KShareMemPacker* m_pMemPacker;	// 一级合包器数组指针，每个管道对应一个合包器
	KShareMemPakPak* m_pMemPakPak;	// 二级合包器数组指针，每个管道对应一个合包器
	BOOL m_bDataFlag;	// 是否有数据
};

// -------------------------------------------------------------------------

#endif /* __KMEMPIPESYS_H__ */
