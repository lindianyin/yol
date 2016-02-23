#ifndef _TOWERDEF_H__
#define _TOWERDEF_H__


struct KMopTowerInfo
{
	INT nLeftTime;		// 剩余时间
	DWORD dwStartMopTime;
	INT nState;		// 当前状态
	BOOL bSpeedUp;	// 是否用了加速
};

#endif // _TOWERDEF_H__