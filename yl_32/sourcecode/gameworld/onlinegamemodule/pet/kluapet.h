/* -------------------------------------------------------------------------
//	文件名		：	kluapet.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-2-28
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KLUAPET_H__
#define __KLUAPET_H__

// -------------------------------------------------------------------------

class KPet;

// -------------------------------------------------------------------------

class KLuaPet : public QLunaBase
{

public:
	explicit KLuaPet(KPet* pPet) : m_pPet(pPet)
	{

	}

	~KLuaPet()
	{

	}

	KPet* GetPet(VOID) { return m_pPet; }


	// 脚本接口定义
	DECLARE_LUA_CLASS(KLuaPet);
	LPCSTR	getName(VOID) CONST;
	INT		getType(VOID) CONST;
	INT		getQuality(VOID) CONST;
	INT		getCharacter(VOID) CONST;
	INT		getHungry(VOID) CONST;
	INT		getLevel(VOID) CONST;
	INT		getRankLevel(VOID) CONST;
	DWORD	getRandSeed(VOID) CONST;
	DWORD	getId(VOID) CONST;
	DWORD	getOwnerId(VOID) CONST;
	INT		getState(VOID) CONST;

	INT		LuaAddExp(QLuaScript& sc);
	INT		LuaGetSkillLevel(QLuaScript& sc);
	INT		LuaGetTotalExp(QLuaScript& sc);
	INT		LuaGetBaseAttr(QLuaScript& sc);
	INT		LuaAddHungry(QLuaScript& sc);
	INT		LuaGetBaseAttrScore(QLuaScript& sc);
	INT		LuaSetFightScore(QLuaScript& sc);
	INT     LuaUnlockEquipPos(QLuaScript& sc);

	INT		LuaDoRest(QLuaScript& sc);

	INT		LuaRegenerate(QLuaScript& sc);

	INT		LuaSync(QLuaScript& sc);

protected:
	KPet* m_pPet;
};

// -------------------------------------------------------------------------

#endif /* __KLUAPET_H__ */
