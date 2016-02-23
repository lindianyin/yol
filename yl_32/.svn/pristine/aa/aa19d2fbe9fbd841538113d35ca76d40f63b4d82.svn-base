
#pragma once

#include "Misc/IDatabase.h"

#ifdef WIN32
#include <mbstring.h>
#else
#include <strings.h>
#endif

class KGD_Name
{
public:
    KGD_Name(const char cszName[KGD_NAME_BUFFER_MAX_SIZE])
    {
        ASSERT(cszName);
        if (cszName)
        {
            strncpy(m_szName, cszName, KGD_NAME_BUFFER_MAX_SIZE - 1);
            m_szName[KGD_NAME_BUFFER_MAX_SIZE - 1] = '\0';
        }
        else
        {
            m_szName[0] = '\0';
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    bool operator <(const KGD_Name& right) const
    {   
        #ifdef WIN32
        return (
            _mbsicmp(
                (unsigned char *)m_szName, 
                (unsigned char *)right.m_szName
            ) < 0
        );
        #else
        return (strcasecmp(m_szName, right.m_szName) < 0);
        #endif
    }

    char m_szName[KGD_NAME_BUFFER_MAX_SIZE];
};
