/* -------------------------------------------------------------------------
//	文件名		：	kmail.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/16 15:56:52
//	功能描述	：	邮件
//
// -----------------------------------------------------------------------*/
#ifndef __KMAIL_H__
#define __KMAIL_H__

// -------------------------------------------------------------------------
#ifdef GAME_CENTER

#include "gcmodule/mail/kmail_datadef.h"
#include <map>
#include <string>

class IKEnumMailItem
{
public:
	virtual BOOL Next(KItemData_Common* pItem, INT& nItemIndex) = 0;
	virtual VOID Reset() = 0;
	virtual INT GetCount() = 0;
};

class KEnumMailItem : public IKEnumMailItem
{
public:
	struct KMAILITEM
	{
		KItemData_Common sItemData;
		BOOL		bLock;			// 是否被锁定
	};
	typedef std::map<INT, KMAILITEM>	KDB_MAP;
public:
	KEnumMailItem() : m_pMap(NULL)
	{
	}
	virtual ~KEnumMailItem()
	{
	}
	BOOL Init(KDB_MAP* pMap)
	{
		QCONFIRM_RET_FALSE(pMap);
		m_pMap = pMap;
		m_iter = pMap->begin();
		return TRUE;
	}
	virtual BOOL Next(KItemData_Common* pItem, INT& nItemIndex)
	{
		QCONFIRM_RET_FALSE(pItem);
		if (m_iter == m_pMap->end())
			return FALSE;
		*pItem = m_iter->second.sItemData;
		nItemIndex = m_iter->first;
		m_iter++;
		return TRUE;
	}
	virtual VOID Reset()
	{
		m_iter = m_pMap->begin();
	}
	virtual INT GetCount()
	{
		return (INT)m_pMap->size();
	}
private:
	KDB_MAP*			m_pMap;
	KDB_MAP::iterator	m_iter;
};

class KMail
{
public:
	typedef std::string KMAIL_STRING;

	struct KMAILHEAD
	{
		DWORD			dwSenderId;		// 发件人
		DWORD			dwReceiverId;	// 收件人
		CHAR			szSender[32];	// 发件人姓名
		KMAIL_DATETIME	sSendTime;		// 发送时间
		INT				nType;			// 邮件类型
		INT				nStat;			// 邮件状态
		INT64			nMailID;		// 邮件ID
		INT				nIconId;		// 邮件显示图标ID
	};
private:
	typedef KEnumMailItem::KMAILITEM	KMAILITEM;
	typedef KEnumMailItem::KDB_MAP		KDB_MAP;
public:
	KMail();
	~KMail();
	VOID Clear();
	/***********************************************************
	 Function		: Load
	 Description	: 加载邮件信息
	 Return			: BOOL
	 Param			: pData		邮件信息
	 ***********************************************************/
	BOOL Load(CONST KMAIL_INFO* pData);
	/***********************************************************
	 Function		: GetInfo
	 Description	: 获取邮件信息
	 Return			: CONST KMAILHEAD*	邮件信息
	 ***********************************************************/
	CONST KMAILHEAD* GetInfo()
	{
		return &m_sMailInfo;
	}
	/***********************************************************
	 Function		: GetCaption
	 Description	: 获取邮件标题
	 Return			: LPCSTR		邮件标题
	 Param			: nSize[out]	邮件信息长度
	 ***********************************************************/
	LPCSTR GetCaption(INT &nSize);
	/***********************************************************
	 Function		: GetContent
	 Description	: 获取邮件正文
	 Return			: LPCSTR		邮件正文
	 Param			: nSize[out]	邮件正文长度
	 ***********************************************************/
	LPCSTR GetContent(INT &nSize);
	/***********************************************************
	 Function		: GetCondition
	 Description	: 获取邮件接收条件
	 Return			: LPCSTR		邮件接收条件
	 Param			: nSize[out]	邮件接收条件长度
	 ***********************************************************/
	LPCSTR GetCondition(INT &nSize);
	/***********************************************************
	 Function		: IsDirty
	 Description	: 邮件是否修改过
	 Return			: BOOL
	 ***********************************************************/
	BOOL IsDirty()
	{
		return m_bDirty;
	}
	/***********************************************************
	 Function		: CleanDirtiness
	 Description	: 清除脏标志
	 Return			: VOID
	 ***********************************************************/
	VOID CleanDirtiness()
	{
		m_bDirty = FALSE;
	}
	/***********************************************************
	 Function		: SetStatus
	 Description	: 修改状态
	 Return			: BOOL
	 Param			: nStat		邮件状态
	 ***********************************************************/
	BOOL SetStatus(INT nStat)
	{
		QCONFIRM_RET_FALSE(m_sMailInfo.nStat != nStat);
		m_sMailInfo.nStat = nStat;
		m_bDirty = TRUE;
		return TRUE;
	}
	/***********************************************************
	 Function		: RemoveItem
	 Description	: 删除物品
	 Return			: BOOL
	 Param			: nIndex	物品索引
	 ***********************************************************/
	BOOL RemoveItem(INT nIndex);
	/***********************************************************
	 Function		: Save
	 Description	: 保存邮件正文到指定缓冲区
	 Return			: BOOL
	 Param			: pBuffer		缓冲区
	 Param			: nBufLen		缓冲区长度
	 Param			: nRetLen[out]	邮件信息长度
	 ***********************************************************/
	BOOL Save(CHAR *pBuffer, INT nBufLen, INT &nRetLen);
	/***********************************************************
	 Function		: GetAttachBufLen
	 Description	: 获取附件缓冲区长度
	 Return			: INT	附件缓冲区长度
	 ***********************************************************/
	INT GetAttachBufLen();
	/***********************************************************
	 Function		: SaveAttachment
	 Description	: 保存邮件附件到指定缓冲区
	 Return			: BOOL
	 Param			: pData		缓冲区
	 Param			: nLen		缓冲区长度
	 ***********************************************************/
	BOOL SaveAttachment(CHAR *pData, INT nLen);
	/***********************************************************
	 Function		: GetAttachItem
	 Description	: 获取指定索引附件
	 Return			: CONST TDBItemData *
	 Param			: nIndex	物品局部索引（从0开始）
	 ***********************************************************/
	CONST KItemData_Common* GetAttachItem(INT nIndex);
	/***********************************************************
	 Function		: DeleteAttachItem
	 Description	: 删除指定索引附件
	 Return			: BOOL
	 Param			: nIndex	物品局部索引（从0开始）
	 ***********************************************************/
	BOOL DeleteAttachItem(INT nIndex);
	/***********************************************************
	 Function		: GetItemList
	 Description	: 获取附件物品列表
	 Return			: BOOL
	 Param			: ppEnum[out]	枚举器
	 ***********************************************************/
	BOOL GetItemList(IKEnumMailItem **ppEnum)
	{
		m_enumItemList.Init(&m_mapItems);
		*ppEnum = &m_enumItemList;
		return TRUE;
	}
	/***********************************************************
	 Function		: CanPost
	 Description	: 是否符合接收条件
	 Return			: BOOL
	 Param			: VOID
	 ***********************************************************/
	BOOL CanPost();
	static time_t MkTime(CONST KMAIL_DATETIME* pTime);
	// 锁定附件物品
	VOID LockItem(INT nIndex);
	// 某附件物品是否锁定
	BOOL IsItemLocked(INT nIndex);
private:
	// 加载附件
	BOOL LoadAttachment(LPSTR pData, INT nLen);
private:
	KMAILHEAD						m_sMailInfo;		// 邮件信息
	KMAIL_STRING					m_strCaption;		// 邮件标题
	KMAIL_STRING					m_strContent;		// 邮件正文
	CHAR	m_strCond[KD_MAIL_MAX_COND_LENGTH];			// 邮件接收条件
	INT								m_nCondLen;			// 接收条件长度
	KDB_MAP							m_mapItems;			// 附带物品
	BOOL							m_bDirty;			// 邮件是否修改过
	KEnumMailItem					m_enumItemList;		// 物品枚举器
};

#endif /* GAME_CENTER */
// -------------------------------------------------------------------------

#endif /* __KMAIL_H__ */
