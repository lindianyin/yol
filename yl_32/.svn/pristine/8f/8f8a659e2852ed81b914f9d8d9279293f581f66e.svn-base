/* -------------------------------------------------------------------------
//	文件名		：	kmempipesys.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/4/29 16:40:13
//	功能描述	：
//	共享内存模拟实现管道以实现更快捷及方便管理的进程间通信。为GcGateway及每个Loader（包括非本机器的）
//	都开辟一个共享内存管道，管道数据存取均为一次性内存拷贝及真正的无锁访问。本机器的Loader管道数据由对应的
//	Loader进程直接读取，非本机器的Loader管道数据则由GcGateway通过GcGateway间的Socket连接发送到对应的其它机器。
//	在一个Loader内的模块要发送数据到另一个Loader的模块时，它就不需要关心另一个Loader是否在本机器，只需往这个
//	Loader对应的管道写入数据即可。
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kmempipesys.h"
#include "ksharememoryprovider.h"
#include "ksharememorypool.h"
#include "kkeysharemem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// -------------------------------------------------------------------------
BOOL KShareMemPacker::Init( INT nMinBufferSize )
{
	m_byBufferPageType = KShareMemProvider::Inst()->GetPageTypeBySize(nMinBufferSize);
	m_ptrBuffer = KShareMemPool::Inst()->Alloc(m_byBufferPageType);
	m_pbBuffer = (LPBYTE)KShareMemProvider::Inst()->SharePtr2Pointer(m_ptrBuffer);
	// 以分配到的定长内存块长度作为最大长度，以免浪费
	m_nBufSize = KShareMemProvider::Inst()->GetPageBlockSize(m_byBufferPageType);
	m_nCurWritePos = sizeof(DWORD); // 开头一个DWORD用来填写入数据总长度，所以从sizeof(DWORD)开始
	m_bInited = TRUE;
	return TRUE;
}

BOOL KShareMemPacker::Pack( LPVOID pvData, UINT uDataSize )
{
	if (!m_bInited)
		m_bInited = Init(KMemPipeSys::emKMAX_MIN_PACKING_BUF_SIZE);
	if ( uDataSize + sizeof(DWORD) > UINT(m_nBufSize - m_nCurWritePos))
		return FALSE;	// 缓冲区不足
	*(LPDWORD)(m_pbBuffer + m_nCurWritePos) = (DWORD)uDataSize;
	m_nCurWritePos += sizeof(DWORD);
	memcpy(m_pbBuffer + m_nCurWritePos, pvData, uDataSize);
	m_nCurWritePos += uDataSize;
	return TRUE;
}

BOOL KShareMemPacker::Pack( BYTE byConnectId, BYTE byInterfaceId, LPVOID pvData, UINT uDataSize )
{
	if (!m_bInited)
		m_bInited = Init(KMemPipeSys::emKMAX_MIN_PACKING_BUF_SIZE);
	uDataSize += 2;
	if ( uDataSize + sizeof(DWORD) > UINT(m_nBufSize - m_nCurWritePos))
		return FALSE;	// 缓冲区不足
	*(LPDWORD)(m_pbBuffer + m_nCurWritePos) = (DWORD)uDataSize;
	m_nCurWritePos += sizeof(DWORD);
	*(m_pbBuffer + m_nCurWritePos++) = byConnectId;
	*(m_pbBuffer + m_nCurWritePos++) = byInterfaceId;
	memcpy(m_pbBuffer + m_nCurWritePos, pvData, uDataSize - 2);
	m_nCurWritePos += uDataSize - 2;
	return TRUE;
}

KSHARE_MEM_PTR KShareMemPacker::Flush()
{
	*(LPDWORD)m_pbBuffer = m_nCurWritePos;
	KSHARE_MEM_PTR ptrRet = m_ptrBuffer;
	m_ptrBuffer = KShareMemPool::Inst()->Alloc(m_byBufferPageType);
	m_pbBuffer = (LPBYTE)KShareMemProvider::Inst()->SharePtr2Pointer(m_ptrBuffer);
	m_nCurWritePos = sizeof(DWORD); // 开头一个DWORD用来填写入数据总长度，所以从sizeof(DWORD)开始
	return ptrRet;
}

KShareMemPacker::KShareMemPacker()
{
	m_bInited = FALSE;
	m_nCurWritePos = 0;
}

BOOL KShareMemPacker::IsEmpty()
{
	return m_nCurWritePos > sizeof(DWORD);
}
// -------------------------------------------------------------------------

KShareMemPakPak::KShareMemPakPak()
{
	m_bInited = FALSE;
}

BOOL KShareMemPakPak::Pack( KSHARE_MEM_PTR ptr )
{
	if (!m_bInited)
		m_bInited = Init(KMemPipeSys::emKMAX_PACKING_COUNT);
	if (m_nCurPakCount >= m_nMaxPakCount)
		return FALSE;
	((KSHARE_MEM_PTR* )(m_pbBuffer + sizeof(DWORD)))[m_nCurPakCount++] = ptr;
	return TRUE;
}

KSHARE_MEM_PTR KShareMemPakPak::Flush()
{
	*(LPDWORD)m_pbBuffer = m_nCurPakCount;
	KSHARE_MEM_PTR ptrRet = m_ptrBuffer;
	m_ptrBuffer = KShareMemPool::Inst()->Alloc(m_byBufferPageType);
	m_pbBuffer = (LPBYTE)KShareMemProvider::Inst()->SharePtr2Pointer(m_ptrBuffer);
	m_nCurPakCount = 0;
	return ptrRet;
}

BOOL KShareMemPakPak::Init( INT nMaxPakCount )
{
	m_byBufferPageType = KShareMemProvider::Inst()->
		GetPageTypeBySize(nMaxPakCount * sizeof(KSHARE_MEM_PTR) + sizeof(DWORD));
	m_ptrBuffer = KShareMemPool::Inst()->Alloc(m_byBufferPageType);
	m_pbBuffer = (LPBYTE)KShareMemProvider::Inst()->SharePtr2Pointer(m_ptrBuffer);
	m_nMaxPakCount = nMaxPakCount;
	m_nCurPakCount = 0;
	m_bInited = TRUE;
	return TRUE;
}

BOOL KShareMemPakPak::IsFull()
{
	return m_nCurPakCount >= m_nMaxPakCount;
}
// -------------------------------------------------------------------------
KMemPipeSys::KMemPipeSys()
{
	// 无锁机制基于KSHARE_MEM_PTR可转化为一个原子量
	ASSERT(sizeof(ATOM_T) == sizeof(KSHARE_MEM_PTR));
	m_pMemPacker = NULL;
	m_pMemPakPak = NULL;
	m_bDataFlag = FALSE;
}

KMemPipeSys::~KMemPipeSys()
{
	SAFE_DELETE_ARRAY(m_pMemPacker);
	SAFE_DELETE_ARRAY(m_pMemPakPak);
}

KMemPipeSys* KMemPipeSys::Inst()
{
	static KMemPipeSys _sObject;
	return &_sObject;
}

BOOL KMemPipeSys::GwInit(INT nTotalLoaderCount)
{
	Q_Printl("Init MemPipeSys...");

	// Share Data Init
	// 申请内存，注意aMemPtr里元素个数为 nTotalLoaderCount + 1 （除Loader外GcGateway也占用一个管道）
	m_pShareMemData = (KSHARE_MEM_DATA* )KKeyShareMem::Inst()->CreateMem(emKSMKEY_MEMPIPESYS,
		sizeof(KSHARE_MEM_DATA) + nTotalLoaderCount * sizeof(KSHARE_MEM_PTR));
	QCONFIRM_RET_FALSE(m_pShareMemData);
	m_pShareMemData->nLoaderCount = nTotalLoaderCount;
	ZeroStruct(m_pShareMemData->aMemPtr);

	QCONFIRM_RET_FALSE(!m_pMemPacker && !m_pMemPakPak);
	m_pMemPacker = new KShareMemPacker[nTotalLoaderCount + 1];
	m_pMemPakPak = new KShareMemPakPak[nTotalLoaderCount + 1];
	return TRUE;
}

BOOL KMemPipeSys::LoaderInit()
{
	Q_Printl("Init MemPipeSys for Loader...");

	m_pShareMemData = (KSHARE_MEM_DATA* )KKeyShareMem::Inst()->GetMem(emKSMKEY_MEMPIPESYS);
	QCONFIRM_RET_FALSE(m_pShareMemData);

	QCONFIRM_RET_FALSE(!m_pMemPacker && !m_pMemPakPak);
	m_pMemPacker = new KShareMemPacker[m_pShareMemData->nLoaderCount + 1];
	m_pMemPakPak = new KShareMemPakPak[m_pShareMemData->nLoaderCount + 1];
	return TRUE;
}

BOOL KMemPipeSys::Write( INT nLoaderId, LPVOID pvData, UINT uSize )
{
	QCONFIRM_RET_FALSE(nLoaderId >= 0 && nLoaderId <= m_pShareMemData->nLoaderCount);
	QCONFIRM_RET_FALSE(uSize < emKMAX_PACKET_SIZE);
	// 目前Loader即对应数组下标，以后若增加新的管道（LoaderId用-1，-2等）则对应关系可能改变
	INT nPos = nLoaderId;
	if (!m_pMemPacker[nPos].Pack(pvData, uSize))
	{
		// 合包已满，放入二级合包器
		if (m_pMemPakPak[nPos].IsFull())
		{
			static BYTE s_aByReported[256] = { 0 };
			if (s_aByReported[nLoaderId & 0xff] == 0)	// 保证只报错一次
			{
				Q_Error("Write Pipe Data Error，Buffer is Full：" << nLoaderId);
				s_aByReported[nLoaderId & 0xff] = 1;
			}
			return FALSE;;
		}
		QCONFIRM_RET_FALSE(m_pMemPakPak[nPos].Pack(m_pMemPacker[nPos].Flush()));
		// 把当前包再放入重置后的合包器，此时应该能放入
		QCONFIRM_RET_FALSE(m_pMemPacker[nPos].Pack(pvData, uSize));
	}
	m_bDataFlag = TRUE;
	return TRUE;
}

BOOL KMemPipeSys::WriteInterfaceData( INT nLoaderId, BYTE byConnectId,
									 BYTE byInterfaceId, LPVOID pvData, UINT uSize )
{
	QCONFIRM_RET_FALSE(nLoaderId >= 0 &&  nLoaderId <= m_pShareMemData->nLoaderCount);
	QCONFIRM_RET_FALSE(uSize + 2 < emKMAX_PACKET_SIZE);
	// 目前Loader即对应数组下标，以后若增加新的管道（LoaderId用-1，-2等）则对应关系可能改变
	INT nPos = nLoaderId;
	if (!m_pMemPacker[nPos].Pack(pvData, uSize))
	{
		// 合包已满，放入二级合包器
		QCONFIRM_RET_FALSE(m_pMemPakPak[nPos].Pack(m_pMemPacker[nPos].Flush()));
		// 把当前包再放入重置后的合包器，此时应该能放入
		QCONFIRM_RET_FALSE(m_pMemPacker[nPos].Pack(pvData, uSize));
	}
	m_bDataFlag = TRUE;
	return TRUE;
}

INT KMemPipeSys::Read( INT nPos, KSHARE_MEM_PTR& ptr )
{
	QCONFIRM_RET((UINT)nPos <= (UINT)m_pShareMemData->nLoaderCount, -1);
	ATOM_T t = _AtomRead(m_pShareMemData->aMemPtr + nPos);
	// 如果读取缓冲区不为空
	if (t)
	{
		ptr = *(KSHARE_MEM_PTR* )&t;
		_AtomSet(m_pShareMemData->aMemPtr + nPos, 0);
		return 1;
	}
	return 0;
}

INT KMemPipeSys::Process( INT nLoaderId, IKPacketProcess* piProcess )
{
	// 在GcGateway端Process可能在初始化前就调用
	if (!m_pShareMemData)
		return 0;

	QCONFIRM_RET_FALSE(nLoaderId >= 0 && nLoaderId <= m_pShareMemData->nLoaderCount);
	KSHARE_MEM_PTR ptr;
	// 目前Loader即对应数组下标，以后若增加新的管道（LoaderId用-1，-2等）则对应关系可能改变
	INT nPos = nLoaderId;
	INT nRet = Read(nPos, ptr);
	if (nRet != 1)
		return nRet;
	LPBYTE pbBuffer = (LPBYTE)KShareMemProvider::Inst()->SharePtr2Pointer(ptr);
	QCONFIRM_RET(pbBuffer, -1);
	DWORD dwPakCount = *(LPDWORD)pbBuffer;	// 并包的数量
	pbBuffer += sizeof(DWORD);
	for (UINT i = 0; i < dwPakCount; ++i)
	{
		KSHARE_MEM_PTR ptrPak = *(KSHARE_MEM_PTR* )pbBuffer;
		pbBuffer += sizeof(KSHARE_MEM_PTR);
		LPBYTE pbPakBuffer = (LPBYTE)KShareMemProvider::Inst()->SharePtr2Pointer(ptrPak);
		DWORD dwDataLen = *(LPDWORD)pbPakBuffer;
		LPBYTE pbPakBufEnd = pbPakBuffer + dwDataLen;
		pbPakBuffer += sizeof(DWORD);
		while (pbPakBuffer < pbPakBufEnd)
		{
			DWORD dwPacketLen = *(LPDWORD)pbPakBuffer;
			pbPakBuffer += sizeof(DWORD);
			piProcess->ProcessData(pbPakBuffer, dwPacketLen);
			pbPakBuffer += dwPacketLen;
		}
		// 回收内存
		KShareMemPool::Inst()->Free(ptrPak);
	}
	// 回收内存
	KShareMemPool::Inst()->Free(ptr);
	return nRet;
}

BOOL KMemPipeSys::Flush()
{
	// 无数据即返回，除提高效率，也为防止GcGateway未调GwInit时就在游戏循环执行Flush()会出错
	if (!m_bDataFlag)
		return TRUE;

	// 先设为FALSE
	m_bDataFlag = FALSE;

	for (INT i = 0; i <= m_pShareMemData->nLoaderCount; ++i)
	{
		// 如果当前写入缓冲区不为空
		if (!m_pMemPacker[i].IsEmpty())
		{
			// 如果读取缓冲区已空出来
			if(!_AtomRead(m_pShareMemData->aMemPtr + i))
			{
				m_pMemPakPak[i].Pack(m_pMemPacker[i].Flush());
				KSHARE_MEM_PTR ptr = m_pMemPakPak[i].Flush();
				_AtomSet(m_pShareMemData->aMemPtr + i, *(ATOM_T* )&ptr);
			}
			else // 上次数据还在读取状态，本次数据暂不能传送
			{
				m_bDataFlag = TRUE;	// 下个游戏循环继续
			}
		}
	}

	return TRUE;
}

ATOM_T KMemPipeSys::_AtomRead( LPVOID pvData )
{
	return *(ATOM_T* )pvData;
}

VOID KMemPipeSys::_AtomSet( LPVOID pvData, ATOM_T tValue )
{
	*(ATOM_T* )pvData = tValue;
}

// -------------------------------------------------------------------------

