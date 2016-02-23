/* -------------------------------------------------------------------------
//	文件名		：	kluaride.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-3-15
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KLUARIDE_H__
#define __KLUARIDE_H__

// -------------------------------------------------------------------------

class KRide;

// -------------------------------------------------------------------------

class KLuaRide : public QLunaBase
{

public:
	explicit KLuaRide(KRide* pRide) : m_pRide(pRide)
	{

	}

	~KLuaRide()
	{

	}

	KRide* GetRide(VOID) { return m_pRide; }


	// 脚本接口定义
	DECLARE_LUA_CLASS(KLuaRide);
	INT		getGenre(VOID) CONST;
	INT		getDetail(VOID) CONST;
	INT		getParticular(VOID) CONST;
	INT		getLevel(VOID) CONST;
	LPCSTR	getName(VOID) CONST;
	INT		getHungry(VOID) CONST;
	INT		getState(VOID) CONST;
	DWORD	getId(VOID) CONST;
	DWORD	getOwnerId(VOID) CONST;
	INT		getMagicRate(VOID) CONST;

	INT		LuaSetMagicRate(QLuaScript& sc);
	INT		LuaSync(QLuaScript& sc);
	INT		LuaRegenerate(QLuaScript& sc);
	INT		LuaAddHungry(QLuaScript& sc);
	INT		LuaSetState(QLuaScript& sc);

protected:
	KRide* m_pRide;
};

// -------------------------------------------------------------------------

#endif /* __KLUARIDE_H__ */
