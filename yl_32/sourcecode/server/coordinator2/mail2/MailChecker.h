#pragma once

#include "config/MailDef.h"

class KRole;


typedef BOOL (*MAIL_CHECK_FUNC)(KRole *pPlayer, CONST MAIL_CONDITION_HEAD *pHead, INT nCondLen);

class MailChecker
{
public:
	static BOOL CheckConditions(DWORD dwRoleID, INT nNum, BYTE *pbyCond);

private:
	static BOOL CheckOneCondition(KRole *pRole, MAIL_CHECK_TYPE checkType, CONST MAIL_CONDITION_HEAD *pHead, INT nCondLen);
	static BOOL CheckSex(KRole *pRole, CONST MAIL_CONDITION_HEAD *pHead, INT nCondLen);
	static BOOL CheckLevel(KRole *pRole, CONST MAIL_CONDITION_HEAD *pHead, INT nCondLen);

	static MAIL_CHECK_FUNC ms_CheckFuncTable[emMailCheckTypeTotal];
};

