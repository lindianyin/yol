/* -------------------------------------------------------------------------
//	文件名		：	rolesvc_cfg.cpp
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:27:15
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "roleserver_cfg.h"
#include "Misc/PassGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KROLESVC_PARAM g_sRoleSvcCfg;


// -------------------------------------------------------------------------

BOOL g_RoleSvcReadCfg(KROLESVC_PARAM& rcParam)
{
	BOOL bOk = FALSE;
	QIniFile	iniFile;
	CHAR szPwd[ROLESVC_CFG_STRING_MAX_LEN] = "";

	ZeroMemory(&rcParam, sizeof(rcParam));
	{
		bOk = iniFile.Load(ROLESVC_CFG_FILE_NAME);
		PROCESS_ERROR(bOk);

		// limit
		iniFile.GetInteger(ROLESVC_CFG_SECTION_LIMIT, "MaxRolePerAccount", 1, &rcParam.nMaxRolePerAccount);
		iniFile.GetInteger(ROLESVC_CFG_SECTION_LIMIT, "MaxRoleListBuffer",
			ROLESVC_DEFAULT_MAX_ROLE_LIST_SIZE, &rcParam.nMaxRoleListBuffer);
		iniFile.GetInteger(ROLESVC_CFG_SECTION_LIMIT, "MaxAccountsPerDB",
			ROLESVC_DEFAULT_MAX_ACCOUNTS_PER_DB, &rcParam.nMaxAccountPerDB);

		// name server
		iniFile.GetString(ROLESVC_CFG_SECTION_NAMESERVER, "Server", "", rcParam.sNameServerParam.szServer,
			sizeof(rcParam.sNameServerParam.szServer));
		iniFile.GetInteger(ROLESVC_CFG_SECTION_NAMESERVER, "Port", ROLESVC_CFG_DEFAULT_DB_PORT,
			&rcParam.sNameServerParam.nPort);
		iniFile.GetString(ROLESVC_CFG_SECTION_NAMESERVER, "User", "", rcParam.sNameServerParam.szUserName,
			sizeof(rcParam.sNameServerParam.szUserName));
		iniFile.GetString(ROLESVC_CFG_SECTION_NAMESERVER, "Password", "", szPwd, sizeof(szPwd));

		bOk = SimplyDecryptPassword(rcParam.sNameServerParam.szPassword, szPwd); // decrypt password
		LOG_PROCESS_ERROR(bOk && "incorrect password");

		iniFile.GetString(ROLESVC_CFG_SECTION_NAMESERVER, "Database", "", rcParam.sNameServerParam.szDatabase,
			sizeof(rcParam.sNameServerParam.szDatabase));
		iniFile.GetInteger(ROLESVC_CFG_SECTION_NAMESERVER, "Thread", 1, &rcParam.sNameServerParam.nThread);

		// database cluster
		iniFile.GetInteger(ROLESVC_CFG_SECTION_DB_CLUSTER, "NodeCount", 0, &rcParam.nDatabaseNodeCount);
		iniFile.GetString(ROLESVC_CFG_SECTION_DB_CLUSTER, "User", "", rcParam.szDbClusterUser,
			sizeof(rcParam.szDbClusterUser));
		iniFile.GetString(ROLESVC_CFG_SECTION_DB_CLUSTER, "Password", "", szPwd, sizeof(szPwd));

		bOk = SimplyDecryptPassword(rcParam.szDbClusterPassword, szPwd); // decrypt password
		LOG_PROCESS_ERROR(bOk && "incorrect password");
	}

	bOk = TRUE;
EXIT0:
	return bOk;
}

BOOL g_RoleSvcReadClusterCfg( KROLESVC_PARAM& rcParam )
{
	BOOL bOk = FALSE;
	QIniFile	iniFile;
	INT nDbNodeCount = 0;
	KROLESVC_DB_PARAM* pDbNodeParams = rcParam.pDbNodeParams;
	CHAR szSectionName[ROLESVC_CFG_STRING_MAX_LEN] = "";

	PROCESS_ERROR(pDbNodeParams);
	nDbNodeCount = rcParam.nDatabaseNodeCount;
	PROCESS_ERROR(nDbNodeCount);

	bOk = iniFile.Load(ROLESVC_CFG_FILE_NAME);
	PROCESS_ERROR(bOk);

	ZeroMemory(pDbNodeParams, sizeof(KROLESVC_DB_PARAM) * nDbNodeCount);

	for (INT i = 0; i < nDbNodeCount; ++i)
	{
		snprintf(szSectionName, ROLESVC_CFG_STRING_MAX_LEN, "%s%d", ROLESVC_CFG_SECTION_DB_NODE, i + 1);
		// name server
		iniFile.GetString(szSectionName, "Server", "", pDbNodeParams[i].szServer, sizeof(pDbNodeParams[i].szServer));
		iniFile.GetInteger(szSectionName, "Port", ROLESVC_CFG_DEFAULT_DB_PORT, &pDbNodeParams[i].nPort);
		iniFile.GetString(szSectionName, "Database", "", pDbNodeParams[i].szDatabase, sizeof(pDbNodeParams[i].szDatabase));
		iniFile.GetInteger(szSectionName, "Thread", 1, &pDbNodeParams[i].nThread);

		QStrCpyLen(pDbNodeParams[i].szUserName,
			rcParam.szDbClusterUser,
			sizeof(pDbNodeParams[i].szUserName));
		QStrCpyLen(pDbNodeParams[i].szPassword,
			rcParam.szDbClusterPassword,
			sizeof(pDbNodeParams[i].szPassword));
	}

	bOk = TRUE;
EXIT0:
	return bOk;
}

INT g_GetLocalTimeString(CHAR szTime[], SIZE_T uBufferSize)
{
	int nRetCode   = 0;
	tm LocalTime   = {0};
	time_t curtime = 0;

	ASSERT(uBufferSize >= sizeof("2005-01-01 22:22:22"));
	ASSERT(szTime);

	time(&curtime);  // get current time
	localtime_r(&curtime, &LocalTime);
	nRetCode = strftime(szTime, uBufferSize, "%Y-%m-%d %H:%M:%S", &LocalTime);
	ASSERT(nRetCode);

	return TRUE;
}

size_t g_uAllocCount = 0;
size_t g_uFreeCount = 0;
size_t g_uAllocSize = 0;
size_t g_uFreeSize = 0;


class KMemory2 : public IMemBlock
{
public:
	KMemory2(size_t uSize)
	{
		m_pData = new char[uSize + 8]; m_uSize = uSize; m_lRefCount = 1; m_uOrignSize = m_uSize;

		//KG_InterlockedIncrement(&g_uAllocCount);
		//KG_InterlockedExchangeAdd(&g_uAllocSize, uSize);
		// 注意不加锁输出有竞态
		//printf("\r%u/%u        %u/%u            ", g_uAllocCount, g_uFreeCount, g_uAllocSize, g_uFreeSize);
	}
	virtual void* GetData() { return m_pData + 8; }
	virtual unsigned GetSize() { return m_uSize; }
	virtual unsigned GetReserveSize() { return 8; }

	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void **ppvObject
		);
private:
	char* m_pData;
	size_t m_uSize;
	volatile long   m_lRefCount;
	unsigned        m_uOrignSize;
};

IMemBlock* KAlloc2(unsigned uSize) { return new KMemory2(uSize); }

ULONG STDMETHODCALLTYPE KMemory2::AddRef(void)
{   
	return InterlockedIncrement((long *)&m_lRefCount);
};

ULONG STDMETHODCALLTYPE KMemory2::Release(void)
{
	int nRetCode = false;
	long lRefNow = 0;

	lRefNow = InterlockedDecrement((long *)&m_lRefCount);
	if (lRefNow > 0)
		return lRefNow;

	if (lRefNow < 0)
	{
		ASSERT(!"Unexpected memory release!");
		QLogPrintf(LOG_ERR, "%s", "Bad use of memory block : Unexpected memory release!");
	}

	//KG_InterlockedIncrement(&g_uFreeCount);
	//KG_InterlockedExchangeAdd(&g_uFreeSize, m_uSize);
	// 注意不加锁输出有竞态
	//printf("\r%u/%u        %u/%u            ", g_uAllocCount, g_uFreeCount, g_uAllocSize, g_uFreeSize);

	SAFE_DELETE_ARRAY(m_pData);
	delete this;

	return lRefNow;
}

HRESULT STDMETHODCALLTYPE KMemory2::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void **ppvObject
	) 
{ 
	HRESULT hrResult = E_FAIL;

	assert(ppvObject);

	*ppvObject = NULL;

	/*if (riid == IID_IKG_Buffer_ReSize)
	{
		*ppvObject = (void *)((IKG_Buffer_ReSize *)this);
	}
	else*/ if (riid == IID_IMemBlock)
	{
		*ppvObject = (void *)((IMemBlock *)this);
	}
	else if (riid == IID_IUnknown)
	{
		*ppvObject = (void *)((IMemBlock *)this);
	}
	else
	{
		COM_PROC_ERR_RET_ERR(E_NOINTERFACE);
	}

	((IUnknown *)(*ppvObject))->AddRef();

	hrResult = S_OK;
EXIT0:
	return hrResult;
}
