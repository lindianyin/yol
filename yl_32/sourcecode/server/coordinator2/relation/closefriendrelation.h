/* -------------------------------------------------------------------------
//	文件名		：	kclosefriendrelation.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KCLOSEFRIENDRELATION_H__
#define __KCLOSEFRIENDRELATION_H__
#include "playerrelation_i.h"
// -------------------------------------------------------------------------
class  KCloseFriendRelation : public IKCloseFriendRelation
{
public:

#pragma pack(push, 1)
	struct KCLOSEFRIEND_DBINFO
	{
		WORD			wVersion;
		INT				nGainBindCoinMaster;
		INT				nGainBindCoinTarget;
		BYTE			bySendMail;
		time_t			tCreateTime;
	};
	struct KCLOSEFRIEND_INFO
	{
		KCLOSEFRIEND_INFO() { btModify = 0; }
		BOOL IsModify() { return btModify == 1; }
		LPVOID GetData() { return this; }
		KCLOSEFRIEND_DBINFO	sDBInfo;						// 存盘数据
		BYTE				btModify;
	};
#pragma pack(pop)
public:
	static KCLOSEFRIEND_INFO GetDefaultValue();
	static KCLOSEFRIEND_INFO Load(LPVOID pData, INT nLen);
	
	KCloseFriendRelation(INT nType, IKRelationFactory *pFactory);
	virtual ~KCloseFriendRelation();
	VOID Init(INT nMaster, INT nTarget, KCLOSEFRIEND_INFO *pInfo);
	// 查询子接口
	virtual BOOL QueryRelation(INT nType, LPVOID *ppv);
	// 查询关系类型
	virtual INT GetType();
	// 获取附加信息长度
	virtual INT GetInfLen();
	// 保存附加数据到指定缓冲区（用于同步好友信息）
	virtual BOOL SaveInfo(CHAR *pBuf, INT nLen);
	// 加载附加数据
	virtual BOOL LoadInfo(CHAR *pBuf, INT nLen);
	// 获取主位/次位玩家（bAsMaster为TRUE表示主位，FALSE表示次位）
	virtual INT	GetRole(BOOL bAsMaster);

	// 领取绑定金币
	virtual	BOOL GainBindCoin(BOOL bIsMaster);
	/***********************************************************************
	Function		: GetBindCoin
	Description		: 获取可以领取的绑定金币数
	Return			: INT 绑定金币数量
	Param			: bMaster 是否为主位
	**********************************************************************/
	virtual	INT GetBindCoin(BOOL bMaster);

	/***********************************************************************
	Function		: AddBindCoin
	Description		: 添加可领取的绑定金币数量
	Param			: INT 可领取绑定金币数量
	Param			: bMaster 是否为主位
	**********************************************************************/	
	virtual	BOOL AddBindCoin(INT nCoin, BOOL bMaster);

	/***********************************************************************
	Function		: CheckOverTime
	Description		: 是否已经超过一年了
	Return			: FALSE：还没超时， TRUE：已经超时
	**********************************************************************/
	virtual	BOOL CheckOverTime();
	// 获取离一年还剩余多少时间
	INT GetRemanentTime();
	// 获取附加数据
	// 密友关系超过一年的信件是否发送了
	virtual	BOOL CanSendMail();
	// 设置邮件已发送标记
	virtual	BOOL SetSendMail(BOOL bSend);

	virtual LPVOID GetInfo(INT &nLen)
	{
		nLen = sizeof(KCLOSEFRIEND_DBINFO);
		return &m_pInfo->sDBInfo;
	}
	
	virtual	INT GetOtherPlayerId(INT nSelfId);

	virtual BOOL IsMaster(INT nSelfId)
	{
		return nSelfId == m_nMaster;
	}
private:
	INT						m_nType;
	IKRelationFactory*		m_pFactory;
	INT						m_nMaster;
	INT						m_nTarget;
	KCLOSEFRIEND_INFO*		m_pInfo;
};

// -------------------------------------------------------------------------

#endif /* __KCLOSEFRIENDRELATION_H__ */
