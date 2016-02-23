/* -------------------------------------------------------------------------
//	文件名		：	kgc_dbmanager.cpp
//	创建者		：	luobaoahang
//	创建时间	：	2007-3-6 11:02:58
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "mysql/mysql.h"
#include "Misc/utilities.h"
#include "serverbase/kdbcallback.h"
#include "serverbase/kgcconfiginterface.h"
#include "kdbmanager.h"
#include "kgcconfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#define  KD_MAX_QUERY_FIELD	10
// -------------------------------------------------------------------------
KDbManager::KDbManager()
{
	m_bRuningState = m_bInited = m_bStratup = FALSE; 
	m_bSafeQuit = TRUE;
	m_nDbAffairCounter = 0;
}

BOOL KDbManager::InitDBManager()
{
	KGCGW_PARAM& cParam = KGcConfig::Inst()->GetGcParam();
	QCONFIRM_RET_FALSE(!m_bInited);
	m_bInited = TRUE;
	m_bRuningState = TRUE;
	m_bSafeQuit = TRUE;

	QLogPrintf(LOG_INFO, "[DataBase]Connecting to DataBase Server(Ip=%s, Port=%d,UserName=%s)..."
		, cParam.sDbParam.szDBIP, cParam.sDbParam.nDBPort, cParam.sDbParam.szUserName);

	// ----------------- connect to database  ----------------- 
	INT nRetCode = m_cDatabase.DBConnect(
		cParam.sDbParam.szDBIP,
		cParam.sDbParam.nDBPort,
		cParam.sDbParam.szUserName,
		cParam.sDbParam.szPassword,
		NULL);
	QCONFIRM_RET_FALSE(nRetCode);
	LPCSTR szCurDatabase = cParam.sDbParam.szDatabase;

	QLogPrintf(LOG_INFO, "[DataBase]Creating(if not exists) DataBase(Name=%s)...", szCurDatabase);

	nRetCode = m_cDatabase.CreateDatabase(cParam.sDbParam.szDatabase, emKDBCREATE_IF_NOT_EXIST);
	QCONFIRM_RET_FALSE(nRetCode);
	QCONFIRM_RET_FALSE(szCurDatabase);
	nRetCode = m_cDatabase.SetCurrentDatabase(szCurDatabase);
	QCONFIRM_RET_FALSE(nRetCode);
#if defined _DEBUG && defined WIN32
	//Q_Printl("WAINING______________________ Now Drop DBShare Table");
	////用于调试时清数据（不用时注掉）
	//sqlStream.AddRemoveTable(KD_DBTBL_SHARE);
	//m_cDatabase.DoQuery(sqlStream);
	//sqlStream.Reset();
#endif

	QLogPrintf(LOG_INFO, "[DataBase]DbManager init OK!");

	return TRUE;
}

BOOL KDbManager::UnInit()
{
	if (!m_bInited)
		return TRUE;
	ReSetRuningState();

	// 要等待线程退出
	while (!m_bSafeQuit)
	{
		if (!MainThreadActive())
			QThread_Sleep(5);
	}
	m_cThreadLock.Lock();
	// 完成callback队列
	while (!m_queMainThreadCallback.empty())
	{
		KMAIN_CALLBACK sCallBack = m_queMainThreadCallback.front();
		m_queMainThreadCallback.pop();
		m_cThreadLock.Unlock();
		IKMainThreadCallback *piCallback = sCallBack.pCallBack;
		piCallback->MainThreadCallback(sCallBack.nParam, sCallBack.pParam);
		piCallback->Release();
		m_cThreadLock.Lock();
	}
	m_cThreadLock.Unlock();
	m_bInited = FALSE;
	//	m_Database.ShutDown(); UnInit后仍有可能存盘
	Q_Printl("Save Thread Exit...!");
	return TRUE;
}

BOOL KDbManager::StratUp()
{
	m_bStratup = TRUE;
	m_bSafeQuit = FALSE;
	m_cDbThread.Create(_DBRoute, this);
	return TRUE;
}

BOOL KDbManager::PushDBAffair( IKDBThreadCallback *pAffair, INT nParam /*= 0*/, LPVOID pParam /*= NULL*/, BOOL bPriority /*= FALSE*/ )
{
	++m_nDbAffairCounter;
	if (m_bSafeQuit || !m_bStratup)	// 如果数据库线程已退出
	{
		pAffair->DBThreadCallback(nParam, pParam);
		pAffair->Release();
		return TRUE;
	}
	KDB_CALLBACK sCallBack;
	sCallBack.pCallBack = pAffair;
	sCallBack.nParam = nParam;
	sCallBack.pParam = pParam;
	m_cThreadLock.Lock();
	if (bPriority)
		m_queDBThreadCallback.push_front(sCallBack);
	else
		m_queDBThreadCallback.push_back(sCallBack);
	m_cThreadLock.Unlock();
	return TRUE;
}

BOOL KDbManager::PushMainThreadCallBack( IKMainThreadCallback *pCallback, INT nParam /*= 0*/, LPVOID pParam /*= NULL*/ )
{
	// 若UnInit已调用，直接调
	if (!m_bInited)
	{
		pCallback->MainThreadCallback(nParam, pParam);
		pCallback->Release();
		return TRUE;
	}
	m_cThreadLock.Lock();
	KMAIN_CALLBACK sCallBack;
	sCallBack.pCallBack = pCallback;
	sCallBack.nParam = nParam;
	sCallBack.pParam = pParam;
	m_queMainThreadCallback.push(sCallBack);
	m_cThreadLock.Unlock();
	return TRUE;
}

BOOL KDbManager::MainThreadActive()
{
	m_cThreadLock.Lock();
	//注意return前要Unlock
	if (!m_queMainThreadCallback.empty())
	{
		KMAIN_CALLBACK sCallBack = m_queMainThreadCallback.front();
		m_queMainThreadCallback.pop();
		m_cThreadLock.Unlock();
		IKMainThreadCallback *piCallback = sCallBack.pCallBack;
		piCallback->MainThreadCallback(sCallBack.nParam, sCallBack.pParam);
		piCallback->Release();
		return TRUE;
	}
	m_cThreadLock.Unlock();
	return FALSE;
}

VOID KDbManager::DBRoute()
{
	while (GetRuningState())
	{
		if (!AffairLoop())
			QThread_Sleep(1);
	}
	//线程结束，继续完成存取操作
	while (AffairLoop())
	{
		continue;
	}
	//安全退出
	do 
	{
		m_bSafeQuit = TRUE;
	} while(!m_bSafeQuit);
}

VOID KDbManager::_DBRoute( LPVOID pvParam )
{
	reinterpret_cast<KDbManager *>(pvParam)->DBRoute();
}

BOOL KDbManager::AffairLoop()
{
	m_cThreadLock.Lock();
	//注意return前要Unlock
	if (!m_queDBThreadCallback.empty())
	{
		//每128个任务的界点输出调试信息
		if ((m_queDBThreadCallback.size() & 127) == 0)
			Q_Printl("DB Affair Count: " << m_queDBThreadCallback.size());
		KDB_CALLBACK sCallBack = m_queDBThreadCallback.front();
		IKDBThreadCallback *pCallback = sCallBack.pCallBack;
		m_queDBThreadCallback.pop_front();
		m_cThreadLock.Unlock();	//先Unlock再执行Callback是肯定的
		pCallback->DBThreadCallback(sCallBack.nParam, sCallBack.pParam);
		pCallback->Release();
		return TRUE;
	}
	m_cThreadLock.Unlock();
	return FALSE;
}

BOOL KDbManager::DBThreadLock()
{
	return m_cThreadLock.Lock();
}

BOOL KDbManager::DBThreadUnLock()
{
	return m_cThreadLock.Unlock();
}

KMySqlDatabase& KDbManager::GetDatabase()
{
	return m_cDatabase;
}

BOOL KDbManager::GetRuningState()
{
	return m_bRuningState;
}

VOID KDbManager::ReSetRuningState()
{
	while (m_bRuningState)
		m_bRuningState = FALSE;
}

KDbManager* KDbManager::Inst()
{
	static KDbManager s_Manager;
	return &s_Manager;
}
	