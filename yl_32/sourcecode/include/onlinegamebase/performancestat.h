#pragma once

#include "Core/QTime.h"

template <int ProtocolMax>
struct QProtocolStatRecord
{
	UINT64	uRecvPackSize[ProtocolMax];
	UINT64	uRecvPackCount[ProtocolMax];
	UINT64	uTimeCost[ProtocolMax];
};

class QProtocolStat
{
public:
	QProtocolStat(const char* pszIdentify, int nMaxProtocol);
	~QProtocolStat();

public:
	void Record(int nProtocol, size_t uPakSize, UINT64 uTimeCost);
	void Output(BOOL bUseTimestamp);

protected:
	QProtocolStatRecord<UCHAR_MAX> m_Record;
	char m_szIdentify[255];
	int m_nMaxProtocol;
};

struct QStatInfo 
{
	INT64 nCost;
	INT64 nCount;
	INT64 nPeakCost;
	time_t  nPeakTime;
};

class QPerformanceStat
{
public:
	QPerformanceStat();
	~QPerformanceStat();

	void Reset();
	void SetName(const char cszName[], const char* pszIdentity = NULL);
	void Stat(const char* pszName, INT64 nTime);
	QStatInfo* GetStatInfo(const char* pszName);
	void Output(BOOL bUseTimestamp = false);

private:
	struct QNameLess
	{
		bool operator()(const char* pszX, const char* pszY) const
		{
			return strcmp(pszX, pszY) < 0;
		}
	};

	typedef std::map<const char*, QStatInfo, QNameLess> QCostTable;
	QCostTable m_CostTable;

	std::string m_strName;
	std::string m_strIdentity;
};

#define CALL_AND_STAT(line) \
	do  \
	{   \
		int64_t nTscTime = (INT64)RDTSC();   \
		static XStatInfo* s_pStat = g_pStat->GetStatInfo(#line);    \
		line;   \
		nTscTime = (INT64)RDTSC() - nTscTime;    \
		s_pStat->nCost += nTscTime;  \
		s_pStat->nCount++;  \
		if (nTscTime > s_pStat->nPeakCost)  \
		{  \
			s_pStat->nPeakCost = nTscTime;  \
			s_pStat->nPeakTime = time(NULL);  \
		}  \
	} while (false)


#define BEGIN_STAT(desc)    \
{   \
	INT64				nTscTime		= (INT64)RDTSC();  \
	static QStatInfo*   s_pStat			= g_pStat->GetStatInfo(desc);

#define END_STAT()  \
	nTscTime = (INT64)RDTSC() - nTscTime;    \
	s_pStat->nCost += nTscTime;  \
	s_pStat->nCount++;  \
	if (nTscTime > s_pStat->nPeakCost)  \
	{  \
		s_pStat->nPeakCost = nTscTime;  \
		s_pStat->nPeakTime = time(NULL);  \
	}  \
}

extern QPerformanceStat* g_pStat;
