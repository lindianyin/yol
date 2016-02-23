/* -------------------------------------------------------------------------
//	文件名		：	unirelationfactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	单向关系工厂（临时好友、黑名单等）
//
// -----------------------------------------------------------------------*/
#ifndef __KUNIRELATIONFACTORY_H__
#define __KUNIRELATIONFACTORY_H__

#include "playerrelation_i.h"
#include "relationcounter.h"
#include "datarelation_t.h"
#include "relationrulemanager.h"

class KRelation;
class KEnumUniRelation;

class KUniRelationFactory : public KRelationFactoryBaseT<>
{
public:
	typedef std::set<INT>					RELATION_SET;
	typedef std::map<INT, RELATION_SET>		RELATION_MAP;
	typedef KRelationRuleManager::KPOLICY	KPOLICY;
public:
	KUniRelationFactory(KEPLAYERRELATION_TYPE nType);
	virtual ~KUniRelationFactory();
	BOOL SetDatabase(IKRelationHelper *pHelper)
	{
		// KGC_CONFIRM_RET_FALSE(pHelper);
		m_pHelper = pHelper;
		return TRUE;
	}
public:
	/***********************************************************
	 Function		: GetRelationInfoLen
	 Description	: 获取当前类型玩家关系的附带信息长度（对于某一种关系类型，附带信息长度是固定的）。
					  该附带数据可以用来表示好感度之类的信息，由每种玩家关系自行解释
	 Return			: INT
	 ***********************************************************/
	virtual INT GetRelationInfoLen()
	{
		return 0;
	}
	/***********************************************************
	 Function		: CreateRelation
	 Description	: 创建玩家关系
	 Return			: BOOL
	 Param			: pMaster			主位玩家
	 Param			: pTarget			次位玩家
	 Param			: bIsMasterActive	主位玩家为主动申请方
	 Param			: ppRelation		创建的玩家关系
	 ***********************************************************/
	virtual BOOL CreateRelation(
			INT nMaster,
			INT nTarget,
			BOOL			bIsMasterActive,
			IKRelation**	ppRelation = NULL);
	/***********************************************************
	 Function		: CanCreateRelation
	 Description	: 检查是否可以创建玩家关系
	 Return			: BOOL
	 Param			: pMaster		主位玩家
	 Param			: pTarget		次位玩家
	 ***********************************************************/
	virtual BOOL CanCreateRelation(
			INT nMaster,
			INT nTarget);
	/***********************************************************
	 Function		: GetAllRelations
	 Description	: 获取指定玩家指定角色的所有玩家关系。
	 Return			: BOOL
	 Param			: bAsMaster		是否作为主位玩家
	 Param			: pGuigd		玩家
	 Param			: ppEnum		返回的玩家关系枚举器
	 ***********************************************************/
	virtual BOOL GetAllRelations(
			BOOL				bAsMaster,
			INT nPlayerId,
			IKEnumRelation**	ppEnum);
	/***********************************************************
	 Function		: FindRelation
	 Description	: 查找指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: pMaster		主位玩家
	 Param			: pTarget		次位玩家
	 Param			: ppRelation	返回的玩家关系
	 ***********************************************************/
	virtual BOOL FindRelation(
			INT nMaster,
			INT nTarget,
			IKRelation**		ppRelation = NULL);
	/***********************************************************
	 Function		: DeleteRelation
	 Description	: 删除指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: pMaster		主位玩家
	 Param			: pTarget		次位玩家
	 ***********************************************************/
	virtual BOOL DeleteRelation(
			INT nMaster,
			INT nTarget);
	/***********************************************************
	 Function		: Unload
	 Description	: 把指定玩家从内存中删除
	 Return			: BOOL
	 Param			: pGuid		玩家角色ID
	 ***********************************************************/
	virtual BOOL Unload(INT nPlayerId);
	/***********************************************************
	 Function		: SaveAll
	 Description	: 存盘
	 Return			: BOOL
	 Param			: nPlayerId		玩家角色ID
	 ***********************************************************/
	virtual BOOL SaveAll(INT nPlayerId, IKRelationSaver* pSaver);
	/***********************************************************
	 Function		: Load
	 Description	: 加载玩家关系列表
	 Return			: BOOL
	 Param			: pMaster		主位玩家角色ID
	 Param			: pMaster		次位玩家角色ID
	 Param			: pData			关系数据
	 Param			: nLen			数据长度
	 ***********************************************************/
	virtual BOOL Load(INT nMaster, INT nTarget, LPVOID pData, INT nLen);
	/***********************************************************
	 Function		: Remove
	 Description	: 删除指定玩家的所有关系
	 Return			: BOOL
	 Param			: pGuid		玩家角色ID
	 ***********************************************************/
	virtual BOOL Remove(INT nPlayerId);
private:
	BOOL AddReverseRelation(INT nPlayerId1, INT nPlayerId2);
	BOOL DelReverseRelation(INT nPlayerId1, INT nPlayerId2);
	INT CountRelation(CONST RELATION_MAP *pMap);
	VOID AfterCreateRelation(INT nMasterId, INT nTargetId, INT nType);
private:
	CONST KEPLAYERRELATION_TYPE	m_nType;
	KEnumUniRelation*			m_pEnum;
	KRelation*					m_pRelation;
	RELATION_MAP				m_mapRelationList;			// 关系表
	RELATION_MAP				m_mapReverseRelationList;	// 反向关系表
};

class KEnumUniRelation : public IKEnumRelation
{
	typedef KUniRelationFactory::RELATION_SET RELATION_SET;
public:
	KEnumUniRelation(KRelation *pRelation);
	BOOL Init(INT nPlayerId, BOOL bAsMaster, RELATION_SET *pSet);
	virtual VOID Reset();
	virtual BOOL Next(IKRelation **ppRelation);
	virtual INT  GetCount();
private:
	RELATION_SET::const_iterator	m_itSet;
	RELATION_SET*					m_pRelationSet;
	KRelation*						m_pRelation;
	INT								m_nPlayerId;
	IKRelationHelper*				m_pHelper;
	BOOL							m_bAsMaster;		// m_sGuid的角色（主位或者次位）
};

#endif //__KUNIRELATIONFACTORY_H__
