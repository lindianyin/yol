#ifndef _MEDITATIONDEF_H__
#define _MEDITATIONDEF_H__


struct KMeditationInfo
{
	INT nMeditationTime;	//今日在线打坐时间
	INT nLastMeditationTime;//昨日在线打坐时间
	DWORD dwTakeOfflineAwardTime; //领取离线打坐经验的时间
};

#endif // _MEDITATIONDEF_H__