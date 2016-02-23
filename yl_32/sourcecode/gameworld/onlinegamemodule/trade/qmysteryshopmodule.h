/* -------------------------------------------------------------------------
//	文件名		：	qmysteryshopmodule.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-10-30 17:13:00
//	功能描述	：  
// -------------------------------------------------------------------------*/

#ifndef __QMYSTERYSHOP_MODULE_H__
#define __QMYSTERYSHOP_MODULE_H__

#include "onlinegameworld/kmodulebase.h"

class QMysteryShopModule : public KModuleBase
{
public:
	QMysteryShopModule(); 
	virtual ~QMysteryShopModule() { }

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL ProcessProtocol(LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();

	
protected:
private:
};



#endif