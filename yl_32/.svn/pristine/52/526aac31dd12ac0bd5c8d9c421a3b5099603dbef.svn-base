/* -------------------------------------------------------------------------
//	文件名		：	tmpfriendfactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	临时好友工厂
//
// -----------------------------------------------------------------------*/
#ifndef __KTMPFRIENDFACTORY_H__
#define __KTMPFRIENDFACTORY_H__


#include "unirelationfactory.h"

class KTmpFriendFactory : public KUniRelationFactory
{
	typedef KUniRelationFactory SUPER_CLASS;
public:
	KTmpFriendFactory();
	virtual ~KTmpFriendFactory();
public:
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
			IKRelation**	ppRelation);
};


#endif //__KTMPFRIENDFACTORY_H__
