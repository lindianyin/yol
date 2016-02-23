#ifndef _KMAIL_DEF_H_
#define _KMAIL_DEF_H_

#include "ProtocolBasic.h"
#include <stdint.h>

#define MAILSYS_VERSION      5

#define KMAIL_MAX_ITEM_COUNT 5
#define MAIL_TITLE_LEN       16
#define MAX_MAIL_TEXT_LEN    256
#define MAX_COND_DATA_LEN	 32		// 邮件条件数据最大长度
#define MAX_COND_NUM		 5		// 邮件条件最大个数

enum KMAIL_TYPE
{
    eMailType_Player = 0,//玩家邮件
    eMailType_System,    //系统邮件
    eMailType_Auction,   //拍卖邮件
    eMailType_GmMessage, //gm邮件
    eMailType_PlayerMessage,//玩家邮件消息
    eMailType_Total
};

enum KMAIL_RESULT_CODE
{
    mrcSucceed,
    mrcFailed,

    mrcSystemBusy,
    mrcDstNotExist,
    mrcNotEnoughMoney,
    mrcItemAmountLimit,
    mrcNotEnoughRoom,
    mrcMoneyLimit,
    mrcMailNotFound,
    mrcMailBoxFull,
    mrcReturnMailFailed,
    mrcItemBeBind,
    mrcTimeLimitItem,
    mrcItemNotInPackage,
    mrcDstNotSelf,
    mrcDeleteRefused,
    mrcSelfMailBoxFull,
    mrcTooFarAway,

    mrcTotal
};

#pragma pack(1)
struct KMailItemDesc 
{  
    bool   bAcquired;   // true 表示已经分发给玩家了 
    BYTE   byDataLen;
    int    nPrice;      // 货到付款的道具价格
};

struct KMail
{
    DWORD           dwMailID;
    BYTE            byType;
    bool            bRead;
    DWORD           dwSenderID;
    char            szSenderName[_NAME_LEN];
    char            szTitle[MAIL_TITLE_LEN];
    uint32_t          nSendTime;
    uint32_t          nRecvTime;
    uint32_t          nLockTime;
    int             nMoney; //钱
	int             nSilver;//真气
	int             nEnergy;//银两
	BYTE			byCondNum;
	BYTE			byCond[MAX_COND_DATA_LEN];
    WORD            wTextLen;
    KMailItemDesc   ItemDesc[KMAIL_MAX_ITEM_COUNT];
    BYTE            byData[0];
};

struct KMailContent
{
    int             nMoney;
	int             nSilver;//add 
	int             nEnergy;//add
    WORD            wTextLen;
    KMailItemDesc   ItemDesc[KMAIL_MAX_ITEM_COUNT];
    BYTE            byData[0];
};

struct KMailListInfo
{
    BYTE        byType;
    DWORD       dwMailID;
    char        szSender[_NAME_LEN];
    char        szTitle[MAIL_TITLE_LEN];
    uint32_t    nSendTime;
    uint32_t    nRecvTime;
    bool        bReadFlag;
    bool        bMoneyFlag;
    bool        bItemFlag;
};


enum MAIL_CHECK_TYPE
{
	emMailCheckSex = 0,
	emMailCheckLevel,
	emMailCheckTypeTotal,
};

enum MAIL_CONDITION_OP
{
	emMailCondOpGreater,		// >
	emMailCondOpGreaterEqual,	// >=
	emMailCondOpLess,			// <
	emMailCondOpLessEqual,		// <=
	emMailCondOpEqual,			// ==
	emMailCondOpNotEqual,		// !=
	emMailCondOpIgnore,			// ...
};

struct MAIL_CONDITION_HEAD
{
};

struct MAIL_CONDITION_SEX : MAIL_CONDITION_HEAD
{
	BYTE byCondOp;
	BYTE bySex;
};

struct MAIL_CONDITION_LEVEL : MAIL_CONDITION_HEAD
{
	BYTE byLevel1Op;
	BYTE byLevel1;
	BYTE byLevel2Op;
	BYTE byLevel2;
};

#pragma pack()

#endif //_KMAIL_DEF_H_
