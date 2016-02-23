
#ifndef	KItemGeneratorH
#define	KItemGeneratorH

#include "klibofbpt.h"
#include "kitem.h"

//---------------------------------------------------------------------------
// KItemGenerator

class KItemGenerator
{

public:

	BOOL	Init(INT nVersion);
	BOOL	Gen_Item(
		KItem					*pItem,
		INT						nItemGenre,
		INT						nDetailType,
		INT						nParticular,
		INT						nLevel,
		INT						nEnhTimes,
		CONST KItem::KSTONE_INFO* pStoneInfo,
		CONST KMagicAttrib*		pAppendAttr,
		CONST KItem::KREFINE_LOCK* pRefineLock,
		UINT					uRandSeed,
		UINT					uRandSeedEx,
		BOOL					bNew,
		BOOL					bTemporary = FALSE,
		CONST GUID*				pGuid = NULL
	);
	CONST KLibOfBPT* GetLibOfBPT(VOID) CONST { return &m_BPTLib; }

protected:

	INT			m_nVersion;
	KLibOfBPT	m_BPTLib;
	KRandom		m_cRandom;
	KRandomEx	m_cRandomEx;

	BOOL	Gen_GeneralEquip(
							KItem* pItem,
							INT nDetailType,
							INT nParticular,
							INT nLevel,
							INT nEnhTimes,
							CONST KItem::KSTONE_INFO* pStoneInfo,
							CONST KMagicAttrib*	pAppendAttr,
							CONST KItem::KREFINE_LOCK* pRefineLock,
							BOOL bNew
							);

	BOOL	Gen_GeneralSuit(
							KItem*					pItem,
							INT						nDetailType,
							INT						nParticular,
							INT						nLevel,
							BOOL bNew
							);

	BOOL	Gen_Medicine(KItem* pItem, INT nDetailType, INT nParticular);
	BOOL	Gen_Stone(KItem* pItem, INT nDetailType, INT nParticular, INT nLevel);
	BOOL	Gen_Ride(KItem* pItem, INT nDetailType, INT nParticular, INT nLevel);
	BOOL	Gen_ScriptItem(KItem* pItem, INT nDetailType, INT nParticular);
	BOOL	Gen_PetItem(KItem* pItem, INT nDetailType, INT nParticular);
	BOOL	Gen_SkillItem(KItem* pItem, INT nDetailType, INT nParticular);
	BOOL	Gen_TaskQuest(KItem* pItem, INT nDetailType, INT nParticular);
	BOOL	Gen_ExtendBag(KItem* pItem, INT nDetailType, INT nParticular);
	BOOL	Gen_StuffItem(KItem* pItem, INT nDetailType, INT nParticular);
	BOOL	Gen_PlanItem(KItem* pItem, INT nDetailType, INT nParticular);

};

//---------------------------------------------------------------------------

#endif	// #ifndef	KItemGeneratorH
