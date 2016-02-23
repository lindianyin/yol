
#include "stdafx.h"
#include "ktasksetting.h"
#include "config/kconfigfile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif


// -------------------------------------------------------------------------
BOOL KTaskSetting::LoadTaskDef(LPCSTR FileName)
{
	QTabFile cTabFile;
	if (!cTabFile.Load((LPSTR)FileName))
	{
		ASSERT(FALSE);
		printf("[ERROR] File loading failed!!!!!! %s\n", FileName);
		return FALSE;
	}
	INT nRow = cTabFile.GetHeight();
	INT nValue;
	INT i = 3;
	for (; i <= nRow; i++)
	{
		// 读ID
		INT nGroupId, nIdFirst, nIdLast, nOverDate;
		cTabFile.GetInteger(i, "TASK_GROUP", 0, &nGroupId);
		cTabFile.GetInteger(i, "TASK_ID_FIRST", 0, &nIdFirst);
		cTabFile.GetInteger(i, "TASK_ID_LAST", 0, &nIdLast);

		// 读属性
		TASK_PROP prop;
		cTabFile.GetInteger(i, "CLIENT_FLAG", 0, &nValue);
		if (nValue == 1)
			prop.m_Flag |= FLAG_CLIENT;
		cTabFile.GetInteger(i, "SYNC_FLAG", 0, &nValue);
		if (nValue == 1)
			prop.m_Flag |= FLAG_SYNC;
		cTabFile.GetInteger(i, "TASK_OVERDATE", 0, &nOverDate);
		DWORD dwDateSec = 0;
		if(nOverDate > 0)
		{
			int nYear   = (int)(nOverDate / 10000)%(100);
			int nMonth  = (int)((nOverDate % 10000) / 100);
			int nDay	= (int)((nOverDate % 100));
			tm cTm;
			::memset(&cTm, 0, sizeof(cTm));
			cTm.tm_mday = nDay; 
			cTm.tm_mon = (nMonth -1); 
			cTm.tm_year = (nYear + 100);
			dwDateSec  = mktime(&cTm) + 86400; // 24hours more
		}
		prop.m_dwDateSec = dwDateSec;
		prop.m_nDefFlag  = 1;
		//if (prop.IsEmpty())	//没任何属性的不录入
		//	continue;

		// 处理属性
		if (nIdFirst == 0)	// 全组同属性
		{
			if (prop.m_Flag & FLAG_SYNC)
				m_setTaskGroupSync.insert(nGroupId);
			m_mapGroupIDProp[nGroupId] = prop;
		}
		else if (nIdFirst != 0)	// 单独变量、区段的属性
		{
			if (nIdLast == 0)
				nIdLast = nIdFirst;
			nGroupId	<<= 16;
			nIdFirst	|= nGroupId;
			nIdLast		|= nGroupId;

			if (prop.m_Flag & FLAG_SYNC)
				m_setTaskSync.insert(TTaskID<UINT>(nIdFirst, nIdLast));
			for (INT j = nIdFirst; j <= nIdLast; j++)
				m_mapIDProp[j] = prop;
		}
	}
	return TRUE;
}

KTaskSetting			g_PlayerTaskSetting;

// -------------------------------------------------------------------------
