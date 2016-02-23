/* -------------------------------------------------------------------------
//	文件名		：	kenemyfactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KENEMYFACTORY_H__
#define __KENEMYFACTORY_H__
// -------------------------------------------------------------------------
#include "bidunequalrelationfactory_t.h"
#include "datarelation_t.h"
#include "playerrelation_datadef.h"

// -------------------------------------------------------------------------
class KEnemyFactory : public KBidUnEqualRelationFactory_T<KNULL, KDataRelation_T<KNULL> >
{
	typedef KBidUnEqualRelationFactory_T<KNULL, KDataRelation_T<KNULL> > SUPER_CLASS;
public:
	KEnemyFactory();
	~KEnemyFactory();
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
	Param			: pData			附带数据
	Param			: nLen			附带数据长度
	***********************************************************/
	virtual BOOL CanCreateRelation(
	INT nMaster,
	INT nTarget);

	/***********************************************************
	Function		: SaveAll
	Description	: 存盘
	Return			: BOOL	
	Param			: nPlayerId		玩家角色ID
	***********************************************************/
	virtual BOOL SaveAll(INT nPlayerId, IKRelationSaver* pSaver);

protected:
private:
};


#endif /* __KENEMYFACTORY_H__ */
