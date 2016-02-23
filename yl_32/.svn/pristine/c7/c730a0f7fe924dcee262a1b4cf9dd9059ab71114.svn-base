
#ifndef	__KACTIVITYPROTOCOL_H__
#define __KACTIVITYPROTOCOL_H__

#include "onlinegameworld/kprotocol.h"
#include "config/kplayerbasedbdef.h"

#pragma pack(1)

enum KC2S_ACTIVITY_PROTOCOL
{
	c2s_activity_protocol_start = 0,
	c2s_get_kinbattle_record_request,
	c2s_get_kinbattle_winnerid_request,
	c2s_get_kinbattle_join_num_request,
};

enum KS2C_ACTIVITY_PROTOCOL
{
	s2c_sync_activity_time = 0,		// 同步活动时间
	s2c_sync_kinbattle_records,
	s2c_sync_kinbattle_winnerId,
	s2c_sync_kinbattle_join_num,
	s2c_sync_last_winkin_info,
};

// c2s

struct KC2S_GET_KINBATTLE_RECORD_REQUEST : KPTC_HEADER<c2s_get_kinbattle_record_request>
{
	
};

struct KC2S_GET_KINBATTLE_WINNERID_REQUEST : KPTC_HEADER<c2s_get_kinbattle_winnerid_request>
{

};

struct KC2S_GET_KINBATTLE_JOINNUM_REQUEST : KPTC_HEADER<c2s_get_kinbattle_join_num_request>
{

};


// s2c
struct KS2C_SYNC_KINBATTLE_RECORD : KLENPTC_HEADER<s2c_sync_kinbattle_records>
{
	INT nRecordNums;
	KKinBattleRecord arrRecords[0];
};

struct KS2C_SYNC_KINBATTLE_WINNERID : KPTC_HEADER<s2c_sync_kinbattle_winnerId>
{
	DWORD dwWinnerId;
};

struct KS2C_SYNC_LAST_WINKIN_INFO : KPTC_HEADER<s2c_sync_last_winkin_info>
{
	KKinBattleRecord sRecord;
};

struct KS2C_SYNC_KINBATTLE_JOIN_NUM : KPTC_HEADER<s2c_sync_kinbattle_join_num>
{
	INT nJoinNum;
	INT nHasJoined;
};

struct KS2C_SYNC_ACTIVITY_TIME : KPTC_HEADER<s2c_sync_activity_time>
{
	INT nActivityIdx;		// 活动索引id
	INT nBaseActivityIdx;

	DWORD dwOpenTime;
	DWORD dwCloseTime;
};

#pragma pack()

#endif
