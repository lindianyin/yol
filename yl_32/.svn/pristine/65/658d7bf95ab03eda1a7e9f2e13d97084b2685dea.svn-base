
#ifndef __KMAGICATTRIBMANAGER_H__
#define __KMAGICATTRIBMANAGER_H__

// -------------------------------------------------------------------------
#include "kmanager_i.h"
#include "kmagicdesc.h"
//#include "knpcattribmodify.h"

class KMagicDesc;
struct KMagicAttrib;
class KNpc;
class KNpcAttribModify;
class KCharacter;

// -------------------------------------------------------------------------

// 此类为一对外接口，具体功能是通过调用KNpcAttribModify和KMagicDesc来实现
class KMagicAttribMgr : public IKManager
{
public:
	LPCSTR	GetName() CONST	{ return "MagicAttrib"; };
	
	// 根据Id获得相应的魔法属性名
	LPCSTR GetStringFromId(INT nAttrib);

	// 根据魔法属性名获得相应的Id
	INT GetIdFormString(PCSTR szMagicAttribName);

	// 获得指定技能指定魔法属性的描述
	LPCSTR	GetMagicDesc(CONST KMagicAttrib *pData, INT nSkillId);

	// 为指定Npc应用魔法属性
	VOID ModifyAttrib(KCharacter *pCharacterLauncher, KCharacter *pCharacter, INT nSkillId, CONST KMagicAttrib& rcpMagic, BOOL bIsEndProcess);

	// 是否是魔法属性失效
	BOOL IsEndProcess() CONST;

	// 获得魔法属性的释放者
	KNpc* GetMagicLauncher() CONST;

private:
	//KNpcAttribModify m_cNpcAttribModify;		// 工具类用于修改Npc的属性

	KMagicDesc m_cMagicDesc;					// 工具类维护一个魔法属性字符串和魔法属性值的表
};

extern KMagicAttribMgr g_cMagicAttribMgr;

// -------------------------------------------------------------------------

#endif /* __KMAGICATTRIBMANAGER_H__ */
