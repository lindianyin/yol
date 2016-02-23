/* -------------------------------------------------------------------------
//	文件名		：	nsbase/kluadoodad.h
//	创建者		：	simon
//	创建时间	：	2010/3/15 9:36:05
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __NSBASE_KLUADOODAD_H__
#define __NSBASE_KLUADOODAD_H__


// -------------------------------------------------------------------------

class KDoodad;

// -------------------------------------------------------------------------

class KLuaDoodad : public QLunaBase
{
public:
	explicit KLuaDoodad(KDoodad* pDoodad) : m_pIt(pDoodad)
	{
	}
	virtual ~KLuaDoodad(void) {}

	static KDoodad* GetIt(QLuaScript& sc, INT nIndex);
	KDoodad* GetIt()	{ return m_pIt; };

	BOOL	ClearTempTable();
	INT		LuaGetTempTable(QLuaScript& sc);

	//脚本接口定义
	DECLARE_LUA_CLASS(KLuaDoodad);


	DWORD		getId(VOID) CONST;
	DWORD		getTemplateId(VOID) CONST;
	DWORD		getSceneId(VOID) CONST;
	INT			getFaceDirection(VOID) CONST;
	LPCSTR		getName() CONST;
	INT			getState() CONST;

	INT			LuaGetPosition(QLuaScript& sc);
	INT			LuaSetPosition(QLuaScript& sc);
	INT			LuaSetOwnerId(QLuaScript& sc);

#ifdef GAME_SERVER
	INT			LuaTeleportTo(QLuaScript& sc); // 瞬移

#endif // GAME_SERVER

protected:
	KDoodad* m_pIt;
};

// -------------------------------------------------------------------------

#endif /* __NSBASE_KLUADOODAD_H__ */
