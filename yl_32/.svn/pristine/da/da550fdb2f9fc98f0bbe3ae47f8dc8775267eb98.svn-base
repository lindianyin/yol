
#pragma once

#if defined(__GNUC__) || defined(unix) || defined(STATIC_QRT)     // linux/gcc
	#define		CORE_API
	#define		CORE_API_C extern "C"
#else               //WIN32
	#ifdef CORE_EXPORTS
		#define CORE_API __declspec(dllexport)
		#define CORE_API_C extern "C" __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
		#define CORE_API_C extern "C" __declspec(dllimport)
	#endif
#endif


