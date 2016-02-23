#ifndef __KITEMPROTOCOL_H__
#define __KITEMPROTOCOL_H__

class KPlayer;

class KItemProtocol
{
public:

	static BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

private:

	static BOOL c2sSwitchItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sPlayerUseItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL	c2sResizeExtBag(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sSplitItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sArrangeBagItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sPlayerThrowAwayItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sViewEquip(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sRepairItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sCloseRepository(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sApplyEnhance(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sUpgrade(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sRefine(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);//×°±¸Ï´Á¶
	static BOOL c2sSaveAppendAttr(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sEquipIdentify(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sStoneMix(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sStoneMount(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sStoneRemove(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sEquipRerand(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sStonePunch(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sSellItems(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL c2sOnReApplyPendants(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
};

#endif
