/* -------------------------------------------------------------------------
//	文件名		：	KRoleSvc.h
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:24:41
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KROLESVC_H__
#define __KROLESVC_H__

// -------------------------------------------------------------------------

struct KROLESVC_PARAM;

class KRoleServer
{
public:
	KRoleServer(void);
	~KRoleServer(void);
	BOOL Start(void);

	BOOL Init();
	BOOL Uninit();
	BOOL LoadConfig();
	//INT TestCase_ThroughputCapacity(void);
	//INT TestCase_Functionality(void);
	BOOL Shutdown() { m_bRunning = FALSE; return TRUE; }
private:
	BOOL m_bRunning;
};

extern KRoleServer g_cRoleSvc;

// -------------------------------------------------------------------------

#endif /* __KROLESVC_H__ */
