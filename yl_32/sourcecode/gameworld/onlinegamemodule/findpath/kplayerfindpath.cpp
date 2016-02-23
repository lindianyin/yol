
#include "stdafx.h"
#include "onlinegameworld/scene_def.h"
#include "onlinegameworld/ksceneobject.h"
#include "kfindpathmodule.h"
#include "kluaplayerfindpath.h"
#include "kplayerfindpath.h"
#include "onlinegamemodule/qfindpathprotocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KPlayerFindPath::KPlayerFindPath(KCharacter& rCharacter) : KCharacterCtrlBase(rCharacter),
														   m_rPlayer(dynamic_cast<KPlayer&>(rCharacter)),
														   m_pLuna(NULL)
{
#ifdef GAME_CLIENT
	m_uNavPathLen = 0;
#endif

	ASSERT(NULL == m_pLuna);
	m_pLuna = new KLuaPlayerFindPath(this);
	ASSERT(NULL != m_pLuna);
}

KPlayerFindPath::~KPlayerFindPath()
{
	SAFE_DELETE(m_pLuna);
}

BOOL KPlayerFindPath::Init()
{
	return TRUE;
}

BOOL KPlayerFindPath::UnInit()
{
	return TRUE;
}

BOOL KPlayerFindPath::Active()
{
#ifdef GAME_CLIENT
	return ProcessAutoPath();
#else
	return TRUE;
#endif
}

BOOL KPlayerFindPath::GoTo(INT nMapId, INT nX, INT nY, INT nZ)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetFindPathModule().GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
 
    QPTC_FINDPATH_S2C_AUTOGOTO cAutoGoto;
	cAutoGoto.dwMapID = (DWORD)nMapId;
	cAutoGoto.nX = nX;
	cAutoGoto.nY = nY;
	cAutoGoto.nZ = nZ;

	BOOL bOK = pProtocolProcessor->SendData((INT)m_rPlayer.GetPlayerIndex(), cAutoGoto);
    QCONFIRM_RET_FALSE(bOK);

	return bOK;
}

#ifdef GAME_CLIENT

#define KD_FIND_DIS_SQUARE 1024	        // 逻辑单位 32*32
#define KD_STOPNEARBY_DIS_SQUARE 16384	//逻辑单位 2 * 64
#define KD_AUTOPATH_STEP_FURTHER (CELL_LENGTH * 3)	// 逻辑单位

BOOL KPlayerFindPath::ProcessAutoPath()
{
	if (!m_cAutoPathState.bAutoFindPath)
		return TRUE;

	INT nCurX, nCurY, nCurZ;
	m_rPlayer.GetPosition(nCurX, nCurY, nCurZ);

	//获取当前目标点
	INT nCurDestX = 0;
	INT nCurDestY = 0;
	INT nCurDestZ = 0;
	nCurDestX = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 0];
	nCurDestY = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 1];
	nCurDestZ = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 2];
	Represent3DToLogic3D(nCurDestX, nCurDestY, nCurDestZ);

	INT nStepDistance = g_GetDistance2(nCurX, nCurY, (INT)nCurDestX, (INT)nCurDestY);
	INT nOverRunDistance = 0; // 超出下一站多远
	if (m_cAutoPathState.nCurDestIndex > 0)
	{
		INT nLastStationX = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 - 3];
		INT nLastStationY = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 - 2];
		INT nLastStationZ = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 - 1];
		Represent3DToLogic3D(nLastStationX, nLastStationY, nLastStationZ);

		INT nMoveDistance = g_GetDistance2(nCurX, nCurY, nLastStationX, nLastStationY); // 已移动距离
		INT nStationDistance = g_GetDistance2(nCurDestX, nCurDestY, nLastStationX, nLastStationY); // 站间距离
		nOverRunDistance = nMoveDistance - nStationDistance;
	}

	if (m_bStopNearby && m_cAutoPathState.nCurDestIndex == m_uNavPathLen - 1)
	{
		if (nStepDistance <= KD_STOPNEARBY_DIS_SQUARE)
		{
			EndAutoPath(TRUE);
		}
	}

	KE_MOVE_STATE nState = m_rPlayer.GetMoveState();
	if (nState == cmsOnRun && nOverRunDistance < KD_FIND_DIS_SQUARE
		/*&& nStepDistance >= KD_FIND_DIS_SQUARE*/)
		return TRUE;

	//goto 下个目标	
	m_cAutoPathState.nCurDestIndex++;
	if (m_cAutoPathState.nCurDestIndex < m_uNavPathLen)
	{
		INT nNextX = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 0];
		INT nNextY = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 1];
		INT nNextZ = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 2];
		Represent3DToLogic3D(nNextX, nNextY, nNextZ);
		UINT i = m_cAutoPathState.nCurDestIndex;

		// 稍微向前走一点
		float fDistX = m_aNavPath[i * 3 + 0] - m_aNavPath[i * 3 - 3];
		//float fDistY = m_aNavPath[i * 3 + 1] - m_aNavPath[i * 3 - 2];
		float fDistZ = m_aNavPath[i * 3 + 2] - m_aNavPath[i * 3 - 1];

		float fDistSquare = fDistX * fDistX + fDistZ * fDistZ;
		if (i < m_uNavPathLen - 1 && fDistSquare > KD_FIND_DIS_SQUARE)
		{
			float fDistSqrt = sqrt(fDistSquare);
			float fNormalizedX = fDistX / fDistSqrt; // 矢量归一化
			float fNormalizedZ = fDistZ / fDistSqrt;

			nNextX += INT(fNormalizedX * KD_AUTOPATH_STEP_FURTHER);
			nNextY += INT(fNormalizedZ * KD_AUTOPATH_STEP_FURTHER);
		}

		m_rPlayer.ClientGoTo(nNextX, nNextY, nNextZ);
	}
	else
	{
		//还有几帧没走完呢
		if (nState == cmsOnRun)
			return TRUE;

		//走完了
		EndAutoPath(TRUE);
	}

	return TRUE;
}

//每切换场景时需要重新加载导航文件
BOOL KPlayerFindPath::LoadNavPath(const char* szFileName)
{
	QCONFIRM_RET_FALSE(szFileName);
	FNLOADNAVMESH pFNLoadNavMesh = GetFindPathModule().GetFNLoadNavMesh();
	QCONFIRM_RET_FALSE(pFNLoadNavMesh);	
	return pFNLoadNavMesh(szFileName);
}

BOOL KPlayerFindPath::StartAutoPath(INT nX, INT nY, INT nZ, BOOL bStopNearby)
{	
	//无论如何，先停止可能已有的寻路
	EndAutoPath(FALSE);

	FNFINDPATH pFNFindPath = GetFindPathModule().GetFNFindPath();
	QCONFIRM_RET_FALSE(pFNFindPath);

	FNGETNAVPATH pFNGetNavPath = GetFindPathModule().GetFNGetNavPath();
	QCONFIRM_RET_FALSE(pFNGetNavPath);
	
	//寻路
	INT nCurX, nCurY, nCurZ;
	m_rPlayer.GetPosition(nCurX, nCurY, nCurZ);	

	INT nDestX = nX;
	INT nDestY = nY;
	INT nDestZ = nZ;

	//转3d坐标
	Logic3DToRepresent3D(nCurX, nCurY, nCurZ);
	Logic3DToRepresent3D(nDestX, nDestY, nDestZ);

	float aStartPos[3] = {0};
	float aEndPos[3]   = {0};
	aStartPos[0] = (float)nCurX;
	aStartPos[1] = (float)nCurY;
	aStartPos[2] = (float)nCurZ;
	aEndPos[0] = (float)nDestX;
	aEndPos[1] = (float)nDestY;
	aEndPos[2] = (float)nDestZ;
	
	memset((BYTE*)&m_aNavPath[0], 0, sizeof(float) * MAX_NAVPATH_COUNT);
	m_uNavPathLen = 0;
	BOOL bResult = pFNFindPath(aStartPos, aEndPos, &m_uNavPathLen);
	if (!bResult)	//允许寻路失败
		return FALSE;
	
	QCONFIRM_RET_FALSE(m_uNavPathLen > 0 && m_uNavPathLen <= MAX_NAVPATH_COUNT);
	bResult = pFNGetNavPath(m_aNavPath);
	QCONFIRM_RET_FALSE(bResult);

	m_cAutoPathState.bAutoFindPath = TRUE;
	m_cAutoPathState.nDestX = nX;
	m_cAutoPathState.nDestY = nY;
	m_cAutoPathState.nDestZ = nZ;

	m_cAutoPathState.nCurDestIndex = 0;
	// bugfix: 有时候返回的包含起始点，直接跳过
	FLOAT fDistX = aStartPos[0] - m_aNavPath[0];
	FLOAT fDistZ = aStartPos[2] - m_aNavPath[2];
	if (fDistX * fDistX + fDistZ * fDistZ <= 1.0f)
	{
		m_cAutoPathState.nCurDestIndex = 1;
	}

	m_bStopNearby = bStopNearby;

	INT nNextX, nNextY, nNextZ;
	nNextX = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 0];
	nNextY = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 1];
	nNextZ = (INT)m_aNavPath[m_cAutoPathState.nCurDestIndex * 3 + 2];
	Represent3DToLogic3D(nNextX, nNextY, nNextZ);	
	m_rPlayer.FireEvent(emKOBJEVENTTYPE_CHARACTER_START_AUTOPATH, nX, nY, nZ);

	// 稍微向前走一点
	{
		float fDistX = 0.0f;
		float fDistY = 0.0f;
		float fDistZ = 0.0f;
		UINT i = m_cAutoPathState.nCurDestIndex;
		if (i > 0)
		{
			fDistX = m_aNavPath[i * 3 + 0] - m_aNavPath[i * 3 - 3];
			//fDistY = m_aNavPath[i * 3 + 1] - m_aNavPath[i * 3 - 2];
			fDistZ = m_aNavPath[i * 3 + 2] - m_aNavPath[i * 3 - 1];
		}
		else
		{
			fDistX = m_aNavPath[i * 3 + 0] - aStartPos[0];
			//fDistY = m_aNavPath[i * 3 + 1] - aStartPos[1];
			fDistZ = m_aNavPath[i * 3 + 2] - aStartPos[2];
		}

		float fDistSquare = fDistX * fDistX + fDistZ * fDistZ;
		if (i < m_uNavPathLen - 1 && fDistSquare >= KD_FIND_DIS_SQUARE)
		{
			float fDistSqrt = sqrt(fDistSquare);
			float fNormalizedX = fDistX / fDistSqrt; // 矢量归一化
			float fNormalizedZ = fDistZ / fDistSqrt;

			nNextX += INT(fNormalizedX * KD_AUTOPATH_STEP_FURTHER);
			nNextY += INT(fNormalizedZ * KD_AUTOPATH_STEP_FURTHER);
		}
	}

	return m_rPlayer.ClientGoTo(nNextX, nNextY, nNextZ);
}

BOOL KPlayerFindPath::EndAutoPath(BOOL bFinish)
{
	if (!m_cAutoPathState.bAutoFindPath)
		return FALSE;
	
	m_cAutoPathState.bAutoFindPath = FALSE;
	m_cAutoPathState.nCurDestIndex = 0;
	m_uNavPathLen = 0;

	//向当前点跑(客户端停止，服务端修正)
	INT nX, nY, nZ;
	m_rPlayer.GetPosition(nX, nY, nZ);
	BOOL bRetCode = m_rPlayer.ClientGoTo(nX, nY, nZ);
	if ( bFinish )
		m_rPlayer.FireEvent(emKOBJEVENTTYPE_CHARACTER_FINISH_AUTOPATH, 0, 0, 0);
	else
		m_rPlayer.FireEvent(emKOBJEVENTTYPE_CHARACTER_END_AUTOPATH, 0, 0, 0);
	return bRetCode;
}

#endif


IKPlayerFindPath* GetPlayerFindPath(KPlayer* pPlayer)
{
	QCONFIRM_RET(NULL != pPlayer, NULL);
	KPlayerFindPath* pPlayerFindPath = pPlayer->GetCtrl<KPlayerFindPath>();
	if (NULL == pPlayerFindPath)
	{
		ASSERT(FALSE);
		return NULL;
	}
	return static_cast<IKPlayerFindPath*>(pPlayerFindPath);
}


// -------------------------------------------------------------------------
