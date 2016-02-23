
#pragma once

enum LOG_RECORD_REMIND_LEVEL
{
	LOG_LEVEL_INFO		= 0x1,
	LOG_LEVEL_ATTENTION	= 0x2,
	LOG_LEVEL_WARNING		= 0x4,
	LOG_LEVEL_ERROR		= 0x8,
};

struct _QLog_SystemTime 
{
	int	Hour;
	int	Minute;
	int	Second;
};

class CORE_API QLogFile
{
public:
	enum KLogFileParam
	{
		MAX_LINE_LEN = 2048,
	};
	///供外界使用的接口
public:
	QLogFile();
	virtual ~QLogFile();

	///brief 指定LOG文件的存放路径，以及每个LOG文件的大小
	int		InitialLogFile(const char* strRootPath, const char* strFilePrefix, size_t nSize = 0x100000);

	///结束记录LOG文件,返回LOG_ATTENTION、LOG_WARNING、LOG_ERROR三者任意的组合情况，表示是否记录有这三种log记录
	int		FinishLogFile();

	//向当前打开的LOG文件中写入一条信息
	//传入参数nInfoLen表示szInfo字符串的长度（不包括结尾符），传入负数表示字符串以'\0'字符结尾。
	void	LogRecord(LOG_RECORD_REMIND_LEVEL nRemindLevel, const char* szInfo, int nInfoLen = -1);
	//纪录自定文本格式的字符串
	void	LogRecordVar(LOG_RECORD_REMIND_LEVEL nRemindLevel, const char* szInfoFmt, ...);

	//调试版本才输出的LOG
	void	DebugLogRecord(LOG_RECORD_REMIND_LEVEL nRemindLevel, const char* szInfo, int nInfoLen = -1);
	//纪录自定文本格式的字符串
	void	DebugLogRecordVar(LOG_RECORD_REMIND_LEVEL nRemindLevel, const char* szInfoFmt, ...);

	///查询当前LOG模块是否有错误发生，返回LOG_ATTENTION、LOG_WARNING、LOG_ERROR三者任意的组合情况，表示是否记录有这三种log记录
	int		QueryState();

	//判断当前log模块如果有错误发生，则给出提示界面信息
	void	StateNotifyUi();

	///内部实现
private:
	///新建一个LOG文件
	int		PrepareLogFile();

	///关闭当前正在记录的LOG文件
	int		CloseLogFile();

	///Attribs
private:
	IFile*			m_pFile;
	char			m_szFullname[MAX_PATH];
	char			m_strRoot[64];
	char			m_strFilePrefix[64];

	size_t			m_nMaxSize;
	int				m_bCurrentError;
	int				m_bCurrentWarning;

	int				m_nCurrentVersion;
	_QLog_SystemTime		m_nCurrentTime;
	int				m_nCurrentState;
};
