/* -------------------------------------------------------------------------
//	文件名		：	kcondchecker.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/27 17:17:28
//	功能描述	：	条件检测
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kcondchecker.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

template <typename T>
class KMailConditionT
{
public:
	static BOOL Check(INT nOperation, CONST T& cond, CONST T& val)
	{
		BOOL bResult = FALSE;
		
		switch (nOperation)
		{
		case emKMAILCOND_OP_GREATER:			// 大于
			bResult = val > cond;
			break;
		case emKMAILCOND_OP_LESS:				// 小于
			bResult = val < cond;
			break;
		case emKMAILCOND_OP_GREATEREQUAL:		// 大于等于
			bResult = val >= cond;
			break;
		case emKMAILCOND_OP_LESSEQUAL:			// 小于等于
			bResult = val <= cond;
			break;
		case emKMAILCOND_OP_EQUAL:				// 等于
			bResult = val == cond;
			break;
		case emKMAILCOND_OP_NOTEQUAL:			// 不等于
			bResult = val != cond;
			break;
		case emKMAILCOND_OP_IGNORE:				// 忽略
			bResult = TRUE;
			break;
		default:
			bResult = FALSE;
			QLogPrintf(LOG_ERR, "[mail] %s", "Invalid Check Option!");
			break;
		}
		
		return bResult;
	}
};

KCondChecker KCondChecker::ms_inst;

KCondChecker::KCondChecker()
{
	INT nCount = sizeof(m_aryCondition) / sizeof(m_aryCondition[0]);
	for (INT i = 0; i < nCount; ++i)
	{
		m_aryCondition[i].nSize = 0;
		m_aryCondition[i].pfnCheckCondition = NULL;
	}

	// 检测性别/等级
	m_aryCondition[emKMAILCOND_TYPE_SEXLEVEL].nSize				= sizeof(KMAILCOND_SEXLEVEL);
	m_aryCondition[emKMAILCOND_TYPE_SEXLEVEL].pfnCheckCondition = &KCondChecker::CheckCond_SexLevel;
	// 检测接收时间
	m_aryCondition[emKMAILCOND_TYPE_SENDTIME].nSize				= sizeof(KMAILCOND_SENDTIME);
	m_aryCondition[emKMAILCOND_TYPE_SENDTIME].pfnCheckCondition = &KCondChecker::CheckCond_SendTime;
}

KCondChecker::~KCondChecker()
{

}

BOOL KCondChecker::CheckCondition(KGcPlayerCommon* pPlayer, CONST KMAIL_CONDHEAD* pCond, INT nCondLen)
{
	QCONFIRM_RET_FALSE(pPlayer && pCond && nCondLen > 0);
	fnCheckCondition pfnCheck = m_aryCondition[pCond->byType].pfnCheckCondition;
	QCONFIRM_RET_FALSE(pfnCheck);

	return (this->*pfnCheck)(pPlayer, pCond, nCondLen);
}

BOOL KCondChecker::CheckCond_SexLevel(KGcPlayerCommon* pPlayer, CONST KMAIL_CONDHEAD* pHead, INT nCondLen)
{
	QCONFIRM_RET_FALSE(pPlayer && pHead);
	QCONFIRM_RET_FALSE(nCondLen == sizeof(KMAILCOND_SEXLEVEL));

	KMAILCOND_SEXLEVEL* pCond = (KMAILCOND_SEXLEVEL*)pHead;
	INT nSexMask = pPlayer->GetSex() == 0 ? 0x2 : 0x1;	// IKPlayer::GetSex()返回0表示男性
	if ((nSexMask & pCond->bySex) == 0)
		return FALSE;

	INT nLevel = pPlayer->GetLevel();
	if (KMailConditionT<INT>::Check(pCond->byLevel1Op, pCond->nLevel1, nLevel) &&
		KMailConditionT<INT>::Check(pCond->byLevel2Op, pCond->nLevel2, nLevel))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL KCondChecker::CheckCond_SendTime(KGcPlayerCommon* pPlayer, CONST KMAIL_CONDHEAD* pHead, INT nCondLen)
{
	QCONFIRM_RET_FALSE(pPlayer && pHead);
	QCONFIRM_RET_FALSE(nCondLen == sizeof(KMAILCOND_SENDTIME));

	KMAILCOND_SENDTIME* pCond = (KMAILCOND_SENDTIME*)pHead;
	INT nCurTime = (INT)KSysService::Time(NULL);
	INT nSendTime = pCond->nSendTime + pCond->nDeltaTime;
	return KMailConditionT<INT>::Check(pCond->byTimeOp, nSendTime, nCurTime);
}

// -------------------------------------------------------------------------
