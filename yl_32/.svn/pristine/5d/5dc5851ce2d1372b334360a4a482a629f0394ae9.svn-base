
#pragma once

#define SALE_NAME_LEN          64
#define SYNC_PAGE_MAX_COUNT    50
#include<stdint.h>

enum KAUCTION_SALE_STATE
{
    assIgnore,
    assSomeoneBid,  // 有人出价
    assNoOneBid,    // 无人出价
};

enum KAUCTION_ORDER_TYPE
{
    aotQuality,
    aotLevel,
    aotLeftTime,
    aotPrice,
    aotBuyItNowPrice,

    aotTotal
};

enum KAUCTION_RESULT_CODE
{
    arcSucceed,
    arcItemNotExist,    // 物品不存在
    arcPriceTooLow,     // 出价太低
    arcCannotCancel,    // 有人出价，不能取消
    arcServerBusy,      // 服务器忙，拒绝查询
    arcMailBoxFull,     // 邮件太多，拒绝买卖

    arcUnknownError
};

enum KAUCTION_MESSAGE_CODE
{
    amcAcquireItem,
    amcBidLost,
    amcSomeoneBid,
    amcItemSold,
    amcTimeOver,

    amcTotalMessageCode
};

#pragma pack(1)
struct KAUCTION_LOOKUP_PARAM
{
    char    szSaleName[SALE_NAME_LEN];
    int     nAucGenre;
    int     nAucSub;
    int     nLevelMin;
    int     nLevelMax;
    int     nQuality;
    char    szSellerName[_NAME_LEN];
    DWORD   dwSellerID;
    DWORD   dwBidderID;
    int     nTopPrice;
    BYTE    byAuction;
    int     nStartIndex;
    BYTE    byOrderType;
    BYTE    byDesc;
    BYTE    byByUnitPrice;
};

struct KAUCTION_PAGE_NODE
{
    DWORD   dwSaleID;
    DWORD   dwSellerID;
    DWORD   dwBidderID;
    char    szSellerName[_NAME_LEN];
    char    szBidderName[_NAME_LEN];
    int     nPrice;
    int     nBuyItNowPrice;
    int     nLeftTime;
    uint32_t  uItemSize;
    BYTE    byItemData[0];
};

struct KAUCTION_PAGE_HEAD 
{
    int     nTotalCount;
    uint32_t  uSize;
    BYTE    bySaleList[0];
};
#pragma pack()


