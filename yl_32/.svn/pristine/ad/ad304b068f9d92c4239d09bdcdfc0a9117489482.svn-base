#include "stdafx.h"
#include "QRandomName.h"
#include "config/kconfigfile.h"
#include "GameCenter.h"

BOOL QRandomName::GenerateRandomName(CHAR* refNameStr, INT nSex, BOOL bDoubleMode)
{
	INT randFamilyName = g_Random(m_vecFamilyNames.size());
	std::string* familyName = m_vecFamilyNames[randFamilyName];

	INT randNickName = 0;
	std::string* nickName;

	if (nSex == 0)
	{
		randNickName = g_Random(m_vecMaleNames.size());  // 男名
		nickName = m_vecMaleNames[randNickName];
	}
	else
	{
		randNickName = g_Random(m_vecFemaleNames.size());  // 女名
		nickName = m_vecFemaleNames[randNickName];
	}
	
	CHAR szConcat[_NAME_LEN];
	if (!bDoubleMode)
	{
		sprintf(szConcat, "%s%s", familyName->c_str(), nickName->c_str());  // doubleMode下，多重复一次
	}
	else
	{
		// doubleMode下，多重复一次,   李依 -> 李依依，  陈林昂 -> 陈林林
		sprintf(szConcat, "%s%s%s", familyName->c_str(), (nickName->substr(0, 2)).c_str() , (nickName->substr(0, 2)).c_str()); // 中文2个bit
	}
	

	//std::string szConcat = *familyName + *nickName;
	//sprintf(refNameStr, "%s%s", *(familyName->CStr()), *(nickName->CStr())); // 姓氏+名字
	strcpy(refNameStr, szConcat);

	// 获得一个随机名字，看是否占用，循环, 如果循环大于10000次了，直接随机一个名字加数字

	/*KRole* pRole = NULL;
	pRole = g_pGameCenter->m_RoleManager.GetRole(pCreateRoleParam->szRoleName);
	if (pRole != NULL)
	{
		g_pGameCenter->m_Gateway.DoCreateRoleRespond(
			eCreateRoleNameAlreadyExist, pRequest->nPlayerIndex, NULL
			);
		goto EXIT1;
	}*/
	return TRUE;
}

BOOL QRandomName::GetRandomName(CHAR* refNameStr, INT nSex)
{
	CHAR szBuf[_NAME_LEN];
	INT nLoopCount = 0;
	while(TRUE)
	{
		nLoopCount++;
		if (nLoopCount > 10000)  // 循环次数过多，强制结束
		{
			sprintf(refNameStr, "%s%d", szBuf, g_Random(999) );  // 名字后加上数字
			break;
		}
		else if (nLoopCount > 100)  //超过循环次数限制, 开始进行双名字组合
		{
			//this->GenerateRandomName(szBuf, nSex, TRUE);//by ldy;
		}
		else
		{
			this->GenerateRandomName(szBuf, nSex);
		}

		KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(szBuf);
		if (pRole != NULL) // 名字已注册
		{
			continue;
		}

		//名字可用
		sprintf(refNameStr, "%s", szBuf);
		break;
	}

	return TRUE;
}

BOOL QRandomName::Init()
{
	BOOL bResult = FALSE;
	ITabFile* pFile = g_OpenTabFile(QDF_RANDOM_NAME);
	LOG_PROCESS_ERROR(pFile);
	for (INT n = 2; n <= pFile->GetHeight(); ++n)
	{
		INT nTmp = 0;
		CHAR szBuf[_NAME_LEN] = "";

		bResult = pFile->GetInteger(n, "index", 0, &nTmp);

		bResult = pFile->GetString(n, "familyname", "", szBuf, countof(szBuf));
		if (szBuf[0] != 0)
		{
			std::string* familyName = new std::string(szBuf);
			m_vecFamilyNames.push_back(familyName);
		}

		bResult = pFile->GetString(n, "malename", "", szBuf, countof(szBuf));
		if (szBuf[0] != 0)
		{
			std::string* maleName = new std::string(szBuf);
			m_vecMaleNames.push_back(maleName);
		}

		bResult = pFile->GetString(n, "femalename", "", szBuf, countof(szBuf));
		if (szBuf[0] != 0)
		{
			std::string* femaleName = new std::string(szBuf);
			m_vecFemaleNames.push_back(femaleName);
		}
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QRandomName::UnInit()
{
	return TRUE;
}