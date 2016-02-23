
#ifndef __HELPER_ITEM_KDUPEDITEMDEF_H__
#define __HELPER_ITEM_KDUPEDITEMDEF_H__

// -------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
namespace DupedDef
{
	// 对复制装备的操作状态
	// 对复制装备的操作级别
	typedef enum
	{
		op_none = 0,
		op_mark,		// 作标记，不删除
		op_del_bymark,	// 删除有复制标记的物品
		op_del_all,		// 删除所有的复制物品
		op_level_end = op_del_all,
	}OP_LEVEL;

	// 操作结果
	typedef enum
	{
		ret_error = -1,
		ret_normal = 0,
		ret_found,
		ret_mark_first,		// 第一次作标记
		ret_mark_exist,		// 标记已存在
		ret_del_bymark,		// 有复制标记的物品被删除
		ret_del_all,		// 复制物品被删除
	}OP_RESULT;

	enum MARK_FLAG
	{
		mark_none		= 0,	// 正常物品
		mark_dupe_free	= 1,	// 复制物品，但不作处理的标记
		mark_dupe_limit	= 2,	// 复制物品，操作受限的标记
		//mark_inown		= 4,	// 与自己身上的物品有雷同复制)的标记
	};
	enum FORBIT_LEVEL
	{
		forbit_none = 0,
		forbit_for_inworld,
		forbit_for_inown,
		forbit_for_all,
		forbit_level_end = forbit_for_all,
	};

	// -----------------------------------
	struct tagDUPEDITEM_KEY
	{
		UINT	uRandSeed;
		DWORD	dwGenTime;
		WORD	wRandSeed2;


		bool operator==(CONST tagDUPEDITEM_KEY& key) CONST
		{
			if (uRandSeed == key.uRandSeed && dwGenTime == key.dwGenTime &&
				wRandSeed2 == key.wRandSeed2)
			{
				return true;
			}

			return false;
		}

		bool operator < (CONST tagDUPEDITEM_KEY& key) CONST
		{
			if (uRandSeed < key.uRandSeed)
			{
				return true;
			}
			else if (uRandSeed == key.uRandSeed)
			{
				if (dwGenTime < key.dwGenTime)
				{
					return true;
				}
				else if (dwGenTime == key.dwGenTime)
				{
					if (wRandSeed2 < key.wRandSeed2)
					{
						return true;
					}
				}
			}
			return false;
		}
	};
};

// -------------------------------------------------------------------------

#endif /* __HELPER_ITEM_KDUPEDITEMDEF_H__ */
