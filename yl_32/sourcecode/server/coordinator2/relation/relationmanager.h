/* -------------------------------------------------------------------------
//	文件名		：	relationmanagers_gc.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KRELATIONMANAGER_GC_H__
#define __KRELATIONMANAGER_GC_H__

#include "playerrelation_i.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "relationcounter.h"
class KRole;
class KCloseFriendRelation;
struct KRELATION_PROTO_FRIENDINFO;

class KRelationManager_GC
{
	enum
	{
		emKRELATION_MAX_COUNT				= 1000,			// 最大关系数
		emKRELATION_MINFAVOR_NOTIFYONLINE	= 1,			// 通知上下线的最小亲密度
		emKRELATION_SEARCHRES_MAXCOUNT		= 50,			// 每次搜索可拜师玩家的最大结果数目
		emKRELATION_TRAININGHINT_CYCLE		= 7 * 24 * 3600,// 拜师/收徒提示周期（秒）
		emKRELATION_NEXTSEARCHSCRAP			= 40,			// 搜索师徒时每次间隔时间
		emKRELATION_SHOWPLAYERNUMBER		= 50,			// 需要同步显示的最大玩家数量
	};
	enum KEADDRELATION_RESULT
	{
		emKEADDRELATION_FAIL	= 0,	// 添加关系失败
		emKEADDRELATION_SUCCESS = 1,	// 添加关系成功
		emKEHAVE_RELATION		= 2,	// 已经有关系了
		emKEPLAYER_NOTONLINE	= 3,	// 对方不在线的时时不能添加为好友
	};
	typedef BOOL (KRelationManager_GC::*fnTrainingCheck)(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID * ppParam);
	typedef BOOL (KRelationManager_GC::*fnTrainingEvent)(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID pParam);
	struct KTRAINING_EVENT
	{
		fnTrainingCheck pfnCheck;
		fnTrainingEvent pfnEvent;
	};
	struct KTPRELATION
	{
		BYTE	btType;
		INT		nPlayerId;
		INT		nBindCoin;
		CHAR	szTarget[_NAME_LEN];
		BOOL	bMaster;
	};
	struct KTPPLAYERSEX
	{
		BYTE bySex;
		INT nPlayerId;
	};
	// sort by 
	struct _KPlayerSexSort
	{
		bool operator()(CONST KTPPLAYERSEX& nStart, CONST KTPPLAYERSEX& nEnd)
		{
			return nStart.bySex > nEnd.bySex;
		}
	};
	struct KFriendAward
	{
		INT nMaxNum; // 最大人数
		INT nAward;  // 奖励数量
	};
public:
	struct KTIMEINFO
	{
		INT nPlayerId;		// 玩家Id
		INT nType;			// 关系种类
		INT	nRemanentTime;	// 距离一年期限的剩余时间
		BOOL bAsMaster;		// 是主位还是次位
	};
	typedef	std::multimap<INT, KTPRELATION> MAPRELATION;
	typedef std::set<INT> SET_PLAYERID;
	typedef std::vector<INT > VECPLAYERID;

	// static KRelationManager_GC *Instance() {return &ms_inst;}
	KRelationManager_GC();
	~KRelationManager_GC();

	// dispose
// 	virtual BOOL SendData(LPVOID pData, INT nSize, INT nConnectID = -1);
	// 判断人际关系表是否加载完成
	BOOL CheckIsLoadEnd(KRole* pPlayerApp, INT nAppId, BOOL bSendMsg = TRUE);
	BOOL CheckIsLoadEnd(INT nAppId);
	// 检查创建关系的冲突和依赖
	BOOL CheckCreateDepRep(INT nType, INT nRole, INT nAppId, INT nDstId);
	// 检查删除关系的冲突和依赖
	BOOL CheckDelDepRep(INT nType, INT nRole, INT nAppId, INT nDstId);
	// 检查加亲密度的日期，跨天则清除每日上限
	BOOL ResetLimtWhenCrossDay(INT nAppId, INT nDstId);
	// 同步关系列表
	BOOL SyncRelationList(INT nPlayerId );
	// 把所有好友列表信息同步给自己
	BOOL SyncFriendListToMyself(INT nPlayerId );
	// 把自己信息同步给所有在线好友
	BOOL SyncMyselfToFriendList(INT nPlayerID, KRole* pPlayer);
	// 通知好友自己上/下线
	BOOL NotifyOnline(INT nPlayer, BOOL bOnline);
	// 保存某个玩家的关系列表并从内存中卸载
	BOOL Unload(INT nPlayer);
	// 完全删除某个玩家的关系
	BOOL Remove(INT nPlayer, LPCSTR pszPlayerName);	
	// 通知关系列表加载完毕
	BOOL NotifyRelationListLoaded(INT nPlayerId);
	// 使用IB道具每日获得的最大亲密度
	inline INT GetMaxFavorEveryDayWithIB() CONST
	{
		return m_nMaxFavorEveryDayWithIB;
	}
	// 非IB道具途径每日获得的最大亲密度
	inline INT GetMaxFavorEveryDayWithoutIB() CONST
	{
		return m_nMaxFavorEveryDayWithoutIB;
	}
	// 最小亲密度
	inline INT GetMinFavor() CONST
	{
		return m_nMinFavor;
	}
	// 最大亲密度
	inline INT GetMaxFavor() CONST
	{
		return m_nMaxFavor;
	}
	// 每个玩家的人际关系的最大数目
	inline INT GetMaxRelationCount() CONST
	{
		return m_nMaxRelationCount;
	}
	// 和师徒相关的检查
	BOOL TrainingCheck(INT nPlayerId);
	// 设置好友信息
	static BOOL InitRelationInfo(KRole* pMaster, KRole* pTarget, INT nFavor, DWORD dwLastTalkTime, KRELATION_PROTO_FRIENDINFO* pInfo);
	// 
	BOOL GetFriendListToMyselfInfo(IKEnumRelation *pEnum, INT nPlayerSelf,	INT nMaxCount, 
		KRELATION_PROTO_FRIENDINFO aryInfo[], INT& nCount);
	BOOL SyncMyselfToFriendList(KRole* pPlayer, INT nPlayerID, BOOL bMaster);
	BOOL DelRelationByHand(INT nAppID, INT nType, BOOL bRole, LPCSTR pszRelation);
	BOOL AddRelationByHand(INT nAppId, INT nType, BOOL bRole, LPCSTR pszRelation);
	BOOL CanCreateRelation(INT nType, INT nRole, INT nMasterId, INT nTargetId);
	BOOL CreateRelation(INT nType, INT nMasterId, INT nTargetId, BOOL bIsMaster);
	BOOL CanDelRelation(INT nType, INT nRole, INT nMasterId, LPCSTR pszTargetName);
	BOOL DelRelation(INT nType, INT nMasterId, INT nTargetId, BOOL bIsMaster);
	BOOL DelOverTimeRelation(INT nType, INT nMasterId, INT nTargetId, BOOL bIsMaster);
	BOOL AddFriendFavor(INT nAppId, INT nDstId, INT nFavor, INT nMethod, BOOL bByHand = FALSE);
	BOOL SetFriendTalkTime(INT nAppId, INT nDstId, DWORD dwTime);
	// 增加亲密度之后同步好友信息
	BOOL SyncFriendInfo(INT nAppId, INT nDstId);
	// 好友聊天频道
	BOOL GetMyChannelFriend(INT nPlayerId,  VECPLAYERID& vecPlayerId);
	// 处理好友亲密度奖励
	BOOL DoFrindFavorAward(KRole* pPlayerApp, KRole* pPlayerDst, INT nAppId, INT nDstId, INT nAwardLevel);
	// 好友亲密度等级提升回调
	VOID OnFavorLevelUp(INT nPlayerAppId, INT nPlayerDstId, INT nFavorLevel);
	// 获取密友离一年期限的情况
	BOOL GetCloseFriendTimeInfo(INT nAppPlayerId, std::vector<KTIMEINFO>& rvctTimeInfo);
	// 获取某玩家指定的人际关系的数量
	INT GetOneRelationCount(INT nPlayerId, INT nType, BOOL bAsMaster = TRUE);
	// 判断玩家之间是否存在指定的人际关系
	BOOL HasRelation(INT nAppId, INT nDstId, INT nType, BOOL bAsMaster = TRUE);
	// 获取指定玩家的亲密度
	INT GetFriendFavor(INT nPlayerId1, INT nPlayerId2);

public:
	BOOL Init();
	BOOL UnInit();
	BOOL Activate();
	BOOL SaveAll();

	BOOL OnLoadPlayerData(INT nPlayerId, BYTE* pbyData, size_t uDataLen);
	BOOL SavePlayerData(INT nPlayerId);
	void OnPlayerLeave(INT nPlayerId);

	void AddDirty(DWORD dwPlayerId);

	// 协议注册 todo 移出这个manager
public:
	static void Process_AddFavor(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_SearchTeacher(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_SearchStudent(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_SetTrainingOption(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_GetTrainingOption(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_RequestTeacher(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_RequestStudent(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_TrainingGraduate(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_TrainingViewInfo(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_TrainingPlayerInfo(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_TrainingMakeFriend(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_TrainingApply(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_AddCloseFriendBindCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_ApplyGainIbBackCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_GainIbBackCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void Process_CancelGainCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex);

private:
	BOOL IbBackMailSend(INT nPlayerId);
	BOOL IbBackOverMailSend(INT nSendId, INT nReceiveId, BOOL bBothSend);
	BOOL AddRelationMap(INT nPlayerId, BYTE btType, KCloseFriendRelation* pRelation, INT nCoin);
	BOOL FeedBackApplyIbCoin(INT nPlayerId, BYTE byFirst = 0);
	KCloseFriendRelation* GetCloseFriendRelation(INT nPlayerId, BYTE btType, INT nPlayerId2);
	// 收集在线的好友id
	BOOL AddFriendIdToVec(INT nSendPlayerId, IKEnumRelation* pEnum, VECPLAYERID& vecPlayerId);
	BOOL SyncRelationList(
		INT					nPlayerID,
		INT					nRelationType,
		IKRelationFactory*	pFactory,
		BOOL				bAsMaster,
		BOOL				bNotifyClient);
	// 告诉客户端要开始同步了
	BOOL SyncStartRelationList(INT nPlayerID);
	// 通知关系玩家上线/下线
	BOOL NotifyOnline(
		INT					nRelationType,
		BOOL				bAsMaster,
		INT					nPlayerId,
		IKRelationFactory*	pFactory,
		BOOL				bOnline,
		SET_PLAYERID& rsetAlreadyNotify);
//	BOOL FindPlayerGuidByName(LPCSTR pszName, GUID nPlayerId);
	// 通知好友自己删号
	BOOL NotifyDelete(INT nSelfId, LPCSTR pszPlayerName, INT nType, BOOL bAsMaster, IKEnumRelation *pEnum);
	// 搜索符合条件的可拜为师父的玩家
	BOOL SearchTeacherList(IKTrainingFactory* pFactroy, KRole* pStudent, INT nPlayerID);
	// 搜索符合条件的可收为弟子的玩家
	BOOL SearchStudentList(IKTrainingFactory* pFactroy, KRole* pTeacher, INT nPlayerID);
	// 检查弟子和师父资格
	BOOL CheckTeacherAndStudent(
		KRole* pTeacher,
		KRole* pStudent,
		INT	nTeacherID,
		INT nStudentID,
		BOOL bIsTeacherActive);
	BOOL TrainingCheck_Student(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID * ppParam);
	BOOL TrainingCheck_Teacher(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID * ppParam);
	BOOL TrainingCheck_TimeOut(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID * ppParam);
	BOOL TrainingEvent_Student(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID pParam);
	BOOL TrainingEvent_Teacher(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID pParam);
	BOOL TrainingEvent_TimeOut(KRole* pPlayer, INT nPlayerID, IKRelationFactory* pFactory, LPVOID pParam);
	// 搜索有选择想收徒或拜师的在线人
	BOOL PreSearchPlayer(IKTrainingFactory*	pFactory, BOOL bStudent);
private:
	BOOL SetBeforeFrindFavor(KRole* pPlayer, INT nLeft, INT nAwardLevel);
	BOOL NeedLogRecord(IKRelationFactory *pFactory, BYTE byRelationType, INT nMasterId, INT nTargetId);
	BOOL RecodeDeleteLog(KRole* pAppPlayer, KRole* pTargetPlayer, BYTE byType);

	// static KRelationManager_GC	ms_inst;
	INT							m_nMaxFavorEveryDayWithIB;		// 每天可以增加的亲密度最大数值（IB道具途径）
	INT							m_nMaxFavorEveryDayWithoutIB;	// 每天可以增加的亲密度最大数值（非IB道具途径）
	INT							m_nMinFavor;					// 最小亲密度
	INT							m_nMaxFavor;					// 最大亲密度
	INT							m_nMaxRelationCount;			// 玩家关系上限
	static KTRAINING_EVENT		ms_aTrainingEvent[];			// 师徒检查
	MAPRELATION					m_mpCloseFriend;	
	time_t						m_tLastSearchStudent;			// 上一次搜索徒弟的时间
	time_t						m_tLastSearchTeacher;			// 上一次搜索师傅的时间
	INT							m_nLastTeacherId;				// 在缓存中的最后一个师傅的id
	INT							m_nLastStudentId;				// 在缓存中的最后一个徒弟的id
	std::vector<KTPPLAYERSEX>	m_vecStudentId;					// 徒弟ID
	std::vector<KTPPLAYERSEX>	m_vecTeacherId;					// 师傅ID
	std::vector<KFriendAward>	m_vecFriendAwardLevel;			// 对应等级上，最大领奖人数

	typedef std::set<DWORD> SET_DIRTY_PLAYER;
	SET_DIRTY_PLAYER m_setDirtyPlayerId;
};

#endif //__KRELATIONMANAGER_GC_H__
