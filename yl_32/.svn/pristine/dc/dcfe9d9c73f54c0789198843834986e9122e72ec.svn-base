/* -------------------------------------------------------------------------
//	文件名		：	bufsetting.h
//	创建者		：	zhangjunjie
//	创建时间	：	2012-5-10
//	功能描述	：	buff配置读取
// -----------------------------------------------------------------------*/

#ifndef bufsetting_h__
#define bufsetting_h__

#include <map>
#include <vector>
#include "onlinegamemodule/bufferdef.h"

#include "bufcommand.h"
#include "buftimereffect.h"
#include "buftriggereffect.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"

class ITabFile;
class KCharacter;

//buffer的模板
struct BufferTemplate
{
	BOOL				Init(ITabFile* piFile, int nRow);
	BYTE				m_nBuffLevel;				//buff等级
	INT					m_nBuffTemplateID;			// Buffer ID
	INT					m_nBuffPersist;				// Buffer 持续时间,帧数
	INT					m_nStackCount;				//叠加层数
	FLOAT				m_fBuffRadius;				//影响范围
	INT					m_nBuffEffectNum;			//buff影响数量
	INT					m_nBuffType;				//buff类型
	INT					m_nStackCategory;			//叠加规则
	INT					m_nStackCategory1;			//划分1
	INT					m_nStackCategory2;			//划分2
	INT					m_nMagnification;			//系数
	INT					m_nGrow;					//成长
	BOOL				m_bSync;					//是否要同步到客户端
	BOOL				m_bSave;					//是否下线保存
	BOOL				m_bDeath;					//死亡是否保留
	BOOL				m_bPlus;					//是否可以点掉
	BOOL				m_bNeedAgree;				//是否需求确认
	BOOL				m_abRelation[RELATION_NUM];	// 关系判断([0]自己、[1]友方、[2]敌人)
	BOOL				m_bIsGroundBuffer;			//是否是对地技能
	INT					m_nBuffDelayFrame;			//时间延迟
	INT					m_nK;						// buff的K值
	INT					m_nIfUseSkillLv;			// 是否使用技能等级
	CHAR				m_szEffectMapType[MAX_NAME_LEN];		// 生效的地图类型

	BufferGroupType		m_eBuffType;
	BUFFER_POSITION		m_BuffPosition;				// 触发位置
	BufConditionCommand	m_cBuffPrecondition;		//前置条件
	std::vector<INT>	m_vecBufTimerEffectID;		// 持续性效果ID
	std::vector<INT>	m_vecBufTriggerEffectID;	// 触发性效果ID
};

// Buffer数值保存
struct BufferNumber
{
	INT m_nBufferId;
	INT m_nValue;  // 数值
	INT m_nSkillCoefficientMin; // 最小技能缩放系数
	INT m_nSkillCoefficientMax; // 最大技能缩放系数
	INT m_nSkillLevel;
	CHAR m_szCommand[SCRIPT_MAX_LENGTH]; // 指令

	BOOL Init(ITabFile* tabFile, int nRow);
};

class BuffSetting
{
	typedef std::map<INT, BufferTemplate*> BUFFER_TEMPLATE_MAP;
	typedef std::map<INT, BufTimerEffect*> BUFFER_TIMER_EFFECT_MAP;
	typedef std::map<INT, BufTriggerEffect*> BUFFER_TRIGGER_EFFECT_MAP;
	typedef std::map<std::string, BufferNumber*> BUFFER_NUMBER_MAP;

public:
	BuffSetting();

	~BuffSetting();

	BOOL Init();

	BOOL UnInit();

	BufferTemplate* GetBufferTemplate(INT nID);

	BufTimerEffect* GetTimerEffect(INT nID);

	TriggerData GetTriggerEffect(INT nID);

	// 获取buffer数值
	BufferNumber* GetBufferNumber(INT nBufferId, INT nSkillLevel, std::string szCommand);

	//检测前置条件,在能否加上buff之前进行判断，因为前置条件是相对于模板而言，而不是相对于buff实体
	BOOL	CheckPreCondition(INT nBuffTemplateId,KCharacter* m_pSender,KCharacter* m_pReceiver);	

private:
	BUFFER_TEMPLATE_MAP m_mapBuffers;
	BUFFER_TIMER_EFFECT_MAP m_mapTimerEffects;
	BUFFER_TRIGGER_EFFECT_MAP m_mapTriggerEffects;

	
	std::map<DWORD, BUFFER_NUMBER_MAP> m_mapBufferNumbers;  // 保存buffer等级数值
};

extern BuffSetting g_cBufferSetting;

#endif // bufsetting_h__
