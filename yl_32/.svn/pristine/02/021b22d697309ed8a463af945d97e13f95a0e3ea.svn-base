/* -------------------------------------------------------------------------
//	文件名		：	kgcconfig.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/6/21 10:51:17
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "Misc/PassGen.h"
#include "kgcconfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KGcConfig::KGcConfig()
{
	//ZeroStruct(m_cDbParam);
	ZeroStruct(m_cGcParam);
}

//DB_SERVER_PARAM& KGcConfig::DbServerParam()
//{
//	return m_cDbParam;
//}


BOOL KGcConfig::LoadConfig()
{
	Q_Printl("Loading Config...");

	QIniFile iniFile;
	QCONFIRM_RET_FALSE(iniFile.Load(KDF_GC_CONFIG));
	BOOL bResult = FALSE;
	CHAR szPassword[64] = "";
	do
	{
		if (!iniFile.GetString("DatabaseServer", "Server", "swgoddess", m_cGcParam.sDbParam.szDBIP, sizeof(m_cGcParam.sDbParam.szDBIP)))
			break;
		if (!iniFile.GetInteger("DatabaseServer", "Port", 3306, &m_cGcParam.sDbParam.nDBPort))
			break;
		if (!iniFile.GetString("DatabaseServer", "UserName", "", m_cGcParam.sDbParam.szUserName, sizeof(m_cGcParam.sDbParam.szUserName)))
			break;
		if (!iniFile.GetString("DatabaseServer", "Password", "", szPassword, sizeof(szPassword)) ||
			!SimplyDecryptPassword(m_cGcParam.sDbParam.szPassword, szPassword))
			break;
		if (!iniFile.GetString("DatabaseServer", "Database", "kg_jxex_gamecenter",
			m_cGcParam.sDbParam.szDatabase, sizeof(m_cGcParam.sDbParam.szDatabase)))
			break;

		//if (!iniFile.GetString("GcMonitor", "Ip", "", m_cGcParam.szMonitorIp, sizeof(m_cGcParam.szMonitorIp)))
		//	break;
		//if (!iniFile.GetInteger("GcMonitor", "Port", 5106, &m_cGcParam.nMonitorPort))
		//	break;

		bResult = TRUE;
	}
	while (0);
	if (!bResult)
		Q_Printl("!!!!!!!!!!!!!!\nRead " << KDF_GC_CONFIG << " Error, Check It!");
	return bResult;
}

KGcConfig* KGcConfig::Inst()
{
	static KGcConfig sConfig;
	return &sConfig;
}

KGCGW_PARAM& KGcConfig::GetGcParam()
{
	return m_cGcParam;
}
// -------------------------------------------------------------------------
