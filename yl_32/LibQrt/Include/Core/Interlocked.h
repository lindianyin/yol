
#pragma once

//#include "Core/CorePrivate.h"

#ifndef WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////
// @berief : performs an atomic addition of a increment value to a addend variable. The function 
//           prevents more than one thread from using the same variable simultaneously.
// @param pValue    : [in][out] Pointer to the variable to be incremented
//        lAddValue : [in]      Value to be added to the variable pointed to by the pValue parameter
// @return : The function returns the initial value of the plValue parameter.
// @remark :The variable pointed to by the parameter must be aligned on a 32-bit boundary; 
//          otherwise, this function will fail on multiprocessor x86 systems and any non-x86 systems.
// Last modify  2008-6-15 11:23:00
inline long InterlockedExchangeAdd(long *plValue, long lAddValue)
{
	return __sync_fetch_and_add(plValue, lAddValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @berief : increments (increases by one) the value of the specified 32-bit variable, The function 
//           prevents more than one thread from using the same variable simultaneously
// @param pValue : [in][out] Pointer to the variable to be incremented
// @return : the resulting incremented value.
// @remark :The variable pointed to by the parameter must be aligned on a 32-bit boundary; 
//          otherwise, this function will fail on multiprocessor x86 systems and any non-x86 systems.
// Last modify by Wu Caizhong, 2005-02-03 21:54:34
inline long InterlockedIncrement(long *plValue)
{
	return __sync_add_and_fetch(plValue, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @berief : decrements (decreases by one) the value of the specified 32-bit variable, The function 
//           prevents more than one thread from using the same variable simultaneously
// @param pValue : [in][out] Pointer to the variable to be decremented
// @return : the resulting decremented value.
// @remark :The variable pointed to by the parameter must be aligned on a 32-bit boundary; 
//          otherwise, this function will fail on multiprocessor x86 systems and any non-x86 systems.
// Last modify by Wu Caizhong, 2005-02-03 21:54:29
inline long InterlockedDecrement(long *plValue)
{
	return __sync_add_and_fetch(plValue, -1);
}
#endif // WIN32

