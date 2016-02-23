
#ifndef __KMAGICDESC_H__
#define __KMAGICDESC_H__

// -------------------------------------------------------------------------
struct KMagicAttrib;

//#include "fightskill/ktypedef.h"
// 魔法属性和Id和Name的对应表
class KMagicDesc
{
private:

	typedef std::map<std::string, INT> KMagicNameIdMap;

public:
	KMagicDesc();

	// 根据Id获得相应的魔法属性名
	LPCSTR GetStringFromId(INT nAttrib);

	// 根据魔法属性名获得相应的Id
	INT GetIdFormString(LPCSTR pszMagicAttribName);

	// 获得指定技能指定魔法属性的描述
	LPCSTR	GetDesc(CONST KMagicAttrib *pData, INT nSkillId);

private:
	KMagicNameIdMap m_mapMagicNameId;	
};

// -------------------------------------------------------------------------

#endif /* __KMAGICDESC_H__ */
