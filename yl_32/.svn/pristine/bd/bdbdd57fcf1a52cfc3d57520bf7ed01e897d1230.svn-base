
#pragma once

#include <queue>
#include "onlinegamebase/mysqldatabase.h"
#include "serverbase/kdbcallback.h"

enum KE_OPER_TYPE
{
	emKOPER_ADD_DATA,	//增加一条数据
	emKOPER_DEL_DATA,	//删除一条数据
	emKOPER_MODIFY_DATA, //修改数据
	emKOPER_DEL_TABLE,	//删除一个表（逻辑表，非数据库表）
};

// -------------------------------------------------------------------------
class IKDBThreadCallback;
class IKMainThreadCallback;
class KDbManager : public IKGcDatabase
{
	struct KDB_CALLBACK
	{
		IKDBThreadCallback*		pCallBack;
		LPVOID					pParam;
		INT						nParam;
	};
	struct KMAIN_CALLBACK
	{
		IKMainThreadCallback*	pCallBack;
		LPVOID					pParam;
		INT						nParam;
	};
	//用Queue，保证先push的先处理
	typedef std::deque<KDB_CALLBACK>	QUE_DBTHREAD_CB;
	typedef std::queue<KMAIN_CALLBACK>	QUE_MAINTHREAD_CB;
public:
	KDbManager();
	static KDbManager* Inst();
	virtual BOOL PushDBAffair(IKDBThreadCallback *pAffair, INT nParam = 0, LPVOID pParam = NULL, BOOL bPriority = FALSE);
	//数据库事务的主线程结果回调
	virtual BOOL PushMainThreadCallBack(IKMainThreadCallback *pCallback, INT nParam = 0, LPVOID pParam = NULL);
	virtual KMySqlDatabase& GetDatabase();
	// 线程锁解决数据库线程与主线程数据访问冲突
	virtual BOOL DBThreadLock();
	virtual BOOL DBThreadUnLock();

	BOOL	InitDBManager();
	BOOL	UnInit();
	//数据库线程启动
	BOOL	StratUp();
	//加入数据库事务
	BOOL	MainThreadActive();	//主线程Active
	VOID	ReSetRuningState();
private:
	BOOL	GetRuningState();
	//Called by Thread (_DBRoute)
	VOID	DBRoute();
	//Called by Thread (_DBRoute)
	BOOL	AffairLoop();
	//存取线程
	static VOID _DBRoute(LPVOID pvParam);
private:
	INT m_nDbAffairCounter;
	BOOL m_bRuningState;
	BOOL m_bSafeQuit;	//是否能安全退出（数据已存盘完毕）
	BOOL m_bStratup;
	QLock m_cThreadLock;	//两线程间尽量不要用多个锁，否则容易互锁
	BOOL m_bInited;
	QThread	m_cDbThread;
	KMySqlDatabase m_cDatabase;
	QUE_MAINTHREAD_CB m_queMainThreadCallback; // 主线程事务回调队列
	QUE_DBTHREAD_CB	m_queDBThreadCallback; // 数据库线线程事务队列
};
// -------------------------------------------------------------------------


