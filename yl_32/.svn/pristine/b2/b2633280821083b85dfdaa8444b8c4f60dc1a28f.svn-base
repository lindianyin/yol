/* -------------------------------------------------------------------------
//	文件名		：	kcouplefactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	夫妻关系工厂
//
// -----------------------------------------------------------------------*/
#ifndef __KCOUPLEFACTORY_H__
#define __KCOUPLEFACTORY_H__
// -------------------------------------------------------------------------
#include "bidunequalrelationfactory_t.h"
#include "datarelation_t.h"
#include "playerrelation_datadef.h"

class KCoupleFactory : public KBidUnEqualRelationFactory_T<KNULL, KDataRelation_T<KNULL> >
{
	typedef KBidUnEqualRelationFactory_T<KNULL, KDataRelation_T<KNULL> > SUPER_CLASS;
public:
	KCoupleFactory();
	~KCoupleFactory();

	/***********************************************************
	Function		: CreateRelation
	Description		: 创建玩家关系
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
	/***********************************************************
	Function		: CanCreateRelation
	Description		: 检查是否可以创建玩家关系
	Return			: BOOL
	Param			: pMaster		主位玩家
	Param			: pTarget		次位玩家
	***********************************************************/
	virtual BOOL CanCreateRelation(
		INT nMaster,
		INT nTarget);

// 	/***********************************************************
// 	Function		: SaveAll
// 	Description	: 存盘
// 	Return			: BOOL	
// 	Param			: nPlayerId		玩家角色ID
// 	***********************************************************/
// 	virtual BOOL SaveAll(INT nPlayerId, IKRelationSaver* pSaver);
protected:
private:
};

// -------------------------------------------------------------------------
#endif /* __KCOUPLEFACTORY_H__ */
