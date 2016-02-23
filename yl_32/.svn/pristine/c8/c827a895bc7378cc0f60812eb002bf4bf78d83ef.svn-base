
#include "stdafx.h"
#include "kawardsetting.h"
#include "../item2/kmagicattrib.h"
#include "../nsworld/kcharacterhelper.h"

KAwardSetting g_cAwardSetting;

KAwardSetting::KAwardSetting()
{

}

KAwardSetting::~KAwardSetting()
{
	UnInit();
}

BOOL KAwardSetting::Init()
{
	ReadNumericalAwardConfigs();

	ReadAssetAwardConfigs();

	ReadExpAwardConfigs();

	return TRUE;
}

BOOL KAwardSetting::UnInit()
{
	ClearNumericalAwardData();

	ClearAssetAwardData();

	return TRUE;
}

BOOL KAwardSetting::ReadNumericalAwardConfigs()
{
	CHAR szStageAwardName[_MAX_PATH];
	CHAR szLevelAwardName[_MAX_PATH];
	ITabFile* pStageAwardFile = NULL;
	ITabFile* pLevelAwardFile = NULL;
	QString szFileTemp;

	ITabFile* pTabFile = g_OpenTabFile(NUMERICAL_TABLE_FILE);
	QCONFIRM_RET_FALSE(pTabFile);

	for (INT i = 1; i < pTabFile->GetHeight(); i++)
	{
		QModuleNumericalAward* pModuleAward = new QModuleNumericalAward();
		QCONFIRM_RET_FALSE(pModuleAward);

		pTabFile->GetString(i + 1, "ModuleName", "", pModuleAward->szModuleName, MAX_NAME_LEN);
		pTabFile->GetString(i + 1, "StageAwardName", "", szStageAwardName, _MAX_PATH);
		pTabFile->GetString(i + 1, "LevelAwardName", "", szLevelAwardName, _MAX_PATH);

		szFileTemp = AWARD_TABLE_FILE_FOLDER;
		szFileTemp = szFileTemp + szStageAwardName;
		pStageAwardFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pStageAwardFile);

		for (INT j = 2; j < pStageAwardFile->GetHeight(); j++)
		{
			QNumericalStageAward* pNumericalStageAward = new QNumericalStageAward();
			QCONFIRM_RET_FALSE(pNumericalStageAward);

			pStageAwardFile->GetInteger(j + 1, "StageID", 0, &pNumericalStageAward->nStageId);
			pStageAwardFile->GetInteger(j + 1, "MinLineNum", 0, &pNumericalStageAward->nStartIndex);
			pStageAwardFile->GetInteger(j + 1, "MaxLineNum", 0, &pNumericalStageAward->nEndIndex);

			// 减去开头的2行，从0开始
			pNumericalStageAward->nStartIndex -= 2;
			pNumericalStageAward->nEndIndex -= 2;

			pModuleAward->mapNumericalStageAwards[pNumericalStageAward->nStageId] = pNumericalStageAward;
		}

		szFileTemp = AWARD_TABLE_FILE_FOLDER;
		szFileTemp = szFileTemp + szLevelAwardName;
		pLevelAwardFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pLevelAwardFile);

		for (INT j = 2; j < pLevelAwardFile->GetHeight(); j++)
		{
			QNumericalAward* pNumericalAward = new QNumericalAward();
			QCONFIRM_RET_FALSE(pNumericalAward);
			pLevelAwardFile->GetInteger(j + 1, "Level", 0, &pNumericalAward->nLevel);
			pLevelAwardFile->GetInteger(j + 1, "Life", 0, &pNumericalAward->nLife);
			pLevelAwardFile->GetInteger(j + 1, "Ability", 0, &pNumericalAward->nAbility);
			pLevelAwardFile->GetInteger(j + 1, "Attack", 0, &pNumericalAward->nAttack);
			pLevelAwardFile->GetInteger(j + 1, "Defense", 0, &pNumericalAward->nDefense);
			pLevelAwardFile->GetInteger(j + 1, "Hit", 0, &pNumericalAward->nHit);
			pLevelAwardFile->GetInteger(j + 1, "Dodge", 0, &pNumericalAward->nDodge);
			pLevelAwardFile->GetInteger(j + 1, "Crit", 0, &pNumericalAward->nCrit);
			pLevelAwardFile->GetInteger(j + 1, "Resist", 0, &pNumericalAward->nResist);
			pLevelAwardFile->GetInteger(j + 1, "Cost", 0, &pNumericalAward->nCost);

			pModuleAward->vecNumericalAwards.push_back(pNumericalAward);
			pModuleAward->vecNumericalIDs.push_back(pNumericalAward->nLevel);
		}

		m_vecModuleNumericalAwards.push_back(pModuleAward);
	}

	return TRUE;
}

BOOL KAwardSetting::ReadAssetAwardConfigs()
{
	CHAR szLevelAwardName[_MAX_PATH];
	ITabFile* pAssetAwardFile = NULL;
	QString szFileTemp;

	ITabFile* pTabFile = g_OpenTabFile(ASSET_AWARD_FILE);
	QCONFIRM_RET_FALSE(pTabFile);

	for (INT i = 1; i < pTabFile->GetHeight(); i++)
	{
		QModuleAssetAward* pModuleAssetAward = new QModuleAssetAward();
		QCONFIRM_RET_FALSE(pModuleAssetAward);

		pTabFile->GetString(i + 1, "ModuleName", "", pModuleAssetAward->szModuleName, MAX_NAME_LEN);
		pTabFile->GetString(i + 1, "LevelAwardName", "", szLevelAwardName, _MAX_PATH);

		szFileTemp = ASSET_AWARD_FILE_FOLDER;
		szFileTemp = szFileTemp + szLevelAwardName;

		pAssetAwardFile = g_OpenTabFile(szFileTemp.CStr());
		assert(pAssetAwardFile);
		QCONFIRM_RET_FALSE(pAssetAwardFile);

		for (INT j = 2; j < pAssetAwardFile->GetHeight(); j++)
		{
			QAssetAward* pAssetAward = new QAssetAward();
			QCONFIRM_RET_FALSE(pAssetAward);

			pAssetAwardFile->GetInteger(j + 1, "Level", 0, &pAssetAward->nLevel);

			pAssetAwardFile->GetInteger(j + 1, "IsSave", 0, &pAssetAward->nSave);
			pAssetAwardFile->GetInteger(j + 1, "OverlayDay", 0, &pAssetAward->nOverlayDay);
			pAssetAwardFile->GetInteger(j + 1, "AutoGive", 0, &pAssetAward->nAutoGive);
			pAssetAwardFile->GetInteger(j + 1, "IsSync", 0, &pAssetAward->nSync);

			pAssetAwardFile->GetInteger(j + 1, "FightingSpirit", 0, &pAssetAward->nFightingSpirit);
			pAssetAwardFile->GetInteger(j + 1, "Energy", 0, &pAssetAward->nEnergy);
			pAssetAwardFile->GetInteger(j + 1, "Prestige", 0, &pAssetAward->nPrestige);
			pAssetAwardFile->GetInteger(j + 1, "Silver", 0, &pAssetAward->nSilver);
			pAssetAwardFile->GetInteger(j + 1, "Exp", 0, &pAssetAward->nExpAwardId);

			ZeroMemory(&pAssetAward->arrAwardItem, sizeof(pAssetAward->arrAwardItem));

			for(INT k = 0; k < ASSET_AWARD_MAX_ITEM_NUM; ++k)
			{
				CHAR szItem[20];
				CHAR szTabItem[10];
				CHAR szTabItemAmountMin[20];
				CHAR szTabItemAmountMax[20];
				CHAR szTabItemRate[15];
				CHAR szTabItemTimeOut[20];

				snprintf(szTabItem, sizeof(szTabItem), "Item%d", k + 1);
				snprintf(szTabItemAmountMin, sizeof(szTabItemAmountMin), "Item%dAmountMin", k + 1);
				snprintf(szTabItemAmountMax, sizeof(szTabItemAmountMax), "Item%dAmountMax", k + 1);
				snprintf(szTabItemRate, sizeof(szTabItemRate), "Item%dRate", k + 1);
				snprintf(szTabItemTimeOut, sizeof(szTabItemTimeOut), "Item%dTimeOut", k + 1);

				pAssetAwardFile->GetString(j + 1, szTabItem, "", szItem, 20);
				std::string strItem(szItem);

				if(!strItem.size())
				{
					pAssetAward->nItemNum = k;
					break;
				}

				{
					INT nIdx = 0;
					std::basic_string<char>::size_type iBegin = 0, iEnd = 0;
					while((iEnd = strItem.find(',', iBegin)) != std::string::npos)
					{
						switch(nIdx++)
						{
						case 0:
							pAssetAward->arrAwardItem[k].sIndex.nGenre = atoi((strItem.substr(iBegin, iEnd - iBegin)).c_str());
							break;
						case 1:
							pAssetAward->arrAwardItem[k].sIndex.nDetailType = atoi((strItem.substr(iBegin, iEnd - iBegin)).c_str());
							break;
						case 2:
							pAssetAward->arrAwardItem[k].sIndex.nParticular = atoi((strItem.substr(iBegin, iEnd - iBegin)).c_str());
							break;
						default:
							ASSERT(FALSE);
						}
						iBegin = iEnd + 1;
					}
					if(iBegin < strItem.size())
					{
						pAssetAward->arrAwardItem[k].sIndex.nLevel = atoi((strItem.substr(iBegin, strItem.size() - iBegin)).c_str());
					}
				}

				pAssetAwardFile->GetInteger(j + 1, szTabItemAmountMin, 0, &pAssetAward->arrAwardItem[k].nAmountMin);
				pAssetAwardFile->GetInteger(j + 1, szTabItemAmountMax, 0, &pAssetAward->arrAwardItem[k].nAmountMax);
				pAssetAwardFile->GetInteger(j + 1, szTabItemRate, 0, &pAssetAward->arrAwardItem[k].nRate);
				pAssetAwardFile->GetInteger(j + 1, szTabItemTimeOut, 0, &pAssetAward->arrAwardItem[k].nTimeOut);

			}
			pModuleAssetAward->mapAssetAwards[pAssetAward->nLevel] = pAssetAward;
		}

		m_vecModuleAssetAwards.push_back(pModuleAssetAward);
	}

	return TRUE;
}

BOOL KAwardSetting::ClearNumericalAwardData()
{
	std::vector<QModuleNumericalAward*>::iterator it;
	std::map<INT, QNumericalStageAward*>::iterator it_stage;
	std::vector<QNumericalAward*>::iterator it_num;
	for (it = m_vecModuleNumericalAwards.begin(); it != m_vecModuleNumericalAwards.end(); ++it)
	{
		for (it_stage = (*it)->mapNumericalStageAwards.begin(); it_stage != (*it)->mapNumericalStageAwards.end(); ++it_stage)
		{
			SAFE_DELETE((*it_stage).second);
		}
		(*it)->mapNumericalStageAwards.clear();

		for (it_num = (*it)->vecNumericalAwards.begin(); it_num != (*it)->vecNumericalAwards.end(); ++it_num)
		{
			SAFE_DELETE((*it_num));
		}
		(*it)->vecNumericalAwards.clear();
		(*it)->vecNumericalIDs.clear();

		SAFE_DELETE((*it));
	}

	m_vecModuleNumericalAwards.clear();

	return TRUE;
}

BOOL KAwardSetting::ClearAssetAwardData()
{
	std::map<INT, QAssetAward*>::iterator it_ast;
	std::vector<QModuleAssetAward*>::iterator it;
	for (it = m_vecModuleAssetAwards.begin(); it != m_vecModuleAssetAwards.end(); ++it)
	{
		for (it_ast = (*it)->mapAssetAwards.begin(); it_ast != (*it)->mapAssetAwards.end(); ++it_ast)
		{
			SAFE_DELETE((*it_ast).second);
		}

		(*it)->mapAssetAwards.clear();
		SAFE_DELETE((*it));
	}

	m_vecModuleAssetAwards.clear();

	return TRUE;
}

CONST INT KAwardSetting::RequestMaxLineValue(QString moduleName, INT nCurrentStage)
{
	QModuleNumericalAward* pTargetModuleNumericalAward = NULL;
	std::vector<QModuleNumericalAward*>::iterator it;
	for (it = m_vecModuleNumericalAwards.begin(); it != m_vecModuleNumericalAwards.end(); ++it)
	{
		if (moduleName ==  (*it)->szModuleName)
		{
			pTargetModuleNumericalAward = (*it);
			break;
		}
	}

	QCONFIRM_RET_FALSE(pTargetModuleNumericalAward);
	QNumericalStageAward* pTargetNumericalStageAward = pTargetModuleNumericalAward->mapNumericalStageAwards[nCurrentStage];
	QCONFIRM_RET_FALSE(pTargetNumericalStageAward);

	return pTargetNumericalStageAward->nEndIndex;
}

CONST INT KAwardSetting::RequestAwardSum(QString moduleName, INT nCurrentStage, INT nStartLevel, INT nLevelCount)
{
	QModuleNumericalAward* pTargetModuleNumericalAward = NULL;
	INT nAwardSum = 0;
	std::vector<QModuleNumericalAward*>::iterator it;
	for (it = m_vecModuleNumericalAwards.begin(); it != m_vecModuleNumericalAwards.end(); ++it)
	{
		if (moduleName ==  (*it)->szModuleName)
		{
			pTargetModuleNumericalAward = (*it);
			break;
		}
	}

	QCONFIRM_RET_FALSE(pTargetModuleNumericalAward);
	QNumericalStageAward* pTargetNumericalStageAward = pTargetModuleNumericalAward->mapNumericalStageAwards[nCurrentStage];
	QCONFIRM_RET_FALSE(pTargetNumericalStageAward);

	/*if (nMaxLevel < 0 || (pTargetNumericalStageAward->nStartIndex + nMaxLevel) > pTargetNumericalStageAward->nEndIndex)
	{
		return FALSE;
	}*/

	for (INT i = pTargetNumericalStageAward->nStartIndex - 1; i < pTargetNumericalStageAward->nEndIndex; i++)
	{
		if (pTargetModuleNumericalAward->vecNumericalIDs.size() > i && pTargetModuleNumericalAward->vecNumericalIDs[i] >= nStartLevel && pTargetModuleNumericalAward->vecNumericalIDs[i] < nStartLevel + nLevelCount)
		{
			nAwardSum += pTargetModuleNumericalAward->vecNumericalAwards[i]->nCost;
		}
	}

	return nAwardSum;
}

INT KAwardSetting::GetLevelNumOfStage(QString moduleName, INT nCurrentStage)
{
	QModuleNumericalAward* pTargetModuleNumericalAward = NULL;
	QNumericalStageAward* pTargetNumericalStageAward = NULL;
	std::vector<QModuleNumericalAward*>::iterator it;
	for (it = m_vecModuleNumericalAwards.begin(); it != m_vecModuleNumericalAwards.end(); ++it)
	{
		if (moduleName ==  (*it)->szModuleName)
		{
			pTargetModuleNumericalAward = (*it);
			break;
		}
	}

	LOG_PROCESS_ERROR(pTargetModuleNumericalAward);

	pTargetNumericalStageAward = pTargetModuleNumericalAward->mapNumericalStageAwards[nCurrentStage];
	LOG_PROCESS_ERROR(pTargetNumericalStageAward);

	return (pTargetNumericalStageAward->nEndIndex - pTargetNumericalStageAward->nStartIndex + 1);

EXIT0:
	return 0;
}

CONST QNumericalAward* KAwardSetting::RequestNumericalAward(QString moduleName, INT nCurrentStage, INT nLevel)
{
	QModuleNumericalAward* pTargetModuleNumericalAward = NULL;
	std::vector<QModuleNumericalAward*>::iterator it;
	for (it = m_vecModuleNumericalAwards.begin(); it != m_vecModuleNumericalAwards.end(); ++it)
	{
		if (moduleName ==  (*it)->szModuleName)
		{
			pTargetModuleNumericalAward = (*it);
			break;
		}
	}

	QCONFIRM_RET_FALSE(pTargetModuleNumericalAward);
	QNumericalStageAward* pTargetNumericalStageAward = pTargetModuleNumericalAward->mapNumericalStageAwards[nCurrentStage];
	QCONFIRM_RET_FALSE(pTargetNumericalStageAward);

	INT nTargetNumericalIndex = -1;
	for (INT i = pTargetNumericalStageAward->nStartIndex - 1; i < pTargetNumericalStageAward->nEndIndex; i++)
	{
		if (pTargetModuleNumericalAward->vecNumericalIDs.size() > i && pTargetModuleNumericalAward->vecNumericalIDs[i] == nLevel)
		{
			nTargetNumericalIndex = i;
			break;
		}
	}

	if (nTargetNumericalIndex == -1) // 如果找不到，直接返回最后一等级的数据
	{
		return NULL;//pTargetModuleNumericalAward->vecNumericalAwards[(INT)pTargetModuleNumericalAward->vecNumericalAwards.size() - 1];
	}

	QNumericalAward* pTargetNumericalAward = pTargetModuleNumericalAward->vecNumericalAwards[nTargetNumericalIndex];
	return pTargetNumericalAward;
}

CONST QAssetAward* KAwardSetting::RequestAssetAward(QString moduleName, INT nLevel)
{
	QModuleAssetAward* pTargetModuleAward = NULL;
	std::vector<QModuleAssetAward*>::iterator it;
	std::map<INT, QAssetAward*>::iterator it_find;
	QAssetAward* pTargetAward = NULL;

	for (it = m_vecModuleAssetAwards.begin(); it != m_vecModuleAssetAwards.end(); ++it)
	{
		if (moduleName ==  (*it)->szModuleName)
		{
			pTargetModuleAward = (*it);
			break;
		}
	}
	assert(pTargetModuleAward);
	LOG_PROCESS_ERROR(pTargetModuleAward);

	it_find = pTargetModuleAward->mapAssetAwards.find(nLevel);
	PROCESS_ERROR(it_find != pTargetModuleAward->mapAssetAwards.end());

	pTargetAward = pTargetModuleAward->mapAssetAwards[nLevel];

	
	if(!pTargetAward)
	{
		QLogPrintf(LOG_ERR,"Can't find level, modulename:%s, level:%d\n", moduleName.CStr(), nLevel);
	}
	//QCONFIRM_RET_NULL(pTargetAward);
	
EXIT0:
	return pTargetAward;
}

INT KAwardSetting::GetAwardValueByMA(INT nAttr, CONST QNumericalAward* pAward)
{
	INT nValue = 0;
	switch(nAttr)
	{
	case modifydmg_v:
		nValue = pAward->nAttack;
		break;
	case adddef_v:
		nValue = pAward->nDefense;
		break;
	case addhitrate_v:
		nValue = pAward->nHit;
		break;
	case addcriticalrate_v:
		nValue = pAward->nCrit;
		break;
	case addresist_v:
		nValue = pAward->nResist;
		break;
	case addmaxlife_v:
		nValue = pAward->nLife;
		break;
	case addmaxmana_v:
		nValue = pAward->nAbility;
		break;
	case adddodge_v:
		nValue = pAward->nDodge;
		break;
	default:
		break;
	}

	return nValue;
}

BOOL KAwardSetting::HasAssetAward(LPCSTR szModuleName, INT nLevel)
{
	BOOL bResult = FALSE;
	QString strModuleName(szModuleName);
	CONST QAssetAward* pAssetAward = g_cAwardSetting.RequestAssetAward(strModuleName, nLevel);
	PROCESS_ERROR(pAssetAward);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KAwardSetting::ReadExpAwardConfigs()
{
	INT nExpAwardId = 0;
	CHAR szPath[_MAX_PATH];
	QString szFileTemp;
	ITabFile* pExpAwardFile = NULL;
	ITabFile* pTabFile = g_OpenTabFile(EXP_AWARD_FILE);
	QCONFIRM_RET_FALSE(pTabFile);

	for (INT i = 2; i < pTabFile->GetHeight(); i++)
	{
		pTabFile->GetInteger(i + 1, "ExpAwardId", 0, &nExpAwardId);
		pTabFile->GetString(i + 1, "Path", "", szPath, _MAX_PATH);

		szFileTemp = EXP_AWARD_FILE_FOLDER;
		szFileTemp = szFileTemp + szPath;

		pExpAwardFile = g_OpenTabFile(szFileTemp.CStr());
		QCONFIRM_RET_FALSE(pExpAwardFile);

		MAP_EXP_AWARD mapExpAward;

		for (INT j = 2; j < pExpAwardFile->GetHeight(); j++)
		{
			KExpAward sAward;
			
			pExpAwardFile->GetInteger(j + 1, "Level", 0, &sAward.nLevel);
			pExpAwardFile->GetInteger(j + 1, "Percent", 0, &sAward.nPercent);
			mapExpAward[sAward.nLevel] = sAward;
		}

		m_mapExpAwards[nExpAwardId] = mapExpAward;
	}

	return TRUE;
}

INT KAwardSetting::GetExpAwardPercent(INT nExpAwardId, INT nLevel)
{
	INT nPercent = 0;
	MAP_EXP_AWARD::iterator it_exp;

	std::map<INT, MAP_EXP_AWARD>::iterator it = m_mapExpAwards.find(nExpAwardId);
	LOG_PROCESS_ERROR(it != m_mapExpAwards.end());

	it_exp = (it->second).find(nLevel);
	LOG_PROCESS_ERROR(it_exp != (it->second).end());

	nPercent = it_exp->second.nPercent;
EXIT0:
	return nPercent;
}

DWORD KAwardSetting::GetExpByLevel(INT nExpAwardId, INT nNextLevel)
{
	INT nLevelExp = 0;
	INT nExpPercent = 0;
	INT nTargetLevel = 0;

	// 找到不为0的经验等级，以当前等级到这个等级的经验和来去算
	for (INT i = nNextLevel - 1; i < 100; i++)
	{
        nExpPercent = g_cAwardSetting.GetExpAwardPercent(nExpAwardId, i);
		if (nExpPercent != 0)
		{
	        nTargetLevel = i + 1;
			break;
		}
	}

	for (INT i = nNextLevel; i <= nTargetLevel; i++)
	{
        nLevelExp += g_cCharacterHelper.GetNextLvExp(nNextLevel);
	}
	
	//INT nExpPercent = g_cAwardSetting.GetExpAwardPercent(nExpAwardId, nNextLevel - 1);
	FLOAT fPercent = (FLOAT)nExpPercent / 10000.0f;
	DWORD dwExp = (DWORD) ((FLOAT)nLevelExp * fPercent);

	return dwExp;
}