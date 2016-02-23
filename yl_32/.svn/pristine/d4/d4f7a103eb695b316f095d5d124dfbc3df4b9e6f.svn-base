/* -------------------------------------------------------------------------
//	文件名		：	factionmanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-25 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef factionmanager_h__
#define factionmanager_h__

#include "stdafx.h"
#include "factiondef.h"
#include "Misc/objevent.h"
#include <string>
using std::string;

enum emEigthTrigram
{
	EIGTH_TRIGRAM_1 = 1,
	EIGTH_TRIGRAM_2 = 2,
	EIGTH_TRIGRAM_3 = 3,
	EIGTH_TRIGRAM_4 = 4,
	EIGTH_TRIGRAM_5 = 5,
	EIGTH_TRIGRAM_6 = 6,
	EIGTH_TRIGRAM_7 = 7,
	EIGTH_TRIGRAM_8 = 8,
	EIGTH_TRIGRAM_TaiChi = 9
};

class KCharacter;
class FactionManager
{
public:
    FactionManager();

	~FactionManager();

	BOOL Init();
   
	BOOL Breath();

	// 得到某一门派路线下指定等级的角色基础数据
	CHARACTER_BASE_ATTRIBUTE* GetCharacterAttribute(INT nFaction, INT nRoute, INT nLevel);

	BOOL SetLevel(KCharacter& rcCharacter, WORD wOldValue, WORD wNewValue);
	BOOL SetRoute(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue);

	BOOL SetMaxLife( KCharacter& rcCharacter, DWORD nOldValue, DWORD nNewValue );

	// 炼卦相关，临时加在这里
	BOOL SetEightTrigramLevel1(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevel2(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevel3(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevel4(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevel5(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevel6(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevel7(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevel8(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel = 0);
	BOOL SetEightTrigramLevelTaiChi(KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue);
	BOOL OnDbLoaded(KCharacter& rcCharacter);


	INT GetBaseMaxLife(KCharacter& rcCharacter);
	INT GetBaseMaxMana(KCharacter& rcCharacter);
	INT GetBaseMaxPower(KCharacter& rcCharacter);

	// 设置快捷药槽！0是红，1是蓝
	BOOL SetMeidicineSlot(DWORD nPlayerId, INT nSlot, INT nRoom, INT nX, INT nY);

private:

	BOOL SplitString(const std::string &src, const std::string &separator, std::vector<string> &dest);

private:
	FactionBaseData m_cFactionData;


};

extern FactionManager g_cFactionManager;

#endif // factionmanager_h__