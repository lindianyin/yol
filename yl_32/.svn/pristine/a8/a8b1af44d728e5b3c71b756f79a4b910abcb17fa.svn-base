/* -------------------------------------------------------------------------
//	文件名		：	kintroductionfactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	介绍人好友工厂
//
// -----------------------------------------------------------------------*/
#ifndef __KINTRODUCTIONFACTORY_H__
#define __KINTRODUCTIONFACTORY_H__

#include "bidunequalrelationfactory_t.h"
#include "datarelation_t.h"
#include "playerrelation_datadef.h"
#include "closefriendrelation.h"

class KRole;
class KIntroductionFactory : public KBidUnEqualRelationFactory_T<KCloseFriendRelation::KCLOSEFRIEND_INFO,
	KCloseFriendRelation, sizeof(KCloseFriendRelation::KCLOSEFRIEND_DBINFO) >
{
	typedef KBidUnEqualRelationFactory_T<KCloseFriendRelation::KCLOSEFRIEND_INFO, 
		KCloseFriendRelation, sizeof(KCloseFriendRelation::KCLOSEFRIEND_DBINFO) > SUPER_CLASS;
	enum
	{
		emKINTRODUCED_MAXLEVEL = 11,		// 被介绍人的最大级别
		emKINTRODUCER_DIFLEVEL = 6,			// 介绍人必须比被介绍人至少大此级别数目
	};
	enum KEPLAYERRELATION_MSG_TYPE
	{
		emADDINTRODUCTION_SUCCESS	= 1,	// 介绍人建立成功
		emINTRODUCTION_OVERMAX_NUMBER,		// 超过最大上限人数
	};
public:
	KIntroductionFactory();
	~KIntroductionFactory();
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
private:
	BOOL CheckCreation(INT nMasterID, INT nTargetID, KRole* pMaster, KRole* pTarget);

	BOOL MsgToShow(INT nMasterId, INT nTargetId, KRole* pMasterPlayer, KRole* pTargetPlayer, BYTE btMsgType);
};

#endif //__KINTRODUCTIONFACTORY_H__

