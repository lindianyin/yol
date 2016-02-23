/* -------------------------------------------------------------------------
//	文件名		：	fightskillmanager.h
//	创建者		：	zhangzhixiong
//	实现者		：	
//	创建时间	：	2011.5.21
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef fightskillmanager_h__
#define fightskillmanager_h__

#include "fightskill.h"
#include "onlinegameworld/kidobjpool.h"

class FightSkillManager
{
public:
	FightSkillManager();

	~FightSkillManager();

	BOOL Init();

	BOOL UnInit();

	BOOL Breath();

	// 是否可以释放技能
	BOOL CanCastSkill(WORD wSkillTemplateId, DWORD dwLauncherId, DWORD dwTargetId, INT nX, INT nY, INT nZ);

	// 释放技能
	BOOL CastSkill(WORD wSkillTemplateId, DWORD dwLauncherId, DWORD dwTargetId,INT nX,INT nY,INT nZ, BYTE bTimeScale = 0);

	// 取消引导技能
	BOOL CancelChannelSkill(WORD wSkillTemplateId, DWORD dwLauncherId);

	BOOL RemoveSkill(DWORD dwLauncherId,WORD wSkillId);

	BOOL AddSkill(DWORD dwLauncherId,WORD wSkillId);

	/*
	* 添加指定职业的默认技能、特定技能
	*/
    BOOL _GetCharacterAvailableSkills(INT nFaction, INT nLevel, std::vector<FightSkillTemplate*>& outSkills);

	// 得到某一门派路线下指定等级所能拥有的角色技能数据
	BOOL GetCharacterAvailableSkills(INT nFaction, INT nCharacterLevel, std::vector<FightSkillTemplate*>& outSkills);


public:
	BOOL m_bInit;
};

extern FightSkillManager g_FightSkillManager;

#endif // fightskillmanager_h__