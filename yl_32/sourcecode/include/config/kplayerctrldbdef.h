/* -------------------------------------------------------------------------
//	文件名		：	config/kplayerctrldbdef.h
//	创建者		：	simon
//	创建时间	：	2009/12/24 17:02:38
//	功能描述	：	玩家Ctrl存盘定义
//
// -----------------------------------------------------------------------*/
#ifndef __CONFIG_KPLAYERCTRLDBDEF_H__
#define __CONFIG_KPLAYERCTRLDBDEF_H__

#include "kplayerdef.h"
#include "kcharacterdef.h"
#include "kplayerbasedbdef.h"
// -------------------------------------------------------------------------

// 存盘模块到PlayerCtrl映射
struct KPLAYER_SAVE_DEF 
{
	KE_DB_ROLE_BLOCK_TYPE	eBlockType;
	INT	nCtrlId;
	CHAR	szDesc[MAX_NAME_LEN]; // 描述/备注
};

// 这个数组可以增删、调整顺序，但从PlayerCtrlId到BlockType的对应关系不能改。
// 如果从这里删除一行映射，下次读盘时这个这块数据就没有Ctrl来处理，
// 也不会写回数据库，这块数据将永久损失，请慎重。
const static KPLAYER_SAVE_DEF gs_asPlayerSaveDef [] = 
{
	//{emKDRBT_FACTION,	emKCHARACTER_CTRL_FACTION, "FactionSaveCtrl"},
	{emKDRBT_TASK,	emKCHARACTER_CTRL_TASK, "TaskSaveCtrl"},
	{emKDRBT_ITEM,	emKCHARACTER_CTRL_PLAYER_ITEM, "ItemSaveCtrl"},
	//{emKDRBT_BUFFER, emKCHARACTER_CTRL_BUFFER,        "BufferSaveCtrl"},
	//{emKDRBT_RECIPE, emKCHARACTER_CTRL_PLAYER_RECIPE, "RecipeSaveCtrl"},
	{emKDRBT_PURSE,  emKCHARACTER_CTRL_PLAYER_PURSE,  "PurseSaveCtrl"},
	{emKDRBT_MAIL, emKCHARACTER_CTRL_PLAYER_MAIL, "MailCtrl"},
	{emDRBT_BUFFER, emCHARACTER_CTRL_BUFFER,    "YLBufferSaveCtrl"},
	{emDRBT_FIGHT_SKILL, emKCHARACTER_CTRL_FIGHTSKILL,  "YLSkillSaveCtrl"},
	{emDRBT_PET, emKCHARACTER_CTRL_PLAYER_PET, "PetSaveCtrl"},
	{emDRBT_FACTION, emCHARACTER_CTRL_FACTION, "YLFactionSaveCtrl"},
	{emDRBT_RIDE, emKCHARACTER_CTRL_PLAYER_RIDE, "RideSaveCtrl"},
	{emDRBT_AWARD, emKCHARACTER_CTRL_PLAYER_AWARD, "AwardSaveCtrl"},

	{emDRBT_QUIZ,emKCHARACTER_CTRL_PLAYER_QUIZ,"QuizSaveCtrl"},
	{emDRBT_ARENA,emKCHARACTER_CTRL_PLAYER_ARENA,"ArenaSaveCtrl"},

	{emDRBT_TOWER, emKCHARACTER_CTRL_PLAYER_TOWER, "TowerSaveCtrl"},
	{emDRBT_STAGE, emKCHARACTER_CTRL_PLAYER_STAGE, "StageSaveCtrl"},
	{emDRBT_MEDITATION, emKCHARACTER_CTRL_PLAYER_MEDITATION,"MeditationSaveCtrl"},
	{emDRBT_TITLE, emKCHARACTER_CTRL_PLAYER_TITLE, "TitleSaveCtrl"},
	{emDRBT_INSTANCE, emKCHARACTER_CTRL_PLAYER_INSTANCE, "InstanceSaveCtrl"},
	{emDRBT_ACTIVITY, emKCHARACTER_CTRL_PLAYER_ACTIVITY, "ActivitySaveCtrl"},
	{emDRBT_RANK, emKCHARACTER_CTRL_PLAYER_RANK, "RankSaveCtrl"},
	{emDRBT_TARGET, emKCHARACTER_CTRL_TARGET, "TargetSaveCtrl"},
	{emDRBT_VIP, emKCHARACTER_CTRL_VIP, "VipSaveCtrl"},
	{emDRBT_CHAT, emKCHARACTER_CTRL_PLAYER_CHAT, "ChatSaveCtrl"},
	{emDRBT_AWARD_LOBBY, emKCHARACTER_CTRL_AWARD_LOBBY, "AwardLobbySaveCtrl"},
	{emDRBT_GEM, emKCHARACTER_CTRL_GEM, "GemSaveCtrl"},
	{emDRBT_ARTIFACT, emKCHARACTER_CTRL_ARTIFACT, "ArtifactSaveCtrl"},
	{emDRBT_BANK, emKCHARACTER_CTRL_PLAYER_BANK, "BankSaveCtrl"},
	{emDRBT_SHOP, emKCHARACTER_CTRL_SHOP, "ShopSaveCtrl"},
	{emDRBT_OPENSE, emKCHARACTER_CTRL_OPENSE, "OpenseSaveCtrl" },
};

// -------------------------------------------------------------------------


#endif /* __CONFIG_KPLAYERCTRLDBDEF_H__ */
