/* -------------------------------------------------------------------------
//	文件名		：	factionmanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-25 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef factiondef_h__
#define factiondef_h__

enum FACTION_TYPE
{
    FACTION_WUSHANG	= 1,	// 无上
	FACTION_TIANYAO	= 2,	// 天曜
	FACTION_YOUMING	= 3,	// 幽冥
	FACTION_SENLUO	= 4,	// 森罗
};

// 无上门派路线
enum FACTION_ROUTE_WUSHANG
{
    ROUTE_WUSHANG_TONGYONG	= 0,	// 通用
	ROUTE_WUSHANG_JIANSHEN	= 1,	// 剑神
	ROUTE_WUSHANG_YUHE		= 2,	// 愈合
 };

// 天曜门派路线
enum FACTION_ROUTE_TIANYAO
{
	ROUTE_TIANYAO_TONGYONG	= 0,	// 通用
	ROUTE_TIANYAO_FANGHU	= 1,	// 防护
	ROUTE_TIANYAO_SHENSHENG	= 2,	// 神圣
};

// 幽冥门派路线
enum FACTION_ROUTE_YOUMING
{
	ROUTE_YOUMING_TONGYONG	= 0,	// 通用
	ROUTE_YOUMING_WENGU		= 1,	// 稳固
	ROUTE_YOUMING_CHONGJI	= 2,	// 冲击
};

// 森罗门派路线
enum FACTION_ROUTE_SENLUO
{
	ROUTE_SENLUO_TONGYONG	= 0,	// 通用
	ROUTE_SENLUO_SHOULIE	= 1,	// 狩猎
	ROUTE_SENLUO_CISHA		= 2,	// 刺杀
};

struct CHARACTER_BASE_ATTRIBUTE
{
	INT nLevel;        // 等级
    INT nConstitution; // 体质
	INT nStrength;     // 筋骨
	INT nAgile;        // 身法
	INT nIntelligence; // 元气
	INT nSpirit;       // 精神
};

struct FACTION_PLAYER_SETTING
{
    INT  nRepresentQuality;
	INT  nBackgroundMusicVolume;
	INT  nGameSoundVolume;
	INT  nAutoAddHp;
	INT  nAutoAddMp;
	BOOL bHideOtherPlayer;
	BOOL bHideParticleEffect;
	BOOL bLowestVolumeLock;
	BOOL bLowestQualityLock;
};

struct FactionBaseData
{
	std::map<INT, std::vector<CHARACTER_BASE_ATTRIBUTE*> > mapFactionGrowthData; // 职业成长数据
	//std::map<INT, std::vector<FactionSkillTemplate*> >  mapFactionSkillTemplates; // 门派技能数据(Templates
};

#endif // factiondef_h__