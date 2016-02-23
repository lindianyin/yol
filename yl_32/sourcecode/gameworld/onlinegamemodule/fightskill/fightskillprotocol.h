/* -------------------------------------------------------------------------
//	文件名		：	fightskillprotocol.h
//	创建者		：	zhangzhixiong
//	创建时间	：	20012-5-21 9:36:58
//	实现者		：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __FIGHTSKILLPROTOCOL_H__
#define __FIGHTSKILLPROTOCOL_H__

// -------------------------------------------------------------------------
#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)
// -------------------------------------------------------------------------
enum FIGHT_SKILL_C2S_PROTOCOL
{
	emFIGHT_SKILL_PTC_C2S_NONE = 0,
	emFIGHT_SKILL_PTC_C2S_REQUESTSTARTSKILL,     // 申请开始释放技能
	emFIGHT_SKILL_PTC_C2S_CANCELCHANNELSKILL,    // 取消引导技能
	emFIGHT_SKILL_PTC_C2S_CHANGE_FACTION_ROUTE,   // 切换职业路线
	emFIGHT_SKILL_PTC_C2S_UPGRADE_SKILL,					// 技能升级
};

struct PTC_FIGHT_SKILL_REQUESTSTARTSKILL : KPTC_HEADER<emFIGHT_SKILL_PTC_C2S_REQUESTSTARTSKILL>
{
	DWORD dwLauncherId;
	DWORD dwTargetId;
	WORD  wSkillTemplateId;
	INT	  nX;
	INT   nY;
	INT	  nZ;
	DWORD dwClientFrame;  // 客户端帧数
	BYTE bTimeScale; // 时间缩放
};

struct PTC_FIGHT_SKILL_C2S_CANCEL_CHANNEL_SKILL : KPTC_HEADER<emFIGHT_SKILL_PTC_C2S_CANCELCHANNELSKILL>
{
    DWORD dwCharacterId;
	WORD  wSkillTemplateId;
};

struct PTC_FIGHT_SKILL_C2S_CHANGE_FACTION_ROUTE : KPTC_HEADER<emFIGHT_SKILL_PTC_C2S_CHANGE_FACTION_ROUTE>
{
	DWORD dwCharacterId;
	BYTE  byFactionRoute;
};

struct PTC_FIGHT_SKILL_C2S_UPGRADE_SKILL : KPTC_HEADER<emFIGHT_SKILL_PTC_C2S_UPGRADE_SKILL>
{
	DWORD dwCharacterId;
	DWORD dwSkillTemplateId;
};


enum FIGHT_SKILL_S2C_PROTOCOL
{
	emFIGHT_SKILL_PTC_S2C_NONE = 0,
	emFIGHT_SKILL_PTC_S2C_STARTSKILL,			// 开始释放技能
	emFIGHT_SKILL_PTC_S2C_ADDSKILL,				// 添加一个技能
	emFIGHT_SKILL_PTC_S2C_REMOVESKILL,			// 删除一个技能
	emFIGHT_SKILL_PTC_S2C_CANCELCHANNELSKILL,   // 取消引导技能
	emFIGHT_SKILL_CLEAR_CD,						// 清除技能CD
	emKPTC_S2C_CHARACTER_REVIVE,				// 复活，临时放在这里
	emKPTC_S2C_CHARACTER_KILLED,				// 死亡，临时放在这里
	emFIGHT_SKILL_PTC_S2C_CHANGECHANNELSTATE,		// 改变技能引导类型
	emFIGHT_SKILL_PTC_S2C_SYNCSKILL,
	//emFIGHT_SKILL_PTC_S2C_SYNC_RUNE,			// 同步技能符文
};

// 用于同步技能
struct SKILL_ID_LEVEL
{
	SKILL_ID_LEVEL(INT nSkillId=0, INT nSkillLevel=1) : nSkillId(nSkillId), nSkillLevel(nSkillLevel) {}

	INT nSkillId;
	INT nSkillLevel;
};
enum {KD_SYNC_SKILLS_MAX_COUNT = 128};
// 同步可用技能
struct PTC_FIGHT_S2C_SKILL_SYNCSKILL : KLENPTC_HEADER<emFIGHT_SKILL_PTC_S2C_SYNCSKILL>
{
	
	DWORD dwCharacterId;
	WORD wSkillsCount;
	SKILL_ID_LEVEL awSkills[0];

	//INT GetSize() CONST
	//{
	//	return sizeof(PTC_FIGHT_S2C_SKILL_SYNCSKILL) + sizeof(SKILL_ID_LEVEL) * wSkillsCount;  // 等级和
	//}
};

// -------------------------------------------------------------------------
struct PTC_FIGHT_SKILL_STARTSKILL : KPTC_HEADER<emFIGHT_SKILL_PTC_S2C_STARTSKILL>
{
	DWORD dwLauncherId;
	DWORD dwTargetId;
	WORD  wSkillTemplateId;
	INT	  nX;
	INT   nY;
	INT	  nZ;
};

struct PTC_FIGHT_SKILL_ADDSKILL : KPTC_HEADER<emFIGHT_SKILL_PTC_S2C_ADDSKILL>
{
	DWORD dwCharacterId;
	WORD nSkillId;
};

struct PTC_FIGHT_SKILL_REMOVESKILL : KPTC_HEADER<emFIGHT_SKILL_PTC_S2C_REMOVESKILL>
{
	DWORD dwCharacterId;
	WORD nSkillId;
};

struct PTC_FIGHT_SKILL_CLEAR_CD : KPTC_HEADER<emFIGHT_SKILL_CLEAR_CD>
{
	DWORD dwCharacterId;
	WORD nSkillId;
};

struct PTC_FIGHT_SKILL_S2C_CANCEL_CHANNEL_SKILL : KPTC_HEADER<emFIGHT_SKILL_PTC_S2C_CANCELCHANNELSKILL>
{
	DWORD dwCharacterId;
	WORD wSkillId;
};

struct PTC_FIGHT_SKILL_S2C_CHANGE_CHANNEL_STATE : KPTC_HEADER<emFIGHT_SKILL_PTC_S2C_CHANGECHANNELSTATE>
{
	DWORD dwCharacterId;
	WORD wSkillId;
	WORD wType;
	DWORD dwValue;
};

//struct PTC_FIGHT_SKILL_S2C_SYNC_RUNE : KPTC_HEADER<emFIGHT_SKILL_PTC_S2C_SYNC_RUNE>
//{
//	BYTE byRuneIds[5];
//};

struct KS2C_CHARACTER_REVIVE : KPTC_HEADER<emKPTC_S2C_CHARACTER_REVIVE>
{
	DWORD dwReviveCharacterId;
};

struct KS2C_CHARACTER_KILLED : KPTC_HEADER<emKPTC_S2C_CHARACTER_KILLED>
{
	DWORD dwDeathCharacterId;
	DWORD dwKillerCharacterId;
};

// -------------------------------------------------------------------------
#pragma pack(pop)
#endif /* __KSKILLPROTOCOL_H__ */
