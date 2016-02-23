/* -------------------------------------------------------------------------
//	文件名		：	kkeysharemem.h
//	创建者		：	luobaohang
//	创建时间	：	2010/8/24 11:30:54
//	功能描述	：	为共享模块提供共享内存
//
// -----------------------------------------------------------------------*/
#ifndef __KKEYSHAREMEM_H__
#define __KKEYSHAREMEM_H__

#include "ksharememoryprovider.h"

enum KE_KEY_SHARE_MEM
{
	emKSMKEY_MEMPIPESYS,
	emKSMKEY_LOADER_SHAREMEM,
	emKSMKEY_MODULE_SETTING,
};
// -------------------------------------------------------------------------
// key共享内存
class KKeyShareMem
{
public:
	KKeyShareMem(void);
	static KKeyShareMem* Inst();
	// Called by GcGateway
	BOOL Init();
	// 一般由GcGateway在共享模块初始化时调用，不需要加锁
	LPVOID CreateMem(BYTE byKey, UINT uSize);
	// 获取已创建的共享内存，Loader调用
	LPVOID GetMem(BYTE byKey);
private:
	LPVOID ShareMemAlloc();
	LPVOID ShareMemGet();
private:
	struct KKeyShareData
	{
		KSHARE_MEM_PTR aShareMemPtr[256];
	};
	LPVOID m_pvBuffer;
};

// -------------------------------------------------------------------------

#endif /* __KKEYSHAREMEM_H__ */
