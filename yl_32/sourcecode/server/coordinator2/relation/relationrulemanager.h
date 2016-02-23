/* -------------------------------------------------------------------------
//	文件名		：	relationrulemanager.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	关系规则管理器（管理关系依赖和冲突）
//
// -----------------------------------------------------------------------*/
#ifndef __KRELATIONRULEMANAGER_H__
#define __KRELATIONRULEMANAGER_H__


#include "playerrelation_i.h"
#include <vector>

class KRelationRuleManager
{
public:
	enum
	{
		emKTYPE_COUNT	= emKPLAYERRELATION_TYPE_COUNT,
		emKMASK_POSSIDE	= 0x1,	// 正向
		emKMASK_REVSIDE	= 0x2,	// 反向
		emKMASK_ALLSIDE	= 0x3,	// 双向
	};
	struct KRELATION_RULE
	{
		KRELATION_RULE(INT nType, BYTE nMode)
		{
			this->nType = nType;
			this->nMode = nMode;
		}
		INT		nType;	// 关系类型
		BYTE	nMode;	// 检查方式，第一位置位表示正向检查（主次位一致），第二位置位表示反向检查（主次位颠倒）
	};
	// 玩家关系策略
	struct KPOLICY
	{
		BOOL bSyncToServerAsMaster;	// 是否把关系信息同步到GameServer（自己作为主位玩家）
		BOOL bSyncToServerAsTarget;	// 是否把关系信息同步到GameServer（自己作为次位玩家）
		BOOL bSyncToClientAsMaster;	// 是否把关系信息同步到客户端（自己作为主位玩家）
		BOOL bSyncToClientAsTarget;	// 是否把关系信息同步到客户端（自己作为次位玩家）
		BOOL bNotifyOnlineAsMaster;	// 自己上线时是否需要通知好友（自己作为主位玩家）
		BOOL bNotifyOnlineAsTarget;	// 自己上线时是否需要通知好友（自己作为次位玩家）
		BOOL bNotifyDeleteAsMaster;	// 自己删号时是否需要通知好友（自己作为主位玩家）
		BOOL bNotifyDeleteAsTarget;	// 自己删号时是否需要通知好友（自己作为次位玩家）
		BOOL bNotifyTargetCreation;	// 是否把建立关系事件通知给次位玩家
		BOOL bNotifyDeleteToMaster;	// 是否把关系删除通知主位玩家
		BOOL bNotifyDeleteToTarget;	// 是否把关系删除通知次位玩家
	};
private:
	typedef std::vector<KRELATION_RULE>	KRELATION_VEC;
public:
	KRelationRuleManager();
	~KRelationRuleManager();
	// 检查新建关系的依赖关系，返回TRUE表示满足依赖关系
	BOOL CheckCreateDependency(INT nType, INT nMaster, INT nTarget);
	// 检查新建关系的冲突关系，返回TRUE表示没有冲突
	BOOL CheckCreateRepugnance(INT nType, INT nMaster, INT nTarget);
	// 检查删除关系的依赖关系，返回TRUE表示满足依赖关系
	BOOL CheckDeleteDependency(INT nType, INT nMaster, INT nTarget);
	inline CONST KPOLICY* GetPolicy(INT nType) const
	{
		// KGC_CONFIRM_RET_NULL(nType >= 0 && nType < emKPLAYERRELATION_TYPE_COUNT);
		return &ms_aryPolicy[nType];
	}
	static KRelationRuleManager *Instance() {return &ms_inst;}
	CONST KPOLICY* GetPolicy(KEPLAYERRELATION_TYPE emType);
	BOOL SetPolicy(KEPLAYERRELATION_TYPE emType, CONST KPOLICY* pPolicy);
private:
	KRELATION_VEC	m_aryCreateDepRules[emKTYPE_COUNT];	// 新建关系的依赖关系
	KRELATION_VEC	m_aryCreateRepRules[emKTYPE_COUNT];	// 新建关系的冲突关系
	KRELATION_VEC	m_aryDeleteDepRules[emKTYPE_COUNT];	// 删除关系的依赖关系
	BOOL			m_aDelPermission[emKTYPE_COUNT];	// 是否允许删除

	static KRelationRuleManager ms_inst;
	static KPOLICY				ms_aryPolicy[emKPLAYERRELATION_TYPE_COUNT];	// 人际关系策略
};


#endif //__KRELATIONRULEMANAGER_H__
