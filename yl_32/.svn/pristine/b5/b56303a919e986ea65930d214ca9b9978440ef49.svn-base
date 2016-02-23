////////////////////////////////////////////////////////////////////////////////
//  FileName    : KGD_MySQL_Result.cpp
//  Version     : 1.0
//  Creater     : Wu Caizhong
//  Create Date : 2005-02-16 19:40:12
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KGD_Error.h"
#include "KGD_MySQL_Field.h"
#include "KGD_MySQL_Result.h"


////////////////////////////////////////////////////////////////////////////////
//                            interface function                              //
////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Result::NextRow()
{
    int nResult  = false;
    unsigned long *pulLengths = NULL;
    unsigned i = 0;

    m_LastErrorCode = KGD_ERROR_UNKNOW; 
    QDB_PROCESS_ERROR(m_pResultSet, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);
    
    // get next row
    m_CurRow = mysql_fetch_row(m_pResultSet);
    //QDB_PROCESS_ERROR(m_CurRow, KGD_ERROR_NO_MORE_ROW);
    if (!m_CurRow)         // not log this error  
    {                                                                      
        m_LastErrorCode = KGD_ERROR_NO_MORE_ROW;                                  
        PROCESS_ERROR(false);                                                       
    };   
    // get next row field list
    pulLengths  = mysql_fetch_lengths(m_pResultSet);
    for (i = 0; i < m_uFieldCount; ++i)
    {
        switch (m_aFields[i].Type) 
        {
        case KGD_TYPE_INT32:
            m_aNumericFieldData[i] = atoi(m_CurRow[i]);
            m_aFields[i].pvData = &m_aNumericFieldData[i];
            m_aFields[i].uDataSize = KGD_INT32_SIZE;
            break;
        case KGD_TYPE_UNSIGNED_INT32:
            m_aNumericFieldData[i] = strtoul(m_CurRow[i], NULL, 10);
            m_aFields[i].pvData = &m_aNumericFieldData[i];
            m_aFields[i].uDataSize = KGD_UNSIGNED_INT32_SIZE;
            break;
        case KGD_TYPE_INT64:
            #ifdef WIN32
                m_aNumericFieldData[i] = _atoi64(m_CurRow[i]);
            #else
                m_aNumericFieldData[i] = atoll(m_CurRow[i]);
            #endif
            m_aFields[i].pvData = &m_aNumericFieldData[i];
            m_aFields[i].uDataSize = KGD_INT64_SIZE;
            break;
        case KGD_TYPE_UNSIGNED_INT64:
            #ifdef WIN32
                m_aNumericFieldData[i] = _strtoui64(m_CurRow[i], NULL, 10);
            #else
                m_aNumericFieldData[i] = strtoull(m_CurRow[i], NULL, 10);
            #endif
            m_aFields[i].pvData = &m_aNumericFieldData[i];
            m_aFields[i].uDataSize = KGD_UNSIGNED_INT64_SIZE;
            break;
        case KGD_TYPE_CHAR:
        case KGD_TYPE_VARCHAR:
        case KGD_TYPE_TEXT:
        case KGD_TYPE_DATETIME:
            m_aFields[i].pvData = m_CurRow[i];
            if (m_CurRow[i])
                m_aFields[i].uDataSize = pulLengths[i] + 1;  // plus '\0'
            else
                m_aFields[i].uDataSize = 0;  // is NULL field
            break;
        case KGD_TYPE_BLOB:
            m_aFields[i].pvData    = m_CurRow[i];
            m_aFields[i].uDataSize = pulLengths[i];
            break;

        default:
            QDB_PROCESS_ERROR(false, KGD_ERROR_INVALID_TYPE);
        }
    }

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (m_LastErrorCode != KGD_ERROR_NO_MORE_ROW)
        {
            QLogPrintf(
                LOG_DEBUG, 
                "[KGD] (0x%p)->KGD_MySQL_Database (0x%p)->%s()", 
                m_piDatabase, this, __FUNCTION__
            );  
        }
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Result::GetCurrentRowField(
    unsigned uIndex, 
    KGD_FIELD *pRetField
)
{
    int nResult = false;

    m_LastErrorCode = KGD_ERROR_UNKNOW;

    QDB_PROCESS_ERROR(m_pResultSet, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);
    QDB_PROCESS_ERROR(m_CurRow, KGD_ERROR_NO_MORE_ROW);

    QDB_PROCESS_ERROR(uIndex < m_uFieldCount, KGD_ERROR_INVALID_INDEX);
    QDB_PROCESS_ERROR(pRetField, KGD_ERROR_NULL_POINTER);
    
    pRetField->pvData     = m_aFields[uIndex].pvData;
    pRetField->uDataSize  = m_aFields[uIndex].uDataSize;

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->KGD_MySQL_Database (0x%p)->%s(%u, 0x%p)",
            m_piDatabase, this, __FUNCTION__, uIndex, pRetField
        );
    }
    return nResult;
}

int KGD_MySQL_Result::GetRowCount()
{
    int nResult = false;
    int nRetRowCount = -1;
    my_ulonglong ullRowCount = (my_ulonglong)-1;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pResultSet, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    ullRowCount = mysql_num_rows(m_pResultSet);
    QDB_PROCESS_ERROR(
        (my_ulonglong)-1 != ullRowCount, 
        KGD_ERROR_DATABASE_ENGINE_ERROR
    );
//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
    nRetRowCount = (int)ullRowCount;
EXIT0:
    if (!nResult)
    {
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->KGD_MySQL_Database (0x%p)->%s()", 
            m_piDatabase, this, __FUNCTION__
        );
    }
    return nRetRowCount;
}

////////////////////////////////////////////////////////////////////////////////
//                                internal use                                //
////////////////////////////////////////////////////////////////////////////////
KGD_MySQL_Result::KGD_MySQL_Result() : 
    m_LastErrorCode(KGD_ERROR_UNKNOW), 
    m_pResultSet(NULL), 
    m_uFieldCount(0),
    m_CurRow(NULL), 
    m_ulRefCount(1), 
    m_piDatabase(NULL)
{
    
}

////////////////////////////////////////////////////////////////////////////////
KGD_MySQL_Result::~KGD_MySQL_Result()
{
    ASSERT(!m_pResultSet);
    QDB_PROCESS_ERROR(!m_pResultSet, KGD_ERROR_RESOURCE_NOT_RELEASE);
EXIT0:
    if (m_pResultSet)
        _ReleaseResult();
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Result::StoreResult(
    KGD_RESULT_FLAG Flag, MYSQL *pMySQL, IKGD_Database *piDatabase
)
{
    int nResult  = false;
    int nRetCode = false;
    const MYSQL_FIELD *pcField = NULL;
    unsigned i = 0;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(!m_pResultSet, KGD_ERROR_RESOURCE_NOT_RELEASE);

    QDB_PROCESS_ERROR(pMySQL, KGD_ERROR_NULL_POINTER);
    QDB_PROCESS_ERROR(piDatabase, KGD_ERROR_NULL_POINTER);

    if (Flag & KGD_RESULT_STORE)
    {
        m_pResultSet = mysql_store_result(pMySQL);
        KGD_MYSQL_ENGINE_PROCESS_ERROR(m_pResultSet, pMySQL, "mysql_store_result()");
    }
    else
    {
        m_pResultSet = mysql_use_result(pMySQL);
        KGD_MYSQL_ENGINE_PROCESS_ERROR(m_pResultSet, pMySQL, "mysql_use_result()");
    }

    // get field count
    m_uFieldCount = mysql_num_fields(m_pResultSet);    
    ASSERT(m_uFieldCount > 0);
    QDB_PROCESS_ERROR(m_uFieldCount < KGD_MAX_FIELD_COUNT, KGD_ERROR_TOO_MANY_FIELD);

    // get field type
    pcField = mysql_fetch_fields(m_pResultSet); // function not return error

    for (i = 0; i < m_uFieldCount; ++i)
    {
        ASSERT(pcField);

        nRetCode = KGD_MySQL_Field::MySQLFieldTypeToKGDFieldType(
            pcField->type, pcField->flags, &m_aFields[i].Type
        );
        QDB_PROCESS_ERROR(nRetCode, KGD_ERROR_INVALID_TYPE);
        
        ++pcField;
    }

    m_piDatabase = piDatabase;
    m_piDatabase->AddRef();
//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (m_pResultSet)
        {
            mysql_free_result(m_pResultSet);
            m_pResultSet = NULL;
        }
        // log parameter
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->KGD_MySQL_Database (0x%p)->%s(%d, 0x%p, 0x%p)", 
            m_piDatabase, this, __FUNCTION__, Flag, pMySQL, piDatabase
        );
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Result::_ReleaseResult()
{
    int nResult = false;

    m_LastErrorCode = KGD_ERROR_UNKNOW;      
    QDB_PROCESS_ERROR(m_pResultSet, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    mysql_free_result(m_pResultSet);
    m_pResultSet  = NULL;
    m_uFieldCount = 0;
    m_CurRow      = NULL;

    SAFE_RELEASE(m_piDatabase);
//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    return nResult;
}




