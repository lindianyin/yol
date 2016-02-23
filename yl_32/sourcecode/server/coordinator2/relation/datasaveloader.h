/* -------------------------------------------------------------------------
//	文件名		：	dbsaver.h
//	创建者		：	
//	创建时间	：	2008-5-21 9:19:33
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGC_DATASAVELOADER_H__
#define __KGC_DATASAVELOADER_H__

#include "playerrelation_i.h"
// -------------------------------------------------------------------------

class RelationSaveLoader : public IKRelationSaver
{
	virtual BOOL SaveBegin(INT nType, INT nDataLen, BOOL bMaster = TRUE);

	virtual BOOL SaveEnd(INT nType);

	virtual BOOL AddRecord(INT nPlayerId, LPVOID pData = NULL);

	LPBYTE m_pbySaveBuf;
	LPBYTE m_pWritePos;
	INT m_nCurType;
	INT m_nCurDataLen;
	WORD* m_pwRelayNum;
public:
	VOID Reset()
	{
		m_pWritePos = m_pbySaveBuf;
		m_nCurType = 0;
		m_nCurDataLen = 0;
		m_pwRelayNum = NULL;
	}
	BOOL AddHead(INT nPlayerId);
	RelationSaveLoader()
	{
		m_pbySaveBuf = new BYTE[1024*1024];
		Reset();
	}

	~RelationSaveLoader()
	{
		if (m_pbySaveBuf)
		{
			delete[] m_pbySaveBuf;
			m_pbySaveBuf = NULL;
		}
	}

	LPBYTE GetSaveBuf()
	{
		return m_pbySaveBuf;
	}

	UINT GetSize()
	{
		return m_pWritePos - m_pbySaveBuf;
	}

	static BOOL OnLoadRelayData(INT nPlayerId, LPBYTE pData, UINT uDataSize);
};

// -------------------------------------------------------------------------
#endif /* __KGC_DATASAVELOADER_H__ */
