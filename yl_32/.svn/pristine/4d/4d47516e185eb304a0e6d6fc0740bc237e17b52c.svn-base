/* -------------------------------------------------------------------------
//	文件名		：	kworker.cpp
//	创建者		：	simon
//	创建时间	：	2010/5/20 15:28:48
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "ikdbworker.h"
#include "kdbclusterworker.h"
#include "kg_nameserver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

IKDbWorker* g_CreateWorker(KEWORKER_CLASS eWorkerClass)
{
	IKDbWorker* pWorker = NULL;
	switch (eWorkerClass)
	{
	case emKWORKER_CLASS_DB_NODE:
		pWorker = KDbClusterWorker::CreateInstance();
		break;
	case emKWORKER_CLASS_NAMESERVER:
		pWorker = KG_NameServer::CreateInstance();
		break;
	default:
		Q_Error("Invalid Worker Class " << eWorkerClass);
		break;
	}
	return pWorker;
}

// -------------------------------------------------------------------------
