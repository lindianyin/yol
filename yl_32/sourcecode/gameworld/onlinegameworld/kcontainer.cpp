

#include "stdafx.h"
#include "onlinegameworld/kcontainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
KContainer::KContainer()
{
}

BOOL KContainer::Create(INT nByValueNum, INT nWValueNum, INT nDwValueNum)
{
	m_cDwArrayValue.Create(nDwValueNum);
	m_cWArrayValue.Create(nWValueNum);
	m_cByArrayValue.Create(nByValueNum);
	return TRUE;
}

BOOL KContainer::Init()
{
	m_cDwArrayValue.Init();
	m_cWArrayValue.Init();
	m_cByArrayValue.Init();
	m_mpDynValue.clear();
	return TRUE;
}

VOID KContainer::UnInit()
{
	m_mpDynValue.clear();
}

KContainer::~KContainer()
{
}

INT KContainer::GetMapValue( DWORD dwKey ) const
{
	std::map<DWORD, DWORD>::const_iterator it = m_mpDynValue.find(dwKey);
	if (it == m_mpDynValue.end())
		return 0;
	return it->second;
}

BOOL KContainer::SetMapValue( DWORD dwKey, INT nValue )
{
	if (nValue != 0 || m_mpDynValue.find(dwKey) != m_mpDynValue.end())
		m_mpDynValue[dwKey] = (DWORD)nValue;
	return TRUE;
}

BOOL KContainer::AppendMapValue( DWORD dwKey, INT nAdd )
{
	m_mpDynValue[dwKey] += nAdd;
	return TRUE;
}

BOOL KContainer::AppendMapValueU( DWORD dwKey, INT nAdd )
{
	DWORD& rValue = m_mpDynValue[dwKey];
	if (nAdd > 0)
	{
		if (rValue > std::numeric_limits<DWORD>::max() - nAdd)
			return FALSE;
	}
	else
	{
		if (rValue < std::numeric_limits<DWORD>::min() - nAdd)
			return FALSE;
	}
	rValue += nAdd;
	return TRUE;
}

INT KContainer_Save::Save( LPBYTE pBuffer, UINT uBufSize )
{
	LPBYTE pEndBuffer = pBuffer + uBufSize;
	// 先存储3种ArrayValue数量及MapValue数量（TODO:优化）
	QCONFIRM_RET(pBuffer + sizeof(WORD) * 3 + sizeof(DWORD) <= pEndBuffer, -1);
	*(LPWORD)pBuffer = (WORD)m_cByArrayValue.Count();
	pBuffer += sizeof(WORD);
	*(LPWORD)pBuffer = (WORD)m_cWArrayValue.Count();
	pBuffer += sizeof(WORD);
	*(LPWORD)pBuffer = (WORD)m_cDwArrayValue.Count();
	pBuffer += sizeof(WORD);
	*(LPDWORD)pBuffer = (DWORD)m_mpDynValue.size();
	pBuffer += sizeof(DWORD);

	// Copy Byte Array Value
	UINT uArraySize = sizeof(BYTE) * (m_cByArrayValue.Count());
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, -1);
	memcpy(pBuffer, m_cByArrayValue.m_pValue, uArraySize);
	pBuffer += uArraySize;
	// Copy WORD Array Value
	uArraySize = sizeof(WORD) * (m_cWArrayValue.Count());
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, -1);
	memcpy(pBuffer, m_cWArrayValue.m_pValue, uArraySize);
	pBuffer += uArraySize;
	// Copy DWORD Array Value
	uArraySize = sizeof(DWORD) * (m_cDwArrayValue.Count());
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, -1);
	memcpy(pBuffer, m_cDwArrayValue.m_pValue, uArraySize);
	pBuffer += uArraySize;

	// Save Map Value
	// 先判断长度是否足够 TODO:压缩优化
	QCONFIRM_RET(pBuffer + m_mpDynValue.size() * sizeof(DWORD) * 2 <= pEndBuffer, -1);
	LPDWORD pdwBuffer = (LPDWORD)pBuffer;
	std::map<DWORD, DWORD>::iterator it = m_mpDynValue.begin();
	for (; it != m_mpDynValue.end(); ++it)
	{
		*pdwBuffer++ = it->first;
		*pdwBuffer++ = it->second;
	}
	pBuffer = (LPBYTE)pdwBuffer;
	return (INT)(uBufSize - (pEndBuffer - pBuffer));
}

BOOL KContainer_Save::Load( LPCBYTE pData, UINT uDataLen )
{
	QCONFIRM_RET_FALSE( sizeof(WORD) * 3 + sizeof(DWORD) <= uDataLen);
	WORD wByArrayValueNum, wWArrayValueNum, wDwArrayValueNum;
	DWORD dwMapValueNum;
	wByArrayValueNum = *(LPWORD)pData;
	pData += sizeof(WORD);
	wWArrayValueNum = *(LPWORD)pData;
	pData += sizeof(WORD);
	wDwArrayValueNum = *(LPWORD)pData;
	pData += sizeof(WORD);
	dwMapValueNum = *(LPDWORD)pData;
	pData += sizeof(DWORD);
	uDataLen -= sizeof(WORD) * 3 + sizeof(DWORD);

	// 检查数据大小是否合法
	QCONFIRM_RET_FALSE(wByArrayValueNum * sizeof(BYTE) + wWArrayValueNum * sizeof(WORD) + 
		wDwArrayValueNum * sizeof(DWORD) + dwMapValueNum * sizeof(DWORD) * 2 == uDataLen);
	// 检查ArrayValue Buffer是否足够
	QCONFIRM_RET_FALSE(wByArrayValueNum <= m_cByArrayValue.Count() &&
		wWArrayValueNum <= m_cWArrayValue.Count() &&
		wDwArrayValueNum <= m_cDwArrayValue.Count());
	memcpy(m_cByArrayValue.m_pValue, pData, wByArrayValueNum * sizeof(BYTE));
	pData += wByArrayValueNum * sizeof(BYTE);
	memcpy(m_cWArrayValue.m_pValue, pData, wWArrayValueNum * sizeof(WORD));
	pData += wWArrayValueNum * sizeof(WORD);
	memcpy(m_cDwArrayValue.m_pValue, pData, wDwArrayValueNum * sizeof(DWORD));
	pData += wDwArrayValueNum * sizeof(DWORD);
	LPCDWORD pdwData = (LPCDWORD)pData;
	for (UINT i = 0; i < dwMapValueNum; ++i)
	{
		DWORD dwKey = *pdwData++;
		DWORD dwValue = *pdwData++;
		m_mpDynValue[dwKey] = dwValue;
	}
	return TRUE;
}
