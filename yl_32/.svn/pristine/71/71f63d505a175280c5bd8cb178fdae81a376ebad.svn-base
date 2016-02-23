
#pragma once

#include <set>
#include <map>
#include <vector>

#include "KRole.h"
#include "config/kplayerbasedbdef.h"

typedef std::vector<KRole*> KACC_ROLE_TAB;
struct KAccount
{
    char          szName[_NAME_LEN];
    KACC_ROLE_TAB vRoleList;
};

class KRoleManager
{
public:
    KRoleManager();

    BOOL Init();
    void UnInit();

    void Activate();

    KAccount* GetAccount(const char cszAccount[]);

    KRole* AddRole(
        const char cszAccountName[], const char cszRoleName[], 
        DWORD dwID, BYTE byLevel, BYTE byType, /*KCAMP eCamp, */
        BYTE byForceID, INT nFightScore, time_t nLastSaveTime, time_t nCreateTime
    );

    KRole* GetRole(DWORD dwID);
	BOOL   GetRoleIds(std::vector<DWORD>& playerIds);
	//获得在线玩家的ID列表
	BOOL   GetOnlineRoleIds(std::vector<DWORD>& playerIds);
    KRole* GetRole(const char cszName[]);

	KRole* GetRoleForAccount(const char cszAccount[]);

    KRole* GetOnlineRoleForAccount(const char cszAccount[]);

    void   OnGSCrash(int nConnIndex);
    
    BOOL   IsRoleBelongToAccount(DWORD dwRoleID, const char cszAccountName[]);

    int    GetRoleCountTotal() { return (int)m_RoleTable.size(); }
    int    GetAccountCountTotal() { return (int)m_AccountNameIndex.size(); }

    BOOL   FreezeRole(DWORD dwRoleID, time_t nFreezeEndTime);
    time_t GetRoleFreezeTime(DWORD dwRoleID);

    
    void   RegisterDelRole(DWORD dwID, time_t nEndTime);
    void   UnregisterDelRole(DWORD dwID);
    time_t GetRoleDeleteTime(DWORD dwID);

    void ProcessDeleteList();
    BOOL DelRole(DWORD dwID);

    void CalculateCampActivePlayerCount();

    BOOL RoleRename(DWORD dwID, char szNewName[]);
    void RecordRenameRole(DWORD dwRoleID);
    BOOL IsCanRename(DWORD dwRoleID);
    void RemoveRenameRole(DWORD dwRoleID);

	BOOL OnSaveRoleData(DWORD dwId, const KRoleBaseInfo& pBaseInfo);
	typedef std::map<DWORD, KRole, std::less<DWORD> >  KROLE_TABLE;
	KROLE_TABLE& GetAllRole(){return m_RoleTable;}
public:
    template <class TFunc>
    void Traverse(TFunc& Func)
    {
    	//m_lock.Lock();
        BOOL bContinue = false;
        KROLE_TABLE::iterator it = m_RoleTable.begin();
        while (it != m_RoleTable.end())
        {
            KROLE_TABLE::iterator Current = it++;

            bContinue = Func(Current->first, Current->second);
            if (!bContinue)
                break;
        }
		//m_lock.Unlock();
    }

private:
	struct KNameLess
	{
		bool operator()(const char* pszX, const char* pszY) const
		{
			return QStrCaseCmp(pszX, pszY) < 0;
		}
	};

	KROLE_TABLE         m_RoleTable;

	typedef std::map<const char*, KRole*, KNameLess> KROLE_NAME_INDEX;
	KROLE_NAME_INDEX    m_RoleNameIndex;
    
    typedef std::map<const char*, KAccount*, KNameLess> KACCOUNT_NAME_INDEX;
    KACCOUNT_NAME_INDEX m_AccountNameIndex;

    typedef std::map<DWORD, time_t, std::less<DWORD> > KROLE_BLACK_LIST;
    KROLE_BLACK_LIST    m_RoleBlackList;

    typedef std::map<DWORD, time_t, std::less<DWORD> > KROLE_DELETE_LIST;
    KROLE_DELETE_LIST  m_RoleDeleteList;

    typedef std::set<DWORD, std::less<DWORD> >KRENAME_ROLE_TABLE;
    KRENAME_ROLE_TABLE m_RenameRoleTable;

public:
	QLock m_lock;
};


