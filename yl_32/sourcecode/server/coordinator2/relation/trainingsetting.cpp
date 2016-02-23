/* -------------------------------------------------------------------------
//	文件名		：	trainingsetting.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	师徒配置信息
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "trainingsetting.h"

#define KD_INI_TRAINING					"\\setting\\relation\\training.ini"
#define KD_SEC_GLOBAL					"Global"
#define KD_KEY_STUDENTMINLEVEL			"StudentMinLevel"					// 弟子最小级别
#define KD_KEY_STUDENTMAXLEVEL			"StudentMaxLevel"					// 弟子最高级别
#define KD_KEY_TEACHERMINLEVEL			"TeacherMinLevel"					// 师父最小级别
#define KD_KEY_CURRENTSTUDENTMAXCOUNT	"CurrentStudentMaxCount"			// 当前弟子最大数目
#define KD_KEY_MONTHLYSTUDENTMAXCOUNT	"MonthlyStudentMaxCount"			// 本月收徒最大数目
#define KD_KEY_GAPMINLEVEL				"GapMinLevel"						// 师父和弟子的最小等级差距
#define KD_KEY_CHECKINDAYCOUNT			"CheckInDayCount"					// 弟子报到间隔天数
#define KD_KEY_STUDENTHINTMAXLEVEL		"StudentHintMaxLevel"				// 当大于此级别时，玩家上线时不再出现拜师提示

KTrainingSetting g_cTrainingSetting;

KTrainingSetting::KTrainingSetting() :
	m_nStudentMinLevel(20),			// 弟子最小级别
	m_nStudentMaxLevel(50),			// 弟子最高级别
	m_nTeacherMinLevel(69),			// 师父最小级别
	m_nCurrentStudentMaxCount(3),	// 当前弟子最大数目
	m_nMonthlyStudentMaxCount(3),	// 本月收徒最大数目
	m_nGapMinLevel(30),				// 师父和弟子的最小等级差距
	m_nCheckInDayCount(7),			// 弟子报到间隔时间（天数）
	m_nCheckInSecCount(168 * 3600),	// 弟子报到间隔时间（秒）
	m_nStudentHintMaxLevel(50)		// 当大于此级别时，玩家上线时不再出现拜师提示
{
}

KTrainingSetting::~KTrainingSetting()
{
}

BOOL KTrainingSetting::Init()
{
// 	KIniFile iniTraining;
// 	if (!iniTraining.Load(KD_INI_TRAINING))
// 		KGC_CONFIRM_RET_FALSE(FALSE);
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_STUDENTMINLEVEL, 20, &m_nStudentMinLevel);
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_STUDENTMAXLEVEL, 60, &m_nStudentMaxLevel);
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_TEACHERMINLEVEL, 90, &m_nTeacherMinLevel);
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_CURRENTSTUDENTMAXCOUNT, 3, &m_nCurrentStudentMaxCount);
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_MONTHLYSTUDENTMAXCOUNT, 3, &m_nMonthlyStudentMaxCount);
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_GAPMINLEVEL, 30, &m_nGapMinLevel);
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_CHECKINDAYCOUNT, 7, &m_nCheckInDayCount);
// 	m_nCheckInSecCount = m_nCheckInDayCount * 24 * 3600;
// 	iniTraining.GetInteger(KD_SEC_GLOBAL, KD_KEY_STUDENTHINTMAXLEVEL, 60, &m_nStudentHintMaxLevel);
	return TRUE;
}
