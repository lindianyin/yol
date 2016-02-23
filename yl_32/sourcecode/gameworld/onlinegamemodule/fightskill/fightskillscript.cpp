#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "fightskill/fightskillsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KFightSkillScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetYlEnery
	// @Description		: 获得游龙真气
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nSkillId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetYlEnery(QLuaScript &sc)
	{
		INT nSkillId = sc.GetInt(1);

		FightSkillTemplate* pTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nSkillId);
		QCONFIRM_RET_FALSE(pTemplate);

		sc.PushNumber(pTemplate->nYlEnergy);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMeidicineSkillNumber
	// @Description		: 获得药物技能相关数值
	// @ReturnCode		: 
	// @ArgumentFlag	: dd
	// @ArgumentComment	: nMedicineType
	// @ArgumentComment	: nSkillLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMeidicineSkillNumber(QLuaScript &sc)
	{
		INT nMedicineType = sc.GetInt(1);
		INT nSkillLevel = sc.GetInt(2);

		MedicineSkillNumber* pTemplate = g_cFightSkillSetting.GetMedicineSkillNumber(nMedicineType, nSkillLevel);
		QCONFIRM_RET_FALSE(pTemplate);

		sc.PushNumber(pTemplate->nValue);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPassiveSkillNumber
	// @Description		: 获得被动技能相关数值
	// @ReturnCode		: 
	// @ArgumentFlag	: dd
	// @ArgumentComment	: nPassiveType
	// @ArgumentComment	: nSkillLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPassiveSkillNumber(QLuaScript &sc)
	{
		INT nPassiveType = sc.GetInt(1);
		INT nSkillLevel = sc.GetInt(2);

		PassiveSkillNumber* pTemplate = g_cFightSkillSetting.GetPassiveSkillNumber(nPassiveType, nSkillLevel);
		QCONFIRM_RET_FALSE(pTemplate);

		sc.PushNumber(pTemplate->nValue);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetResistSkillNumber
	// @Description		: 获得抗体技能相关数值
	// @ReturnCode		: 
	// @ArgumentFlag	: dd
	// @ArgumentComment	: nPassiveType
	// @ArgumentComment	: nSkillLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetResistSkillNumber(QLuaScript &sc)
	{
		INT nResistType = sc.GetInt(1);
		INT nSkillLevel = sc.GetInt(2);

		ResistSkillNumber* pTemplate = g_cFightSkillSetting.GetResistSkillNumber(nResistType, nSkillLevel);
		QCONFIRM_RET_FALSE(pTemplate);

		sc.PushNumber(pTemplate->nValue);

		return 1;
	}
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetSkillSlot
	// @Description		: 获得技能槽位信息
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nSkillId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetSkillSlot(QLuaScript &sc)
	{
		INT nSkillId = sc.GetInt(1);

		FightSkillTemplate* pTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nSkillId);
		QCONFIRM_RET_FALSE(pTemplate);

		sc.PushNumber(pTemplate->nSlot);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetSkillRoute
	// @Description		: 获得技能路线
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nSkillId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetSkillRoute(QLuaScript &sc)
	{
		INT nSkillId = sc.GetInt(1);

		FightSkillTemplate* pTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nSkillId);
		QCONFIRM_RET_FALSE(pTemplate);

		sc.PushNumber(pTemplate->nRouteLimit);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetReqLevel
	// @Description		: 获得技能需求等级
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nSkillId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetReqLevel(QLuaScript &sc)
	{
		INT nSkillId = sc.GetInt(1);

		FightSkillTemplate* pTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nSkillId);
		QCONFIRM_RET_FALSE(pTemplate);

		sc.PushNumber(pTemplate->nReqLevel);

		return 1;
	}

	#include "fightskillscript_table.hpp"
}

BOOL g_RegisterFightSkillScript()
{
	return g_cScriptManager.RegisterTableFuncs("KFightSkill", KFightSkillScript::arFunction, countof(KFightSkillScript::arFunction));
}