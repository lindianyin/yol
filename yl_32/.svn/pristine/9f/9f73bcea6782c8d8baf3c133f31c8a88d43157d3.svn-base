#pragma once

#include "onlinegameworld/kgcmodulebase.h"

class KPlayer;

class QRelationModule : public KModuleBase
{
private:
	virtual BOOL InitProtocol();
	virtual BOOL OnInit();
	virtual BOOL OnUnInit();
	virtual BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

public:
	QRelationModule(void);
	~QRelationModule(void);

	static void C2SOnRequestTeacher			(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnRequestStudent			(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnSearchTeacher			(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnSearchStudent			(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnSetTrainingOpt			(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnGetTrainingOpt			(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnTrainingViewInfo		(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnTrainingMakeFriend		(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnTrainingApply			(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static void C2SOnWishTheacherComeHere	(KPlayer* pPlayer, char* pData, size_t uDataLen);

	// ===================================================================================

	static void R2SOnStartSyncRelation		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnSyncRelation			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnDelRelation			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnFriendInfo				(BYTE* pbyData, size_t uDataLen);
	static void R2SOnNotifyOnline			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnNotifyRelation			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnDeletePlayer			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnTrainingError			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnSearchTeacher			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnSearchStudent			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnTrainingOption			(BYTE* pbyData, size_t uDataLen);
	static void R2SOnApplyingTeacher		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnApplyingStudent		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnTrainingViewInfo		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnTrainingRoleInfo		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnApplyForTeacher		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnResultApplyIbCoin		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnResultGainIbCoin		(BYTE* pbyData, size_t uDataLen);
	static void R2SOnProcessAwardFriendFavor(BYTE* pbyData, size_t uDataLen);

public:
	static BOOL SendToClient(INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);
	static BOOL SendLenToClient(INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

	static BOOL SendToCordinator( IMemBlock* piBuffer );

	static BYTE	m_byTempData[32*1024];

private:
	typedef void (*PROCESS_PROTOCOL_FUNC)(KPlayer* pPlayer, char* pData, size_t uDataLen);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[100];
	// size_t m_nProtocolSize[c2s_tong_end];

};

extern QRelationModule g_RelationModule;


