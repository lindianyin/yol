
#ifndef __KCDTIME_H__
#define __KCDTIME_H__

// -------------------------------------------------------------------------

class KCDTime
{

public:

	VOID Init(VOID);
	VOID Clear(VOID);								// 清除所有CD时间
	DWORD GetCDTime(INT nID) CONST;					// 获得指定CD类型的冷却时间（帧数）
	DWORD GetNextUseFrame(INT nID) CONST;
	INT GetFramePass(INT nID) CONST;				// 获得指定CD类型已经过时间
	BOOL SetNextUseFrame(INT nID, DWORD dwFrame);
	BOOL IsTimeUp(INT nID) CONST;
	BOOL Start(INT nID);							// 开始CD时间
	BOOL Stop(INT nID);								// 中止CD时间

private:

	typedef std::map<INT, DWORD> KLastFrameMap;		// Last Use Info Map

	KLastFrameMap m_mapNextUse;

	BOOL CheckID(INT nID) CONST;

};

// -------------------------------------------------------------------------

#endif /* __KCDTIME_H__ */
