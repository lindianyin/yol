/* -------------------------------------------------------------------------
//	文件名		：	groundbuffer.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __GROUND_BUFFER_H__
#define __GROUND_BUFFER_H__

#include <vector>
#include "onlinegameworld/synchelper.h"
#include "buftriggereffect.h"
#include "buftimereffect.h"
#include "bufsetting.h"
#include "buffer.h"

#define REGION_TRAVERSE_RADIUS 1	//遍历的region的范围长度

class KScene;
class KRegion;
class KCharacter;

//对地buffer，搜索region内的玩家
class GroundBuffer
{
public:

	GroundBuffer();

	~GroundBuffer();

	BOOL Init(INT nBufferTemplateId,DWORD dwSceneId,INT nX,INT nY,INT nZ);

	VOID SetActive(BOOL bActive)	{ m_bIsActive = bActive; }

	VOID SetBuffPersist(INT nBuffPersist) { m_nBuffPersist = nBuffPersist; }

	VOID Clear();

	VOID UnInit();

	VOID Breath();

	BOOL CheckSelfCondition();

	VOID BindCaster(KCharacter* pCaster);

	VOID BindFightSkill(INT nSkillTemplateId);

	VOID SetSkillLevel(INT nSkillLevel) { m_nSkillLevel = nSkillLevel;}

	INT GetCastSkill() CONST {return m_nSkillTemplateId;}

	INT GetMagnification() CONST {return m_nMagnification;}

	INT GetGrow() CONST {return m_nGrow;}

	INT GetK() CONST {return m_nK;}

	INT GetIfUseSkillLv() CONST {return m_nIfUseSkillLv;}

	INT GetID() CONST { return m_dwId; }

	KCharacter* GetBufCaster() CONST { return m_pCaster; }

	INT GetStackCategory() CONST { return m_nStackCategory; }

	INT GetSceneId() CONST {return m_dwSceneId;}

	BOOL ScanCharacter(std::vector<KCharacter*>& rtCharacter);	//遍历region，找寻Buff实际作用角色

	BOOL BroadCastGroundBuffer();

	BOOL BroadcastRemoveGroundBuffer();

	VOID AppendData(BYTE type, INT value);

	INT GetAppendData(BYTE type);

	INT GetBuffPersist() CONST { return m_pTemplate->m_nBuffPersist; }

	INT GetTemplateId() CONST { return m_nTemplateId; }

	VOID BindToCharacter(KCharacter* pBinder) { m_pBinder = pBinder; }

	VOID RemoveFromPool();

	BOOL ModifyThreat(const std::vector<KCharacter*>& rtBeCastCha);

	BOOL CheckTriggerCondition(LPCSTR szEvent, const std::vector<KCharacter*>& rtBeCastCha);

	BOOL IsActive() CONST { return m_bIsActive; }

	VOID GetPosition(INT& nX, INT& nY, INT& nZ) { nX = m_nPositionX; nY = m_nPositionY; nZ = m_nPositionZ;}

	DWORD m_dwId;

private:
	BOOL CheckTimmerCondition(const BufTimerEffect* pEffect);
	VOID ProcessTimer(const std::vector<KCharacter*>& rtBeCastCha);
	VOID ProcessTrigger(const std::vector<KCharacter*>& rtBeCastCha);
	VOID OnRemoveGroundBuffer(const std::vector<KCharacter*>& rtBeCastCha);
	//VOID ExcecuteTiggerReverse(const std::vector<KCharacter*>& rtBeCastCha);

private:
	INT m_nTemplateId;
	INT m_nPositionX;
	INT m_nPositionY;
	INT m_nPositionZ;

	DWORD m_dwSceneId;		//当前场景id

	BufferTemplate* m_pTemplate;	//该buff对应的模板
	
	BOOL m_bIsActive;	// 是否已经激活
	BOOL m_bDoSelf;		// 对自己有效
	BOOL m_bAffectOnce;		// 只生效一次标志
	BOOL m_bDeath;		// 死亡是否保留

	UINT	m_uLifeTime;		// 当前的生命期
	INT     m_nBuffPersist;     // buff时长
	INT		m_nBuffDelayFrame;	// BUff效果延迟

	INT		m_nMagnification;		// 系数
	INT		m_nGrow;				// 成长
	INT		m_nIfUseSkillLv;	// buff在计算数值时是否用到技能等级
	INT		m_nK;

	INT		m_nStackCategory;		// 叠加类型

	std::vector<BufTimerEffect*>			m_vecBufTimerEffect;			// 持续性效果
	std::vector<TriggerData>				m_vecBufTriggerEffect;			// 触发性效果

	KCharacter* m_pCaster;		// 施法者
	KCharacter* m_pBinder;		// 绑定者(如果存在,范围Buff跟随移动)
	INT m_nSkillTemplateId;
	INT m_nSkillLevel;
	
	std::vector<KCharacter*> m_vecLastEffectCharacter;		//最后一次作用效果的玩家列表

	std::vector<BuffData> m_vecGroundAppendData;			// GroundBuff的脚本加成数据
};

#endif