/* -------------------------------------------------------------------------
//	文件名		：	kgcplayercommon.cpp
//	创建者		：	simon
//	创建时间	：	2011/9/16 17:42:20
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "gclogicbase/kgcplayercommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

VOID KGcPlayerCommon::SetAccountName( PCSTR pszAccountName )
{
	QStrCpyLen(m_szAccount, pszAccountName, sizeof(m_szAccount));
	m_bDirty = TRUE;
}

BOOL KGcPlayerCommon::LoadFromBuffer( PBYTE pbyBuffer, SIZE_T uSize )
{
	WORD wByArrayValueNum = 0, wWArrayValueNum = 0, wDwArrayValueNum = 0;
	QCONFIRM_RET_FALSE(uSize >= sizeof(WORD) * 3);
	SIZE_T uSizeRemain = uSize;
	wByArrayValueNum = *(LPWORD)pbyBuffer;
	pbyBuffer += sizeof(WORD);
	wWArrayValueNum = *(LPWORD)pbyBuffer;
	pbyBuffer += sizeof(WORD);
	wDwArrayValueNum = *(LPWORD)pbyBuffer;
	pbyBuffer += sizeof(WORD);
	uSizeRemain -= sizeof(WORD) * 3;
	// 检查数据大小是否合法
	QCONFIRM_RET_FALSE(wByArrayValueNum * sizeof(BYTE) + wWArrayValueNum * sizeof(WORD) +
		wDwArrayValueNum * sizeof(DWORD) == uSizeRemain);
	// 检查ArrayValue Buffer是否足够
	QCONFIRM_RET_FALSE(wByArrayValueNum <= emKGCPLAYER_TASK_BYTE_COMMON_MAX &&
		wWArrayValueNum <= emKGCPLAYER_TASK_WORD_COMMON_MAX &&
		wDwArrayValueNum <= emKGCPLAYER_TASK_DWORD_COMMON_MAX);

	// Start copying...
	// byte/word/dword容器
	memcpy(m_aByTask, pbyBuffer, wByArrayValueNum * sizeof(BYTE));
	pbyBuffer += wByArrayValueNum * sizeof(BYTE);
	memcpy(m_aWTask, pbyBuffer, wWArrayValueNum * sizeof(WORD));
	pbyBuffer += wWArrayValueNum * sizeof(WORD);
	memcpy(m_aDwTask, pbyBuffer, wDwArrayValueNum * sizeof(DWORD));
	pbyBuffer += wDwArrayValueNum * sizeof(DWORD);

	return TRUE;
}

SIZE_T KGcPlayerCommon::SaveToBuffer( PBYTE pBuffer, SIZE_T uBufSize )
{
	LPBYTE pEndBuffer = pBuffer + uBufSize;

	QCONFIRM_RET(pBuffer + sizeof(WORD) * 3 <= pEndBuffer, 0);

	*(LPWORD)pBuffer = emKGCPLAYER_TASK_BYTE_COMMON_MAX;
	pBuffer += sizeof(WORD);

	*(LPWORD)pBuffer = emKGCPLAYER_TASK_WORD_COMMON_MAX;
	pBuffer += sizeof(WORD);

	*(LPWORD)pBuffer = emKGCPLAYER_TASK_DWORD_COMMON_MAX;
	pBuffer += sizeof(WORD);

	// Copy Byte Array Value
	SIZE_T uArraySize = sizeof(BYTE) * emKGCPLAYER_TASK_BYTE_COMMON_MAX;
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, 0);
	memcpy(pBuffer, m_aByTask, uArraySize);
	pBuffer += uArraySize;

	// Copy WORD Array Value
	uArraySize = sizeof(WORD) * emKGCPLAYER_TASK_WORD_COMMON_MAX;
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, 0);
	memcpy(pBuffer, m_aWTask, uArraySize);
	pBuffer += uArraySize;

	// Copy DWORD Array Value
	uArraySize = sizeof(DWORD) * emKGCPLAYER_TASK_DWORD_COMMON_MAX;
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, 0);
	memcpy(pBuffer, m_aDwTask, uArraySize);
	pBuffer += uArraySize;

	return uBufSize - (pEndBuffer - pBuffer); // 返回已使用的缓冲区大小
}

SIZE_T KGcPlayerCommon::CalculateSaveBufferSize()
{
	return sizeof(WORD) * 3 + sizeof(BYTE) * emKGCPLAYER_TASK_BYTE_COMMON_MAX +
		sizeof(WORD) * emKGCPLAYER_TASK_WORD_COMMON_MAX +
		sizeof(DWORD) * emKGCPLAYER_TASK_DWORD_COMMON_MAX;
}

VOID KGcPlayerCommon::Reset()
{
	//KGcPlayerCommonBase::Reset();
	KGcPlayerCommonBase<emKGCPLAYER_TASK_BYTE_COMMON_MAX, emKGCPLAYER_TASK_WORD_COMMON_MAX, emKGCPLAYER_TASK_DWORD_COMMON_MAX>::Reset();
	m_bNew = FALSE;
	m_szAccount[0] = '\0';
}

KGcPlayerCommon::KGcPlayerCommon()
{
	m_szAccount[0] = '\0';
	m_bNew = FALSE;
}
