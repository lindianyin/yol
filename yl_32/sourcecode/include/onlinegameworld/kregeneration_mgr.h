///////////////////////////////////////////////////////////////////////

///	File Name	: kregeneration_mgr.h
///	Author		: Dai Zejun
///	Created Date: 2010-10-29
///
/// Version history:
///		2010-10-29dzj: Created
///	
///	Purpose		: NPC/doodad重生管理
///				  目前实现暂不支持scence_id复用的情况。
///				  如果要支持scence_id复用，须实现OnScenceRemove()接口，并在场景删除的时候回调，删除该场景的重生物。
///
///////////////////////////////////////////////////////////////////////
#ifndef __KREGENERATION_MGR__H__
#define __KREGENERATION_MGR__H__

#ifdef GAME_SERVER

#include <map>
#include "Misc/objectpool.h"
#include "Runtime/qstring.h"

enum KE_OBJTYPE
{
	emKOBJ_PLAYER,
	emKOBJ_NPC,
	emKOBJ_DOODAD,
};

class KS_REGENERATION_INFO
{
public:
	KS_REGENERATION_INFO() : 
	dwIdExpect(0),
	dwFrames(0),
	dwTemplateId(0),
	dwSceneId(0),
	nX(0),
	nY(0),
	nZ(0),
	nFaceDirection(0),
	nNormalAIType(0),
	nRelation(0),
	byType(0),
	dwExValue(0),
	bRegnerate(1),
	pDetailInfo(NULL)
	{
		
	}
	~KS_REGENERATION_INFO()
	{
		if (pDetailInfo)
		{
			free(pDetailInfo);
		}
	}
	DWORD dwIdExpect;	//期望ID(可借此实现ID复用)
	DWORD dwFrames;		//重生时间间隔(单位：帧)
	DWORD dwTemplateId; //模板ID
	DWORD dwSceneId;	//重生场景ID
	INT nX;				//重生点X
	INT nY;				//重生点Y
	INT nZ;				//重生点Z
	INT nFaceDirection;	//初始朝向
	INT nNormalAIType;	//Normal AI type
	INT nRelation;		//Relation
	BYTE byType;		//重生物类别
	QString szNickName;
	DWORD dwExValue;
	BOOL bRegnerate;		// 是否还需要重生
	CHAR szExInfo[MAX_NAME_LEN];
	void* pDetailInfo;	//指向malloc申请的内存，供扩展用的附加信息指针，目前可以全部填空
};

typedef std::multimap<UINT32, KS_REGENERATION_INFO*> KRegenerationMMap;

typedef KDynReUseObjectPool<KS_REGENERATION_INFO> KRegenerationMemPool;

class KRegenerationMgr
{
private:
	KRegenerationMgr();
	~KRegenerationMgr();

public:
	//定时器(每帧调用)
	BOOL	Activate();

	//注册重生事件
	BOOL	Register(const KS_REGENERATION_INFO& sInfo);

	BOOL UnRegister(DWORD dwTemplateId, DWORD dwSceneId);

	//清空重生队列
	VOID	Clear();

	//scence卸载时的回调(解决scence_id复用的接口，先保留接口，这个功能目前先不做)
	BOOL	OnScenceRemove(DWORD dwScenceId){ QCONFIRM_RET_FALSE(FALSE); }

private:
	//执行重生
	BOOL	_ActionRegeneration(const KS_REGENERATION_INFO& sInfo);

	//NPC重生
	BOOL	_ActionRegenerationNPC(const KS_REGENERATION_INFO& sInfo);

	//doodad重生
	BOOL	_ActionRegenerationDoodad(const KS_REGENERATION_INFO& sInfo);

private:
	//数据管理器
	KRegenerationMMap		m_cDataMMap;//KEY:触发重生的帧

	//帧计数器
	DWORD					m_dwFrameCounter;

	//第一个需要重生的对象Frame(效率优化参数)
	DWORD					m_dwFirstRegenerationFrame;

	//对象池
	KRegenerationMemPool*	m_pObjPool;

public:
	//获取实例
	static KRegenerationMgr& GetInstance(){ return ms_instance; }
private:
	//实例
	static KRegenerationMgr ms_instance;
};

#endif

#endif //__KREGENERATION_MGR__H__

