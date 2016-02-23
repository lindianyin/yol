#ifndef	__KBIDFRIENDRELATION_H__
#define __KBIDFRIENDRELATION_H__

#include "playerrelation_i.h"

// 普通好友接口实现
class KBidFriendRelation : public IKBidFriendRelation
{
public:
	// 增加好感度的途径
	enum KE_METHOD
	{
		emKMETHOD_NORMAL,		// 正常途径
		emKMETHOD_IBITEM,		// 通过IB道具
		emKMETHOD_COUNT
	};
#pragma pack(push, 1)
	struct KFRIEND_DBINFO
	{
		INT				nFavor;
		DWORD			dwLastTalkTime;
		WORD			aryDayPoint[emKMETHOD_COUNT];	// 当日已加的亲密度
	};
	struct KFRIEND_INFO
	{
		KFRIEND_INFO() { btModify = 0; }
		BOOL IsModify() { return btModify == 1; }
		LPVOID GetData() { return this; }
		KFRIEND_DBINFO	sDBInfo;						// 存盘数据
		INT				aryPrePoint[emKMETHOD_COUNT];	// 预分配的亲密度
		BYTE			btModify;
	};
#pragma pack(pop)
public:
	static KFRIEND_INFO GetDefaultValue();
	static KFRIEND_INFO Load(LPVOID pData, INT nLen);
	KBidFriendRelation(INT nType, IKRelationFactory *pFactory);
	virtual ~KBidFriendRelation();
	VOID Init(INT nMaster, INT nTarget, KFRIEND_INFO *pInfo);
public:
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
	virtual INT GetRole(BOOL bAsMaster);
	// 获取关系中的另一个玩家id
	virtual INT GetOtherPlayerId(INT nSelfId);

	virtual DWORD GetLastTalkTime();
	/***********************************************************
	 Function		: GetFavor
	 Description	: 获取好感度。
	 Return			: INT 好感度
	 ***********************************************************/
	virtual INT GetFavor();
	/***********************************************************
	 Function		: AddFavor
	 Description	: 增加好感度。
	 Return			: BOOL
	 Param			: nPoint	增加的好感度
	 Param			: nMethod	增加好感度的途径，见KE_FAVOR_METHOD
	 Param			: bByHand	表示是否通过手工添加，这样不检查每日上限
	 ***********************************************************/
	virtual BOOL AddFavor(INT nPoint, INT nMethod,  INT& nAward, BOOL bByHand = FALSE);
	/***********************************************************
	 Function		: PreAddFavor
	 Description	: 预增加好感度（确保下次增加好感度成功）
	 Return			: BOOL
	 Param			: nPoint	增加的好感度
	 Param			: nMethod	增加好感度的途径，见KE_FAVOR_METHOD
	 ***********************************************************/
	virtual BOOL PreAddFavor(INT nPoint, INT nMethod);
	/***********************************************************
	 Function		: ActAddFavor
	 Description	: 增加好感度（和预增加好感度对应）
	 Return			: BOOL
	 Param			: nPoint	增加的好感度
	 Param			: nMethod	增加好感度的途径，见KE_FAVOR_METHOD
	 ***********************************************************/
	virtual BOOL ActAddFavor(INT nPoint, INT nMethod);
	/***********************************************************
	 Function		: CancelPreAddFavor
	 Description	: 取消预增加亲密度
	 Return			: BOOL
	 Param			: nPoint	要取消预分配的亲密度
	 Param			: nMethod	途径，见KE_FAVOR_METHOD
	 ***********************************************************/
	virtual BOOL CancelPreAddFavor(INT nPoint, INT nMethod);
	// 清除当日已加亲密度

	static BOOL ClearFavorLimit(INT nPlayerId);

	BOOL SetLastTalkTime(DWORD dwTime);


	// 获取附加数据
	virtual LPVOID GetInfo(INT &nLen)
	{
		nLen = sizeof(KFRIEND_DBINFO);
		return &m_pInfo->sDBInfo;
	}
private:	
	BOOL CheckFavor(INT nPoint, INT nMethod, KFRIEND_INFO *pInfo);
	INT  GetFavorLevel(INT nPoint);

private:
	BOOL _ClearFavorLimit();
	INT					m_nType;
	IKRelationFactory*	m_pFactory;
	INT					m_nMaster;
	INT					m_nTarget;
	KFRIEND_INFO*		m_pInfo;
};

#endif //__KBIDFRIENDRELATION_H__
