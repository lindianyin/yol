/* -------------------------------------------------------------------------
//	文件名		：	trainingfactory.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	师徒（未出师）工厂
//
// -----------------------------------------------------------------------*/
#ifndef __KTRAININGFACTORY_H__
#define __KTRAININGFACTORY_H__

#include "bidunequalrelationfactory_t.h"
#include "bidfriendrelation.h"
#include "playerrelation_datadef.h"
#include "closefriendrelation.h"

class KTrainingFactory : public KBidUnEqualRelationFactory_T<KCloseFriendRelation::KCLOSEFRIEND_INFO, 
	KCloseFriendRelation, sizeof(KCloseFriendRelation::KCLOSEFRIEND_DBINFO), IKTrainingFactory>
{
	typedef KBidUnEqualRelationFactory_T<KCloseFriendRelation::KCLOSEFRIEND_INFO, KCloseFriendRelation, sizeof(KCloseFriendRelation::KCLOSEFRIEND_DBINFO), IKTrainingFactory> SUPER_CLASS;
public:
	KTrainingFactory();
	virtual ~KTrainingFactory();
private:
	/***********************************************************
	 Function		: CreateRelation
	 Description	: 创建玩家关系
	 Return			: BOOL
	 Param			: pMaster			师父玩家
	 Param			: pTarget			弟子玩家
	 Param			: bIsTeacherActive	是否师父为主动申请方
	 Param			: ppRelation		创建的玩家关系
	 ***********************************************************/
	virtual BOOL CreateRelation(
			INT nMaster,
			INT nTarget,
			BOOL			bIsTeacherActive,
			IKRelation**	ppRelation);
	/***********************************************************
	Function		: CanCreateRelation
	Description	: 检查是否可以创建玩家关系
	Return			: BOOL
	Param			: nPlayerIdMaster		主位玩家
	Param			: nPlayerIdTarget		次位玩家
	Param			: pData			附带数据
	Param			: nLen			附带数据长度
	***********************************************************/
	virtual BOOL CanCreateRelation(
		INT nPlayerIdMaster,
		INT nPlayerIdTarget);
	virtual BOOL QueryFactory(INT nType, LPVOID * ppFactory);
	// 检查师父的资格
	virtual BOOL CheckTeacherQualification(INT nTeacherId, LPINT pnTeacherErr = NULL, LPINT pnStudentErr = NULL);
	// 检查弟子的资格
	virtual BOOL CheckStudentQualification(INT nStudentId, LPINT pnTeacherErr = NULL, LPINT pnStudentErr = NULL);
	// 检查师父和弟子之间是否符合一定关系
	virtual BOOL CheckRelation(KGC_Player* pTeacher, KGC_Player* pStudent, LPINT pnTeacherErr = NULL, LPINT pnStudentErr = NULL);
	// 检查建立师徒的条件
	BOOL CheckQualification(INT nTeacherId, INT nStudentId, LPINT pnTeacherErr = NULL, LPINT pnStudentErr = NULL);
	// 检查师父的资格
	BOOL CheckTeacher(
		INT nTeacherId,									// 师父
		LPINT pnTeacherErr = NULL,
		LPINT pnStudentErr = NULL);
	// 检查徒弟的资格
	BOOL CheckStudent(
		INT nStudentId,
		LPINT pnTeacherErr = NULL,
		LPINT pnStudentErr = NULL);
	// 检查师父和弟子之间是否满足一定的条件
	BOOL CheckTeacherWithStudent(
		KGC_Player* pTeacher,
		KGC_Player* pStudent,
		LPINT pnTeacherErr = NULL,
		LPINT pnStudentErr = NULL);
	// 获取当前时间的年份月份编码
	inline INT GetMonthCode()
	{
// 		time_t tt = KSysService::Time(NULL);
// 		tm* pTime = KSysService::LocalTime(&tt);
// 		return 100 * (pTime->tm_year + 1900) + pTime->tm_mon + 1;
		return 0;
	}
};

#endif //__KTRAININGFACTORY_H__
