#include "stdafx.h"
#include "kitemroom.h"
#include "kitem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------
// KItemRoom

KItemRoom::KItemRoom() : m_pnArray(NULL)
{
	m_nWidth		= 0;
	m_nHeight		= 0;
	m_nMaxWidth		= 0;
	m_nMaxHeight	= 0;
}

KItemRoom::~KItemRoom()
{
    Release();
}

KItemRoom::KItemRoom(const KItemRoom &rhl) : m_pnArray(NULL)
{
	Init(rhl.m_nMaxWidth, rhl.m_nMaxHeight, rhl.m_nWidth, rhl.m_nHeight);
	memcpy(m_pnArray, rhl.m_pnArray, m_nMaxWidth * m_nMaxHeight * sizeof(INT));
}

KItemRoom& KItemRoom::operator=(const KItemRoom &rhl)
{
	if (m_pnArray)
		delete m_pnArray;
	Init(rhl.m_nMaxWidth, rhl.m_nMaxHeight, rhl.m_nWidth, rhl.m_nHeight);
	memcpy(m_pnArray, rhl.m_pnArray, m_nMaxWidth * m_nMaxHeight * sizeof(INT));
	return *this;
}


INT KItemRoom::CountUsed(VOID) CONST
{
	if (!m_pnArray)
		return	0;

	INT nCount = 0;
	KFinder cFind(*this);
	while (cFind.Next(KRoomFindCondition::Used))
		++nCount;
	return	nCount;
}

BOOL KItemRoom::Init(INT nMaxWidth, INT nMaxHeight, INT nWidth /* = -1 */, INT nHeight /* = -1 */)
{
	ASSERT(nMaxWidth > 0 && nMaxHeight > 0 && nMaxWidth >= nWidth && nMaxHeight >= nHeight);

	if (nWidth < 0)
		nWidth = nMaxWidth;
	if (nHeight < 0)
		nHeight = nMaxHeight;

	Release();

	m_pnArray = new INT[nMaxWidth * nMaxHeight];
	::memset(m_pnArray, 0, sizeof(INT) * nMaxWidth * nMaxHeight);
	m_nMaxWidth = nMaxWidth;
	m_nMaxHeight = nMaxHeight;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	return TRUE;
}

VOID KItemRoom::Release(VOID)
{
	if (m_pnArray)
	{
		delete [] m_pnArray;
		m_pnArray = NULL;
	}

	m_nWidth		= 0;
	m_nHeight		= 0;
	m_nMaxWidth		= 0;
	m_nMaxHeight	= 0;
}

VOID KItemRoom::Clear(VOID)
{
	if (m_pnArray)
		::memset(m_pnArray, 0, sizeof(INT) * m_nMaxWidth * m_nMaxHeight);
}

BOOL KItemRoom::Resize(INT nWidth, INT nHeight)
{
	if (!CanResize(nWidth, nHeight))
		return	FALSE;

	m_nWidth  = nWidth;
	m_nHeight = nHeight;
	return	TRUE;
}

BOOL KItemRoom::CanResize(INT nWidth, INT nHeight) CONST
{
	if (!m_pnArray || nWidth < 0 || nWidth > m_nMaxWidth || nHeight < 0 || nHeight > m_nMaxHeight)
		return	FALSE;

	if (!nWidth != !nHeight)	// nWidth, nHeight要么都是零，要么都不是零
		return	FALSE;

	if (CountUsed() > nWidth * nHeight)
		return	FALSE;			// 背包变小后不够装下原有的东西，失败

	return	TRUE;
}

INT KItemRoom::GetCell(INT nX /* = 0 */, INT nY /* = 0 */) CONST
{
	LPINT pCell = Cell(nX, nY);
	if (!pCell)
		return	0;
	return	*pCell;
}

BOOL KItemRoom::SetCell(INT nIdx, INT nX /* = 0 */, INT nY /* = 0 */)
{
	LPINT pCell = Cell(nX, nY);
	if (!pCell)
		return	FALSE;
	*pCell = nIdx;
	return	TRUE;
}

BOOL KItemRoom::IsPickable(INT nIdx /* = 0 */, INT nX /* = 0 */, INT nY /* = 0 */) CONST
{
	LPINT pnCell = Cell(nX, nY);
	if (!pnCell || (nIdx > 0 && *pnCell != nIdx))
		return	FALSE;
	return	TRUE;
}

BOOL KItemRoom::IsDropable(INT nX /* = 0 */, INT nY /* = 0 */) CONST
{
	LPINT pnCell = Cell(nX, nY);
	if (!pnCell || *pnCell > 0)
		return	FALSE;
	return	TRUE;
}

BOOL KItemRoom::PickItem(INT nIdx, INT nX /* = 0 */, INT nY /* = 0 */)
{
	LPINT pnCell = Cell(nX, nY);
	if (nIdx <= 0 || !pnCell || *pnCell != nIdx)
		return	FALSE;
	*pnCell = 0;
	return	TRUE;
}

BOOL KItemRoom::DropItem(INT nIdx, INT nX /* = 0 */, INT nY /* = 0 */)
{
	LPINT pnCell = Cell(nX, nY);
	if (nIdx <= 0 || !pnCell || *pnCell > 0)
		return	FALSE;
	*pnCell = nIdx;
	return	TRUE;
}

BOOL KItemRoom::PickItemDirectly(INT nIdx, INT nX /* = 0 */, INT nY /* = 0 */)
{
	LPINT pCell = CellDirectly(nX, nY);
	if (nIdx <= 0 || !pCell || *pCell != nIdx)
		return	FALSE;
	*pCell = 0;
	return	TRUE;
}

BOOL KItemRoom::DropItemDirectly(INT nIdx, INT nX /* = 0 */, INT nY /* = 0 */)
{
	LPINT pCell = CellDirectly(nX, nY);
	if (nIdx <= 0 || !pCell || *pCell > 0)
		return	FALSE;
	*pCell = nIdx;
	return	TRUE;
}

BOOL KItemRoom::GetFreeCell(INT& rnX, INT& rnY) CONST
{
	KFinder cFind(*this);
	if (!cFind.Next(KRoomFindCondition::Free))
		return	FALSE;				// 空间已满
	CONST KPos &sPos = cFind.Pos();
	rnX = sPos.nX;
	rnY = sPos.nY;
	return	TRUE;
}

BOOL KItemRoom::FindItem(INT nIndex, LPINT pnX /* = NULL */, LPINT pnY /* = NULL */) CONST
{
	if (nIndex <= 0 || nIndex >= MAX_ITEM)
		return	FALSE;

	KFinder cFinder(*this);
	if (!cFinder.Next(KRoomFindCondition::SameIndex(nIndex)))
		return	FALSE;

	CONST KPos &sPos = cFinder.Pos();
	if (pnX)
		*pnX = sPos.nX;
	if (pnY)
		*pnY = sPos.nY;

	return	TRUE;
}

KItemRoom::KFinder KItemRoom::GetFinder(VOID) CONST
{
	return KFinder(*this);
}

LPINT KItemRoom::Cell(INT nX, INT nY) CONST
{
	if (nX < 0 || nY < 0 || nX >= m_nWidth || nY >= m_nHeight)
		return	NULL;
	return	m_pnArray + nY * m_nMaxWidth + nX;
}

LPINT KItemRoom::CellDirectly(INT nX, INT nY) CONST
{
	if (nX < 0 || nY < 0 || nX >= m_nMaxWidth || nY >= m_nMaxHeight)
		return	NULL;
	return	m_pnArray + nY * m_nMaxWidth + nX;
}

VOID KItemRoom::ClearCell(INT nX /* = 0 */, INT nY /* = 0 */)
{
	SetCell(0, nX, nY);
}

//---------------------------------------------------------------------------------
// KItemRoom::KFinder

VOID KItemRoom::KFinder::Attach(CONST KItemRoom& rcRoom)
{
	m_pcRoom	= &rcRoom;
	m_nPos		= 0;
	m_nLast		= 0;
}

KPos KItemRoom::KFinder::Pos(VOID) CONST
{
	ASSERT(m_pcRoom);
	INT  nPos = m_nPos - 1;
	KPos sPos;
	sPos.nX = nPos % m_pcRoom->GetWidth();
	sPos.nY = nPos / m_pcRoom->GetWidth();
	return	sPos;
}

BOOL KItemRoom::KFinder::Begin(INT nX /* = 0 */, INT nY /* = 0 */)
{
	ASSERT(m_pcRoom);
	if (nX < 0 || nY < 0 || nX >= m_pcRoom->GetWidth() || nY >= m_pcRoom->GetHeight())
		return	FALSE;
	m_nPos	= nY * m_pcRoom->GetWidth() + nX;
	m_nLast	= 0;
	return	TRUE;
}

//---------------------------------------------------------------------------------
// KRoomFindCondition

bool KRoomFindCondition::Any(INT nIdx)
{
	return true;
}

bool KRoomFindCondition::Free(INT nIdx)
{
	return nIdx <= 0;
}

bool KRoomFindCondition::Used(INT nIdx)
{
	return nIdx > 0;
}

bool KRoomFindCondition::Equip(INT nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_ITEM)
		return	false;
	return	KD_ISEQUIP(Item[nIdx].GetGenre());
}

KRoomFindCondition::SameIndex::SameIndex(CONST KItem& rcItem)
: m_nIndex(rcItem.GetGameIndex())
{

}

KRoomFindCondition::SameItem::SameItem(CONST KItem& rcItem)
: m_sIndex(rcItem.GetIndex())
{

}

KRoomFindCondition::SameItem::SameItem(INT nGenre,		// = KD_ITEM_INVALID_TYPE
									   INT nDetail,		// = KD_ITEM_INVALID_TYPE
									   INT nParticular,	// = KD_ITEM_INVALID_TYPE
									   INT nLevel)		// = KD_ITEM_NO_LEVEL_LIMIT
{
	m_sIndex.nGenre			= nGenre;
	m_sIndex.nDetailType	= nDetail;
	m_sIndex.nParticular	= nParticular;
	m_sIndex.nLevel			= nLevel;
}

bool KRoomFindCondition::SameItem::operator () (INT nIdx) CONST
{
	if (nIdx <= 0 || nIdx >= MAX_ITEM)
		return	false;

	if (m_sIndex.nGenre <= ITEM_INVALID_TYPE)
		return	true;
	if (m_sIndex.nGenre != Item[nIdx].GetGenre())
		return	false;
	if (m_sIndex.nDetailType <= ITEM_INVALID_TYPE)
		return	true;
	if (m_sIndex.nDetailType != Item[nIdx].GetDetailType())
		return	false;
	if (m_sIndex.nParticular <= ITEM_INVALID_TYPE)
		return	true;
	if (m_sIndex.nParticular != Item[nIdx].GetParticular())
		return	false;
	if (m_sIndex.nLevel <= KD_ITEM_NO_LEVEL_LIMIT)
		return	true;
	if (m_sIndex.nLevel != Item[nIdx].GetLevel())
		return	false;

	return	true;
}

KRoomFindCondition::SameIndexSeries::SameIndexSeries(INT nGenre,		// = KD_ITEM_INVALID_TYPE
													 INT nDetail,		// = KD_ITEM_INVALID_TYPE
													 INT nParticular,	// = KD_ITEM_INVALID_TYPE
													 INT nLevel,		// = KD_ITEM_NO_LEVEL_LIMIT
													 INT nBind			// = KD_ITEM_NO_BIND_LIMIT
													 )
: SameItem(nGenre, nDetail, nParticular, nLevel)
{
	m_nBind = nBind;
}

bool KRoomFindCondition::SameIndexSeries::operator () (INT nIdx) CONST
{
	if (!SameItem::operator () (nIdx))
		return	false;
	return	(m_nBind <= KD_ITEM_NO_BIND_LIMIT || m_nBind == Item[nIdx].IsBind());
}

bool KRoomFindCondition::SameClass::operator () (INT nIdx) CONST
{
	if (nIdx <= 0 || nIdx >= MAX_ITEM)
		return	false;

	LPCSTR pszClass = Item[nIdx].GetClass();
	if (!strcmp(pszClass, m_pszClass))
		return	true;

	return	false;
}

KRoomFindCondition::StackWith::StackWith(CONST KItem& rcItem)
: SameIndexSeries(rcItem.GetIndex(), rcItem.IsBind())
{

}

bool KRoomFindCondition::StackWith::operator () (INT nIdx) CONST
{
	if (!SameIndexSeries::operator () (nIdx))
		return	false;
	if (Item[nIdx].GetTimeout()->dwTimeout > 0)
		return	false;
	return (Item[nIdx].GetCount() < Item[nIdx].GetMaxCount());
}

//---------------------------------------------------------------------------------
