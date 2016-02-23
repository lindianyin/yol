/* -------------------------------------------------------------------------
//	文件名		：	Buffer.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include <vector>
#include "onlinegamemodule/bufferdef.h"
#include "buftriggereffect.h"

class KCharacter;
struct BufferTemplate;
class BufTriggerEffect;
class BufTimerEffect;
struct FightSkillTemplate;

struct BuffData
{
	BYTE	data_type;
	INT		data_value;
};

class Buffer
{
public:
	Buffer();

	~Buffer();

//const 常函数，应用于某些需要const Buffer*的对象上
public:
	DWORD GetID() CONST {return m_dwId;}

	INT  GetTemplateID() CONST {return m_nBuffTemplateID;}

	INT GetBuffPersist() CONST {return m_nBuffPersist;}

	INT GetCurrentBuffPersist() CONST {return m_nCurrentPersist;}

	BufferGroupType GetBuffType() CONST {return m_eBuffType;}

	BYTE GetBuffLevel() CONST {return m_nBuffLevel;}

	BUFFER_POSITION GetBuffPosition() CONST {return m_BuffPosition;}

	INT GetStackCategory() CONST {return m_nStackCategory;}	//相同的才可叠加

	//INT GetStackDetail() CONST {return m_nStackDetail;}

	INT GetMaxStackCount() CONST {return m_nStackCount;}

	INT GetStatckCount() CONST {return m_nCurrentStackCount;}

	BOOL IsSync() CONST {return m_bSync;}

	BOOL IsSave() CONST {return m_bSave;}

	BOOL IsDeathKeep() CONST {return m_bDeath;}

	BOOL IsCanPlus() CONST {return m_bPlus;}

	BOOL IsNeedAgree() CONST {return m_bNeedAgree;}

	FLOAT GetBuffRadius() CONST {return m_fBuffRadius;}

	INT GetBuffEffectNum() CONST {return m_nBuffEffectNum;}

	KCharacter* GetReceiveCharacter() CONST { return m_pCharacter;}

	KCharacter* GetCasterCharacter() CONST { return m_pSender;}

	INT GetCastSkill() CONST {return m_nSkillTemplateId;}

	INT GetMagnification() CONST {return m_nMagnification;}

	INT GetGrow() CONST {return m_nGrow;}

	INT GetK() CONST {return m_nK;}

	INT GetIfUseSkillLv() CONST {return m_nIfUseSkillLv;}

	INT GetBuffCategory(INT index);

//对外公用函数
public:
	BOOL Init(BufferTemplate* buffTemplate, INT nPersist = 0);

	BOOL BindCharacter(KCharacter* pCharacter);	//绑定接受者,在接受者接受的时候绑定

	BOOL BindSender(KCharacter* pCharacter);//绑定释放者,在释放者释放的时候绑定

	BOOL BindFightSkill(INT nFightSkillTemplateId); // 绑定释放此buffer的技能ID

	BOOL SetSkillLevel(INT nSkillLevel);  // 设置buffer等级

	BOOL Reset();

    VOID Breath();

	INT GetCurrentStackCount() { return m_nCurrentStackCount; }

	VOID UpdateBufferStack();

	VOID UpdateInterval(BOOL bStack = FALSE);	//设置时间

	VOID SetInterval(INT nInterval)	{ if (nInterval > 0) m_nCurrentPersist = nInterval;}

	INT GetBufferType() {return m_nBuffType;}

//条件处理函数
public:
	BOOL OnRemoveBuff();	//解除buff

	BOOL CheckSelfCondition(BOOL& bSync);	//检测自身的一些属性

	BOOL CheckSceneBuffEffect();

	BOOL IsLifeEnd()	{return m_nCurrentPersist <= 0; }	//是否到生命结束期了

	BOOL CheckRelation(KCharacter* pCaster, KCharacter* pTarget);

	BOOL ModifyThreat(KCharacter* pCaster, KCharacter* pTarget);

	BOOL CheckTimerCondition(INT nIndex);		//时间触发的一些判断

	BOOL ProcessTimer();		//处理持续性buff效果

	/*
	* 事件触发的一些判断(可能会修改玩家身上Buff队列的状态)
	* args:
	* return:
	*/
	BOOL CheckTriggerCondition(LPCSTR szEvent, KCharacter* pEventReceive, KCharacter* pEventSender);

	BOOL ProcessTrigger();				//处理事件触发

	BOOL TriggerEffect(INT nIndex);				//触发效果

	VOID AddBuffPersist(INT value) { m_nBuffPersist += value; }

	VOID RefreshBuffPersist();

	DWORD m_dwId;

	CONST std::vector<BuffData>& GetAppendData() CONST { return m_vecAppendData; };
	VOID SetAppendDataFromDB(CONST VOID* pAppendData, size_t append_size);

	//添加一项加成数据，不应用到玩家身上
	VOID AppendData(BYTE type, INT value);

	INT GetAppendData(BYTE type);

	CONST FightSkillTemplate* GetBelongSkillTemplate() CONST;

	VOID SetDestPosition(INT nX, INT nY, INT nZ);

	VOID GetDestPosition(INT& nX, INT& nY, INT& nZ) { nX = m_nDestX; nY = m_nDestY; nZ = m_nDestZ;}

protected:
    INT		m_nBuffTemplateID;		// Buffer 模板ID
	INT		m_nBuffPersist;			// Buffer 持续时间,帧数
	INT		m_nCurrentPersist;		// 当前的时间
	BYTE	m_nBuffLevel;			// buff等级
	INT		m_nBuffType;			// 是否是debuff
	INT		m_nStackCategory;		// 叠加类型
	INT		m_nStackCategory1;		// 叠加细类
	INT		m_nStackCategory2;		// 叠加细类
	INT		m_nMagnification;		// 系数
	INT		m_nGrow;				// 成长
	INT		m_nStackCount;			// 叠加层数
	INT		m_nCurrentStackCount;	// 当前叠加层数
	BOOL	m_bSync;				// 是否要同步到客户端
	BOOL	m_bSave;				// 是否下线保存
	BOOL	m_bDeath;				// 死亡是否保留
	BOOL	m_bPlus;				// 是否可以点掉
	BOOL	m_bNeedAgree;			// 是否需求确认
	BOOL	m_bAffectOnce;			// 被动技能只生效一次的标志
	BOOL	m_bStackAffectOnce;		// buff叠加只生效一次的标志

	INT		m_nDestX;
	INT		m_nDestY;
	INT		m_nDestZ;

	BufferGroupType		m_eBuffType;
	BUFFER_POSITION		m_BuffPosition;		// 触发位置
	FLOAT				m_fBuffRadius;		// 影响范围,半径
	INT					m_nBuffEffectNum;	// buff影响数量
	INT					m_nBuffDelayFrame;	// BUff效果延迟
	INT					m_nK;				// buff的K值
	INT					m_nIfUseSkillLv;	// buff在计算数值时是否用到技能等级
	CHAR				m_szEffectMapType[MAX_NAME_LEN];		// 生效的地图类型


	std::vector<BufTimerEffect*>		m_vecBufTimerEffect;			// 持续性效果
	std::vector<INT>					m_vecBufTimerInterval;			// 持续性效果对应的时间，和效果vec对应

	std::vector<TriggerData>			m_vecBufTriggerEffect;			// 触发性效果

	KCharacter* m_pCharacter;	//接受者
	KCharacter* m_pSender;		//释放者
	INT m_nSkillTemplateId;

	DWORD m_dwSenderId;		// 释放者id

	INT m_nSkillLevel;

	std::vector<BuffData> m_vecAppendData;		// 当前Buff的脚本加成数据
};

#endif