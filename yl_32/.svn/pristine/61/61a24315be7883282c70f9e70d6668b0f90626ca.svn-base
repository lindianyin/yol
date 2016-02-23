
#include "CorePrivate.h"
#include "IniFile2.h"

#ifdef __GNUC__ 
#include <strings.h>
#else
#include <string.h>
#endif

using namespace std;

// Used to identify the arguments including ini data
#define _INI_ARGUMENT_PREFIX "--"

////////////////////////////////////////////////////////////////////////////////
static int _GetIniItems(
    const string &rcsArgument,
    string *psRetSection,
    string *psRetKey,
    string *psRetValue
)
{
    int nResult  = false;
    int nRetCode = false;
    size_t uArgumentLen = 0;
    size_t uSectionPos = 0;
    size_t uDotPos  = 0;
    size_t uEqualSignPos = 0;
    const unsigned PREFIX_LENGTH = sizeof(_INI_ARGUMENT_PREFIX) - 1;

    PROCESS_ERROR(psRetSection);
    PROCESS_ERROR(psRetKey);
    PROCESS_ERROR(psRetValue);

    // Check the prefix
    uArgumentLen = rcsArgument.length();
    PROCESS_ERROR(uArgumentLen > PREFIX_LENGTH);

    nRetCode = (int)rcsArgument.find(_INI_ARGUMENT_PREFIX);
    PROCESS_ERROR(nRetCode == 0);

    uSectionPos = PREFIX_LENGTH;

    // '=' should be in (uSectionPos, uArgumentLen)
    uEqualSignPos = rcsArgument.find('=', uSectionPos);    
    PROCESS_ERROR(uEqualSignPos > uSectionPos);

    // find "." between "--" and "="
    uDotPos  = rcsArgument.find_first_of('.', uSectionPos);
    PROCESS_ERROR(uDotPos > uSectionPos);       // section name can't be empty
    PROCESS_ERROR(uDotPos < uEqualSignPos - 1); // "." should before "=" and the key name could't be empty
   
    // make sure just one "." between "--" and "="
    nRetCode = (int)rcsArgument.find_last_of('.', uEqualSignPos);
    PROCESS_ERROR(nRetCode == uDotPos); 

    // section name : [uSectionPos, unDotPos - 1]
    // key name : [uDotPos + 1, uEqualSignPos - 1]
    // value : [uEqulSignPos + 1, uArgumentLen - 1], could be empty
    *psRetSection = rcsArgument.substr(uSectionPos, uDotPos - uSectionPos);
    *psRetKey     = rcsArgument.substr(uDotPos + 1, uEqualSignPos - uDotPos -1);
    *psRetValue   = rcsArgument.substr(uEqualSignPos + 1, uArgumentLen - uEqualSignPos - 1);

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
// Add new entry into pSectionMap, return false if repeated key name in one section
static int _AddIniEntry(
    _QIni_SectionMap *pSectionMap,
    const string &rcsSection,
    const string &rcsKey,
    const string &rcsValue
)
{
    int nResult  = false;
    int nRetCode = false;
    _QIni_SectionMap::iterator itSection;
    _QIni_KeyMap *pKeyMap = NULL;
    pair<_QIni_SectionMap::iterator, bool> pairInsertSectionResult;
    pair<_QIni_KeyMap::iterator, bool>     pairInsertKeyResult;

    PROCESS_ERROR(pSectionMap);

    nRetCode = (int)rcsSection.length();
    PROCESS_ERROR(nRetCode > 0);
    PROCESS_ERROR(nRetCode < IIniFile::INI_MAX_SUPPORT_SECTION_AND_KEY_SIZE);
    
    nRetCode = (int)rcsKey.length();
    PROCESS_ERROR(nRetCode > 0);
    PROCESS_ERROR(nRetCode < IIniFile::INI_MAX_SUPPORT_SECTION_AND_KEY_SIZE);
    
    // the value string could be empty
    nRetCode = (int)rcsValue.length();
    PROCESS_ERROR(nRetCode < IIniFile::INI_MAX_SUPPORT_VALUE_SIZE);

    pairInsertSectionResult = pSectionMap->insert(
        make_pair(rcsSection, _QIni_KeyMap())
    );
    //PROCESS_ERROR(pairInsertSectionResult.second);
    // if second is false, first will point to the exist value, so first is always valid
    itSection = pairInsertSectionResult.first; 

    pKeyMap = &(itSection->second);
    pairInsertKeyResult = pKeyMap->insert(make_pair(rcsKey, rcsValue));
    PROCESS_ERROR(pairInsertKeyResult.second);  //this key exist

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////
// pSectionMap combine with piIniFile, ignore repeated keys in one section
static int _CombineWithIniFile(
    _QIni_SectionMap *pSectionMap, IIniFile *piIniFile
)
{
    int nResult  = false;
    int nRetCode = false;
    char szPreSection[IIniFile::INI_MAX_SUPPORT_SECTION_AND_KEY_SIZE];
    char szSection[IIniFile::INI_MAX_SUPPORT_SECTION_AND_KEY_SIZE];
    char szPreKey[IIniFile::INI_MAX_SUPPORT_SECTION_AND_KEY_SIZE];
    char szKey[IIniFile::INI_MAX_SUPPORT_SECTION_AND_KEY_SIZE];
    char szValue[IIniFile::INI_MAX_SUPPORT_VALUE_SIZE];
    _QIni_SectionMap::iterator itSection;
    _QIni_KeyMap *pKeyMap = NULL;
    _QIni_KeyMap::iterator itKey;
    pair<_QIni_SectionMap::iterator, bool> pairInsertSectionReslut;
    pair<_QIni_KeyMap::iterator, bool> pairInsertKeyResult;

    PROCESS_ERROR(pSectionMap);
    PROCESS_ERROR(piIniFile);

    szPreSection[0] = '\0';
    while (true)    // Get sections orderly and insert them into pSectionMap
    {
        nRetCode = piIniFile->GetNextSection(szPreSection, szSection);
        if (!nRetCode)
            break;

        pairInsertSectionReslut = pSectionMap->insert(
            make_pair(szSection, _QIni_KeyMap())
        );
        //PROCESS_ERROR(pairInsertSectionResult.second); 
        // if second is false, first will point to the exist value, so first is always valid
        itSection = pairInsertSectionReslut.first;

        pKeyMap = &(itSection->second);
        szPreKey[0] = '\0';
        while (true)    // Insert keys and corresponding values orderly
        {
            nRetCode = piIniFile->GetNextKey(szSection, szPreKey, szKey);
            if (!nRetCode)
                break;

            nRetCode = piIniFile->GetString(
                szSection, szKey, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
            );
            PROCESS_ERROR(nRetCode);

            pairInsertKeyResult = pKeyMap->insert(make_pair(szKey, szValue));
            //PROCESS_ERROR(pairInsertKeyResult.second); // Combination ignores repeated value
           
            strncpy(szPreKey, szKey, sizeof(szPreKey) / sizeof(szPreKey[0]));
            szPreKey[sizeof(szPreKey) / sizeof(szPreKey[0]) - 1] = '\0';
        }

        strncpy(szPreSection, szSection, sizeof(szPreSection) / sizeof(szPreSection[0]));
        szPreSection[sizeof(szPreSection) / sizeof(szPreSection[0]) - 1] = '\0';
    }

    nResult = true;
EXIT0:
    return nResult;
}


////////////////////////////////////////////////////////////////////////////////
int QIniFile2::Init(const _QIni_SectionMap &rcSectionMap, void *pvContext)
{
    USE_ARGUMENT(pvContext);

    m_SectionMap = rcSectionMap;

    return true;
}

int QIniFile2::UnInit(void *pvContext)
{
    USE_ARGUMENT(pvContext);

    Clear();

    return true;
}

int QIniFile2::GetNextSection(
    const char *pSection, char *pNextSection
)
{    
    int nResult  = false;
    int nRetCode = false;    
    _QIni_SectionMap::iterator itSection;
    string sNextSection;

    PROCESS_ERROR(pSection);
    PROCESS_ERROR(pNextSection);

    if (!pSection[0])
    {
        itSection = m_SectionMap.begin();
    }
    else
    {
        itSection = m_SectionMap.find(pSection);
        PROCESS_ERROR(itSection != m_SectionMap.end());
        ++itSection;
    }

    PROCESS_ERROR(itSection != m_SectionMap.end());
    sNextSection = itSection->first;    
    strcpy(pNextSection, sNextSection.c_str());

    nResult = true;
EXIT0:
    return nResult;
}

int QIniFile2::GetNextKey(
    const char *pSection,
    const char *pKey,
    char *pNextKey
)
{
    int nResult  = false;
    int nRetCode = false;    
    _QIni_SectionMap::iterator itSection;
    _QIni_KeyMap *pKeyMap;
    _QIni_KeyMap::iterator itKey;
    string sNextKey;

    PROCESS_ERROR(pSection);
    PROCESS_ERROR(pKey);
    PROCESS_ERROR(pNextKey);

    itSection = m_SectionMap.find(pSection);
    PROCESS_ERROR(itSection != m_SectionMap.end());

    pKeyMap = &itSection->second;

    if (!pKey[0])
    {
        itKey = pKeyMap->begin();
    }
    else
    {
        itKey = pKeyMap->find(pKey);
        PROCESS_ERROR(itKey != pKeyMap->end());
        itKey++;
    }

    PROCESS_ERROR(itKey != pKeyMap->end());
    sNextKey = itKey->first;
    strcpy(pNextKey, sNextKey.c_str());

    nResult = true;
EXIT0:
    return nResult;
}

int QIniFile2::GetSectionCount()
{
    return (int)m_SectionMap.size();
}

int QIniFile2::IsSectionExist(const char *lpSection)
{
    int nResult  = false;
    _QIni_SectionMap::iterator itSectionMap;

    PROCESS_ERROR(lpSection);

    itSectionMap = m_SectionMap.find(lpSection);
    PROCESS_ERROR(itSectionMap != m_SectionMap.end());

    nResult = true;
EXIT0:
    return nResult;    
}

int QIniFile2::GetString(
    const char *lpSection,
    const char *lpKeyName,
    const char *lpDefault,
    char *lpRString,    
    unsigned int dwSize
)
{
    int nResult  = false;
    int nRetCode = false;    
    _QIni_SectionMap::iterator itSection;
    _QIni_KeyMap *pKeyMap;
    _QIni_KeyMap::iterator itKey;   
    string sValue;

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    //PROCESS_ERROR(lpDefault);
    PROCESS_ERROR(lpRString); 
    PROCESS_ERROR(dwSize > 0);

    itSection = m_SectionMap.find(lpSection);
    PROCESS_ERROR(itSection != m_SectionMap.end());

    pKeyMap = &itSection->second;
    itKey = pKeyMap->find(lpKeyName);
    PROCESS_ERROR(itKey != pKeyMap->end());

    sValue = itKey->second;   
    strncpy(lpRString, sValue.c_str(), dwSize);
    lpRString[dwSize - 1] = '\0';

    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (lpRString && lpDefault && (dwSize > 0))
        {
            strncpy(lpRString, lpDefault, dwSize);
            lpRString[dwSize - 1] = '\0';
        }
    }
    return nResult;    
}

int QIniFile2::GetInteger(
    const char *lpSection,
    const char *lpKeyName,
    int nDefault,    
    int *pnValue        
)
{
    int nResult  = false;
    int nRetCode = false;    
    char szValue[INI_MAX_SUPPORT_VALUE_SIZE];

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pnValue);

    nRetCode = GetString(
        lpSection, lpKeyName, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
    );
    PROCESS_ERROR(nRetCode);

    *pnValue = atoi(szValue);    
    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (pnValue)
        {
            *pnValue = nDefault;
        }
    }
    return nResult;
}

int QIniFile2::GetInteger2(
    const char *lpSection,
    const char *lpKeyName,
    int *pnValue1,    
    int *pnValue2    
)
{
    int nResult  = false;
    int nGetValueCount = 0;
    int nGetValueArray[INI_MAX_SUPPORT_MULTI_VALUE_COUNT];    

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pnValue1);
    PROCESS_ERROR(pnValue2);   

    nGetValueCount = GetMultiInteger(
        lpSection,
        lpKeyName,
        nGetValueArray,
        sizeof(nGetValueArray) / sizeof(nGetValueArray[0])
    );
    PROCESS_ERROR(nGetValueCount == 2);

    *pnValue1 = nGetValueArray[0];
    *pnValue2 = nGetValueArray[1];
    
    nResult = true;    
EXIT0:
    return nResult;
}

int QIniFile2::GetMultiInteger(
    const char *lpSection,
    const char *lpKeyName,
    int *pValues,    
    int nCount        
)
{
    int nGetCount = 0;
    int nRetCode  = false;    
    char szValue[INI_MAX_SUPPORT_VALUE_SIZE];
    char *pchBuffer = NULL;
    char *pchValue  = NULL;

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pValues);
    PROCESS_ERROR(nCount > 0);
    PROCESS_ERROR(nCount <= INI_MAX_SUPPORT_MULTI_VALUE_COUNT);

    nRetCode = GetString(
        lpSection, lpKeyName, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
    );
    PROCESS_ERROR(nRetCode);

    // Values are partitioned by ',' in szValue, refer strtok() function
    nGetCount = 0;
    pchBuffer = szValue;
    pchValue = strtok(pchBuffer, ",");
    while (pchValue && (nGetCount < nCount))
    {
        pValues[nGetCount++] = atoi(pchValue);
        pchValue = strtok(NULL, ",");
    }

EXIT0:
    return nGetCount;
}

int QIniFile2::GetMultiLong(
    const char *lpSection,
    const char *lpKeyName,
    long *pValues,    
    int  nCount        
)
{
    int nGetCount = 0;
    int nRetCode  = false;    
    char szValue[INI_MAX_SUPPORT_VALUE_SIZE];
    char *pchBuffer = NULL;
    char *pchValue  = NULL;

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pValues);
    PROCESS_ERROR(nCount > 0);
    PROCESS_ERROR(nCount <= INI_MAX_SUPPORT_MULTI_VALUE_COUNT);

    nRetCode = GetString(
        lpSection, lpKeyName, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
    );
    PROCESS_ERROR(nRetCode);

    // Values are partitioned by ',' in szValue, refer strtok() function
    nGetCount = 0;
    pchBuffer = szValue;
    pchValue = strtok(pchBuffer, ",");
    while (pchValue && (nGetCount < nCount))
    {
        pValues[nGetCount++] = atol(pchValue);
        pchValue = strtok(NULL, ",");
    }

EXIT0:
    return nGetCount;
}

int QIniFile2::GetFloat(
    const char *lpSection,
    const char *lpKeyName,
    float  fDefault,    
    float *pfValue    
)
{
    int nResult  = false;
    int nRetCode = false;    
    char szValue[INI_MAX_SUPPORT_VALUE_SIZE];

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pfValue);

    nRetCode = GetString(
        lpSection, lpKeyName, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
    );
    PROCESS_ERROR(nRetCode);

    *pfValue = (float)atof(szValue);
    
    nResult = true;
EXIT0:
    if (!nResult)
    {
         if(pfValue)
         {
             *pfValue = fDefault;
         }
    }
    return nResult;
}

int QIniFile2::GetFloat2(
    const char *lpSection,
    const char *lpKeyName,
    float *pfValue1,
    float *pfValue2    
)
{
    int nResult  = false;
    int nGetValueCount = 0;
    float fGetValueArray[INI_MAX_SUPPORT_MULTI_VALUE_COUNT];

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pfValue1);
    PROCESS_ERROR(pfValue2);

    nGetValueCount = GetMultiFloat(
        lpSection,
        lpKeyName,
        fGetValueArray,
        sizeof(fGetValueArray) / sizeof(fGetValueArray[0])
    );
    PROCESS_ERROR(nGetValueCount == 2);

    *pfValue1 = fGetValueArray[0];
    *pfValue2 = fGetValueArray[1];
    
    nResult = true;
EXIT0:
    return nResult;
}

int QIniFile2::GetMultiFloat(
    const char *lpSection,
    const char *lpKeyName,
    float *pValues,    
    int   nCount
)
{
    int nGetCount = 0;
    int nRetCode  = false;    
    char szValue[INI_MAX_SUPPORT_VALUE_SIZE];
    char *pchBuffer = NULL;
    char *pchValue  = NULL;

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pValues);
    PROCESS_ERROR(nCount > 0);
    PROCESS_ERROR(nCount <= INI_MAX_SUPPORT_MULTI_VALUE_COUNT);

    nRetCode = GetString(
        lpSection, lpKeyName, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
    );
    PROCESS_ERROR(nRetCode);

    // Values are partitioned by ',' in szValue, refer strtok() function
    nGetCount = 0;
    pchBuffer = szValue;
    pchValue = strtok(pchBuffer, ",");
    while (pchValue && (nGetCount < nCount))
    {
        pValues[nGetCount++] = (float)atof(pchValue);
        pchValue = strtok(NULL, ",");
    }

EXIT0:
    return nGetCount;
}

int QIniFile2::GetStruct(
    const char  *lpSection,
    const char  *lpKeyName,
    void        *lpStruct,
    unsigned int dwSize
)
{
    int nResult  = false;
    int nRetCode = false;
    char szValue[INI_MAX_SUPPORT_VALUE_SIZE];
    size_t uValueLen = 0;
    unsigned char *pbyWritePos = NULL;
    unsigned char byHigh = 0;
    unsigned char byLow = 0;
    unsigned char byValue = 0;

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(lpStruct);

    nRetCode = GetString(
        lpSection, lpKeyName, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
    );
    PROCESS_ERROR(nRetCode);

    uValueLen = strlen(szValue);
    PROCESS_ERROR(uValueLen == (dwSize * 2));

    pbyWritePos = (unsigned char *)lpStruct;
    for (unsigned i = 0; i < uValueLen; i += 2)
    {
        // Get byte high
        byHigh = szValue[i];
        if ((byHigh >= '0') && (byHigh <= '9'))
        {
            byValue = (unsigned char)(byHigh - '0') << 4;
        }
        else if ((byHigh >= 'A') && (byHigh <= 'F'))
        {
            byValue = (unsigned char)(byHigh - 'A' + 10) << 4;
        }
        else
        {
            PROCESS_ERROR(false);
        }

        // Get byte low
        byLow = szValue[i + 1];
        if ((byLow >= '0') && (byLow <= '9'))
        {
            byValue += (unsigned char)(byLow - '0');
        }
        else if ((byLow >= 'A') && (byLow <= 'F'))
        {
            byValue += (unsigned char)(byLow - 'A' + 10);
        }
        else
        {
            PROCESS_ERROR(false);
        }
        
        // Set struct byte
        *pbyWritePos = byValue;
        ++pbyWritePos;
    }
    
    nResult = true;
EXIT0:
    return nResult;
}

int QIniFile2::GetBool(
    const char *lpSection,
    const char *lpKeyName,
    int *pBool
)
{
    int nResult  = false;
    int nRetCode = false;    
    int nGetSuccessFlag = false;
    char szValue[INI_MAX_SUPPORT_VALUE_SIZE];
    const char*	cszValueArray[] =
    {
        "true",	"false",
        "1",	"0",
        "yes",	"no"
    };

    PROCESS_ERROR(lpSection);
    PROCESS_ERROR(lpKeyName);
    PROCESS_ERROR(pBool);

    nRetCode = GetString(
        lpSection, lpKeyName, NULL, szValue, sizeof(szValue) / sizeof(szValue[0])
    );
    PROCESS_ERROR(nRetCode);

    for (int i = 0; i < sizeof(cszValueArray) / sizeof(cszValueArray[0]); i++)
    {
#ifdef __GNUC__

        if (strcasecmp(szValue, cszValueArray[i]) == 0)
#else
        if (_stricmp(szValue, cszValueArray[i]) == 0)
#endif
        {
            *pBool = ((i % 2) == 0);
            nGetSuccessFlag = true;
            break;
        }
    }    
    PROCESS_ERROR(nGetSuccessFlag);

    nResult = true;
EXIT0:
    return nResult;
}


////////////////////////////////////////////////////////////////////////////////
IIniFile *g_OpenIniFileFromArguments(int argc, char *argv[])
{
    int nResult   = false;
    int nRetCode  = false;    
    int nIniFileInitFlag = false;
    QIniFile2 *pIniFile = NULL;    
    _QIni_SectionMap sectionMap;
    string sArgument;
    string sSection;
    string sKey;
    string sValue;

    PROCESS_ERROR(argc >= 0);
    PROCESS_ERROR(argv);

    sectionMap.clear();
    for (int i = 0; i < argc; i++)
    {
        PROCESS_ERROR(argv[i]);
        sArgument = argv[i];

        nRetCode = _GetIniItems(sArgument, &sSection, &sKey, &sValue);
        if (!nRetCode)  // Not be the ini argument
        {
            continue;
        }

        nRetCode = _AddIniEntry(&sectionMap, sSection, sKey, sValue);
        PROCESS_ERROR(nRetCode);
    }

    pIniFile = new QIniFile2();
    PROCESS_ERROR(pIniFile);
    
    nRetCode = pIniFile->Init(sectionMap, NULL);
    PROCESS_ERROR(nRetCode); 
    nIniFileInitFlag = true;

    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (nIniFileInitFlag)
        {
            pIniFile->UnInit(NULL);
            nIniFileInitFlag = false;
        }
        SAFE_DELETE(pIniFile);
    }
    return pIniFile;
}

////////////////////////////////////////////////////////////////////////////////
IIniFile *g_OpenCombinatorialIniFile(
    IIniFile *piHighPriority, IIniFile *piLowPriority
)
{
    int nResult   = false;
    int nRetCode  = false;
    int nIniFileInitFlag = false;
    QIniFile2 *pIniFile = NULL;
    _QIni_SectionMap sectionMap;

    PROCESS_ERROR(piHighPriority);
    PROCESS_ERROR(piLowPriority);

    pIniFile = new QIniFile2();
    PROCESS_ERROR(pIniFile);

    sectionMap.clear();

    nRetCode = _CombineWithIniFile(&sectionMap, piHighPriority);
    PROCESS_ERROR(nRetCode);

    nRetCode = _CombineWithIniFile(&sectionMap, piLowPriority);
    PROCESS_ERROR(nRetCode);

    nRetCode = pIniFile->Init(sectionMap, NULL);
    PROCESS_ERROR(nRetCode);
    nIniFileInitFlag = true;

    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (nIniFileInitFlag)
        {
            pIniFile->UnInit(NULL);
            nIniFileInitFlag = false;
        }
        SAFE_DELETE(pIniFile);
    }
    return pIniFile;
}

////////////////////////////////////////////////////////////////////////////////
IIniFile *g_LoadStartArgument(int argc, char *argv[], const char *FileName)
{       
    int nResult   = false;
    IIniFile *piCmdArguments = NULL;
    IIniFile *piIniFile = NULL;
    IIniFile *piStartArgument = NULL;

    PROCESS_ERROR(argc > 0);
    PROCESS_ERROR(argv);
    PROCESS_ERROR(FileName);    
    
    piCmdArguments = g_OpenIniFileFromArguments(argc, argv);
    PROCESS_ERROR(piCmdArguments);

    piIniFile = g_OpenIniFile(FileName);
    PROCESS_ERROR(piIniFile);    

    piStartArgument = g_OpenCombinatorialIniFile(piCmdArguments, piIniFile);
    PROCESS_ERROR(piStartArgument);    

    nResult = true;
EXIT0:
    if (!nResult)
    {
        SAFE_RELEASE(piStartArgument);
    }
    SAFE_RELEASE(piIniFile);
    SAFE_RELEASE(piCmdArguments);

    return piStartArgument;
}
