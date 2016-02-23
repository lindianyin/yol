/* -------------------------------------------------------------------------
//	文件名		：	kfriendfactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	普通好友工厂
//
// -----------------------------------------------------------------------*/
#ifndef __KFRIENDFACTORY_H__
#define __KFRIENDFACTORY_H__

#include "bidequalrelationfactory_t.h"
#include "bidfriendrelation.h"

class KFriendFactory : public KBidEqualRelationFactory_T<KBidFriendRelation::KFRIEND_INFO, 
	KBidFriendRelation, sizeof(KBidFriendRelation::KFRIEND_DBINFO)>
{
	typedef KBidEqualRelationFactory_T<KBidFriendRelation::KFRIEND_INFO, KBidFriendRelation, sizeof(KBidFriendRelation::KFRIEND_DBINFO)> SUPER_CLASS;
public:
	KFriendFactory();
	virtual ~KFriendFactory();
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
		BOOL				bIsMasterActive,
		IKRelation**		ppRelation = NULL);
	/***********************************************************
	 Function		: DeleteRelation
	 Description	: 删除指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: pMaster		主位玩家
	 Param			: pTarget		次位玩家
	 ***********************************************************/
	virtual BOOL DeleteRelation(INT nMaster, INT nTarget);
};

#endif //__KFRIENDFACTORY_H__
