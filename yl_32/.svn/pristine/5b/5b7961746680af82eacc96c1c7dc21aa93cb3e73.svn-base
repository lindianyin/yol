
#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "kluapet.h"
#include "kpet.h"
#include "kpetctrl.h"
#include "onlinegameworld/kplayermgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: Name
// @Description		: 宠物名字
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaPet::getName(VOID) CONST
{
	return	m_pPet->GetPetName();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: Id
// @Description		: 宠物Id
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaPet::getId(VOID) CONST
{
	return	m_pPet->GetId();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: RandSeed
// @Description		: 宠物随机种子
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaPet::getRandSeed(VOID) CONST
{
	return	m_pPet->GetRandSeed();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: OwnerId
// @Description		: 宠物主人Id
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaPet::getOwnerId(VOID) CONST
{
	return	m_pPet->GetOwnerId();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: Type
// @Description		: 宠物种类
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::getType(VOID) CONST
{
	return	m_pPet->GetType();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: Quality
// @Description		: 宠物品质
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::getQuality(VOID) CONST
{
	return	m_pPet->GetQuality();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: Character
// @Description		: 宠物性格
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::getCharacter(VOID) CONST
{
	return	m_pPet->GetCharacter();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: Hungry
// @Description		: 宠物饱食度
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::getHungry(VOID) CONST
{
	return	m_pPet->GetHungry();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: Level
// @Description		: 宠物等级
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::getLevel(VOID) CONST
{
	return	m_pPet->GetLevel();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: RankLevel
// @Description		: 宠物阶级
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::getRankLevel(VOID) CONST
{
	return	m_pPet->GetRankLevel();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: State
// @Description		: 宠物状态
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::getState(VOID) CONST
{
	return	m_pPet->GetPetState();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: AddExp
// @Description		: 增加经验
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加经验值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPet::LuaAddExp(QLuaScript& sc)
{
	INT nExp = sc.GetInt(1);
	m_pPet->AddExp(nExp);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: GetTotalExp
// @Description		: 得到宠物的总经验
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPet::LuaGetTotalExp(QLuaScript& sc)
{
	INT nLevel = sc.GetInt(1);
	DWORD dwExp = m_pPet->GetTotalExp(nLevel);
	sc.PushNumber(dwExp);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: GetBaseAttr
// @Description		: 得到宠物基础属性
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPet::LuaGetBaseAttr(QLuaScript& sc)
{
	sc.PushTable();

	INT nLife = m_pPet->GetBaseAttrValue(0);
	sc.PushNumber(nLife);
	sc.SetTableField("nLife");

	INT nAttack = m_pPet->GetBaseAttrValue(1);
	sc.PushNumber(nAttack);
	sc.SetTableField("nAttack");

	INT nHit = m_pPet->GetBaseAttrValue(2);
	sc.PushNumber(nHit);
	sc.SetTableField("nHit");

	INT nCritical = m_pPet->GetBaseAttrValue(3);
	sc.PushNumber(nCritical);
	sc.SetTableField("nCritical");

	INT nResist = m_pPet->GetBaseAttrValue(4);
	sc.PushNumber(nResist);
	sc.SetTableField("nResist");

	INT nArmor = m_pPet->GetBaseAttrValue(5);
	sc.PushNumber(nArmor);
	sc.SetTableField("nArmor");

	INT nDodge = m_pPet->GetBaseAttrValue(6);
	sc.PushNumber(nDodge);
	sc.SetTableField("nDodge");

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPet
// @LuaApiName		: AddHungry
// @Description		: 增加饱食度
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 饱食度
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPet::LuaAddHungry(QLuaScript& sc)
{
	INT nHungry = sc.GetInt(1);

	nHungry += m_pPet->GetHungry();
	MAKE_IN_RANGE(nHungry, 0, KD_PET_HUNGRY_MAX);

	m_pPet->SetHungry(nHungry);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPet
// @LuaApiName		: Regenerate
// @Description		: 重新生成一只宠物
// @ReturnCode		: 成功返回1
// @ArgumentFlag	: dddddu
// @ArgumentComment	: nType		
// @ArgumentComment	: nQuality	
// @ArgumentComment	: nCharacter
// @ArgumentComment	: nLevel
// @ArgumentComment	: nRankLevel
// @ArgumentComment	: uRandSeed
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::LuaRegenerate(QLuaScript& sc)
{
	INT nType			= sc.GetInt(1);
	INT nQuality		= sc.GetInt(2);
	INT nCharacter	= sc.GetInt(3);
	INT nLevel			= sc.GetInt(4);
	INT nRankLevel		= sc.GetInt(5);

	UINT uRandSeed = 0;
	if(sc.IsNumber(6))
		uRandSeed = sc.GetInt(6);

	BOOL bRet = m_pPet->Regenerate(
		nType,
		nQuality,
		nCharacter, 
		nLevel, 
		nRankLevel, 
		uRandSeed);

	sc.PushNumber(bRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPet
// @LuaApiName		: Sync
// @Description		: 同步宠物数据到客户端
// @ReturnCode		: 1成功
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::LuaSync(QLuaScript& sc)
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_pPet->GetOwnerId());
	QCONFIRM_RET_FALSE(pPlayer);

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	BOOL nRet = pPetCtrl->SyncPetInfo(m_pPet);

	sc.PushNumber(nRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPet
// @LuaApiName		: UnlockEquipPos
// @Description		: 解锁装备格子
// @ReturnCode		: 1成功
// @ArgumentFlag	: d
// @ArgumentComment	: 装备槽位置
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::LuaUnlockEquipPos(QLuaScript& sc)
{
	INT nPos = sc.GetInt(1);

	BOOL nRet = m_pPet->UnLockEquipPos(nPos);

	sc.PushNumber(nRet);

	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPet
// @LuaApiName		: GetSkillLevel
// @Description		: 得到技能等级
// @ReturnCode		: 1成功
// @ArgumentFlag	: d
// @ArgumentComment	: 技能id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::LuaGetSkillLevel(QLuaScript& sc)
{
	INT nSkillId = sc.GetInt(1);

	INT nSkillLevel = m_pPet->GetSkillLevel(nSkillId);

	sc.PushNumber(nSkillLevel);

	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPet
// @LuaApiName		: GetBaseAttrScore
// @Description		: 得到基础属性战斗力
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::LuaGetBaseAttrScore(QLuaScript& sc)
{
	INT nFightScore = m_pPet->GetBaseAttrScore();
	sc.PushNumber(nFightScore);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPet
// @LuaApiName		: SetFightScore
// @Description		: 设置宠物战斗力
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nScore
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::LuaSetFightScore(QLuaScript& sc)
{
	INT nScore = sc.GetInt(1);
	m_pPet->SetFightScore(nScore);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPet
// @LuaApiName		: DoRest
// @Description		: 设置当前出战神仙休息
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPet::LuaDoRest(QLuaScript& sc)
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_pPet->GetOwnerId());
	QCONFIRM_RET_FALSE(pPlayer);

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	QCONFIRM_RET_FALSE(m_pPet->GetPetState() == emPET_STATE_FIGHT);

	pPetCtrl->OnChangeStateRequest(m_pPet, emPET_STATE_REST);

	return 0;
}


#include "kluapet_table.hpp"