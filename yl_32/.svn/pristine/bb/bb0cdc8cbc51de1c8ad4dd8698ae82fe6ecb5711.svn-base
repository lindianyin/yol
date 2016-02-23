/* -------------------------------------------------------------------------
//	文件名		：	bufferprotocolprocessor.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUFFER_PROTOCOL_PROCESSOR_H__
#define __BUFFER_PROTOCOL_PROCESSOR_H__

class Buffer;
class KCharacter;

class BufferProtocolProcessor
{
public:
    // 通知周围玩家增加了角色buffer
	static BOOL Broadcast_AddCharacterBuffer(KCharacter& rcHost,CONST Buffer* pBuf,BOOL bAddOnLogin = FALSE, INT nCasterID = 0);

	// 同步叠加buff
	static BOOL Broadcast_StackCharacterBuffer(KCharacter& rcHost,CONST Buffer* pBuf);

	// 通知周围玩家减少了角色buffer
	static BOOL Broadcast_RemoveCharacterBuffer(KCharacter& rcHost, Buffer* pBuf);

	// 对地buffer广播，放在groundbuffer里面
};

#endif