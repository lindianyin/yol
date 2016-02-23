#ifndef _ACTIVITYDEF_H__
#define _ACTIVITYDEF_H__

struct KActivityInfo
{
	DWORD dwLastEnterZBTTime;		// 上次进入战宝堂的时间
	DWORD dwLastMPJJRank;			// 上次门派竞技排名
	BYTE  byKinBattleWinTimes;		// 三界争霸连胜场数
};

#endif // _ACTIVITYDEF_H__