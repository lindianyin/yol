/* -------------------------------------------------------------------------
//	文件名		：	nsbase/knpcdef.h
//	创建者		：	luobaohang
//	创建时间	：	2009/12/29 9:21:18
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __NSBASE_KNPCDEF_H__
#define __NSBASE_KNPCDEF_H__

//----------------------------存盘变量枚举不可变更，最大值255---------------------------------------
enum KE_NPC_TASK_DEF
{
	emKCHARACTER_TASK_REPRESENT_ID,			// 表现ID
	//emKCHARACTER_TASK_RELATION,           // npc对玩家的关系
	emKCHARACTER_TASK_FIERCETITIMES,        // NPC在一个战斗中可狂暴次数
	emCHARACTER_TASK_LIFEREPLENISH,         // 生命回复速度
    emCHARACTER_TASK_MANAREPLENISH,         // 真气回复速度
	// End
	emKCHARACTER_TASK_MAX
};

// -------------------------------------------------------------------------

#endif /* __NSBASE_KNPCDEF_H__ */
