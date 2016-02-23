////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGD_MySQL_Database.h
//  Version     : 1.0
//  Creater     : Wu Caizhong
//  Create Date : 2005-01-24 16:14:59
//  Comment     : Internal use
//                Do no use as interface, us KGD_Interface.h as interface file.
//
//  MySQL Server config require: 
//     1. set lower_case_table_names = 1 to make table names are stored 
//        in lowercase on disk and name comparisons are not case sensitive.
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGD_MYSQL_DATABASE_H_
#define _INCLUDE_KGD_MYSQL_DATABASE_H_

#include <vector>
#include <map>
#include "Misc/IDatabase.h"
#include "KGD_Name.h"
#include "KGD_MySQL_Field.h"
#include "KGD_MySQL_Result.h"
#include "Core/QMemory.h"
#include "mysql.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
struct KGD_MYSQL_FIELD_PAIR
{
    const KGD_MySQL_Field  *pcMySQLField;
    const KGD_FIELD        *pcKGDField;
};
typedef vector<KGD_MYSQL_FIELD_PAIR> KGD_MySQL_FieldPairVector;

typedef map<KGD_Name, KGD_MySQL_Field> KGD_MySQL_FieldMap;

// table information
struct KGD_MYSQL_TABLE
{
    char               szName[KGD_NAME_BUFFER_MAX_SIZE];
    KGD_MySQL_FieldMap FieldMap;
};
typedef map<KGD_Name, KGD_MYSQL_TABLE> KGD_MySQL_TableMap; 

////////////////////////////////////////////////////////////////////////////
class KGD_MySQL_Database : public IKGD_Database
{
public:    
    ////////////////////////////////////////////////////////////////////////////
    //                               interface function                       //
    ////////////////////////////////////////////////////////////////////////////
    HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject
        )
    {
        USE_ARGUMENT(riid);
        USE_ARGUMENT(ppvObject);
        return E_FAIL;
    }
    ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return (unsigned long)InterlockedIncrement((long *)&m_ulRefCount);
    }

    ULONG STDMETHODCALLTYPE Release(void)
    {
        unsigned long ulCount = InterlockedDecrement((long *)&m_ulRefCount);
        if (ulCount == 0)
        {
            if (m_pMySQL)
            {
                _Close();
                m_pMySQL = NULL;
            }
            delete this;
        }
        return ulCount;
    }
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Create new database.
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int CreateDatabase(
        const char cszDatabaseName[KGD_NAME_BUFFER_MAX_SIZE], 
        int nCreateFlag
    );
    
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Remove an exist database.
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int RemoveDatabase(
        const char cszDatabaseName[KGD_NAME_BUFFER_MAX_SIZE]
    );    
    
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Set current database,
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int SetCurrentDatabase(
        const char cszDatabaseName[KGD_NAME_BUFFER_MAX_SIZE]
    );        

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Create a new table in current database
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int CreateTable(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE],
        unsigned uFieldCount,
        const KGD_FIELD caFields[],
        int nCreateFlag
    );
    
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Remove table from current database
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int RemoveTable(const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE]);

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Add new fields to the table of current database.
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int AddField(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
        unsigned uFieldCount,
        const KGD_FIELD caFields[] 
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Remove fields from table of current database.
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int RemoveField(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
        unsigned uFieldCount,
        const KGD_FIELD caFields[] 
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Query record from table of current database
    //     SELECT (caResultFields[0].szName[, caResultFields[1].szName, ...])
    //     FROM cszTableName
    //     [WHERE pcMatchField1.szName Operator1 pcMatchField1.pvData 
    //        [AND pcMatchField2.szName Operator2 pcMatchField2.pvData]
    //     ]
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int Query(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
        unsigned uResultFieldCount,
        const KGD_FIELD caResultFields[], 
        const KGD_FIELD *pcMatchField1, 
        KGD_OPERATOR Operator1,
        const KGD_FIELD *pcMatchField2, 
        KGD_OPERATOR Operator2
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Get query result of success call of Query()
    // @remark : interface function, see more information in KGD_Interface.h
    virtual IKGD_Result *GetQueryResult(KGD_RESULT_FLAG Flag);

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Update record for specify table of current database
    //     UPDATE cszTableName
    //     SET caBeUpdateFields[0].szName = caBeUpdateFields[0].pvData
    //         [, caBeUpdateFields[1].szName = caBeUpdateFields[1].pvData, ...]
    //     [WHERE pcMatchField1.szName Operator1 pcMatchField1.pvData 
    //        [AND pcMatchField2.szName Operator2 pcMatchField2.pvData]
    //     ]
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int Update(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
        unsigned uFieldCount,
        const KGD_FIELD caBeUpdateFields[],
        const KGD_FIELD *pcMatchField1, 
        KGD_OPERATOR Operator1,
        const KGD_FIELD *pcMatchField2, 
        KGD_OPERATOR Operator2
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Delete record from specify table of current database
    //     DELETE FROM cszTableName 
    //     [WHERE pcMatchField1.szName Operator1 pcMatchField1.pvData 
    //        [AND pcMatchField2.szName Operator2 pcMatchField2.pvData]
    //     ]
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int Delete(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE],
        const KGD_FIELD *pcMatchField1, 
        KGD_OPERATOR Operator1,
        const KGD_FIELD *pcMatchField2, 
        KGD_OPERATOR Operator2
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Insert new record into specify table of current database
    //   INSERT INTO cszTableName(caFields[0].szName[, caFields[1].szName, ...])
    //   VALUES(caFields[0].pvData[, caFields[1].pvData, ...])
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int Insert(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
        unsigned uFieldCount, 
        const KGD_FIELD caFields[]
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Returns the affected row number by last call of Insert(), 
    //           Delete() or Update()
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int GetAffectedRow();

    ////////////////////////////////////////////////////////////////////////////
    // @return : Last error code. see more at define of KGD_ERROR_CODE
    virtual int GetLastErrorCode() { return m_LastErrorCode; };

public:
    KGD_MySQL_Database();
    ////////////////////////////////////////////////////////////////////////////
    //                                internal use                            //
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Connect to database server
    // @param cszServerName       : [in] specify database server. 
    // @param usPort              : [in] If port is not 0, the value will be 
    //                                   used as the port number for the TCP/IP 
    //                                   connection. Note that the pcszHost
    //                                   parameter determines the type of 
    //                                   the connection. 
    // @param cszUserName         : [in] User's login ID. If user is NULL or the
    //                                   empty string "", current user assumed. 
    // @param cszPassword         : [in] specify password for User.
    // @param pcszCurrentDatabase : [in] set current database after connected.
    //                                   Can be NULL, if set to NULL, you should
    //                                   call IKGD_Database::SetCurrentDatabase
    //                                   before do anything relate to a table.
    // @return : true if successful, false if failed.
    int Connect(
        const char cszServerName[], 
        unsigned short usPort, 
        const char cszUserName[], 
        const char cszPassword[],
        const char *pcszCurrentDatabase
    );

private: 
    ~KGD_MySQL_Database();     // make is only can be create by new operator
                                // and only can be destroy by Release()
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Close connection to database server.
    // @return : true if successful, false if failed.
    int _Close();

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Load all tables information of current database
    // @return : true if successful, false if failed.
    int _LoadAllTables();

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Load information of one table
    // @param cszTableName : [in] name of table.
    // @return : true if successful, false if failed.
    // @remark : if return false, usually means that, this table was be create 
    //           by another way (maybe by mysql.exe), it contain some type or
    //           name not support by this interface.
    int _LoadTable(const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE]);
    
    ////////////////////////////////////////////////////////////////////////////
    // @brief : map interface param to internal structure
    // @param cszTableName     : [in] Table name
    // @param uFieldCount      : [in] Element count of param pFields.  
    // @param pFields          : [in] An array, if uFieldCount > 0, 
    //                                 should set element's szName
    // @param paFieldPair      : [out] Be ignore if uFieldCount == 0 
    // @param pcMatchField1    : [in] if not NULL, should set szName
    // @param pMatchFieldPair1 : [out] can't NULL, if pcMatchField1 is NULL
    //                                 will set it's member pointer to NULL
    // @param pcMatchField2    : [in] if not NULL, set szName 
    // @param pMatchFieldPair2 : [out] can't NULL, if pcMatchField1 is NULL
    //                                 will set it's member pointer to NULL 
    // @return : true if successful, false if failed.
    int _MapParam(
        const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE],

        unsigned uFieldCount, 
        const KGD_FIELD *pFields,
        KGD_MySQL_FieldPairVector *paFieldPair,

        const KGD_FIELD *pcMatchField1, 
        KGD_MYSQL_FIELD_PAIR *pMatchFieldPair1,

        const KGD_FIELD *pcMatchField2, 
        KGD_MYSQL_FIELD_PAIR *pMatchFieldPair2
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Create buffer to store query string. 
    //           buffer size is count from the fowling parameter
    //           include field name, field data, operator and append size
    // @param paFieldPair        : [in] Point to  vector contain field pair.
    //                                  can be NULL.
    // @param pcMatchFieldPair1  : [in] can be NULL.
    // @param Operator1          : [in] operator associate with match file 1
    // @param pcMatchFieldPair2  : [in] can be NULL.
    // @param Operator2          : [in] operator associate with match file 2
    // @param uAppendSize        : [in] append size of buffer
    // @return : if success, return point to interface IMemBlock
    //           if failed,  return NULL.
    // @remark : call IGK_Buffer::Release() to free buffer.
    IMemBlock *_CreateBuffer(
        const KGD_MySQL_FieldPairVector *paFieldPair,
        const KGD_MYSQL_FIELD_PAIR *pcMatchFieldPair1,
        KGD_OPERATOR Operator1,
        const KGD_MYSQL_FIELD_PAIR *pcMatchFieldPair2,
        KGD_OPERATOR Operator2,
        unsigned uAppendSize
    );
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store where... condition to buffer. (NULL terminal string.)
    //     [WHERE crMatchFieldPair1.pcKGDFiel->szName Operator1 
    //            crMatchFieldPair1.pcKGDFiel->pvData 
    //         [AND crMatchFieldPair2.pcKGDFiel->szName Operator2 
    //              crMatchFieldPair2.pcKGDFiel->pvData 
    //         ]
    //     ]
    // @param pszWritePos       : [in] Specify buffer to store string.
    //                                 caller must insure buffer size is enough.
    // @param crMatchFieldPair1 : [in] match field pair1
    // @param Operator1         : [in] operator associate with match file 1
    // @param crMatchFieldPair2 : [in] match field pair2
    // @param Operator2         : [in] operator associate with match file 1
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    int _StoreWhereCondition(
        char *pszWritePos,
        const KGD_MYSQL_FIELD_PAIR &crMatchFieldPair1,
        KGD_OPERATOR Operator1,
        const KGD_MYSQL_FIELD_PAIR &crMatchFieldPair2,
        KGD_OPERATOR Operator2
    );
    // @brief : Store single condition to buffer. (NULL terminal string.)
    //     [WHERE crMatchFieldPair.pcKGDFiel->szName Operator 
    //            crMatchFieldPair.pcKGDFiel->pvData 
    // @param pszWritePos       : [in] Specify buffer to store string.
    //                                 caller must insure buffer size is enough.
    // @param crMatchFieldPair1 : [in] match field pair1
    // @param Operator1         : [in] operator associate with match file 1
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    int _StoreSingleCondition(
        char *pszWritePos,
        const KGD_MYSQL_FIELD_PAIR &crMatchFieldPair,
        KGD_OPERATOR Operator
    );
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store description string of field to buffer.
    // @param pszWritePos : [in] Specify buffer to store string.
    //                            caller should insure buffer size is enough.
    // @param crField     : [in] field want to get description.
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    // @remark : 1.call by CreateTable and AddField(), 
    //           2.static function, not set m_LastErrorCode member.
    int _StoreFieldDefiniens(
        char *pszWritePos,
        const KGD_FIELD &crField 
    );
    
    ////////////////////////////////////////////////////////////////////////////
    // @brief : send query to server, if connect lost, reconnect.
    // @return : true if successful, false if failed.
    int _DoQuey(const char cszQuery[], unsigned long ulLength);

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Get MySQL server variables of this connection
    // @param szVariablesName : [in] variables you want get
    // @param uRetBufferSize  : [in] buffer size to store result
    // @param pszRetBuffer    : [out] NULL terminal string contain value
    // @return : true if successful, false if failed.
    int _GetServerVariable(
        const char cszVariablesName[], 
        unsigned uRetBufferSize,
        char *pszRetBuffer
    );

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Check database character set with server character set.
    // @return : true if equal, false if failed.
    int _CheckDatabaseCharacterSet();
private:
    KGD_ERROR_CODE      m_LastErrorCode;
    KGD_MySQL_TableMap  m_TableMap;
    MYSQL              *m_pMySQL;
    unsigned long       m_ulRefCount;
    char                m_szCharacterSet[64];
};

////////////////////////////////////////////////////////////////////////////////
// @brief : initialize runtime environment 
// @remark : interface function, see more information in KGD_Interface.h
int KGD_MySQL_Init(void *pvContext);

////////////////////////////////////////////////////////////////////////////////
// @brief : 
// @remark : interface function, see more information in KGD_Interface.h
int KGD_MySQL_Uninit(void *pvContext);

#endif //_INCLUDE_KGD_MYSQL_DATABASE_H_
