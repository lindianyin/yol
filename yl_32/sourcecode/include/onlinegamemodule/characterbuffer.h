/* -------------------------------------------------------------------------
//	文件名		：	buffermanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __CHARACTER_BUFFER_H__
#define __CHARACTER_BUFFER_H__

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegamemodule/objectbuffer.h"
//#include "./buffer/buftriggereffect.h"
#include "onlinegamemodule/luabuffereffect.h"
#include <vector>

class Buffer;

#pragma pack(push, 1)
struct BufferSaveData
{
	DWORD dwCasterId;
	INT nSkillId;
	INT nBufferTemplateId;
	INT nRemainPersist;			// 剩余时间
};
#pragma pack(pop)

typedef std::map<INT, DOUBLE> MAP_TRANSPARAM_DATA;
typedef std::map<INT, MAP_TRANSPARAM_DATA> MAP_TRANS_PARAM;


class CharacterBuffer : public KCharacterCtrlBase, public ObjectBuffer
{
public:
    CharacterBuffer(KCharacter& rcPlayer);

	~CharacterBuffer();

	enum { emKCtrlId = emCHARACTER_CTRL_BUFFER };

	typedef std::vector<Buffer*> BUFFER_ARRAY;

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	BOOL Broadcast_AddBuffer(CONST Buffer* pBuf,BOOL bAddOnLogin = FALSE,INT nCasterID = 0);

	BOOL Broadcast_RemoveBuffer(Buffer* pBuf);

	BOOL Broadcast_StackBuffer(CONST Buffer* pBuf);

	// 存盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	// 读盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	virtual BOOL OnClientReady();

	CONST LuaCharacterBufferEffect& GetScriptInterface() CONST;

	INT GetAppendDataId(INT nType);

	BOOL SetTransParam(INT nSrcParam, INT nDstParam, DOUBLE dPercent);

	VOID GetTransParam(INT nSrcParam, MAP_TRANSPARAM_DATA& mapParam);

private:
	LuaCharacterBufferEffect m_cLunaObj;

	MAP_TRANS_PARAM m_mapTransParam;

};

#endif