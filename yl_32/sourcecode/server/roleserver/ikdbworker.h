/* -------------------------------------------------------------------------
//	文件名		：	ikdbworker.h
//	创建者		：	simon
//	创建时间	：	2010/5/20 15:47:32
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __IKDBWORKER_H__
#define __IKDBWORKER_H__

/************************************************************************/
/*                                  包处理流程                                     */
/**********************************************************************/
/*
网络接收（IMemBlock）
↓
GatewayProcess（创建PackageId, CallerArg及pAdditional）
↓
Manager寻找对应的工作组(Workgroup)和工作线程
↓
工作线程处理（创建IMemBlock，CallerArg和PackageId原封不动）
↓
Manager收集返回结果
↓
GatewayProcess处理返回结果
↓
网络发送（IMemBlock）
*/

// -------------------------------------------------------------------------

struct KProcessingPackage
{
	KProcessingPackage()
	{ u64CallerArg = 0; pData = NULL; pAdditional = NULL; }

	UINT64 u64CallerArg;
	IMemBlock* pData; // 请求或处理结果，都是网络包（避免多次内存拷贝）
	IMemBlock* pAdditional; // 传给Worker的额外数据或Worker返回的额外数据
};

typedef std::list<KProcessingPackage> KProcessingPackageList;

// -------------------------------------------------------------------------

class string;
struct KROLESVC_DB_PARAM;

enum KEWORKER_CLASS
{
	emKWORKER_CLASS_NONE,

	emKWORKER_CLASS_DB_NODE,
	emKWORKER_CLASS_NAMESERVER,

	emKWORKER_CLASS_ALL
};

class IKDbWorker
{
public:
	virtual BOOL Init(const KROLESVC_DB_PARAM& rcDatabaseCfg, const std::string& strName) PURE;
	virtual BOOL Uninit() PURE;

	virtual BOOL TestWork(const KProcessingPackage& rPacks) PURE; // 预先测试可否接受此任务
	//virtual INT QueryPendingWork() PURE; // 检查负载状况，用于动态均衡负载

	virtual BOOL PushWork(const KProcessingPackage& rPacks) PURE;
	virtual BOOL GetResult(KProcessingPackageList& rPacks) PURE;

	virtual LPCSTR GetName() PURE;
	virtual BOOL GetStatics(INT& rnJobTotal, INT& rnJobPending, INT& rnJobOk,
		INT& rnBytesRecv, INT& rnBytesSend) PURE;
};

extern IKDbWorker* g_CreateWorker(KEWORKER_CLASS eWorkerClass);

// -------------------------------------------------------------------------

#endif /* __IKDBWORKER_H__ */
