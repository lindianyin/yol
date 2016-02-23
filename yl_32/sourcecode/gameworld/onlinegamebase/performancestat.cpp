#include "stdafx.h"
#include "onlinegamebase/performancestat.h"

using namespace std;

QPerformanceStat* g_pStat = NULL;

QProtocolStat::QProtocolStat(const char* pszIdentify, int nMaxProtocol)
{
	memset(&m_Record, 0, sizeof(m_Record));
	strcpy(m_szIdentify, pszIdentify);
	m_nMaxProtocol = nMaxProtocol;
}

QProtocolStat::~QProtocolStat()
{
	Output(true);
}

void QProtocolStat::Record(int nProtocol, size_t uPakSize, UINT64 uTimeCost)
{
	m_Record.uRecvPackSize[nProtocol] += uPakSize;
	m_Record.uRecvPackCount[nProtocol]++;
	m_Record.uTimeCost[nProtocol] += uTimeCost;
}

void QProtocolStat::Output(BOOL bUseTimestamp)
{
	int     nRetCode    = 0;
	FILE*   pFile       = NULL;
	tm		tmTime;
	char    szTime[64];
	char    szPath[255];

	mkdir("stat");

	szTime[0] = '\0';
	if (bUseTimestamp)
	{
		time_t	nTime	= time(NULL);
		tm*		pNow	= localtime_r(&nTime, &tmTime);

		LOG_PROCESS_ERROR(pNow);

		sprintf(
			szTime, "_%d_%d_%d_%d_%d",         
			pNow->tm_mon + 1,
			pNow->tm_mday,
			pNow->tm_hour,
			pNow->tm_min,
			pNow->tm_sec
			);
	}

	nRetCode = snprintf(szPath, sizeof(szPath), "stat/protocol_%s_%s.tab", m_szIdentify, szTime);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < sizeof(szPath));

	pFile = fopen(szPath, "w");
	LOG_PROCESS_ERROR(pFile);

	fprintf(pFile, "protocol\tcount\tsize\tcost\n");
	for (int i = 0; i < m_nMaxProtocol; i++)
	{
		fprintf(pFile, "%d\t%llu\t%llu\t%llu\n", i, m_Record.uRecvPackCount[i],  m_Record.uRecvPackSize[i],  m_Record.uTimeCost[i]);
	}

EXIT0:
	if (pFile)
		fclose(pFile);

	return;
}










QPerformanceStat::QPerformanceStat()
{
}

QPerformanceStat::~QPerformanceStat()
{
	QCostTable::iterator it = m_CostTable.begin(); 
	while (it != m_CostTable.end())
	{
		const char* pszName = it->first;
		m_CostTable.erase(it++);
		free((void*)pszName);
	}
}

void QPerformanceStat::Reset()
{
	for (QCostTable::iterator it = m_CostTable.begin(); it != m_CostTable.end(); ++it)
	{
		it->second.nCost = 0;
		it->second.nCount = 0;
		it->second.nPeakCost = 0;
		it->second.nPeakTime = 0;
	}
}

void QPerformanceStat::SetName(const char cszName[], const char* pszIdentity)
{
	m_strName = cszName;
	m_strIdentity = "";

	if (pszIdentity && pszIdentity[0])
	{
		m_strIdentity = "_";
		m_strIdentity += pszIdentity;
	}
}

void QPerformanceStat::Stat(const char* pszName, INT64 nTime)
{
	QStatInfo* pInfo = GetStatInfo(pszName);
	pInfo->nCost += nTime;
	pInfo->nCount++;

	if (nTime > pInfo->nPeakCost)
	{
		pInfo->nPeakCost = nTime;
		pInfo->nPeakTime = time(NULL);
	}
}

QStatInfo* QPerformanceStat::GetStatInfo(const char* pszName)
{
	QCostTable::iterator it = m_CostTable.find(pszName);
	if (it == m_CostTable.end())
	{
		char* pszCopy = strdup(pszName);
		QStatInfo info;
		info.nCost = 0;
		info.nCount = 0;
		info.nPeakCost = 0;
		info.nPeakTime = 0;
		m_CostTable[pszCopy] = info;
		return &m_CostTable[pszCopy];
	}
	return &it->second;
}

void QPerformanceStat::Output(BOOL bUseTimestamp)
{
	int         nRetCode    = 0;
	FILE*       pFile       = NULL;
	struct tm   tmTime;
	char        szTime[64];
	char        szFileName[255];

	szTime[0] = '\0';
	if (bUseTimestamp)
	{
		time_t      nTime       = time(NULL);
		struct tm*  pTimeNow    = localtime_r(&nTime, &tmTime);

		LOG_PROCESS_ERROR(pTimeNow);

		sprintf(
			szTime, "_%d_%d_%d_%d_%d",         
			pTimeNow->tm_mon + 1,
			pTimeNow->tm_mday,
			pTimeNow->tm_hour,
			pTimeNow->tm_min,
			pTimeNow->tm_sec
			);
	}

	mkdir("stat");

	nRetCode = snprintf(szFileName, sizeof(szFileName), "stat/%s%s%s.tab", m_strName.c_str(), m_strIdentity.c_str(), szTime);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < sizeof(szFileName));

	pFile = fopen(szFileName, "w");
	LOG_PROCESS_ERROR(pFile);

	fprintf(pFile, "function\tcost\tcount\tpeak_cost\tpeak_time\n");

	for (QCostTable::iterator it = m_CostTable.begin(); it != m_CostTable.end(); ++it)
	{
		struct tm* pPeakTime = localtime_r(&it->second.nPeakTime, &tmTime);

		LOG_PROCESS_ERROR(pPeakTime);

		sprintf(
			szTime, "%d-%d %d:%d:%d",         
			pPeakTime->tm_mon + 1,
			pPeakTime->tm_mday,
			pPeakTime->tm_hour,
			pPeakTime->tm_min,
			pPeakTime->tm_sec
			);

		fprintf(
			pFile, "%s\t%lld\t%lld\t%lld\t%s\n", 
			it->first, it->second.nCost, it->second.nCount, it->second.nPeakCost, szTime
			);
	}

EXIT0:
	if (pFile)
		fclose(pFile);
	return;
}

