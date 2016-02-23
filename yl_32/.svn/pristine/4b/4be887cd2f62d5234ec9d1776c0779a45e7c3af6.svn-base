
#pragma once

#include <vector>
// -------------------------------------------------------------------------
class INameFilter
{
public:
	// -------------------------------------------------------------------------
	// Function name   : InitFromFile
	// Description     : Read all legal-words from file
	// Return type     : BOOL 
	// Argument        : LPCSTR szFile
	// -------------------------------------------------------------------------
	virtual BOOL InitFromFile(LPCSTR szFile) = 0;

	// -------------------------------------------------------------------------
	// Function name   : IsNamePass
	// Description     : Check if name can pass
	// Return type     : BOOL 
	// Argument        : LPCSTR szName
	// -------------------------------------------------------------------------
	virtual BOOL IsNamePass(LPCSTR pszName) = 0;

	virtual void Release() = 0;
};

// -------------------------------------------------------------------------
extern "C" HRESULT CreateNameFilter(INameFilter** ppNameFilter);

