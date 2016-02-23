#include "stdafx.h"
#include "MailChecker.h"
#include "KRole.h"
#include "GameCenter.h"

// =============================================================================
// checker
template <typename T>
static BOOL MailCondCheck(MAIL_CONDITION_OP nOp, const T &cond, const T &val)
{
	BOOL bResult = FALSE;

	switch (nOp)
	{
	case emMailCondOpGreater:		bResult = (val > cond);		break;
	case emMailCondOpGreaterEqual:	bResult = (val >= cond);	break;
	case emMailCondOpLess:			bResult = (val < cond);		break;
	case emMailCondOpLessEqual:		bResult = (val <= cond);	break;
	case emMailCondOpEqual:			bResult = (val == cond);	break;
	case emMailCondOpNotEqual:		bResult = (val != cond);	break;
	case emMailCondOpIgnore:		bResult = TRUE;				break;
	default: 
		{
			bResult = FALSE;
			QLogPrintf(LOG_DEBUG, "Mail Condition Check Error: Invalid Check Option");
		}
		break;
	}

	return bResult;
}

// func table
MAIL_CHECK_FUNC MailChecker::ms_CheckFuncTable[emMailCheckTypeTotal] = {
	&MailChecker::CheckSex,
	&MailChecker::CheckLevel
};


BOOL MailChecker::CheckConditions( DWORD dwRoleID, INT nNum, BYTE *pbyCond )
{
	BOOL bResult = FALSE;
	KRole *pRole = NULL;
	BYTE *pbyOffset = NULL;

	LOG_PROCESS_ERROR(dwRoleID);
	LOG_PROCESS_ERROR(nNum <= MAX_COND_NUM);
	LOG_PROCESS_ERROR(pbyCond);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
	LOG_PROCESS_ERROR(pRole);

	for (int i = 0; i < nNum; ++i)
	{
		pbyOffset = pbyCond;

		MAIL_CHECK_TYPE nType = (MAIL_CHECK_TYPE)*pbyOffset;
		pbyOffset ++;

		INT nCondLen = *pbyOffset;
		pbyOffset ++;

		bResult = CheckOneCondition(pRole, nType, (MAIL_CONDITION_HEAD*)pbyOffset, nCondLen);
		if (!bResult)
			break;

		pbyOffset += nCondLen;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL MailChecker::CheckOneCondition( KRole *pRole, MAIL_CHECK_TYPE checkType, CONST MAIL_CONDITION_HEAD *pHead, INT nCondLen )
{
	BOOL bResult = FALSE;

	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(checkType >= 0 && checkType < countof(ms_CheckFuncTable));
	LOG_PROCESS_ERROR(pHead);
	LOG_PROCESS_ERROR(nCondLen > 0);

	LOG_PROCESS_ERROR(ms_CheckFuncTable[checkType]);
	bResult = ms_CheckFuncTable[checkType](pRole, pHead, nCondLen);

EXIT0:
	return bResult;
}

BOOL MailChecker::CheckSex( KRole *pRole, CONST MAIL_CONDITION_HEAD *pHead, INT nCondLen )
{
	BOOL					bResult = FALSE;
	MAIL_CONDITION_SEX*		pCond	= (MAIL_CONDITION_SEX*)pHead;
	INT						nSex	= 0;

	ASSERT(pRole);
	ASSERT(pHead);
	LOG_PROCESS_ERROR(nCondLen == sizeof(MAIL_CONDITION_SEX));

	nSex = pRole->GetSex();
	bResult = MailCondCheck((MAIL_CONDITION_OP)pCond->byCondOp, (INT)pCond->bySex, nSex);

EXIT0:
	return bResult;
}

BOOL MailChecker::CheckLevel( KRole *pRole, CONST MAIL_CONDITION_HEAD *pHead, INT nCondLen )
{
	BOOL					bResult = FALSE;
	MAIL_CONDITION_LEVEL*	pCond	= (MAIL_CONDITION_LEVEL*)pHead;
	INT						nLevel	= 0;

	ASSERT(pRole);
	ASSERT(pHead);
	LOG_PROCESS_ERROR(nCondLen == sizeof(MAIL_CONDITION_LEVEL));

	nLevel = pRole->GetLevel();
	bResult = MailCondCheck((MAIL_CONDITION_OP)pCond->byLevel1Op, (INT)pCond->byLevel1, nLevel) 
		&& MailCondCheck((MAIL_CONDITION_OP)pCond->byLevel2Op, (INT)pCond->byLevel2, nLevel);

EXIT0:
	return bResult;
}

