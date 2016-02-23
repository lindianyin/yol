#include "stdafx.h"
#include "mysql.h"
#include "DBBase.h"
#include "DBTools.h"
#include "GameCenter.h"

#ifdef WIN32
#define atoll _atoi64
#endif
KDBTools::KDBTools()
{
    m_pDBHandle         = NULL;
}

BOOL KDBTools::Init()
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;
    
    m_pDBHandle = MySQL_Connect(
        g_pGameCenter->m_Settings.m_szDBIP,
        g_pGameCenter->m_Settings.m_szDBName,
        g_pGameCenter->m_Settings.m_szDBAcc, 
        g_pGameCenter->m_Settings.m_szDBPsw
    );
    LOG_PROCESS_ERROR(m_pDBHandle);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        if (m_pDBHandle)
        {
            MySQL_Disconnect(m_pDBHandle);
            m_pDBHandle = NULL;
        }
    }
    return bResult;
}

void KDBTools::UnInit()
{
    if (m_pDBHandle)
    {
        MySQL_Disconnect(m_pDBHandle);
        m_pDBHandle = NULL;
    }
}

BOOL KDBTools::CheckDatabase()
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;

    bRetCode = CheckRoleTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckRelationTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckMapCopyTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckRoleBlakListTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckTongTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckMailTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckGlobalMailTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckPQTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckAuctionTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckGlobalCustomDataTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckGlobalSystemValueTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckStatDataTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckActivityTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckGameCardTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckRoleDeleteTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckAntiFarmerTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckMentorTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CheckRenameRoleTable();
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckDelayLoadTable();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckPayRecordTable();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckRankPoolTable();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckArenaRecordTable();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckServerRecordTable();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckGMSendAwardTale();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckSaveOpenseTable();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = CheckPayListTable();
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::LoadGameData()
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;

    bRetCode = LoadRoleTable();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = LoadMapCopyInfo();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = LoadRoleBlackList();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = LoadTongTable();
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = LoadRankTable();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = LoadPayRecordTable();
	LOG_PROCESS_ERROR(bRetCode);
    
    //bRetCode = LoadGlobalMailTable();
    //LOG_PROCESS_ERROR(bRetCode);

    //bRetCode = LoadGlobalSystemValue();
    //LOG_PROCESS_ERROR(bRetCode);

    //bRetCode = LoadGlobalCustomDataTable();
    //LOG_PROCESS_ERROR(bRetCode);

    bRetCode = LoadStatDataTable();
    LOG_PROCESS_ERROR(bRetCode);

    //bRetCode = LoadActivityTable();
    //LOG_PROCESS_ERROR(bRetCode);

    bRetCode = LoadRoleDeleteTable();
    LOG_PROCESS_ERROR(bRetCode);

    //bRetCode = LoadAntiFarmerTable();
    //LOG_PROCESS_ERROR(bRetCode);

    //bRetCode = LoadMentorTable();
    //LOG_PROCESS_ERROR(bRetCode);

    bRetCode = LoadRenameRoleTable();
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckRoleTable()
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = 0;
    int                 nStrLen     = 0;
    
    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s "
        "("
        "   %s bigint not null,"
        "   %s varchar(32) not null,"
        "   %s varchar(32) not null,"
        "   %s blob not null,"
        "   %s mediumblob,"
        "   %s datetime not null,"
        "   primary key (%s), index (%s), unique (%s)"
        ");",
        ROLE_TABLE_NAME,            KG_ID_FIELD_NAME,         KG_NAME_FIELD_NAME, 
        KG_ACCOUNT_FIELD_NAME,      KG_BASE_INFO_FIELD_NAME,  KG_EXT_INFO_FIELD_NAME, 
        KG_LAST_MODIFY_FIELD_NAME,  KG_ID_FIELD_NAME,         KG_ACCOUNT_FIELD_NAME,    KG_NAME_FIELD_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            ROLE_TABLE_NAME
        );
        goto EXIT0;
    }
    
    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s "
        "("
        "   %s bigint not null auto_increment,"
        "   %s varchar(32) not null,"
        "   %s varchar(32) not null,"
        "   %s blob not null,"
        "   %s mediumblob,"
        "   %s datetime not null,"
        "   primary key (%s), index (%s)"
        ");",
        RESTORE_ROLE_TABLE_NAME,    KG_ID_FIELD_NAME,         KG_NAME_FIELD_NAME, 
        KG_ACCOUNT_FIELD_NAME,      KG_BASE_INFO_FIELD_NAME,  KG_EXT_INFO_FIELD_NAME, 
        KG_DELETE_TIME_FIELD_NAME,  KG_ID_FIELD_NAME,         KG_ACCOUNT_FIELD_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            RESTORE_ROLE_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckRelationTable()
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = 0;
    int                 nStrLen     = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists "
        "    %s "
        "    ( "
        "        PlayerID               integer unsigned    not null, "
        "        RelationData           blob, "
        "        primary key (PlayerID) "
        "    ) "
        ";",
        RELATION_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            RELATION_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckMapCopyTable()
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = 0;
    int                 nStrLen     = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   MapID integer unsigned, "
        "   MapCopyIndex integer, "
        "   SceneData blob, "
        "   CreateTime datetime not null, "
        "   LastModifyTime datetime not null, "
        "   primary key  (MapID, MapCopyIndex) "
        "); ",
        MAP_COPY_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            MAP_COPY_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckRoleBlakListTable()
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = 0;
    int                 nStrLen     = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s "
        "("
        "   ID bigint not null,"
        "   FreezeTime datetime not null,"
        "   primary key (ID), index (ID)"
        ");",
        ROLE_BLACK_LIST_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0);
    LOG_PROCESS_ERROR(nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            ROLE_BLACK_LIST_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckTongTable()
{
    BOOL            bResult     = false;
    BOOL            bRetCode    = 0;
    int             nStrLen     = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s "
        "("
        "   ID              bigint not null,"
        "   Data            mediumblob,"
        "   primary         key (ID) "
        ");",
        TONG_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0);
    LOG_PROCESS_ERROR(nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            TONG_TABLE_NAME
        );
        goto EXIT0;
    }


    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckGlobalMailTable()
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = 0;
    int                 nStrLen     = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   Version      integer    not null, "
        "   MailIndex    bigint     not null, "
        "   EndTime      datetime   not null, "
        "   MailInfo     mediumblob not null, "
        "   primary key (MailIndex) "
        "); ",
        GLOBAL_MAIL_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));
    
    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            GLOBAL_MAIL_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckMailTable()
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = 0;
    int                 nStrLen     = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   ID           bigint     not null, "
        "   BaseTime     datetime   not null, "
        "   MailBoxInfo  mediumblob not null, "
        "   primary key (ID), index (ID, BaseTime) "
        "); ",
        MAIL_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            MAIL_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckPQTable()
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = 0;
    int                 nStrLen     = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s "
        "("
        "   ID integer unsigned not null, "
        "   Data blob not null, "
        "   primary key (ID), index (ID) "
        ");",
        PQ_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0);
    LOG_PROCESS_ERROR(nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            PQ_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;

}

BOOL KDBTools::CheckAuctionTable()
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = 0;
    int                 nStrLen         = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   ID              bigint      not null auto_increment, "
        "   Name            varchar(32) not null, "
        "   AucGenre        integer     not null, "
        "   AucSub          integer     not null, "
        "   RequireLevel    integer     not null, "
        "   Quality         integer     not null, "
        "   SellerID        bigint      not null, "
        "   SellerName      varchar(32) not null, "
        "   Price           integer     not null, "
        "   BuyItNowPrice   integer     not null, "
        "   BidderID        bigint      not null, "
        "   CustodyCharges  integer     not null, "
        "   DurationTime    datetime    not null, "
        "   ItemData        blob        not null, "
        "   primary key (ID) "
        ");",
        AUCTION_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            AUCTION_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckGlobalCustomDataTable()
{
    BOOL    beResult = false;
    BOOL    bRetCode = 0;
    int     nStrLen  = 0;

    nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   Name      varchar(32)   not null, "
        "   Value     mediumblob    not null, "
        "   primary key (Name) "
        ");",
        GLOBAL_CUSTOM_DATA_TABLE_NAME
	);
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            GLOBAL_CUSTOM_DATA_TABLE_NAME
        );
        goto EXIT0;
    }

    beResult = true;
EXIT0:
    return beResult;
}

BOOL KDBTools::CheckGlobalSystemValueTable()
{
    BOOL    beResult = false;
    BOOL    bRetCode = 0;
    int     nStrLen  = 0;

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   Name      varchar(32)   not null, "
        "   Value     integer       not null, "
        "   primary key (Name) "
        ");",
        GLOBAL_SYSTEM_VALUE_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            GLOBAL_SYSTEM_VALUE_TABLE_NAME
        );
        goto EXIT0;
    }

    beResult = true;
EXIT0:
    return beResult;
}

BOOL KDBTools::CheckStatDataTable()
{
    BOOL    bResult  = false;
    BOOL    bRetCode = 0;
    int     nStrLen  = 0;

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   Name      varchar(64)   not null, "
        "   ID        integer       not null, "
        "   primary key (Name) "
        ");",
        STAT_DATA_NAME_INDEX_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            STAT_DATA_NAME_INDEX_TABLE_NAME
        );
        goto EXIT0;
    }

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   StatTime    datetime        not null, "
        "   StatData    mediumblob      not null, "
        "   primary key (StatTime) "
        ");",
        STAT_DATA_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            STAT_DATA_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckActivityTable()
{
    BOOL    bResult  = false;
    BOOL    bRetCode = 0;
    int     nStrLen  = 0;

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   ID      integer     not null, "
        "   Type    integer     not null, "
        "   Time    datetime    not null, "
        "   Value0  integer     not null, "
        "   Value1  integer     not null, "
        "   Value2  integer     not null, "
        "   Value3  integer     not null, "
        "   Value4  integer     not null, "
        "   Value5  integer     not null, "
        "   primary key (ID, Type) "
        ");",
        ACTIVITY_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            ACTIVITY_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckGameCardTable()
{
    BOOL    bResult  = false;
    BOOL    bRetCode = 0;
    int     nStrLen  = 0;

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   ID          bigint      not null auto_increment, "
        "   SellerID    bigint      not null, "
        "   Coin        integer     not null, "
        "   GameTime    integer     not null, "
        "   Type        integer     not null, "
        "   Price       integer     not null, "
        "   EndTime     datetime    not null, "
        "   primary key (ID) "
        ");",
        GAME_CARD_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            GAME_CARD_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckRoleDeleteTable()
{
    BOOL    bResult  = false;
    BOOL    bRetCode = 0;
    int     nStrLen  = 0;

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   ID          bigint      not null, "
        "   EndTime     datetime    not null, "
        "   primary key (ID) "
        ");",
        ROLE_DELETE_LIST_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            ROLE_DELETE_LIST_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckAntiFarmerTable()
{
    BOOL    bResult  = false;
    BOOL    bRetCode = 0;
    int     nStrLen  = 0;

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   Account         varchar(32) not null, "
        "   PunishEndTime   datetime    not null, "
        "   primary key (Account) "
        ");",
        ANTI_FARMER_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR, 
            "DB ERROR when try to create table \'%s\'\n", 
            ANTI_FARMER_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckMentorTable()
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;
    int  nStrLen    = 0;

    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL), 
        "create table if not exists %s( "
        "   MentorID      bigint    not null, "
        "   ApprenticeID  bigint    not null, "
        "   Data          tinyblob  not null, "
        "   primary key (MentorID, ApprenticeID) "
        ");",
        MENTOR_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR,
            "DB ERROR when try to create table \'%s\'\n",
            MENTOR_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckRenameRoleTable()
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;
    int  nStrLen    = 0;
    
    nStrLen = snprintf(
        m_szSQL, sizeof(m_szSQL),
        "create table if not exists %s("
        " RoleID bigint not null,"
        " primary key(RoleID));",
        RENAME_ROLE_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    if (!bRetCode)
    {
        QLogPrintf(
            LOG_ERR,
            "DB ERROR when try to create table \'%s\'\n",
            RENAME_ROLE_TABLE_NAME
        );
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KDBTools::CheckDelayLoadTable()
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"create table if not exists "
		"    %s "
		"    ( "
		"        PlayerID               integer unsigned    not null, "
		"        DelayLoadData           blob, "
		"        primary key (PlayerID) "
		"    ) "
		";",
		DELAY_LOAD_TABLE_NAME
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

	bRetCode = DoQuery(m_szSQL);
	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			DELAY_LOAD_TABLE_NAME
			);
		goto EXIT0;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KDBTools::CheckPayRecordTable()
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"create table if not exists "
		"    %s "
		"    ( "
		"        PlayerID               integer unsigned    not null, "
		"        PayRecordData           blob, "
		"        primary key (PlayerID) "
		"    ) "
		";",
		PAY_RECORD_TABLE_NAME
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

	bRetCode = DoQuery(m_szSQL);
	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			PAY_RECORD_TABLE_NAME
			);
		goto EXIT0;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KDBTools::CheckRankPoolTable()
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"create table if not exists "
		"    %s "
		"    ( "
		"        RankID               integer unsigned    not null, "
		"        Data           blob, "
		"        primary key (RankID) "
		"    ) "
		";",
		RANKPOOL_TABLE_NAME
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

	bRetCode = DoQuery(m_szSQL);
	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			RANKPOOL_TABLE_NAME
			);
		goto EXIT0;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KDBTools::CheckArenaRecordTable()
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"create table if not exists "
		"    %s "
		"    ( "
		"        PlayerID               integer unsigned    not null, "
		"        Record           blob, "
		"        primary key (PlayerID) "
		"    ) "
		";",
		ARENA_RECORD_TABLE_NAME
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

	bRetCode = DoQuery(m_szSQL);
	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			ARENA_RECORD_TABLE_NAME
			);
		goto EXIT0;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KDBTools::CheckServerRecordTable()
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"create table if not exists "
		"    %s "
		"    ( "
		"        RecordID               integer unsigned    not null, "
		"        Record           blob, "
		"        primary key (RecordID) "
		"    ) "
		";",
		SERVER_RECORD_TABLE_NAME
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));

	QLogPrintf(LOG_DEBUG,m_szSQL);
	bRetCode = DoQuery(m_szSQL);
	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			SERVER_RECORD_TABLE_NAME
			);
		goto EXIT0;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KDBTools::LoadRoleTable()
{
    BOOL             bResult         = false;
    BOOL             bRetCode        = false;
    int              nRetCode        = false;
    MYSQL_RES*       pQueryRes       = NULL;
    DWORD            dwRowCount      = 0;
    BYTE*            pbyData         = NULL;
    unsigned long*   pulDataLeng     = NULL;
    DWORD            dwRoleCount     = 0;
    KRoleBaseInfo*   pBaseInfo       = NULL;
    MYSQL_ROW        QueryRow;

    assert(m_pDBHandle);

    QLogPrintf(LOG_INFO, "[DB] Role data loading ... ...");

    nRetCode = snprintf(
        m_szSQL, sizeof(m_szSQL), "select %s, %s, %s, %s from %s", 
        KG_ID_FIELD_NAME,           KG_NAME_FIELD_NAME,     KG_ACCOUNT_FIELD_NAME, 
        KG_BASE_INFO_FIELD_NAME,    ROLE_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        KRole*          pRetRole    = NULL;
        DWORD           dwRoleID    = KD_BAD_ID;
        unsigned long*  ulLengths   = NULL;

        QueryRow = mysql_fetch_row(pQueryRes);
        if(!QueryRow)
        {
            break;
        }
        
        ulLengths = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(ulLengths);
        
        LOG_PROCESS_ERROR(ulLengths[1] < _NAME_LEN);
        LOG_PROCESS_ERROR(ulLengths[2] < _NAME_LEN);
        LOG_PROCESS_ERROR(ulLengths[3] == sizeof(KRoleBaseInfo));

        assert(QueryRow[0]);
        assert(QueryRow[1]);
        assert(QueryRow[2]);
        assert(QueryRow[3]);
        
        dwRoleID = (DWORD)strtoul(QueryRow[0], NULL, 10);
        pBaseInfo = (KRoleBaseInfo*)QueryRow[3];

        //LOG_PROCESS_ERROR(pBaseInfo->nVersion == 0);

		INT nFightScore = *((INT*)pBaseInfo->byReserved);
        pRetRole = g_pGameCenter->m_RoleManager.AddRole(
            QueryRow[2], QueryRow[1], dwRoleID,
            pBaseInfo->byLevel, (BYTE)pBaseInfo->cRoleType, /*(KCAMP)pBaseInfo->byCamp, */pBaseInfo->byFaction, nFightScore, pBaseInfo->nLastSaveTime, pBaseInfo->nCreateTime
        );
        LOG_PROCESS_ERROR(pRetRole);

        ++dwRoleCount;
    }

    bResult = true;
EXIT0:

    QLogPrintf(LOG_INFO, "[DB] %u roles loaded !", dwRoleCount);

    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}


BOOL KDBTools::LoadPayRecordTable()
{
	BOOL             bResult         = false;
	BOOL             bRetCode        = false;
	int              nRetCode        = false;
	MYSQL_RES*       pQueryRes       = NULL;
	DWORD            dwRowCount      = 0;
	BYTE*            pbyData         = NULL;
	unsigned long*   pulDataLeng     = NULL;
	DWORD            dwRoleCount     = 0;
	KRoleBaseInfo*   pBaseInfo       = NULL;
	MYSQL_ROW        QueryRow;

	assert(m_pDBHandle);

	QLogPrintf(LOG_INFO, "[DB] Role data loading ... ...");

	nRetCode = snprintf(
		m_szSQL, sizeof(m_szSQL), "select PlayerID, PayRecordData from %s", PAY_RECORD_TABLE_NAME
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

	bRetCode = DoQuery(m_szSQL);
	LOG_PROCESS_ERROR(bRetCode);

	pQueryRes = mysql_use_result(m_pDBHandle);
	LOG_PROCESS_ERROR(pQueryRes);

	while (true)
	{
		DWORD           dwRoleID    = KD_BAD_ID;
		unsigned long*  ulLengths   = NULL;

		QueryRow = mysql_fetch_row(pQueryRes);
		if (!QueryRow)
		{
			break;
		}

		ulLengths = mysql_fetch_lengths(pQueryRes);
		LOG_PROCESS_ERROR(ulLengths);

		dwRoleID = (DWORD)strtoul(QueryRow[0], NULL, 10);
		bRetCode = g_pGameCenter->m_RoleDB.LoadPayRecord(
			dwRoleID, 
			(BYTE*)QueryRow[1], (size_t)ulLengths[1]
		);

		LOG_PROCESS_ERROR(bRetCode);

		++dwRoleCount;
	}

	bResult = true;
EXIT0:

	QLogPrintf(LOG_INFO, "[DB] %u roles loaded !", dwRoleCount);

	if (pQueryRes)
	{
		mysql_free_result(pQueryRes);
		pQueryRes = NULL;
	}
	return bResult;
}

BOOL KDBTools::LoadMapCopyInfo()
{
    BOOL                bResult             = false;
    BOOL                bRetCode            = false;
    int                 nRetCode            = false;
    MYSQL_RES*          pQueryRes           = NULL;
    unsigned long*      pulFieldLen         = NULL;
    DWORD               dwMapID             = 0;
    int                 nMapCopyIndex       = 0;
    KMapInfo*           pMapInfo            = NULL;
    KMapCopy*           pMapCopy            = NULL;
    BYTE*               pbySceneData        = NULL;
    size_t              uSceneDataLen       = 0;
    time_t              nCreateTime         = 0;
    time_t              nLastSaveTime       = 0;
    time_t              nNextRefreshTime    = 0;
    BYTE*               pbyData             = NULL;
    int                 nMapCopyCount       = 0;
    BOOL                bAutoDelete         = false;
    KMapManager*        pMapManager         = &(g_pGameCenter->m_MapManager);
    MYSQL_ROW           QueryRow;

    assert(pMapManager);

    assert(m_pDBHandle);

    QLogPrintf(LOG_DEBUG, "[DB] Map copys loading ... ...");

    nRetCode = snprintf(
        m_szSQL, sizeof(m_szSQL),
        "select " 
        "   MapID, MapCopyIndex, SceneData, "
        "   UNIX_TIMESTAMP(CreateTime), UNIX_TIMESTAMP(LastModifyTime) "
        "from " 
        "   %s;",
        MAP_COPY_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        pulFieldLen = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(pulFieldLen);

        dwMapID             = (DWORD)strtoul(QueryRow[0], NULL, 10);
        nMapCopyIndex       = atoi(QueryRow[1]);
        pbySceneData        = (BYTE*)QueryRow[2];
        uSceneDataLen       = pulFieldLen[2];
        nCreateTime         = strtol(QueryRow[3], NULL, 10);
        nLastSaveTime       = strtol(QueryRow[4], NULL, 10);
        
        LOG_PROCESS_ERROR(uSceneDataLen > 0);

        pMapInfo = pMapManager->GetMapInfo(dwMapID);
        LOG_PROCESS_ERROR(pMapInfo);
        
        if (pMapInfo->m_nRefreshCycle > 0)
        {
            pMapInfo->m_nNextRefreshTime = (nLastSaveTime / pMapInfo->m_nRefreshCycle + 1) * (pMapInfo->m_nRefreshCycle);
        }

        bRetCode = pMapManager->IsMapCopyInPreloadGroup(dwMapID, nMapCopyIndex); // 在预加载表中的地图不是自动删除的
        bAutoDelete = !bRetCode;

        pMapCopy = pMapInfo->CreateMapCopy(nMapCopyIndex, nCreateTime, nLastSaveTime, 0, bAutoDelete);
        LOG_PROCESS_ERROR(pMapCopy);

        bRetCode = pMapCopy->Load(pbySceneData, uSceneDataLen);
        LOG_PROCESS_ERROR(bRetCode);

        pMapCopy->m_eState          = eMapStateOffline;
        pMapCopy->m_bIsChanged      = false;
        pMapCopy->m_nCreateTime     = nCreateTime;
        pMapCopy->m_nLastSaveTime   = nLastSaveTime;

        ++nMapCopyCount;
    }

    QLogPrintf(LOG_DEBUG, "[DB] %d map copys loaded !", nMapCopyCount);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KDBTools::LoadRoleBlackList()
{
    BOOL        bResult         = false;
    BOOL        bRetCode        = false;
    int         nRetCode        = false;
    MYSQL_RES*  pQueryRes       = NULL;
    MYSQL_ROW   QueryRow        = NULL;
    DWORD       dwRoleID        = 0;
    time_t      nFreezeTime     = 0;
    int         nBlackListCount = 0;

    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL), "select ID, UNIX_TIMESTAMP(FreezeTime) from %s", 
        ROLE_BLACK_LIST_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        dwRoleID = strtol(QueryRow[0], NULL, 10);
        nFreezeTime = strtol(QueryRow[1], NULL, 10);

        g_pGameCenter->m_RoleManager.FreezeRole(dwRoleID, nFreezeTime);

        nBlackListCount++;
    }

    QLogPrintf(LOG_DEBUG, "[DB] %d roles in black list loaded !", nBlackListCount);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KDBTools::LoadRoleDeleteTable()
{
    BOOL        bResult     = false;
    BOOL        bRetCode    = false;
    int         nRetCode    = false;
    MYSQL_RES*  pQueryRes   = NULL;
    MYSQL_ROW   QueryRow    = NULL;
    DWORD       dwID        = 0;
    time_t      nEndTime    = 0;
    int         nCount      = 0;

    nRetCode = snprintf(
        m_szSQL, sizeof(m_szSQL),
        "select ID, UNIX_TIMESTAMP(EndTime) from %s", 
        ROLE_DELETE_LIST_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        dwID        = strtoul(QueryRow[0], NULL, 10);
        nEndTime    = strtol(QueryRow[1], NULL, 10);

        g_pGameCenter->m_RoleManager.RegisterDelRole(dwID, nEndTime);

        nCount++;
    }

    QLogPrintf(LOG_DEBUG, "[DB] %d roles in delete queue !", nCount);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KDBTools::LoadTongTable()
{
    BOOL            bResult     = false;
    BOOL            bRetCode    = false;
    int             nRetCode    = 0;
    MYSQL_RES*      pQueryRes   = NULL;
    MYSQL_ROW       QueryRow    = NULL;
    DWORD           dwTongID    = KD_BAD_ID;
    unsigned long*  pulFieldLen = NULL;
    int             nTongCount  = 0;

    QLogPrintf(LOG_DEBUG, "[DB] tong table loading ... ...");

    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
        "select ID, Data from %s",
        TONG_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        pulFieldLen = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(pulFieldLen);

        dwTongID = (DWORD)strtoul(QueryRow[0], NULL, 10);
        bRetCode = g_pGameCenter->m_TongManager.LoadTongData(
            dwTongID, 
            (BYTE*)QueryRow[1], (size_t)pulFieldLen[1]
        );

        LOG_PROCESS_ERROR(bRetCode);

        ++nTongCount;
    }

    QLogPrintf(LOG_DEBUG, "[DB] %d tongs loaded !", nTongCount);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KDBTools::LoadRankTable()
{
	BOOL            bResult     = false;
	BOOL            bRetCode    = false;
	int             nRetCode    = 0;
	MYSQL_RES*      pQueryRes   = NULL;
	MYSQL_ROW       QueryRow    = NULL;
	unsigned long*  pulFieldLen = NULL;
	INT				nRankId		= 0;
	INT				nRankCount	= 0;

	QLogPrintf(LOG_DEBUG, "[DB] rank table loading ... ...");

	nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
		"select RankID, Data from %s",
		RANKPOOL_TABLE_NAME
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

	bRetCode = DoQuery(m_szSQL);
	LOG_PROCESS_ERROR(bRetCode);

	pQueryRes = mysql_use_result(m_pDBHandle);
	LOG_PROCESS_ERROR(pQueryRes);

	while (true)
	{
		QueryRow = mysql_fetch_row(pQueryRes);
		if (!QueryRow)
			break;

		pulFieldLen = mysql_fetch_lengths(pQueryRes);
		LOG_PROCESS_ERROR(pulFieldLen);

		nRankId = (INT)strtoul(QueryRow[0], NULL, 10);
		bRetCode = g_pGameCenter->m_RankManager.LoadRankData(
			nRankId, 
			(BYTE*)QueryRow[1], (size_t)pulFieldLen[1]	
		);

		LOG_PROCESS_ERROR(bRetCode);

		++nRankCount;
	}

	QLogPrintf(LOG_DEBUG, "[DB] %d rankpool loaded !", nRankCount);

	bResult = true;
EXIT0:
	if (pQueryRes)
	{
		mysql_free_result(pQueryRes);
		pQueryRes = NULL;
	}
	return bResult;
}


#define LOAD_GLOBAL_SYSTEM_VALUE(name) \
    GetGlobalSystemValue(&(g_pGameCenter->m_GlobalSystemValueManager.m_##name.nValue), #name)

//BOOL KDBTools::LoadGlobalSystemValue()
//{
//    BOOL    bResult     = false;
//    int     nRetCode    = 0;
//    int     nCampScore  = 0;
//
//    nRetCode = GetGlobalSystemValue(&nCampScore, CAMP_SCORE_IN_DB);
//    LOG_PROCESS_ERROR(nRetCode != -1);
//    if (nRetCode == 1)
//    {
//        g_pGameCenter->m_CampManager.AddScore(nCampScore);
//    }
//
//    nRetCode = LOAD_GLOBAL_SYSTEM_VALUE(GameWorldStartTime);
//    LOG_PROCESS_ERROR(nRetCode != -1);
//    if (nRetCode == 0)
//    {
//        g_pGameCenter->m_GlobalSystemValueManager.m_GameWorldStartTime.nValue = (int)time(NULL);
//        g_pGameCenter->m_GlobalSystemValueManager.m_GameWorldStartTime.bUpdate = true;
//    }
//
//    nRetCode = LOAD_GLOBAL_SYSTEM_VALUE(MaxPlayerLevel);
//    LOG_PROCESS_ERROR(nRetCode != -1);
//
//    nRetCode = LOAD_GLOBAL_SYSTEM_VALUE(GameWorldChargeTime);
//    LOG_PROCESS_ERROR(nRetCode != -1);
//
//    bResult = true;
//EXIT0:
//    return bResult;
//}

#undef LOAD_GLOBAL_SYSTEM_VALUE

BOOL KDBTools::LoadGlobalMailTable()
{
    BOOL            bResult          = false;
    BOOL            bRetCode         = false;
    int             nRetCode         = 0;
    MYSQL_RES*      pQueryRes        = NULL;
    MYSQL_ROW       QueryRow         = NULL;
    int             nVersion         = 0;
    DWORD           dwMailID         = 0;
    unsigned long*  pulFieldLen      = NULL;
    time_t          nEndTime         = 0;
    int             nGlobalMailCount = 0;

    QLogPrintf(LOG_DEBUG, "[DB] GlobalMail loading ... ...");

    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
        "select Version, MailIndex, UNIX_TIMESTAMP(EndTime), MailInfo from %s",
        GLOBAL_MAIL_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        pulFieldLen = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(pulFieldLen);

        nVersion = atoi(QueryRow[0]);
        dwMailID = (DWORD)strtoul(QueryRow[1], NULL, 10);
        nEndTime = strtol(QueryRow[2], NULL, 10);

        bRetCode = g_pGameCenter->m_MailManager.LoadGlobalMail(
            nVersion, dwMailID, nEndTime, (BYTE*)QueryRow[3], (size_t)pulFieldLen[3]
        );
        LOG_PROCESS_ERROR(bRetCode);

        nGlobalMailCount++;
    }

    QLogPrintf(LOG_DEBUG, "[DB] %d GlobalMails loaded !", nGlobalMailCount);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

//BOOL KDBTools::LoadGlobalCustomDataTable()
//{
//    BOOL            bResult          = false;
//    BOOL            bRetCode         = false;
//    int             nRetCode         = 0;
//    MYSQL_RES*      pQueryRes        = NULL;
//    MYSQL_ROW       QueryRow         = NULL;
//    unsigned long*  pulFieldLen      = NULL;
//    int             nGlobalDataCount = 0;
//
//    QLogPrintf(LOG_DEBUG, "[DB] GlobalCustomData loading ... ...");
//
//    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
//        "select Name, Value from %s",
//        GLOBAL_CUSTOM_DATA_TABLE_NAME
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));
//
//    bRetCode = DoQuery(m_szSQL);
//    LOG_PROCESS_ERROR(bRetCode);
//
//    pQueryRes = mysql_use_result(m_pDBHandle);
//    LOG_PROCESS_ERROR(pQueryRes);
//
//    while (true)
//    {
//        QueryRow = mysql_fetch_row(pQueryRes);
//        if (!QueryRow)
//            break;
//
//        pulFieldLen = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLen);
//
//        bRetCode = g_pGameCenter->m_GlobalCustomDataManager.Add(
//            QueryRow[0], (BYTE*)QueryRow[1], (size_t)pulFieldLen[1], true
//        );
//        LOG_PROCESS_ERROR(bRetCode);
//
//        nGlobalDataCount++;
//    }
//
//    QLogPrintf(LOG_DEBUG, "[DB] %d GlobalCustomData loaded !", nGlobalDataCount);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    return bResult;
//}

BOOL KDBTools::LoadStatDataTable()
{
    BOOL            bResult          = false;
    BOOL            bRetCode         = false;
    int             nRetCode         = 0;
    MYSQL_RES*      pQueryRes        = NULL;
    MYSQL_ROW       QueryRow         = NULL;
    unsigned long*  pulFieldLen      = NULL;
    int             nNameCount       = 0;
    int             nNameID          = 0;
    time_t          nTime            = 0;
    time_t          nStatTime        = 0;

    QLogPrintf(LOG_DEBUG, "[DB] StatDataNameIndex loading ... ...");

    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
        "select Name, ID from %s",
        STAT_DATA_NAME_INDEX_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        nNameID = (int)strtol(QueryRow[1], NULL, 10);
        bRetCode = g_pGameCenter->m_StatDataManager.LoadDataName(QueryRow[0], nNameID);
        LOG_PROCESS_ERROR(bRetCode);

        nNameCount++;
    }

    QLogPrintf(LOG_DEBUG, "[DB] %d StatDataName loaded !", nNameCount);

    nTime = time(NULL);
    nStatTime = (nTime / 3600) * 3600;

    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
        "select StatTime, StatData from %s where StatTime = FROM_UNIXTIME(%ld)",
        STAT_DATA_TABLE_NAME, nStatTime
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    bRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(bRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        pulFieldLen = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(pulFieldLen);

        bRetCode = g_pGameCenter->m_StatDataManager.LoadData(
            nStatTime, (BYTE*)QueryRow[1], (size_t)pulFieldLen[1]
        );
        LOG_PROCESS_ERROR(bRetCode);
    }

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

//BOOL KDBTools::LoadActivityTable()
//{
//    BOOL            bResult         = false;
//    BOOL            bRetCode        = false;
//    int             nRetCode        = 0;
//    MYSQL_RES*      pQueryRes       = NULL;
//    MYSQL_ROW       QueryRow        = NULL;
//    unsigned long*  pulFieldLen     = NULL;
//    int             nActivityCount  = 0;
//    KActivityRecord Record;
//
//    QLogPrintf(LOG_DEBUG, "[DB] Activity data loading ... ...");
//
//    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
//        " select ID, Type, UNIX_TIMESTAMP(Time), Value0, Value1, Value2, Value3, Value4, Value5 from %s; ",
//        ACTIVITY_TABLE_NAME
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));
//
//    bRetCode = DoQuery(m_szSQL);
//    LOG_PROCESS_ERROR(bRetCode);
//
//    pQueryRes = mysql_use_result(m_pDBHandle);
//    LOG_PROCESS_ERROR(pQueryRes);
//
//    while (true)
//    {
//        QueryRow = mysql_fetch_row(pQueryRes);
//        if (!QueryRow)
//            break;
//
//        pulFieldLen = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLen);
//
//        Record.dwRoleID     = strtoul(QueryRow[0], NULL, 10);
//        Record.nType        = strtol(QueryRow[1], NULL, 10);
//        Record.nEndTime     = strtol(QueryRow[2], NULL, 10);
//        Record.anValue[0]   = strtol(QueryRow[3], NULL, 10);
//        Record.anValue[1]   = strtol(QueryRow[4], NULL, 10);
//        Record.anValue[2]   = strtol(QueryRow[5], NULL, 10);
//        Record.anValue[3]   = strtol(QueryRow[6], NULL, 10);
//        Record.anValue[4]   = strtol(QueryRow[7], NULL, 10);
//        Record.anValue[5]   = strtol(QueryRow[8], NULL, 10);
//        Record.bUpdate      = false;
//
//        bRetCode = g_pGameCenter->m_ActivityManager.Add(Record);
//        LOG_PROCESS_ERROR(bRetCode);
//
//        nActivityCount++;
//    }
//
//    QLogPrintf(LOG_DEBUG, "[DB] %d Activity data loaded !", nActivityCount);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    return bResult;
//}
//
//BOOL KDBTools::LoadAntiFarmerTable()
//{
//    BOOL            bResult         = false;
//    BOOL            bRetCode        = false;
//    int             nRetCode        = 0;
//    MYSQL_RES*      pQueryRes       = NULL;
//    MYSQL_ROW       QueryRow        = NULL;
//    unsigned long*  pulFieldLen     = NULL;
//    const char*     pszAccount      = NULL;
//    size_t          uStrLen         = 0;
//    time_t          nPunishEndTime  = 0;
//    int             nCount          = 0;
//
//    QLogPrintf(LOG_DEBUG, "[DB] AntiFarmer data loading ... ...");
//
//    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
//        " select Account, unix_timestamp(PunishEndTime) from %s; ",
//        ANTI_FARMER_TABLE_NAME
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));
//
//    bRetCode = DoQuery(m_szSQL);
//    LOG_PROCESS_ERROR(bRetCode);
//
//    pQueryRes = mysql_use_result(m_pDBHandle);
//    LOG_PROCESS_ERROR(pQueryRes);
//
//    while (true)
//    {
//        QueryRow = mysql_fetch_row(pQueryRes);
//        if (!QueryRow)
//            break;
//
//        pulFieldLen = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLen);
//
//        pszAccount = QueryRow[0];
//        LOG_PROCESS_ERROR(pszAccount);
//
//        uStrLen = strlen(pszAccount);
//        LOG_PROCESS_ERROR(uStrLen < NAME_LEN);
//
//        nPunishEndTime = strtol(QueryRow[1], NULL, 10);
//
//        bRetCode = g_pGameCenter->m_AntiFarmerManager.AddFarmerRecord(pszAccount, nPunishEndTime);
//        LOG_PROCESS_ERROR(bRetCode);
//
//        ++nCount;
//    }
//
//    QLogPrintf(LOG_DEBUG, "[DB] %d AntiFarmer data loaded !", nCount);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    return bResult;
//}
//
//BOOL KDBTools::LoadMentorTable()
//{
//    BOOL            bResult         = false;
//    int             nRetCode        = 0;
//    MYSQL_RES*      pQueryRes       = NULL;
//    MYSQL_ROW       QueryRow        = NULL;
//    unsigned long*  pulFieldLen     = NULL;
//    DWORD           dwMentorID      = KD_BAD_ID;
//    DWORD           dwApprenticeID  = KD_BAD_ID;
//    int             nMentorCount    = 0;
//
//    QLogPrintf(LOG_DEBUG, "[DB] Mentor data loading ... ...");
//
//    nRetCode = snprintf(m_szSQL, sizeof(m_szSQL),
//        " select MentorID, ApprenticeID, Data from %s; ",
//        MENTOR_TABLE_NAME
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));
//
//    nRetCode = DoQuery(m_szSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    pQueryRes = mysql_use_result(m_pDBHandle);
//    LOG_PROCESS_ERROR(pQueryRes);
//
//    while (true)
//    {
//        BYTE*   pbyData     = NULL;
//        size_t  uDataLen    = 0;
//
//        QueryRow = mysql_fetch_row(pQueryRes);
//        if (!QueryRow)
//            break;
//
//        pulFieldLen = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLen);
//
//        uDataLen = (size_t)pulFieldLen[2];
//
//        dwMentorID      = (DWORD)strtoul(QueryRow[0], NULL, 10);
//        dwApprenticeID  = (DWORD)strtoul(QueryRow[1], NULL, 10);
//        pbyData         = (BYTE*)QueryRow[2];
//
//        nRetCode = g_pGameCenter->m_MentorCenter.Load(
//            dwMentorID, dwApprenticeID, pbyData, uDataLen
//        );
//        LOG_PROCESS_ERROR(nRetCode);
//
//        nMentorCount++;
//    }
//
//    QLogPrintf(LOG_DEBUG, "[DB] %d pairs of Mentor records loaded !", nMentorCount);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    return bResult;
//}

BOOL KDBTools::LoadRenameRoleTable()
{
    BOOL        bResult     = false;
    int         nRetCode    = 0;
    MYSQL_RES*  pQueryRes   = NULL;
    MYSQL_ROW   QueryRow    = NULL;
    DWORD       dwRoleID    = KD_BAD_ID;
    int         nCount      = 0;

    QLogPrintf(LOG_DEBUG, "[DB] RenameRole data loading ... ...");

    nRetCode = snprintf(
        m_szSQL, sizeof(m_szSQL), " select RoleID from %s; ",
        RENAME_ROLE_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    nRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        BYTE*   pbyData     = NULL;
        size_t  uDataLen    = 0;

        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        dwRoleID = (DWORD)strtoul(QueryRow[0], NULL, 10);
        g_pGameCenter->m_RoleManager.RecordRenameRole(dwRoleID);

        nCount++;
    }

    QLogPrintf(LOG_DEBUG, "[DB] %d RenameRole records loaded !", nCount);
    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

int KDBTools::GetGlobalSystemValue(int* pnRetValue, const char cszKey[])
{
    int             nResult     = -1;
    int             nRetCode    = 0;
    MYSQL_RES*      pQueryRes   = NULL;
    MYSQL_ROW       QueryRow    = NULL;
    my_ulonglong    ullRows     = 0;

    assert(pnRetValue);

    nRetCode = snprintf(
        m_szSQL, sizeof(m_szSQL),
        " select Value from %s where Name = '%s'; ",
        GLOBAL_SYSTEM_VALUE_TABLE_NAME, cszKey
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(m_szSQL));

    nRetCode = DoQuery(m_szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    ullRows = mysql_num_rows(pQueryRes);
    if (ullRows == 0)
    {
        nResult = 0;
        goto EXIT0;
    }

    LOG_PROCESS_ERROR(ullRows == 1);
    
    QueryRow = mysql_fetch_row(pQueryRes);
    LOG_PROCESS_ERROR(QueryRow);
    
    *pnRetValue = (int)strtol(QueryRow[0], NULL, 10);

    nResult = 1;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return nResult;
}

BOOL KDBTools::DoQuery(const char cszSQL[])
{
    BOOL     bResult            = false;
    BOOL     bRetCode           = false;
    unsigned ulMySQLErrorCode   = 0;

    LOG_PROCESS_ERROR(cszSQL);
    LOG_PROCESS_ERROR(m_pDBHandle);
    
    bRetCode = MySQL_Query(m_pDBHandle, cszSQL, &ulMySQLErrorCode);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    if (ulMySQLErrorCode != 0 && ulMySQLErrorCode != ER_DUP_ENTRY) // 主键冲突不用断开链接
    {
        MySQL_Disconnect(m_pDBHandle);
       	m_pDBHandle = NULL;
    }
    return bResult;
}

BOOL KDBTools::CheckGMSendAwardTale()
{

	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"CREATE TABLE  IF NOT EXISTS `GMCmdAWARD`(id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,playerID INT UNSIGNED NOT NULL ,mark INT UNSIGNED NOT NULL  ,GMCMD CHAR(255) NOT NULL,LastTime DATETIME,GMType INT DEFAULT NULL,InsertTime timestamp DEFAULT CURRENT_TIMESTAMP);"
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));
	//QLogPrintf(LOG_DEBUG,m_szSQL);
	bRetCode = DoQuery(m_szSQL);

	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			GM_SEND_AWARD_TABLE_NAME
			);
		goto EXIT0;
	}
	bResult = TRUE;
EXIT0:
	return bResult;
}


BOOL KDBTools::CheckPayListTable()
{

	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"CREATE TABLE  IF NOT EXISTS `pay_list` ("
		"`id` int(10) unsigned NOT NULL AUTO_INCREMENT,"
		"`money` decimal(6,2) NOT NULL,"
		"`account` char(32) NOT NULL,"
		"`orderno` char(32) CHARACTER SET latin1 COLLATE latin1_general_ci NOT NULL,"
		"`paytime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,"
		"`serverid` int(11) NOT NULL,"
		"`platform` char(32) NOT NULL,"
		"`paytype` tinyint(4) NOT NULL DEFAULT '0',"
		"`comment` char(32) DEFAULT NULL,"
		"`comment2` char(32) DEFAULT NULL,"
		"PRIMARY KEY (`id`)"
		") ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;"
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));
	QLogPrintf(LOG_DEBUG,m_szSQL);
	bRetCode = DoQuery(m_szSQL);

	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			"pay_list"
			);
		goto EXIT0;
	}
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KDBTools::CheckSaveOpenseTable()
{

	BOOL                bResult     = false;
	BOOL                bRetCode    = 0;
	int                 nStrLen     = 0;

	nStrLen = snprintf(m_szSQL, sizeof(m_szSQL), 
		"CREATE TABLE  IF NOT EXISTS `OpenseSave`(id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,day INT UNSIGNED NOT NULL ,mark INT UNSIGNED NOT NULL ,LastTime DATETIME);"
		);
	LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(m_szSQL));
	QLogPrintf(LOG_DEBUG,m_szSQL);
	bRetCode = DoQuery(m_szSQL);

	if (!bRetCode)
	{
		QLogPrintf(
			LOG_ERR, 
			"DB ERROR when try to create table \'%s\'\n", 
			GM_SEND_AWARD_TABLE_NAME
			);
		goto EXIT0;
	}
	bResult = TRUE;
EXIT0:
	return bResult;
}
