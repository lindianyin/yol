
#pragma once

// 数据库用的是emK_FIELDTYPE_MEDIUMBLOB来存，每个Group记录为65535个Data，平均一个是256字节
// 其中Key占了2字节
#define KD_BIN_DATA_MAX_LEN 200

#ifdef GAME_CENTER
	// -------------------------------------------------------------------------
	// bit enum
	enum KE_DATA_STATE
	{
		emKDATA_STATE_MODIFYED,	// 是否被修改
	};

#pragma pack(push, 1)
	struct KDB_INT_DATA_ITEM
	{
		WORD wKey;
		INT nValue;
	};

	struct KDB_BIN_DATA_ITEM
	{
		WORD wKey;
		WORD wLen;
		BYTE abValue[KD_BIN_DATA_MAX_LEN];
	};
#pragma pack(pop)

	typedef std::map<WORD, INT> INT_GROUP_BASE_DATA;						// key -> int value
	typedef std::map<WORD, std::vector<BYTE> > BINARY_GROUP_BASE_DATA;		// key -> binary value

	struct KGB_INT_DATA_GROUP
	{
		KGB_INT_DATA_GROUP() {byState = 0;}
		BYTE byState;
		INT_GROUP_BASE_DATA cIntDataMap;
	};

	struct KGB_BINARY_DATA_GROUP
	{
		KGB_BINARY_DATA_GROUP() {byState = 0;}
		BYTE byState;
		BINARY_GROUP_BASE_DATA cBinDataMap;
	};

	typedef std::map<WORD, KGB_INT_DATA_GROUP> INT_DATA_GROUP_MAP;			// groupid -> int group value
	typedef std::map<WORD, KGB_BINARY_DATA_GROUP> BINARY_DATA_GROUP_MAP;	// groupid -> binary group value

#else
	typedef std::map<DWORD, INT> FLAT_INT_DATA_MAP;
	typedef std::map<DWORD, std::vector<BYTE> > FLAT_BINARY_DATA_MAP;
#endif
// -------------------------------------------------------------------------

