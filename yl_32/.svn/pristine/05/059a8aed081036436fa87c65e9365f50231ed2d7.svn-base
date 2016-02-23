/* -------------------------------------------------------------------------
//	文件名		：	kobjeventcheck.h
//	创建者		：	liuchang
//	创建时间	：	2009-10-23 14:53:55
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KOBJEVENTCHECK_H__
#define __KOBJEVENTCHECK_H__

// -------------------------------------------------------------------------
// 事件检查类
template<typename TObj>
class TKEventCheck
{
	typedef BOOL(*CheckFun)(TObj&, CONST KOBJEVENT&, CONST KOBJEVENT&);
	typedef std::map<KE_OBJEVENT_TYPE, CheckFun> CHECK_FUN_MAP;

public:
	static BOOL Check(TObj& rcSender, CONST KOBJEVENT& rcEvent, CONST KOBJEVENT& rcTriggerEvent)
	{
		CHECK_FUN_MAP smapProcessFun = _GetCheckFunList();
		CHECK_FUN_MAP::iterator it = smapProcessFun.find(rcEvent.nEvent);
		if (it != smapProcessFun.end())
			return (it->second)(rcSender, rcEvent, rcTriggerEvent);

		return TRUE;
	}
private:
	static BOOL _NONE(TObj& rcSender, CONST KOBJEVENT& rcEvent, CONST KOBJEVENT& rcTriggerEvent);

	static CHECK_FUN_MAP& _GetCheckFunList()
	{
		static CHECK_FUN_MAP smapProcessFun;
		smapProcessFun[emKOBJEVENTTYPE_NONE] = _NONE;
		// TODO:liuchang 不断添加...


		return smapProcessFun;
	}
};

// -------------------------------------------------------------------------
#include "kobjeventcheck.hpp"
#endif /* __KOBJEVENTCHECK_H__ */
