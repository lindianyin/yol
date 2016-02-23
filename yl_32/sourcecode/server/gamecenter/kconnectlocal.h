/* -------------------------------------------------------------------------
//	文件名		：	kconnectlocal.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/9 16:03:07
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KCONNECTLOCAL_H__
#define __KCONNECTLOCAL_H__
#include "serverbase/kgcmoduleinterface.h"
#include "kgcinterfacedef.h"

// 进程内连接
class KConnectLocal : public KConnectBase
{
public:
	KConnectLocal();
	// 实现KConnectBase接口
	virtual BOOL Send(BYTE byAcceptorModuleId, BYTE byConnectId, LPVOID pvData, UINT uDataSize);
	virtual BOOL IsValid() { return TRUE; }
};

// -------------------------------------------------------------------------
class KConnnectLocalMgr 
{
public:
	KConnnectLocalMgr();
	~KConnnectLocalMgr();
	static KConnnectLocalMgr* Inst();
	KConnectLocal* NewConnect();
	// 新增一个包，返回写入缓冲区指针
	LPBYTE NewPackage(UINT uDataLen);
	// 协议处理
	BOOL ProcessPackage();
private:
	BOOL NewPackageBufBlock();
private:
	std::list<KConnectLocal> m_lstConnect;	// 连接对象
	std::vector<LPBYTE> m_vecPackageBuf;	// 协议缓冲区，Connect和accept的协议数据都在这里面，通过标识位区分
	INT m_nCurblock;						// 当前缓冲区块
	LPBYTE m_pCurBlockWritepos;				// 当前缓冲区块写指针
	UINT m_uCurBlockRestSize;				// 当前缓冲区块剩余内存大小
};

// -------------------------------------------------------------------------

#endif /* __KCONNECTLOCAL_H__ */
