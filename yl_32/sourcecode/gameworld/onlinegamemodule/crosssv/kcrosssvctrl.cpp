#include "stdafx.h"
#include "kcrosssvctrl.h"
#include "onlinegameworld/kplayer.h"

KCrossSvCtrl::KCrossSvCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KCrossSvCtrl::~KCrossSvCtrl()
{

}

BOOL KCrossSvCtrl::Init()
{

	return TRUE;
}

BOOL KCrossSvCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	return TRUE;
}

BOOL KCrossSvCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KCrossSvCtrl::Active()
{

EXIT0:
	return TRUE;
}

VOID KCrossSvCtrl::ClearDropItems()
{
	m_mapDropItems.clear();
}

VOID KCrossSvCtrl::RecordDropItems(INT nGenre, INT nDetail, INT nParticular, INT nLevel, INT nAmount)
{
	KITEM_INDEX sIndex;
	std::map<KITEM_INDEX, INT>::iterator it;

	sIndex.nGenre = nGenre;
	sIndex.nDetailType = nDetail;
	sIndex.nParticular = nParticular;
	sIndex.nLevel = nLevel;

	it = m_mapDropItems.find(sIndex);
	if(it == m_mapDropItems.end())
	{
		m_mapDropItems[sIndex] = 0;
		it = m_mapDropItems.find(sIndex);
	}

	LOG_PROCESS_ERROR(it != m_mapDropItems.end());

	it->second += nAmount;

EXIT0:
	return;
}

BOOL KCrossSvCtrl::GetDropItemsDataSize(SIZE_T& uUsedSize)
{
	uUsedSize = 0;
	BOOL bResult = FALSE;

	uUsedSize = m_mapDropItems.size() * sizeof(KBZONE_DROPITEM);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KCrossSvCtrl::SaveDropItems(LPBYTE pBuffer, SIZE_T& uUsedSize)
{
	BOOL bResult = FALSE;

	for(std::map<KITEM_INDEX, INT>::iterator it = m_mapDropItems.begin(); it != m_mapDropItems.end(); ++it)
	{
		KBZONE_DROPITEM* pData = (KBZONE_DROPITEM*)pBuffer;
		pData->nGenre = it->first.nGenre;
		pData->nDetail = it->first.nDetailType;
		pData->nParticular = it->first.nParticular;
		pData->nLevel = it->first.nLevel;

		pData->nAmount = it->second;

		pBuffer += sizeof(KBZONE_DROPITEM);
		uUsedSize += sizeof(KBZONE_DROPITEM);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KCrossSvCtrl::LoadDropItems(LPBYTE pBuffer, SIZE_T uDataLen)
{
	BOOL bResult = FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	while(uDataLen > 0)
	{
		KBZONE_DROPITEM* pData = (KBZONE_DROPITEM*)pBuffer;
		cSafeScript->CallTableFunction("MultiInstance", "GiveDropItems", 0, "oddddd", m_rcPlayer.GetScriptInterface(), pData->nGenre, pData->nDetail, pData->nParticular, pData->nLevel, pData->nAmount);
		pBuffer += sizeof(KBZONE_DROPITEM);
		uDataLen -= sizeof(KBZONE_DROPITEM);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KCrossSvCtrl::GetCrossSvDataSize(SIZE_T& uUsedSize)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	bRetCode = GetDropItemsDataSize(uUsedSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KCrossSvCtrl::SaveCrossSvDatas(LPBYTE pBuffer, SIZE_T& uUsedSize)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	bRetCode = SaveDropItems(pBuffer, uUsedSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KCrossSvCtrl::LoadCrossSvDatas(LPBYTE pBuffer, SIZE_T uDataSize)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	bRetCode = LoadDropItems(pBuffer, uDataSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}