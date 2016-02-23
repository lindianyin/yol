/* -------------------------------------------------------------------------
//	文件名		：	relationcounter.h
//	创建者		：	
//	创建时间	：	
//	功能描述	：	玩家好友计数器（防止超出好友数目限制）
//
// -----------------------------------------------------------------------*/
#ifndef __KRELATIONCOUNTER_H__
#define __KRELATIONCOUNTER_H__

#include <map>
#include <functional>

class KRelationCounter
{
public:
	struct KRELATION_COUNT
	{
		INT		nCurCount;	// 当前数量
		INT		nMaxCount;	// 最大数量
	};
	enum KEPLAYERRELATION_TYPE_MAXNUMBER
	{
		emKRELATION_FRIENDRELATION_COUNT	= 11, // 暂时开二百
		emKRELATION_BLACKRELATION_COUNT		= 11,
		emKRELATION_ENEMYRELATION_COUNT		= 40,
		emKRELATION_COUPLE_COUNT			= 1,
		//emKRELATION_TRAIN_AND_BUDDY_COUNT	= 240,		// 最多师徒,密友关系个数
	};
	typedef std::map<INT, KRELATION_COUNT>	RELATION_MAP;
public:
	inline static KRelationCounter *Instance()
	{
		return &ms_isnt;
	}
	KRelationCounter();
	~KRelationCounter();
	INT  CheckReachMaxCount(INT nType, INT nPlayerId);
	INT  GetTrainAndBuddyCount(INT nPlayerId);// 获取现有师徒，密友关系数量
	INT  GetOneTypeCount(INT nPlayerId, BYTE btType, BOOL bMaster = TRUE);// 获取制定关系类型的关系数量
private:
	RELATION_MAP			m_mapRelationCount;
	static KRelationCounter	ms_isnt;
};

#endif //__KRELATIONCOUNTER_H__

