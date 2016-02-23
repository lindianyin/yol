/* -------------------------------------------------------------------------
//	文件名		：	buddyfactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	指定密友（双向关系）工厂
//
// -----------------------------------------------------------------------*/
#ifndef __KBUDDYFACTORY_H__
#define __KBUDDYFACTORY_H__

#include "bidequalrelationfactory_t.h"
#include "datarelation_t.h"
#include "playerrelation_datadef.h"
#include "closefriendrelation.h"

class KRole;
class KBuddyFactory : public KBidEqualRelationFactory_T<KCloseFriendRelation::KCLOSEFRIEND_INFO,
	KCloseFriendRelation, sizeof(KCloseFriendRelation::KCLOSEFRIEND_DBINFO) >
{
	typedef KBidEqualRelationFactory_T<KCloseFriendRelation::KCLOSEFRIEND_INFO, KCloseFriendRelation, sizeof(KCloseFriendRelation::KCLOSEFRIEND_DBINFO)> SUPER_CLASS;
	enum
	{
		emKBUDDY_MAXCOUNT = 4,		// 指定密友的最大数目,暂时开放4
		emKBUDDY_MINLEVEL = 60,		// 指定密友的最小级别
		emKBUDDY_MINFAVOR = 2501,	// 应该是2501，成为指定密友的最小亲密度
	};
public:
	KBuddyFactory();
	~KBuddyFactory();
	/***********************************************************
	Function		: CreateRelation
	Description	: 创建玩家关系
	Return			: BOOL
	Param			: pMaster			主位玩家
	Param			: pTarget			次位玩家
	 Param			: bIsMasterActive	主位玩家是否主动申请方
	Param			: ppRelation		创建的玩家关系
	***********************************************************/
	virtual BOOL CreateRelation(
		INT nMaster,
		INT nTarget,
		BOOL			bIsMasterActive,
		IKRelation**	ppRelation);
	/***********************************************************
	Function		: CanCreateRelation
	Description	: 检查是否可以创建玩家关系
	Return			: BOOL
	Param			: pMaster		主位玩家
	Param			: pTarget		次位玩家
	Param			: pData			附带数据
	Param			: nLen			附带数据长度
	***********************************************************/
	virtual BOOL CanCreateRelation(
		INT nMaster,
		INT nTarget);
	/***********************************************************
	Function		: DeleteRelation
	Description	: 删除指定主次位玩家的玩家关系。
	Return			: BOOL
	Param			: nPlayerId1		玩家1
	Param			: nPlayerId2		玩家2
	***********************************************************/
	virtual BOOL DeleteRelation(INT nPlayerId1, INT nPlayerId2);
	
private:
	BOOL CheckCreation(INT nMasterID, INT nTargetID, KRole* pMaster, KRole* pTarget);
	INT GetBuddyCount(INT nPlayerId);
};

#endif //__KBUDDYFACTORY_H__
