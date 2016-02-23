/* -------------------------------------------------------------------------
//	文件名		：	kloader.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/12 15:03:59
//	功能描述	：	Loader实现gcgateway的子进程逻辑
//
// -----------------------------------------------------------------------*/
#ifndef __KLOADER_H__
#define __KLOADER_H__

#include "serverbase/kgcmoduleinterface.h"
#include "kgcinterface.h"
// -------------------------------------------------------------------------
class KLoader
{
public:
	KLoader(void);
	~KLoader(void);
	static KLoader* Inst();

	BOOL Init(INT nLoaderId);
	BOOL Uninit();
	VOID GameLoop();
	INT InitScript();

	// 注册一个Gc间通迅的接收端，在模块初始化时调用
	IKInterfaceAcceptor* RegisterAcceptor(BYTE byModuleId, IKProcessAcceptor* pIServer);
	// 注册一个Gc间通迅接入端，在模块初始时调用
	IKInterfaceConnector* RegisterConnector(
		BYTE byConnectorModuleId,
		BYTE byAcceptorModuleId,
		IKProcessConnector* pIClient,
		BOOL bIsEssencial);

	BOOL ProcessInterfaceData(BYTE byConnectId, BYTE byAcceptorModuleId, LPBYTE pbData, UINT uDataSize);
private:
	// Acceptor的Processor协议处理调用
	BOOL ProcessAcceptorData(BYTE byConnectorModuleId, BYTE byAcceptorModuleId, LPBYTE pbData, UINT uDataSize);
	// Connector的Processor协议处理调用
	BOOL ProcessConnectorData(BYTE byConnectorModuleId, BYTE byAcceptorModuleId, LPBYTE pbData, UINT uDataSize);
	// Pipe数据处理
	BOOL ProcessPipeData(LPVOID pvData, UINT uDataSize);
	BOOL LoadModules();
	// 创建Gc间虚连接
	BOOL CreateGcInterfaces();
private:
	typedef std::list<KInterfaceAcceptor> LST_ACCEPTOR;
	typedef std::list<KInterfaceConnector> LST_CONNECTOR;
	// AcceptModuleId对应的IKProcessAcceptor
	typedef std::map<BYTE, IKProcessAcceptor*> MP_PROCESS_ACCEPTOR;
	// ConnectorModuleId | AcceptorModuleId对应的IKProcessConnector
	typedef std::map<WORD, IKProcessConnector*> MP_PROCESS_CONNECTOR;
	// 模块列表
	typedef std::vector<IKGC_Module*> VEC_MODULE_LIST;
	LST_ACCEPTOR m_lstAcceptor;
	LST_CONNECTOR m_lstConnetor;
	MP_PROCESS_ACCEPTOR m_mpProcessAcceptor;
	MP_PROCESS_CONNECTOR m_mpProcessConnector;
	VEC_MODULE_LIST m_vecModuleList;
	UINT m_nCurElapse;
	INT m_nLoaderId;
};

// -------------------------------------------------------------------------

#endif /* __KLOADER_H__ */
