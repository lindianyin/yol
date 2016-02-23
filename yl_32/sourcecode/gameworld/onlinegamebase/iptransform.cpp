
#include "stdafx.h"
#include "onlinegamebase/iptransform.h"
#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

KIpTransForm::KIpTransForm()
{
	m_pIpTable = NULL;
	m_bInit = FALSE;
	ZeroStruct(m_sIpHashTable);
}


KIpTransForm::~KIpTransForm() 
{ 
	if (m_pIpTable)
		delete[] m_pIpTable;
}


BOOL KIpTransForm::Init(LPCSTR pszFileName)
{
	if (!pszFileName)
		return FALSE;

	m_bInit = FALSE;
	INT nFileLen = 0;
	INT nIpDataLen = 0;

	QFile cFile;
	BOOL bOk = cFile.Open(pszFileName);
	LOG_PROCESS_ERROR(bOk);
	
	// 检查数据文件正确性
	nFileLen = cFile.Size();
	nIpDataLen = nFileLen - (INT)sizeof(m_sIpHashTable);
	PROCESS_ERROR(nIpDataLen > 0 && (nIpDataLen % sizeof(KIP_TABLE)) == 0);

	if (m_pIpTable)
	{
		delete[] m_pIpTable;
		m_pIpTable = NULL;
	}
	m_pIpTable = (KIP_TABLE*)new BYTE[nIpDataLen];
	PROCESS_ERROR(m_pIpTable);

	// 读文件,先读hash表
	PROCESS_ERROR(cFile.Read(m_sIpHashTable, sizeof(m_sIpHashTable)));

	// 读取ip表
	PROCESS_ERROR(cFile.Read(m_pIpTable, nIpDataLen));

	m_bInit = TRUE;

EXIT0:

	cFile.Close();

	return m_bInit;
}


UINT KIpTransForm::GetIpFromString(LPCSTR pszIp)
{
	UINT uRet = 0;
	LPSTR pszResult = NULL;
	CHAR szCopy[MAX_PATH] = "";

	PROCESS_ERROR(pszIp);

	QStrCpyLen(szCopy, pszIp, sizeof(szCopy));
	pszResult = strtok(szCopy, ".");
	PROCESS_ERROR(pszResult);

	while (pszResult)
	{
		uRet <<= 8;
		uRet |= (UINT)atoi(pszResult);
		pszResult = strtok(NULL, ".");
	}
	
EXIT0:
	return uRet;
}

BOOL KIpTransForm::GetIpAddr(LPCSTR pszIp, LPSTR pszArea)
{
	if (!pszIp || !pszArea)
		return FALSE;
	UINT uIp = GetIpFromString(pszIp);
	return GetIpAddr(uIp, pszArea);
}


BOOL KIpTransForm::GetIpAddr(UINT uOrgIp, LPSTR pszArea)
{

	if (!pszArea)
		return FALSE;

	if (!m_bInit)
		return FALSE;
	UINT uIp = 0;

	// 把IP地址转为低为存储
	uIp = (uOrgIp & 0xFF) << 24;
	uIp |= (uOrgIp & 0xFF00) << 8;
	uIp |= (uOrgIp & 0xFF0000) >> 8;
	uIp |= (uOrgIp & 0xFF000000) >> 24;

	USHORT ushHash = ((uIp & 0xFFFF0000) >> 16) % KD_IP_BUCKET_COUNT;
	
	// 直接通过hash 表直接取对应地区
	INT nM = 0;
	INT nL = m_sIpHashTable[ushHash].nBeginPos;
	INT nR = m_sIpHashTable[ushHash].nEndPos;
	UINT uCurStartIp = 0;
	UINT uCurEndIp = 0;

	// 通过hash 表的指针查找,二分查找, nL == nR 的话,就不用找了,直接HASH 表就定位了, 很大几率都是 L = R
	while (nL < nR - 1) 
	{
		nM = (nL + nR) / 2;
		uCurStartIp = m_pIpTable[nM].uIp;
		if (uIp == uCurStartIp) 
		{
			nL = nM;
			break;
		}
		if (uIp > uCurStartIp)
			nL = nM;
		else
			nR = nM;
	}
	uCurStartIp = m_pIpTable[nL].uIp;
	uCurEndIp = m_pIpTable[nR].uIp;
	if (uIp < uCurStartIp)
		strcpy(pszArea, m_pIpTable[nL - 1].szAddr);		// 获取上一个IP 的位置	
	else if(uIp >= uCurStartIp && uIp < uCurEndIp)
		strcpy(pszArea, m_pIpTable[nL].szAddr);
	else
		strcpy(pszArea, m_pIpTable[nR].szAddr);
	
	return TRUE;
}
