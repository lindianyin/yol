/* -------------------------------------------------------------------------
//	文件名		：	fightskillmodule.h
//	创建者		：	zhangzhixiong
//	实现者		：	
//	创建时间	：	20012-5-21 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef fightskillmodule_h__
#define fightskillmodule_h__

#include "onlinegameworld/kmodulebase.h"
#include "fightskillprotocol.h"

class FightSkillModule : public KModuleBase
{
public:
	FightSkillModule();
	virtual ~FightSkillModule();

public:	
	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();
};

inline FightSkillModule* GetFightSkillModule()
{
	static FightSkillModule s_FightSkillModule;
	return &s_FightSkillModule;
}

#endif // fightskillmodule_h__