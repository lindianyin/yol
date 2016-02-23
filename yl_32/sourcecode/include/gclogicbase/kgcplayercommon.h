/* -------------------------------------------------------------------------
//	文件名		：	gclogicbase/kgcplayercommon.h
//	创建者		：	luobaohang
//	创建时间	：	2010/7/12 10:41:38
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __GCLOGICBASE_KGCPLAYERCOMMON_H__
#define __GCLOGICBASE_KGCPLAYERCOMMON_H__

#include "gclogicbase/kgcplayerbase.h"

//玩家变量ID定义
enum KE_GCPLAYER_TASK_BYTE_COMMON
{
	emKGCPLAYER_TASK_BYTE_SEX,	// 性别

	/*------------*/
	emKGCPLAYER_TASK_BYTE_COMMON_MAX,
};

enum KE_GCPLAYER_TASK_WORD
{
	emKGCPLAYER_TASK_WORD_LEVEL,	// 等级
	emKGCPLAYER_TASK_WORD_PRIMARY_FACTION,	// 职业

	emKGCPLAYER_TASK_WORD_COMMON_MAX,
};

//玩家变量ID定义
enum KE_GCPLAYER_TASK_DWORD
{
	emKGCPLAYER_TASK_DWORD_RANK,		// 水平
	emKGCPLAYER_TASK_DWORD_SOURCE_ID,		// 源服务器玩家Id（仅跨服有效）

	//emKGCPLAYER_TASK_DWORD_SYSMAILID,	// 最近一次收到的系统邮件ID
	//emKGCPLAYER_TASK_DWORD_MAILLOCK,	// 锁住的邮件

	/*------------*/
	emKGCPLAYER_TASK_DWORD_COMMON_MAX,
};

// 需要继承基础变量映射的可从此类继承
template<INT N_TASK_BYTE_MAX, INT N_TASK_WORD_MAX, INT N_TASK_DWORD_MAX>
class KGcPlayerCommonBase : public KGcPlayerBase<N_TASK_BYTE_MAX, N_TASK_WORD_MAX, N_TASK_DWORD_MAX>
{
public:
	// 在此添加变量映射
	KD_GCPLAYER_TASKFUN_BYTE(Sex, emKGCPLAYER_TASK_BYTE_SEX)

	KD_GCPLAYER_TASKFUN_WORD(Level, emKGCPLAYER_TASK_WORD_LEVEL)
	KD_GCPLAYER_TASKFUN_WORD(PrimaryFaction, emKGCPLAYER_TASK_WORD_PRIMARY_FACTION)

	KD_GCPLAYER_TASKFUN_DWORD(Rank, emKGCPLAYER_TASK_DWORD_RANK)
	KD_GCPLAYER_TASKFUN_DWORD(SourcePlayerId, emKGCPLAYER_TASK_DWORD_SOURCE_ID)

};

// -------------------------------------------------------------------------
class KGcPlayerCommon : public KGcPlayerCommonBase<emKGCPLAYER_TASK_BYTE_COMMON_MAX, emKGCPLAYER_TASK_WORD_COMMON_MAX, emKGCPLAYER_TASK_DWORD_COMMON_MAX>
{
public:
	KGcPlayerCommon();

	PCSTR GetAccountName() { return m_szAccount; }
	VOID SetAccountName(PCSTR pszAccountName);

	BOOL LoadFromBuffer(PBYTE pbyBuffer, SIZE_T uSize);
	SIZE_T SaveToBuffer(PBYTE pBuffer, SIZE_T uBufSize);
	SIZE_T CalculateSaveBufferSize();
	virtual VOID Reset();

	BOOL m_bNew; // 需要插入数据库
protected:
	CHAR m_szAccount[KD_GCPLAYER_NAME_LEN];
};


// -------------------------------------------------------------------------

#endif /* __GCLOGICBASE_KGCPLAYERCOMMON_H__ */
