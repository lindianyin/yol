
#include "stdafx.h"
#include "onlinegamemodule/ai/PatrolPath.h"
#include "config/kconfigfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PATROL_POINT_X			"X"
#define PATROL_POINT_Y			"Y"
#define PATROL_REST_FRAME		"RestFrame"
#define PATROL_FACE_TO			"FaceTo"
#define PATROL_ANIMATION_ID		"AnimationID"
#define PATROL_ANIMATION_LOOP	"AniLoop"
#define PATROL_IS_RUN			"IsRun"
#define PATROL_SCRIPT_NAME		"ScriptName"

KPatrolPath::KPatrolPath(void)
{
	m_DefaultPatrolNode.nPatrolPointX = 0;
	m_DefaultPatrolNode.nPatrolPointY = 0;
	m_DefaultPatrolNode.nFaceTo = 0;
	m_DefaultPatrolNode.nRestFrame = 0;
	m_DefaultPatrolNode.nAniLoop = 0;
	m_DefaultPatrolNode.nAnimationID = 0;
	m_DefaultPatrolNode.bIsRun = false;
	m_DefaultPatrolNode.szScriptName[0] = '\0';
}

KPatrolPath::~KPatrolPath(void)
{
	
}

BOOL KPatrolPath::Init(const char* szFileName)
{
    BOOL        bResult         = false;
	BOOL		bRetCode		= false;
	ITabFile*   piTabFile       = NULL;
    int         nTableHight     = 0;
    KPatrolNode PatrolNode;

	if (szFileName == NULL || szFileName[0] == '\0')
	{
		m_vecPatrolPath.clear();
		goto EXIT1;
	}

	if (m_vecPatrolPath.size())
	{
		UnInit();
	}

	memset(&m_DefaultPatrolNode, 0, sizeof(m_DefaultPatrolNode));
	strcpy(this->m_szTabFileName, szFileName);

	piTabFile = g_OpenTabFile(szFileName);
	if (!piTabFile)
	{
		QLogPrintf(LOG_ERR, "[KPatrolPathList] Failed to open tab file \"%s\" !\n", szFileName);
		goto EXIT0;
	}

	nTableHight = piTabFile->GetHeight() - 2;
    if (nTableHight <= 0)
    {
        QLogPrintf(LOG_ERR, "[KPatrolPathList] Tab file \"%s\" has no content. ", szFileName);
        goto EXIT0;
    }

	for (int i = 0; i < nTableHight; i++)
	{
		bRetCode = LoadFromTemplate(piTabFile, i + 3, PatrolNode);
		LOG_PROCESS_ERROR(bRetCode);

		m_vecPatrolPath.push_back(PatrolNode);
	}

EXIT1:
	bResult = true;
EXIT0:
	SAFE_RELEASE(piTabFile);
	return bResult;
}

BOOL KPatrolPath::UnInit()
{
	m_vecPatrolPath.clear();

	return true;
}

BOOL KPatrolPath::GetPoint(int nPointIndex, OUT KPatrolNode& PatrolNode)
{
	BOOL bResult = false;
	
	int nSize = GetPointCount();
	LOG_PROCESS_ERROR(nPointIndex >= 0 && nPointIndex < nSize);

	PatrolNode = m_vecPatrolPath[nPointIndex];

	bResult = true;
EXIT0:
	return bResult;
}

int KPatrolPath::GetPointCount(void)
{
	int nResult = (int)m_vecPatrolPath.size();

	return nResult;
}

BOOL KPatrolPath::Save()
{
    BOOL bResult  = false;
	BOOL bRetCode = false;
	ITabFile* piTabFile = NULL;

	LOG_PROCESS_ERROR(m_szTabFileName[0] != '\0');

	piTabFile = g_CreateTabFile();
	LOG_PROCESS_ERROR(piTabFile);

	//建表
	bRetCode = piTabFile->InsertNewCol(PATROL_POINT_X);
	LOG_PROCESS_ERROR(bRetCode);	   	

	bRetCode = piTabFile->InsertNewCol(PATROL_POINT_Y);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piTabFile->InsertNewCol(PATROL_REST_FRAME);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piTabFile->InsertNewCol(PATROL_FACE_TO);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piTabFile->InsertNewCol(PATROL_ANIMATION_ID);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piTabFile->InsertNewCol(PATROL_ANIMATION_LOOP);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piTabFile->InsertNewCol(PATROL_IS_RUN);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piTabFile->InsertNewCol(PATROL_SCRIPT_NAME);
	LOG_PROCESS_ERROR(bRetCode);

	//写入默认值
	bRetCode = SaveToTemplate(piTabFile, 2, m_DefaultPatrolNode);
	LOG_PROCESS_ERROR(bRetCode);

	for (int nIndex = 0; nIndex < (int)m_vecPatrolPath.size(); nIndex++)
	{
		bRetCode = SaveToTemplate(piTabFile, nIndex + 3, m_vecPatrolPath[nIndex]);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bRetCode = piTabFile->Save(m_szTabFileName);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piTabFile);
	return bResult;
}

BOOL KPatrolPath::AddPoint(const KPatrolNode& rPatrol)
{
	m_vecPatrolPath.push_back(rPatrol);

	return true;
}

BOOL KPatrolPath::LoadFromTemplate(ITabFile* pTabFile, int nIndex, OUT KPatrolNode& rPatrolNode)
{
    BOOL bResult  = false;
	BOOL bRetCode = false;

	LOG_PROCESS_ERROR(pTabFile);

	bRetCode = pTabFile->GetInteger(nIndex, PATROL_POINT_X, 
		m_DefaultPatrolNode.nPatrolPointX, (int*)&(rPatrolNode.nPatrolPointX));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->GetInteger(nIndex, PATROL_POINT_Y, 
		m_DefaultPatrolNode.nPatrolPointY, (int*)&(rPatrolNode.nPatrolPointY));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->GetInteger(nIndex, PATROL_REST_FRAME, 
		m_DefaultPatrolNode.nRestFrame, (int*)&(rPatrolNode.nRestFrame));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->GetInteger(nIndex, PATROL_FACE_TO, 
		m_DefaultPatrolNode.nFaceTo, (int*)&(rPatrolNode.nFaceTo));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->GetInteger(nIndex, PATROL_ANIMATION_ID, 
		m_DefaultPatrolNode.nAnimationID, (int*)&(rPatrolNode.nAnimationID));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->GetInteger(nIndex, PATROL_ANIMATION_LOOP, 
		m_DefaultPatrolNode.nAniLoop, (int*)&(rPatrolNode.nAniLoop));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->GetInteger(nIndex, PATROL_IS_RUN,
		m_DefaultPatrolNode.bIsRun, (int*)&(rPatrolNode.bIsRun));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->GetString(nIndex, PATROL_SCRIPT_NAME,
		m_DefaultPatrolNode.szScriptName, rPatrolNode.szScriptName, MAX_PATH);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KPatrolPath::SaveToTemplate(ITabFile* pTabFile, int nIndex, IN KPatrolNode& rPatrolNode)
{
    BOOL bResult  = false;
	BOOL bRetCode = false;

	LOG_PROCESS_ERROR(pTabFile);

	bRetCode = pTabFile->WriteInteger(nIndex, PATROL_POINT_X, rPatrolNode.nPatrolPointX);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->WriteInteger(nIndex, PATROL_POINT_Y, rPatrolNode.nPatrolPointY);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->WriteInteger(nIndex, PATROL_REST_FRAME, rPatrolNode.nRestFrame);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->WriteInteger(nIndex, PATROL_FACE_TO, rPatrolNode.nFaceTo);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->WriteInteger(nIndex, PATROL_ANIMATION_ID, rPatrolNode.nAnimationID);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->WriteInteger(nIndex, PATROL_ANIMATION_LOOP, rPatrolNode.nAniLoop);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->WriteInteger(nIndex, PATROL_IS_RUN, rPatrolNode.bIsRun);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pTabFile->WriteString(nIndex, PATROL_SCRIPT_NAME, rPatrolNode.szScriptName, MAX_PATH);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}
