
#pragma once

// -------------------------------------------------------------------------

class KGuidHelper
{
public:
	// GUID×ª»»³É×Ö·û´®
	static LPCSTR GuidToString(const GUID& rGuid)
	{
		static CHAR szGuid[] = "{FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF}";
		ZeroMemory(szGuid, sizeof(szGuid));

		snprintf(szGuid, countof(szGuid), "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			rGuid.Data1, 
			rGuid.Data2, 
			rGuid.Data3, 
			rGuid.Data4[0],
			rGuid.Data4[1],
			rGuid.Data4[2],
			rGuid.Data4[3],
			rGuid.Data4[4],
			rGuid.Data4[5],
			rGuid.Data4[6],
			rGuid.Data4[7]
		);

		return szGuid;
	}

	// ×Ö·û´®×ª»»ÎªGUID
	static BOOL StringToGuid(LPSTR lpszGuid, GUID *pGUID)
	{
		if (!lpszGuid || pGUID)
			return FALSE;

		INT data1, data2, data3;
		INT data4[8];

		if (11 ==  sscanf(lpszGuid, "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
			&data1, &data2, &data3,
			&data4[0], &data4[1], &data4[2], &data4[3], 
			&data4[4], &data4[5], &data4[6], &data4[7])) 
		{
			pGUID->Data1 = data1;
			pGUID->Data2 = data2 & 0xffff;
			pGUID->Data3 = data3 & 0xffff;

			for (INT i = 0 ; i < 8 ; ++i)
				pGUID->Data4[i] = data4[i] & 0xff;

			return TRUE;
		}
		return FALSE;
	}
};
