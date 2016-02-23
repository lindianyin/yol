/* -------------------------------------------------------------------------
//	文件名		：	fightskill.h
//	创建者		：	zhangzhixiong
//	实现者		：	
//	创建时间	：	2011.5.22
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef fightskill_h__
#define fightskill_h__

#include "onlinegameworld/kbaseobject.h"

#define MISSILE_REACH_HEIGHT_OFFSET	2	//子弹到达多少高度的时候就算击中

class KCharacter;
struct FightSkillTemplate;

class FightSkill : public KBaseObject
{
	enum FIGHT_SKILL_STATE  // 技能运行阶段
	{
		SKILL_STOP,
		SKILL_CHANNEL_GOING,  // 引导中
		SKILL_CASTING, // 正常技能状态
	};

public:
	FightSkill();

	~FightSkill();

	BOOL Init(KCharacter* pCaster,WORD nTemplateId, INT nSkillLevel = 1);

	VOID StartSkill(); // 开始技能

	BOOL BindTarget(KCharacter* pTarget);

	VOID Breath();

	BOOL CheckRelation(KCharacter* pCaster, KCharacter* pTarget, BOOL outputWarning = TRUE);

	BOOL CheckCastConsume();

	BOOL CheckCastRelation();

	BOOL ProcessResult();

	FIGHT_SKILL_STATE GetFightSkillState() { return m_eSkillState; }

	//VOID SetActive(BOOL bActive) { m_bIsActive = bActive;}

	BOOL IsActive() const { return m_bIsActive;}

	VOID CancelChannelSkill();

	VOID SetDestPostion(INT nX,INT nY,INT nZ);

	VOID SetCurrentIncroBuffer(INT nIndex, INT nBufId);

	INT GetCurrentIncroBuffer(INT nIndex);

private:
	BOOL OnChannelSkillState();  // 引导技能过程中

	BOOL OnSkillState();  // 正常技能

	BOOL CheckSkillCharacters();

	BOOL IsChainLightningSkill();  // 判断是否闪电链技能

	BOOL ApplyBuffer(); // 应用技能结果（添加buffer）

	/*VOID ProcessTargetSkill();

	VOID ProcessGroundSkill();*/

	//BOOL IsHorizontalReachTarget(INT targetX,INT targetY);

	BOOL TrackHorizontalOperation();	//水平轨迹计算

	//BOOL TrackVerticalOperation();	//垂直轨迹计算

	//BOOL TrackGroundHorizontalOperation();	//对地水平轨迹计算

	//BOOL TrackGroundVerticalOperation();	//对地垂直轨迹计算

	//BOOL IsVerticalReachTarget(INT targetZ);

	VOID GenerateChainLightningTargets(KCharacter* pCenterTarget, FLOAT nRadius);

	VOID AddBuffer(INT nBufId, INT& rnSelfBufIdVal);

	VOID EndSkill();

	INT  ChangeGodSkillId(int nGodSkillId);
	
private:

	FIGHT_SKILL_STATE m_eSkillState;

	KCharacter*  m_pCaster;
	KCharacter*  m_pTarget;
	BOOL m_bIsMissileTimeInit;
	//DWORD m_dwMissileEndTime;  // 导弹结束时间帧
	DWORD m_dwMissileTimeRemaining; // 导弹剩下多少帧结束

	//INT  m_nMissileHorizontalSpeed;
	//INT  m_nMissileVerticalSpeed;
	INT  m_nMissileX;
	INT  m_nMissileY;
	INT  m_nMissileHeight;
	INT  m_nMissileEndHeight;
	BOOL m_bIsActive;	//是否是激活状态,再导弹结束，设置为非激活状态
	
	INT  m_nCurrentChainLightningTargetIndex;  // 闪电链技能
	INT  m_nChainFixedBuffAddonDelay;
	BOOL m_bChainSelfBufferAdded; // 治疗闪电链技能一次性为自己添加buffer

	//对地技能
	INT  m_nDestX;
	INT  m_nDestY;
	INT  m_nDestZ;
public:
	BOOL m_bSpecialTimeScale;              // 是否时间缩放

	INT  m_nSkillLevel;

	INT  m_nCurrentCastDelay;				// 当前释放延迟
	INT  m_nCurrentChannelTime;				// 实际引导时间
	INT	 m_nCostPercent;				// 实际消耗百分比
	INT	 m_nReduceCost;						// 减少多少消耗点数，默认为0
	FLOAT  m_fReduceCostPercent;				// 减少多少百分比消耗，默认为0
	INT	 m_nFightSkillChannel;				// 引导技能类型
	std::vector<KCharacter*> m_arrChainLightningTargets; // 闪电链技能目标 

	FightSkillTemplate* m_pFightSkillTemplate;
	INT m_nIncroBuffer1Id; // 所添加buffer1的object id
	INT m_nIncroBuffer2Id; // 所添加buffer2的object id
	INT m_nIncroBuffer3Id; // 所添加buffer3的object id
	INT m_arrCurrentIncroBuffer[3];
	INT m_nIncroBuffer4Id;
	INT m_nIncroBuffer4; // 额外的添加buffer4，动态改变，如无是0 
};

#endif // fightskill_h__