////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGD_MySQL_Field.cpp
//  Version     : 1.0
//  Creater     : Wu Caizhong
//  Create Date : 2005-02-04 10:33:16
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <string.h>
#include "KGD_MySQL_Field.h"

static const unsigned KGD_INT32_STRING_MAX_SIZE = 12; // sizeof("-2147483648")
static const unsigned KGD_INT64_STRING_MAX_SIZE = 21; // sizeof("-9223372036854775808")
static const unsigned KGD_NULL_STRING_SIZE = sizeof("NULL");
static const unsigned NONSUPPORT_MYSQL_FLAG = 
    ENUM_FLAG | AUTO_INCREMENT_FLAG | TIMESTAMP_FLAG | SET_FLAG;

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::MySQLFieldTypeToKGDFieldType(
    enum_field_types MySQLType, unsigned uMySQLFlag, KGD_FIELD_TYPE *pKGDType
)
{
    int nResult  = false;

    PROCESS_ERROR(pKGDType);

    // nonsupport type
    PROCESS_ERROR(!(uMySQLFlag & NONSUPPORT_MYSQL_FLAG));

    // support type
    switch (MySQLType) 
    {
    case FIELD_TYPE_LONG:
        if (uMySQLFlag & UNSIGNED_FLAG)
        {
            *pKGDType = KGD_TYPE_UNSIGNED_INT32;
        }
        else
        {
            *pKGDType = KGD_TYPE_INT32;
        }
        break;

    case FIELD_TYPE_LONGLONG:
        if (uMySQLFlag & UNSIGNED_FLAG)
        {
            *pKGDType = KGD_TYPE_UNSIGNED_INT64;
        }
        else
        {
            *pKGDType = KGD_TYPE_INT64;
        }
        break;

    case FIELD_TYPE_STRING:
        if (uMySQLFlag & BINARY_FLAG)
        {
            //*pKGDType = KGD_TYPE_BINARY;
            PROCESS_ERROR(false); // not support type
        }
        else
        {
            *pKGDType = KGD_TYPE_CHAR;
        }
        break;

    case FIELD_TYPE_VAR_STRING:
        if (uMySQLFlag & BINARY_FLAG)
        {
            //*pKGDType = KGD_TYPE_VARBINARY;
            PROCESS_ERROR(false); // not support type
        }
        else
        {
            *pKGDType = KGD_TYPE_VARCHAR;
        }
        break;

    case FIELD_TYPE_BLOB:
        if (uMySQLFlag & BINARY_FLAG)
            *pKGDType = KGD_TYPE_BLOB;
        else
            *pKGDType = KGD_TYPE_TEXT;
        break;
    
    case FIELD_TYPE_DATETIME:
        *pKGDType = KGD_TYPE_DATETIME;
        break;

    default:
        PROCESS_ERROR(false); // not support type
    };
//EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::SetFieldInfo(
    const MYSQL_FIELD &MySQLField, 
    MYSQL *pMySQL
)
{
    int nResult  = false;
    int nRetCode = false;

    // type
    nRetCode = MySQLFieldTypeToKGDFieldType(
        MySQLField.type, MySQLField.flags, &m_KGDField.Type
    );
    PROCESS_ERROR(nRetCode);

    // name
    PROCESS_ERROR(MySQLField.name_length < KGD_NAME_BUFFER_MAX_SIZE);
    m_uKGDField_NameSize = MySQLField.name_length + 1;
    memcpy(m_KGDField.szName, MySQLField.name, m_uKGDField_NameSize);
    
    // flag, because not use this member, so just set to normal
    m_KGDField.nFlag = KGD_FIELD_NORMAL;

    // set max size
    switch (m_KGDField.Type) 
    {
    case KGD_TYPE_CHAR:
    case KGD_TYPE_VARCHAR:
    //case KGD_TYPE_BINARY:
    //case KGD_TYPE_VARBINARY:
        if (MySQLField.length > KGD_TINY_FIELD_MAX_DATA_SIZE)
            m_KGDField.uMaxDataSize = KGD_TINY_FIELD_MAX_DATA_SIZE;
        else
            m_KGDField.uMaxDataSize = MySQLField.length;
        break;

    case KGD_TYPE_TEXT:
    case KGD_TYPE_BLOB:
        if (MySQLField.length > KGD_MEDIUM_FIELD_MAX_DATA_SIZE)
            m_KGDField.uMaxDataSize = KGD_MEDIUM_FIELD_MAX_DATA_SIZE;
        else
            m_KGDField.uMaxDataSize = MySQLField.length;
        break;

    case KGD_TYPE_DATETIME:
        m_KGDField.uMaxDataSize = MySQLField.length + 1; // plus '\0'
        break;

    default:
        m_KGDField.uMaxDataSize = MySQLField.length;
    };
    // MySQL connect
    m_pMySQL = pMySQL;
//EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::StoreDataToBuffer(
    char *pszWritePos, const KGD_FIELD &crField
) const
{
    ASSERT(pszWritePos);

    switch (m_KGDField.Type) 
    {
    case KGD_TYPE_INT32:
        return _StoreInt32(pszWritePos, crField);
    case KGD_TYPE_UNSIGNED_INT32:
        return _StoreUnsignedInt32(pszWritePos, crField);
    case KGD_TYPE_INT64:
        return _StoreInt64(pszWritePos, crField);
    case KGD_TYPE_UNSIGNED_INT64:
        return _StoreUnsignedInt64(pszWritePos, crField);
    case KGD_TYPE_CHAR:
    case KGD_TYPE_VARCHAR:
    case KGD_TYPE_TEXT:
    case KGD_TYPE_DATETIME:
        return _StoreString(pszWritePos, crField);
    //case KGD_TYPE_BINARY:
    //case KGD_TYPE_VARBINARY:
    case KGD_TYPE_BLOB:
        return _StoreBinary(pszWritePos, crField);
    default:
        ASSERT(false);
        return -1;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
unsigned KGD_MySQL_Field::GetStoreBufferSize(const KGD_FIELD &crField) const
{

    if (crField.uDataSize > m_KGDField.uMaxDataSize) 
        return 0;

    if (
        (!crField.pvData) || 
        (crField.uDataSize == 0)
    ) 
    {
        return m_uKGDField_NameSize + sizeof('=') + KGD_NULL_STRING_SIZE;
    }

    if (
        (m_KGDField.Type == KGD_TYPE_INT32) || 
        (m_KGDField.Type == KGD_TYPE_UNSIGNED_INT32)
    )
    {
        return m_uKGDField_NameSize + KGD_INT32_STRING_MAX_SIZE;
    }

    if (
        (m_KGDField.Type == KGD_TYPE_INT64) || 
        (m_KGDField.Type == KGD_TYPE_UNSIGNED_INT64)
    )
    {
        return m_uKGDField_NameSize + KGD_INT64_STRING_MAX_SIZE;
    }

    return m_uKGDField_NameSize + sizeof('=') + crField.uDataSize * 2 + sizeof("\'\'");
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::_StoreInt32(
    char *pszWritePos, const KGD_FIELD &crField
) const
{
    int nRetCode = false;

    if (!m_pMySQL)
        return -1;

    ASSERT(pszWritePos);
    
    if (
        (!crField.pvData) || 
        (crField.uDataSize == 0)
    ) 
    {
        memcpy(pszWritePos, "NULL", KGD_NULL_STRING_SIZE);
        return KGD_NULL_STRING_SIZE;
    }

    ASSERT(crField.uDataSize == sizeof(int));

    nRetCode = sprintf(pszWritePos, "%d", *((int *)(crField.pvData)));
    ASSERT(nRetCode != -1);

    return nRetCode + 1;    // plus terminal '\0'
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::_StoreUnsignedInt32(
    char *pszWritePos, const KGD_FIELD &crField
) const
{
    int nRetCode = false;

    if (!m_pMySQL)
        return -1;

    ASSERT(pszWritePos);

    if (
        (!crField.pvData) || 
        (crField.uDataSize == 0)
    ) 
    {
        memcpy(pszWritePos, "NULL", KGD_NULL_STRING_SIZE);
        return KGD_NULL_STRING_SIZE;
    }

    ASSERT(crField.uDataSize == sizeof(unsigned));

    nRetCode = sprintf(pszWritePos, "%u", *((unsigned *)(crField.pvData)));
    ASSERT(nRetCode != -1);

    return nRetCode + 1;    // plus terminal '\0'
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::_StoreInt64(
    char *pszWritePos, const KGD_FIELD &crField
) const
{
    int nRetCode = false;

    if (!m_pMySQL)
        return -1;

    ASSERT(pszWritePos);

    if (
        (!crField.pvData) ||
        (crField.uDataSize == 0)
    ) 
    {
        memcpy(pszWritePos, "NULL", KGD_NULL_STRING_SIZE);
        return KGD_NULL_STRING_SIZE;
    }

    ASSERT(crField.uDataSize == sizeof(long long));

    #ifdef WIN32
        nRetCode = sprintf(pszWritePos, "%I64d", *((long long *)(crField.pvData)));
    #else  // Linux
        nRetCode = sprintf(pszWritePos, "%lld", *((long long *)(crField.pvData)));
    #endif
    ASSERT(nRetCode != -1);

    return nRetCode + 1;    // plus terminal '\0'
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::_StoreUnsignedInt64(
    char *pszWritePos, const KGD_FIELD &crField
) const
{
    int nRetCode = false;

    if (!m_pMySQL)
        return -1;

    ASSERT(pszWritePos);

    if (
        (!crField.pvData) || 
        (crField.uDataSize == 0)
    ) 
    {
        memcpy(pszWritePos, "NULL", KGD_NULL_STRING_SIZE);
        return KGD_NULL_STRING_SIZE;
    }

    ASSERT(crField.uDataSize == sizeof(unsigned long long));

    #ifdef WIN32
        nRetCode = sprintf(
            pszWritePos, "%I64u", *((unsigned long long*)(crField.pvData))
        );
    #else // Linux
        nRetCode = sprintf(
            pszWritePos, "%llu", *((unsigned long long*)(crField.pvData))
        );
    #endif
    ASSERT(nRetCode != -1);

    return nRetCode + 1;    // plus terminal '\0'
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::_StoreBinary(
    char *pszWritePos, const KGD_FIELD &crField
) const
{
    if (!m_pMySQL)
        return -1;

    ASSERT(pszWritePos);
    char *pszOrignWritePos = pszWritePos;
    
    if (
        (!crField.pvData) || 
        (crField.uDataSize == 0)
    ) 
    {
        memcpy(pszWritePos, "NULL", KGD_NULL_STRING_SIZE);
        return KGD_NULL_STRING_SIZE;
    }

    *pszWritePos++ = '\'';
    unsigned uRetCode = mysql_real_escape_string(
        m_pMySQL, pszWritePos, (char *)crField.pvData, crField.uDataSize
    );
    pszWritePos += uRetCode;    
    *pszWritePos++ = '\'';
    *pszWritePos   = '\0';

    return (int)(pszWritePos - pszOrignWritePos + 1); // plus '0'
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Field::_StoreString(
    char *pszWritePos, const KGD_FIELD &crField
) const
{
    if (!m_pMySQL)
        return -1;

    ASSERT(pszWritePos);
    char *pszOrignWritePos = pszWritePos;

    if (
        (!crField.pvData) || 
        (crField.uDataSize == 0)
    ) 
    {
        memcpy(pszWritePos, "NULL", KGD_NULL_STRING_SIZE);
        return KGD_NULL_STRING_SIZE;
    }

    *pszWritePos++ = '\'';
    unsigned uRetCode = mysql_real_escape_string(
        m_pMySQL, pszWritePos, (char*)crField.pvData, (crField.uDataSize - 1)
    );
    pszWritePos += uRetCode;    
    *pszWritePos++ = '\'';
    *pszWritePos  = '\0';

    return (int)(pszWritePos - pszOrignWritePos + 1); // plus '0'
}
