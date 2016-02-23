/* -------------------------------------------------------------------------
//	文件名		：	relationfactorylist.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	玩家关系工厂集合
//
// -----------------------------------------------------------------------*/
#ifndef __KRELATIONFACTORYLIST_H__
#define __KRELATIONFACTORYLIST_H__

#include "playerrelation_i.h"
#include "playerrelation_datadef.h"
#include "bidequalrelationfactory_t.h"
#include "bidunequalrelationfactory_t.h"
#include "unirelationfactory.h"
#include "tmpfriendfactory.h"
#include "friendfactory.h"
#include "trainingfactory.h"
#include "introductionfactory.h"
#include "buddyfactory.h"
#include <vector>
#include "closefriendrelation.h"
#include "enemyfactory.h"
#include "couplefactory.h"

class KRelationFactoryList
{
	struct KCHECK_RULE
	{
		INT		nType;
		BYTE	nFlag;	// 第1位置1表示要顺序检查（主位次位），第2位置1表示要逆序检查（次位主位）
	};
public:
	inline static KRelationFactoryList *Instance()
	{
		return &ms_inst;
	}
	KRelationFactoryList();
	~KRelationFactoryList();
	BOOL CreateFactory(INT nType, IKRelationFactory **ppFactory);
private:
	static KRelationFactoryList										ms_inst;
	std::vector<KCHECK_RULE>										m_aryCheckRules[emKPLAYERRELATION_TYPE_COUNT];
	IKRelationFactory*												m_aryFactories[emKPLAYERRELATION_TYPE_COUNT];
	KTmpFriendFactory												m_factoryTmpFriend;		// 临时好友
	KUniRelationFactory												m_factoryBlackList;		// 黑名单
	KFriendFactory													m_factoryBidFriend;		// 普通好友
	KBidEqualRelationFactory_T<KNULL, KDataRelation_T<KNULL> >		m_factorySibling;		// 结拜弟兄/姐妹
	KEnemyFactory													m_factoryEnemy;			// 仇人
	KTrainingFactory												m_factoryTraining;		// 师徒（未出师）
	KBidUnEqualRelationFactory_T<KCloseFriendRelation::KCLOSEFRIEND_INFO, 
		KCloseFriendRelation, 
		sizeof(KCloseFriendRelation::KCLOSEFRIEND_DBINFO) >			m_factoryTrained;		// 师徒（已出师）
//	KBidUnEqualRelationFactory_T<KNULL, KDataRelation_T<KNULL> >	m_factoryCouple;		// 夫妻
	KCoupleFactory													m_factoryCouple;		// 夫妻
 	KIntroductionFactory											m_factoryIntroduction;	// 介绍人
	KBuddyFactory													m_factoryBuddy;			// 密友
	IKRelationHelper*												m_pHelper;
};

#endif //__KRELATIONFACTORYLIST_H__
