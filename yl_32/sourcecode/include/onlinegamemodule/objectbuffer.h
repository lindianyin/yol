/* -------------------------------------------------------------------------
//	文件名		：	objectbuffer.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __OBJECT_BUFFER_H__
#define __OBJECT_BUFFER_H__

#include "onlinegamemodule/bufferdef.h"
#include <vector>

class Buffer;
class KCharacter;
struct BufferTemplate;

class ObjectBuffer
{
public:
	 typedef std::vector<Buffer*> BUFFER_ARRAY;

	 BOOL Init();

	 BOOL UnInit();

	 VOID Breath();

	 BOOL HasBuffer(Buffer* buf);

	 BOOL AddBuffer(Buffer* buf, BOOL bSync = TRUE, INT nCasterID = 0);

	 BOOL RemoveBuffer(Buffer* buf, BOOL bSync = TRUE);

	 Buffer* GetBuffer(INT nBufID);

	 Buffer* GetBufferByTemplateId(INT nBufTemplateId);

	 Buffer* StackBuffer(INT nCategory);

	 Buffer* OverWriteBuffer(INT nCategory,INT nTemplateId);

	 Buffer* GetBufferByStackCategory(INT nCategory);

	 BOOL CheckBuffLifeTime(Buffer* buf);

	 BOOL CheckBuffTrigger(LPCSTR szEvent, KCharacter* pEventReceive, KCharacter* pEventSender);

	 CONST Buffer* GetCurrentProcessingBuffer() CONST { return m_pCurrentProcessingBuffer; }
	 Buffer* GetCurrentProcessingBuffer() { return m_pCurrentProcessingBuffer; }

	 VOID SetCurrentProcessingBuffer(Buffer* pBuffer) { m_pCurrentProcessingBuffer = pBuffer; }

	 // 通知客户端增加了一个Buffer
	 virtual BOOL Broadcast_AddBuffer(CONST Buffer* pBuf,BOOL bAddOnLogin = FALSE, INT nCasterID = 0) = 0;

	 // 通知叠加buff
	 virtual BOOL Broadcast_StackBuffer(CONST Buffer* pBuf) = 0;
	 
	 // 通知客户端删除了一个Buffer
	 virtual BOOL Broadcast_RemoveBuffer(Buffer* pBuf) = 0;

	 //获取所有Buffer
	 const BUFFER_ARRAY& GetBuffers() const { return m_arrBuffers; }

	 /**
	 * 检查是否免疫此Buff
	 * 返回:true,false(免疫，不免疫)
	 */
	 BOOL CheckIgnore(const BufferTemplate* pBufferConfig);

	 /**
	 * 添加免疫Buff ID
	 */
	 VOID AddIgnoreBuffID(INT buff_id);

	 /**
	 * 添加免疫Buff Group
	 */
	 VOID AddIgnoreBuffGroup(INT buff_group);
	 
	 /**
	 * 添加免疫Buff Cate
	 */
	 VOID AddIgnoreBuffCate(INT CateType, INT CateId);

protected:
	BUFFER_ARRAY m_arrBuffers;
	Buffer* m_pCurrentProcessingBuffer;

private:
	std::vector<INT> m_arrIgnoreBuffID;				//免疫的Buff列表(包括Debuf)
	std::vector<INT> m_arrIgnoreBuffGroup;			//免疫的Buff Group
	struct CateInfo {
		INT CateType;
		INT CateId;
		bool operator==(const CateInfo& right)
		{
			return (CateType == right.CateType) && (CateId == right.CateId);
		}
	};
	std::vector<CateInfo> m_arrIgnoreBuffCate;		//免疫的Buff Category

	struct BUFF_CATE
	{
		DWORD dwCasterId;
		DWORD dwCharacterId;
		INT nBuffTemplateId;
		INT nSkillId;
	};
};

#endif
