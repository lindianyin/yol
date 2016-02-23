/* -------------------------------------------------------------------------
//	文件名		：	playerrelation_i.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	玩家关系接口（这里的玩家关系是一对一的关系，比如好友、师徒、夫妻、仇人、结拜等）
//
// -----------------------------------------------------------------------*/
#ifndef __PLAYERRELATION_I_H__
#define __PLAYERRELATION_I_H__

#define  KD_RELAYTIONDATA_MAX_SIZE	255	//must less equal KD_TINY_FIELD_MAX_DATA_SIZE
/*
struct IKRelationDatabase
{
	virtual VOID AddIntoDatabase(INT nType, INT nPlayerId1, INT nPlayerId2, LPVOID pData = NULL, INT nLen = 0) = 0;
	virtual VOID DelFromDatabase(INT nType, INT nPlayerId1, INT nPlayerId2) = 0;
	virtual VOID Remove(INT nPlayerId) = 0;
	virtual VOID Update(INT nType, INT nPlayerId1, INT nPlayerId2, LPVOID pData = NULL, INT nLen = 0) = 0;
};
*/
struct IKRelationHelper
{
	virtual BOOL CheckCanLoad(INT nPlayerId) = 0;
	virtual BOOL CheckCanUnload(INT nPlayerId) = 0;
	virtual BOOL Dirty(INT nPlayerId1, INT nPlayerId2) = 0;
};

struct IKRelationSaver 
{
	virtual BOOL SaveBegin(INT nType, INT nDataLen, BOOL bMaster = TRUE) = 0;
	virtual BOOL SaveEnd(INT nType) = 0;
	virtual BOOL AddRecord(INT nPlayerId, LPVOID pData = NULL) = 0;
};

struct IKRelationSender
{
	virtual BOOL SendData(LPVOID pData, INT nSize, INT nConnectID = -1) = 0;
};

/**
 * Interface	: IKRelation
 * Description	: 玩家关系。玩家关系可以是单向的（临时好友），
				  可以是对等双向的（一般好友），
				  也可以是不对等双向的（师徒）。
 */
struct IKRelation
{
	virtual LPVOID GetInfo(INT &nLen) = 0;						// 获取附加数据
	virtual BOOL SaveInfo(CHAR *pBuf, INT nLen) = 0;			// 保存附加数据到指定缓冲区（用于同步好友信息）
	virtual BOOL LoadInfo(CHAR *pBuf, INT nLen) = 0;			// 加载附加数据
	virtual BOOL QueryRelation(INT nType, LPVOID *ppv) = 0;		// 查询子接口
	virtual INT GetRole(BOOL bAsMaster) = 0;					// 获取主位/次位玩家（bAsMaster为TRUE表示主位，FALSE表示次位）
	virtual INT GetType() = 0;									// 查询关系类型
	virtual INT GetInfLen() = 0;								// 获取附加信息长度
	virtual INT GetOtherPlayerId(INT nSelfId) = 0;							// 获取另一个人id
};


/**
 * Interface	: IKEnumRelation
 * Description	: 玩家关系枚举器
 */
struct IKEnumRelation
{
	virtual VOID Reset() = 0;
	virtual BOOL Next(IKRelation **ppRelation) = 0;
	virtual INT  GetCount() = 0;
};

// class KGC_ProtocolProcessor;
/**
 * Interface	: IKRelationFactory
 * Description	: 玩家关系工厂，用于创建和管理玩家关系。每一类的玩家关系都有自己的工厂。
 */
struct IKRelationFactory
{
	/***********************************************************
	 Function		: QueryFactory
	 Description	: 查询子接口
	 Param			: nType			子接口类型，见KEPLAYERRELATION_FACTORY_SUBTYPE
	 Param			: ppFactory		返回接口指针
	 Return			: BOOL
	 ***********************************************************/
	virtual BOOL QueryFactory(INT nType, LPVOID * ppFactory) {return FALSE;}
	/***********************************************************
	 Function		: GetRelationInfoLen
	 Description	: 获取当前类型玩家关系的附带信息长度（对于某一种关系类型，附带信息长度是固定的）。
					  该附带数据可以用来表示亲密度之类的信息，由每种玩家关系自行解释
	 Return			: INT
	 ***********************************************************/
	virtual INT GetRelationInfoLen() = 0;
	/***********************************************************
	 Function		: CreateRelation
	 Description	: 创建玩家关系
	 Return			: BOOL
	 Param			: pMaster			主位玩家
	 Param			: pTarget			次位玩家
	 Param			: bIsMasterActive	主位玩家为主动申请方
	 Param			: ppRelation		创建的玩家关系
	 ***********************************************************/
	virtual BOOL CreateRelation(
			INT nMaster,
			INT nTarget,
			BOOL				bIsMasterActive = TRUE,
			IKRelation**		ppRelation = NULL) = 0;
	/***********************************************************
	 Function		: CanCreateRelation
	 Description	: 检查是否可以创建玩家关系
	 Return			: BOOL
	 Param			: pMaster		主位玩家
	 Param			: pTarget		次位玩家
	 Param			: pData			附带数据
	 Param			: nLen			附带数据长度
	 ***********************************************************/
	virtual BOOL CanCreateRelation(
			INT nMaster,
			INT nTarget) = 0;
	/***********************************************************
	 Function		: GetAllRelations
	 Description	: 获取指定玩家指定角色的所有玩家关系。
	 Return			: BOOL
	 Param			: bAsMaster		是否作为主位玩家
	 Param			: pGuigd		玩家
	 Param			: ppEnum		返回的玩家关系枚举器
	 ***********************************************************/
	virtual BOOL GetAllRelations(
			BOOL				bAsMaster,
			INT nPlayerId,
			IKEnumRelation**	ppEnum) = 0;
	/***********************************************************
	 Function		: FindRelation
	 Description	: 查找指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: pMaster		主位玩家
	 Param			: pTarget		次位玩家
	 Param			: ppRelation	返回的玩家关系
	 ***********************************************************/
	virtual BOOL FindRelation(
			INT nMaster,
			INT nTarget,
			IKRelation**		ppRelation = NULL) = 0;
	/***********************************************************
	 Function		: DeleteRelation
	 Description	: 删除指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: pMaster		主位玩家
	 Param			: pTarget		次位玩家
	 ***********************************************************/
	virtual BOOL DeleteRelation(
			INT nMaster,
			INT nTarget) = 0;
	/***********************************************************
	 Function		: Unload
	 Description	: 把指定玩家从内存中删除
	 Return			: BOOL
	 Param			: pGuid		玩家角色ID
	 ***********************************************************/
	virtual BOOL Unload(INT nPlayerId) = 0;
	/***********************************************************
	 Function		: SaveAll
	 Description	: 存盘
	 Return			: BOOL
	 Param			: nPlayerId		玩家角色ID
	 ***********************************************************/
	virtual BOOL SaveAll(INT nPlayerId, IKRelationSaver* pSaver) = 0;
	/***********************************************************
	 Function		: Load
	 Description	: 加载玩家关系列表
	 Return			: BOOL
	 Param			: pMaster		主位玩家角色ID
	 Param			: pMaster		次位玩家角色ID
	 Param			: pData			关系数据
	 Param			: nLen			数据长度
	 ***********************************************************/
	virtual BOOL Load(INT nMaster, INT nTarget, LPVOID pData, INT nLen) = 0;
	/***********************************************************
	 Function		: Remove
	 Description	: 删除指定玩家的所有关系
	 Return			: BOOL
	 Param			: pGuid		玩家角色ID
	 ***********************************************************/
	virtual BOOL Remove(INT nPlayerId) = 0;
};

/**
 * Interface	: IKTrainingFactory
 * Description	: 师徒工厂（未出师）
 */
class KGC_Player;
struct IKTrainingFactory : public IKRelationFactory
{
	// 检查师父的资格
	virtual BOOL CheckTeacherQualification(INT nTeacher, LPINT pnTeacherErr = NULL, LPINT pnStudentErr = NULL) = 0;
	// 检查弟子的资格
	virtual BOOL CheckStudentQualification(INT nStudent, LPINT pnTeacherErr = NULL, LPINT pnStudentErr = NULL) = 0;
	// 检查师父和弟子之间是否符合一定关系
	virtual BOOL CheckRelation(KGC_Player* pTeacher, KGC_Player* pStudent, LPINT pnTeacherErr = NULL, LPINT pnStudentErr = NULL) = 0;
};

/**
*	Interface	: IKCloseFriendRelation
*	Description : 密友
*/
struct IKCloseFriendRelation : public IKRelation
{
	/***********************************************************************
	Function		: GetBindCoin
	Description		: 获取可以领取的绑定金币数
	Return			: INT 绑定金币数量
	Param			: bMaster 是否为主位
	**********************************************************************/
	virtual	INT GetBindCoin(BOOL bMaster)	= 0;

	/***********************************************************************
	Function		: AddBindCoin
	Description		: 添加可领取的绑定金币数量
	Param			: INT 可领取绑定金币数量
	Param			: bMaster 是否为主位
	**********************************************************************/	
	virtual	BOOL AddBindCoin(INT nCoin, BOOL bMaster) = 0;

	/***********************************************************************
	Function		: CheckOverTime
	Description		: 是否已经超过一年了
	Return			: FALSE：还没超时， TRUE：已经超时
	**********************************************************************/
	virtual	BOOL CheckOverTime()	= 0;
	/***********************************************************************
	Function		: GainBindCoin
	Description		: 领取绑定金币，
	Return			: FALSE：领取失败， TRUE：领取成功
	Param			: bMaster 是否为主位
	**********************************************************************/
	virtual	BOOL GainBindCoin(BOOL bMaster)		= 0;
	
	virtual BOOL IsMaster(INT nSelfId) = 0;						// 判断自己是否为主位，
	//virtual	INT  GetOtherPlayerId(INT nSelfId) = 0;
};

/**
 * Interface	: IKBidFriendRelation
 * Description	: 普通好友
 */
struct IKBidFriendRelation : public IKRelation
{
	// 增加亲密度的途径
	enum KE_FAVOR_METHOD
	{
		emKFAVOR_METHOD_NORMAL,		// 正常途径
		emKFAVOR_METHOD_IBITEM,		// 通过IB道具
		emKFAVOR_METHOD_COUNT
	};
	virtual DWORD GetLastTalkTime() = 0;

	virtual BOOL SetLastTalkTime(DWORD lastTalkTime) = 0;
	/***********************************************************
	 Function		: GetFavor
	 Description	: 获取亲密度。
	 Return			: INT 亲密度
	 ***********************************************************/
	virtual INT GetFavor() = 0;
	/***********************************************************
	 Function		: AddFavor
	 Description	: 增加亲密度。
	 Return			: BOOL
	 Param			: nPoint	增加的亲密度
	 Param			: nMethod	增加亲密度的途径，见KE_FAVOR_METHOD
	Param			: bByHand	是否手工添加，是的话不检查每日上限
	 ***********************************************************/
	virtual BOOL AddFavor(INT nPoint, INT nMethod, INT& nAward, BOOL bByHand = FALSE) = 0;
	/***********************************************************
	 Function		: PreAddFavor
	 Description	: 预增加亲密度（确保下次增加亲密度成功）
	 Return			: BOOL
	 Param			: nPoint	增加的亲密度
	 Param			: nMethod	增加亲密度的途径，见KE_FAVOR_METHOD
	 ***********************************************************/
	virtual BOOL PreAddFavor(INT nPoint, INT nMethod) = 0;
	/***********************************************************
	 Function		: ActAddFavor
	 Description	: 增加亲密度（和预增加亲密度对应）
	 Return			: BOOL
	 Param			: nPoint	增加的亲密度
	 Param			: nMethod	增加亲密度的途径，见KE_FAVOR_METHOD
	 ***********************************************************/
	virtual BOOL ActAddFavor(INT nPoint, INT nMethod) = 0;
	/***********************************************************
	 Function		: CancelPreAddFavor
	 Description	: 取消预增加亲密度
	 Return			: BOOL
	 Param			: nPoint	要取消预分配的亲密度
	 Param			: nMethod	途径，见KE_FAVOR_METHOD
	 ***********************************************************/
	virtual BOOL CancelPreAddFavor(INT nPoint, INT nMethod) = 0;
};

// 玩家关系子类型（对应IKRelationFactory的子接口）
enum KEPLAYERRELATION_FACTORY_SUBTYPE
{
	emKPLAERRELATION_FACTORY_SUBTYPE_TRAINING,	// 师徒工厂（未出师），对应IKTrainingFactory
	emKPLAERRELATION_FACTORY_SUBTYPE_COUNT
};

// 玩家关系类型
// 注意！！！包含普通好友的玩家关系（如结拜/师徒/夫妻）的枚举值必须比普通好友类型的枚举值大！！！
// 因用于存储，不要随意改变枚举的顺序
enum KEPLAYERRELATION_TYPE
{
	emKPLAYERRELATION_TYPE_TMPFRIEND	= 0,	// 临时好友，单向关系，A把B加为临时好友
	emKPLAYERRELATION_TYPE_BLACKLIST	= 1,	// 黑名单，单向关系，A把B加入黑名单
	emKPLAYERRELATION_TYPE_BINDFRIEND	= 2,	// 普通好友, 对等双向关系，A和B互为好友
	emKPLAYERRELATION_TYPE_SIBLING		= 3,	// 结拜（兄弟、姐妹），对等双向关系，A和B互为结拜（兄弟、姐妹）
	emKPLAYERRELATION_TYPE_ENEMEY		= 4,	// 仇人，不对等双向关系，A曾经被B杀死
	emKPLAYERRELATION_TYPE_TRAINING		= 5,	// 师徒，不对等双向关系，A是B的师父（未出师）
	emKPLAYERRELATION_TYPE_TRAINED		= 6,	// 师徒，不对等双向关系，A是B的师父（已出师）
	emKPLAYERRELATION_TYPE_COUPLE		= 7,	// 夫妻，不对等双向关系，A是B的丈夫
	emKPLAYERRELATION_TYPE_INTRODUCTION = 8,	// 介绍，不对等双向关系，A是B的介绍人
	emKPLAYERRELATION_TYPE_BUDDY		= 9,	// 指定密友，双向对等关系，A和B互为密友同时也互为普通好友
	emKPLAYERRELATION_TYPE_COUNT
};

#endif //__PLAYERRELATION_I_H__
