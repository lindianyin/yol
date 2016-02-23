
#pragma once

#include "Misc/IDatabase.h"
#include "mysql.h"
#include <vector>

using namespace std;

// KGD_MySQL_Result manage the memory of returned result set.
class KGD_MySQL_Result : public IKGD_Result
{
public:
    ////////////////////////////////////////////////////////////////////////////
    //                 interface function                                     //
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
            if (m_pResultSet)
            {
                _ReleaseResult();
                m_pResultSet = NULL;
            }
            delete this;
        }
        return ulCount;
    }
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Retrieves the next row of a result set.
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int NextRow();

    ////////////////////////////////////////////////////////////////////////////
    // @brief : Returns the value describe for the specify field of current row
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int GetCurrentRowField(unsigned uIndex, KGD_FIELD *pRetField);

    ////////////////////////////////////////////////////////////////////////////
    // @return : Returns the number of rows in the result set. 
    // @remark : interface function, see more information in KGD_Interface.h
    virtual int GetRowCount();

    ////////////////////////////////////////////////////////////////////////////
    // @return : The count of filed in result set.
    virtual unsigned GetFieldCount() { return m_uFieldCount; };

    ////////////////////////////////////////////////////////////////////////////
    // @return : Last error code. see more at define of KGD_ERROR_CODE
    virtual int GetLastErrorCode() { return m_LastErrorCode; };

public:
    ////////////////////////////////////////////////////////////////////////////
    //                            internal use                                //
    ////////////////////////////////////////////////////////////////////////////
    KGD_MySQL_Result();
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Store result set.
    // @param Flag       : [in] specify how to process result set. 
    //                          see more at define of KGD_RESULT_FLAG
    // @param pMySQL     : [in] Must be a valid, opened MySQL connection.
    // @param piDatabase : [in] Query result is from the specify database.
    // @return : true if successful, false if failed.
    int StoreResult(KGD_RESULT_FLAG Flag, MYSQL *pMySQL, IKGD_Database *piDatabase);

private:
    ~KGD_MySQL_Result();    // make is only can be create by new operator
                             // and only can be destroy by Release()
    ////////////////////////////////////////////////////////////////////////////
    // @brief : Release all resource for store result
    // @return : true if successful, false if failed.
    int _ReleaseResult();

private:
    KGD_ERROR_CODE      m_LastErrorCode;
    MYSQL_RES          *m_pResultSet;       // point to MySQL result set.
    MYSQL_ROW           m_CurRow;           // current row.
   
    unsigned            m_uFieldCount;      // count of field in each row
    KGD_FIELD           m_aFields[KGD_MAX_FIELD_COUNT]; // current row fields
    unsigned long long  m_aNumericFieldData[KGD_MAX_FIELD_COUNT]; // save the numeric data

    unsigned long       m_ulRefCount;
    IKGD_Database      *m_piDatabase;
};
