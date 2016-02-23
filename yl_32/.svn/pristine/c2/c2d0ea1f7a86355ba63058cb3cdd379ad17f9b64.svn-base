/* -------------------------------------------------------------------------
//	文件名		：	kawardsetting.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-9-10
//	功能描述	：	奖励配置读取
// -----------------------------------------------------------------------*/

#ifndef kawardsetting_h__
#define kawardsetting_h__

#include "awarddef.h"
#include "Runtime/qstring.h"

class ITabFile;

class KAwardSetting
{
	
public:
	KAwardSetting();
	~KAwardSetting();

	BOOL Init();
	BOOL UnInit();

public:

	CONST QNumericalAward* RequestNumericalAward(QString moduleName, INT nCurrentStage, INT nLevel);

	INT GetLevelNumOfStage(QString moduleName, INT nCurrentStage);

	CONST QAssetAward* RequestAssetAward(QString moduleName, INT nLevel);
	
	CONST INT RequestMaxLineValue(QString moduleName, INT nCurrentStage);

	CONST INT RequestAwardSum(QString moduleName, INT nCurrentStage, INT nStartLevel, INT nLevelCount);

	INT GetAwardValueByMA(INT nAttr, CONST QNumericalAward* pAward);

	BOOL HasAssetAward(LPCSTR szModuleName, INT nLevel);

	INT GetExpAwardPercent(INT nExpAwardId, INT nLevel);

	DWORD GetExpByLevel(INT nExpAwardId, INT nNextLevel);

private:
	BOOL ReadNumericalAwardConfigs();
	BOOL ClearNumericalAwardData();

	BOOL ReadAssetAwardConfigs();
	BOOL ClearAssetAwardData();

	BOOL ReadExpAwardConfigs();

private:
	std::vector<QModuleNumericalAward*>  m_vecModuleNumericalAwards;

	std::vector<QModuleAssetAward*> m_vecModuleAssetAwards;

	//std::map<INT, KExpAward> m_mapExpAwards;

	typedef std::map<INT, KExpAward> MAP_EXP_AWARD;
	std::map<INT, MAP_EXP_AWARD> m_mapExpAwards;

};

extern KAwardSetting g_cAwardSetting;

#endif // kawardsetting_h__
