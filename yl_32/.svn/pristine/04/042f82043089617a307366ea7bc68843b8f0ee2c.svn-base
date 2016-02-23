/* -------------------------------------------------------------------------
//	文件名		：	buftriggereffect.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUF_TRIGGER_EFFECT_H__
#define __BUF_TRIGGER_EFFECT_H__

#include "bufcommand.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegamebase/ksysservice.h"
#include <cstdarg>

class ITabFile;

class BufTriggerEffect
{
public:
	BufTriggerEffect();

	~BufTriggerEffect();

	// 根据表格文件初始化所有属性
	BOOL Init(ITabFile* tabFile, INT nRow);

	INT GetID() {return m_nID;}

	INT GetProbPercent() {return m_nProbPercent;}

	CONST string& GetEvent() {return m_szEvent;}

	//是否有CD
	BOOL BeHaveCD() CONST { return m_nIntervalTime; }

	BufMagicEffectCommand& GetMagicEffect() {return m_cEffect;}

	BufConditionCommand& GetCondition() {return m_cCondition;}

private:
	INT			m_nID;
	INT			m_nProbPercent;					// 触发的概率
	INT         m_nIntervalTime;				// 间隔检测时间

	string		m_szEvent;				// 触发的事件,用字符串记录，由事件触发到脚本进行回调
	BufConditionCommand m_cCondition;   // 如是持续性状态，不通过事件，用条件属性判断是否可以触发
	BufMagicEffectCommand m_cEffect;    // 事件效果

friend struct TriggerData;
};

struct TriggerData
{
	TriggerData(BufTriggerEffect* _psEffect = 0)
		: m_bTriggered(FALSE), m_nCDTime(0), psEffect(_psEffect) { }
	BOOL		m_bTriggered;					// 是否触发过
	INT			m_nCDTime;						// 剩余CD时间
	BufTriggerEffect* psEffect;

	//更新CD时间
	VOID UpdateCDTime() { (0 < m_nCDTime) ? --m_nCDTime : 0 ; }

	//更新CD时间
	INT GetCDTime() { return m_nCDTime; }

	//重置CD时间
	VOID ResetCDTime() { psEffect ? m_nCDTime = psEffect->m_nIntervalTime: 0; }

	//是否触发过
	BOOL BeTriggered() { return m_bTriggered; }

	//设置已被触发过
	VOID SetTriggered() { m_bTriggered = true; }
};

#endif