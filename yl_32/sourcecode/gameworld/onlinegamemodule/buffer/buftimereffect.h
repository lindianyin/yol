/* -------------------------------------------------------------------------
//	文件名		：	buftimereffect.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUF_TIMER_EFFECT_H__
#define __BUF_TIMER_EFFECT_H__

#include "bufcommand.h"

class ITabFile;

class BufTimerEffect
{
public:
	BufTimerEffect();

	~BufTimerEffect();

	// 根据表格文件初始化所有属性
	BOOL Init(ITabFile* tabFile, INT nRow);

	INT GetID() {return m_nID;}

	TimerEffectLaunchType GetLanuchType() const {return m_eLaunchType;}

	INT	GetInterval() const {return m_nInterval;}

	BufConditionCommand& GetCondition() {return m_cCondition;}

	BufMagicEffectCommand& GetUpperEffect() {return m_cUpperBoundEffect;}

	BufMagicEffectCommand& GetLowerEffect() {return m_cLowerBoundEffect;}

protected:
    INT m_nID;
	INT m_nInterval;
	TimerEffectLaunchType m_eLaunchType;
	BufConditionCommand   m_cCondition;
	BufMagicEffectCommand m_cUpperBoundEffect;
	BufMagicEffectCommand m_cLowerBoundEffect;
};

#endif