/* -------------------------------------------------------------------------
//	文件名		：	kmanager_i.h
//	创建者		：	zhangfan
//	创建时间	：	2007-7-3 10:10:08
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#ifndef __KMANAGER_I_H__
#define __KMANAGER_I_H__

// -------------------------------------------------------------------------

class KPlayer;

class IKManager
{
public:
	// 定义一个名字，用于调试
	virtual LPCSTR GetName() CONST	{ return ""; };

	// 初始化，返回BOOL表示是否基本成功（允许继续启动）
	virtual BOOL Init()		{ return TRUE; }

	// 后续初始化，资源准备好后初始化(比如服务端任务系统要等待地图加载后才能初始化)
	virtual BOOL PostInit()	{ return TRUE; }

	// 每帧呼吸，返回BOOL表示是否基本成功（允许继续运行）
	virtual BOOL Breathe()	{ return TRUE; }
	
	// 释放该模块，返回是否成功
	virtual BOOL Release()	{ return TRUE; }

	// 将此模块相关的数据重新加载，用于调试
	virtual BOOL Reload()	{ return TRUE; }

	// 注册协议处理接口，ppHandlers是一个长度为256的IKManager指针数组
	virtual VOID RegisterProtocol(IKManager** ppHandlers) {}

#ifdef _SERVER
	// GS/Client之间处理协议，返回协议是否正确（出错时踢玩家下线）
	virtual BOOL ProtocolProcess(LPBYTE pData, INT nDataLen, KPlayer& rcPlayer)	{ return TRUE; }
#else
	virtual BOOL ProtocolProcess(LPBYTE pData, INT nDataLen)					{ return TRUE; }
#endif
};

// -------------------------------------------------------------------------

#endif /* __KMANAGER_I_H__ */
