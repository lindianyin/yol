
#ifndef _KBASEOBJECT_H_
#define	_KBASEOBJECT_H_

//#include "Luna.h"
#include "kobjgeneraleventcreator.h"

class KBaseObject : public KObjGeneralEventCreator
{
public:
	KBaseObject() { m_dwId = KD_BAD_ID; }
	virtual ~KBaseObject() { m_dwId = KD_BAD_ID; }

	DWORD GetId() { return m_dwId; }
	DWORD m_dwId;
	//DECLARE_LUA_DWORD(ID);
};


#endif

