/* -------------------------------------------------------------------------
//	文件名		：	buffermanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__

#include <map>
#include "onlinegamemodule/bufferdef.h"
#include "onlinegameworld/kidobjpool.h"
#include "buffer.h"
#include "groundbuffer.h"

class Buffer;
class GroundBuffer;
class CharacterBuffer;

class BufferManager
{
public:
	BufferManager();

	~BufferManager();

	BOOL Init();

	BOOL UnInit();

	VOID Breath();

	INT AddBufferToCharacter(DWORD dwCasterId, 
		INT nSkillId, 
		DWORD dwCharacterId, 
		INT nBufferTemplateId,
		INT nBuffPersist = 0,
		BOOL bSync = TRUE,
		INT nDestX = 0,
		INT nDestY = 0, 
		INT nDestZ = 0,
		INT nSkillLevel = 1);  // Buffer全部默认1级

	BufferStackMode CheckBuffStack(KCharacter* pCharacter, Buffer* pBuf,CharacterBuffer* pCharacterBuf);

	Buffer* ReplaceBuffer(KCharacter* pCharacter, Buffer* pBuf, Buffer* pOldBuf);

	BOOL RemoveBufferFromCharacter(DWORD dwCharacterID, DWORD nBufferIndex, BOOL bSync = TRUE);

	//检测玩家的buff事件效果，由事件触发到脚本，脚本回调到这里
	BOOL CheckCharacterBuffTrigger(DWORD dwCharacterID,LPCSTR szEvent, KCharacter* pEventSender, KCharacter* pEventReceiver = NULL);

	VOID RemoveGroundBufferBySceneId(INT nSceneId);

	BOOL RemoveGroundBufferByCasterId(INT nCasterId, DWORD dwTemplateId = 0);

	//玩家buff
	Buffer* GetBufferById(DWORD nID);

	Buffer* GetBufferByIndex(INT nIndex);

	UINT GetIndexById(DWORD nID);

	INT AddBuffer(INT nTemplateId, INT nPersist = 0, DWORD dwId = KD_BAD_ID);

	BOOL RemoveBuffer(DWORD dwId,BOOL bClearEffect = TRUE);

	//地面buff
	GroundBuffer* GetGroundBufferById(DWORD nID);

	GroundBuffer* GetGroundBufferByIndex(INT nIndex);

	INT AddGroundBuffer(INT nTemplateId, DWORD dwCharacterId, INT nSkillId, INT dwSceneId,INT nX,INT nY,INT nZ, BOOL bSync = TRUE, INT nBuffPersist = 0, INT nSkillLevel=1);

	BOOL RemoveGroundBuffer(DWORD dwId, BOOL bSync = TRUE);

	BOOL RemoveGroundBufferByIndex(INT nIndex);

	GroundBuffer* GetCurrentProcessingBuff() CONST { return m_pCurrentProcessingGroundBuff; }

	INT FindSamePassiveGroundBuffer(INT nTemplateId, DWORD dwCharacterId);

	INT GetGroundBufferByCate(INT nCate, DWORD dwCharacterId);

	GroundBuffer* GetGroundBufferByTemplateId(INT nBuffTemplateId, DWORD dwCharacterId);

private:
	TKIdObjPool<Buffer> m_cBufferPool;				//玩家buff
	TKIdObjPool<GroundBuffer> m_cGroundBufferPool;	//地面buff

	GroundBuffer*			m_pCurrentProcessingGroundBuff;	// 正在处理的对地Buff
};

extern BufferManager g_cBufferMgr;

#endif