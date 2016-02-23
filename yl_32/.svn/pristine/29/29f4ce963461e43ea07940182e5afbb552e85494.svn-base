#include "stdafx.h"
#include "kbladesetting.h"

KBladeSetting g_cBladeSetting;

KBladeSetting::KBladeSetting()
{

}

KBladeSetting::~KBladeSetting()
{

}

BOOL KBladeSetting::Init()
{
	INT nRow = 2;

	ITabFile* pTabFile = g_OpenTabFile("\\setting\\blade\\bladeattr.txt");
	QCONFIRM_RET_FALSE(pTabFile);

	pTabFile->GetInteger(nRow, "Life", 0, &m_cBladeAttrPercent.nLifeP);
	pTabFile->GetInteger(nRow, "Mana", 0, &m_cBladeAttrPercent.nManaP);
	pTabFile->GetInteger(nRow, "Attack", 0, &m_cBladeAttrPercent.nAttackP);
	pTabFile->GetInteger(nRow, "Def", 0, &m_cBladeAttrPercent.nDefP);
	pTabFile->GetInteger(nRow, "Hit", 0, &m_cBladeAttrPercent.nHitP);
	pTabFile->GetInteger(nRow, "Miss", 0, &m_cBladeAttrPercent.nMissP);
	pTabFile->GetInteger(nRow, "Crit", 0, &m_cBladeAttrPercent.nCritP);
	pTabFile->GetInteger(nRow, "Resist", 0, &m_cBladeAttrPercent.nResistP);

	return TRUE;
}

BOOL KBladeSetting::UnInit()
{
	return TRUE;
}

