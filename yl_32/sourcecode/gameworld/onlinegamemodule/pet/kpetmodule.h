/* -------------------------------------------------------------------------
//	文件名		：	kpetmodule.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-11-28 18:13:00
//	功能描述	：  
// -------------------------------------------------------------------------*/

#ifndef __KPETMODULE_H__
#define __KPETMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "onlinegameworld/kplayer.h"

class KPetModule : public KModuleBase
{
public:
	KPetModule();
	~KPetModule() { }

public:
	BOOL OnInit();

	BOOL OnUnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData );

	BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize );

	BOOL SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize );

private:

	BOOL c2sChangePetStateRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sSyncPetInfoRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sPetFreeRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sChangeCharacterRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sResizePetBagRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sChangeNameRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sUpRankRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sFosterRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sFeedRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sPutOnEquipRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen); 

	BOOL c2sTakeOffEquipRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sTransExpRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	//改变阵法
	BOOL c2sChangeTacticRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);
	
	BOOL c2sChangePetSkillState(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sSyncRideInfoRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sTakeOffRideRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sRideUpRankRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sChangeRideState(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sAddRideMagicRate(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sFeedRide(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sChangeRideRepresent(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sComposePetCard(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL InitProtocol();
};

extern KPetModule g_cPetModule;

#endif