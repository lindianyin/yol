/* -------------------------------------------------------------------------
//	文件名		：	serverbase/kgcmoduleinterface.h
//	创建者		：	luobaohang
//	创建时间	：	2010/5/7 16:05:54
//	功能描述	：	gc模块接口定义
//
// -----------------------------------------------------------------------*/
#ifndef __SERVERBASE_KGCMODULEINTERFACE_H__
#define __SERVERBASE_KGCMODULEINTERFACE_H__

#include "serverbase/kgcbaseinterface.h"
#include "serverbase/kdbcallback.h"

struct IKGC_Module;
struct IKGcQuery;
// 获取IKGC_Module的函数名，该函数需导出
typedef IKGC_Module* (*LPFN_CREATE_MODULE_INST)(IKGcQuery* piQuery);
#define KD_CREATE_MODULE_INSTANCE_FUN "CreateModuleInstance"

// 该接口专用来Query其他所有接口，每个模块一个实例
// IKGcQuery由gcloader通过KD_CREATE_MODULE_INSTANCE_FUN函数传入模块中
struct IKGcQuery
{
	// 获取接口
	virtual BOOL QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void **ppvObject) = 0;
};

// {E167AC5F-B3CD-4801-B417-BF6314F09C78}

static const GUID IID_IKG_GCServer = 
{ 0xe167ac5f, 0xb3cd, 0x4801, { 0xb4, 0x17, 0xbf, 0x63, 0x14, 0xf0, 0x9c, 0x78 } };

// Gc端网络接口（发送到Gs），Query到的该接口发送议不会与其他模块冲突
// 每个模块一个实例
struct IKGcServer
{
	// nConnectId：Gameserver连接号，-1表示发送给所有
	virtual BOOL Send(INT nConnectId, LPVOID pvData, SIZE_T uDataSize) = 0;
	// send 2 random one
	virtual BOOL Send2Random(LPVOID pvData, UINT uDataSize) = 0;
	// 注册对Gameserver的协议处理
	virtual BOOL RegisterGsProcessor(IKProcessServer* pIServer) = 0;
	// 注册本模块开放的Gc间连接接口（每模块只开放一个），并指定协议处理接口，须在模块初始化时调用
	virtual IKInterfaceAcceptor* RegisterGcAcceptor(IKProcessAcceptor* pIServer) = 0;
	// 注册一个跟其他Gc模块的连接，须在模块初始化时调用
	virtual IKInterfaceConnector* RegisterGcConnector(BYTE byModuleId,
													IKProcessConnector* pIClient,
													BOOL bIsEssencial) = 0;
};

// {045F4391-1756-4a4c-82C4-A27D8E972EF6}
static  CONST GUID IID_IKG_GCClient = 
{ 0x45f4391, 0x1756, 0x4a4c, { 0x82, 0xc4, 0xa2, 0x7d, 0x8e, 0x97, 0x2e, 0xf6 } };

// Gs端网络接口（Gs->Gc）
struct IKGcClient
{
	// 发送协议到Gc
	virtual BOOL Send(LPVOID pvData, UINT uDataSize) = 0;
	// 注册对Gc的协议处理
	virtual BOOL RegisterProcessor(IKProcessConnector* pIServer) = 0;
};

// 数据库接口
// {FDF5EC0C-BFEF-436e-9942-576D7BFBB393}
static const GUID IID_IKG_GCDatabase = 
{ 0xfdf5ec0c, 0xbfef, 0x436e, { 0x99, 0x42, 0x57, 0x6d, 0x7b, 0xfb, 0xb3, 0x93 } };
// 见serverbase/kdbcallback.h
struct IKGcDatabase;

// 日志接口
// {A95AE7B3-7EBE-4013-81E2-0194C3553D86}
static const GUID IID_IKG_GCLog = 
{ 0xa95ae7b3, 0x7ebe, 0x4013, { 0x81, 0xe2, 0x1, 0x94, 0xc3, 0x55, 0x3d, 0x86 } };

struct IKGcLog
{
	// ...
};

// -------------------------------------------------------------------------
struct IKGC_Module
{
	// 是否激每循环调用
	// virtual BOOL IsLoopActive() PURE;
	// 是否每帧调用
	// virtual BOOL IsBreathActive() PURE;
	// 获取模块名字
	virtual LPCSTR GetModuleName() PURE;
	// 每循环调用
	virtual BOOL Loop() PURE;
	// 每帧调用
	virtual BOOL Breath() PURE;
	//  初始化调用
	virtual BOOL OnInit() PURE;
	// 初始化完毕进入运行状态时调用
	virtual BOOL OnStartUp() PURE;
	// 反初始化调用
	virtual BOOL OnUninit() PURE;
};


#define GCMODULE_API 

// -------------------------------------------------------------------------

#endif /* __SERVERBASE_KGCMODULEINTERFACE_H__ */
