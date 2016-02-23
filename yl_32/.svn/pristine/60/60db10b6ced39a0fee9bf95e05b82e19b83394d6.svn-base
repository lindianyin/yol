/* -------------------------------------------------------------------------
//	文件名		：	playerfaction.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-25 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef playerfaction_h__
#define playerfaction_h__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "../onlinegamemodule/fightskilldef.h"
#include "../onlinegamemodule/faction/factiondef.h"

#define KD_SKILL_SLOT_SIZE 5

struct FACTION_PLAYER_SETTING;
class PlayerFaction : public KCharacterCtrlBase
{
public:
	PlayerFaction(KCharacter& rcPlayer);

	~PlayerFaction();

	enum { emKCtrlId = emCHARACTER_CTRL_FACTION };

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	BOOL InitAttrs();

	// 存盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 读盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	// 设置药槽
	BOOL SetMeidicineSlot(INT nSlot, INT nRoom, INT nX, INT nY);

	// 改变职业路线
	BOOL OnFactionRouteChange(INT nLevel, INT nFaction, INT nRoute);

	BOOL OnLevelChange(INT nOldLevel, INT nNewLevel);

	VOID SetFactionSetting(FACTION_PLAYER_SETTING setting){ m_cSetting = setting; }

	CONST FACTION_PLAYER_SETTING& GetFactionSetting() { return m_cSetting; }

	//BOOL ChangeRune(INT nSlot, INT nRune);

	//BOOL SyncSkillRuneIds();

	//BOOL SyncMedicineSlots();

	

private:

	//INT m_aryRuneId[emALL_SKILL_COUNT];  // 符文选择

	//PlayerMedicineSlot m_PlayerMedicineSlots[QD_PLAYER_MEDICINE_SLOT_SIZE];  // 药槽

	BOOL m_bDbLoaded;    // 判断是否已经读取数据库~~~ 刚进入游戏时 SetLevel会引起ReloadPlayerSkills, 但SetLevel比DbLoad优先，所以会导致还没读取人物技能等级数据就传递错误技能
	VOID LifeAutoRecover();

	VOID ManaAutoRecover();

	INT GetSkillSlotRuneKey(INT nSlot, INT nRune);  // 返回槽位符文组合键

private:
	FACTION_PLAYER_SETTING m_cSetting;
};

#endif // playerfaction_h__