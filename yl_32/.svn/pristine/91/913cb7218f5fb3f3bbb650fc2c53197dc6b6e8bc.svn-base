
#ifndef __HELPER_ITEM_KDUPEDITEMBASE_H__
#define __HELPER_ITEM_KDUPEDITEMBASE_H__


#include "kdupeditemdef.h"
#include <map>
// -------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
namespace DupedDef
{
	//template <class KItemT>
	//KItemT& GetItem(INT nIndex);
	QLogFile& GetDupedLogFile();

	LPVOID GetItemFromItemSet(INT nItemIdx);

	BOOL RemoveItemFromItemSet(INT nPlayerIdx, INT nItemIdx, BOOL bEraseInMap);
	BOOL RemoveItemFromPlayer(INT nPlayerIdx, INT nItemIdx);

	LPCSTR GetPlayerAccount(INT nPlayerIdx);
	LPCSTR GetPlayerName(INT nPlayerIdx);
	BOOL CheckPlayerTrading(INT nPlayerIdx);

	BOOL SendMsgForMark(INT nPlayerIndex, LPCSTR szItemName);
	BOOL SendMsgForDel(INT nPlayerIndex, LPCSTR szItemName);
};

///////////////////////////////////////////////////////////////////////////////
class KDupedItemSetting
{
private:
	DupedDef::OP_LEVEL		m_OpLevel;		// 对复制物品的操作级别
	BOOL					m_bDelAllInOne; // 是否删除同一人身上的相同复制装备
	DupedDef::FORBIT_LEVEL	m_ForbitLevel;
	
	QIniFile				m_cfgFile;

#ifndef SYSALARM_CONFIG_FILENAME
	#define SYSALARM_CONFIG_FILENAME	"\\alarm_config.ini"
#endif // #ifndef SYSALARM_CONFIG_FILENAME

public:
	KDupedItemSetting() 
		:	m_OpLevel(DupedDef::op_none),		// 默认是不作任何处理
			m_bDelAllInOne(FALSE),				// 默认不删除身上相同的物品
			m_ForbitLevel(DupedDef::forbit_none)// 默认不禁止任何行为
	{
		// Init();
	}
	~KDupedItemSetting()
	{
	}

	BOOL Init()
	{
		if (m_cfgFile.Load(SYSALARM_CONFIG_FILENAME))
		{
			INT nValue;
			// 默认是作标记
			m_cfgFile.GetInteger("DupedItem", "OpLevel", 
										DupedDef::op_mark, &nValue);
			if (nValue >= DupedDef::op_none && 
				nValue <= DupedDef::op_level_end)
			{
				SetOpLevel((DupedDef::OP_LEVEL)nValue);
			}
			// 默认不禁止任何行为
			m_cfgFile.GetInteger("DupedItem", "DelAllInOne", FALSE, &nValue);
			if (TRUE == nValue)
			{
				SetDelAllInOne(TRUE);
			}
			else
			{
				SetDelAllInOne(FALSE);
			}
			// 默认不删除身上相同的物品
			m_cfgFile.GetInteger("DupedItem", "ForbitLevel", 
										DupedDef::forbit_none, &nValue);
			if (nValue >= DupedDef::forbit_none && 
				nValue <= DupedDef::forbit_level_end)
			{
				SetForbitLevel((DupedDef::FORBIT_LEVEL)nValue);
			}
		}
		/*
		else
		{
			// 创建一个空的配置文件，莫名其妙
			FILE* fpFile = fopen(SYSALARM_CONFIG_FILENAME, "ab+");
			fclose(fpFile);
			m_cfgFile.Load(SYSALARM_CONFIG_FILENAME);

			m_cfgFile.WriteInteger("DupedItem", "OpLevel", GetOpLevel());
			m_cfgFile.WriteInteger("DupedItem", "DelAllInOne", GetDelAllInOne());
			m_cfgFile.WriteInteger("DupedItem", "ForbitLevel", GetForbitLevel());
		}
		*/

		return TRUE;
	}
public:
	DupedDef::OP_LEVEL GetOpLevel() { return m_OpLevel; }
	BOOL GetDelAllInOne() { return m_bDelAllInOne; }
	DupedDef::FORBIT_LEVEL GetForbitLevel() { return m_ForbitLevel; }
	
	VOID SetOpLevel(DupedDef::OP_LEVEL level)
	{
		m_OpLevel = level;
		m_cfgFile.WriteInteger("DupedItem", "OpLevel", m_OpLevel);
		//m_cfgFile.Save(SYSALARM_CONFIG_FILENAME);
	}

	VOID SetDelAllInOne(BOOL bDel)
	{
		m_bDelAllInOne = bDel;

		m_cfgFile.WriteInteger("DupedItem", "DelAllInOne", m_bDelAllInOne);
		//m_cfgFile.Save(SYSALARM_CONFIG_FILENAME);
	}

	VOID SetForbitLevel(DupedDef::FORBIT_LEVEL level)
	{
		m_ForbitLevel = level;

		m_cfgFile.WriteInteger("DupedItem", "ForbitLevel", m_ForbitLevel);
		//m_cfgFile.Save(SYSALARM_CONFIG_FILENAME);
	}
};

///////////////////////////////////////////////////////////////////////////////
template <class KItemT>
class KDupedItemBase
{
public:
	KDupedItemBase() : m_markFlag(DupedDef::mark_none)
	{
	}
	~KDupedItemBase()
	{
	}

private:
	// 物品正当性检查
	typedef std::map<DupedDef::tagDUPEDITEM_KEY, INT> ITEMDUPEDCHECK_MAP;
	ITEMDUPEDCHECK_MAP	m_mapItemDuped;

	DupedDef::MARK_FLAG m_markFlag;

public:
	// 标记类型
	DupedDef::MARK_FLAG GetMarkFlag() { return m_markFlag; }
	VOID SetMarkFlag(DupedDef::MARK_FLAG flag) { m_markFlag = flag; }

	INT		GetItemCount() { return m_mapItemDuped.size(); };
	VOID	ClearAll() { m_mapItemDuped.clear(); }
	BOOL	Remove(INT nItemIndex, BOOL bErase);

	DupedDef::OP_RESULT CheckDuped(INT nItemIdx, INT& nOrgItemIdx);
	
	DupedDef::OP_RESULT Deal(INT nItemIdx, INT nPlayerIndex, DupedDef::OP_LEVEL level);

	VOID WriteLogForMark(INT nPlayerIdx, INT nMarkFlag, KItemT& item);

	VOID WriteLogForDel(INT nPlayerIdx, KItemT& item);

	VOID WriteLogForMark(INT nMarkFlag, 
							LPCSTR szAccount, LPCSTR szName, 
							KItemT& item);

	VOID WriteLogForDel(LPCSTR szAccount, LPCSTR szName, 
							KItemT& item);
private:
	// ---------------------------
	BOOL	FilterDupedItem(KItemT& item);
	
	DupedDef::OP_RESULT MarkDupedItem(INT nNewItemIdx, INT nPlayerIndex, INT nOrgItemIdx);
	DupedDef::OP_RESULT DelDupedItemByMark(INT nNewItemIdx, INT nPlayerIndex, INT nOrgItemIdx);
	DupedDef::OP_RESULT DelDupedItemAll(INT nNewItemIdx, INT nPlayerIndex, INT nOrgItemIdx);

	BOOL _WirteLogForItem(QLogFile& logFile, LPCSTR szMsg, 
							LPCSTR szAccount, LPCSTR szName, 
							KItemT& item);
	// -----
	BOOL			ReadMapKeyFromFile(LPSTR szFileName);
	BOOL			WriteMapKeyToFile(LPSTR szFileName);
	// ---------------------------
};

template <class KItemT>
BOOL KDupedItemBase<KItemT>::Remove(INT nItemIndex, BOOL bErase)
{
	if (m_mapItemDuped.size() > 0)
	{
		DupedDef::tagDUPEDITEM_KEY key;
		KItemT* pItem = (KItemT*)DupedDef::GetItemFromItemSet(nItemIndex);
		if (NULL == pItem)
			return FALSE;

		key.dwGenTime = pItem->GetGenTime();
		key.uRandSeed = pItem->GetRandSeed();
		key.wRandSeed2 = pItem->GetRandSeed2();
		
		ITEMDUPEDCHECK_MAP::iterator it = m_mapItemDuped.find(key);
		if (it != m_mapItemDuped.end())
		{
			if (TRUE == bErase)
			{	
				// 将Key从Map中清除
				m_mapItemDuped.erase(it);
			}
			else
			{
				// 将Key所在值清0 (Key保留，以便继续查复制装备)
				it->second = 0;
			}
		}
	}
	return TRUE;
}

template <class KItemT>
DupedDef::OP_RESULT KDupedItemBase<KItemT>::CheckDuped(INT nItemIdx, INT& nOrgItemIdx)
{
	//if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
	//	return ret_error;
	KItemT* pItem = (KItemT*)DupedDef::GetItemFromItemSet(nItemIdx);
	if (NULL == pItem)
		return DupedDef::ret_error;
	
	KItemT& item = *pItem;
	nOrgItemIdx = -1;

	// 条件未满足
	if (FilterDupedItem(item))
		return DupedDef::ret_normal;

	DupedDef::tagDUPEDITEM_KEY key;
	key.dwGenTime = item.GetGenTime();
	key.uRandSeed = item.GetRandSeed();
	key.wRandSeed2 = item.GetRandSeed2();

	typedef std::pair<ITEMDUPEDCHECK_MAP::iterator, bool> INSERTRET;
	INSERTRET ins_ret = m_mapItemDuped.insert(
							ITEMDUPEDCHECK_MAP::value_type(key, nItemIdx));

	// Map中原本不存在此Key，没有复制装备，按正常流程走
	if (false != ins_ret.second)
	{
		return DupedDef::ret_normal;
	}
	
	//物品原件
	if (ins_ret.first->second > 0 && ins_ret.first->second < MAX_ITEM)
	{
		nOrgItemIdx = ins_ret.first->second;
	}

	return DupedDef::ret_found;
}
//-----------------------------------------------------------------------------
// TRUE - 被过滤掉
template <class KItemT>
inline BOOL KDupedItemBase<KItemT>::FilterDupedItem(KItemT& item)
{
	// 生成时间且随机种子为零，不作处理(过滤)
	if (0 == item.GetGenTime() && 0 == item.GetRandSeed())
		return TRUE;

	// LLG_MODIFY_2005_3_8 在加入第二随机种子后，非贵重物品可要查复制
	// 非贵重物品，不作处理(过滤)
	//if (FALSE == item.IsForbitValuable())
	//	return TRUE;

	INT nGenre = item.GetGenre();

	// 目前只查三类物品，其它查复制的意义不大
	if (!KD_ISEQUIP(nGenre) && 
		item_quest != nGenre && 
		item_script != nGenre &&
		item_skill != nGenre)
	{
		return TRUE;
	}

	return FALSE;
}
//-----------------------------------------------------------------------------
template <class KItemT>
DupedDef::OP_RESULT KDupedItemBase<KItemT>::Deal(INT nItemIdx, 
									INT nPlayerIndex, DupedDef::OP_LEVEL level)
{
	// 级别为不检测复制装备
	if (DupedDef::op_none == level)
		return DupedDef::ret_normal;

	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return DupedDef::ret_error;


	INT nOrgItemIdx = -1;

	// 查找复制装备
	DupedDef::OP_RESULT op_ret = CheckDuped(nItemIdx, nOrgItemIdx);

	// 先处理已有标记的物品 - start
	if (DupedDef::op_del_bymark == level)
	{
		// 已直接删除有标记的物品，不用查找
		if (DupedDef::ret_del_bymark == DelDupedItemByMark(nItemIdx, nPlayerIndex, nOrgItemIdx))
			return DupedDef::ret_del_bymark;
	}
	// 先处理已有标记的物品 - end

	if (DupedDef::ret_found != op_ret)
		return op_ret;

	switch (level)
	{
	case DupedDef::op_mark:
		op_ret = MarkDupedItem(nItemIdx, nPlayerIndex, nOrgItemIdx);
		break;
		
	case DupedDef::op_del_bymark:
		op_ret = DelDupedItemByMark(nItemIdx, nPlayerIndex, nOrgItemIdx);
		break;

	case DupedDef::op_del_all:
		op_ret = DelDupedItemAll(nItemIdx, nPlayerIndex, nOrgItemIdx);
		break;
	}

	return op_ret;
}

// -------------------------------------------------------------------------
template <class KItemT>
DupedDef::OP_RESULT KDupedItemBase<KItemT>::MarkDupedItem(INT nNewItemIdx, 
											INT nPlayerIndex, INT nOrgItemIdx)
{
	KItemT* pItem = (KItemT*)DupedDef::GetItemFromItemSet(nNewItemIdx);
	if (NULL == pItem)
		return DupedDef::ret_error;

	KItemT& newItem = *pItem;
	KItemT* pOrgItem = (KItemT*)DupedDef::GetItemFromItemSet(nOrgItemIdx);

	DupedDef::OP_RESULT op_ret = DupedDef::ret_normal;

	//(在此处不知是哪位玩家身上的物品，Log在外面写)
	BYTE mark = newItem.GetDupedItemMark();
	if (mark & m_markFlag)
	{
		op_ret = DupedDef::ret_mark_exist;
	}
	else
	{
		newItem.SetDupedItemMark(mark | m_markFlag);
		op_ret = DupedDef::ret_mark_first;

		WriteLogForMark(nPlayerIndex, m_markFlag, newItem);

	}

	// 物品原件若也存在，也作标记，并写Log(若是一次标记)
	if (pOrgItem && FALSE == (m_markFlag & pOrgItem->GetDupedItemMark()))
	{
		pOrgItem->SetDupedItemMark(pOrgItem->GetDupedItemMark() | m_markFlag);

		WriteLogForMark(pOrgItem->GetBelong(), m_markFlag, *pOrgItem);
	}

	return op_ret;
}

// -------------------------------------------------------------------------
template <class KItemT>
DupedDef::OP_RESULT KDupedItemBase<KItemT>::DelDupedItemByMark(INT nNewItemIdx, 
															INT nPlayerIndex, 
															INT nOrgItemIdx)
{
	return DupedDef::ret_normal;
}

// -------------------------------------------------------------------------
// 删除与表中有相同编号的(复制)物品
template <class KItemT>
DupedDef::OP_RESULT KDupedItemBase<KItemT>::DelDupedItemAll(INT nNewItemIdx, 
													INT nPlayerIndex, 
													INT nOrgItemIdx)
{
	DupedDef::OP_RESULT op_ret = DupedDef::ret_normal;

	// ----------------------
	// 写Log
	KItem* pNewItem = (KItemT*)DupedDef::GetItemFromItemSet(nNewItemIdx);
	if (pNewItem)
		WriteLogForDel(nPlayerIndex, *pNewItem);//Item[nNewItemIdx]);

	//g_cItemMgr.Remove(nNewItemIdx, FALSE);
	DupedDef::RemoveItemFromItemSet(nPlayerIndex, nNewItemIdx, FALSE);

	// 未加到玩家身上，不用Remove
	//BOOL bOk = Player[pItem->GetBelong()].m_cPlayerItem.Remove(nItemIdx);
	// ----------------------

	// 原件不存在
	//if (nOrgItemIdx < 0 || nOrgItemIdx >= MAX_ITEM)
	//	return DupedDef::ret_del_all;
	KItemT* pOrgItem = (KItemT*)DupedDef::GetItemFromItemSet(nOrgItemIdx);
	if (NULL == pOrgItem)
		return DupedDef::ret_del_all;

	// 物品存在，但不属于某个玩家(可能掉在地上)
	KItemT& orgItem = *pOrgItem;
	INT nOrgBelong = orgItem.GetBelong();
	if (nOrgBelong <= 0)
	{
		DupedDef::RemoveItemFromItemSet(0, nOrgItemIdx, FALSE);
		//g_cItemMgr.Remove(nOrgItemIdx, FALSE);
		return DupedDef::ret_del_all;
	}

	// KPlayer& otherPlayer = Player[nOrgBelong];
	// 玩家处于交易状态中，不删除复制装备
	if (DupedDef::CheckPlayerTrading(nOrgBelong))
	{
		//但会加标记
		orgItem.SetDupedItemMark(orgItem.GetDupedItemMark() | DupedDef::mark_dupe_free);
		return DupedDef::ret_del_all;
	}

	// -----------------------------
	// 再删除原件 - start
	// 先写Log...
	WriteLogForDel(nOrgBelong, orgItem);

	//otherPlayer.m_cPlayerItem.Remove(nOrgItemIdx);
	//g_cItemMgr.Remove(nOrgItemIdx, FALSE);
	DupedDef::RemoveItemFromPlayer(nOrgBelong, nOrgItemIdx);
	DupedDef::RemoveItemFromItemSet(nOrgBelong, nOrgItemIdx, FALSE);
	// 再删除原件 - end
	// -----------------------------

	return DupedDef::ret_normal;
}
// -------------------------------------------------------------------------
template <class KItemT>
BOOL KDupedItemBase<KItemT>::_WirteLogForItem(QLogFile& logFile, LPCSTR szMsg, 
						LPCSTR szAccount, LPCSTR szName, KItemT& item)
{
	/*
	if (item.GetGenre() == item_equip_gold)
	{
		logFile.printf_t("[%s][%s(%s)] Gold(%s) T_S(%u %u) RSeed2(%u) AddVerGoldItem(%d,%d,%d)\r\n",
				(szMsg == NULL) ? "unknown" : szMsg,
				(szAccount == NULL) ? "??" : szAccount, 
				(szName == NULL) ? "??" : szName, 
				item.GetName(), 
				item.GetGenTime(), item.GetRandSeed(), item.GetRandSeed2(),
				item.GetVersion(), item.GetGenre(), item.GetRecordIndex());
	}
	else
	{
		logFile.printf_t("[%s][%s(%s)] Item(%s) T_S(%u %u) RSeed2(%u) AddVerItem(%d,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)\r\n",
				(szMsg == NULL) ? "unknown" : szMsg,
				(szAccount == NULL) ? "??" : szAccount, 
				(szName == NULL) ? "??" : szName, 
				item.GetName(), 
				item.GetGenTime(), item.GetRandSeed(), item.GetRandSeed2(),
				item.GetVersion(), item.GetRandSeed(), 
				item.GetGenre(), item.GetDetailType(), item.GetParticular(),
				item.GetLevel(), item.GetSeries(), item.GetLuck(),
				item.GetMagicLevel(0), item.GetMagicLevel(1),
				item.GetMagicLevel(2), item.GetMagicLevel(3),
				item.GetMagicLevel(4), item.GetMagicLevel(5));
	}
	*/

	return TRUE;
}

// -------------------------------------------------------------------------
template <class KItemT>
VOID KDupedItemBase<KItemT>::WriteLogForMark(INT nPlayerIndex, INT nMarkFlag, KItemT& item)
{
	if (nPlayerIndex > 0 && nPlayerIndex < MAX_PLAYER)
	{
		WriteLogForMark(nMarkFlag, DupedDef::GetPlayerAccount(nPlayerIndex), 
							DupedDef::GetPlayerName(nPlayerIndex),
							item);

		DupedDef::SendMsgForMark(nPlayerIndex, item.GetName());
	}
	else
	{
		WriteLogForMark(nMarkFlag, "?A?", "?N?", item);
	}
}

template <class KItemT>
VOID KDupedItemBase<KItemT>::WriteLogForDel(INT nPlayerIndex, KItemT& item)
{
	if (nPlayerIndex > 0 && nPlayerIndex < MAX_PLAYER)
	{
		WriteLogForDel(DupedDef::GetPlayerAccount(nPlayerIndex), 
							DupedDef::GetPlayerName(nPlayerIndex),
							item);

		DupedDef::SendMsgForDel(nPlayerIndex, item.GetName());
	}
	else
	{
		WriteLogForDel("?A?", "?N?", item);
	}
}

template <class KItemT>
VOID KDupedItemBase<KItemT>::WriteLogForMark(INT nMarkFlag, 
												LPCSTR szAccount, 
												LPCSTR szName, 
												KItemT& item)
{
	CHAR szInfo[20];
	sprintf(szInfo, "Mark-%d", nMarkFlag);
	/*
	KSG_LogFile& logFile = DupedDef::GetDupedLogFile();

	_WirteLogForItem(logFile, szInfo, szAccount, szName, item);
	*/
}

template <class KItemT>
VOID KDupedItemBase<KItemT>::WriteLogForDel(LPCSTR szAccount, 
												LPCSTR szName, 
												KItemT& item)
{
	/*
	KSG_LogFile& logFile = DupedDef::GetDupedLogFile();

	_WirteLogForItem(logFile, "Delete", szAccount, szName, item);
	*/
}
// -------------------------------------------------------------------------

#endif /* __HELPER_ITEM_KDUPEDITEMBASE_H__ */
