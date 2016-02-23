/* -------------------------------------------------------------------------
//	文件名		：	fightskillsetting.cpp
//	创建者		：	zhangzhixiong
//	创建时间	：	2011.5.21
//	实现者		：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "fightskillsetting.h"

#define SKILL_TABLE_FILE                 "\\setting\\fightskill\\fightskill.txt"
#define PASSIVE_SKILL_NUMBER_FILE		 "\\setting\\fightskill\\passive_skill_number.txt"  // 被动技能
#define MEDICINE_SKILL_NUMBER_FILE		 "\\setting\\fightskill\\medicine_skill_number.txt"  // 药槽技能
#define RESIST_SKILL_NUMBER_FILE		 "\\setting\\fightskill\\resist_skill_number.txt"  // 抗体技能
#define INACTIVE_SKILL_NUMBER_FILE		 "\\setting\\fightskill\\inactive_skill_number.txt"  // 抗体技能

FightSkillSetting g_cFightSkillSetting;

FightSkillSetting::FightSkillSetting()
{
}

FightSkillSetting::~FightSkillSetting()
{

}

BOOL FightSkillSetting::Init()
{
	using namespace std;
	INT nHeight = 0;

	m_mapFightSkills.clear();

	ITabFile* tabFile = g_OpenTabFile(SKILL_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		FightSkillTemplate* fightSkillTemplate = new FightSkillTemplate();
		tabFile->GetInteger(i, "SkillId", 0, &fightSkillTemplate->nSkillID);
		tabFile->GetInteger(i, "SkillType", 0, &fightSkillTemplate->nFightSkillType);
		tabFile->GetInteger(i, "PassiveSkill", 0, &fightSkillTemplate->nPassiveSkillType);
		tabFile->GetInteger(i, "NeedTarget", 0, &fightSkillTemplate->bNeedTarget);
		tabFile->GetInteger(i, "IsImmediately", 0, &fightSkillTemplate->bIsImmediately);
		tabFile->GetInteger(i, "PositionType", 0, &fightSkillTemplate->nFightSkillStartPos);
		tabFile->GetInteger(i, "Kind", 0, &fightSkillTemplate->nFightSkillKind);
		//tabFile->GetInteger(i, "MissileHorizontalSpeed", 0, &fightSkillTemplate->nMissileHorizontalSpeed);
		//tabFile->GetInteger(i, "MissileVerticalSpeed", 0, &fightSkillTemplate->nMissileVerticalSpeed);
		tabFile->GetInteger(i, "StartHeight", 0, &fightSkillTemplate->nMissileHeight);
		tabFile->GetInteger(i, "IsAura", 0, &fightSkillTemplate->bAura);
		
		tabFile->GetInteger(i, "IsMelee", 0, &fightSkillTemplate->bMelee);
		tabFile->GetInteger(i, "SkillCostType", 0, &fightSkillTemplate->nSkillCostType);
		tabFile->GetInteger(i, "CostValue", 0, &fightSkillTemplate->nCostPercent);
		tabFile->GetInteger(i, "TimePerCast", 0, &fightSkillTemplate->nTimePerCast);
		tabFile->GetFloat(i, "TimeBeforeCast", 0, &fightSkillTemplate->nTimeBeforeCast);
		tabFile->GetInteger(i, "IsPhysical", 0, &fightSkillTemplate->bIsPhysical);
		tabFile->GetInteger(i, "DmgOrRvv", 0, &fightSkillTemplate->bDmgOrRvv);
		tabFile->GetInteger(i, "IsUseAR", 0, &fightSkillTemplate->bUseAR);
		tabFile->GetInteger(i, "IsChannelSkill", 0, &fightSkillTemplate->nFightSkillChannel);
		tabFile->GetInteger(i, "ChannelTime", 0, &fightSkillTemplate->nChannelTime);
		tabFile->GetInteger(i, "ChannelFreq", 0, &fightSkillTemplate->nChannelFreq);
		tabFile->GetFloat(i, "RepelPercent", 0, &fightSkillTemplate->fRepelPercent);
		tabFile->GetFloat(i, "RepelValue", 0, &fightSkillTemplate->fRepelValue);
		tabFile->GetInteger(i, "ReqLevel", 0, &fightSkillTemplate->nReqLevel);
		tabFile->GetInteger(i, "MaxLevel", 0, &fightSkillTemplate->nMaxLevel);
		//tabFile->GetInteger(i, "SkillLevel", 0, &fightSkillTemplate->nSkillLevel);
		tabFile->GetInteger(i, "WeaponLimit", 0, &fightSkillTemplate->nWeaponLimit);
		tabFile->GetInteger(i, "HorseLimit", 0, &fightSkillTemplate->bHorseLimit);
		tabFile->GetInteger(i, "PeaceCanUse", 0, &fightSkillTemplate->bPeachCanUse);
		tabFile->GetInteger(i, "IsExpSkill", 0, &fightSkillTemplate->bExpSkill);
		tabFile->GetFloat(i, "AttackRadius", 0.0f, &fightSkillTemplate->fAttackRadius);
		tabFile->GetInteger(i, "Category", 0, &fightSkillTemplate->nCategory);
		tabFile->GetInteger(i, "ChainNum", 0, &fightSkillTemplate->nChainLightningTargetNum);
		tabFile->GetFloat(i, "ChainRadius", 0, &fightSkillTemplate->fChainLightningRange);
		tabFile->GetInteger(i, "CallBuffer1", 0, &fightSkillTemplate->nIncorBuff1Id);
		tabFile->GetInteger(i, "CallBuffer2", 0, &fightSkillTemplate->nIncorBuff2Id);
		tabFile->GetInteger(i, "CallBuffer3", 0, &fightSkillTemplate->nIncorBuff3Id);
		tabFile->GetFloat(i, "MissileSpeed", 0, &fightSkillTemplate->fMissileSpeed);
		tabFile->GetInteger(i, "Slot", 0, &fightSkillTemplate->nSlot);
		//tabFile->GetInteger(i, "RuneSkill", 0, &fightSkillTemplate->nRune);
		tabFile->GetInteger(i, "YLEnergy", 0, &fightSkillTemplate->nYlEnergy);

		// 技能系数默认为1000
		tabFile->GetInteger(i, "CoefficientMin", 1000, &fightSkillTemplate->nCoefficientMin);
		tabFile->GetInteger(i, "CoefficientMax", 1000, &fightSkillTemplate->nCoefficientMax);
		tabFile->GetFloat(i, "SpecialSkillTime", 1, &fightSkillTemplate->fSpecialTimeScale);
		tabFile->GetInteger(i, "SpecialCDTime", 0, &fightSkillTemplate->nSpecialCdTime);
		tabFile->GetFloat(i, "SpecialTimeBeforeCast", 0, &fightSkillTemplate->nSpecialTimeBeforeCast);

		fightSkillTemplate->bNeedJumpTo = FALSE;
		tabFile->GetString(i, "SkillPlugins", "", fightSkillTemplate->szPlugins, countof(fightSkillTemplate->szPlugins));
		if (!strcmp(fightSkillTemplate->szPlugins, "QRLSkillJumpTo")) // 是否跳砸类技能
		{
		    fightSkillTemplate->bNeedJumpTo = TRUE;
		}

		//tabFile->GetInteger(i, "FactionLimit", 0, &fightSkillTemplate->nFactionLimit); 

		char factions[64];  // 获取字符串，逗号分隔，获取BOOL列表数组
		tabFile->GetString(i, "FactionLimit", "", factions, countof(factions));
		std::vector<string> factionList;
		this->SplitString(factions, ",", factionList);
		fightSkillTemplate->nFactionLimit[0] = FALSE;
		fightSkillTemplate->nFactionLimit[1] = FALSE;
		fightSkillTemplate->nFactionLimit[2] = FALSE;
		fightSkillTemplate->nFactionLimit[3] = FALSE;
		fightSkillTemplate->nFactionLimit[4] = FALSE;
		for (std::vector<string>::iterator iter = factionList.begin(); iter != factionList.end(); ++iter)
		{
			INT number = std::atoi((*iter).c_str());
			fightSkillTemplate->nFactionLimit[number] = TRUE;  // Index对应门派是否为TRUE
		}

		tabFile->GetInteger(i, "RouteLimit", 0, &fightSkillTemplate->nRouteLimit);
		
		{
			memset(fightSkillTemplate->abRelation, 0, sizeof(fightSkillTemplate->abRelation));
			char achRelation[5+1] = { 0 };
			tabFile->GetString(i, "SkillRelation", "", achRelation, 5+1);
			string tRelation(achRelation);
			if (tRelation.size())
			{
				basic_string<char>::size_type iBegin = 0, iEnd = 0;
				while ( (iEnd = tRelation.find("|", iBegin)) != string::npos )
				{
					INT nType = atoi(tRelation.substr(iBegin, iEnd - iBegin).c_str());
					fightSkillTemplate->abRelation[nType-1] = TRUE;
					iBegin = iEnd + 1;
				}
				if (iBegin < tRelation.size())
				{
					INT nType = atoi(tRelation.substr(iBegin, tRelation.size() - iBegin).c_str());
					fightSkillTemplate->abRelation[nType-1] = TRUE;
				}
			}
			else
			{
				// TODO:全改成FALSE
				fightSkillTemplate->abRelation[0] = TRUE;
				fightSkillTemplate->abRelation[1] = TRUE;
				fightSkillTemplate->abRelation[2] = TRUE;
				fightSkillTemplate->abRelation[3] = TRUE;
			}
		}

		m_mapFightSkills[fightSkillTemplate->nSkillID] = fightSkillTemplate;
	}

	// 被动技能数值
	tabFile = g_OpenTabFile(PASSIVE_SKILL_NUMBER_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		PassiveSkillNumber* pPassiveSkillNumber = new PassiveSkillNumber();
		tabFile->GetInteger(i, "PassiveType", 0, &pPassiveSkillNumber->nType);
		tabFile->GetInteger(i, "SkillLevel", 0, &pPassiveSkillNumber->nSkillLevel);
		tabFile->GetInteger(i, "Value", 0, &pPassiveSkillNumber->nValue);

		INT nKey = MAKELONG(pPassiveSkillNumber->nSkillLevel, pPassiveSkillNumber->nType);  // Id + Level 组合
		m_mapPassiveSkillNumbers[nKey] = pPassiveSkillNumber;
	}

	// 药物技能数值
	tabFile = g_OpenTabFile(MEDICINE_SKILL_NUMBER_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		MedicineSkillNumber* pMedicineSkillNumber = new MedicineSkillNumber();
		tabFile->GetInteger(i, "MedicineType", 0, &pMedicineSkillNumber->nType);
		tabFile->GetInteger(i, "SkillLevel", 0, &pMedicineSkillNumber->nSkillLevel);
		tabFile->GetInteger(i, "Value", 0, &pMedicineSkillNumber->nValue);
		tabFile->GetInteger(i, "NeedMedicineCount", 0, &pMedicineSkillNumber->nNeedMedicineCount);

		INT nKey = MAKELONG(pMedicineSkillNumber->nSkillLevel, pMedicineSkillNumber->nType);  // Id + Level 组合
		m_mapMedicineSkillNumbers[nKey] = pMedicineSkillNumber;
	}

	// 抗体技能技能数值
	tabFile = g_OpenTabFile(RESIST_SKILL_NUMBER_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		ResistSkillNumber* pResistSkillNumber = new ResistSkillNumber();
		tabFile->GetInteger(i, "ResistType", 0, &pResistSkillNumber->nType);
		tabFile->GetInteger(i, "SkillLevel", 0, &pResistSkillNumber->nSkillLevel);
		tabFile->GetInteger(i, "Value", 0, &pResistSkillNumber->nValue);
		tabFile->GetInteger(i, "ResistSkillId", 0, &pResistSkillNumber->nResistSkillId);

		INT nKey = MAKELONG(pResistSkillNumber->nSkillLevel, pResistSkillNumber->nType);  // Id + Level 组合
		m_mapResistSkillNumbers[nKey] = pResistSkillNumber;

		nKey = MAKELONG(pResistSkillNumber->nSkillLevel, pResistSkillNumber->nResistSkillId);  // Id + Level 组合
		m_mapResistSkillNumbersBySkillIds[nKey] = pResistSkillNumber;
	}

	// 主动技能数值
	tabFile = g_OpenTabFile(INACTIVE_SKILL_NUMBER_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 2; i <= nHeight; i++)
	{
		InactiveSkillNumber* pInactiveSkillNumber = new InactiveSkillNumber();
		tabFile->GetInteger(i, "SkillId", 0, &pInactiveSkillNumber->nSkillId);
		tabFile->GetInteger(i, "ResistSkillId", 0, &pInactiveSkillNumber->nResistSkillId);
		tabFile->GetInteger(i, "SkillLevel", 0, &pInactiveSkillNumber->nSkillLevel);
		tabFile->GetInteger(i, "BufferIncroRate", 0, &pInactiveSkillNumber->nBufferIncroRate);
		tabFile->GetInteger(i, "BufferExistTime", 0, &pInactiveSkillNumber->nBufferExistTime);
		tabFile->GetInteger(i, "BufferValue", 0, &pInactiveSkillNumber->nBufferValue);
		tabFile->GetInteger(i, "BufferId", 0, &pInactiveSkillNumber->nBufferId);
		tabFile->GetInteger(i, "SkillCoefficient", 0, &pInactiveSkillNumber->nSkillCoefficient);
		tabFile->GetInteger(i, "ManaCost", 0, &pInactiveSkillNumber->nManaCost);
		tabFile->GetInteger(i, "HarmValue", 0, &pInactiveSkillNumber->nHarmValue);
		tabFile->GetInteger(i, "AddBuffToSelf", 0, &pInactiveSkillNumber->nAddBuffToSelf);
		tabFile->GetInteger(i, "AddAnger", 0, &pInactiveSkillNumber->nAddAnger);

		INT nKey = MAKELONG(pInactiveSkillNumber->nSkillLevel, pInactiveSkillNumber->nSkillId);  // Id + Level 组合
		m_mapInactiveSkillNumbers[nKey] = pInactiveSkillNumber;
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL FightSkillSetting::SplitString(const std::string& src, const std::string &separator, std::vector<string> &dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return TRUE;
		}
	}while(index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);

	return TRUE;
}

FightSkillTemplate* FightSkillSetting::GetFightSkillTemplate(WORD wTemplateID)
{
	if (m_mapFightSkills.find((INT)wTemplateID) == m_mapFightSkills.end())
		return NULL;
	return m_mapFightSkills[(INT)wTemplateID];
}

// 获取抗体技能数值
ResistSkillNumber* FightSkillSetting::GetResistSkillNumber(INT nResistType, INT nSkillLevel)
{
	INT nKey = MAKELONG(nSkillLevel, nResistType);

	if (m_mapResistSkillNumbers.find(nKey) == m_mapResistSkillNumbers.end())
		return NULL;
	return m_mapResistSkillNumbers[nKey];
}

ResistSkillNumber* FightSkillSetting::GetResistSkillNumberByResistSkillId(INT nResistSkillId, INT nSkillLevel)
{
	INT nKey = MAKELONG(nSkillLevel, nResistSkillId);

	if (m_mapResistSkillNumbersBySkillIds.find(nKey) == m_mapResistSkillNumbersBySkillIds.end())
		return NULL;
	return m_mapResistSkillNumbersBySkillIds[nKey];

}

// 获取被动技能数值
PassiveSkillNumber* FightSkillSetting::GetPassiveSkillNumber(INT nPassiveType, INT nSkillLevel)
{
	INT nKey = MAKELONG(nSkillLevel, nPassiveType);

	if (m_mapPassiveSkillNumbers.find(nKey) == m_mapPassiveSkillNumbers.end())
		return NULL;
	return m_mapPassiveSkillNumbers[nKey];
}

// 获取主动技能数值
InactiveSkillNumber* FightSkillSetting::GetInactiveSkillNumber(INT nSkillId, INT nSkillLevel)
{
	INT nKey = MAKELONG(nSkillLevel, nSkillId);

	if (m_mapInactiveSkillNumbers.find(nKey) == m_mapInactiveSkillNumbers.end())
		return NULL;
	return m_mapInactiveSkillNumbers[nKey];
}

// 获取吃药技能数值
MedicineSkillNumber* FightSkillSetting::GetMedicineSkillNumber(INT nMedicineType, INT nSkillLevel)
{
	INT nKey = MAKELONG(nSkillLevel, nMedicineType);

	if (m_mapMedicineSkillNumbers.find(nKey) == m_mapMedicineSkillNumbers.end())
		return NULL;
	return m_mapMedicineSkillNumbers[nKey];
}

INT FightSkillSetting::GetSkillCdTime(INT nSkillTemplateId)
{
	FightSkillTemplate* pTemplate = this->GetFightSkillTemplate(nSkillTemplateId);
	QCONFIRM_RET_NULL(pTemplate);

	return pTemplate->nTimePerCast;
}

// 获取技能释放总时长（参数可选是否包括导弹技能， 填距离）
INT FightSkillSetting::GetSkillCastTime(INT nSkillTemplateId, INT nDistance)
{
	FightSkillTemplate* pTemplate = this->GetFightSkillTemplate(nSkillTemplateId);
	QCONFIRM_RET_NULL(pTemplate);
	
	INT nSkillCastTime = 0;

	if (pTemplate->nFightSkillChannel > FIGHT_SKILL_CHANNEL_NO)  // 引导技能
	{
		nSkillCastTime = pTemplate->nChannelTime + pTemplate->nTimeBeforeCast;
	}
	else
	{
		if (pTemplate->nFightSkillType == FIGHT_SKILL_MISSILE)  // TODO: 闪电链
		{
			// 导弹技能还需要计算导弹飞行时间
			// 时间(*/s) = 总路程（逻辑距离） / 速度(逻辑速度)
			INT nSpeed = pTemplate->fMissileSpeed * GAME_FPS;
			nSkillCastTime = nDistance / nSpeed;
		}
		else
		{
			// 非引导技能, buffer技能
			nSkillCastTime = pTemplate->nTimeBeforeCast;
		}
	}
	
	return nSkillCastTime;
}
