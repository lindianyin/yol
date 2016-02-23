/* -------------------------------------------------------------------------
//	文件名		：	fightskillprotocolprocessor.h
//	创建者		：	zhangzhixiong
//	创建时间	：	20012-5-21 
//	实现者		：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef fightskillprotocolprocessor_h__
#define fightskillprotocolprocessor_h__

class KCharacter;

class FightSkillProtocolProcessor
{
public:
	static BOOL BroadcastStartSkill(KCharacter& rcLauncher, DWORD dwTargetId,WORD wSkillTemplateId,INT nX,INT nY,INT nZ, BOOL bSyncSelf);

	static BOOL BroadcastCancelChannelSkill(KCharacter& rcLauncher, WORD wSkillTemplateId);

	static BOOL BroadcastChangeChannelState(KCharacter& rcLauncher, WORD wSkillTemplateId, WORD wType, DWORD dwValue);

	static BOOL ProcessC2S_UpgradeSkill(INT nPlayerIndex, LPCBYTE pProtocol);

	//static BOOL ProcessC2S_ChangeRune(INT nPlayerIndex, LPCBYTE pProtocol);

	static BOOL ProcessC2S_CancelChannelSkill(INT nPlayerIndex, LPCBYTE pProtocol);

	static BOOL ProtocolProcess(INT nPlayerIndex, LPCBYTE pProtocol, UINT uDataLen);

	static BOOL ProcessC2S_RequestStartSkill(INT nPlayerIndex, LPCBYTE pProtocol);

	static BOOL ProcessC2S_ChangeFactionRoute(INT nPlayerIndex, LPCBYTE pProtocol);

	static BOOL SyncS2C_AddSkill(INT nPlayerIdx, DWORD dwCharacterId, INT nSkillId);

	static BOOL SyncS2C_RemoveSkill(INT nPlayerIdx, DWORD dwCharacterId, INT nSkillId);

	static BOOL SyncS2C_ClearCD(INT nPlayerIdx, DWORD dwCharacterId, INT nSkillId);

	static BOOL SyncS2C_SyncSkills(INT nPlayerIdx, DWORD dwCharacterId, WORD wSkillsCount, WORD wSkills[], WORD wSkillsLevels[]);

	//static BOOL SyncS2C_SyncSkills2(INT nPlayerIdx, DWORD dwCharacterId, WORD wSkillsCount, WORD wSkills[], INT wSkillsArraySize);

	//static BOOL SyncS2C_SyncRuneIds(INT nPlayerIdx, INT wRuneIds[]);
};
#endif // fightskillprotocolprocessor_h__

