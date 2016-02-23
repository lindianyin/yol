/* -------------------------------------------------------------------------
//	文件名		：	kbcallback.h
//	创建者		：	luobaohang
//	创建时间	：	2007-5-16 9:58:48
//	功能描述	：	数据库事务回调接口
//
// -----------------------------------------------------------------------*/
#ifndef __KGC_DBCALLBACK_H__
#define __KGC_DBCALLBACK_H__

#include "Misc/utilities.h"
#include "Misc/objectpool.h"
#include "onlinegamebase/mysqldatabase.h"

// -------------------------------------------------------------------------
class IKDBThreadCallback;
class IKMainThreadCallback;
struct IKGcDatabase
{
	// 加入数据库事务
	virtual BOOL PushDBAffair(IKDBThreadCallback *pAffair, INT nParam = 0,
		LPVOID pParam = NULL, BOOL bPriority = FALSE) = 0;
	// 数据库事务的主线程结果回调
	virtual BOOL PushMainThreadCallBack(IKMainThreadCallback *pCallback,
		INT nParam = 0, LPVOID pParam = NULL) = 0;
	// 允许外部自执行数据库存取查询事务（通过IKDBThreadCallback）
	virtual KMySqlDatabase& GetDatabase()  = 0;
	// 线程锁解决数据库线程与主线程数据访问冲突
	virtual BOOL DBThreadLock() = 0;
	virtual BOOL DBThreadUnLock() = 0;
};

class KDBBaseCallback
{
public:
	INT m_nObjectId;	//用于存储回调对象ID以便释放
};

//数据库线程的事务回调接口，用于主线程中push事务到数据库线程回调队列中调用
//注意所有的数据库操作必须在数据库线程进行，线程锁必须使用IKGcDatabase::DBThreadLock()，不要另外定义锁
class IKDBThreadCallback
{
public:
	//释放这个回调对象，主线程和数据库线程都可能调用
	virtual VOID Release() {};
	//数据库线程调用
	virtual BOOL DBThreadCallback(INT nParam, LPVOID pParam) = 0;
};
//创建回调对象类时从该类继承
class KDBThreadCallback : public IKDBThreadCallback, public KDBBaseCallback
{
};

//主线程回调接口，用于数据库线程中push事务到主线程回调队列中调用
class IKMainThreadCallback
{
public:
	//释放这个回调对象，主线程和数据库线程都可能调用
	virtual VOID Release() {};
	//主线程调用
	virtual BOOL MainThreadCallback(INT nParam, LPVOID pParam) = 0;
};
//创建回调对象类时从该类继承
class KMainThreadCallback : public IKMainThreadCallback, public KDBBaseCallback
{
};

//用于数据库线程查询完毕，在主线程处理查询结果
class KDBQueryRSCallback : public KMainThreadCallback
{
public:
	//在派生类的Release中必须调用本类的Release
	virtual VOID Release() { m_QueryResult.Release(); }
	virtual BOOL MainThreadCallback(INT nParam, LPVOID pParam) = 0;
	KMySqlResult m_QueryResult;
};	

//数据库回调对象生成器，用于回调对象生成和释放
//ICLASST：实际的回调对象类，一般从KDBThreadCallback和KMainThreadCallback继承
template<class ICLASST, INT TN_BASESIZE = 64, INT TN_INCSIZE = TN_BASESIZE>
class KDBCallbackClassGentor
{
public:
	KDBCallbackClassGentor(){ m_DBCBObjGenerator.Init(TN_BASESIZE, TN_INCSIZE); }
	//Call this to create new callback object
	static ICLASST *CreateNew(IKGcDatabase* pDatabase)
	{
		INT nObjectID;
		pDatabase->DBThreadLock();
		ICLASST * pNewObject = s_Self.m_DBCBObjGenerator.New(&nObjectID);
		pDatabase->DBThreadUnLock();
		if (!pNewObject)
		{
			ASSERT(FALSE);
			return NULL;
		}
		//ICLASST Must be Drived From KDBBaseCallback
		static_cast<KDBBaseCallback *>(pNewObject)->m_nObjectId = nObjectID;
		return pNewObject;
	}
	//Call this in callback object's release() function
	static VOID Free(IKGcDatabase* pDatabase, ICLASST *piClass)
	{
		pDatabase->DBThreadLock();
		s_Self.m_DBCBObjGenerator.Free(static_cast<KDBBaseCallback *>(piClass)->m_nObjectId);
		pDatabase->DBThreadUnLock();
	}
	~KDBCallbackClassGentor()
	{
		if (m_DBCBObjGenerator.GetPeakValue() != m_DBCBObjGenerator.GetFreeCount())
			Q_Error(m_DBCBObjGenerator.GetPeakValue() << " " << m_DBCBObjGenerator.GetFreeCount());
	}
private:
	KIncObjPool2<ICLASST> m_DBCBObjGenerator;
	static KDBCallbackClassGentor<ICLASST, TN_BASESIZE, TN_INCSIZE> s_Self;
};
template<class ICLASST, INT TN_BASESIZE, INT TN_INCSIZE> 
	KDBCallbackClassGentor<ICLASST, TN_BASESIZE, TN_INCSIZE> 
		KDBCallbackClassGentor<ICLASST, TN_BASESIZE, TN_INCSIZE>::s_Self;
// -------------------------------------------------------------------------
#endif /* __KGC_DBCALLBACK_H__ */
