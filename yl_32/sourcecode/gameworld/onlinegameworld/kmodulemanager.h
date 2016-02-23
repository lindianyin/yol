
#pragma once

class IKModuleInterface;
// -------------------------------------------------------------------------
class KModuleManager
{
public:
	// 前初始化，在引擎的初始化之前调用
	BOOL PreInit();
	// 初始化
	BOOL Init();
	BOOL Breath();
	BOOL UnInit();
	IKModuleInterface* GetModule(BYTE btModuleId) { return m_anModuleInterface[btModuleId]; }
	IKModuleInterface* GetGcModule(BYTE btGcModuleId) { return m_anGcModuleInterface[btGcModuleId]; }
private:
	typedef std::vector<IKModuleInterface* >::iterator ITPModule;
	std::vector<IKModuleInterface* > m_vecPiModule;
	IKModuleInterface* m_anModuleInterface[256];
	IKModuleInterface* m_anGcModuleInterface[256];	
};

extern KModuleManager g_cModuleManager;

