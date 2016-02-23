/* -------------------------------------------------------------------------
//	文件名		：	trainingsetting.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	师徒配置信息
// -----------------------------------------------------------------------*/
#ifndef __KTRAININGSETTING_H__
#define __KTRAININGSETTING_H__

class KTrainingSetting
{
public:
	KTrainingSetting();
	~KTrainingSetting();
	BOOL Init();
	// 弟子最小级别
	inline INT GetStudentMinLevel() CONST
	{
		return m_nStudentMinLevel;
	}
	// 弟子最高级别
	inline INT GetStudentMaxLevel() CONST
	{
		return m_nStudentMaxLevel;
	}
	// 师父最小级别
	inline INT GetTeacherMinLevel() CONST
	{
		return m_nTeacherMinLevel;
	}
	// 当前弟子最大数目
	inline INT GetCurrentStudentMaxCount() CONST
	{
		return m_nCurrentStudentMaxCount;
	}
	// 本月收徒最大数目
	inline INT GetMonthlyStudentMaxCount() CONST
	{
		return m_nMonthlyStudentMaxCount;
	}
	// 师父和弟子的最小等级差距
	inline INT GetGapMinLevel() CONST
	{
		return m_nGapMinLevel;
	}
	// 弟子报到间隔时间（天数）
	inline INT GetCheckInDayCount() CONST
	{
		return m_nCheckInDayCount;
	}
	// 弟子报到间隔时间（秒）
	inline INT GetCheckInSecCount() CONST
	{
		return m_nCheckInSecCount;
	}
	// 当大于此级别时，玩家上线时不再出现拜师提示
	inline INT GetStudentHintMaxLevel() CONST
	{
		return m_nStudentHintMaxLevel;
	}
private:
	INT m_nStudentMinLevel;			// 弟子最小级别
	INT m_nStudentMaxLevel;			// 弟子最高级别
	INT m_nTeacherMinLevel;			// 师父最小级别
	INT m_nCurrentStudentMaxCount;	// 当前弟子最大数目
	INT m_nMonthlyStudentMaxCount;	// 本月收徒最大数目
	INT m_nGapMinLevel;				// 师父和弟子的最小等级差距
	INT m_nCheckInDayCount;			// 弟子报到间隔时间（天数）
	INT m_nCheckInSecCount;			// 弟子报到间隔时间（秒）
	INT m_nStudentHintMaxLevel;		// 当大于此级别时，玩家上线时不再出现拜师提示
};

extern KTrainingSetting g_cTrainingSetting;

#endif //__KTRAININGSETTING_H__
