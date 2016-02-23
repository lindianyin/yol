
#ifndef __KITEMSETTING_H__
#define __KITEMSETTING_H__

// -------------------------------------------------------------------------

class KItemSetting
{
	// 类型定义
public:
	typedef std::map<INT, DWORD> KCdTypeMap;
	typedef std::map<INT, INT> KMAP_EQUIP_SCORE;
	typedef std::map<INT, INT> KMAP_STONE_SCORE;


	// 接口方法实现
public:
	BOOL	Init();

	INT		GetAttribScore(INT nAttrib);
	INT		GetStoneAttribScore(INT nAttrib);

private:
	BOOL	InitCdType();
	BOOL	InitEquipScore();
	BOOL	InitStoneScore();


	// 成员数据定义
public:
	KCdTypeMap		m_mapCdType;		// Cd时间类型MAP
	KMAP_EQUIP_SCORE m_mapEquipScore;	// 装备属性战斗力
	KMAP_STONE_SCORE m_mapStoneScore;	// 宝石属性战斗力
};

// -------------------------------------------------------------------------

#endif /* __KITEMSETTING_H__ */
