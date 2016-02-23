
#include "CorePrivate.h"
#include "Misc/qidpool.h"
#include "Misc/utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

QIdPool::QIdPool() : m_pnFreeId(NULL), m_nFreeCount(0), m_nMaxNum(0)
{
}

QIdPool::~QIdPool()
{
	Uninit();
}

VOID QIdPool::Init( INT nMaxNum )
{
	m_pnFreeId = new INT[nMaxNum];
	m_nFreeCount = nMaxNum;
	for (INT i = 0; i < nMaxNum; i++)
	{
		m_pnFreeId[i] = i + 1;
	}
	m_cUsedArray.Init(nMaxNum + 1);
	m_nMaxNum = nMaxNum;
}

INT QIdPool::New()
{
	if (m_nFreeCount <= 0)
		return 0;
	INT nId = m_pnFreeId[--m_nFreeCount];
	//QCONFIRM_RET_NULL(m_cUsedArray.Insert(nId));
	m_cUsedArray.Insert(nId);
	return nId;
}

BOOL QIdPool::Delete( INT nId )
{
	//if (!m_cUsedArray.Remove(nId))
	//	return FALSE;
	m_cUsedArray.Remove(nId);
	m_pnFreeId[m_nFreeCount++] = nId;
	return TRUE;
}

VOID QIdPool::Uninit()
{
	SAFE_DELETE_ARRAY(m_pnFreeId);

	m_nMaxNum = 0;
	m_nFreeCount = 0;

	m_cUsedArray.Clear();
}

