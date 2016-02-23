
#include "CorePrivate.h"
#include "Misc/qindexlist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

BOOL QIndexList::Init(INT nMax)
{
	if (!m_cFreeIdx.Init(nMax) || !m_cUseIdx.Init(nMax))
		return FALSE;

	for (INT i = nMax - 1; i > 0; --i)
		m_cFreeIdx.Insert(i);

	m_nMax = nMax;
	return TRUE;
}

VOID QIndexList::Release(VOID)
{
	m_cFreeIdx.Clear();
	m_cUseIdx.Clear();
	m_nMax = 0;
}

BOOL QIndexList::Use(INT nIndex)
{
	if (nIndex < 0 || nIndex >= m_nMax)
		return FALSE;

	m_cFreeIdx.Remove(nIndex);
	m_cUseIdx.Insert(nIndex);
	return TRUE;
}

BOOL QIndexList::Free(INT nIndex)
{
	if (nIndex < 0 || nIndex >= m_nMax)
		return FALSE;

	m_cUseIdx.Remove(nIndex);
	m_cFreeIdx.Insert(nIndex);
	return TRUE;
}
// -------------------------------------------------------------------------
