
#ifndef __KMASOCKETSET_H__
#define __KMASOCKETSET_H__

#include "klibofbpt.h"

//---------------------------------------------------------------------------
// KMASocketSet类系		魔法孔组，记录一组相关的魔法属性，用于属性激活系统。

class KMASocketSet							// 抽象基类
{

public:

	struct KSOCKET							// 魔法孔结构
	{
		KMagicAttrib	sMagicAttrib;		// 魔法属性
		BOOL			bActive;			// 是否被激活
		BOOL			bVisible;			// 是否可见
	};

	explicit KMASocketSet(INT nCount);
	virtual ~KMASocketSet();

	virtual KMASocketSet &operator = (CONST KMASocketSet &cMASS);
	virtual	BOOL Init(VOID);				// 初始化魔法属性
	virtual BOOL Reset(VOID);				// 清空所有魔法属性

	INT		GetCount(VOID) CONST { return m_nCount; }
	CONST	KMagicAttrib* GetMagicAttrib(INT nIndex) CONST;
	BOOL	IsActive(INT nIndex) CONST;
	BOOL	IsVisible(INT nIndex) CONST;
	VOID	UpdateState(INT nPlayerIdx = 0, BOOL bActive = FALSE);	// idxPlayer是装备了该装备的角色索引，为0表示该装备没有装备到角色身上，bActive 是否强制激活

protected:

	KSOCKET* CONST	m_pSocket;				// 魔法孔数组
	CONST INT		m_nCount;				// 魔法孔总数
	BOOL			m_bInit;				// 是否已经初始化

	virtual	VOID Update(INT nPlayerIdx, BOOL bActive = FALSE) = 0;

	BOOL	PreAssign(CONST KMASocketSet& cMASS);
	BOOL	ClearSocket(INT nIndex);

};

class KMASS_Item : public KMASocketSet		// 应用于Item的魔法孔组基类
{
	friend class KItem;

public:
	KMASS_Item(KItem& cItem, INT nCount);
	virtual ~KMASS_Item() {}

	virtual KMASS_Item &operator = (CONST KMASS_Item& cMASS);
	virtual	BOOL Init(VOID);

protected:
	KItem* CONST	m_pItem;				// 所属道具
	virtual	VOID Update(INT nPlayerIdx, BOOL bActive = FALSE);
};

class KMASS_Random : public KMASS_Item		// 用于装备随机魔法属性
{
public:
	explicit KMASS_Random(KItem &cItem);
	virtual ~KMASS_Random() {}

	virtual	BOOL Init(VOID);

protected:
	virtual	VOID Update(INT nPlayerIdx = 0, BOOL bActive = FALSE);
};

class KMASS_Enhance : public KMASS_Item		// 用于装备强化激活属性
{
public:
	explicit KMASS_Enhance(KItem &cItem);
	virtual ~KMASS_Enhance() {}

	INT GetTimes(INT nIndex) CONST;
	virtual	BOOL Init(VOID);
	virtual BOOL Reset(VOID);

protected:
	CONST KBPT_EQUIP_ENHMA*	m_pBPT;
	virtual	VOID Update(INT nPlayerIdx = 0, BOOL bActive = FALSE);
};

class KMASS_Suite : public KMASocketSet		// 用于装备套装激活属性
{
public:
	typedef std::vector<KITEM_INDEX> KItemVec;

public:
	KMASS_Suite(CONST KSUITE_INFO& sSuiteInfo, KE_SUITE_TYPE eSuiteType);
	virtual ~KMASS_Suite() {}

	INT		GetSuiteID(VOID) CONST { return m_pSuiteInfo->pInfo->nSuiteID; }
	CONST	KItemVec& GetSuiteParts(INT nEquipType) CONST;
#ifndef GAME_SERVER
	PCSTR	GetSuiteName(VOID) CONST { return m_pSuiteInfo->pInfo->szName; }
#endif

protected:
	CONST KSUITE_INFO*	m_pSuiteInfo;
	KE_SUITE_TYPE		m_eType;

	virtual	VOID Update(INT nPlayerIdx = 0, BOOL bActive = FALSE);
};

class KMASS_Strengthen : public KMASS_Item		// 用于装备改造属性
{
public:
	explicit KMASS_Strengthen(KItem &cItem);
	virtual ~KMASS_Strengthen() {}

	INT GetTimes(INT nIndex) CONST;
	virtual	BOOL Init(VOID);
	virtual BOOL Reset(VOID);

protected:
	CONST KBPT_EQUIP_ENHMA*	m_pBPT;
	virtual	VOID Update(INT nPlayerIdx = 0, BOOL bActive = FALSE);
};


// -------------------------------------------------------------------------

extern VOID g_PushMagicAttribAsTable(QLuaScript &sc, CONST KMagicAttrib *pMA);
extern VOID g_PushMASSAsTable(QLuaScript &sc, CONST KMASocketSet &cMASS);

// -------------------------------------------------------------------------

#endif /* __KMASOCKETSET_H__ */
