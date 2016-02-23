

#include "stdafx.h"
#include "ksocketinterface.h"
#include "onlinegameworld/kgcmodulebase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef GAME_SERVER
class KGcSvcClient : public IKGcSvcClient
{
public:
	KGcSvcClient(BYTE byModuleId) : m_byGcModule(byModuleId) {}
	virtual BOOL SendData(LPCVOID pData, UINT uSize);
	virtual BOOL SendData(INT nConnectId, LPCVOID pData, UINT uSize);
private:
	BYTE m_byGcModule;
};

BOOL KGcSvcClient::SendData( LPCVOID pData, UINT uSize )
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE *)pData;
	QCONFIRM_RET_FALSE(uSize >= sizeof(KPTC_HEADER_BASE));
	pHeader->byProtocolFamily = m_byGcModule;
	//return g_piGcConnect->Send((INT)m_byGcModule, 0, pData, (INT)uSize);
	return 0;
}

BOOL KGcSvcClient::SendData(INT nConnectId, LPCVOID pData, UINT uSize)
{
	//return g_piGcConnect->Send(nConnectId, 0, pData, (INT)uSize);
	return 0;
}
#endif // GAME_SERVER

// -------------------------------------------------------------------------

KGcModuleBase::KGcModuleBase( CHAR cModuleId, BYTE byGcModuleId ) :
	KModuleBase(cModuleId), m_byGcModuleId(byGcModuleId)
{
#ifdef GAME_SERVER
	m_piGcSvcClient = new KGcSvcClient(m_byGcModuleId);
#endif // GAME_SERVER
}

KGcModuleBase::~KGcModuleBase()
{
#ifdef GAME_SERVER
	delete m_piGcSvcClient;
#endif // GAME_SERVER
}
// -------------------------------------------------------------------------

