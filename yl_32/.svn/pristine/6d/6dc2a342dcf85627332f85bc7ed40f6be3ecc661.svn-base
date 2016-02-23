/********************************************************************
	created:	2012/06/20 11:55
	file base:	luabuffereffect.cpp
	author:		zhangjunjie
*********************************************************************/

#include "stdafx.h"
#include "onlinegamemodule/luabuffereffect.h"
#include "onlinegamemodule/characterbuffer.h"
#include "onlinegameworld/kluacharacter.h"
#include "buffer.h"
#include "buffermanager.h"
#include "../fightskill/fightskillsetting.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/knpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::Register2Character(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaCharacter* pLunaCharacter = dynamic_cast<KLuaCharacter*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaCharacter);
	KCharacter* pCharacter = pLunaCharacter->GetHim();
	QCONFIRM_RET_FALSE(pCharacter);

	CharacterBuffer* pKCharacterBuffer = pCharacter->GetCtrl<CharacterBuffer>();
	QCONFIRM_RET_FALSE(pKCharacterBuffer);
	sc.PushObj(&pKCharacterBuffer->GetScriptInterface());

	return 1;
}



// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: DamageLifeV
// @Description		: 伤害生命值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 伤害生命值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaDamageLifeV(QLuaScript& sc)
{
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: DamageLifeP
// @Description		: 伤害生命百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 伤害生命值百分比值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaDamageLifeP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: DamageManaV
// @Description		: 伤害真气值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 伤害真气值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaDamageManaV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: DamageManaP
// @Description		: 伤害真气百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 伤害真气值百分比值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaDamageManaP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: Stun
// @Description		: 眩晕
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaStun(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: Fix
// @Description		: 定身
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaFix(QLuaScript& sc)
{
	/*if (IS_PLAYER(m_rcCharacter.GetId()))
	{
		KPlayer* pPlayer = (KPlayer*)(&m_rcCharacter);
		DWORD dwSceneId = pPlayer->GetSceneId();
		pPlayer->m_cPlayerServer.TeleportTo2(GET_MAP_ID(dwSceneId), GET_MAP_COPY_INDEX(dwSceneId), pPlayer->m_nX, pPlayer->m_nY, pPlayer->m_nZ);
	}
	else if (IS_NPC(m_rcCharacter.GetId()))
	{
		KNpc* pNpc = (KNpc*)(&m_rcCharacter);
		pNpc->m_cNpcServer.TeleportTo(pNpc->GetSceneId(), pNpc->m_nX, pNpc->m_nY, pNpc->m_nZ);
	}*/
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: Fear
// @Description		: 恐惧
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaFear(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: Silence
// @Description		: 沉默
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaSilence(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: Crazy
// @Description		: 疯狂
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaCrazy(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: SlowP
// @Description		: 减低移动速度百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 减速百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaSlowP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: Knock
// @Description		: 击退
// @ReturnCode		: 
// @ArgumentFlag	: ddddd
// @ArgumentComment	: 击退距离
// @ArgumentComment	: 对方坐标x
// @ArgumentComment	: 对方坐标y
// @ArgumentComment	: 对方坐标z
// @ArgumentComment	: 对方方向
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaKnock(QLuaScript& sc)
{
	BOOL	bRet = FALSE;
	INT		nLength			= sc.GetInt(1) * CELL_LENGTH * 2;
	INT		nMePx			= sc.GetInt(2);
	INT		nMePy			= sc.GetInt(3);
	INT		nMePz			= sc.GetInt(4);
	INT		nHimDirection	= sc.GetInt(5);

	INT nDeltaX         = nLength * g_Cos(nHimDirection) / SIN_COS_NUMBER;
	INT nDeltaY         = nLength * g_Sin(nHimDirection) / SIN_COS_NUMBER;

	INT nDestX          = nMePx + nDeltaX;
	INT nDestY          = nMePy + nDeltaY;
	INT nDestZ			= nMePz;

	INT nLastOkX = 0;
	INT nLastOkY = 0;

	m_rcCharacter.CheckObstacleDest(nMePx, nMePy, nMePz, nDestX, nDestY, nDestZ, m_rcCharacter.m_nTouchRange, nLastOkX, nLastOkY);

	if (m_rcCharacter.GetIsFix() == 0 && m_rcCharacter.GetIsStun() == 0 && m_rcCharacter.GetMoveState() != cmsOnKnockedDown)
	{
		if (IS_PLAYER(m_rcCharacter.GetId()))
		{
			KPlayer* pPlayer = (KPlayer*)(&m_rcCharacter);
			pPlayer->m_cPlayerServer.SyncKnockToSelf(nDestX, nDestY, nDestZ);
		}
		else
		{
			m_rcCharacter.KnockTo(nLastOkX, nLastOkY, nDestZ, nHimDirection);
		}
	}

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: JumpTo
// @Description		: 跳跃到指定点
// @ReturnCode		: 
// @ArgumentFlag	: dddddd
// @ArgumentComment	: 起始点坐标x
// @ArgumentComment	: 起始点坐标y
// @ArgumentComment	: 起始点坐标z
// @ArgumentComment	: 目标点坐标x
// @ArgumentComment	: 目标点坐标y
// @ArgumentComment	: 目标点坐标z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaJumpTo(QLuaScript& sc)
{
	BOOL	bRet = FALSE;
	INT		nHimPx			= sc.GetInt(1);
	INT		nHimPy			= sc.GetInt(2);
	INT		nHimPz			= sc.GetInt(3);
	INT		nDestX			= sc.GetInt(4);
	INT		nDestY			= sc.GetInt(5);
	INT		nDestZ			= sc.GetInt(6);

	INT nLastOkX = 0;
	INT nLastOkY = 0;

	m_rcCharacter.CheckObstacleDest(nHimPx, nHimPy, nHimPz, nDestX, nDestY, nDestZ, m_rcCharacter.m_nTouchRange, nLastOkX, nLastOkY);

	sc.PushNumber(nLastOkX);
	sc.PushNumber(nLastOkY);
	sc.PushNumber(nDestZ);

	return 3;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCurLifeV
// @Description		: 增加当前生命值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加生命值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCurLifeV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCurLifeP
// @Description		: 增加当前生命百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加生命百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCurLifeP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCurManaV
// @Description		: 增加当前真气值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加真气值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCurManaV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCurManaP
// @Description		: 增加当前真气百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加真气百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCurManaP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCurPowerV
// @Description		: 增加当前集中值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加集中值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCurPowerV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCurPowerP
// @Description		: 增加当前集中值百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加集中值百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCurPowerP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: DoDeath
// @Description		: 致死
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaDoDeath(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: DoRevive
// @Description		: 复活
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 复活后当前生命值占最大生命值的百分比
// @ArgumentComment	: 复活后当前真气/集中值占最大真气/集中值的百分比
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaDoRevive(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaGetCurrentBufferStack
// @LuaApiName		: GetCurrentBufferStack
// @Description		: 得到当前的buff层数
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nBufTemplateId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaGetCurrentBufferStack(QLuaScript& sc)
{
	INT		nStack = 0;
	INT		nBufTemplateId	= sc.GetInt(1);
	CharacterBuffer* pCharacterBuffer = NULL;
	Buffer* pBuffer = NULL;

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pBuffer = pCharacterBuffer->GetBufferByTemplateId(nBufTemplateId);
	LOG_PROCESS_ERROR(pBuffer);

	nStack = pBuffer->GetCurrentStackCount();

	sc.PushNumber(nStack);

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ClearBuff
// @Description		: 清除对应ID的Buff
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 需要清除的Buff的ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaClearBuff(QLuaScript& sc)
{
	INT		nBufTemplateId	= sc.GetInt(1);
	INT nIdx = 0;
	BOOL bResult = FALSE;
	CharacterBuffer* pCharacterBuffer = NULL;
	Buffer* pBuffer = NULL;

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pBuffer = pCharacterBuffer->GetBufferByTemplateId(nBufTemplateId);
	if (!pBuffer) 
	{
		g_cBufferMgr.RemoveGroundBufferByCasterId(m_rcCharacter.GetId(), nBufTemplateId);
		return 0;
	}

	nIdx = g_cBufferMgr.GetIndexById(pBuffer->GetID());
	bResult = g_cBufferMgr.RemoveBufferFromCharacter(m_rcCharacter.GetId(), nIdx);

	LOG_PROCESS_ERROR(bResult);

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: HasBuff
// @Description		: 判断玩家身上是否有对应模板id的buff
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: buffTemplateId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaHasBuff(QLuaScript& sc)
{
	INT nBufTemplateId	= sc.GetInt(1);
	CharacterBuffer* pCharacterBuffer = NULL;
	Buffer* pBuffer = NULL;

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pBuffer = pCharacterBuffer->GetBufferByTemplateId(nBufTemplateId);
	if (pBuffer) 
	{
		sc.PushNumber(1);
	}
	else
	{
		sc.PushNumber(0);
	}

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ClearCurrentBuff
// @Description		: 清除对应ID的Buff
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaClearCurrentBuff(QLuaScript& sc)
{
	CharacterBuffer*	pCharacterBuffer = NULL;
	GroundBuffer*	pGroundProcBuff = NULL;
	Buffer*			pCharacterProcBuff = NULL;
	BOOL bC = FALSE;
	BOOL bG = FALSE;
	INT nIdx = 0;

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pCharacterProcBuff = pCharacterBuffer->GetCurrentProcessingBuffer();
	pGroundProcBuff		= g_cBufferMgr.GetCurrentProcessingBuff();
	bC = (pCharacterProcBuff != NULL);
	bG = (pGroundProcBuff != NULL);
	PROCESS_ERROR(bC | bG);

	if (pGroundProcBuff)
	{
		g_cBufferMgr.RemoveGroundBuffer(pGroundProcBuff->m_dwId, TRUE);
	}
	else
	{
		nIdx = g_cBufferMgr.GetIndexById(pCharacterProcBuff->GetID());
		g_cBufferMgr.RemoveBufferFromCharacter(m_rcCharacter.GetId(), nIdx);
		
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaSetCurrentBuffer
// @LuaApiName		: SetCurrentBuffer
// @Description		: 设置当前生效buffer
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaSetCurrentBuffer(QLuaScript& sc)
{
	Buffer* pBuffer = NULL;
	INT nBufTemplateId = sc.GetInt(1);
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pBuffer = pCharacterBuffer->GetBufferByTemplateId(nBufTemplateId);
	LOG_PROCESS_ERROR(pBuffer);
	pCharacterBuffer->SetCurrentProcessingBuffer(pBuffer);
	
	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ClearBuffByType
// @Description		: 清除特定类型的buff
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 需要清除的Buff的类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaClearBuffByType(QLuaScript& sc)
{
	INT nType = sc.GetInt(1);
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	{
		const ObjectBuffer::BUFFER_ARRAY& buffers = pCharacterBuffer->GetBuffers();
		std::vector<DWORD> delete_buffer;

		for(size_t i = 0; i < buffers.size(); ++i)
		{
			if( buffers[i]->GetBufferType() == nType)
			{
				INT nIdx = g_cBufferMgr.GetIndexById(buffers[i]->GetID());
				delete_buffer.push_back(nIdx);
			}
		}

		for(size_t i = 0; i < delete_buffer.size(); ++i)
		{
			g_cBufferMgr.RemoveBufferFromCharacter(m_rcCharacter.GetId(), delete_buffer[i]);
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ClearCD
// @Description		: 清除技能的冷却时间
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 技能的Category类型，对应于Skill表
// @ArgumentComment	: 技能的Category值，对应于Skill表的对应Category列
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaClearCD(QLuaScript& sc)
{

	// TODO zhaoyu
	/*
	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
	*/
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: PullTarget
// @Description		: 将目标拉到自己身边，拉回的速度固定，目标需要执行完拉回后才能执行其他指令
// @ReturnCode		: 
// @ArgumentFlag	: ddddd
// @ArgumentComment	: 击退距离
// @ArgumentComment	: 对方坐标x
// @ArgumentComment	: 对方坐标y
// @ArgumentComment	: 对方坐标z
// @ArgumentComment	: 对方方向
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaPullTarget(QLuaScript& sc)
{
	BOOL	bRet = FALSE;
	INT		nLength			= sc.GetInt(1) * CELL_LENGTH * 2;
	INT		nHimPx			= sc.GetInt(2);
	INT		nHimPy			= sc.GetInt(3);
	INT		nHimPz			= sc.GetInt(4);
	INT		nHimDirection	= sc.GetInt(5);

	INT nSelfX, nSelfY, nSelfZ;
	m_rcCharacter.GetPosition(nSelfX, nSelfY, nSelfZ);

	nHimDirection = g_GetDirection(nHimPx, nHimPy, nSelfX, nSelfY);

	INT nDeltaX         = nLength * g_Cos(nHimDirection) / SIN_COS_NUMBER;
	INT nDeltaY         = nLength * g_Sin(nHimDirection) / SIN_COS_NUMBER;

	INT nDestX          = nHimPx + nDeltaX;
	INT nDestY          = nHimPy + nDeltaY;
	INT nDestZ			= nHimPz;

	INT nLastOkX = 0;
	INT nLastOkY = 0;

	m_rcCharacter.CheckObstacleDest(nSelfX, nSelfY, nSelfZ, nDestX, nDestY, nDestZ, m_rcCharacter.m_nTouchRange, nLastOkX, nLastOkY);

	//if (m_rcCharacter.GetIsFix() == 0 && m_rcCharacter.GetIsStun() == 0 && m_rcCharacter.GetMoveState() != cmsOnPull)
	if (m_rcCharacter.GetMoveState() != cmsOnPull)
	{
		if (IS_PLAYER(m_rcCharacter.GetId()))
		{
			KPlayer* pPlayer = (KPlayer*)(&m_rcCharacter);
			pPlayer->m_cPlayerServer.SyncPullToSelf(nDestX, nDestY, nDestZ);
		}
		else
		{
			bRet = m_rcCharacter.Pull(nDestX, nDestY, nDestZ, nHimDirection);
		}
	}
	
	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: SetMapPos
// @Description		: 传送到目标地图的对应点
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 地图ID，对应于MapList之类的配置表
// @ArgumentComment	: X轴偏移量
// @ArgumentComment	: Y轴偏移量
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaSetMapPos(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: Tp
// @Description		: 回门派
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 门派Id，1：无上轩辕；2：诛邪天曜；3：赤炼幽冥；4：魅影森罗
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaTp(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: IgnorarmorV
// @Description		: 忽略目标护甲值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 忽略护甲值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaIgnorarmorV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddBuffTime
// @Description		: 添加对应Buff的持续时间
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Buff表
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列
// @ArgumentComment	: 增加的帧数数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddBuffTime(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: RefreshBuffTime
// @Description		: 刷新对应Buff的持续时间
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 技能的Category类型，对应于Buff表
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaRefreshBuffTime(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ModifyDmgP
// @Description		: 更改造成的伤害百分比，为最终伤害
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Skill表，可填-1，意义为所有技能都增加
// @ArgumentComment	: 技能的Category值，对应于Skill表的对应Category列，当第一个参数为-1时无意义，当第一个参数不为-1时，该值可填-1 
// @ArgumentComment	: 增加伤害的百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaModifyDmgP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ModifyDmgV
// @Description		: 更改造成的伤害的值，为最终伤害
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Buff表
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列，当第一个参数为-1时无意义，当第一个参数不为-1时，该值可填-1  
// @ArgumentComment	: 增加伤害的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaModifyDmgV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddConst
// @Description		: 增加体质
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的体质的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddConst(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddStrth
// @Description		: 增加筋骨
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加筋骨的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddStrth(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddAgile
// @Description		: 增加身法
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加身法的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddAgile(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddIntel
// @Description		: 增加智力
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加智力的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddIntel(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddSpirt
// @Description		: 增加精神
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加精神的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddSpirt(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddAllAttributeV
// @Description		: 增加全属性值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加全属性的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddAllAttributeV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddAllAttributeP
// @Description		: 增加全属性百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加百分比的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddAllAttributeP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddMaxLifeV
// @Description		: 按值增加生命上限值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的生命上限值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddMaxLifeV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddMaxLifeP
// @Description		: 按百分比增加生命上限值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的生命上限百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddMaxLifeP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddMaxManaV
// @Description		: 按值增加真气上限值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的真气上限值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddMaxManaV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddMaxManaP
// @Description		: 按百分比增加真气上限值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的真气上限百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddMaxManaP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddMaxPowerV
// @Description		: 按值增加集中值上限值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的集中值上限值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddMaxPowerV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddMaxPowerP
// @Description		: 按百分比增加集中值上限值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的集中值上限百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddMaxPowerP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddHitRateV
// @Description		: 增加命中值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加命中值数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddHitRateV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCriticalRateV
// @Description		: 增加会心一击值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加会心一击的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCriticalRateV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddSkillCriticalRateV
// @Description		: 增加固定技能的会心一击值
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Skill表
// @ArgumentComment	: 技能的Category值，对应于Skill表的对应Category列，当第一个参数为-1时无意义，当第一个参数不为-1时，该值可填-1  
// @ArgumentComment	: 增加的会心一击值的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddSkillCriticalRateV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddCriticalDmgP
// @Description		: 增加会心一击伤害百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加伤害的百分比
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddCriticalDmgP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddRunSpeedV
// @Description		: 增加移动速度
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加速度数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddRunSpeedV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddArmorV
// @Description		: 增加护甲值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加护甲的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddArmorV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddDodgeV
// @Description		: 增加闪避值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加闪避的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddDodgeV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: HideToNpc
// @Description		: 对npc隐身
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaHideToNpc(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: HideToPlayer
// @Description		: 对玩家隐身
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaHideToPlayer(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddThreat
// @Description		: 增加仇恨值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的仇恨的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddThreat(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ReboundDmgP
// @Description		: 按比例反弹伤害
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 反弹伤害的百分比
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaReboundDmgP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ReboundDmgV
// @Description		: 按值反弹伤害
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 反弹伤害的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaReboundDmgV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ReduceDmg
// @Description		: 按比例缩小伤害
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 缩小伤害百分比
// @ArgumentComment	: 缩小的伤害最大值占生命上限的百分比，填-1为无上限
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaReduceDmg(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ReduceDmgByMana
// @Description		: 使用真气值抵挡伤害
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 抵挡伤害的百分比
// @ArgumentComment	: 每抵挡一点伤害需要消耗的真气值
// @ArgumentComment	: 抵挡伤害的上限占生命上限的百分比，填-1为无上限
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaReduceDmgByMana(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: TransDmgToLife
// @Description		: 造成的伤害转化为自身生命回复
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 伤害转化生命的百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaTransDmgToLife(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: TransDmgToMana
// @Description		: 造成的伤害转化为自身真气回复
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 伤害转化生命真气
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaTransDmgToMana(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ForceAttack
// @Description		: 强制Buff的接收者攻击Buff的发送者
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaForceAttack(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ChangeChannelType
// @Description		: 改变技能的引导类型
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 技能的Category类型，对应于Skill表
// @ArgumentComment	: 技能的Category值，对应于Skill表的对应Category列，当第一个参数为-1时无意义，当第一个参数不为-1时，该值可填-1  
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaChangeChannelType(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ReduceChannelTime
// @Description		: 缩短引导时间
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Skill表，不可填-1
// @ArgumentComment	: 技能的Category值，对应于Skill表的对应Category列，该值可填-1 
// @ArgumentComment	: 缩短的帧数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaReduceChannelTime(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddBuff
// @Description		: 给Buff的接收者添加Buff
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 添加的Buff的ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddBuff(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddBuffToSender
// @Description		: 给Buff的发送者添加Buff
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 添加的Buff的ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddBuffToSender(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: KillSelf
// @Description		: 消除改魔法属性所属的Buff自身
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaKillSelf(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: IgnorBuff
// @Description		: 免疫Buff，可成为被免疫Buff的接收者，但是该Buff的所有效果不会对该目标产生影响
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 免疫的Buff的ID(+:添加-:)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaIgnorBuff(QLuaScript& sc)
{
	INT nBuffID = sc.GetInt(1);

	ObjectBuffer::BUFFER_ARRAY::const_iterator iter;
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pCharacterBuffer->AddIgnoreBuffID(nBuffID);

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: IgnorBuffGroup
// @Description		: 免疫某一个Group的Buff效果，可成为被免疫Buff的接收者，但是该Buff的所有效果不会对该目标产生影响
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 免疫的Buff的Group的ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaIgnorBuffGroup(QLuaScript& sc)
{
	INT nCateGroup = sc.GetInt(1);

	ObjectBuffer::BUFFER_ARRAY::const_iterator iter;
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pCharacterBuffer->AddIgnoreBuffGroup(nCateGroup);

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: IgnorBuffCate
// @Description		: 免疫某一个Cate的Buff效果，可成为被免疫Buff的接收者，但是该Buff的所有效果不会对该目标产生影响
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 技能的Category类型，对应于Buff表，不可填-1
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列，该值可填-1 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaIgnorBuffCate(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);

	ObjectBuffer::BUFFER_ARRAY::const_iterator iter;
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pCharacterBuffer->AddIgnoreBuffCate(nCateType, nCateId);

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: CastSkill
// @Description		: 释放技能，若为Out类条件，则由Buff的拥有者向事件的接收者释放，若为In类事件，则由Buff的拥有者向事件的发送者释放
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 释放的技能的ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaCastSkill(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddTreatLifeV
// @Description		: 增加治疗生命值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddTreatLifeV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddTreatLifeP
// @Description		: 增加治疗生命百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加治疗生命的百分比数值，对应于Skill表的SkillMagnification列
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddTreatLifeP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddTreatManaV
// @Description		: 增加治疗真气值
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddTreatManaV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddTreatManaP
// @Description		: 增加治疗真气百分比
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加治疗真气的百分比数值，对应于Skill表的SkillMagnification列
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddTreatManaP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ReduceCostV
// @Description		: 按值减少释放技能的消耗
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 减少的数值
// @ArgumentComment	: 技能的Category类型，对应于Skill表，可填-1
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列，前一个参数填-1时，该值无意义，前一个参数不为-1时，该值可填-1 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaReduceCostV(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ReduceCostP
// @Description		: 按百分比减少释放技能的消耗
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 减少的百分比
// @ArgumentComment	: 技能的Category类型，对应于Skill表，可填-1
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列，前一个参数填-1时，该值无意义，前一个参数不为-1时，该值可填-1 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaReduceCostP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: IgnorDmg
// @Description		: 免疫伤害，所有可能令Buff拥有者造成生命值减少的效果均不生效
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaIgnorDmg(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddExtSkillBuff
// @Description		: 给某一类技能添加一个Buff
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Skill表，可填-1
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列，前一个参数填-1时，该值无意义，前一个参数不为-1时，该值可填-1 
// @ArgumentComment	: 增加的Buff的ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddExtSkillBuff(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: DelExtSkillBuff
// @Description		: 删除某一类技能的一个Buff
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Skill表，可填-1
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列，前一个参数填-1时，该值无意义，前一个参数不为-1时，该值可填-1 
// @ArgumentComment	: 删除的Buff的ID 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaDelExtSkillBuff(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddMagnification
// @Description		: 增加某一类技能的攻击力百分比
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能的Category类型，对应于Skill表，可填-1
// @ArgumentComment	: 技能的Category值，对应于Buff表的对应Category列，前一个参数填-1时，该值无意义，前一个参数不为-1时，该值可填-1 
// @ArgumentComment	: 增加的百分比数值，正数为增加，负数为减少
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddMagnification(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AddWeaponDmgP
// @Description		: 按百分比增加武器攻击力
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加的百分比数值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	LuaCharacterBufferEffect::LuaAddWeaponDmgP(QLuaScript& sc)
{
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: GetCurrProcBuffFightInfo
// @Description		: 獲得目前buff的nSkillId,nMagnification,nGrow,nIfUseSkillLv, nK
// @ReturnCode		: 
// @ArgumentFlag	: [d]
// @ArgumentComment	: nBufferTemplateId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaGetCurrProcBuffFightInfo(QLuaScript& sc)
{
	INT nBuffTemplateId = sc.GetInt(1);
	const Buffer*		pCharacterProcBuff = NULL;
	const GroundBuffer*	pGroundProcBuff = NULL;
	CharacterBuffer* pCharacterBuffer = NULL;
	BOOL bC = FALSE;
	BOOL bG = FALSE;
	INT nSkillId		= 0;
	INT nMagnification	= 0;
	INT nGrow			= 0;
	INT nK				= 0;
	INT nIfUseSkillLv	= 0;

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pGroundProcBuff	= g_cBufferMgr.GetCurrentProcessingBuff();
	if(pGroundProcBuff)
	{
		nSkillId		= pGroundProcBuff->GetCastSkill();
		nMagnification	= pGroundProcBuff->GetMagnification();
		nGrow			= pGroundProcBuff->GetGrow();
		nIfUseSkillLv = pGroundProcBuff->GetIfUseSkillLv();
		nK = pGroundProcBuff->GetK();
	}
	else
	{
		pCharacterProcBuff = pCharacterBuffer->GetBufferByTemplateId(nBuffTemplateId);
		if(!pCharacterProcBuff)
		{
			pCharacterProcBuff = pCharacterBuffer->GetCurrentProcessingBuffer();
		}
		PROCESS_ERROR(pCharacterProcBuff);
		nSkillId		= pCharacterProcBuff->GetCastSkill();
		nMagnification	= pCharacterProcBuff->GetMagnification();
		nGrow			= pCharacterProcBuff->GetGrow();
		nIfUseSkillLv	= pCharacterProcBuff->GetIfUseSkillLv();
		nK				= pCharacterProcBuff->GetK();
	}

	sc.PushNumber(nSkillId);
	sc.PushNumber(nMagnification);
	sc.PushNumber(nGrow);
	sc.PushNumber(nIfUseSkillLv);
	sc.PushNumber(nK);

	return 5;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: GetGroundBuffPosition
// @Description		: 得到地面buffer位置
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaGetGroundBuffPosition(QLuaScript& sc)
{
	INT nX = 0, nY = 0, nZ = 0;
	GroundBuffer* pGroundProcBuff = NULL;
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pGroundProcBuff = g_cBufferMgr.GetCurrentProcessingBuff();
	m_rcCharacter.GetPosition(nX, nY, nZ);
	if(pGroundProcBuff)
	{
		pGroundProcBuff->GetPosition(nX, nY, nZ);
	}
	else
	{
		//ASSERT(FALSE && "处理魔法属性时当前buffer为空！");
	}
  
	sc.PushNumber(nX);
	sc.PushNumber(nY);
	sc.PushNumber(nZ);

EXIT0:
	return 3;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: ResetBufferTime
// @Description		: 重置Buffer时间
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @ArgumentComment	: 修改值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaResetBufferTime(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);
	INT		nValue			= sc.GetInt(3);

	ObjectBuffer::BUFFER_ARRAY::const_iterator iter;
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	{
		const ObjectBuffer::BUFFER_ARRAY& buffers = pCharacterBuffer->GetBuffers();

		for( iter = buffers.begin(); iter != buffers.end(); ++iter)
		{
			if( (*iter)->GetBuffCategory(nCateType) == nCateId )
			{
				(*iter)->AddBuffPersist(nValue);
			}
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: RefreshBufferTime
// @Description		: 重置Buffer时间为原时间
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaRefreshBufferTime(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);

	ObjectBuffer::BUFFER_ARRAY::const_iterator iter;
	CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	{
		const ObjectBuffer::BUFFER_ARRAY& buffers = pCharacterBuffer->GetBuffers();

		for( iter = buffers.begin(); iter != buffers.end(); ++iter)
		{
			switch(nCateType)
			{
			case 1:
				{
					if( (*iter)->GetBuffCategory(nCateType) == nCateId )
					{
						(*iter)->RefreshBuffPersist();
					}
				}
				break;
			case 2:
				break;
			case 3:
				break;
			}
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: AppendData
// @Description		: 给玩家身上Buff添加附加数据
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: buff模板id
// @ArgumentComment	: 附加数据类型
// @ArgumentComment	: 附加数据值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaAppendData(QLuaScript& sc)
{
	BOOL	bRet								= FALSE;
	INT		nBuffTemplateId						= sc.GetInt(1);
	INT		nType								= sc.GetInt(2);
	INT		nValue								= sc.GetInt(3);
	CONST BufferTemplate* pTemplate				= NULL;
	CharacterBuffer*	pCharacterBuffer		= NULL;
	GroundBuffer*	pGroundProcBuff				= NULL;
	Buffer*			pCharacterProcBuff			= NULL;

	pTemplate = g_cBufferSetting.GetBufferTemplate(nBuffTemplateId);
	LOG_PROCESS_ERROR(pTemplate);

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	if(!pTemplate->m_bIsGroundBuffer && pTemplate->m_fBuffRadius == 0.0f)
	{
		Buffer* pCharacterProcBuff = pCharacterBuffer->GetBufferByTemplateId(nBuffTemplateId);
		if(!pCharacterProcBuff)
		{
			pCharacterProcBuff = pCharacterBuffer->GetCurrentProcessingBuffer();
		}
		LOG_PROCESS_ERROR(pCharacterProcBuff);
		pCharacterProcBuff->AppendData(nType, nValue);
	}
	else
	{
		pGroundProcBuff	= g_cBufferMgr.GetCurrentProcessingBuff();
		LOG_PROCESS_ERROR(pGroundProcBuff);
		pGroundProcBuff->AppendData(nType, nValue);
	}

	bRet = TRUE;
EXIT0:
	sc.PushNumber(bRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: GetAppendData
// @Description		: 给玩家身上Buff添加附加数据
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 附加数据类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaGetAppendData(QLuaScript& sc)
{
	INT nBuffTemplateId						= sc.GetInt(1);
	INT	nType								= sc.GetInt(2);
	INT	nValue								= 0;

	CONST BufferTemplate* pTemplate			= NULL;
	CharacterBuffer*	pCharacterBuffer	= NULL;
	GroundBuffer*	pGroundProcBuff			= NULL;
	Buffer*			pCharacterProcBuff		= NULL;

	pTemplate = g_cBufferSetting.GetBufferTemplate(nBuffTemplateId);
	LOG_PROCESS_ERROR(pTemplate);

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	if(!pTemplate->m_bIsGroundBuffer && pTemplate->m_fBuffRadius == 0.0f)
	{
		pCharacterProcBuff = pCharacterBuffer->GetBufferByTemplateId(nBuffTemplateId);
		if(!pCharacterProcBuff)
		{
			pCharacterProcBuff = pCharacterBuffer->GetCurrentProcessingBuffer();
		}
		LOG_PROCESS_ERROR(pCharacterProcBuff);
		nValue = pCharacterProcBuff->GetAppendData(nType);
	}
	else
	{
		pGroundProcBuff	= g_cBufferMgr.GetCurrentProcessingBuff();
		LOG_PROCESS_ERROR(pGroundProcBuff);
		nValue = pGroundProcBuff->GetAppendData(nType);
	}
	
	sc.PushNumber(nValue);

	return 1;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: GetDestPosition
// @Description		: 得到玩家身上buff设置的目标点
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: buff模板id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaGetDestPosition(QLuaScript& sc)
{
	INT nDestX = 0;
	INT nDestY = 0;
	INT nDestZ = 0;

	INT nBuffTemplateId						= sc.GetInt(1);
	INT	nValue								= 0;

	CONST BufferTemplate* pTemplate			= NULL;
	CharacterBuffer*	pCharacterBuffer	= NULL;
	GroundBuffer*	pGroundProcBuff			= NULL;
	Buffer*			pCharacterProcBuff		= NULL;

	pTemplate = g_cBufferSetting.GetBufferTemplate(nBuffTemplateId);
	LOG_PROCESS_ERROR(pTemplate);

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	if(!pTemplate->m_bIsGroundBuffer)
	{
		pCharacterProcBuff = pCharacterBuffer->GetBufferByTemplateId(nBuffTemplateId);
		if(!pCharacterProcBuff)
		{
			pCharacterProcBuff = pCharacterBuffer->GetCurrentProcessingBuffer();
		}
		LOG_PROCESS_ERROR(pCharacterProcBuff);
		pCharacterProcBuff->GetDestPosition(nDestX, nDestY, nDestZ);
	}
	else
	{
		pGroundProcBuff	= g_cBufferMgr.GetCurrentProcessingBuff();
		LOG_PROCESS_ERROR(pGroundProcBuff);
		pGroundProcBuff->GetPosition(nDestX, nDestY, nDestZ);
	}

EXIT0:
	sc.PushNumber(nDestX);
	sc.PushNumber(nDestY);
	sc.PushNumber(nDestZ);
	return 3;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: SetTransParam
// @Description		: 设置转换属性数据
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 转换的原属性
// @ArgumentComment	: 要转换的属性
// @ArgumentComment	: 转换百分比
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaSetTransParam(QLuaScript& sc)
{
	BOOL	bRet								= FALSE;
	INT		nSrcParam							= sc.GetInt(1);
	INT		nDstParam							= sc.GetInt(2);
	DOUBLE	dPercent							= sc.GetNum(3);
	CharacterBuffer*	pCharacterBuffer		= NULL;

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	bRet = pCharacterBuffer->SetTransParam(nSrcParam, nDstParam, dPercent);

EXIT0:
	sc.PushNumber(bRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterBufferEffect
// @LuaApiName		: GetTransParam
// @Description		: 得到一个属性的关联属性
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 原属性
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterBufferEffect::LuaGetTransParam(QLuaScript& sc)
{
	BOOL bRet = FALSE;
	INT		nDstParam							= 0;
	INT		nPercent							= 0;
	INT		nSrcParam							= sc.GetInt(1);
	CharacterBuffer*	pCharacterBuffer		= NULL;
	MAP_TRANSPARAM_DATA mapParam;

	pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
	LOG_PROCESS_ERROR(pCharacterBuffer);

	pCharacterBuffer->GetTransParam(nSrcParam, mapParam);

	sc.PushTable();

	for(MAP_TRANSPARAM_DATA::iterator it = mapParam.begin(); it != mapParam.end(); ++it)
	{
		sc.PushNumber(it->second);
		sc.SetTableIndex(it->first);
	}

EXIT0:
	return 1;
}

#include "luabuffereffect_table.hpp"