/* -------------------------------------------------------------------------
//	文件名		：	serverbase/kgclogicbaseinterface.h
//	创建者		：	luobaohang
//	创建时间	：	2010/7/13 15:49:02
//	功能描述	：	gc游戏逻辑层接口定义
//
// 逻辑层为供Loader所链接的一个游戏逻辑相关的lib，为所有gc模块提供基础游戏逻辑，gc模块
//	要访问逻辑层功能时，同样需用Query接口的方式
// -----------------------------------------------------------------------*/
#ifndef __SERVERBASE_KGCLOGICBASEINTERFACE_H__
#define __SERVERBASE_KGCLOGICBASEINTERFACE_H__

#include "serverbase/kgcbaseinterface.h"

// -------------------------------------------------------------------------
// Loader提供给逻辑层的接口
struct IKGcLogicProvide
{
	// 注册逻辑层扩展接口（提供给gc模块Query调用）
	virtual BOOL RegisterExtInterface(REFIID rrid, LPVOID pvObject) PURE;
	virtual BOOL RegisterGsProcessor(IKProcessServer* pIServer) PURE;
	// nConnectId：Gameserver连接号，-1表示发送给所有
	virtual BOOL Send(INT nConnectId, LPVOID pvData, UINT uDataSize) PURE;
	virtual BOOL Send2OtherGc(INT nModuleId, LPVOID pData, UINT uDataSize) PURE;
};

// Loader对逻辑层的需求接口
struct IKGcLogicRequire
{
	// 每帧调用
	virtual BOOL Breath() PURE;
	//  初始化调用，该调用会在Loader加载模块之前
	virtual BOOL OnInit(void* pGcDataBase) PURE;
	// 初始化完毕进入运行状态时调用
	virtual BOOL OnStartUp() PURE;
	// 反初始化调用
	virtual BOOL OnUninit() PURE;
};

// 该函数在逻辑层实现，由Loader调用，Loader与逻辑层间互相提供接口访问
extern IKGcLogicRequire* g_ResgisterGcLogic(IKGcLogicProvide* piGcLogicRequire);
// -------------------------------------------------------------------------

#endif /* __SERVERBASE_KGCLOGICBASEINTERFACE_H__ */
