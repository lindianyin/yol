/********************************************************************
	created:	2012/05/28  9:12
	filename: 	characterskill.h
	author:		zhangjunjie
*********************************************************************/
#ifndef __CHARACTER_SKILL_H__
#define __CHARACTER_SKILL_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"

#include "onlinegamemodule/luacharacterskill.h"
#include "../onlinegamemodule/fightskilldef.h"
//#include "onlinegameworld/kcharacter.h"
//#include "onlinegameworld/kplayermgr.h"
#include <map>

class FightSkill;
struct QDelaySkillData;

enum SKILL_SLOT_DEF
{
	PASSIVE_SKILL_SLOT = 6,
};

#pragma pack(push, 1)
struct SkillSaveData
{
	SkillSaveData(INT nSkillId = 0,INT nCDTime = 0,INT nCurrentCDTime = 0, INT nSkillLevel=1)
		:nSkillId(nSkillId), nCDTime(nCDTime), nCurrentCDTime(nCurrentCDTime), nSkillLevel(nSkillLevel)
	{}
	INT nSkillId;
	INT nCDTime;			// CD时间
	INT nCurrentCDTime;		// 当前cd时间
	INT nSkillLevel;	// 技能等级
};

// 技能等级保存
struct SkillLevelDbSaveData
{
	SkillLevelDbSaveData(INT nSkillTemplateId = 0, INT nGrade = 0)
		:nSkillTemplateId(nSkillTemplateId), nGrade(nGrade)
	{}
	INT nSkillTemplateId;				// 槽位
	INT nGrade;							// 等级
};
#pragma pack(pop)


class CharacterSkill: public KCharacterCtrlBase
{
public:
	static CONST INT COMMON_SKILL_COOLDOWN = 11; // 公共CD定义
	//static CONST INT SKILL_CHANGE_RUNE_COOLDOWN = 16; // 符文切换冷却CD

	struct SKILLDATA
	{
		SKILLDATA(INT nSkillId = 0,INT nSkillLevel=1, INT nCDTime = 0,INT nCurrentCDTime = 0)
			:nSkillId(nSkillId), nSkillLevel(nSkillLevel), nCDTime(nCDTime), nCurrentCDTime(nCurrentCDTime),pSkill(NULL),bStartCalcCD(FALSE)
		{}
		INT nSkillId;
		FightSkill* pSkill;		// 技能
		INT nCDTime;			// CD时间
		INT nCurrentCDTime;		// 当前cd时间
		BOOL bStartCalcCD;		// 是否开始计算CD时间
		
		INT nSkillLevel;			// 技能等级

		INT nSlotId;  // 保存槽ID和符文ID
		//INT nRuneId;
	};

	typedef std::map<INT,SKILLDATA*> SKILLDATAMAP;
	typedef std::map<INT, INT> FACTIONSKILLMAP;  // SkillId -> Level

	CharacterSkill(KCharacter& rcPlayer);

	~CharacterSkill();

	virtual BOOL Active();

	VOID ForwardFrame(INT nFrame);

	INT GetCastingSkill();

	enum { emKCtrlId = emKCHARACTER_CTRL_FIGHTSKILL };

	BOOL m_bDbLoaded;    // 判断是否已经读取数据库~~~ 刚进入游戏时 SetLevel会引起ReloadPlayerSkills, 但SetLevel比DbLoad优先，所以会导致还没读取人物技能等级数据就传递错误技能
protected:
	virtual BOOL Init();
	virtual BOOL UnInit();	
	virtual BOOL OnLogin();
	virtual BOOL OnLogout()	{return TRUE;}
	// 存盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	// 读盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	virtual BOOL OnClientReady();

public:

	// 获取技能等级
	INT GetSkillLevel(INT nSkillTemplateId);

	// 升级技能
	BOOL UpgradeSkill(DWORD nSkillTemplateId);
	// 获取抗性技能的等级
	INT GetResistSkillLevel(INT nResistType);
	// 刷新传送技能
	BOOL ReloadPlayerSkills();

	// 判断有无此技能
	BOOL HasSkill(WORD wSkillId);
	// 添加一个指定Id的技能
	BOOL AddSkill(WORD nSkillId, BOOL bSync = TRUE, INT nSkillLevel = 1);
	// 应用被动技能
	BOOL SetPassiveSkill(INT nPassiveType, INT nSkillLevel, BOOL bAddOrRemove, BOOL bSync = TRUE);
	// 添加相应等级的被动技能数值
	BOOL AddPassiveSkillValue(INT nPassiveType, INT nSkillLevel, BOOL bTakeBackNumber = FALSE, BOOL bSync = FALSE);
	// 应用被动技能
	/*BOOL ApplyPassiveSkill();*/
	//删除一个技能
	BOOL RemoveSkill(WORD nSkillId, BOOL bSync = TRUE);
	// 清空所有技能，不会同步给客户端
	BOOL ClearSkill();
	// 清空技能CD
	BOOL ClearSkillCD(WORD wSkillId);
	//是否可以放当前的技能
	BOOL CanCastSkill(WORD wSkillId,DWORD dwTargetId, INT nX, INT nY, INT nZ);
	// 检查技能释放距离
	BOOL CheckSkillCastDistance(WORD wSkillId, DWORD dwTargetId, INT nX, INT nY, INT nZ);
	//当前技能是否在CD中
	BOOL IsSkillInCD(WORD wSkillId);

	// 冷却技能, 冷却符文切换
	BOOL CooldownSkill(WORD wSkillId);
	// 释放技能前调整朝向
	VOID TurnToBeforeCast(KCharacter* pCaster, KCharacter* pTarget);

	//释放技能
	BOOL CastSkill(WORD wSkillId,DWORD dwTargetId,INT nX,INT nY,INT nZ);
	BOOL CastSkill(WORD wSkillId,DWORD dwTargetId,INT nX,INT nY,INT nZ, BOOL syncSelf = FALSE, BOOL bSpecialTimeScale = FALSE);

		// 升级被动技能时删除原技能属性用rawset
	VOID RawSetLock();
	VOID RawSetUnLock();

	// 取消引导技能
	BOOL CancelChannelSkill(WORD wSkillId);

	// 添加技能前操作
	BOOL PreAddSkill(WORD wSkillId);

	// 获得技能map
	CONST SKILLDATAMAP& GetSkillMap() CONST;
	SKILLDATAMAP& GetSkillMap();

	LuaCharacterSkill* GetScriptInterface() CONST;

	// 检查技能升级消耗
	BOOL CheckSkillUpgradeCost(DWORD dwSkillTemplateId);

	/*
	* 给技能添加会心百分比
	*/
	VOID AddSkillCritical(WORD wCateType, WORD wCateId, UINT uPercent);

	/*
	* 减少技能会心百分比
	*/
	VOID DelSkillCritical(WORD wCateType, WORD wCateId, UINT uPercent);

	/*
	* 获得技能会心百分比
	*/
	INT GetSkillCritical(WORD wCateType, WORD wCateId);

	BOOL SaveDelaySkillData(QDelaySkillData* pDelaySkillData);

	INT GetSkillType(INT nSkillId);

	BOOL SyncSkillsToClient(BOOL resetCD = FALSE);

	BOOL CheckObstacle(INT nStartX, INT nStartY, INT nStartZ, INT nEndX, INT nEndY, INT nEndZ, INT nTouchRange, INT& nLastOkX, INT& nLastOkY);
	BOOL CheckObstacle(INT nStartX, INT nStartY, INT nStartZ, INT nEndX, INT nEndY, INT nEndZ, INT nTouchRange);
	BOOL CheckObstacle(DWORD dwTargetId);

public:
	INT m_LastChangeRouteTime;

private:
	//typedef std::map<INT, INT> SKILLLEVELMAP;
	//SKILLLEVELMAP m_mapSkillLevelData;  // 记录角色技能等级设置 SLOT->等级

	FACTIONSKILLMAP m_mapSkillLevelData;  // 记录角色技能等级设置
	SKILLDATAMAP m_mapSkillData;
	
	LuaCharacterSkill* m_pLunaObj;

	BOOL m_bMapLoadEmpty;	//如果加载的数据库为空，说明是新角色，在OnClientReady就不会同步了

	std::map<DWORD, INT> m_tSkillCriticalP;		//增加暴击率的Skill类型,KEY=MAKEWORD(nCateType, nCateId)
	
	BOOL m_arrPassiveSkill[emSKILL_ATTR_COUNT];  //记录被动技能状态

	BOOL ApplyMedicineSkill(INT nSkillSlot, INT nSkillLevel);  // 吃药技能
};


#endif