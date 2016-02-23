
#include "stdafx.h"
#include "kbasproptbl.h"
#include "kmagicattribmanager.h"
#include "config/knpcdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------------

#define	_KD_TABFILE_PATH					KD_ITEM_SETTING_PATH
#define	_KD_TABFILE_SUFFIX					".txt"

// for common items
#define	_KD_TABTITLE_NAME					"Name"
#define	_KD_TABTITLE_KIND					"Desc"
#define	_KD_TABTITLE_GENRE					"Genre"
#define	_KD_TABTITLE_DETAIL					"DetailType"
#define	_KD_TABTITLE_PARTIC					"ParticularType"
#define	_KD_TABTITLE_ICON					"Icon"
#define	_KD_TABTITLE_VIEW					"View"
#define	_KD_TABTITLE_OBJECTID				"ObjID"
#define	_KD_TABTITLE_INTRO					"Intro"
#define _KD_TABTITLE_HELP					"Help"
#define	_KD_TABTITLE_SERIES					"Series"
#define	_KD_TABTITLE_PRICE					"Price"
#define _KD_TABTITLE_CURRENCYTYPE			"CurrencyType"
#define	_KD_TABTITLE_LEVEL					"Level"
#define _KD_TABTITLE_QUALITY				"Quality"
#define	_KD_TABTITLE_VALUE					"ItemValue"
#define	_KD_TABTITLE_BIND					"BindType"
#define	_KD_TABTITLE_CANTRADE				"CanTrade"
#define	_KD_TABTITLE_CANDESTROY				"CanDestroy"
#define	_KD_TABTITLE_VALUABLE				"Valuable"
#define _KD_TABTITLE_CLASS					"ClassName"
#define _KD_TABTITLE_MAKECOST				"MakeCost"

// for other items
#define	_KD_TABTITLE_OTH_STACK				"Stack"
#define _KD_TABTITLE_OTH_REQLEVEL			"ReqLevel"
#define _KD_TABTITLE_OTH_CDTYPE				"CDType"
#define _KD_TABTITLE_OTH_SHORTCUT			"Shortcut"
#define _KD_TABTITLE_OTH_FORBIDTYPE			"ForbidType"

// for stones
#define	_KD_TABTITLE_STONE_STACK				"Stack"
#define _KD_TABTITLE_STONE_REQLEVEL				"ReqLevel"
#define _KD_TABTITLE_STONE_COLOR				"StoneColor"
#define _KD_TABTITLE_STONE_EQUIPLEVEL			"EquipLevel"
#define _KD_TABTITLE_STONE_TYPE					"StoneType"

// for script items
#define _KD_TABTITLE_SS_SKILLLEVEL			"SkillLevel"
#define	_KD_TABTITLE_SS_CASTTYPE			"CastType"
#define	_KD_TABTITLE_SS_BROADCAST			"IsBroadcasting"
#define _KD_TABTITLE_SS_EXTPARAM(n)			"ExtParam"#n

// for skill items
#define	_KD_TABTITLE_SK_SKILL				"SkillID"

// for medicines
#define	_KD_TABTITLE_MED_MAGIC1				"MagicName1"
#define	_KD_TABTITLE_MED_VALUE1				"Value1"
#define	_KD_TABTITLE_MED_TIME1				"Time1"
#define	_KD_TABTITLE_MED_MAGIC2				"MagicName2"
#define	_KD_TABTITLE_MED_VALUE2				"Value2"
#define	_KD_TABTITLE_MED_TIME2				"Time2"
#define	_KD_TABTITLE_MED_MAGIC3				"MagicName3"
#define	_KD_TABTITLE_MED_VALUE3				"Value3"
#define	_KD_TABTITLE_MED_TIME3				"Time3"
#define _KD_TABTITLE_MED_BZONE_CANUSE		"CanUseInBZone"

// for plans
#define _KD_TABTITLE_PLAN_ID				"PlanID"

// for all equipments
#define	_KD_TABTITLE_EQ_RESMALE				"RepresentId"
#define	_KD_TABTITLE_EQ_RESFEMALE			"RepresentId"
#define _KD_TABTITLE_EQ_PANDENT(n)			"Pandent"#n"ID"
#define _KD_TABTITLE_EQ_EX_PANDENT(n)		"ExPandent"#n
#define	_KD_TABTITLE_EQ_BASICTYPE(n)		"BasicPropType"
#define _KD_TABTITLE_EQ_BASICVALUE(n)		"BasicPropValue"
#define	_KD_TABTITLE_EQ_BASICMIN(n, m)		"BasicProp"#n"PA"#m"Min"
#define	_KD_TABTITLE_EQ_BASICMAX(n, m)		"BasicProp"#n"PA"#m"Max"
#define	_KD_TABTITLE_EQ_REQTYPE(n)			"ReqProp"#n"Type"
#define	_KD_TABTITLE_EQ_REQVALUE(n)			"ReqProp"#n"Value"
#define _KD_TABTITLE_BG_REQLEVEL			"ReqLevel"
#define _KD_TABTITLE_EQ_STRTIMES(n)			"Str"#n"Times"
#define _KD_TABTITLE_EQ_STRMANAME(n)		"StrMA"#n"Name"
#define _KD_TABTITLE_EQ_STRMAMIN(n, m)		"StrMA"#n"PA"#m"Min"
#define _KD_TABTITLE_EQ_STRMAMAX(n, m)		"StrMA"#n"PA"#m"Max"
#define _KD_TABTITLE_EQ_CATEGORY			"Category"
#define _KD_TABTITLE_EQ_REFINELEVEL			"RefineLevel"
#define _KD_TABTITLE_EQ_UNLOCKINTERVAL		"UnLockInterval"
#define _KD_TABTITLE_EQ_ACTVRULEID(n)		"ActiveProp"#n"RuleID"
#define _KD_TABTITLE_EQ_ACTVMANAME(n)		"ActiveProp"#n"MAName"
#define _KD_TABTITLE_EQ_ACTVMAPART(n)		"ActiveProp"#n"Part"
#define _KD_TABTITLE_EQ_ACTVPRIORLEVEL(n)	"ActiveProp"#n"PriorLevel"
#define _KD_TABTITLE_EQ_ACTVPROPMIN(n, m)	"ActiveProp"#n"PA"#m"Min"
#define _KD_TABTITLE_EQ_ACTVPROPMAX(n, m)	"ActiveProp"#n"PA"#m"Max"
#define _KD_TABTITLE_EQ_FIGHTPOWER			"FightPower"
#define _KD_TABTITLE_EQ_SLOTMIN				"SlotMin"
#define _KD_TABTITLE_EQ_SLOTMAX				"SlotMax"
#define _KD_TABTITLE_EQ_NEEDITEM_G(n)		"NeedItem"#n"G"
#define _KD_TABTITLE_EQ_NEEDITEM_D(n)		"NeedItem"#n"D"
#define _KD_TABTITLE_EQ_NEEDITEM_P(n)		"NeedItem"#n"P"
#define _KD_TABTITLE_EQ_NEEDITEM_AMOUNT(n)	"NeedItem"#n"Amount"

// for weapon
#define _KD_TABTITLE_WP_DMGINFMIN			"DmgInfMin"
#define _KD_TABTITLE_WP_DMGINFMAX			"DmgInfMax"
#define _KD_TABTITLE_WP_DMGSECMIN			"DmgSectionMin"
#define _KD_TABTITLE_WP_DMGSECMAX			"DmgSectionMax"

// for general common equipments
#define _KD_TABTITLE_GC_STUFFDETAIL(n)		"Stuff"#n"Detail"
#define _KD_TABTITLE_GC_STUFFPARTICULAR(n)	"Stuff"#n"Particular"

// for ride on horse equipments
#define	_KD_TABTITLE_EQ_RIDETYPE(n)			"RideProp"#n"Type"
#define	_KD_TABTITLE_EQ_RIDEMIN(n, m)		"RideProp"#n"PA"#m"Min"
#define	_KD_TABTITLE_EQ_RIDEMAX(n, m)		"RideProp"#n"PA"#m"Max"
#define _KD_TABTITLE_EQ_RIDEMAGICTYPE		"RideType"
#define _KD_TABTITLE_EQ_RIDEMAGICVALUE		"RideValue"

// for purple equipments
#define _KD_TABTITLE_PE_RANDNAME(n)			"MagicName"#n
#define _KD_TABTITLE_PE_RANDLEVEL(n)		"MagicLevel"#n

// for suite equipments
#define _KD_TABTITLE_SE_RANDNAME(n)			"MagicName"#n
#define _KD_TABTITLE_SE_RANDMIN(n, m)		"MA"#n"Prop"#m"Min"
#define _KD_TABTITLE_SE_RANDMAX(n, m)		"MA"#n"Prop"#m"Max"
#define _KD_TABTITLE_SE_SUITEID				"SuiteID"

// for bags
#define _KD_TABTITLE_BG_POS					"Pos"

// for magicattribs
#define _KD_TABTITLE_MA_SUFFIX				"Suffix"
#define _KD_TABTITLE_MA_LEVEL				"Level"
#define	_KD_TABTITLE_MA_VALUE				"ItemValue"
#define	_KD_TABTITLE_MA_DARKNESS			"IsDarkness"
#define _KD_TABTITLE_MA_SERIES				"Series"
#define _KD_TABTITLE_MA_MAGICNAME			"MagicName"
#define _KD_TABTITLE_MA_MA1MIN				"MA1Min"
#define _KD_TABTITLE_MA_MA1MAX				"MA1Max"
#define _KD_TABTITLE_MA_MA2MIN				"MA2Min"
#define _KD_TABTITLE_MA_MA2MAX				"MA2Max"
#define _KD_TABTITLE_MA_MA3MIN				"MA3Min"
#define _KD_TABTITLE_MA_MA3MAX				"MA3Max"
#define _KD_TABTITLE_MA_MELEEWEAPON			"MeleeWeapon"
#define _KD_TABTITLE_MA_RANGEWEAPON			"RangeWeapon"
#define _KD_TABTITLE_MA_ARMOR				"Armor"
#define _KD_TABTITLE_MA_RING				"Ring"
#define _KD_TABTITLE_MA_NECKLACE			"Necklace"
#define _KD_TABTITLE_MA_AMULET				"Amulet"
#define _KD_TABTITLE_MA_BOOTS				"Boots"
#define _KD_TABTITLE_MA_BELT				"Belt"
#define _KD_TABTITLE_MA_HELM				"Helm"
#define _KD_TABTITLE_MA_CUFF				"Cuff"
#define _KD_TABTITLE_MA_PENDANT				"Pendant"
#define _KD_TABTITLE_MA_HORSE				"Horse"
#define _KD_TABTITLE_MA_MASK				"Mask"
#define _KD_TABTITLE_MA_BOOK				"Book"
#define _KD_TABTITLE_MA_PARTNERRING			"PartnerRing"
#define _KD_TABTITLE_MA_PARTNERBODY			"PartnerBody"
#define _KD_TABTITLE_MA_PARTNERWEAPON		"PartnerWeapon"
#define _KD_TABTITLE_MA_PARTNERCUFF			"PartnerCuff"
#define _KD_TABTITLE_MA_PARTNERAMULET		"PartnerAmulet"

// for magic active rule
#define  _KD_TABTITLE_MR_RULEID				"RuleId"
#define  _KD_TABTITLE_MR_SKILLID			"SkillId"
#define	 _KD_TABTITLE_MR_BEGINLEVEl			"BeginLevel"
#define  _KD_TABTITLE_MR_ENDLEVEL			"EndLevel"
#define  _KD_TABTITLE_MR_DESCRIPTION		"Desc"

// for suite informations
#define _KD_TABTITLE_SI_SUITEID				"SuiteID"
#define _KD_TABTITLE_SI_NAME				"Name"
#define _KD_TABTITLE_SI_REQNUM(n)			"SuiteNum"#n
#define _KD_TABTITLE_SI_MANAME(n)			"SuiteMA"#n"Name"
#define _KD_TABTITLE_SI_MAVALUE(n, m)		"SuiteMA"#n"PA"#m
#define _KD_TABTITLE_SI_HEAD				"Head"
#define _KD_TABTITLE_SI_BODY				"Body"
#define _KD_TABTITLE_SI_BELT				"Belt"
#define _KD_TABTITLE_SI_WEAPON				"Weapon"
#define _KD_TABTITLE_SI_FOOT				"Foot"
#define _KD_TABTITLE_SI_CUFF				"Cuff"
#define _KD_TABTITLE_SI_AMULET				"Amulet"
#define _KD_TABTITLE_SI_RING				"Ring"
#define _KD_TABTITLE_SI_NECKLACE			"Necklace"
#define _KD_TABTITLE_SI_PENDANT				"Pendant"
#define _KD_TABTITLE_SI_HORSE				"Horse"
#define _KD_TABTITLE_SI_MASK				"Mask"
#define _KD_TABTITLE_SI_BOOK				"Book"

// for equip effect
#define _KD_TABTITLE_EE_SERIES				"Series"
#define _KD_TABTITLE_EE_CATEGORY			"Category"
#define _KD_TABTITLE_EE_EFFECTRES			"EffectRes"

// for stones
#define _KD_TABTITLE_ST_ATTRNAME(n)			"MagicName"#n
#define _KD_TABTITLE_ST_PARAM(n)			"Magic"#n"Param"

//------------------------------------------------------------------------

// 装备基础属性
#define _KD_BPT_EQ_BASEATTR(p, t, n)																						\
	{ emSTR,	_KD_TABTITLE_EQ_BASICTYPE(n),	(t)[(n) - 1],										sizeof((t)[(n) - 1]) },	\
	{ emINT,	_KD_TABTITLE_EQ_BASICVALUE(n),	(LPSTR)&(p)->aryBasic[(n) - 1].aryRange[0].nMin,	0 },					\
	{ emINT,	_KD_TABTITLE_EQ_BASICVALUE(n),	(LPSTR)&(p)->aryBasic[(n) - 1].aryRange[0].nMax,	0 },					\
	{ emINT,	_KD_TABTITLE_EQ_BASICVALUE(n),	(LPSTR)&(p)->aryBasic[(n) - 1].aryRange[1].nMin,	0 },					\
	{ emINT,	_KD_TABTITLE_EQ_BASICVALUE(n),	(LPSTR)&(p)->aryBasic[(n) - 1].aryRange[1].nMax,	0 }

// 装备升级需求材料
#define _KD_BPT_EQ_NEEDITEM(p, n)																						\
	{ emINT,	_KD_TABTITLE_EQ_NEEDITEM_G(n),		(LPSTR)&(p)->aryNeedItems[(n) - 1].nGenre,			0 },			\
	{ emINT,	_KD_TABTITLE_EQ_NEEDITEM_D(n),		(LPSTR)&(p)->aryNeedItems[(n) - 1].nDetail,			0 },			\
	{ emINT,	_KD_TABTITLE_EQ_NEEDITEM_P(n),		(LPSTR)&(p)->aryNeedItems[(n) - 1].nParticular,		0 },			\
	{ emINT,	_KD_TABTITLE_EQ_NEEDITEM_AMOUNT(n),	(LPSTR)&(p)->aryNeedItems[(n) - 1].nAmount,			0 }

// 装备需求属性
#define _KD_BPT_EQ_REQATTR(p, n)																							\
	{ emINT,	_KD_TABTITLE_EQ_REQTYPE(n),		(LPSTR)&(p)->aryReq[(n) - 1].eType,					emEQUIP_REQ_NONE },		\
	{ emINT,	_KD_TABTITLE_EQ_REQVALUE(n),	(LPSTR)&(p)->aryReq[(n) - 1].nValue,				0 }

// 装备改造激活属性
#define _KD_BPT_EQ_STRMA(p, t, n)																							\
	{ emINT,	_KD_TABTITLE_EQ_STRTIMES(n),	(LPSTR)&(p)->aryStr[(n) - 1].nTimes,			0 },						\
	{ emSTR,	_KD_TABTITLE_EQ_STRMANAME(n),	(t)[(n) - 1],									sizeof((t)[(n) - 1]) },		\
	{ emINT,	_KD_TABTITLE_EQ_STRMAMIN(n, 1),	(LPSTR)&(p)->aryStr[(n) - 1].aryRange[0].nMin,	0 },						\
	{ emINT,	_KD_TABTITLE_EQ_STRMAMAX(n, 1),	(LPSTR)&(p)->aryStr[(n) - 1].aryRange[0].nMax,	0 },						\
	{ emINT,	_KD_TABTITLE_EQ_STRMAMIN(n, 2),	(LPSTR)&(p)->aryStr[(n) - 1].aryRange[1].nMin,	0 },						\
	{ emINT,	_KD_TABTITLE_EQ_STRMAMAX(n, 2),	(LPSTR)&(p)->aryStr[(n) - 1].aryRange[1].nMax,	0 }

// 上马激活属性
#define _KD_BPT_EQ_RIDEMA(p, t, n)																						\
	{ emSTR,	_KD_TABTITLE_EQ_RIDETYPE(n),	(t)[(n) - 1],									sizeof((t)[(n) - 1]) },	\
	{ emINT,	_KD_TABTITLE_EQ_RIDEMIN(n, 1),	(LPSTR)&(p)->aryRide[(n) - 1].aryRange[0].nMin,	0 },					\
	{ emINT,	_KD_TABTITLE_EQ_RIDEMAX(n, 1),	(LPSTR)&(p)->aryRide[(n) - 1].aryRange[0].nMax,	0 },					\
	{ emINT,	_KD_TABTITLE_EQ_RIDEMIN(n, 2),	(LPSTR)&(p)->aryRide[(n) - 1].aryRange[1].nMin,	0 },					\
	{ emINT,	_KD_TABTITLE_EQ_RIDEMAX(n, 2),	(LPSTR)&(p)->aryRide[(n) - 1].aryRange[1].nMax,	0 }

// 紫装制定随机魔法属性
#define  _KD_BPT_PE_RANDMA(p, t, n)																							\
	{ emSTR,	_KD_TABTITLE_PE_RANDNAME(n),	(t)[(n) - 1],									sizeof((t)[(n) - 1]) },		\
	{ emINT,	_KD_TABTITLE_PE_RANDLEVEL(n),	(LPSTR)&(p)->aryRand[(n) - 1].nLevel,			0 }

// 套装激活属性
#define _KD_BPT_EQ_SUITEMA(p, t, n)																							\
	{ emINT,	_KD_TABTITLE_SI_REQNUM(n),		(LPSTR)&(p)->arySuite[(n) - 1].nNum,			0 },						\
	{ emSTR,	_KD_TABTITLE_SI_MANAME(n),		(t)[(n) - 1],									sizeof((t)[(n) - 1]) },		\
	{ emINT,	_KD_TABTITLE_SI_MAVALUE(n, 1),	(LPSTR)&(p)->arySuite[(n) - 1].aryValue[0],		0 },						\
	{ emINT,	_KD_TABTITLE_SI_MAVALUE(n, 2),	(LPSTR)&(p)->arySuite[(n) - 1].aryValue[1],		0 }

// 套装随机魔法属性
#define _KD_BPT_SE_RANDMA(p, t, n)																							\
	{ emSTR,	_KD_TABTITLE_SE_RANDNAME(n),	(t)[(n) - 1],									sizeof((t)[(n) - 1]) },		\
	{ emINT,	_KD_TABTITLE_SE_RANDMIN(n, 1),	(LPSTR)&(p)->aryRand[(n) - 1].aryRange[0].nMin,	0 },						\
	{ emINT,	_KD_TABTITLE_SE_RANDMAX(n, 1),	(LPSTR)&(p)->aryRand[(n) - 1].aryRange[0].nMax,	0 },						\
	{ emINT,	_KD_TABTITLE_SE_RANDMIN(n, 2),	(LPSTR)&(p)->aryRand[(n) - 1].aryRange[1].nMin,	0 },						\
	{ emINT,	_KD_TABTITLE_SE_RANDMAX(n, 2),	(LPSTR)&(p)->aryRand[(n) - 1].aryRange[1].nMax,	0 }

//------------------------------------------------------------------------

// 道具基本数据
#define __KD_ITEM_BASEDATA(p)																			\
	{ emSTR,	_KD_TABTITLE_NAME,			(p)->szName,					sizeof((p)->szName) },			\
	{ emSTR,	_KD_TABTITLE_KIND,			(p)->szKind,					sizeof((p)->szKind) },			\
	{ emSTR,	_KD_TABTITLE_CLASS,			(p)->szClass,					sizeof((p)->szClass) },			\
	{ emINT,	_KD_TABTITLE_GENRE,			(LPSTR)&(p)->nItemGenre,		ITEM_INVALID_TYPE },			\
	{ emINT,	_KD_TABTITLE_DETAIL,		(LPSTR)&(p)->nDetailType,		ITEM_INVALID_TYPE },			\
	{ emINT,	_KD_TABTITLE_PARTIC,		(LPSTR)&(p)->nParticular,		ITEM_INVALID_TYPE },			\
	{ emINT,	_KD_TABTITLE_PRICE,			(LPSTR)&(p)->nPrice,			0 },							\
	{ emINT,	_KD_TABTITLE_CURRENCYTYPE,	(LPSTR)&(p)->byCurrencyType,	0 },							\
	{ emINT,	_KD_TABTITLE_QUALITY,		(LPSTR)&(p)->nQuality,			1 },							\
	{ emINT,	_KD_TABTITLE_BIND,			(LPSTR)&(p)->nBindType,			emITEM_BIND_NONE },				\
	{ emINT,	_KD_TABTITLE_CANDESTROY,	(LPSTR)&(p)->nCanDestroy,		emITEM_DESTROY_NONE }

#ifdef GAME_SERVER
#define _KD_ITEM_BASEDATA				__KD_ITEM_BASEDATA
#else
#define _KD_ITEM_BASEDATA(p)																			\
	__KD_ITEM_BASEDATA(p),																				\
	{ emSTR,	_KD_TABTITLE_ICON,		(p)->szIconImage,				sizeof((p)->szIconImage) },		\
	{ emSTR,	_KD_TABTITLE_VIEW,		(p)->szViewImage,				sizeof((p)->szViewImage) },		\
	{ emSTR,	_KD_TABTITLE_INTRO,		(p)->szIntro,					sizeof((p)->szIntro) },			\
	{ emSTR,	_KD_TABTITLE_HELP,		(LPSTR)&(p)->szHelp,			sizeof((p)->szHelp) }
#endif

// 普通使用道具的基本数据类型变量表
#define _KD_OTHER_DATA(p)																				\
	_KD_ITEM_BASEDATA(p),																				\
	{ emINT,	_KD_TABTITLE_OTH_STACK,		(LPSTR)&(p)->nStackMax,		KD_ITEM_MAX_STACK_COUNT },		\
	{ emINT,	_KD_TABTITLE_OTH_REQLEVEL,	(LPSTR)&(p)->nReqLevel,		KD_ITEM_NO_LEVEL_LIMIT },		\
	{ emINT,	_KD_TABTITLE_OTH_CDTYPE,	(LPSTR)&(p)->nCDType,		0 },							\
	{ emSTR,	_KD_TABTITLE_OTH_FORBIDTYPE,(LPSTR)&(p)->szForbidType,	sizeof((p)->szForbidType)}

#define _KD_BPT_ST_ATTRIB(p, t, n)																						\
	{ emSTR,	_KD_TABTITLE_ST_ATTRNAME(n),	(t)[(n) - 1],								sizeof((t)[(n) - 1]) },		\
	{ emINT,	_KD_TABTITLE_ST_PARAM(n),	(LPSTR)&(p)->aryAttrib[(n) - 1].aryParam,		0 }

#define  _KD_STONE_DATA(p, t)																						\
	_KD_ITEM_BASEDATA(p),																							\
	{ emINT,	_KD_TABTITLE_STONE_COLOR,			(LPSTR)&(p)->nColor,			0 },							\
	{ emINT,	_KD_TABTITLE_LEVEL,					(LPSTR)&(p)->nLevel,			0 },							\
	{ emINT,	_KD_TABTITLE_STONE_STACK,			(LPSTR)&(p)->nStackMax,			KD_ITEM_MAX_STACK_COUNT },		\
	{ emINT,	_KD_TABTITLE_STONE_REQLEVEL,		(LPSTR)&(p)->nReqLevel,			KD_ITEM_NO_LEVEL_LIMIT },		\
	{ emINT,	_KD_TABTITLE_STONE_EQUIPLEVEL,		(LPSTR)&(p)->nEquipLevel,		KD_ITEM_NO_LEVEL_LIMIT },		\
	{ emINT,	_KD_TABTITLE_STONE_TYPE,			(LPSTR)&(p)->nStoneType,		0 },							\
	_KD_BPT_ST_ATTRIB(p, t, 1),																						\
	_KD_BPT_ST_ATTRIB(p, t, 2),																						\
	_KD_BPT_ST_ATTRIB(p, t, 3)

// 装备属性表的类型变量表
#define _KD_EQUIP_BASEDATA(p, t0)																\
	_KD_ITEM_BASEDATA(p),																				\
	{ emINT,	_KD_TABTITLE_LEVEL,				(LPSTR)&(p)->nLevel,			0					},	\
	{ emINT,	_KD_TABTITLE_EQ_PANDENT(1),		(LPSTR)&(p)->nPandent1,			0					},	\
	{ emINT,	_KD_TABTITLE_EQ_PANDENT(2),		(LPSTR)&(p)->nPandent2,			0					},	\
	{ emINT,	_KD_TABTITLE_BG_REQLEVEL,		(LPSTR)&(p)->nReqLevel,			0					},	\
	{ emINT,	_KD_TABTITLE_EQ_CATEGORY,		(LPSTR)&(p)->nCategory,			0					},	\
	{ emINT,	_KD_TABTITLE_EQ_SLOTMIN,		(LPSTR)&(p)->arySlot.nMin,		0					},	\
	{ emINT,	_KD_TABTITLE_EQ_SLOTMAX,		(LPSTR)&(p)->arySlot.nMax,		0					},	\
	{ emINT,	_KD_TABTITLE_EQ_EX_PANDENT(1),		(LPSTR)&(p)->aryExPandents[0],			0		},	\
	{ emINT,	_KD_TABTITLE_EQ_EX_PANDENT(2),		(LPSTR)&(p)->aryExPandents[1],			0		},	\
	{ emINT,	_KD_TABTITLE_EQ_EX_PANDENT(3),		(LPSTR)&(p)->aryExPandents[2],			0		},	\
	_KD_BPT_EQ_BASEATTR(p, t0, 1),																		\
	_KD_BPT_EQ_NEEDITEM(p, 1),																			\
	_KD_BPT_EQ_NEEDITEM(p, 2),																			\
	_KD_BPT_EQ_NEEDITEM(p, 3)

// 套装属性表的类型变量表
#define _KD_SUIT_BASEDATA(p, t0)																\
	_KD_ITEM_BASEDATA(p),																				\
	_KD_BPT_EQ_BASEATTR(p, t0, 1),																		\
	{ emINT,	_KD_TABTITLE_LEVEL,				(LPSTR)&(p)->nLevel,			0					},	\
	{ emINT,	_KD_TABTITLE_EQ_PANDENT(1),		(LPSTR)&(p)->nPandent1,			0					},	\
	{ emINT,	_KD_TABTITLE_EQ_PANDENT(2),		(LPSTR)&(p)->nPandent2,			0					},	\
	{ emINT,	_KD_TABTITLE_EQ_EX_PANDENT(1),		(LPSTR)&(p)->aryExPandents[0],			0		},	\
	{ emINT,	_KD_TABTITLE_EQ_EX_PANDENT(2),		(LPSTR)&(p)->aryExPandents[1],			0		},	\
	{ emINT,	_KD_TABTITLE_EQ_EX_PANDENT(3),		(LPSTR)&(p)->aryExPandents[2],			0		},	\
	{ emINT,	_KD_TABTITLE_EQ_EX_PANDENT(4),		(LPSTR)&(p)->aryExPandents[3],			0		},	\
	{ emINT,	_KD_TABTITLE_BG_REQLEVEL,		(LPSTR)&(p)->nReqLevel,			0					}

// 强化装备
#define _KD_STR_EQUIP(p, t0, t1, t2, t3)																	\
	_KD_EQUIP_BASEDATA(p, t0, t1, t3),																		\
	_KD_BPT_EQ_STRMA(p, t2, 1),																			\
	_KD_BPT_EQ_STRMA(p, t2, 2)

// 紫装属性表的类型变量表
#define _KD_PURPLEEQUIP_DATA(p, t0, t1, t2, t3, t4)															\
	_KD_STR_EQUIP(p, t0, t1, t3, t4),																		\
	_KD_BPT_PE_RANDMA(p, t2, 1),																		\
	_KD_BPT_PE_RANDMA(p, t2, 2),																		\
	_KD_BPT_PE_RANDMA(p, t2, 3),																		\
	_KD_BPT_PE_RANDMA(p, t2, 4),																		\
	_KD_BPT_PE_RANDMA(p, t2, 5),																		\
	_KD_BPT_PE_RANDMA(p, t2, 6)

// 定制套装属性表的类型变量表
#define  _KD_GREENSUITE_DATA(p, t0, t1, t2, t3, t4)															\
	_KD_STR_EQUIP(p, t0, t1, t3, t4),																		\
	_KD_BPT_PE_RANDMA(p, t2, 1),																		\
	_KD_BPT_PE_RANDMA(p, t2, 2),																		\
	_KD_BPT_PE_RANDMA(p, t2, 3),																		\
	_KD_BPT_PE_RANDMA(p, t2, 4),																		\
	_KD_BPT_PE_RANDMA(p, t2, 5),																		\
	_KD_BPT_PE_RANDMA(p, t2, 6),																		\
	{ emINT,	_KD_TABTITLE_SE_SUITEID,	(LPSTR)&((p)->nSuiteID),		KD_ITEM_INVALID_SUITEID }


// 黄金套装属性表的类型变量表
#define _KD_SUITE_DATA(p, t0, t1, t2, t3, t4)																\
	_KD_STR_EQUIP(p, t0, t1, t3, t4),																		\
	_KD_BPT_SE_RANDMA(p, t2, 1),																		\
	_KD_BPT_SE_RANDMA(p, t2, 2),																		\
	_KD_BPT_SE_RANDMA(p, t2, 3),																		\
	_KD_BPT_SE_RANDMA(p, t2, 4),																		\
	_KD_BPT_SE_RANDMA(p, t2, 5),																		\
	_KD_BPT_SE_RANDMA(p, t2, 6),																		\
	{ emINT,	_KD_TABTITLE_SE_SUITEID,	(LPSTR)&((p)->nSuiteID),		KD_ITEM_INVALID_SUITEID }

// 物品属性表的魔法属性变量表
#define _KD_MAGICATTRIB_DATA(p, s)																		\
	{ emSTR,	_KD_TABTITLE_MA_SUFFIX,		(p)->szSuffix,					sizeof((p)->szSuffix) },	\
	{ emINT,	_KD_TABTITLE_MA_LEVEL,		(LPSTR)&(p)->nLevel,			KD_ITEM_NO_LEVEL_LIMIT },	\
	{ emINT,	_KD_TABTITLE_MA_VALUE,		(LPSTR)&(p)->nValue,			0 },						\
	{ emSTR,	_KD_TABTITLE_MA_MAGICNAME,	(s),							sizeof(s) },				\
	{ emINT,	_KD_TABTITLE_MA_MA1MIN,		(LPSTR)&(p)->sMA.aryRange[0].nMin,	-1 },					\
	{ emINT,	_KD_TABTITLE_MA_MA1MAX,		(LPSTR)&(p)->sMA.aryRange[0].nMax,	-1 },					\
	{ emINT,	_KD_TABTITLE_MA_MA2MIN,		(LPSTR)&(p)->sMA.aryRange[1].nMin,	-1 },					\
	{ emINT,	_KD_TABTITLE_MA_MA2MAX,		(LPSTR)&(p)->sMA.aryRange[1].nMax,	-1 },					\
	{ emINT,	_KD_TABTITLE_MA_MA3MIN,		(LPSTR)&(p)->sMA.aryRange[2].nMin,	-1 },					\
	{ emINT,	_KD_TABTITLE_MA_MA3MAX,		(LPSTR)&(p)->sMA.aryRange[2].nMax,	-1 }

// 物品魔法属性激活规则表
#define  _KD_MAGICACTIVERULE_DATA(p)																	\
	{ emINT,	_KD_TABTITLE_MR_RULEID,			(LPSTR)&(p)->nRuleId,			 0 },					\
	{ emINT,	_KD_TABTITLE_MR_SKILLID,		(LPSTR)&(p)->nSkillId,			 0 },					\
	{ emINT,	_KD_TABTITLE_MR_BEGINLEVEl,		(LPSTR)&(p)->nBeginLevel,		 0 },					\
	{ emINT,	_KD_TABTITLE_MR_ENDLEVEL,		(LPSTR)&(p)->nEndLevel,			 0 },					\
	{ emSTR,	_KD_TABTITLE_MR_DESCRIPTION,	(p)->szDesc,			sizeof((p)->szDesc)}

//------------------------------------------------------------------------

enum KE_VARTYPE
{
	emINT,
	emSTR,
};

// 以下定义的结构用于辅助从tabfile中读出属性的初始值
struct PROPINFO
{
	INT			nType;				// 属性的类型. 详见 PI_VARTYPE_...系列定义
	PCSTR		pszTitle;			// 属性的标题（TAB文件第一行中给出）
	//union
	//{
	//	LPSTR	pszBuf;				// 指向字符串缓冲区的指针
	//	PINT	pnData;				// 指向INT变量的指针
	//};
	PVOID		pData; // 指向字符串缓冲区 或 INT变量
	INT			nRemark;			// 若m_pData使用字符串则表示缓冲区的长度，若使用INT则表示默认值
};

typedef CHAR KMAGICATTRIB_NAME[SZBUFLEN_0];		// 存放魔法属性名的临时字符串

//------------------------------------------------------------------------
// 全局函数，从tab file中读入特定的数据记录

static VOID LoadRecord(IN QTabFile* pTF, IN INT nRow, IN OUT CONST PROPINFO *pPI, IN INT cbFields)
{
	nRow += 2;	// 加1: 跳过tabfile的第一行. 该行给出的是各列的名称
				// 再加1: QTabFile::GetInteger()函数要求nRow从1开始算起
	// 逐个读入各项属性
	for (INT n = 0; n < cbFields; n++)
	{
		CONST PROPINFO *pCurPI = pPI + n;

		switch (pCurPI->nType)
		{
			// TODO: xyf	临时去掉了VERIFY。需要确保配置文件正确填写后恢复。
		case emINT:		// 读入 INT 型数据
			pTF->GetInteger(nRow, pCurPI->pszTitle, pCurPI->nRemark, (PINT)pCurPI->pData);
			break;
		case emSTR:		// 读入字符串型数据 (for server & client)
			pTF->GetString(nRow, pCurPI->pszTitle, "", (LPSTR)pCurPI->pData, pCurPI->nRemark);
// 			if (n == 1 && (pCurPI->pszBuf[0] >= '0' || pCurPI->pszBuf[0] <= '9'))
// 				ASSERT(pCurPI->pszBuf[1] < '0' || pCurPI->pszBuf[1] > '9');
			break;
		default:
			ASSERT(FALSE);
		}
	}
}

//------------------------------------------------------------------------
// KBasicPropertyTable

KBasicPropertyTable::KBasicPropertyTable()
{
	m_pBuf			= NULL;
	m_nNumOfEntries	= 0;
	m_nSizeOfEntry	= 0;
	m_szTabFile[0]	= 0;
}

KBasicPropertyTable::~KBasicPropertyTable()
{
	ReleaseMemory();
}

VOID KBasicPropertyTable::SetCount(INT cbCount)
{
	ASSERT(cbCount > 0);
	ASSERT(0 == m_nNumOfEntries);	// this function is supposed to be called only once
	m_nNumOfEntries = cbCount;
}

VOID KBasicPropertyTable::GetMemory()
{
	ASSERT(!m_pBuf);
	ASSERT(m_nNumOfEntries > 0 && m_nSizeOfEntry > 0);
	CONST INT nMemSize = m_nSizeOfEntry * m_nNumOfEntries;
	m_pBuf = new BYTE[nMemSize];
	::memset(m_pBuf, 0, nMemSize);
}

VOID KBasicPropertyTable::ReleaseMemory()
{
	delete [] m_pBuf;
	m_pBuf = NULL;
	m_nNumOfEntries = 0;
}

BOOL KBasicPropertyTable::Load(INT nVersion)
{
	QTabFile theLoader;

	//::g_SetRootPath(NULL);

	CHAR szFileName[MAX_PATH];
    CHAR szTABFilePath[MAX_PATH];

	//::sprintf(szTABFilePath, _KD_TABFILE_PATH"\\%03d", nVersion);
	::sprintf(szTABFilePath, _KD_TABFILE_PATH);
	::g_UnitePathAndName(szTABFilePath, m_szTabFile, szFileName);
	::strcat(szFileName, _KD_TABFILE_SUFFIX);

	if (!theLoader.Load(szFileName))
	{
		ASSERT(FALSE);
		return	FALSE;
	}

	//theLoader.SetErrorLog(FALSE);

	// 确定file内给出了多少项记录
	CONST INT cbItems = theLoader.GetHeight() - 1;	// 第一行给出各列名称,
	if (cbItems <= 0)			// 表格是空的，忽略
		return	TRUE;
	SetCount(cbItems);

	GetMemory();				// 分配内存，构建属性表

	// 将属性记录逐条读入
	for (INT i = 0; i < cbItems; i++)
		LoadRecord(i, &theLoader);

	return TRUE;
}

//------------------------------------------------------------------------

VOID _BPT_GeneralEquip_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_GENERALEQUIP* pBufBase, 
								  KBPT_BaseT<KITEM_INDEX,KBASICPROP_GENERALEQUIP>::KINDEX_MAP &mapIndex)
{
	KBASICPROP_GENERALEQUIP* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aszBaseMAName[emITEM_COUNT_BASE];
	//KMAGICATTRIB_NAME aszDisguiseMAName[emITEM_COUNT_ACITVEMA];
	CONST PROPINFO aryPI[] = { _KD_EQUIP_BASEDATA(pBuf, aszBaseMAName) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	for (INT j = 0; j < emITEM_COUNT_BASE; j++)
		pBuf->aryBasic[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszBaseMAName[j]);
	

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	if(mapIndex.count(sIndex))
	{
		std::cout << "gdp: " << pBuf->nItemGenre << " " << pBuf->nDetailType << " " << pBuf->nParticular << std::endl;
	}
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Suit_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_SUIT* pBufBase, 
								  KBPT_BaseT<KITEM_INDEX,KBASICPROP_SUIT>::KINDEX_MAP &mapIndex)
{
	KBASICPROP_SUIT* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aszBaseMAName[emITEM_COUNT_BASE];
	CONST PROPINFO aryPI[] = { _KD_SUIT_BASEDATA(pBuf, aszBaseMAName) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	for (INT j = 0; j < emITEM_COUNT_BASE; j++)
		pBuf->aryBasic[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszBaseMAName[j]);

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	if(mapIndex.count(sIndex))
	{
		std::cout << "gdp: " << pBuf->nItemGenre << " " << pBuf->nDetailType << " " << pBuf->nParticular << std::endl;
	}
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_GE_Common_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_GENERALEQUIP* pBufBase, KBPT_BaseT<KITEM_INDEX, KBASICPROP_GENERALEQUIP>::KINDEX_MAP &mapIndex)
{
	/*
	KBASICPROP_GENERALEQUIP* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aszBaseMAName[emITEM_COUNT_BASE];
	KMAGICATTRIB_NAME aszEnhMAName[emITEM_COUNT_ENHANCE];
	KMAGICATTRIB_NAME aszStrMAName[emITEM_COUNT_STRENGTHEN];
	KMAGICATTRIB_NAME aszDisguiseMAName[emITEM_COUNT_ACITVEMA];
	CONST PROPINFO aryPI[] =
	{
		_KD_STR_EQUIP(pBuf, aszBaseMAName, aszEnhMAName, aszStrMAName, aszDisguiseMAName),
		{ emINT,	_KD_TABTITLE_GC_STUFFDETAIL(1),		(LPSTR)&pBuf->aryExtParam[0],	0 },
		{ emINT,	_KD_TABTITLE_GC_STUFFPARTICULAR(1),	(LPSTR)&pBuf->aryExtParam[1],	0 },
		{ emINT,	_KD_TABTITLE_GC_STUFFDETAIL(2),		(LPSTR)&pBuf->aryExtParam[2],	0 },
		{ emINT,	_KD_TABTITLE_GC_STUFFPARTICULAR(2),	(LPSTR)&pBuf->aryExtParam[3],	0 },
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	for (INT j = 0; j < emITEM_COUNT_BASE; j++)
		pBuf->aryBasic[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszBaseMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_ENHANCE; j++)
		pBuf->aryEnh[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszEnhMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_STRENGTHEN; j++)
		pBuf->aryStr[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszStrMAName[j]);

	for (INT j = 0; j < emITEM_COUNT_ACITVEMA; j++)
	{
		if (aszDisguiseMAName[j] && aszDisguiseMAName[0])
			pBuf->aryActv[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszDisguiseMAName[j]);
	}

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
	*/
}

VOID _BPT_PurpleEquip_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_PURPLEEQUIP* pBufBase, KBPT_BaseT<KITEM_INDEX, KBASICPROP_PURPLEEQUIP>::KINDEX_MAP &mapIndex)
{
	/*
	KBASICPROP_PURPLEEQUIP* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aszBaseMAName[emITEM_COUNT_BASE];
	KMAGICATTRIB_NAME aszRandMAName[emITEM_COUNT_RANDOM];
	KMAGICATTRIB_NAME aszEnhMAName[emITEM_COUNT_ENHANCE];
	KMAGICATTRIB_NAME aszStrMAName[emITEM_COUNT_STRENGTHEN];
	KMAGICATTRIB_NAME aszDisguiseMAName[emITEM_COUNT_ACITVEMA];
	CONST PROPINFO aryPI[] = { _KD_PURPLEEQUIP_DATA(pBuf, aszBaseMAName, aszEnhMAName, aszRandMAName, aszStrMAName, aszDisguiseMAName) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	for (INT j = 0; j < emITEM_COUNT_BASE; j++)
		pBuf->aryBasic[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszBaseMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_RANDOM; j++)
		pBuf->aryRand[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszRandMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_ENHANCE; j++)
		pBuf->aryEnh[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszEnhMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_STRENGTHEN; j++)
		pBuf->aryStr[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszStrMAName[j]);

	for (INT j = 0; j < emITEM_COUNT_ACITVEMA; j++)
	{
		if (aszDisguiseMAName[j] && aszDisguiseMAName[0])
			pBuf->aryActv[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszDisguiseMAName[j]);
	}

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
	*/
}

VOID _BPT_GreenSuite_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_GREENSUITE* pBufBase, KBPT_BaseT<KITEM_INDEX, KBASICPROP_GREENSUITE>::KINDEX_MAP &mapIndex)
{
	/*
	KBASICPROP_GREENSUITE* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aszBaseMAName[emITEM_COUNT_BASE];
	KMAGICATTRIB_NAME aszRandMAName[emITEM_COUNT_RANDOM];
	KMAGICATTRIB_NAME aszEnhMAName[emITEM_COUNT_ENHANCE];
	KMAGICATTRIB_NAME aszStrMAName[emITEM_COUNT_STRENGTHEN];
	KMAGICATTRIB_NAME aszDisguiseMAName[emITEM_COUNT_ACITVEMA];
	CONST PROPINFO aryPI[] = { _KD_GREENSUITE_DATA(pBuf, aszBaseMAName, aszEnhMAName, aszRandMAName, aszStrMAName, aszDisguiseMAName) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	for (INT j = 0; j < emITEM_COUNT_BASE; j++)
		pBuf->aryBasic[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszBaseMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_RANDOM; j++)
		pBuf->aryRand[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszRandMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_ENHANCE; j++)
		pBuf->aryEnh[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszEnhMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_STRENGTHEN; j++)
		pBuf->aryStr[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszStrMAName[j]);

	for (INT j = 0; j < emITEM_COUNT_ACITVEMA; j++)
	{
		if (aszDisguiseMAName[j] && aszDisguiseMAName[0])
			pBuf->aryActv[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszDisguiseMAName[j]);
	}

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
	*/
};

VOID _BPT_SuiteEquip_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_EQUIP_SUITE* pBufBase, KBPT_BaseT<KITEM_INDEX, KBASICPROP_EQUIP_SUITE>::KINDEX_MAP &mapIndex)
{
	/*
	KBASICPROP_EQUIP_SUITE* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aszBaseMAName[emITEM_COUNT_BASE];
	KMAGICATTRIB_NAME aszRandMAName[emITEM_COUNT_RANDOM];
	KMAGICATTRIB_NAME aszEnhMAName[emITEM_COUNT_ENHANCE];
	KMAGICATTRIB_NAME aszStrMAName[emITEM_COUNT_STRENGTHEN];
	KMAGICATTRIB_NAME aszDisguiseMAName[emITEM_COUNT_ACITVEMA];
	CONST PROPINFO aryPI[] = { _KD_SUITE_DATA(pBuf, aszBaseMAName, aszEnhMAName, aszRandMAName, aszStrMAName, aszDisguiseMAName) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	for (INT j = 0; j < emITEM_COUNT_BASE; j++)
		pBuf->aryBasic[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszBaseMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_RANDOM; j++)
		pBuf->aryRand[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszRandMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_ENHANCE; j++)
		pBuf->aryEnh[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszEnhMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_STRENGTHEN; j++)
		pBuf->aryStr[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszStrMAName[j]);
	for (INT j = 0; j < emITEM_COUNT_STRENGTHEN; j++)
		pBuf->aryStr[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszStrMAName[j]);

	for (INT j = 0; j < emITEM_COUNT_ACITVEMA; j++)
	{
		if (aszDisguiseMAName[j] && aszDisguiseMAName[0])
			pBuf->aryActv[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszDisguiseMAName[j]);
	}


	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
	*/
}

VOID _BPT_Medicine_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_MEDICINE* pBufBase, KBPT_Medicine::KINDEX_MAP &mapIndex)
{
	KBASICPROP_MEDICINE* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aryMAName[3];
	CONST PROPINFO aryPI[] =
	{
		_KD_OTHER_DATA(pBuf),
		{ emINT,	_KD_TABTITLE_MED_BZONE_CANUSE,	(LPSTR)&pBuf->nCanUseInBZone,	0 },	
		{ emSTR,	_KD_TABTITLE_MED_MAGIC1,	aryMAName[0],						sizeof(aryMAName[0]) },
		{ emINT,	_KD_TABTITLE_MED_VALUE1,	(LPSTR)&pBuf->aryAttrib[0].nValue,	-1 },
		{ emINT,	_KD_TABTITLE_MED_TIME1,		(LPSTR)&pBuf->aryAttrib[0].nTime,	-1 },
		{ emSTR,	_KD_TABTITLE_MED_MAGIC2,	aryMAName[1],						sizeof(aryMAName[1]) },
		{ emINT,	_KD_TABTITLE_MED_VALUE2,	(LPSTR)&pBuf->aryAttrib[1].nValue,	-1 },
		{ emINT,	_KD_TABTITLE_MED_TIME2,		(LPSTR)&pBuf->aryAttrib[1].nTime,	-1 },
		{ emSTR,	_KD_TABTITLE_MED_MAGIC3,	aryMAName[2],						sizeof(aryMAName[2]) },
		{ emINT,	_KD_TABTITLE_MED_VALUE3,	(LPSTR)&pBuf->aryAttrib[2].nValue,	-1 },
		{ emINT,	_KD_TABTITLE_MED_TIME3,		(LPSTR)&pBuf->aryAttrib[2].nTime,	-1 },
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	pBuf->aryAttrib[0].nAttrib = g_cMagicAttribMgr.GetIdFormString(aryMAName[0]);
	pBuf->aryAttrib[1].nAttrib = g_cMagicAttribMgr.GetIdFormString(aryMAName[1]);
	pBuf->aryAttrib[2].nAttrib = g_cMagicAttribMgr.GetIdFormString(aryMAName[2]);

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Stone_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_STONE* pBufBase, KBPT_Stone::KINDEX_MAP &mapIndex)
{
	KBASICPROP_STONE* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aryParamName[3];
	CONST PROPINFO aryPI[] =
	{
		_KD_STONE_DATA(pBuf, aryParamName),
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	pBuf->aryAttrib[0].nAttrib = g_cMagicAttribMgr.GetIdFormString(aryParamName[0]);
	pBuf->aryAttrib[1].nAttrib = g_cMagicAttribMgr.GetIdFormString(aryParamName[1]);
	pBuf->aryAttrib[2].nAttrib = g_cMagicAttribMgr.GetIdFormString(aryParamName[2]);

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Script_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_SCRIPT* pBufBase, KBPT_Script_Item::KINDEX_MAP &mapIndex)
{
	KBASICPROP_SCRIPT* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] =
	{
		_KD_OTHER_DATA(pBuf),
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(1),	(LPSTR)&pBuf->aryExtParam[0],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(2),	(LPSTR)&pBuf->aryExtParam[1],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(3),	(LPSTR)&pBuf->aryExtParam[2],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(4),	(LPSTR)&pBuf->aryExtParam[3],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(5),	(LPSTR)&pBuf->aryExtParam[4],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(6),	(LPSTR)&pBuf->aryExtParam[5],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(7),	(LPSTR)&pBuf->aryExtParam[6],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(8),	(LPSTR)&pBuf->aryExtParam[7],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(9),	(LPSTR)&pBuf->aryExtParam[8],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(10),	(LPSTR)&pBuf->aryExtParam[9],	0 },
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	printf("%d %d %d %d",pBuf->nItemGenre,pBuf->nDetailType,pBuf->nParticular,pBuf->nLevel);
	ASSERT(!mapIndex.count(sIndex) && "配置表项有重复，道具ID冲突，请检查配置表");		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Ride_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_SCRIPT* pBufBase, KBPT_Script_Item::KINDEX_MAP &mapIndex)
{

	KBASICPROP_SCRIPT* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] =
	{
		_KD_OTHER_DATA(pBuf),
		{ emINT,	_KD_TABTITLE_LEVEL,				&pBuf->nLevel,					0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(1),	(LPSTR)&pBuf->aryExtParam[0],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(2),	(LPSTR)&pBuf->aryExtParam[1],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(3),	(LPSTR)&pBuf->aryExtParam[2],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(4),	(LPSTR)&pBuf->aryExtParam[3],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(5),	(LPSTR)&pBuf->aryExtParam[4],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(6),	(LPSTR)&pBuf->aryExtParam[5],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(7),	(LPSTR)&pBuf->aryExtParam[6],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(8),	(LPSTR)&pBuf->aryExtParam[7],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(9),	(LPSTR)&pBuf->aryExtParam[8],	0 },
		{ emINT,	_KD_TABTITLE_SS_EXTPARAM(10),	(LPSTR)&pBuf->aryExtParam[9],	0 },
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Skill_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_SKILL* pBufBase, KBPT_Skill_Item::KINDEX_MAP &mapIndex)
{
	KBASICPROP_SKILL* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] =
	{
		_KD_OTHER_DATA(pBuf),
		{ emINT,	_KD_TABTITLE_SK_SKILL,			(LPSTR)&pBuf->aryExtParam[0],	0 },		// 调用技能ID
		{ emINT,	_KD_TABTITLE_SS_SKILLLEVEL,		(LPSTR)&pBuf->aryExtParam[1],	1 },		// 调用技能级别
		{ emINT,	_KD_TABTITLE_SS_CASTTYPE,		(LPSTR)&pBuf->aryExtParam[2],	0 },		// 放技能类型
		{ emINT,	_KD_TABTITLE_SS_BROADCAST,		(LPSTR)&pBuf->aryExtParam[3],	FALSE },	// 是否广播
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Quest_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_QUEST* pBufBase, KBPT_Task_Quest::KINDEX_MAP &mapIndex)
{
	KBASICPROP_QUEST* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] = { _KD_OTHER_DATA(pBuf) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_ExtBag_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_EXTBAG* pBufBase, KBPT_Extend_Bag::KINDEX_MAP &mapIndex)
{
	KBASICPROP_EXTBAG* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] = 
	{ 
		_KD_OTHER_DATA(pBuf),
		{ emINT,	_KD_TABTITLE_BG_POS, (LPSTR)&pBuf->nPos, -1},			// 背包摆放位置限制
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Stuff_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_STUFF* pBufBase, KBPT_Stuff_Item::KINDEX_MAP &mapIndex)
{
	KBASICPROP_STUFF* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] = { _KD_OTHER_DATA(pBuf) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_Plan_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_PLAN* pBufBase, KBPT_Plan_Item::KINDEX_MAP &mapIndex)
{
	KBASICPROP_PLAN* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] =
	{
		_KD_OTHER_DATA(pBuf),
		{ emINT,	_KD_TABTITLE_PLAN_ID,	(LPSTR)&pBuf->aryExtParam[0],	0 },
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	KITEM_INDEX sIndex = { pBuf->nItemGenre, pBuf->nDetailType, pBuf->nParticular, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_MagicAttrib_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_MAGICATTRIB* pBufBase, KBPT_MagicAttrib::KINDEX_MAP &mapIndex)
{
	KBASICPROP_MAGICATTRIB* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME szMAName;
	CONST PROPINFO aryPI[] =
	{
		_KD_MAGICATTRIB_DATA(pBuf, szMAName),
		//{ emINT,	_KD_TABTITLE_MA_DARKNESS,		(LPSTR)&pBuf->bDarkness,							FALSE },
		//{ emINT,	_KD_TABTITLE_MA_SERIES,			(LPSTR)&pBuf->nSeries,								KD_ITEM_NO_SERIES_LIMIT },
		//{ emINT,	_KD_TABTITLE_MA_MELEEWEAPON,	(LPSTR)&pBuf->aryDropRate[equip_meleeweapon - 1],	0 },
		//{ emINT,	_KD_TABTITLE_MA_RANGEWEAPON,	(LPSTR)&pBuf->aryDropRate[equip_rangeweapon - 1],	0 },
		//{ emINT,	_KD_TABTITLE_MA_ARMOR,			(LPSTR)&pBuf->aryDropRate[equip_armor - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_RING,			(LPSTR)&pBuf->aryDropRate[equip_ring - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_NECKLACE,		(LPSTR)&pBuf->aryDropRate[equip_necklace - 1],		0 },
		//{ emINT,	_KD_TABTITLE_MA_AMULET,			(LPSTR)&pBuf->aryDropRate[equip_amulet - 1],		0 },
		//{ emINT,	_KD_TABTITLE_MA_BOOTS,			(LPSTR)&pBuf->aryDropRate[equip_boots - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_BELT,			(LPSTR)&pBuf->aryDropRate[equip_belt - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_HELM,			(LPSTR)&pBuf->aryDropRate[equip_helm - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_CUFF,			(LPSTR)&pBuf->aryDropRate[equip_cuff - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_PENDANT,		(LPSTR)&pBuf->aryDropRate[equip_pendant - 1],		0 },
		//{ emINT,	_KD_TABTITLE_MA_HORSE,			(LPSTR)&pBuf->aryDropRate[equip_horse - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_MASK,			(LPSTR)&pBuf->aryDropRate[equip_mask - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_BOOK,			(LPSTR)&pBuf->aryDropRate[equip_book - 1],			0 },
		//{ emINT,	_KD_TABTITLE_MA_PARTNERWEAPON,	(LPSTR)&pBuf->aryDropRate[equip_partner_weapon - 1],0 },
		//{ emINT,	_KD_TABTITLE_MA_PARTNERBODY,	(LPSTR)&pBuf->aryDropRate[equip_partner_body - 1],	0 },
		//{ emINT,	_KD_TABTITLE_MA_PARTNERRING,	(LPSTR)&pBuf->aryDropRate[equip_partner_ring - 1],	0 },
		//{ emINT,	_KD_TABTITLE_MA_PARTNERCUFF,	(LPSTR)&pBuf->aryDropRate[equip_partner_cuff - 1],	0 },
		//{ emINT,	_KD_TABTITLE_MA_PARTNERAMULET,	(LPSTR)&pBuf->aryDropRate[equip_partner_amulet - 1],0 },
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	pBuf->sMA.nAttrib = g_cMagicAttribMgr.GetIdFormString(szMAName);
	ASSERT(pBuf->sMA.nAttrib > KD_MAGIC_INVALID);

	KMAGIC_INDEX sIndex = { pBuf->sMA.nAttrib, pBuf->nLevel };
	ASSERT(!mapIndex.count(sIndex));		// 配置表项有重复，魔法属性索引冲突，请检查配置表
	mapIndex[sIndex] = pBuf;
}

VOID _BPT_MagicActiveRule_LoadReord(INT i, QTabFile* pTF, KBASICPROP_MAGICACTIVERULE* pBufBase, KBPT_BaseT<INT, KBASICPROP_MAGICACTIVERULE>::KINDEX_MAP& mapIndex)
{
	KBASICPROP_MAGICACTIVERULE* pBuf = pBufBase + i;
	CONST PROPINFO aryPI[] = { _KD_MAGICACTIVERULE_DATA(pBuf) };

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	INT nIndex = pBuf->nRuleId;
	ASSERT(!mapIndex.count(nIndex));
	mapIndex[nIndex] = pBuf;	
}

VOID _BPT_SuiteInfo_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_SUITEINFO* pBufBase, KBPT_BaseT<INT, KBASICPROP_SUITEINFO>::KINDEX_MAP &mapIndex)
{
	KBASICPROP_SUITEINFO* pBuf = pBufBase + i;
	KMAGICATTRIB_NAME aszSuiteMAName[emITEM_COUNT_SUITE];
	CONST PROPINFO aryPI[] =
	{
		{ emINT,	_KD_TABTITLE_SI_SUITEID,	(LPSTR)&pBuf->nSuiteID,				0 },
		_KD_BPT_EQ_SUITEMA(pBuf, aszSuiteMAName, 1),
		_KD_BPT_EQ_SUITEMA(pBuf, aszSuiteMAName, 2),
		_KD_BPT_EQ_SUITEMA(pBuf, aszSuiteMAName, 3),
		_KD_BPT_EQ_SUITEMA(pBuf, aszSuiteMAName, 4),
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	for (INT j = 0; j < emITEM_COUNT_SUITE; j++)
		pBuf->arySuite[j].nAttrib = g_cMagicAttribMgr.GetIdFormString(aszSuiteMAName[j]);

	INT nIndex = pBuf->nSuiteID;
	ASSERT(!mapIndex[nIndex]);		// 配置表项有重复，套装ID冲突，请检查配置表
	mapIndex[nIndex] = pBuf;
}

VOID _BPT_EquipEffect_LoadRecord(INT i, QTabFile* pTF, KBASICPROP_EQUIPEFFECT* pBufBase, KBPT_EquipEffect::KINDEX_MAP &mapIndex)
{
	KBASICPROP_EQUIPEFFECT* pBuf = pBufBase + i;
	INT nEquipEffectRes = 0;
	CONST PROPINFO aryPI[] =
	{
		{ emINT,	_KD_TABTITLE_EE_CATEGORY,		(LPSTR)&pBuf->nCategory,	0 },
		{ emINT,	_KD_TABTITLE_EE_EFFECTRES,		(LPSTR)&nEquipEffectRes,	0 },
	};

	::LoadRecord(pTF, i, aryPI, countof(aryPI));

	if (!mapIndex[pBuf->nCategory])
		mapIndex[pBuf->nCategory] = pBuf;

	ASSERT(!mapIndex[pBuf->nCategory]->anEquipEffectRes[0]);		// 配置表项有重复，道具ID冲突，请检查配置表
	mapIndex[pBuf->nCategory]->anEquipEffectRes[0] = nEquipEffectRes;
}

//------------------------------------------------------------------------
