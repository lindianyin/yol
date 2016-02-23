/* -------------------------------------------------------------------------
//	文件名		：	kpetsetting.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-2-25
//	功能描述	：	宠物配置读取
// -----------------------------------------------------------------------*/

#ifndef kpetsetting_h__
#define kpetsetting_h__

#include "petdef.h"
#include "onlinegamemodule/item2/itemdef.h"

class ITabFile;

struct KPET_MAGIC_ATTR
{
	INT nAttrib;
	INT nValue;
};

typedef std::map<INT, INT> KMAP_PET_ATTR_SCORE;
typedef std::map<INT, INT> KMAP_RIDE_ATTR_SCORE;

// Pet模板
struct KPetTemplate
{
	BOOL Init(ITabFile* piFile, INT nRow);

	INT		m_nType;									// 类型ID，对应Npc表里的NpcId
	CHAR	m_szName[MAX_NAME_LEN];						// 名字									
	INT		m_nHungry;									// 战斗时食物消耗
	INT     nQuality;		 // 宠物品质
	INT		m_PetSkill1;
	INT     m_PetSkill2;
	INT     m_PetNpcId;				// 对应的静态NPCID
};

struct KPetSkillTemplate
{
	BOOL Init(ITabFile* piFile, INT nRow);

	INT		m_nTemplateId;
	CHAR	m_szSkillName[MAX_NAME_LEN];
	INT		m_nSkillCD;								// 技能CD时间
	INT     m_nSkillType;                           // 技能类型
	BOOL	m_bToSelf;
	INT		m_arySkillId[emPET_COUNT_SKILL];
	INT		m_aryBuffId[emPET_COUNT_SKILL];		    // 1到5级的buffid
};

struct KRideRankTemplate
{
	BOOL Init(ITabFile* piFile, INT nRow);

	INT m_nGrowId;

	KPET_MAGIC_ATTR m_aryRankAttr[emRIDE_COUNT_BASE];	// 基本属性
	KPET_MAGIC_ATTR m_aryRideAttr[emRIDE_COUNT_RIDE];	// 骑乘属性
};

// 坐骑模板
struct KRideTemplate
{
	BOOL Init(ITabFile* piFile, INT nRow);

	INT m_nGenre;
	INT m_nDetail;
	INT m_nParticular;
	INT m_nLevel;			// 坐骑的阶数

	CHAR m_szName[MAX_NAME_LEN];						// 名字

	INT m_nRepresentId;			// 表现ID
	INT m_nMaxLevel;		// 最大阶数
	INT m_nQuality;			// 品质
	INT m_nRankId;
	INT m_nRankConsume;		// 升到下一阶消耗的幻化果
};

class KPetSetting
{
	typedef std::map<INT, KPetTemplate*> PET_TEMPLATE_MAP;
	typedef std::map<INT, KPetSkillTemplate*> PET_SKILL_TEMPLATE_MAP;

	typedef std::map<KITEM_INDEX, KRideTemplate*> RIDE_TEMPLATE_MAP;
	typedef std::map<INT, KRideRankTemplate*> RIDE_RANK_TEMPLATE_MAP;
	
public:
	KPetSetting();
	~KPetSetting();

	BOOL Init();
	BOOL UnInit();

	KPetTemplate* GetPetTemplate(INT nType);
	INT GetPetNpcIdByType(INT nType);  // 获取宠物NPD
	KPetSkillTemplate* GetPetSkillTemplate(INT nSkillId);

	INT GetBuffIdBySkillId(INT nSkillId);

	KRideTemplate* GetRideTemplate(KITEM_INDEX sIndex);
	KRideRankTemplate* GetRideRankTemplate(INT nGrowId);

	INT GetPetAttribScore(INT nAttrib);
	INT GetRideAttribScore(INT nAttrib);

private:
	PET_TEMPLATE_MAP m_mapPetTemplates;
	PET_SKILL_TEMPLATE_MAP m_mapPetSkillTemplates;

	RIDE_TEMPLATE_MAP m_mapRideTemplates;
	RIDE_RANK_TEMPLATE_MAP m_mapRideRankTemplates;

	KMAP_PET_ATTR_SCORE m_mapPetAttrScore;
	KMAP_RIDE_ATTR_SCORE m_mapRideAttrScore;
	
};

extern KPetSetting g_cPetSetting;

#endif // petsetting_h__
