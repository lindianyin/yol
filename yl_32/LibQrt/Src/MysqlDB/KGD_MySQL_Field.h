
#pragma once

#include "Misc/IDatabase.h"
#include "mysql.h"

class KGD_MySQL_Field 
{
private:
    KGD_FIELD m_KGDField;
    unsigned  m_uKGDField_NameSize;  // size of field name
    MYSQL    *m_pMySQL;              // a valid, opened MySQL connection 
public:
    KGD_MySQL_Field() : m_uKGDField_NameSize(0), m_pMySQL(NULL)
    {
        memset(&m_KGDField, 0, sizeof(m_KGDField));
    };

    ////////////////////////////////////////////////////////////////////////////
    // @brief : set field information by an MySQL filed information.
    // @param MySQLField : [in] a MySQL field struct
    // @param pMySQL     : [in] must be a valid, opened MySQL connection .
    // @return : true if successful, false if failed.
    // @remark : must call before another member function.
    int SetFieldInfo(const MYSQL_FIELD &MySQLField, MYSQL *pMySQL);

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store field name to buffer as null terminal string.
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @return : if successful, return bytes written by this function.
    //           if failed, return -1.
    // @remark :  must do a success call of SetFieldInfo() before call this.
    int StoreNameToBuffer(char *pszWritePos) const
    {
        memcpy(pszWritePos, m_KGDField.szName, m_uKGDField_NameSize);
        return m_uKGDField_NameSize;
    }
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store data to a buffer as null terminal string,
    //           if need, transfer data first.
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @param crField     : [in] will store specify field's data, 
    //                           should set uDataSize, pvData member
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    // @remark :  must do a success call of SetFieldInfo() before call this.
    int StoreDataToBuffer(char *pszWritePos, const KGD_FIELD &crField) const;

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Get buffer size to store field name and the value.
    // @param crField : [in] field need to store, should set uDataSize member
    // @return : if error return 0, (Value.size is large then enable vale)
    //           if success, return buffer size need.
    // @remark :  must do a success call of SetFieldInfo() before call this.
    unsigned GetStoreBufferSize(const KGD_FIELD &crField) const;

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Cover MySQL field type to KGD field type
    // @param MySQLType  : [in] MySQL field type
    // @param uMySQLFlag : [in] MySQL field flag
    // @param pKGDType   : [out] KGD field type
    // @return : true if successful, false if failed.
    static int MySQLFieldTypeToKGDFieldType(
        enum_field_types MySQLType, unsigned uMySQLFlag, KGD_FIELD_TYPE *pKGDType
    );
private:
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store int to a buffer (trans number to string)
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @param crField     : [in] will store specify field's data, 
    //                           should set uDataSize, pvData member
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    // @remark : because is inner use, caller must insure param pointer is valid.
    int _StoreInt32(char *pszWritePos, const KGD_FIELD &crField) const;

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store unsigned int to a buffer(trans number to string)
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @param crField     : [in] will store specify field's data, 
    //                           should set uDataSize, pvData member
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    int _StoreUnsignedInt32(char *pszWritePos, const KGD_FIELD &crField) const;

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store long long to a buffer( trans number to string)
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @param crField     : [in] will store specify field's data, 
    //                           should set uDataSize, pvData member
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    int _StoreInt64(char *pszWritePos, const KGD_FIELD &crField) const;

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store unsigned long long to a buffer( trans number to string)
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @param crField     : [in] will store specify field's data, 
    //                           should set uDataSize, pvData member
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    int _StoreUnsignedInt64(char *pszWritePos, const KGD_FIELD &crField) const;

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store binary data or string to buffer. NULL terminal.
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @param crField     : [in] will store specify field's data, 
    //                           should set uDataSize, pvData member
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    int _StoreBinary(char *pszWritePos, const KGD_FIELD &crField) const;

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store string to buffer. NULL terminal.
    // @param pszWritePos : [in] Write position, data will be store to here, 
    //                           caller must insure this pointer is valid, and 
    //                           buffer size is enough.
    // @param crField     : [in] will store specify field's data, 
    //                           should set uDataSize, pvData member
    // @return : if successful, return bytes written by this function. 
    //           if failed, return -1.
    int _StoreString(char *pszWritePos, const KGD_FIELD &crField) const;
};

